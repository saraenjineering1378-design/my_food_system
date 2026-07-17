#include "customerloginwindow.h"
#include "ui_customerloginwindow.h"
#include "customerpanelwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "DatabaseManager.h"
#include "DatabaseInitializer.h"

CustomerLoginWindow::CustomerLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomerLoginWindow)
{
    ui->setupUi(this);
}

CustomerLoginWindow::~CustomerLoginWindow()
{
    delete ui;
}

// dokme login
void CustomerLoginWindow::on_pushButton_login_clicked()
{
    QString idStr = ui->lineEdit_id->text();
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    //check kardan hadaghal  id ya username por shode bashe
    if (idStr.isEmpty() && username.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter Customer ID or Username!");
        return;
    }

    if (password.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter Password!");
        return;
    }

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    std::string sql;
    bool found = false;
    int customerId = 0;
    QString name;

    //aval ba id jostojo kon agar ba id vared shode
    if (!idStr.isEmpty()) {
        bool ok;
        int id = idStr.toInt(&ok);
        if (ok && id > 0) {
            sql = "SELECT id, name FROM Customers WHERE id = " + std::to_string(id) +
                  " AND password = '" + password.toStdString() + "'";
            auto rows = db.fetchAll(sql);
            if (!rows.empty())
            {
                found = true;
                customerId = std::stoi(rows[0].at("id"));
                name = QString::fromStdString(rows[0].at("name"));
            }
        }
    }

    // agar ba id vared shode peyda nashod ba username begard
    if (!found && !username.isEmpty())
    {
        sql = "SELECT id, name FROM Customers WHERE UPPER(name) = '" +
              username.toUpper().toStdString() + "' AND password = '" +
              password.toStdString() + "'";
        auto rows = db.fetchAll(sql);
        if (!rows.empty())
        {
            found = true;
            customerId = std::stoi(rows[0].at("id"));
            name = QString::fromStdString(rows[0].at("name"));
        }
    }

    if (found)
    {
        QMessageBox::information(this, "Success",
                                 "🎉 Login movafaghiyat amiz bood! Khosh amadid, " + name + "! 🎉");

        this->close();
        CustomerPanelWindow *panel = new CustomerPanelWindow(customerId);
        panel->show();
    } else
    {
        QMessageBox::warning(this, "Error", "❌ Khata: ID/Name ya Password eshtebah ast!");
    }

    db.close();
}

//dokme signup
void CustomerLoginWindow::on_pushButton_signup_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString idStr = ui->lineEdit_id->text();

    if (!idStr.isEmpty())
    {
        QMessageBox::warning(this, "Warning",
                             "⚠️ For Sign Up, please do NOT enter Customer ID.\n"
                             "Customer ID will be generated automatically.");
        return;
    }

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter Username and Password!");
        return;
    }


    QString dbPath = "C:/Users/User/Documents/food_system/food_system.db";
    DatabaseManager db(dbPath.toStdString());

    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    // sakht jadavel agar vojod nadaran
    DatabaseInitializer::initialize(db);

    // barrasi tekrari nabodan name
    std::string checkSql = "SELECT * FROM Customers WHERE UPPER(name) = '" +
                           username.toUpper().toStdString() + "'";
    auto checkRows = db.fetchAll(checkSql);

    if (!checkRows.empty())
    {
        QMessageBox::warning(this, "Error", "❌ Username already exists!");
        db.close();
        return;
    }

    //sabt nam
    std::string sql = "INSERT INTO Customers (name, walletBalance, password, loyaltyPoints, membershipLevel, upgradeMessageShown) VALUES ('" +
                      username.toUpper().toStdString() + "', 0, '" +
                      password.toStdString() + "', 0, 'Normal', 0)";

    if (db.executeQuery(sql))
    {
        int newId = db.getLastInsertId();
        QMessageBox::information(this, "Success",
                                 "✅ Sabt nam anjam shod! ID-e shoma: " + QString::number(newId) + "\nYaddasht konid! 📝");
        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_id->clear();
    }
    else
    {
        QMessageBox::warning(this, "Error", "❌ Registration failed!");
    }

    db.close();
}

// dokme exit
void CustomerLoginWindow::on_pushButton_exit_clicked()
{
    this->close();
}
void CustomerLoginWindow::on_pushButton_backmenu_clicked()
{
    this->close();// bastan panjare feli

    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();  //namyesh menue asli
}
