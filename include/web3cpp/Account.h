#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Module that contains information about a given account, imported by the Wallet class which controls all instances of Accounts.
// Also contains information about transactions of that specific account.

class Account {
  private:
  std::string _address;
  std::string _derivationPath;
  bool _isLedger;

  public:
  std::string address() { return _address; };
  std::string derivationPath() { return _derivationPath; };
  bool isLedger() { return _isLedger; };

  // Constructors

  // New account from keyManager DB, require loading derivationPath from levelDB.
  Account(std::string &__address);

  // New generated account, or loaded directly from LedgerDB.
  Account(std::string &__address, std::string &__derivationPath, bool &__isLedger);
};

#endif  // ACCOUNTS_H
