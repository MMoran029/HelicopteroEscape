#include "menuprincipal.h"
#include <QPainter>
#include <iostream>
using namespace std;

MenuPrincipal::MenuPrincipal(QWidget *parent) : QWidget(parent){
    fondoMenu.load(":/imagenes/menu_principal.png");
    if(fondoMenu.isNull() == true){
        cout<<"[ERROR] no se pudo cargar la imagen del menu principal"<<endl;
    }
    configurarBotones();
}

void MenuPrincipal::configurarBotones(){
    botonJugar = new CrearBoton("JUGAR", this);
    botonMisiones = new CrearBoton("MISIONES", this);
    botonRanking = new CrearBoton("RANKING", this);
    botonInstrucciones = new CrearBoton("INSTRUCCIONES", this);
    botonSalir = new CrearBoton("SALIR", this);

    botonJugar->resize(200, 50);
    botonMisiones->resize(200, 50);
    botonRanking->resize(200, 50);
    botonInstrucciones->resize(200, 50);
    botonSalir->resize(200, 50);
    connect(botonMisiones, &QPushButton::clicked, this, &MenuPrincipal::misionesPresionado);
    connect(botonJugar, &QPushButton::clicked, this, &MenuPrincipal::jugarPresionado);
    connect(botonInstrucciones, &QPushButton::clicked, this, &MenuPrincipal::instruccionesPresionado);
    connect(botonSalir, &QPushButton::clicked, this, &MenuPrincipal::salirPresionado);
}

void MenuPrincipal::acomodarBotones(){
    int totalBotones = 5;
    int alturaBoton = botonJugar->height();
    int espacio = 62;
    int alturaTotal = (totalBotones - 1) * espacio + alturaBoton;

    int posX = (width() - botonJugar->width()) / 2;
    int posYInicial = height() - alturaTotal - 40;

    if(posYInicial < 10){
        posYInicial = 10;
    }

    botonJugar->move(posX, posYInicial);
    botonMisiones->move(posX, posYInicial + espacio);
    botonRanking->move(posX, posYInicial + espacio * 2);
    botonInstrucciones->move(posX, posYInicial + espacio * 3);
    botonSalir->move(posX, posYInicial + espacio * 4);
}

void MenuPrincipal::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), fondoMenu);
}

void MenuPrincipal::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    acomodarBotones();
}