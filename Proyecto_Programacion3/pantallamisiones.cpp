#include "pantallamisiones.h"
#include <QPainter>
#include <iostream>
using namespace std;
PantallaMisiones::PantallaMisiones(QWidget *parent) : QWidget(parent){
    fondoMisiones.load(":/imagenes/Imagenes/pantallamisiones.png");
    if(fondoMisiones.isNull() == true){
        cout<<"[ERROR] no se pudo cargar la imagen de misiones"<<endl;
    }
    configurarElementos();
}
void PantallaMisiones::configurarElementos(){
    botonMision1 = new CrearBoton("1", this);
    botonMision2 = new CrearBoton("2", this);
    botonMision3 = new CrearBoton("3", this);
    botonVolver = new CrearBoton("VOLVER", this);
    botonMision1->resize(100, 100);
    botonMision2->resize(100, 100);
    botonMision3->resize(100, 100);
    botonVolver->resize(200, 50);
    connect(botonMision1, &QPushButton::clicked, this, &PantallaMisiones::mision1Presionado);
    connect(botonMision2, &QPushButton::clicked, this, &PantallaMisiones::mision2Presionado);
    connect(botonMision3, &QPushButton::clicked, this, &PantallaMisiones::mision3Presionado);
    connect(botonVolver, &QPushButton::clicked, this, &PantallaMisiones::volverPresionado);
}
void PantallaMisiones::acomodarElementos(){
    int espacio = 40;
    int anchoTotal = botonMision1->width() * 3 + espacio * 2;
    int posXInicial = (width() - anchoTotal) / 2;
    int posY = (height() - botonMision1->height()) / 2;
    botonMision1->move(posXInicial, posY);
    botonMision2->move(posXInicial + botonMision1->width() + espacio, posY);
    botonMision3->move(posXInicial + (botonMision1->width() + espacio) * 2, posY);
    int posXVolver = (width() - botonVolver->width()) / 2;
    int posYVolver = height() - 80;
    if(posYVolver < 10){
        posYVolver = 10;
    }
    botonVolver->move(posXVolver, posYVolver);
}
void PantallaMisiones::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), fondoMisiones);
}
void PantallaMisiones::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    acomodarElementos();
}