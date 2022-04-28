#include "Tests.h"

int main() {
  std::string password = "password";
  Tests t(password);
  t.generateAccount(
    "m/44'/60'/0'/0/0",
    "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space",
    "testAccount",
    "0x3E8467983bA80734654208b274EBf01264526117"
  );

  t.loadAndTestContract();
  t.showResults();
  return 0;
}
