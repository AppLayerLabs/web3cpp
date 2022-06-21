#include <web3cpp/RPC.h>

json RPC::_buildJSON(std::string method, json params) {
  return {{"jsonrpc", "2.0"}, {"method", method}, {"params", params}, {"id", 1}};
}

bool RPC::_checkHexData(std::string hex, bool strict) {
  return (strict) ? Utils::isHexStrict(hex) : Utils::isHex(hex);
}

bool RPC::_checkHexLength(std::string hex, int hexLength) {
  int start = (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") ? 2 : 0;
  return (hex.substr(start).length() / 2 == hexLength);
}

bool RPC::_checkAddress(std::string add) {
  return Utils::isAddress(add);
}

bool RPC::_checkDefaultBlock(std::string defaultBlock) {
  return (
    defaultBlock == "latest" ||
    defaultBlock == "earliest" ||
    defaultBlock == "pending" ||
    std::all_of(defaultBlock.begin(), defaultBlock.end(), ::isdigit)
  );
}

bool RPC::_checkNumber(std::string num) {
  return std::all_of(num.begin(), num.end(), ::isdigit);
}

json RPC::web3_clientVersion() {
  return _buildJSON("web3_clientVersion");
}

json RPC::web3_sha3(std::string data, Error &err) {
  int errCode = 0;
  if (!_checkHexData(data)) errCode = 4; // Invalid Hex Data
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("web3_sha3", {data});
}

json RPC::net_version() {
  return _buildJSON("net_version");
}

json RPC::net_listening() {
  return _buildJSON("net_listening");
}

json RPC::net_peerCount() {
  return _buildJSON("net_peerCount");
}

json RPC::eth_protocolVersion() {
  return _buildJSON("eth_protocolVersion");
}

json RPC::eth_syncing(){
  return _buildJSON("eth_syncing");
}

json RPC::eth_coinbase(){
  return _buildJSON("eth_coinbase");
}

json RPC::eth_mining(){
  return _buildJSON("eth_mining");
}

json RPC::eth_hashrate() {
  return _buildJSON("eth_hashrate");
}

json RPC::eth_gasPrice() {
  return _buildJSON("eth_gasPrice");
}

json RPC::eth_accounts() {
  return _buildJSON("eth_accounts");
}

json RPC::eth_blockNumber() {
  return _buildJSON("eth_blockNumber");
}

json RPC::eth_getBalance(std::string address, std::string defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBalance", {address, defaultBlock});
}

json RPC::eth_getBalance(std::string address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getBalance(address, ss.str(), err);
}

json RPC::eth_getStorageAt(std::string address, std::string position, std::string defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkHexData(position)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getStorageAt", {address, position, defaultBlock});
}

json RPC::eth_getStorageAt(std::string address, std::string position, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getStorageAt(address, position, ss.str(), err);
}

json RPC::eth_getTransactionCount(std::string address, std::string defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionCount", {address, defaultBlock});
}

json RPC::eth_getTransactionCount(std::string address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getTransactionCount(address, ss.str(), err);
}

json RPC::eth_getBlockTransactionCountByHash(std::string hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockTransactionCountByHash", {hash});
}

json RPC::eth_getBlockTransactionCountByNumber(std::string number, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockTransactionCountByNumber", {number});
}

json RPC::eth_getBlockTransactionCountByNumber(BigNumber number, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getBlockTransactionCountByNumber(ss.str(), err);
}

json RPC::eth_getUncleCountByBlockHash(std::string hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleCountByBlockHash", {hash});
}

json RPC::eth_getUncleCountByBlockNumber(std::string number, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleCountByBlockNumber", {number});
}

json RPC::eth_getUncleCountByBlockNumber(BigNumber number, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getUncleCountByBlockNumber(ss.str(), err);
}

json RPC::eth_getCode(std::string address, std::string defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getCode", {address, defaultBlock});
}

json RPC::eth_getCode(std::string address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getCode(address, ss.str(), err);
}

json RPC::eth_sign(std::string address, std::string data, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkHexData(data)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_sign", {data});
}

json RPC::eth_signTransaction(json txObj, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(txObj["from"]) ||
      (txObj.count("to") && !_checkAddress(txObj["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      !_checkHexData(txObj["data"]) ||
      (txObj.count("gas") && !_checkHexData(txObj["gas"])) ||
      (txObj.count("gasPrice") && !_checkHexData(txObj["gasPrice"])) ||
      (txObj.count("value") && !_checkHexData(txObj["value"])) ||
      (txObj.count("nonce") && !_checkHexData(txObj["nonce"]))
    ) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_signTransaction", {txObj});
}

json RPC::eth_sendTransaction(json txObj, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(txObj["from"]) ||
      (txObj.count("to") && !_checkAddress(txObj["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      !_checkHexData(txObj["data"]) ||
      (txObj.count("gas") && !_checkHexData(txObj["gas"])) ||
      (txObj.count("gasPrice") && !_checkHexData(txObj["gasPrice"])) ||
      (txObj.count("value") && !_checkHexData(txObj["value"])) ||
      (txObj.count("nonce") && !_checkHexData(txObj["nonce"]))
    ) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_sendTransaction", {txObj});
}

json RPC::eth_sendRawTransaction(std::string signedTxData, Error &err) {
  err.setCode((!_checkHexData(signedTxData)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_sendRawTransaction", {signedTxData});
}

json RPC::eth_call(json callObject, std::string defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(callObject["from"]) ||
      (callObject.count("to") && !_checkAddress(callObject["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      !_checkHexData(callObject["data"]) ||
      (callObject.count("gas") && !_checkHexData(callObject["gas"])) ||
      (callObject.count("gasPrice") && !_checkHexData(callObject["gasPrice"])) ||
      (callObject.count("value") && !_checkHexData(callObject["value"]))
    ) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_call", {callObject, defaultBlock});
}

json RPC::eth_call(json callObject, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_call(callObject, ss.str(), err);
}

json RPC::eth_estimateGas(json callObject, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(callObject["from"]) ||
      (callObject.count("to") && !_checkAddress(callObject["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      !_checkHexData(callObject["data"]) ||
      (callObject.count("gas") && !_checkHexData(callObject["gas"])) ||
      (callObject.count("gasPrice") && !_checkHexData(callObject["gasPrice"])) ||
      (callObject.count("value") && !_checkHexData(callObject["value"]))
    ) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_estimateGas", {callObject});
}

json RPC::eth_getBlockByHash(std::string hash, bool returnTransactionObjects, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockByHash", {hash, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(std::string number, bool returnTransactionObjects, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockByNumber", {number, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(BigNumber number, bool returnTransactionObjects, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getBlockByNumber(ss.str(), returnTransactionObjects, err);
}

json RPC::eth_getTransactionByHash(std::string hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByHash", {hash});
}

json RPC::eth_getTransactionByBlockHashAndIndex(std::string hash, std::string index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(std::string number, std::string index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(number) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(BigNumber number, std::string index, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getTransactionByBlockNumberAndIndex(ss.str(), index, err);
}

json RPC::eth_getTransactionReceipt(std::string hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionReceipt", {hash});
}

json RPC::eth_getUncleByBlockHashAndIndex(std::string hash, std::string index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(std::string number, std::string index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(number) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(BigNumber number, std::string index, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getUncleByBlockNumberAndIndex(ss.str(), index, err);
}

json RPC::eth_getCompilers() {
  return _buildJSON("eth_getCompilers");
}

json RPC::eth_newFilter(json filterOptions, Error &err) {
  int errCode = 0;
  [&](){
    if (!filterOptions.count("fromBlock")) {
      filterOptions["fromBlock"] = "latest";
    } else if (!_checkDefaultBlock(filterOptions["fromBlock"])) {
      errCode = 9; return; // Invalid Block Number
    }
    if (!filterOptions.count("toBlock")) {
      filterOptions["toBlock"] = "latest";
    } else if (!_checkDefaultBlock(filterOptions["toBlock"])) {
      errCode = 9; return; // Invalid Block Number
    }
    if (filterOptions.count("address")) {
      if (filterOptions["address"].is_string()) {
        if (!_checkAddress(filterOptions["address"])) {
          errCode = 5; return; // Invalid Address
        }
      } else if (filterOptions["address"].is_array()) {
        for (json address : filterOptions["address"]) {
          std::string addressStr = address.get<std::string>();
          if (!_checkAddress(addressStr)) {
            errCode = 5; return; // Invalid Address
          }
        }
      }
    }
    if (filterOptions.count("topics")) {
      for (json topic : filterOptions["topics"]) {
        std::string topicStr = topic.get<std::string>();
        if (!_checkHexData(topicStr)) { errCode = 4; return; } // Invalid Hex Data
      }
    }
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_newFilter", {filterOptions});
}

json RPC::eth_newBlockFilter() {
  return _buildJSON("eth_newBlockFilter");
}

json RPC::eth_newPendingTransactionFilter() {
  return _buildJSON("eth_newPendingTransactionFilter");
}

json RPC::eth_uninstallFilter(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_uninstallFilter", {filterId});
}

json RPC::eth_getFilterChanges(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getFilterChanges", {filterId});
}

json RPC::eth_getFilterLogs(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getFilterLogs", {filterId});
}

json RPC::eth_getLogs(json filterOptions, Error &err) {
  int errCode = 0;
  [&](){
    if (!filterOptions.count("fromBlock")) {
      filterOptions["fromBlock"] = "latest";
    } else if (!_checkDefaultBlock(filterOptions["fromBlock"])) {
      errCode = 9; return; // Invalid Block Number
    }
    if (!filterOptions.count("toBlock")) {
      filterOptions["toBlock"] = "latest";
    } else if (!_checkDefaultBlock(filterOptions["toBlock"])) {
      errCode = 9; return; // Invalid Block Number
    }
    if (filterOptions.count("address")) {
      if (filterOptions["address"].is_string()) {
        if (!_checkAddress(filterOptions["address"])) {
          errCode = 5; return; // Invalid Address
        }
      } else if (filterOptions["address"].is_array()) {
        for (json address : filterOptions["address"]) {
          std::string addressStr = address.get<std::string>();
          if (!_checkAddress(addressStr)) {
            errCode = 5; return; // Invalid Address
          }
        }
      }
    }
    if (filterOptions.count("topics")) {
      for (json topic : filterOptions["topics"]) {
        std::string topicStr = topic.get<std::string>();
        if (!_checkHexData(topicStr)) { errCode = 4; return; } // Invalid Hex Data
      }
    }
    if (filterOptions.count("blockhash")) {
      if (!_checkHexData(filterOptions["blockhash"])) { errCode = 4; return; } // Invalid Hex Data
      if (!_checkHexLength(filterOptions["blockhash"], 32)) { errCode = 6; return; } // Invalid Hash Length
    }
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getLogs", {filterOptions});
}

json RPC::eth_getWork() {
  return _buildJSON("eth_getWork");
}

json RPC::eth_submitWork(std::string nonce, std::string powHash, std::string digest, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkHexData(nonce) || !_checkHexData(powHash) || !_checkHexData(digest)
    ) { errCode = 4; return; } // Invalid Hex Data
    if (
      !_checkHexLength(nonce, 8) || !_checkHexLength(powHash, 32) || !_checkHexLength(nonce, 32)
    ) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_submitWork", {nonce, powHash, digest});
}

json RPC::eth_submitHashrate(std::string hashrate, std::string id, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkHexData(hashrate) || !_checkHexData(id)
    ) { errCode = 4; return; } // Invalid Hex Data
    if (
      !_checkHexLength(hashrate, 32) || !_checkHexLength(id, 32)
    ) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_submitHashrate", {hashrate, id});
}

json RPC::db_putString(std::string dbName, std::string key, std::string value) {
  return _buildJSON("db_putString", {dbName, key, value});
}

json RPC::db_getString(std::string dbName, std::string key) {
  return _buildJSON("db_getString", {dbName, key});
}

json RPC::db_putHex(std::string dbName, std::string key, std::string value, Error &err) {
  err.setCode((!_checkHexData(value)) ? 4 : 0); // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("db_putHex", {dbName, key, value});
}

json RPC::db_getHex(std::string dbName, std::string key) {
  return _buildJSON("db_getHex", {dbName, key});
}

json RPC::shh_version() {
  return _buildJSON("shh_version");
}

json RPC::shh_post(json whisperPostObject, Error &err) {
  int errCode = 0;
  [&](){
    for (json topic : whisperPostObject["topics"]) {
      std::string topicStr = topic.get<std::string>();
      if (!_checkHexData(topicStr)) { errCode = 4; return; } // Invalid Hex Data
    }
    if (!_checkHexData(whisperPostObject["data"])) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexData(whisperPostObject["priority"])) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexData(whisperPostObject["ttl"])) { errCode = 4; return; } // Invalid Hex Data
    if (whisperPostObject.count("from")) {
      if (!_checkHexData(whisperPostObject["from"])) { errCode = 4; return; } // Invalid Hex Data
      if (!_checkHexLength(whisperPostObject["from"], 65)) { errCode = 6; return; } // Invalid Hash Length
    }
    if (whisperPostObject.count("to")) {
      if (!_checkHexData(whisperPostObject["to"])) { errCode = 4; return; } // Invalid Hex Data
      if (!_checkHexLength(whisperPostObject["to"], 65)) { errCode = 6; return; } // Invalid Hash Length
    }
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_post", {whisperPostObject});
}

json RPC::shh_newIdentity() {
  return _buildJSON("shh_newIdentity");
}

json RPC::shh_hasIdentity() {
  return _buildJSON("shh_hasIdentity");
}

json RPC::shh_newGroup() {
  return _buildJSON("shh_newGroup");
}

json RPC::shh_addToGroup(std::string identityAddress, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(identityAddress)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(identityAddress, 65)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_addToGroup", {identityAddress});
}

json RPC::shh_newFilter(json filterOptions, Error &err) {
  int errCode = 0;
  [&](){
    for (json topic : filterOptions["topics"]) {
      std::string topicStr = topic.get<std::string>();
      if (!_checkHexData(topicStr)) { errCode = 4; return; } // Invalid Hex Data
    }
    if (filterOptions.count("to")) {
      if (!_checkHexData(filterOptions["to"])) { errCode = 4; return; } // Invalid Hex Data
      if (!_checkHexLength(filterOptions["to"], 65)) { errCode = 6; return; } // Invalid Hash Length
    }
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_newFilter", {filterOptions});
}

json RPC::shh_uninstallFilter(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_uninstallFilter", {filterId});
}

json RPC::shh_getFilterChanges(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_getFilterChanges", {filterId});
}

json RPC::shh_getMessages(std::string filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("shh_getMessages", {filterId});
}

