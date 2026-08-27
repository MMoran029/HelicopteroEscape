#include "obstaculomovil.h"
#include <cmath>

ObstaculoMovil::ObstaculoMovil(qreal posX, qreal posY, qreal ancho, qreal alto,
                               qreal amplitud, qreal velocidadVertical)
    : Obstaculo(posX, posY, ancho, alto),
    posYInicial(posY),
    amplitud(amplitud),
    velocidadVertical(velocidadVertical),
    anguloOscilacion(0)
{
}

void ObstaculoMovil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    qreal mitadAncho = ancho / 2;
    qreal mitadAlto = alto / 2;

    // Cuerpo del helicoptero enemigo.
    painter->setBrush(QColor(150, 30, 30));
    painter->setPen(Qt::black);
    painter->drawRoundedRect(-mitadAncho, -mitadAlto * 0.4, ancho, alto * 0.7, 4, 4);

    // Barra del rotor principal.
    painter->setPen(QPen(Qt::black, 2));
    painter->drawLine(-mitadAncho, -mitadAlto * 0.4, mitadAncho, -mitadAlto * 0.4);

    // Cola hacia atras (queda mirando a la izquierda, de frente al jugador).
    painter->drawLine(-mitadAncho, 0, -mitadAncho * 0.5, -mitadAlto * 0.7);
}

void ObstaculoMovil::actualizar(qreal velocidadScroll, int altoEscena)
{
    Q_UNUSED(altoEscena);

    // Desplazamiento horizontal igual que cualquier obstaculo (scroll).
    qreal nuevaX = x() - velocidadScroll;

    // Oscilacion vertical tipo pendulo alrededor de su posicion inicial.
    anguloOscilacion += velocidadVertical;
    qreal nuevaY = posYInicial + amplitud * std::sin(anguloOscilacion);

    setPos(nuevaX, nuevaY);
}
