#include <web3cpp/Provider.h>

json Provider::presets = {
  {"avax-c-main", {
    {"name", "Avalanche Mainnet (C-Chain)"},
    {"host", "api.avax.network"},
    {"target", "/ext/bc/C/rpc"},
    {"port", 443},
    {"chainId", 43114},
    {"currency", "AVAX"},
    {"explorerUrl", "https://snowtrace.io"},
  }},
  {"avax-c-test", {
    {"name", "Avalanche Testnet (C-Chain)"},
    {"host", "api.avax-test.network"},
    {"target", "/ext/bc/C/rpc"},
    {"port", 443},
    {"chainId", 43113},
    {"currency", "AVAX"},
    {"explorerUrl", "https://testnet.snowtrace.io"},
  }}
};

void Provider::_setData(json data) {
  this->name = data["name"].get<std::string>();
  this->host = data["host"].get<std::string>();
  this->target = data["target"].get<std::string>();
  this->port = data["port"].get<uint64_t>();
  this->chainId = data["chainId"].get<uint64_t>();
  this->currency = data["currency"].get<std::string>();
  this->explorerUrl = data["explorerUrl"].get<std::string>();
}

Provider::Provider(std::string id) {
  if (!presets.count(id)) {
    std::cout << "Provider preset " << id << " does not exist, "
      << "falling back to avax-c-main" << std::endl;
    id = "avax-c-main";
  }
  _setData(presets[id]);
}

Provider::Provider(const Provider &p) {
  json pJ = {
    {"name", p.name}, {"host", p.host}, {"target", p.target}, {"port", p.port},
    {"chainId", p.chainId}, {"currency", p.currency}, {"explorerUrl", p.explorerUrl}
  };
  _setData(pJ);
}

Provider::Provider(
  std::string name, std::string host, std::string target, uint64_t port,
  uint64_t chainId, std::string currency, std::string explorerUrl
) {
  json pJ = {
    {"name", name}, {"host", host}, {"target", target}, {"port", port},
    {"chainId", chainId}, {"currency", currency}, {"explorerUrl", explorerUrl}
  };
  _setData(pJ);
}

Provider& Provider::operator=(const Provider &p) {
  json pJ = {
    {"name", p.name}, {"host", p.host}, {"target", p.target}, {"port", p.port},
    {"chainId", p.chainId}, {"currency", p.currency}, {"explorerUrl", p.explorerUrl}
  };
  _setData(pJ);
  return *this;
}

