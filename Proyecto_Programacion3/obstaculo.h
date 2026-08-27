#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsItem>
#include <QPainter>

// Clase base abstracta para todos los obstaculos del juego.
// ObstaculoEstatico y ObstaculoMovil heredan de esta clase.
class Obstaculo : public QGraphicsItem
{
public:
    Obstaculo(qreal posX, qreal posY, qreal ancho, qreal alto);
    virtual ~Obstaculo();

    QRectF boundingRect() const override;

    // Cada tipo de obstaculo dibuja su propia forma (obligatorio implementarlo).
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override = 0;

    // Se llama en cada frame. La version base solo desplaza el obstaculo
    // hacia la izquierda segun la velocidad de scroll de la escena.
    // ObstaculoMovil la redefine para agregar movimiento vertical propio.
    virtual void actualizar(qreal velocidadScroll, int altoEscena);

    // Indica si el obstaculo ya salio completamente de la pantalla por la izquierda,
    // momento en el cual MainWindow debe liberarlo de memoria (delete).
    bool fueraDePantalla() const;

protected:
    qreal ancho;
    qreal alto;
};

#endif // OBSTACULO_H
