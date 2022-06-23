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
 * Namespace for BIP39 functions.
 * Abstracts functions from the [bip3x](https://github.com/itamarcps/bip3x) library.
 */

namespace BIP39 {
  /**
   * Generate a new random mnemonic phrase.
   * @return A struct that contains details about the mnemonic phrase.
   */
  bip3x::Bip39Mnemonic::MnemonicResult createNewMnemonic();

  /**
   * Create a public/private key pair.
   * @param &phrase The full BIP39 mnemonic phrase string.
   * @param derivPath The full derivation path (e.g. "m/44'/60'/0'/0").
   * @return An object that contains details about the key pair.
   */
  bip3x::HDKey createKey(std::string &phrase, std::string derivPath);

  /**
   * Check if a word exists in the **English** BIP39 wordlist.
   * @return `true` if the word exists, `false` otherwise.
   */
  bool wordExists(std::string word);

  /**
   * Generate a list with 10 addresses based on a given seed and a starting index.
   * @param &phrase The full BIP39 mnemonic phrase string.
   * @param derivPath The derivation path **without** the last digit (e.g. "m/44'/60'/0'/0/").
   * @param start The derivation index to start counting from (e.g. "10" will count from indexes 10-19).
   * @return A vector of pairs of generated addresses and their respective full derivation paths.
   */
  std::vector<std::pair<std::string,std::string>> generateAccountsFromSeed(
    std::string &phrase, std::string derivPath, int64_t start
  );
}

#endif  // BIP3X_H
