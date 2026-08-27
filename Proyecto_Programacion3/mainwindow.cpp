#include "mainwindow.h"
#include "obstaculoestatico.h"
#include "obstaculomovil.h"
#include <QMessageBox>
#include <QApplication>
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    listaObstaculos(nullptr),
    capacidadObstaculos(0),
    cantidadObstaculos(0),
    contadorFrames(0),
    velocidadScroll(3.5)
{
    setWindowTitle("Helicoptero Rescate - Prototipo");
    resize(ANCHO_ESCENA + 20, ALTO_ESCENA + 40);

    escena = new QGraphicsScene(0, 0, ANCHO_ESCENA, ALTO_ESCENA, this);
    escena->setBackgroundBrush(QBrush(QColor(30, 30, 60)));

    vista = new QGraphicsView(escena, this);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(vista);

    helicoptero = new Helicoptero();
    // El helicoptero queda fijo en X; el avance lo dan los obstaculos moviendose hacia el.
    helicoptero->setPos(150, ALTO_ESCENA / 2);
    escena->addItem(helicoptero);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerJuego->start(16);

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    // Liberar cada obstaculo dinamico y luego el arreglo de punteros.
    for (int i = 0; i < cantidadObstaculos; ++i) {
        delete listaObstaculos[i];
    }
    delete[] listaObstaculos;
}

void MainWindow::agregarObstaculo(Obstaculo *obstaculo)
{
    if (cantidadObstaculos == capacidadObstaculos) {
        int nuevaCapacidad = (capacidadObstaculos == 0) ? 4 : capacidadObstaculos * 2;
        Obstaculo **nuevaLista = new Obstaculo*[nuevaCapacidad];

        for (int i = 0; i < cantidadObstaculos; ++i) {
            nuevaLista[i] = listaObstaculos[i];
        }

        delete[] listaObstaculos; // solo libera el arreglo viejo, no los objetos
        listaObstaculos = nuevaLista;
        capacidadObstaculos = nuevaCapacidad;
    }

    listaObstaculos[cantidadObstaculos] = obstaculo;
    cantidadObstaculos++;
    escena->addItem(obstaculo);
}

void MainWindow::eliminarObstaculo(int indice)
{
    escena->removeItem(listaObstaculos[indice]);
    delete listaObstaculos[indice];

    for (int i = indice; i < cantidadObstaculos - 1; ++i) {
        listaObstaculos[i] = listaObstaculos[i + 1];
    }

    cantidadObstaculos--;
}

void MainWindow::generarObstaculoAleatorio()
{
    int tipo = std::rand() % 2;
    qreal posX = ANCHO_ESCENA + 40;

    if (tipo == 0) {
        // Edificio en ruinas: siempre se levanta desde el suelo.
        qreal alto = 60 + (std::rand() % 120);
        qreal posY = ALTO_ESCENA - alto / 2;
        agregarObstaculo(new ObstaculoEstatico(posX, posY, 45, alto));
    } else {
        // Helicoptero enemigo: patrulla a distintas alturas.
        qreal posY = 80 + (std::rand() % (ALTO_ESCENA - 160));
        qreal amplitud = 40 + (std::rand() % 40);
        qreal velocidadVertical = 0.03 + (std::rand() % 5) * 0.01;
        agregarObstaculo(new ObstaculoMovil(posX, posY, 40, 24, amplitud, velocidadVertical));
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(true);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(true);
        break;
    }

    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(false);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(false);
        break;
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::actualizarJuego()
{
    helicoptero->actualizarFisica(ALTO_ESCENA);

    contadorFrames++;
    if (contadorFrames % 90 == 0) {
        generarObstaculoAleatorio();
    }

    // Recorrido de atras hacia adelante: al eliminar un indice, los que
    // faltan por revisar no cambian de posicion en el arreglo.
    for (int i = cantidadObstaculos - 1; i >= 0; --i) {
        listaObstaculos[i]->actualizar(velocidadScroll, ALTO_ESCENA);

        if (listaObstaculos[i]->fueraDePantalla()) {
            eliminarObstaculo(i);
        }
    }

    // El helicoptero toco el suelo: fin del juego.
    if (helicoptero->y() >= ALTO_ESCENA - 15) {
        finDelJuego("El helicoptero se estrello contra el suelo.");
        return;
    }

    // Colision contra cualquier obstaculo (edificio en ruinas o helicoptero enemigo).
    for (int i = 0; i < cantidadObstaculos; ++i) {
        if (helicoptero->collidesWithItem(listaObstaculos[i])) {
            finDelJuego("El helicoptero choco contra un obstaculo.");
            return;
        }
    }
}

void MainWindow::finDelJuego(const QString &mensaje)
{
    // Detenemos el juego antes de mostrar el letrero.
    timerJuego->stop();

    QMessageBox cuadro(this);
    cuadro.setWindowTitle("Fin del juego");
    cuadro.setIcon(QMessageBox::Critical);
    cuadro.setText(mensaje);
    cuadro.addButton("Cerrar juego", QMessageBox::AcceptRole);
    cuadro.exec();

    // El unico boton del letrero cierra el programa por completo.
    QApplication::quit();
}
