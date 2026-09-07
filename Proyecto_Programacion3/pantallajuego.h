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
#include "panelresultado.h"

// Estados posibles de la partida actual.
enum class EstadoJuego {
    Jugando,
    Victoria,
    Derrota
};

class PantallaJuego : public QWidget {
    Q_OBJECT
public:
    PantallaJuego(QWidget *parent = nullptr);
    ~PantallaJuego();

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

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
    QTimer *timerJuego;
    Helicoptero *helicoptero;
    QLabel *hud;
    PanelResultado *panelResultado;

    static const int ANCHO_ESCENA = 800;
    static const int ALTO_ESCENA = 500;

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

    // ---- Estado de la partida ----
    int vidas;
    int civilesRescatados;
    int civilesTotalNivel;
    int civilesPerdidos;
    qreal distanciaRecorrida;
    qreal distanciaMeta;
    qreal velocidadScroll;
    EstadoJuego estado;

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
    void configurarPanelResultado();

    void generarEdificio();
    void generarEnemigo();
    void generarCivilesSobreEdificio(qreal posXEdificio, qreal topYEdificio);

    void agregarObstaculo(Obstaculo *obs);
    void agregarCivil(Civil *civil);

    void actualizarObstaculos();
    void actualizarCiviles();

    void eliminarObstaculosFuera();
    void eliminarCivilesFuera();
    void eliminarObstaculoEnIndice(int indice);
    void eliminarCivilEnIndice(int indice);

    void revisarColisiones();
    void revisarRescates();
    void actualizarHUD();

    void finalizarJuego(EstadoJuego resultado);
    void limpiarNivel();
};
#endif // PANTALLAJUEGO_H
