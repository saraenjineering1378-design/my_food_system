#include "Customer.h"
#include <iostream>
#include "MenuItem.h"
#include "SQLiteOrderDAO.h"
#include "MembershipLevel.h"

// sazande ha

Customer::Customer(int CustomerId, std::string name, double Wallet, std::string password, int monthlyCoupons)
    : CustomerID(CustomerId), name(name), Wallet(Wallet), password(password),
      loyaltyPoints(0), currentLevel(new NormalLevel()),
      monthlyCoupons(monthlyCoupons), badge("None"),
      upgradeMessageShown(0)
{
}

Customer::Customer(const Customer& other)
    : CustomerID(other.CustomerID), name(other.name), Wallet(other.Wallet), 
      password(other.password), loyaltyPoints(other.loyaltyPoints), 
      monthlyCoupons(other.monthlyCoupons), badge(other.badge),
      upgradeMessageShown(other.upgradeMessageShown)
{
    if (other.currentLevel) 
    {
        currentLevel = createLevelFromName(other.currentLevel->getLevelName());
    } else 
    {
        currentLevel = new NormalLevel();
    }
}

Customer::~Customer()
{
    delete currentLevel;
    OrderHistory.clear();
}

//getter ha

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
std::string Customer::getPassword() const 
{ 
    return password; 
}

// setter ha

void Customer::setName(std::string Name) 
{ 
    name = Name; 
}
void Customer::setCustomerId(int id) 
{ 
    CustomerID = id; 
}
void Customer::setPassword(const std::string& pass) 
{ 
    password = pass; 
}
void Customer::setWallet(double newBalance) 
{ 
    Wallet = newBalance; 
}

// method haye kif pol

void Customer::addFunds(double amount)
{
    if (amount > 0)
    {
        Wallet += amount;
        std::cout << "Account charged successfully. New balance: " << Wallet << std::endl;
    }
}

bool Customer::payForOrder(double amount)
{
    if (Wallet >= amount) 
    {
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

// method haye sefaresh
void Customer::addOrderToHistory(Order* order)
{
    OrderHistory.push_back(order);
}

void Customer::addToCart(MenuItem* item) 
{
    if (item != nullptr) 
    {
        cart.push_back(item);
    }
}

double Customer::getTotal() const 
{
    double total = 0.0;
    for (MenuItem* item : cart) 
    {
        if (item != nullptr) 
        {
            total += item->getBasePrice();
        }
    }
    return total;
}

void Customer::clearCart() 
{ 
    cart.clear(); 
}

void Customer::removeFromCart(int foodId) 
{
    for (auto it = cart.begin(); it != cart.end(); ++it) 
    {
        if ((*it)->getId() == foodId) 
        {
            cart.erase(it);
            return;
        }
    }
}

std::vector<MenuItem*> Customer::getCart() const
{
    return cart;
}

void Customer::clearOrderHistory()
{
    OrderHistory.clear();
}

// method haye vafadari

int Customer::getLoyaltyPoints() const 
{ 
    return loyaltyPoints; 
}

void Customer::setLoyaltyPoints(int points) 
{
    loyaltyPoints = points;
}

std::string Customer::getLevelName() const 
{
    if (currentLevel) 
    {
        return currentLevel->getLevelName();
    }
    return "Normal";
}

IMembershipLevel* Customer::getLevel() const 
{ 
    return currentLevel; 
}

void Customer::setLevel(IMembershipLevel* newLevel) 
{
    if (currentLevel) delete currentLevel;
    currentLevel = newLevel;
}

void Customer::addPoints(int points) 
{
    loyaltyPoints += points;
    checkAndUpgrade();
}

void Customer::checkAndUpgrade() 
{
    if (!currentLevel) return;
    
    std::string currentName = currentLevel->getLevelName();
    int points = loyaltyPoints;
    
    IMembershipLevel* newLevel = nullptr;
   /* if (points >= 1000) 
    {
       newLevel = new PlatinumLevel();
    } 
    else */if (points >= 700) {
        newLevel = new VIPLevel();
    } else if (points >= 300) {
        newLevel = new GoldLevel();
    } else if (points >= 100) {
        newLevel = new SilverLevel();
    } else {
        newLevel = new NormalLevel();
    }
    
    std::string newName = newLevel->getLevelName();
    if (newName != currentName) 
    {
        delete currentLevel;
        currentLevel = newLevel;
        std::cout << "🎉 Congratulations! You've been upgraded to " << currentLevel->getLevelName() << " level!" << std::endl;
    } else {
        delete newLevel;
    }
}

void Customer::downgradeLevel(const std::string& reason) 
{
    if (!currentLevel) return;
    
    std::string currentName = currentLevel->getLevelName();
    IMembershipLevel* newLevel = nullptr;
    
    /*if (currentName == "Platinum") 
    {
        newLevel = new VIPLevel();
    } else */if (currentName == "VIP") 
    {
        newLevel = new GoldLevel();
    } else if (currentName == "Gold") 
    {
        newLevel = new SilverLevel();
    } else if (currentName == "Silver") 
    {
        newLevel = new NormalLevel();
    } else {
        std::cout << "❌ Normal level cannot be downgraded further." << std::endl;
        return;
    }
    
    std::string newName = newLevel->getLevelName();
    if (newName != currentName) 
    {
        delete currentLevel;
        currentLevel = newLevel;
        std::cout << "⛔ Your level has been downgraded to " << currentLevel->getLevelName() 
                  << " (reason: " << reason << ")" << std::endl;
    } else 
    {
        delete newLevel;
    }
}

double Customer::applyDiscount(double total) const 
{
    if (!currentLevel) return total;
    double discount = currentLevel->getDiscountPercent();
    return total * (1 - discount);
}

double Customer::calculateShipping(double baseCost) const 
{
    if (!currentLevel) return baseCost;
    return currentLevel->getShippingCost(baseCost);
}

int Customer::getPointsForNextLevel() const 
{
    if (!currentLevel) return 0;
    
    std::string levelName = currentLevel->getLevelName();
    if (levelName == "Normal") return 100;
    if (levelName == "Silver") return 300;
    if (levelName == "Gold") return 700;
    if (levelName == "VIP") return 0; //*baray platinum 1000
    return 0;
}

// method haye copoun

int Customer::getMonthlyCoupons() const 
{ 
    return monthlyCoupons; 
}

void Customer::setMonthlyCoupons(int count) 
{
    monthlyCoupons = count;
}

void Customer::useCoupon() 
{
    if (monthlyCoupons > 0) 
    {
        monthlyCoupons--;
    }
}

bool Customer::hasAvailableCoupon() const 
{
    return monthlyCoupons > 0;
}

// method haye neshan

std::string Customer::getBadge() const 
{ 
    return badge; 
}

void Customer::setBadge(const std::string& newBadge) 
{
    badge = newBadge;
}

std::string Customer::getDisplayBadge() const 
{
    std::string result = "";
    
    if (badge.find("Frequent Buyer") != std::string::npos) 
    {
        result = "Frequent Buyer";
    }
    if (badge.find("Night Customer") != std::string::npos) 
    {
        if (!result.empty()) result += ", ";
        result += "Night Customer";
    }
    
    return result.empty() ? "None" : result;
}

int Customer::getUpgradeMessageShown() const
{
    return upgradeMessageShown;
}
void  Customer::setUpgradeMessageShown(int shown)
{
    upgradeMessageShown = shown;
}