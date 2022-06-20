#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

#include <web3cpp/DB.h>
#include <web3cpp/Net.h>
#include <web3cpp/Provider.h>
#include <web3cpp/Utils.h>
#include <web3cpp/RPC.h>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>

using json = nlohmann::ordered_json;

/**
 * Module that contains information about a given account,
 * imported by the Wallet class which controls all instances of Accounts.
 * Also contains information about transactions of that specific account.
 */

class Account {
  private:
    std::string _address;
    std::string _name;
    std::string _derivationPath;
    uint64_t _nonce;
    bool _isLedger;
    Provider *provider;
    std::atomic<bool> ready;
    mutable std::mutex accountLock;
    Database transactionDB;

  public:
    // Constructors: empty, default and copy
    Account(){}
    Account(
      boost::filesystem::path walletPath, std::string name, std::string __address,
      std::string __derivationPath, bool __isLedger, Provider *_provider
    );
    Account(Account& other) noexcept :
      _address(other._address),
      _name(other._name),
      _derivationPath(other._derivationPath),
      _isLedger(other._isLedger),
      _nonce(other._nonce),
      provider(other.provider),
      transactionDB(other.transactionDB)
    {}

    // Getters.
    std::string address() { return _address; };
    std::string name() { return _name; }
    std::string derivationPath() { return _derivationPath; };
    uint64_t nonce() { return _nonce; };
    bool isLedger() { return _isLedger; };

    // Get the Account's balance from the network.
    std::future<BigNumber> balance();

    // Save a transaction to the Account's local history.
    bool saveTxToHistory(std::string signedTx);

    // Get all saved transactions from this Account's local history.
    json getTxHistory();
};

#endif  // ACCOUNTS_H
