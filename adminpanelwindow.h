#ifndef ADMINPANELWINDOW_H
#define ADMINPANELWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminPanelWindow; }
QT_END_NAMESPACE

class AdminPanelWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminPanelWindow(QWidget *parent = nullptr);
    ~AdminPanelWindow();

private slots:
    void on_pushButton_users_by_level_clicked();
    void on_pushButton_level_history_clicked();
    void on_pushButton_edit_level_clicked();
    void on_pushButton_exit_clicked();

    void on_pushButton_active_deactive_clicked();

    void on_pushButton_add_new_res_clicked();

    void on_pushButton_view_reports_clicked();

    void on_pushButton_update_info_clicked();

    void on_pushButton_activate_monthly_coupons_clicked();

    void on_pushButton_coupon_activation_history_clicked();

    void on_pushButton_view_coupon_status_clicked();

private:
    Ui::AdminPanelWindow *ui;
};

#endif // ADMINPANELWINDOW_H