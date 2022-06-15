#ifndef CIPHER_H
#define CIPHER_H

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <string>

#include <nlohmann/json.hpp>

#include <web3cpp/Error.h>
#include <web3cpp/devcrypto/SecretStore.h>
#include <web3cpp/devcore/CommonData.h>
#include <web3cpp/devcore/SHA3.h>

using json = nlohmann::ordered_json;

/**
 * Namespace for encrypting/decrypting any kind of string.
 * Originally assimilated and adapted from Aleth's devcrypto/SecretStore.cpp.
 */

namespace Cipher {
  /**
   * Encrypts a given text string.
   * Returns a JSON string with the encrypted data + parameters for decrypt().
   */
  std::string encrypt(
    std::string const& plainText, std::string const& password, Error& error
  );

  /**
   * Decrypts a given JSON string with encrypted data + parameters.
   * Returns the original text string from encrypt().
   */
  std::string decrypt(
    std::string const& cipherText, std::string const& password, Error& error
  );

  /**
   * Same thing as the above, but actually uses Aleth/SecretStore functions
   * directly instead of re-implementing them.
   * Use only if the functions above act wonky for some reason.
   */
  std::string encryptAleth(
    std::string const& plainText, std::string const& password
  );
  std::string decryptAleth(
    std::string const& cipherText, std::string const& password
  );
}

#endif // CIPHER_H
