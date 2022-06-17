#include <web3cpp/Utils.h>
#include <web3cpp/Solidity.h>

std::mutex storageLock;

#ifdef __MINGW32__
boost::filesystem::path Utils::GetSpecialFolderPath(int nFolder, bool fCreate) {
  WCHAR pszPath[MAX_PATH] = L"";
  if (SHGetSpecialFolderPathW(nullptr, pszPath, nFolder, fCreate)) {
    return boost::filesystem::path(pszPath);
  }
  return boost::filesystem::path("");
}
#endif

boost::filesystem::path Utils::getDefaultDataDir() {
  namespace fs = boost::filesystem;
  #ifdef __MINGW32__
    // Windows: C:\Users\Username\AppData\Roaming\web3cpp
    return GetSpecialFolderPath(CSIDL_APPDATA) / "web3cpp";
  #else
    // Unix: ~/.web3cpp
    fs::path pathRet;
    char* pszHome = getenv("HOME");
    if (pszHome == NULL || strlen(pszHome) == 0)
      pathRet = fs::path("/");
    else
      pathRet = fs::path(pszHome);
  #ifdef __APPLE__
    return pathRet / "Library/Application Support/web3cpp";
  #else
    return pathRet / ".web3cpp";
  #endif
  #endif
}

uint64_t Utils::roundUp(uint64_t num, uint64_t mul) {
  return (mul != 0 && num % mul != 0) ? (num + mul - (num % mul)) : num;
}

std::string Utils::randomHex(unsigned int size, bool prefixed) {
  unsigned char saltBytes[size];
  RAND_bytes(saltBytes, sizeof(saltBytes));
  std::string ret = (prefixed) ? "0x" : "";
  ret += dev::toHex(
    dev::sha3(std::string((char*)saltBytes, sizeof(saltBytes)), false)
  ).substr(0, size * 2);
  return ret;
}

BigNumber Utils::toBN(std::string number) {
  BigNumber ret;
  std::stringstream ss;
  if (isHexStrict(number)) {
    ss << std::hex << number;
  } else {
    ss << number;
  }
  ss >> ret;
  return ret;
}

std::string Utils::sha3(std::string string, bool isNibble) {
  return (!string.empty()) ? dev::toHex(dev::sha3(string, isNibble)) : "";
}

std::string Utils::keccak256(std::string string, bool isNibble) {
  return Utils::sha3(string, isNibble);
}

std::string Utils::sha3Raw(std::string string, bool isNibble) {
  return dev::toHex(dev::sha3(string, isNibble));
}

std::string Utils::soliditySha3(json params, Error &err) {
  std::string ret = Utils::stripHexPrefix(Solidity::packMulti(params, err));
  return ((err.getCode() == 0) ? "0x" + Utils::sha3(ret, true) : "");
}

std::string Utils::soliditySha3Raw(json params, Error &err) {
  std::string ret = Utils::stripHexPrefix(Solidity::packMulti(params, err));
  return ((err.getCode() == 0) ? "0x" + Utils::sha3Raw(ret, true) : "");
}

bool Utils::isHex(std::string hex) {
  if (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") { hex = hex.substr(2); }
  return (hex.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos);
}

bool Utils::isHexStrict(std::string hex) {
  return (hex.substr(0, 2) == "0x" || hex.substr(0, 2) == "0X") ? isHex(hex) : false;
}

bool Utils::isAddress(std::string address) {
  // Regexes for checking the basic requirements of an address,
  // all lower or all upper case, respectively.
  std::regex addRegex = std::regex("^(0x|0X)?[0-9a-fA-F]{40}$");
  std::regex lowRegex = std::regex("^(0x|0X)?[0-9a-f]{40}$");
  std::regex uppRegex = std::regex("^(0x|0X)?[0-9A-F]{40}$");
  if (!std::regex_match(address, addRegex)) {
    return false;
  } else if (std::regex_match(address, lowRegex) || std::regex_match(address, uppRegex)) {
    return true;
  } else {
    return checkAddressChecksum(address);
  }
}

std::string Utils::toLowercaseAddress(std::string address) {
  if (address.substr(0, 2) == "0x" || address.substr(0, 2) == "0X") {
    address = address.substr(2);
  }
  std::string ret = address;
  std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
  return "0x" + ret;
}

std::string Utils::toUppercaseAddress(std::string address) {
  if (address.substr(0, 2) == "0x" || address.substr(0, 2) == "0X") {
    address = address.substr(2);
  }
  std::string ret = address;
  std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
  return "0x" + ret;
}

std::string Utils::toChecksumAddress(std::string address) {
  // Hash needs address to be all lower-case and without the "0x" part
  address = Utils::toLowercaseAddress(address);
  if (address.substr(0, 2) == "0x" || address.substr(0, 2) == "0X") {
    address = address.substr(2);
  }
  std::string hash = dev::toHex(dev::sha3(address));
  std::string ret;
  for (int i = 0; i < address.length(); i++) {
    if (std::isdigit(address[i])) {
      // [0-9] - Can't uppercase so we skip it
      ret += address[i];
    } else {
      // [A-F] - If character hash is 8-F then make it uppercase
      int nibble = std::stoi(hash.substr(i, 1), nullptr, 16);
      ret += (nibble >= 8) ? std::toupper(address[i]) : std::tolower(address[i]);
    }
  }
  return "0x" + ret;
}

bool Utils::checkAddressChecksum(std::string address) {
  // Hash needs address to be all lower-case and without the "0x" part
  if (address.substr(0, 2) == "0x" || address.substr(0, 2) == "0X") {
    address = address.substr(2);
  }
  std::string hash = dev::toHex(dev::sha3(
    Utils::toLowercaseAddress(address).substr(2)
  ));
  for (int i = 0; i < address.length(); i++) {
    if (!std::isdigit(address[i])) {  // Only check A-F
      int nibble = std::stoi(hash.substr(i, 1), nullptr, 16);
      if (
        (nibble >= 8 && !std::isupper(address[i])) ||
        (nibble < 8 && !std::islower(address[i]))
      ) {
        return false;
      }
    }
  }
  return true;
}

std::string Utils::toHex(std::string value) {
  std::stringstream ss;
  if (std::all_of(value.begin(), value.end(), ::isdigit)) { // Number string
    return toHex(boost::lexical_cast<dev::u256>(value));
  } else {  // Text string
    for (int i = 0; i < value.length(); i++) {
      ss << std::hex << (int)value[i];
    }
  }
  return ss.str();
}

std::string Utils::toHex(BigNumber value) {
  std::stringstream ss;
  ss << std::hex << value;
  return ss.str();
}

std::string Utils::stripHexPrefix(std::string str) {
  return (
    !str.empty() && isHex(str) && (str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X")
  ) ? str.substr(2) : str;
}

std::string Utils::hexToNumberString(std::string hex) {
  if (hex.substr(0,2) != "0x" || hex.substr(0, 2) != "0X") { hex.insert(0, "0x"); }
  BigNumber bn = toBN(hex);
  std::stringstream ss;
  ss << bn;
  return ss.str();
}

BigNumber Utils::hexToBigNumber(std::string hex) {
  if (hex.substr(0,2) == "0x" || hex.substr(0, 2) == "0X") { hex = hex.substr(2); }
  return boost::lexical_cast<HexTo<dev::u256>>(hex);
}

std::string Utils::hexToUtf8(std::string hex) {
  if (!isHexStrict(hex)) {
    throw std::string("Error converting non-hex value to UTF-8: ") + hex;
  }
  hex = hex.substr(2);  // Remove "0x"

  std::string str;
  for (int i = 0; i < hex.length(); i += 2) {
    str += std::stoul(hex.substr(i, 2), nullptr, 16);
  }

  return str;
}

std::string Utils::hexToString(std::string hex) {
  return Utils::hexToUtf8(hex);
}

std::string Utils::hexToAscii(std::string hex) {
  if (!isHexStrict(hex)) {
    throw std::string("Error converting non-hex value to ASCII: ") + hex;
  }
  hex = hex.substr(2);  // Remove "0x"

  std::string str;
  for (int i = 0; i < hex.length(); i += 2) {
    str += std::stoul(hex.substr(i, 2), nullptr, 16);
  }
  return str;
}

std::vector<unsigned int> Utils::hexToBytes(std::string hex) {
  if (!isHexStrict(hex)) {
    throw std::string("Error converting non-hex value to bytes: ") + hex;
  }
  hex = hex.substr(2);  // Remove "0x"

  std::vector<unsigned int> bytes;
  for (int i = 0; i < hex.length(); i += 2) {
    bytes.push_back(std::stoul(hex.substr(i, 2), nullptr, 16));
  }
  return bytes;
}

std::string Utils::utf8ToHex(std::string str) {
  std::string hex = "";

  std::stringstream ss;
  for (int i = 0; i < str.length(); i++) {
    // You need two casts in order to properly cast char to uint.
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint>(static_cast<uint8_t>(str[i]));
  }
  hex += ss.str();

  return "0x" + hex;
}

std::string Utils::stringToHex(std::string hex) {
  return Utils::utf8ToHex(hex);
}

std::string Utils::asciiToHex(std::string str) {
  std::string hex = "";

  for (int i = 0; i < str.length(); i++) {
    int code = str[i];
    std::stringstream ss;
    ss << std::hex << code;
    std::string n = ss.str();
    hex += (n.length() < 2) ? "0" + n : n;
  }

  return "0x" + hex;
}

std::string Utils::bytesToHex(std::vector<unsigned int> byteArray) {
  std::string hex = "";

  std::ostringstream result;
  result << std::setw(2) << std::setfill('0') << std::hex;
  for (int i = 0; i < byteArray.size(); i++) {
    result << byteArray[i];
  }
  hex = result.str();

  return "0x" + hex;
}

std::string Utils::toWei(std::string amount, int decimals) {
  std::string digitPadding = "";
  std::string valuestr = "";

  // Check if input is valid
  if (amount.find_first_not_of("0123456789.") != std::string::npos) return "";

  // Read value from input string
  size_t index = 0;
  while (index < amount.size() && amount[index] != '.') {
    valuestr += amount[index];
    ++index;
  }
  ++index;  // Jump fixed point

  // Check if fixed point exists
  if (amount[index-1] == '.' && (amount.size() - (index)) > decimals) return "";

  // Check if input precision matches digit precision
  if (index < amount.size()) {
    // Read precision point into digitPadding
    while (index < amount.size()) {
      digitPadding += amount[index];
      ++index;
    }
  }

  // Create padding if there are missing decimals
  while (digitPadding.size() < decimals) digitPadding += '0';
  valuestr += digitPadding;
  while (valuestr[0] == '0') valuestr.erase(0,1);
  if (valuestr == "") valuestr = "0";
  return valuestr;
}

std::string Utils::toWei(BigNumber amount, int decimals) {
  std::string amountHex = Utils::toHex(amount);
  std::string amountStr = Utils::hexToNumberString(amountHex);
  return toWei(amountStr, decimals);
}

std::string Utils::fromWei(std::string amount, int decimals) {
  std::string result;
  if (amount.size() <= decimals) {
    int valueToPoint = decimals - amount.size();
    result += "0.";
    for (int i = 0; i < valueToPoint; ++i) result += "0";
    result += amount;
  } else {
    result = amount;
    int pointToPlace = result.size() - decimals;
    result.insert(pointToPlace, ".");
  }
  if (result == "") result = "0";
  return result;
}

std::string Utils::fromWei(BigNumber amount, int decimals) {
  std::string amountHex = Utils::toHex(amount);
  std::string amountStr = Utils::hexToNumberString(amountHex);
  return fromWei(amountStr, decimals);
}

std::string Utils::padLeft(
  std::string string, unsigned int characterAmount, char sign
) {
  bool hasPrefix = (string.substr(0, 2) == "0x" || string.substr(0, 2) == "0X");
  if (hasPrefix) { string = string.substr(2); }
  size_t padding = (characterAmount > string.length())
    ? (characterAmount - string.length()) : 0;
  std::string padded = (padding != 0) ? std::string(padding, sign) : "";
  return (hasPrefix ? "0x" : "") + padded + string;
}

std::string Utils::leftPad(
  std::string string, unsigned int characterAmount, char sign
) {
  return padLeft(string, characterAmount, sign);
}

std::string Utils::padRight(
  std::string string, unsigned int characterAmount, char sign
) {
  bool hasPrefix = (string.substr(0, 2) == "0x" || string.substr(0, 2) == "0X");
  if (hasPrefix) { string = string.substr(2); }
  size_t padding = (characterAmount > string.length())
    ? (characterAmount - string.length()) : 0;
  std::string padded = (padding != 0) ? std::string(padding, sign) : "";
  return (hasPrefix ? "0x" : "") + string + padded;
}

std::string Utils::rightPad(
  std::string string, unsigned int characterAmount, char sign
) {
  return padRight(string, characterAmount, sign);
}

json Utils::readJSONFile(boost::filesystem::path &filePath) {
  json returnData;
  storageLock.lock();
  //std::cout << "File path: " << filePath.string() << std::endl;
  if (!boost::filesystem::exists(filePath)) {
    throw std::string("Error reading json file: File does not exist");
  }
  try {
    boost::nowide::ifstream jsonFile(filePath.string());
    jsonFile >> returnData;
    jsonFile.close();
  } catch (std::exception &e) {
    storageLock.unlock();
    throw std::string("Error reading json file: ") + std::string(e.what());
  }

  storageLock.unlock();
  return returnData;
}

void Utils::writeJSONFile(json &obj, boost::filesystem::path &filePath) {
  json returnData;
  storageLock.lock();

  try {
    boost::nowide::ofstream os(filePath.string());
    os << std::setw(2) << obj << std::endl;
    os.close();
  } catch (std::exception &e) {
    storageLock.unlock();
    throw std::string("Error writing json file: ") + std::string(e.what());
  }

  storageLock.unlock();
  return;
}
