#include <web3cpp/Eth.h>

std::future<std::string> Eth::getProtocolVersion() {
  return {}; // TODO
}

std::future<json> Eth::isSyncing() {
  return {}; // TODO
}

std::future<std::string> Eth::getCoinbase() {
  return {}; // TODO
}

std::future<bool> Eth::isMining() {
  return {}; // TODO
}

std::future<BigNumber> Eth::getHashrate() {
  return {}; // TODO
}

std::future<std::string> Eth::getGasPrice() {
  return {}; // TODO
}

std::future<json> Eth::getFeeHistory(
  std::string blockCount, std::string newestBlock,
  std::vector<std::string> rewardPercentiles
) {
  return {}; // TODO
}

std::future<json> Eth::getFeeHistory(
  BigNumber blockCount, std::string newestBlock,
  std::vector<std::string> rewardPercentiles
) {
  std::stringstream ss;
  ss << blockCount;
  return getFeeHistory(ss.str(), newestBlock, rewardPercentiles);
}

std::future<json> Eth::getFeeHistory(
  std::string blockCount, BigNumber newestBlock,
  std::vector<std::string> rewardPercentiles
) {
  std::stringstream ss;
  ss << newestBlock;
  return getFeeHistory(blockCount, ss.str(), rewardPercentiles);
}

std::future<json> Eth::getFeeHistory(
  BigNumber blockCount, BigNumber newestBlock,
  std::vector<std::string> rewardPercentiles
) {
  std::stringstream ss, ss2;
  ss << blockCount;
  ss2 << newestBlock;
  return getFeeHistory(ss.str(), ss2.str(), rewardPercentiles);
}

std::future<std::vector<std::string>> Eth::getAccounts() {
  return {}; // TODO
}

std::future<unsigned int> Eth::getBlockNumber() {
  return {}; // TODO
}

std::future<std::string> Eth::getBalance(std::string address, std::string defaultBlock) {
  return {}; // TODO
}

std::future<std::string> Eth::getStorageAt(
  std::string address, std::string position, std::string defaultBlock
) {
  return {}; // TODO
}

std::future<std::string> Eth::getStorageAt(
  std::string address, BigNumber position, std::string defaultBlock
) {
  std::stringstream ss;
  ss << position;
  return getStorageAt(address, ss.str(), defaultBlock);
}

std::future<std::string> Eth::getCode(std::string address, std::string defaultBlock) {
  return {}; // TODO
}

std::future<json> Eth::getBlock(
  std::string blockHashOrBlockNumber, bool returnTransactionObjects
) {
  return {}; // TODO
}

std::future<unsigned int> Eth::getBlockTransactionCount(std::string blockHashOrBlockNumber) {
  return {}; // TODO
}

std::future<unsigned int> Eth::getBlockUncleCount(std::string blockHashOrBlockNumber) {
  return {}; // TODO
}

std::future<json> Eth::getUncle(
  std::string blockHashOrBlockNumber, bool returnTransactionObjects
) {
  return {}; // TODO
}

std::future<json> Eth::getTransaction(std::string transactionHash) {
  return {}; // TODO
}

std::future<std::vector<json>> Eth::getPendingTransactions() {
  return {}; // TODO
}

std::future<json> Eth::getTransactionFromBlock(
  std::string hashStringOrNumber, unsigned int indexNumber
) {
  return {}; // TODO
}

std::future<json> Eth::getTransactionReceipt(std::string hash) {
  return {}; // TODO
}

std::future<unsigned int> Eth::getTransactionCount(
  std::string address, std::string defaultBlock
) {
  return {}; // TODO
}

std::future<std::string> Eth::sign(std::string dataToSign, std::string address) {
  return {}; // TODO
}

std::future<std::string> Eth::sign(std::string dataToSign, unsigned int address) {
  return {}; // TODO
}

std::future<json> Eth::signTransaction(json txObj, std::string address) {
  return {}; // TODO
}

std::future<std::string> Eth::call(json callObject, std::string defaultBlock) {
  return {}; // TODO
}

std::future<std::string> Eth::estimateGas(json callObject) {
  return {}; // TODO
}

std::future<std::vector<json>> Eth::getPastLogs(json options) {
  return {}; // TODO
}

std::future<std::vector<std::string>> Eth::getWork() {
  return {}; // TODO
}

std::future<bool> Eth::submitWork(
  std::string nonce, std::string powHash, std::string digest
) {
  return {}; // TODO
}

std::future<std::vector<std::string>> Eth::requestAccounts() {
  return {}; // TODO
}

std::future<unsigned int> Eth::getChainId() {
  return {}; // TODO
}

std::future<std::string> Eth::getNodeInfo() {
  return {}; // TODO
}

std::future<json> Eth::getProof(
  std::string address, std::vector<std::string> storageKey, std::string blockNumber
) {
  return {}; // TODO
}

std::future<json> Eth::getProof(
  std::string address, std::vector<BigNumber> storageKey, std::string blockNumber
) {
  std::vector<std::string> vec;
  for (BigNumber bn : storageKey) {
    std::stringstream ss;
    ss << bn;
    vec.push_back(ss.str());
  }
  return getProof(address, vec, blockNumber);
}

std::future<json> Eth::createAccessList(json callObject) {
  return {}; // TODO
}

