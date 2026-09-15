#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipal.h"
#include "pantallajuego.h"
#include "nivel2.h"
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
    void irAMisiones();
    void irAMenu();
    void salirDelJuego();
    void mision1Elegida();
    void mision2Elegida();
    void mision3Elegida();
    void siguienteNivelSolicitado();

private:
    QStackedWidget *stack;
    MenuPrincipal *menu;
    PantallaJuego *pantallaJuego;
    Nivel2 *pantallaNivel2;
    PantallaInstrucciones *pantallaInstrucciones;
    PantallaMisiones *pantallaMisiones;

    void configurarMenu();
    void configurarJuego();
    void configurarNivel2();
    void configurarInstrucciones();
    void configurarMisiones();

    // RECORDATORIO: mision3Elegida() queda como marcador de posicion
    // hasta que exista el Nivel 3.
};
#endif // MAINWINDOW_H