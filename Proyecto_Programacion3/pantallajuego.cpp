#include "pantallajuego.h"
#include <QPainter>
#include <QBrush>
#include <QVBoxLayout>

PantallaJuego::PantallaJuego(QWidget *parent) : QWidget(parent){
    configurarEscena();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(vista);
    setLayout(layout);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &PantallaJuego::actualizarJuego);
    timerJuego->start(16);

    setFocusPolicy(Qt::StrongFocus);
}

void PantallaJuego::configurarEscena(){
    escena = new QGraphicsScene(0, 0, ANCHO_ESCENA, ALTO_ESCENA, this);
    escena->setBackgroundBrush(QBrush(QColor(30, 30, 60)));

    vista = new QGraphicsView(escena, this);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    helicoptero = new Helicoptero();
    helicoptero->setPos(100, ALTO_ESCENA / 2);
    escena->addItem(helicoptero);
}

void PantallaJuego::keyPressEvent(QKeyEvent *event){
    if(event->isAutoRepeat() == true){
        return;
    }
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(true);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(true);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(true);
        break;
    }
    QWidget::keyPressEvent(event);
}

void PantallaJuego::keyReleaseEvent(QKeyEvent *event){
    if(event->isAutoRepeat() == true){
        return;
    }
    switch(event->key()){
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(false);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(false);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(false);
        break;
    }
    QWidget::keyReleaseEvent(event);
}

void PantallaJuego::actualizarJuego(){
    helicoptero->actualizarFisica(ANCHO_ESCENA, ALTO_ESCENA);
}