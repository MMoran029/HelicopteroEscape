#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipal.h"
#include "pantallajuego.h"
#include "pantallainstrucciones.h"
#include "pantallamisiones.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void irAJuego();
    void irAInstrucciones();
    void irAMenu();
    void salirDelJuego();
    void irAMisiones();
    void mision1Elegida();
    void mision2Elegida();
    void mision3Elegida();
private:
    QStackedWidget *stack;
    MenuPrincipal *menu;
    PantallaJuego *pantallaJuego;
    PantallaInstrucciones *pantallaInstrucciones;
    PantallaMisiones *pantallaMisiones;
    void configurarMisiones();
    void configurarMenu();
    void configurarJuego();
    void configurarInstrucciones();

};
#endif // MAINWINDOW_H