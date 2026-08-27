#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include "helicoptero.h"
#include "obstaculo.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void actualizarJuego();

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timerJuego;
    Helicoptero *helicoptero;

    // --- Arreglo dinamico de punteros a Obstaculo (puntero doble: Obstaculo**) ---
    // No se usa QList/std::vector: el arreglo se administra manualmente con
    // new/delete, tal como pide el requisito de "matriz dinamica obligatoria".
    Obstaculo **listaObstaculos;
    int capacidadObstaculos;
    int cantidadObstaculos;

    int contadorFrames;
    qreal velocidadScroll;

    void agregarObstaculo(Obstaculo *obstaculo);
    void eliminarObstaculo(int indice);
    void generarObstaculoAleatorio();
    void finDelJuego(const QString &mensaje);

    static const int ANCHO_ESCENA = 800;
    static const int ALTO_ESCENA = 500;
};

#endif // MAINWINDOW_H
