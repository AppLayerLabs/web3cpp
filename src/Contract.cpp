#include <web3cpp/Contract.h>

Contract::Contract(json jsonInterface, std::string address, json options) {
  // Parse options
  if (options == NULL) {
    this->options.jsonInterface = jsonInterface;
    this->options.address = address;
  } else {
    ; // TODO: parse options
  }

  // Parse contract
  for (auto item : jsonInterface) {
    if (item["type"].get<std::string>() == "function") {
      std::string functionName = item["name"].get<std::string>();
      std::string functionAll = functionName + "(";
      for (auto arguments : item["inputs"]) {
        Types argType;
        std::string argTypeStr = arguments["type"].get<std::string>();
        functionAll += argTypeStr + ",";
        if (argTypeStr == "uint256") argType = Types::uint256;
        else if (argTypeStr == "uint256[]") argType = Types::uint256Arr;
        else if (argTypeStr == "address") argType = Types::address;
        else if (argTypeStr == "address[]") argType = Types::addressArr;
        else if (argTypeStr == "bool") argType = Types::boolean;
        else if (argTypeStr == "bool[]") argType = Types::booleanArr;
        else if (argTypeStr == "bytes") argType = Types::bytes;
        else if (argTypeStr == "bytes[]") argType = Types::bytesArr;
        else if (argTypeStr == "string") argType = Types::string;
        else if (argTypeStr == "string[]") argType = Types::stringArr;
        else {
          // All uints (128, 64, etc.) are encoded the same way
          if (argTypeStr.find("uint") != std::string::npos) {
            argType = (argTypeStr.find("[]") != std::string::npos)
              ? Types::uint256Arr : Types::uint256;
          } else if (argTypeStr.find("bytes") != std::string::npos) {
            argType = (argTypeStr.find("[]") != std::string::npos)
              ? Types::bytesArr : Types::bytes;
          }
        }
        methods[functionName].push_back(argType);
      }
      functionAll.pop_back(); // Remove last ,
      functionAll += ")";
      functors[functionName] = dev::toHex(dev::sha3(functionAll)).substr(0,8);
    }
  }
}

bool Contract::isTypeArray(Types const &type) {
  return (
    type == Types::uint256Arr || type == Types::addressArr ||
    type == Types::booleanArr || type == Types::bytesArr ||
    type == Types::stringArr
  );
}

std::string Contract::operator() (std::string function, json arguments, Error &error) {
  // Check if function exists.
  std::cout << arguments.dump() << std::endl;
  std::string ret = "0x";
  if (!methods.count(function)) { error.setCode(16); return ""; } // ABI Functor Not Found
  if (!arguments.is_array()) { error.setCode(19); return ""; } // ABI Invalid JSON Array
  if (arguments.size() != methods[function].size()) { error.setCode(18); return ""; } // ABI Invalid Arguments Length

  // Create function ABI.
  ret += functors[function];
  uint64_t index = 0;
  std::string arrToAppend;
  uint64_t array_start = 32 * arguments.size();
  uint64_t bytesOffSet = 0;
  while (true) {
    if (index == arguments.size() || index == methods[function].size()) { break; }
    Types argType = methods[function][index];
    bool isArray;
    if (isTypeArray(argType)) {
      ret += Utils::padLeft(
        Utils::toHex(boost::lexical_cast<std::string>(array_start)), 64
      );
      isArray = true;
    }
    // Arrays are parsed differently, as we need to encode every argument.
    if (isArray) {
      arrToAppend += Utils::padLeft(
        Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
      );
      // Check arguments
      for (auto item : arguments[index]) {
        // Uint256[]
        if (argType == Types::uint256Arr) {
          array_start += 32 * arguments[index].size();
          if (!std::all_of(item.begin(), item.end(), ::isdigit)) {
            error.setCode(20); return ""; // ABI Invalid Uint256 Array
          }
          arrToAppend += Utils::padLeft(Utils::toHex(std::string(item)), 64);
        }

        // address[]
        if (argType == Types::addressArr) {
          array_start += 32 * arguments[index].size();
          item = Utils::stripHexPrefix(item); // Remove "0x"
          if (!Utils::isAddress(item)) {
            error.setCode(21); return ""; // ABI Invalid Address Array
          }
          arrToAppend += Utils::padLeft(item, 64);
        }

        // boolean[]
        if (argType == Types::booleanArr) {
          array_start += 32 * arguments[index].size();
          if (item[0] != "0" || item[0] != "1") {
            error.setCode(22); return ""; // ABI Invalid Boolean Array
          }
          arrToAppend += Utils::padLeft(item, 64);
        }

        // bytes[]
        if (argType == Types::bytesArr) {
          /**
           * Bytes arrays are encoded differently from the others:
           * Encode bytes 0xaaaa and 0xaaaa as first argument of the function (function(bytes[] calldata list)).
           * [0]:  0000000000000000000000000000000000000000000000000000000000000020 // Start of bytes[]
           * [1]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of items inside bytes[]
           * [2]:  0000000000000000000000000000000000000000000000000000000000000040 // Location of bytes[0]
           * [3]:  0000000000000000000000000000000000000000000000000000000000000080 // Location of bytes[1]
           * [4]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of bytes on bytes[0] (2, 'aa' and 'aa')
           * [5]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Raw bytes.
           * [6]:  0000000000000000000000000000000000000000000000000000000000000002 // Same as 4 for bytes[1].
           * [7]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Same as 5 for bytes[1].
           * array_start needs to be encoded relatively to quantity of bytes.
           * Besides that we need to parse bytes larger than 32 bytes.
           * Then, put all bytes items in a string array.
           */
          bytesOffSet = array_start + 32;
          arrToAppend += Utils::padLeft(
            Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
          );
          std::vector<std::string> bytesTmpVec;
          for (auto item : arguments[index]) {
            bytesTmpVec.push_back(Utils::stripHexPrefix(item));
          }
          // Split bytes vector into a vector of vectors of 64 chars max each.
          // Pair of vector of bytes + vector total size.
          std::vector<std::pair<std::vector<std::string>,uint64_t>> bytesVec;
          for (auto bytesString : bytesTmpVec) {
            std::vector<std::string> tmp;
            for (size_t i = 0; i < bytesString.size(); i += 64) {
              tmp.push_back(bytesString.substr(i, 64));
            }
            bytesVec.push_back(std::make_pair(tmp, bytesString.size()));
          }
          // Append location of each byte inside the array.
          for (auto bytes : bytesVec) {
            arrToAppend += Utils::padLeft(
              dev::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64
            );
            bytesOffSet += 32 * bytes.second;
          }
          // Append the byte itself.
          for (auto bytes : bytesVec) {
            arrToAppend += Utils::padLeft(
              dev::toHex(boost::lexical_cast<std::string>(bytes.second)), 64
            );
            for (auto rawBytes : bytes.first) {
              if(Utils::isHex(rawBytes)) {
                error.setCode(23); return ""; // ABI Invalid Bytes Array
              }
              arrToAppend += Utils::padRight(rawBytes, 64);
            }
          }
          break;  // Exit for loop
        }

        // string[]
        if (argType == Types::stringArr) {
          /**
           * String arrays look very similar to byte arrays.
           * See example function addToAddressListArr(address[] calldata list) external
           * ["aaaaaaaaaa","bbbbbbbbbbbb"]
           * https://testnet.snowtrace.io/tx/0x6eb2870d4ecd49c4c1f35c01a2a21d963848f02b39320feb4fdf4a266f1f8779
           * [0]:  0000000000000000000000000000000000000000000000000000000000000020 // Start of string array
           * [1]:  0000000000000000000000000000000000000000000000000000000000000002 // Size of string array
           * [2]:  0000000000000000000000000000000000000000000000000000000000000040 // Location of string[0]
           * [3]:  0000000000000000000000000000000000000000000000000000000000000080 // Location of string[1]
           * [4]:  000000000000000000000000000000000000000000000000000000000000000a // Size of string[0]
           * [5]:  6161616161616161616100000000000000000000000000000000000000000000 // String[0]
           * [6]:  000000000000000000000000000000000000000000000000000000000000000c // Size of string[1]
           * [7]:  6262626262626262626262620000000000000000000000000000000000000000 // String[1]
           */
          bytesOffSet = array_start + 32;
          arrToAppend += Utils::padLeft(
            Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
          );

          // Array start also needs to be encoded correctly.
          // Let's do the same parsing we do on bytes, but with extra steps
          // to convert the raw string to a literal hex string.
          std::vector<std::string> stringTmpVec;
          for (auto item : arguments[index]) {
            stringTmpVec.push_back(Utils::stripHexPrefix(Utils::utf8ToHex(item)));
          }

          // Split string vector into a vector of vectors of 64 chars max each.
          // Pair of vector of strings + vector total size.
          std::vector<std::pair<std::vector<std::string>,uint64_t>> stringVec;
          for (auto string : stringTmpVec) {
            std::vector<std::string> tmp;
            for (size_t i = 0; i < string.size(); i += 64) {
              tmp.push_back(string.substr(i, 64));
            }
            stringVec.push_back(std::make_pair(tmp, string.size()));
          }

          // Append location of each byte inside the array.
          for (auto string : stringVec) {
            arrToAppend += Utils::padLeft(
              dev::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64
            );
            bytesOffSet += 32 * string.second;
          }

          // Append the string itself.
          for (auto string : stringVec) {
            arrToAppend += Utils::padLeft(
              dev::toHex(boost::lexical_cast<std::string>(string.second)), 64
            );
            for (auto rawString : string.first) {
              if(Utils::isHex(rawString)) {
                error.setCode(24); return ""; // ABI Invalid String Array
              }
              arrToAppend += Utils::padRight(rawString, 64);
            }
          }
          break; // Exit for loop
        }
      }
    }
    index++;
  }
  ret = ret + arrToAppend;
  return ret;
}

