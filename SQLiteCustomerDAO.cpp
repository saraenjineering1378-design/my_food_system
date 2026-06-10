
#include "SQLiteCustomerDAO.h"
#include "Customer.h"        
#include "DatabaseManager.h"  
#include <string>             


void SQLiteCustomerDAO::addCustomer(Customer* customer)
{
    // ساخت کوئری با رعایت دقیق پرانتزها و کوتیشن‌ها
    
std::string sql = "INSERT INTO Customers (name, walletBalance, password) VALUES ('" +
                  DatabaseManager::escapeSql(customer->getName()) + "', " + 
                  std::to_string(customer->getWallet()) + ", '" + 
                  DatabaseManager::escapeSql(customer->getPassword()) + "');";;
    // اجرای کوئری
    if (!dbManager.executeQuery(sql)) 
    {
        std::cerr << "❌ addCustomer failed\n";
        return;
    }

    // گرفتن ID جدید
    int newId = dbManager.getLastInsertId();
    customer->setCustomerId(newId);
}


std::vector<Customer*> SQLiteCustomerDAO::getAllCustomers() const 
{
    std::vector<Customer*> list;
    std::string sql = "SELECT * FROM Customers;";

    auto rows = dbManager.fetchAll(sql);
        for (const auto& row : rows) 
        {
       
Customer* c = new Customer(
    std::stoi(row.at("id")),
    row.at("name"),
    std::atof(row.at("walletBalance").c_str()), 
    row.at("password") 
);
        list.push_back(c);
        }
    return list;
}


Customer* SQLiteCustomerDAO::findCustomerById(int id) const 
{
    std::string sql = "SELECT * FROM Customers WHERE id = " + std::to_string(id) + ";";
    auto rows = dbManager.fetchAll(sql);
    
    if (rows.empty()) return nullptr;
    
    auto& row = rows[0];
return new Customer(
    std::stoi(row.at("id")),
    row.at("name"),
    std::atof(row.at("walletBalance").c_str()),
    row.at("password") // پسورد را اینجا هم اضافه کن
);
}
void SQLiteCustomerDAO::updateWallet(int customerId, double newBalance) {
    // 🔹 نام ستون دقیقاً به walletBalance اصلاح شد
    std::string sql = "UPDATE Customers SET walletBalance = " + std::to_string(newBalance) + " WHERE id = " + std::to_string(customerId) + ";";
    char* errMsg = nullptr;
    
    // 🔹 پوینتر دیتابیس را از dbManager شما بیرون می‌کشیم
    sqlite3* db = dbManager.getDatabase(); 
    
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error updating wallet: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "🌐 Wallet synced with Database!\n";
    }
}

