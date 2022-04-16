#include <web3cpp/Web3.h>

Web3::Web3(Utils::Provider *provider, boost::filesystem::path *path) : wallet(&defaultProvider, (path == NULL) ? Utils::getDefaultDataDir() : *path) {
  boost::nowide::nowide_filesystem();
  defaultProvider.ProviderLock.lock();
  //std::cout << provider << std::endl;
  if (provider == NULL) {
    defaultProvider = Utils::Provider();
  } else {
    defaultProvider = *provider;
  }
  defaultProvider.ProviderLock.unlock();
  defaultPath = (path == NULL) ? Utils::getDefaultDataDir() : *path;
}

