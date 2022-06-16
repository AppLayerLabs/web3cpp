#include <web3cpp/Provider.h>

Provider::Provider() {
  name = "Avalanche Mainnet";
  host = "api.avax.network";
  target = "/ext/bc/C/rpc";
  port = 443;
  chainId = 43114;
  currency = "AVAX";
  explorerUrl = "";
}

Provider::Provider(const Provider &p) {
  name = p.name;
  host = p.host;
  target = p.target;
  port = p.port;
  chainId = p.chainId;
  currency = p.currency;
  explorerUrl = p.explorerUrl;
}

Provider& Provider::operator=(Provider const &p) {
  name = p.name;
  host = p.host;
  target = p.target;
  port = p.port;
  chainId = p.chainId;
  currency = p.currency;
  explorerUrl = p.explorerUrl;
  return *this;
}

