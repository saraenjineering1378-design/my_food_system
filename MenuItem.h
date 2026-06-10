#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>

#include "Enums.h"

class MenuItem {
private:
    int id;
    std::string name;
    std::string description;
    double basePrice;
    bool isAvailable;
    ItemType type;

public:
    
    MenuItem(ItemType t, int id, const std::string& name, const std::string& description, double basePrice, bool isAvailable);

    // (Getters)
    int getId() const;
    std::string getName() const;
    std::string getDescription() const;
    double getBasePrice() const;
    bool getIsAvailable() const;
    ItemType getType() const;

    // (Setters)
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setBasePrice(double price);
    void setIsAvailable(bool status);

    
    virtual void display() const;

    //tabdil enum va adad b yekdigar
    static int typeToInt(ItemType type);
    static ItemType intToType(int val);
};

#endif 

