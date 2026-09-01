#ifndef CIVIL_H
#define CIVIL_H
#include <QGraphicsItem>
#include <QPainter>

class Civil : public QGraphicsItem {
public:
    Civil(qreal posX, qreal posY);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool estaRescatado() const;
    bool estaActivo() const;
    void rescatar();
    void desactivar();
    bool verificarCercania(qreal helicX, qreal helicY, qreal distanciaMax);

    int getPuntosRescate() const { return puntosRescate; }

private:
    bool rescatado;
    bool activo;
    int puntosRescate;
    double tiempoParpadeo;

    // RECORDATORIO: animacion de parpadeo cuando quede poco tiempo en el nivel
    // RECORDATORIO: sonido o efecto visual al rescatar
};
#endif // CIVIL_H