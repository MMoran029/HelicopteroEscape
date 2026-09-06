#ifndef PANTALLAMISIONES_H
#define PANTALLAMISIONES_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include "crearboton.h"
class PantallaMisiones : public QWidget {
    Q_OBJECT
public:
    PantallaMisiones(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void mision1Presionado();
    void mision2Presionado();
    void mision3Presionado();
    void volverPresionado();
private:
    QPixmap fondoMisiones;
    CrearBoton *botonMision1;
    CrearBoton *botonMision2;
    CrearBoton *botonMision3;
    CrearBoton *botonVolver;
    void configurarElementos();
    void acomodarElementos();
};
#endif // PANTALLAMISIONES_H