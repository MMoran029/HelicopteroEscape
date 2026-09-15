#ifndef ESTRUCTURABLOQUEADORA_H
#define ESTRUCTURABLOQUEADORA_H
#include "obstaculo.h"
#include <QPixmap>

// Estructura enemiga que bloquea el paso. Aparece una sola vez por
// nivel (en Nivel 2 y Nivel 3): avanza con el scroll normal hasta
// llegar a una posicion fija cerca del centro de la pantalla, donde
// se queda plantada -sin seguir avanzando- hasta que el jugador la
// destruye a disparos. Mientras siga viva, el nivel no se puede
// terminar.
class EstructuraBloqueadora : public Obstaculo
{
public:
    EstructuraBloqueadora(qreal posX, qreal posY, qreal alto);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void actualizar(qreal velocidadScroll, int altoEscena) override;

    static qreal calcularAncho(qreal alto);

    // Le resta una vida a la estructura. Devuelve true si con ese
    // impacto quedo destruida.
    bool recibirImpacto();
    bool estaDestruida() const;

private:
    QPixmap imagen;
    int vidasRestantes;
    qreal posXBloqueo;

    static QPixmap obtenerImagen();
};
#endif // ESTRUCTURABLOQUEADORA_H