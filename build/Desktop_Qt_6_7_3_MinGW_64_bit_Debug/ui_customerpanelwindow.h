/********************************************************************************
** Form generated from reading UI file 'customerpanelwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERPANELWINDOW_H
#define UI_CUSTOMERPANELWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomerPanelWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QPushButton *pushButton_charge_wallet;
    QListWidget *listWidget_restaurants;
    QPushButton *pushButton_view_menu;
    QPushButton *pushButton_order_history;
    QPushButton *pushButton_place_order;
    QPushButton *pushButton_logout;
    QLabel *label_level;
    QLabel *label_balance;
    QLabel *label_points;
    QLabel *label_badge;
    QLabel *label_coupons;
    QLabel *label_next_level;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CustomerPanelWindow)
    {
        if (CustomerPanelWindow->objectName().isEmpty())
            CustomerPanelWindow->setObjectName("CustomerPanelWindow");
        CustomerPanelWindow->resize(1196, 771);
        centralwidget = new QWidget(CustomerPanelWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(140, 80, 681, 571));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        pushButton_charge_wallet = new QPushButton(formLayoutWidget);
        pushButton_charge_wallet->setObjectName("pushButton_charge_wallet");

        formLayout->setWidget(9, QFormLayout::FieldRole, pushButton_charge_wallet);

        listWidget_restaurants = new QListWidget(formLayoutWidget);
        listWidget_restaurants->setObjectName("listWidget_restaurants");

        formLayout->setWidget(0, QFormLayout::FieldRole, listWidget_restaurants);

        pushButton_view_menu = new QPushButton(formLayoutWidget);
        pushButton_view_menu->setObjectName("pushButton_view_menu");

        formLayout->setWidget(7, QFormLayout::FieldRole, pushButton_view_menu);

        pushButton_order_history = new QPushButton(formLayoutWidget);
        pushButton_order_history->setObjectName("pushButton_order_history");

        formLayout->setWidget(8, QFormLayout::FieldRole, pushButton_order_history);

        pushButton_place_order = new QPushButton(formLayoutWidget);
        pushButton_place_order->setObjectName("pushButton_place_order");

        formLayout->setWidget(10, QFormLayout::FieldRole, pushButton_place_order);

        pushButton_logout = new QPushButton(formLayoutWidget);
        pushButton_logout->setObjectName("pushButton_logout");

        formLayout->setWidget(11, QFormLayout::FieldRole, pushButton_logout);

        label_level = new QLabel(formLayoutWidget);
        label_level->setObjectName("label_level");

        formLayout->setWidget(2, QFormLayout::FieldRole, label_level);

        label_balance = new QLabel(formLayoutWidget);
        label_balance->setObjectName("label_balance");

        formLayout->setWidget(4, QFormLayout::FieldRole, label_balance);

        label_points = new QLabel(formLayoutWidget);
        label_points->setObjectName("label_points");

        formLayout->setWidget(3, QFormLayout::FieldRole, label_points);

        label_badge = new QLabel(formLayoutWidget);
        label_badge->setObjectName("label_badge");

        formLayout->setWidget(5, QFormLayout::FieldRole, label_badge);

        label_coupons = new QLabel(formLayoutWidget);
        label_coupons->setObjectName("label_coupons");

        formLayout->setWidget(6, QFormLayout::FieldRole, label_coupons);

        label_next_level = new QLabel(formLayoutWidget);
        label_next_level->setObjectName("label_next_level");

        formLayout->setWidget(1, QFormLayout::FieldRole, label_next_level);

        CustomerPanelWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CustomerPanelWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1196, 26));
        CustomerPanelWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CustomerPanelWindow);
        statusbar->setObjectName("statusbar");
        CustomerPanelWindow->setStatusBar(statusbar);

        retranslateUi(CustomerPanelWindow);

        QMetaObject::connectSlotsByName(CustomerPanelWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CustomerPanelWindow)
    {
        CustomerPanelWindow->setWindowTitle(QCoreApplication::translate("CustomerPanelWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("CustomerPanelWindow", "Select  Restaurant:", nullptr));
        pushButton_charge_wallet->setText(QCoreApplication::translate("CustomerPanelWindow", "Charge Wallet\360\237\222\265", nullptr));
        pushButton_view_menu->setText(QCoreApplication::translate("CustomerPanelWindow", "View Restaurant Menu\360\237\233\216", nullptr));
        pushButton_order_history->setText(QCoreApplication::translate("CustomerPanelWindow", "View Order History\360\237\233\215", nullptr));
        pushButton_place_order->setText(QCoreApplication::translate("CustomerPanelWindow", "Place Order\360\237\216\260", nullptr));
        pushButton_logout->setText(QCoreApplication::translate("CustomerPanelWindow", "Logout", nullptr));
        label_level->setText(QCoreApplication::translate("CustomerPanelWindow", "\360\237\217\205 Level: Normal", nullptr));
        label_balance->setText(QCoreApplication::translate("CustomerPanelWindow", "\360\237\222\260 Balance: $0", nullptr));
        label_points->setText(QCoreApplication::translate("CustomerPanelWindow", "\342\255\220 Points: 0", nullptr));
        label_badge->setText(QCoreApplication::translate("CustomerPanelWindow", "\360\237\217\267\357\270\217 Badge: None", nullptr));
        label_coupons->setText(QCoreApplication::translate("CustomerPanelWindow", "\360\237\216\253 Coupons: 0", nullptr));
        label_next_level->setText(QCoreApplication::translate("CustomerPanelWindow", "\342\236\241\357\270\217 Next Level at: 100 points", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CustomerPanelWindow: public Ui_CustomerPanelWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERPANELWINDOW_H
