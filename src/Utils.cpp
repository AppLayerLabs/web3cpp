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
    // Windows: C:\Users\Username\AppData\Roaming\AVME
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
