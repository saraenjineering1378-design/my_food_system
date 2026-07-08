#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <vector>
#include "MenuItem.h"

enum OrderStatus { Pending, Preparing, Delivered, Completed, Cancelled };

//biron class hast va baes mishavad erroro chap enum hal beshe
inline std::ostream& operator<<(std::ostream& os, const OrderStatus& status) 
{
    switch (status) {
        case OrderStatus::Pending:   return os << "Pending";
        case OrderStatus::Preparing: return os << "Preparing";
        case OrderStatus::Delivered: return os << "Delivered";
        case OrderStatus::Completed: return os << "Completed";
        case OrderStatus::Cancelled: return os << "Cancelled";
        default: return os << "Unknown";
    }
}

class Order 
{
private:
    int orderId;
    int customerId;
    int restaurantId;
    std::vector<MenuItem*> items;
    OrderStatus status;
    double totalPrice ;

public:
    Order(int orderId, int customerId, int restaurantId);
    ~Order();

    // Getter ha
    int getOrderId() const;
    int getCustomerId() const;
    int getRestaurantId() const;
    OrderStatus getStatus() const;
    double getTotalPrice() const;

    void addItem(MenuItem* item);
    void removeItem(int itemId);
    void updateStatus(OrderStatus newStatus);
    double calculateTotalPrice();
    void displayOrderDetails() const;

    std::string statusToString() const;
    const std::vector<MenuItem*>& getItems() const;
    void setStatus(OrderStatus newStatus);
    void setTotalPrice(double price);
};

#endif

