#include <web3cpp/Bip39.h>

bip3x::Bip39Mnemonic::MnemonicResult BIP39::createNewMnemonic() {
  return bip3x::Bip39Mnemonic::generate();
}

bip3x::HDKey BIP39::createKey(std::string &phrase, std::string derivPath) {
  bip3x::bytes_64 seed = bip3x::HDKeyEncoder::makeBip39Seed(phrase);
  bip3x::HDKey rootKey = bip3x::HDKeyEncoder::makeBip32RootKey(seed);
  bip3x::HDKeyEncoder::makeExtendedKey(rootKey, derivPath);
  return rootKey;
}

bool BIP39::wordExists(std::string word) {
  struct words* wordlist;
  bip39_get_wordlist(NULL, &wordlist);
  size_t idx = wordlist_lookup_word(wordlist, word);
  return (idx != 0);
}

std::vector<std::pair<std::string,std::string>> BIP39::generateAccountsFromSeed(
  std::string &seed, std::string derivPath, int64_t index
) {
  std::vector<std::pair<std::string,std::string>> ret;
  for (int64_t i = 0; i < 10; ++i, ++index) {
    std::string address;
    std::string derivPath = derivPath + boost::lexical_cast<std::string>(index);
    bip3x::HDKey rootKey = BIP39::createKey(seed, derivPath);
    dev::KeyPair k(dev::Secret::frombip3x(rootKey.privateKey));
    address += "0x" + k.address().hex();
    ret.push_back(std::make_pair(address, derivPath));
  }
  return ret;
}

