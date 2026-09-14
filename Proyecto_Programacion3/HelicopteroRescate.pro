QT += core gui widgets

CONFIG += c++17

TARGET = HelicopteroRescate
TEMPLATE = app

SOURCES += \
    bidon.cpp \
    civil.cpp \
    crearboton.cpp \
    disparo.cpp \
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
    bidon.h \
    civil.h \
    crearboton.h \
    disparo.h \
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
    recursos.qrc