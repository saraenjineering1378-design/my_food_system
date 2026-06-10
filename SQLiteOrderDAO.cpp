#include "SQLiteOrderDAO.h"
#include <iostream>
#include <string>
#include <vector>


SQLiteOrderDAO::SQLiteOrderDAO(DatabaseManager& manager)
    : dbManager(manager) {}

void SQLiteOrderDAO::loadOrderItems(Order* order) {
    if (!order || !menuItemDAO) {
        return;
    }

    std::string sql = "SELECT menuItemId, quantity FROM OrderItems WHERE orderId = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement for loading order items: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, order->getOrderId());

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int menuItemId = sqlite3_column_int(stmt, 0);
        int quantity = sqlite3_column_int(stmt, 1);

        
        MenuItem* item = menuItemDAO->findMenuItemById(menuItemId);
        if (item) 
        {
            
            for (int i = 0; i < quantity; ++i) 
            {
                order->addItem(item);
            }
        }
    }
    sqlite3_finalize(stmt);
}


bool SQLiteOrderDAO::addOrder(Order* order) 
{
    if (!order) return false;

    std::string sqlOrder = "INSERT INTO Orders (customerId, restaurantId, totalPrice, status, orderDate) VALUES (?, ?, ?, ?, datetime('now'));";
    sqlite3_stmt* stmt = nullptr;

    // ۱. ثبت سفارش اصلی در جدول Orders
    if (sqlite3_prepare_v2(dbManager.getDatabase(), sqlOrder.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, order->getCustomerId());
        sqlite3_bind_int(stmt, 2, order->getRestaurantId());
        sqlite3_bind_double(stmt, 3, order->calculateTotalPrice()); // محاسبه قیمت کل
        sqlite3_bind_int(stmt, 4, static_cast<int>(order->getStatus()));

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error inserting order: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }
    } else {
        std::cerr << "Error preparing order statement: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return false;
    }
    sqlite3_finalize(stmt);

    // ۲. گرفتن ID آخرین سفارش ثبت شده
    int orderId = sqlite3_last_insert_rowid(dbManager.getDatabase());

    // ۳. 🔹 اصلاح باگ اصلی: ثبت تک تک غذاهای سفارش در جدول OrderItems 🔹
    for (auto const& item : order->getItems()) {
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
        sqlite3_finalize(itemStmt); // آزاد کردن حافظه برای هر آیتم
    }

    return true; // همه چیز با موفقیت ثبت شد
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

        // ۱. دیتابیس غذاها را لود می‌کند تا در آرایه داخلی سفارش قرار گیرند
        loadOrderItems(order);

        // ۲. حالا که غذاها لود شدند، قیمت واقعی را مستقیماً از روی غذاها حساب می‌کنیم
        order->calculateTotalPrice();

        orders.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orders;
}




Order* SQLiteOrderDAO::findOrderById(int id) {
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


void SQLiteOrderDAO::updateOrderStatus(Order* order) {
    if (!order) return; //age sefareshi nabod alaki kari nakonim
    std::string sql = "UPDATE Orders SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) 
    {
        //shenase va vaziyat ro az khod shey mikeshim biron
        sqlite3_bind_int(stmt, 1, static_cast<int>(order->getStatus()));
        sqlite3_bind_int(stmt, 2, order->getOrderId());

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error updating order status: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        }
    }
    sqlite3_finalize(stmt);
}


std::vector<Order*> SQLiteOrderDAO::getOrdersByCustomer(int customerId) 
{
    std::vector<Order*> orders;
    const char* sql = "SELECT id, customerId, restaurantId, status FROM Orders WHERE customerId = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
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


std::vector<Order*> SQLiteOrderDAO::getOrdersByRestaurant(int restaurantId) {
    std::vector<Order*> orders;
    const char* sql = "SELECT id, customerId, restaurantId, status FROM Orders WHERE restaurantId = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare getOrdersByRestaurant: " << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        return orders;
    }

    sqlite3_bind_int(stmt, 1, restaurantId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int orderId = sqlite3_column_int(stmt, 0);
        int customerId = sqlite3_column_int(stmt, 1);
        int restId = sqlite3_column_int(stmt, 2);
        OrderStatus status = static_cast<OrderStatus>(sqlite3_column_int(stmt, 3));

        Order* order = new Order(orderId, customerId, restId);
        order->updateStatus(status);
        loadOrderItems(order);
        orders.push_back(order);
    }

    sqlite3_finalize(stmt);
    return orders;
}
