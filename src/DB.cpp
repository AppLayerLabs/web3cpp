#include <web3cpp/DB.h>

bool Database::openDB() {
  this->dbMutex.lock();
  if (!boost::filesystem::exists(databasePath)); {
    boost::filesystem::create_directories(databasePath);
  }
  this->dbStatus = leveldb::DB::Open(this->dbOpts, databasePath.string(), &db);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    throw std::string("Error opening ") + databaseName + "database! " + this->dbStatus.ToString();
  }
  this->dbMutex.unlock();
  return this->dbStatus.ok();
}

bool Database::closeDB() {
  this->dbMutex.lock();
  delete this->db;
  this->db = NULL;
  this->dbMutex.unlock();
  return true;
}

bool Database::keyExists(std::string &key) {
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

std::string Database::getKeyValue(std::string &key) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Get(leveldb::ReadOptions(), key, &this->tmpValue);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    throw std::string("Error reading at ") + databaseName + " database at key "
      + key + this->dbStatus.ToString();
  }
  this->dbMutex.unlock();
  return this->tmpValue;
}

bool Database::putKeyValue(std::string &key, std::string &value) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Put(leveldb::WriteOptions(), key, value);
  if (!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    throw std::string("Error writing at ") + databaseName + " database at key "
      + key + this->dbStatus.ToString();
  }
  this->dbMutex.unlock();
  return this->dbStatus.ok();
}

bool Database::deleteKeyValue(std::string &key) {
  this->dbMutex.lock();
  this->dbStatus = this->db->Delete(leveldb::WriteOptions(), key);
  if(!this->dbStatus.ok()) {
    this->dbMutex.unlock();
    throw std::string("Error deleting at ") + databaseName + " database at key "
      + key + this->dbStatus.ToString();
  }
  this->dbMutex.unlock();
  return this->dbStatus.ok();
}

std::vector<std::string> Database::getAllKeys() {
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

std::vector<std::string> Database::getAllValues() {
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

std::map<std::string, std::string> Database::getAllPairs() {
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
