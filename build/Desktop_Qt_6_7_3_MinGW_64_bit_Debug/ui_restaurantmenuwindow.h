/********************************************************************************
** Form generated from reading UI file 'restaurantmenuwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTAURANTMENUWINDOW_H
#define UI_RESTAURANTMENUWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestaurantMenuWindow
{
public:
    QWidget *centralwidget;
    QTextBrowser *textBrowser_menu;
    QTextBrowser *textBrowser_cart;
    QTextBrowser *textBrowser_invoice;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_add_to_cart;
    QPushButton *pushButton_remove_item;
    QPushButton *pushButton_clear_cart;
    QPushButton *pushButton_cancel_order;
    QPushButton *pushButton_checkout;
    QPushButton *pushButton_back;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *RestaurantMenuWindow)
    {
        if (RestaurantMenuWindow->objectName().isEmpty())
            RestaurantMenuWindow->setObjectName("RestaurantMenuWindow");
        RestaurantMenuWindow->resize(1202, 748);
        centralwidget = new QWidget(RestaurantMenuWindow);
        centralwidget->setObjectName("centralwidget");
        textBrowser_menu = new QTextBrowser(centralwidget);
        textBrowser_menu->setObjectName("textBrowser_menu");
        textBrowser_menu->setGeometry(QRect(370, 10, 256, 192));
        textBrowser_cart = new QTextBrowser(centralwidget);
        textBrowser_cart->setObjectName("textBrowser_cart");
        textBrowser_cart->setGeometry(QRect(370, 220, 256, 192));
        textBrowser_invoice = new QTextBrowser(centralwidget);
        textBrowser_invoice->setObjectName("textBrowser_invoice");
        textBrowser_invoice->setGeometry(QRect(370, 420, 256, 192));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(70, 90, 160, 391));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_add_to_cart = new QPushButton(verticalLayoutWidget);
        pushButton_add_to_cart->setObjectName("pushButton_add_to_cart");

        verticalLayout->addWidget(pushButton_add_to_cart);

        pushButton_remove_item = new QPushButton(verticalLayoutWidget);
        pushButton_remove_item->setObjectName("pushButton_remove_item");

        verticalLayout->addWidget(pushButton_remove_item);

        pushButton_clear_cart = new QPushButton(verticalLayoutWidget);
        pushButton_clear_cart->setObjectName("pushButton_clear_cart");

        verticalLayout->addWidget(pushButton_clear_cart);

        pushButton_cancel_order = new QPushButton(verticalLayoutWidget);
        pushButton_cancel_order->setObjectName("pushButton_cancel_order");

        verticalLayout->addWidget(pushButton_cancel_order);

        pushButton_checkout = new QPushButton(verticalLayoutWidget);
        pushButton_checkout->setObjectName("pushButton_checkout");

        verticalLayout->addWidget(pushButton_checkout);

        pushButton_back = new QPushButton(verticalLayoutWidget);
        pushButton_back->setObjectName("pushButton_back");

        verticalLayout->addWidget(pushButton_back);

        RestaurantMenuWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(RestaurantMenuWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1202, 26));
        RestaurantMenuWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(RestaurantMenuWindow);
        statusbar->setObjectName("statusbar");
        RestaurantMenuWindow->setStatusBar(statusbar);

        retranslateUi(RestaurantMenuWindow);

        QMetaObject::connectSlotsByName(RestaurantMenuWindow);
    } // setupUi

    void retranslateUi(QMainWindow *RestaurantMenuWindow)
    {
        RestaurantMenuWindow->setWindowTitle(QCoreApplication::translate("RestaurantMenuWindow", "MainWindow", nullptr));
        pushButton_add_to_cart->setText(QCoreApplication::translate("RestaurantMenuWindow", "Add to Cart", nullptr));
        pushButton_remove_item->setText(QCoreApplication::translate("RestaurantMenuWindow", "Remove Item ", nullptr));
        pushButton_clear_cart->setText(QCoreApplication::translate("RestaurantMenuWindow", "ClearCart", nullptr));
        pushButton_cancel_order->setText(QCoreApplication::translate("RestaurantMenuWindow", "Cancel Order", nullptr));
        pushButton_checkout->setText(QCoreApplication::translate("RestaurantMenuWindow", " Finalize & Checkout", nullptr));
        pushButton_back->setText(QCoreApplication::translate("RestaurantMenuWindow", "Back", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RestaurantMenuWindow: public Ui_RestaurantMenuWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTAURANTMENUWINDOW_H
