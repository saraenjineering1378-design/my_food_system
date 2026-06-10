#ifndef DESSERT_ITEM_H
#define DESSERT_ITEM_H

#include "MenuItem.h"
#include <string>
#include "Enums.h"


class DessertItem : public MenuItem
{
private:

    ItemType type;
    double sugarLevel; // darsad shirini

public:
    DessertItem(ItemType t,int id, const std::string& name, const std::string& description,
                double basePrice, bool isAvailable, double sugarLevel);

    void display() const override;
};
#endif
