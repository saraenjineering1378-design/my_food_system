#ifndef IRESTAURANT_DAO_H
#define IRESTAURANT_DAO_H

#include "Restaurant.h"
#include <vector>


class IRestaurantDAO
{

public:

    virtual ~IRestaurantDAO() {}
    virtual void addRestaurant(Restaurant* restaurant) = 0;
    virtual Restaurant* findRestaurantById(int id) const = 0;
    virtual std::vector<Restaurant*> getAllRestaurants() const = 0;
    virtual void removeRestaurant(int id) = 0;
    virtual void updateRestaurant(Restaurant* restaurant) = 0;
    virtual std::vector<Order*> getOrdersByRestaurantId(int restaurantId) const = 0;
    virtual bool updateOrderStatus(int orderId, int newStatus) = 0;


};
#endif