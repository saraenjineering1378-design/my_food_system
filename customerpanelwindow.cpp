#include "customerpanelwindow.h"
#include "ui_customerpanelwindow.h"
#include "DatabaseManager.h"
#include "restaurantmenuwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
#include <QTextEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QDate>



CustomerPanelWindow::CustomerPanelWindow(int customerId, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomerPanelWindow)
    , currentCustomerId(customerId)
    , selectedRestaurantId(-1)
    , hasShownUpgradeMessage(false)
{
    ui->setupUi(this);
    setWindowTitle("Customer Panel - ID: " + QString::number(customerId));

    ui->pushButton_view_menu->setEnabled(false);

    loadRestaurants();
    updateWalletDisplay();
    updateLoyaltyDisplay();
    checkForUpgradeMessage();

    connect(ui->listWidget_restaurants, &QListWidget::itemClicked,
            this, &CustomerPanelWindow::on_restaurantSelected);
}

CustomerPanelWindow::~CustomerPanelWindow()
{
    delete ui;
}

//barrasi payam ertegha faghat yek bar
void CustomerPanelWindow::checkForUpgradeMessage()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) return;

    // barrasi kon ghablan payam namayesh dade shode
    std::string checkSql = "SELECT upgradeMessageShown FROM Customers WHERE id = " + std::to_string(currentCustomerId);
    auto checkRows = db.fetchAll(checkSql);

    if (!checkRows.empty())
    {
        int shown = std::stoi(checkRows[0].at("upgradeMessageShown"));
        if (shown == 1)
        {
            db.close();
            return;
        }
    }

    //akharin taghirat sath ro begir
    std::string sql = "SELECT oldLevel, newLevel, changeDate, reason FROM MembershipHistory WHERE customerId = " +
                      std::to_string(currentCustomerId) + " ORDER BY changeDate DESC LIMIT 1";
    auto rows = db.fetchAll(sql);

    if (!rows.empty())
    {
        std::string changeDate = rows[0].at("changeDate");
        std::string oldLevel = rows[0].at("oldLevel");
        std::string newLevel = rows[0].at("newLevel");
        std::string reason = rows[0].at("reason");

        // check kon emroz bashe va sath taghir karde bashe
        if (changeDate.find(QDate::currentDate().toString("yyyy-MM-dd").toStdString()) != std::string::npos)
        {
            if (oldLevel != newLevel)
            {
                QString msg;


                if (reason == "Auto upgrade")
                {
                    msg = QString("🎉 Congratulations! You've been upgraded to ") +
                          QString::fromStdString(newLevel) + " level!\n\n" +
                          "Your loyalty points reached the required amount.";
                }
                else if (reason == "Admin manual change")
                {
                    msg = QString("🔄 Your level has been changed by Admin.\n\n") +
                          "📊 Level changed from " + QString::fromStdString(oldLevel) + " to " + QString::fromStdString(newLevel) + ".\n" +
                          "👤 Action by: Admin\n" +
                          "📝 Reason: " + QString::fromStdString(reason);
                }
                else if (reason == "Admin points change")
                {
                    msg = QString("⭐ Your points have been updated by Admin.\n\n") +
                          "📊 Level changed from " + QString::fromStdString(oldLevel) + " to " + QString::fromStdString(newLevel) + ".\n" +
                          "👤 Action by: Admin\n" +
                          "📝 Reason: " + QString::fromStdString(reason);
                }
                else if (reason.find("downgrade") != std::string::npos)
                {
                    msg = QString("⛔ Your level has been downgraded by Admin.\n\n") +
                          "📊 Level changed from " + QString::fromStdString(oldLevel) + " to " + QString::fromStdString(newLevel) + ".\n" +
                          "👤 Action by: Admin\n" +
                          "📝 Reason: " + QString::fromStdString(reason);
                }
                else
                {
                    msg = QString("🔄 Your level has been changed.\n\n") +
                          "📊 Level changed from " + QString::fromStdString(oldLevel) + " to " + QString::fromStdString(newLevel) + ".\n" +
                          "📝 Reason: " + QString::fromStdString(reason);
                }

                QMessageBox::information(this, "Level Update", msg);

                //bad az namayesh dar database sabt kon
                std::string updateSql = "UPDATE Customers SET upgradeMessageShown = 1 WHERE id = " + std::to_string(currentCustomerId);
                db.executeQuery(updateSql);
            }
        }
    }
    db.close();
}
// beroz resani etelaat namayesh etelaat vafa dari
void CustomerPanelWindow::updateLoyaltyDisplay()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT membershipLevel, loyaltyPoints, badge, monthlyCoupons FROM Customers WHERE id = " + std::to_string(currentCustomerId);
    auto rows = db.fetchAll(sql);

    if (rows.empty()) {
        ui->label_level->setText("🏅 Level: Normal");
        ui->label_points->setText("⭐ Points: 0");
        ui->label_next_level->setText("➡️ Next Level at: 100 points");
        ui->label_badge->setText("🏷️ Badge: None");
        ui->label_coupons->setText("🎫 Coupons: 0");
        db.close();
        return;
    }

    auto& row = rows[0];
    QString level = QString::fromStdString(row.at("membershipLevel"));
    int points = std::stoi(row.at("loyaltyPoints"));
    QString badge = QString::fromStdString(row.at("badge"));
    int coupons = std::stoi(row.at("monthlyCoupons"));

    // tashkhis taghir level
    std::string historySql = "SELECT reason FROM MembershipHistory WHERE customerId = " +
                             std::to_string(currentCustomerId) + " ORDER BY changeDate DESC LIMIT 1";
    auto historyRows = db.fetchAll(historySql);

    QString changeType = "";
    if (!historyRows.empty()) 
    {
        std::string reason = historyRows[0].at("reason");
        if (reason.find("Admin") != std::string::npos) 
        {
            changeType = " (by Admin 👑)";
        } else if (reason == "Auto upgrade") 
        {
            changeType = " (by System 🚀)";
        } else if (reason.find("downgrade") != std::string::npos) 
        {
            changeType = " (by Admin ⬇️)";
        }
    }

    //age tarikhche nabod pish farz addmin bezar
    if (changeType.isEmpty()) 
    {
        changeType = " (by Admin 👑)";
    }

    // namayesh
    ui->label_level->setText("🏅 Level: " + level + changeType);
    ui->label_points->setText("⭐ Points: " + QString::number(points));

    int nextLevelPoints = 0;
    if (level == "Normal") nextLevelPoints = 100;
    else if (level == "Silver") nextLevelPoints = 300;
    else if (level == "Gold") nextLevelPoints = 700;
    else if (level == "VIP") nextLevelPoints = 0;

    if (nextLevelPoints > 0) {
        ui->label_next_level->setText("➡️ Next Level at: " + QString::number(nextLevelPoints) + " points");
    } else {
        ui->label_next_level->setText("🏆 Max Level!");
    }

    if (badge != "None" && !badge.isEmpty()) 
    {
        ui->label_badge->setText("🏷️ Badge: " + badge);
    } else 
    {
        ui->label_badge->setText("🏷️ Badge: None");
    }

    ui->label_coupons->setText("🎫 Coupons: " + QString::number(coupons));

    db.close();
}
// moshahede menu
void CustomerPanelWindow::on_pushButton_view_menu_clicked()
{
    if (selectedRestaurantId == -1)
    {
        QMessageBox::warning(this, "Error", "Please select a restaurant first!");
        return;
    }

    this->close();
    RestaurantMenuWindow *menuWindow = new RestaurantMenuWindow(selectedRestaurantId, currentCustomerId);
    menuWindow->show();
}

//sabt sefaresh
void CustomerPanelWindow::on_pushButton_place_order_clicked()
{
    if (selectedRestaurantId == -1)
    {
        QMessageBox::warning(this, "Error", "Please select a restaurant first!");
        return;
    }

    this->close();
    RestaurantMenuWindow *menuWindow = new RestaurantMenuWindow(selectedRestaurantId, currentCustomerId);
    menuWindow->show();
}

// tarikhche sefareshat
void CustomerPanelWindow::on_pushButton_order_history_clicked()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT o.id, o.totalPrice, o.status, o.orderDate, r.name as restaurantName "
                      "FROM Orders o "
                      "LEFT JOIN Restaurants r ON o.restaurantId = r.id "
                      "WHERE o.customerId = " + std::to_string(currentCustomerId) + " "
                                                            "ORDER BY o.id DESC";
    auto rows = db.fetchAll(sql);

    QString report = "========== ORDER HISTORY ==========\n\n";

    if (rows.empty())
    {
        report += "📭 No orders found.\n";
    }
    else
    {
        for (const auto& row : rows)
        {
            int orderId = std::stoi(row.at("id"));
            double totalPrice = std::stod(row.at("totalPrice"));
            int statusInt = std::stoi(row.at("status"));  // be int tabdil kon
            QString orderDate = QString::fromStdString(row.at("orderDate"));
            QString restaurantName = QString::fromStdString(row.at("restaurantName"));

            // namayesh vaziyat ha ba ghesmat modir hamahang hast
            QString statusText;
            if (statusInt == 0) statusText = "⏳ Pending";
            else if (statusInt == 1) statusText = "🔧 Preparing";    // ✅ 1 = Preparing
            else if (statusInt == 2) statusText = "🚚 Delivered";    // ✅ 2 = Delivered
            else if (statusInt == 3) statusText = "✅ Completed";    // ✅ 3 = Completed
            else if (statusInt == 4) statusText = "❌ Cancelled";    // ✅ 4 = Cancelled
            else statusText = "Unknown";

            report += "📦 Order #" + QString::number(orderId) + "\n";
            report += "   🏠 " + restaurantName + "\n";
            report += "   📅 " + orderDate + "\n";
            report += "   💰 " + QString::number(totalPrice, 'f', 0) + " Toman\n";
            report += "   📊 " + statusText + "\n";
            report += "   -------------------------\n\n";
        }
    }

    db.close();

    // namayesh dar yek QDialog (ba ghabeliyat scroll)
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Order History");
    dialog->setModal(true);
    dialog->resize(550, 450);

    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setPlainText(report);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Courier New", 10));
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QPushButton* closeButton = new QPushButton("Close", dialog);
    closeButton->setFixedWidth(100);
    QObject::connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(textEdit);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    dialog->exec();
}
//sharg kif pol
void CustomerPanelWindow::on_pushButton_charge_wallet_clicked()
{
    bool ok;
    double amount = QInputDialog::getDouble(this, "Charge Wallet",
                                            "Enter amount to add:",
                                            0, 10000, 10000000, 2, &ok);

    if (!ok) return;

    if (amount < 10000)
    {
        QMessageBox::warning(this, "Error", "❌ Amount too low! Minimum deposit is $10,000.");
        return;
    }
    if (amount > 10000000)
    {
        QMessageBox::warning(this, "Error", "❌ Amount is too high! Maximum deposit at once is $10,000,000.");
        return;
    }

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "UPDATE Customers SET walletBalance = walletBalance + " + std::to_string(amount) +
                      " WHERE id = " + std::to_string(currentCustomerId);

    if (db.executeQuery(sql))
    {
        QMessageBox::information(this, "Success", "✅ Wallet charged successfully!");
        updateWalletDisplay();
    } else
    {
        QMessageBox::warning(this, "Error", "Failed to charge wallet!");
    }
    db.close();
}

// khoroj
void CustomerPanelWindow::on_pushButton_logout_clicked()
{
    this->close();
    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();
}

// bar gozari resturan ha
void CustomerPanelWindow::loadRestaurants()
{
    ui->listWidget_restaurants->clear();
    selectedRestaurantId = -1;
    ui->pushButton_view_menu->setEnabled(false);

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT id, name, address, description FROM Restaurants WHERE isActive = 1 ORDER BY name";
    auto rows = db.fetchAll(sql);

    if (rows.empty())
    {
        ui->listWidget_restaurants->addItem("No restaurants available.");
        db.close();
        return;
    }

    for (const auto& row : rows)
    {
        int id = std::stoi(row.at("id"));
        QString name = QString::fromStdString(row.at("name"));
        QString address = QString::fromStdString(row.at("address"));
        QString desc = QString::fromStdString(row.at("description"));

        QString displayText = name + "\n   📍 " + address;
        if (!desc.isEmpty() && desc != "NULL")
        {
            displayText += "\n   📝 " + desc;
        }

        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, id);
        ui->listWidget_restaurants->addItem(item);
    }
    db.close();
}

// entekhab resturan
void CustomerPanelWindow::on_restaurantSelected(QListWidgetItem *item)
{
    if (!item) return;
    selectedRestaurantId = item->data(Qt::UserRole).toInt();
    ui->pushButton_view_menu->setEnabled(true);
}
//be roz resani namayesh kif pol
void CustomerPanelWindow::updateWalletDisplay()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT walletBalance FROM Customers WHERE id = " + std::to_string(currentCustomerId);
    auto rows = db.fetchAll(sql);

    if (!rows.empty()) {
        double balance = std::stod(rows[0].at("walletBalance"));
        ui->label_balance->setText("💰 Balance: $" + QString::number(balance, 'f', 0));
    } else {
        ui->label_balance->setText("💰 Balance: $0");
    }
    db.close();
}