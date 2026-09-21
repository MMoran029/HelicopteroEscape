#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include "menuprincipal.h"
#include "pantallajuego.h"
#include "nivel2.h"
#include "nivel3.h"
#include "nivelextra.h"
#include "pantallainstrucciones.h"
#include "pantallamisiones.h"
#include "pantalla_inicio_sesion.h"
#include "pantallaranking.h"
#include <QMediaPlayer>
#include <QAudioOutput>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Usuario que inicio sesion (carpeta en USUARIOS/<nombre>).
    // Sirve para guardar su mejor puntaje y el futuro ranking.
    QString usuarioActual() const { return m_usuarioActual; }

private slots:
    void irAJuego();
    void irAInstrucciones();
    void irAMisiones();
    void irAMenu();
    void salirDelJuego();
    void mision1Elegida();
    void mision2Elegida();
    void mision3Elegida();
    void misionExtraElegida();
    void siguienteNivelSolicitado();
    void sesionIniciada(const QString &nombreUsuario);
    void irARanking();
    void guardarPuntajeRanking(int nivel, int puntos, bool victoria);

private:
    QStackedWidget *stack;
    pantalla_inicio_sesion *pantallaLogin;
    MenuPrincipal *menu;
    PantallaJuego *pantallaJuego;
    Nivel2 *pantallaNivel2;
    Nivel3 *pantallaNivel3;
    NivelExtra *pantallaNivelExtra;
    PantallaInstrucciones *pantallaInstrucciones;
    PantallaMisiones *pantallaMisiones;
    PantallaRanking *pantallaRanking;

    QMediaPlayer *musicaMenu;
    QAudioOutput *salidaAudioMenu;

    QString m_usuarioActual;

    // Indices del QStackedWidget (el orden lo define el constructor).
    enum IndicePantalla {
        INDICE_LOGIN = 0,
        INDICE_MENU = 1,
        INDICE_JUEGO = 2,
        INDICE_INSTRUCCIONES = 3,
        INDICE_MISIONES = 4,
        INDICE_NIVEL2 = 5,
        INDICE_NIVEL3 = 6,
        INDICE_RANKING = 7,
        INDICE_NIVELEXTRA = 8
    };

    void configurarLogin();
    void configurarMenu();
    void configurarJuego();
    void configurarNivel2();
    void configurarNivel3();
    void configurarNivelExtra();
    void configurarInstrucciones();
    void configurarMisiones();
    void configurarRanking();
    void configurarMusicaMenu();
};
#endif // MAINWINDOW_H