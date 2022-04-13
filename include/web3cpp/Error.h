#ifndef ERROR_H
#define ERROR_H


#include <map>
#include <string>

// Mapping error code -> error string
const std::map<uint64_t,std::string> errorCodes {
  {0, "No error"},
  {999, "Unknown Error"}
};

class Error {
  private:
    uint64_t code;
    std::string message;
    bool isSet = false;
  public:
    void setErrorCode(uint64_t errorCode);
    std::string what();


    bool operator==(uint64_t errorCode) const {
      return this->code == errorCode;
    }
};

#endif // ERROR_H