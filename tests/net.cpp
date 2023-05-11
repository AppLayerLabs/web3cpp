#include "../src/libs/catch2/catch_amalgamated.hpp"
#include "../include/web3cpp/Web3.h"
#include "Tests.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;
using Catch::Matchers::Equals;

namespace TNet
{
    TEST_CASE("Standard HTTP Request")
    {
        SECTION("Normal HTTP Request")
        {
            std::unique_ptr<Web3>web3 = std::make_unique<Web3>();
            json reqBody = RPC::eth_blockNumber();
            std::string reqStr = Net::HTTPRequest(
                web3->getProvider(), Net::RequestTypes::POST, reqBody.dump());
            json respObj = json::parse(reqStr);

            REQUIRE(!respObj.count("error"));
        }
    }

    TEST_CASE("Custom HTTP Request")
    {
        SECTION("Custom HTTP Request")
        {
            std::string host = "api.avax.network";
            std::string port = "443";
            std::string target = "/ext/bc/C/rpc";
            std::string reqType = "POST";
            std::string reqCont = "application/json";
            json reqBody = RPC::eth_blockNumber();
            std::string reqStr = Net::customHTTPRequest(
                reqBody.dump(), host, port, target, reqType, reqCont);
            json respObj = json::parse(reqStr);

            REQUIRE(!respObj.count("error"));
        }
    }

}