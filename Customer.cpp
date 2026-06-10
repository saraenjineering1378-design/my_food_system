#include "Customer.h"
#include <iostream>
#include "MenuItem.h"
#include "IOrderDAO.h"



Customer::Customer(int CustomerId, std::string name, double Wallet, std::string password)
    : CustomerID(CustomerId), name(name), Wallet(Wallet), password(password)
{
}


Customer::~Customer()
{
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

void Customer::setWallet(double newBalance) 
{
    this-> Wallet = newBalance;
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



void Customer::displayOrderHistory() {
     //pak sazi list ghabli baray jologiri az tekrar dade ha 
    for (auto order : OrderHistory) {
        delete order; 
    }
    OrderHistory.clear();

    // daryaft list jadid az database
    OrderHistory = IOrderDAO->getAllOrders(); 
    
    for (size_t i = 0; i < OrderHistory.size(); ++i) {
        std::cout << "Order " << i + 1 << ":" << std::endl;
        OrderHistory[i]->displayOrderDetails(); 
        
        OrderStatus s = OrderHistory[i]->getStatus();
        if (s == OrderStatus::Preparing) {
            std::cout << ">> 👨‍🍳 Restaurant is preparing your order." << std::endl;
        } else if (s == OrderStatus::Delivered) {
            std::cout << ">> 🚚 Your order has been delivered! Enjoy." << std::endl;
        } else if (s == OrderStatus::Cancelled) {
            std::cout << ">> ❌ Sorry, your order was cancelled." << std::endl;
        }
        std::cout << "--------------------" << std::endl;
    }
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



void Customer::addToCart(MenuItem* item) {
    if (item != nullptr) {
        cart.push_back(item);
    }
}

double Customer::getTotal() const {
    double total = 0.0;
    for (MenuItem* item : cart) {
        if (item != nullptr) {
            total += item->getBasePrice();
        }
    }
    return total;
}

void Customer::clearCart() {
    cart.clear();
}

void Customer::clearOrderHistory()
{
    OrderHistory.clear();
}
