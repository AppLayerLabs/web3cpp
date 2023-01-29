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

/// @cond
// Doxygen should ignore this
template <typename ElemT>
struct HexTo {
  ElemT value;
  operator ElemT() const { return value; }
  friend std::istream& operator>>(std::istream& in, HexTo& out) {
    in >> std::hex >> out.value;
    return in;
  }
};
/// @endcond

/**
 * Namespace that provides utility functions.
 * Also includes two helper structures:
 * - **BigNumber** - an unsigned 256-bit integer (or more specifically an alias to Aleth's `dev::u256`)
 * - **HexTo** - a conversion template for usage with boost::lexical_cast (e.g. `boost::lexical_cast<HexTo<int>>(var)`)
 */

namespace Utils {
  /**
   * Handle the web3cpp history storage directory.
   * Default data dir paths are as follows:
   * - Windows: `C:\Users\Username\AppData\Roaming\web3cpp`
   * - Linux: `~/.web3cpp`
   * - MacOS: `/Library/Application Support/web3cpp`
   */
  #ifdef __MINGW32__
  boost::filesystem::path GetSpecialFolderPath(int nFolder, bool fCreate = true);
  #endif
  boost::filesystem::path getDefaultDataDir();

  /**
   * Round up a number to its nearest multiple (e.g. `roundUp(65,64)` = **128**).
   * Adapted from [this StackOverflow thread](https://stackoverflow.com/a/3407254).
   * @param num The number to round up.
   * @param mul The multiplier to use as reference for rounding up to.
   * @return The rounded number.
   */
  uint64_t roundUp(uint64_t num, uint64_t mul);

  /**
   * Generate a cryptographically strong pseudo-random hex string.
   * @param size The size in hex bytes (e.g. "1a2b3c4d" has a size of 4, it's
   *             counted as "1a 2b 3c 4d" instead of "1 a 2 b 3 c 4 d").
   * @param prefixed (optional) If set to `true`, will insert "0x" at the
   *                 beginning of the string. Defaults to false.
   */
  std::string randomHex(unsigned int size, bool prefixed = false);

  /**
   * Convert a number string to BigNumber. Works with decimal *and* hex numbers.
   * @param number The number to convert. Hex numbers REQUIRE the "0x" prefix,
   *               otherwise they are treated as decimal and will be cut at the
   *               first non-digit character.
   * @return The number string in BigNumber format.
   */
  BigNumber toBN(const std::string& number);

  /**
   * Calculate the Keccak256/SHA3 hash of a string.
   * Hashing is done like on [this website](https://emn178.github.io/online-tools/keccak_256.html).
   * To mimic %Solidity's behaviour use soliditySha3().
   * @param string The string to be hashed.
   * @param isNibble (optional) If `true`, treats input as raw bytes. Defaults to false.
   * @return The hash of the string.
   */
  std::string sha3(const std::string& string, bool isNibble = false);

  std::string keccak256(const std::string& string, bool isNibble = false); ///< Alias of sha3().

  /// Same as sha3(), but returns the hash value instead of empty if an empty string is passed.
  std::string sha3Raw(const std::string& string, bool isNibble = false);

  /**
   * Calculate the Keccak256/SHA3 of given arguments in the same way %Solidity would.
   * @param params The arguments to be hashed. Will be converted/packed to ABI
   *               format with Solidity::packMulti() before being hashed.
   *               Does NOT autodetect types.
   * @param &err Error object.
   * @return The hash of the packed ABI string, or an empty string in case of error.
   */
  std::string soliditySha3(json params, Error &err);

  /// Same as soliditySha3(), but returns the hash value instead of empty if an empty string is passed.
  std::string soliditySha3Raw(json params, Error &err);

  /**
   * Check if a given string is in hex format.
   * @param hex The string to be checked.
   * @return `true` if the string is in hex format, `false` otherwise.
   */
  bool isHex(const std::string& hex);

  /// Same as isHex() but will return `false` if the "0x" prefix is missing.
  bool isHexStrict(const std::string& hex);

  /**
    * check if a given string contains only numbers
    * @param str The string to be checked.
    * @return `true` if the string contains only numbers, `false` otherwise.
    */
  
  bool isNumber(const std::string &str);
  
  /**
   * Check if a given string is a valid address.
   * @param address The address to be checked. If the address has both upper
   *                *and* lowercase letters, will also check the checksum.
   * @return `true` if the address is valid, `false` otherwise.
   */
  bool isAddress(const std::string& address);

  /**
   * Convert an address to all lowercase.
   * @param address The address to convert.
   * @return The lowercase address.
   */
  std::string toLowercaseAddress(const std::string& address);

  /**
   * Convert an address to all uppercase.
   * @param address The address to convert.
   * @return The uppercase address.
   */
  std::string toUppercaseAddress(const std::string& address);

  /**
   * Convert an address to its [EIP-55](https://eips.ethereum.org/EIPS/eip-55)
   * mixed-case checksum format.
   * @param address The address to convert.
   * @return The checksum address.
   */
  std::string toChecksumAddress(std::string address);

  /**
   * Check the checksum of a given address.
   * @param address The address to check.
   * @return `true` if the checksum is correct, `false` if the checksum is
   *         incorrect or the address is not mixed-case to being with.
   */
  bool checkAddressChecksum(std::string address);

  /**
   * Convert any given value to hex. Does NOT prefix with "0x".
   * Number strings will be interpreted as numbers.
   * Text strings will be interpreted as UTF-8 strings.
   * @param value The value to convert.
   * @return The converted hex value string.
   */
  std::string toHex(const std::string&value);

  /// Overload of toHex() that takes a BigNumber as value.
  std::string toHex(BigNumber value);

  /**
   * Strip the "0x" prefix from a hex string.
   * @param str The hex string to strip.
   * @return The stripped hex string.
   */
  std::string stripHexPrefix(const std::string& str);

  /**
   * Convert a given hex value to a number string.
   * @param hex The hex value to convert.
   * @return The converted value as a hex string.
   */
  std::string hexToNumberString(const std::string& hex);

  /**
   * Convert a given hex value to a BigNumber.
   * @param hex The hex value to convert.
   * @return The converted value as a BigNumber.
   */
  BigNumber hexToBigNumber(std::string hex);

  /**
   * Convert a given hex value to a UTF-8 string.
   * @param hex The hex value to convert.
   * @return The converted UTF-8 string.
   */
  std::string hexToUtf8(std::string hex);

  std::string hexToString(const std::string& hex); ///< Alias of hexToUtf8().

  /**
   * Convert a given hex value to an ASCII string.
   * @param hex The hex value to convert.
   * @return The converted ASCII string.
   */
  std::string hexToAscii(std::string hex);

  /**
   * Convert a given hex value to a byte array.
   * @param hex The hex value to convert.
   * @return The converted byte array.
   */
  std::vector<unsigned int> hexToBytes(std::string hex);

  /**
   * Convert a given UTF-8 string to hex format.
   * @param str The UTF-8 string to convert.
   * @return The converted hex string.
   */
  std::string utf8ToHex(const std::string& str);

  std::string stringToHex(const std::string& str); ///< Alias of utf8ToHex().

  /**
   * Convert a given ASCII string to hex format.
   * @param str The ASCII string to convert.
   * @return The converted hex string.
   */
  std::string asciiToHex(const std::string& str);

  /**
   * Convert a given byte array to hex format.
   * @param byteArray The byte array to convert.
   * @return The converted hex string.
   */
  std::string bytesToHex(const std::vector<unsigned int>& byteArray);

  /**
   * Convert a given currency value in fixed point to Wei.
   * @param amount The amount to convert.
   * @param decimals The number of decimals the amount has. e.g. `toWei("5", 2)`
   *                 implies a 2 decimal currency -> **5.00 = 500 Wei**
   * @return The amount in Wei as a string.
   */
  std::string toWei(const std::string& amount, int decimals);

  /// Overload of toWei() that takes a BigNumber as the amount.
  std::string toWei(const BigNumber& amount, int decimals);

  /**
   * Convert a given currency amount in Wei to fixed point.
   * @param amount The amount to convert.
   * @param decimals The number of decimals the amount should have. e.g. `fromWei("80000", 4)`
   *                 implies a 4 decimal currency -> **80000 Wei = 8.0000**
   * @return The amount in fixed point as a string.
   */
  std::string fromWei(const std::string& amount, int decimals);

  /// Overload of fromWei() that takes a BigNumber as the amount.
  std::string fromWei(const BigNumber& amount, int decimals);

  /**
   * Add a padding to the left of a string.
   * @param string The string to pad.
   * @param characterAmount The total amount of characters the resulting string
   *        should have. If this is less than the string's original size,
   *        the string will remain untouched.
   *        e.g. `padLeft("aaa", 5)` = **"00aaa"**, `padLeft("aaa", 2)` = **"aaa"**.
   * @param sign (optional) The character to use as padding. Defaults to '0'.
   * @return The padded string.
   */
  std::string padLeft(
    std::string string, unsigned int characterAmount, char sign = '0'
  );

  /// Alias of padLeft.
  std::string leftPad(
    std::string string, unsigned int characterAmount, char sign = '0'
  );

  /**
   * Add a padding to the right of a string.
   * @param string The string to pad.
   * @param characterAmount The total amount of characters the resulting string
   *        should have. If this is less than the string's original size,
   *        the string will remain untouched.
   *        e.g. `padRight("aaa", 5)` = **"aaa00"**, `padRight("aaa", 2)` = **"aaa"**.
   * @param sign (optional) The character to use as padding. Defaults to '0'.
   * @return The padded string.
   */
  std::string padRight(
    std::string string, unsigned int characterAmount, char sign = '0'
  );

  /// Alias of padRight.
  std::string rightPad(
    std::string string, unsigned int characterAmount, char sign = '0'
  );

  /**
   * Read from a JSON file.
   * @param &filePath The full path of the JSON file to read from.
   * @param &err Error object.
   * @return A JSON object with the file's contents.
   */
  json readJSONFile(boost::filesystem::path &filePath, Error &err);

  /**
   * Write to a JSON file.
   * @param &obj The JSON object to write.
   * @param &err Error object.
   * @param &filePath The full path of the JSON file to write to.
   */
  void writeJSONFile(json &obj, boost::filesystem::path &filePath, Error &err);

  /**
   * Decode a signed transaction.
   * @param signedTx The raw transaction signature to decode.
   * @return A JSON object with details of the transaction.
   */
  json decodeRawTransaction(std::string signedTx);
};

#endif  // UTILS_H
