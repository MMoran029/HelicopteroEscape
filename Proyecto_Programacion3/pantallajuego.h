#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QLabel>
#include "helicoptero.h"
#include "obstaculo.h"
#include "obstaculoestatico.h"
#include "obstaculomovil.h"
#include "civil.h"
#include "bidon.h"
#include "disparo.h"
#include "estructurabloqueadora.h"
#include "panelresultado.h"
#include "barrahud.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QHideEvent>

class QGraphicsPixmapItem;

// Estados posibles de la partida actual.
enum class EstadoJuego {
    Jugando,
    Victoria,
    Derrota
};

class PantallaJuego : public QWidget {
    Q_OBJECT
public:
    // nivelJuego: 1, 2 o 3. Por ahora solo existe contenido para el
    // Nivel 1; los niveles 2 y 3 se agregaran como clases hijas que
    // sobreescriben los metodos virtuales marcados mas abajo.
    PantallaJuego(QWidget *parent = nullptr, int nivelJuego = 1);
    virtual ~PantallaJuego();

    // Reinicia por completo el nivel (memoria, vidas, contadores y
    // posicion del helicoptero). La usa MainWindow cada vez que el
    // jugador entra a la Mision 1 desde el menu, para garantizar que
    // siempre arranque una partida nueva.
    void reiniciarNivel();

    // Puntaje y nivel actuales. Los usa MainWindow para guardar el
    // record del usuario (USUARIOS/<nombre>/datos.txt) al terminar.
    // El puntaje nunca es negativo (ver modificarPuntos).
    int puntajeActual() const { return puntos < 0 ? 0 : puntos; }
    int numeroNivel() const { return nivelJuego; }

    // Marca de supervivencia (solo NivelExtra): segundos vividos y
    // metros recorridos. MainWindow los usa para el ranking.
    int tiempoSupervivenciaSegundos() const { return framesSupervivencia / 60; }
    int distanciaSupervivenciaMetros() const {
        int m = static_cast<int>(distanciaRecorrida / 10.0);
        return m < 0 ? 0 : m;
    }

    // Reajusta la escala de la vista a la escena logica de 800x500.
    // Es publico para que MainWindow pueda invocarlo (de forma
    // diferida) justo despues de navegar a esta pantalla con el
    // QStackedWidget, cuando el layout ya asento el tamano real.
    void ajustarVista();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

signals:
    // El panel de resultado dispara estos botones; PantallaJuego los
    // reenvia hacia arriba porque solo MainWindow sabe navegar entre
    // pantallas (menu, misiones, etc.).
    void solicitaMenu();
    void solicitaMisiones();
    void solicitaSiguienteNivel();
    // Se emite al terminar la partida (victoria o derrota) con el
    // nivel jugado y los puntos conseguidos, para el ranking.
    void partidaTerminada(int nivel, int puntos, bool victoria);

private slots:
    void actualizarJuego();

protected:
    // ---- Ganchos para los niveles 2 y 3 (clases hijas) ----
    // Nivel de dificultad actual (1, 2 o 3). Se usa para escalar la
    // frecuencia de bidones y el consumo de combustible mientras no
    // existan todavia las clases Nivel2/Nivel3.
    int nivelJuego;

    // ---- Musica de fondo del nivel (cada nivel, incluida la
    // supervivencia, tiene su propia cancion) ----
    QMediaPlayer *musicaJuego;
    QAudioOutput *salidaAudioJuego;

    // Cuantos frames pasan entre cada bidon generado. Sube con el
    // nivel (menos bidones = mas dificil), lo calcula obtenerIntervaloBidon().
    virtual int obtenerIntervaloBidon() const;

    // Cuanto combustible se consume por frame mientras el helicoptero
    // esta volando. Sube con el nivel.
    virtual double obtenerConsumoCombustible() const;

    // ---- Ganchos de armas / estructura / empuje (Nivel 2 y 3) ----
    // Base = comportamiento del Nivel 1: sin armas, sin empuje, sin
    // estructura bloqueadora. Nivel2/Nivel3 sobreescriben lo que
    // necesiten.
    virtual bool jugadorTieneArmas() const { return false; }
    virtual double probabilidadEnemigoArmado() const { return 0.0; }
    virtual double probabilidadDisparoEnemigoPorFrame() const { return 1.0 / 150.0; }
    virtual bool usaEstructuraBloqueadora() const { return false; }

    // Fuerza externa constante que empuja al helicoptero cada frame
    // (por ejemplo el viento del Nivel 2). Base = sin empuje.
    virtual void obtenerFuerzaEmpuje(qreal &fuerzaX, qreal &fuerzaY) const {
        fuerzaX = 0.0;
        fuerzaY = 0.0;
    }

    // ---- Ganchos del nivel infinito / supervivencia (NivelExtra) ----
    // Base = comportamiento de la historia (niveles 1-3): con meta de
    // distancia, con civiles y con una sola estructura bloqueadora.
    virtual bool esInfinito() const { return false; }
    virtual bool generaCiviles() const { return true; }
    // Cada cuantos frames aparece el proximo edificio / enemigo.
    // La base replica los rangos originales de actualizarJuego().
    virtual int obtenerIntervaloEdificioBase() const;
    virtual int obtenerIntervaloEnemigoBase() const;
    // Cada cuantos frames reaparece una estructura bloqueadora en el
    // modo infinito. Base = -1 (sin reaparicion: modo historia).
    virtual int obtenerIntervaloEstructura() const { return -1; }
    // Velocidad de scroll con la que arranca el nivel.
    virtual qreal obtenerVelocidadInicial() const { return 1.6; }
    // Se llama cada frame en el modo infinito para subir la
    // dificultad (mas velocidad, mas enemigos, etc.). Base = nada.
    virtual void ajustarDificultad() {}

    // Accesos protegidos para que NivelExtra pueda escalar la
    // dificultad sin exponer los miembros privados.
    void fijarVelocidadScroll(qreal nuevaVelocidad);
    qreal leerVelocidadScroll() const;
    qreal leerDistancia() const;
    int leerFramesSupervivencia() const;

    // HUD y cierre: virtuales para que el modo infinito muestre
    // tiempo/distancia en vez de porcentaje de progreso.
    virtual void actualizarHUD();
    virtual void actualizarBarrasHUD();
    virtual void finalizarJuego(EstadoJuego resultado);

    // La supervivencia no tiene meta, asi que no muestra la barra de
    // progreso (solo la de combustible). Base = mostrarla.
    virtual bool mostrarBarraProgreso() const { return true; }
    // Muestra u oculta la barra de progreso segun
    // mostrarBarraProgreso(). No es virtual: NivelExtra la llama en
    // su constructor y en cada reinicio.
    void aplicarVisibilidadBarras();

    // Aplica el sprite correcto (con o sin armas) al helicoptero segun
    // jugadorTieneArmas(). Los constructores de Nivel2/Nivel3 deben
    // llamarla ellos mismos DESPUES de que termine el constructor base
    // (llamar a un virtual desde el constructor de PantallaJuego no
    // funcionaria: en ese momento el objeto todavia es de tipo
    // PantallaJuego y no se puede saber aun que el nivel es 2 o 3).
    void actualizarSpriteJugador();

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timerJuego;
    Helicoptero *helicoptero;
    QGraphicsPixmapItem *piso1;
    QGraphicsPixmapItem *piso2;
    QGraphicsPixmapItem *fondo1;
    QGraphicsPixmapItem *fondo2;
    QLabel *hud;
    BarraHUD *barraProgreso;
    BarraHUD *barraCombustible;
    PanelResultado *panelResultado;

    static const int ANCHO_ESCENA = 800;
    static const int ALTO_ESCENA = 500;
    static const int ALTURA_SUELO = 42; // alto visual de la franja de piso

    // ---- Matriz dinamica de obstaculos: PUNTERO DOBLE (Obstaculo**) ----
    // obstaculos es un puntero a un arreglo de punteros a Obstaculo.
    // Cuando se llena, se reserva un arreglo nuevo mas grande con new,
    // se copian los punteros existentes y se libera el arreglo viejo
    // con delete[] (el arreglo, no los objetos que siguen vivos).
    Obstaculo **obstaculos;
    int numObstaculos;
    int capacidadObstaculos;

    // ---- Arreglo dinamico de civiles (mismo patron, Civil**) ----
    Civil **civiles;
    int numCiviles;
    int capacidadCiviles;

    // ---- Arreglo dinamico de bidones de combustible (mismo patron) ----
    Bidon **bidones;
    int numBidones;
    int capacidadBidones;

    // ---- Arreglo dinamico de disparos (mismo patron) ----
    Disparo **disparos;
    int numDisparos;
    int capacidadDisparos;

    // ---- Estructura bloqueadora (una sola por nivel, Nivel 2 y 3) ----
    EstructuraBloqueadora *estructura;
    bool estructuraGenerada;

    // ---- Control de disparo del jugador ----
    bool teclaEspacioPresionada;
    int contadorEnfriamientoDisparo;

    // Frames de invulnerabilidad tras chocar contra la estructura
    // bloqueadora, para que no pierda varias vidas de golpe mientras
    // sigue tocandola.
    int contadorInvulnerable;

    // ---- Estado de la partida ----
    int vidas;
    int civilesRescatados;
    int civilesTotalNivel;
    int civilesPerdidos;
    int puntos;
    qreal distanciaRecorrida;
    qreal distanciaMeta;
    qreal velocidadScroll;
    EstadoJuego estado;

    // ---- Combustible (0 a 100) ----
    double combustible;

    // ---- Control de generacion de bidones (en frames) ----
    int contadorFramesBidon;
    int intervaloBidon;

    // El helicoptero queda quieto (sin gravedad ni generacion de
    // obstaculos) hasta que el jugador presiona una tecla de control
    // por primera vez.
    bool juegoIniciado;

    // ---- Control de generacion (en frames, sin QTimer adicional) ----
    int contadorFramesEdificio;
    int intervaloEdificio;
    int contadorFramesEnemigo;
    int intervaloEnemigo;

    // ---- Control del modo infinito / supervivencia ----
    // framesSupervivencia cuenta los frames desde que arranco la
    // partida (sirve como cronometro: ~60 frames = 1 segundo).
    int framesSupervivencia;
    int contadorFramesEstructura;
    int intervaloEstructura;

    void configurarEscena();
    void configurarHUD();
    void configurarBarrasHUD();
    void configurarPanelResultado();

    void generarEdificio();
    void generarEnemigo();
    Civil* generarCivilesSobreEdificio(qreal posXEdificio, qreal topYEdificio);
    void generarBidon();
    void generarEstructura();

    // Revisa si una posicion Y (con cierto alto) se solapa con un
    // edificio recien generado (o su grupo de civiles encima). La usa
    // generarEnemigo() para no hacer aparecer enemigos incrustados en
    // edificios o civiles.
    bool zonaOcupadaPorEdificio(qreal posYEnemigo, qreal altoEnemigo) const;

    void agregarObstaculo(Obstaculo *obs);
    void agregarCivil(Civil *civil);
    void agregarBidon(Bidon *bidon);
    void agregarDisparo(Disparo *disparo);

    void dispararJugador();
    void dispararEnemigos();

    void actualizarObstaculos();
    void actualizarCiviles();
    void actualizarBidones();
    void actualizarCombustible();
    void actualizarPiso();
    void actualizarFondo();
    void actualizarDisparos();
    void actualizarEstructura();

    void eliminarObstaculosFuera();
    void eliminarCivilesFuera();
    void eliminarBidonesFuera();
    void eliminarDisparosFuera();
    void eliminarObstaculoEnIndice(int indice);
    void eliminarCivilEnIndice(int indice);
    void eliminarBidonEnIndice(int indice);
    void eliminarDisparoEnIndice(int indice);

    void revisarColisiones();
    void revisarRescates();
    void revisarRecoleccionCombustible();
    void revisarColisionesDisparos();
    void revisarColisionEstructura();

    // Suma (o resta, si delta es negativo) puntos, sin dejar que el
    // total baje de 0. Todo cambio al puntaje pasa por aqui.
    void modificarPuntos(int delta);

    void limpiarNivel();
};
#endif // PANTALLAJUEGO_H