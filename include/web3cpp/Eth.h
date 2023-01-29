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

/**
 * Class that interacts with the blockchain and smart contracts on the network.
 * Network-related functions (those that return futures) return the pure JSON
 * response from the network, which have to be filtered manually by checking
 * if they have a "return" or "error" parameter.
 */

class Eth {
  private:
    Provider *provider; ///< Pointer to Web3::defaultProvider.

  public:
    /**
     * Constructor.
     * @param _provider Pointer to the provider that will be used.
     */
    Eth(Provider* _provider) : provider(_provider) {};

    Provider* getProvider() { return this->provider; } ///< Getter for the provider pointer.

    /**
     * Address used as the default "from" property, if no "from"
     * property is specified in functions that use it (e.g. call()).
     * Defaults to an empty string.
     */
    std::string defaultAccount = "";

    /**
     * Default block used for certain methods, if no block is specified
     * when calling those methods.
     * Can be set to **"latest"**, **"earliest"**, **"pending"**, or a number.
     * Defaults to "latest".
     */
    std::string defaultBlock = "latest";

    /**
     * Get the protocol version of the node.
     * @return A string with the protocol version.
     */
    std::future<json> getProtocolVersion();

    /**
     * Check if the node is currently syncing.
     * @return A syncing object, or `false` if not syncing.
     */
    std::future<json> isSyncing();

    /**
     * Get the coinbase address to which mining rewards will go.
     * @return The coinbase address set in the node for mining rewards.
     */
    std::future<json> getCoinbase();

    /**
     * Check whether the node is mining or not.
     * @return `true` if the node is mining, `false` otherwise.
     */
    std::future<json> isMining();

    /**
     * Get the number of hashes per second that the node is mining with.
     * @return The number of hashes per second.
     */
    std::future<json> getHashrate();

    /**
     * Get the current gas price oracle.
     * The gas price is determined by the last few blocks median gas price.
     * @return The number string of the current gas price in Wei.
     */
    std::future<json> getGasPrice();

    /**
     * Get a list of accounts that the node controls.
     * @return An array of addresses controlled by the node.
     */
    std::future<json> getAccounts();

    /**
     * Get the current block number.
     * @return The number of the most recent block.
     */
    std::future<json> getBlockNumber();

    /**
     * Get the balance of an address at a given block.
     * @param address The address to get the balance from.
     * @param defaultBlock (optional) The block to use as reference. Defaults to Eth::defaultBlock.
     * @return The current balance for the given address in Wei.
     */
    std::future<json> getBalance(
      const std::string& address, const std::string& defaultBlock = ""
    );

    /**
     * Get the value in storage at a specific position of an address.
     * @param address The address to get the storage from.
     * @param position The position of the storage value in hex.
     * @param defaultBlock (optional) The block to use as reference. Defaults to Eth::defaultBlock.
     * @return The value in storage at the given position.
     */
    std::future<json> getStorageAt(
      std::string address, std::string position, const std::string& defaultBlock = ""
    );

    /**
     * Overload of getStorageAt() that takes a BigNumber as the position.
     * Said position will be automatically converted to hex.
     */
    std::future<json> getStorageAt(
      const std::string& address, const BigNumber& position, const std::string& defaultBlock = ""
    );

    /**
     * Get the code at a specific address.
     * @param address The address to get the code from.
     * @param defaultBlock (optional) The block to use as reference. Defaults to Eth::defaultBlock.
     * @return The data at the given address.
     */
    std::future<json> getCode(const std::string& address, const std::string& defaultBlock = "");

    /**
     * Get a block matching the given block number or hash.
     * @param blockHashOrBlockNumber The block number in hex, or the block hash.
     * @param isHash If `true`, will treat the first parameter as a block hash,
     *               and if `false` will treat it as a block number.
     * @param returnTransactionObjects (optional) If `true`, the returned block
     *                                 will contain all transactions as objects,
     *                                 and if `false` will only contain the
     *                                 transaction hashes. Defaults to `false`.
     * @return The block object.
     */
    std::future<json> getBlock(
      std::string blockHashOrBlockNumber, bool isHash,
      bool returnTransactionObjects = false
    );

    /**
     * Get the number of transactions in a given block.
     * @param blockHashOrBlockNumber The block number in hex, or the block hash.
     * @param isHash If `true`, will treat the first parameter as a block hash,
     *               and if `false` will treat it as a block number.
     * @return The number of transactions in the given block.
     */
    std::future<json> getBlockTransactionCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    /**
     * Get the number of uncles from a given block.
     * @param blockHashOrBlockNumber The block number in hex, or the block hash.
     * @param isHash If `true`, will treat the first parameter as a block hash,
     *               and if `false` will treat it as a block number.
     * @return The number of uncles in the given block.
     */
    std::future<json> getBlockUncleCount(
      std::string blockHashOrBlockNumber, bool isHash
    );

    /**
     * Get a block's uncle by a given position.
     * An uncle does NOT contain individual transactions.
     * @param blockHashOrBlockNumber The block number in hex, or the block hash.
     * @param uncleIndex The index position of the uncle.
     * @param isHash If `true`, will treat the first parameter as a block hash,
     *               and if `false` will treat it as a block number.
     * @param returnTransactionObjects (optional) If `true`, the returned block
     *                                 will contain all transactions as objects,
     *                                 and if `false` will only contain the
     *                                 transaction hashes. Defaults to `false`.
     * @return The uncle object. Structure is the same as getBlock().
     */
    std::future<json> getUncle(
      std::string blockHashOrBlockNumber, std::string uncleIndex,
      bool isHash, bool returnTransactionObjects = false
    );

    /**
     * Get a transaction matching the given hash.
     * @param transactionHash The transaction hash.
     * @return The transaction object.
     */
    std::future<json> getTransaction(const std::string& transactionHash);

    /**
     * Get a transaction from a block.
     * @param hashStringOrNumber The block number in hex, or the block hash.
     * @param isHash If `true`, will treat the first parameter as a block hash,
     *               and if `false` will treat it as a block number.
     * @param indexNumber The transaction's index position.
     * @return The transaction object. Structure is the same as getTransaction().
     */
    std::future<json> getTransactionFromBlock(
      std::string hashStringOrNumber, bool isHash, std::string indexNumber
    );

    /**
     * Get the receipt of a transaction.
     * @param hash The transaction hash.
     * @return The transaction receipt object, or an empty object for pending/non-existant transactions.
     */
    std::future<json> getTransactionReceipt(const std::string& hash);

    /**
     * Get the number of transactions sent from an address.
     * @param address The address to look for.
     * @param defaultBlock (optional) The block to use as reference. Defaults to Eth::defaultBlock.
     * @return The number of transactions sent from the given address.
     */
    std::future<json> getTransactionCount(
      const std::string& address, std::string defaultBlock = ""
    );

    /**
     * Sign data using a specific account.
     * @param dataToSign Data to be signed. If it's a non-hex string, it will be
     *                   converted to hex internally.
     * @param address The address to use for signing.
     * @return The signature.
     */
    std::future<json> sign(std::string dataToSign, const std::string& address);

    /**
     * Sign a transaction. 
     * @param txObj The transaction data to sign.
     * @return The RLP encoded transaction.
     */
    std::future<json> signTransaction(const json& txObj);

    /**
     * Execute a message call transaction, which is directly executed in the
     * VM of the node, but never mined in the blockchain.
     * @param callObject A transaction object.
     * @param defaultBlock (optional) The block to use as reference. Defaults to Eth::defaultBlock.
     * @return The data of the call, e.g. a smart contract function's return value.
     */
    std::future<json> call(const json& callObject,const std::string& defaultBlock = "");

    /**
     * Execute a message call or transaction and return the amount of gas used.
     * @param callObject A transaction object. The `from` address MUST be
     *                   specified, otherwise odd behaviour may be experienced.
     * @return The used gas for the simulated call or transaction.
     */
    std::future<json> estimateGas(const json& callObject);

    /**
     * Get past logs matching the given options.
     * @param options The filter options.
     * @return An array of log objects, or an empty array on failure.
     */
    std::future<json> getPastLogs(const json& options);

    /**
     * Get work for miners to mine on.
     * @return The mining work as an array with the following structure:
     *         - 32 bytes string at index 0: current block header pow-hash
     *         - 32 bytes string at index 1: seed hash used for DAG
     *         - 32 bytes string at index 2: boundary condition ("target"), 2^256 difficulty
     */
    std::future<json> getWork();

    /**
     * Submit a proof-of-work solution.
     * @param nonce The nonce found (8 bytes string).
     * @param powHash The header's pow-hash (32 bytes string).
     * @param digest The mix digest (32 bytes string).
     * @return `true` if solution is valid, `false` otherwise.
     */
    std::future<json> submitWork(
      std::string nonce, std::string powHash, std::string digest
    );

    /**
     * Get the chain ID of the current provider.
     * @return The chain ID integer.
     */
    uint64_t getChainId();

    /**
     * Get info about the node.
     * @return A string with the current client version and info about the node.
     */
    std::string getNodeInfo();
};

#endif  // ETH_H
