#include "Tests.h"

int main(int argc, char* argv[]) {
  // Parse CLI options
  std::vector<std::string> items;
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " all|[CLASS]..." << std::endl
      << "Available classes: bip39 database net cipher solidity utils wallet contract" << std::endl;
    return 0;
  } else if (argc == 2 && strcmp(argv[1], "all") == 0) {
    items = {"bip39", "database", "net", "cipher", "solidity", "utils", "wallet", "contract"};
  } else {
    for (int i = 1; i < argc; i++) {
      if (
        strcmp(argv[i], "bip39") == 0 ||
        strcmp(argv[i], "database") == 0 ||
        strcmp(argv[i], "net") == 0 ||
        strcmp(argv[i], "cipher") == 0 ||
        strcmp(argv[i], "solidity") == 0 ||
        strcmp(argv[i], "utils") == 0 ||
        strcmp(argv[i], "wallet") == 0 ||
        strcmp(argv[i], "contract") == 0
      ) {
        items.push_back(argv[i]);
      }
    }
  }
  if (items.empty()) {
    std::cout << "No classes selected." << std::endl
      << "Available classes: bip39 database net cipher solidity utils wallet contract" << std::endl;
    return 0;
  }

  // Start testing
  std::string password = "password";
  Tests t(password);
  for (std::string item : items) {
    std::cout << "----------------------------------------" << std::endl;
    if (item == "bip39") {
      std::cout << "* Testing functions from Bip39" << std::endl;
      t.testMnemonicGeneration();
      t.testKeyPairGeneration();
      t.testMnemonicWordValidity();
      t.testMnemonicAddressGeneration();
    } else if (item == "database") {
      std::cout << "* Testing functions from Database" << std::endl;
      t.testDatabaseCRUD();
    } else if (item == "net") {
      std::cout << "* Testing functions from Net" << std::endl;
      t.testHTTPRequest();
      t.testCustomHTTPRequest();
    } else if (item == "cipher") {
      std::cout << "* Testing functions from Cipher" << std::endl;
      t.testCipherNormal();
      t.testCipherLegacy();
    } else if (item == "solidity") {
      std::cout << "* Testing functions from Solidity" << std::endl;
      t.testTypeChecks();
      t.testFunction();
      t.testUint();
      t.testAddress();
      t.testBool();
      t.testBytes();
      t.testString();
      t.testUintArray();
      t.testAddressArray();
      t.testBoolArray();
      t.testBytesArray();
      t.testStringArray();
      t.testMulti();
    } else if (item == "utils") {
      std::cout << "* Testing functions from Utils" << std::endl;
      t.testRandomHexes();
      t.testBigNumbers();
      t.testSHA3();
      t.testSoliditySHA3();
      t.testSoliditySHA3Raw();
      t.testHexes();
      t.testAddresses();
      t.testAddressConversions();
      t.testAddressChecksums();
      t.testHexConversions();
      t.testHexStripping();
      t.testFromHexToTypes();
      t.testFromTypesToHex();
      t.testWeiConversions();
      t.testHexPadding();
    } else if (item == "wallet") {
      std::cout << "* Testing functions from Wallet" << std::endl;
      t.generateAccount();
      t.handlePasswordCheck();
      t.handlePasswordStorageAuto();
      t.handlePasswordStorageManual();
      t.signAndRecoverMessage();
    } else if (item == "contract") {
      std::cout << "* Testing functions from Contract" << std::endl;
      t.loadAndTestContractLegacy();
      t.loadAndTestContractNormal();
    }
    std::cout << "----------------------------------------" << std::endl;
  }
  t.showResults();
  return 0;
}

