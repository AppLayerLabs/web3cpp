#ifndef UTILS_H
#define UTILS_H

#include <string>

// Module that provides utility functions.
// https://web3js.readthedocs.io/en/v1.7.0/web3-utils.html

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
    // Generates a cryptographically strong pseudo-random HEX string from a given byte size.
    // e.g. a size of 32 will result in a 32-byte HEX string,
    // which is a 64-char string prefixed with "0x".
    std::string randomHex(unsigned int size);

    // Calculates the sha3 of the input.
    // To mimic the sha3 behaviour of Solidity use soliditySha3().
    // keccak256() is an alias of sha3().
    // sha3Raw() returns the has value instead of NULL if an empty string
    // is passed, for example.
    std::string sha3(std::string string);
    std::string keccak256(std::string string);
    std::string sha3Raw(std::string string);

    // Calculates the sha3 of given input parameters in the same way Solidity would.
    // This means arguments will be ABI converted and tightly packed before being hashed.
    // Same details as above.
    //std::string soliditySha3(mixed param1 [, mixed param2, ...]);
    //std::string soliditySha3Raw(mixed param1 [, mixed param2, ...]);

    // Checks if a given string is a HEX string.
    // isHexStrict expects the "0x" prefix.
    bool isHex(std::string hex);
    bool isHexStrict(std::string hex);

    // Checks if a given string is a valid address.
    // It will also check the checksum, if the address has upper and
    // lowercase letters.
    bool isAddress(std::string address);

    // Converts an upper or lowercase address to a checksum address.
    std::string toChecksumAddress(std::string address);

    // Checks the checksum of a given address. Will also return false on
    // non-checksum addresses.
    bool checkAddressChecksum(std::string address);

    // Converts any given value to HEX.
    // Number strings will be interpreted as numbers.
    // Text strings will be interpreted as UTF-8 strings.
    //std::string toHex(mixed);

    // Returns the provided string without the "0x" prefix.
    std::string stripHexPrefix(std::string str);

    // Returns the number representation of a given HEX value as a string.
    std::string hexToNumberString(std::string hex);

    // Returns the number representation of a given HEX value.
    // Not useful for big numbers.
    int hexToNumber(std::string hex);

    // Returns the HEX representation of a given number value.
    //std::string numberToHex(number);

    // Returns the UTF-8 string representation of a given HEX value.
    // hexToString() is an alias of hexToUtf8().
    std::string hexToUtf8(std::string hex);
    std::string hexToString(std::string hex);

    // Returns the ASCII string representation of a given HEX value.
    std::string hexToAscii(std::string hex);

    // Returns the HEX representation of a given UTF-8 string.
    // stringToHex() is an alias of utf8ToHex().
    std::string utf8ToHex(std::string hex);
    std::string stringToHex(std::string hex);

    // Returns the HEX representation of a given ASCII string.
    std::string asciiToHex(std::string hex);

    // Returns a byte array from the given HEX string.
    //bytes[] hexToBytes(hex);

    // Returns a HEX string form a byte array.
    //std::string bytesToHex(byteArray);

    // Converts any value to/from Wei, respectively.
    //std::string toWei(std::string number, std::string unit);
    //std::string fromWei(std::string number, std::string unit);

    // Shows all possible values and their amounts in Wei.
    //jsonObj unitMap();

    // Adds a padding on the left or right of a string, respectively.
    // Useful for adding paddings to HEX strings.
    // leftPad() and rightPad() are aliases of padLeft() and padRight().
    std::string padLeft(std::string string, unsigned int characterAmount, std::string sign = "0");
    std::string leftPad(std::string string, unsigned int characterAmount, std::string sign = "0");
    std::string padRight(std::string string, unsigned int characterAmount, std::string sign = "0");
    std::string rightPad(std::string string, unsigned int characterAmount, std::string sign = "0");

    // Converts a negative number into a two's complement.
    // Returns the converted HEX string.
    //std::string toTwosComplement(number);
};

#endif  // UTILS_H
