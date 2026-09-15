#ifndef DISPARO_H
#define DISPARO_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>

// Proyectil disparado por el jugador o por un helicoptero enemigo
// armado. No usa sprite: se dibuja como una forma simple, coloreada
// segun quien dispara (amarillo jugador, rojo enemigo) para
// diferenciarlos de un vistazo.
class Disparo : public QGraphicsItem {
public:
    Disparo(qreal posX, qreal posY, qreal velocidadX, bool disparoDeJugador);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Se llama cada frame: el disparo viaja en linea recta segun su
    // propia velocidad, independiente del scroll del escenario.
    void actualizar();

    bool fueraDePantalla(int anchoEscena) const;
    bool esDeJugador() const { return deJugador; }

private:
    qreal velocidadX;
    bool deJugador;
    qreal ancho;
    qreal alto;
    QPixmap imagen;
};
#endif // DISPARO_H