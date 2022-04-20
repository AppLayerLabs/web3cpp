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
    std::string _address;
    std::string _name;
    std::string _derivationPath;
    uint64_t _nonce;
    bool _isLedger;
    Utils::Provider *provider;
    std::atomic<bool> ready;
    mutable std::mutex accountLock;

  public:
    // Empty constructor.
    Account(){}

    // Constructor.
    Account(
      boost::filesystem::path walletPath, std::string name, std::string __address,
      std::string __derivationPath, bool __isLedger, Utils::Provider *_provider
    );

    // Copy constructor.
    Account(Account& other) noexcept :
      _address(other._address),
      _name(other._name),
      _derivationPath(other._derivationPath),
      _isLedger(other._isLedger),
      _nonce(other._nonce),
      provider(other.provider)
    {}

    // Getters.
    std::string address() { return _address; };
    std::string name() { return _name; }
    std::string derivationPath() { return _derivationPath; };
    uint64_t nonce() { return _nonce; };

    // Network related requests.
    std::future<BigNumber> balance();
    bool isLedger() { return _isLedger; };
};

#endif  // ACCOUNTS_H
