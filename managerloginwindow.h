#ifndef MANAGERLOGINWINDOW_H
#define MANAGERLOGINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ManagerLoginWindow; }
QT_END_NAMESPACE

class ManagerLoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerLoginWindow(QWidget *parent = nullptr);
    ~ManagerLoginWindow();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_exit_clicked();

private:
    Ui::ManagerLoginWindow *ui;
};

#endif // MANAGERLOGINWINDOW_H