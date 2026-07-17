#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customerloginwindow.h"
#include <QMessageBox>
#include "managerloginwindow.h"
#include "adminloginwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_customer_clicked()
{
    this->hide();
    CustomerLoginWindow *loginWindow = new CustomerLoginWindow(this);
    loginWindow->show();
}

void MainWindow::on_pushButton_manager_clicked()
{
    this->hide();
    ManagerLoginWindow *managerLogin = new ManagerLoginWindow(this);
    managerLogin->show();
}

void MainWindow::on_pushButton_admin_clicked()
{
    this->hide();
    AdminLoginWindow *adminLogin = new AdminLoginWindow(this);
    adminLogin->show();
}
void MainWindow::on_pushButton_exit_clicked()
{
    this->close(); //bastan menu asli
}
