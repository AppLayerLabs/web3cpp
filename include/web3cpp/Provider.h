#ifndef PROVIDER_H
#define PROVIDER_H

#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

class Provider {
  private:
    std::string name;
    std::string host;
    std::string target;
    uint64_t port;
    uint64_t chainId;
    std::string currency;
    std::string explorerUrl;
    static json presets;
    void _setData(json data);

  public:
    std::mutex mutable lock;

    // Constructors: default, copy and custom
    Provider(std::string id = "avax-c-main");
    Provider(const Provider &p);
    Provider(
      std::string name, std::string host, std::string target, uint64_t port,
      uint64_t chainId, std::string currency, std::string explorerUrl
    );

    // Operators
    Provider& operator=(const Provider &p);

    // Getters
    std::string getName() { return this->name; }
    std::string getHost() { return this->host; }
    std::string getTarget() { return this->target; }
    uint64_t getPort() { return this->port; }
    uint64_t getChainId() { return this->chainId; }
    std::string getCurrency() { return this->currency; }
    std::string getExplorerUrl() { return this->explorerUrl; }
    static json getPresets() { return Provider::presets; }
};

#endif  // PROVIDER_H
