#include "../src/libs/catch2/catch_amalgamated.hpp"
#include <string>
#include <vector>
#include <unordered_map>

using Catch::Matchers::Equals;

namespace TestExample {
  TEST_CASE("Test Case Example One", "[example-one]") {
    SECTION("Test Str == Str") {
      std::string str1 = "Hello World";
      std::string str2 = "Hello World";
      REQUIRE(str1 == str2);
    }
    SECTION("Test uint == uint") {
      uint64_t uint1 = 1023818412;
      uint64_t uint2 = 1023818412;
      REQUIRE(uint1 == uint2);
    }
  }
  TEST_CASE("Test Case Example Two", "[example-two]") {
    SECTION("Test std::vector<std::string> == std::vector<std::string>") {
      std::vector<std::string> vec1 = {"Hello", "World"};
      std::vector<std::string> vec2 = {"Hello", "World"};
      REQUIRE(vec1 == vec2);
    }
    SECTION("Test std::unordered_map<std::string,std::string> == std::unordered_map<std::string,std::string>") {
      std::unordered_map<std::string,std::string> map1 = {{"Hello", "World"}};
      std::unordered_map<std::string,std::string> map2 = {{"Hello", "World"}};
      REQUIRE(map1 == map2);
    }
  }
}