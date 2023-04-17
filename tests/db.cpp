#include "../src/libs/catch2/catch_amalgamated.hpp"
#include "../old_tests/Tests.h"
#include "../include/web3cpp/DB.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using Catch::Matchers::Equals;

namespace TDb
{
    TEST_CASE("Test DB Operations (CRUD)")
    {
        SECTION("TEST Operations")
        {
            boost::filesystem::path walletFolder = "web3cpp-test-wallet";
            Database db("testdb", walletFolder);
            std::map<std::string, std::string> testData = {
                {"value0", "Test Value 0"},
                {"value1", "Test Value 1"},
                {"value2", "Test Value 2"},
                {"value3", "Test Value 3"},
                {"value4", "Test Value 4"}};
            bool okC, okR, okU, okD;
            std::string logC = "", logR = "", logU = "", logD = "";
            logC = "* Subtest: CREATE (add values to DB)\n";
            int ctC = 0;
            for (std::pair<std::string, std::string> testPair : testData)
            {
                bool added = db.putKeyValue(testPair.first, testPair.second);
                logC += "* {'" + testPair.first + "', '" + testPair.second + "'} added: " + ((added) ? "YES" : "NO") + "\n";
                ctC += added;
            }
            okC = (ctC == testData.size());

            // (R)ead (keyExists, getKeyValue, getAllKeys, getAllValues, getAllPairs)
            if (okC)
            {
                logR = "* Subtest: READ (get values from DB)\n";
                bool allExists = true, allSameValue = true;
                for (std::pair<std::string, std::string> testPair : testData)
                {
                    bool exists = db.keyExists(testPair.first);
                    bool isSameValue = (db.getKeyValue(testPair.first) == testPair.second);
                    logR += "* Key '" + testPair.first + "' exists: " + ((exists) ? "YES" : "NO") + "\n";
                    logR += "* Value '" + testPair.second + "' matches: " + ((isSameValue) ? "YES" : "NO") + "\n";
                    if (!exists)
                        allExists = false;
                    if (!isSameValue)
                        allSameValue = false;
                }
                std::vector<std::string> splitKeys, splitValues;
                for (std::pair<std::string, std::string> testPair : testData)
                {
                    splitKeys.push_back(testPair.first);
                    splitValues.push_back(testPair.second);
                }
                std::vector<std::string> allKeys = db.getAllKeys();
                std::vector<std::string> allValues = db.getAllValues();
                std::map<std::string, std::string> allPairs = db.getAllPairs();
                bool allKeyCheck = (allKeys == splitKeys);
                bool allValueCheck = (allValues == splitValues);
                bool allPairCheck = (allPairs == testData);
                logR += std::string("* getAllKeys() matches test keys: ") + ((allKeyCheck) ? "YES" : "NO") + "\n";
                logR += std::string("* getAllValues() matches test values: ") + ((allValueCheck) ? "YES" : "NO") + "\n";
                logR += std::string("* getAllPairs() matches test pairs: ") + ((allPairCheck) ? "YES" : "NO") + "\n";
                okR = (allExists && allSameValue && allKeyCheck && allValueCheck && allPairCheck);
            }

            // (U)pdate (putKeyValue)
            if (okC && okR)
            {
                logU = "* Subtest: UPDATE (overwrite existing values for keys)\n";
                int ctU = 0;
                std::map<std::string, std::string> newData = {
                    {"value0", "New Value A"},
                    {"value1", "New Value B"},
                    {"value2", "New Value C"},
                    {"value3", "New Value D"},
                    {"value4", "New Value E"}};
                for (std::pair<std::string, std::string> newPair : newData)
                {
                    bool changed = db.putKeyValue(newPair.first, newPair.second);
                    logU += "* Key '" + newPair.first + "' changed value to '" +
                            newPair.second + "': " + ((changed) ? "YES" : "NO") + "\n";
                    ctU += changed;
                }
                okU = (ctU == newData.size());
            }

            // (D)elete (deleteKeyValue, dropDatabase)
            if (okC && okR && okU)
            {
                logD = "* Subtest: DELETE (erase existing keys and the whole database)\n";
                bool deleteOK = db.deleteKeyValue("value0");
                db.dropDatabase();
                bool dropOK = (db.getAllKeys().size() == 0);
                logD += std::string("* 'value0' deleted: ") + ((deleteOK) ? "YES" : "NO") + "\n";
                logD += std::string("* Database dropped: ") + ((dropOK) ? "YES" : "NO") + "\n";
                okD = (deleteOK && dropOK);
            }
            REQUIRE(okC);
            REQUIRE(okR);
            REQUIRE(okU);
            REQUIRE(okD);

            db.dropDatabase();
        }
    }

}