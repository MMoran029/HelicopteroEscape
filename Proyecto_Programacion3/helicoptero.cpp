#include "helicoptero.h"
#include <algorithm>

Helicoptero::Helicoptero()
    : velocidadY(0),
    aceleracionSubida(0.6), aceleracionBajada(0.5),
    gravedad(0.35),
    subiendo(false), bajando(false),
    angulo(0)
{
}

QRectF Helicoptero::boundingRect() const
{
    return QRectF(-30, -15, 65, 35);
}

void Helicoptero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    painter->setBrush(QColor(60, 140, 60));
    painter->setPen(Qt::black);
    painter->drawRoundedRect(-20, -10, 40, 20, 5, 5);

    painter->drawLine(20, 0, 35, -5);

    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(-30, -12, 30, -12);

    painter->drawLine(-15, 10, -15, 16);
    painter->drawLine(15, 10, 15, 16);
    painter->drawLine(-15, 16, 15, 16);
}

void Helicoptero::setSubiendo(bool valor)
{
    subiendo = valor;
}

void Helicoptero::setBajando(bool valor)
{
    bajando = valor;
}

void Helicoptero::actualizarFisica(int altoEscena)
{
    if (subiendo) {
        velocidadY -= aceleracionSubida;
    } else if (bajando) {
        velocidadY += aceleracionBajada + gravedad;
    } else {
        velocidadY += gravedad;
    }

    velocidadY = std::max(-10.0, std::min(velocidadY, 10.0));

    qreal nuevaY = y() + velocidadY;

    if (nuevaY < 15) {
        nuevaY = 15;
        velocidadY = 0;
    }

    if (nuevaY > altoEscena - 15) {
        nuevaY = altoEscena - 15;
        velocidadY = 0;
    }

    // La posicion X ya no cambia: el helicoptero se queda fijo horizontalmente
    // mientras el "avance" lo dan los obstaculos que se desplazan hacia el.
    setPos(x(), nuevaY);

    angulo = std::max(-15.0, std::min(15.0, -velocidadY * 1.5));
    setRotation(angulo);
}
