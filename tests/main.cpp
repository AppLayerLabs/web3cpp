#include "Tests.h"

int main(int argc, char* argv[]) {
  // Parse CLI options
  std::vector<std::string> items;
  if (argc == 1) {
    std::cout << "Usage: " << argv[0] << " all|[CLASS]..." << std::endl
      << "Available classes: utils" << std::endl;
    return 0;
  } else if (argc == 2 && strcmp(argv[1], "all") == 0) {
    items = {"utils"};
  } else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "utils") == 0) {
        items.push_back(argv[i]);
      }
    }
  }
  if (items.empty()) {
    std::cout << "No classes selected." << std::endl
      << "Available classes: utils" << std::endl;
    return 0;
  }

  // Start testing
  std::string password = "password";
  Tests t(password);
  for (std::string item : items) {
    if (item == "utils") {
      std::cout << "Running tests from Utils" << std::endl;
      t.generateRandomHexes();
      t.createBigNumbers();
    }
  }
  t.showResults();
  return 0;

  /*
  t.generateAccount(
    "m/44'/60'/0'/0/0",
    "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space",
    "testAccount",
    "0x3E8467983bA80734654208b274EBf01264526117"
  );
  t.loadAndTestContract();
  */
}
