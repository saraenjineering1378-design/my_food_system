#include "SQLiteOrderDAO.h"
#include <iostream>
#include <string>
#include <vector>

#include "FoodItem.h"
#include "DrinkItem.h"
#include "DessertItem.h"


SQLiteOrderDAO::SQLiteOrderDAO(DatabaseManager& manager)
    : dbManager(manager) {}



void SQLiteOrderDAO::loadOrderItems(Order* order) 
{
    if (!order) return;

    // yek ja khandan dade ha
    std::string sql = 
        "SELECT oi.menuItemId, oi.quantity, m.name, m.description, m.basePrice, m.isAvailable, m.type "
        "FROM OrderItems oi "
        "JOIN MenuItems m ON oi.menuItemId = m.id "
        "WHERE oi.orderId = ?;";
        
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return;
    }

    sqlite3_bind_int(stmt, 1, order->getOrderId());

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int menuItemId = sqlite3_column_int(stmt, 0);
        int quantity = sqlite3_column_int(stmt, 1);
        
        const char* nameText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* descText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string name = nameText ? nameText : "";
        std::string desc = descText ? descText : "";
        
        double basePrice = sqlite3_column_double(stmt, 4);
        bool isAvailable = sqlite3_column_int(stmt, 5) != 0;
        int typeInt = sqlite3_column_int(stmt, 6);

        
        for (int i = 0; i < quantity; ++i) {
            MenuItem* item = new MenuItem(static_cast<ItemType>(typeInt), menuItemId, name, desc, basePrice, isAvailable);
            order->addItem(item);
        }
    }

    //dastor azad sazi ghofl database
    sqlite3_finalize(stmt);
}



bool SQLiteOrderDAO::addOrder(Order* order) 
{
    if (!order) return false;

    std::string sqlOrder = "INSERT INTO Orders (customerId, restaurantId, totalPrice, status, orderDate) VALUES (?, ?, ?, ?, datetime('now'));";
    sqlite3_stmt* stmt = nullptr;

    // sabt dar database
    if (sqlite3_prepare_v2(dbManager.getDatabase(), sqlOrder.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, order->getCustomerId());
        sqlite3_bind_int(stmt, 2, order->getRestaurantId());
        sqlite3_bind_double(stmt, 3, order->calculateTotalPrice()); // mohasebe gheymat kol
        sqlite3_bind_int(stmt, 4, static_cast<int>(order->getStatus()));

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error inserting order: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
    } else 
    {
        std::cerr << "Error preparing order statement: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return false;
    }
    sqlite3_finalize(stmt);

    // gereftan id akharin sefaresh sabt shode
    int orderId = sqlite3_last_insert_rowid(dbManager.getDatabase());

    //sabt tak tak ghazahay sefaresh dar jadval 
    for (auto const& item : order->getItems()) 
    {
        std::string sqlItems = "INSERT INTO OrderItems (orderId, menuItemId, quantity) VALUES (?, ?, 1);";
        sqlite3_stmt* itemStmt = nullptr;
        
        if (sqlite3_prepare_v2(dbManager.getDatabase(), sqlItems.c_str(), -1, &itemStmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(itemStmt, 1, orderId);
            sqlite3_bind_int(itemStmt, 2, item->getId());
            
            if (sqlite3_step(itemStmt) != SQLITE_DONE) {
                std::cerr << "Error inserting order item: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
            }
        } else {
            std::cerr << "Error preparing order item statement: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
        sqlite3_finalize(itemStmt); //azad  kardan hafeze baray har item
    }
    return true; // hame chiz ba movafaghiyat sabt shod
    
}




std::vector<Order*> SQLiteOrderDAO::getAllOrders() 
{
    std::vector<Order*> orders;
    std::string sql = "SELECT id, customerId, restaurantId, status FROM Orders;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing getAllOrders: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return orders;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        int id = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int restaurantId = sqlite3_column_int(stmt, 2);
        OrderStatus status = static_cast<OrderStatus>(sqlite3_column_int(stmt, 3));

        Order* order = new Order(id, customerId, restaurantId);
        order->updateStatus(status);

        
        loadOrderItems(order);

        
        order->calculateTotalPrice();

        orders.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orders;
}




Order* SQLiteOrderDAO::findOrderById(int id) 
{
    const char* sql = "SELECT id, customerId, restaurantId, status FROM Orders WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    Order* order = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare findOrderById: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return nullptr;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int restaurantId = sqlite3_column_int(stmt, 2);
        OrderStatus status = static_cast<OrderStatus>(sqlite3_column_int(stmt, 3));

        order = new Order(orderId, customerId, restaurantId);
        order->updateStatus(status);

        
        loadOrderItems(order);
    }

    sqlite3_finalize(stmt);
    return order;
}


void SQLiteOrderDAO::updateOrderStatus(Order* order) 
{
    if (!order) return;

    sqlite3_busy_timeout(dbManager.getDatabase(), 5000); 

    std::string sql = "UPDATE Orders SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, static_cast<int>(order->getStatus()));
        sqlite3_bind_int(stmt, 2, order->getOrderId());
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr <<  "error with update : " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
    }
    
    sqlite3_finalize(stmt); // azad kardan ghofl database
}


std::vector<Order*> SQLiteOrderDAO::getOrdersByRestaurant(int restaurantId) 
{
    std::vector<Order*> orders;
    const char* sql = "SELECT id, customerId, restaurantId, status FROM Orders WHERE restaurantId = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare getOrdersByRestaurant: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return orders;
    }

    sqlite3_bind_int(stmt, 1, restaurantId);

    //yek sakhtar movaghat
    struct TempOrder { int id; int custId; int restId; int status; };
    std::vector<TempOrder> tempOrders;

    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        tempOrders.push_back({
            sqlite3_column_int(stmt, 0),
            sqlite3_column_int(stmt, 1),
            sqlite3_column_int(stmt, 2),
            sqlite3_column_int(stmt, 3)
        });
    }

    
    sqlite3_finalize(stmt);

    
    for (const auto& t : tempOrders) 
    {
        Order* order = new Order(t.id, t.custId, t.restId);
        order->updateStatus(static_cast<OrderStatus>(t.status));
        
        
        loadOrderItems(order); 
        orders.push_back(order);
    }
    
    return orders;
}



std::vector<Order*> SQLiteOrderDAO::getOrdersByCustomer(int customerId) 
{
    std::vector<Order*> orders;
    const char* sql = "SELECT id, customerId, restaurantId, status FROM Orders WHERE customerId = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Failed to prepare getOrdersByCustomer: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return orders;
    }

    sqlite3_bind_int(stmt, 1, customerId);

    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        int orderId = sqlite3_column_int(stmt, 0);
        int custId = sqlite3_column_int(stmt, 1);
        int restId = sqlite3_column_int(stmt, 2);
        OrderStatus status = static_cast<OrderStatus>(sqlite3_column_int(stmt, 3));

        Order* order = new Order(orderId, custId, restId);
        order->updateStatus(status);
        loadOrderItems(order);
        orders.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orders;
}