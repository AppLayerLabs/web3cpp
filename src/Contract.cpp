#include <web3cpp/Contract.h>

Contract::Contract(json jsonInterface, std::string address, json options) {
  // Set option defaults, then parse custom options if given
  this->options.jsonInterface = jsonInterface;
  this->options.address = address;
  this->options.data = "";
  this->options.from = "";
  this->options.gasPrice = "";
  this->options.gas = "";
  this->options.handleRevert = false;
  this->options.transactionBlockTimeout = 50;
  this->options.transactionConfirmationBlocks = 24;
  this->options.transactionPollingTimeout = 750;
  this->options.chain = "mainnet";
  this->options.hardfork = "fuji";
  if (options != NULL) {
    if (options.count("jsonInterface")) {
      this->options.jsonInterface = options["jsonInterface"];
    }
    if (options.count("address")) {
      this->options.address = options["address"].get<std::string>();
    }
    if (options.count("data")) {
      this->options.data = options["data"].get<std::string>();
    }
    if (options.count("from")) {
      this->options.from = options["from"].get<std::string>();
    }
    if (options.count("gasPrice")) {
      this->options.gasPrice = options["gasPrice"].get<std::string>();
    }
    if (options.count("gas")) {
      this->options.gas = options["gas"].get<std::string>();
    }
    if (options.count("handleRevert")) {
      this->options.handleRevert = options["handleRevert"].get<bool>();
    }
    if (options.count("transactionBlockTimeout")) {
      this->options.transactionBlockTimeout = options["transactionBlockTimeout"].get<unsigned int>();
    }
    if (options.count("transactionConfirmationBlocks")) {
      this->options.transactionConfirmationBlocks = options["transactionConfirmationBlocks"].get<unsigned int>();
    }
    if (options.count("transactionPollingTimeout")) {
      this->options.transactionPollingTimeout = options["transactionPollingTimeout"].get<unsigned int>();
    }
    if (options.count("chain")) {
      this->options.chain = options["chain"].get<std::string>();
    }
    if (options.count("hardfork")) {
      this->options.hardfork = options["hardfork"].get<std::string>();
    }
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

Contract Contract::clone() {
  json opts;
  opts["jsonInterface"] = this->options.jsonInterface;
  opts["address"] = this->options.address;
  opts["data"] = this->options.data;
  opts["from"] = this->options.from;
  opts["gasPrice"] = this->options.gasPrice;
  opts["gas"] = this->options.gas;
  opts["handleRevert"] = this->options.handleRevert;
  opts["transactionBlockTimeout"] = this->options.transactionBlockTimeout;
  opts["transactionConfirmationBlocks"] = this->options.transactionConfirmationBlocks;
  opts["transactionPollingTimeout"] = this->options.transactionPollingTimeout;
  opts["chain"] = this->options.chain;
  opts["hardfork"] = this->options.hardfork;
  return Contract(this->options.jsonInterface, this->options.address, opts);
}

std::string Contract::operator() (json arguments, std::string function, Error &error) {
  if (!methods.count(function)) { error.setCode(16); return ""; } // ABI Functor Not Found
  if (!arguments.is_array()) { error.setCode(19); return ""; } // ABI Invalid JSON Array
  if (arguments.size() != methods[function].size()) { error.setCode(18); return ""; } // ABI Invalid Arguments Length

  // Streamline all types from function into a string
  std::vector<std::string> funcTypes;
  for (Types t : methods[function]) {
    switch (t) {
      case Types::uint256: funcTypes.push_back("uint256"); break;
      case Types::uint256Arr: funcTypes.push_back("uint256[]"); break;
      case Types::address: funcTypes.push_back("address"); break;
      case Types::addressArr: funcTypes.push_back("address[]"); break;
      case Types::boolean: funcTypes.push_back("bool"); break;
      case Types::booleanArr: funcTypes.push_back("bool[]"); break;
      case Types::bytes: funcTypes.push_back("bytes"); break;
      case Types::bytesArr: funcTypes.push_back("bytes[]"); break;
      case Types::string: funcTypes.push_back("string"); break;
      case Types::stringArr: funcTypes.push_back("string[]"); break;
    }
  }

  // Mount the function header and JSON arguments manually,
  // with the proper formatting both need to go through packMulti()
  std::string func = function + "(";
  json args;
  for (int i = 0; i < funcTypes.size(); i++) {
    func += funcTypes[i] + ",";
    args.push_back({{"t", funcTypes[i]}, {"v", arguments[i]}});
  }
  func.pop_back();  // Remove last ","
  func += ")";

  return Solidity::packMulti(args, error, func);
}

std::string Contract::operator() (std::string function, json arguments, Error &error) {
  // Check if function exists.
  //std::cout << arguments.dump() << std::endl;
  std::string ret = "0x";
  if (!methods.count(function)) { error.setCode(16); return ""; } // ABI Functor Not Found
  if (!arguments.is_array()) { error.setCode(19); return ""; } // ABI Invalid JSON Array
  if (arguments.size() != methods[function].size()) { error.setCode(18); return ""; } // ABI Invalid Arguments Length

  // Create function ABI.
  ret += functors[function];
  uint64_t index = 0;
  std::string arrToAppend;
  uint64_t array_start = 32 * arguments.size();
  while (true) {
    if (index == arguments.size() || index == methods[function].size()) { break; }
    Types argType = methods[function][index];
    bool isArray = false;
    if (isTypeArray(argType)) {
      ret += Utils::padLeft(
        Utils::toHex(boost::lexical_cast<std::string>(array_start)), 64
      );
      isArray = true;
    }
    // Arrays are parsed differently, as we need to encode every argument.
    if (isArray) {
      // Append array size... except for bytes and strings
      if (argType != Types::bytesArr && argType != Types::stringArr) {
        array_start += (32) + 32 * arguments[index].size();
        arrToAppend += Utils::padLeft(
          Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
        );
      }
      // Check arguments
      for (auto item : arguments[index]) {
        // Uint256[]
        if (argType == Types::uint256Arr) {
          if (!std::all_of(item.begin(), item.end(), ::isdigit)) {
            error.setCode(20); return ""; // ABI Invalid Uint256 Array
          }
          arrToAppend += Utils::padLeft(Utils::toHex(std::string(item)), 64);
        }

        // address[]
        if (argType == Types::addressArr) {
          item = Utils::stripHexPrefix(item); // Remove "0x"
          if (!Utils::isAddress(item)) {
            error.setCode(21); return ""; // ABI Invalid Address Array
          }
          arrToAppend += Utils::padLeft(
            Utils::stripHexPrefix(Utils::toLowercaseAddress(item))
          , 64);
        }

        // boolean[]
        if (argType == Types::booleanArr) {
          if (item[0] != '0' && item[0] != '1') {
            error.setCode(22); return ""; // ABI Invalid Boolean Array
          }
          arrToAppend += Utils::padLeft(item, 64);
        }

        // bytes[]
        if (argType == Types::bytesArr) {
          /**
           * Bytes arrays are encoded differently from the others:
           * Encode bytes 0xaaaa and 0xaaaa as first argument of the function (function(bytes[] calldata list)).
           * [0]:  0000000000000000000000000000000000000000000000000000000000000020 // Start of bytes[] <- this locate is global, the offset here is compared to the whole ABI
           * [1]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of items inside bytes[]
           * [2]:  0000000000000000000000000000000000000000000000000000000000000040 // Location of bytes[0] <- this locate is local, it is 64 bytes from the start of the array (item [1])
           * [3]:  0000000000000000000000000000000000000000000000000000000000000080 // Location of bytes[1] <- this locate is local, it is 128 bytes from the start of the array (item [1])
           * [4]:  0000000000000000000000000000000000000000000000000000000000000002 // Quantity of bytes on bytes[0] (2, 'aa' and 'aa')
           * [5]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Raw bytes.
           * [6]:  0000000000000000000000000000000000000000000000000000000000000002 // Same as 4 for bytes[1].
           * [7]:  aaaa000000000000000000000000000000000000000000000000000000000000 // Same as 5 for bytes[1].
           * array_start needs to be encoded relatively to quantity of bytes.
           * Besides that we need to parse bytes larger than 32 bytes.
           * Then, put all bytes items in a string array.
           */
          std::string tmpRet = Utils::padLeft(
            Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
          );
          uint64_t bytesOffSet = 32 * arguments[index].size(); // 32 * quantity of arguments
          std::vector<std::string> bytesTmpVec;
          for (auto item : arguments[index]) {
            bytesTmpVec.push_back(Utils::stripHexPrefix(item));
          }

          // Assert Sizing if missing.
          // example: bytes 0xaaa will be 0aaa00000... in the ABI!.
          for (auto &bytesString : bytesTmpVec) {
            if (bytesString.size() % 2 == 1) { // Odd number, missing a extra char on the first byte.
              bytesString = std::string("0") + bytesString;
            }
          }
          // Split bytes vector into a vector of vectors of 64 chars (32 bytes) max each.
          // Pair of vector of bytes + vector total size.
          std::vector<std::pair<std::vector<std::string>,uint64_t>> bytesVec;
          for (auto bytesString : bytesTmpVec) {
            std::vector<std::string> tmp;
            for (size_t i = 0; i < bytesString.size(); i += 64) {
              tmp.push_back(bytesString.substr(i, 64));
            }
            bytesVec.push_back(std::make_pair(tmp, bytesString.size()/2));
          }

          // Append location of each byte inside the array.
          for (auto bytes : bytesVec) {
            tmpRet += Utils::padLeft(
              Utils::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64
            );
            // Offset by next item size + 32 bytes used to determine item size..
            // Round it to upper (nearest upwards 32 multiple).
            bytesOffSet += (32) + bytes.second;
            dev::bigfloat division = dev::bigfloat(bytesOffSet) / dev::bigfloat(32);
            dev::bigfloat multiplication = boost::multiprecision::ceil(division);
            bytesOffSet = (32) * boost::lexical_cast<uint64_t>(multiplication);
          }
          // Append the byte itself.
          for (auto bytes : bytesVec) {
            tmpRet += Utils::padLeft(
              Utils::toHex(boost::lexical_cast<std::string>(bytes.second)), 64
            );
            for (auto rawBytes : bytes.first) {
              if(!Utils::isHex(rawBytes)) {
                error.setCode(23); return ""; // ABI Invalid Bytes Array
              }
              tmpRet += Utils::padRight(rawBytes, 64);
            }
          }
          // entire message in packets of 32 bytes size
          array_start += (32 * (tmpRet.size()/64)); // 64 chars, 32 bytes
          arrToAppend += tmpRet;
          break; // Exit for loop
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
          std::string tmpRet = Utils::padLeft(
            Utils::toHex(boost::lexical_cast<std::string>(arguments[index].size())), 64
          );
          uint64_t bytesOffSet = 32 * arguments[index].size(); // 32 * quantity of arguments

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
            stringVec.push_back(std::make_pair(tmp, string.size()/2));
          }

          // Append location of each byte inside the array.
          for (auto string : stringVec) {
            tmpRet += Utils::padLeft(
              Utils::toHex(boost::lexical_cast<std::string>(bytesOffSet)), 64
            );
            // Offset by next item size + 32 bytes used to determine item size..
            // Round it to upper (nearest upwards 32 multiple).
            bytesOffSet += (32) + string.second;
            dev::bigfloat division = dev::bigfloat(bytesOffSet) / dev::bigfloat(32);
            dev::bigfloat multiplication = boost::multiprecision::ceil(division);
            bytesOffSet = (32) * boost::lexical_cast<uint64_t>(multiplication);
          }

          // Append the string itself.
          for (auto string : stringVec) {
            tmpRet += Utils::padLeft(
              Utils::toHex(boost::lexical_cast<std::string>(string.second)), 64
            );
            for (auto rawString : string.first) {
              if(!Utils::isHex(rawString)) {
                error.setCode(24); return ""; // ABI Invalid String Array
              }
              tmpRet += Utils::padRight(rawString, 64);
            }
          }
          // Adjust array_start size correctly.
          array_start += (32 * (tmpRet.size()/64)); // 64 chars, 32 bytes
          arrToAppend += tmpRet;
          break; // Exit for loop
        }
      }
    } else {
      // Item is not an array.
      std::string argument = arguments[index].get<std::string>();
      // uint256
      if (argType == Types::uint256) {
          if (!std::all_of(argument.begin(), argument.end(), ::isdigit)) {
            error.setCode(25); return ""; // ABI Invalid Uint256
          }
          ret += Utils::padLeft(Utils::toHex(argument), 64);
      }
      // Address
      if (argType == Types::address) {
        argument = Utils::stripHexPrefix(argument); // Remove "0x"
        if (!Utils::isAddress(argument)) {
          error.setCode(26); return ""; // ABI Invalid Address Array
        }
        ret += Utils::padLeft(
          Utils::stripHexPrefix(Utils::toLowercaseAddress(argument))
        , 64);
      }
      // Boolean
      if (argType == Types::boolean) {
        if (argument[0] != '0' && argument[0] != '1') {
          error.setCode(27); return ""; // ABI Invalid Boolean Array
        }
        ret += Utils::padLeft(argument, 64);
      }
      // Bytes
      if (argType == Types::bytes) {
        ret += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(array_start)), 64,'0');
        // Append a extral 0 to argument if odd.
        argument = Utils::stripHexPrefix(argument);
        if (argument.size() % 2 == 1) {
          argument = std::string("0") + argument;
        }

        std::string tmpStr = Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(argument.size()/2)),64,'0');

        bigfloat division = bigfloat(argument.size()) / 64;
        uint64_t multiplication = boost::lexical_cast<uint64_t>(boost::multiprecision::ceil(division));
        argument = Utils::padRight(argument, (64 * multiplication), '0');
        tmpStr += argument;

        array_start += 32 + (32 * multiplication);
        arrToAppend += tmpStr;
      }
      // String
      if (argType == Types::string) {
        ret += Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(array_start)), 64,'0');
        // Convert string to raw hex.
        argument = Utils::stripHexPrefix(Utils::utf8ToHex(argument));

        std::string tmpStr = Utils::padLeft(Utils::toHex(boost::lexical_cast<std::string>(argument.size()/2)),64,'0');

        bigfloat division = bigfloat(argument.size()) / 64;
        uint64_t multiplication = boost::lexical_cast<uint64_t>(boost::multiprecision::ceil(division));
        argument = Utils::padRight(argument, (64 * multiplication), '0');
        tmpStr += argument;

        array_start += 32 + (32 * multiplication);
        arrToAppend += tmpStr;
      }
    }
    index++;
  }
  ret = ret + arrToAppend;
  return ret;
}

