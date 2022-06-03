#ifndef CONTRACT_H
#define CONTRACT_H

#include <future>
#include <string>
#include <vector>
#include <web3cpp/devcore/Common.h>
#include <web3cpp/devcore/FixedHash.h>
#include <web3cpp/devcore/Address.h>
#include <web3cpp/ethcore/KeyManager.h>
#include <web3cpp/ethcore/TransactionBase.h>

#include <web3cpp/Utils.h>
#include <web3cpp/Error.h>
#include <web3cpp/Solidity.h>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

// Module that abstracts a smart contract.

// TODO:
// - Implement promievents to deal with those functions:
//   - once(), events(), allEvents(), getPastEvents()

class Contract {
  private:
    enum Types {
      uint256, uint256Arr,
      address, addressArr,
      boolean, booleanArr,
      bytes, bytesArr,
      string, stringArr
    };
    bool isTypeArray(Types const &type);

  public:
    class Options {
      public:
        std::string address;  // "to" address
        json jsonInterface; // JSON array
        std::string data;
        std::string from;
        std::string gasPrice;
        BigNumber gas;
        bool handleRevert;
        unsigned int transactionBlockTimeout;
        unsigned int transactionConfirmationBlocks;
        unsigned int transactionPollingTimeout;
        std::string chain;
        std::string hardfork;
        std::string common;
    };

    // Constructor.
    Contract(json jsonInterface, std::string address, json options = NULL);

    // Object with options for the contract.
    Options options;

    // Methods from the contract.
    std::map<std::string,std::vector<Types>> methods;
    // Functors from the contract
    // Contract Function -> 8 bytes ABI hash.
    std::map<std::string,std::string> functors;

    // Clones the current contract instance.
    Contract clone();

    // ABI constructor from arguments. Uses the custom Solidity class.
    // Pay attention to the arg order! "args, func" = this one.
    std::string operator() (json arguments, std::string function, Error &error);

    // ABI constructor from arguments (old/fallback/legacy).
    // Use this one if something funky happens with the one above.
    // Pay attention to the arg order! "func, args" = this one.
    // -----
    // ALL ARGUMENTS ARE PARSED AS STRINGS INSIDE THE JSON!
    // Always use ordered_json!
    // Example:
    // Function(uint256, address[], uint256)
    // [
    //   "5123815123858123",
    //   [
    //     "0xaaaaaaaaaaaaaaa...",
    //     "0xbbbbbbbbbbbbbbb..."
    //   ],
    //   "189237815123"
    // ]
    // jsonArgs = json::parse(
    //   "[\"5123815123858123\",
    //   [\"0xaaaaaaaaaaaaaaa...\",
    //   \"0xbbbbbbbbbbbbbbb...\"],
    //   \"189237815123\"]"
    // );
    // contract("Function", jsonArgs, error);
    std::string operator() (std::string function, json arguments, Error &error);
};

#endif  // CONTRACT_H
