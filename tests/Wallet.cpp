#include "Tests.h"

void Tests::generateAccount() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  Error error;
  std::string derivPath = "m/44'/60'/0'/0/0";
  std::string seed = "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space";
  std::string name = "testAccount";
  std::string example = "0x3e8467983ba80734654208b274ebf01264526117";

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

