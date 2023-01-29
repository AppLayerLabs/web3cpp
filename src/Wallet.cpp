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
  Error encErr, writeErr;
  seedJson = json::parse(Cipher::encrypt(seedPhrase.raw, password, encErr));
  if (encErr.getCode() != 0) {
    error.setCode(encErr.getCode());
    return false;
  }
  boost::filesystem::path tmpPath = seedPhraseFile();
  Utils::writeJSONFile(seedJson, tmpPath, writeErr);
  if (writeErr.getCode() != 0) {
    error.setCode(writeErr.getCode());
    return false;
  }

  // Create the first (default) account.
  bip3x::HDKey rootKey = BIP39::createKey(seedPhrase.raw, "m/44'/60'/0'/0");
  dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
  if (!this->importPrivKey(k.secret(), password, "default", "m/44'/60'/0'/0", error)) {
    std::cout << "WARNING: Wallet created but failed to create a default Account: "
      << error.what() << std::endl;
  }
  return true;
}

bool Wallet::loadWallet(const std::string& password, Error &error) {
  // Create the wallet if it doesn't exist already
  if (!boost::filesystem::exists(walletExistsPath())) {
    Error createErr;
    if (!createNewWallet(password, createErr)) {
      error.setCode(createErr.getCode());
      return false;
    }
  }

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
    error.setCode(0);
    return true;
  } else {
    error.setCode(1); // Incorrect Password.
    return false;
  }
}

bool Wallet::checkPassword(const std::string& password) {
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
  std::string derivPath, const std::string &password, std::string name,
  Error &error, std::string seed
) {
  if (!checkPassword(password)) { error.setCode(1); return ""; } // Incorrect Password
  std::string seedPhrase;
  if (seed.empty()) {
    // Load the seed phrase from JSON.
    Error readErr, decErr;
    boost::filesystem::path tmpPath = seedPhraseFile();
    json seedJson = Utils::readJSONFile(tmpPath, readErr);
    if (readErr.getCode() != 0) {
      error.setCode(readErr.getCode());
      return "";
    }
    seedPhrase = Cipher::decrypt(seedJson.dump(), password, decErr);
    if (decErr.getCode() != 0) { error.setCode(decErr.getCode()); return ""; }
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
  std::string const &name, std::string const &derivPath, Error &error
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
  Error encErr;
  json encryptedKey = json::parse(Cipher::encrypt(secret.ref().toString(), password, encErr));
  if (encErr.getCode() != 0) {
    error.setCode(encErr.getCode());
    return false;
  }
  encryptedKey["address"] = Utils::toLowercaseAddress(
    "0x" + dev::toHex(dev::toAddress(secret))
  );
  encryptedKey["name"] = name;
  encryptedKey["derivationPath"] = derivPath;
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

std::string Wallet::sign(
  std::string dataToSign, std::string address, std::string password
) {
  // EIP-712 requires us to hash the message before signing
  address = Utils::toLowercaseAddress(address);
  json accRaw = getAccountRawDetails(address);
  Error decErr;
  std::string dec = Cipher::decrypt(accRaw.dump(), password, decErr);
  if (decErr.getCode() != 0) return decErr.what();
  Secret s(dev::toHex(dec));
  std::string signableData = std::string("\x19") + "Ethereum Signed Message:\n"
    + boost::lexical_cast<std::string>(dataToSign.size()) + dataToSign;
  dev::h256 messageHash(dev::toHex(dev::sha3(signableData, false)));
  dev::h520 signature = dev::sign(s, messageHash);
  return std::string("0x") + dev::toHex(signature);
}

std::string Wallet::ecRecover(
  std::string dataThatWasSigned, std::string signature
) {
  // Aleth does some stupidly arcane magic to make this work, better use it for now
  if (signature.substr(0, 2) == "0x" || signature.substr(0, 2) == "0X") {
    signature = signature.substr(2); // Remove "0x"
  }
  std::string data = std::string("\x19") + "Ethereum Signed Message:\n"
    + boost::lexical_cast<std::string>(dataThatWasSigned.size())
    + dataThatWasSigned;
  dev::h256 messageHash(dev::toHex(dev::sha3(data, false)));
  dev::h520 sig = dev::FixedHash<65>(signature);
  dev::Public p = dev::recover(sig, messageHash);
  return "0x" + dev::toHex(dev::toAddress(p));
}

dev::eth::TransactionSkeleton Wallet::buildTransaction(
  std::string from, std::string to,
  BigNumber value, BigNumber gasLimit, BigNumber gasPrice,
  std::string dataHex, int nonce, Error &error, bool creation
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
    ret.chainId = this->provider->getChainId();
  } catch (std::exception &e) {
    error.setCode(11);  // Transaction Build Error
    return ret;
  }
  error.setCode(0);
  return ret;
}


std::string Wallet::signTransaction(
  dev::eth::TransactionSkeleton txObj, std::string password, Error &err
) {
  Error e;
  Secret s(dev::toHex(Cipher::decrypt(
    getAccountRawDetails("0x" + dev::toString(txObj.from)).dump(), password, e
  )));
  if (e.getCode() != 0) { err.setCode(e.getCode()); return ""; }
  try {
    std::stringstream txHexBuffer;
    dev::eth::TransactionBase t(txObj);
    t.setNonce(txObj.nonce);
    t.sign(s);
    txHexBuffer << dev::toHex(t.rlp());
    err.setCode(0);
    return txHexBuffer.str();
  } catch (std::exception &e) {
    err.setCode(12); return ""; // Transaction Sign Error
  }
}

std::future<json> Wallet::sendTransaction(std::string signedTx, Error &err) {
  if (signedTx.substr(0,2) != "0x" || signedTx.substr(0,2) != "0X") {
    signedTx.insert(0, "0x");
  }
  return std::async([this, signedTx, &err]{
    json txResult;
    Error rpcErr;
    std::string rpcStr = RPC::eth_sendRawTransaction(signedTx, rpcErr).dump();
    if (rpcErr.getCode() != 0) {
      err.setCode(rpcErr.getCode());
      return txResult;
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    txResult["signature"] = signedTx;
    if (reqJson.count("error")) {
      txResult["error"] = reqJson["error"].get<std::string>();
      err.setCode(13);  // Transaction Send Error
    } else {
      txResult["result"] = reqJson["result"].get<std::string>();
      err.setCode(0);
    }
    return txResult;
  });
}

void Wallet::storePassword(const std::string& password, unsigned int seconds) {
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

std::string Wallet::getSeedPhrase(const std::string& password, Error &err) {
  std::string seedPhrase;
  Error readErr, decErr;
  boost::filesystem::path tmpPath = seedPhraseFile();
  json seedJson = Utils::readJSONFile(tmpPath, readErr);
  if (readErr.getCode() != 0) {
    err.setCode(readErr.getCode());
    return "";
  }
  seedPhrase = Cipher::decrypt(seedJson.dump(), password, decErr);
  if (decErr.getCode() != 0) { err.setCode(decErr.getCode()); return ""; }
  return seedPhrase;
}