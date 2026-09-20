#include "obstaculo.h"

Obstaculo::Obstaculo(qreal posX, qreal posY, qreal ancho, qreal alto)
    : ancho(ancho), alto(alto)
{
    setPos(posX, posY);
}

Obstaculo::~Obstaculo()
{
    // Destructor virtual: permite que 'delete' sobre un puntero base
    // (Obstaculo*) llame correctamente al destructor de la clase derivada.
}

QRectF Obstaculo::boundingRect() const
{
    return QRectF(-ancho / 2, -alto / 2, ancho, alto);
}

void Obstaculo::actualizar(qreal velocidadScroll, int altoEscena)
{
    Q_UNUSED(altoEscena);
    setPos(x() - velocidadScroll, y());
}

bool Obstaculo::fueraDePantalla() const
{
    // Margen extra: se elimina solo cuando el borde derecho ya quedo
    // varios pixeles mas alla del borde izquierdo de la escena. Sin este
    // margen, el redondeo del dibujado escalado (boundingRect().toRect()
    // + SmoothPixmapTransform) hace que el sprite parezca desvanecerse
    // antes de terminar de salir, muy notorio en pantalla completa.
    static const qreal MARGEN_SALIDA = 10.0;
    return (x() + ancho / 2) < -MARGEN_SALIDA;
}
