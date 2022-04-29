#include "Tests.h"

void Tests::generateRandomHexes() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<unsigned int> sizes = {4, 8, 16, 32};
  std::vector<std::string> results;
  std::vector<std::string> resultsPrefixed;
  bool allPass = true;

  for (unsigned int size : sizes) {
    std::string res = Utils::randomHex(size);
    std::string resPfx = Utils::randomHex(size, true);
    results.push_back(res);
    resultsPrefixed.push_back(resPfx);
    if (allPass &&
      ((res.length() / 2) != size || ((resPfx.length() - 2) / 2) != size)
    ) {
      allPass = false;
    }
  }
  if (!allPass) {
    failed("Wrong size for one or more hexes");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Outputs: " << std::endl;
  for (int i = 0; i < sizes.size(); i++) {
    logStream << "* " << sizes[i] << " -> " << results[i] << std::endl
      << "* " << sizes[i] << " (0x) -> " << resultsPrefixed[i] << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
  return;
}

void Tests::createBigNumbers() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string decNumber = "1234567890987654321";
  std::string hexNumber = "0x112210f4b16c1cb1";

  BigNumber decBN = Utils::toBN(decNumber);
  BigNumber hexBN = Utils::toBN(hexNumber);
  std::string decBNStr = boost::lexical_cast<std::string>(decBN);
  std::string hexBNStr = "0x" + Utils::toHex(hexBN);

  if (decBN != hexBN) {
    failed("Decimal and hex numbers don't match");
  } else if (decBNStr != decNumber) {
    failed("Decimal input and output don't match");
  } else if (hexBNStr != hexNumber) {
    failed("Hex input and output don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Input (DEC): " << decNumber << std::endl
    << "* Input (HEX): " << hexNumber << std::endl
    << "* Output (DEC): " << decBNStr << std::endl
    << "* Output (HEX): " << hexBNStr << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

