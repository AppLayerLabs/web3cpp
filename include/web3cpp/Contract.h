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
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

// Module that abstracts a smart contract.

// TODO:
// - Implement promievents to deal with those functions:
//   - once(), events(), allEvents(), getPastEvents(), Method::send()
// - Deal with mixed params on Method::call()

class Contract {
  private:
    enum Types {
      uint256, uint256Arr,
      address, addressArr,
      boolean, booleanArr,
      bytes, bytesArr,
      string, stringArr
    };

    std::string encodeABI(
      std::vector<Types> argumentTypes, std::vector<std::string> arguments
    );
    bool isTypeArray(Types const &type);

  public:
    class Method {
      public:
        json arguments;  // JSON array
        //std::future<mixed> call(json options, BigNumber defaultBlock = Eth.defaultBlock); // from, gasPrice, gas
        //promievent send(json options); // from, gasPrice, gas, value, nonce
        std::future<BigNumber> estimateGas(json options); // from, gas, value
        std::string encodeABI();
        std::future<json> createAccessList(json options, std::string block); // from, gas
        std::future<json> createAccessList(json options, BigNumber block); // from, gas
    };

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

    // Deploys the contract to the blockchain.
    // After successful deployment it will resolve to a new contract instance.
    // object is a transaction object.
    //object deploy(json options); // data, arguments

    // ABI constructor from arguments.
    // ALL ARGUMENTS ARE PARSED AS STRINGS INSIDE THE JSON!
    // Always used ordered_json!
    // Example json:
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
