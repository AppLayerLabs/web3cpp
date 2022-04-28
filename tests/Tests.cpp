#include "Tests.h"

void Tests::generateAccount(
  std::string derivPath, std::string seed, std::string name, std::string example
) {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  Error error;
  example = Utils::toLowercaseAddress(example);
  std::string result = Utils::toLowercaseAddress(web3->wallet.createAccount(
    derivPath, this->password, name, error, seed
  ));

  if (result == "0x") {
    failed(error.what());
  } else if (result != example) {
    failed("Accounts don't match");
  } else {
    passed();
  }

  web3->wallet.deleteAccount(result);
  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Derivation path: " << derivPath << std::endl
    << "* Seed: " << seed << std::endl
    << "* Name: " << name << std::endl
    << "* Example: " << example << std::endl
    << "* Output: " << result << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::loadAndTestContract() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  Error error;
  std::string ABIexample = "0x5fd673e8000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000020000000000000000000000002E913a79206280B3882860B3eF4dF8204a62C8B10000000000000000000000002E913a79206280B3882860B3eF4dF8204a62C8B1";
  boost::filesystem::path contractPath(
    boost::filesystem::current_path().parent_path().string()
    + "/tests/ArrayTest.json"
  );
  json contractJson = Utils::readJSONFile(contractPath);
  Contract ArrayTest(contractJson, "0x0000000000000000000000000000000000000000");
  std::string addToAddressListArrStr = ArrayTest("addToAddressListArr",
    json::array({
      json::array({
        "0x2E913a79206280B3882860B3eF4dF8204a62C8B1",
        "0x2E913a79206280B3882860B3eF4dF8204a62C8B1"
      })
    })
  , error);

  if (error.getCode() != 0) {
    failed(error.what());
  } else if (addToAddressListArrStr != ABIexample) {
    failed("ABI doesn't match function: addToAddressListArr(address[])");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Expected Output: " << ABIexample << std::endl
    << "* Actual Output: " << addToAddressListArrStr << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;

  return;
}
