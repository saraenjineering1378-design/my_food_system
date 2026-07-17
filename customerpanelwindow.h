#ifndef CUSTOMERPANELWINDOW_H
#define CUSTOMERPANELWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomerPanelWindow; }
QT_END_NAMESPACE

class CustomerPanelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomerPanelWindow(int customerId, QWidget *parent = nullptr);
    ~CustomerPanelWindow();


private slots:
    void on_pushButton_view_menu_clicked();
    void on_pushButton_place_order_clicked();
    void on_pushButton_order_history_clicked();
    void on_pushButton_charge_wallet_clicked();
    void on_pushButton_logout_clicked();

    void on_restaurantSelected(QListWidgetItem *item);


private:
    void loadRestaurants();
    void updateWalletDisplay();
    void updateLoyaltyDisplay();
    void checkForUpgradeMessage();

    Ui::CustomerPanelWindow *ui;
    int currentCustomerId;
    int selectedRestaurantId;
    bool hasShownUpgradeMessage;  // baray in ke dar hafeze ham check kone

};

#endif // CUSTOMERPANELWINDOW_H