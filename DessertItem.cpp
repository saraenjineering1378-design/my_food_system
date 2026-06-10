#include <iostream>
#include "DessertItem.h"

using namespace std;

DessertItem::DessertItem(ItemType t,int id, const std::string& name, const std::string& description,
                double basePrice, bool isAvailable, double sugarLevel)

: MenuItem(ItemType::DESSERT, id, name, description, basePrice, isAvailable),
  sugarLevel(sugarLevel) {}

void DessertItem::display() const
{
    cout << "----- Dessert Item -----\n";
    MenuItem::display();
    cout <<"sugarLevel :" << sugarLevel << endl;
    cout << "------------------------\n";
}