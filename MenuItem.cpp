#include "MenuItem.h"
#include <iostream>

#include "MenuItem.h"


using namespace std;

MenuItem::MenuItem(ItemType t, int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable)

: id(id), name(name), description(description), basePrice(basePrice), isAvailable(isAvailable),
type(t) {}

//getterha

int MenuItem::getId() const
{
    return id;
}
std::string MenuItem::getName() const
{
    return name;
}
std::string MenuItem::getDescription() const
{
    return description;
}
double MenuItem::getBasePrice() const
{
    return basePrice;
}
bool MenuItem::getIsAvailable() const
{
    return isAvailable;
}

ItemType MenuItem::getType() const
{
    return type;
}


//setterha
void MenuItem::setId(int id) 
{
    this->id = id; 
}

void MenuItem::setName(const std::string& name)
{
    this->name = name;
}
void MenuItem::setDescription(const std::string& description)
{
    this->description = description;
}
void MenuItem::setBasePrice(double price)
{
    this->basePrice = price;
}
void MenuItem::setIsAvailable(bool status)
{
    this->isAvailable = status;
}

void MenuItem::display() const  //dar class pedar darim va mitavanand farzand ha az an estefade konand mitavanand override konand
{
    cout << "ID: " << id << std::endl;
    cout << "Name: " << name << std::endl;
    cout << "Description: " << description << std::endl;
    cout << "Base Price: " << basePrice << std::endl;
    cout << "Available: " << (isAvailable ? "Yes" : "No") << std::endl;
}

int MenuItem::typeToInt(ItemType type) // adad kardan food drink dessert
{
    return static_cast<int>(type);
}

ItemType MenuItem::intToType(int val) // tabdil adad b food dessert drink 
{
    return static_cast<ItemType>(val);
}

