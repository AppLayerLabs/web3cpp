#include <web3cpp/Account.h>


Account::Account (std::string &__address) : _address(__address) {
  // Load remaining information from DB itself.

  _derivationPath = "";
  _isLedger = false;
}


Account::Account (std::string &__address, std::string &__derivationPath, bool &__isLedger) : _address(__address), _derivationPath(__derivationPath), _isLedger(__isLedger) {}