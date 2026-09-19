#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipal.h"
#include "pantallajuego.h"
#include "nivel2.h"
#include "nivel3.h"
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
    Nivel3 *pantallaNivel3;
    PantallaInstrucciones *pantallaInstrucciones;
    PantallaMisiones *pantallaMisiones;

    void configurarMenu();
    void configurarJuego();
    void configurarNivel2();
    void configurarNivel3();
    void configurarInstrucciones();
    void configurarMisiones();
};
#endif // MAINWINDOW_H