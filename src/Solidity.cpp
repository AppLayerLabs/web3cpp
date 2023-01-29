#include <web3cpp/Solidity.h>

bool Solidity::checkType(const std::string& type, const json& value, Error &err) {
  if (type == "function") {
    // Check both "funcName()" and every type inside the "()"
    std::string hdr = value.get<std::string>();
    if (hdr.find("(") == std::string::npos || hdr.find(")") == std::string::npos) {
      err.setCode(30); return false; // ABI Invalid Function
    }
    hdr.erase(0, hdr.find("(") + 1);
    hdr.replace(hdr.find(")"), 1, ",");
    int pos;
    while ((pos = hdr.find(",")) != std::string::npos) {
      std::string hdrType = hdr.substr(0, pos);
      if (
        hdrType != "uint256" && hdrType != "address" &&
        hdrType != "bool" && hdrType != "bytes" &&
        hdrType != "string" && hdrType != "uint256[]" &&
        hdrType != "address[]" && hdrType != "bool[]" &&
        hdrType != "bytes[]" && hdrType != "string[]"
      ) {
        err.setCode(30); return false; // ABI Invalid Function
      }
      hdr.erase(0, pos + 1);
    }
    err.setCode(0); return true;
  } else if (type == "uint256") {
    std::string it = value.get<std::string>();
    if (!std::all_of(it.begin(), it.end(), ::isdigit)) {
      err.setCode(25); return false; // ABI Invalid Uint256
    }
    err.setCode(0); return true;
  } else if (type == "address") {
    std::string it = value.get<std::string>();
    if (!Utils::isAddress(it)) {
      err.setCode(26); return false; // ABI Invalid Address
    }
    err.setCode(0); return true;
  } else if (type == "bool") {
    std::string it = value.get<std::string>();
    if (it != "0" && it != "1" && it != "true" && it != "false") {
      err.setCode(27); return false; // ABI Invalid Boolean
    }
    err.setCode(0); return true;
  } else if (type == "bytes") {
    if (!Utils::isHex(value.get<std::string>())) {
      err.setCode(28); return false; // ABI Invalid Bytes
    }
    err.setCode(0); return true;
  } else if (type == "string") {
    std::string it = Utils::utf8ToHex(value.get<std::string>());
    if (!Utils::isHex(it)) {
      err.setCode(29); return false; // ABI Invalid String
    }
    err.setCode(0); return true;
  } else if (type == "uint256[]") {
    for (json item : value) {
      std::string it = item.get<std::string>();
      if (!std::all_of(it.begin(), it.end(), ::isdigit)) {
        err.setCode(20); return false; // ABI Invalid Uint256 Array
      }
    }
    err.setCode(0); return true;
  } else if (type == "address[]") {
    for (json item : value) {
      std::string it = item.get<std::string>();
      if (!Utils::isAddress(it)) {
        err.setCode(21); return false; // ABI Invalid Address Array
      }
    }
    err.setCode(0); return true;
  } else if (type == "bool[]") {
    for (json item : value) {
      std::string it = item.get<std::string>();
      if (it != "0" && it != "1" && it != "true" && it != "false") {
        err.setCode(22); return false; // ABI Invalid Boolean Array
      }
    }
    err.setCode(0); return true;
  } else if (type == "bytes[]") {
    for (json item : value) {
      if (!Utils::isHex(item)) {
        err.setCode(23); return false; // ABI Invalid Bytes Array
      }
    }
    err.setCode(0); return true;
  } else if (type == "string[]") {
    for (json item : value) {
      std::string it = Utils::utf8ToHex(item.get<std::string>());
      if (!Utils::isHex(it)) {
        err.setCode(24); return false; // ABI Invalid String Array
      }
    }
    err.setCode(0); return true;
  }
  err.setCode(31); return false;  // ABI Unsupported Or Invalid Type
}

std::string Solidity::packFunction(const std::string& func) {
  return dev::toHex(dev::sha3(func)).substr(0, 8);
}

std::string Solidity::packUint(const std::string& num) {
  return Utils::padLeft(Utils::toHex(num), 64);
}

std::string Solidity::packAddress(const std::string& add) {
  return Utils::padLeft(
    Utils::stripHexPrefix(Utils::toLowercaseAddress(add))
  , 64);
}

std::string Solidity::packBool(const std::string& b) {
  std::string ret;
  if (b == "true") ret = "1";
  else if (b == "false") ret = "0";
  return Utils::padLeft(ret, 64);
}

std::string Solidity::packBytes(const std::string& hex) {
  std::string hexStrip, hexOffset, hexLength, hexData = "";
  int padding = 0;
  hexStrip = Utils::stripHexPrefix(hex);
  hexOffset = Utils::padLeft(Utils::toHex(32), 64);
  hexLength = Utils::padLeft(Utils::toHex(hexStrip.length() / 2), 64);
  do { padding += 64; } while (padding < hexStrip.length());
  hexData = Utils::padRight(hexStrip, padding);
  return hexOffset + hexLength + hexData;
}

std::string Solidity::packString(const std::string& str) {
  std::string strStrip, strOffset, strLength, strData = "";
  int padding = 0;
  strStrip = Utils::stripHexPrefix(Utils::utf8ToHex(str));
  strOffset = Utils::padLeft(Utils::toHex(32), 64);
  strLength = Utils::padLeft(Utils::toHex(strStrip.length() / 2), 64);
  do { padding += 64; } while (padding < strStrip.length());
  strData = Utils::padRight(strStrip, padding);
  return strOffset + strLength + strData;
}

std::string Solidity::packUintArray(const std::vector<std::string> numV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(numV.size()), 64);
  for (std::string num : numV) {
    arrData += Utils::padLeft(Utils::toHex(num), 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packAddressArray(const std::vector<std::string> addV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(addV.size()), 64);
  for (std::string add : addV) {
    arrData += Utils::padLeft(
      Utils::stripHexPrefix(Utils::toLowercaseAddress(add))
    , 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packBoolArray(const std::vector<std::string> bV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(bV.size()), 64);
  for (std::string b : bV) {
    if (b == "true") b = "1";
    else if (b == "false") b = "0";
    arrData += Utils::padLeft(b, 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packBytesArray(const std::vector<std::string> hexV) {
  std::string arrOffset, arrSize = "";
  std::vector<std::string> hexStrip, hexOffset, hexLength, hexData = {};
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(hexV.size()), 64);
  int totalPaddings = 0;
  for (int i = 0; i < hexV.size(); i++) {
    std::string hS, hO, hL, hD = "";
    int padding = 0;
    hS = Utils::stripHexPrefix(hexV[i]);
    if (hS.length() % 2 != 0) hS.insert(0, "0");  // Complete odd bytes ("aaa" = "0aaa")
    hL = Utils::toHex(hS.length() / 2); // Get length first so we can get the right offset
    hO = Utils::toHex((32 * hexV.size()) + (32 * i) + (32 * totalPaddings)); // (offsets) + (lengths) + (datas)
    do { padding += 64; } while (padding < hS.length());
    totalPaddings += padding / 64;
    hD = Utils::padRight(hS, padding);
    hexStrip.push_back(Utils::padLeft(hS, 64));
    hexOffset.push_back(Utils::padLeft(hO, 64));
    hexLength.push_back(Utils::padLeft(hL, 64));
    hexData.push_back(Utils::padRight(hD, 64));
  }
  std::string ret = arrOffset + arrSize;
  for (std::string offset : hexOffset) ret += offset;
  for (int i = 0; i < hexV.size(); i++) {
    ret += hexLength[i] + hexData[i];
  }
  return ret;
}

std::string Solidity::packStringArray(const std::vector<std::string> strV) {
  std::string arrOffset, arrSize = "";
  std::vector<std::string> strStrip, strOffset, strLength, strData = {};
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(strV.size()), 64);
  int totalPaddings = 0;
  for (int i = 0; i < strV.size(); i++) {
    std::string sS, sO, sL, sD = "";
    int padding = 0;
    sS = Utils::stripHexPrefix(Utils::utf8ToHex(strV[i]));
    sL = Utils::toHex(sS.length() / 2); // Get length first so we can get the right offset
    sO = Utils::toHex((32 * strV.size()) + (32 * i) + (32 * totalPaddings)); // (offsets) + (lengths) + (datas)
    do { padding += 64; } while (padding < sS.length());
    totalPaddings += padding / 64;
    sD = Utils::padRight(sS, padding);
    strStrip.push_back(Utils::padLeft(sS, 64));
    strOffset.push_back(Utils::padLeft(sO, 64));
    strLength.push_back(Utils::padLeft(sL, 64));
    strData.push_back(Utils::padRight(sD, 64));
  }
  std::string ret = arrOffset + arrSize;
  for (std::string offset : strOffset) ret += offset;
  for (int i = 0; i < strV.size(); i++) {
    ret += strLength[i] + strData[i];
  }
  return ret;
}

std::string Solidity::packMulti(const json& args, Error &err, const std::string& func) {
  // Handle function ID first if it exists
  std::string ret = "0x";
  if (!func.empty()) {
    Error funcErr;
    if (!checkType("function", func, funcErr)) {
      err.setCode(funcErr.getCode()); return "";
    }
    ret += packFunction(func);
  }
  uint64_t nextOffset = 32 * args.size();
  std::string arrToAppend = "";

  // Treat singular and multiple types differently
  // (one is a single object, the other is an array)
  if (!args.is_array()) {
    // Get type and value, and check if both are valid
    std::string type;
    json value;
    Error argErr;
    if (
      (args.contains("type") && args.contains("value")) ||
      (args.contains("t") && args.contains("v"))
    ) {
      type = (args.contains("t") ? args["t"] : args["type"]);
      value = (args.contains("v") ? args["v"] : args["value"]);
    } else {
      err.setCode(32); return "";  // ABI Missing Type Or Value
    }
    if (!checkType(type, value, argErr)) {
      err.setCode(argErr.getCode()); return "";
    }

    // Parse value according to type
    if (type == "uint256[]") {
      ret += packUintArray(value.get<std::vector<std::string>>());
    } else if (type == "address[]") {
      ret += packAddressArray(value.get<std::vector<std::string>>());
    } else if (type == "bool[]") {
      ret += packBoolArray(value.get<std::vector<std::string>>());
    } else if (type == "bytes[]") {
      ret += packBytesArray(value.get<std::vector<std::string>>());
    } else if (type == "string[]") {
      ret += packStringArray(value.get<std::vector<std::string>>());
    } else if (type == "uint256") {
      ret += packUint(value.get<std::string>());
    } else if (type == "address") {
      ret += packAddress(value.get<std::string>());
    } else if (type == "bool") {
      ret += packBool(value.get<std::string>());
    } else if (type == "bytes") {
      ret += packBytes(value.get<std::string>());
    } else if (type == "string") {
      ret += packString(value.get<std::string>());
    }
  } else {
    for (json arg : args) {
      // Get type and value, and check if both are valid
      std::string type;
      json value;
      Error argErr;
      if (
        (arg.contains("type") && arg.contains("value")) ||
        (arg.contains("t") && arg.contains("v"))
      ) {
        type = (arg.contains("t") ? arg["t"] : arg["type"]);
        value = (arg.contains("v") ? arg["v"] : arg["value"]);
      } else {
        err.setCode(32); return "";  // ABI Missing Type Or Value
      }
      if (!checkType(type, value, argErr)) {
        err.setCode(argErr.getCode()); return "";
      }

      // Parse value according to type
      if (type.find("[") != std::string::npos) {  // Type is array
        std::string arrType = type.substr(0, type.find("["));
        ret += Utils::padLeft(Utils::toHex(nextOffset), 64);  // Array offset
        if (arrType != "bytes" && arrType != "string") {
          nextOffset += 64 * arg.size();  // In chars
        }
        if (arrType == "uint256") {
          arrToAppend += packUintArray(value.get<std::vector<std::string>>()).substr(64);
        } else if (arrType == "address") {
          arrToAppend += packAddressArray(value.get<std::vector<std::string>>()).substr(64);
        } else if (arrType == "bool") {
          arrToAppend += packBoolArray(value.get<std::vector<std::string>>()).substr(64);
        } else if (arrType == "bytes" || arrType == "string") {
          std::string packed = (arrType == "bytes")
            ? packBytesArray(value.get<std::vector<std::string>>()).substr(64)
            : packStringArray(value.get<std::vector<std::string>>()).substr(64);
          nextOffset += 32 * (packed.length() / 64); // Offset in bytes, packed in chars
          arrToAppend += packed;
        }
      } else {  // Type is not array
        std::string val = value.get<std::string>();
        if (type == "uint256") {
          ret += packUint(val);
        } else if (type == "address") {
          ret += packAddress(val);
        } else if (type == "bool") {
          ret += packBool(val);
        } else if (type == "bytes" || type == "string") {
          ret += Utils::padLeft(Utils::toHex(nextOffset), 64);
          std::string packed = (type == "bytes")
            ? packBytes(value).substr(64) : packString(value).substr(64);
          nextOffset += 32 * (packed.length() / 64);
          arrToAppend += packed;
        }
      }
    }
  }

  err.setCode(0);
  ret += arrToAppend;
  return ret;
}

