#include "obstaculo.h"

Obstaculo::Obstaculo(qreal posX, qreal posY, qreal ancho, qreal alto)
    : ancho(ancho), alto(alto)
{
    setPos(posX, posY);
}

Obstaculo::~Obstaculo()
{
    // Destructor virtual: permite que 'delete' sobre un puntero base
    // (Obstaculo*) llame correctamente al destructor de la clase derivada.
}

QRectF Obstaculo::boundingRect() const
{
    return QRectF(-ancho / 2, -alto / 2, ancho, alto);
}

void Obstaculo::actualizar(qreal velocidadScroll, int altoEscena)
{
    Q_UNUSED(altoEscena);
    setPos(x() - velocidadScroll, y());
}

bool Obstaculo::fueraDePantalla() const
{
    return (x() + ancho / 2) < 0;
}
