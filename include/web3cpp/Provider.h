#ifndef PROVIDER_H
#define PROVIDER_H

#include <cstdint>
#include <mutex>
#include <string>

class Provider {
  private:
    std::string name;
    std::string host;
    std::string target;
    uint64_t port;
    uint64_t chainId;
    std::string currency;
    std::string explorerUrl;

  public:
    std::mutex mutable lock;

    // Constructors
    Provider(); // Defaults to AVAX mainnet
    Provider(const Provider &p);

    // Operators
    Provider& operator=(Provider const &p);

    // Getters
    std::string getName() { return this->name; }
    std::string getHost() { return this->host; }
    std::string getTarget() { return this->target; }
    uint64_t getPort() { return this->port; }
    uint64_t getChainId() { return this->chainId; }
    std::string getCurrency() { return this->currency; }
    std::string getExplorerUrl() { return this->explorerUrl; }
};

#endif  // PROVIDER_H
