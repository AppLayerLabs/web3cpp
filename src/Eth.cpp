#include <web3cpp/Eth.h>

std::future<json> Eth::getProtocolVersion() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_protocolVersion().dump()
    ));
  });
}

std::future<json> Eth::isSyncing() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_syncing().dump()
    ));
  });
}

std::future<json> Eth::getCoinbase() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_coinbase().dump()
    ));
  });
}

std::future<json> Eth::isMining() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_mining().dump()
    ));
  });
}

std::future<json> Eth::getHashrate() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_hashrate().dump()
    ));
  });
}

std::future<json> Eth::getGasPrice() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_gasPrice().dump()
    ));
  });
}

std::future<json> Eth::getAccounts() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_accounts().dump()
    ));
  });
}

std::future<json> Eth::getBlockNumber() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_blockNumber().dump()
    ));
  });
}

std::future<json> Eth::getBalance(std::string address, std::string defaultBlock) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getBalance(address,
      ((!defaultBlock.empty()) ? defaultBlock : this->defaultBlock),
    err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getStorageAt(
  std::string address, std::string position, std::string defaultBlock
) {
  if (position.substr(0, 2) != "0x" || position.substr(0, 2) != "0X") {
    position.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getStorageAt(address, position,
      ((!defaultBlock.empty()) ? defaultBlock : this->defaultBlock),
    err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getStorageAt(
  std::string address, BigNumber position, std::string defaultBlock
) {
  std::stringstream ss;
  ss << std::hex << position;
  return getStorageAt(address, ss.str(), defaultBlock);
}

std::future<json> Eth::getCode(std::string address, std::string defaultBlock) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getCode(address,
      ((!defaultBlock.empty()) ? defaultBlock : this->defaultBlock),
    err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getBlock(
  std::string blockHashOrBlockNumber, bool isHash, bool returnTransactionObjects
) {
  if (
    blockHashOrBlockNumber.substr(0, 2) != "0x" ||
    blockHashOrBlockNumber.substr(0, 2) != "0X"
  ) {
    blockHashOrBlockNumber.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getBlockByHash(
        blockHashOrBlockNumber, returnTransactionObjects, err
      ).dump()
      : RPC::eth_getBlockByNumber(
        blockHashOrBlockNumber, returnTransactionObjects, err
      ).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getBlockTransactionCount(
  std::string blockHashOrBlockNumber, bool isHash
) {
  if (
    blockHashOrBlockNumber.substr(0, 2) != "0x" ||
    blockHashOrBlockNumber.substr(0, 2) != "0X"
  ) {
    blockHashOrBlockNumber.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getBlockTransactionCountByHash(blockHashOrBlockNumber, err).dump()
      : RPC::eth_getBlockTransactionCountByNumber(blockHashOrBlockNumber, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getBlockUncleCount(
  std::string blockHashOrBlockNumber, bool isHash
) {
  if (
    blockHashOrBlockNumber.substr(0, 2) != "0x" ||
    blockHashOrBlockNumber.substr(0, 2) != "0X"
  ) {
    blockHashOrBlockNumber.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getUncleCountByBlockHash(blockHashOrBlockNumber, err).dump()
      : RPC::eth_getUncleCountByBlockNumber(blockHashOrBlockNumber, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getUncle(
  std::string blockHashOrBlockNumber, std::string uncleIndex,
  bool isHash, bool returnTransactionObjects
) {
  if (
    blockHashOrBlockNumber.substr(0, 2) != "0x" ||
    blockHashOrBlockNumber.substr(0, 2) != "0X"
  ) {
    blockHashOrBlockNumber.insert(0, "0x");
  }
  if (uncleIndex.substr(0, 2) != "0x" || uncleIndex.substr(0, 2) != "0X") {
    uncleIndex.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getUncleByBlockHashAndIndex(
        blockHashOrBlockNumber, uncleIndex, err
      ).dump()
      : RPC::eth_getUncleByBlockNumberAndIndex(
        blockHashOrBlockNumber, uncleIndex, err
      ).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getTransaction(std::string transactionHash) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getTransactionByHash(transactionHash, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getTransactionFromBlock(
  std::string hashStringOrNumber, bool isHash, std::string indexNumber
) {
  if (
    hashStringOrNumber.substr(0, 2) != "0x" ||
    hashStringOrNumber.substr(0, 2) != "0X"
  ) {
    hashStringOrNumber.insert(0, "0x");
  }
  if (indexNumber.substr(0, 2) != "0x" || indexNumber.substr(0, 2) != "0X") {
    indexNumber.insert(0, "0x");
  }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getTransactionByBlockHashAndIndex(
        hashStringOrNumber, indexNumber, err
      ).dump()
      : RPC::eth_getTransactionByBlockNumberAndIndex(
        hashStringOrNumber, indexNumber, err
      ).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getTransactionReceipt(std::string hash) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getTransactionReceipt(hash, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getTransactionCount(
  std::string address, std::string defaultBlock
) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getTransactionCount(address,
      ((!defaultBlock.empty()) ? defaultBlock : this->defaultBlock),
    err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::sign(std::string dataToSign, std::string address) {
  if (!Utils::isHex(dataToSign)) { dataToSign = Utils::utf8ToHex(dataToSign); }
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_sign(address, dataToSign, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::signTransaction(json txObj) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_signTransaction(txObj, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::call(json callObject, std::string defaultBlock) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_call(callObject,
      ((!defaultBlock.empty()) ? defaultBlock : this->defaultBlock),
    err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::estimateGas(json callObject) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_estimateGas(callObject, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getPastLogs(json options) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_getLogs(options, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

std::future<json> Eth::getWork() {
  return std::async([=]{
    return json::parse(Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, RPC::eth_getWork().dump()
    ));
  });
}

std::future<json> Eth::submitWork(
  std::string nonce, std::string powHash, std::string digest
) {
  return std::async([=]{
    json ret;
    Error err;
    std::string rpcStr = RPC::eth_submitWork(nonce, powHash, digest, err).dump();
    if (err.getCode() != 0) {
      ret["error"]["message"] = err.what();
    } else {
      ret = json::parse(Net::HTTPRequest(
        this->provider, Net::RequestTypes::POST, rpcStr
      ));
    }
    return ret;
  });
}

uint64_t Eth::getChainId() {
  return this->provider->getChainId();
}

std::string Eth::getNodeInfo() {
  return std::string("web3cpp/") + PROJECT_VERSION;
}

