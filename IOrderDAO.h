#ifndef IORDER_DAO_H
#define IORDER_DAO_H

#include "Order.h"
#include "Enums.h" 
#include <vector>

class IOrderDAO 
{
public:

    virtual ~IOrderDAO() {}

    
    virtual bool addOrder(Order* order) = 0;
    virtual Order* findOrderById(int id) = 0;
    virtual std::vector<Order*> getAllOrders() = 0;
    virtual std::vector<Order*> getOrdersByCustomer(int customerId) = 0;
    virtual std::vector<Order*> getOrdersByRestaurant(int restaurantId) = 0;
    virtual void updateOrderStatus(Order* order) = 0;
   

};

#endif
