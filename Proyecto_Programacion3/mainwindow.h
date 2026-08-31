#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include "helicoptero.h"

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

    static const int ANCHO_ESCENA = 800;
    static const int ALTO_ESCENA = 500;
};

#endif // MAINWINDOW_H
