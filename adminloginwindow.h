#ifndef ADMINLOGINWINDOW_H
#define ADMINLOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminLoginWindow; }
QT_END_NAMESPACE

class AdminLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminLoginWindow(QWidget *parent = nullptr);
    ~AdminLoginWindow();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_exit_clicked();

private:
    Ui::AdminLoginWindow *ui;
};

#endif // ADMINLOGINWINDOW_H