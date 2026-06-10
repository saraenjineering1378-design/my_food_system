#include "SQLiteMenuItemDAO.h"
#include <string>

bool SQLiteMenuItemDAO::addMenuItem(int restaurantId, MenuItem* item)
{
    if (!item) return false;

    int typeNum = 3;
    if (dynamic_cast<DessertItem*>(item)) typeNum = 1;
    else if (dynamic_cast<DrinkItem*>(item)) typeNum = 2;
    else if (FoodItem* food = dynamic_cast<FoodItem*>(item)) {
        typeNum = 3;
    }

    std::string sql =
        "INSERT INTO MenuItems (restaurantId, name, description, basePrice, isAvailable, type) VALUES (" +
        std::to_string(restaurantId) + ", '" +
        item->getName() + "', '" +
        item->getDescription() + "', " +
        std::to_string(item->getBasePrice()) + ", " +
        (item->getIsAvailable() ? "1" : "0") + ", " +
        std::to_string(typeNum) + ");";

    if (!dbManager.executeQuery(sql)) 
    {
        std::cerr << "addMenuItemToRestaurant failed\n";
        return false;
    }
    
    //daryaft id tolid shode az database va tanzim an roy shey menu 
    int newId = dbManager.getLastInsertId(); 
    item->setId(newId); 
    
    return true; 
}




std::vector<MenuItem*> SQLiteMenuItemDAO::getMenuItemsByRestaurant(int restaurantId) 
{
    std::vector<MenuItem*> list;
    std::string sql = "SELECT * FROM MenuItems WHERE restaurantId = " + std::to_string(restaurantId) + ";";
    
    auto rows = dbManager.fetchAll(sql);

   
    for (size_t i = 0; i < rows.size(); ++i) 
    {
        std::map<std::string, std::string>& row = rows[i];
        
        int id = std::stoi(row["id"]);
        std::string name = row["name"];
        std::string desc = row["description"];
        double price = std::stod(row["basePrice"]);
        bool available = (std::stoi(row["isAvailable"]) != 0);
        int type = std::stoi(row["type"]);

        MenuItem* item = nullptr;

    
        
        bool isVeg = false;
        if (type == 1) item = new DessertItem(ItemType::DESSERT, id, name, desc, price, available, 0.5);//polymorphism
        else if (type == 2) item = new DrinkItem(ItemType::DRINK, id, name, desc, price, available, 330.0, true);
        else item = new FoodItem(id, name, desc, price, available, 20, isVeg);


        if (!available) item->setIsAvailable(false);
        list.push_back(item);
    }
    return list;
}
std::vector<MenuItem*> SQLiteMenuItemDAO::getAllMenuItems()
{
    std::vector<MenuItem*> list;
    std::string sql = "SELECT * FROM MenuItems;";
    auto rows = dbManager.fetchAll(sql);

    for (auto& row : rows) // for jadid
    {
        int id = std::stoi(row["id"]);
        std::string name = row["name"];
        std::string desc = row["description"];
        double price = std::stod(row["basePrice"]);
        bool available = (std::stoi(row["isAvailable"]) != 0);
        int type = std::stoi(row["type"]);

        MenuItem* item = nullptr;
        if (type == 1) item = new DessertItem(ItemType::DESSERT, id, name, desc, price, available, 0.5);
        else if (type == 2) item = new DrinkItem(ItemType::DRINK, id, name, desc, price, available, 330.0, true);
        else item = new FoodItem(id, name, desc, price, available, 20, false);

        list.push_back(item);
    }
    return list;
}

MenuItem* SQLiteMenuItemDAO::findMenuItemById(int id) 
{
    std::string sql = "SELECT id, name, description, basePrice, isAvailable, type FROM MenuItems WHERE id = ?;";
    sqlite3_stmt* stmt;
    MenuItem* item = nullptr;

    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // estekhraj dade ha az radif peyda shode
            int itemId = sqlite3_column_int(stmt, 0);
            
            std::string name = "";
            const unsigned char* nameText = sqlite3_column_text(stmt, 1);
            if (nameText) name = reinterpret_cast<const char*>(nameText);
            
            std::string desc = "";
            const unsigned char* descText = sqlite3_column_text(stmt, 2);
            if (descText) desc = reinterpret_cast<const char*>(descText);
            
            double price = sqlite3_column_double(stmt, 3);
            bool available = (sqlite3_column_int(stmt, 4) != 0);
            int type = sqlite3_column_int(stmt, 5);

            if (type == 1) {
                //adad 0 be onvan pish farz sugurlevel
                 item = new DessertItem(ItemType::DESSERT, id, name, desc, price, available, 0);
            } else if (type == 2) {
                // 0.0 و false baray volume va iscold
                item = new DrinkItem(ItemType::DRINK, id, name, desc, price, available, 0.0, false);

            } else {
                
                item = new FoodItem(itemId, name, desc, price, available, 20, false); 
            }
        }
    }
    sqlite3_finalize(stmt);
    return item;
}


void SQLiteMenuItemDAO::updateMenuItem(MenuItem* item) {
    if (!item) return;

    std::string sql = "UPDATE MenuItems SET name = ?, basePrice = ?, description = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, item->getName().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, item->getBasePrice());
        sqlite3_bind_text(stmt, 3, item->getDescription().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, item->getId());
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "❌ Error updating item in the database! Details: " 
                      << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        } else {
            
            if (sqlite3_changes(dbManager.getDatabase()) > 0) {
                std::cout << "✅ Item with ID " << item->getId() << " successfully updated in the database! 🎉" << std::endl;
            } else {
                std::cout << "⚠️+ Warning: No item found with ID " << item->getId() << " to update!" << std::endl;
            }
        }
    } else {
        std::cerr << "❌ Failed to prepare update query! Details: " 
                  << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
    }
    
    sqlite3_finalize(stmt);
}







void SQLiteMenuItemDAO::removeMenuItem(int id) {
    std::string sql = "DELETE FROM MenuItems WHERE id = ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(dbManager.getDatabase(), sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "❌ Error deleting item from database: " 
                      << sqlite3_errmsg(dbManager.getDatabase()) << std::endl;
        } else {
            // barrasi in ke radifi hazf shode ya na
                    if (sqlite3_changes(dbManager.getDatabase()) > 0) {
                std::cout << "✅ Item with ID " << id << " successfully deleted from database!" << std::endl;
            } else {
                std::cout << "⚠️ No item found with ID " << id << " in database!" << std::endl;
            }
        }
    } else {
        std::cerr << "❌ Failed to prepare delete query!" << std::endl;
    }
    
    sqlite3_finalize(stmt);
}


