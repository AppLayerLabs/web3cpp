#include <web3cpp/Solidity.h>

std::string Solidity::packFunction(std::string func) {
  return dev::toHex(dev::sha3(func)).substr(0, 8);
}

std::string Solidity::packUint(std::string num) {
  return Utils::padLeft(Utils::toHex(num), 64);
}

std::string Solidity::packUint(BigNumber num) {
  return Utils::padLeft(Utils::toHex(num), 64);
}

std::string Solidity::packAddress(std::string add) {
  return Utils::padLeft(
    Utils::stripHexPrefix(Utils::toLowercaseAddress(add))
  , 64);
}

std::string Solidity::packBool(bool b) {
  return Utils::padLeft(((b) ? "1" : "0"), 64);
}

std::string Solidity::packBool(std::string b) {
  if (b == "true") b = "1";
  else if (b == "false") b = "0";
  return Utils::padLeft(b, 64);
}

std::string Solidity::packBytes(std::string hex) {
  std::string hexStrip, hexOffset, hexLength, hexData = "";
  int padding = 0;
  hexStrip = Utils::stripHexPrefix(hex);
  hexOffset = Utils::padLeft(Utils::toHex(32), 64);
  hexLength = Utils::padLeft(Utils::toHex(hexStrip.length() / 2), 64);
  do { padding += 64; } while (padding < hexStrip.length());
  hexData = Utils::padRight(hexStrip, padding);
  return hexOffset + hexLength + hexData;
}

std::string Solidity::packString(std::string str) {
  std::string strStrip, strOffset, strLength, strData = "";
  int padding = 0;
  strStrip = Utils::stripHexPrefix(Utils::utf8ToHex(str));
  strOffset = Utils::padLeft(Utils::toHex(32), 64);
  strLength = Utils::padLeft(Utils::toHex(strStrip.length() / 2), 64);
  do { padding += 64; } while (padding < strStrip.length());
  strData = Utils::padRight(strStrip, padding);
  return strOffset + strLength + strData;
}

std::string Solidity::packUintArray(std::vector<std::string> numV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(numV.size()), 64);
  for (std::string num : numV) {
    arrData += Utils::padLeft(Utils::toHex(num), 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packUintArray(std::vector<BigNumber> numV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(numV.size()), 64);
  for (BigNumber num : numV) {
    arrData += Utils::padLeft(Utils::toHex(num), 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packAddressArray(std::vector<std::string> addV) {
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

std::string Solidity::packBoolArray(std::vector<bool> bV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(bV.size()), 64);
  for (bool b : bV) {
    arrData += Utils::padLeft(((b) ? "1" : "0"), 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packBoolArray(std::vector<std::string> bV) {
  std::string arrOffset, arrSize, arrData = "";
  arrOffset = Utils::padLeft(Utils::toHex(32), 64);
  arrSize = Utils::padLeft(Utils::toHex(bV.size()), 64);
  for (std::string b : bV) {
    if (b == "true") b = "1";
    else if (b == "false") b = "0";
    arrData+= Utils::padLeft(b, 64);
  }
  return arrOffset + arrSize + arrData;
}

std::string Solidity::packBytesArray(std::vector<std::string> hexV) {
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

std::string Solidity::packStringArray(std::vector<std::string> strV) {
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

std::string Solidity::packMulti(json args, std::string func) {
  std::string ret = "0x" + (
    (!func.empty()) ? dev::toHex(dev::sha3(func)).substr(0, 8) : ""
  );
  uint64_t nextOffset = 32 * args.size();
  std::string arrToAppend = "";

  for (json arg : args) {
    // TODO: error handling for "type"/"t" and "value"/"v"
    std::string type = (arg.contains("t") ? arg["t"] : arg["type"]);
    json value = (arg.contains("v") ? arg["v"] : arg["value"]);

    if (type.find("[") != std::string::npos) {  // Type is array
      std::string arrType = type.substr(0, type.find("["));
      ret += Utils::padLeft(Utils::toHex(nextOffset), 64);  // Array offset
      if (arrType != "bytes" && arrType != "string") {
        nextOffset += 64 * arg.size();  // In chars
      }
      if (arrType == "uint256") {
        // TODO: error handling
        arrToAppend += packUintArray(value.get<std::vector<std::string>>()).substr(64);
      } else if (arrType == "address") {
        // TODO: error handling
        arrToAppend += packAddressArray(value.get<std::vector<std::string>>()).substr(64);
      } else if (arrType == "bool") {
        // TODO: error handling
        arrToAppend += packBoolArray(value.get<std::vector<std::string>>()).substr(64);
      } else if (arrType == "bytes" || arrType == "string") {
        // TODO: error handling
        std::string packed = (arrType == "bytes")
          ? packBytesArray(value.get<std::vector<std::string>>()).substr(64)
          : packStringArray(value.get<std::vector<std::string>>()).substr(64);
        nextOffset += 32 * (packed.length() / 64); // In bytes
        arrToAppend += packed;
      }
    } else {  // Type is not array
      if (type == "uint256") {
        // TODO: error handling
        ret += packUint(value.get<std::string>());
      } else if (type == "address") {
        // TODO: error handling
        ret += packAddress(value.get<std::string>());
      } else if (type == "bool") {
        // TODO: error handling
        ret += packBool(value.get<std::string>());
      } else if (type == "bytes" || type == "string") {
        // TODO: error handling
        ret += Utils::padLeft(Utils::toHex(nextOffset), 64);
        std::string packed = (type == "bytes")
          ? packBytes(value).substr(64) : packString(value).substr(64);
        nextOffset += 32 * (packed.length() / 64);
        arrToAppend += packed;
      }
    }
  }

  ret += arrToAppend;
  return ret;
}

