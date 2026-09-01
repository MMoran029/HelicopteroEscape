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

private:
    QPixmap fondoMenu;
    CrearBoton *botonJugar;
    CrearBoton *botonMisiones;
    CrearBoton *botonRanking;
    CrearBoton *botonInstrucciones;
    CrearBoton *botonSalir;

    void configurarBotones();
    void acomodarBotones();

    // RECORDATORIO: conectar botonMisiones, botonRanking,
    // botonInstrucciones y botonSalir a su funcionalidad real cuando
    // existan esas pantallas (mision actual, tabla de ranking,
    // instrucciones, y cierre controlado del programa).
};
#endif // MENUPRINCIPAL_H