#ifndef BIDON_H
#define BIDON_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>
class Bidon : public QGraphicsItem {
public:
    Bidon(qreal posX, qreal posY);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Se llama cada frame para desplazarlo junto con el escenario.
    void actualizar(qreal velocidadScroll);

    // Indica si ya salio de la vista por la izquierda (para liberarlo).
    bool fueraDePantalla() const;

    bool estaRecogido() const;
    void recoger();

    // Cuanto combustible entrega este bidon al ser recogido (0 a 100).
    int getCantidadCombustible() const { return cantidadCombustible; }

    static qreal alturaVisual() { return 30.0; }

private:
    bool recogido;
    int cantidadCombustible;

    QPixmap imagen;
    qreal anchoImagen;
    qreal altoImagen;

    static QPixmap obtenerImagen();
};
#endif // BIDON_H