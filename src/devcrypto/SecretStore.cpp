// Aleth: Ethereum C++ client, tools and libraries.
// Copyright 2014-2019 Aleth Authors.
// Licensed under the GNU General Public License, Version 3.
#include <web3cpp/devcrypto/SecretStore.h>

static const int c_keyFileVersion = 3;

/// Upgrade the json-format to the current version.
static json upgraded(std::string const& _s) {
  json ret = json::parse(_s);
  if (!ret.is_object()) return json::object();

	unsigned version = ret.count("Version")
    ? stoi(ret["Version"].get<std::string>())
    : ret.count("version")
      ? ret["version"].get<int>() : 0;

	if (version == 1) {
		// upgrade to version 2
		json old;
		swap(old, ret);
		ret["id"] = old["Id"];
    json c;
		c["ciphertext"] = old["Crypto"]["CipherText"];
		c["cipher"] = "aes-128-cbc";
		{
      json cp;
			cp["iv"] = old["Crypto"]["IV"];
			c["cipherparams"] = cp;
		}
		c["kdf"] = old["Crypto"]["KeyHeader"]["Kdf"];
		{
      json kp;
			kp["salt"] = old["Crypto"]["Salt"];
			for (std::pair<std::string,int> const& i : old["Crypto"]["KeyHeader"]["KdfParams"]) {
				if (i.first != "SaltLen") {
					kp[boost::to_lower_copy(i.first)] = i.second;
        }
      }
			c["kdfparams"] = kp;
		}
		c["sillymac"] = old["Crypto"]["MAC"];
		c["sillymacjson"] = _s;
		ret["crypto"] = c;
		version = 2;
	}

	if (ret.count("Crypto") && !ret.count("crypto")) {
		ret["crypto"] = ret["Crypto"];
		ret.erase("Crypto");
	}

	if (version == 2) {
		ret["crypto"]["cipher"] = "aes-128-ctr";
		ret["crypto"]["compat"] = "2";
		version = 3;
	}

	if (version == c_keyFileVersion) return ret;
	return json::object();
}

SecretStore::SecretStore(fs::path const& _path): m_path(_path)
{
	load();
}

void SecretStore::setPath(fs::path const& _path)
{
	m_path = _path;
	load();
}

bytesSec SecretStore::secret(h128 const& _uuid, std::function<std::string()> const& _pass, bool _useCache) const
{
    auto rit = m_cached.find(_uuid);
    if (_useCache && rit != m_cached.end())
        return rit->second;
    auto it = m_keys.find(_uuid);
    bytesSec key;
    if (it != m_keys.end())
    {
        key = bytesSec(decrypt(it->second.encryptedKey, _pass()));
        if (!key.empty())
        {
            m_cached[_uuid] = key;
            // TODO: Fix constness.
            const_cast<SecretStore*>(this)->noteAddress(_uuid, toAddress(Secret{key}));
        }
    }
    return key;
}

bytesSec SecretStore::secret(Address const& _address, std::function<std::string()> const& _pass) const
{
	bytesSec ret;
	if (auto k = key(_address))
		ret = bytesSec(decrypt(k->second.encryptedKey, _pass()));
	return ret;
}

bytesSec SecretStore::secret(std::string const& _content, std::string const& _pass) {
	try {
		json u = upgraded(_content);
		if (!u.is_object()) return bytesSec();
    return decrypt(u["crypto"].dump(), _pass);
	} catch (...) {
		return bytesSec();
	}
}

h128 SecretStore::importSecret(bytesSec const& _s, std::string const& _pass)
{
	h128 r = h128::random();
	EncryptedKey key{encrypt(_s.ref(), _pass), toUUID(r), KeyPair(Secret(_s)).address()};
	m_cached[r] = _s;
	m_keys[r] = std::move(key);
	save();
	return r;
}

h128 SecretStore::importSecret(bytesConstRef _s, std::string const& _pass)
{
	h128 r = h128::random();
	EncryptedKey key{encrypt(_s, _pass), toUUID(r), KeyPair(Secret(_s)).address()};
	m_cached[r] = bytesSec(_s);
	m_keys[r] = std::move(key);
	save();
	return r;
}

void SecretStore::kill(h128 const& _uuid)
{
	m_cached.erase(_uuid);
	if (m_keys.count(_uuid))
	{
		fs::remove(m_keys[_uuid].filename);
		m_keys.erase(_uuid);
	}
}

void SecretStore::clearCache() const
{
	m_cached.clear();
}

void SecretStore::save(fs::path const& _keysPath) {
	fs::create_directories(_keysPath);
	DEV_IGNORE_EXCEPTIONS(fs::permissions(_keysPath, fs::owner_all));
	for (auto& k: m_keys) {
		std::string uuid = toUUID(k.first);
		fs::path filename = (_keysPath / uuid).string() + ".json";
		json v;
    json crypto = json::parse(k.second.encryptedKey);
    v["address"] = k.second.address.hex();
		v["crypto"] = crypto;
		v["id"] = uuid;
		v["version"] = c_keyFileVersion;
		writeFile(filename, v.dump());
    swap(k.second.filename, filename);
		if (!filename.empty() && !fs::equivalent(filename, k.second.filename)) {
			fs::remove(filename);
    }
	}
}

bool SecretStore::noteAddress(h128 const& _uuid, Address const& _address)
{
    auto it = m_keys.find(_uuid);
    if (it != m_keys.end() && it->second.address == ZeroAddress)
    {
        it->second.address = _address;
        return true;
    }
    return false;
}

void SecretStore::load(fs::path const& _keysPath)
{
	try
	{
		for (fs::directory_iterator it(_keysPath); it != fs::directory_iterator(); ++it)
			if (fs::is_regular_file(it->path()))
				readKey(it->path().string(), true);
	}
	catch (...) {}
}

h128 SecretStore::readKey(fs::path const& _file, bool _takeFileOwnership)
{
	// ctrace << "Reading" << _file.string();
	return readKeyContent(contentsString(_file), _takeFileOwnership ? _file : std::string());
}

h128 SecretStore::readKeyContent(std::string const& _content, fs::path const& _file) {
	try {
		json u = upgraded(_content);
		if (u.is_object()) {
			json& o = u;
			auto uuid = fromUUID(o["id"].get<std::string>());
			Address address = ZeroAddress;
			if (o.find("address") != o.end() && isHex(o["address"].get<std::string>())) {
				address = Address(o["address"].get<std::string>());
      }
			// else
				// cwarn << "Account address is either not defined or not in hex format" << _file.string();
			m_keys[uuid] = EncryptedKey{o["crypto"].dump(), _file, address};
			return uuid;
		}
		// else
			// cwarn << "Invalid JSON in key file" << _file.string();
		return h128();
	} catch (...) {
		return h128();
	}
}

bool SecretStore::recode(Address const& _address, std::string const& _newPass, std::function<std::string()> const& _pass, KDF _kdf)
{
	if (auto k = key(_address))
	{
		bytesSec s = secret(_address, _pass);
		if (s.empty())
			return false;
		else
		{
			k->second.encryptedKey = encrypt(s.ref(), _newPass, _kdf);
			save();
			return true;
		}
	}
	return false;
}

std::pair<h128 const, SecretStore::EncryptedKey> const* SecretStore::key(Address const& _address) const
{
	for (auto const& k: m_keys)
		if (k.second.address == _address)
			return &k;
	return nullptr;
}

std::pair<h128 const, SecretStore::EncryptedKey>* SecretStore::key(Address const& _address)
{
	for (auto& k: m_keys)
		if (k.second.address == _address)
			return &k;
	return nullptr;
}

bool SecretStore::recode(h128 const& _uuid, std::string const& _newPass, std:: function<std::string()> const& _pass, KDF _kdf)
{
	bytesSec s = secret(_uuid, _pass, true);
	if (s.empty())
		return false;
	m_cached.erase(_uuid);
	m_keys[_uuid].encryptedKey = encrypt(s.ref(), _newPass, _kdf);
	save();
	return true;
}

bytesSec dev::deriveNewKey(std::string const& _pass, KDF _kdf, json& jsonObj)
{
	unsigned dklen = 32;
	unsigned iterations = 1 << 18;
	bytes salt = h256::random().asBytes();
	if (_kdf == KDF::Scrypt)
	{
		unsigned p = 1;
		unsigned r = 8;
		jsonObj["kdf"] = "scrypt";
		{
			json params;
			params["n"] = int64_t(iterations);
			params["r"] = int(r);
			params["p"] = int(p);
			params["dklen"] = int(dklen);
			params["salt"] = toHex(salt);
			jsonObj["kdfparams"] = params;
		}
		return scrypt(_pass, salt, iterations, r, p, dklen);
	}
	else
	{
		jsonObj["kdf"] = "pbkdf2";
		{
			json params;
			params["prf"] = "hmac-sha256";
			params["c"] = int(iterations);
			params["salt"] = toHex(salt);
			params["dklen"] = int(dklen);
			jsonObj["kdfparams"] = params;
		}
		return pbkdf2(_pass, salt, iterations, dklen);
	}
}

std::string SecretStore::encrypt(bytesConstRef _v, std::string const& _pass, KDF _kdf) {
  json ret;
	bytesSec derivedKey = deriveNewKey(_pass, _kdf, ret);
	if (derivedKey.empty()) {
		BOOST_THROW_EXCEPTION(crypto::CryptoException() << errinfo_comment("Key derivation failed."));
  }

	ret["cipher"] = "aes-128-ctr";
	SecureFixedHash<16> key(derivedKey, h128::AlignLeft);
	h128 iv = h128::random();
	{
		json params;
    params["iv"] = toHex(iv.ref());
		ret["cipherparams"] = params;
	}

	// cipher text
	bytes cipherText = encryptSymNoAuth(key, iv, _v);
	if (cipherText.empty()) {
		BOOST_THROW_EXCEPTION(crypto::CryptoException() << errinfo_comment("Key encryption failed."));
  }
	ret["ciphertext"] = toHex(cipherText);

	// and mac.
	h256 mac = sha3(derivedKey.ref().cropped(16, 16).toBytes() + cipherText);
	ret["mac"] = toHex(mac.ref());

  return ret.dump();
}

bytesSec SecretStore::decrypt(std::string const& _v, std::string const& _pass) {
	// derive key
  json o = json::parse(_v);
	bytesSec derivedKey;
	if (o["kdf"].get<std::string>() == "pbkdf2") {
		auto params = o["kdfparams"];
		if (params["prf"].get<std::string>() != "hmac-sha256") {
			// cwarn << "Unknown PRF for PBKDF2" << params["prf"].get<std::string>() << "not supported.";
			return bytesSec();
		}
		unsigned iterations = params["c"].get<int>();
		bytes salt = fromHex(params["salt"].get<std::string>());
		derivedKey = pbkdf2(_pass, salt, iterations, params["dklen"].get<int>());
	} else if (o["kdf"].get<std::string>() == "scrypt") {
		auto p = o["kdfparams"];
		derivedKey = scrypt(
      _pass, fromHex(p["salt"].get<std::string>()), p["n"].get<int>(),
      p["r"].get<int>(), p["p"].get<int>(), p["dklen"].get<int>()
    );
	} else {
		// cwarn << "Unknown KDF" << o["kdf"].get<std::string>() << "not supported.";
		return bytesSec();
	}

	if (derivedKey.size() < 32 && !(o.count("compat") && o["compat"].get<std::string>() == "2")) {
		// cwarn << "Derived key's length too short (<32 bytes)";
		return bytesSec();
	}

	bytes cipherText = fromHex(o["ciphertext"].get<std::string>());

	// check MAC
	if (o.count("mac")) {
		h256 mac(o["mac"].get<std::string>());
		h256 macExp;
		if (o.count("compat") && o["compat"].get<std::string>() == "2")
			macExp = sha3(derivedKey.ref().cropped(derivedKey.size() - 16).toBytes() + cipherText);
		else
			macExp = sha3(derivedKey.ref().cropped(16, 16).toBytes() + cipherText);
		if (mac != macExp) {
			// cwarn << "Invalid key - MAC mismatch; expected" << toString(macExp) << ", got" << toString(mac);
			return bytesSec();
		}
	} else if (o.count("sillymac")) {
		h256 mac(o["sillymac"].get<std::string>());
		h256 macExp = sha3(asBytes(o["sillymacjson"].get<std::string>()) + derivedKey.ref().cropped(derivedKey.size() - 16).toBytes() + cipherText);
		if (mac != macExp) {
			// cwarn << "Invalid key - MAC mismatch; expected" << toString(macExp) << ", got" << toString(mac);
			return bytesSec();
		}
	}
	// else
		// cwarn << "No MAC. Proceeding anyway.";

	// decrypt
	if (o["cipher"].get<std::string>() == "aes-128-ctr") {
		auto params = o["cipherparams"];
		h128 iv(params["iv"].get<std::string>());
		if (o.count("compat") && o["compat"].get<std::string>() == "2") {
			SecureFixedHash<16> key(sha3Secure(derivedKey.ref().cropped(derivedKey.size() - 16)), h128::AlignRight);
			return decryptSymNoAuth(key, iv, &cipherText);
		} else {
			return decryptSymNoAuth(SecureFixedHash<16>(derivedKey, h128::AlignLeft), iv, &cipherText);
    }
	} else {
		// cwarn << "Unknown cipher" << o["cipher"].get<std::string>() << "not supported.";
		return bytesSec();
	}
}
