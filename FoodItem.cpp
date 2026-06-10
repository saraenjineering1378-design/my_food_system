#include "FoodItem.h"
#include <iostream>

using namespace std;

FoodItem::FoodItem(int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable,
    int cookingTime, bool isVegetarian)

: MenuItem(ItemType::FOOD, id, name, description, basePrice, isAvailable),
cookingTime(cookingTime), isVegetarian(isVegetarian) {}

void FoodItem::display() const
{
    cout << "----- Food Item -----\n";
     std::cout << "[FOOD] ";//baes mishe befahmim toye list chi hast
    MenuItem::display();

    cout << "Cooking Time: " << cookingTime << " minutes\n";
    cout << "Vegetarian: " << (isVegetarian ? "Yes" : "No") << "\n";
    cout << "----------------------\n";

}
bool FoodItem::getIsVegetarian() const {
    return isVegetarian;
}

void FoodItem::setIsVegetarian(bool isVeg) 
{
    isVegetarian = isVeg;
}


int FoodItem::getPreparationTime() const
{
     return cookingTime;
}