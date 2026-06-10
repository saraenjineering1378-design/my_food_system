#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>
#include <string>
#include  "Order.h"
#include "MenuItem.h"

class Customer
{
private:

    std::string password;
    int CustomerID;
    std::string name;   
    double Wallet;
    std::vector<Order*> OrderHistory;
    std::vector<MenuItem*> cart;//sabad kharid moshtari

public:

    Customer(int CustomerId, std::string name, double Wallet, std::string password = "1234");

    ~Customer();

    //getterha
    int getCustomerId() const;
    std::string getName() const;
    double getWallet() const;
    std::string getPassword() const { return password; }



    void setName(std::string Name); // chon b motghayer private, mostaghim dastresi nadarim baray taviz nam az set kardan mirim
    void setCustomerId(int id);
    void setPassword(const std::string& pass) { password = pass; }
    void setWallet(double newBalance);

    
    //baray moshtariha
    void addFunds(double amount);   //sharj hesab        
    bool payForOrder(double amount); //pardakht sefaresh
    void addOrderToHistory(Order* order);  //ezafe kardan be history 
    void displayOrderHistory();     //namayesh list sefareshat

    void addToCart(MenuItem* item);//ghesmat sabad kharid
    double getTotal() const;
    void clearCart();
    std::vector<MenuItem*> getCart() const; 

    void removeFromCart(int foodId);
    void clearOrderHistory();
    
};
#endif