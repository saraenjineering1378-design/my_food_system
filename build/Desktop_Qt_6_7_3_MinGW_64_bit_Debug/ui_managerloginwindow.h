/********************************************************************************
** Form generated from reading UI file 'managerloginwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERLOGINWINDOW_H
#define UI_MANAGERLOGINWINDOW_H

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

class Ui_ManagerLoginWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *lineEdit_restId;
    QLabel *label_2;
    QLineEdit *lineEdit_password;
    QPushButton *pushButton_exit;
    QPushButton *pushButton_login;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ManagerLoginWindow)
    {
        if (ManagerLoginWindow->objectName().isEmpty())
            ManagerLoginWindow->setObjectName("ManagerLoginWindow");
        ManagerLoginWindow->resize(800, 600);
        centralwidget = new QWidget(ManagerLoginWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(330, 90, 291, 211));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        lineEdit_restId = new QLineEdit(formLayoutWidget);
        lineEdit_restId->setObjectName("lineEdit_restId");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_restId);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit_password = new QLineEdit(formLayoutWidget);
        lineEdit_password->setObjectName("lineEdit_password");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit_password);

        pushButton_exit = new QPushButton(formLayoutWidget);
        pushButton_exit->setObjectName("pushButton_exit");

        formLayout->setWidget(5, QFormLayout::LabelRole, pushButton_exit);

        pushButton_login = new QPushButton(formLayoutWidget);
        pushButton_login->setObjectName("pushButton_login");

        formLayout->setWidget(4, QFormLayout::LabelRole, pushButton_login);

        ManagerLoginWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ManagerLoginWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        ManagerLoginWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ManagerLoginWindow);
        statusbar->setObjectName("statusbar");
        ManagerLoginWindow->setStatusBar(statusbar);

        retranslateUi(ManagerLoginWindow);

        QMetaObject::connectSlotsByName(ManagerLoginWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ManagerLoginWindow)
    {
        ManagerLoginWindow->setWindowTitle(QCoreApplication::translate("ManagerLoginWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("ManagerLoginWindow", "Restaurant ID", nullptr));
        label_2->setText(QCoreApplication::translate("ManagerLoginWindow", "Password", nullptr));
        pushButton_exit->setText(QCoreApplication::translate("ManagerLoginWindow", "Exit", nullptr));
        pushButton_login->setText(QCoreApplication::translate("ManagerLoginWindow", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManagerLoginWindow: public Ui_ManagerLoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERLOGINWINDOW_H
