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
  std::string const addToAddressListExpected = "0x5fd673e8000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000020000000000000000000000002E913a79206280B3882860B3eF4dF8204a62C8B10000000000000000000000002E913a79206280B3882860B3eF4dF8204a62C8B1";
  std::string const addToStringListExpected = "0xece4955100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000052616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161610000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004f626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262620000000000000000000000000000000000";
  std::string const addToStringListFourExpected = "0xece4955100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000e0000000000000000000000000000000000000000000000000000000000000016000000000000000000000000000000000000000000000000000000000000001c0000000000000000000000000000000000000000000000000000000000000002461616161616161616161616161616161616161616161616161616161616161616161616100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005862626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262000000000000000000000000000000000000000000000000000000000000000000000000000000286363636363636363636363636363636363636363636363636363636363636363636363636363636300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000046464646400000000000000000000000000000000000000000000000000000000";
  std::string const addToBytesListExpected = "0x8ab94fd600000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000002aaaa0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002bbbb000000000000000000000000000000000000000000000000000000000000";
  std::string const addToBytesListFourExpected = "0x8ab94fd600000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000008000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000014000000000000000000000000000000000000000000000000000000000000000090aaaaaaaaaaaaaaaaa0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001fbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb0000000000000000000000000000000000000000000000000000000000000000020ccc00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000120ddddddddddddddddddddddddddddddddddd0000000000000000000000000000";
  std::string const testMultipleByteArrayExpected = "0x70afa559000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000002aaaa0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002bbbb0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000002cccc0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002dddd000000000000000000000000000000000000000000000000000000000000";
  std::string const addMultipleToByteListExpected = "0xf953151e000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000c00000000000000000000000000000000000000000000000000000000000000046aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002bbbb000000000000000000000000000000000000000000000000000000000000";
  std::string const addMultipleToStringListExpected = "0x4aee7a8d000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000000800000000000000000000000000000000000000000000000000000000000000003616161000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000036262620000000000000000000000000000000000000000000000000000000000";

  boost::filesystem::path contractPath(
    boost::filesystem::current_path().parent_path().string()
    + "/tests/ArrayTest.json"
  );
  json contractJson = Utils::readJSONFile(contractPath);
  Contract ArrayTest(contractJson, "0x0000000000000000000000000000000000000000");
  std::string addToAddressList = ArrayTest("addToAddressListArr",
    json::array({
      json::array({
        "0x2E913a79206280B3882860B3eF4dF8204a62C8B1",
        "0x2E913a79206280B3882860B3eF4dF8204a62C8B1"
      })
    })
  , error);

  std::string addToStringList = ArrayTest("addToStringListArr",
    json::array({
      json::array({
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
      })
    })
  , error);

  std::string addToStringListFour = ArrayTest("addToStringListArr",
    json::array({
      json::array({
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
        "cccccccccccccccccccccccccccccccccccccccc",
        "dddd"
      })
    })
  , error);

  std::string addToBytesList = ArrayTest("addToByteListArr",
    json::array({
      json::array({
        "0xaaaa",
        "0xbbbb"
      })
    })
  , error);

  std::string addToBytesListFour = ArrayTest("addToByteListArr",
    json::array({
      json::array({
        "0xaaaaaaaaaaaaaaaaa",
        "0xbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
        "0xccc",
        "0xddddddddddddddddddddddddddddddddddd"
      })
    })
  , error);

  std::string testMultipleByteArray = ArrayTest("testMultipleByteArray",
    json::array({
      json::array({
        "0xaaaa",
        "0xbbbb"
      }),
      json::array({
        "0xcccc",
        "0xdddd"
      })
    })
  , error);

  std::string addMultipleToByteList = ArrayTest("addMultipleToByteList",
    json::array({
      "0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
      "0xbbbb"
    })
  , error);

  std::string addMultipleToStringList = ArrayTest("addMultipleToStringList",
    json::array({
      "aaa",
      "bbb"
    })
  , error);

  if (error.getCode() != 0) {
    failed(error.what());
  } else if (addToAddressList != addToAddressListExpected) {
    failed("ABI doesn't match function: addToAddressListArr(address[]) two items");
  } else if (addToStringList != addToStringListExpected) {
    failed("ABI doesn't match function: addToStringListArr(string[]) two items");
  } else if (addToStringListFour != addToStringListFourExpected) {
    failed("ABI doesn't match function: addToStringListArr(string[]) four items");
  } else if (addToBytesList != addToBytesListExpected) {
    failed("ABI doesn't match function: addToByteListArr(bytes[]) two items");
  } else if (addToBytesListFour != addToBytesListFourExpected) {
    failed("ABI doesn't match function: addToByteListArr(bytes[]) four items");
  } else if (testMultipleByteArray != testMultipleByteArrayExpected) {
    failed("ABI doesn't match function: testMultipleByteArray(bytes[],bytes[])");
  } else if (addMultipleToByteList != addMultipleToByteListExpected) {
    failed("ABI doesn't match function: addMultipleToByteList(bytes,bytes)");
  } else if (addMultipleToStringList != addMultipleToStringListExpected) {
    failed("ABI doesn't match function: addMultipleToStringList(string,string)");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Expected Outputs: " << std::endl
    << "* " << addToAddressListExpected << std::endl
    << "* " << addToStringListExpected << std::endl
    << "* " << addToStringListFourExpected << std::endl
    << "* " << addToBytesListExpected << std::endl
    << "* " << addToBytesListFourExpected << std::endl
    << "* " << testMultipleByteArrayExpected << std::endl
    << "* " << addMultipleToByteListExpected << std::endl
    << "* " << addMultipleToStringListExpected << std::endl
    << "* Actual Outputs: " << std::endl
    << "* " << addToAddressList << std::endl
    << "* " << addToStringList << std::endl
    << "* " << addToStringListFour << std::endl
    << "* " << addToBytesList << std::endl
    << "* " << addToBytesListFour << std::endl
    << "* " << testMultipleByteArray << std::endl
    << "* " << addMultipleToByteList << std::endl
    << "* " << addMultipleToStringList << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;

  return;
}
