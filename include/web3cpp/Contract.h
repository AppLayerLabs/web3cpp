#ifndef CONTRACT_H
#define CONTRACT_H

#include <future>
#include <string>
#include <vector>

#include <web3cpp/Utils.h>

// Module that abstracts a smart contract.

// TODO:
// - Implement promievents to deal with those functions:
//   - once(), events(), allEvents(), getPastEvents())

class Contract {
  public:
    class Method {
      json arguments;  // JSON array
      //std::future<mixed> call(json options, BigNumber defaultBlock = Eth.defaultBlock); // from, gasPrice, gas
      //promievent send(json options); // from, gasPrice, gas, value, nonce
      std::future<BigNumber> estimateGas(json options); // from, gas, value
      std::string encodeABI();
      std::future<json> createAccessList(json options, std::string block); // from, gas
      std::future<json> createAccessList(json options, BigNumber block); // from, gas
    };

    class Options {
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
    std::vector<Method> methods;

    // Clones the current contract instance.
    Contract clone();

    // Deploys the contract to the blockchain.
    // After successful deployment it will resolve to a new contract instance.
    // object is a transaction object.
    //object deploy(json options); // data, arguments
};

#endif  // CONTRACT_H
