#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
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

using json = nlohmann::json;
using BigNumber = dev::u256;

// Module that provides utility functions.
// https://web3js.readthedocs.io/en/v1.7.0/web3-utils.html

namespace Utils {
  // Struct that contains information about a provider.
  struct Provider {
    std::string networkName;
    std::string rpcUrl;
    std::string rpcTarget;
    uint64_t rpcPort;
    uint64_t chainID;
    std::string currencySymbol;
    std::string blockExplorerUrl;
    std::mutex m_lock;
  };

  // Mutex for reading JSON files.
  std::mutex storageLock;

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
   * Bloom filter functions.
   * See https://web3js.readthedocs.io/en/v1.7.0/web3-utils.html#functions
   */
  bool isBloom(std::string bloom);
  bool isUserAddressInBloom(std::string bloom, std::string address);
  bool isContractAddressInBloom(std::string bloom, std::string contractAddress);
  bool isTopic(std::string topic);
  bool isTopicInBloom(std::string bloom, std::string topic);
  bool isInBloom(std::string bloom, std::string value);

  /**
   * Generates a cryptographically strong pseudo-random HEX string from a given byte size.
   * e.g. a size of 32 will result in a 32-byte HEX string,
   * which is a 64-char string prefixed with "0x".
   */
  std::string randomHex(unsigned int size);

  // Convert a number string to BigNumber. Works with DEC and HEX numbers.
  BigNumber toBN(std::string number);

  /**
   * Calculates the sha3 of the input.
   * To mimic the sha3 behaviour of Solidity use soliditySha3().
   * keccak256() is an alias of sha3().
   * sha3Raw() returns the hash value instead of empty if an empty string
   * is passed, for example.
   */
  std::string sha3(std::string string);
  std::string keccak256(std::string string);
  std::string sha3Raw(std::string string);

  /**
   * Calculates the sha3 of given input parameters in the same way Solidity would.
   * This means arguments will be ABI converted and tightly packed before being hashed.
   * Same details as above.
   */
  //std::string soliditySha3(mixed param1 [, mixed param2, ...]);
  //std::string soliditySha3Raw(mixed param1 [, mixed param2, ...]);

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
   * Converts an upper or lowercase address to a checksum address.
   * Adapted from https://github.com/ethereum/EIPs/issues/55
   */
  std::string toChecksumAddress(std::string address);

  /**
   * Checks the checksum of a given address.
   * Will also return false on non-checksum addresses.
   */
  bool checkAddressChecksum(std::string address);

  /**
   * Converts any given value to HEX.
   * Number strings will be interpreted as numbers.
   * Text strings will be interpreted as UTF-8 strings.
   */
  std::string toHex(std::string value);
  std::string toHex(BigNumber value);

  // Returns the provided string without the "0x" prefix.
  std::string stripHexPrefix(std::string str);

  // Returns the number representation of a given HEX value as a string.
  std::string hexToNumberString(std::string hex);

  /**
   * Returns the UTF-8 string representation of a given HEX value.
   * hexToString() is an alias of hexToUtf8().
   */
  std::string hexToUtf8(std::string hex);
  std::string hexToString(std::string hex);

  // Returns the ASCII string representation of a given HEX value.
  std::string hexToAscii(std::string hex);

  /**
   * Returns the HEX representation of a given UTF-8 string.
   * stringToHex() is an alias of utf8ToHex().
   */
  std::string utf8ToHex(std::string str);
  std::string stringToHex(std::string str);

  // Returns the HEX representation of a given ASCII string.
  std::string asciiToHex(std::string str);

  // Returns a byte array from the given HEX string.
  //bytes[] hexToBytes(hex);

  // Returns a HEX string form a byte array.
  //std::string bytesToHex(byteArray);

  /**
   * Converts any amount to/from Wei, respectively.
   * decimals is the number of decimals said amount has (for toWei)
   * and the number of decimals you want the amount to have (for fromWei).
   */
  std::string toWei(std::string amount, int decimals);
  std::string fromWei(std::string amount, int decimals);

  /**
   * Adds a padding on the left or right of a string, respectively.
   * Useful for adding paddings to HEX strings.
   * leftPad() and rightPad() are aliases of padLeft() and padRight().
   */
  std::string padLeft(std::string string, unsigned int characterAmount, std::string sign = "0");
  std::string leftPad(std::string string, unsigned int characterAmount, std::string sign = "0");
  std::string padRight(std::string string, unsigned int characterAmount, std::string sign = "0");
  std::string rightPad(std::string string, unsigned int characterAmount, std::string sign = "0");

  /**
   * Converts a negative number into a two's complement.
   * Returns the converted HEX string.
   */
  //std::string toTwosComplement(number);

  /**
   * Read from/write to a JSON file, respectively.
   * On success, read returns the stringified JSON with the file's contents,
   * and write returns an empty string.
   * On failure, both functions throw an error.
   */
  json readJSONFile(boost::filesystem::path &filePath);
  void writeJSONFile(json &obj, boost::filesystem::path &filePath);
};

#endif  // UTILS_H
