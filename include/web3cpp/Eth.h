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
// Network functions always return the pure JSON response, which
// has to be filtered afterwards ("result" or "error").
// https://web3js.readthedocs.io/en/v1.7.0/web3-eth.html
// https://eth.wiki/json-rpc/API

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
    std::future<json> getProtocolVersion();

    // Checks if the node is currently syncing.
    // Returns either a syncing object, or "false".
    std::future<json> isSyncing();

    // Returns the Coinbase address to which mining rewards will go.
    std::future<json> getCoinbase();

    // Checks whether the node is mining or not.
    // Returns "true" if the node is mining, "false" otherwise.
    std::future<json> isMining();

    // Returns the number of hashes per second that the node is mining with.
    std::future<json> getHashrate();

    // Returns the current gas price oracle in Wei.
    // The gas price is determined by the last few blocks median gas price.
    std::future<json> getGasPrice();

    // Returns a list of accounts the node controls.
    std::future<json> getAccounts();

    // Returns the current block number.
    std::future<json> getBlockNumber();

    // Returns the Wei balance of an address at a given block.
    std::future<json> getBalance(
      std::string address, std::string defaultBlock = ""
    );

    // Returns the value in storage at a specific position of an address.
    // Position has to be in HEX if string.
    // Position in BigNumber will be automatically converted to HEX.
    std::future<json> getStorageAt(
      std::string address, std::string position, std::string defaultBlock = ""
    );
    std::future<json> getStorageAt(
      std::string address, BigNumber position, std::string defaultBlock = ""
    );

    // Returns the code at a specific address.
    std::future<json> getCode(std::string address, std::string defaultBlock = "");

    // Returns a block matching the given block number or hash.
    // Block number has to be in HEX.
    // If returnTransactionObjects is true, the returned block will contain
    // all transactions as objects.
    // If false, will only contain transaction hashes.
    std::future<json> getBlock(
      std::string blockHashOrBlockNumber, bool isHash,
      bool returnTransactionObjects = false
    );

    // Returns the number of transactions in a given block.
    // Block number has to be in HEX.
    std::future<json> getBlockTransactionCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    // Returns the number of uncles in a block from a block matching the
    // given block hash or number.
    // Block number has to be in HEX.
    std::future<json> getBlockUncleCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    // Returns a block's uncle by a given uncle index position.
    // Both block number AND uncle index have to be in HEX.
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
    // Both block number AND index have to be in HEX.
    // Return structure is the same as getTransaction().
    std::future<json> getTransactionFromBlock(
      std::string hashStringOrNumber, bool isHash, std::string indexNumber
    );

    // Returns the receipt of a transaction by transaction hash,
    // or an empty object on pending/non-existant transactions.
    std::future<json> getTransactionReceipt(std::string hash);

    // Returns the number of transactions sent from the given address.
    std::future<json> getTransactionCount(
      std::string address, std::string defaultBlock = ""
    );

    // Signs data using a specific account.
    // If data is a normal string it will be converted to HEX internally.
    std::future<json> sign(std::string dataToSign, std::string address);

    // Signs a transaction. transactionObject is the same as sendTransaction().
    // Returns the signed transaction object.
    std::future<json> signTransaction(json txObj);

    // Executes a message call transaction, which is directly executed in the
    // VM of the node, but never mined in the blockchain.
    // callObject is the same as sendTransaction().
    // Returns the data of the call, e.g. a smart contract function's return value.
    std::future<json> call(json callObject, std::string defaultBlock = "");

    // Executes a message call or transaction and returns the amount of gas used.
    // The `from` address MUST be specified, otherwise odd behaviour may be experienced.
    // callObject is the same as sendTransaction().
    // Returns the used gas for the simulated call/transaction, as a HEX string.
    std::future<json> estimateGas(json callObject);

    // Returns past logs matching the given options.
    // json return is actually a JSON array of log objects
    // (same as getFilterChanges()), or an empty array on failure.
    std::future<json> getPastLogs(json options);

    // Returns work for miners to mine on.
    // Returned strings are all in HEX.
    std::future<json> getWork();

    // Submits a proof-of-work solution.
    // Provided strings are all in hex form.
    // Returns true if solution is valid, false otherwise.
    std::future<json> submitWork(
      std::string nonce, std::string powHash, std::string digest
    );

    // Returns the chain ID of the current connected node as described here:
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-695.md
    uint64_t getChainId();

    // Returns a string with the current client version and info about the node.
    std::string getNodeInfo();
};

#endif  // ETH_H
