#include "adminpanelwindow.h"
#include "ui_adminpanelwindow.h"
#include "DatabaseManager.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QDebug>
#include <stdexcept>
#include <QTextEdit>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>

AdminPanelWindow::AdminPanelWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminPanelWindow)
{
    ui->setupUi(this);
}

AdminPanelWindow::~AdminPanelWindow()
{
    delete ui;
}

// gozaresh tedad karbaran bar asa sath
void AdminPanelWindow::on_pushButton_users_by_level_clicked()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT membershipLevel, COUNT(*) as count FROM Customers GROUP BY membershipLevel";
    auto rows = db.fetchAll(sql);

    QString report = "========== USERS BY LEVEL ==========\n\n";

    int total = 0;
    for (const auto& row : rows) {
        QString level = QString::fromStdString(row.at("membershipLevel"));
        int count = std::stoi(row.at("count"));
        total += count;

        QString emoji;
        if (level == "Normal") emoji = "👤";
        else if (level == "Silver") emoji = "🥈";
        else if (level == "Gold") emoji = "🥇";
        else if (level == "VIP") emoji = "👑";

        report += emoji + " " + level + ": " + QString::number(count) + " users\n";
    }
    report += "\nTotal Users: " + QString::number(total);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Users by Level");
    msgBox.setText(report);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    db.close();
}

//moshahede tarikhche taghirat sath yek karbar
void AdminPanelWindow::on_pushButton_level_history_clicked()
{
    bool ok;
    int customerId = QInputDialog::getInt(this, "Level History",
                                          "Enter Customer ID:", 1, 1, 1000, 1, &ok);
    if (!ok) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT * FROM MembershipHistory WHERE customerId = " + std::to_string(customerId) +
                      " ORDER BY changeDate DESC";
    auto rows = db.fetchAll(sql);

    QString report = "========== LEVEL CHANGE HISTORY ==========\n";
    report += "Customer ID: " + QString::number(customerId) + "\n\n";

    if (rows.empty()) {
        report += "No history found for this user.";
    } else {
        for (const auto& row : rows)
        {
            report += "📅 " + QString::fromStdString(row.at("changeDate")) + "\n";
            report += "   " + QString::fromStdString(row.at("oldLevel")) + " → " +
                      QString::fromStdString(row.at("newLevel")) + "\n";
            report += "   Reason: " + QString::fromStdString(row.at("reason")) + "\n";
            report += "   ---------------------------------\n";
        }
    }

    QMessageBox msgBox;
    msgBox.setWindowTitle("Level History");
    msgBox.setText(report);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();

    db.close();
}

//taghir dasti sath va emtiyaz tavasot addmin
void AdminPanelWindow::on_pushButton_edit_level_clicked()
{
    bool ok;
    int customerId = QInputDialog::getInt(this, "Edit Level/Points",
                                          "Enter Customer ID:", 1, 1, 1000, 1, &ok);
    if (!ok) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    //daryaft etelaat feli kar bar
    std::string getSql = "SELECT name, membershipLevel, loyaltyPoints FROM Customers WHERE id = " + std::to_string(customerId);
    auto rows = db.fetchAll(getSql);

    if (rows.empty())
    {
        QMessageBox::warning(this, "Error", "Customer not found!");
        db.close();
        return;
    }

    QString customerName = QString::fromStdString(rows[0].at("name"));
    QString currentLevel = QString::fromStdString(rows[0].at("membershipLevel"));
    int currentPoints = std::stoi(rows[0].at("loyaltyPoints"));

    //namayesh etelaat feli va entekhab amal
    QStringList actions = {"Change Level", "Change Points", "Downgrade Level"};
    QString action = QInputDialog::getItem(this, "Edit Level/Points",
                                           "Customer: " + customerName +
                                               "\nCurrent Level: " + currentLevel +
                                               "\nCurrent Points: " + QString::number(currentPoints) +
                                               "\n\nSelect action:",
                                           actions, 0, false, &ok);
    if (!ok)
    {
        db.close();
        return;
    }

    QString oldLevel = currentLevel;

    // taghir sath
    if (action == "Change Level")
    {
        QStringList levels = {"Normal", "Silver", "Gold", "VIP"};
        QString newLevel = QInputDialog::getItem(this, "Change Level",
                                                 "Select new level for " + customerName + ":",
                                                 levels, 0, false, &ok);
        if (!ok)
        {
            db.close();
            return;
        }

        if (newLevel == currentLevel)
        {
            QMessageBox::information(this, "Info", "Level is already " + currentLevel);
            db.close();
            return;
        }

        QString reason = QInputDialog::getText(this, "Reason",
                                               "Enter reason for changing " + customerName +
                                                   "\nfrom " + currentLevel + " to " + newLevel + ":");
        if (reason.isEmpty()) reason = "Admin manual change";

        //be roz resani sath
        std::string updateSql = "UPDATE Customers SET membershipLevel = '" + newLevel.toStdString() +
                                "' WHERE id = " + std::to_string(customerId);
        db.executeQuery(updateSql);

        // sabt tarikhche
        std::string historySql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                                 std::to_string(customerId) + ", '" +
                                 oldLevel.toStdString() + "', '" +
                                 newLevel.toStdString() + "', datetime('now'), '" +
                                 reason.toStdString() + "')";
        db.executeQuery(historySql);

        QMessageBox::information(this, "Success",
                                 "✅ Level updated successfully!\n\n" +
                                     customerName + ": " + oldLevel + " → " + newLevel +
                                     "\nReason: " + reason);
    }

    // taghir emtiyaz
    else if (action == "Change Points")
    {
        int newPoints = QInputDialog::getInt(this, "Change Points",
                                             "Enter new points for " + customerName +
                                                 "\n(Current: " + QString::number(currentPoints) + "):",
                                             currentPoints, 0, 10000, 1, &ok);
        if (!ok)
        {
            db.close();
            return;
        }

        if (newPoints == currentPoints)
        {
            QMessageBox::information(this, "Info", "Points are already " + QString::number(currentPoints));
            db.close();
            return;
        }

        QString reason = QInputDialog::getText(this, "Reason",
                                               "Enter reason for changing points:");
        if (reason.isEmpty()) reason = "Admin points change";

        // be roz resani emtiyaz
        std::string updateSql = "UPDATE Customers SET loyaltyPoints = " + std::to_string(newPoints) +
                                " WHERE id = " + std::to_string(customerId);
        db.executeQuery(updateSql);

        // be roz resani sath bar asa emtiyaz jadid
        int silverThreshold = 100;
        int goldThreshold = 300;
        int vipThreshold = 700;

        std::string newLevel = currentLevel.toStdString();

        if (newPoints >= vipThreshold)
        {
            newLevel = "VIP";
        }
        else if (newPoints >= goldThreshold)
        {
            newLevel = "Gold";
        }
        else if (newPoints >= silverThreshold)
        {
            newLevel = "Silver";
        }
        else
        {
            newLevel = "Normal";
        }

        //sabt tarikhche taghir emtiyaz
        std::string historySql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                                 std::to_string(customerId) + ", '" +
                                 currentLevel.toStdString() + "', '" +
                                 newLevel + "', datetime('now'), 'Admin points change')";
        db.executeQuery(historySql);

        //age sath taghir karde be roz resani kon
        if (newLevel != currentLevel.toStdString())
        {
            std::string updateLevelSql = "UPDATE Customers SET membershipLevel = '" + newLevel +
                                         "' WHERE id = " + std::to_string(customerId);
            db.executeQuery(updateLevelSql);

            QMessageBox::information(this, "Level Changed",
                                     "🔄 Level automatically changed due to points update.\n\n" +
                                         customerName + ": " + currentLevel + " → " +
                                         QString::fromStdString(newLevel) +
                                         "\nPoints: " + QString::number(newPoints));
        }
        else
        {
            QMessageBox::information(this, "Info",
                                     "Level remains " + currentLevel +
                                         "\nPoints: " + QString::number(newPoints));
        }

        QMessageBox::information(this, "Success",
                                 "✅ Points updated successfully!\n\n" +
                                     customerName + ": " + QString::number(currentPoints) +
                                     " → " + QString::number(newPoints) +
                                     "\nLevel: " + QString::fromStdString(newLevel));
    }
    // tanazol sath
    else if (action == "Downgrade Level")
    {
        QStringList levels;
        if (currentLevel == "VIP") levels = {"Gold", "Silver", "Normal"};
        else if (currentLevel == "Gold") levels = {"Silver", "Normal"};
        else if (currentLevel == "Silver") levels = {"Normal"};
        else {
            QMessageBox::warning(this, "Error", "Normal level cannot be downgraded!");
            db.close();
            return;
        }

        QString newLevel = QInputDialog::getItem(this, "Downgrade Level",
                                                 "Select new level for " + customerName + ":",
                                                 levels, 0, false, &ok);
        if (!ok) { db.close(); return; }

        QString reason = QInputDialog::getText(this, "Reason",
                                               "Enter reason for downgrading " + customerName +
                                                   "\nfrom " + currentLevel + " to " + newLevel + ":");
        if (reason.isEmpty()) reason = "Admin manual downgrade";

        // be roz resani sath
        std::string updateSql = "UPDATE Customers SET membershipLevel = '" + newLevel.toStdString() +
                                "' WHERE id = " + std::to_string(customerId);
        db.executeQuery(updateSql);

        //sabt tarikhche
        std::string historySql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                                 std::to_string(customerId) + ", '" +
                                 oldLevel.toStdString() + "', '" +
                                 newLevel.toStdString() + "', datetime('now'), '" +
                                 reason.toStdString() + "')";
        db.executeQuery(historySql);

        QMessageBox::information(this, "Success",
                                 "✅ Downgrade completed!\n\n" +
                                     customerName + ": " + oldLevel + " → " + newLevel +
                                     "\nReason: " + reason);
    }

    db.close();
}
//khoroj
void AdminPanelWindow::on_pushButton_exit_clicked()
{
    this->close();
    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();
}
void AdminPanelWindow::on_pushButton_active_deactive_clicked()
{
    bool ok;
    int restId = QInputDialog::getInt(this, "Toggle Status",
                                      "Enter Restaurant ID:",
                                      1, 1, 1000, 1, &ok);
    if (!ok) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    // peyda kardan resturan
    auto rows = db.fetchAll("SELECT id, name, isActive FROM Restaurants WHERE id = " + std::to_string(restId));
    if (rows.empty())
    {
        QMessageBox::warning(this, "Error", "Restaurant not found!");
        db.close();
        return;
    }

    QString name = QString::fromStdString(rows[0].at("name"));
    int isActive = std::stoi(rows[0].at("isActive"));
    QString status = isActive ? "Active" : "Inactive";

    // taeid az karbar
    int reply = QMessageBox::question(this, "Toggle Status",
                                      "Restaurant: " + name + "\nCurrent Status: " + status +
                                          "\n\nDo you want to change status?",
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        db.close();
        return;
    }

    // taghir vaziyat
    int newStatus = isActive ? 0 : 1;
    std::string sql = "UPDATE Restaurants SET isActive = " + std::to_string(newStatus) +
                      " WHERE id = " + std::to_string(restId);

    if (db.executeQuery(sql))
    {
        QMessageBox::information(this, "Success",
                                 "✅ Status changed to " + QString(newStatus ? "Active" : "Inactive"));
    } else
    {
        QMessageBox::warning(this, "Error", "Failed to update!");
    }

    db.close();
}


void AdminPanelWindow::on_pushButton_add_new_res_clicked()
{
    bool ok;

    QString name = QInputDialog::getText(this, "Add Restaurant",
                                         "Enter restaurant name:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty()) return;

    QString address = QInputDialog::getText(this, "Add Restaurant",
                                            "Enter address:",
                                            QLineEdit::Normal, "", &ok);
    if (!ok || address.isEmpty()) return;

    QString phone = QInputDialog::getText(this, "Add Restaurant",
                                          "Enter phone:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || phone.isEmpty()) return;

    QString desc = QInputDialog::getText(this, "Add Restaurant",
                                         "Enter description:",
                                         QLineEdit::Normal, "", &ok);

    int prepTime = QInputDialog::getInt(this, "Add Restaurant",
                                        "Enter prep time (minutes):",
                                        30, 1, 120, 1, &ok);
    if (!ok) return;

    QString password = QInputDialog::getText(this, "Add Restaurant",
                                             "Enter manager password:",
                                             QLineEdit::Normal, "1234", &ok);
    if (!ok || password.isEmpty()) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "INSERT INTO Restaurants (name, address, phoneNumber, description, estimatedPrepTime, password, isActive) VALUES ('" +
                      name.toStdString() + "', '" + address.toStdString() + "', '" +
                      phone.toStdString() + "', '" + desc.toStdString() + "', " +
                      std::to_string(prepTime) + ", '" + password.toStdString() + "', 1)";

    if (db.executeQuery(sql))
    {
        int newId = db.getLastInsertId();
        QMessageBox::information(this, "Success",
                                 "✅ Restaurant added!\nID: " + QString::number(newId));
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to add restaurant!");
    }

    db.close();
}

void AdminPanelWindow::on_pushButton_view_reports_clicked()
{
    qDebug() << "View Reports button clicked!";

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    qDebug() << "Database opened!";

    try
    {
        auto restRows = db.fetchAll("SELECT COUNT(*) FROM Restaurants");
        int totalRest = std::stoi(restRows[0].at("COUNT(*)"));

        auto activeRows = db.fetchAll("SELECT COUNT(*) FROM Restaurants WHERE isActive = 1");
        int activeRest = std::stoi(activeRows[0].at("COUNT(*)"));

        auto custRows = db.fetchAll("SELECT COUNT(*) FROM Customers");
        int totalCust = std::stoi(custRows[0].at("COUNT(*)"));

        auto orderRows = db.fetchAll("SELECT COUNT(*), COALESCE(SUM(totalPrice), 0) FROM Orders");
        int totalOrders = std::stoi(orderRows[0].at("COUNT(*)"));
        double totalSales = std::stod(orderRows[0].at("COALESCE(SUM(totalPrice), 0)"));

        // daryaft tarikhche sefareshat
        std::string orderSql = "SELECT id, customerId, restaurantId, totalPrice, orderDate FROM Orders ORDER BY id DESC";
        auto orderRows2 = db.fetchAll(orderSql);

        // sakht gozaresh
        QString report;
        report += "========== SYSTEM REPORT ==========\n\n";
        report += "Total Restaurants: " + QString::number(totalRest) + "\n";
        report += "Active Restaurants: " + QString::number(activeRest) + "\n";
        report += "Inactive Restaurants: " + QString::number(totalRest - activeRest) + "\n";
        report += "-----------------------------------\n";
        report += "Total Orders Placed: " + QString::number(totalOrders) + "\n";
        report += "Total Sales Amount: " + QString::number(totalSales, 'f', 0) + " Toman\n";
        report += "-----------------------------------\n";
        report += "       🧾 ALL ORDERS DETAILS 🧾\n";
        report += "-----------------------------------\n";
        report += "Order ID | Customer ID | Restaurant ID | Total Price\n";
        report += "-----------------------------------\n";

        if (orderRows2.empty())
        {
            report += "No orders found.\n";
        }
        else
        {
            for (const auto& row : orderRows2)
            {
                int orderId = std::stoi(row.at("id"));
                int customerId = std::stoi(row.at("customerId"));
                int restaurantId = std::stoi(row.at("restaurantId"));
                double totalPrice = std::stod(row.at("totalPrice"));

                report += "   " + QString::number(orderId) + "    |     " +
                          QString::number(customerId) + "     |       " +
                          QString::number(restaurantId) + "       |  " +
                          QString::number(totalPrice, 'f', 0) + " Toman\n";
            }
        }

        report += "===================================\n";

        qDebug() << "Report created!";

        // namayesh QDialog ,ba ghabeliyat scroll
        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("System Report");
        dialog->setModal(true);
        dialog->resize(550, 500);

        QTextEdit* textEdit = new QTextEdit(dialog);
        textEdit->setPlainText(report);
        textEdit->setReadOnly(true);
        textEdit->setFont(QFont("Courier New", 10));
        textEdit->setLineWrapMode(QTextEdit::NoWrap);

        QPushButton* closeButton = new QPushButton("Close", dialog);
        closeButton->setFixedWidth(100);
        QObject::connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

        QVBoxLayout* layout = new QVBoxLayout(dialog);
        layout->addWidget(textEdit);
        layout->addWidget(closeButton, 0, Qt::AlignCenter);

        dialog->exec();

    }
    catch (const std::exception& e)
    {
        qDebug() << "Error: " << e.what();
        QMessageBox::warning(this, "Error", "Failed to generate report: " + QString::fromStdString(e.what()));
    }

    db.close();

    qDebug() << "Function ended!";
}
void AdminPanelWindow::on_pushButton_update_info_clicked()
{
    bool ok;
    int restId = QInputDialog::getInt(this, "Update Restaurant Info",
                                      "Enter Restaurant ID to update:",
                                      1, 1, 1000, 1, &ok);
    if (!ok) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    //daryaft etaat feli resturan
    auto rows = db.fetchAll("SELECT name, address, phoneNumber, description, estimatedPrepTime FROM Restaurants WHERE id = " + std::to_string(restId));

    if (rows.empty())
    {
        QMessageBox::warning(this, "Error", "Restaurant not found!");
        db.close();
        return;
    }

    QString currentName = QString::fromStdString(rows[0].at("name"));
    QString currentAddress = QString::fromStdString(rows[0].at("address"));
    QString currentPhone = QString::fromStdString(rows[0].at("phoneNumber"));
    QString currentDesc = QString::fromStdString(rows[0].at("description"));
    int currentPrep = std::stoi(rows[0].at("estimatedPrepTime"));

    //entekhab field baray virayesh
    QStringList fields =
        {
        "1. Name: " + currentName,
        "2. Address: " + currentAddress,
        "3. Phone: " + currentPhone,
        "4. Description: " + currentDesc,
        "5. Prep Time: " + QString::number(currentPrep) + " mins",
        "6. Cancel"
    };

    QString selected = QInputDialog::getItem(this, "Update Restaurant Info",
                                             "Select field to update:",
                                             fields, 0, false, &ok);

    if (!ok || selected == "6. Cancel")
    {
        db.close();
        return;
    }

    std::string updateSql;

    if (selected.startsWith("1."))
    {
        QString newName = QInputDialog::getText(this, "Update Name",
                                                "Enter new name:",
                                                QLineEdit::Normal, currentName, &ok);
        if (!ok || newName.isEmpty() || newName == currentName)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET name = '" + newName.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("2."))
    {
        QString newAddress = QInputDialog::getText(this, "Update Address",
                                                   "Enter new address:",
                                                   QLineEdit::Normal, currentAddress, &ok);
        if (!ok || newAddress.isEmpty() || newAddress == currentAddress)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET address = '" + newAddress.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("3."))
    {
        QString newPhone = QInputDialog::getText(this, "Update Phone",
                                                 "Enter new phone:",
                                                 QLineEdit::Normal, currentPhone, &ok);
        if (!ok || newPhone.isEmpty() || newPhone == currentPhone)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET phoneNumber = '" + newPhone.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("4."))
    {
        QString newDesc = QInputDialog::getText(this, "Update Description",
                                                "Enter new description:",
                                                QLineEdit::Normal, currentDesc, &ok);
        if (!ok || newDesc == currentDesc)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET description = '" + newDesc.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("5."))
    {
        int newPrep = QInputDialog::getInt(this, "Update Prep Time",
                                           "Enter new prep time (minutes):",
                                           currentPrep, 1, 120, 1, &ok);
        if (!ok || newPrep == currentPrep)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET estimatedPrepTime = " + std::to_string(newPrep) +
                    " WHERE id = " + std::to_string(restId);
    }

    if (!updateSql.empty() && db.executeQuery(updateSql))
    {
        QMessageBox::information(this, "Success", "✅ Restaurant info updated successfully!");
    } else if (!updateSql.empty())
    {
        QMessageBox::warning(this, "Error", "Failed to update!");
    }

    db.close();
}




// faal sazi copoun haye mahane(reset,takhsis mojada)
void AdminPanelWindow::on_pushButton_activate_monthly_coupons_clicked()
{
    int confirm = QMessageBox::question(this, "Activate Monthly Coupons",
                                        "⚠️ This will:\n"
                                        "1. Reset ALL existing coupons to 0\n"
                                        "2. Assign new coupons based on user levels\n"
                                        "   • Silver: 1 coupon\n"
                                        "   • Gold: 1 coupon\n"
                                        "   • VIP: 3 coupons\n"
                                        "   • Normal: 0 coupons\n\n"
                                        "Are you sure you want to continue?",
                                        QMessageBox::Yes | QMessageBox::No);

    if (confirm == QMessageBox::No) return;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    // daryaft tarikh emroz
    time_t now = time(nullptr);
    struct tm* localTime = localtime(&now);
    char currentDate[20];
    strftime(currentDate, sizeof(currentDate), "%Y-%m-%d %H:%M:%S", localTime);
    QString today = QString::fromStdString(currentDate);

    // reset hame copoun ha be 0
    std::string resetSql = "UPDATE Customers SET monthlyCoupons = 0";
    if (!db.executeQuery(resetSql))
    {
        QMessageBox::warning(this, "Error", "Failed to reset coupons!");
        db.close();
        return;
    }

    // daryaft hame karbaran
    std::string sql = "SELECT id, name, membershipLevel FROM Customers";
    auto rows = db.fetchAll(sql);

    int updatedCount = 0;
    QString report = "========== MONTHLY COUPONS ACTIVATED ==========\n\n";
    report += "📅 Date: " + today + "\n\n";

    for (const auto& row : rows)
    {
        int id = std::stoi(row.at("id"));
        QString name = QString::fromStdString(row.at("name"));
        QString level = QString::fromStdString(row.at("membershipLevel"));

        int newCoupons = 0;
        QString levelName;

        if (level == "Silver")
        {
            newCoupons = 1;
            levelName = "🥈 Silver";
        } else if (level == "Gold")
        {
            newCoupons = 1;
            levelName = "🥇 Gold";
        } else if (level == "VIP")
        {
            newCoupons = 3;
            levelName = "👑 VIP";
        } else {
            newCoupons = 0;
            levelName = "👤 Normal";
        }

        if (newCoupons > 0)
        {
            std::string updateSql = "UPDATE Customers SET monthlyCoupons = " + std::to_string(newCoupons) +
                                    " WHERE id = " + std::to_string(id);
            db.executeQuery(updateSql);
            updatedCount++;

            report += "✅ " + name + " (ID: " + QString::number(id) + ") | " +
                      levelName + " | 🎫 +" + QString::number(newCoupons) + " coupons\n";
        }
        else
        {
            report += "❌ " + name + " (ID: " + QString::number(id) + ") | " +
                      levelName + " | 🎫 0 coupons\n";
        }
    }

    report += "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    report += "📊 Total users updated: " + QString::number(updatedCount);
    report += "\n✅ All coupons reset and re-assigned successfully!";

    // zalhire dar MembershipHistory
    if (updatedCount > 0) {
        std::string historySql = "INSERT INTO MembershipHistory (customerId, oldLevel, newLevel, changeDate, reason) VALUES (" +
                                 std::to_string(0) + ", 'System', 'Coupon Reset', datetime('now'), " +
                                 "'Monthly coupons activated for " + std::to_string(updatedCount) + " users')";
        db.executeQuery(historySql);
    }

    db.close();

    QMessageBox::information(this, "Monthly Coupons Activated", report);
}

// moshahede tarikhche faal sazi copoun
void AdminPanelWindow::on_pushButton_coupon_activation_history_clicked()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    //daryaft tarikhche az MembershipHistory
    std::string sql = "SELECT changeDate, reason FROM MembershipHistory WHERE customerId = 0 AND oldLevel = 'System' ORDER BY changeDate DESC";
    auto rows = db.fetchAll(sql);

    QString report = "========== COUPON ACTIVATION HISTORY ==========\n\n";

    if (rows.empty()) {
        report += "📭 No activation history found yet.\n\n";
        report += "💡 Click 'Activate Monthly Coupons' to start logging history.";
    }
    else
    {
        int count = 0;
        for (const auto& row : rows)
        {
            count++;
            QString date = QString::fromStdString(row.at("changeDate"));
            QString reason = QString::fromStdString(row.at("reason"));
            report += "📅 " + date + "\n";
            report += "   " + reason + "\n";
            report += "   -------------------------\n";
        }
        report += "\n📊 Total activations: " + QString::number(count);
    }

    db.close();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Activation History");
    msgBox.setText(report);
    msgBox.setStyleSheet("QMessageBox { min-width: 500px; }");
    msgBox.exec();
}
// namayesh vaziyat copoun haye karbaran
void AdminPanelWindow::on_pushButton_view_coupon_status_clicked()
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");
    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT id, name, membershipLevel, monthlyCoupons FROM Customers ORDER BY membershipLevel DESC";
    auto rows = db.fetchAll(sql);

    QString report = "========== CURRENT COUPON STATUS ==========\n\n";

    int totalCoupons = 0;
    int silverCount = 0, goldCount = 0, vipCount = 0, normalCount = 0;

    for (const auto& row : rows)
    {
        int id = std::stoi(row.at("id"));
        QString name = QString::fromStdString(row.at("name"));
        QString level = QString::fromStdString(row.at("membershipLevel"));
        int coupons = std::stoi(row.at("monthlyCoupons"));
        totalCoupons += coupons;

        QString emoji;
        if (level == "VIP")
        {
            emoji = "👑";
            vipCount++;
        }
        else if (level == "Gold")
        {
            emoji = "🥇";
            goldCount++;
        }
        else if (level == "Silver")
        {
            emoji = "🥈";
            silverCount++;
        }
        else
        {
            emoji = "👤";
            normalCount++;
        }

        report += emoji + " ID:" + QString::number(id) + " | " + name +
                  " | " + level + " | 🎫 " + QString::number(coupons) + "\n";
    }

    report += "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    report += "📊 Summary:\n";
    report += "   👑 VIP: " + QString::number(vipCount) + " users\n";
    report += "   🥇 Gold: " + QString::number(goldCount) + " users\n";
    report += "   🥈 Silver: " + QString::number(silverCount) + " users\n";
    report += "   👤 Normal: " + QString::number(normalCount) + " users\n";
    report += "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    report += "🎫 Total coupons in system: " + QString::number(totalCoupons);

    db.close();

    QMessageBox::information(this, "Coupon Status", report);
}

