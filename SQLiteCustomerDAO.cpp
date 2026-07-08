#include "SQLiteCustomerDAO.h"
#include "Customer.h"        
#include "DatabaseManager.h"  
#include "MembershipLevel.h"
#include <string>             


void SQLiteCustomerDAO::addCustomer(Customer* customer)
{
    std::string sql = "INSERT INTO Customers (name, walletBalance, password, loyaltyPoints, membershipLevel) VALUES ('" +
                  DatabaseManager::escapeSql(customer->getName()) + "', " + 
                  std::to_string(customer->getWallet()) + ", '" + 
                  DatabaseManager::escapeSql(customer->getPassword()) + "', 0, 'Normal');";
    
    if (!dbManager.executeQuery(sql)) 
    {
        std::cerr << "❌ addCustomer failed\n";
        return;
    }

    int newId = dbManager.getLastInsertId();
    customer->setCustomerId(newId);
}

Customer* SQLiteCustomerDAO::findCustomerById(int id) const 
{
    std::string sql = "SELECT id, name, walletBalance, password, loyaltyPoints, membershipLevel, badge, monthlyCoupons FROM Customers WHERE id = " + std::to_string(id) + ";";
    auto rows = dbManager.fetchAll(sql);
    
    if (rows.empty()) return nullptr;
    
    auto& row = rows[0];
    int loyalty = 0;
    std::string levelName = "Normal";
    std::string badge = "None";
    int monthlyCoupons = 0;
    
    try {
        loyalty = std::stoi(row.at("loyaltyPoints"));
    } catch (...) { loyalty = 0; }
    try {
        levelName = row.at("membershipLevel");
    } catch (...) { levelName = "Normal"; }
    try {
        badge = row.at("badge");
    } catch (...) { badge = "None"; }
    try {
        monthlyCoupons = std::stoi(row.at("monthlyCoupons"));
    } catch (...) { monthlyCoupons = 0; }

    Customer* c = new Customer(
        std::stoi(row.at("id")),
        row.at("name"),
        std::atof(row.at("walletBalance").c_str()),
        row.at("password")
    );
    c->setLoyaltyPoints(loyalty);
    c->setLevel(createLevelFromName(levelName));
    c->setBadge(badge);
    c->setMonthlyCoupons(monthlyCoupons);
    return c;
}

std::vector<Customer*> SQLiteCustomerDAO::getAllCustomers() const 
{
    std::vector<Customer*> list;
    std::string sql = "SELECT id, name, walletBalance, password, loyaltyPoints, membershipLevel, badge, monthlyCoupons FROM Customers;";

    auto rows = dbManager.fetchAll(sql);
    for (const auto& row : rows) 
    {
        int loyalty = 0;
        std::string levelName = "Normal";
        std::string badge = "None";
        int monthlyCoupons = 0;
        
        try {
            loyalty = std::stoi(row.at("loyaltyPoints"));
        } catch (...) { loyalty = 0; }
        
        try {
            levelName = row.at("membershipLevel");
        } catch (...) { levelName = "Normal"; }
        
        try {
            badge = row.at("badge");
        } catch (...) { badge = "None"; }
        
        try {
            monthlyCoupons = std::stoi(row.at("monthlyCoupons"));
        } catch (...) { monthlyCoupons = 0; }

        Customer* c = new Customer(
            std::stoi(row.at("id")),
            row.at("name"),
            std::atof(row.at("walletBalance").c_str()), 
            row.at("password")
        );
        
        c->setLoyaltyPoints(loyalty);
        c->setLevel(createLevelFromName(levelName));
        c->setBadge(badge);
        c->setMonthlyCoupons(monthlyCoupons);
        list.push_back(c);
    }
    return list;
}
void SQLiteCustomerDAO::updateCustomerLevelAndPoints(int customerId, int points, const std::string& level) 
{
    std::string sql = "UPDATE Customers SET loyaltyPoints = " + std::to_string(points) + 
                      ", membershipLevel = '" + level + "' WHERE id = " + std::to_string(customerId) + ";";
    dbManager.executeQuery(sql);
}


void SQLiteCustomerDAO::addMembershipHistory(int customerId, const std::string& oldLevel, const std::string& newLevel, const std::string& reason) 
{
    std::string sql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                      std::to_string(customerId) + ", '" + oldLevel + "', '" + newLevel + "', datetime('now'), '" + reason + "');";
    dbManager.executeQuery(sql);
}


void SQLiteCustomerDAO::assignMonthlyCoupons() 
{
    // daryaft hame karbaran
    auto customers = getAllCustomers();
    for (auto* c : customers) 
    {
        int coupons = 0;
        std::string level = c->getLevelName();
        if (level == "Silver" || level == "Gold") coupons = 1;
        else if (level == "VIP") coupons = 3;
        // else Normal = 0
        
        c->setMonthlyCoupons(coupons);
        updateMonthlyCoupons(c->getCustomerId(), coupons);
        delete c;
    }
}

void SQLiteCustomerDAO::updateMonthlyCoupons(int customerId, int count) 
{
    std::string sql = "UPDATE Customers SET monthlyCoupons = " + std::to_string(count) + 
                      " WHERE id = " + std::to_string(customerId) + ";";
    dbManager.executeQuery(sql);
}

void SQLiteCustomerDAO::updateBadge(int customerId, const std::string& badge) 
{
    std::string sql = "UPDATE Customers SET badge = '" + badge + 
                      "' WHERE id = " + std::to_string(customerId) + ";";
    dbManager.executeQuery(sql);
}

void SQLiteCustomerDAO::updateWallet(int customerId, double newBalance) {
    std::string sql = "UPDATE Customers SET walletBalance = " + std::to_string(newBalance) + " WHERE id = " + std::to_string(customerId) + ";";
    char* errMsg = nullptr;
    sqlite3* db = dbManager.getDatabase(); 
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error updating wallet: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } 
    /*else {
        std::cout << "NOSH JAN 😋\n";
    }*/
}