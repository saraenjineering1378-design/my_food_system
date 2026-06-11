#ifndef SQLITE_RESTAURANT_DAO_H
#define SQLITE_RESTAURANT_DAO_H

#include "IRestaurantDAO.h"
#include "DatabaseManager.h"
#include <vector>

class SQLiteRestaurantDAO : public IRestaurantDAO {
private:
    DatabaseManager& dbManager;

public:
    SQLiteRestaurantDAO(DatabaseManager& db) : dbManager(db) {}

    void addRestaurant(Restaurant* restaurant) override;
    void removeRestaurant(int id) override;
    Restaurant* findRestaurantById(int id) const override;
    std::vector<Restaurant*> getAllRestaurants() const override;
    void updateRestaurant(Restaurant* restaurant) override;
    std::vector<Order*> getOrdersByRestaurantId(int restaurantId) const;
    bool updateOrderStatus(int orderId, int newStatus);

};

#endif
