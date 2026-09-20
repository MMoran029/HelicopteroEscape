#include "nivelextra.h"
#include <cstdlib>

NivelExtra::NivelExtra(QWidget *parent)
    : PantallaJuego(parent, 4), vientoX(0.0), vientoY(0.0), framesRestantesRacha(0)
{
    // Igual que Nivel2/Nivel3: el sprite con armas se aplica aqui y no
    // en el constructor base (el virtual aun no dispatcheaba alla).
    actualizarSpriteJugador();
    // Oculta la barra de progreso desde el arranque (el virtual ya
    // dispatchea a NivelExtra en este punto).
    aplicarVisibilidadBarras();
}

bool NivelExtra::mostrarBarraProgreso() const
{
    return false;
}

bool NivelExtra::jugadorTieneArmas() const
{
    return true;
}

double NivelExtra::probabilidadEnemigoArmado() const
{
    return 1.0; // en supervivencia todos los enemigos salen armados
}

double NivelExtra::probabilidadDisparoEnemigoPorFrame() const
{
    return 1.0 / 70.0; // disparan mas del doble de rapido que en el Nivel 1
}

bool NivelExtra::usaEstructuraBloqueadora() const
{
    return true;
}

bool NivelExtra::esInfinito() const
{
    return true;
}

bool NivelExtra::generaCiviles() const
{
    return false; // sin civiles en supervivencia
}

int NivelExtra::obtenerIntervaloBidon() const
{
    return 240;
}

double NivelExtra::obtenerConsumoCombustible() const
{
    return 0.055;
}

qreal NivelExtra::obtenerVelocidadInicial() const
{
    return 2.0; // arranca mas rapido que los niveles 1-3
}

int NivelExtra::obtenerIntervaloEdificioBase() const
{
    // Edificios mas seguidos que en la historia (tapan la pantalla
    // junto con las estructuras) y cada vez mas seguidos.
    int segundos = leerFramesSupervivencia() / 60;
    int reduccion = segundos / 10 * 8; // -8 frames cada 10s, tope -40
    if (reduccion > 40) {
        reduccion = 40;
    }
    int intervalo = 100 + (rand() % 80) - reduccion; // 100-180 al inicio
    if (intervalo < 70) {
        intervalo = 70;
    }
    return intervalo;
}

int NivelExtra::obtenerIntervaloEnemigoBase() const
{
    // Enemigos mucho mas seguidos que en la historia (200-380 alla,
    // 90-160 aca) y cada vez mas seguidos.
    int segundos = leerFramesSupervivencia() / 60;
    int reduccion = segundos / 10 * 10; // -10 frames cada 10s, tope -50
    if (reduccion > 50) {
        reduccion = 50;
    }
    int intervalo = 90 + (rand() % 70) - reduccion;
    if (intervalo < 50) {
        intervalo = 50;
    }
    return intervalo;
}

int NivelExtra::obtenerIntervaloEstructura() const
{
    // Una estructura nueva cada ~9 segundos al inicio, cada vez mas
    // seguido hasta un minimo de ~5 segundos.
    int segundos = leerFramesSupervivencia() / 60;
    int intervalo = 550 - (segundos / 10 * 40);
    if (intervalo < 300) {
        intervalo = 300;
    }
    return intervalo;
}

void NivelExtra::ajustarDificultad()
{
    // El scroll se acelera +0.25 cada 10 segundos hasta 4.5.
    int bloques = leerFramesSupervivencia() / 600;
    qreal nueva = 2.0 + bloques * 0.25;
    if (nueva > 4.5) {
        nueva = 4.5;
    }
    fijarVelocidadScroll(nueva);
}

void NivelExtra::obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const
{
    // Viento por rachas como en el Nivel 3 para que no sea un vuelo
    // recto y predecible.
    const int FRAMES_POR_RACHA = 45;
    const int RANGO = 15;

    if (framesRestantesRacha <= 0) {
        vientoX = (rand() % RANGO - RANGO / 2) / 100.0;
        vientoY = (rand() % RANGO - RANGO / 2) / 100.0;
        framesRestantesRacha = FRAMES_POR_RACHA;
    }

    framesRestantesRacha--;
    fuerzaX = vientoX;
    fuerzaY = vientoY;
}
