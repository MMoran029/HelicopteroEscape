#include "nivel2.h"

Nivel2::Nivel2(QWidget *parent) : PantallaJuego(parent, 2){
    // El sprite del helicoptero se actualiza aqui, no en el
    // constructor de PantallaJuego: llamar a un metodo virtual desde
    // el constructor base no dispatchearia a Nivel2 todavia.
    actualizarSpriteJugador();
}

bool Nivel2::jugadorTieneArmas() const{
    return true;
}

double Nivel2::probabilidadEnemigoArmado() const{
    return 0.5;
}

bool Nivel2::usaEstructuraBloqueadora() const{
    return true;
}

void Nivel2::obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const{
    fuerzaX = -0.23;
    fuerzaY = 0.0;
}