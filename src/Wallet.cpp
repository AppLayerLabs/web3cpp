#include <web3cpp/Wallet.h>

bool Wallet::createNewWallet(std::string const &password, Error &error) {
  // Create the paths if they don't exist yet
  if (!boost::filesystem::exists(walletExistsPath())) {
    boost::filesystem::create_directories(walletExistsPath());
  }
  if (!boost::filesystem::exists(walletFolder())) {
    boost::filesystem::create_directories(walletFolder());
  }
  if (!boost::filesystem::exists(accountsFolder())) {
    boost::filesystem::create_directories(accountsFolder());
  }

  // Initialize a new Wallet
  auto salt = dev::h256::random().asBytes();
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, salt, 262144, 16));
  json walletInfo = {
    {"salt", dev::toHex(salt)},
    {"key", key.makeInsecure().hex()},
    {"iterations", 262144}
  };
  walletDB.putKeyValue("walletInfo", walletInfo.dump());

  // Create a new account and insert into database
  try {
    // Create the first account using BIP39, and store the seedphrase.
    auto seedPhrase = BIP39::createNewMnemonic();

    // Initialize the seed json and cipher, then create the salt.
    // Use exactly as many bytes for the cipher as needed.
    json seedJson;
    seedJson = json::parse(cipher::encrypt(seedPhrase.raw, password, error));
    boost::filesystem::path tmpPath = seedPhraseFile();
    Utils::writeJSONFile(seedJson, tmpPath);

    // Create default account.
    bip3x::HDKey rootKey = BIP39::createKey(seedPhrase.raw, "m/44'/60'/0'/0");
    dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
    this->importPrivKey(k.secret(), password, "default", "m/44'/60'/0'/0",error);
    return true;
  } catch (dev::Exception const& _e) {
    throw std::string("Failed to create a new wallet: ") + _e.what();
  }
}

bool Wallet::importPrivKey(
  dev::Secret const &secret, std::string const &password,
  std::string const &name, std::string const &derivationPath, Error &error
) {
  // Check if wallet already exists inside the database, do not allow specific names
  if (name == "walletInfo") { error.setCode(2); return false; } // Forbidden Account Name
  if (walletDB.getKeyValue(name) != "") { error.setCode(3); return false; } // Account Name Exists

  // Check if password is correct
  json walletInfo = json::parse(walletDB.getKeyValue("walletInfo"));
  auto originalKey = dev::fromHex(walletInfo["key"]);
  dev::SecureFixedHash<16> oldKey(originalKey);
  auto originalSalt = dev::fromHex(walletInfo["salt"]);
  auto originalIterations = walletInfo["iterations"];
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, originalSalt, originalIterations, 16));
  if (key != oldKey) { error.setCode(1); return false; } // Incorrect Password.

  // Create and import the key
  json encryptedKey = json::parse(dev::SecretStore::encrypt(secret.ref(), password));
  encryptedKey["address"] = "0x" + dev::toHex(dev::toAddress(secret));
  encryptedKey["derivationPath"] = derivationPath;
  encryptedKey["isLedger"] = false;
  walletDB.putKeyValue(name, encryptedKey.dump());
  return true;
}

void Wallet::loadAccounts() {
  accounts.clear();
  auto allAccountsFromDB = walletDB.getAllPairs();
  for (auto accountInfoStr : allAccountsFromDB) {
    json accountJson = json::parse(accountInfoStr.second);
    std::string tmpAddress = accountJson["address"].get<std::string>();
    std::string tmpDerivationPath = accountJson["derivationPath"].get<std::string>();
    bool tmpIsLedger = accountJson["isLedger"].get<bool>();
    //std::cout << accountJson.dump(2) << std::endl;  // Uncomment to see account details as JSON
    Account newTmpAccount(
      boost::filesystem::path(path.string() + "/wallet"),
      accountJson["address"].get<std::string>(),
      accountInfoStr.first,
      accountJson["derivationPath"].get<std::string>(),
      accountJson["isLedger"].get<bool>(),
      provider
    );
    accounts.emplace_back(std::move(newTmpAccount));
  }
}

bool Wallet::loadWallet(std::string const &password, Error &error) {
  if (!boost::filesystem::exists(walletExistsPath())) { createNewWallet(password, error); }
  // Check if password is correct
  json walletInfo = json::parse(walletDB.getKeyValue("walletInfo"));
  auto originalKey = dev::fromHex(walletInfo["key"]);
  dev::SecureFixedHash<16> oldKey(originalKey);
  auto originalSalt = dev::fromHex(walletInfo["salt"]);
  auto originalIterations = walletInfo["iterations"];
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, originalSalt, originalIterations, 16));
  if (key == oldKey) {
    this->passSalt = dev::h256::random();
    this->passHash = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
    this->loadAccounts();
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

bool Wallet::createNewAccount(
  std::string derivPath, std::string &password, Error &error, std::string seed
) {
  if (!checkPassword(password)) { error.setCode(1); return false; } // Incorrect Password
  try {
    std::string seedPhrase;
    if (seed.empty()) {
      // Load the phrase from JSON
      boost::filesystem::path tmpPath = seedPhraseFile();
      json seedJson = Utils::readJSONFile(tmpPath);
      auto decrypt = dev::SecretStore::decrypt(seedJson.dump(), password);
      seedPhrase = decrypt.ref().toString();
    } else {
      // Use the custom phrase
      seedPhrase = seed;
    }

    // Derive and import the account.
    bip3x::HDKey rootKey = BIP39::createKey(seedPhrase, derivPath);
    dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
    this->importPrivKey(k.secret(), password, "default", derivPath, error);
    this->loadAccounts();
  } catch (const std::exception &e) {
    throw std::string("Error when creating new account: ") + e.what();
  }
  return true;
}

dev::eth::TransactionSkeleton Wallet::buildTransaction(
  std::string from, std::string to,
  BigNumber value, BigNumber gasLimit, BigNumber gasPrice,
  std::string dataHex, int nonce, bool creation
) {
  dev::eth::TransactionSkeleton ret;
  try {
    ret.creation = creation;
    ret.from = dev::eth::toAddress(from);
    ret.to = dev::eth::toAddress(to);
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
  std::string dataToSign, std::string address, std::string password, Error &err
) {
  // EIP-712 requires us to hash the message before signing
  Secret s(dev::toHex(cipher::decrypt(address, password, err)));
  std::string signedData = std::string("\x19") + "Ethereum Signed Message:\n"
    + boost::lexical_cast<std::string>(dataToSign.size()) + dataToSign;
  dev::h256 messageHash(dev::toHex(dev::sha3(signedData, false)));
  dev::h520 signature = dev::sign(s, messageHash);
  dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&signature;
  sigStruct.v = sigStruct.v + 27;
  return std::string("0x") + dev::toHex(dev::Signature(sigStruct));
}

std::future<std::string> Wallet::ecRecover(
  std::string dataThatWasSigned, std::string signature
) {
  return {}; // TODO
}

std::string Wallet::signTransaction(
  dev::eth::TransactionSkeleton txObj, std::string password, Error &err
) {
  Secret s(dev::toHex(cipher::decrypt(dev::toString(txObj.from), password, err)));
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

void Wallet::unlockAccount(
  std::string address, std::string password, unsigned int unlockDuration
) {
  ; // TODO
}

std::future<bool> Wallet::lockAccount(std::string address) {
  return {}; // TODO
}

std::future<std::vector<std::string>> Wallet::getAccounts() {
  return std::async([=]{
    std::vector<std::string> ret;
    for (int i = 0; i < accounts.size(); i++) {
      ret.push_back(accounts[i].address());
    }
    return ret;
  });
}

std::future<std::string> Wallet::importRawKey(
  std::string privateKey, std::string password
) {
  return {}; // TODO
}
