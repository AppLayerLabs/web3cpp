#ifndef UTILS_H
#define UTILS_H

#include <string>

// Module that provides utility functions.

// TODO: 
// - Check if we need to implement bloom filters
//   - https://web3js.readthedocs.io/en/v1.7.0/web3-utils.html#bloom-filters
// - Check if we need to implement big number functions
//   - BN(), isBN(), isBigNumber(), toBN()
//   - Will depend on how we deal with big numbers - probably Boost will be used
// - Check if raw hex can be used as input for some functions
//   - isHex(), isHexStrict(), hexToNumberString(), hexToNumber(), hexToBytes()
//   - e.g. isHex(0x1234...)
// - Find a way to deal with mixed params (String|Number|BN|BigNumber|etc.)
// - Decide how to deal with units in toWei(), fromWei() and unitMap()

public class Utils {
  public:
    std::string randomHex(int size);
    std::string sha3(std::string string);
    std::string keccak256(std::string string); // ALIAS of sha3()
    std::string sha3Raw(std::string string);
    //std::string soliditySha3(param1 [, param2, ...]);
    //std::string soliditySha3Raw(param1 [, param2, ...]);
    bool isHex(std::string hex);
    bool isHexStrict(std::string hex);
    bool isAddress(std::string address);
    std::string toChecksumAddress(std::string address);
    bool checkAddressChecksum(std::string address);
    //std::string toHex(mixed);
    std::string stripHexPrefix(std::string str);
    std::string hexToNumberString(std::string hex);
    int hexToNumber(std::string hex); // Not useful for big numbers
    //std::string numberToHex(number);
    std::string hexToUtf8(std::string hex);
    std::string hexToString(std::string hex); // ALIAS of hexToUtf8()
    std::string hexToAscii(std::string hex);
    std::string utf8ToHex(std::string hex);
    std::string stringToHex(std::string hex); // ALIAS of utf8ToHex()
    std::string asciiToHex(std::string hex);
    //bytes[] hexToBytes(hex);
    //std::string bytesToHex(byteArray);
    //std::string toWei(std::string number, std::string unit);
    //std::string fromWei(std::string number, std::string unit);
    //object unitMap();
    std::string padLeft(std::string string, int characterAmount, std::string sign = "0");
    std::string leftPad(std::string string, int characterAmount, std::string sign = "0"); // ALIAS of padLeft()
    std::string padRight(std::string string, int characterAmount, std::string sign = "0");
    std::string rightPad(std::string string, int characterAmount, std::string sign = "0"); // ALIAS of padRight()
    //std::string toTwosComplement(number);
};

#endif  // UTILS_H
