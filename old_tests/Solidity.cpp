#include "Tests.h"

void Tests::testTypeChecks(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;

  std::vector<std::pair<std::string, json>> testData = {
    {"function", "func(uint256,address,bool,bytes,string,uint256[],address[],bool[],bytes[],string[])"},
    {"uint256", "234"},
    {"address", "0x1234567890abcdefedcb1234567890abcdefedbc"},
    {"bool", "false"},
    {"bytes", "0xaaaaa"},
    {"string", "Hello!%"},
    {"uint256[]", {"123", "456", "789", "9852394853798"}},
    {"address[]", {"0x1234567890123456789012345678901234567890", "0xacbefabecfabefcbaefcbabcafbaebfabcfaebfe"}},
    {"bool[]", {"0", "1", "1", "0", "1", "0", "0", "1", "1", "1"}},
    {"bytes[]", {"0xaaa", "0xbbbbb", "0xcccccccc"}},
    {"string[]", {"Text1", "Text2", "Text3"}}
  };
  std::vector<std::pair<bool, std::string>> testRes;
  bool allOk = true;

  for (std::pair<std::string, json> t : testData) {
    Error e;
    bool res = Solidity::checkType(t.first, t.second, e);
    testRes.push_back(std::make_pair(res, e.what()));
    if (!res && allOk) allOk = false;
  }
  if (!allOk) {
    failed("One or more types are invalid");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl;
  for (std::pair<std::string, json> t : testData) {
    logStream << "* " << t.first << " - " << t.second << std::endl;
  }
  logStream << "* Outputs: " << std::endl;
  for (std::pair<bool, std::string> t : testRes) {
    logStream << "* " << t.first << " - " << t.second << std::endl;
  }
  logStream << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testFunction(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string funcHdr = "func(uint256,address,bool,bytes,string,uint256[],address[],bool[],bytes[],string[])";
  std::string funcIDExp = "f80dfe6b";
  std::string funcIDRes = Solidity::packFunction(funcHdr);

  if (funcIDRes != funcIDExp) {
    failed("Function hash doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Function header: " << funcHdr << std::endl
    << "* Expected output: " << funcIDExp << std::endl
    << "* Actual output: " << funcIDRes << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testUint(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string data = "234";
  std::string exp = "00000000000000000000000000000000000000000000000000000000000000ea";
  std::string res = Solidity::packUint(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testAddress(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string data = "0x1234567890abcdefedcb1234567890abcdefedbc";
  std::string exp = "0000000000000000000000001234567890abcdefedcb1234567890abcdefedbc";
  std::string res = Solidity::packAddress(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testBool(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string data1 = "0", data2 = "1", data3 = "false", data4 = "true";
  std::string exp1 = "0000000000000000000000000000000000000000000000000000000000000000",
    exp2 = "0000000000000000000000000000000000000000000000000000000000000001",
    exp3 = "0000000000000000000000000000000000000000000000000000000000000000",
    exp4 = "0000000000000000000000000000000000000000000000000000000000000001";
  std::string res1 = Solidity::packBool(data1),
    res2 = Solidity::packBool(data2),
    res3 = Solidity::packBool(data3),
    res4 = Solidity::packBool(data4);

  if (res1 != exp1 || res2 != exp2 || res3 != exp3 || res4 != exp4) {
    failed("One or more packings don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << std::endl
    << "* 1: " << data1 << std::endl
    << "* 2: " << data2 << std::endl
    << "* 3: " << data3 << std::endl
    << "* 4: " << data4 << std::endl
    << "* Expected output: " << std::endl
    << "* 1: " << exp1 << std::endl
    << "* 2: " << exp2 << std::endl
    << "* 3: " << exp3 << std::endl
    << "* 4: " << exp4 << std::endl
    << "* Actual output: " << std::endl
    << "* 1: " << res1 << std::endl
    << "* 2: " << res2 << std::endl
    << "* 3: " << res3 << std::endl
    << "* 4: " << res4 << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testBytes(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string data = "0xaaaaa";
  std::string exp = "00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000002aaaaa00000000000000000000000000000000000000000000000000000000000";
  std::string res = Solidity::packBytes(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testString(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string data = "Hello!%";
  std::string exp = "0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000748656c6c6f212500000000000000000000000000000000000000000000000000";
  std::string res = Solidity::packString(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testUintArray(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> data = {"123", "456", "789", "9852394853798"};
  std::string exp = "00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000007b00000000000000000000000000000000000000000000000000000000000001c80000000000000000000000000000000000000000000000000000000000000315000000000000000000000000000000000000000000000000000008f5f07ed5a6";
  std::string res = Solidity::packUintArray(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testAddressArray(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> data = {
    "0x1234567890123456789012345678901234567890",
    "0xacbefabecfabefcbaefcbabcafbaebfabcfaebfe"
  };
  std::string exp = "000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000020000000000000000000000001234567890123456789012345678901234567890000000000000000000000000acbefabecfabefcbaefcbabcafbaebfabcfaebfe";
  std::string res = Solidity::packAddressArray(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testBoolArray(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> data = {
    "0", "1", "1", "0", "1", "0", "0", "1", "1", "1"
  };
  std::string exp = "0000000000000000000000000000000000000000000000000000000000000020000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000001";
  std::string res = Solidity::packBoolArray(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testBytesArray(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> data = {
   "0xaaa", "0xbbbbb", "0xcccccccc"
  };
  std::string exp = "00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e000000000000000000000000000000000000000000000000000000000000000020aaa00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000030bbbbb00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004cccccccc00000000000000000000000000000000000000000000000000000000";
  std::string res = Solidity::packBytesArray(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testStringArray(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> data = {
    "Text1", "Text2", "Text3"
  };
  std::string exp = "00000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e0000000000000000000000000000000000000000000000000000000000000000554657874310000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005546578743200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000055465787433000000000000000000000000000000000000000000000000000000";
  std::string res = Solidity::packStringArray(data);

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << data << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testMulti(){
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  json testData = {
    { {"t", "uint256"}, {"v", "234"} },
    { {"t", "address"}, {"v", "0x1234567890abcdefedcb1234567890abcdefedbc"} },
    { {"t", "bool"}, {"v", "false"} },
    { {"t", "bytes"}, {"v", "0xaaaaa"} },
    { {"t", "string"}, {"v", "Hello!%"} },
    { {"t", "uint256[]"}, {"v", {
      "123", "456", "789", "9852394853798"
    } } },
    { {"t", "address[]"}, {"v", {
      "0x1234567890123456789012345678901234567890",
      "0xacbefabecfabefcbaefcbabcafbaebfabcfaebfe"
    } } },
    { {"t", "bool[]"}, {"v", {
      "0", "1", "1", "0", "1", "0", "0", "1", "1", "1"
    } } },
    { {"t", "bytes[]"}, {"v", {
      "0xaaa", "0xbbbbb", "0xcccccccc"
    } } },
    { {"t", "string[]"}, {"v", {
      "Text1", "Text2", "Text3"
    } } }
  };
  std::string exp = "0xf80dfe6b00000000000000000000000000000000000000000000000000000000000000ea0000000000000000000000001234567890abcdefedcb1234567890abcdefedbc00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000140000000000000000000000000000000000000000000000000000000000000018000000000000000000000000000000000000000000000000000000000000001c0000000000000000000000000000000000000000000000000000000000000024000000000000000000000000000000000000000000000000000000000000002c0000000000000000000000000000000000000000000000000000000000000034000000000000000000000000000000000000000000000000000000000000004800000000000000000000000000000000000000000000000000000000000000002aaaaa00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000748656c6c6f2125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004000000000000000000000000000000000000000000000000000000000000007b00000000000000000000000000000000000000000000000000000000000001c80000000000000000000000000000000000000000000000000000000000000315000000000000000000000000000000000000000000000000000008f5f07ed5a600000000000000000000000000000000000000000000000000000000000000020000000000000000000000001234567890123456789012345678901234567890000000000000000000000000acbefabecfabefcbaefcbabcafbaebfabcfaebfe000000000000000000000000000000000000000000000000000000000000000a00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001000000000000000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e000000000000000000000000000000000000000000000000000000000000000020aaa00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000030bbbbb00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000004cccccccc000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a000000000000000000000000000000000000000000000000000000000000000e0000000000000000000000000000000000000000000000000000000000000000554657874310000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005546578743200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000055465787433000000000000000000000000000000000000000000000000000000";
  Error e;
  std::string res = Solidity::packMulti(testData, e,
    "func(uint256,address,bool,bytes,string,uint256[],address[],bool[],bytes[],string[])"
  );

  if (res != exp) {
    failed("Packing doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Data: " << testData << std::endl
    << "* Expected output: " << exp << std::endl
    << "* Actual output: " << res << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

