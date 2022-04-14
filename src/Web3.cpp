#include <web3cpp/Web3.h>

Web3::Web3(Utils::Provider *provider, boost::filesystem::path *path) : wallet(&defaultProvider, &defaultPath) {
  boost::nowide::nowide_filesystem();
  defaultProvider.ProviderLock.lock();
  if (provider == NULL) {
    defaultProvider = Utils::Provider();
  } {
    defaultProvider = *provider;
  }
  defaultProvider.ProviderLock.unlock();
  defaultPath = (path == NULL) ? Utils::getDefaultDataDir() : *path;
}

