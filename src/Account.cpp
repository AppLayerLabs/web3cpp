#include <web3cpp/Account.h>

Account::Account (boost::filesystem::path walletPath, std::string __address, std::string __derivationPath, bool __isLedger) : 
_address(__address), _derivationPath(__derivationPath), _isLedger(__isLedger), transactionDb(__address, walletPath.string() + "/transactions") {
  
  // TODO: Load transaction data.
}