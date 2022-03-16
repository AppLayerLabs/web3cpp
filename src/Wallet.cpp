#include <web3cpp/Wallet.h>

bool Wallet::createNewWallet(std::string &password) {
  // Create the paths if they don't exist yet
  if (!boost::filesystem::exists(walletFile().parent_path())) {
    boost::filesystem::create_directories(walletFile().parent_path());
  }
  if (!boost::filesystem::exists(secretsFolder())) {
    boost::filesystem::create_directories(secretsFolder());
  }

  // Initialize a new Wallet
  dev::eth::KeyManager w(walletFile(), secretsFolder());
  try {
    w.create(password);
    // Create the first account using BIP39, and store the seedphrase.
    auto seedPhrase = BIP39::createNewMnemonic();

    // Initialize the seed json and cipher, then create the salt.
    // Use exactly as many bytes for the cipher as needed.
    json seedJson;
    Cipher cipher("aes-256-cbc", "sha256");
    unsigned char saltBytes[CIPHER_SALT_BYTES] = {0};
    RAND_bytes(saltBytes, CIPHER_SALT_BYTES);
    
    std::string salt = dev::toHex(
      dev::sha3(std::string((char*)saltBytes, sizeof(saltBytes)), false)
    ).substr(0, CIPHER_SALT_BYTES);

    std::string encryptedPhrase;

    encryptedPhrase = cipher.encrypt(seedPhrase.raw, password, salt);
    // Replace newlines with space, when saving to JSON newlines will break it
    boost::replace_all(encryptedPhrase, "\n", " ");
    seedJson["seed"] = encryptedPhrase;
    seedJson["salt"] = salt;

    boost::filesystem::path tmpPath = seedPhraseFile();
    Utils::writeJSONFile(seedJson, tmpPath);

    // Create default account.
    bip3x::HDKey rootKey = BIP39::createKey(seedPhrase.raw, "m/44'/60'/0'/0");
    dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
    this->keyManager.import(k.secret(), "default", password, "");

    return true;
  } catch (dev::Exception const& _e) {
    throw std::string("Failed to create a new wallet: ") + _e.what();
  }
}

bool Wallet::loadWallet(std::string &password) {
  if (!boost::filesystem::exists(walletFile())) { createNewWallet(password); }
  dev::eth::KeyManager tmpManager(walletFile(), secretsFolder());
  if (tmpManager.load(password)) {
    this->keyManager = std::move(tmpManager);
    this->passSalt = dev::h256::random();
    this->passHash = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
    this->loadAccounts();
    return true;
  } else {
    return false;
  }
}

bool Wallet::walletExists(boost::filesystem::path &wallet_path) {
  return (
    boost::filesystem::exists(wallet_path.string() + "/wallet/wallet.info") &&
    boost::filesystem::exists(wallet_path.string() + "/wallet/secrets")
  );
}

void Wallet::loadAccounts() {
  auto allAccountsFromDB = accountsDB.getAllPairs();
  for (auto accountInfoStr : allAccountsFromDB) {
    json accountJson = json::parse(accountInfoStr.second);
    std::string tmpAddress = accountJson["address"].get<std::string>();
    std::string tmpDerivationPath = accountJson["derivationPath"].get<std::string>();
    bool tmpIsLedger = accountJson["isLedger"].get<bool>();
    Account newTmpAccount(
      boost::filesystem::path(path->string() + "/wallet"),
      accountJson["address"].get<std::string>(),
      accountJson["derivationPath"].get<std::string>(),
      accountJson["isLedger"].get<bool>()
    );
    accounts.emplace_back(std::move(newTmpAccount));
  }

  // Sanity check if all accounts found on DB match with accounts inside KeyManager.
  if (this->keyManager.store().keys().empty()) { return; }
  dev::AddressHash got;
  std::vector<dev::h128> keys = this->keyManager.store().keys();
  for (auto const& u : keys) {
    if (dev::Address a = this->keyManager.address(u)) {
      bool found = false;
      got.insert(a);
      std::string address = "0x" + boost::lexical_cast<std::string>(a);
      for (auto &c : address) {
        std::tolower(c);
      }
      for (uint64_t i = 0; i < accounts.size(); ++i) {
        std::string accAddr = accounts[i].address();
        for (auto &c : accAddr) {
          std::tolower(c);
        }
        if (accAddr == address) {
          found = true;
        }
      }
      if (!found) {
        throw std::string(
          "Error loading accounts! Mismatch accounts from keyManager and local database"
        );
      }
    }
  }
}

