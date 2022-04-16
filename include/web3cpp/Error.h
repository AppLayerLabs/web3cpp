#ifndef ERROR_H
#define ERROR_H

#include <map>
#include <mutex>
#include <string>

// Class for handling error codes.

class Error {
  private:
    const std::map<uint64_t,std::string> codeMap {
      {0, "No Error"},
      {1, "Incorrect Password"},
      {2, "Forbidden Account Name"},
      {3, "Account Name Exists"},
      {4, "Invalid Hex Data"},
      {5, "Invalid Address"},
      {6, "Invalid Hash Length"},
      {7, "Key Derivation Failed"},
      {8, "Key Encryption Failed"},
      {9, "Invalid Block Number"},
      {999, "Unknown Error"},
    };
    uint64_t code;
    std::string message;
    bool isSet = false;
    std::mutex lock;

  public:
    void setErrorCode(uint64_t errorCode);
    std::string what();

    // Operators
    bool operator==(uint64_t errorCode) const {
      return this->code == errorCode;
    }

    bool operator!=(uint64_t errorCode) const {
      return this->code != errorCode;
    }

    bool operator!() const {
      return this->code != 0;
    }
};

#endif // ERROR_H
