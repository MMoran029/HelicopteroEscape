#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include "helicoptero.h"

class PantallaJuego : public QWidget {
    Q_OBJECT
public:
    PantallaJuego(QWidget *parent = nullptr);

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
    static const int ANCHO_ESCENA = 800;
    static const int ALTO_ESCENA = 500;

    void configurarEscena();

    // RECORDATORIO: cuando existan Obstaculo y Civil, esta clase debe
    // guardar sus listas (idealmente con punteros dobles para los
    // obstaculos) y revisar colisiones entre todos los objetos dentro
    // de actualizarJuego().
};
#endif // PANTALLAJUEGO_H