#include <web3cpp/Cipher.h>

std::string Cipher::encrypt(std::string const& plainText, std::string const& password, Error &error) {
  // Derive the key.
  // operator+ for vectors are defined inside a dev namespace on CommonData.h
  using namespace dev;
  json ret;
  dev::bytesSec derivedKey = dev::deriveNewKey(password, dev::KDF::Scrypt, ret);
  if (derivedKey.empty()) { error.setCode(7); return ""; } // Key Derivation Failed
  ret["cipher"] = "aes-128-ctr";
  dev::SecureFixedHash<16> key(derivedKey, dev::h128::AlignLeft);
  dev::h128 iv = dev::h128::random();
  {
    json params;
    params["iv"] = dev::toHex(iv.ref());
    ret["cipherparams"] = params;
  }

  // Create cipher text and MAC.
  dev::bytesConstRef _v(plainText);
  dev::bytes cipherText = encryptSymNoAuth(key, iv, _v);
  if (cipherText.empty()) { error.setCode(8); return ""; } // Key Encryption Failed
  ret["ciphertext"] = dev::toHex(cipherText);
  dev::h256 mac = dev::sha3(derivedKey.ref().cropped(16, 16).toBytes() + cipherText);
  ret["mac"] = dev::toHex(mac.ref());

  error.setCode(0);
  return ret.dump();
}

std::string Cipher::decrypt(std::string const& cipherText, std::string const& password, Error& error) {
  // Derive the key.
  json o = json::parse(cipherText);
  dev::bytesSec derivedKey;
  if (o["kdf"].get<std::string>() == "pbkdf2") {
    json params = o["kdfparams"];
    if (params["prf"].get<std::string>() != "hmac-sha256") {
      // cwarn << "Unknown PRF for PBKDF2" << params["prf"].get<std::string() << "not supported.";
      error.setCode(7); // Key Derivation Error
      return bytesSec().ref().toString();
    }
    unsigned int iterations = params["c"].get<int>();
    dev::bytes salt = dev::fromHex(params["salt"].get<std::string>());
    derivedKey = dev::pbkdf2(password, salt, iterations, params["dklen"].get<int>());
  } else if (o["kdf"].get<std::string>() == "scrypt") {
    json p = o["kdfparams"];
    derivedKey = dev::scrypt(
      password, dev::fromHex(p["salt"].get<std::string>()), p["n"].get<int>(),
      p["r"].get<int>(), p["p"].get<int>(), p["dklen"].get<int>()
    );
  } else {
    // cwarn << "Unknown KDF" << o["kdf"].get<std::string>() << "not supported.";
    error.setCode(7); // Key Derivation Error
    return bytesSec().ref().toString();
  }

  // Check if derived key has the correct length.
  if (derivedKey.size() < 32 && !(o.count("compat") && o["compat"].get<std::string>() == "2")) {
    // cwarn << "Derived key's length too short (<32 bytes)";
    error.setCode(14); // Key Derivation Invalid Length
    return bytesSec().ref().toString();
  }
  dev::bytes cipherBytes = dev::fromHex(o["ciphertext"].get<std::string>());

  // Check MAC.
  if (o.count("mac")) {
    dev::h256 mac(o["mac"].get<std::string>());
    dev::h256 macExp = (o.count("compat") && o["compat"].get<std::string>() == "2")
      ? dev::sha3(derivedKey.ref().cropped(derivedKey.size() - 16).toBytes() + cipherBytes)
      : dev::sha3(derivedKey.ref().cropped(16, 16).toBytes() + cipherBytes);
    if (mac != macExp) {
      // cwarn << "Invalid key - MAC mismatch; expected" << dev::toString(macExp) << ", got" << dev::toString(mac);
      error.setCode(15); // Key Decryption MAC Mismatch
      return bytesSec().ref().toString();
    }
  } else if (o.count("sillymac")) {
    dev::h256 mac(o["sillymac"].get<std::string>());
    dev::h256 macExp = dev::sha3(
      dev::asBytes(o["sillymacjson"].get<std::string>()) +
      derivedKey.ref().cropped(derivedKey.size() - 16).toBytes() + cipherBytes
    );
    if (mac != macExp) {
      // cwarn << "Invalid key - MAC mismatch; expected" << dev::toString(macExp) << ", got" << dev::toString(mac);
      error.setCode(15); // Key Decryption MAC Mismatch
      return bytesSec().ref().toString();
    }
  }
  // else
  //   cwarn << "No MAC. Proceeding anyway.";

  // Decrypt the cipher.
  if (o["cipher"].get<std::string>() == "aes-128-ctr") {
    json params = o["cipherparams"];
    dev::h128 iv(params["iv"].get<std::string>());
    if (o.count("compat") && o["compat"].get<std::string>() == "2") {
      dev::SecureFixedHash<16> key(
        dev::sha3Secure(derivedKey.ref().cropped(derivedKey.size() - 16)), dev::h128::AlignRight
      );
      error.setCode(0);
      return dev::decryptSymNoAuth(key, iv, &cipherBytes).ref().toString();
    } else {
      error.setCode(0);
      return dev::decryptSymNoAuth(
        dev::SecureFixedHash<16>(derivedKey, dev::h128::AlignLeft), iv, &cipherBytes
      ).ref().toString();
    }
  } else {
    // cwarn << "Unknown cipher" << o["cipher"].get<std::string>() << "not supported.";
    error.setCode(16); // Key Decryption Failed
    return bytesSec().ref().toString();
  }
}

std::string Cipher::encryptAleth(
  std::string const& plainText, std::string const& password
) {
  return dev::SecretStore::encrypt(plainText, password);
}

std::string Cipher::decryptAleth(
  std::string const& cipherText, std::string const& password
) {
  return dev::SecretStore::decrypt(cipherText, password).ref().toString();
}

