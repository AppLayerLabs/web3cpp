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
  if (name == "walletInfo") {
    error.setErrorCode(2); // Forbidden Account Name.
    return false;
  }

  if (walletDB.getKeyValue(name) != "") {
    error.setErrorCode(3); // Account Name Exists.
    return false;
  }

  // Check if password is correct
  json walletInfo = json::parse(walletDB.getKeyValue("walletInfo"));
  auto originalKey = dev::fromHex(walletInfo["key"]);
  dev::SecureFixedHash<16> oldKey(originalKey);
  auto originalSalt = dev::fromHex(walletInfo["salt"]);
  auto originalIterations = walletInfo["iterations"];
  auto key = dev::SecureFixedHash<16>(dev::pbkdf2(password, originalSalt, originalIterations, 16));
  if (key != oldKey) {
    error.setErrorCode(1); // Incorrect Password.
    return false;
  }
  json encryptedKey = json::parse(dev::SecretStore::encrypt(secret.ref(), password));
  encryptedKey["address"] = "0x" + dev::toHex(dev::toAddress(secret));
  encryptedKey["derivationPath"] = derivationPath;
  encryptedKey["isLedger"] = false;
  walletDB.putKeyValue(name, encryptedKey.dump());
  return true;
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
    error.setErrorCode(1); // Incorrect Password.
    return false;
  }
}

bool Wallet::walletExists(boost::filesystem::path &wallet_path) {
  return (
    boost::filesystem::exists(wallet_path.string() + "/wallet") &&
    boost::filesystem::exists(wallet_path.string() + "/wallet/secrets")
  );
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

bool Wallet::createNewAccount(
  std::string derivPath, std::string &password, Error &error, std::string seed
) {
  if(!checkPassword(password)) {
    error.setErrorCode(1); // Incorrect Password
    return false;
  }
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

    // Derive the account.
    bip3x::HDKey rootKey = BIP39::createKey(seedPhrase, derivPath);
    dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));

    this->importPrivKey(k.secret(), password, "default", derivPath, error);
    this->loadAccounts();
  } catch (const std::exception &e) {
    throw std::string("Error when creating new account: ") + e.what();
  }
  return true;
}

bool Wallet::checkPassword(std::string &password) {
  auto inputPassword = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
  return (inputPassword.ref().toString() == passHash.ref().toString());
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

std::future<std::vector<std::string>> Wallet::getAccounts() {
  return std::async([=]{
    std::vector<std::string> ret;
    for (int i = 0; i < accounts.size(); i++) {
      ret.push_back(accounts[i].address());
    }
    return ret;
  });
}
