#ifndef RESTAURANTMENUWINDOW_H
#define RESTAURANTMENUWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class RestaurantMenuWindow; }
QT_END_NAMESPACE

class RestaurantMenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RestaurantMenuWindow(int restaurantId, int customerId, QWidget *parent = nullptr);
    ~RestaurantMenuWindow();

private slots:
    void on_pushButton_add_to_cart_clicked();
    void on_pushButton_checkout_clicked();
    void on_pushButton_back_clicked();

    void loadMenu();

    void on_pushButton_remove_item_clicked();

    void on_pushButton_clear_cart_clicked();

    void on_pushButton_cancel_order_clicked();

private:
    void updateCartDisplay();
    void updateInvoiceDisplay();

    Ui::RestaurantMenuWindow *ui;
    int currentRestaurantId;
    int currentCustomerId;
    std::vector<int> cart;  // zakhire item haye entekhabi id
    double totalPrice;
};

#endif // RESTAURANTMENUWINDOW_H