#ifndef CIVIL_H
#define CIVIL_H
#include <QGraphicsItem>
#include <QPainter>

class Civil : public QGraphicsItem {
public:
    // cantidadPersonas: cuantas personas representa este grupo/rectangulo.
    // Se pinta como un solo bloque para no generar un objeto por persona;
    // mas adelante este paint() se puede reemplazar por una imagen (QPixmap).
    Civil(qreal posX, qreal posY, int cantidadPersonas = 1);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool estaRescatado() const;
    bool estaActivo() const;
    bool estaAplastado() const;
    void rescatar();
    void aplastar();
    void desactivar();
    bool verificarCercania(qreal helicX, qreal helicY, qreal distanciaMax);

    // Se llama cada frame: desplaza al civil junto con el scroll del
    // escenario para que se mantenga "parado" sobre su edificio.
    void actualizar(qreal velocidadScroll);

    // Igual que Obstaculo::fueraDePantalla(): indica si ya salio de la
    // vista por la izquierda y debe liberarse de memoria.
    bool fueraDePantalla() const;

    int getPuntosRescate() const { return puntosRescate; }
    int getCantidadPersonas() const { return cantidadPersonas; }

private:
    bool rescatado;
    bool activo;
    bool aplastado;
    int puntosRescate;
    int cantidadPersonas;
    double tiempoParpadeo;

    // RECORDATORIO: animacion de parpadeo cuando quede poco tiempo en el nivel
    // RECORDATORIO: reemplazar paint() por una imagen (QPixmap) de civiles
};
#endif // CIVIL_H