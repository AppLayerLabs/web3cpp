#include <web3cpp/Error.h>

void Error::setCode(uint64_t errorCode) {
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

