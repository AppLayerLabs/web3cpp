#include "Tests.h"

void Tests::testRandomHexes() {
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
}

void Tests::testBigNumbers() {
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

void Tests::testSHA3() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string strA = "Test String";
  std::string strB = "";  // Empty on purpose, for testing sha3Raw()
  std::string strHash = "5a3f690ae57a7ead22e6ced03d1c290406abe7adffcd556d84244ed91b119ee8";
  std::string zeroHash = "c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";

  std::string hashA1 = Utils::sha3(strA);
  std::string hashA2 = Utils::sha3Raw(strA);
  std::string hashB1 = Utils::sha3(strB);
  std::string hashB2 = Utils::sha3Raw(strB);

  if (
    (hashA1 != strHash) || (hashA2 != strHash) ||
    (hashB1 != "") || (hashB2 != zeroHash)
  ) {
    failed("One or more hashes don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* A: " << strA << " (hash: " << strHash << ")" << std::endl
    << "* B: (empty)" << " (hash: " << zeroHash << ")" << std::endl
    << "* Outputs:" << std::endl
    << "* sha3(A): " << hashA1 << std::endl
    << "* sha3Raw(A): " << hashA2 << std::endl
    << "* sha3(B): " << (hashB1.empty() ? "(empty)" : hashB1) << std::endl
    << "* sha3Raw(B): " << hashB2 << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testHexes() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> hexStrings = {
    "0x0a1B2c3D4e5F6789", "0x0g1H2i3J4k5L6789",
    "0a1B2c3D4e5F6789", "0g1H2i3J4k5L6789"
  };
  std::vector<std::pair<bool, bool>> hexPassExpect = {
    {true, true}, {false, false},
    {true, false}, {false, false}
  };
  std::vector<std::pair<bool, bool>> hexPassResult;

  for (std::string hex : hexStrings) {
    hexPassResult.push_back({Utils::isHex(hex), Utils::isHexStrict(hex)});
  }

  bool allPass = true;
  for (int i = 0; i < hexPassExpect.size(); i++) {
    if (allPass &&
      (hexPassResult[i].first != hexPassExpect[i].first) ||
      (hexPassResult[i].second != hexPassExpect[i].second)
    ) {
      allPass = false; break;
    }
  }
  if (!allPass) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl;
  for (int i = 0; i < hexStrings.size(); i++) {
    logStream << "* " << i << ": " << hexStrings[i] << std::endl;
  }
  logStream << "* Expected outputs: " << std::endl;
  for (int i = 0; i < hexPassExpect.size(); i++) {
    logStream << "* " << i
      << " - Normal: " << hexPassExpect[i].first
      << " - Strict: " << hexPassExpect[i].second
    << std::endl;
  }
  logStream << "* Actual outputs: " << std::endl;
  for (int i = 0; i < hexPassResult.size(); i++) {
    logStream << "* " << i
      << " - Normal: " << hexPassResult[i].first
      << " - Strict: " << hexPassResult[i].second
    << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testAddresses() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> addresses = {
    "0x3E8467983bA80734654208b274EBf01264526117",
    "0x3e8467983ba80734654208b274ebf01264526117",
    "0X3E8467983BA80734654208B274EBF01264526117",
    "0x3e8467983Ba80734654208B274ebF01264526117",
    "0x3e8467983Ba80734654208B274ebF0126452611"
  };
  std::vector<bool> expects = {true, true, true, false, false};
  std::vector<bool> results;

  for (std::string address : addresses) {
    results.push_back(Utils::isAddress(address));
  }

  bool allPass = true;
  for (int i = 0; i < expects.size(); i++) {
    if (allPass && results[i] != expects[i]) {
      allPass = false; break;
    }
  }
  if (!allPass) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl;
  for (int i = 0; i < addresses.size(); i++) {
    logStream << "* " << i << ": " << addresses[i] << std::endl;
  }
  logStream << "* Expected outputs: " << std::endl;
  for (int i = 0; i < expects.size(); i++) {
    logStream << "* " << i << " - " << expects[i] << std::endl;
  }
  logStream << "* Actual outputs: " << std::endl;
  for (int i = 0; i < results.size(); i++) {
    logStream << "* " << i << " - " << results[i] << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testAddressConversions() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string lowerAdd = "0x3e8467983ba80734654208b274ebf01264526117";
  std::string upperAdd = "0x3E8467983BA80734654208B274EBF01264526117";
  std::string checkAdd = "0x3E8467983bA80734654208b274EBf01264526117";

  std::string lowerRes = Utils::toLowercaseAddress(upperAdd);
  std::string upperRes = Utils::toUppercaseAddress(lowerAdd);
  std::string checkRes = Utils::toChecksumAddress(lowerAdd);

  if (lowerRes != lowerAdd || upperRes != upperAdd || checkRes != checkAdd) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* Lowercase: " << lowerAdd << std::endl
    << "* Uppercase: " << upperAdd << std::endl
    << "* Checksum: " << checkAdd << std::endl
    << "* Outputs: " << std::endl
    << "* Lowercase: " << lowerRes << std::endl
    << "* Uppercase: " << upperRes << std::endl
    << "* Checksum: " << checkRes << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testAddressChecksums() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> addresses = {
    "0x3E8467983bA80734654208b274EBf01264526117",
    "0x3e8467983Ba80734654208B274ebF01264526117",
    "0x3e8467983ba80734654208b274ebf01264526117",
    "0x3E8467983BA80734654208B274EBF01264526117"
  };
  std::vector<bool> expects = {true, false, false, false};
  std::vector<bool> results;

  for (std::string address : addresses) {
    results.push_back(Utils::checkAddressChecksum(address));
  }

  bool allPass = true;
  for (int i = 0; i < expects.size(); i++) {
    if (results[i] != expects[i]) { allPass = false; break; }
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl;
  for (int i = 0; i < addresses.size(); i++) {
    logStream << "* " << i << ": " << addresses[i] << std::endl;
  }
  logStream << "* Expected outputs: " << std::endl;
  for (int i = 0; i < expects.size(); i++) {
    logStream << "* " << i << " - " << expects[i] << std::endl;
  }
  logStream << "* Actual outputs: " << std::endl;
  for (int i = 0; i < results.size(); i++) {
    logStream << "* " << i << " - " << results[i] << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testHexConversions() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  BigNumber bn(836502639245);
  std::string bnStr = "836502639245";
  std::string textStr = "Test String";
  std::string bnHex = "c2c371528d";
  std::string textHex = "5465737420537472696e67";

  std::string bnRes = Utils::toHex(bn);
  std::string bnStrRes = Utils::toHex(bnStr);
  std::string textRes = Utils::toHex(textStr);

  if ((bnRes != bnHex) || (bnStrRes != bnHex) || (textRes != textHex)) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* Number: " << bn << std::endl
    << "* Number string: " << bnStr << std::endl
    << "* Text string: " << textStr << std::endl
    << "* Expected outputs: " << std::endl
    << "* Number: " << bnHex << std::endl
    << "* Number string: " << bnHex << std::endl
    << "* Text string: " << textHex << std::endl
    << "* Actual outputs: " << std::endl
    << "* Number: " << bnRes << std::endl
    << "* Number string: " << bnStrRes << std::endl
    << "* Text string: " << textRes << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testHexStripping() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> hexes = {"0x12345", "0X12345", "0xghijk", ""}; // Last one is empty on purpose
  std::vector<std::string> expects = {"12345", "12345", "0xghijk", ""};
  std::vector<std::string> results;

  for (std::string hex : hexes) {
    results.push_back(Utils::stripHexPrefix(hex));
  }

  bool allPass = true;
  for (int i = 0; i < expects.size(); i++) {
    if (allPass && results[i] != expects[i]) {
      allPass = false; break;
    }
  }
  if (!allPass) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl;
  for (int i = 0; i < hexes.size(); i++) {
    logStream << "* " << (!hexes[i].empty() ? hexes[i] : "(empty)") << std::endl;
  }
  logStream << "* Expected outputs: " << std::endl;
  for (int i = 0; i < expects.size(); i++) {
    logStream << "* " << (!expects[i].empty() ? expects[i] : "(empty)") << std::endl;
  }
  logStream << "* Actual outputs: " << std::endl;
  for (int i = 0; i < results.size(); i++) {
    logStream << "* " << (!results[i].empty() ? results[i] : "(empty)") << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testFromHexToTypes() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string bnHex = "0x16a5b628c2de92f4";
  std::string utf8Hex = "0xa2bce697ade5";
  std::string asciiHex = "0x5465737420537472696e67";
  std::string bytesHex = "0x5465737420537472696e67";
  BigNumber bnExp(1631910726175986420);
  std::string bnStrExp = "1631910726175986420";
  std::string utf8Exp = u8"漢字";
  std::string asciiExp = "Test String";
  std::vector<unsigned int> bytesExp = {
    84, 101, 115, 116, 32, 83, 116, 114, 105, 110, 103
  };

  BigNumber bnRes(Utils::hexToBigNumber(bnHex));
  std::string bnStrRes = Utils::hexToNumberString(bnHex);
  std::string utf8Res = Utils::hexToUtf8(utf8Hex);
  std::string asciiRes = Utils::hexToAscii(asciiHex);
  std::vector<unsigned int> bytesRes = Utils::hexToBytes(bytesHex);

  if (
    (bnRes != bnExp) || (bnStrRes != bnStrExp) ||
    (utf8Res != utf8Exp) || (asciiRes != asciiExp) || (bytesRes != bytesExp)
  ) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* Number hex: " << bnHex << std::endl
    << "* UTF-8 hex: " << utf8Hex << std::endl
    << "* ASCII hex: " << asciiHex << std::endl
    << "* Bytes hex: " << bytesHex << std::endl
    << "* Expected outputs: " << std::endl
    << "* Number: " << bnExp << std::endl
    << "* Number string: " << bnStrExp << std::endl
    << "* UTF-8 string: " << utf8Exp << std::endl
    << "* ASCII string: " << asciiExp << std::endl
    << "* Bytes string: " << bytesExp << std::endl
    << "* Actual outputs: " << std::endl
    << "* Number: " << bnRes << std::endl
    << "* Number string: " << bnStrRes << std::endl
    << "* UTF-8 string: " << utf8Res << std::endl
    << "* ASCII string: " << asciiRes << std::endl
    << "* Bytes string: " << bytesRes << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testFromTypesToHex() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string utf8Str = u8"漢字";
  std::string asciiStr = "Test String";
  std::vector<unsigned int> bytesStr = {
    84, 101, 115, 116, 32, 83, 116, 114, 105, 110, 103
  };
  std::string utf8Exp = "0xa2bce697ade5";
  std::string asciiExp = "0x5465737420537472696e67";
  std::string bytesExp = "0x5465737420537472696e67";

  std::string utf8Res = Utils::utf8ToHex(utf8Str);
  std::string asciiRes = Utils::asciiToHex(asciiStr);
  std::string bytesRes = Utils::bytesToHex(bytesStr);

  if ((utf8Res != utf8Exp) || (asciiRes != asciiExp) || (bytesRes != bytesExp)) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* UTF-8 string: " << utf8Str << std::endl
    << "* ASCII string: " << asciiStr << std::endl
    << "* Bytes string: " << bytesStr << std::endl
    << "* Expected outputs: " << std::endl
    << "* UTF-8 hex: " << utf8Exp << std::endl
    << "* ASCII hex: " << asciiExp << std::endl
    << "* Bytes hex: " << bytesExp << std::endl
    << "* Actual outputs: " << std::endl
    << "* UTF-8 hex: " << utf8Res << std::endl
    << "* ASCII hex: " << asciiRes << std::endl
    << "* Bytes hex: " << bytesRes << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testWeiConversions() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::pair<std::string, int>> amountsTo = {
    {"15", 18}, {"30", 9}, {"5", 2}, {"2", 5}
  };
  std::vector<std::pair<std::string, int>> amountsFrom = {
    {"5000000000000000000", 18}, {"1000000000", 9}, {"80000", 4}, {"1337", 2}
  };
  std::vector<std::string> toExp = {
    "15000000000000000000", "30000000000", "500", "200000"
  };
  std::vector<std::string> fromExp = {
    "5.000000000000000000", "1.000000000", "8.0000", "13.37"
  };
  std::vector<std::string> toRes, fromRes;

  for (int i = 0; i < amountsTo.size(); i++) {
    toRes.push_back(Utils::toWei(amountsTo[i].first, amountsTo[i].second));
  }
  for (int i = 0; i < amountsFrom.size(); i++) {
    fromRes.push_back(Utils::fromWei(amountsFrom[i].first, amountsFrom[i].second));
  }

  bool allPass = true;
  for (int i = 0; i < toExp.size(); i++) {
    if (allPass && toExp[i] != toRes[i]) { allPass = false; break; }
  }
  for (int i = 0; i < fromExp.size(); i++) {
    if (allPass && fromExp[i] != fromRes[i]) { allPass = false; break; }
  }
  if (!allPass) {
    failed("One or more amounts don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs (amount, decimals, to/from): " << std::endl;
  for (int i = 0; i < amountsTo.size(); i++) {
    logStream << "* (" << amountsTo[i].first << ", "
      << amountsTo[i].second << ", TO)" << std::endl;
  }
  for (int i = 0; i < amountsFrom.size(); i++) {
    logStream << "* (" << amountsFrom[i].first << ", "
      << amountsFrom[i].second << ", FROM)" << std::endl;
  }
  logStream << "* Expected outputs: " << std::endl;
  for (int i = 0; i < toExp.size(); i++) {
    logStream << "* " << toExp[i] << std::endl;
  }
  for (int i = 0; i < fromExp.size(); i++) {
    logStream << "* " << fromExp[i] << std::endl;
  }
  logStream << "* Actual outputs: " << std::endl;
  for (int i = 0; i < toRes.size(); i++) {
    logStream << "* " << toRes[i] << std::endl;
  }
  for (int i = 0; i < fromRes.size(); i++) {
    logStream << "* " << fromRes[i] << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

void Tests::testHexPadding() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string hexRaw = "DeaDBeeF";
  std::string hexPfx = "0xDeaDBeeF";
  std::vector<int> sizes = {10, 12, 14, 16};
  std::vector<std::pair<std::string, std::string>> rawPads;
  std::vector<std::pair<std::string, std::string>> pfxPads;
  bool allPass = true;

  for (int size : sizes) {
    rawPads.push_back({Utils::padLeft(hexRaw, size), Utils::padRight(hexRaw, size)});
    pfxPads.push_back({Utils::padLeft(hexPfx, size), Utils::padRight(hexPfx, size)});
  }

  for (int i = 0; i < sizes.size(); i++) {
    if (allPass && (
      rawPads[i].first.length() != sizes[i] ||
      rawPads[i].second.length() != sizes[i] ||
      pfxPads[i].first.length() != sizes[i] + 2 ||
      pfxPads[i].second.length() != sizes[i] + 2
    )) {
      allPass = false; break;
    }
  }
  if (!allPass) {
    failed("Wrong size on one or more results");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs:" << std::endl
    << "* " << hexRaw << " (raw)" << std::endl
    << "* " << hexPfx << " (prefixed)" << std::endl
    << "* Outputs: " << std::endl;
  for (int i = 0; i < sizes.size(); i++) {
    logStream << "* " << sizes[i]
      << " -> " << rawPads[i].first
      << " | " << rawPads[i].second
      << " | " << pfxPads[i].first
      << " | " << pfxPads[i].second
    << std::endl;
  }
  logStream << "* Test result: " <<
    ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}

