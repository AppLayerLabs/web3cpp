#include <web3cpp/Web3.h>

Web3::Web3(Provider *provider, boost::filesystem::path *path) :
  wallet(&defaultProvider, (path == NULL) ? Utils::getDefaultDataDir() : *path),
  eth(&defaultProvider)
{
  boost::nowide::nowide_filesystem();
  defaultProvider.lock.lock();
  defaultProvider = (provider == NULL) ? Provider() : *provider;
  defaultProvider.lock.unlock();
  defaultPath = (path == NULL) ? Utils::getDefaultDataDir() : *path;
  this->version = PROJECT_VERSION;
}

