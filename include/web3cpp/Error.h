#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <map>
#include <mutex>
#include <string>

/**
 * Class for handling custom error codes and messages.
 */

class Error {
  private:
    /**
     * Fixed map for all the known error codes that can be set.
     * List of error codes is as follows:
     * \arg \c 0 - **No %Error**
     * \arg \c 1 - **Incorrect Password**
     * \arg \c 2 - **%Database Insert Failed**
     * \arg \c 3 - **%Account Exists**
     * \arg \c 4 - **Invalid Hex Data**
     * \arg \c 5 - **Invalid Address**
     * \arg \c 6 - **Invalid Hash Length**
     * \arg \c 7 - **Key Derivation Failed**
     * \arg \c 8 - **Key Encryption Failed**
     * \arg \c 9 - **Invalid Block Number**
     * \arg \c 10 - **Invalid Number**
     * \arg \c 11 - **Transaction Build %Error**
     * \arg \c 12 - **Transaction Sign %Error**
     * \arg \c 13 - **Transaction Send %Error**
     * \arg \c 14 - **Key Derivation Invalid Length**
     * \arg \c 15 - **Key Decryption MAC Mismatch**
     * \arg \c 16 - **Key Decryption Failed**
     * \arg \c 17 - **ABI Functor Not Found**
     * \arg \c 18 - **ABI Invalid Arguments Length**
     * \arg \c 19 - **ABI Invalid JSON Array**
     * \arg \c 20 - **ABI Invalid Uint256 Array**
     * \arg \c 21 - **ABI Invalid Address Array**
     * \arg \c 22 - **ABI Invalid Boolean Array**
     * \arg \c 23 - **ABI Invalid Bytes Array**
     * \arg \c 24 - **ABI Invalid String Array**
     * \arg \c 25 - **ABI Invalid Uint256**
     * \arg \c 26 - **ABI Invalid Address**
     * \arg \c 27 - **ABI Invalid Boolean**
     * \arg \c 28 - **ABI Invalid Bytes**
     * \arg \c 29 - **ABI Invalid String**
     * \arg \c 30 - **ABI Invalid Function**
     * \arg \c 31 - **ABI Unsupported Or Invalid Type**
     * \arg \c 32 - **ABI Missing Type Or Value**
     * \arg \c 999 - **Unknown %Error**
     */
    static const std::map<uint64_t, std::string> codeMap;

    uint64_t code = 0;    ///< Code for the set error object.
    std::string message;  ///< Message for the set error object.
    bool isSet = false;   ///< Indicates if error object is already set.
    std::mutex lock;      ///< Mutex for managing read/write access to the error object.

  public:
    /**
     * Set the error code in the object, if it isn't already set.
     * @param errorCode The code that will be set. If it doesn't exist in the code map,
     *                  it'll be automatically set to the last one ("Unknown Error").
     */
    void setCode(uint64_t errorCode);

    /**
     * Get the error code that was set.
     * @return The error code.
     */
    uint64_t getCode();

    /**
     * Get the message for the error code that was set.
     * @return The message string.
     */
    std::string what();

    bool operator==(uint64_t code) const { return this->code == code; } ///< Checks if the error code on two Error objects are equal.
    bool operator!=(uint64_t code) const { return this->code != code; } ///< Checks if the error code on two Error objects are not equal.
    bool operator!() const { return this->code != 0; }                  ///< Checks if the error code on the Error object is different from 0.
};

#endif // ERROR_H
