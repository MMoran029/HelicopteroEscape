#include "mainwindow.h"
#include "gestorusuarios.h"
#include <QApplication>
#include <QTimer>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    setWindowTitle("Helicoptero Rescate - Prototipo");
    resize(820, 540);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    configurarLogin();
    configurarMenu();
    configurarJuego();
    configurarNivel2();
    configurarNivel3();
    configurarInstrucciones();
    configurarMisiones();
    configurarRanking();
    // El orden de insercion define el indice de cada pantalla en el stack:
    // 0 = login, 1 = menu, 2 = juego (Mision 1), 3 = instrucciones,
    // 4 = misiones, 5 = Nivel 2, 6 = Nivel 3, 7 = ranking.
    stack->addWidget(pantallaLogin);       // 0
    stack->addWidget(menu);                // 1
    stack->addWidget(pantallaJuego);       // 2
    stack->addWidget(pantallaInstrucciones);// 3
    stack->addWidget(pantallaMisiones);    // 4
    stack->addWidget(pantallaNivel2);      // 5
    stack->addWidget(pantallaNivel3);      // 6
    stack->addWidget(pantallaRanking);     // 7
    stack->setCurrentIndex(INDICE_LOGIN);  // se abre el login de primeras

    // --- Login ---
    connect(pantallaLogin, &pantalla_inicio_sesion::sesionIniciada,
            this, &MainWindow::sesionIniciada);
    connect(pantallaLogin, &pantalla_inicio_sesion::salirPresionado,
            this, &MainWindow::salirDelJuego);

    connect(menu, &MenuPrincipal::jugarPresionado, this, &MainWindow::irAJuego);
    connect(menu, &MenuPrincipal::instruccionesPresionado, this, &MainWindow::irAInstrucciones);
    connect(menu, &MenuPrincipal::misionesPresionado, this, &MainWindow::irAMisiones);
    connect(menu, &MenuPrincipal::rankingPresionado, this, &MainWindow::irARanking);
    connect(menu, &MenuPrincipal::salirPresionado, this, &MainWindow::salirDelJuego);

    connect(pantallaRanking, &PantallaRanking::volverPresionado, this, &MainWindow::irAMenu);

    connect(pantallaInstrucciones, &PantallaInstrucciones::volverPresionado, this, &MainWindow::irAMenu);

    connect(pantallaMisiones, &PantallaMisiones::volverPresionado, this, &MainWindow::irAMenu);
    connect(pantallaMisiones, &PantallaMisiones::mision1Presionado, this, &MainWindow::mision1Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision2Presionado, this, &MainWindow::mision2Elegida);
    connect(pantallaMisiones, &PantallaMisiones::mision3Presionado, this, &MainWindow::mision3Elegida);

    // El panel de resultado (victoria/derrota) dentro de PantallaJuego
    // reenvia estos botones hasta aca, que es quien controla el stack.
    connect(pantallaJuego, &PantallaJuego::solicitaMenu, this, &MainWindow::irAMenu);
    connect(pantallaJuego, &PantallaJuego::solicitaMisiones, this, &MainWindow::irAMisiones);
    connect(pantallaJuego, &PantallaJuego::solicitaSiguienteNivel, this, &MainWindow::siguienteNivelSolicitado);

    // El Nivel 2 usa las mismas señales (heredadas de PantallaJuego).
    connect(pantallaNivel2, &PantallaJuego::solicitaMenu, this, &MainWindow::irAMenu);
    connect(pantallaNivel2, &PantallaJuego::solicitaMisiones, this, &MainWindow::irAMisiones);
    connect(pantallaNivel2, &PantallaJuego::solicitaSiguienteNivel, this, &MainWindow::siguienteNivelSolicitado);

    // Cada nivel avisa al terminar para guardar el record del usuario.
    connect(pantallaJuego, &PantallaJuego::partidaTerminada,
            this, &MainWindow::guardarPuntajeRanking);
    connect(pantallaNivel2, &PantallaJuego::partidaTerminada,
            this, &MainWindow::guardarPuntajeRanking);
    connect(pantallaNivel3, &PantallaJuego::partidaTerminada,
            this, &MainWindow::guardarPuntajeRanking);
    // El Nivel 3 usa las mismas señales (heredadas de PantallaJuego).
    connect(pantallaNivel3, &PantallaJuego::solicitaMenu, this, &MainWindow::irAMenu);
    connect(pantallaNivel3, &PantallaJuego::solicitaMisiones, this, &MainWindow::irAMisiones);
    connect(pantallaNivel3, &PantallaJuego::solicitaSiguienteNivel, this, &MainWindow::siguienteNivelSolicitado);
}

MainWindow::~MainWindow(){
}

void MainWindow::configurarLogin(){
    pantallaLogin = new pantalla_inicio_sesion(this);
}

void MainWindow::configurarMenu(){
    menu = new MenuPrincipal(this);
}

void MainWindow::configurarJuego(){
    pantallaJuego = new PantallaJuego(this);
}

void MainWindow::configurarNivel2(){
    pantallaNivel2 = new Nivel2(this);
}

void MainWindow::configurarNivel3(){
    pantallaNivel3 = new Nivel3(this);
}

void MainWindow::configurarInstrucciones(){
    pantallaInstrucciones = new PantallaInstrucciones(this);
}

void MainWindow::configurarMisiones(){
    pantallaMisiones = new PantallaMisiones(this);
}

void MainWindow::configurarRanking(){
    pantallaRanking = new PantallaRanking(this);
}

void MainWindow::sesionIniciada(const QString &nombreUsuario){
    m_usuarioActual = nombreUsuario;
    cout << "[INFO] sesion iniciada: " << nombreUsuario.toStdString() << endl;
    irAMenu();
}

void MainWindow::irAJuego(){
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(INDICE_JUEGO);
    QTimer::singleShot(0, pantallaJuego, &PantallaJuego::ajustarVista);
    pantallaJuego->setFocus();
}

void MainWindow::irAInstrucciones(){
    stack->setCurrentIndex(INDICE_INSTRUCCIONES);
}

void MainWindow::irAMisiones(){
    stack->setCurrentIndex(INDICE_MISIONES);
}

void MainWindow::irARanking(){
    pantallaRanking->recargar();
    stack->setCurrentIndex(INDICE_RANKING);
}

void MainWindow::guardarPuntajeRanking(int nivel, int puntos, bool victoria){
    Q_UNUSED(victoria);
    // Sin sesion no hay donde guardar.
    if (m_usuarioActual.isEmpty()) {
        return;
    }
    if (puntos < 0) {
        puntos = 0;
    }
    bool recordNuevo = GestorUsuarios::guardarMejorPuntaje(m_usuarioActual, nivel, puntos);
    if (recordNuevo) {
        cout << "[INFO] record nuevo: " << m_usuarioActual.toStdString()
             << " nivel " << nivel << " puntos " << puntos << endl;
    }
}

void MainWindow::irAMenu(){
    stack->setCurrentIndex(INDICE_MENU);
}

void MainWindow::salirDelJuego(){
    QApplication::quit();
}

void MainWindow::mision1Elegida(){
    // La Mision 1 es el nivel del helicoptero que ya esta implementado.
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(INDICE_JUEGO);
    QTimer::singleShot(0, pantallaJuego, &PantallaJuego::ajustarVista);
    pantallaJuego->setFocus();
}

void MainWindow::mision2Elegida(){
    pantallaNivel2->reiniciarNivel();
    stack->setCurrentIndex(INDICE_NIVEL2);
    QTimer::singleShot(0, pantallaNivel2, &PantallaJuego::ajustarVista);
    pantallaNivel2->setFocus();
}

void MainWindow::mision3Elegida(){
    pantallaNivel3->reiniciarNivel();
    stack->setCurrentIndex(INDICE_NIVEL3);
    QTimer::singleShot(0, pantallaNivel3, &PantallaJuego::ajustarVista);
    pantallaNivel3->setFocus();
}

void MainWindow::siguienteNivelSolicitado(){
    if(sender() == pantallaJuego){
        // Se completo el Nivel 1: se pasa al Nivel 2.
        pantallaNivel2->reiniciarNivel();
        stack->setCurrentIndex(INDICE_NIVEL2);
        QTimer::singleShot(0, pantallaNivel2, &PantallaJuego::ajustarVista);
        pantallaNivel2->setFocus();
        return;
    }

    if(sender() == pantallaNivel2){
        // Se completo el Nivel 2: se pasa al Nivel 3.
        pantallaNivel3->reiniciarNivel();
        stack->setCurrentIndex(INDICE_NIVEL3);
        QTimer::singleShot(0, pantallaNivel3, &PantallaJuego::ajustarVista);
        pantallaNivel3->setFocus();
        return;
    }

    // Se completo el Nivel 3 (el ultimo por ahora): se vuelve a la
    stack->setCurrentIndex(INDICE_MISIONES);
}
