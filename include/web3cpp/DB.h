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
    std::mutex dbMutex;
  public:

  bool cleanCloseDB() { return closeDB(); };
  bool isDBOpen() { return (this->db != NULL); }
  bool keyExists(std::string &key);
  std::string getKeyValue(std::string &key);
  bool putKeyValue(std::string &key, std::string &value);
  bool deleteKeyValue(std::string &key);
  std::vector<std::string> getAllKeys();
  std::vector<std::string> getAllValues();
  std::map<std::string, std::string> getAllPairs();
  void dropDatabase();

  Database(std::string &_databaseName, boost::filesystem::path &rootPath) : databaseName(_databaseName), databasePath(rootPath.string() + databaseName) {
    this->dbOpts.create_if_missing = true;
    openDB();
  }
};