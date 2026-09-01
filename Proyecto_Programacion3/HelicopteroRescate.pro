QT += core gui widgets

CONFIG += c++17

TARGET = HelicopteroRescate
TEMPLATE = app

SOURCES += \
    civil.cpp \
    crearboton.cpp \
    main.cpp \
    mainwindow.cpp \
    helicoptero.cpp \
    menuprincipal.cpp \
    pantallajuego.cpp

HEADERS += \
    civil.h \
    crearboton.h \
    mainwindow.h \
    helicoptero.h \
    menuprincipal.h \
    pantallajuego.h

RESOURCES += \
    recursos.qrc
