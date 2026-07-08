#include "Order.h"
#include <iostream>
#include <algorithm>

using namespace std;

Order::Order(int orderId, int customerId, int restaurantId)
    : orderId(orderId),customerId(customerId),restaurantId(restaurantId),
      status(OrderStatus::Pending),totalPrice(0.0)
{
}

Order::~Order()
{
    items.clear();
}


int Order::getOrderId() const
{
    return orderId;
}

int Order::getCustomerId() const
{
    return customerId;
}

int Order::getRestaurantId() const
{
    return restaurantId;
}

OrderStatus Order::getStatus() const
{
    return status;
}

double Order::getTotalPrice() const
{
    return totalPrice;
}


void Order::addItem(MenuItem* item)
{
    items.push_back(item);
    calculateTotalPrice();
}

void Order::removeItem(int itemId)
{
    items.erase(
        std::remove_if(items.begin(), items.end(),
            [itemId](MenuItem* item)
            {
                return item->getId() == itemId;
            }),
        items.end()
    );

    calculateTotalPrice();
}

void Order::updateStatus(OrderStatus newStatus)
{
    status = newStatus;
}

double Order::calculateTotalPrice() 
{
    totalPrice = 0;

    for (int i = 0; i < items.size(); i++)
    {
        totalPrice += items[i]->getBasePrice();
    }
     return totalPrice;
}

void Order::displayOrderDetails() const
{
    cout << "Order ID: " << orderId << endl;
    cout << "Customer ID: " << customerId << endl;
    cout << "Status: " << statusToString() << endl;

    cout << "\nItems:\n";

    for (int i = 0; i < items.size(); i++)
    {
        items[i]->display();
    }

    cout << "\nTotal Price: " << totalPrice << endl;
}

std::string Order::statusToString() const
{
    switch (status)
    {
        case OrderStatus::Pending:   return "Pending";
        case OrderStatus::Preparing: return "Preparing";
        case OrderStatus::Delivered: return "Delivered";
        case OrderStatus::Completed: return "Completed";   
        case OrderStatus::Cancelled: return "Cancelled";
        default: return "Unknown";
    }
}
const std::vector<MenuItem*>& Order::getItems() const 
{
    return items;
}
void Order::setStatus(OrderStatus newStatus)
{
    status = newStatus;
}

void Order::setTotalPrice(double price)

{
    totalPrice = price;
}