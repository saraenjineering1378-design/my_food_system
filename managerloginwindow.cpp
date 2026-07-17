#include "managerloginwindow.h"
#include "ui_managerloginwindow.h"
#include "managerpanelwindow.h"
#include <QMessageBox>
#include "DatabaseManager.h"
#include "mainwindow.h"

ManagerLoginWindow::ManagerLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ManagerLoginWindow)
{
    ui->setupUi(this);
}

ManagerLoginWindow::~ManagerLoginWindow()
{
    delete ui;
}

void ManagerLoginWindow::on_pushButton_login_clicked()
{
    QString restIdStr = ui->lineEdit_restId->text();
    QString password = ui->lineEdit_password->text();

    if (restIdStr.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter Restaurant ID and Password!");
        return;
    }

    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open())
    {
        QMessageBox::critical(this, "Error", "Could not open database!");
        return;
    }

    int restId = restIdStr.toInt();
    std::string sql = "SELECT * FROM Restaurants WHERE id = " + std::to_string(restId) +
                      " AND password = '" + password.toStdString() + "'";

    auto rows = db.fetchAll(sql);

    if (!rows.empty()) {
        auto& row = rows[0];
        QString name = QString::fromStdString(row["name"]);
        QMessageBox::information(this, "Success", "✅ Login Successful! Welcome " + name + " manager.");

        this->close();

        //restid be panel modir ersal shod
        ManagerPanelWindow *panel = new ManagerPanelWindow(restId);
        panel->show();
    } else
    {
        QMessageBox::warning(this, "Error", "❌ Invalid Restaurant ID or Password!");
    }

    db.close();
}
void ManagerLoginWindow::on_pushButton_exit_clicked()
{
    this->close();
    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();
}