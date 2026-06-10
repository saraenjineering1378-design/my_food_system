#include "Restaurant.h"
#include<algorithm> //baray remove if
#include <iostream>
#include <vector>
#include <string>

#include "IMenuItemDAO.h"
#include "IOrderDAO.h"


using namespace std;

Restaurant:: Restaurant(int id, const string& name, const string& address,
           int estimatedPrepTime, const string& phone, const string& description,
           bool status, IMenuItemDAO* mDAO, IOrderDAO* oDAO, std::string password)
    : id(id), name(name), address(address), estimatedPrepTime(estimatedPrepTime),
      phoneNumber(phone), description(description), isActive(status),
      menuDAO(mDAO), orderDAO(oDAO), password(password){}

    Restaurant::~Restaurant()
{
    menuDAO = nullptr;//chon pak kardan be ohde appcontroller hast
    orderDAO = nullptr;
}


    int Restaurant::getId() const
    {
        return id;
    }
    std::string Restaurant::getName() const
    {
        return name;
    }
    std::string Restaurant::getAddress() const
    {
        return address;
    }

    int Restaurant::getEstimatedPrepTime() const
    {
        return estimatedPrepTime;
    }
    std::string Restaurant::getPhoneNumber() const
    {
        return phoneNumber;
    }
    std::string Restaurant::getDescription() const
    { 
        return description; 
    }



    void Restaurant::setName(const string& name)
    {
        this->name = name;
    }
    void Restaurant::setAddress(const string& address)
    {
        this->address = address;
    }
    
    void Restaurant::setEstimatedPrepTime(int time)
    {
        this->estimatedPrepTime = time;
    }
    void Restaurant::setPhoneNumber(const string& phone)
    {
        this->phoneNumber = phone;
    }
    void Restaurant::setDescription(const string& description)
    {
        this->description = description; 
    }

    void Restaurant::setId(int id)//ezafe kardan id
    {
        this->id = id;
    }


    void Restaurant::removeMenuItem(int itemId)
    {
        menuDAO->removeMenuItem(itemId);
    }
    MenuItem* Restaurant::findMenuItem(int itemId) const
    {
    
    for (size_t i = 0; i < menu.size(); i++)
    {
        // age pointeri poch bod barname error nadahad
        if (menu[i] != nullptr && menu[i]->getId() == itemId)
        {
            return menu[i]; //item peyda shod
        }
    }
    
    return nullptr; // age hamchin shenase nabod poch bar migardone ta barname edame peyda kone
}
    void Restaurant::displayRestaurantInfo() const {
    cout << "\n======= Restaurant Details =======" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Status: " << (isActive ? "Open " : "Closed ") << endl;
    cout << "Address: " << address << endl;
    cout << "Phone: " << phoneNumber << endl;
    cout << "Prep Time: " << estimatedPrepTime << " mins" << endl;
    cout << "Description: " << description << endl;
    cout << "==================================" << endl;
}


void Restaurant::displayMenu() const 
{
    cout << "\n--- Current Menu for " << name << " ---" << endl;
    
    
    if (menu.empty()) 
    {
        cout << "Menu is currently empty. Chef is on vacation!" << endl;
    }
    else 
    {
        for (size_t i = 0; i < menu.size(); i++)
        {
           //age item motabar bod chapesh kon
            if (menu[i] != nullptr) 
            {
                menu[i]->display(); 
            }
        }
    }
}

Order* Restaurant::findOrderId(int orderId) const
{
    
    return orderDAO->findOrderById(orderId);
}


bool Restaurant::getIsActive() const 
{
     return isActive; 
}
void Restaurant::setIsActive(bool status) 
{ 
    isActive = status; 
}
void Restaurant::deactivate() 
{
    isActive = false; 
}
void Restaurant::addMenuItem(int restaurantId, MenuItem* item) 
{
    if (!item) return;

    
    menu.push_back(item);//ezafe kardan b list mahali (hafeze movaghat)

    if (menuDAO) {
        menuDAO->addMenuItem(this->id, item);
    }
}

void Restaurant::setPassword(const std::string& pass) 
{
    this->password = pass; 
}

std::string Restaurant::getPassword() const 
{
    return this->password; 
}

