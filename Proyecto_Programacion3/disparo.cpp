#include "disparo.h"

Disparo::Disparo(qreal posX, qreal posY, qreal velocidadX, bool disparoDeJugador)
    : velocidadX(velocidadX), deJugador(disparoDeJugador){
    setPos(posX, posY);

    if(deJugador == true){
        imagen = QPixmap(":/imagenes/Imagenes/Bala_Jugador.png");
    } else {
        imagen = QPixmap(":/imagenes/Imagenes/Bala_Enemigo.png");
    }

    if(imagen.width() > 0 && imagen.height() > 0){
        ancho = imagen.width();
        alto = imagen.height();
    } else {
        ancho = 14;
        alto = 4;
    }
}

QRectF Disparo::boundingRect() const{
    return QRectF(-ancho / 2, -alto / 2, ancho, alto);
}

void Disparo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    // El sprite de bala viene orientado hacia la derecha; si el disparo
    // es enemigo (viaja hacia la izquierda), se refleja horizontalmente.
    painter->drawPixmap(boundingRect().toRect(), imagen);
}

void Disparo::actualizar(){
    setPos(x() + velocidadX, y());
}

bool Disparo::fueraDePantalla(int anchoEscena) const{
    if(x() < -20){
        return true;
    }
    if(x() > anchoEscena + 20){
        return true;
    }
    return false;
}