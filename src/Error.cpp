#include <web3cpp/Error.h>

void Error::setErrorCode(uint64_t errorCode) {
  this->lock.lock();
  if (isSet) { throw "Error already set"; };
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

std::string Error::what() {
  this->lock.lock();
  std::string ret = this->message;
  this->lock.lock();
  return ret;
}

