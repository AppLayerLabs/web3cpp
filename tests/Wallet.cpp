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

void Tests::handlePasswordCheck() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  std::string p1 = this->password;
  std::string p2 = this->password + "123";
  std::string p3 = "321" + this->password;
  bool e1 = true, e2 = false, e3 = false;

  bool c1 = web3->wallet.checkPassword(p1);
  bool c2 = web3->wallet.checkPassword(p2);
  bool c3 = web3->wallet.checkPassword(p3);
  if (c1 != e1 || c2 != e2 || c3 != e3) {
    failed("One or more outputs don't match");
  } else {
    passed();
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Inputs: " << std::endl
    << "* 1: " << p1 << " (" << e1 << ")" << std::endl
    << "* 2: " << p2 << " (" << e2 << ")" << std::endl
    << "* 3: " << p3 << " (" << e3 << ")" << std::endl
    << "* Outputs: " << std::endl
    << "* 1: " << c1 << std::endl
    << "* 2: " << c2 << std::endl
    << "* 3: " << c3 << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::handlePasswordStorageAuto() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  int secs = 3;

  web3->wallet.storePassword(this->password, secs);
  if (!web3->wallet.isPasswordStored()) {
    failed("Could not store password into memory");
  } else {
    bool inMemory = true;
    for (int i = 0; i <= secs; i++) {
      if (!web3->wallet.isPasswordStored()) {
        inMemory = false;
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (!inMemory) {
      failed("Password cleared from memory before timeout");
    } else if (web3->wallet.isPasswordStored()) {
      failed("Password not cleared from memory after timeout");
    } else {
      passed();
    }
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Time stored in memory (seconds): " << secs << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::handlePasswordStorageManual() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  int secs = 3;
  int cutoff = 2;

  web3->wallet.storePassword(this->password, 0);
  if (!web3->wallet.isPasswordStored()) {
    failed("Could not store password into memory");
  } else {
    bool inMemory = true;
    for (int i = 0; i <= secs; i++) {
      if (i == cutoff) web3->wallet.clearPassword();
      if (i < cutoff && !web3->wallet.isPasswordStored()) {
        inMemory = false;
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (!inMemory) {
      failed("Password cleared from memory before cutoff");
    } else if (web3->wallet.isPasswordStored()) {
      failed("Password not cleared from memory after cutoff");
    } else {
      passed();
    }
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Time stored in memory (seconds): " << secs << std::endl
    << "* Time for manual cutoff (seconds): " << cutoff << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

void Tests::signAndRecoverMessage() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  if (web3->wallet.getAccounts().size() == 0) {
    Error error;
    std::string result = Utils::toLowercaseAddress(web3->wallet.createAccount(
      "m/44'/60'/0'/0/0", this->password, "testAccount", error,
      "corn girl crouch desk duck save hedgehog choose kitchen unveil dragon space"
    ));
    if (result == "0x") {
      failed(error.what());
      std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
      logStream << "* Test: " << __func__ << std::endl
        << "* Aborted due to failed Account generation" << std::endl
        << "* Test result: " <<
          ((curPass) ? "PASSED" : "FAILED - " + curReason)
        << std::endl << std::endl;
      return;
    }
  }

  std::string msg, sig, acc, rec = "";
  msg = "Test Message";
  acc = web3->wallet.getAccounts().at(0);
  sig = web3->wallet.sign(msg, acc, this->password);
  if (!Utils::isHexStrict(sig)) {
    failed("Signing failed");
  } else {
    rec = web3->wallet.ecRecover(msg, sig);
    if (rec != acc) {
      failed("Recovered address doesn't match the one used for signing");
    } else {
      passed();
    }
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  logStream << "* Test: " << __func__ << std::endl
    << "* Message: " << msg << std::endl
    << "* Used address: " << acc << std::endl
    << "* Signature: " << sig << std::endl
    << "* Recovered address: " << rec << std::endl
    << "* Test result: " <<
      ((curPass) ? "PASSED" : "FAILED - " + curReason)
    << std::endl << std::endl;
  return;
}

