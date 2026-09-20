#include "obstaculoestatico.h"

QPixmap ObstaculoEstatico::obtenerImagen(int variante, int nivel){
    if(nivel == 2){
        if(variante == 1){
            return QPixmap(":/imagenes/Imagenes/Torre_1.png");
        }
        return QPixmap(":/imagenes/Imagenes/Torre_2.png");
    }
    if(nivel == 3){
        if(variante == 1){
            return QPixmap(":/imagenes/Imagenes/Base_1.png");
        }
        if(variante == 2){
            return QPixmap(":/imagenes/Imagenes/Base_2.png");
        }
        return QPixmap(":/imagenes/Imagenes/Base_3.png");
    }
    if(variante == 2){
        return QPixmap(":/imagenes/Imagenes/Edificio_2.png");
    }
    if(variante == 3){
        return QPixmap(":/imagenes/Imagenes/Edificio_3.png");
    }
    return QPixmap(":/imagenes/Imagenes/Edificio_1.png");
}

qreal ObstaculoEstatico::calcularAncho(qreal alto, int variante, int nivel){
    QPixmap referencia = obtenerImagen(variante, nivel);
    if(referencia.height() == 0){
        return alto;
    }
    qreal proporcion = static_cast<qreal>(referencia.width()) / static_cast<qreal>(referencia.height());
    return alto * proporcion;
}

ObstaculoEstatico::ObstaculoEstatico(qreal posX, qreal posY, qreal alto, int variante, int nivel)
    : Obstaculo(posX, posY, calcularAncho(alto, variante, nivel), alto), civilAsociado(nullptr)
{
    imagen = obtenerImagen(variante, nivel);
}

void ObstaculoEstatico::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
}
