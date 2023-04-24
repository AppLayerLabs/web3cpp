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
    for (int i = 0; i <= secs - 1; i++) { // We cannot sleep more than the storePassword sleep.
      if (!web3->wallet.isPasswordStored()) {
        inMemory = false;
        break;
      }
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Ensure we wait 3 seconds.
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

void Tests::testTransaction() {
  std::cout << "* Running test: " << __func__ << "... " << std::flush;
  if (web3->wallet.getAccounts().size() == 0) {
    failed("No account found, please create one manually");
    std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
    logStream << "* Test: " << __func__ << std::endl
      << "* Test result: " <<
        ((curPass) ? "PASSED" : "FAILED - " + curReason)
      << std::endl << std::endl;
    return;
  }

  std::string acc = web3->wallet.getAccounts().at(0);
  const std::unique_ptr<Account> &a = web3->wallet.getAccountDetails(acc);

  // This is dependent on the wallet having balance.
  // BigNumber bal = a.balance().get();
  // if (bal < Utils::toBN(Utils::toWei("0.01", 18))) {
  //   failed("No account found with bal, please create one manually");
  //   std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  //   logStream << "* Test: " << __func__ << std::endl
  //     << "* Address: " << acc << std::endl
  //     << "* Balance (Wei): " << bal << std::endl
  //     << "* Balance (Fix): " << Utils::fromWei(bal, 18) << std::endl
  //     << "* Test result: " <<
  //       ((curPass) ? "PASSED" : "FAILED - " + curReason)
  //     << std::endl << std::endl;
  //   return;
  // }

  if (a) {
    Error buildErr, signErr, sendErr;
    std::string signedTx = "";
    json txRes;
    std::string from = a->address();
    std::string to = a->address();
    BigNumber value(Utils::toWei("0.0001", 18));
    BigNumber gasLimit = 21000; // 21000 wei
    BigNumber gasPrice = 40000000000;  // 40 gwei = 40000000000 wei
    std::string dataHex = "";
    int nonce = a->nonce();
    dev::eth::TransactionSkeleton txSkel = web3->wallet.buildTransaction(
      from, to, value, gasLimit, gasPrice, dataHex, nonce, buildErr
    );
    if (buildErr.getCode() != 0) {
      failed("Error on transaction build - " + buildErr.what());
    }
  
    if (buildErr.getCode() == 0) {
      signedTx = web3->wallet.signTransaction(txSkel, password, signErr);
      if (signErr.getCode() != 0) {
        failed("Error on transaction sign - " + signErr.what());
      }
    }
  
    // This is dependent on the wallet having balance.
    //  if (buildErr.getCode() == 0 && signErr.getCode() == 0) {
    //    txRes = web3->wallet.sendTransaction(signedTx, sendErr).get();
    //    if (sendErr.getCode() != 0) {
    //      failed("Error on transaction send - " + sendErr.what());
    //    }
    //  }
    //
    //  if (buildErr.getCode() == 0 && signErr.getCode() == 0 && sendErr.getCode() == 0) {
    //    if (!a.saveTxToHistory(signedTx)) {
    //      failed("Transaction was made but not stored in history");
    //    } else {
    //      passed();
    //    }
    //  }
  
    passed();
    logStream << "* Test: " << __func__ << std::endl
      << "* Address: " << acc << std::endl
      // This is dependent on the wallet having balance.
      // << "* Balance (Wei): " << bal << std::endl
      // << "* Balance (Fix): " << Utils::fromWei(bal, 18) << std::endl
      << "* From: " << dev::toString(txSkel.from) << std::endl
      << "* To: " << dev::toString(txSkel.to) << std::endl
      << "* Value (Wei): " << txSkel.value << std::endl
      << "* Data: " << dev::toHex(txSkel.data) << std::endl
      << "* Chain ID: " << txSkel.chainId << std::endl
      << "* Nonce: " << txSkel.nonce << std::endl
      << "* Gas (Wei): " << txSkel.gas << std::endl
      << "* Gas Price (Wei): " << txSkel.gasPrice << std::endl
      << "* Signed tx: " << signedTx << std::endl
      << "* Tx response: " << txRes.dump() << std::endl
      << "* Build error: " << buildErr.what() << std::endl
      << "* Sign error: " << signErr.what() << std::endl
      << "* Send error: " << sendErr.what() << std::endl
      << "* Test result: " <<
        ((curPass) ? "PASSED" : "FAILED - " + curReason)
      << std::endl << std::endl;
  } else {
    failed("Error on getting account for transaction - account is nullptr (doesn't exist)");
    logStream << "* Test: " << __func__ << std::endl
      << "* Test result: " << ((curPass) ? "PASSED" : "FAILED - " + curReason) << std::endl << std::endl;
  }

  std::cout << ((curPass) ? "PASSED" : "FAILED") << std::endl;
  return;
}

