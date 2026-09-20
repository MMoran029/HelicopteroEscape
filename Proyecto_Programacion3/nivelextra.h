#ifndef NIVELEXTRA_H
#define NIVELEXTRA_H
#include "pantallajuego.h"

// NivelExtra (SUPERVIVENCIA): nivel infinito aparte de la historia.
//
// - No hay civiles: los edificios salen vacios.
// - Nunca hay victoria: solo se termina por derrota. Cada segundo
//   vivo da +5 puntos (ver PantallaJuego::actualizarJuego).
// - Mas enemigos que en los niveles 1-3 (aparecen mas seguido, todos
//   armados y disparan mas rapido) y estructuras bloqueadoras que
//   reaparecen una y otra vez tapando la pantalla.
// - La dificultad sube con el tiempo: el scroll se acelera y los
//   intervalos de aparicion se acortan.
class NivelExtra : public PantallaJuego
{
public:
    NivelExtra(QWidget *parent = nullptr);

protected:
    bool jugadorTieneArmas() const override;
    double probabilidadEnemigoArmado() const override;
    double probabilidadDisparoEnemigoPorFrame() const override;
    bool usaEstructuraBloqueadora() const override;

    bool esInfinito() const override;
    bool generaCiviles() const override;
    int obtenerIntervaloBidon() const override;
    double obtenerConsumoCombustible() const override;
    int obtenerIntervaloEdificioBase() const override;
    int obtenerIntervaloEnemigoBase() const override;
    int obtenerIntervaloEstructura() const override;
    qreal obtenerVelocidadInicial() const override;
    void ajustarDificultad() override;
    void obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const override;
    // Sin barra de progreso: no hay meta que mostrar.
    bool mostrarBarraProgreso() const override;

private:
    // Viento por rachas como en el Nivel 3 (cambia cada ~1.5s).
    mutable qreal vientoX;
    mutable qreal vientoY;
    mutable int framesRestantesRacha;
};
#endif // NIVELEXTRA_H
