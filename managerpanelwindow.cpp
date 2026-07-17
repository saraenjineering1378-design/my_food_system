#include "managerpanelwindow.h"
#include "ui_managerpanelwindow.h"
#include "DatabaseManager.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QFont>


ManagerPanelWindow::ManagerPanelWindow(int restId, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagerPanelWindow)
    , currentRestaurantId(restId)
{
    ui->setupUi(this);
    setWindowTitle("Manager Panel - Restaurant ID: " + QString::number(restId));
}

ManagerPanelWindow::~ManagerPanelWindow()
{
    delete ui;
}

// tabe taghir vaziyat sefaresh
void ManagerPanelWindow::changeOrderStatus(int orderId, QString newStatus)
{
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    //barrasi inke aya in sefaresh motealegh be in resturan hast ya na
    std::string checkSql = "SELECT id FROM Orders WHERE id = " + std::to_string(orderId) +
                           " AND restaurantId = " + std::to_string(currentRestaurantId);
    auto checkRows = db.fetchAll(checkSql);

    if (checkRows.empty())
    {
        QMessageBox::warning(this, "Error", "Order not found or does not belong to this restaurant!");
        db.close();
        return;
    }

    int statusInt = 0;
    if (newStatus == "Pending") statusInt = 0;
    else if (newStatus == "Preparing") statusInt = 1;
    else if (newStatus == "Delivered") statusInt = 2;
    else if (newStatus == "Completed") statusInt = 3;
    else if (newStatus == "Cancelled") statusInt = 4;

    std::string updateSql = "UPDATE Orders SET status = " + std::to_string(statusInt) +
                            " WHERE id = " + std::to_string(orderId);

    if (db.executeQuery(updateSql))
    {
        QMessageBox::information(this, "Success", "✅ Order status updated to " + newStatus + "!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Failed to update order status!");
    }

    db.close();
}

//dokme modiriyat menu
void ManagerPanelWindow::on_pushButton_add_edit_remove_items_clicked()
{
    int restId = currentRestaurantId;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    QStringList items;
    items << "1. Add Food Item"
          << "2. Remove Food Item"
          << "3. Update Food Item"
          << "4. Back";

    bool ok2;
    QString selected = QInputDialog::getItem(this, "Manage Menu",
                                             "Select action:",
                                             items, 0, false, &ok2);

    if (!ok2 || selected == "4. Back")
    {
        db.close();
        return;
    }

    // afzodan item
    if (selected == "1. Add Food Item")
    {
        // etelaat omomi
        QString name = QInputDialog::getText(this, "Add Item", "Enter food name:");
        if (name.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Name cannot be empty!");
            db.close();
            return;
        }

        bool ok;
        double price = QInputDialog::getDouble(this, "Add Item", "Enter price:", 0, 0, 1000000, 2, &ok);
        if (!ok || price <= 0)
        {
            QMessageBox::warning(this, "Error", "Price must be greater than 0!");
            db.close();
            return;
        }

        QString desc = QInputDialog::getText(this, "Add Item", "Enter description:");
        if (desc.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Description cannot be empty!");
            db.close();
            return;
        }

        // entekhab noe
        QStringList types = {"Food", "Drink", "Dessert"};
        bool ok3;
        QString type = QInputDialog::getItem(this, "Add Item", "Select type:", types, 0, false, &ok3);
        if (!ok3)
        {
            db.close();
            return;
        }

        int typeNum = (type == "Food") ? 3 : (type == "Drink") ? 2 : 1;

        // joziyat bar asas noe
        int isVegetarian = 0;
        int cookingTime = 0;

        if (type == "Food")
        {
            // zaman pokht va giyahi bodan
            cookingTime = QInputDialog::getInt(this, "Add Food",
                                               "Enter cooking time (minutes):",
                                               20, 1, 120, 1, &ok);
            if (!ok)
            {
                db.close();
                return;
            }

            QStringList vegOptions = {"No", "Yes"};
            QString veg = QInputDialog::getItem(this, "Add Food",
                                                "Is it Vegetarian?",
                                                vegOptions, 0, false, &ok);
            if (!ok)
            {
                db.close();
                return;
            }
            isVegetarian = (veg == "Yes") ? 1 : 0;

            //zakhire dar database
            std::string sql = "INSERT INTO MenuItems (restaurantId, name, description, basePrice, isAvailable, type, isVegetarian) VALUES (" +
                              std::to_string(restId) + ", '" +
                              name.toStdString() + "', '" +
                              desc.toStdString() + "', " +
                              std::to_string(price) + ", 1, " +
                              std::to_string(typeNum) + ", " +
                              std::to_string(isVegetarian) + ")";

            if (db.executeQuery(sql))
            {
                QMessageBox::information(this, "Success", "✅ Food item added successfully!");
            } else
            {
                QMessageBox::warning(this, "Error", "Failed to add item!");
            }
        }
        else if (type == "Drink")
        {
            //hajm/sard garm bodan
            volume = QInputDialog::getDouble(this, "Add Drink",
                                             "Enter volume (ml):",
                                             330, 100, 2000, 1, &ok);
            if (!ok)
            {
                db.close();
                return;
            }

            QStringList coldOptions = {"No", "Yes"};
            QString cold = QInputDialog::getItem(this, "Add Drink",
                                                 "Is it Cold/Ice?",
                                                 coldOptions, 0, false, &ok);
            if (!ok)
            {
                db.close();
                return;
            }
            isCold = (cold == "Yes") ? 1 : 0;

            // zakhire dar database
            std::string sql = "INSERT INTO MenuItems (restaurantId, name, description, basePrice, isAvailable, type) VALUES (" +
                              std::to_string(restId) + ", '" +
                              name.toStdString() + "', '" +
                              desc.toStdString() + "', " +
                              std::to_string(price) + ", 1, " +
                              std::to_string(typeNum) + ")";

            if (db.executeQuery(sql))
            {
                QMessageBox::information(this, "Success", "✅ Drink item added successfully!");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Failed to add item!");
            }
        }
        else if (type == "Dessert")
        {
            // mizan shker
            sugarLevel = QInputDialog::getDouble(this, "Add Dessert",
                                                 "Enter sugar level (%):",
                                                 10, 0, 100, 1, &ok);
            if (!ok)
            {
                db.close();
                return;
            }

            // zakhire dar database
            std::string sql = "INSERT INTO MenuItems (restaurantId, name, description, basePrice, isAvailable, type) VALUES (" +
                              std::to_string(restId) + ", '" +
                              name.toStdString() + "', '" +
                              desc.toStdString() + "', " +
                              std::to_string(price) + ", 1, " +
                              std::to_string(typeNum) + ")";

            if (db.executeQuery(sql))
            {
                QMessageBox::information(this, "Success", "✅ Dessert item added successfully!");
            }
            else
            {
                QMessageBox::warning(this, "Error", "Failed to add item!");
            }
        }
    }

    // hazf item
    else if (selected == "2. Remove Food Item")
    {
        std::string listSql = "SELECT id, name, basePrice FROM MenuItems WHERE restaurantId = " + std::to_string(restId);
        auto rows = db.fetchAll(listSql);

        if (rows.empty())
        {
            QMessageBox::warning(this, "Info", "No items found in this restaurant!");
            db.close();
            return;
        }

        QStringList itemList;
        for (const auto& row : rows)
        {
            itemList << QString::fromStdString(row.at("id")) + " - " +
                            QString::fromStdString(row.at("name")) + " ($" +
                            QString::fromStdString(row.at("basePrice")) + ")";
        }

        bool ok4;
        QString selectedItem = QInputDialog::getItem(this, "Remove Item",
                                                     "Select item to remove:",
                                                     itemList, 0, false, &ok4);
        if (!ok4)
        {
            db.close();
            return;
        }


        int itemId = selectedItem.split(" - ")[0].toInt();

        int reply = QMessageBox::question(this, "Confirm Delete",
                                          "Are you sure you want to delete this item?",
                                          QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
        {
            db.close();
            return;
        }

        std::string deleteSql = "DELETE FROM MenuItems WHERE id = " + std::to_string(itemId);
        if (db.executeQuery(deleteSql))
        {
            QMessageBox::information(this, "Success", "✅ Item removed successfully!");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Failed to remove item!");
        }
    }


    // virayesh item
    else if (selected == "3. Update Food Item")
    {
        std::string listSql = "SELECT id, name, basePrice, type, isVegetarian FROM MenuItems WHERE restaurantId = " + std::to_string(restId);
        auto rows = db.fetchAll(listSql);

        if (rows.empty())
        {
            QMessageBox::warning(this, "Info", "No items found in this restaurant!");
            db.close();
            return;
        }

        QStringList itemList;
        for (const auto& row : rows)
        {
            QString veg = (std::stoi(row.at("isVegetarian")) == 1) ? " (Veg)" : "";
            itemList << QString::fromStdString(row.at("id")) + " - " +
                            QString::fromStdString(row.at("name")) + veg + " ($" +
                            QString::fromStdString(row.at("basePrice")) + ")";
        }

        bool ok5;
        QString selectedItem = QInputDialog::getItem(this, "Update Item",
                                                     "Select item to update:",
                                                     itemList, 0, false, &ok5);
        if (!ok5)
        {
            db.close();
            return;
        }

        int itemId = selectedItem.split(" - ")[0].toInt();

        std::string getSql = "SELECT type, isVegetarian FROM MenuItems WHERE id = " + std::to_string(itemId);
        auto currentRows = db.fetchAll(getSql);
        if (currentRows.empty())
        {
            QMessageBox::warning(this, "Error", "Item not found!");
            db.close();
            return;
        }

        int currentType = std::stoi(currentRows[0].at("type"));
        int currentVeg = std::stoi(currentRows[0].at("isVegetarian"));

        QString newName = QInputDialog::getText(this, "Update Item", "Enter new name (leave empty to keep):");
        bool ok;
        double newPrice = QInputDialog::getDouble(this, "Update Item", "Enter new price (0 to keep):", 0, 0, 1000000, 2, &ok);

        int newVeg = currentVeg;
        if (currentType == 3)
        {
            QStringList vegOptions = {"No", "Yes"};
            bool ok6;
            QString veg = QInputDialog::getItem(this, "Update Item",
                                                "Is it Vegetarian? (current: " +
                                                    QString(currentVeg ? "Yes" : "No") + ")",
                                                vegOptions, currentVeg, false, &ok6);
            if (ok6)
            {
                newVeg = (veg == "Yes") ? 1 : 0;
            }
        }

        if (newName.isEmpty() && !ok && newVeg == currentVeg)
        {
            QMessageBox::information(this, "Info", "No changes made!");
            db.close();
            return;
        }

        std::string updateSql = "UPDATE MenuItems SET ";
        bool first = true;
        if (!newName.isEmpty())
        {
            updateSql += "name = '" + newName.toStdString() + "'";
            first = false;
        }
        if (ok && newPrice > 0)
        {
            if (!first) updateSql += ", ";
            updateSql += "basePrice = " + std::to_string(newPrice);
            first = false;
        }
        if (newVeg != currentVeg)
        {
            if (!first) updateSql += ", ";
            updateSql += "isVegetarian = " + std::to_string(newVeg);
        }
        updateSql += " WHERE id = " + std::to_string(itemId);

        if (db.executeQuery(updateSql))
        {
            QMessageBox::information(this, "Success", "✅ Item updated successfully!");
        } else
        {

            QMessageBox::warning(this, "Error", "Failed to update item!");
        }
    }

    db.close();
}
// dokme moshahede sefareshat
void ManagerPanelWindow::on_pushButton_incomingorders_clicked()
{
    int restId = currentRestaurantId;

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql = "SELECT id, customerId, totalPrice, status, orderDate FROM Orders WHERE restaurantId = " +
                      std::to_string(restId) + " ORDER BY id DESC";
    auto rows = db.fetchAll(sql);

    if (rows.empty())
    {
        QMessageBox::information(this, "Orders", "No orders found for this restaurant!");
        db.close();
        return;
    }

    QString report = "========== INCOMING ORDERS ==========\n\n";

    int orderCount = 0;
    for (const auto& row : rows)
    {
        orderCount++;
        int orderId = std::stoi(row.at("id"));
        int customerId = std::stoi(row.at("customerId"));
        double totalPrice = std::stod(row.at("totalPrice"));
        int statusInt = std::stoi(row.at("status"));  //be int tabdil kon
        QString date = QString::fromStdString(row.at("orderDate"));

        // namayesh vaziyat ha
        QString status;
        if (statusInt == 0) status = "Pending";
        else if (statusInt == 1) status = "Preparing";
        else if (statusInt == 2) status = "Delivered";
        else if (statusInt == 3) status = "Completed";
        else if (statusInt == 4) status = "Cancelled";
        else status = "Unknown";

        report += "📦 Order #" + QString::number(orderId) + "\n";
        report += "   Customer ID: " + QString::number(customerId) + "\n";
        report += "   Total: $" + QString::number(totalPrice, 'f', 0) + "\n";
        report += "   Status: " + status + "\n";
        report += "   Date: " + date + "\n";
        report += "   ---------------------------------\n";
    }

    report += "\nTotal Orders: " + QString::number(orderCount);

    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Incoming Orders - Restaurant " + QString::number(restId));
    dialog->setMinimumSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);

    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setFont(QFont("Courier New", 10));
    textEdit->setText(report);
    textEdit->setReadOnly(true);

    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *btnPending = new QPushButton("Pending", dialog);
    QPushButton *btnPreparing = new QPushButton("Preparing", dialog);
    QPushButton *btnDelivered = new QPushButton("Delivered", dialog);
    QPushButton *btnCompleted = new QPushButton("Completed", dialog);
    QPushButton *btnCancelled = new QPushButton("Cancelled", dialog);
    QPushButton *btnClose = new QPushButton("Close", dialog);

    btnLayout->addWidget(btnPending);
    btnLayout->addWidget(btnPreparing);
    btnLayout->addWidget(btnDelivered);
    btnLayout->addWidget(btnCompleted);
    btnLayout->addWidget(btnCancelled);
    btnLayout->addWidget(btnClose);

    layout->addWidget(textEdit);
    layout->addLayout(btnLayout);

    connect(btnPending, &QPushButton::clicked, [this]()
        {
        bool ok;
        int orderId = QInputDialog::getInt(this, "Change Status", "Enter Order ID:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            changeOrderStatus(orderId, "Pending");
        }
    });
    connect(btnPreparing, &QPushButton::clicked, [this]()
            {
        bool ok;
        int orderId = QInputDialog::getInt(this, "Change Status", "Enter Order ID:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            changeOrderStatus(orderId, "Preparing");
        }
    });
    connect(btnDelivered, &QPushButton::clicked, [this]()
            {
        bool ok;
        int orderId = QInputDialog::getInt(this, "Change Status", "Enter Order ID:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            changeOrderStatus(orderId, "Delivered");
        }
    });
    connect(btnCompleted, &QPushButton::clicked, [this]()
            {
        bool ok;
        int orderId = QInputDialog::getInt(this, "Change Status", "Enter Order ID:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            changeOrderStatus(orderId, "Completed");
        }
    });
    connect(btnCancelled, &QPushButton::clicked, [this]()
            {
        bool ok;
        int orderId = QInputDialog::getInt(this, "Change Status", "Enter Order ID:", 1, 1, 10000, 1, &ok);
        if (ok)
        {
            changeOrderStatus(orderId, "Cancelled");
        }
    });
    connect(btnClose, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->exec();
    db.close();
}

// dokme khoroj
void ManagerPanelWindow::on_pushButton_exit_clicked()
{
    this->close();
    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();
}
void ManagerPanelWindow::on_pushButton_edit_res_info_clicked()
{
    int restId = currentRestaurantId;  // estefade az id zakhire shode

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    //daryaft etelaat feli resturan
    std::string sql = "SELECT name, address, phoneNumber, description, estimatedPrepTime FROM Restaurants WHERE id = " + std::to_string(restId);
    auto rows = db.fetchAll(sql);

    if (rows.empty())
    {
        QMessageBox::warning(this, "Error", "Restaurant not found!");
        db.close();
        return;
    }

    auto& row = rows[0];
    QString currentName = QString::fromStdString(row.at("name"));
    QString currentAddress = QString::fromStdString(row.at("address"));
    QString currentPhone = QString::fromStdString(row.at("phoneNumber"));
    QString currentDesc = QString::fromStdString(row.at("description"));
    int currentPrep = std::stoi(row.at("estimatedPrepTime"));

    //namayesh menu virayesh
    QStringList items;
    items << "1. Name: " + currentName
          << "2. Address: " + currentAddress
          << "3. Phone: " + currentPhone
          << "4. Description: " + currentDesc
          << "5. Prep Time: " + QString::number(currentPrep) + " mins"
          << "6. Back";

    bool ok;
    QString selected = QInputDialog::getItem(this, "Edit Restaurant Info",
                                             "Select field to update:",
                                             items, 0, false, &ok);

    if (!ok || selected == "6. Back")
    {
        db.close();
        return;
    }

    QString newValue;
    std::string updateSql;

    if (selected.startsWith("1."))
    {
        newValue = QInputDialog::getText(this, "Update Name", "Enter new name:",
                                         QLineEdit::Normal, currentName);
        if (newValue.isEmpty() || newValue == currentName)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET name = '" + newValue.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("2."))
    {
        newValue = QInputDialog::getText(this, "Update Address", "Enter new address:",
                                         QLineEdit::Normal, currentAddress);
        if (newValue.isEmpty() || newValue == currentAddress)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET address = '" + newValue.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("3."))
    {
        newValue = QInputDialog::getText(this, "Update Phone", "Enter new phone:",
                                         QLineEdit::Normal, currentPhone);
        if (newValue.isEmpty() || newValue == currentPhone)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET phoneNumber = '" + newValue.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("4."))
    {
        newValue = QInputDialog::getText(this, "Update Description", "Enter new description:",
                                         QLineEdit::Normal, currentDesc);
        if (newValue == currentDesc)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET description = '" + newValue.toStdString() +
                    "' WHERE id = " + std::to_string(restId);
    }
    else if (selected.startsWith("5."))
    {
        int newPrep = QInputDialog::getInt(this, "Update Prep Time",
                                           "Enter new preparation time (minutes):",
                                           currentPrep, 1, 120, 1);
        if (newPrep == currentPrep)
        {
            db.close();
            return;
        }
        updateSql = "UPDATE Restaurants SET estimatedPrepTime = " + std::to_string(newPrep) +
                    " WHERE id = " + std::to_string(restId);
    }

    if (db.executeQuery(updateSql))
    {
        QMessageBox::information(this, "Success", "✅ Restaurant info updated successfully!");
    }
    else
    {
        QMessageBox::warning(this, "Error", "Update failed!");
    }

    db.close();
}

