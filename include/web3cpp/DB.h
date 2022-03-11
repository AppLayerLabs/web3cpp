#include <string>
#include <lib/nlohmann_json/json.hpp>
#include <boost/filesystem.hpp>
#include <leveldb/db.h>

class Database {
  private:
    std::string databaseName;
    boost::filesystem::path databasePath;
    bool openDB();
    bool closeDB();
    leveldb::DB* db;
    leveldb::Options dbOpts;
    leveldb::Status dbStatus;
  public:


  Database(std::string &_databaseName, boost::filesystem::path &rootPath) : databaseName(_databaseName), databasePath(rootPath.string() + databaseName) {
    this->dbOpts.create_if_missing = true;
  }
}