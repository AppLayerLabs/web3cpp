#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <leveldb/db.h>

using json = nlohmann::json;

class Database {
  private:
    std::string databaseName;
    boost::filesystem::path databasePath;
    bool openDB();
    bool closeDB();
    leveldb::DB* db;
    leveldb::Options dbOpts;
    leveldb::Status dbStatus;
    std::string tmpValue;
    std::mutex mutable dbMutex;

  public:
    // Constructor.
    Database(std::string _databaseName, boost::filesystem::path rootPath)
    : databaseName(_databaseName), databasePath(rootPath.string() + "/" + databaseName) {
      this->dbOpts.create_if_missing = true;
      openDB();
    }

    // Copy constructor.
    Database(Database&& other) noexcept :
      databaseName(std::move(other.databaseName)),
      databasePath(std::move(other.databasePath)),
      db(std::move(other.db)),
      dbOpts(std::move(other.dbOpts)),
      dbStatus(std::move(other.dbStatus)),
      tmpValue(std::move(other.tmpValue))
    {}

    // Functions.
    bool cleanCloseDB() { return closeDB(); };
    bool isDBOpen() { return (this->db != NULL); }
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
