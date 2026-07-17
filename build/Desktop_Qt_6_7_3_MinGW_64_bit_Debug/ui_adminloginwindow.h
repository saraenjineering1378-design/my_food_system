/********************************************************************************
** Form generated from reading UI file 'adminloginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINLOGINWINDOW_H
#define UI_ADMINLOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminLoginWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QPushButton *pushButton_login;
    QPushButton *pushButton_exit;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *AdminLoginWindow)
    {
        if (AdminLoginWindow->objectName().isEmpty())
            AdminLoginWindow->setObjectName("AdminLoginWindow");
        AdminLoginWindow->resize(800, 600);
        centralwidget = new QWidget(AdminLoginWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(260, 170, 301, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_login = new QPushButton(formLayoutWidget);
        pushButton_login->setObjectName("pushButton_login");

        formLayout->setWidget(2, QFormLayout::LabelRole, pushButton_login);

        pushButton_exit = new QPushButton(formLayoutWidget);
        pushButton_exit->setObjectName("pushButton_exit");

        formLayout->setWidget(3, QFormLayout::LabelRole, pushButton_exit);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        lineEdit_password = new QLineEdit(formLayoutWidget);
        lineEdit_password->setObjectName("lineEdit_password");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_password);

        AdminLoginWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AdminLoginWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        AdminLoginWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(AdminLoginWindow);
        statusbar->setObjectName("statusbar");
        AdminLoginWindow->setStatusBar(statusbar);

        retranslateUi(AdminLoginWindow);

        QMetaObject::connectSlotsByName(AdminLoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AdminLoginWindow)
    {
        AdminLoginWindow->setWindowTitle(QCoreApplication::translate("AdminLoginWindow", "MainWindow", nullptr));
        pushButton_login->setText(QCoreApplication::translate("AdminLoginWindow", "login", nullptr));
        pushButton_exit->setText(QCoreApplication::translate("AdminLoginWindow", "exit", nullptr));
        label_2->setText(QCoreApplication::translate("AdminLoginWindow", "password", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminLoginWindow: public Ui_AdminLoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINLOGINWINDOW_H
