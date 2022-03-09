#ifndef CONTRACT_H
#define CONTRACT_H

#include <future>
#include <string>
#include <vector>

// Module that abstracts a smart contract.

// TODO:
// - Inherit Eth to have access to some properties
//   - defaultAccount/Chain/Block/Hardfork/Common, transaction timeouts, handleRevert, etc.
// - Decide how to deal with "options" (it's a JSON object but should also be a subclass?)
// - Implement events (once, events, events.allEvents, getPastEvents)
// - Check if Method implementation should be this way
// - Decide how to deal with callbacks and promievents for some functions
// - estimateGas can handle just an unsigned int? Probably it'll need a big number
// - Maybe force json::array in Method::arguments and Contract::options?

class Contract {
  public:
    class Method {
      //json arguments;  // Probably a JSON array?
      //std::future<mixed> call(json options, defaultBlock); // from, gasPrice, gas
      //promievent send(json options); // from, gasPrice, gas, value, nonce
      std::future<unsigned int> estimateGas(json options); // from, gas, value
      std::string encodeABI();
      //std::future<json> createAccessList(json options, block); // from, gas
    };

    // Constructor.
    Contract(json jsonInterface, std::string address, json options = NULL);

    // Array of options.
    json options;

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
