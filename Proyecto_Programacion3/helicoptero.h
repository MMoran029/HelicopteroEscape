#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPixmap>

class Helicoptero : public QGraphicsItem {
public:
    Helicoptero();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setSubiendo(bool valor);
    void setBajando(bool valor);
    void setIzquierda(bool valor);
    void setDerecha(bool valor);

    // fuerzaExternaX/Y: empuje constante que no depende del jugador
    // (por ejemplo el viento del Nivel 2). Se suma despues del frenado
    // normal, asi que el jugador tiene que contrarrestarlo activamente
    // con los controles en vez de que la friccion lo cancele solo.
    void actualizarFisica(int anchoEscena, int altoEscena, qreal fuerzaExternaX = 0.0, qreal fuerzaExternaY = 0.0);

    // Cambia el sprite a la variante con armas (Nivel 2 y 3) o sin
    // armas (Nivel 1). Se llama una sola vez al armar la pantalla del
    // nivel, no cada frame.
    void setConArmas(bool valor);

    double getVelocidadY() const { return velocidadY; }
    double getVelocidadX() const { return velocidadX; }

    // Indica si en el ultimo frame el helicoptero quedo pegado al piso
    // de la escena. PantallaJuego usa esto para terminar la partida
    // de inmediato (choque contra el suelo = derrota automatica).
    bool tocoElSuelo() const { return enSuelo; }

    // Posa el helicoptero exactamente sobre una superficie (por ejemplo
    // el techo de un edificio), deteniendo su caida en seco. La usa
    // PantallaJuego cuando detecta un aterrizaje seguro (sin choque).
    void aterrizarSobre(qreal nuevaX, qreal nuevaY);
    void reiniciarFisica(); //para evitar ser lanzado a la hora de iniciar de nuevo una partida
private:
    void cargarImagen(const QString &ruta);

    double velocidadY;
    double velocidadX;
    double aceleracionSubida;
    double aceleracionBajada;
    double aceleracionHorizontal;
    double frenoHorizontal;
    double gravedad;
    bool subiendo;
    bool bajando;
    bool izquierda;
    bool derecha;
    double angulo;
    bool enSuelo;

    QPixmap imagen;
    qreal anchoImagen;
    qreal altoImagen;
};

#endif // HELICOPTERO_H