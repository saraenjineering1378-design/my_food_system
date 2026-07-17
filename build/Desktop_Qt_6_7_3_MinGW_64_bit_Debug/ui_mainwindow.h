/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPushButton *pushButton_admin;
    QPushButton *pushButton_manager;
    QPushButton *pushButton_customer;
    QPushButton *pushButton_exit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(220, 100, 341, 311));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        pushButton_admin = new QPushButton(verticalLayoutWidget);
        pushButton_admin->setObjectName("pushButton_admin");

        verticalLayout->addWidget(pushButton_admin);

        pushButton_manager = new QPushButton(verticalLayoutWidget);
        pushButton_manager->setObjectName("pushButton_manager");

        verticalLayout->addWidget(pushButton_manager);

        pushButton_customer = new QPushButton(verticalLayoutWidget);
        pushButton_customer->setObjectName("pushButton_customer");

        verticalLayout->addWidget(pushButton_customer);

        pushButton_exit = new QPushButton(verticalLayoutWidget);
        pushButton_exit->setObjectName("pushButton_exit");

        verticalLayout->addWidget(pushButton_exit);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "              Welcome to the Food System! \360\237\215\275\357\270\217", nullptr));
        pushButton_admin->setText(QCoreApplication::translate("MainWindow", "Admin Portal \360\237\221\221", nullptr));
        pushButton_manager->setText(QCoreApplication::translate("MainWindow", "Manager Portal \360\237\221\250\342\200\215\360\237\215\263", nullptr));
        pushButton_customer->setText(QCoreApplication::translate("MainWindow", "Customer Portal \360\237\215\224", nullptr));
        pushButton_exit->setText(QCoreApplication::translate("MainWindow", "Exit \360\237\217\203\342\200\215\342\231\202\342\200\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
