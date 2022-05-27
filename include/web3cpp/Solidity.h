#ifndef SOLIDITY_H
#define SOLIDITY_H

#include <string>
#include <vector>

#include <web3cpp/Utils.h>

/**
 * Namespace for encoding data like Solidity does.
 * https://docs.soliditylang.org/en/latest/abi-spec.html
 * Supported types: uint, address, bool, bytes, string (+ arrays), and function.
 * Normal types (except function) are always padded to a multiple of 32 hex bytes.
 * uint/bool/address are padded to the LEFT.
 * string/bytes are padded to the RIGHT.
 * An easier to digest rationale for encoding each type individually:
 * uint: data
 * address: data
 * bool: data
 * bytes: offset, length, data
 * string: offset, length, data
 * uint[]: offset[], size, data...
 * address[]: offset[], size, data...
 * bool[]: offset[], size, data...
 * bytes[]: offset[], size, offset..., (length, data)...
 * string[]: offset[], size, offset..., (length, data)...
 */

namespace Solidity {
  std::string packFunction(std::string func);

  std::string packUint(std::string num);
  std::string packUint(BigNumber num);

  std::string packAddress(std::string add);

  std::string packBool(bool b);
  std::string packBool(std::string b);

  std::string packBytes(std::string hex);

  std::string packString(std::string str);

  std::string packUintArray(std::vector<std::string> numV);
  std::string packUintArray(std::vector<BigNumber> numV);

  std::string packAddressArray(std::vector<std::string> addV);

  std::string packBoolArray(std::vector<bool> bV);
  std::string packBoolArray(std::vector<std::string> bV);

  std::string packBytesArray(std::vector<std::string> hexV);

  std::string packStringArray(std::vector<std::string> strV);
};

#endif  // SOLIDITY_H
