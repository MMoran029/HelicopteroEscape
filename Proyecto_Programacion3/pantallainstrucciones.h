#ifndef PANTALLAINSTRUCCIONES_H
#define PANTALLAINSTRUCCIONES_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include "crearboton.h"
class PantallaInstrucciones : public QWidget {
    Q_OBJECT
public:
    PantallaInstrucciones(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
signals:
    void volverPresionado();
private:
    QPixmap fondoInstrucciones;
    CrearBoton *botonVolver;
    void configurarElementos();
    void acomodarElementos();
};
#endif // PANTALLAINSTRUCCIONES_H