#include <web3cpp/RPC.h>

json RPC::_buildJSON(const std::string& method, const json& params) {
  return {{"jsonrpc", "2.0"}, {"method", method}, {"params", params}, {"id", 1}};
}

bool RPC::_checkHexData(const std::string& hex, bool strict) {
  return (strict) ? Utils::isHexStrict(hex) : Utils::isHex(hex);
}

bool RPC::_checkHexLength(const std::string& hex, int hexLength) {
  int start = (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") ? 2 : 0;
  return (hex.substr(start).length() / 2 == hexLength);
}

bool RPC::_checkAddress(const std::string& add) {
  return Utils::isAddress(add);
}

bool RPC::_checkDefaultBlock(const std::string& block) {
  return (
    block == "latest" || block == "earliest" || block == "pending" ||
    Utils::isHexStrict(block)
  );
}

json RPC::web3_clientVersion() {
  return _buildJSON("web3_clientVersion");
}

json RPC::web3_sha3(const std::string& data, Error &err) {
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

json RPC::eth_getBalance(const std::string& address, const std::string& defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBalance", {address, defaultBlock});
}

json RPC::eth_getBalance(const std::string& address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getBalance(address, ss.str(), err);
}

json RPC::eth_getStorageAt(const std::string& address, const std::string& position, const std::string& defaultBlock, Error &err) {
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

json RPC::eth_getStorageAt(const std::string& address, const std::string& position, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getStorageAt(address, position, ss.str(), err);
}

json RPC::eth_getTransactionCount(const std::string& address, const std::string& defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionCount", {address, defaultBlock});
}

json RPC::eth_getTransactionCount(const std::string& address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getTransactionCount(address, ss.str(), err);
}

json RPC::eth_getBlockTransactionCountByHash(const std::string& hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockTransactionCountByHash", {hash});
}

json RPC::eth_getBlockTransactionCountByNumber(const std::string& number, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockTransactionCountByNumber", {number});
}

json RPC::eth_getBlockTransactionCountByNumber(BigNumber number, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getBlockTransactionCountByNumber(ss.str(), err);
}

json RPC::eth_getUncleCountByBlockHash(const std::string& hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleCountByBlockHash", {hash});
}

json RPC::eth_getUncleCountByBlockNumber(const std::string& number, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleCountByBlockNumber", {number});
}

json RPC::eth_getUncleCountByBlockNumber(BigNumber number, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getUncleCountByBlockNumber(ss.str(), err);
}

json RPC::eth_getCode(const std::string& address, const std::string& defaultBlock, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkDefaultBlock(defaultBlock)) { errCode = 9; return; } // Invalid Block Number
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getCode", {address, defaultBlock});
}

json RPC::eth_getCode(const std::string& address, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_getCode(address, ss.str(), err);
}

json RPC::eth_sign(const std::string& address, const std::string& data, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkAddress(address)) { errCode = 5; return; } // Invalid Address
    if (!_checkHexData(data)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_sign", {data});
}

json RPC::eth_signTransaction(const json& txObj, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(txObj["from"]) ||
      (txObj.count("to") && !_checkAddress(txObj["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      (txObj.count("data") && !_checkHexData(txObj["data"])) ||
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

json RPC::eth_sendTransaction(const json& txObj, Error &err) {
  int errCode = 0;
  [&](){
    if (
      !_checkAddress(txObj["from"]) ||
      (txObj.count("to") && !_checkAddress(txObj["to"]))
    ) { errCode = 5; return; } // Invalid Address
    if (
      (txObj.count("data") && !_checkHexData(txObj["data"])) ||
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

json RPC::eth_sendRawTransaction(const std::string& signedTxData, Error &err) {
  err.setCode((!_checkHexData(signedTxData)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_sendRawTransaction", {signedTxData});
}

json RPC::eth_call(const json& callObject, const std::string& defaultBlock, Error &err) {
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

json RPC::eth_call(const json& callObject, BigNumber defaultBlock, Error &err) {
  std::stringstream ss;
  ss << defaultBlock;
  return eth_call(callObject, ss.str(), err);
}

json RPC::eth_estimateGas(const json& callObject, Error &err) {
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

json RPC::eth_getBlockByHash(const std::string& hash, bool returnTransactionObjects, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockByHash", {hash, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(const std::string& number, bool returnTransactionObjects, Error &err) {
  err.setCode((!_checkHexData(number)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getBlockByNumber", {number, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(BigNumber number, bool returnTransactionObjects, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getBlockByNumber(ss.str(), returnTransactionObjects, err);
}

json RPC::eth_getTransactionByHash(const std::string& hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByHash", {hash});
}

json RPC::eth_getTransactionByBlockHashAndIndex(const std::string& hash, const std::string& index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(const std::string& number, const std::string& index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(number) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(BigNumber number, const std::string& index, Error &err) {
  std::stringstream ss;
  ss << number;
  return eth_getTransactionByBlockNumberAndIndex(ss.str(), index, err);
}

json RPC::eth_getTransactionReceipt(const std::string& hash, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getTransactionReceipt", {hash});
}

json RPC::eth_getUncleByBlockHashAndIndex(const std::string& hash, const std::string& index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(hash) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
    if (!_checkHexLength(hash, 32)) { errCode = 6; return; } // Invalid Hash Length
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(const std::string& number, const std::string& index, Error &err) {
  int errCode = 0;
  [&](){
    if (!_checkHexData(number) || !_checkHexData(index)) { errCode = 4; return; } // Invalid Hex Data
  }();
  err.setCode(errCode);
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getUncleByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(BigNumber number, const std::string& index, Error &err) {
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
        for (const json& address : filterOptions["address"]) {
          const std::string& addressStr = address.get<std::string>();
          if (!_checkAddress(addressStr)) {
            errCode = 5; return; // Invalid Address
          }
        }
      }
    }
    if (filterOptions.count("topics")) {
      for (const json& topic : filterOptions["topics"]) {
        const std::string& topicStr = topic.get<std::string>();
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

json RPC::eth_uninstallFilter(const std::string& filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_uninstallFilter", {filterId});
}

json RPC::eth_getFilterChanges(const std::string& filterId, Error &err) {
  err.setCode((!_checkHexData(filterId)) ? 4 : 0);  // Invalid Hex Data
  return (err.getCode() != 0) ? json::object()
    : _buildJSON("eth_getFilterChanges", {filterId});
}

json RPC::eth_getFilterLogs(const std::string& filterId, Error &err) {
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
        for (const json& address : filterOptions["address"]) {
          const std::string& addressStr = address.get<std::string>();
          if (!_checkAddress(addressStr)) {
            errCode = 5; return; // Invalid Address
          }
        }
      }
    }
    if (filterOptions.count("topics")) {
      for (const json& topic : filterOptions["topics"]) {
        const std::string& topicStr = topic.get<std::string>();
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

json RPC::eth_submitWork(const std::string& nonce, const std::string& powHash, const std::string& digest, Error &err) {
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

json RPC::eth_submitHashrate(const std::string& hashrate, const std::string& id, Error &err) {
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

