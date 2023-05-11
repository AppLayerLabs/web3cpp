#include "Tests.h"
#include "../include/web3cpp/Utils.h"
#include "../src/libs/catch2/catch_amalgamated.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;
using Catch::Matchers::Equals;


    TEST_CASE("Test Utils Functionality")
    {

        SECTION("Test Utilities")
        {
            std::string password = "password";
            Tests test(password);

            bool bigNumbers = test.testBigNumbers();
            bool randomHexes = test.testRandomHexes();
            bool sha3 = test.testSHA3();
            bool solditySha3 = test.testSoliditySHA3();
            bool soliditySha3Raw = test.testSoliditySHA3Raw();
            bool hexes = test.testHexes();
            bool addresses = test.testAddresses();
            bool addressConversion = test.testAddressConversions();
            bool addressChecksums = test.testAddressChecksums();
            bool hexConversion = test.testHexConversions();
            bool hexStriping = test.testHexStripping();      
            bool fromHextoType = test.testFromHexToTypes();
            bool fromTypestoHex = test.testFromTypesToHex();
            bool weiConversion = test.testWeiConversions();
            bool hexPadding = test.testHexPadding();

            REQUIRE(hexes);
            REQUIRE(bigNumbers);
            REQUIRE(sha3);
            REQUIRE(solditySha3);
            REQUIRE(soliditySha3Raw);
            REQUIRE(randomHexes);
            REQUIRE(addresses);
            REQUIRE(addressChecksums);
            REQUIRE(addressConversion);
            REQUIRE(hexConversion);
            REQUIRE(hexStriping);
            REQUIRE(fromHextoType);
            REQUIRE(fromTypestoHex);
            REQUIRE(hexPadding);
            REQUIRE(weiConversion);
        }
    }
