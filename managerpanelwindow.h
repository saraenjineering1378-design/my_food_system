#ifndef MANAGERPANELWINDOW_H
#define MANAGERPANELWINDOW_H

#include <QMainWindow>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class ManagerPanelWindow; }
QT_END_NAMESPACE

class ManagerPanelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerPanelWindow(int restId, QWidget *parent = nullptr);
    ~ManagerPanelWindow();

private slots:
    void on_pushButton_add_edit_remove_items_clicked();
    void on_pushButton_incomingorders_clicked();
    void on_pushButton_edit_res_info_clicked();
    void on_pushButton_exit_clicked();

    void changeOrderStatus(int orderId, QString newStatus);

private:
    Ui::ManagerPanelWindow *ui;
    int currentRestaurantId;
};

#endif // MANAGERPANELWINDOW_H