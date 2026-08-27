#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsItem>
#include <QPainter>

class Helicoptero : public QGraphicsItem {
public:
    Helicoptero();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setSubiendo(bool valor);
    void setBajando(bool valor);
    void actualizarFisica(int altoEscena);

    double getVelocidadY() const { return velocidadY; }

private:
    double velocidadY;
    double aceleracionSubida;
    double aceleracionBajada;
    double gravedad;
    bool subiendo;
    bool bajando;
    double angulo;
};

#endif // HELICOPTERO_H
