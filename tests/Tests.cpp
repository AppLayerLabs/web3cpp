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

  if (result == "0x") failed(error.what());
  else if (result != example) failed("Accounts don't match");
  else passed();

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

