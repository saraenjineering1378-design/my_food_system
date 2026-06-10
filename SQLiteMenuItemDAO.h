#ifndef SQLITE_MENU_ITEM_DAO_H
#define SQLITE_MENU_ITEM_DAO_H

#include "IMenuItemDAO.h"
#include "DatabaseManager.h"
#include "FoodItem.h"
#include "DrinkItem.h"
#include "DessertItem.h"
#include <vector>

class SQLiteMenuItemDAO : public IMenuItemDAO 
{
private:
    DatabaseManager& dbManager;
    IMenuItemDAO* menuItemDAO;

public:
    SQLiteMenuItemDAO(DatabaseManager& db) : dbManager(db) {}

    bool addMenuItem(int restaurantId, MenuItem* item) override;
    void removeMenuItem(int id) override;
    MenuItem* findMenuItemById(int id) override; 
    std::vector<MenuItem*> getMenuItemsByRestaurant(int restaurantId) override;
    void updateMenuItem(MenuItem* item) override;
    std::vector<MenuItem*> getAllMenuItems() override;
};

#endif
