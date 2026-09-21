#include "nivel3.h"

Nivel3::Nivel3(QWidget *parent) : PantallaJuego(parent, 3),vientoX(0.0), vientoY(0.0), framesRestantesRacha(0){
    actualizarSpriteJugador();
}

bool Nivel3::jugadorTieneArmas() const{
    return true;
}

double Nivel3::probabilidadEnemigoArmado() const{
    return 1.0; // en el Nivel 3 los enemigos siempre tienen armas
}

void Nivel3::obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const{
    const int FRAMES_POR_RACHA = 45; // cuanto dura cada racha (~1.5s a 60fps)
    const int RANGO = 23;            // -25 a 24 centesimas por eje

    if(framesRestantesRacha <= 0){
        vientoX = (rand() % RANGO - RANGO / 2) / 100.0;
        vientoY = (rand() % RANGO - RANGO / 2) / 100.0;
        framesRestantesRacha = FRAMES_POR_RACHA;
    }

    framesRestantesRacha--;
    fuerzaX = vientoX;
    fuerzaY = vientoY;
}

bool Nivel3::usaEstructuraBloqueadora() const{
    return true;
}