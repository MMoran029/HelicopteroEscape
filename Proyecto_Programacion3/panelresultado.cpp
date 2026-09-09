#include "panelresultado.h"
#include <QPainter>

PanelResultado::PanelResultado(QWidget *parent) : QWidget(parent){
    configurarElementos();
    hide(); // permanece oculto hasta que una partida termine
}

void PanelResultado::configurarElementos(){
    labelTitulo = new QLabel(this);
    labelTitulo->setAlignment(Qt::AlignCenter);

    labelInfo = new QLabel(this);
    labelInfo->setAlignment(Qt::AlignCenter);
    labelInfo->setStyleSheet("color: rgb(225,225,225); font-size: 14px;");
    labelInfo->setWordWrap(true);

    botonReiniciar = new CrearBoton("REINICIAR", this);
    botonMisiones = new CrearBoton("SELECCIONAR MISION", this);
    botonMenu = new CrearBoton("MENU PRINCIPAL", this);
    botonSiguienteNivel = new CrearBoton("SIGUIENTE NIVEL", this);

    botonReiniciar->resize(170, 44);
    botonMisiones->resize(220, 44);
    botonMenu->resize(190, 44);
    botonSiguienteNivel->resize(190, 44);

    connect(botonReiniciar, &QPushButton::clicked, this, &PanelResultado::reiniciarPresionado);
    connect(botonMisiones, &QPushButton::clicked, this, &PanelResultado::misionesPresionado);
    connect(botonMenu, &QPushButton::clicked, this, &PanelResultado::menuPresionado);
    connect(botonSiguienteNivel, &QPushButton::clicked, this, &PanelResultado::siguienteNivelPresionado);
}

void PanelResultado::mostrarResultado(bool victoria, const QString &infoTexto){
    if (victoria == true) {
        labelTitulo->setText("¡ESCAPE EXITOSO!");
        labelTitulo->setStyleSheet("color: rgb(110,220,110); font-size: 32px; font-weight: bold;");
        botonSiguienteNivel->show();
    } else {
        labelTitulo->setText("PERDISTE");
        labelTitulo->setStyleSheet("color: rgb(225,80,80); font-size: 32px; font-weight: bold;");
        botonSiguienteNivel->hide();
    }

    labelInfo->setText(infoTexto);
    acomodarElementos();
    raise();
    show();
}

void PanelResultado::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fondo oscuro semitransparente sobre toda la pantalla de juego.
    painter.fillRect(rect(), QColor(0, 0, 0, 165));

    // Tarjeta central del resultado.
    QRect tarjeta(width() / 2 - 220, height() / 2 - 205, 440, 410);
    painter.setBrush(QColor(32, 38, 52));
    painter.setPen(QPen(QColor(95, 105, 125), 2));
    painter.drawRoundedRect(tarjeta, 14, 14);
}

void PanelResultado::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    acomodarElementos();
}

void PanelResultado::acomodarElementos(){
    int centroX = width() / 2;
    int centroY = height() / 2;

    labelTitulo->setGeometry(centroX - 210, centroY - 190, 420, 45);
    labelInfo->setGeometry(centroX - 190, centroY - 135, 380, 90);

    // Posiciones fijas para los 4 botones (no dependen de cuales esten
    // visibles). Asi el orden nunca cambia entre victoria y derrota:
    // si se pierde, "Siguiente nivel" simplemente no se dibuja y su
    // espacio queda vacio, en vez de recorrer a los demas botones.
    int y = centroY - 45;
    botonSiguienteNivel->move(centroX - botonSiguienteNivel->width() / 2, y);

    y += 50;
    botonReiniciar->move(centroX - botonReiniciar->width() / 2, y);

    y += 50;
    botonMisiones->move(centroX - botonMisiones->width() / 2, y);

    y += 50;
    botonMenu->move(centroX - botonMenu->width() / 2, y);
}
