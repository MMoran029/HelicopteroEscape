#ifndef NIVEL2_H
#define NIVEL2_H
#include "pantallajuego.h"

// Nivel 2: el jugador y los helicopteros enemigos ya tienen armas
// (los rojos pueden salir armados o desarmados, 50/50), aparece una
// estructura bloqueadora que hay que destruir si o si para poder
// terminar la mision, y el helicoptero es empujado levemente hacia
// atras todo el tiempo (hay que contrarrestarlo con los controles).
class Nivel2 : public PantallaJuego
{
public:
    Nivel2(QWidget *parent = nullptr);

protected:
    bool jugadorTieneArmas() const override;
    double probabilidadEnemigoArmado() const override;
    bool usaEstructuraBloqueadora() const override;
    void obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const override;
};
#endif // NIVEL2_H