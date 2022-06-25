#include <web3cpp/Error.h>

const std::map<uint64_t, std::string> Error::codeMap = {
  {0, "No Error"},
  {1, "Incorrect Password"},
  {2, "Database Insert Failed"},
  {3, "Account Exists"},
  {4, "Invalid Hex Data"},
  {5, "Invalid Address"},
  {6, "Invalid Hash Length"},
  {7, "Key Derivation Failed"},
  {8, "Key Encryption Failed"},
  {9, "Invalid Block Number"},
  {10, "Invalid Number"},
  {11, "Transaction Build Error"},
  {12, "Transaction Sign Error"},
  {13, "Transaction Send Error"},
  {14, "Key Derivation Invalid Length"},
  {15, "Key Decryption MAC Mismatch"},
  {16, "Key Decryption Failed"},
  {17, "ABI Functor Not Found"},
  {18, "ABI Invalid Arguments Length"},
  {19, "ABI Invalid JSON Array"},
  {20, "ABI Invalid Uint256 Array"},
  {21, "ABI Invalid Address Array"},
  {22, "ABI Invalid Boolean Array"},
  {23, "ABI Invalid Bytes Array"},
  {24, "ABI Invalid String Array"},
  {25, "ABI Invalid Uint256"},
  {26, "ABI Invalid Address"},
  {27, "ABI Invalid Boolean"},
  {28, "ABI Invalid Bytes"},
  {29, "ABI Invalid String"},
  {30, "ABI Invalid Function"},
  {31, "ABI Unsupported Or Invalid Type"},
  {32, "ABI Missing Type Or Value"},
  {33, "JSON File Does Not Exist"},
  {34, "JSON File Read Error"},
  {35, "JSON File Write Error"},
  {999, "Unknown Error"}
};

void Error::setCode(uint64_t errorCode) {
  this->lock.lock();
  if (isSet) {
    this->lock.unlock();
    std::cout << "Error already set" << std::endl;
    return;
  };
  auto match = codeMap.find(errorCode);
  if (match != codeMap.end()) {
    code = match->first;
    message = match->second;
    isSet = true;
  } else {
    code = 999;
    message = codeMap.find(code)->second;
    isSet = true;
  }
  this->lock.unlock();
}

uint64_t Error::getCode() {
  this->lock.lock();
  uint64_t ret = this->code;
  this->lock.unlock();
  return ret;
}

std::string Error::what() {
  this->lock.lock();
  std::string ret = this->message;
  this->lock.unlock();
  return ret;
}

