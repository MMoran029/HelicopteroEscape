#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipal.h"
#include "pantallajuego.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void irAJuego();

private:
    QStackedWidget *stack;
    MenuPrincipal *menu;
    PantallaJuego *pantallaJuego;

    /*QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timerJuego;
    Helicoptero *helicoptero;
los movi a PantallaJuego*/

    void configurarMenu();
    void configurarJuego();

    // RECORDATORIO: cuando exista pantalla de Instrucciones y logica de
    // Salir, agregar sus propios slots aqui, similares a irAJuego().
};
#endif // MAINWINDOW_H