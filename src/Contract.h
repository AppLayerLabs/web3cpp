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

class Contract {
  public:
    //Contract(jsonObj jsonInterface, std::string address, jsonObj options = NULL);

    //jsonObj options;

    Contract clone();
    //object deploy(jsonObj options); // data, arguments
    std::vector<Method> methods;

    class Method {
      //array arguments;  // Probably a JSON array?
      //std::future<mixed> call(jsonObj options, defaultBlock); // from, gasPrice, gas
      //promievent send(jsonObj options); // from, gasPrice, gas, value, nonce
      //std::future<unsigned int> estimateGas(jsonObj options); // from, gas, value
      std::string encodeABI();
      //std::future<jsonObj> createAccessList(jsonObj options, block); // from, gas
    };
};

#endif  // CONTRACT_H
