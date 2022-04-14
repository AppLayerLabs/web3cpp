#ifndef TESTS_H
#define TESTS_H

#include <future>
#include <iostream>
#include <string>
#include <vector>

#include <web3cpp/Web3.h>

// Helper class for unit testing.

class Tests {
  private:
    // The Web3 object that will be tested
    Web3 web3;

    // Counters for passed/failed tests, and functions for controlling those
    int pass = 0;
    int fail = 0;
    void passed() {
      std::cout << "PASSED" << std::endl;
      pass++;
    }
    void failed(std::string msg) {
      std::cout << "FAILED - " << msg << std::endl;
      fail++;
    }

  public:
    // Show overall test results
    void showResults() {
      std::cout << "----------------------------------------\n"
        << "RESULTS: " << pass << " passed, " << fail << " failed\n"
        << "----------------------------------------\n";
    }

    // The tests themselves - each test is a function of its own
    void generateAccount(std::string derivPath, std::string &password) {
      std::cout << "* Running test: " << __func__ << std::endl;
      if (!web3.wallet.createNewAccount(derivPath, password)) {
        failed("Could not create account"); return;
      }
      std::string acc = web3.wallet.getAccounts().get().at(0);
      if (!Utils::isAddress(acc)) {
        failed("Account is invalid");
        std::cout << "Output: " << acc << std::endl;
        return;
      }
      passed(); return;
    }
};

#endif // TESTS_H
