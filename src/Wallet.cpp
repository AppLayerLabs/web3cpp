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
//    this->loadAccounts();
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