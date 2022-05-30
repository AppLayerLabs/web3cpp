#ifndef SOLIDITY_H
#define SOLIDITY_H

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>
#include <web3cpp/Error.h>
#include <web3cpp/Utils.h>

/**
 * Namespace for encoding data like Solidity does.
 * https://docs.soliditylang.org/en/latest/abi-spec.html
 * Supported types: uint256, address, bool, bytes, string (+ arrays), and function.
 * Normal types (except function) are always padded to a multiple of 32 hex bytes.
 * uint256/bool/address are padded to the LEFT.
 * string/bytes are padded to the RIGHT.
 * An easier to digest rationale for encoding each type individually:
 * uint256: data
 * address: data
 * bool: data
 * bytes: offset, length, data
 * string: offset, length, data
 * uint256[]: offset[], size, data...
 * address[]: offset[], size, data...
 * bool[]: offset[], size, data...
 * bytes[]: offset[], size, offset..., (length, data)...
 * string[]: offset[], size, offset..., (length, data)...
 * packMulti expects each arg to be a JSON object, with "type" and "value" (or "t" and "v").
 * ALL VALUES MUST BE INSIDE STRINGS, INCLUDING NUMBERS.
 * Example:
 *   json j = {
 *     { {"type", "string"}, {"value", "234"} },
 *     { {"t", "uint256"}, {"v", "234"} },
 *     { {"t", "bool"}, {"v", "true"} },
 *     { {"t", "bool"}, {"v", "0"} }
 *   };
 */

namespace Solidity {
  bool checkType(std::string type, json value, Error &err);

  std::string packFunction(std::string func);
  std::string packUint(std::string num);
  std::string packAddress(std::string add);
  std::string packBool(std::string b);
  std::string packBytes(std::string hex);
  std::string packString(std::string str);

  std::string packUintArray(std::vector<std::string> numV);
  std::string packAddressArray(std::vector<std::string> addV);
  std::string packBoolArray(std::vector<std::string> bV);
  std::string packBytesArray(std::vector<std::string> hexV);
  std::string packStringArray(std::vector<std::string> strV);

  std::string packMulti(json args, Error &err, std::string func = "");
};

#endif  // SOLIDITY_H
