#include "obstaculoestatico.h"

QPixmap ObstaculoEstatico::obtenerImagen(int variante){
    switch (variante) {
    case 2:
        return QPixmap(":/imagenes/Imagenes/Edificio_2.png");
    case 3:
        return QPixmap(":/imagenes/Imagenes/Edificio_3.png");
    default:
        return QPixmap(":/imagenes/Imagenes/Edificio_1.png");
    }
}

qreal ObstaculoEstatico::calcularAncho(qreal alto, int variante){
    QPixmap referencia = obtenerImagen(variante);
    if (referencia.height() == 0) {
        return alto; // resguardo por si la imagen no cargo
    }
    qreal proporcion = static_cast<qreal>(referencia.width()) / static_cast<qreal>(referencia.height());
    return alto * proporcion;
}

ObstaculoEstatico::ObstaculoEstatico(qreal posX, qreal posY, qreal alto, int variante)
    : Obstaculo(posX, posY, calcularAncho(alto, variante), alto), civilAsociado(nullptr)
{
    imagen = obtenerImagen(variante);
}

void ObstaculoEstatico::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
}
