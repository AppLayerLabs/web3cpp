#include <web3cpp/Utils.h>

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

std::string Utils::randomHex(unsigned int size) {
  unsigned char saltBytes[size];
  RAND_bytes(saltBytes, sizeof(saltBytes));
  return dev::toHex(
    dev::sha3(std::string((char*)saltBytes, sizeof(saltBytes)), false)
  ).substr(0,16);
}

std::string Utils::sha3(std::string string) {
  return "";
}

std::string Utils::keccak256(std::string string) {
  return "";
}

std::string Utils::sha3Raw(std::string string) {
  return "";
}

bool Utils::isHex(std::string hex) {
  if (hex.substr(0, 2) == "0x") { hex = hex.substr(2); }
  return (hex.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos);
}

bool Utils::isHexStrict(std::string hex) {
  return (hex.substr(0, 2) == "0x") ? isHex(hex) : false;
}

bool Utils::isAddress(std::string address) {
  // Regexes for checking the basic requirements of an address,
  // all lower or all upper case, respectively.
  std::regex addRegex = std::regex("^(0x)?[0-9a-f]{40}$");
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

std::string Utils::toChecksumAddress(std::string address) {
  // Address has to be hashed as all lower-case and without the "0x" part
  std::string add = address;
  std::transform(add.begin(), add.end(), add.begin(), ::tolower);
  if (add.substr(0, 2) == "0x") { add = add.substr(2); }
  std::string hash = dev::toHex(dev::sha3(add));
  std::string ret = "0x";
  for (int i = 0; i < add.length(); i++) {
    // If ith character hash is 8 to f then make it uppercase
    ret += (std::stoi(hash.substr(i, 1), nullptr, 16) > 7)
      ? std::toupper(add[i]) : std::tolower(add[i]);
  }
  return ret;
}

bool Utils::checkAddressChecksum(std::string address) {
  // Address has to be hashed as all lower-case and without the "0x" part
  if (address.substr(0, 2) == "0x") { address = address.substr(2); }
  std::string hash = dev::toHex(dev::sha3(address));
  for (int i = 0; i < address.length(); i++) {
    bool upperHash = (std::stoi(hash.substr(i, 1), nullptr, 16) > 7);
    bool lowerHash = (std::stoi(hash.substr(i, 1), nullptr, 16) <= 7);
    bool upperChar = (std::toupper(address[i]) == address[i]);
    bool lowerChar = (std::tolower(address[i]) == address[i]);
    if ((upperHash && !upperChar) || (lowerHash && !lowerChar)) {
      return false;
    }
  }
  return true;
}

std::string Utils::stripHexPrefix(std::string str) {
  return (!str.empty() && isHex(str) && str.substr(0, 2) == "0x") ? str.substr(2) : str;
}

std::string Utils::hexToUtf8(std::string hex) {
  if (!isHexStrict(hex)) {
    return "";  // TODO: throw an error here
  }
  hex = hex.substr(2);  // Remove "0x"

  // Remove 00 padding from either side
  std::regex paddingRegex = std::regex("^(?:00)*");
  regex_replace(hex, paddingRegex, "");
  reverse(hex.begin(), hex.end());
  regex_replace(hex, paddingRegex, "");
  reverse(hex.begin(), hex.end());

  std::string str = "";
  int len = hex.length();
  for (int i = 0; i < len; i += 2) {
    int code = std::stoi(hex.substr(i, 2), nullptr, 16);
    str += std::to_string(code);
  }

  return str; // TODO: check if UTF-8 works here (should be utf8.decode(str))
}

std::string Utils::hexToString(std::string hex) {
  return Utils::hexToUtf8(hex);
}

std::string Utils::utf8ToHex(std::string str) {
  // TODO: check if UTF-8 works here (should be utf8.encode(str))
  std::string hex = "";

  // Remove \u0000 padding from either side
  std::regex paddingRegex = std::regex("^(?:\u0000)*");
  regex_replace(str, paddingRegex, "");
  reverse(str.begin(), str.end());
  regex_replace(str, paddingRegex, "");
  reverse(str.begin(), str.end());

  for (int i = 0; i < str.length(); i++) {
    int code = str[i];
    std::stringstream ss;
    ss << std::hex << code;
    std::string n = ss.str();
    hex += (n.length() < 2) ? "0" + n : n;
  }

  return "0x" + hex;
}

std::string Utils::stringToHex(std::string hex) {
  return utf8ToHex(hex);
}

std::string Utils::toWei(std::string amount, int decimals) {
  std::string digitPadding = "";
  std::string valuestr = "";

  // Check if input is valid
  if (amount.find_first_not_of("0123456789.") != std::string::npos) {
    return "";
  }

  // Read value from input string
  size_t index = 0;
  while (index < amount.size() && amount[index] != '.') {
    valuestr += amount[index];
    ++index;
  }
  ++index;  // Jump fixed point

  // Check if fixed point exists
  if (amount[index-1] == '.' && (amount.size() - (index)) > decimals) {
    return "";
  }

  // Check if input precision matches digit precision
  if (index < amount.size()) {
    // Read precision point into digitPadding
    while (index < amount.size()) {
      digitPadding += amount[index];
      ++index;
    }
  }

  // Create padding if there are missing decimals
  while (digitPadding.size() < decimals) {
    digitPadding += '0';
  }
  valuestr += digitPadding;
  while (valuestr[0] == '0') {
    valuestr.erase(0,1);
  }

  if (valuestr == "") valuestr = "0";
  return valuestr;
}

std::string Utils::fromWei(std::string amount, int decimals) {
  std::string result;

  if (amount.size() <= decimals) {
    int valueToPoint = decimals - amount.size();
    result += "0.";
    for (int i = 0; i < valueToPoint; ++i) {
      result += "0";
    }
    result += amount;
  } else {
    result = amount;
    int pointToPlace = result.size() - decimals;
    result.insert(pointToPlace, ".");
  }

  if (result == "") result = "0";
  return result;
}

std::string padLeft(std::string string, unsigned int characterAmount, std::string sign = "0") {
  bool hasPrefix = (string.substr(0, 2) == "0x");
  if (hasPrefix) { string = string.substr(2); }
  int padding = (characterAmount - string.length() + 1 >= 0)
    ? characterAmount - string.length() + 1 : 0;
  std::string padded(sign, padding);
  return (hasPrefix ? "0x" : "") + padded + string;
}

std::string leftPad(std::string string, unsigned int characterAmount, std::string sign = "0") {
  return padLeft(string, characterAmount, sign);
}

std::string padRight(std::string string, unsigned int characterAmount, std::string sign = "0") {
  bool hasPrefix = (string.substr(0, 2) == "0x");
  if (hasPrefix) { string = string.substr(2); }
  int padding = (characterAmount - string.length() + 1 >= 0)
    ? characterAmount - string.length() + 1 : 0;
  std::string padded(sign, padding);
  return (hasPrefix ? "0x" : "") + string + padded;
}

std::string rightPad(std::string string, unsigned int characterAmount, std::string sign = "0") {
  return padRight(string, characterAmount, sign);
}

