#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <boost/filesystem.hpp>
#include <leveldb/db.h>

using json = nlohmann::ordered_json;

/**
 * Abstraction of a single [LevelDB](https://github.com/google/leveldb) database.
 */

class Database {
  private:
    std::string name;             ///< Name for the database.
    boost::filesystem::path path; ///< Full path for the database (includes name).
    leveldb::DB* db;              ///< Pointer to the actual LevelDB database object.
    leveldb::Options dbOpts;      ///< Struct with options for the database.
    leveldb::Status dbStatus;     ///< Struct with the status of the last database operation.
    std::string tmpValue;         ///< Buffer for a temporary value.
    std::mutex mutable dbMutex;   ///< Mutex for managing read/write access to the database.

    /**
     * Opens the proper database object.
     * @return `true` on success, `false` on failure.
     */
    bool openDB();

    /**
     * Closes the proper database object.
     * "Closing" a LevelDB database actually means just deleting the database object.
     * @return `true` on success, `false` on failure.
     */
    bool closeDB();

  public:
    /// Empty constructor.
    Database(){}

    /**
     * Default constructor.
     * @param _name The database's name.
     * @param rootPath The parent folder of the database.
     */
    Database(std::string _name, boost::filesystem::path rootPath)
    : name(_name), path(rootPath.string() + "/" + name) {
      this->dbOpts.create_if_missing = true;
      openDB();
    }

    /// Copy constructor.
    Database(const Database& other) noexcept :
      name(other.name), path(other.path), db(other.db),
      dbOpts(other.dbOpts), dbStatus(other.dbStatus), tmpValue(other.tmpValue)
    {}

    /// Destructor.
    ~Database() { closeDB(); }

    /**
     * Check if a key exists in the database.
     * @param &key The key to search for.
     * @return `true` if the key exists, `false` otherwise.
     */
    bool keyExists(std::string const &key);

    /**
     * Get the value of a key from the database.
     * @param &key The key to get the value from.
     * @return The value linked to the given key.
     */
    std::string getKeyValue(std::string const &key);

    /**
     * Insert a key/value pair into the database.
     * @param &key The key to insert.
     * @param &value The value to insert under the given key.
     * @return `true` if insertion was successful, `false` otherwise.
     */
    bool putKeyValue(std::string const &key, std::string const &value);

    /**
     * Delete a key/value pair from the database.
     * @param &key The key to delete.
     * @return `true` if deletion was successful or if the key didn't exist
     *         anyway prior to requesting deletion, `false` otherwise.
     */
    bool deleteKeyValue(std::string const &key);

    /**
     * Get all the individual keys stored in the database.
     * @return A vector with all the key strings.
     */
    std::vector<std::string> getAllKeys();

    /**
     * Get all the individual values stored in the database.
     * @return A vector with all the value strings.
     */
    std::vector<std::string> getAllValues();

    /**
     * Get all the key/value pairs stored in the database.
     * @return A list with all the key/value strings.
     */
    std::map<std::string, std::string> getAllPairs();

    /// Clear all entries stored in the database.
    void dropDatabase();
};

#endif  // DATABASE_H
