#ifndef WALLET_H
#define WALLET_H

#include <future>
#include <string>
#include <vector>

#include "Utils.h"

#include <boost/algorithm/string/replace.hpp>
#include <nlohmann/json.hpp>

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

// TODO:
// - Inherit the umbrella class to get access to common functions
//   - setProvider(), providers, givenProvider, currentProvider, BatchRequest(), extend()
// - Decide how to deal with callbacks on (almost) all functions

class Wallet {
  private:
    // Wallet password hash, salt and number of PBKDF2 iterations.
    dev::bytesSec passHash;
    dev::h256 passSalt;
    int passIterations = 100000;

    // Wallet path, provider and key manager,
    boost::filesystem::path path;
    Utils::Provider* provider;

    bool _isLoaded;

    // Account database and list.
    Database walletDB;
    std::vector<Account> accounts;

    // Paths for wallet file and secrets folder.
    boost::filesystem::path walletExistsPath() { return path.string() + "/wallet.info"; };
    boost::filesystem::path walletFolder()     { return path.string() + "/wallet"; };
    boost::filesystem::path accountsFolder()   { return path.string() + "/wallet/accounts"; };
    boost::filesystem::path seedPhraseFile()   { return path.string() + "/wallet/seed"; };

    // Called by loadWallet() if no wallet is found on the desired path.
    bool createNewWallet(std::string const &password, Error &error);

    // Import a given private key into the wallet database.
    bool importPrivKey(dev::Secret const &secret, 
                       std::string const &password, 
                       std::string const &name, 
                       std::string const &derivationPath, 
                       Error &error);

    // Called by loadWallet() when a wallet is successfully loaded.
    void loadAccounts();

  public:
    // Constructor.
    Wallet(Utils::Provider *providerPointer, boost::filesystem::path _path)
      : provider(providerPointer), path(_path),
        walletDB("accounts", walletFolder())
    {};

    /**
     * Load a wallet with the given password.
     * Creates a new wallet if no wallet exists.
     * Returns true on success, false if password is incorrect..
     */
    bool loadWallet(std::string const &password, Error &error);

    // Check if a wallet is loaded.
    bool isLoaded() { return this->_isLoaded; }

    // Check if password matches with hashed PW.
    bool checkPassword(std::string &password);

    // Check if the wallet file and its keys folder exists in a given path.
    static bool walletExists(boost::filesystem::path &wallet_path);

    // Creates a new account.
    // Uses BIP39 seed stored in json file, throws in case of error.
    bool createNewAccount(std::string derivationPath, std::string &password, Error &error);

    /**
     * Build a transaction from user data.
     * Coin transactions would have a blank dataHex and the "to" address
     * being the destination address.
     * Token transactions would have a filled dataHex, 0 txValue and
     * the "to" address being the token contract's address.
     * Returns a skeleton filled with data for the transaction, which has to be signed.
     */

    dev::eth::TransactionSkeleton buildTransaction(std::string from,
                                              std::string to,
                                              BigNumber value,
                                              BigNumber gasLimit,
                                              BigNumber gasPrice,
                                              std::string dataHex,
                                              int nonce,
                                              bool creation = false // Is TX creating a contract?
                                              );


    /**
     * Signs a data string as an "Ethereum Signed Message".
     * Usable with ecRecover().
     * dataToSign will be converted to hex using Utils.utf8ToHex().
     */
    std::future<std::string> sign(std::string dataToSign, std::string address, std::string password);

    /**
     * Recovers the account that signed the data.
     * dataThatWasSigned will be converted to hex using Utils.utf8ToHex().
     */
    std::future<std::string> ecRecover(std::string dataThatWasSigned, std::string signature);

    // Signs a transaction. "from" account needs to be unlocked.
    std::future<json> signTransaction(json transaction, std::string password);

    // Sends a transaction over the management API.
    std::future<std::string> sendTransaction(json transactionOptions, std::string password);

    // Unlocks a specific account for a given number of seconds.
    void unlockAccount(std::string address, std::string password, unsigned int unlockDuration);

    // Locks a specific account.
    std::future<bool> lockAccount(std::string address);

    /**
     * Returns a list of accounts the node controls by using the provider
     * and calling the RPC method `personal_listAccounts`.
     * Using Accounts.create() will not add accounts into this list.
     * For that use Personal.newAccount().
     * Results are the same as Eth.getAccounts(), except that function
     * calls the RPC method `eth_accounts`.
     */
    std::future<std::vector<std::string>> getAccounts();

    /**
     * Imports a given private key into the keystore, encrypting it with the password.
     * privateKey is a hex string without the "0x".
     * Returns the address of the new account.
     */
    std::future<std::string> importRawKey(std::string privateKey, std::string password);
};

#endif  // WALLET_H
