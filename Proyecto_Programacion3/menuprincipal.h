#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H
#include <QWidget>
#include <QPixmap>
#include <QResizeEvent>
#include <QPaintEvent>
#include "crearboton.h"

class MenuPrincipal : public QWidget {
    Q_OBJECT
public:
    MenuPrincipal(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void jugarPresionado();
    void instruccionesPresionado();
    void salirPresionado();
    void misionesPresionado();

private:
    QPixmap fondoMenu;
    CrearBoton *botonJugar;
    CrearBoton *botonMisiones;
    CrearBoton *botonRanking;
    CrearBoton *botonInstrucciones;
    CrearBoton *botonSalir;

    void configurarBotones();
    void acomodarBotones();

    // RECORDATORIO: conectar botonRanking a su funcionalidad real cuando
    // exista la tabla de ranking.
};
#endif // MENUPRINCIPAL_H