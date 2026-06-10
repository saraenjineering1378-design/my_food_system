#include <iostream>
#include "DrinkItem.h"

using namespace std;

DrinkItem::DrinkItem(ItemType t,int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable,
        double volume, bool isCold)

: MenuItem(ItemType::DRINK, id, name, description, basePrice, isAvailable),
volume(volume), isCold(isCold){}

void DrinkItem::display() const 
{
    cout << "----- DrinkItem -----\n";
    MenuItem::display();
    cout << "Volume: " << volume << " ml\n";
    cout << "Cold: " << (isCold ? "Yes" : "No") << "\n";
    cout << "----------------------\n";
}

