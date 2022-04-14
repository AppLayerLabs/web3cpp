#include <web3cpp/RPC.h>

json _buildJSON(std::string method, json params) {
  return {
    {"jsonrpc", "2.0"}, {"method", method}, {"params", params}, {"id", 1}
  };
}

json RPC::web3_clientVersion() {
  return _buildJSON("web3_clientVersion");
}

json RPC::web3_sha3(std::string data, Error &err) {
  if (!Utils::isHexStrict(data)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("web3_sha3", {data});
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
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getBalance", {address, defaultBlock});
}

json RPC::eth_getBalance(std::string address, BigNumber defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  std::stringstream ss;
  ss << defaultBlock;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getBalance", {address, ss.str()});
}

json RPC::eth_getStorageAt(std::string address, std::string position, std::string defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  if (!Utils::isHexStrict(position)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: defaultBlock sanity check
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getStorageAt", {address, position, defaultBlock});
}

json RPC::eth_getStorageAt(std::string address, std::string position, BigNumber defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  if (!Utils::isHexStrict(position)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: defaultBlock sanity check
  std::stringstream ss;
  ss << defaultBlock;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getStorageAt", {address, position, ss.str()});
}

json RPC::eth_getTransactionCount(std::string address, std::string defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionCount", {address, defaultBlock});
}

json RPC::eth_getTransactionCount(std::string address, BigNumber defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  std::stringstream ss;
  ss << defaultBlock;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionCount", {address, ss.str()});
}

json RPC::eth_getBlockTransactionCountByHash(std::string hash, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getBlockTransactionCountByHash", {hash});
}

json RPC::eth_getBlockTransactionCountByNumber(std::string number) {
  // TODO: number sanity check
  return _buildJSON("eth_getBlockTransactionCountByNumber", {number});
}

json RPC::eth_getBlockTransactionCountByNumber(BigNumber number) {
  // TODO: number sanity check
  std::stringstream ss;
  ss << number;
  return _buildJSON("eth_getBlockTransactionCountByNumber", {ss.str()});
}

json RPC::eth_getUncleCountByBlockHash(std::string hash, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getUncleCountByBlockHash", {hash});
}

json RPC::eth_getUncleCountByBlockNumber(std::string number) {
  // TODO: number sanity check
  return _buildJSON("eth_getUncleCountByBlockNumber", {number});
}

json RPC::eth_getUncleCountByBlockNumber(BigNumber number) {
  // TODO: number sanity check
  std::stringstream ss;
  ss << number;
  return _buildJSON("eth_getUncleCountByBlockNumber", {ss.str()});
}

json RPC::eth_getCode(std::string address, std::string defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getCode", {address, defaultBlock});
}

json RPC::eth_getCode(std::string address, BigNumber defaultBlock, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  // TODO: defaultBlock sanity check
  std::stringstream ss;
  ss << defaultBlock;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getCode", {address, ss.str()});
}

json RPC::eth_sign(std::string address, std::string data, Error &err) {
  if (!Utils::isAddress(address)) {
    err.setErrorCode(5); // Invalid Address
    return json::object();
  }
  if (!Utils::isHexStrict(data)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_sign", {data});
}

json RPC::eth_signTransaction(json transactionObject) {
  // TODO: transactionObject sanity check (there's a lot)
  return _buildJSON("eth_signTransaction", {transactionObject});
}

json RPC::eth_sendTransaction(json transactionObject) {
  // TODO: transactionObject sanity check (there's a lot)
  return _buildJSON("eth_sendTransaction", {transactionObject});
}

json RPC::eth_sendRawTransaction(std::string signedTxData, Error &err) {
  if (!Utils::isHexStrict(signedTxData)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_sendRawTransaction", {signedTxData});
}

json RPC::eth_call(json callObject, std::string defaultBlock) {
  // TODO: callObject sanity check (there's a lot)
  // TODO: defaultBlock sanity check
  return _buildJSON("eth_call", {callObject, defaultBlock});
}

json RPC::eth_call(json callObject, BigNumber defaultBlock) {
  // TODO: callObject sanity check (there's a lot)
  // TODO: defaultBlock sanity check
  std::stringstream ss;
  ss << defaultBlock;
  return _buildJSON("eth_call", {callObject, ss.str()});
}

json RPC::eth_estimateGas(json callObject) {
  return _buildJSON("eth_estimateGas", {callObject});
}

json RPC::eth_getBlockByHash(std::string hash, bool returnTransactionObjects, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getBlockByHash", {hash, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(std::string number, bool returnTransactionObjects) {
  // TODO: number sanity check
  return _buildJSON("eth_getBlockByNumber", {number, returnTransactionObjects});
}

json RPC::eth_getBlockByNumber(BigNumber number, bool returnTransactionObjects) {
  // TODO: number sanity check
  std::stringstream ss;
  ss << number;
  return _buildJSON("eth_getBlockByNumber", {ss.str(), returnTransactionObjects});
}

json RPC::eth_getTransactionByHash(std::string hash, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionByHash", {hash});
}

json RPC::eth_getTransactionByBlockHashAndIndex(std::string hash, std::string index, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(std::string number, std::string index, Error &err) {
  // TODO: number sanity check
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  return _buildJSON("eth_getTransactionByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getTransactionByBlockNumberAndIndex(BigNumber number, std::string index, Error &err) {
  // TODO: number sanity check
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  std::stringstream ss;
  ss << number;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionByBlockNumberAndIndex", {ss.str(), index});
}

json RPC::eth_getTransactionReceipt(std::string hash, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getTransactionReceipt", {hash});
}

json RPC::eth_getUncleByBlockHashAndIndex(std::string hash, std::string index, Error &err) {
  if (!Utils::isHexStrict(hash)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if block hash always has 32 bytes
  if (hash.length() != 66) { // "0x" + 32 hex bytes (64 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getUncleByBlockHashAndIndex", {hash, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(std::string number, std::string index, Error &err) {
  // TODO: number sanity check
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getUncleByBlockNumberAndIndex", {number, index});
}

json RPC::eth_getUncleByBlockNumberAndIndex(BigNumber number, std::string index, Error &err) {
  // TODO: number sanity check
  if (!Utils::isHexStrict(index)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  std::stringstream ss;
  ss << number;
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getUncleByBlockNumberAndIndex", {ss.str(), index});
}

json RPC::eth_getCompilers() {
  return _buildJSON("eth_getCompilers");
}

json RPC::eth_compileSolidity(std::string sourceCode) {
  // TODO: sourceCode sanity check(? is it even possible?)
  return _buildJSON("eth_compileSolidity", {sourceCode});
}

json RPC::eth_compileLLL(std::string sourceCode) {
  // TODO: sourceCode sanity check(? is it even possible?)
  return _buildJSON("eth_compileLLL", {sourceCode});
}

json RPC::eth_compileSerpent(std::string sourceCode) {
  // TODO: sourceCode sanity check(? is it even possible?)
  return _buildJSON("eth_compileSerpent", {sourceCode});
}

json RPC::eth_newFilter(json filterOptions) {
  // TODO: filterOptions sanity check (there's a lot)
  return _buildJSON("eth_newFilter", {filterOptions});
}

json RPC::eth_newBlockFilter() {
  return _buildJSON("eth_newBlockFilter");
}

json RPC::eth_newPendingTransactionFilter() {
  return _buildJSON("eth_newPendingTransactionFilter");
}

json RPC::eth_uninstallFilter(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_uninstallFilter", {filterId});
}

json RPC::eth_getFilterChanges(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getFilterChanges", {filterId});
}

json RPC::eth_getFilterLogs(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_getFilterLogs", {filterId});
}

json RPC::eth_getLogs(json filterOptions) {
  // TODO: filterOptions sanity check (there's a lot)
  return _buildJSON("eth_getLogs", {filterOptions});
}

json RPC::eth_getWork() {
  return _buildJSON("eth_getWork");
}

json RPC::eth_submitWork(std::string nonce, std::string powHash, std::string digest, Error &err) {
  if (!Utils::isHexStrict(nonce) || !Utils::isHexStrict(powHash) || !Utils::isHexStrict(digest)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if byte size of all params are correct
  // nonce: "0x" + 8 hex bytes (16 chars)
  // powHash: "0x" + 32 hex bytes (64 chars)
  // digest: "0x" + 32 hex bytes (64 chars)
  if (nonce.length() != 18 || powHash.length() != 66 || digest.length() != 66) {
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_submitWork", {nonce, powHash, digest});
}

json RPC::eth_submitHashrate(std::string hashrate, std::string id, Error &err) {
  if (!Utils::isHexStrict(hashrate) || !Utils::isHexStrict(id)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check if byte size of all params are correct
  // hashrate: "0x" + 32 hex bytes (64 chars)
  // id: "0x" + 32 hex bytes (64 chars)
  if (hashrate.length() != 66 || id.length() != 66) { //
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("eth_submitHashrate", {hashrate, id});
}

json RPC::db_putString(std::string dbName, std::string key, std::string value) {
  return _buildJSON("db_putString", {dbName, key, value});
}

json RPC::db_getString(std::string dbName, std::string key) {
  return _buildJSON("db_getString", {dbName, key});
}

json RPC::db_putHex(std::string dbName, std::string key, std::string value, Error &err) {
  if (!Utils::isHexStrict(value)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("db_putHex", {dbName, key, value});
}

json RPC::db_getHex(std::string dbName, std::string key) {
  return _buildJSON("db_getHex", {dbName, key});
}

json RPC::shh_version() {
  return _buildJSON("shh_version");
}

json RPC::shh_post(json whisperPostObject) {
  // TODO: whisperPostObject sanity check (there's a lot)
  return _buildJSON("shh_post", {whisperPostObject});
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
  if (!Utils::isHexStrict(identityAddress)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  // TODO: check identity address length (docs say 60 hex bytes but example adds
  // up to 130 chars, not 120)
  if (identityAddress.length() != 132) { // "0x" + 60 hex bytes (120 chars)
    err.setErrorCode(6); // Invalid Hash Length
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("shh_addToGroup", {identityAddress});
}

json RPC::shh_newFilter(json filterOptions) {
  // TODO: filterOptions sanity check (it's a bit complicated)
  return _buildJSON("shh_newFilter", {filterOptions});
}

json RPC::shh_uninstallFilter(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("shh_uninstallFilter", {filterId});
}

json RPC::shh_getFilterChanges(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("shh_getFilterChanges", {filterId});
}

json RPC::shh_getMessages(std::string filterId, Error &err) {
  if (!Utils::isHexStrict(filterId)) {
    err.setErrorCode(4); // Invalid Hex Data
    return json::object();
  }
  err.setErrorCode(0); // No error
  return _buildJSON("shh_getMessages", {filterId});
}

