#ifndef BIP3X_H
#define BIP3X_H

#include <vector>

#include <bip3x/bip39.h>
#include <bip3x/Bip39Mnemonic.h>
#include <bip3x/HDKeyEncoder.h>
#include <bip3x/utils.h>
#include <bip3x/wordlist.h>

#include <web3cpp/devcrypto/Common.h>

#include <boost/lexical_cast.hpp>
/**
 * Simple wrapper for BIP39 interactions.
 * uses https://github.com/itamarcps/bip3x to ensure safety (OpenSSL as randomness).
 */

namespace BIP39 {
  /**
   * Generate a new random mnemonic phrase.
   * Returns the mnemonic phrase.
   */
  bip3x::Bip39Mnemonic::MnemonicResult createNewMnemonic();


  /**
   * Create a public/private key pair for an Account using a mnemonic phrase
   * and a derivation path (e.g. "m/44'/60'/0'/0" for Ethereum).
   * Returns the key pair for the Account.
   */
  bip3x::HDKey createKey(std::string &phrase, std::string derivPath);

  /**
   * Check if a word exists in the English BIP39 wordlist.
   * Returns true on success, false on failure.
   */
  bool wordExists(std::string word);

  /**
   * Generate a list with 10 Accounts based on a given seed and a starting index.
   * Returns a vector with the addresses.
   * derivPath should not include last digit, example:
   * "m/44'/60'/0'/"
   * return a vector of pairs, where first = address and second = path.
   */

  std::vector<std::pair<std::string,std::string>> generateAccountsFromSeed(std::string &phrase, std::string derivPath, int64_t start);

}



#endif  // BIP3X_H
