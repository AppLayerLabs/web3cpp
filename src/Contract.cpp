#include <web3cpp/Contract.h>



Contract::Contract(json jsonInterface, std::string address, json options) {
    if (options == NULL) {

        this->options.jsonInterface = jsonInterface;
        this->options.address = address;

    } else {
        // Parse options
    }
    // Loop through contract...
    for (auto item : jsonInterface) {
        if(item["type"].get<std::string>() == "function") {
            std::string functionName = item["name"].get<std::string>();
            std::string functionAll = functionName + "(";
            for (auto arguments : item["inputs"]) {
                Types argumentType;
                std::string argumentTypeStr = arguments["type"].get<std::string>();
                functionAll += argumentTypeStr + ",";
                bool argFound;
                if (argumentTypeStr == "uint256") {
                    argumentType = Types::uint256;
                    argFound = true;
                }
                if (argumentTypeStr == "uint256[]") {
                    argumentType = Types::uint256Arr;
                    argFound = true;
                }
                if (argumentTypeStr == "address") {
                    argumentType = Types::address;
                    argFound = true;
                }
                if (argumentTypeStr == "address[]") {
                    argumentType = Types::addressArr;
                    argFound = true;
                }
                if(argumentTypeStr == "bool") {
                    argumentType = Types::boolean;
                    argFound = true;
                }
                if(argumentTypeStr == "bool[]") {
                    argumentType = Types::booleanArr;
                    argFound = true;
                }
                if(argumentTypeStr == "bytes") {
                    argumentType = Types::bytes;
                    argFound = true;
                }
                if(argumentTypeStr == "bytes[]") {
                    argumentType = Types::bytesArr;
                    argFound = true;
                }
                if(argumentTypeStr == "string") {
                    argumentType = Types::string;
                    argFound = true;
                }
                if(argumentTypeStr == "string[]") {
                    argumentType = Types::stringArr;
                    argFound = true;
                }

                if (!argFound) {
                  // Check for uint128, uint64... etc types, they are all encoded the same way
                  bool isArray = false;
                  if (argumentTypeStr.find("[]") != std::string::npos) { isArray = true; }

                  if (argumentTypeStr.find("uint") != std::string::npos) {
                    if (isArray) {
                      argumentType = Types::uint256Arr;
                    } else {
                      argumentType = Types::uint256;
                    }
                    argFound = true;
                  }
                  if (argumentTypeStr.find("bytes") != std::string::npos) {
                    if (isArray) {
                      argumentType = Types::bytesArr;
                    } else {
                      argumentType = Types::bytes;
                    }
                    argFound = true;
                  }
                }
                methods[functionName].push_back(argumentType);
                
            }
          functionAll.pop_back(); // Remove last ,
          functionAll += ")";
          functors[functionName] = dev::toHex(dev::sha3(functionAll)).substr(0,8);
        }   
    }
}

bool Contract::isTypeArray(Types const &type) {
  if (type == Types::uint256Arr || type == Types::addressArr || type == Types::booleanArr || type == Types::bytesArr || type == Types::stringArr) {
    return true;
  } else {
    return false;
  }
}

std::string Contract::operator() (std::string function, json arguments, Error &error) {
  // Check if function exists.
  std::cout << arguments.dump() << std::endl;
  std::string ret = "0x";
  if (!methods.count(function)) { 
    error.setCode(16);
    return "";
  }
  if (!arguments.is_array()) {
    error.setCode(19);
    return "";
  }

  if (arguments.size() != methods[function].size()) {
    error.setCode(18);
    return "";
  }

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
      ret += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(array_start)), 64);
      isArray = true;
    } 

    // Arrays are parsed differently, as we need to encode every argument.
    if (isArray) {
        arrToAppend += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())),64);
      for (auto item : arguments[index]) {
        if (argType == Types::uint256Arr) {
          // Check argument...
          array_start += 32 * arguments[index].size();
          std::string number = item;
          if (!std::all_of(number.begin(), number.end(), ::isdigit)) { error.setCode(20); return ""; }
          arrToAppend += Utils::padLeft(Utils::toHex(number), 64);
        }
        if (argType == Types::addressArr) {
          // Check argument...
          array_start += 32 * arguments[index].size();
          std::string address = Utils::stripHexPrefix(item);
          if (!Utils::isAddress(address)) { error.setCode(21); return ""; }
          arrToAppend += Utils::padLeft(address, 64);
        }
        if (argType == Types::booleanArr) {
          // Check argument...
          array_start += 32 * arguments[index].size();
          if (item[0] != "0" || item[0] != "1") { error.setCode(22); return ""; }
          arrToAppend += Utils::padLeft(item, 64);
        }
        if (argType == Types::bytesArr) {
          bytesOffSet = array_start + 32;
          arrToAppend += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())),64);
          // Bytes array are encoded differently compared to other arrays... see the example:
          // Encode bytes 0xaaaa and 0xaaaa as first argument of the function (function(bytes[] calldata list)).
          // [0]:  0000000000000000000000000000000000000000000000000000000000000020 // Start of bytes[]
          // [1]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of items inside bytes[] 
          // [2]:  0000000000000000000000000000000000000000000000000000000000000040 // Location of bytes[0]
          // [3]:  0000000000000000000000000000000000000000000000000000000000000080 // Location of bytes[1]
          // [4]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of bytes on bytes[0] (2, 'aa' and 'aa')
          // [5]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Raw bytes.
          // [6]:  0000000000000000000000000000000000000000000000000000000000000002 // Same as 4 for bytes[1].
          // [7]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Same as 5 for bytes[1].
          // array_start needs to be encoded relatively to quantity of bytes, besides that we need to parse bytes larger than 32 bytes.
          // Get all bytes items to a string array.
          std::vector<std::string> bytesTmpVec;
          for (auto item : arguments[index]) {
            bytesTmpVec.push_back(Utils::stripHexPrefix(item));
          }
          // Split bytes vector to vector of vectors of 64 chars max each.
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
            arrToAppend += Utils::padLeft(dev::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64);
            bytesOffSet += 32 * bytes.second;
          }

          // Append byte itself to arrToAppend.
          for (auto bytes : bytesVec) {
            arrToAppend += Utils::padLeft(dev::toHex(boost::lexical_cast<std::string>(bytes.second)), 64);
            for (auto rawBytes : bytes.first) {
              if(Utils::isHex(rawBytes)) { error.setCode(23); return ""; }
              arrToAppend += Utils::padRight(rawBytes, 64);
            }
          }
          // Exit for loop.
          break;
        }

        if (argType == Types::stringArr) {
          // String arrays looks very similar to byte arrays...
          // See example function addToAddressListArr(address[] calldata list) external
          // ["aaaaaaaaaa","bbbbbbbbbbbb"]
          // https://testnet.snowtrace.io/tx/0x6eb2870d4ecd49c4c1f35c01a2a21d963848f02b39320feb4fdf4a266f1f8779
          // [0]:  0000000000000000000000000000000000000000000000000000000000000020 // Start of string array
          // [1]:  0000000000000000000000000000000000000000000000000000000000000002 // Size of string array
          // [2]:  0000000000000000000000000000000000000000000000000000000000000040 // Location of string[0]
          // [3]:  0000000000000000000000000000000000000000000000000000000000000080 // Location of string[1]
          // [4]:  000000000000000000000000000000000000000000000000000000000000000a // Size of string[0]
          // [5]:  6161616161616161616100000000000000000000000000000000000000000000 // String[0]
          // [6]:  000000000000000000000000000000000000000000000000000000000000000c // Size of string[1]
          // [7]:  6262626262626262626262620000000000000000000000000000000000000000 // String[1]
          bytesOffSet = array_start + 32;
          arrToAppend += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())),64);

          // Array start also needs to be encoded correctly, let's do the same parsing we do on bytes but with extra steps to convert the
          // raw string to a literal hex string.
          std::vector<std::string> stringTmpVec;
          for (auto item : arguments[index]) {
            stringTmpVec.push_back(Utils::stripHexPrefix(Utils::utf8ToHex(item)));
          }
          // Split string vector to vector of vectors of 64 chars max each.
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
            arrToAppend += Utils::padLeft(dev::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64);
            bytesOffSet += 32 * string.second;
          }

          // Append string itself to arrToAppend.
          for (auto string : stringVec) {
            arrToAppend += Utils::padLeft(dev::toHex(boost::lexical_cast<std::string>(string.second)), 64);
            for (auto rawString : string.first) {
              if(Utils::isHex(rawString)) { error.setCode(24); return ""; }
              arrToAppend += Utils::padRight(rawString, 64);
            }
          }
          // Exit for loop.
          break;
        }
      }
    }
    ++index;
  }

  ret = ret + arrToAppend;
  return ret;
}