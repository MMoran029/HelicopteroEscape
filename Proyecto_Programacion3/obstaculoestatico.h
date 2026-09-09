#ifndef OBSTACULOESTATICO_H
#define OBSTACULOESTATICO_H

#include "obstaculo.h"
#include <QPixmap>

class Civil; // forward declaration: solo se guarda un puntero

// Obstaculo fijo en su eje vertical (edificio). Solo se mueve
// horizontalmente por el efecto de scroll de la escena. Existen 3
// variantes de imagen (edificio sano, en llamas, y en ruinas); cada
// instancia elige una al crearse.
class ObstaculoEstatico : public Obstaculo
{
public:
    // variante: 1, 2 o 3 (Edificio_1.png, Edificio_2.png, Edificio_3.png).
    // El ancho se calcula automaticamente a partir del alto para
    // respetar la proporcion real de la imagen elegida.
    ObstaculoEstatico(qreal posX, qreal posY, qreal alto, int variante);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Publico para que PantallaJuego pueda calcular el punto de
    // aparicion (fuera de pantalla) antes de construir el objeto.
    static qreal calcularAncho(qreal alto, int variante);

    // Vincula este edificio con el grupo de civiles que aparecio sobre
    // su techo (si lo hubo). PantallaJuego lo usa para saber a quien
    // marcar como abatido si el edificio es destruido por un choque.
    void asociarCivil(Civil *civil) { civilAsociado = civil; }
    Civil *obtenerCivilAsociado() const { return civilAsociado; }

private:
    QPixmap imagen;
    Civil *civilAsociado;

    static QPixmap obtenerImagen(int variante);
};

#endif // OBSTACULOESTATICO_H
