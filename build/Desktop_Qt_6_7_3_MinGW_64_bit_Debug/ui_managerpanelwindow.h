/********************************************************************************
** Form generated from reading UI file 'managerpanelwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERPANELWINDOW_H
#define UI_MANAGERPANELWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ManagerPanelWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QPushButton *pushButton_edit_res_info;
    QPushButton *pushButton_incomingorders;
    QLabel *label;
    QPushButton *pushButton_exit;
    QPushButton *pushButton_add_edit_remove_items;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ManagerPanelWindow)
    {
        if (ManagerPanelWindow->objectName().isEmpty())
            ManagerPanelWindow->setObjectName("ManagerPanelWindow");
        ManagerPanelWindow->resize(800, 600);
        ManagerPanelWindow->setMinimumSize(QSize(0, 0));
        centralwidget = new QWidget(ManagerPanelWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(190, 140, 319, 281));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_edit_res_info = new QPushButton(formLayoutWidget);
        pushButton_edit_res_info->setObjectName("pushButton_edit_res_info");

        formLayout->setWidget(9, QFormLayout::LabelRole, pushButton_edit_res_info);

        pushButton_incomingorders = new QPushButton(formLayoutWidget);
        pushButton_incomingorders->setObjectName("pushButton_incomingorders");

        formLayout->setWidget(8, QFormLayout::LabelRole, pushButton_incomingorders);

        label = new QLabel(formLayoutWidget);
        label->setObjectName("label");
        label->setMinimumSize(QSize(4, 5));

        formLayout->setWidget(1, QFormLayout::SpanningRole, label);

        pushButton_exit = new QPushButton(formLayoutWidget);
        pushButton_exit->setObjectName("pushButton_exit");

        formLayout->setWidget(10, QFormLayout::LabelRole, pushButton_exit);

        pushButton_add_edit_remove_items = new QPushButton(formLayoutWidget);
        pushButton_add_edit_remove_items->setObjectName("pushButton_add_edit_remove_items");

        formLayout->setWidget(7, QFormLayout::LabelRole, pushButton_add_edit_remove_items);

        ManagerPanelWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ManagerPanelWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        ManagerPanelWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ManagerPanelWindow);
        statusbar->setObjectName("statusbar");
        ManagerPanelWindow->setStatusBar(statusbar);

        retranslateUi(ManagerPanelWindow);

        QMetaObject::connectSlotsByName(ManagerPanelWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ManagerPanelWindow)
    {
        ManagerPanelWindow->setWindowTitle(QCoreApplication::translate("ManagerPanelWindow", "MainWindow", nullptr));
        pushButton_edit_res_info->setText(QCoreApplication::translate("ManagerPanelWindow", "Edit Restaurant Info", nullptr));
        pushButton_incomingorders->setText(QCoreApplication::translate("ManagerPanelWindow", "View & Process Incoming Orders", nullptr));
        label->setText(QCoreApplication::translate("ManagerPanelWindow", "                        \360\237\221\250\342\200\215\360\237\215\263 Manager Panel", nullptr));
        pushButton_exit->setText(QCoreApplication::translate("ManagerPanelWindow", "Exit", nullptr));
        pushButton_add_edit_remove_items->setText(QCoreApplication::translate("ManagerPanelWindow", "Manage Menu (Add/Edit/Remove Items)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ManagerPanelWindow: public Ui_ManagerPanelWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERPANELWINDOW_H
