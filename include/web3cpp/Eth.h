#ifndef ETH_H
#define ETH_H

#include <future>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <web3cpp/Net.h>
#include <web3cpp/Provider.h>
#include <web3cpp/RPC.h>
#include <web3cpp/Utils.h>

#include "version.h"

using json = nlohmann::ordered_json;

// Module that interacts with the blockchain and smart contracts.
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth.html
// https://eth.wiki/json-rpc/API

// TODO:
// - Implement promievents to deal with those functions:
//   - sendTransaction(), sendSignedTransaction()

class Eth {
  private:
    Provider *provider;

  public:
    // Constructor.
    Eth(Provider* _provider) : provider(_provider) {};

    // Getter for provider.
    Provider* getProvider() { return this->provider; }

    // Default address used as the default "from" property, if no "from"
    // property is specified in functions like sendTransaction() or call().
    std::string defaultAccount;

    // Default block used for certain methods.
    // Can be overriden in some functions' last parameters.
    std::string defaultBlock = "latest";

    // Returns the protocol version of the node.
    std::future<std::string> getProtocolVersion();

    // Checks if the node is currently syncing and returns a JSON syncing object.
    // If not syncing, returns "false" inside a JSON object.
    std::future<json> isSyncing();

    // Returns the Coinbase address to which mining rewards will go.
    std::future<std::string> getCoinbase();

    // Checks whether the node is mining or not.
    std::future<bool> isMining();

    // Returns the number of hashes per second that the node is mining with.
    std::future<std::string> getHashrate();

    // Returns the current gas price oracle in Wei.
    // Current gas price = median gas price of the last few blocks.
    std::future<std::string> getGasPrice();

    // Returns a list of accounts the node controls.
    std::future<std::vector<std::string>> getAccounts();

    // Returns the current block number.
    std::future<std::string> getBlockNumber();

    // Returns the Wei balance of an address at a given block,
    // or an empty string in case of error.
    std::future<std::string> getBalance(
      std::string address, std::string defaultBlock = ""
    );

    // Returns the value in storage at a specific position of an address.
    std::future<std::string> getStorageAt(
      std::string address, std::string position, std::string defaultBlock = ""
    );
    std::future<std::string> getStorageAt(
      std::string address, BigNumber position, std::string defaultBlock = ""
    );

    // Returns the code at a specific address.
    std::future<std::string> getCode(std::string address, std::string defaultBlock = "");

    // Returns a block matching the given block number or hash.
    // If returnTransactionObjects is true, the returned block will contain
    // all transactions as objects.
    // If false, will only contain transaction hashes.
    // Returns an empty JSON object on failure.
    std::future<json> getBlock(
      std::string blockHashOrBlockNumber, bool isHash,
      bool returnTransactionObjects = false
    );

    // Returns the number of transactions in a given block.
    std::future<std::string> getBlockTransactionCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    // Returns the number of uncles in a block from a block matching the
    // given block hash or number.
    std::future<std::string> getBlockUncleCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    // Returns a block's uncle by a given uncle index position.
    // An uncle does NOT contain individual transactions.
    // Return structure is the same as getBlock().
    std::future<json> getUncle(
      std::string blockHashOrBlockNumber, std::string uncleIndex,
      bool isHash, bool returnTransactionObjects = false
    );

    // Returns a transaction matching the given hash.
    std::future<json> getTransaction(std::string transactionHash);

    // Returns a transaction based on a block hash or number and the
    // transaction's index position.
    // Return structure is the same as getTransaction().
    std::future<json> getTransactionFromBlock(
      std::string hashStringOrNumber, bool isHash, std::string indexNumber
    );

    // Returns the receipt of a transaction by transaction hash,
    // or an empty object on pending/non-existant transactions.
    std::future<json> getTransactionReceipt(std::string hash);

    // Returns the number of transactions sent from the given address.
    std::future<std::string> getTransactionCount(
      std::string address, std::string defaultBlock = ""
    );

    // Signs data using a specific account.
    std::future<std::string> sign(std::string dataToSign, std::string address);

    // Signs a transaction. transactionObject is the same as sendTransaction().
    // Returns the signed transaction object.
    std::future<json> signTransaction(json txObj);

    // Executes a message call transaction, which is directly executed in the
    // VM of the node, but never mined in the blockchain.
    // callObject is the same as sendTransaction().
    // Returns the data of the call, e.g. a smart contract function's return value.
    std::future<std::string> call(json callObject, std::string defaultBlock = "");

    // Executes a message call or transaction and returns the amount of gas used.
    // The `from` address MUST be specified, otherwise odd behaviour may be experienced.
    // callObject is the same as sendTransaction().
    // Returns the used gas for the simulated call/transaction, as a HEX string.
    std::future<std::string> estimateGas(json callObject);

    // Returns past logs matching the given options.
    // json return is actually a JSON array of log objects
    // (same as getFilterChanges()), or an empty array on failure.
    std::future<json> getPastLogs(json options);

    // Returns work for miners to mine on.
    // Returned strings are all in hex form.
    std::future<std::vector<std::string>> getWork();

    // Submits a proof-of-work solution.
    // Provided strings are all in hex form.
    // Returns true if solution is valid, false otherwise.
    std::future<bool> submitWork(
      std::string nonce, std::string powHash, std::string digest
    );

    // Returns the chain ID of the current connected node as described here:
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-695.md
    uint64_t getChainId();

    // Returns a string with the current client version and info about the node.
    std::string getNodeInfo();
};

#endif  // ETH_H
