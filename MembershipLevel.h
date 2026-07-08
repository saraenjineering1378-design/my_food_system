#ifndef MEMBERSHIP_LEVEL_H
#define MEMBERSHIP_LEVEL_H

#include <string>

// class haye entezaei
class IMembershipLevel 
{
public:
    virtual ~IMembershipLevel() {}
    
    virtual std::string getLevelName() const = 0;
    virtual int getRequiredPoints() const = 0;
    virtual double getMultiplier() const = 0;       // zarib emtiyaz
    virtual double getDiscountPercent() const = 0;  // darsad takhfif (0.1 = 10%)
    virtual double getShippingCost(double baseCost) const = 0; // hazine ersal
};

// sath Normal
class NormalLevel : public IMembershipLevel 
{
public:
    std::string getLevelName() const override;
    int getRequiredPoints() const override;
    double getMultiplier() const override;
    double getDiscountPercent() const override;
    double getShippingCost(double baseCost) const override;
};

// sath Silver
class SilverLevel : public IMembershipLevel 
{
public:
    std::string getLevelName() const override;
    int getRequiredPoints() const override;
    double getMultiplier() const override;
    double getDiscountPercent() const override;
    double getShippingCost(double baseCost) const override;
};

// sath Gold
class GoldLevel : public IMembershipLevel 
{
public:
    std::string getLevelName() const override;
    int getRequiredPoints() const override;
    double getMultiplier() const override;
    double getDiscountPercent() const override;
    double getShippingCost(double baseCost) const override;
};

// sath VIP
class VIPLevel : public IMembershipLevel 
{
public:
    std::string getLevelName() const override;
    int getRequiredPoints() const override;
    double getMultiplier() const override;
    double getDiscountPercent() const override;
    double getShippingCost(double baseCost) const override;
};

// class platinum
/*class PlatinumLevel : public IMembershipLevel 
{
public:
    std::string getLevelName() const override;
    int getRequiredPoints() const override;
    double getMultiplier() const override;
    double getDiscountPercent() const override;
    double getShippingCost(double baseCost) const override;
    
};*/


// tabe komaki baray sakht sath az roye nam
IMembershipLevel* createLevelFromName(const std::string& levelName);

#endif