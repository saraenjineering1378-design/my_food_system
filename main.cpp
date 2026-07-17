#include <QApplication>
#include "mainwindow.h"
#include "DatabaseManager.h"
#include "DatabaseInitializer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // ✅ اتصال به دیتابیس
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open()) {
        return 1;
    }

    // ✅ ساخت جداول (اگه وجود ندارن)
    DatabaseInitializer::initialize(db);

    // ✅ اجرای منوی اصلی
    MainWindow menu;
    menu.show();

    return a.exec();
}