#ifndef WALLET_H
#define WALLET_H

#include <chrono>
#include <ctime>
#include <future>
#include <string>
#include <thread>
#include <vector>

#include "Utils.h"

#include <boost/algorithm/string/replace.hpp>
#include <nlohmann/json.hpp>

#include <bip3x/crypto/sha3.h>  // keccak_256()
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include <web3cpp/devcore/Common.h>
#include <web3cpp/devcore/FixedHash.h>
#include <web3cpp/devcore/Address.h>
#include <web3cpp/ethcore/KeyManager.h>
#include <web3cpp/ethcore/TransactionBase.h>
#include <web3cpp/Error.h>
#include <web3cpp/Account.h>
#include <web3cpp/DB.h>
#include <web3cpp/Cipher.h>
#include <web3cpp/Bip39.h>

using json = nlohmann::json;

// Module that interacts with a node's accounts.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth-personal.html

class Wallet {
  private:
    // Wallet password hash, salt and number of PBKDF2 iterations.
    dev::bytesSec passHash;
    dev::h256 passSalt;
    int passIterations = 100000;

    // Wallet path, provider, Account list and internal databases.
    boost::filesystem::path path;
    Utils::Provider* provider;
    Database infoDB;
    Database accountDB;
    Database transactionDB;

    // Variables for Wallet load status and password storage.
    bool _isLoaded;
    std::string _password;
    std::time_t _passEnd;
    std::thread _passThread;

    // Paths for wallet file and secrets folder.
    boost::filesystem::path walletExistsPath()   { return path.string() + "/wallet.info"; };
    boost::filesystem::path walletFolder()       { return path.string() + "/wallet"; };
    boost::filesystem::path accountsFolder()     { return path.string() + "/wallet/accounts"; };
    boost::filesystem::path seedPhraseFile()     { return path.string() + "/wallet/seed"; };
    boost::filesystem::path transactionsFolder() { return path.string() + "/wallet/transactions"; }

    // Thread function for storing/clearing password in memory
    void passHandler() {
      while (true) {
        std::time_t now = std::time(nullptr);
        if (now > this->_passEnd) break;
        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
      this->_password = "";
      this->_passEnd = 0;
      return;
    }

    // Called by loadWallet() if no wallet is found on the desired path.
    bool createNewWallet(std::string const &password, Error &error);

  public:
    // Constructor.
    Wallet(Utils::Provider *providerPointer, boost::filesystem::path _path)
      : provider(providerPointer), path(_path),
        infoDB("walletInfo", walletFolder()),
        accountDB("accounts", walletFolder()),
        transactionDB("transactions", walletFolder())
    {};

    /**
     * Load a wallet with the given password.
     * Creates a new wallet if no wallet exists.
     * Returns true on success, false if password is incorrect..
     */
    bool loadWallet(std::string const &password, Error &error);

    // Check if a wallet is loaded.
    bool isLoaded() { return this->_isLoaded; }

    // Check if the given password matches with the wallet's.
    bool checkPassword(std::string &password);

    // Check if the wallet file and its keys folder exists in a given path.
    static bool walletExists(boost::filesystem::path &wallet_path);

    /**
     * Creates a new account.
     * Always stores address as lowercase.
     * If no custom seed is passed, uses BIP39 seed stored in json file.
     * Returns the checksum address of the newly created Account,
     * or an empty string on failure.
     */
    std::string createAccount(
      std::string derivPath, std::string &password, std::string name,
      Error &error, std::string seed = ""
    );

    // Import a given private key into the wallet database.
    // Always stores address as lowercase.
    bool importPrivKey(
      dev::Secret const &secret, std::string const &password,
      std::string const &name, std::string const &derivationPath, Error &error
    );

    /**
     * Deletes a previously created account.
     * Converts input to lowercase before attempting to delete.
     * Returns true on success, false on failure.
     */
    bool deleteAccount(std::string address);

    /**
     * Build a transaction from user data.
     * Coin transactions would have a blank dataHex and the "to" address
     * being the destination address.
     * Token transactions would have a filled dataHex, 0 txValue and
     * the "to" address being the token contract's address.
     * "creation" sets whether the transaction is creating a contract.
     * Returns a skeleton filled with data for the transaction, which has to be signed.
     */
    dev::eth::TransactionSkeleton buildTransaction(
      std::string from, std::string to, BigNumber value,
      BigNumber gasLimit, BigNumber gasPrice, std::string dataHex,
      int nonce, bool creation = false
    );

    /**
     * Signs a data string as an "Ethereum Signed Message" (EIP-712 compliant).
     * Usable with ecRecover().
     * dataToSign will be converted to hex using Utils.utf8ToHex().
     */
    std::string sign(
      std::string dataToSign, std::string address, std::string password, Error &err
    );

    /**
     * Recovers the account that signed the given data.
     * Signature is the one returned from sign().
     * dataThatWasSigned will be converted to hex using Utils.utf8ToHex().
     */
    std::string ecRecover(
      std::string dataThatWasSigned, std::string signature
    );

    /**
     * Signs a transaction. "from" account needs to be unlocked.
     * Returns the signed transaction hash, or an empty string on failure.
     */
    std::string signTransaction(
      dev::eth::TransactionSkeleton txObj, std::string password, Error &err
    );

    /**
     * Sends a transaction over the management API.
     * Returns a JSON containing the send results, or an empty JSON
     * object on failure.
     */
    std::future<json> sendTransaction(
      std::string txHash, std::string password, Error &err
    );

    // Stores/wipes the Wallet's password to/from memory, respectively.
    // 0 seconds = "store indefinitely until wiped manually".
    // Any non-zero value will spawn a thread that wipes automatically
    // after said value counts down to 0.
    void storePassword(std::string password, unsigned int seconds = 0);
    void clearPassword();

    // Returns a list of addresses controlled by the node, and the
    // details for a specific address, respectively.
    // If no address is found, getAccountDetails() returns an empty
    // Account object.
    std::vector<std::string> getAccounts();
    Account getAccountDetails(std::string address);
};

#endif  // WALLET_H
