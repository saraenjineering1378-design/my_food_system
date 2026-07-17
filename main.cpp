#include <QApplication>
#include "mainwindow.h"
#include "DatabaseManager.h"
#include "DatabaseInitializer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //etesal be data base
    DatabaseManager db("C:/Users/User/Documents/food_system/food_system.db");

    if (!db.open()) {
        return 1;
    }

    //sakht jadavel age vojod nadaran
    DatabaseInitializer::initialize(db);

    //ejraye menu asli
    MainWindow menu;
    menu.show();

    return a.exec();
}