#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <future>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Module that contains information about a given account, imported by the Wallet class which controls all instances of Accounts.
// Also contains information about transactions.

class Account {
  public:
  const std::string address;
  const std::string derivationPath;
  const bool isLedger;


  // Constructors
  Account() {}
};

#endif  // ACCOUNTS_H
