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
    pantallainstrucciones.cpp \
    pantallajuego.cpp \
    pantallamisiones.cpp

HEADERS += \
    civil.h \
    crearboton.h \
    mainwindow.h \
    helicoptero.h \
    menuprincipal.h \
    pantallainstrucciones.h \
    pantallajuego.h \
    pantallamisiones.h

RESOURCES += \
    recursos.qrc
