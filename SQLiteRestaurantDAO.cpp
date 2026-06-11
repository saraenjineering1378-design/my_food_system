#include <iostream>
#include <vector>
#include <sstream>
#include "SQLiteRestaurantDAO.h"
#include"Order.h"

// tabe komaki baray jolo girri az eror 
static std::string escapeSql(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c == '\'') out += "''";
        else out += c;
    }
    return out;
}

std::vector<Restaurant*> SQLiteRestaurantDAO::getAllRestaurants() const
{
    std::vector<Restaurant*> list;
    std::string sql = "SELECT id, name, address, phoneNumber, description, isActive, estimatedPrepTime, password FROM Restaurants;";
    
    auto rows = dbManager.fetchAll(sql);
    
    for (size_t i = 0; i < rows.size(); i++) 
    {
        std::map<std::string, std::string>& row = rows[i];

        int id = std::stoi(row["id"]);
        std::string name = row["name"];
        std::string address = row["address"];
        std::string phone = row["phoneNumber"];
        std::string desc = row["description"];
        int prepTime = std::stoi(row["estimatedPrepTime"]);
        bool active = (std::stoi(row["isActive"]) != 0);
        std::string password = row["password"]; 

        Restaurant* r = new Restaurant(id, name, address, prepTime, phone, desc, active, nullptr, nullptr, password);
        list.push_back(r);
    }
    return list;
}

void SQLiteRestaurantDAO::addRestaurant(Restaurant* restaurant) 
{
    if (!restaurant) return;

    std::ostringstream sql;
    sql << "INSERT INTO Restaurants (name, address, estimatedPrepTime, phoneNumber, description, isActive, password) VALUES ('"
        << escapeSql(restaurant->getName()) << "', '"
        << escapeSql(restaurant->getAddress()) << "', "
        << restaurant->getEstimatedPrepTime() << ", '"
        << escapeSql(restaurant->getPhoneNumber()) << "', '"
        << escapeSql(restaurant->getDescription()) << "', "
        << (restaurant->getIsActive() ? 1 : 0) << ", '"
        << escapeSql(restaurant->getPassword()) << "');";

    if (!dbManager.executeQuery(sql.str())) {
        std::cerr << "addRestaurant failed.\n";
        return;
    }

    int newId = dbManager.getLastInsertId();
    restaurant->setId(newId);
}

void SQLiteRestaurantDAO::updateRestaurant(Restaurant* restaurant) 
{
    if (!restaurant) return;

    std::string sql = "UPDATE Restaurants SET name = ?, address = ?, isActive = ?, estimatedPrepTime = ?, phoneNumber = ?, description = ?, password = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, restaurant->getName().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, restaurant->getAddress().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, restaurant->getIsActive() ? 1 : 0);
        sqlite3_bind_int(stmt, 4, restaurant->getEstimatedPrepTime());
        sqlite3_bind_text(stmt, 5, restaurant->getPhoneNumber().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, restaurant->getDescription().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, restaurant->getPassword().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 8, restaurant->getId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating restaurant: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
    }
    sqlite3_finalize(stmt);
}

void SQLiteRestaurantDAO::removeRestaurant(int id) 
{
    std::string sql = "DELETE FROM Restaurants WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error deleting restaurant: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
    }
    sqlite3_finalize(stmt);
}
Restaurant* SQLiteRestaurantDAO::findRestaurantById(int id) const 
{
    std::string sql = "SELECT id, name, address, isActive, estimatedPrepTime, phoneNumber, description, password FROM Restaurants WHERE id = ?;";
    sqlite3_stmt* stmt;
    Restaurant* restaurant = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* passText = sqlite3_column_text(stmt, 7);
            std::string password = passText ? reinterpret_cast<const char*>(passText) : "1234";

            restaurant = new Restaurant(id, "", "", 30, "", "", true, nullptr, nullptr, password);

            restaurant->setId(sqlite3_column_int(stmt, 0));
            const unsigned char* nameText = sqlite3_column_text(stmt, 1);
            if (nameText) restaurant->setName(reinterpret_cast<const char*>(nameText));
            const unsigned char* addressText = sqlite3_column_text(stmt, 2);
            if (addressText) restaurant->setAddress(reinterpret_cast<const char*>(addressText));
            restaurant->setIsActive(sqlite3_column_int(stmt, 3) != 0);
            restaurant->setEstimatedPrepTime(sqlite3_column_int(stmt, 4));
            const unsigned char* phoneText = sqlite3_column_text(stmt, 5);
            if (phoneText) restaurant->setPhoneNumber(reinterpret_cast<const char*>(phoneText));
            const unsigned char* descText = sqlite3_column_text(stmt, 6);
            if (descText) restaurant->setDescription(reinterpret_cast<const char*>(descText));
        }
    }
    sqlite3_finalize(stmt);
    return restaurant; 
}
std::vector<Order*> SQLiteRestaurantDAO::getOrdersByRestaurantId(int restaurantId) const 
{
    std::vector<Order*> orders;
    const char* sql = "SELECT id, customerId, restaurantId, status, totalPrice FROM Orders WHERE restaurantId = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, restaurantId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            int custId = sqlite3_column_int(stmt, 1);
            int restId = sqlite3_column_int(stmt, 2);
            int statusInt = sqlite3_column_int(stmt, 3);      
            double price = sqlite3_column_double(stmt, 4);

            Order* o = new Order(id, custId, restId);
            o->setTotalPrice(price);

            // tabdil adad sahih b enum
            switch (statusInt) {
                case 1: o->setStatus(OrderStatus::Preparing); break;
                case 2: o->setStatus(OrderStatus::Delivered); break;
                case 3: o->setStatus(OrderStatus::Completed); break;
                case 4: o->setStatus(OrderStatus::Cancelled); break;
                default: o->setStatus(OrderStatus::Pending); break;
            }
            orders.push_back(o);
        }
    } else {
        std::cerr << "Failed to prepare getOrdersByRestaurantId: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
    }
    sqlite3_finalize(stmt);
    return orders;
}

bool SQLiteRestaurantDAO::updateOrderStatus(int orderId, int newStatus) 
{
    sqlite3_busy_timeout(dbManager.getDatabase(), 5000);
    const char* sql = "UPDATE Orders SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    bool success = false;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, newStatus);
        sqlite3_bind_int(stmt, 2, orderId);
        
        int rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            success = true;
        } else {
            std::cerr << "Error updating order status: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
    }
    
    sqlite3_finalize(stmt);
    return success;
}

