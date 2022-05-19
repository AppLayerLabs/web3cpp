#include "Tests.h"

int main(int argc, char* argv[]) {
  // Parse CLI options
  std::vector<std::string> items;
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " all|[CLASS]..." << std::endl
      << "Available classes: utils wallet contract" << std::endl;
    return 0;
  } else if (argc == 2 && strcmp(argv[1], "all") == 0) {
    items = {"utils", "wallet", "contract"};
  } else {
    for (int i = 1; i < argc; i++) {
      if (
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
      << "Available classes: utils wallet contract" << std::endl;
    return 0;
  }

  // Start testing
  std::string password = "password";
  Tests t(password);
  for (std::string item : items) {
    std::cout << "----------------------------------------" << std::endl;
    if (item == "utils") {
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
    } else if (item == "contract") {
      std::cout << "* Testing functions from Contract" << std::endl;
      t.loadAndTestContract();
    }
    std::cout << "----------------------------------------" << std::endl;
  }
  t.showResults();
  return 0;
}

