#include "restaurantmenuwindow.h"
#include "ui_restaurantmenuwindow.h"
#include "customerpanelwindow.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <ctime>
#include <string>

RestaurantMenuWindow::RestaurantMenuWindow(int restaurantId, int customerId, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RestaurantMenuWindow)
    , currentRestaurantId(restaurantId)
    , currentCustomerId(customerId)
    , totalPrice(0.0)
{
    ui->setupUi(this);
    setWindowTitle("Restaurant Menu - ID: " + QString::number(restaurantId));

    loadMenu();
    updateCartDisplay();
    updateInvoiceDisplay();
}

RestaurantMenuWindow::~RestaurantMenuWindow()
{
    delete ui;
}

// bargozari menu
void RestaurantMenuWindow::loadMenu()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT id, name, description, basePrice, type, isVegetarian FROM MenuItems WHERE restaurantId = " +
                      std::to_string(currentRestaurantId) + " AND isAvailable = 1";
    auto rows = db.fetchAll(sql);

    ui->textBrowser_menu->clear();
    ui->textBrowser_menu->append("========== " + QString::fromStdString("Menu") + " ==========");

    if (rows.empty())
    {
        ui->textBrowser_menu->append("No items available.");
        db.close();
        return;
    }

    for (const auto& row : rows)
    {
        int id = std::stoi(row.at("id"));
        QString name = QString::fromStdString(row.at("name"));
        QString desc = QString::fromStdString(row.at("description"));
        double price = std::stod(row.at("basePrice"));
        int type = std::stoi(row.at("type"));
        int veg = std::stoi(row.at("isVegetarian"));

        QString typeName = (type == 3) ? "🍔 Food" : (type == 2) ? "🥤 Drink" : "🍰 Dessert";
        QString vegText = (veg == 1) ? " 🌱" : "";

        ui->textBrowser_menu->append(QString::number(id) + ". " + name + vegText);
        ui->textBrowser_menu->append("   " + desc);
        ui->textBrowser_menu->append("   $" + QString::number(price, 'f', 0) + " (" + typeName + ")");
        ui->textBrowser_menu->append("   --------------------");
        ui->textBrowser_menu->append("");
    }
    db.close();
}

// ezafe kardan be sabad kharid
void RestaurantMenuWindow::on_pushButton_add_to_cart_clicked()
{
    if (currentRestaurantId == -1)
    {
        QMessageBox::warning(this, "Error", "Please select a restaurant first!");
        return;
    }

    bool ok;
    int itemId = QInputDialog::getInt(this, "Add to Cart",
                                      "Enter Item ID to ADD:",
                                      1, 1, 1000, 1, &ok);

    if (!ok) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string checkSql = "SELECT id, basePrice FROM MenuItems WHERE id = " + std::to_string(itemId) +
                           " AND restaurantId = " + std::to_string(currentRestaurantId) +
                           " AND isAvailable = 1";
    auto checkRows = db.fetchAll(checkSql);

    if (checkRows.empty())
    {
        QMessageBox::warning(this, "Error", "❌ Invalid Food ID!");
        db.close();
        return;
    }

    int qty = QInputDialog::getInt(this, "Add Item",
                                   "How many?",
                                   1, 1, 100, 1, &ok);
    if (!ok) {
        db.close();
        return;
    }

    double price = std::stod(checkRows[0].at("basePrice"));
    for (int i = 0; i < qty; i++)
    {
        cart.push_back(itemId);
        totalPrice += price;
    }

    db.close();

    updateCartDisplay();
    updateInvoiceDisplay();

    QMessageBox::information(this, "Added", "✅ " + QString::number(qty) + " Item(s) added to cart!");
}

// hazf az sabad kharid
void RestaurantMenuWindow::on_pushButton_remove_item_clicked()
{
    if (cart.empty())
    {
        QMessageBox::warning(this, "Error", "Cart is empty!");
        return;
    }

    bool ok;
    int itemId = QInputDialog::getInt(this, "Remove Item",
                                      "Enter Item ID to REMOVE:",
                                      1, 1, 1000, 1, &ok);

    if (!ok) return;

    int qty = QInputDialog::getInt(this, "Remove Item",
                                   "How many to remove?",
                                   1, 1, 100, 1, &ok);
    if (!ok) return;

    int removed = 0;
    for (auto it = cart.begin(); it != cart.end() && removed < qty; )
    {
        if (*it == itemId)
        {
            it = cart.erase(it);
            removed++;

            DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
            if (db.open())
            {
                std::string sql = "SELECT basePrice FROM MenuItems WHERE id = " + std::to_string(itemId);
                auto rows = db.fetchAll(sql);
                if (!rows.empty())
                {
                    totalPrice -= std::stod(rows[0].at("basePrice"));
                }
                db.close();
            }
        }
        else
        {
            ++it;
        }
    }

    if (removed > 0)
    {
        QMessageBox::information(this, "Removed", "✅ " + QString::number(removed) + " Item(s) removed from cart!");
        updateCartDisplay();
        updateInvoiceDisplay();
    }
    else
    {
        QMessageBox::warning(this, "Error", "❌ Item not found in cart!");
    }
}

//khali kardan sabad kharid
void RestaurantMenuWindow::on_pushButton_clear_cart_clicked()
{
    if (cart.empty())
    {
        QMessageBox::warning(this, "Error", "Cart is already empty!");
        return;
    }

    int reply = QMessageBox::question(this, "Clear Cart",
                                      "Are you sure you want to clear the entire cart?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        cart.clear();
        totalPrice = 0.0;
        updateCartDisplay();
        updateInvoiceDisplay();
        QMessageBox::information(this, "Cleared", "✅ Cart cleared successfully!");
    }
}

// laghv sefaresh
void RestaurantMenuWindow::on_pushButton_cancel_order_clicked()
{
    if (cart.empty())
    {
        QMessageBox::warning(this, "Error", "Cart is already empty!");
        return;
    }

    int reply = QMessageBox::question(this, "Cancel Order",
                                      "Are you sure you want to cancel the entire order?\n"
                                      "All items will be removed from your cart.",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        cart.clear();
        totalPrice = 0.0;
        updateCartDisplay();
        updateInvoiceDisplay();
        QMessageBox::information(this, "Cancelled", "✅ Order cancelled successfully!");
    }
}

//tasviye hesab
void RestaurantMenuWindow::on_pushButton_checkout_clicked()
{
    if (cart.empty())
    {
        QMessageBox::warning(this, "Error", "Cart is empty!");
        return;
    }

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    // daryaft etaat karbar
    std::string customerSql = "SELECT walletBalance, membershipLevel, loyaltyPoints, monthlyCoupons FROM Customers WHERE id = " + std::to_string(currentCustomerId);
    auto customerRows = db.fetchAll(customerSql);

    if (customerRows.empty())
    {
        QMessageBox::warning(this, "Error", "Customer not found!");
        db.close();
        return;
    }

    double balance = std::stod(customerRows[0].at("walletBalance"));
    QString level = QString::fromStdString(customerRows[0].at("membershipLevel"));
    int currentPoints = std::stoi(customerRows[0].at("loyaltyPoints"));
    int currentCoupons = std::stoi(customerRows[0].at("monthlyCoupons"));

    //mohasebe takhfif sath
    double discountPercent = 0.0;
    if (level == "Silver") discountPercent = 0.05;
    else if (level == "Gold") discountPercent = 0.10;
    else if (level == "VIP") discountPercent = 0.15;

    double discount = totalPrice * discountPercent;
    double shipping = (level == "VIP") ? 0.0 : 50000.0;
    double finalTotal = totalPrice - discount + shipping;

    //copoun
    bool usedCoupon = false;
    double couponDiscount = 0.0;

    if (currentCoupons > 0)
    {
        int reply = QMessageBox::question(this, "Coupon Available",
                                          "You have " + QString::number(currentCoupons) + " coupon(s) available.\n"
                                                                                          "Do you want to use a coupon for 10% extra discount?",
                                          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            usedCoupon = true;
            couponDiscount = finalTotal * 0.10;
            finalTotal = finalTotal - couponDiscount;

            currentCoupons--;
            std::string updateCouponSql = "UPDATE Customers SET monthlyCoupons = " + std::to_string(currentCoupons) +
                                          " WHERE id = " + std::to_string(currentCustomerId);
            db.executeQuery(updateCouponSql);
        }
    }

    //mohasebe emtiyaz kasb shode
    double multiplier = 1.0;
    if (level == "Silver") multiplier = 1.2;
    else if (level == "Gold") multiplier = 1.5;
    else if (level == "VIP") multiplier = 2.0;

    int earnedPoints = static_cast<int>((totalPrice / 1000) * multiplier);

    // namayesh factor
    QString invoice = "========== INVOICE ==========\n";
    invoice += "Total Items: " + QString::number(cart.size()) + "\n";
    invoice += "Base Price: $" + QString::number(totalPrice, 'f', 0) + "\n";
    if (discount > 0) {
        invoice += "Level Discount (" + QString::number(discountPercent * 100) + "%): -$" + QString::number(discount, 'f', 0) + "\n";
    }
    if (usedCoupon) {
        invoice += "Coupon Discount (10%): -$" + QString::number(couponDiscount, 'f', 0) + "\n";
    }
    invoice += "Shipping: $" + QString::number(shipping, 'f', 0) + "\n";
    invoice += "--------------------------------\n";
    invoice += "💰 Final Total: $" + QString::number(finalTotal, 'f', 0) + "\n";
    invoice += "⭐ Points Earned: " + QString::number(earnedPoints) + "\n";
    invoice += "Current Balance: $" + QString::number(balance, 'f', 0) + "\n";

    if (balance < finalTotal)
    {
        invoice += "\n⚠️ Insufficient funds! Please charge your wallet.";
        ui->textBrowser_invoice->setText(invoice);
        db.close();
        return;
    }

    // taeeid nahaei
    int reply = QMessageBox::question(this, "Confirm Order",
                                      "Do you want to finalize this order?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        db.close();
        return;
    }

    // sabt sefaresh
    std::string orderSql = "INSERT INTO Orders (customerId, restaurantId, totalPrice, status, orderDate) VALUES (" +
                           std::to_string(currentCustomerId) + ", " +
                           std::to_string(currentRestaurantId) + ", " +
                           std::to_string(finalTotal) + ", 0, datetime('now'))";

    // bad az sabt sefaresh movafagh
    if (db.executeQuery(orderSql))
    {
        int orderId = db.getLastInsertId();

        // sabt item haye sefaresh
        for (int itemId : cart)
        {
            std::string itemSql = "INSERT INTO OrderItems (orderId, menuItemId, quantity) VALUES (" +
                                  std::to_string(orderId) + ", " + std::to_string(itemId) + ", 1)";
            db.executeQuery(itemSql);
        }

        //kasr kif az pol
        std::string updateBalance = "UPDATE Customers SET walletBalance = walletBalance - " + std::to_string(finalTotal) +
                                    " WHERE id = " + std::to_string(currentCustomerId);
        db.executeQuery(updateBalance);

        //ezafe kardan emtiyaz
        int newPoints = currentPoints + earnedPoints;
        std::string updatePointsSql = "UPDATE Customers SET loyaltyPoints = " + std::to_string(newPoints) +
                                      " WHERE id = " + std::to_string(currentCustomerId);
        db.executeQuery(updatePointsSql);

        // barrasi ertegha sath
        int silverThreshold = 100;
        int goldThreshold = 300;
        int vipThreshold = 700;

        std::string newLevelStr = level.toStdString();
        if (newPoints >= vipThreshold)
        {
            newLevelStr = "VIP";
        } else if (newPoints >= goldThreshold)
        {
            newLevelStr = "Gold";
        } else if (newPoints >= silverThreshold)
        {
            newLevelStr = "Silver";
        } else
        {
            newLevelStr = "Normal";
        }

        if (newLevelStr != level.toStdString())
        {
            std::string updateLevelSql = "UPDATE Customers SET membershipLevel = '" + newLevelStr +
                                         "' WHERE id = " + std::to_string(currentCustomerId);
            db.executeQuery(updateLevelSql);

            std::string historySql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                                     std::to_string(currentCustomerId) + ", '" +
                                     level.toStdString() + "', '" +
                                     newLevelStr + "', datetime('now'), 'Auto upgrade')";
            db.executeQuery(historySql);

            QMessageBox::information(this, "Upgrade", "🎉 Congratulations! You've been upgraded to " +
                                                          QString::fromStdString(newLevelStr) + " level!");
        }

        // barrasi neshan haye karbari
        std::string orderCountSql = "SELECT COUNT(*) FROM Orders WHERE customerId = " + std::to_string(currentCustomerId);
        auto orderCountRows = db.fetchAll(orderCountSql);
        int totalOrders = std::stoi(orderCountRows[0].at("COUNT(*)"));

        //daryaft neshan feli
        std::string badgeSql = "SELECT badge FROM Customers WHERE id = " + std::to_string(currentCustomerId);
        auto badgeRows = db.fetchAll(badgeSql);
        std::string currentBadge = badgeRows[0].at("badge");
        std::string newBadge = currentBadge;

        // Frequent Buyer
        if (totalOrders >= 3 && currentBadge.find("Frequent Buyer") == std::string::npos)
        {
            if (newBadge == "None")
            {
                newBadge = "Frequent Buyer";
            } else {
                newBadge = newBadge + ", Frequent Buyer";
            }
        }

        // 2. Night Customer
        time_t now = time(nullptr);
        struct tm* localTime = localtime(&now);
        int currentHour = localTime->tm_hour;

        if (currentHour >= 21 && currentHour <= 24 && currentBadge.find("Night Customer") == std::string::npos)
        {
            if (newBadge == "None")
            {
                newBadge = "Night Customer";
            } else
            {
                newBadge = newBadge + ", Night Customer";
            }
        }

        // zakhire nashan jadid
        if (newBadge != currentBadge)
        {
            std::string updateBadgeSql = "UPDATE Customers SET badge = '" + newBadge +
                                         "' WHERE id = " + std::to_string(currentCustomerId);
            db.executeQuery(updateBadgeSql);
            QMessageBox::information(this, "Badge", "🏅 Congratulations! You earned the '" +
                                                        QString::fromStdString(newBadge) + "' badge!");
        }

        // daryaft mojodi jadid
        std::string newBalanceSql = "SELECT walletBalance FROM Customers WHERE id = " + std::to_string(currentCustomerId);
        auto newBalanceRows = db.fetchAll(newBalanceSql);
        double newBalance = 0;
        if (!newBalanceRows.empty())
        {
            newBalance = std::stod(newBalanceRows[0].at("walletBalance"));
        }

        // namayesh factor nahaei
        invoice += "\n✅ Payment successful!";
        invoice += "\nRemaining balance: $" + QString::number(newBalance, 'f', 0);
        invoice += "\n🎉 Points: " + QString::number(newPoints);

        ui->textBrowser_invoice->setText(invoice);

        // pak kardan sabad kharid
        cart.clear();
        totalPrice = 0.0;
        updateCartDisplay();

        // payam movafaghiyat
        QString successMsg = QString("✅ Payment successful!\n") +
                             "Remaining balance: $" + QString::number(newBalance, 'f', 0) + "\n" +
                             "Points: " + QString::number(newPoints);

        QMessageBox::information(this, "Success", successMsg);
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to place order!");
    }

    db.close();
}

//beroz resani sabad kharid
void RestaurantMenuWindow::updateCartDisplay()
{
    ui->textBrowser_cart->clear();
    ui->textBrowser_cart->append("========== CART ==========");

    if (cart.empty())
    {
        ui->textBrowser_cart->append("Cart is empty.");
        return;
    }

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) return;

    for (int id : cart)
    {
        std::string sql = "SELECT name, basePrice FROM MenuItems WHERE id = " + std::to_string(id);
        auto rows = db.fetchAll(sql);
        if (!rows.empty())
        {
            QString name = QString::fromStdString(rows[0].at("name"));
            double price = std::stod(rows[0].at("basePrice"));
            ui->textBrowser_cart->append("- " + name + " ($" + QString::number(price, 'f', 0) + ")");
        }
    }
    db.close();

    ui->textBrowser_cart->append("Total: $" + QString::number(totalPrice, 'f', 0));
}

//be rozresani factor
void RestaurantMenuWindow::updateInvoiceDisplay()
{
    ui->textBrowser_invoice->clear();
    ui->textBrowser_invoice->append("========== INVOICE ==========");
    ui->textBrowser_invoice->append("Total: $" + QString::number(totalPrice, 'f', 0));
    ui->textBrowser_invoice->append("Items: " + QString::number(cart.size()));
}

//bargasht
void RestaurantMenuWindow::on_pushButton_back_clicked()
{
    this->close();
    CustomerPanelWindow *customerPanel = new CustomerPanelWindow(currentCustomerId);
    customerPanel->show();
}