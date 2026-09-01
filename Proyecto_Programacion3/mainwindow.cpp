#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setWindowTitle("Helicoptero Rescate - Prototipo");
    resize(820, 540);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    configurarMenu();
    configurarJuego();

    stack->addWidget(menu);
    stack->addWidget(pantallaJuego);
    stack->setCurrentIndex(0);

    connect(menu, &MenuPrincipal::jugarPresionado, this, &MainWindow::irAJuego);
}

MainWindow::~MainWindow(){
}

void MainWindow::configurarMenu(){
    menu = new MenuPrincipal(this);
}

void MainWindow::configurarJuego(){
    pantallaJuego = new PantallaJuego(this);
}

void MainWindow::irAJuego(){
    stack->setCurrentIndex(1);
    pantallaJuego->setFocus();
}