#include "obstaculomovil.h"
#include <cmath>

QPixmap ObstaculoMovil::obtenerImagen(bool conArmas){
    if(conArmas == true){
        return QPixmap(":/imagenes/Imagenes/H_Enemigo_Armas.png");
    }
    return QPixmap(":/imagenes/Imagenes/H_Enemigo.png");
}

qreal ObstaculoMovil::calcularAncho(qreal alto, bool conArmas){
    QPixmap referencia = obtenerImagen(conArmas);
    if (referencia.height() == 0) {
        return alto;
    }
    qreal proporcion = static_cast<qreal>(referencia.width()) / static_cast<qreal>(referencia.height());
    return alto * proporcion;
}

ObstaculoMovil::ObstaculoMovil(qreal posX, qreal posY, qreal ancho, qreal alto,
                               qreal amplitud, qreal velocidadVertical, bool conArmas)
    : Obstaculo(posX, posY, ancho, alto),
    posYInicial(posY),
    amplitud(amplitud),
    velocidadVertical(velocidadVertical),
    anguloOscilacion(0),
    conArmas(conArmas)
{
    // El sprite ya viene orientado hacia la izquierda (direccion real
    // de vuelo de los enemigos, que se desplazan por el scroll), asi
    // que se usa tal cual, sin reflejar.
    imagen = obtenerImagen(conArmas);
}

void ObstaculoMovil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
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