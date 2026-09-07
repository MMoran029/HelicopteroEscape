#include "mainwindow.h"
#include <QApplication>
#include <iostream>
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

    // El orden de insercion define el indice de cada pantalla en el stack:
    // 0 = menu, 1 = juego (Mision 1), 2 = instrucciones, 3 = misiones.
    stack->addWidget(menu);
    stack->addWidget(pantallaJuego);
    stack->addWidget(pantallaInstrucciones);
    stack->addWidget(pantallaMisiones);
    stack->setCurrentIndex(0);

    connect(menu, &MenuPrincipal::jugarPresionado, this, &MainWindow::irAJuego);
    connect(menu, &MenuPrincipal::instruccionesPresionado, this, &MainWindow::irAInstrucciones);
    connect(menu, &MenuPrincipal::misionesPresionado, this, &MainWindow::irAMisiones);
    connect(menu, &MenuPrincipal::salirPresionado, this, &MainWindow::salirDelJuego);

    connect(pantallaInstrucciones, &PantallaInstrucciones::volverPresionado, this, &MainWindow::irAMenu);

    connect(pantallaMisiones, &PantallaMisiones::volverPresionado, this, &MainWindow::irAMenu);
    connect(pantallaMisiones, &PantallaMisiones::mision1Presionado, this, &MainWindow::mision1Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision2Presionado, this, &MainWindow::mision2Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision3Presionado, this, &MainWindow::mision3Elegida);

    // El panel de resultado (victoria/derrota) dentro de PantallaJuego
    // reenvia estos botones hasta aca, que es quien controla el stack.
    connect(pantallaJuego, &PantallaJuego::solicitaMenu, this, &MainWindow::irAMenu);
    connect(pantallaJuego, &PantallaJuego::solicitaMisiones, this, &MainWindow::irAMisiones);
    connect(pantallaJuego, &PantallaJuego::solicitaSiguienteNivel, this, &MainWindow::siguienteNivelSolicitado);
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

void MainWindow::configurarMisiones(){
    pantallaMisiones = new PantallaMisiones(this);
}

void MainWindow::irAJuego(){
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(1);
    pantallaJuego->setFocus();
}

void MainWindow::irAInstrucciones(){
    stack->setCurrentIndex(2);
}

void MainWindow::irAMisiones(){
    stack->setCurrentIndex(3);
}

void MainWindow::irAMenu(){
    stack->setCurrentIndex(0);
}

void MainWindow::salirDelJuego(){
    QApplication::quit();
}

void MainWindow::mision1Elegida(){
    // La Mision 1 es el nivel del helicoptero que ya esta implementado.
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(1);
    pantallaJuego->setFocus();
}

void MainWindow::mision2Elegida(){
    // RECORDATORIO: aun no existe el Nivel 2 (Valle de Conflicto).
    cout << "Mision 2 seleccionada: nivel aun no implementado" << endl;
}

void MainWindow::mision3Elegida(){
    // RECORDATORIO: aun no existe el Nivel 3 (Base Enemiga bajo Tormenta).
    cout << "Mision 3 seleccionada: nivel aun no implementado" << endl;
}

void MainWindow::siguienteNivelSolicitado(){
    // RECORDATORIO: mientras no exista el Nivel 2, "Siguiente Nivel"
    // reinicia el Nivel 1. Cuando se implemente el Nivel 2, aqui se debe
    // cambiar a la pantalla de ese nivel en vez de reiniciar este.
    cout << "Siguiente nivel aun no implementado, reiniciando Nivel 1" << endl;
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(1);
    pantallaJuego->setFocus();
}
