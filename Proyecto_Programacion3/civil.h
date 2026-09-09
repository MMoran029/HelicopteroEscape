#ifndef CIVIL_H
#define CIVIL_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>

class Civil : public QGraphicsItem {
public:
    // cantidadPersonas: cuantas personas representa este grupo (1 a 3).
    // Un solo objeto Civil dibuja la imagen correspondiente (Persona_1,
    // Persona_2 o Persona_3) en vez de instanciar un objeto por persona.
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

    // Alto visual fijo que usan todas las imagenes de civiles (ver
    // constructor). PantallaJuego lo usa para calcular con precision
    // en que Y colocar el grupo justo sobre el techo de un edificio.
    static qreal alturaVisual() { return 42.0; }

private:
    bool rescatado;
    bool activo;
    bool aplastado;
    int puntosRescate;
    int cantidadPersonas;
    double tiempoParpadeo;

    QPixmap imagen;
    qreal anchoImagen;
    qreal altoImagen;

    static QPixmap obtenerImagen(int cantidadPersonas);

    // RECORDATORIO: animacion de parpadeo cuando quede poco tiempo en el nivel
};
#endif // CIVIL_H