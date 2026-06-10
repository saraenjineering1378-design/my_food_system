#ifndef SQLITE_ORDER_DAO_H
#define SQLITE_ORDER_DAO_H

#include "IOrderDAO.h"
#include "DatabaseManager.h"
#include <vector>
#include "IMenuItemDAO.h"

class SQLiteOrderDAO : public IOrderDAO 
{
private:
    DatabaseManager& dbManager;
    IMenuItemDAO* menuItemDAO; 



    void loadOrderItems(Order* order); 

public:
    SQLiteOrderDAO(DatabaseManager& db); 

    bool addOrder(Order* order) override;
    std::vector<Order*> getOrdersByCustomer(int customerId) override;
    std::vector<Order*> getOrdersByRestaurant(int restaurantId) override;
    void updateOrderStatus(Order* order) override;
    virtual std::vector<Order*> getAllOrders() override;
    virtual Order* findOrderById(int id) override;

};

#endif
