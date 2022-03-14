#ifndef WALLET_H
#define WALLET_H

#include <future>
#include <string>
#include <vector>

#include "Utils.h"

#include <nlohmann/json.hpp>
#include <web3cpp/devcore/Common.h>
#include <web3cpp/devcore/FixedHash.h>
#include <web3cpp/devcore/Address.h>
#include <web3cpp/ethcore/KeyManager.h>
#include <web3cpp/Account.h>
#include <web3cpp/DB.h>

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
    boost::filesystem::path* path;
    Utils::Provider* provider;
    dev::eth::KeyManager keyManager;

    // Account database and list.
    Database accountsDB;
    std::vector<Account> accounts;

    // Paths for wallet file and secrets folder.
    boost::filesystem::path walletFile()    { return path->string() + "/wallet/wallet.info"; };
    boost::filesystem::path secretsFolder() { return path->string() + "/wallet/secrets"; };

    // Called by loadWallet() if no wallet is found on the desired path.
    bool createNewWallet(std::string &password);

    // Called by loadWallet() when a wallet is successfully loaded.
    void loadAccounts();

  public:
    // Constructor.
    Wallet(Utils::Provider *providerPointer, boost::filesystem::path *pathPointer)
      : provider(providerPointer), path(pathPointer),
        accountsDB("/wallet/accounts", boost::filesystem::path(path->string() + "/accounts/"))
    {};

    /**
     * Load a wallet with the given password.
     * Creates a new wallet if no wallet exists.
     * Returns true on success, false if password is incorrect..
     */
    bool loadWallet(std::string &password);

    // Check if a wallet is loaded.
    bool isLoaded() { return this->keyManager.exists(); }

    // Check if the wallet file and its keys folder exists in a given path.
    static bool walletExists(boost::filesystem::path &wallet_path);

    // Creates a new account.
    std::future<std::string> newAccount(std::string password);

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
