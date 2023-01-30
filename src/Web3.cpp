#include <web3cpp/Web3.h>

// Default Constructor 
Web3::Web3() :
  defaultProvider(std::make_unique<Provider>(Provider(""))),
  defaultPath(Utils::getDefaultDataDir()),  
  wallet(defaultProvider, defaultPath),
  eth(defaultProvider) {}

// Custom provider overload
Web3::Web3(Provider provider) :
  defaultProvider(std::make_unique<Provider>(provider)),
  defaultPath(Utils::getDefaultDataDir()),  
  wallet(defaultProvider, defaultPath),
  eth(defaultProvider) {}

// Custom path overload
Web3::Web3(boost::filesystem::path path) :
  defaultProvider(std::make_unique<Provider>(Provider(""))), 
  defaultPath(path),  
  wallet(defaultProvider, defaultPath),
  eth(defaultProvider) {}

// Custom path & provider overload
Web3::Web3(Provider provider, boost::filesystem::path path) :
  defaultProvider(std::make_unique<Provider>(provider)),
  defaultPath(path),  
  wallet(defaultProvider, defaultPath),
  eth(defaultProvider) {}