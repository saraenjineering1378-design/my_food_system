#ifndef DATABASE_INITIALIZER_H
#define DATABASE_INITIALIZER_H

#include "DatabaseManager.h"
#include <string>
#include <vector>

class DatabaseInitializer {
public:
    static void initialize(DatabaseManager& db) {
        // forien key
        db.executeQuery("PRAGMA foreign_keys = ON;");

        // list tamam dastorat sakht jadval
        std::vector<std::string> queries;

        // jadval resturanha
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS Restaurants ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "address TEXT, "
            "estimatedPrepTime INTEGER DEFAULT 30, "
            "phoneNumber TEXT, "
            "description TEXT, "
            "isActive INTEGER DEFAULT 1, "
            "password TEXT DEFAULT '1234'" 
            ");"
        );

        // jadval moshtariha
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS Customers ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "walletBalance REAL DEFAULT 0.0, "
            "password TEXT DEFAULT '1234', "
            "loyaltyPoints INTEGER DEFAULT 0, "
            "membershipLevel TEXT DEFAULT 'Normal', " 
            "badge TEXT DEFAULT 'None', "        
            "monthlyCoupons INTEGER DEFAULT 0 "  
            ");"
);


      //sakht jadval dar faz dovom //jadval membershiphietory
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS MembershipHistory ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "customerId INTEGER, "
            "oldLevel TEXT, "
            "newLevel TEXT, "
            "changeDate TEXT, "
            "reason TEXT, "
            "FOREIGN KEY(customerId) REFERENCES Customers(id) ON DELETE CASCADE"
            ");"
);


        // jadval item hay menu
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS MenuItems ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "restaurantId INTEGER, "
            "name TEXT NOT NULL, "
            "description TEXT, "
            "basePrice REAL NOT NULL, "
            "isAvailable INTEGER DEFAULT 1, "
            "type INTEGER NOT NULL, " // 1: Dessert, 2: Drink, 3: Food
            "isVegetarian INTEGER DEFAULT 0, "
            "FOREIGN KEY(restaurantId) REFERENCES Restaurants(id) ON DELETE CASCADE);"
        );

        // jadval sefareshat vabaste b menu va moshtari
    queries.push_back(
        "CREATE TABLE IF NOT EXISTS Orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "customerId INTEGER, "
        "restaurantId INTEGER, "
        "totalPrice REAL, "
        "status INTEGER DEFAULT 0, " 
        "orderDate TEXT, "
        "FOREIGN KEY(customerId) REFERENCES Customers(id) ON DELETE CASCADE, "
        "FOREIGN KEY(restaurantId) REFERENCES Restaurants(id) ON DELETE CASCADE);"
);

        // ۵. jadval vaset item hay sefaresh
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS OrderItems ("
            "orderId INTEGER, "
            "menuItemId INTEGER, "
            "quantity INTEGER DEFAULT 1, "
            "FOREIGN KEY(orderId) REFERENCES Orders(id) ON DELETE CASCADE, "
            "FOREIGN KEY(menuItemId) REFERENCES MenuItems(id) ON DELETE CASCADE);"
        );


        // jadval copen ha
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS Coupons ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "code TEXT UNIQUE NOT NULL, "
            "discountPercent REAL DEFAULT 10.0, "
            "expiryDate TEXT, "
            "isUsed INTEGER DEFAULT 0"
            ");"
);

        // jadval ertebat karbaran ba copen
        queries.push_back(
            "CREATE TABLE IF NOT EXISTS UserCoupons ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "customerId INTEGER, "
            "couponId INTEGER, "
            "assignedDate TEXT, "
            "isUsed INTEGER DEFAULT 0, "
            "FOREIGN KEY(customerId) REFERENCES Customers(id) ON DELETE CASCADE, "
            "FOREIGN KEY(couponId) REFERENCES Coupons(id) ON DELETE CASCADE"
            ");"
);

        // ejray tamam dastorat
        
    for (const std::string& sql : queries) {
            db.executeQuery(sql);
        }

        
    }
};

#endif
