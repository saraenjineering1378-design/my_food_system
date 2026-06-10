#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <string>
#include <vector>
#include "MenuItem.h"
#include "Order.h" 

#include "IMenuItemDAO.h"
#include "IOrderDAO.h"


class Restaurant
{
private:

    int id;
    std::string name;
    std::string address;
    bool isActive;
    int estimatedPrepTime;
    std::string phoneNumber;
    std::string password;
    std::string description;
    std::vector<MenuItem*> menu;

    //std::string category;


    IMenuItemDAO* menuDAO;//chon ke dige restaurant dargir dade ha nist va DAOinkaro mikone
    IOrderDAO* orderDAO; 


public:

    Restaurant(int id,const std::string& name, const std::string& address,int estimatedPrepTime,
           const std::string& phone, const std::string& description, bool status = true,  
           IMenuItemDAO* mDAO = nullptr, IOrderDAO* oDAO = nullptr, std::string password = "1234");
    

    ~Restaurant();


        //getter ha
    int getId() const;
    std::string getName() const;
    std::string getAddress() const;
    //bool getStatus () const;
    int getEstimatedPrepTime() const;
    std::string getPhoneNumber() const;
    std::string getDescription() const;
    bool getIsActive() const;
    std::string getPassword() const;


    void deactivate();


        //setterha
    void setName(const std::string& name);
    void setId(int id);//ezafe kardan id 
    void setAddress(const std::string& address);
    //void setStatus(bool status);
    void setEstimatedPrepTime(int time);
    void setPhoneNumber(const std::string& phone);
    void setDescription(const std::string& description);
    //void setCategory(const std::string& category);
    void setIsActive(bool status);
    void setPassword(const std::string& pass);

        //baray modiriyat menu
    void addMenuItem(int restaurantId,MenuItem* item);
    void removeMenuItem(int itemId);
    MenuItem* findMenuItem(int itemId) const;
    
        //namayesh etelaalat baray bakhsh consoli proje
    void displayRestaurantInfo() const;
    void displayMenu() const;

    void addOrder(Order* order);
    void displayReceivedOrders() const;

    Order* findOrderId(int orderId) const;//metod komaki baray baray dastresi bsefaresh ha baray taghir dadan vaziyat
    

    
};
#endif
