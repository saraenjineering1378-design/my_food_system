/********************************************************************************
** Form generated from reading UI file 'customerloginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERLOGINWINDOW_H
#define UI_CUSTOMERLOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomerLoginWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QPushButton *pushButton_username;
    QLineEdit *lineEdit_username;
    QPushButton *pushButton_password;
    QLineEdit *lineEdit_password;
    QPushButton *pushButton_signup;
    QPushButton *pushButton_login;
    QPushButton *pushButton_backmenu;
    QPushButton *pushButton_id;
    QLineEdit *lineEdit_id;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CustomerLoginWindow)
    {
        if (CustomerLoginWindow->objectName().isEmpty())
            CustomerLoginWindow->setObjectName("CustomerLoginWindow");
        CustomerLoginWindow->resize(800, 600);
        centralwidget = new QWidget(CustomerLoginWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(220, 130, 321, 221));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_username = new QPushButton(formLayoutWidget);
        pushButton_username->setObjectName("pushButton_username");

        formLayout->setWidget(0, QFormLayout::LabelRole, pushButton_username);

        lineEdit_username = new QLineEdit(formLayoutWidget);
        lineEdit_username->setObjectName("lineEdit_username");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_username);

        pushButton_password = new QPushButton(formLayoutWidget);
        pushButton_password->setObjectName("pushButton_password");

        formLayout->setWidget(2, QFormLayout::LabelRole, pushButton_password);

        lineEdit_password = new QLineEdit(formLayoutWidget);
        lineEdit_password->setObjectName("lineEdit_password");

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEdit_password);

        pushButton_signup = new QPushButton(formLayoutWidget);
        pushButton_signup->setObjectName("pushButton_signup");

        formLayout->setWidget(4, QFormLayout::LabelRole, pushButton_signup);

        pushButton_login = new QPushButton(formLayoutWidget);
        pushButton_login->setObjectName("pushButton_login");

        formLayout->setWidget(3, QFormLayout::LabelRole, pushButton_login);

        pushButton_backmenu = new QPushButton(formLayoutWidget);
        pushButton_backmenu->setObjectName("pushButton_backmenu");

        formLayout->setWidget(5, QFormLayout::FieldRole, pushButton_backmenu);

        pushButton_id = new QPushButton(formLayoutWidget);
        pushButton_id->setObjectName("pushButton_id");

        formLayout->setWidget(1, QFormLayout::LabelRole, pushButton_id);

        lineEdit_id = new QLineEdit(formLayoutWidget);
        lineEdit_id->setObjectName("lineEdit_id");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_id);

        CustomerLoginWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CustomerLoginWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        CustomerLoginWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CustomerLoginWindow);
        statusbar->setObjectName("statusbar");
        CustomerLoginWindow->setStatusBar(statusbar);

        retranslateUi(CustomerLoginWindow);

        QMetaObject::connectSlotsByName(CustomerLoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CustomerLoginWindow)
    {
        CustomerLoginWindow->setWindowTitle(QCoreApplication::translate("CustomerLoginWindow", "MainWindow", nullptr));
        pushButton_username->setText(QCoreApplication::translate("CustomerLoginWindow", "username ", nullptr));
        pushButton_password->setText(QCoreApplication::translate("CustomerLoginWindow", "password", nullptr));
        pushButton_signup->setText(QCoreApplication::translate("CustomerLoginWindow", "signup", nullptr));
        pushButton_login->setText(QCoreApplication::translate("CustomerLoginWindow", "login", nullptr));
        pushButton_backmenu->setText(QCoreApplication::translate("CustomerLoginWindow", "Back to Main Menu ", nullptr));
        pushButton_id->setText(QCoreApplication::translate("CustomerLoginWindow", "Customer ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerLoginWindow: public Ui_CustomerLoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERLOGINWINDOW_H
