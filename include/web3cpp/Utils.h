#ifndef UTILS_H
#define UTILS_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <sstream>
#include <string>
#include <mutex>
#include <regex>

#include <boost/filesystem.hpp>
#include <boost/nowide/filesystem.hpp>
#include <boost/nowide/fstream.hpp>
#include <openssl/rand.h>

#ifdef __MINGW32__
#include <winsock2.h> // Windows.h asked for winsock2 to be included.
#include <windows.h>
#include <io.h>
#include <shellapi.h>
#include <shlobj.h>
#endif

#include <nlohmann/json.hpp>
#include <web3cpp/devcore/CommonIO.h>
#include <web3cpp/devcore/SHA3.h>
#include <web3cpp/ethcore/Common.h>
#include <web3cpp/ethcore/TransactionBase.h>
#include <web3cpp/Error.h>

using json = nlohmann::ordered_json;
using BigNumber = dev::u256;

/**
 * Conversion template for usage with boost::lexical_cast.
 * Rounds number to nearest multiple.
 * e.g. boost::lexical_cast<HexTo<int>>(var);
 */
template <typename ElemT>
struct HexTo {
  ElemT value;
  operator ElemT() const { return value; }
  friend std::istream& operator>>(std::istream& in, HexTo& out) {
    in >> std::hex >> out.value;
    return in;
  }
};

/**
 * Namespace that provides utility functions.
 * https://web3js.readthedocs.io/en/v1.7.0/web3-utils.html
 */

namespace Utils {
  /**
   * Handle the web3cpp history storage directory.
   * Default data dir paths are as follows:
   * - Windows: C:\Users\Username\AppData\Roaming\web3cpp
   * - Unix: ~/.web3cpp
   */
  #ifdef __MINGW32__
  boost::filesystem::path GetSpecialFolderPath(int nFolder, bool fCreate = true);
  #endif
  boost::filesystem::path getDefaultDataDir();

  /**
   * Rounds up a number to its nearest multiple.
   * e.g. roundUp(65,64) = 128
   * Adapted from https://stackoverflow.com/a/3407254
   */
  uint64_t roundUp(uint64_t num, uint64_t mul);

  /**
   * Generates a cryptographically strong pseudo-random HEX string
   * from a given HEX byte size (e.g. a size of 4 will result in a
   * 4-byte HEX string like "1a2b3c4d", which is an 8-char string).
   */
  std::string randomHex(unsigned int size, bool prefixed = false);

  /**
   * Convert a number string to BigNumber. Works with DEC and HEX numbers.
   * HEX numbers REQUIRE the "0x" prefix, otherwise they are treated as DEC
   * and will be cut at the first non-number character.
   */
  BigNumber toBN(std::string number);

  /**
   * Calculates the sha3 of the input, as per this site:
   * https://emn178.github.io/online-tools/keccak_256.html
   * To mimic the sha3 behaviour of Solidity use soliditySha3().
   * isNibble, if true, tells the function to treat input as bytes (used in soliditySha3()).
   * keccak256() is an alias of sha3().
   * sha3Raw() returns the hash value instead of empty if an empty string
   * is passed, for example.
   */
  std::string sha3(std::string string, bool isNibble = false);
  std::string keccak256(std::string string, bool isNibble = false);
  std::string sha3Raw(std::string string, bool isNibble = false);

  /**
   * Calculates the sha3 of given input parameters in the same way Solidity would.
   * This means arguments will be ABI converted and tightly packed before being hashed.
   * soliditySha3Raw() returns the hash value instead of empty if an empty
   * string is passed, for example.
   * Check Solidity.h for more details on formatting.
   * Does NOT autodetect types.
   * Returns an empty string in case of error.
   */
  std::string soliditySha3(json params, Error &err);
  std::string soliditySha3Raw(json params, Error &err);

  /**
   * Checks if a given string is a HEX string.
   * isHexStrict expects the "0x" prefix.
   */
  bool isHex(std::string hex);
  bool isHexStrict(std::string hex);

  /**
   * Checks if a given string is a valid address.
   * It will also check the checksum, if the address has upper and
   * lowercase letters.
   */
  bool isAddress(std::string address);

  /**
   * Converts an address to its all-lowercase, all-uppercase and
   * checksum version, respectively.
   * "0x" is automatically added at the beginning of the string.
   * Adapted from https://eips.ethereum.org/EIPS/eip-55
   */
  std::string toLowercaseAddress(std::string address);
  std::string toUppercaseAddress(std::string address);
  std::string toChecksumAddress(std::string address);

  /**
   * Checks the checksum of a given address.
   * Will also return false on non-checksum addresses.
   */
  bool checkAddressChecksum(std::string address);

  /**
   * Converts any given value to HEX. Does NOT prefix with "0x".
   * Number strings will be interpreted as numbers.
   * Text strings will be interpreted as UTF-8 strings.
   */
  std::string toHex(std::string value);
  std::string toHex(BigNumber value);

  // Returns the provided string without the "0x" prefix.
  std::string stripHexPrefix(std::string str);

  // Returns the number representation of a given HEX value as a string.
  std::string hexToNumberString(std::string hex);

  // Returns the number representation of a given HEX value as a BigNumber.
  BigNumber hexToBigNumber(std::string hex);

  /**
   * Returns the UTF-8/ASCII/byte array string representation
   * of a given HEX value, respectively.
   * hexToString() is an alias of hexToUtf8().
   */
  std::string hexToUtf8(std::string hex);
  std::string hexToString(std::string hex);
  std::string hexToAscii(std::string hex);
  std::vector<unsigned int> hexToBytes(std::string hex);

  /**
   * Returns the HEX representation of a given
   * UTF-8/ASCII/byte array string, respectively.
   * stringToHex() is an alias of utf8ToHex().
   */
  std::string utf8ToHex(std::string str);
  std::string stringToHex(std::string str);
  std::string asciiToHex(std::string str);
  std::string bytesToHex(std::vector<unsigned int> byteArray);

  /**
   * Converts any amount to/from Wei, respectively.
   * decimals is the number of decimals said amount has (for toWei),
   * and the number of decimals you want the amount to have (for fromWei).
   * For example:
   * - toWei("5", 2) implies a 2 decimal currency -> 5.00 = 500 Wei
   * - fromWei("80000", 4) implies a 4 decimal currency -> 80000 Wei = 8.0000
   * amounts in BigNumber are overloads converted to hex, then string.
   */
  std::string toWei(std::string amount, int decimals);
  std::string toWei(BigNumber amount, int decimals);
  std::string fromWei(std::string amount, int decimals);
  std::string fromWei(BigNumber amount, int decimals);

  /**
   * Adds a padding on the left or right of a string, respectively.
   * Useful for adding paddings to HEX strings.
   * If characterAmount happens to be less than the string's original size,
   * the string will be returned untouched.
   * leftPad() and rightPad() are aliases of padLeft() and padRight().
   */
  std::string padLeft(
    std::string string, unsigned int characterAmount, char sign = '0'
  );
  std::string leftPad(
    std::string string, unsigned int characterAmount, char sign = '0'
  );
  std::string padRight(
    std::string string, unsigned int characterAmount, char sign = '0'
  );
  std::string rightPad(
    std::string string, unsigned int characterAmount, char sign = '0'
  );

  /**
   * Read from/write to a JSON file, respectively.
   * On success, read returns the stringified JSON with the file's contents,
   * and write returns an empty string.
   * On failure, both functions throw an error.
   */
  json readJSONFile(boost::filesystem::path &filePath);
  void writeJSONFile(json &obj, boost::filesystem::path &filePath);

  /**
   * Decode a raw transaction signature.
   * Returns a JSON object with details of the transaction.
   */
  json decodeRawTransaction(std::string signedTx);
};

#endif  // UTILS_H
