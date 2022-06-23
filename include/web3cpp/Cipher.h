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
 * Namespace for encrypting/decrypting any kind of data.
 * Originally assimilated and adapted from Ethereum's Aleth libs
 * (specifically from devcrypto/SecretStore.cpp).
 */

namespace Cipher {
  /**
   * Encrypts a given data string.
   * @param &plainText The data string to be encrypted.
   * @param &password The password used for encrypting.
   * @param &error Error object.
   * @return A JSON string with the encrypted data and parameters for decrypting.
   */
  std::string encrypt(
    std::string const& plainText, std::string const& password, Error& error
  );

  /**
   * Decrypts data from a JSON string.
   * @param &cipherText The JSON string with encrypted data and parameters for decrypting.
   * @param &password The password used for decrypting.
   * @param &error Error object.
   * @return The decrypted data string.
   */
  std::string decrypt(
    std::string const& cipherText, std::string const& password, Error& error
  );

  /**
   * **LEGACY/UNMAINTAINED**. Same as Cipher::encrypt(), but calls the
   * original Aleth `dev::SecretStore::encrypt()` implementation directly.
   * Use only if absolutely required.
   */
  std::string encryptAleth(
    std::string const& plainText, std::string const& password
  );

  /**
   * **LEGACY/UNMAINTAINED**. Same as Cipher::decrypt(), but calls the
   * original Aleth `dev::SecretStore::decrypt()` implementation directly.
   * Use only if absolutely required.
   */
  std::string decryptAleth(
    std::string const& cipherText, std::string const& password
  );
}

#endif // CIPHER_H
