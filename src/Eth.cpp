#include <web3cpp/Eth.h>

std::future<std::string> Eth::getProtocolVersion() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_protocolVersion().dump()
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<json> Eth::isSyncing() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_syncing().dump()
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<std::string> Eth::getCoinbase() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_coinbase().dump()
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<bool> Eth::isMining() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_mining().dump()
    );
    json reqJson = json::parse(req);
    bool ret = reqJson["result"].get<bool>();
    return ret;
  });
}

std::future<std::string> Eth::getHashrate() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_hashrate().dump()
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::getGasPrice() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_gasPrice().dump()
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::vector<std::string>> Eth::getAccounts() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_accounts().dump()
    );
    json reqJson = json::parse(req);
    json reqRes = reqJson["result"].get<std::string>();
    std::vector<std::string> ret;
    for (std::string acc : reqRes) {
      ret.push_back(acc);
    }
    return ret;
  });
}

std::future<std::string> Eth::getBlockNumber() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_blockNumber().dump()
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::getBalance(
  std::string address, std::string defaultBlock
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getBalance(address, defaultBlock, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::getStorageAt(
  std::string address, std::string position, std::string defaultBlock
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getStorageAt(
      address, position, defaultBlock, err
    ).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::getStorageAt(
  std::string address, BigNumber position, std::string defaultBlock
) {
  std::stringstream ss;
  ss << position;
  return getStorageAt(address, ss.str(), defaultBlock);
}

std::future<std::string> Eth::getCode(std::string address, std::string defaultBlock) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getCode(address, defaultBlock, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<json> Eth::getBlock(
  std::string blockHashOrBlockNumber, bool isHash, bool returnTransactionObjects
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getBlockByHash(
        blockHashOrBlockNumber, returnTransactionObjects, err
      ).dump()
      : RPC::eth_getBlockByNumber(
        blockHashOrBlockNumber, returnTransactionObjects, err
      ).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<std::string> Eth::getBlockTransactionCount(
  std::string blockHashOrBlockNumber, bool isHash
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getBlockTransactionCountByHash(blockHashOrBlockNumber, err).dump()
      : RPC::eth_getBlockTransactionCountByNumber(blockHashOrBlockNumber, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::getBlockUncleCount(
  std::string blockHashOrBlockNumber, bool isHash
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getUncleCountByBlockHash(blockHashOrBlockNumber, err).dump()
      : RPC::eth_getUncleCountByBlockNumber(blockHashOrBlockNumber, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<json> Eth::getUncle(
  std::string blockHashOrBlockNumber, std::string uncleIndex,
  bool isHash, bool returnTransactionObjects
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getUncleByBlockHashAndIndex(
        blockHashOrBlockNumber, uncleIndex, err
      ).dump()
      : RPC::eth_getUncleByBlockNumberAndIndex(
        blockHashOrBlockNumber, uncleIndex, err
      ).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<json> Eth::getTransaction(std::string transactionHash) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getTransactionByHash(transactionHash, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<json> Eth::getTransactionFromBlock(
  std::string hashStringOrNumber, bool isHash, std::string indexNumber
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = (isHash)
      ? RPC::eth_getTransactionByBlockHashAndIndex(
        hashStringOrNumber, indexNumber, err
      ).dump()
      : RPC::eth_getTransactionByBlockNumberAndIndex(
        hashStringOrNumber, indexNumber, err
      ).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<json> Eth::getTransactionReceipt(std::string hash) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getTransactionReceipt(hash, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<std::string> Eth::getTransactionCount(
  std::string address, std::string defaultBlock
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getTransactionCount(
      address, defaultBlock, err
    ).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::sign(std::string dataToSign, std::string address) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_sign(address, dataToSign, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<json> Eth::signTransaction(json txObj) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_signTransaction(txObj, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::object();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<std::string> Eth::call(json callObject, std::string defaultBlock) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_call(callObject, defaultBlock, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<std::string> Eth::estimateGas(json callObject) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_estimateGas(callObject, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return std::string("");
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    std::string ret = reqJson["result"].get<std::string>();
    return ret;
  });
}

std::future<json> Eth::getPastLogs(json options) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_getLogs(options, err);
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return json::array();
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    json ret = reqJson["result"];
    return ret;
  });
}

std::future<std::vector<std::string>> Eth::getWork() {
  return std::async([=]{
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, RPC::eth_getWork().dump()
    );
    json reqJson = json::parse(req);
    json reqObj = reqJson["result"].get<std::string>();
    std::vector<std::string> ret;
    for (std::string item : reqObj) {
      ret.push_back(item);
    }
    return ret;
  });
}

std::future<bool> Eth::submitWork(
  std::string nonce, std::string powHash, std::string digest
) {
  return std::async([=]{
    Error err;
    std::string rpcStr = RPC::eth_submitWork(nonce, powHash, digest, err).dump();
    if (err.getCode() != 0) {
      std::cout << "Error: " << __func__ << ": " << err.what() << std::endl;
      return false;
    }
    std::string req = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST, rpcStr
    );
    json reqJson = json::parse(req);
    bool ret = reqJson["result"].get<bool>();
    return ret;
  });
}

uint64_t Eth::getChainId() {
  return this->provider->chainID;
}

std::string Eth::getNodeInfo() {
  return std::string("web3cpp/") + PROJECT_VERSION;
}

