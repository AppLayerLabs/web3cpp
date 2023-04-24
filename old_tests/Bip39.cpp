#include "Tests.h"

void Tests::testMnemonicGeneration() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  bip3x::Bip39Mnemonic::MnemonicResult seed = BIP39::createNewMnemonic();
  std::string seedStatus;

  if (seed.status == bip3x::Bip39Mnemonic::MnemonicStatus::Ok) {
    seedStatus = "Ok";
  } else if (seed.status == bip3x::Bip39Mnemonic::MnemonicStatus::UnsupportedEntropy) {
    seedStatus = "UnsupportedEntropy";
  } else if (seed.status == bip3x::Bip39Mnemonic::MnemonicStatus::UnknownError) {
    seedStatus = "UnknownError";
  }
  if (seed.status != bip3x::Bip39Mnemonic::MnemonicStatus::Ok) {
    failed("Error on mnemonic status");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Mnemonic status: " << seedStatus << std::endl
    << "* Mnemonic length: " << seed.len << std::endl
    << "* Mnemonic (raw): " << seed.raw << std::endl
    << "* Mnemonic (words): " << seed.words << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testKeyPairGeneration() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string phrase = "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space";
  std::string derivPath = "m/44'/60'/0'/0/0";
  std::string expAddr = "0x3e8467983ba80734654208b274ebf01264526117";
  std::string addr = "";

  bip3x::HDKey key = BIP39::createKey(phrase, derivPath);
  dev::KeyPair k(dev::Secret::frombip3x(key.privateKey));
  addr += "0x" + k.address().hex();
  if (addr != expAddr) {
    failed("Resulting address doesn't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Phrase: " << phrase << std::endl
    << "* Derivation path: " << derivPath << std::endl
    << "* Expected address: " << expAddr << std::endl
    << "* Resulting address: " << addr << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testMnemonicWordValidity() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::vector<std::string> words = {
    "absent", "absont", "benefit", "benfitr", "charge", "chager", "desk", "dkse", "empty", "emptor",
    "faint", "fatin", "gauge", "gague", "hollow", "hallow", "index", "ixden", "joke", "jeko",
    "key", "koy", "latin", "litna", "merry", "marry", "normal", "nermal", "online", "offline",
    "pet", "pit", "quiz", "quip", "rail", "ruin", "share", "shore", "tone", "toon",
    "unit", "uint", "vote", "veto", "wheel", "whele", "you", "yee", "zero", "zore"
  };
  std::vector<bool> checks = {
    true, false, true, false, true, false, true, false, true, false,
    true, false, true, false, true, false, true, false, true, false,
    true, false, true, false, true, false, true, false, true, false,
    true, false, true, false, true, false, true, false, true, false,
    true, false, true, false, true, false, true, false, true, false
  };
  std::vector<bool> answers = {};

  for (std::string word : words) {
    answers.push_back(BIP39::wordExists(word));
  }
  if (answers != checks) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Words: " << words << std::endl
    << "* Expected output: " << checks << std::endl
    << "* Actual output: " << answers << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::testMnemonicAddressGeneration() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string seed = "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space";
  std::string derivPath = "m/44'/60'/0'/0/";
  std::vector<std::string> expected = { // Indexes 0-9
    "0x3e8467983ba80734654208b274ebf01264526117",
    "0xad569723983ec2dd7ff147d2379d79384db04b5f",
    "0x239003b32adf77c81854c02eb72c450e259f2673",
    "0x93a84e1ffb77070850eadc0f0a4e864f9dadb4ef",
    "0x87c100e2d03fe0cc1f7dc296ad8e480a71c857d3",
    "0xb82af14bda8474289834cbfc1836833ef1a89c70",
    "0x1fa3d77eec4787b5e6c6cc3ffc880314413c74aa",
    "0x512fc214161349dedabd2861cfd5e94c01ee7e84",
    "0xa43b590b073962ef6a98816c788fb94d81722dca",
    "0xcb16f1d7221eaf229cf7a385c1e9993160decce7"
  };
  std::vector<std::string> results = {};

  std::vector<std::pair<std::string,std::string>> addresses;
  addresses = BIP39::generateAccountsFromSeed(seed, derivPath, 0);
  for (std::pair<std::string, std::string> add : addresses) {
    results.push_back(add.first);
  }
  bool allOk = true;
  for (int i = 0; i < expected.size(); i++) {
    if (results[i] != expected[i]) { allOk = false; break; }
  }
  if (!allOk) {
    failed("One or more results don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Seed: " << seed << std::endl
    << "* Derivation path: " << derivPath << std::endl
    << "* Expected outputs: " << std::endl;
    for (int i = 0; i < expected.size(); i++) {
      logStream << "* " << i << " - " << expected[i] << std::endl;
    }
    logStream << "* Actual outputs: " << std::endl;
    for (int i = 0; i < results.size(); i++) {
      logStream << "* " << i << " - " << results[i] << std::endl;
    }
    logStream << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

