#include <web3cpp/Account.h>

Account::Account(
  boost::filesystem::path walletPath, std::string __address,
  std::string __derivationPath, bool __isLedger, Utils::Provider *_provider
) : _address(__address), _derivationPath(__derivationPath),
  _isLedger(__isLedger), transactionDb(__address, walletPath.string() + "/transactions"), provider(_provider)
{
  // TODO: Load transaction data.
}

std::future<BigNumber> Account::balance() {
  return std::async([=]{
    BigNumber ret;
    json request;
    request["jsonrpc"] = "2.0";
    request["method"] = "eth_getBalance";
    request["params"] = json::array();
    request["params"].push_back(this->address());
    request["params"].push_back("latest");
    
    auto balanceRequestStr = Net::HTTPRequest(provider, Net::RequestTypes::GET, request.dump(0));
    json balanceRequest = json::parse(balanceRequestStr.get());
    BigNumber balance = Utils::hexToBigNumber(balanceRequest["result"].get<std::string>());
    return ret;
  });
}