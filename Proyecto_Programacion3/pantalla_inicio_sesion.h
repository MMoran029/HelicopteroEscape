#ifndef PANTALLA_INICIO_SESION_H
#define PANTALLA_INICIO_SESION_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include "crearboton.h"

class pantalla_inicio_sesion : public QWidget {
    Q_OBJECT
public:
    pantalla_inicio_sesion(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void iniciarSesionPresionado();
    void registrarsePresionado();
    void salirPresionado();

private:
    QPixmap fondoInicioSesion;
    CrearBoton *botonIniciarSesion;
    CrearBoton *botonRegistrarse;
    CrearBoton *botonSalir;

    void configurarBotones();
    void acomodarBotones();

    // RECORDATORIO: por ahora se usa el fondo del menu principal.
    // Cambiar por la imagen propia de inicio de sesion cuando exista.
};
#endif // PANTALLA_INICIO_SESION_H