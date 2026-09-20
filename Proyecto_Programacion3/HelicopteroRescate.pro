QT += core gui widgets

CONFIG += c++17

TARGET = HelicopteroRescate
TEMPLATE = app

SOURCES += \
    bidon.cpp \
    civil.cpp \
    crearboton.cpp \
    disparo.cpp \
    estructurabloqueadora.cpp \
    gestorusuarios.cpp \
    main.cpp \
    mainwindow.cpp \
    helicoptero.cpp \
    menuprincipal.cpp \
    nivel2.cpp \
    nivel3.cpp \
    obstaculo.cpp \
    obstaculoestatico.cpp \
    obstaculomovil.cpp \
    panelresultado.cpp \
    pantalla_inicio_sesion.cpp \
    pantallainstrucciones.cpp \
    pantallajuego.cpp \
    pantallamisiones.cpp \
    pantallaranking.cpp

HEADERS += \
    bidon.h \
    civil.h \
    crearboton.h \
    disparo.h \
    estructurabloqueadora.h \
    gestorusuarios.h \
    mainwindow.h \
    helicoptero.h \
    menuprincipal.h \
    nivel2.h \
    nivel3.h \
    obstaculo.h \
    obstaculoestatico.h \
    obstaculomovil.h \
    panelresultado.h \
    pantalla_inicio_sesion.h \
    pantallainstrucciones.h \
    pantallajuego.h \
    pantallamisiones.h \
    pantallaranking.h

RESOURCES += \
    recursos.qrc \