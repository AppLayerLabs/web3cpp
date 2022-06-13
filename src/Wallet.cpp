#include <web3cpp/Wallet.h>

bool Wallet::createNewWallet(std::string const &password, Error &error) {
  // Create the paths if they don't exist yet
  std::vector<boost::filesystem::path> paths {
    walletExistsPath(), walletFolder(), accountsFolder(), transactionsFolder()
  };
  for (boost::filesystem::path path : paths) {
    if (!boost::filesystem::exists(path)) {
      boost::filesystem::create_directories(path);
    }
  }

  // Initialize a new Wallet
  auto salt = dev::h256::random().asBytes();
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, salt, 262144, 16));
  json walletInfo = {
    {"salt", dev::toHex(salt)},
    {"key", key.makeInsecure().hex()},
    {"iterations", 262144}
  };
  if (!infoDB.putKeyValue("walletInfo", walletInfo.dump())){
    error.setCode(2); // Database Insert Failed
    std::cout << "Failed to create wallet: " << error.what() << std::endl;
    return false;
  }

  // Initialize the seed json and cipher, then create the salt.
  // Use exactly as many bytes for the cipher as needed.
  auto seedPhrase = BIP39::createNewMnemonic();
  json seedJson;
  seedJson = json::parse(Cipher::encrypt(seedPhrase.raw, password, error));
  boost::filesystem::path tmpPath = seedPhraseFile();
  Utils::writeJSONFile(seedJson, tmpPath);

  // Create the first (default) account.
  bip3x::HDKey rootKey = BIP39::createKey(seedPhrase.raw, "m/44'/60'/0'/0");
  dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
  if (!this->importPrivKey(k.secret(), password, "default", "m/44'/60'/0'/0", error)) {
    std::cout << "WARNING: Wallet created but failed to create a default Account: "
      << error.what() << std::endl;
  }
  return true;
}

bool Wallet::loadWallet(std::string const &password, Error &error) {
  if (!boost::filesystem::exists(walletExistsPath())) { createNewWallet(password, error); }
  // Check if password is correct
  json walletInfo = json::parse(infoDB.getKeyValue("walletInfo"));
  auto originalKey = dev::fromHex(walletInfo["key"]);
  dev::SecureFixedHash<16> oldKey(originalKey);
  auto originalSalt = dev::fromHex(walletInfo["salt"]);
  auto originalIterations = walletInfo["iterations"];
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, originalSalt, originalIterations, 16));
  if (key == oldKey) {
    this->passSalt = dev::h256::random();
    this->passHash = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
    _isLoaded = true;
    return true;
  } else {
    error.setCode(1); // Incorrect Password.
    return false;
  }
}

bool Wallet::checkPassword(std::string &password) {
  auto inputPassword = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
  return (inputPassword.ref().toString() == passHash.ref().toString());
}

bool Wallet::walletExists(boost::filesystem::path &wallet_path) {
  return (
    boost::filesystem::exists(wallet_path.string() + "/wallet") &&
    boost::filesystem::exists(wallet_path.string() + "/wallet/secrets")
  );
}

std::string Wallet::createAccount(
  std::string derivPath, std::string &password, std::string name,
  Error &error, std::string seed
) {
  if (!checkPassword(password)) { error.setCode(1); return ""; } // Incorrect Password
  std::string seedPhrase;
  if (seed.empty()) {
    // Load the seed phrase from JSON.
    boost::filesystem::path tmpPath = seedPhraseFile();
    json seedJson = Utils::readJSONFile(tmpPath);
    auto decrypt = dev::SecretStore::decrypt(seedJson.dump(), password);
    seedPhrase = decrypt.ref().toString();
  } else {
    // Use the provided custom seed phrase.
    seedPhrase = seed;
  }

  // Derive and import the account.
  bip3x::HDKey rootKey = BIP39::createKey(seedPhrase, derivPath);
  dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
  Error importErr;
  if (!this->importPrivKey(k.secret(), password, name, derivPath, importErr)) {
    std::cout << "Error when importing account: " << importErr.what() << std::endl;
    error.setCode(importErr.getCode());
    return "";
  }
  std::string ret;
  std::map<std::string, std::string> accs = this->accountDB.getAllPairs();
  for (std::pair<std::string, std::string> acc : accs) {
    json accJson = json::parse(acc.second);
    if (accJson["name"] == name) { ret = acc.first; break; }
  }
  error.setCode(0); // No Error
  return ret;
}

bool Wallet::importPrivKey(
  dev::Secret const &secret, std::string const &password,
  std::string const &name, std::string const &derivationPath, Error &error
) {
  // Check if password is correct
  json walletInfo = json::parse(infoDB.getKeyValue("walletInfo"));
  auto originalKey = dev::fromHex(walletInfo["key"]);
  dev::SecureFixedHash<16> oldKey(originalKey);
  auto originalSalt = dev::fromHex(walletInfo["salt"]);
  auto originalIterations = walletInfo["iterations"];
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, originalSalt, originalIterations, 16));
  if (key != oldKey) { error.setCode(1); return false; } // Incorrect Password.

  // Create the encrypted key
  json encryptedKey = json::parse((secret.ref(), password));
  encryptedKey["address"] = Utils::toLowercaseAddress(
    "0x" + dev::toHex(dev::toAddress(secret))
  );
  encryptedKey["name"] = name;
  encryptedKey["derivationPath"] = derivationPath;
  encryptedKey["isLedger"] = false;

  // Import the account
  std::vector<std::string> ret;
  std::map<std::string, std::string> accs = this->accountDB.getAllPairs();
  for (std::pair<std::string, std::string> acc : accs) {
    if (acc.first == encryptedKey["address"]) {
      error.setCode(3);  // Account Exists
      return false;
    }
  }
  if (!accountDB.putKeyValue(encryptedKey["address"], encryptedKey.dump())) {
    error.setCode(2); // Database Insert Failed
    return false;
  }
  error.setCode(0); // No Error
  return true;
}

bool Wallet::deleteAccount(std::string address) {
  return this->accountDB.deleteKeyValue(Utils::toLowercaseAddress(address));
}

dev::eth::TransactionSkeleton Wallet::buildTransaction(
  std::string from, std::string to,
  BigNumber value, BigNumber gasLimit, BigNumber gasPrice,
  std::string dataHex, int nonce, bool creation
) {
  dev::eth::TransactionSkeleton ret;
  try {
    ret.creation = creation;
    ret.from = dev::eth::toAddress(Utils::toLowercaseAddress(from));
    ret.to = dev::eth::toAddress(Utils::toLowercaseAddress(to));
    ret.value = value;
    if(!dataHex.empty()) { ret.data = dev::fromHex(dataHex); }
    ret.nonce = nonce;
    ret.gas = gasLimit;
    ret.gasPrice = gasPrice;
    ret.chainId = this->provider->chainID;
  } catch (const std::exception &e) {
    throw std::string("buildTransaction error: ") + e.what();
  }
  return ret;
}

std::string Wallet::sign(
  std::string dataToSign, std::string address, std::string password
) {
  // EIP-712 requires us to hash the message before signing
  address = Utils::toLowercaseAddress(address);
  json accRaw = getAccountRawDetails(address);
  Secret s(dev::SecretStore::decrypt(accRaw.dump(), password));
  std::string signableData = std::string("\x19") + "Ethereum Signed Message:\n"
    + boost::lexical_cast<std::string>(dataToSign.size()) + dataToSign;
  dev::h256 messageHash(dev::toHex(dev::sha3(signableData, false)));
  dev::h520 signature = dev::sign(s, messageHash);
  return std::string("0x") + dev::toHex(signature);
}

std::string Wallet::ecRecover(
  std::string dataThatWasSigned, std::string signature
) {
  // Split signature into r/s/v
  if (signature.substr(0, 2) == "0x" || signature.substr(0, 2) == "0X") {
    signature = signature.substr(2); // Remove "0x"
  }
  // TODO: check why r and s are unused
  //std::string r = signature.substr(0, 64);  // 32 hex bytes (64 chars - 0-64)
  //std::string s = signature.substr(65, 64); // 32 hex bytes (64 chars - 65-128)
  std::string v = signature.substr(128, 2); // 1 hex byte (2 chars - 129-130)

  // Hash the data that was signed
  std::string data = std::string("\x19") + "Ethereum Signed Message:\n"
    + boost::lexical_cast<std::string>(dataThatWasSigned.size())
    + dataThatWasSigned;
  dev::h256 messageHash(dev::toHex(dev::sha3(data, false)));

  // Set up the context, recoverable signature and raw public key for secp256k1
  secp256k1_context* ctx = secp256k1_context_create(
    SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY
  );
  secp256k1_ecdsa_recoverable_signature rawSig;
  secp256k1_pubkey rawPubkey;

  // Parse the signature compact, then recover the raw public key
  if (!secp256k1_ecdsa_recoverable_signature_parse_compact(
    ctx, &rawSig, (unsigned char*) signature.c_str(), std::stoi(v)
  )) {
    std::cout << "Cannot parse compact" << std::endl;
    return "";
  }
  if (!secp256k1_ecdsa_recover(ctx, &rawPubkey, &rawSig, messageHash.data())) {  // 32 bit hash
    std::cout << "Cannot recover public key" << std::endl;
    return "";
  }

  // Serialize the public key
  std::array<uint8_t, 65> pubkey;
  size_t pubsize = pubkey.size();
  secp256k1_ec_pubkey_serialize(
    ctx, pubkey.data(), &pubsize, &rawPubkey, SECP256K1_EC_UNCOMPRESSED
  );
  dev::Public p{&pubkey[1], Public::ConstructFromPointer};
  return "0x" + dev::toHex(dev::toAddress(p));
}

std::string Wallet::signTransaction(
  dev::eth::TransactionSkeleton txObj, std::string password, Error &err
) {
  Secret s(dev::toHex(Cipher::decrypt(dev::toString(txObj.from), password, err)));
  std::stringstream txHexBuffer;
  try {
    dev::eth::TransactionBase t(txObj);
    t.setNonce(txObj.nonce);
    t.sign(s);
    txHexBuffer << dev::toHex(t.rlp());
  } catch (...) {
    err.setCode(11);  // Transaction Sign Error
  }
  return txHexBuffer.str();
}

std::future<json> Wallet::sendTransaction(
  std::string txHash, std::string password, Error &err
) {
  return std::async([=]{
    json txResult;
    // TODO: use web3cpp methods to send and save tx in history, set error code 12 on fail
    /*
    json txResult = json::parse(API::broadcastTx(txidHex));
    if (txResult.contains("result")) {
      // Store the successful transaction in the Account's history.
      // Since the AVAX chain is pretty fast, we can ask if the transaction was
      // already confirmed even immediately after sending it.
      TxData txData = Utils::decodeRawTransaction(txidHex);
      txData.operation = operation;
      saveTxToHistory(txData);
    }
    */
    return txResult;
  });
}

void Wallet::storePassword(std::string password, unsigned int seconds) {
  this->_password = password;
  if (seconds > 0) {
    this->_passEnd = std::time(nullptr) + seconds;
    this->_passThread = std::thread(std::bind(&Wallet::passHandler, this));
    this->_passThread.detach();
  }
}

void Wallet::clearPassword() {
  this->_password = "";
  this->_passEnd = 0; // This ensures the thread will be terminated
}

bool Wallet::isPasswordStored() {
  return !this->_password.empty();
}

std::vector<std::string> Wallet::getAccounts() {
  std::vector<std::string> ret;
  std::map<std::string, std::string> accs = this->accountDB.getAllPairs();
  for (std::pair<std::string, std::string> acc : accs) {
    ret.push_back(acc.first);
  }
  return ret;
}

Account Wallet::getAccountDetails(std::string address) {
  address = Utils::toLowercaseAddress(address);
  std::map<std::string, std::string> accs = this->accountDB.getAllPairs();
  for (std::pair<std::string, std::string> acc : accs) {
    if (acc.first == address) {
      json accJson = json::parse(acc.second);
      return Account(
        boost::filesystem::path(path.string() + "/wallet"),
        accJson["address"].get<std::string>(),
        acc.first,
        accJson["derivationPath"].get<std::string>(),
        accJson["isLedger"].get<bool>(),
        this->provider
      );
    }
  }
  return Account();
}

json Wallet::getAccountRawDetails(std::string address) {
  json ret;
  address = Utils::toLowercaseAddress(address);
  std::map<std::string, std::string> accs = this->accountDB.getAllPairs();
  for (std::pair<std::string, std::string> acc : accs) {
    if (acc.first == address) {
      ret = json::parse(acc.second);
      break;
    }
  }
  return ret;
}

