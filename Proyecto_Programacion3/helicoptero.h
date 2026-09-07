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

    // Indica si en el ultimo frame el helicoptero quedo pegado al piso
    // de la escena. PantallaJuego usa esto para terminar la partida
    // de inmediato (choque contra el suelo = derrota automatica).
    bool tocoElSuelo() const { return enSuelo; }

    // Posa el helicoptero exactamente sobre una superficie (por ejemplo
    // el techo de un edificio), deteniendo su caida en seco. La usa
    // PantallaJuego cuando detecta un aterrizaje seguro (sin choque).
    void aterrizarSobre(qreal nuevaX, qreal nuevaY);

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
    bool enSuelo;
};

#endif // HELICOPTERO_H