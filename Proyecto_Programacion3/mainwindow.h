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
    PantallaInstrucciones *pantallaInstrucciones;
    PantallaMisiones *pantallaMisiones;

    void configurarMenu();
    void configurarJuego();
    void configurarInstrucciones();
    void configurarMisiones();

    // RECORDATORIO: mision2Elegida() y mision3Elegida() quedan como
    // marcador de posicion hasta que existan el Nivel 2 y el Nivel 3.
};
#endif // MAINWINDOW_H
