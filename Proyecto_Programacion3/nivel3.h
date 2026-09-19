#ifndef NIVEL3_H
#define NIVEL3_H
#include "pantallajuego.h"
class Nivel3 : public PantallaJuego{
public:
    Nivel3(QWidget *parent = nullptr);

protected:

    bool jugadorTieneArmas() const override;
    double probabilidadEnemigoArmado() const override;
    void obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const override;

private:
    mutable qreal vientoX;
    mutable qreal vientoY;
    mutable int framesRestantesRacha;
};
#endif // NIVEL3_H