#ifndef OBSTACULOMOVIL_H
#define OBSTACULOMOVIL_H

#include "obstaculo.h"
#include <QPixmap>

// Obstaculo que ademas del scroll horizontal, oscila verticalmente
// (ej. dron o enemigo que patrulla arriba/abajo).
class ObstaculoMovil : public Obstaculo
{
public:
    ObstaculoMovil(qreal posX, qreal posY, qreal ancho, qreal alto,
                   qreal amplitud, qreal velocidadVertical);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void actualizar(qreal velocidadScroll, int altoEscena) override;

    // Calcula el ancho correcto segun la proporcion real de la imagen,
    // para no deformarla al elegir un alto aleatorio.
    static qreal calcularAncho(qreal alto);

private:
    qreal posYInicial;
    qreal amplitud;
    qreal velocidadVertical;
    qreal anguloOscilacion;
    QPixmap imagen;

    static QPixmap obtenerImagen();
};

#endif // OBSTACULOMOVIL_H
