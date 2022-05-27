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

/*
void Tests::testSoliditySHA3() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  json j1A = { {"type", "uint256"}, {"value", "129831751235123"} };
  json j1B = { {"type", "address"}, {"value", "0xc4ea73d428ab6589c36905d0f0b01f3051740ff8"} };
  json j1C = { {"type", "bool"}, {"value", true} };
  json j1D = { {"type", "bytes"}, {"value", "0xaaaa"} };
  json j1E = { {"type", "string"}, {"value", "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque eget porttitor tortor, et tincidunt nibh. Aenean erat quam, maximus id gravida sit amet, rhoncus sed nulla. Curabitur maximus tellus diam, vel vulputate sapien maximus vitae. Duis consectetur, felis at efficitur consectetur, tortor nisl bibendum mauris, eget venenatis augue justo et orciSimple "} };
  json j2A = { {"type", "uint256[]"}, {"value", {
    // ...
  }}};
  json j2B = { {"type", "address[]"}, {"value", {
    "0xc4ea73d428ab6589c36905d0f0b01f3051740ff8",
    "0xc4ea73d428ab6589c36905d0f0b01f3051740ff8",
    "0xc4ea73d428ab6589c36905d0f0b01f3051740ff8"
  }}};
  json j2C = { {"type", "bool[]"}, {"value", {
    // ...
  }}};
  json j2D = { {"type", "bytes[]"}, {"value", {
    "0xaaa",
    "0xbbb",
    "0xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
  }}};
  json j2E = { {"type", "string[]"}, {"value", {
    "aaaaa",
    "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
    "ccc"
  }}};

  std::string r1A = "0x23ca82fd08978ec8bdedfb49a8c82cc8bd1f2f1e6734f39e7d54bfe13dc36c25";
  std::string r1B = "0x512f675e26efc88a1dba5491171ed07ca7bdec44d0822a65eeeb06b3f8c636d0";
  std::string r1C = "0xb10e2d527612073b26eecdfd717e6a320cf44b4afac2b0732d9fcbe2b7fa0cf6";
  std::string r1D = "0xd69a7fe744d274f77aadbee2739de42fcd2fa333337de675727937e3cad5bb77";
  std::string r1E = "0xe45cef14cb78752441492c1a0d7a89fe7a643fbc8d9a204d14a5526870a2c012";
  std::string r2A = "0x";
  std::string r2B = "0xb936902b161c1344a2b5a9b9e87a0ac807ecc569844403cf41fd78d9a01f5451";
  std::string r2C = "0x";
  std::string r2D = "0xc2223977caa88504241d4197cdb9e0ebf4ea498463d0fd6e51d80fb26425c380";
  std::string r2E = "0x33cdc4b381534e45e8fdc57c990d35d3143af04501fcfe1178d3dbf9c606f2de";

  Error e1A, e1B, e1C, e2, e3, e4A, e4B, e4C, e5A, e5B, e6, e7;
  std::string h1A = Utils::soliditySha3(j1A, e1A);
  std::string h1B = Utils::soliditySha3(j1B, e1B);
  std::string h1C = Utils::soliditySha3(j1C, e1C);
  std::string h2 = Utils::soliditySha3(j2, e2);
  std::string h3 = Utils::soliditySha3(j3, e3);
  std::string h4A = Utils::soliditySha3(j4A, e4A);
  std::string h4B = Utils::soliditySha3(j4B, e4B);
  std::string h4C = Utils::soliditySha3(j4C, e4C);
  std::string h5A = Utils::soliditySha3(j5A, e5A);
  std::string h5B = Utils::soliditySha3(j5B, e5B);
  std::string h6 = Utils::soliditySha3(j6, e6);
  std::string h7 = Utils::soliditySha3(j7, e7);

  bool errOk = true;
  if (e1A.getCode() != 0) { errOk = false; h1A = e1A.what(); }
  if (e1B.getCode() != 0) { errOk = false; h1B = e1B.what(); }
  if (e1C.getCode() != 0) { errOk = false; h1C = e1C.what(); }
  if (e2.getCode() != 0) { errOk = false; h2 = e2.what(); }
  if (e3.getCode() != 0) { errOk = false; h3 = e3.what(); }
  if (e4A.getCode() != 0) { errOk = false; h4A = e4A.what(); }
  if (e4B.getCode() != 0) { errOk = false; h4B = e4B.what(); }
  if (e4C.getCode() != 0) { errOk = false; h4C = e4C.what(); }
  if (e5A.getCode() != 0) { errOk = false; h5A = e5A.what(); }
  if (e5B.getCode() != 0) { errOk = false; h5B = e5B.what(); }
  if (e6.getCode() != 0) { errOk = false; h6 = e6.what(); }
  if (e7.getCode() != 0) { errOk = false; h7 = e7.what(); }
  if (!errOk) {
    failed("Couldn't hash one or more inputs");
  } else {
    int passTotal = 12;
    int passCount = (
      (h1A == r1) + (h1B == r1) + (h1C == r1) + (h2 == r2) + (h3 == r3) +
      (h4A == r4) + (h4B == r4) + (h4C == r4) + (h5A == r5) + (h5B == r5) +
      (h6 == r6) + (h7 == r7)
    );
    if (passCount != passTotal) {
      failed("One or more hashes don't match");
    } else {
      passed();
    }
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* 1A: " << j1A << std::endl
    << "* 1B: " << j1B << std::endl
    << "* 1C: " << j1C << std::endl
    << "* 2: " << j2 << std::endl
    << "* 3: " << j3 << std::endl
    << "* 4A: " << j4A << std::endl
    << "* 4B: " << j4B << std::endl
    << "* 4C: " << j4C << std::endl
    << "* 5A: " << j5A << std::endl
    << "* 5B: " << j5B << std::endl
    << "* 6: " << j6 << std::endl
    << "* 7: " << j7 << std::endl
    << "* Expected outputs:" << std::endl
    << "* 1A: " << r1 << std::endl
    << "* 1B: " << r1 << std::endl
    << "* 1C: " << r1 << std::endl
    << "* 2: " << r2 << std::endl
    << "* 3: " << r3 << std::endl
    << "* 4A: " << r4 << std::endl
    << "* 4B: " << r4 << std::endl
    << "* 4C: " << r4 << std::endl
    << "* 5A: " << r5 << std::endl
    << "* 5B: " << r5 << std::endl
    << "* 6: " << r6 << std::endl
    << "* 7: " << r7 << std::endl
    << "* Actual outputs:" << std::endl
    << "* 1A: " << h1A << std::endl
    << "* 1B: " << h1B << std::endl
    << "* 1C: " << h1C << std::endl
    << "* 2: " << h2 << std::endl
    << "* 3: " << h3 << std::endl
    << "* 4A: " << h4A << std::endl
    << "* 4B: " << h4B << std::endl
    << "* 4C: " << h4C << std::endl
    << "* 5A: " << h5A << std::endl
    << "* 5B: " << h5B << std::endl
    << "* 6: " << h6 << std::endl
    << "* 7: " << h7 << std::endl
    << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason)
  << std::endl << std::endl;
}
*/

void Tests::testSoliditySHA3Raw() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  json jZero = {
    { {"type", "string"}, {"value", ""} }
  };
  std::string zeroHash = "0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470";

  Error e;
  std::string zeroHashRes = Utils::soliditySha3Raw(jZero, e);
  if (e.getCode() != 0) {
    zeroHashRes = e.what();
    failed("Couldn't hash input");
  } else if (zeroHashRes != zeroHash) {
    failed("Zero hash doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Input: (empty)" << std::endl
    << "* Expected output: " << zeroHash << std::endl
    << "* Actual output: " << zeroHashRes << std::endl
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
  std::string utf8Hex = "0xe6bca2e5ad97";
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

  std::string utf8Exp = "0xe6bca2e5ad97";
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

