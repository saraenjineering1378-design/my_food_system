#ifndef DRINK_ITEM_H
#define DRINK_ITEM_H

#include "MenuItem.h"
#include <string>
#include "Enums.h"



class DrinkItem :public MenuItem
{
private:

     ItemType type;

    double volume; // masalan liter
    bool isCold;  // noshidani  sard ya garm


public:

    DrinkItem(ItemType t, int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable,
        double volume, bool isCold);

    void display() const override;

};
#endif