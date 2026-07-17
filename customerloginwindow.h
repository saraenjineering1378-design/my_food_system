#ifndef CUSTOMERLOGINWINDOW_H
#define CUSTOMERLOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class CustomerLoginWindow; }
QT_END_NAMESPACE

class CustomerLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomerLoginWindow(QWidget *parent = nullptr);
    ~CustomerLoginWindow();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_signup_clicked();
    void on_pushButton_exit_clicked();

    void on_pushButton_backmenu_clicked();

private:
    Ui::CustomerLoginWindow *ui;
};

#endif // CUSTOMERLOGINWINDOW_H