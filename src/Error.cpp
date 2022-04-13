#include <web3cpp/Error.h>




class Error {
  void setErrorCode(uint64_t errorCode) {
    if (isSet) { throw "Error already set"; };
    if (errorCodes.contains(errorCode)) {
      code = errorCode;
      message = errorCodes[code];
      isSet = true;
    } else {
      code = 999;
      message = errorCodes[code];
      isSet = true;
    }
  }

  std::string what() {
    return this->message;
  }
}