#include <web3cpp/Account.h>

Account::Account(
  const boost::filesystem::path& walletPath, const std::string& __address, const std::string& __name,
  const std::string& __derivationPath, bool __isLedger, Provider *_provider
) : _address(__address), _name(__name), _derivationPath(__derivationPath),
  _isLedger(__isLedger), provider(_provider),
  transactionDB("transactions/" + __address, walletPath)
{
  Error error;
  std::string nonceRequest = Net::HTTPRequest(
    this->provider, Net::RequestTypes::POST,
    RPC::eth_getTransactionCount(_address, "latest", error).dump()
  );
  json nonceJson = json::parse(nonceRequest);
  _nonce = boost::lexical_cast<HexTo<uint64_t>>(nonceJson["result"].get<std::string>());
}

std::future<BigNumber> Account::balance() const {
  return std::async([=]{
    Error error;
    BigNumber ret;
    std::string balanceRequestStr = Net::HTTPRequest(
      this->provider, Net::RequestTypes::POST,
      RPC::eth_getBalance(this->_address, "latest", error).dump()
    );
    if (error.getCode() != 0) {
      std::cout << "Error on getting balance for account " << this->_address
        << ": " << error.what() << std::endl;
      return ret;
    }
    json balanceRequest = json::parse(balanceRequestStr);
    ret = Utils::hexToBigNumber(balanceRequest["result"].get<std::string>());
    return ret;
  });
}

bool Account::saveTxToHistory(std::string signedTx) {
  json txData = Utils::decodeRawTransaction(signedTx);
  return this->transactionDB.putKeyValue(txData["hash"], txData.dump());
}

json Account::getTxHistory() const {
  json ret;
  std::map<std::string, std::string> hist = this->transactionDB.getAllPairs();
  for (std::pair<std::string, std::string> item : hist) {
    ret[item.first] = item.second;
  }
  return ret;
}

