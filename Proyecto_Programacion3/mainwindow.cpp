#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    helicoptero->setPos(100, ALTO_ESCENA / 2);
    escena->addItem(helicoptero);

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &MainWindow::actualizarJuego);
    timerJuego->start(16);

    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
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
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(true);
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
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(false);
        break;
    }

    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::actualizarJuego()
{
    helicoptero->actualizarFisica(ANCHO_ESCENA, ALTO_ESCENA);
}