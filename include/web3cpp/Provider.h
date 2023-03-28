#ifndef PROVIDER_H
#define PROVIDER_H

#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

// Forward declaration
class Web3;

/**
 * Abstraction for a single provider.
 */

class Provider {
  private:
    const std::string _id;    ///< The ID of the provider.
    std::string name;         ///< The name of the provider.
    std::string host;         ///< The host of the provider.
    std::string target;       ///< The RPC target of the provider.
    uint64_t port;            ///< The port of the provider.
    uint64_t chainId;         ///< The chain ID of the provider.
    std::string currency;     ///< The currency the provider uses.
    std::string explorerUrl;  ///< The block explorer URL the provider uses.

    /**
     * A JSON object with predefined provider templates.
     * Each provider template is linked to a short alias.
     * Use getPresets() to check all available aliases/templates.
     */
    static json presets;

    /**
     * Sets the current provider to a template.
     * @param data The provider template object.
     */
    void _setData(json data);
    void setProvider(const Provider &p);

  public:
    std::mutex mutable lock;  ///< Mutex to manage read/write access to the provider object.

    /**
     * Default constructor.
     * @param id (optional) A template alias. Defaults to "avax-c-main" (Avalanche Mainnet).
     */
    Provider(std::string id = "avax-c-main");

    /// Copy constructor.
    Provider(const Provider &p);

    /**
     * Custom constructor.
     * @param name The provider's name.
     * @param host The provider's host.
     * @param target The provider's %RPC endpoint target.
     * @param port The provider's port.
     * @param chainId The provider's chain ID.
     * @param currency The provider's used currency.
     * @param explorerUrl The provider's used block explorer URL.
     */
    Provider(
      std::string name, std::string host, std::string target, uint64_t port,
      uint64_t chainId, std::string currency, std::string explorerUrl
    );

    const std::string& getName()        const { return this->name; }                ///< Getter for provider name.
    const std::string& getHost()        const { return this->host; }                ///< Getter for provider host.
    const std::string& getTarget()      const { return this->target; }            ///< Getter for provider %RPC endpoint target.
    const uint64_t& getPort()           const { return this->port; }                   ///< Getter for provider port.
    const uint64_t& getChainId()        const { return this->chainId; }             ///< Getter for provider chain ID.
    const std::string& getCurrency()    const { return this->currency; }        ///< Getter for provider currency.
    const std::string& getExplorerUrl() const { return this->explorerUrl; }  ///< Getter for provider block explorer URL.
    const static json& getPresets()           { return Provider::presets; }      ///< Getter for provider presets.

    friend class Web3;
};

#endif  // PROVIDER_H
