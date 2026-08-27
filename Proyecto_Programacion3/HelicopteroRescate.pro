QT += core gui widgets

CONFIG += c++17

TARGET = HelicopteroRescate
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    helicoptero.cpp \
    obstaculo.cpp \
    obstaculoestatico.cpp \
    obstaculomovil.cpp

HEADERS += \
    mainwindow.h \
    helicoptero.h \
    obstaculo.h \
    obstaculoestatico.h \
    obstaculomovil.h
