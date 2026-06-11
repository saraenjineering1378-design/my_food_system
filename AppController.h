#ifndef APP_CONTROLLER_H
#define APP_CONTROLLER_H

#include "DatabaseManager.h"
#include "SQLiteOrderDAO.h"
#include "IRestaurantDAO.h"
#include "ICustomerDAO.h"
#include "IMenuItemDAO.h"
#include "IOrderDAO.h"
#include "Customer.h"
#include "Restaurant.h"
#include "MenuItem.h"
#include "Order.h"
#include <vector>
#include "Enums.h"

#include "SQLiteRestaurantDAO.h"
#include "SQLiteCustomerDAO.h"
#include "SQLiteMenuItemDAO.h"
#include "SQLiteOrderDAO.h"

class AppController 
{
private:
    DatabaseManager& dbManager;
    IRestaurantDAO* restaurantDAO;
    ICustomerDAO* customerDAO;
    IMenuItemDAO* menuItemDAO;
    IOrderDAO* orderDAO;

    std::vector<Restaurant*> restaurants;
    Customer* currentCustomer; 
    int nextOrderId = 1;

    
    void showAdminMenu();
    void removeMenuItemById(int id);
    bool customerSignup();
    bool customerLogin();

    bool isValidName(const std::string& name) const; 

public:
    AppController(DatabaseManager& db);
    ~AppController();

    void run(); 
    void showCustomerMenu();

    void addCustomer(Customer* c);
    Customer* findCustomerById(int id);
    void addMenuItemToRestaurant(int restId, MenuItem* item);
    void addOrder(Order* o);
    std::vector<Customer*> getAllCustomers();
    std::vector<Restaurant*> getAllRestaurants();
    std::vector<MenuItem*> getAllMenuItemsForRestaurant(int restId);
    void addRestaurant(Restaurant* r);

    
    // 👨‍🍳 RESTAURANT MANAGER PANEL 
    
    void managerLoginMenu();
    void runRestaurantManagerPanel(Restaurant* restaurant);
    void editCurrentRestaurantInfo(Restaurant* restaurant);
    void manageRestaurantMenu(Restaurant* restaurant); 
    void viewRestaurantOrders(Restaurant* restaurant);
    void editRestaurantInfo(Restaurant* restaurant);
    
    
    void managerAddFoodItem(Restaurant* restaurant);
    void managerRemoveItem(Restaurant* restaurant); 
    void managerUpdateItem(Restaurant* restaurant);

    
    // 👑 SYSTEM ADMINISTRATOR PANEL
   
    void runSystemAdminPanel();
    void addNewRestaurant();
    void manageRestaurantActivation();
    void viewSystemReports();
    void selectAndEditRestaurant();

    
    // 🍔 CUSTOMER PORTAL LOGIN
    void customerLoginMenu();

    void viewOrderHistory();

};

#endif 