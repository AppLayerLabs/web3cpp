#ifndef WEB3_H
#define WEB3_H

#include <string>

#include <nlohmann/json.hpp>

#include <web3cpp/Eth.h>
#include <web3cpp/Provider.h>
#include <web3cpp/Utils.h>
#include <web3cpp/Wallet.h>

#include "version.h"

/**
 * Main/Umbrella class for %Web3.
 * Entry point for accessing the wallet and other classes.
 */

class Web3 {
  private:
    boost::filesystem::path defaultPath;  ///< Path for the %Web3 wallet.
    Provider defaultProvider;             ///< Provider used for the whole %Web3 library.

  public:
    /**
     * Constructor.
     * @param *provider (optional) The provider that will be used. Defaults to
     *                  whatever the Provider constructor sets as the default.
     * @param *path (optional) The folder where the wallet is and will be
     *              opened/created. Defaults to Utils::getDefaultDataDir().
     */
    Web3(Provider *provider = NULL, boost::filesystem::path *path = NULL);

    /// Constructor overload that uses a custom Provider.
    Web3(Provider provider) : Web3(&provider, NULL) {}

    /// Constructor overload that uses a custom wallet path.
    Web3(boost::filesystem::path path) : Web3(NULL, &path) {}

    std::string version;  ///< Current version of the library.
    Eth eth;              ///< Object for accessing functions from the Eth class.
    Wallet wallet;        ///< Object for accessing the wallet.

    /**
     * Getter for the library provider.
     * @returns A pointer to Web3::defaultProvider.
     */
    Provider* getProvider() { return &this->defaultProvider; }

    /**
     * Setter for the library provider.
     * @param p The provider to use.
     */
    void setProvider(Provider p) { this->defaultProvider = p; }
};

#endif  // WEB3_H
