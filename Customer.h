#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>
#include <string>
#include  "Order.h"
#include "MenuItem.h"
#include "DatabaseManager.h"
#include "MembershipLevel.h"

class Customer
{
private:
    DatabaseManager* dbManager; 
    std::string password;
    int CustomerID;
    std::string name;   
    double Wallet;
    std::vector<Order*> OrderHistory;
    std::vector<MenuItem*> cart;//sabad kharid moshtariint 
    int monthlyCoupons;  // tedad copoun haye mahane
    std::string badge;  //neshan karbar



    //field hay jadid vafadari
    int loyaltyPoints;
    IMembershipLevel* currentLevel; // esharegar be sath feli
 
public:
    Customer(int CustomerId, std::string name, double Wallet, std::string password = "1234", int monthlyCoupons = 0);
    Customer(const Customer& other);
    
    ~Customer();

    //getterha
    int getCustomerId() const;
    std::string getName() const;
    double getWallet() const;
    std::string getPassword() const; 



    void setName(std::string Name); // chon b motghayer private, mostaghim dastresi nadarim baray taviz nam az set kardan mirim
    void setCustomerId(int id);
    void setPassword(const std::string& pass); 
    void setWallet(double newBalance);

    
    //baray moshtariha
    void addFunds(double amount);   //sharj hesab        
    bool payForOrder(double amount); //pardakht sefaresh
    void addOrderToHistory(Order* order);  //ezafe kardan be history 

    void addToCart(MenuItem* item);//ghesmat sabad kharid
    double getTotal() const;
    void clearCart();
    std::vector<MenuItem*> getCart() const; 

    void removeFromCart(int foodId);
    void clearOrderHistory();



 
    // method haye vafadari
    int getLoyaltyPoints() const;
    void setLoyaltyPoints(int points);
    std::string getLevelName() const;
    IMembershipLevel* getLevel() const;
    void setLevel(IMembershipLevel* newLevel);
    void addPoints(int points);
    void checkAndUpgrade();
    void downgradeLevel(const std::string& reason);
    double applyDiscount(double total) const;
    double calculateShipping(double baseCost) const;
    int getPointsForNextLevel() const;

    // method haye copoun
    int getMonthlyCoupons() const;
    void setMonthlyCoupons(int count);
    void useCoupon();
    bool hasAvailableCoupon() const;

    // method haye nashan
    std::string getBadge() const;
    void setBadge(const std::string& newBadge);
    std::string getDisplayBadge() const;
    
};
#endif