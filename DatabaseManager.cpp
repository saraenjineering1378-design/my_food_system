#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& name) : db(nullptr), dbName(name) {}

DatabaseManager::~DatabaseManager() 
{ 
    close(); 
}

bool DatabaseManager::open() 
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) 
    {
        std::cerr << "error : data base isnt open " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::close() 
{
    if (db) 
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool DatabaseManager::executeQuery(const std::string& sql) 
{
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) 
    {
        std::cerr << "error dar query" << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// metod  khandan etelaat
std::vector<std::vector<std::string>> DatabaseManager::fetchQuery(const std::string& sql) 
{
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        int cols = sqlite3_column_count(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW) 
        {
            std::vector<std::string> row;
            for (int i = 0; i < cols; i++) 
            {
                row.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, i)));
            }
            results.push_back(row);
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

    // method komaki baray daryaft natayej
static int callback(void* data, int argc, char** argv, char** azColName) 
{
    auto* results = static_cast<std::vector<std::map<std::string, std::string>>*>(data);
    std::map<std::string, std::string> row;
    for (int i = 0; i < argc; i++) 
    {
        row[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }
    results->push_back(row);
    return 0;
}

std::vector<std::map<std::string, std::string>> DatabaseManager::fetchAll(const std::string& sql) const
{
    std::vector<std::map<std::string, std::string>> results;
    char* zErrMsg = 0;
    
    int rc = sqlite3_exec(db, sql.c_str(), callback, &results, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    
    return results;

}

int DatabaseManager::getLastInsertId() const
{
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

sqlite3* DatabaseManager::getDatabase() const 
{
    return db;
}

std::string DatabaseManager::escapeSql(const std::string& input) 
{
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find("'", pos)) != std::string::npos) 
    {
        result.replace(pos, 1, "''");
        pos += 2;
    }
    return result;
}

