#include <web3cpp/Account.h>

Account::Account(
  boost::filesystem::path walletPath, std::string __address, std::string __name,
  std::string __derivationPath, bool __isLedger, Provider *_provider
) : _address(__address), _name(__name), _derivationPath(__derivationPath),
  _isLedger(__isLedger), provider(_provider)
{
  Error error;
  std::string nonceRequest = Net::HTTPRequest(
    this->provider, Net::RequestTypes::POST,
    RPC::eth_getTransactionCount(_address, "latest", error).dump()
  );
  json nonceJson = json::parse(nonceRequest);
  _nonce = boost::lexical_cast<HexTo<uint64_t>>(nonceJson["result"].get<std::string>());
}

std::future<BigNumber> Account::balance() {
  return std::async([=]{
    Error error;
    BigNumber ret;
    std::string balanceRequestStr = Net::HTTPRequest(
      this->provider, Net::RequestTypes::GET,
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
