#ifndef RPC_H
#define RPC_H

#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>

#include <web3cpp/Error.h>
#include <web3cpp/Utils.h>

/**
 * Namespace for building [JSON-RPC](https://ethereum.org/en/developers/docs/apis/json-rpc/) requests.
 * Every function returns a properly formatted JSON object that can be sent
 * to a provider through an HTTP request.
 * The structure of the JSON return is as follows:
 * 
 * `{"jsonrpc": "2.0", "id": 1, "method": "<method-name>", "params": ["<method-params>"]}`
 * 
 * Every JSON return defaults to an id of `1`. If multiple requests will be
 * sent at once, their fields will have to be ordered manually.
 * Functions that require an Error object will do sanity checks on inputs,
 * and when one of those checks fail, the appropriate error code will be set
 * and an empty JSON object will be returned.
 */

namespace RPC {
  /**
   * Build the actual JSON object that will be returned by all the other functions.
   * @param method The method that will be called on the network request.
   * @param params (optional) The parameters of the method, if they exist.
   *               Defaults to an empty array.
   * @return The JSON object that will be sent as a request.
   */
  json _buildJSON(std::string method, json params = json::array());

  /**
   * Check if a given data string is in hex format.
   * @param hex The data string to check.
   * @param strict (optional) If enabled, will check with Utils::isHexStrict().
   *                          instead of Utils::isHex(). Defaults to true.
   * @return `true` if data is in hex format, `false` otherwise.
   */
  bool _checkHexData(std::string hex, bool strict = true);

  /**
   * Check the length of a given hex string. Ignores the "0x" prefix if it exists.
   * @param hex The hex string to check.
   * @param hexLength The length of the hex string, **in hex bytes**
   *                  (e.g. "deadbeef" = 4 ("de ad be ef"), not 8 ("d e a d b e e f")).
   * @return `true` if hex string has the given length, `false` otherwise.
   */
  bool _checkHexLength(std::string hex, int hexLength);

  /**
   * Check if a given string is a valid address, as defined by Utils::isAddress().
   * @param add The address to check.
   * @return `true` if string is a valid address, `false` otherwise.
   */
  bool _checkAddress(std::string add);

  /**
   * Check if a given string is a valid block number.
   * A "valid block number" is either `"latest"`, `"earliest"`, `"pending"` or a digit.
   * @param block The block number to check.
   * @return `true` if block number is valid, `false` otherwise.
   */
  bool _checkDefaultBlock(std::string block);

  /**
   * Check if a given string is a number.
   * @param num The number string to check.
   * @return `true` if string is a number, `false` otherwise.
   */
  bool _checkNumber(std::string num);

  json web3_clientVersion(); ///< Build data for `web3_clientVersion`.

  /**
   * Build data for `web3_sha3`.
   * @param data The data to convert into a SHA3 hash.
   * @param &err Error object.
   */
  json web3_sha3(std::string data, Error &err);

  json net_version();   ///< Build data for `net_version`.
  json net_listening(); ///< Build data for `net_listening`.
  json net_peerCount(); ///< Build data for `net_peerCount`.

  json eth_protocolVersion(); ///< Build data for `eth_protocolVersion`.
  json eth_syncing();         ///< Build data for `eth_syncing`.
  json eth_coinbase();        ///< Build data for `eth_coinbase`.
  json eth_mining();          ///< Build data for `eth_mining`.
  json eth_hashrate();        ///< Build data for `eth_hashrate`.
  json eth_gasPrice();        ///< Build data for `eth_gasPrice`.
  json eth_accounts();        ///< Build data for `eth_accounts`.
  json eth_blockNumber();     ///< Build data for `eth_blockNumber`.

  /**
   * Build data for `eth_getBalance`.
   * @param address The address to check for balance.
   * @param defaultBlock The block to use as reference.
   * @param &err Error object.
   */
  json eth_getBalance(std::string address, std::string defaultBlock, Error &err);

  /// Overload of getBalance() that takes a BigNumber as the block.
  json eth_getBalance(std::string address, BigNumber defaultBlock, Error &err);

  /**
   * Build data for `eth_getStorageAt`.
   * @param address The address to check for storage.
   * @param position The index position of the storage.
   * @param defaultBlock The block to use as reference.
   * @param &err Error object.
   */
  json eth_getStorageAt(std::string address, std::string position, std::string defaultBlock, Error &err);

  /// Overload of getStorageAt() that takes a BigNumber as the block.
  json eth_getStorageAt(std::string address, std::string position, BigNumber defaultBlock, Error &err);

  /**
   * Build data for `eth_getTransactionCount`.
   * @param address The address to check for number of sent transactions.
   * @param defaultBlock The block to use as reference.
   * @param &err Error object.
   */
  json eth_getTransactionCount(std::string address, std::string defaultBlock, Error &err);

  /// Overload of getTransactionCount() that takes a BigNumber as the block.
  json eth_getTransactionCount(std::string address, BigNumber defaultBlock, Error &err);

  /**
   * Build data for `eth_getBlockTransactionCountByHash`.
   * @param hash The hash of a block.
   * @param &err Error object.
   */
  json eth_getBlockTransactionCountByHash(std::string hash, Error &err);

  /**
   * Build data for `eth_getBlockTransactionCountByNumber`.
   * @param number The block number.
   * @param &err Error object.
   */
  json eth_getBlockTransactionCountByNumber(std::string number, Error &err);

  /// Overload of eth_getBlockTransactionCountByNumber() that takes a BigNumber as the block.
  json eth_getBlockTransactionCountByNumber(BigNumber number, Error &err);

  /**
   * Build data for `eth_getUncleCountByBlockHash`.
   * @param hash The hash of a block.
   * @param &err Error object.
   */
  json eth_getUncleCountByBlockHash(std::string hash, Error &err);

  /**
   * Build data for `eth_getUncleCountByBlockNumber`.
   * @param number The block number.
   * @param &err Error object.
   */
  json eth_getUncleCountByBlockNumber(std::string number, Error &err);

  /// Overload of eth_getUncleCountByBlockNumber() that takes a BigNumber as the block.
  json eth_getUncleCountByBlockNumber(BigNumber number, Error &err);

  /**
   * Build data for `eth_getCode`.
   * @param address The address to check for data.
   * @param defaultBlock The block to use as reference.
   * @param &err Error object.
   */
  json eth_getCode(std::string address, std::string defaultBlock, Error &err);

  /// Overload of eth_getCode() that takes a BigNumber as the block.
  json eth_getCode(std::string address, BigNumber defaultBlock, Error &err);

  /**
   * Build data for `eth_sign`.
   * @param address The address to use for signing.
   * @param data The data to sign.
   * @param &err Error object.
   */
  json eth_sign(std::string address, std::string data, Error &err);

  /**
   * Build data for `eth_signTransaction`.
   * @param txObj The transaction object to sign.
   * @param &err Error object.
   */
  json eth_signTransaction(json txObj, Error &err);

  /**
   * Build data for `eth_sendTransaction`.
   * @param txObj The transaction object to send.
   * @param &err Error object.
   */
  json eth_sendTransaction(json txObj, Error &err);

  /**
   * Build data for `eth_sendRawTransaction`.
   * @param signedTxData The raw transaction signature to send.
   * @param &err Error object.
   */
  json eth_sendRawTransaction(std::string signedTxData, Error &err);

  /**
   * Build data for `eth_call`.
   * @param callObject The transaction call object.
   * @param defaultBlock The block to use as reference.
   * @param &err Error object.
   */
  json eth_call(json callObject, std::string defaultBlock, Error &err);

  /// Overload of eth_call() that takes a BigNumber as the block.
  json eth_call(json callObject, BigNumber defaultBlock, Error &err);

  /**
   * Build data for `eth_estimateGas`.
   * @param callObject The transaction call object.
   * @param &err Error object.
   */
  json eth_estimateGas(json callObject, Error &err);

  /**
   * Build data for `eth_getBlockByHash`.
   * @param hash The hash of a block.
   * @param returnTransactionObjects If `true`, returns full transaction objects,
   *                                 and if `false` returns only transaction hashes.
   * @param &err Error object.
   */
  json eth_getBlockByHash(std::string hash, bool returnTransactionObjects, Error &err);

  /**
   * Build data for `eth_getBlockByNumber`.
   * @param number The block number.
   * @param returnTransactionObjects If `true`, returns full transaction objects,
   *                                 and if `false` returns only transaction hashes.
   * @param &err Error object.
   */
  json eth_getBlockByNumber(std::string number, bool returnTransactionObjects, Error &err);

  /// Overload of eth_getBlockByNumber() that takes a BigNumber as the block.
  json eth_getBlockByNumber(BigNumber number, bool returnTransactionObjects, Error &err);

  /**
   * Build data for `eth_getTransactionByHash`.
   * @param hash The hash of a transaction.
   * @param &err Error object.
   */
  json eth_getTransactionByHash(std::string hash, Error &err);

  /**
   * Build data for `eth_getTransactionByBlockHashAndIndex`.
   * @param hash The hash of a block.
   * @param index The index position of the transaction.
   * @param &err Error object.
   */
  json eth_getTransactionByBlockHashAndIndex(std::string hash, std::string index, Error &err);

  /**
   * Build data for `eth_getTransactionByBlockNumberAndIndex`.
   * @param number The block number.
   * @param index The index position of the transaction.
   * @param &err Error object.
   */
  json eth_getTransactionByBlockNumberAndIndex(std::string number, std::string index, Error &err);

  /// Overload of eth_getTransactionByBlockNumberAndIndex() that takes a BigNumber as the block.
  json eth_getTransactionByBlockNumberAndIndex(BigNumber number, std::string index, Error &err);

  /**
   * Build data for `eth_getTransactionReceipt`.
   * @param hash The hash of a transaction.
   * @param &err Error object.
   */
  json eth_getTransactionReceipt(std::string hash, Error &err);

  /**
   * Build data for `eth_getUncleByBlockHashAndIndex`.
   * @param hash The hash of a block.
   * @param index The index position of the uncle.
   * @param &err Error object.
   */
  json eth_getUncleByBlockHashAndIndex(std::string hash, std::string index, Error &err);

  /**
   * Build data for `eth_getUncleByBlockNumberAndIndex`.
   * @param number The block number.
   * @param index The index position of the uncle.
   * @param &err Error object.
   */
  json eth_getUncleByBlockNumberAndIndex(std::string number, std::string index, Error &err);

  /// Overload of eth_getUncleByBlockNumberAndIndex() that takes a BigNumber as the block.
  json eth_getUncleByBlockNumberAndIndex(BigNumber number, std::string index, Error &err);

  json eth_getCompilers();  ///< Build data for `eth_getCompilers`.

  /**
   * Build data for `eth_newFilter`.
   * @param filterOptions The filter options.
   * @param &err Error object.
   */
  json eth_newFilter(json filterOptions, Error &err);

  json eth_newBlockFilter();               ///< Build data for `eth_newBlockFilter`.
  json eth_newPendingTransactionFilter();  ///< Build data for `eth_newPendingTransactionFilter`.

  /**
   * Build data for `eth_uninstallFilter`.
   * @param filterId The filter id.
   * @param &err Error object.
   */
  json eth_uninstallFilter(std::string filterId, Error &err);

  /**
   * Build data for `eth_getFilterChanges`.
   * @param filterId The filter id.
   * @param &err Error object.
   */
  json eth_getFilterChanges(std::string filterId, Error &err);

  /**
   * Build data for `eth_getFilterLogs`.
   * @param filterId The filter id.
   * @param &err Error object.
   */
  json eth_getFilterLogs(std::string filterId, Error &err);

  /**
   * Build data for `eth_getLogs`.
   * @param filterOptions The filter options.
   * @param &err Error object.
   */
  json eth_getLogs(json filterOptions, Error &err);

  json eth_getWork(); ///< Build data for `eth_getWork`.

  /**
   * Build data for `eth_submitWork`.
   * @param nonce The nonce found (8 bytes string).
   * @param powHash The header's pow-hash (32 bytes string).
   * @param digest The mix digest (32 bytes string).
   * @param &err Error object.
   */
  json eth_submitWork(std::string nonce, std::string powHash, std::string digest, Error &err);

  /**
   * Build data for `eth_submitHashrate`.
   * @param hashrate A 32 bytes string representing the hashrate.
   * @param id A 32 bytes string representing the client ID.
   * @param &err Error object.
   */
  json eth_submitHashrate(std::string hashrate, std::string id, Error &err);
};

#endif  // RPC_H
