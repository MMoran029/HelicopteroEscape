#include "estructurabloqueadora.h"

QPixmap EstructuraBloqueadora::obtenerImagen(){
    return QPixmap(":/imagenes/Imagenes/EstructuraBloqueadora.png");
}

qreal EstructuraBloqueadora::calcularAncho(qreal alto){
    QPixmap referencia = obtenerImagen();
    if(referencia.height() == 0){
        return alto;
    }
    qreal proporcion = static_cast<qreal>(referencia.width()) / static_cast<qreal>(referencia.height());
    return alto * proporcion;
}

EstructuraBloqueadora::EstructuraBloqueadora(qreal posX, qreal posY, qreal alto)
    : Obstaculo(posX, posY, calcularAncho(alto), alto),
    vidasRestantes(10), posXBloqueo(600){
    imagen = obtenerImagen();
}

void EstructuraBloqueadora::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
}
void EstructuraBloqueadora::actualizar(qreal velocidadScroll, int altoEscena){
    Q_UNUSED(altoEscena);
    setPos(x() - velocidadScroll, y());
}

bool EstructuraBloqueadora::recibirImpacto(){
    vidasRestantes--;
    return vidasRestantes <= 0;
}

bool EstructuraBloqueadora::estaDestruida() const{
    return vidasRestantes <= 0;
}