/********************************************************************************
** Form generated from reading UI file 'adminpanelwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINPANELWINDOW_H
#define UI_ADMINPANELWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminPanelWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton_active_deactive;
    QPushButton *pushButton_activate_monthly_coupons;
    QPushButton *pushButton_coupon_activation_history;
    QPushButton *pushButton_view_coupon_status;
    QPushButton *pushButton_add_new_res;
    QPushButton *pushButton_view_reports;
    QPushButton *pushButton_update_info;
    QPushButton *pushButton_edit_level;
    QPushButton *pushButton_users_by_level;
    QPushButton *pushButton_level_history;
    QPushButton *pushButton_exit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *AdminPanelWindow)
    {
        if (AdminPanelWindow->objectName().isEmpty())
            AdminPanelWindow->setObjectName("AdminPanelWindow");
        AdminPanelWindow->resize(800, 740);
        centralwidget = new QWidget(AdminPanelWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(209, 10, 411, 651));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_active_deactive = new QPushButton(verticalLayoutWidget);
        pushButton_active_deactive->setObjectName("pushButton_active_deactive");

        verticalLayout->addWidget(pushButton_active_deactive);

        pushButton_activate_monthly_coupons = new QPushButton(verticalLayoutWidget);
        pushButton_activate_monthly_coupons->setObjectName("pushButton_activate_monthly_coupons");

        verticalLayout->addWidget(pushButton_activate_monthly_coupons);

        pushButton_coupon_activation_history = new QPushButton(verticalLayoutWidget);
        pushButton_coupon_activation_history->setObjectName("pushButton_coupon_activation_history");

        verticalLayout->addWidget(pushButton_coupon_activation_history);

        pushButton_view_coupon_status = new QPushButton(verticalLayoutWidget);
        pushButton_view_coupon_status->setObjectName("pushButton_view_coupon_status");

        verticalLayout->addWidget(pushButton_view_coupon_status);

        pushButton_add_new_res = new QPushButton(verticalLayoutWidget);
        pushButton_add_new_res->setObjectName("pushButton_add_new_res");

        verticalLayout->addWidget(pushButton_add_new_res);

        pushButton_view_reports = new QPushButton(verticalLayoutWidget);
        pushButton_view_reports->setObjectName("pushButton_view_reports");

        verticalLayout->addWidget(pushButton_view_reports);

        pushButton_update_info = new QPushButton(verticalLayoutWidget);
        pushButton_update_info->setObjectName("pushButton_update_info");

        verticalLayout->addWidget(pushButton_update_info);

        pushButton_edit_level = new QPushButton(verticalLayoutWidget);
        pushButton_edit_level->setObjectName("pushButton_edit_level");

        verticalLayout->addWidget(pushButton_edit_level);

        pushButton_users_by_level = new QPushButton(verticalLayoutWidget);
        pushButton_users_by_level->setObjectName("pushButton_users_by_level");

        verticalLayout->addWidget(pushButton_users_by_level);

        pushButton_level_history = new QPushButton(verticalLayoutWidget);
        pushButton_level_history->setObjectName("pushButton_level_history");

        verticalLayout->addWidget(pushButton_level_history);

        pushButton_exit = new QPushButton(verticalLayoutWidget);
        pushButton_exit->setObjectName("pushButton_exit");

        verticalLayout->addWidget(pushButton_exit);

        AdminPanelWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(AdminPanelWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        AdminPanelWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(AdminPanelWindow);
        statusbar->setObjectName("statusbar");
        AdminPanelWindow->setStatusBar(statusbar);

        retranslateUi(AdminPanelWindow);

        QMetaObject::connectSlotsByName(AdminPanelWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AdminPanelWindow)
    {
        AdminPanelWindow->setWindowTitle(QCoreApplication::translate("AdminPanelWindow", "MainWindow", nullptr));
        pushButton_active_deactive->setText(QCoreApplication::translate("AdminPanelWindow", "Activate/Deactivate Restaurants", nullptr));
        pushButton_activate_monthly_coupons->setText(QCoreApplication::translate("AdminPanelWindow", "\360\237\216\201 Activate Monthly Coupons", nullptr));
        pushButton_coupon_activation_history->setText(QCoreApplication::translate("AdminPanelWindow", "\360\237\223\234 Activation History", nullptr));
        pushButton_view_coupon_status->setText(QCoreApplication::translate("AdminPanelWindow", "\360\237\223\212 View Coupon Status", nullptr));
        pushButton_add_new_res->setText(QCoreApplication::translate("AdminPanelWindow", " Add New Restaurant", nullptr));
        pushButton_view_reports->setText(QCoreApplication::translate("AdminPanelWindow", "View System Reports", nullptr));
        pushButton_update_info->setText(QCoreApplication::translate("AdminPanelWindow", "Update Information", nullptr));
        pushButton_edit_level->setText(QCoreApplication::translate("AdminPanelWindow", "\342\234\217\357\270\217 Edit Level/Points", nullptr));
        pushButton_users_by_level->setText(QCoreApplication::translate("AdminPanelWindow", "\360\237\221\245 Users by Level", nullptr));
        pushButton_level_history->setText(QCoreApplication::translate("AdminPanelWindow", "\360\237\223\234 Level History", nullptr));
        pushButton_exit->setText(QCoreApplication::translate("AdminPanelWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdminPanelWindow: public Ui_AdminPanelWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINPANELWINDOW_H
