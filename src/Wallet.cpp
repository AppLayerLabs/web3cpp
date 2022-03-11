#include <web3cpp/Wallet.h>

bool Wallet::createNewWallet(std::string &password) {
  // Create the paths if they don't exist yet
  if (!boost::filesystem::exists(walletFile().parent_path())) { boost::filesystem::create_directories(walletFile().parent_path()); }
  if (!boost::filesystem::exists(secretsFolder())) { boost::filesystem::create_directories(secretsFolder()); }

  // Initialize a new Wallet
  dev::eth::KeyManager w(walletFile(), secretsFolder());
  try {
    w.create(password);
    return true;
  } catch (dev::Exception const& _e) {
    throw std::string("Failed to create a new wallet: ") + _e.what();
  }
}

bool Wallet::loadWallet(std::string &password) {
  // Wallet does not exist, create a new one.
  if (!boost::filesystem::exists(walletFile()))
    createNewWallet(password);

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
  // We basically check if the wallet file and it's keys folder exists,
  if (boost::filesystem::exists(wallet_path.string() + "/wallet/wallet.info") && boost::filesystem::exists(wallet_path.string() + "/wallet/secrets")) {
    return true;
  }
  return false;
}

void Wallet::loadAccounts() {
  
  auto allAccountsFromDB = accountsDB.getAllPairs();
  for (auto accountInfoStr : allAccountsFromDB) {
    json accountJson = json::parse(accountInfoStr.second);

    std::string tmpAddress = accountJson["address"].get<std::string>();    
    std::string tmpDerivationPath = accountJson["derivationPath"].get<std::string>();    
    bool tmpIsLedger = accountJson["isLedger"].get<bool>();

    Account newTmpAccount(boost::filesystem::path(path->string() + "/wallet"), 
                          accountJson["address"].get<std::string>(),
                          accountJson["derivationPath"].get<std::string>(),
                          accountJson["isLedger"].get<bool>());
    accounts.emplace_back(std::move(newTmpAccount));
  }

  // Account sanity check! Check if all accounts found on the DB matches with accounts inside the KeyManager.
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
        throw "Error loading accounts! Mismatch accounts from keyManager and local database";
      }
    }
  }
}
