QT += widgets

CONFIG += c++17

SOURCES += \
    adminloginwindow.cpp \
    adminpanelwindow.cpp \
    customerloginwindow.cpp \
    customerpanelwindow.cpp \
    main.cpp \
    AppController.cpp \
    Customer.cpp \
    DatabaseManager.cpp \
    DessertItem.cpp \
    DrinkItem.cpp \
    FoodItem.cpp \
    MembershipLevel.cpp \
    MenuItem.cpp \
    Order.cpp \
    Restaurant.cpp \
    SQLiteCustomerDAO.cpp \
    SQLiteMenuItemDAO.cpp \
    SQLiteOrderDAO.cpp \
    SQLiteRestaurantDAO.cpp \
    mainwindow.cpp \
    managerloginwindow.cpp \
    managerpanelwindow.cpp \
    restaurantmenuwindow.cpp \
    sqlite3.c

HEADERS += \
    DatabaseInitializer.h \
    AppController.h \
    Customer.h \
    DatabaseManager.h \
    DessertItem.h \
    DrinkItem.h \
    FoodItem.h \
    MembershipLevel.h \
    MenuItem.h \
    Order.h \
    Restaurant.h \
    SQLiteCustomerDAO.h \
    SQLiteMenuItemDAO.h \
    SQLiteOrderDAO.h \
    SQLiteRestaurantDAO.h \
    ICustomerDAO.h \
    IOrderDAO.h \
    IMenuItemDAO.h \
    IRestaurantDAO.h \
    Enums.h \
    adminloginwindow.h \
    adminpanelwindow.h \
    customerloginwindow.h \
    customerpanelwindow.h \
    mainwindow.h \
    managerloginwindow.h \
    managerpanelwindow.h \
    restaurantmenuwindow.h \
    sqlite3.h

FORMS += \
    adminloginwindow.ui \
    adminpanelwindow.ui \
    customerloginwindow.ui \
    customerpanelwindow.ui \
    mainwindow.ui \
    managerloginwindow.ui \
    managerpanelwindow.ui \
    restaurantmenuwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    food_system.db