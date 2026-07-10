#include "AppController.h"
#include <iostream>
#include "DatabaseInitializer.h"
#include <limits>
#include "Customer.h"
#include "FoodItem.h"
#include "DrinkItem.h"
#include "DessertItem.h"
#include "Enums.h"
#include "MenuItem.h"
#include <ctime>



AppController::AppController(DatabaseManager& db) : dbManager(db), currentCustomer(nullptr) 
{
    // age jadval ha vojod nadaran misazim
    DatabaseInitializer::initialize(dbManager);

    // jay gozin kardan sqlite b jay memory
    restaurantDAO = new SQLiteRestaurantDAO(dbManager);
    customerDAO = new SQLiteCustomerDAO(dbManager);
    menuItemDAO = new SQLiteMenuItemDAO(dbManager);
    orderDAO = new SQLiteOrderDAO(dbManager);
    
}



void AppController::addRestaurant(Restaurant* r) //ezafe kardan resturan
{
    if (!r) return;
    restaurantDAO->addRestaurant(r);   //ham insert ham setid
    restaurants.push_back(r);          // va bad ham toy list hafeze negah midarim
}

void AppController::run()
{
    int role = 0;
    while (role != 4)
    {
        std::cout << "\n=== Food Ordering System ===\n";
        std::cout << "1. Admin\n2. Restaurant Owner\n3. Customer\n4. Exit\nSelect an option: ";
        std::cin >> role;

        switch (role)
        {
        case 1:
            showAdminMenu();
            break;
        case 2:
            for(size_t i=0; i<restaurants.size(); ++i) std::cout << i+1 << ". " << restaurants[i]->getName() << "\n";
            int idx; std::cin >> idx;
            if(idx > 0 && idx <= (int)restaurants.size()) runRestaurantManagerPanel(restaurants[idx-1]);
            break;
        case 3:
            customerLoginMenu();
            break;
        case 4:
            std::cout << "Exiting system...\n";
            break;
        default:
            std::cout << "Invalid choice!\n";
            break;
        }
    }
}

//Admin Menu 
void AppController::showAdminMenu()
{
    int choice = 0;
    while (choice != 3)
    {
        std::cout << "\n--- Admin Panel ---\n";
        std::cout << "1. Manage Restaurants (Activate/Deactivate)\n2. View General Report\n3. Back\nSelect: ";
        std::cin >> choice;

        if (choice == 1)
            std::cout << "Restaurant status updated.\n";
        else if (choice == 2)
            std::cout << "Report: Total Orders: " << (nextOrderId - 1) << " | Total Sales: Calculating...\n";
    }
}

// Restaurant Owner Menu 
void AppController::manageRestaurantMenu(Restaurant* restaurant) 
{
    if (!restaurant) return;

    int choice;
    while (true) 
    {
        std::cout << "\n==================================" << std::endl;
        std::cout << "   MANAGE MENU: " << restaurant->getName() << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Add Food Item" << std::endl;
        std::cout << "2. Remove Food Item" << std::endl;
        std::cout << "3. Update Food Item" << std::endl;
        std::cout << "4. Back to Manager Panel" << std::endl;
        std::cout << "Selection: ";
        
        if (!(std::cin >> choice)) {
            std::cout << "❌ Invalid choice! Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n');

        if (choice == 4) break;

        switch (choice) 
        {
            case 1: managerAddFoodItem(restaurant); break;
            case 2: managerRemoveItem(restaurant); break;
            case 3: managerUpdateItem(restaurant); break;
            default: std::cout << "❌ Invalid choice! Select between 1 and 4." << std::endl;
        }
    }
}

void AppController::editCurrentRestaurantInfo(Restaurant* restaurant) 
{
    if (!restaurant) return;
    
    while (true) {
        std::cout << "\n==================================" << std::endl;
        std::cout << "     EDIT RESTAURANT INFO         " << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "Current Name: " << restaurant->getName() << std::endl;
        std::cout << "Current Address: " << restaurant->getAddress() << std::endl;
        std::cout << "Current Phone: " << restaurant->getPhoneNumber() << std::endl;
        std::cout << "----------------------------------" << std::endl;
        std::cout << "1. Change Restaurant Name\n";
        std::cout << "2. Change Restaurant Address\n";
        std::cout << "3. Change Restaurant Phone\n";
        std::cout << "4. Back to Manager Panel\n";
        std::cout << "Selection: ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        std::cin.ignore(10000, '\n'); // pak kardan bafer enter
        
        if (choice == 4) break; // bazgasht be menu ghabl

        if (choice == 1) {
            std::string newName;
            std::cout << "Enter new restaurant name: ";
            std::getline(std::cin, newName);
            if (!newName.empty()) {
                restaurant->setName(newName);
                // 🔹zakhire dar database
                if (restaurantDAO) restaurantDAO->updateRestaurant(restaurant);
                std::cout << "✅ Name updated successfully in Database!" << std::endl;
            }
        } 
        else if (choice == 2) 
        {
            std::string newAddress;
            std::cout << "Enter new restaurant address: ";
            std::getline(std::cin, newAddress);
            if (!newAddress.empty()) 
            {
                restaurant->setAddress(newAddress);
                // 🔹zakhire dar database
                if (restaurantDAO) restaurantDAO->updateRestaurant(restaurant);
                std::cout << "✅ Address updated successfully in Database!" << std::endl;
            }
        } 
        else if (choice == 3) 
        {
            std::string newPhone;
            std::cout << "Enter new phone number: ";
            std::getline(std::cin, newPhone);
            if (!newPhone.empty()) 
            {
                restaurant->setPhoneNumber(newPhone);
                // 🔹zakhire dar database
                if (restaurantDAO) restaurantDAO->updateRestaurant(restaurant);
                std::cout << "✅ Phone number updated successfully in Database!" << std::endl;
            }
        } 
        else 
        {
            std::cout << "❌ Invalid choice!" << std::endl;
        }
    }
}





// Customer Menu
void AppController::showCustomerMenu()
{ 
    if (currentCustomer == nullptr) //baray in ke age karbari nabod ghati nakone
    {
        std::cout << "\n Wait a minute! You need to login first to access the Customer Panel!\n";
        return;//khoroj baray jologiri kardan az ghati kardan barname 
    }

    for (auto* res : restaurants) {//model for jadid
        delete res;
    }
    restaurants.clear();

    // 🔹 list taza va update shode ro az database bakesh biron
    if (restaurantDAO) {
        restaurants = restaurantDAO->getAllRestaurants();
    }

    // 🔹 load kardan menu ghaza v noshidani baray har resturan az database
    if (menuItemDAO) { 
        for (auto* res : restaurants) 
        {
            if (res != nullptr) 
            {
                // khandan item hay menu resturan khas az database 
                std::vector<MenuItem*> items = menuItemDAO->getMenuItemsByRestaurant(res->getId());
                
                // enteghal itemha b vector dakheli khod restaurant
                for (auto* item : items) 
                {
                    if (item != nullptr) 
                    {
                        res->addMenuItem(res->getId(), item);
                    }
                }
            }
        }
    }
if (restaurants.empty()) 
{
    std::cout << "Sorry! There are no restaurants available." << std::endl;
    std::cout << "Please ask admin to add a restaurant.\n";
    std::cout << "Press Enter to continue...";
    std::cin.ignore();
    std::cin.get();
    return;
}
    std::cout << "\n--- Select a Restaurant ---\n"; // baray entekhab restaurant morednazar
    for (size_t i = 0; i < restaurants.size(); i++) 
    {
        std::cout << i + 1 << ". " << restaurants[i]->getName() << "\n"
        << " (address " << restaurants[i]->getAddress() << ")" << std::endl;
    }

    std::cout << "0. Back to Main Menu\n";

    int restChoice;
    std::cout << "Select option: ";
    std::cin >> restChoice;

    if (restChoice == 0) return; // bargasht b menu asli
    if (restChoice < 1 || restChoice > restaurants.size()) // etebar sanji entekhab karbar
    {
        std::cout << "Invalid choice!\n";
        return;
    }

    Restaurant* selected= restaurants[restChoice - 1];//resturan entekhab shode ro bardar

    int choice = 0;
while (choice != 5) 
{
    std::cout << "\n--- Customer Panel ---\n";
    std::cout << "Your Current Balance: 🧮$" << currentCustomer->getWallet() << "\n";
    
    // namyesh sath va emtiyza
    std::cout << "Level: " << currentCustomer->getLevelName() 
              << " | Points: " << currentCustomer->getLoyaltyPoints();
    std::string displayBadge = currentCustomer->getDisplayBadge();
    if (displayBadge != "None") 
    {
        std::cout << " | 🏅 " << displayBadge;
}
    
    int nextLevel = currentCustomer->getPointsForNextLevel();
    if (nextLevel > 0) {
        std::cout << " | Next Level at: " << nextLevel << " points";
    } else {
        std::cout << " | 🏆 Max Level!";
    }
    std::cout << "\n";
    std::cout << "1. View Restaurant Menu🛎\n";
    std::cout << "2. Place Order🎰\n";
    std::cout << "3. View Order History🛍\n";
    std::cout << "4. Charge Wallet💵\n"; 
    std::cout << "5. Back to Main Menu\n";
    std::cout << "Select option: ";
    std::cin >> choice;

        switch (choice) 
        {
            case 1:
                if (!selected->getIsActive()) 
                {
                    std::cout << "Sorry! The restaurant is currently closed.\n";
                } else 
                {
                    selected->displayMenu();
                }
                break;
            case 2:
                if (!selected->getIsActive()) 
                {
                    std::cout << "Cannot place order! The restaurant is closed.\n";
                } else 
                {
                    std::cout << "\n--- Ordering Menu ---\n";
                    selected->displayMenu(); 

                    bool ordering = true;
                    int cartChoice = -1;

                    
                    while (ordering) {
                        std::cout << "\n--- Cart Menu ---\n";
                        std::cout << "Live Total: $" << currentCustomer->getTotal() << "\n"; 
                        std::cout << "1. Add Item (by ID)\n";
                        std::cout << "2. Remove Item (by ID)\n";
                        std::cout << "3. Finalize & Checkout\n";
                        std::cout << "0. Cancel Order\n";
                        std::cout << "Choose an action: ";
                        std::cin >> cartChoice;
                        
                        if (cartChoice == 1) 
                        {
                            int foodId;
                            std::cout << "Enter Food ID to ADD: ";
                            std::cin >> foodId;
                            
                            MenuItem* item = selected->findMenuItem(foodId);
                            if (item) 
                            {
                                int qty;
                                std::cout << "How many? ";
                                std::cin >> qty;
                                if (qty > 0) {
                                    for(int i=0; i < qty; ++i) 
                                    {
                                        currentCustomer->addToCart(item);
                                    }
                                    std::cout << " " << qty << " Item(s) added to cart!\n";
                                } else 
                                {
                                    std::cout << "❌ Quantity must be greater than zero!\n";
                                }
                            } else 
                            {
                                std::cout << " ❌ Invalid Food ID!\n";
                            }
                        }
                        else if (cartChoice == 2) 
                        {
                            int foodId;
                            std::cout << "Enter Food ID to REMOVE: ";
                            std::cin >> foodId;
                            currentCustomer->removeFromCart(foodId);
                            std::cout << " Item removed (if existed).\n";
                        }
                        else if (cartChoice == 3) 
                        {
                            ordering = false; 
                        }
                        else if (cartChoice == 0) 
                        {
                            currentCustomer->clearCart(); 
                            std::cout << "Order cancelled!\n";
                            break; 
                        }
                        else 
                        {
                            std::cout << "Invalid choice! Try again.\n";
                        }
                    }

                    if (cartChoice == 0) 
                    {
                        break; 
                    }

                    
                    //bakhsh taeid 2 va sabt sefaresh
                    double baseTotal = currentCustomer->getTotal();
                    if (baseTotal > 0) {
    
                     // mohasebat takhfif 
                    double discountPercent = currentCustomer->getLevel()->getDiscountPercent();
                    double discountAmount = baseTotal * discountPercent;
                    double discountedTotal = baseTotal - discountAmount;
                    double shippingCost = currentCustomer->calculateShipping(50000); // hazine paye ersal
                    double finalTotal = discountedTotal + shippingCost;
                    int earnedPoints = static_cast<int>((baseTotal / 1000) * currentCustomer->getLevel()->getMultiplier());

                    
                    //ezafe kardan copoun
                    bool hasCoupon = currentCustomer->hasAvailableCoupon();
                    double couponDiscount = 0;
                    double couponAmount = 0;


                    if (hasCoupon) 
                    {
                        std::cout << "Do you want to use a coupon? (yes/no): ";
                        std::string useCoupon;
                        std::cin >> useCoupon;
                    if (useCoupon == "yes" || useCoupon == "YES" || useCoupon == "y") 
                    {
                        couponDiscount = 0.10;  // 10% takhfif
                        couponAmount = finalTotal * couponDiscount;
                        finalTotal = finalTotal - couponAmount;
                        currentCustomer->useCoupon();  // kahesh tedad copoun ha
                        std::cout << "✅ Coupon applied! 10% extra discount!\n";
                    }
                    }

    
                    std::cout << "\n=========================================\n";
                    std::cout << "       🧾 FINAL INVOICE (FAKTOR)        \n";
                    std::cout << "=========================================\n";
                    std::cout << "Level: " << currentCustomer->getLevelName() << "\n";
                    std::cout << "Base Price: $" << baseTotal << "\n";
                    if (discountAmount > 0) 
                    {
                        std::cout << "Discount (" << (discountPercent * 100) << "%): -$" << discountAmount << "\n";
                    }
                    std::cout << "Shipping: $" << shippingCost << "\n";
                    if (couponAmount > 0) 
                    {
                        std::cout << "Coupon Discount (10%): -$" << couponAmount << "\n";  
                    }
                    std::cout << "Points Earned: " << earnedPoints << "\n";
                    std::cout << "-----------------------------------------\n";
                    std::cout << "💰 Final Total: $" << finalTotal << "\n";
                    std::cout << "=========================================\n";
               
                        std::string confirm;
                        while (true) 
                        {
                            std::cout << "Do you want to finalize this order? (yes/no): ";
                            std::cin >> confirm;
                            if (confirm == "yes" || confirm == "YES" || confirm == "Yes" || confirm == "y" ||
                                confirm == "no" || confirm == "NO" || confirm == "No" || confirm == "n") 
                                {
                                break; 
                            }
                            
                            std::cout << "❌ Invalid input! Please enter 'yes' or 'no'.\n";
                        }
                        
                        if (confirm == "yes" || confirm == "YES" || confirm == "Yes" || confirm == "y") 
                        {
                            
                            //kasr pol az hafeze moshtari
                            if (currentCustomer->payForOrder(finalTotal)) 
                            { 
    
                        // beroz resani kif pol
                        if (customerDAO) {
                        customerDAO->updateWallet(currentCustomer->getCustomerId(), currentCustomer->getWallet()); 
                        }
    
                        // ezafe kardan emtiyaz vafadari
                        int earnedPoints = static_cast<int>((baseTotal / 1000) * currentCustomer->getLevel()->getMultiplier());
                        currentCustomer->addPoints(earnedPoints); 

                        customerDAO->updateCustomerLevelAndPoints(
                        currentCustomer->getCustomerId(),
                        currentCustomer->getLoyaltyPoints(), 
                        currentCustomer->getLevelName()
                        );
                        //barrasi neshan karbari
                        std::string currentBadge = currentCustomer->getBadge();
                        std::string newBadge = currentBadge;

                        // barrasi 3 sefaresh motavali
                        std::vector<Order*> allOrders = orderDAO->getAllOrders();
                        int consecutiveOrders = 0;
                        for (Order* o : allOrders) 
                        {
                        if (o->getCustomerId() == currentCustomer->getCustomerId()) 
                        {
                            consecutiveOrders++;
                        }
                        }

                        // sefaresh bad az 9 shab
                        time_t now = time(nullptr);
                        struct tm* localTime = localtime(&now);
                        int currentHour = localTime->tm_hour;

                        bool hasFrequentBuyer = (currentBadge.find("Frequent Buyer") != std::string::npos);
                        bool hasNightCustomer = (currentBadge.find("Night Customer") != std::string::npos);

                        // Frequent Buyer
                        if (consecutiveOrders >= 3 && !hasFrequentBuyer) 
                        {
                             if (newBadge == "None") 
                             {
                                newBadge = "Frequent Buyer";
                        }else 
                        {
                            newBadge = newBadge + ", Frequent Buyer";
                        }
                        }

                        // Night Customer
                        if (currentHour >= 21 && currentHour <= 23 && !hasNightCustomer) 
                        {
                            if (newBadge == "None") 
                            {
                                newBadge = "Night Customer";
                        } else 
                        {
                                newBadge = newBadge + ", Night Customer";
                        }
                        }

                        // agar neshan jadidi ezafe shod
                        if (newBadge != currentBadge) 
                        {
                            currentCustomer->setBadge(newBadge);
                        customerDAO->updateBadge(currentCustomer->getCustomerId(), newBadge);
                        std::cout << "🏅 Congratulations! You earned the '" << newBadge << "' badge!\n";
                        }
                        // sabt sefaresh dar database baray eslah amar admin ba enteghal ghazaha
                        if (orderDAO) 
                        {
                        //sakht shey sefaresh  
                        Order* newOrder = new Order(0, currentCustomer->getCustomerId(), selected->getId());
                                    
                        //enteghal done b done item ha b sabad kharid baray mohasebe gheymat 
                        for (MenuItem* item : currentCustomer->getCart()) 
                        {
                            newOrder->addItem(item);
                        }
                                    
                        // zakhire dar database
                        orderDAO->addOrder(newOrder); 
                                    
                        // ezafe kardan b tarikhche
                        currentCustomer->addOrderToHistory(newOrder); 
                                }
                                
                                currentCustomer->clearCart(); 
                            }
                            
                        } else 
                        {
                            std::cout << "❌ Order cancelled. Items remain in your cart.\n";
                        }
                    } else 
                    {
                        std::cout << "Your cart is empty. No order placed.\n";
                    }
                }
                break;



            case 3:
                
                 viewOrderHistory(); 
                 break;

            case 4: 
            {
                double amount;
                std::cout << "Enter the amount to add to your wallet: ";
                std::cin >> amount;

                // jologiri az vorod alefba ya ghofl shodan
                if (std::cin.fail()) {
                    std::cin.clear(); // pak kardan vaziyat khata
                    std::cin.ignore(10000, '\n'); // nadide gereftan vorodi hay eshtebah ghabli
                    std::cout << "❌ Invalid input! Please enter a valid number.\n";
                    break;
                }

                // jologiri az vared kardan mabalegh manfi ya 0
                if (amount <= 0) 
                {
                    std::cout << "❌ Amount must be greater than zero!\n";
                    break;
                }

                // jologiri az vared kardan mabalegh kazaei va kheyli ziyad
                if (amount > 10000000) 
                {
                    std::cout << "❌ Amount is too high! Maximum deposit at once is $10,000,000.\n";
                    break;
                }

                if (amount < 10000) 
                {
                    std::cout << "❌ Amount too low! Minimum deposit is $10000.\n";
                    break;
                }

                // age sharayet dorost bod zakhire dar database
                currentCustomer->addFunds(amount);
                
                if (customerDAO) 
                {
                    customerDAO->updateWallet(currentCustomer->getCustomerId(), currentCustomer->getWallet());
                }
                
                std::cout << "✅ Wallet charged successfully! Your new balance: $" << currentCustomer->getWallet() << "\n";
                break;
            }


            
            case 5:
            {
                std::cout << "Returning to main menu...\n";
                break;
            }
            
            default:
            {
                std::cout << "Invalid choice! Please select 1-5.\n";
                break;
            }
        }
    }
   
}



void AppController::runRestaurantManagerPanel(Restaurant* restaurant) //safhe modir resturan
{
    int choice;
    while (true) 
    {
        std::cout << "\n==================================" << std::endl;
        std::cout << "   MANAGER PANEL: " << restaurant->getName() << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Edit Restaurant Info" << std::endl;
        std::cout << "2. Manage Menu (Add/Edit/Remove Items)" << std::endl;
        std::cout << "3. View & Process Incoming Orders" << std::endl;
        std::cout << "4. Logout" << std::endl;
        std::cout << "Selection: ";
        std::cin >> choice;

        if (choice == 4) break;

        switch (choice) 
{
    case 1: editCurrentRestaurantInfo(restaurant); break; 
    
    case 2: manageRestaurantMenu(restaurant); break; 
    
    case 3: viewRestaurantOrders(restaurant); break; 
    
    default: std::cout << "Invalid choice" << std::endl;
}
    }
} 



void AppController::viewRestaurantOrders(Restaurant* restaurant) 
{
    while (true) {
        
        std::vector<Order*> orders = orderDAO->getOrdersByRestaurant(restaurant->getId());
        
        std::cout << "\n--- Orders for " << restaurant->getName() << " ---" << std::endl;
        if (!orders.empty()) 
        {
            for (Order* o : orders) 
            {
                std::cout << "Order ID: " << o->getOrderId()
                     << " | Total: $" << o->getTotalPrice()
                     << " | Status: " << o->statusToString() << std::endl;
            }
        } else 
        {
            std::cout << "No orders found." << std::endl;
        }

        std::cout << "\nEnter ID to update (or 0 to return): ";
        int orderId; 
        std::cin >> orderId;
        
        if (orderId == 0) 
        {
            for(Order* o : orders) delete o;
            break; 
        }

        // peyda kardan sefaresh entekhab shode tavasot modir
        Order* selectedOrder = nullptr;
        for (Order* o : orders) {
            if (o->getOrderId() == orderId) 
            {
                selectedOrder = o;
                break;
            }
        }

        if (!selectedOrder) 
        {
            std::cout << "❌ Order ID not found." << std::endl;
            for(Order* o : orders) delete o;
            continue;
        }

        std::cout << "1. Pending | 2. Delivered | 3. Completed | 4. Cancelled\nEnter choice: ";
        int choice; 
        std::cin >> choice;

        if (choice >= 1 && choice <= 4) 
        {
            selectedOrder->updateStatus(static_cast<OrderStatus>(choice));
    
            if (restaurantDAO->updateOrderStatus(orderId, choice)) 
            {
                std::cout << "✅ Status updated successfully!" << std::endl;
            }else 
            {
                std::cout << "❌ Database update failed. Please try again." << std::endl;
           }
        }

        // azad sazi hafeze b sorat yekja 
        for(Order* o : orders) delete o;
        orders.clear();
        
        if(std::cin.fail()) 
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


void AppController::editRestaurantInfo(Restaurant* restaurant) 
{
    // daryaft tamam resturan ha az database
    restaurants = restaurantDAO->getAllRestaurants();
    
    std::cout << "\n--- ALL THE RESTAURANTS ---" << std::endl;
    for (size_t i = 0; i < restaurants.size(); ++i) 
    {
        std::cout << "ID: " << restaurants[i]->getId() << " NAME: " << restaurants[i]->getName() << std::endl;
    }

    int targetId = 0;
    std::string inputStr;

    std::cout << "\n>> ENTER RESTAURANTS ID YOU WANT TO CHANGE IT: ";
    
    // ravesh hoshmand va yek parche baray adam tadakhol ba bafer ghabli
    while (std::getline(std::cin, inputStr)) 
    {
        
        if (inputStr.empty() || inputStr == "\n" || inputStr == "\r") 
        {
            continue; 
        }
        if (inputStr == " ") 
        {
            continue;
        }
        
        
        try 
        {
            targetId = std::stoi(inputStr); // tabdil reshte b adad
            break; 
        } catch (...)
        {
            std::cout << "❌ Invalid Input! Please enter a valid number: ";
        }
    }

    // peyda kardan resturan bar asas id vared shode
    Restaurant* targetRes = nullptr;
    for (size_t i = 0; i < restaurants.size(); ++i) 
    {
        if (restaurants[i]->getId() == targetId) 
        {
            targetRes = restaurants[i];
            break;
        }
    }

    if (!targetRes) 
    {
        std::cout << "❌ Restaurant not found!" << std::endl;
        return;
    }

    // vorod b menu virayesh etelaat
    int choice = 0;
    while (choice != 6) 
    { 
        std::cout << "\n----- Edit Restaurant Information -----" << std::endl;
        std::cout << "Current Name: " << targetRes->getName() << std::endl;
        std::cout << "Current Address: " << targetRes->getAddress() << std::endl;
        std::cout << "Current Phone: " << targetRes->getPhoneNumber() << std::endl;
        std::cout << "Current Prep Time: " << targetRes->getEstimatedPrepTime() << " mins" << std::endl; 
        std::cout << "Current Description: " << targetRes->getDescription() << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        std::cout << "1. Change Restaurant Name" << std::endl;
        std::cout << "2. Change Restaurant Address" << std::endl;
        std::cout << "3. Change Restaurant Phone" << std::endl;
        std::cout << "4. Change Preparation Time" << std::endl;
        std::cout << "5. Change Description" << std::endl;
        std::cout << "6. Back to Admin Panel" << std::endl;
        std::cout << "Selection: ";
        std::cin >> choice;
        std::cin.ignore(10000, '\n'); // pak kardan bafer enter baz az adad

        if (choice == 6) break;

        std::string newStrValue;
        int newIntValue;

        switch (choice) {
            case 1:
                std::cout << "Enter new restaurant name: ";
                std::getline(std::cin, newStrValue);
                targetRes->setName(newStrValue);
                break;
            case 2:
                std::cout << "Enter new restaurant address: ";
                std::getline(std::cin, newStrValue);
                targetRes->setAddress(newStrValue);
                break;
            case 3:
                std::cout << "Enter new restaurant phone: ";
                std::getline(std::cin, newStrValue);
                targetRes->setPhoneNumber(newStrValue);  
                break;
            case 4:
                std::cout << "Enter new preparation time (minutes): ";
                if (std::cin >> newIntValue && newIntValue > 0) 
                {
                    targetRes->setEstimatedPrepTime(newIntValue);
                } else 
                {
                    std::cout << "Invalid time!" << std::endl;
                }
                std::cin.ignore(10000, '\n');
                break;
            case 5:
                std::cout << "Enter new description: ";
                std::getline(std::cin, newStrValue);
                targetRes->setDescription(newStrValue);
                break;
            default:
                std::cout << "Invalid selection!" << std::endl;
                continue;
        }

        // update database bad az har taghir
        restaurantDAO->updateRestaurant(targetRes);
        std::cout << ">> UPDATE FOR INFORMATION WAS SUCCESSFULLY <<" << std::endl;
    }
}


void AppController::runSystemAdminPanel() 
{
    int choice;
    while (true) {
        std::cout << "\n====================================" << std::endl;
        std::cout << "      SYSTEM ADMINISTRATOR PANEL     " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "1. Add New Restaurant" << std::endl;
        std::cout << "2. Activate/Deactivate Restaurants" << std::endl;
        std::cout << "3. View System Reports" << std::endl;
        std::cout << "4. Update Information" << std::endl;
        std::cout << "5. View Users by Level" << std::endl;        
        std::cout << "6. Edit User Level Manually" << std::endl;    
        std::cout << "7. View Level Change History" << std::endl;  
        std::cout << "8. Assign Monthly Coupons\n" << std::endl;   
        std::cout << "9. Logout" << std::endl;                      
        std::cout << "Selection: ";
        std::cin >> choice;

        if (choice == 9) break;

        switch (choice) 
        {
            case 1: addNewRestaurant(); break;
            case 2: manageRestaurantActivation(); break;
            case 3: viewSystemReports(); break;
            case 4: selectAndEditRestaurant(); break;
            case 5: showUsersByLevel(); break;
            case 6: editUserLevelManually(); break;
            case 7: viewLevelChangeHistory(); break;
            case 8: assignMonthlyCoupons(); break;
            default: std::cout << "Invalid choice" << std::endl;
        }
    }
}
void AppController::addNewRestaurant()
{
    std::string name, address, phone, description, password;
    int prepTime;

    std::cout << "Enter restaurant name: ";
    
    std::cin.ignore(10000, '\n'); 
    std::getline(std::cin, name);

    if (name.empty() || name == " ") 
    {
        std::cout << "❌ Error: Restaurant name cannot be empty!\n";
        return; 
    }

    std::cout << "Enter address: ";
    std::getline(std::cin, address);

    std::cout << "Enter phone: ";
    std::getline(std::cin, phone);

    std::cout << "Enter description: ";
    std::getline(std::cin, description);

    std::cout << "Enter preparation time (minutes): ";
    while (!(std::cin >> prepTime) || prepTime <= 0) 
    {
        std::cout << "❌ Invalid input! Please enter a positive number for minutes: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n'); 

    
    std::cout << "Enter Password for Restaurant Manager: ";
    std::getline(std::cin, password);

    // sakht resturan ba password
    Restaurant* newRes = new Restaurant(0, name, address, prepTime, phone, description, true, nullptr, nullptr, password);
    
    // ezafe kardan b database
    restaurantDAO->addRestaurant(newRes);
    
    //ezafe kardan b list ram
    restaurants.push_back(newRes);

    std::cout << "✅ Restaurant \"" << name << "\" added successfully to the system!" << std::endl;
    std::cout << "New ID: " << newRes->getId() << std::endl;
}



void AppController::viewSystemReports() //gozareshat
{
    std::cout << "\n========== SYSTEM REPORT ==========" << std::endl;
    restaurants = restaurantDAO->getAllRestaurants(); 

    std::cout << "Total Restaurants: " << restaurants.size() << std::endl;
    
    int activeCount = 0;
    for (size_t i = 0; i < restaurants.size(); ++i)
    {
        Restaurant* res = restaurants[i];
        if (res && res->getIsActive())
            ++activeCount;
    }
    
    std::cout << "Active Restaurants: " << activeCount << std::endl;
    std::cout << "Inactive Restaurants: " << restaurants.size() - activeCount << std::endl;
    
   //mohasebe sefareshat va mabalegh forosh
    std::cout << "-----------------------------------" << std::endl;
    
    // gereftan sefareshat
    std::vector<Order*> allOrders = orderDAO->getAllOrders(); 
    std::cout << "Total Orders Placed: " << allOrders.size() << std::endl;
    
    // mohasebe maj mo forosh
    double totalSalesAmount = 0.0;
    for (size_t i = 0; i < allOrders.size(); ++i) 
    {
        if (allOrders[i]) 
        {
            //mohasebe mostaghim ke 0 nashon nade
            totalSalesAmount += allOrders[i]->calculateTotalPrice(); 
        }
    }
    std::cout << "Total Sales Amount: " << totalSalesAmount << " Toman" << std::endl;
    
    //namayesh list kamel joziyat b addmin
    if (!allOrders.empty()) {
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "       🧾 ALL ORDERS DETAILS 🧾" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Order ID | Customer ID | Restaurant ID | Total Price" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        
        for (size_t i = 0; i < allOrders.size(); ++i) {
            if (allOrders[i]) {
                std::cout << "   " << allOrders[i]->getOrderId() << "     |      " 
                          << allOrders[i]->getCustomerId() << "      |       " 
                          << allOrders[i]->getRestaurantId() << "       | " 
                          << allOrders[i]->getTotalPrice() << " Toman" << std::endl;
            }
        }
    } else 
    {
        std::cout << "No orders registered in the system yet." << std::endl;
    }
    // -----------------------------------------------------------------

    std::cout << "===================================" << std::endl;
}



Customer* AppController::findCustomerById(int id) 
{
    return customerDAO->findCustomerById(id);
}

void AppController::addMenuItemToRestaurant(int restId, MenuItem* item) 
{
    if (item) 
    {

         menuItemDAO->addMenuItem(restId, item);
    }
}

void AppController::addOrder(Order* o) 
{
    if (o) 
    {
        orderDAO->addOrder(o);
    }
}

std::vector<Customer*> AppController::getAllCustomers() 
{
    return customerDAO->getAllCustomers();
}

std::vector<Restaurant*> AppController::getAllRestaurants() 
{
    return restaurants;
}

std::vector<MenuItem*> AppController::getAllMenuItemsForRestaurant(int restId) 
{
    return menuItemDAO->getMenuItemsByRestaurant(restId);
}

void AppController::removeMenuItemById(int id) 
{
    for (auto res : restaurants) 
    {
        res->removeMenuItem(id);
    }
    menuItemDAO->removeMenuItem(id);
}
void AppController::managerAddFoodItem(Restaurant* restaurant) 
{
    if (!restaurant) return;
    
    std::string name, description;
    double price;
    int typeChoice;
    
    std::cout << "\n--- Add New Item to Menu ---" << std::endl;
    std::cout << "Select Type (1. Food | 2. Drink | 3. Dessert): ";
    std::cin >> typeChoice;
    std::cin.ignore(10000, '\n');
    
    std::cout << "Enter Name: ";
    std::getline(std::cin, name);
    
    std::cout << "Enter Price: ";
    std::cin >> price;
    std::cin.ignore(10000, '\n');
    
    std::cout << "Enter Description: ";
    std::getline(std::cin, description);
    
    MenuItem* newItem = nullptr;

    if (typeChoice == 1) 
    { 
        int cookTime;
        char vegChar;
        bool isVeg = false;
        
        std::cout << "Enter Cooking Time (minutes): ";
        std::cin >> cookTime;
        std::cout << "Is it Vegetarian? (y/n): ";
        std::cin >> vegChar;
        std::cin.ignore(10000, '\n');
        if (vegChar == 'y' || vegChar == 'Y') isVeg = true;
        
        newItem = new FoodItem(0, name, description, price, true, cookTime, isVeg);
        
    } 
    else if (typeChoice == 2) 
    { 
        char iceChar;
        bool isCold = false;
        double volume;
        
        std::cout << "Is it Ice/Cold Drink? (y/n): ";
        std::cin >> iceChar;
        if (iceChar == 'y' || iceChar == 'Y') isCold = true;

        std::cin.ignore(10000, '\n');
        
        std::cout << "Enter Volume (e.g., 0.33 or 0.5): ";
        std::string volumeStr;
        std::getline(std::cin, volumeStr);
        
        size_t pos = volumeStr.find(',');
        if (pos != std::string::npos) 
        {
            volumeStr.replace(pos, 1, ".");
        }
        
        try 
        {
            volume = std::stod(volumeStr);
        } catch (...) 
        {
            std::cout << "❌ Invalid volume! Please enter a number (e.g., 0.5).\n";
            return;
        }
        
        newItem = new DrinkItem(ItemType::DRINK, 0, name, description, price, true, volume, isCold); 
    }
    else 
    { 
        double sugarLevel;
        
        std::cout << "Enter Sugar Level percentage (e.g., 10 or 25.5): ";
        std::string sugarStr;
        std::getline(std::cin, sugarStr);
        
        size_t pos2 = sugarStr.find(',');
        if (pos2 != std::string::npos) {
            sugarStr.replace(pos2, 1, ".");
        }
        
        try {
            sugarLevel = std::stod(sugarStr);
        } catch (...) {
            std::cout << "❌ Invalid sugar level! Please enter a number (e.g., 25.5).\n";
            return;
        }
        
        newItem = new DessertItem(ItemType::DESSERT, 0, name, description, price, true, sugarLevel);
    }
    
    if (newItem) 
    {
        menuItemDAO->addMenuItem(restaurant->getId(), newItem);
        std::cout << "✅ Item added successfully!" << std::endl;
    }
}
void AppController::managerUpdateItem(Restaurant* restaurant) 
{
    if (!restaurant) return;
    
    std::cout << "\n==================================" << std::endl;
    std::cout << "       UPDATE FOOD ITEM           " << std::endl;
    std::cout << "==================================" << std::endl;
    
    // gereftan tamam ghazahay in restuan moshakhas az database
    std::vector<MenuItem*> items = menuItemDAO->getMenuItemsByRestaurant(restaurant->getId());
    if (items.empty()) 
    {
        std::cout << "⚠️ Your menu is empty! Nothing to update." << std::endl;
        return;
    }
    
    // namayesh list ghazaha b hamrah id modir
    std::cout << "\n--- Available Items for Update ---" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) 
    {
        std::cout << "ID: " << items[i]->getId() 
                  << " | Name: " << items[i]->getName() 
                  << " | Price: " << items[i]->getBasePrice() << std::endl;
    }
    
    int itemId;
    std::cout << "\nEnter Item ID to update: ";
    std::cin >> itemId;
    std::cin.ignore(10000, '\n'); 
    
    //peyda kardan ghazay entekhab shode dar beyn list
    MenuItem* itemToUpdate = nullptr;
    for (auto* it : items) 
    {
        if (it->getId() == itemId) 
        {
            itemToUpdate = it;
            break;
        }
    }
    
    if (!itemToUpdate) 
    {
        std::cout << "❌ Item not found with the given ID!" << std::endl;
        return;
    }
    
    // darkhast moshakhasat jadid az karbar
    std::string newName;
    std::cout << "Current Name: " << itemToUpdate->getName() << " -> Enter new name (Leave empty to keep): ";
    std::getline(std::cin, newName);
    if (!newName.empty()) itemToUpdate->setName(newName);
    
    double newPrice;
    std::cout << "Current Price: " << itemToUpdate->getBasePrice() << " -> Enter new price (0 to keep): ";
    std::cin >> newPrice;
    std::cin.ignore(10000, '\n');
    if (newPrice > 0) itemToUpdate->setBasePrice(newPrice);
    
   
    menuItemDAO->updateMenuItem(itemToUpdate);
}


void AppController::managerRemoveItem(Restaurant* restaurant) 
{
    if (!restaurant) return;
    
    std::cout << "\n--- Remove Food Item from " << restaurant->getName() << " ---" << std::endl;
    
    // load kardan mostaghuim akharin vaziyat data base
    std::vector<MenuItem*> items = menuItemDAO->getMenuItemsByRestaurant(restaurant->getId());
    if (items.empty()) 
    {
        std::cout << "Your menu is empty!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < items.size(); ++i) 
    {
        std::cout << "ID: " << items[i]->getId() << " | Name: " << items[i]->getName() << " | Price: " << items[i]->getBasePrice() << std::endl;
    }
    
    int itemId;
    std::cout << "Enter Item ID to remove: ";
    if (!(std::cin >> itemId)) {
        for (auto* it : items) delete it;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }
    std::cin.ignore(10000, '\n');
    
    // seda zadan method hazf database
    menuItemDAO->removeMenuItem(itemId);
    
    // pak sazi hafeze movaghat load shode
    for (auto* it : items) delete it;
}






AppController::~AppController()
{
    // azad sazi hafede resturan ha
    for (size_t i = 0; i < restaurants.size(); ++i) 
    {
        delete restaurants[i];
    }
    restaurants.clear();
    
}
void AppController::customerLoginMenu() 
{
    while (true) 
    { 
        std::cout << "\n=========================================\n";
        std::cout << "      🍔 Welcome to Customer Portal 🍔      \n";
        std::cout << "=========================================\n";
        std::cout << "1. Login (vorod)\n";
        std::cout << "2. Sign Up (sabt nam)\n";
        std::cout << "0. Back to Main Menu (bazgasht)\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        if (!(std::cin >> choice)) 
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n❌ Voroodi namotabar! Lotfan faghat adad vared konid.\n";
            continue;
        }

        if (choice == 1) 
        {
            if (customerLogin()) 
            {
                showCustomerMenu();
                
                //paksazi
                delete currentCustomer;
                currentCustomer = nullptr;
                
              
            } 
        } 
        else if (choice == 2) 
        {
            if (customerSignup()) 
            {
                std::cout << "\n✅ Sabt nam anjam shod! Hala mitavanid Login konid.\n";
            }
        }
        else if (choice == 0) 
        {
            std::cout << "Returning to main menu...\n";
            break;
        } 
        else 
        {
            std::cout << "\n❌ Entekhab eshtebah ast!\n";
        }
    } 
}
bool AppController::customerSignup() 
{
    std::string name;
    std::string password; 
    
    std::cout << "\n--- 📝 Customer Sign Up (Sabt Nam) ---\n";
    
    if (std::cin.peek() == '\n') std::cin.ignore();

    while (true) {
        std::cout << "Enter Name (Nam-e shoma): ";
        std::getline(std::cin, name);
        if (!name.empty()) break;
        std::cout << "❌ Name cannot be empty!\n";
    }

    
    std::cout << "Enter Password (only numbers): ";
    std::getline(std::cin, password);

    // sakht moshtari
    Customer* newCustomer = new Customer(0, name, 0.0, password);
    
    if (customerDAO) {
        customerDAO->addCustomer(newCustomer);
        // namayesh id tavasot database
        std::cout << "\n✅ Sabt nam anjam shod! ID-e shoma: " << newCustomer->getCustomerId() << std::endl;
        std::cout << "Yaddasht konid! 📝\n";
    }
    
    delete newCustomer; 
    return true;
}



bool AppController::customerLogin() 
{
    int inputId;
    std::string inputName;
    std::string inputPass; 
    
    std::cout << "\n--- 🔐 Customer Login (Vorood) ---\n";
    std::cout << "Enter Customer ID (Shenase): ";
    std::cin >> inputId;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n❌ Shenase bayad adad bashe!\n";
        return false;
    }

    std::cout << "Enter Name: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, inputName);

   
    std::cout << "Enter Password: ";
    std::getline(std::cin, inputPass);

    std::vector<Customer*> allCustomers = customerDAO->getAllCustomers(); 
    Customer* foundCustomer = nullptr; 

    for (auto* c : allCustomers) {
        
        if (c->getCustomerId() == inputId && c->getName() == inputName && c->getPassword() == inputPass) 
        { 
            foundCustomer = c; 
            break; 
        }
    }

    if (foundCustomer) 
    {
        currentCustomer = new Customer(*foundCustomer); 
        std::cout << "\n🎉 Login movafaghiyat amiz bood! Khosh amadid, " << currentCustomer->getName() << "! 🎉\n";
        
        for (auto* customer : allCustomers) delete customer;
        allCustomers.clear(); 
        return true; 
    } else {
        std::cout << "\n❌ Khata: ID, Name ya Password eshtebah ast!\n";
        for (auto* c : allCustomers) delete c;
        allCustomers.clear();
        return false;
    }
}



void AppController::selectAndEditRestaurant() 
{
    std::vector<Restaurant*> restaurants = restaurantDAO->getAllRestaurants();
    if (restaurants.empty()) {
        std::cout << "\n>>THERE ISNT ANY THING HERE TO REFRESH \n";
        return;
    }

    std::cout << "\n--- ALL THE RESTAURANTS ---\n";
    for (const auto& restaurant : restaurants) 
    {
        if (restaurant != nullptr) 
        { //baray in ke barname crash nakone
            std::cout << "ID: " << restaurant->getId() << " NAME: " << restaurant->getName() << std::endl;
        }
    }


    int targetId;
    std::cout << "\n>> ENTER RESTAURANTS ID YOU WANT TO CHANGE IT ";
    std::cin >> targetId;

    // check kardan inke vorodi adad bashe
    if (std::cin.fail()) {
        std::cin.clear(); // pak kardan flag khata
        std::cin.ignore(10000, '\n'); // khali kardan bafer vorodi
        std::cout << "\n>> !!!JUST ENTER NUMBER!!!\n";
        return;
    }
    
    //pak kardan bafer baray getline
    std::cin.ignore(10000, '\n'); 

    Restaurant* selectedRestaurant = nullptr;
    for (auto& restaurant : restaurants) 
    {
        if (restaurant != nullptr && restaurant->getId() == targetId) 
        {
            selectedRestaurant = restaurant;
            break;
        }
    }

    if (selectedRestaurant) 
    {
        // hala ke resturan ro peyda kardim tabe asli virayesh ro seda mizanim
        editRestaurantInfo(selectedRestaurant);
        std::cout << "\n>> **UPDATE FOR INFORMATION WAS SECCUSFULLY**\n";
    } else 
    {
        std::cout << "\n>>!! WE CANT FIND A RESTAURANT WHITH THIS ID!!\n";
    }
}

void AppController::managerLoginMenu() 
{
    // load kardan resturan ha
    restaurants = restaurantDAO->getAllRestaurants(); 

    int restId;
    std::cout << "\n--- 👨‍🍳 Manager Login ---\n";
    std::cout << "Enter your Restaurant ID: ";
    
    //jologiri az crash dar sorat vared kardan horof
    while (!(std::cin >> restId)) {
        std::cout << "❌ Invalid ID! Please enter a numeric ID: ";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
    std::cin.ignore(10000, '\n'); 

    // peyda kardan resturan dar list
    Restaurant* targetRest = nullptr;
    for (size_t i = 0; i < restaurants.size(); ++i) 
    {
        if (restaurants[i]->getId() == restId) 
        {
            targetRest = restaurants[i];
            break;
        }
    }
    
    //barrasi vojod resturan va chek kardan password
    if (targetRest != nullptr) {
        std::string inputPassword;
        std::cout << "Enter your Password: ";
        std::getline(std::cin, inputPassword); //khandan imen password

        // check katdan ramz obor
        if (inputPassword == targetRest->getPassword()) 
        {
            std::cout << "✅ Login Successful! Welcome " << targetRest->getName() << " manager.\n";
            runRestaurantManagerPanel(targetRest);
        } else 
        {
            std::cout << "❌ Access Denied! Wrong password. 🛑\n";
        }
    } else 
    {
        std::cout << "❌ Restaurant not found!  👽\n";
    }
   
    for (auto* r : restaurants) 
    {
        delete r;
    }
    restaurants.clear();
}

void AppController::manageRestaurantActivation() 
{
    std::cout << "\n--- 🔄 Manage Restaurant Activation ---\n";
    std::cout << "Enter Restaurant ID to change status: ";
    int restId;
    std::cin >> restId;

    //peyda kardan resturan dar database
    Restaurant* rest = restaurantDAO->findRestaurantById(restId);

    if (rest != nullptr) 
    {
        std::cout << "Restaurant: " << rest->getName() << " | Current Status: " << (rest->getIsActive() ? "Active" : "Inactive") << std::endl;
        std::cout << "Do you want to toggle status? (1 for Yes, 0 for No): ";
        int choice;
        std::cin >> choice;

        if (choice == 1) 
        {
            // taghir vaziyat dar object
            bool newStatus = !rest->getIsActive();
            rest->setIsActive(newStatus);
            
            // zakhire taghirat dar database
            restaurantDAO->updateRestaurant(rest);
            
            std::cout << "✅ Status updated successfully to: " << (rest->getIsActive() ? "Active" : "Inactive") << std::endl;
        }
    } else {
        std::cout << "❌ Restaurant not found!\n";
    }
}



void AppController::viewOrderHistory() 
{
    if (currentCustomer == nullptr) 
    {
        std::cout << "❌ Please login first to view your order history!" << std::endl;
        return;
    }

    std::cout << "\n===================================" << std::endl;
    std::cout << "  Order History for " << currentCustomer->getName() << " 📜" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << " 🍗🍖🌭🍔🍟🍕🍱🥙🧆🌮🌯🫔🥘🥗"<< std::endl;

    //khandan be sorat zende az database
    std::vector<Order*> allOrders = orderDAO->getAllOrders(); 
    int orderCount = 0;

    for (size_t i = 0; i < allOrders.size(); ++i) 
    {
        if (allOrders[i] != nullptr && allOrders[i]->getCustomerId() == currentCustomer->getCustomerId()) 
        {
            
            orderCount++;
            std::cout << "📦 Order " << orderCount << ":" << std::endl;
            
            // chap joziyat
            allOrders[i]->displayOrderDetails();

            std::cout << "Status: " << allOrders[i]->getStatus() << std::endl;
            std::cout << "-----------------------------------" << std::endl;
        }
    }

    if (orderCount == 0) 
    {
        std::cout << "You haven't placed any orders yet! 🍔" << std::endl;
    }
    std::cout << "===================================" << std::endl;
}

void AppController::showUsersByLevel() 
{
    std::vector<Customer*> customers = customerDAO->getAllCustomers();
    int normal = 0, silver = 0, gold = 0, vip = 0;
    
    for (Customer* c : customers) 
    {
        std::string level = c->getLevelName();
        if (level == "Normal") normal++;
        else if (level == "Silver") silver++;
        else if (level == "Gold") gold++;
        else if (level == "VIP") vip++;
        delete c;
    }
    
    std::cout << "\n========== USERS BY LEVEL ==========\n";
    std::cout << "👤 Normal: " << normal << " users\n";
    std::cout << "🥈 Silver: " << silver << " users\n";
    std::cout << "🥇 Gold: " << gold << " users\n";
    std::cout << "👑 VIP: " << vip << " users\n";
    std::cout << "====================================\n";
}

void AppController::editUserLevelManually() 
{
    int customerId;
    std::cout << "\n--- Edit User Level ---\n";
    std::cout << "Enter Customer ID: ";
    std::cin >> customerId;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "❌ Invalid input!\n";
        return;
    }
    
    Customer* customer = customerDAO->findCustomerById(customerId);
    if (!customer) 
    {
        std::cout << "❌ Customer not found!\n";
        return;
    }
    
    std::cout << "\nCurrent Level: " << customer->getLevelName() << "\n";
    std::cout << "Current Points: " << customer->getLoyaltyPoints() << "\n";
    std::cout << "------------------------\n";
    std::cout << "1. Change Level\n";
    std::cout << "2. Change Points\n";
    std::cout << "3. Downgrade Level\n";
    std::cout << "Enter choice: ";
    int choice;
    std::cin >> choice;
    
    std::string oldLevel = customer->getLevelName();
    
    if (choice == 1) {
        std::cout << "Select new level (Normal, Silver, Gold, VIP): ";
        std::string newLevel;
        std::cin >> newLevel;
        
        IMembershipLevel* level = createLevelFromName(newLevel);
        customer->setLevel(level);
        customer->setLoyaltyPoints(customer->getLoyaltyPoints()); // hefz emtiyaz
        
        customerDAO->updateCustomerLevelAndPoints(
            customerId,
            customer->getLoyaltyPoints(),
            customer->getLevelName()
        );
        customerDAO->addMembershipHistory(customerId, oldLevel, customer->getLevelName(), "Admin manual change");
        std::cout << "✅ Level updated successfully!\n";
    } 
    else if (choice == 2) 
    {
        int newPoints;
        std::cout << "Enter new points: ";
        std::cin >> newPoints;
        customer->setLoyaltyPoints(newPoints);
        customer->checkAndUpgrade(); // barrasi ertegha
        
        customerDAO->updateCustomerLevelAndPoints(
            customerId,
            customer->getLoyaltyPoints(),
            customer->getLevelName()
        );
        customerDAO->addMembershipHistory(customerId, oldLevel, customer->getLevelName(), "Admin points change");
        std::cout << "✅ Points updated successfully! New level: " << customer->getLevelName() << "\n";
    }
    else if (choice == 3) 
    {
        std::string reason;
        std::cout << "Enter reason for downgrade: ";
        std::cin.ignore();
        std::getline(std::cin, reason);
        customer->downgradeLevel(reason);
        
        customerDAO->updateCustomerLevelAndPoints(
            customerId,
            customer->getLoyaltyPoints(),
            customer->getLevelName()
        );
        customerDAO->addMembershipHistory(customerId, oldLevel, customer->getLevelName(), reason);
        std::cout << "✅ Downgrade completed!\n";
    }
    else {
        std::cout << "❌ Invalid choice!\n";
    }
    
    delete customer;
}

void AppController::viewLevelChangeHistory() 
{
    std::string sql = "SELECT * FROM MembershipHistory ORDER BY changeDate DESC LIMIT 20;";
    auto rows = dbManager.fetchAll(sql);
    
    std::cout << "\n========== LEVEL CHANGE HISTORY ==========\n";
    if (rows.empty()) 
    {
        std::cout << "No history found.\n";
        return;
    }
    
    for (const auto& row : rows) {
        std::cout << "Customer: " << row.at("customerId") 
                  << " | " << row.at("oldLevel") << " → " << row.at("newLevel")
                  << " | " << row.at("changeDate")
                  << " | Reason: " << row.at("reason") << "\n";
    }
    std::cout << "===========================================\n";
}
void AppController::assignMonthlyCoupons() 
{
    std::vector<Customer*> customers = customerDAO->getAllCustomers();
    
    for (Customer* c : customers) 
    {
        int coupons = 0;
        std::string level = c->getLevelName();
        
        if (level == "Silver" || level == "Gold") 
        {
            coupons = 1;
        } else if (level == "VIP") 
        {
            coupons = 3;
        }
        // Normal = 0 
        
        customerDAO->updateMonthlyCoupons(c->getCustomerId(), coupons);
        c->setMonthlyCoupons(coupons);
        delete c;
    }
    
    std::cout << "✅ Monthly coupons assigned to all users!\n";
}