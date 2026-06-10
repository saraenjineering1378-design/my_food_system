#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H
#pragma once



#include "sqlite3.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>


class DatabaseManager
{

private:

    sqlite3* db;
    std::string dbName;

public:

    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();

    
    bool open(); // baray baz kardan data base
    void close(); //baray bastan data base
    bool executeQuery(const std::string& sql); // baray crud kardan

     std::vector<std::vector<std::string>> fetchQuery(const std::string& sql);
     //gereftan natayej select toy yek list do bodi

    std::vector<std::map<std::string, std::string>> fetchAll(const std::string& sql) const;
    //method baray khandan dadeha 

    int getLastInsertId() const;

    sqlite3* getDatabase() const;// be moteghayer private dastresi dashte bashim
    static std::string escapeSql(const std::string& input);


};
#endif