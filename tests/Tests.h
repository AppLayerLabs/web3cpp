#ifndef TESTS_H
#define TESTS_H

#include <chrono>
#include <ctime>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include <web3cpp/Web3.h>
#include <web3cpp/Contract.h>
#include <web3cpp/Utils.h>

// Helper class for unit testing.

class Tests {
  private:
    // Password for the wallet and log stream object for writing test processes
    std::string password;
    std::ofstream logStream;

    // Log file name and test wallet folder name
    std::string logFile = "web3cpp-test-log.txt";
    std::string walletFolder = "web3cpp-test-wallet";

    // Counters for passed/failed tests, error reason (if any),
    // and functions for controlling those
    int pass = 0;
    int fail = 0;
    bool curPass;
    std::string curReason;
    void passed() { curPass = true; curReason = ""; pass++; }
    void failed(std::string msg) { curPass = false; curReason = msg; fail++; }

  public:
    // The Web3 object that will be tested
    Web3* web3 = NULL;

    // Constructor
    Tests(std::string &password) {
      std::cout << "Creating/Loading wallet..." << std::endl;
      web3 = new Web3(walletFolder);
      Error error;
      if (!web3->wallet.loadWallet(password, error)) {
        std::cout << std::string("Could not load wallet: ") << error.what() << std::endl;
        throw;
      }
      this->password = password;
      logStream.open(logFile, std::ofstream::out | std::ofstream::app);
      std::time_t t = std::time(nullptr);
      std::tm* tm = std::localtime(&t);
      std::stringstream timestream;
      timestream << std::put_time(tm, "%d-%m-%Y %H-%M-%S");
      logStream << "[web3cpp Test Log - " << timestream.str() << "]\n\n";
      std::cout << "Wallet created/loaded, starting tests" << std::endl;
    }

    // Destructor
    ~Tests() { logStream.close(); delete web3; }

    // Show overall test results
    void showResults() {
      std::cout <<
        "* RESULTS: " << pass << " passed, " << fail << " failed" << std::endl
        << "* Check the log file " << logFile << " for details"
      << std::endl;
    }

    /**
     * Tests start here, divided by class.
     * Each test should be completely contained into a function of its own
     * and output the details to the log file.
     */

    // ==============================================================
    // UTILS
    // ==============================================================

    // Test for random hex string generation at preset sizes.
    void testRandomHexes();

    // Test for parsing of large numbers as BigNumbers.
    void testBigNumbers();

    // Test for SHA3 hashing (normal and raw).
    void testSHA3();

    // Test for Solidity SHA3 hashing (normal only).
    void testSoliditySHA3();

    // Test for Solidity SHA3 hashing (raw only).
    void testSoliditySHA3Raw();

    // Test for hex string detection.
    void testHexes();

    // Test for address detection.
    void testAddresses();

    // Test for address conversions (upper, lower and checksum).
    void testAddressConversions();

    // Test for address checksums.
    void testAddressChecksums();

    // Test for hex data conversions (number, number string and text string).
    void testHexConversions();

    // Test for hex prefix stripping.
    void testHexStripping();

    // Test for type conversions, from HEX to something.
    void testFromHexToTypes();

    // Test for type conversions, from something to HEX.
    void testFromTypesToHex();

    // Test for currency conversions, from Wei to fixed point and vice-versa.
    void testWeiConversions();

    // Test for HEX string padding (left and right).
    void testHexPadding();

    // ==============================================================
    // WALLET
    // ==============================================================

    /**
     * Test for valid Account generation.
     * Created test Account is automatically deleted at the end of the test.
     */
    void generateAccount();

    // ==============================================================
    // CONTRACT
    // ==============================================================

    /**
     * Test for proper Contract loading and ABI manipulation.
     * Inputs: none
     */
    void loadAndTestContract();
};

#endif // TESTS_H
