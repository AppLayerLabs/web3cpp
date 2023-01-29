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

// TODO:
// - Implement promievents to deal with those functions:
//   - once(), events(), allEvents(), getPastEvents()

/**
 * Abstraction of a single smart contract.
 */

class Contract {
  private:
    /// Enum for parsing Solidity variable types.
    enum Types {
      uint256, uint256Arr,
      address, addressArr,
      boolean, booleanArr,
      bytes, bytesArr,
      string, stringArr
    };

    /**
     * Check if a given type is an array.
     * @param &type Type value that will be checked.
     * @return `true` if type is array, `false` otherwise.
     */
    bool isTypeArray(Types const &type);

    /**
     * List of methods from the contract, as key and value pairs.
     * Key is the method name, value is a vector with each of the method's parameter types.
     */
    std::map<std::string,std::vector<Types>> _methods;

    /**
     * List of functors from the contract.
     * Key is the method name, value is the method hash (first 8 bytes of the ABI hash).
     */
    std::map<std::string,std::string> _functors;

  public:
    /// Transaction options for the contract.
    class Options {
      public:
        json jsonInterface;                         ///< JSON array for the contract's ABI.
        std::string address;                        ///< "to" address. Defaults to the contract's address.
        std::string data;                           ///< Arbitrary data. Defaults to an empty string.
        std::string from;                           ///< "from" address. Defaults to an empty string.
        std::string gasPrice;                       ///< Gas price (in Wei). Defaults to an empty string.
        std::string gas;                            ///< Gas limit (in Wei). Defaults to an empty string.
        bool handleRevert;                          ///< If enabled, returns the revert reason. Defaults to false.
        unsigned int transactionBlockTimeout;       ///< Blocks to wait until first confirmation happens. Used over socket. Defaults to 50.
        unsigned int transactionConfirmationBlocks; ///< Blocks required to confirm a transaction. Defaults to 24.
        unsigned int transactionPollingTimeout;     ///< Seconds to wait for a transaction receipt. Used over HTTP. Defaults to 750.
        std::string chain;                          ///< Chain used for transactions. Defaults to "mainnet".
        std::string hardfork;                       ///< Hardfork used for transactions. Defaults to "fuji".
    };

    /**
     * Constructor.
     * @param jsonInterface The contract ABI as a JSON object.
     * @param address The contract's address.
     * @param options (optional) The transaction options to override for the
     *                contract as a JSON object. Defaults to NULL.
     */
    Contract(const json& jsonInterface, const std::string& address, json options = NULL);

    Options options;  ///< Object for the contract's transaction options.

    /**
     * List of methods from the contract, as key and value pairs.
     * Key is the method name, value is a vector with each of the method's parameter types.
     */
    const std::map<std::string,std::vector<Types>>& methods() { return _methods; }

    /**
     * List of functors from the contract.
     * Key is the method name, value is the method hash (first 8 bytes of the ABI hash).
     */
    const std::map<std::string,std::string>& functors() { return _functors; };

    /**
     * Clones the current contract instance.
     * @return The cloned contract object.
     */
    Contract clone();

    /**
     * ABI constructor. Uses the custom Solidity class.
     * **Pay attention to the arg order! "args, func" = this one.**
     * @param arguments The function's arguments as a JSON object,
     *                      each with "type" and "value" (or "t" and "v").
     *                      e.g. `{{"t", "string"}, {"v", "Hello!%"}}`
     * @param function The function's name.
     * @param &error Error object.
     * @return The %Solidity encoded function ABI.
     */
    std::string operator() (const json& arguments, const std::string& function, Error &error);

    /**
     * ABI constructor.
     * **LEGACY/UNMAINTAINED**
     * Use only if the normal one doesn't work as intended for some reason.
     * **Pay attention to the arg order! "func, args" = this one.**
     * @param function The function's name.
     * @param arguments The function's arguments as a JSON object,
     *                      each with "type" and "value" (or "t" and "v").
     *                      e.g. `{{"t", "string"}, {"v", "Hello!%"}}`
     * @param &error Error object.
     * @return The %Solidity encoded function ABI.
     */
    std::string operator() (const std::string& function, const json& arguments, Error &error);
};

#endif  // CONTRACT_H
