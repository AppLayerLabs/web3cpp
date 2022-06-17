#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <map>
#include <mutex>
#include <string>

// Class for handling error codes.

class Error {
  private:
    const std::map<uint64_t,std::string> codeMap {
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
      {999, "Unknown Error"}
    };
    uint64_t code = 0;
    std::string message;
    bool isSet = false;
    std::mutex lock;

  public:
    void setCode(uint64_t errorCode);
    uint64_t getCode();
    std::string what();

    // Operators
    bool operator==(uint64_t code) const { return this->code == code; }
    bool operator!=(uint64_t code) const { return this->code != code; }
    bool operator!() const { return this->code != 0; }
};

#endif // ERROR_H
