#include "obstaculomovil.h"
#include <cmath>
#include <QTransform>

QPixmap ObstaculoMovil::obtenerImagen(){
    return QPixmap(":/imagenes/Imagenes/H_Enemigo.png");
}

qreal ObstaculoMovil::calcularAncho(qreal alto){
    QPixmap referencia = obtenerImagen();
    if (referencia.height() == 0) {
        return alto;
    }
    qreal proporcion = static_cast<qreal>(referencia.width()) / static_cast<qreal>(referencia.height());
    return alto * proporcion;
}

ObstaculoMovil::ObstaculoMovil(qreal posX, qreal posY, qreal ancho, qreal alto,
                               qreal amplitud, qreal velocidadVertical)
    : Obstaculo(posX, posY, ancho, alto),
    posYInicial(posY),
    amplitud(amplitud),
    velocidadVertical(velocidadVertical),
    anguloOscilacion(0)
{
    QPixmap original = obtenerImagen();

    // La imagen original mira hacia la derecha; se refleja para que mire
    // hacia la izquierda, su direccion real de vuelo (los enemigos se
    // desplazan de derecha a izquierda por el scroll del escenario).
    imagen = original.transformed(QTransform().scale(-1, 1));
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
