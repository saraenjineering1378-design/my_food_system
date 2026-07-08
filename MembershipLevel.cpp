#include "MembershipLevel.h"

// ========== NormalLevel ==========

std::string NormalLevel::getLevelName() const {
    return "Normal";
}

int NormalLevel::getRequiredPoints() const {
    return 0;
}

double NormalLevel::getMultiplier() const {
    return 1.0;
}

double NormalLevel::getDiscountPercent() const {
    return 0.0;
}

double NormalLevel::getShippingCost(double baseCost) const {
    return baseCost;
}

// ========== SilverLevel ==========

std::string SilverLevel::getLevelName() const {
    return "Silver";
}

int SilverLevel::getRequiredPoints() const {
    return 100;
}

double SilverLevel::getMultiplier() const {
    return 1.2;
}

double SilverLevel::getDiscountPercent() const {
    return 0.05;
}

double SilverLevel::getShippingCost(double baseCost) const {
    return baseCost;
}

// ========== GoldLevel ==========

std::string GoldLevel::getLevelName() const {
    return "Gold";
}

int GoldLevel::getRequiredPoints() const {
    return 300;
}

double GoldLevel::getMultiplier() const {
    return 1.5;
}

double GoldLevel::getDiscountPercent() const {
    return 0.10;
}

double GoldLevel::getShippingCost(double baseCost) const {
    return baseCost * 0.5;
}

// ========== VIPLevel ==========

std::string VIPLevel::getLevelName() const {
    return "VIP";
}

int VIPLevel::getRequiredPoints() const {
    return 700;
}

double VIPLevel::getMultiplier() const {
    return 2.0;
}

double VIPLevel::getDiscountPercent() const {
    return 0.15;
}

double VIPLevel::getShippingCost(double baseCost) const {
    return 0.0;
}

/* PlatinumLevel

std::string PlatinumLevel::getLevelName() const {
    return "Platinum";
}

int PlatinumLevel::getRequiredPoints() const {
    return 1000;
}

double PlatinumLevel::getMultiplier() const {
    return 2.5;
}

double PlatinumLevel::getDiscountPercent() const {
    return 0.20;
}

double PlatinumLevel::getShippingCost(double baseCost) const {
    return 0.0;
}*/

// ========== تابع کمکی ==========

IMembershipLevel* createLevelFromName(const std::string& levelName) {
    if (levelName == "Silver") return new SilverLevel();
    if (levelName == "Gold") return new GoldLevel();
    if (levelName == "VIP") return new VIPLevel();
    //*if (levelName == "Platinum") return new PlatinumLevel();
    return new NormalLevel(); // پیش‌فرض
}