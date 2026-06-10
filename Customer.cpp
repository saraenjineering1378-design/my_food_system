#include "Customer.h"
#include <iostream>
#include "MenuItem.h"
#include "SQLiteOrderDAO.h"



Customer::Customer(DatabaseManager* db, int CustomerId, std::string name, double Wallet, std::string password)
    : CustomerID(CustomerId), name(name), Wallet(Wallet), password(password)
{
}


Customer::~Customer()
{

/* for (size_t i = 0; i < OrderHistory.size(); ++i) 
    {
        delete OrderHistory[i]; //azad kardan hafeze har sefaresh
    }
    *///chon delete ro be memoryorderdao sepordim
    OrderHistory.clear();
}



int Customer::getCustomerId() const 
{
     return CustomerID; 
}
std::string Customer::getName() const 
{ 
    return name;
}
double Customer::getWallet() const 
{ 
    return Wallet; 
}


void Customer::setName(std::string Name) 
{ 
    name = Name; 
}

void Customer::setCustomerId(int id)
{
    CustomerID = id;
}



void Customer::addFunds(double amount)
{
    if (amount > 0) // poli ke mikhay berizi bayad + bashad
    {
        Wallet += amount;
        std::cout << "Account charged successfully. New balance: " << Wallet << std::endl;
    }
}


bool Customer::payForOrder(double amount)
{
    if (Wallet >= amount) {
        Wallet -= amount;
        std::cout << "Payment successful! Remaining balance: " << Wallet << std::endl;
        return true;
    } 
    else 
    {
        std::cout << "Insufficient funds! Please charge your wallet." << std::endl;
        return false;
    }
}


void Customer::addOrderToHistory(Order* order)
{
    OrderHistory.push_back(order);
}


// +item be sabad kharid
void Customer::addToCart(MenuItem* item) {
    if (item != nullptr) {
        cart.push_back(item);
    }
}

//mohasebe majmo factor
double Customer::getTotal() const {
    double total = 0.0;
    // tak tak item hay sabad jam mizanim
    for (MenuItem* item : cart) {
        if (item != nullptr) {
            total += item->getBasePrice();
        }
    }
    return total;
}

//khali kardan sabad khaid
void Customer::clearCart() 
{
    cart.clear(); 
   
}
void Customer::removeFromCart(int foodId) 
{
    for (auto it = cart.begin(); it != cart.end(); ++it) {
        if ((*it)->getId() == foodId) {
            cart.erase(it);
            return; //  baes mishe dar har bar faghat yedone az ghaza hazf beshe
        }
    }
}
std::vector<MenuItem*> Customer::getCart() const
{
    return cart;
}