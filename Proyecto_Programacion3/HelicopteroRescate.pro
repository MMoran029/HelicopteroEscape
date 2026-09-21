QT += core gui widgets multimedia

CONFIG += c++17

TARGET = HelicopteroRescate
TEMPLATE = app

SOURCES += \
    barrahud.cpp \
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
    nivelextra.cpp \
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
    barrahud.h \
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
    nivelextra.h \
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
    recursos.qrc

DISTFILES += \
    Inicio_sesion.png

CONFIG(debug, debug|release) {
    COPIA_AUDIO_DEST = $$OUT_PWD/debug/Audio
}else{
    COPIA_AUDIO_DEST = $$OUT_PWD/release/Audio
}
copyaudio.commands = $(COPY_DIR) $$shell_quote($$shell_path($$PWD/Audio)) $$shell_quote($$shell_path($$COPIA_AUDIO_DEST))
first.depends = $(first) copyaudio
export(first.depends)
export(copyaudio.commands)
QMAKE_EXTRA_TARGETS += first copyaudio