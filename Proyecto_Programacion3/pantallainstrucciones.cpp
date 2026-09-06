#include "pantallainstrucciones.h"
#include <QPainter>
#include <iostream>
using namespace std;

PantallaInstrucciones::PantallaInstrucciones(QWidget *parent) : QWidget(parent){
    fondoInstrucciones.load(":/imagenes/Imagenes/instrucciones.png");
    if(fondoInstrucciones.isNull() == true){
        cout<<"[ERROR] no se pudo cargar la imagen de instrucciones"<<endl;
    }
    configurarElementos();
}
void PantallaInstrucciones::configurarElementos(){
    botonVolver = new CrearBoton("VOLVER", this);
    botonVolver->resize(200, 50);
    connect(botonVolver, &QPushButton::clicked, this, &PantallaInstrucciones::volverPresionado);
}
void PantallaInstrucciones::acomodarElementos(){
    int posX = (width() - botonVolver->width()) / 2;
    int posY = height() - 80;
    if(posY < 10){
        posY = 10;
    }
    botonVolver->move(posX, posY);
}
void PantallaInstrucciones::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), fondoInstrucciones);
}
void PantallaInstrucciones::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    acomodarElementos();
}