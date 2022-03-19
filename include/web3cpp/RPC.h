#ifndef RPC_H
#define RPC_H

#include <string>
#include <sstream>

#include <web3cpp/Utils.h>

// Module that helps with building JSON-RPC requests, for use in Eth.
// Every function returns a formatted and ready-to-use JSON object that
// can be sent through an HTTP request to the network.
// When required, functions will do sanity checks on inputs and will throw
// an error if the check fails.
// Every JSON object returned defaults to an id of "1".
// See the docs at https://eth.wiki/json-rpc/API

namespace RPC {
  json _buildJSON(std::string method, json params = json::array());

  json web3_clientVersion();
  json web3_sha3(std::string data);

  json net_version();
  json net_listening();
  json net_peerCount();

  json eth_protocolVersion();
  json eth_syncing();
  json eth_coinbase();
  json eth_mining();
  json eth_hashrate();
  json eth_gasPrice();
  json eth_accounts();
  json eth_blockNumber();
  json eth_getBalance(std::string address, std::string defaultBlock = "latest");
  json eth_getBalance(std::string address, BigNumber defaultBlock);
  json eth_getStorageAt(std::string address, std::string position, std::string defaultBlock = "latest");
  json eth_getStorageAt(std::string address, std::string position, BigNumber defaultBlock);
  json eth_getTransactionCount(std::string address, std::string defaultBlock = "latest");
  json eth_getTransactionCount(std::string address, BigNumber defaultBlock);
  json eth_getBlockTransactionCountByHash(std::string hash);
  json eth_getBlockTransactionCountByNumber(std::string number);
  json eth_getBlockTransactionCountByNumber(BigNumber number);
  json eth_getUncleCountByBlockHash(std::string hash);
  json eth_getUncleCountByBlockNumber(std::string number);
  json eth_getUncleCountByBlockNumber(BigNumber number);
  json eth_getCode(std::string address, std::string defaultBlock = "latest");
  json eth_getCode(std::string address, BigNumber defaultBlock);
  json eth_sign(std::string address, std::string data);
  json eth_signTransaction(json transactionObject);
  json eth_sendTransaction(json transactionObject);
  json eth_sendRawTransaction(std::string signedTxData);
  json eth_call(json callObject, std::string defaultBlock = "latest");
  json eth_call(json callObject, BigNumber defaultBlock);
  json eth_estimateGas(json callObject);
  json eth_getBlockByHash(std::string hash, bool returnTransactionObjects);
  json eth_getBlockByNumber(std::string number, bool returnTransactionObjects);
  json eth_getBlockByNumber(BigNumber number, bool returnTransactionObjects);
  json eth_getTransactionByHash(std::string hash);
  json eth_getTransactionByBlockHashAndIndex(std::string hash, std::string index);
  json eth_getTransactionByBlockNumberAndIndex(std::string number, std::string index);
  json eth_getTransactionByBlockNumberAndIndex(BigNumber number, std::string index);
  json eth_getTransactionReceipt(std::string hash);
  json eth_getUncleByBlockHashAndIndex(std::string hash, std::string index);
  json eth_getUncleByBlockNumberAndIndex(std::string number, std::string index);
  json eth_getUncleByBlockNumberAndIndex(BigNumber number, std::string index);
  json eth_getCompilers();
  json eth_compileSolidity(std::string sourceCode);
  json eth_compileLLL(std::string sourceCode);
  json eth_compileSerpent(std::string sourceCode);
  json eth_newFilter(json filterOptions);
  json eth_newBlockFilter();
  json eth_newPendingTransactionFilter();
  json eth_uninstallFilter(std::string filterId);
  json eth_getFilterChanges(std::string filterId);
  json eth_getFilterLogs(std::string filterId);
  json eth_getLogs(json filterOptions);
  json eth_getWork();
  json eth_submitWork(std::string nonce, std::string powHash, std::string digest);
  json eth_submitHashrate(std::string hashrate, std::string id);

  json db_putString(std::string dbName, std::string key, std::string value);
  json db_getString(std::string dbName, std::string key);
  json db_putHex(std::string dbName, std::string key, std::string value);
  json db_getHex(std::string dbName, std::string key);

  json shh_version();
  json shh_post(json whisperPostObject);
  json shh_newIdentity();
  json shh_hasIdentity();
  json shh_newGroup();
  json shh_addToGroup(std::string identityAddress);
  json shh_newFilter(json filterOptions);
  json shh_uninstallFilter(std::string filterId);
  json shh_getFilterChanges(std::string filterId);
  json shh_getMessages(std::string filterId);
};

#endif  // RPC_H
