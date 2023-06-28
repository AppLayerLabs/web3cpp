#include <web3cpp/DB.h>

bool Database::openDB() {
  this->dbMutex.lock();
  if (!boost::filesystem::exists(this->path)) {
    boost::filesystem::create_directories(this->path);
  }
  this->dbStatus = leveldb::DB::Open(this->dbOpts, this->path.string(), &this->db);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    std::cout << "Error opening " << this->name << " database!"
      << this->dbStatus.ToString() << std::endl;
    return false;
  }
  this->dbMutex.unlock();
  return true;
}

bool Database::closeDB() {
  this->dbMutex.lock();
  if (this->db == nullptr) {
    return true;
  }
  delete this->db;
  this->db = nullptr;
  this->dbMutex.unlock();
  return true;
}

bool Database::keyExists(std::string const &key) const {
  this->dbMutex.lock();
  leveldb::Iterator* it = this->db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    if (it->key().ToString() == key) {
      delete it;
      this->dbMutex.unlock();
      return true;
    }
  }
  delete it;
  this->dbMutex.unlock();
  return false;
}

std::string Database::getKeyValue(std::string const &key) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Get(leveldb::ReadOptions(), key, &this->tmpValue);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    return "";
  }
  this->dbMutex.unlock();
  return this->tmpValue;
}

bool Database::putKeyValue(std::string const &key, std::string const &value) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Put(leveldb::WriteOptions(), key, value);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    std::cout << "Error putting key " << key << " at database " << this->name
      << ": " << this->dbStatus.ToString();
    return false;
  }
  this->dbMutex.unlock();
  return this->dbStatus.ok();
}

bool Database::deleteKeyValue(std::string const &key) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Delete(leveldb::WriteOptions(), key);
  if(!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    std::cout << "Error deleting key " << key << " at database " << this->name
      << ": " << this->dbStatus.ToString();
    return false;
  }
  this->dbMutex.unlock();
  return this->dbStatus.ok();
}

std::vector<std::string> Database::getAllKeys() const {
  this->dbMutex.lock();
  std::vector<std::string> ret;
  leveldb::Iterator* it = this->db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    ret.push_back(it->key().ToString());
  }
  delete it;
  this->dbMutex.unlock();
  return ret;
}

std::vector<std::string> Database::getAllValues() const {
  this->dbMutex.lock();
  std::vector<std::string> ret;
  leveldb::Iterator* it = this->db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    ret.push_back(it->value().ToString());
  }
  delete it;
  this->dbMutex.unlock();
  return ret;
}

std::map<std::string, std::string> Database::getAllPairs() const {
  this->dbMutex.lock();
  std::map<std::string, std::string> ret;
  leveldb::Iterator* it = this->db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    ret.emplace(it->key().ToString(), it->value().ToString());
  }
  delete it;
  this->dbMutex.unlock();
  return ret;
}

void Database::dropDatabase() {
  this->dbMutex.lock();
  leveldb::Iterator* it = this->db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    this->db->Delete(leveldb::WriteOptions(), it->key().ToString());
  }
  this->dbMutex.unlock();
  delete it;
}

