#include "obstaculoestatico.h"
#include <QPolygonF>

ObstaculoEstatico::ObstaculoEstatico(qreal posX, qreal posY, qreal ancho, qreal alto)
    : Obstaculo(posX, posY, ancho, alto)
{
}

void ObstaculoEstatico::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    qreal mitadAncho = ancho / 2;
    qreal mitadAlto = alto / 2;

    // Cuerpo principal del edificio en ruinas.
    painter->setBrush(QColor(95, 85, 80));
    painter->setPen(Qt::black);
    painter->drawRect(-mitadAncho, -mitadAlto, ancho, alto);

    // Techo irregular (silueta de escombros), sin salir del bounding rect.
    QPolygonF ruinas;
    ruinas << QPointF(-mitadAncho, -mitadAlto + 20)
           << QPointF(-mitadAncho + ancho * 0.15, -mitadAlto)
           << QPointF(-mitadAncho + ancho * 0.35, -mitadAlto + 14)
           << QPointF(-mitadAncho + ancho * 0.55, -mitadAlto + 2)
           << QPointF(-mitadAncho + ancho * 0.75, -mitadAlto + 16)
           << QPointF(mitadAncho, -mitadAlto + 6)
           << QPointF(mitadAncho, -mitadAlto + 20);
    painter->setBrush(QColor(70, 60, 55));
    painter->drawPolygon(ruinas);

    // Ventanas rotas.
    painter->setBrush(QColor(20, 20, 20));
    for (int i = 0; i < 3; ++i) {
        qreal ventanaX = -mitadAncho + 8 + i * (ancho - 16) / 3.0;
        painter->drawRect(ventanaX, -mitadAlto + 28, 8, 10);
    }
}
