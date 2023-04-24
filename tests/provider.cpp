#include "../src/libs/catch2/catch_amalgamated.hpp"
#include "../include/web3cpp/Provider.h"


namespace TProvider {
  TEST_CASE("Provider Class Tests","[provider]") {
    SECTION("Provider std::string constructor") {
      Provider providerAvaxCCHain("avax-c-main");
      REQUIRE(providerAvaxCCHain.getName() == "Avalanche Mainnet (C-Chain)");
      REQUIRE(providerAvaxCCHain.getHost() == "api.avax.network");
      REQUIRE(providerAvaxCCHain.getTarget() == "/ext/bc/C/rpc");
      REQUIRE(providerAvaxCCHain.getPort() == 443);
      REQUIRE(providerAvaxCCHain.getChainId() == 43114);
      REQUIRE(providerAvaxCCHain.getCurrency() == "AVAX");
      REQUIRE(providerAvaxCCHain.getExplorerUrl() == "https://snowtrace.io");

      Provider providerAvaxCCHainTest("avax-c-test");
      REQUIRE(providerAvaxCCHainTest.getName() == "Avalanche Testnet (C-Chain)");
      REQUIRE(providerAvaxCCHainTest.getHost() == "api.avax-test.network");
      REQUIRE(providerAvaxCCHainTest.getTarget() == "/ext/bc/C/rpc");
      REQUIRE(providerAvaxCCHainTest.getPort() == 443);
      REQUIRE(providerAvaxCCHainTest.getChainId() == 43113);
      REQUIRE(providerAvaxCCHainTest.getCurrency() == "AVAX");
      REQUIRE(providerAvaxCCHainTest.getExplorerUrl() == "https://testnet.snowtrace.io");

      // Provider defaults to avax-c-main if preset does not exist
      Provider providerFallback("aaaaaaaa");
      REQUIRE(providerAvaxCCHain.getName() == "Avalanche Mainnet (C-Chain)");
      REQUIRE(providerAvaxCCHain.getHost() == "api.avax.network");
      REQUIRE(providerAvaxCCHain.getTarget() == "/ext/bc/C/rpc");
      REQUIRE(providerAvaxCCHain.getPort() == 443);
      REQUIRE(providerAvaxCCHain.getChainId() == 43114);
      REQUIRE(providerAvaxCCHain.getCurrency() == "AVAX");
      REQUIRE(providerAvaxCCHain.getExplorerUrl() == "https://snowtrace.io");

    }

    SECTION("Provider Copy Constructor") {
      Provider provider("avax-c-test");
      Provider providerCopy(provider);
      REQUIRE(providerCopy.getName() == provider.getName());
      REQUIRE(providerCopy.getHost() == provider.getHost());
      REQUIRE(providerCopy.getTarget() == provider.getTarget());
      REQUIRE(providerCopy.getPort() == provider.getPort());
      REQUIRE(providerCopy.getChainId() == provider.getChainId());
      REQUIRE(providerCopy.getCurrency() == provider.getCurrency());
      REQUIRE(providerCopy.getExplorerUrl() == provider.getExplorerUrl());
    }

    SECTION("Provider complete constructor") {
      std::string name = "Avalanche Testnet (C-Chain)";
      std::string host = "api.avax-test.network";
      std::string target = "/ext/bc/C/rpc";
      uint64_t port = 443;
      uint64_t chainId = 43113;
      std::string currency = "AVAX";
      std::string explorerUrl = "https://testnet.snowtrace.io";
      Provider provider(
        name, host, target, port, chainId, currency, explorerUrl
      );
      REQUIRE(provider.getName() == name);
      REQUIRE(provider.getHost() == host);
      REQUIRE(provider.getTarget() == target);
      REQUIRE(provider.getPort() == port);
      REQUIRE(provider.getChainId() == chainId);
      REQUIRE(provider.getCurrency() == currency);
      REQUIRE(provider.getExplorerUrl() == explorerUrl);
    }
  }
}