#include "pantalla_inicio_sesion.h"
#include <QPainter>
#include <iostream>
using namespace std;

pantalla_inicio_sesion::pantalla_inicio_sesion(QWidget *parent) : QWidget(parent){
    fondoInicioSesion.load(":/imagenes/menu_principal.png");
    if(fondoInicioSesion.isNull() == true){
        cout<<"[ERROR] no se pudo cargar la imagen de fondo de inicio de sesion"<<endl;
    }
    configurarBotones();
}

void pantalla_inicio_sesion::configurarBotones(){
    botonIniciarSesion = new CrearBoton("INICIAR SESION", this);
    botonRegistrarse = new CrearBoton("REGISTRARSE", this);
    botonSalir = new CrearBoton("SALIR", this);

    botonIniciarSesion->resize(200, 50);
    botonRegistrarse->resize(200, 50);
    botonSalir->resize(200, 50);

    connect(botonIniciarSesion, &QPushButton::clicked, this, &pantalla_inicio_sesion::iniciarSesionPresionado);
    connect(botonRegistrarse, &QPushButton::clicked, this, &pantalla_inicio_sesion::registrarsePresionado);
    connect(botonSalir, &QPushButton::clicked, this, &pantalla_inicio_sesion::salirPresionado);
}

void pantalla_inicio_sesion::acomodarBotones(){
    int totalBotones = 3;
    int alturaBoton = botonIniciarSesion->height();
    int espacio = 62;
    int alturaTotal = (totalBotones - 1) * espacio + alturaBoton;

    int posX = (width() - botonIniciarSesion->width()) / 2;
    int posYInicial = height() - alturaTotal - 40;

    if(posYInicial < 10){
        posYInicial = 10;
    }

    botonIniciarSesion->move(posX, posYInicial);
    botonRegistrarse->move(posX, posYInicial + espacio);
    botonSalir->move(posX, posYInicial + espacio * 2);
}

void pantalla_inicio_sesion::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(rect(), fondoInicioSesion);
}

void pantalla_inicio_sesion::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event);
    acomodarBotones();
}