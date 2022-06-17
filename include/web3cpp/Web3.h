#ifndef WEB3_H
#define WEB3_H

#include <string>

#include <nlohmann/json.hpp>

#include <web3cpp/Eth.h>
#include <web3cpp/Provider.h>
#include <web3cpp/Utils.h>
#include <web3cpp/Wallet.h>

#include "version.h"

// Main/Umbrella class that houses all modules.

class Web3 {
  private:
    boost::filesystem::path defaultPath;
    Provider defaultProvider;

  public:
    // Constructor and overloads.
    Web3(Provider *provider = NULL, boost::filesystem::path *path = NULL);
    Web3(Provider provider) : Web3(&provider, NULL) {}
    Web3(boost::filesystem::path path) : Web3(NULL, &path) {}

    // Current version of the library.
    std::string version;

    // The proper objects for other classes.
    Eth eth;
    Wallet wallet;

    // Getter/Setter for library-wide provider.
    // ALL sub modules that use providers should point to this one.
    Provider* getProvider() { return &this->defaultProvider; }
    void setProvider(Provider p) { this->defaultProvider = p; }
};

#endif  // WEB3_H
