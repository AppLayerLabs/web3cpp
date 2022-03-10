#ifndef WALLET_H
#define WALLET_H

#include <future>
#include <string>
#include <vector>

#include "Utils.h"

#include <nlohmann/json.hpp>
#include <web3cpp/devcore/Common.h>
#include <web3cpp/devcore/FixedHash.h>
#include <web3cpp/ethcore/KeyManager.h>

using json = nlohmann::json;

// Module that interacts with a node's accounts.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth-personal.html

// TODO:
// - Inherit the umbrella class to get access to common functions
//   - setProvider(), providers, givenProvider, currentProvider, BatchRequest(), extend()
// - Decide how to deal with callbacks on (almost) all functions

class Wallet {
  private:
    dev::bytesSec passHash;
    dev::h256 passSalt;
    int passIterations = 100000;

    boost::filesystem::path* path;
    Utils::Provider* provider;
    dev::eth::KeyManager keyManager;

    boost::filesystem::path walletFile()    { return path->string() + "/wallet/wallet.info"; };
    boost::filesystem::path secretsFolder() { return path->string() + "/wallet/secrets"; };

    bool createNewWallet(std::string password);

  public:
    // Wallet load with passphrase, create new wallet if empty.
    // Return false if incorrect password
    bool loadWallet(std::string password);

    bool isLoaded() { return this->keyManager.exists(); }
    // Constructor.
    Wallet(Utils::Provider *providerPointer, boost::filesystem::path *pathPointer) : provider(providerPointer), path(pathPointer) {};

    // Creates a new account.
    std::future<std::string> newAccount(std::string password);

    // Signs a data string as an "Ethereum Signed Message". Usable with ecRecover().
    // dataToSign will be converted to hex using Utils.utf8ToHex().
    std::future<std::string> sign(std::string dataToSign, std::string address, std::string password);

    // Recovers the account that signed the data.
    // dataThatWasSigned will be converted to hex using Utils.utf8ToHex().
    std::future<std::string> ecRecover(std::string dataThatWasSigned, std::string signature);

    // Signs a transaction. "from" account needs to be unlocked.
    std::future<json> signTransaction(json transaction, std::string password);

    // Sends a transaction over the management API.
    std::future<std::string> sendTransaction(json transactionOptions, std::string password);

    // Unlocks a specific account for a given number of seconds.
    void unlockAccount(std::string address, std::string password, unsigned int unlockDuration);

    // Locks a specific account.
    std::future<bool> lockAccount(std::string address);

    // Returns a list of accounts the node controls by using the provider and
    // calling the RPC method `personal_listAccounts`.
    // Using Accounts.create() will not add accounts into this list.
    // For that use Personal.newAccount().
    // Results are the same as Eth.getAccounts() except that calls the
    // RPC method `eth_accounts`.
    std::future<std::vector<std::string>> getAccounts();

    // Imports a given private key into the keystore, encrypting it with the password.
    // privateKey is a hex string without the "0x".
    // Returns the address of the new account.
    std::future<std::string> importRawKey(std::string privateKey, std::string password);
};

#endif  // WALLET_H
