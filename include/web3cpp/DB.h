#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <leveldb/db.h>

using json = nlohmann::json;

class Database {
  private:
    std::string name;
    boost::filesystem::path path;
    leveldb::DB* db;
    leveldb::Options dbOpts;
    leveldb::Status dbStatus;
    std::string tmpValue;
    std::mutex mutable dbMutex;
    bool openDB();
    bool closeDB();

  public:
    // Constructor.
    Database(std::string _name, boost::filesystem::path rootPath)
    : name(_name), path(rootPath.string() + "/" + name) {
      this->dbOpts.create_if_missing = true;
      openDB();
    }

    // Copy constructor.
    Database(Database& other) noexcept :
      name(other.name), path(other.path), db(other.db),
      dbOpts(other.dbOpts), dbStatus(other.dbStatus), tmpValue(other.tmpValue)
    {}

    // Destructor.
    ~Database() { closeDB(); }

    // Functions.
    bool keyExists(std::string const &key);
    std::string getKeyValue(std::string const &key);
    bool putKeyValue(std::string const &key, std::string const &value);
    bool deleteKeyValue(std::string const &key);
    std::vector<std::string> getAllKeys();
    std::vector<std::string> getAllValues();
    std::map<std::string, std::string> getAllPairs();
    void dropDatabase();
};

#endif  // DATABASE_H
