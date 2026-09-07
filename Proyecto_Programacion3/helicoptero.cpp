#include "helicoptero.h"
#include <algorithm>

Helicoptero::Helicoptero()
    : velocidadY(0), velocidadX(0),
    aceleracionSubida(0.30), aceleracionBajada(0.10),
    aceleracionHorizontal(0.4), frenoHorizontal(0.2),
    gravedad(0.12),
    subiendo(false), bajando(false), izquierda(false), derecha(false),
    angulo(0), enSuelo(false)
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

}

void Helicoptero::setSubiendo(bool valor)
{
    subiendo = valor;
}

void Helicoptero::setBajando(bool valor)
{
    bajando = valor;
}

void Helicoptero::setIzquierda(bool valor)
{
    izquierda = valor;
}

void Helicoptero::setDerecha(bool valor)
{
    derecha = valor;
}

void Helicoptero::aterrizarSobre(qreal nuevaX, qreal nuevaY)
{
    setPos(nuevaX, nuevaY);
    velocidadY = 0; // se detiene la caida en seco, como si tocara una superficie solida
}

void Helicoptero::actualizarFisica(int anchoEscena, int altoEscena)
{
    if (subiendo) {
        velocidadY -= aceleracionSubida;
    } else if (bajando) {
        velocidadY += aceleracionBajada + gravedad;
    } else {
        velocidadY += gravedad;
    }

    velocidadY = std::max(-6.0, std::min(velocidadY, 6.0));

    if (izquierda) {
        velocidadX -= aceleracionHorizontal;
    } else if (derecha) {
        velocidadX += aceleracionHorizontal;
    } else {
        if (velocidadX > 0) {
            velocidadX = std::max(0.0, velocidadX - frenoHorizontal);
        } else if (velocidadX < 0) {
            velocidadX = std::min(0.0, velocidadX + frenoHorizontal);
        }
    }

    velocidadX = std::max(-8.0, std::min(velocidadX, 8.0));

    qreal nuevaY = y() + velocidadY;
    qreal nuevaX = x() + velocidadX;

    enSuelo = false;

    if (nuevaY < 15) {
        nuevaY = 15;
        velocidadY = 0;
    }

    if (nuevaY > altoEscena - 15) {
        nuevaY = altoEscena - 15;
        velocidadY = 0;
        enSuelo = true; // el helicoptero quedo pegado al piso: derrota inmediata
    }

    if (nuevaX < 30) {
        nuevaX = 30;
        velocidadX = 0;
    }

    if (nuevaX > anchoEscena - 35) {
        nuevaX = anchoEscena - 35;
        velocidadX = 0;
    }

    setPos(nuevaX, nuevaY);

    angulo = std::max(-15.0, std::min(15.0, -velocidadY * 1.5));
    setRotation(angulo);
}