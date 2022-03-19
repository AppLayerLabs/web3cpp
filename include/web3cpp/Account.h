#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

#include <web3cpp/DB.h>
#include <web3cpp/Net.h>
#include <web3cpp/Utils.h>
#include <web3cpp/RPC.h>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>

using json = nlohmann::json;

/**
 * Module that contains information about a given account,
 * imported by the Wallet class which controls all instances of Accounts.
 * Also contains information about transactions of that specific account.
 */

class Account {
  private:
    Utils::Provider *provider;
    std::string _address;
    std::string _derivationPath;
    uint64_t _nonce;
    bool _isLedger;
    Database transactionDb;
    std::atomic<bool> ready;
    mutable std::mutex accountLock;

  public:
    // Constructor.
    Account(
      boost::filesystem::path walletPath, std::string __address,
      std::string __derivationPath, bool __isLedger, Utils::Provider *_provider
    );

    // Copy constructor.
    Account (Account&& other) noexcept :
      _address(std::move(other._address)),
      _derivationPath(std::move(other._derivationPath)),
      _isLedger(std::move(other._isLedger)),
      _nonce(std::move(other._nonce)),
      transactionDb(std::move(other.transactionDb)),
      provider(std::move(other.provider))
    {}

    // Getters.
    std::string address() { return _address; };
    std::string derivationPath() { return _derivationPath; };
    uint64_t nonce() { return _nonce; };

    // Network related requests.

    std::future<BigNumber> balance();
    bool isLedger() { return _isLedger; };
};

#endif  // ACCOUNTS_H
