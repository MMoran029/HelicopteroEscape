#include "bidon.h"

QPixmap Bidon::obtenerImagen(){
    return QPixmap(":/imagenes/Imagenes/BidonGasolina.png");
}

Bidon::Bidon(qreal posX, qreal posY): recogido(false), cantidadCombustible(25){
    setPos(posX, posY);

    imagen = obtenerImagen();

    const qreal ALTO_OBJETIVO = alturaVisual();
    if(imagen.height() > 0){
        qreal proporcion = static_cast<qreal>(imagen.width()) / static_cast<qreal>(imagen.height());
        altoImagen = ALTO_OBJETIVO;
        anchoImagen = ALTO_OBJETIVO * proporcion;
    }else{
        anchoImagen = 24;
        altoImagen = 30;
    }
}

QRectF Bidon::boundingRect() const{
    return QRectF(-anchoImagen / 2, -altoImagen / 2, anchoImagen, altoImagen);
}

void Bidon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
}

void Bidon::actualizar(qreal velocidadScroll){
    setPos(x() - velocidadScroll, y());
}

bool Bidon::fueraDePantalla() const{
    return x() < -anchoImagen;
}

bool Bidon::estaRecogido() const{
    return recogido;
}

void Bidon::recoger(){
    if(recogido == false){
        recogido = true;
    }
}