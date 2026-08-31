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
    void setIzquierda(bool valor);
    void setDerecha(bool valor);
    void actualizarFisica(int anchoEscena, int altoEscena);

    double getVelocidadY() const { return velocidadY; }
    double getVelocidadX() const { return velocidadX; }

private:
    double velocidadY;
    double velocidadX;
    double aceleracionSubida;
    double aceleracionBajada;
    double aceleracionHorizontal;
    double frenoHorizontal;
    double gravedad;
    bool subiendo;
    bool bajando;
    bool izquierda;
    bool derecha;
    double angulo;
};

#endif // HELICOPTERO_H