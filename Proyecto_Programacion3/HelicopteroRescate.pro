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
    obstaculo.cpp \
    obstaculoestatico.cpp \
    obstaculomovil.cpp \
    panelresultado.cpp \
    pantallainstrucciones.cpp \
    pantallajuego.cpp \
    pantallamisiones.cpp

HEADERS += \
    civil.h \
    crearboton.h \
    mainwindow.h \
    helicoptero.h \
    menuprincipal.h \
    obstaculo.h \
    obstaculoestatico.h \
    obstaculomovil.h \
    panelresultado.h \
    pantallainstrucciones.h \
    pantallajuego.h \
    pantallamisiones.h

RESOURCES += \
    recursos.qrc \
    recursos.qrc \
    recursos.qrc
