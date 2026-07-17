#include "adminloginwindow.h"
#include "ui_adminloginwindow.h"
#include "adminpanelwindow.h"
#include <QMessageBox>
#include "mainwindow.h"

AdminLoginWindow::AdminLoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminLoginWindow)
{
    ui->setupUi(this);
}

AdminLoginWindow::~AdminLoginWindow()
{
    delete ui;
}

void AdminLoginWindow::on_pushButton_login_clicked()
{
    QString password = ui->lineEdit_password->text();

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter Admin Password!");
        return;
    }

    if (password == "admin123")
    {
        QMessageBox::information(this, "Success", "✅ Access Granted! Welcome Admin.");
        this->close();
        AdminPanelWindow *panel = new AdminPanelWindow();
        panel->show();
    } else {
        QMessageBox::warning(this, "Error", "❌ Access Denied! Wrong password.");
    }
}

void AdminLoginWindow::on_pushButton_exit_clicked()
{
    this->close();
    MainWindow *mainMenu = new MainWindow();
    mainMenu->show();
}