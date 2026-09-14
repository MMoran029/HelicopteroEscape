#ifndef PANTALLAJUEGO_H
#define PANTALLAJUEGO_H
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QLabel>
#include "helicoptero.h"
#include "obstaculo.h"
#include "obstaculoestatico.h"
#include "obstaculomovil.h"
#include "civil.h"
#include "bidon.h"
#include "panelresultado.h"

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

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    // El panel de resultado dispara estos botones; PantallaJuego los
    // reenvia hacia arriba porque solo MainWindow sabe navegar entre
    // pantallas (menu, misiones, etc.).
    void solicitaMenu();
    void solicitaMisiones();
    void solicitaSiguienteNivel();

private slots:
    void actualizarJuego();

protected:
    // ---- Ganchos para los niveles 2 y 3 (clases hijas) ----
    // Nivel de dificultad actual (1, 2 o 3). Se usa para escalar la
    // frecuencia de bidones y el consumo de combustible mientras no
    // existan todavia las clases Nivel2/Nivel3.
    int nivelJuego;

    // Cuantos frames pasan entre cada bidon generado. Sube con el
    // nivel (menos bidones = mas dificil), lo calcula obtenerIntervaloBidon().
    virtual int obtenerIntervaloBidon() const;

    // Cuanto combustible se consume por frame mientras el helicoptero
    // esta volando. Sube con el nivel.
    virtual double obtenerConsumoCombustible() const;

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timerJuego;
    Helicoptero *helicoptero;
    QGraphicsPixmapItem *piso1;
    QGraphicsPixmapItem *piso2;
    QLabel *hud;
    QLabel *hudCombustible;
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

    // ---- Estado de la partida ----
    int vidas;
    int civilesRescatados;
    int civilesTotalNivel;
    int civilesPerdidos;
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

    void configurarEscena();
    void configurarHUD();
    void configurarHUDCombustible();
    void configurarPanelResultado();

    void generarEdificio();
    void generarEnemigo();
    Civil* generarCivilesSobreEdificio(qreal posXEdificio, qreal topYEdificio);
    void generarBidon();

    // Revisa si una posicion Y (con cierto alto) se solapa con un
    // edificio recien generado (o su grupo de civiles encima). La usa
    // generarEnemigo() para no hacer aparecer enemigos incrustados en
    // edificios o civiles.
    bool zonaOcupadaPorEdificio(qreal posYEnemigo, qreal altoEnemigo) const;

    void agregarObstaculo(Obstaculo *obs);
    void agregarCivil(Civil *civil);
    void agregarBidon(Bidon *bidon);

    void actualizarObstaculos();
    void actualizarCiviles();
    void actualizarBidones();
    void actualizarCombustible();
    void actualizarPiso();

    void eliminarObstaculosFuera();
    void eliminarCivilesFuera();
    void eliminarBidonesFuera();
    void eliminarObstaculoEnIndice(int indice);
    void eliminarCivilEnIndice(int indice);
    void eliminarBidonEnIndice(int indice);

    void revisarColisiones();
    void revisarRescates();
    void revisarRecoleccionCombustible();
    void actualizarHUD();
    void actualizarHUDCombustible();

    void finalizarJuego(EstadoJuego resultado);
    void limpiarNivel();
};
#endif // PANTALLAJUEGO_H