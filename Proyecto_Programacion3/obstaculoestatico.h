#ifndef OBSTACULOESTATICO_H
#define OBSTACULOESTATICO_H

#include "obstaculo.h"

// Obstaculo fijo en su eje vertical (ej. torre, estalagmita, edificio).
// Solo se mueve horizontalmente por el efecto de scroll de la escena.
class ObstaculoEstatico : public Obstaculo
{
public:
    ObstaculoEstatico(qreal posX, qreal posY, qreal ancho, qreal alto);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // OBSTACULOESTATICO_H
