#include "helicoptero.h"
#include <algorithm>
#include <QTransform>

Helicoptero::Helicoptero()
    : velocidadY(0), velocidadX(0),
    aceleracionSubida(0.30), aceleracionBajada(0.10),
    aceleracionHorizontal(0.4), frenoHorizontal(0.2),
    gravedad(0.12),
    subiendo(false), bajando(false), izquierda(false), derecha(false),
    angulo(0), enSuelo(false)
{
    QPixmap original(":/imagenes/Imagenes/H_Jugador.png");

    // La imagen original mira hacia la izquierda; se refleja horizontal-
    // mente para que el helicoptero del jugador mire hacia la derecha
    // (su direccion de avance, ya que el escenario se desplaza a la
    // izquierda por debajo de el).
    imagen = original.transformed(QTransform().scale(-1, 1));

    const qreal ANCHO_OBJETIVO = 84.0;
    if (imagen.height() > 0) {
        qreal proporcion = static_cast<qreal>(imagen.width()) / static_cast<qreal>(imagen.height());
        anchoImagen = ANCHO_OBJETIVO;
        altoImagen = ANCHO_OBJETIVO / proporcion;
    } else {
        anchoImagen = 65;
        altoImagen = 35;
    }
}

QRectF Helicoptero::boundingRect() const
{
    return QRectF(-anchoImagen / 2, -altoImagen / 2, anchoImagen, altoImagen);
}

void Helicoptero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawPixmap(boundingRect().toRect(), imagen);
}

void Helicoptero::setSubiendo(bool valor)
{
    subiendo = valor;
}

void Helicoptero::setBajando(bool valor)
{
    bajando = valor;
}

void Helicoptero::setIzquierda(bool valor)
{
    izquierda = valor;
}

void Helicoptero::setDerecha(bool valor)
{
    derecha = valor;
}

void Helicoptero::aterrizarSobre(qreal nuevaX, qreal nuevaY)
{
    setPos(nuevaX, nuevaY);
    velocidadY = 0; // se detiene la caida en seco, como si tocara una superficie solida
}

void Helicoptero::actualizarFisica(int anchoEscena, int altoEscena)
{
    if (subiendo) {
        velocidadY -= aceleracionSubida;
    } else if (bajando) {
        velocidadY += aceleracionBajada + gravedad;
    } else {
        velocidadY += gravedad;
    }

    velocidadY = std::max(-6.0, std::min(velocidadY, 6.0));

    if (izquierda) {
        velocidadX -= aceleracionHorizontal;
    } else if (derecha) {
        velocidadX += aceleracionHorizontal;
    } else {
        if (velocidadX > 0) {
            velocidadX = std::max(0.0, velocidadX - frenoHorizontal);
        } else if (velocidadX < 0) {
            velocidadX = std::min(0.0, velocidadX + frenoHorizontal);
        }
    }

    velocidadX = std::max(-8.0, std::min(velocidadX, 8.0));

    qreal nuevaY = y() + velocidadY;
    qreal nuevaX = x() + velocidadX;

    enSuelo = false;

    qreal margenVertical = altoImagen / 2;
    qreal margenHorizontal = anchoImagen / 2;

    if (nuevaY < margenVertical) {
        nuevaY = margenVertical;
        velocidadY = 0;
    }

    if (nuevaY > altoEscena - margenVertical) {
        nuevaY = altoEscena - margenVertical;
        velocidadY = 0;
        enSuelo = true; // el helicoptero quedo pegado al piso: derrota inmediata
    }

    if (nuevaX < margenHorizontal) {
        nuevaX = margenHorizontal;
        velocidadX = 0;
    }

    if (nuevaX > anchoEscena - margenHorizontal) {
        nuevaX = anchoEscena - margenHorizontal;
        velocidadX = 0;
    }

    setPos(nuevaX, nuevaY);

    angulo = std::max(-15.0, std::min(15.0, -velocidadY * 1.5));
    setRotation(angulo);
}