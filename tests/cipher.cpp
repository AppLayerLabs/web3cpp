#include "../src/libs/catch2/catch_amalgamated.hpp"
#include "../include/web3cpp/Cipher.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using Catch::Matchers::Equals;

namespace TCipher
{
    TEST_CASE("Cipher is normal")
    {
        SECTION("Cipher Test")
        {
            std::string pass = "password";
            std::string msg1 = "Test Message";
            std::string msg2 = "0x1234567890123456789012345678901234567890";
            std::string msg3 = "{\"object\": \"testJson\", \"params\": [\"aaa\", \"bbb\", \"ccc\"]}";
            Error e1, e2, e3, e4, e5, e6;
            std::string enc1 = Cipher::encrypt(msg1, pass, e1);
            std::string dec1 = Cipher::decrypt(enc1, pass, e2);
            std::string enc2 = Cipher::encrypt(msg2, pass, e3);
            std::string dec2 = Cipher::decrypt(enc2, pass, e4);
            std::string enc3 = Cipher::encrypt(msg3, pass, e5);
            std::string dec3 = Cipher::decrypt(enc3, pass, e6);
            REQUIRE(e1.getCode() == 0);
            REQUIRE(e2.getCode() == 0);
            REQUIRE(e3.getCode() == 0);
            REQUIRE(e4.getCode() == 0);
            REQUIRE(e5.getCode() == 0);
            REQUIRE(e6.getCode() == 0);
                        
            REQUIRE(dec1 == msg1);
            REQUIRE(dec2 == msg2);
            REQUIRE(dec3 == msg3);
        }
    }
    TEST_CASE("Legacy Cipher Test")
        {
        SECTION("Legacy Cipher ")
        {
            std::string pass = "password";
            std::string msg1 = "Test Message";
            std::string msg2 = "0x1234567890123456789012345678901234567890";
            std::string msg3 = "{\"object\": \"testJson\", \"params\": [\"aaa\", \"bbb\", \"ccc\"]}";
            std::string enc1 = Cipher::encryptAleth(msg1, pass);
            std::string dec1 = Cipher::decryptAleth(enc1, pass);
            std::string enc2 = Cipher::encryptAleth(msg2, pass);
            std::string dec2 = Cipher::decryptAleth(enc2, pass);
            std::string enc3 = Cipher::encryptAleth(msg3, pass);
            std::string dec3 = Cipher::decryptAleth(enc3, pass);

            REQUIRE(!dec1.empty());
            REQUIRE(!dec2.empty());
            REQUIRE(!dec3.empty());

        }
    }

}