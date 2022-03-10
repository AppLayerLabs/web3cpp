#include <web3cpp/Wallet.h>


bool Wallet::createNewWallet(std::string password) {
  // Create the paths if they don't exist yet
  if (!boost::filesystem::exists(walletFile().parent_path())) { boost::filesystem::create_directories(walletFile().parent_path()); }
  if (!boost::filesystem::exists(secretsFolder())) { boost::filesystem::create_directories(secretsFolder()); }

  // Initialize a new Wallet
  dev::eth::KeyManager w(walletFile(), secretsFolder());
  try {
    w.create(password);
    return true;
  } catch (dev::Exception const& _e) {
    return false;
  }
}

bool Wallet::loadWallet(std::string password) {

  // Wallet does not exist, create a new one.
  if (!boost::filesystem::exists(walletFile()))
    createNewWallet(password);


  dev::eth::KeyManager tmpManager(walletFile(), secretsFolder());
  
  if (tmpManager.load(password)) {
    this->keyManager = std::move(tmpManager);
    this->passSalt = dev::h256::random();
    this->passHash = dev::pbkdf2(password, this->passSalt.asBytes(), this->passIterations);
    return true;
  } else {
    return false;
  }
}