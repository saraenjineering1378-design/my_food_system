#ifndef FOOD_ITEM_H
#define FOOD_ITEM_H

#include <string>
#include "MenuItem.h"
#include "Enums.h"

class FoodItem :public MenuItem //verasat
{
private:

    int cookingTime; //be dadighe
    bool isVegetarian; //ghazaye giyahi ya na?

public:

    FoodItem(int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable,
    int cookingTime,bool isVegetarian);

    void display() const override;
    bool getIsVegetarian() const;
    void setIsVegetarian(bool isVeg);
    int getPreparationTime() const;

};
#endif