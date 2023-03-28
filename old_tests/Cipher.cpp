#include "Tests.h"

void Tests::testCipherNormal() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string pass = "password";
  std::string msg1 = "Test Message";
  std::string msg2 = "0x1234567890123456789012345678901234567890";
  std::string msg3 = "{\"object\": \"testJson\", \"params\": [\"aaa\", \"bbb\", \"ccc\"]}";
  Error e1, e2, e3, e4, e5, e6;
  std::string enc1 = Cipher::encrypt(msg1, pass, e1);
  std::string dec1 = Cipher::decrypt(enc1, pass, e2);
  std::string enc2 = Cipher::encrypt(msg2, pass, e3);
  std::string dec2 = Cipher::decrypt(enc2, pass, e4);
  std::string enc3 = Cipher::encrypt(msg3, pass, e5);
  std::string dec3 = Cipher::decrypt(enc3, pass, e6);

  if (
    e1.getCode() != 0 || e2.getCode() != 0 || e3.getCode() != 0 ||
    e4.getCode() != 0 || e5.getCode() != 0 || e6.getCode() != 0
  ) {
    failed("One or more steps returned an error");
  } else if (dec1 != msg1 || dec2 != msg2 || dec3 != msg3) {
    failed("One or more decodes don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Password: " << pass << std::endl
    << "* Message 1: " << msg1 << std::endl
    << "* Message 2: " << msg2 << std::endl
    << "* Message 3: " << msg3 << std::endl
    << "* Encode 1: " << enc1 << std::endl
    << "* Encode 2: " << enc2 << std::endl
    << "* Encode 3: " << enc3 << std::endl
    << "* Decode 1: " << dec1 << std::endl
    << "* Decode 2: " << dec2 << std::endl
    << "* Decode 3: " << dec3 << std::endl
    << "* Encode 1 errors: " << e1.what() << std::endl
    << "* Decode 1 errors: " << e2.what() << std::endl
    << "* Encode 2 errors: " << e3.what() << std::endl
    << "* Decode 2 errors: " << e4.what() << std::endl
    << "* Encode 3 errors: " << e5.what() << std::endl
    << "* Decode 3 errors: " << e6.what() << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testCipherLegacy() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string pass = "password";
  std::string msg1 = "Test Message";
  std::string msg2 = "0x1234567890123456789012345678901234567890";
  std::string msg3 = "{\"object\": \"testJson\", \"params\": [\"aaa\", \"bbb\", \"ccc\"]}";
  std::string enc1 = Cipher::encryptAleth(msg1, pass);
  std::string dec1 = Cipher::decryptAleth(enc1, pass);
  std::string enc2 = Cipher::encryptAleth(msg2, pass);
  std::string dec2 = Cipher::decryptAleth(enc2, pass);
  std::string enc3 = Cipher::encryptAleth(msg3, pass);
  std::string dec3 = Cipher::decryptAleth(enc3, pass);

  if (dec1.empty() || dec2.empty() || dec3.empty()) {
    failed("One or more decryptions failed (empty return)");
  } else if (dec1 != msg1 || dec2 != msg2 || dec3 != msg3) {
    failed("One or more decodes don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Password: " << pass << std::endl
    << "* Message 1: " << msg1 << std::endl
    << "* Message 2: " << msg2 << std::endl
    << "* Message 3: " << msg3 << std::endl
    << "* Encode 1: " << enc1 << std::endl
    << "* Encode 2: " << enc2 << std::endl
    << "* Encode 3: " << enc3 << std::endl
    << "* Decode 1: " << ((!dec1.empty()) ? dec1 : "(empty)") << std::endl
    << "* Decode 2: " << ((!dec2.empty()) ? dec2 : "(empty)") << std::endl
    << "* Decode 3: " << ((!dec3.empty()) ? dec3 : "(empty)") << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

