#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "pantallamisiones.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setWindowTitle("Helicoptero Rescate - Prototipo");
    resize(820, 540);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);
    configurarMenu();
    configurarJuego();
    configurarInstrucciones();
    configurarMisiones();

    stack->addWidget(menu);
    stack->addWidget(pantallaJuego);
    stack->addWidget(pantallaInstrucciones);
    stack->setCurrentIndex(0);
    stack->addWidget(pantallaMisiones);

    connect(menu, &MenuPrincipal::jugarPresionado, this, &MainWindow::irAJuego);
    connect(menu, &MenuPrincipal::instruccionesPresionado, this, &MainWindow::irAInstrucciones);
    connect(pantallaInstrucciones, &PantallaInstrucciones::volverPresionado, this, &MainWindow::irAMenu);
    connect(menu, &MenuPrincipal::salirPresionado, this, &MainWindow::salirDelJuego);
    connect(menu, &MenuPrincipal::misionesPresionado, this, &MainWindow::irAMisiones);
    connect(pantallaMisiones, &PantallaMisiones::volverPresionado, this, &MainWindow::irAMenu);
    connect(pantallaMisiones, &PantallaMisiones::mision1Presionado, this, &MainWindow::mision1Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision2Presionado, this, &MainWindow::mision2Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision3Presionado, this, &MainWindow::mision3Elegida);

}

MainWindow::~MainWindow(){
}

void MainWindow::configurarMenu(){
    menu = new MenuPrincipal(this);
}

void MainWindow::configurarJuego(){
    pantallaJuego = new PantallaJuego(this);
}

void MainWindow::configurarInstrucciones(){
    pantallaInstrucciones = new PantallaInstrucciones(this);
}

void MainWindow::irAInstrucciones(){
    stack->setCurrentIndex(2);
}
void MainWindow::irAMenu(){
    stack->setCurrentIndex(0);
}

void MainWindow::irAJuego(){
    stack->setCurrentIndex(1);
    pantallaJuego->setFocus();
}

void MainWindow::salirDelJuego(){
    QApplication::quit();
}
void MainWindow::configurarMisiones(){
    pantallaMisiones = new PantallaMisiones(this);
}
void MainWindow::irAMisiones(){
    stack->setCurrentIndex(3);
}
void MainWindow::mision1Elegida(){
    cout<<"Mision 1 seleccionada "<<endl;
}
void MainWindow::mision2Elegida(){
    cout<<"Mision 2 seleccionada "<<endl;
}
void MainWindow::mision3Elegida(){
    cout<<"Mision 3 seleccionada"<<endl;
}