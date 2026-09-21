#include "mainwindow.h"
#include "gestorusuarios.h"
#include <QApplication>
#include <QTimer>
#include <QUrl>
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
    configurarNivelExtra();
    configurarInstrucciones();
    configurarMisiones();
    configurarRanking();
    configurarMusicaMenu();
    // El orden de insercion define el indice de cada pantalla en el stack:
    // 0 = login, 1 = menu, 2 = juego (Mision 1), 3 = instrucciones,
    // 4 = misiones, 5 = Nivel 2, 6 = Nivel 3, 7 = ranking,
    // 8 = NivelExtra (supervivencia).
    stack->addWidget(pantallaLogin);       // 0
    stack->addWidget(menu);                // 1
    stack->addWidget(pantallaJuego);       // 2
    stack->addWidget(pantallaInstrucciones);// 3
    stack->addWidget(pantallaMisiones);    // 4
    stack->addWidget(pantallaNivel2);      // 5
    stack->addWidget(pantallaNivel3);      // 6
    stack->addWidget(pantallaRanking);     // 7
    stack->addWidget(pantallaNivelExtra);  // 8
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
    connect(pantallaMisiones, &PantallaMisiones::misionExtraPresionado, this, &MainWindow::misionExtraElegida);

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
    // El NivelExtra (supervivencia) usa las mismas señales heredadas.
    connect(pantallaNivelExtra, &PantallaJuego::solicitaMenu, this, &MainWindow::irAMenu);
    connect(pantallaNivelExtra, &PantallaJuego::solicitaMisiones, this, &MainWindow::irAMisiones);
    connect(pantallaNivelExtra, &PantallaJuego::solicitaSiguienteNivel, this, &MainWindow::siguienteNivelSolicitado);
    connect(pantallaNivelExtra, &PantallaJuego::partidaTerminada,
            this, &MainWindow::guardarPuntajeRanking);
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

void MainWindow::configurarNivelExtra(){
    pantallaNivelExtra = new NivelExtra(this);
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

void MainWindow::configurarMusicaMenu(){
    musicaMenu = new QMediaPlayer(this);
    salidaAudioMenu = new QAudioOutput(this);
    musicaMenu->setAudioOutput(salidaAudioMenu);
    musicaMenu->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/Audio/musica_menu.mp3"));
    salidaAudioMenu->setVolume(0.05);
    musicaMenu->setLoops(QMediaPlayer::Infinite);
    musicaMenu->setPosition(0);
    musicaMenu->play();

    cout << "[INFO] musica de menu iniciada" << endl;
}

void MainWindow::continuarMusicaMenu(){
    if (musicaMenu->playbackState() != QMediaPlayer::PlayingState)
        musicaMenu->play();
}

void MainWindow::reiniciarMusicaMenu(){
    musicaMenu->setPosition(0);
    musicaMenu->play();
}


void MainWindow::sesionIniciada(const QString &nombreUsuario){
    m_usuarioActual = nombreUsuario;
    cout << "[INFO] sesion iniciada: " << nombreUsuario.toStdString() << endl;
    irAMenu();
}

void MainWindow::irAJuego(){
    musicaMenu->stop();
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(INDICE_JUEGO);
    QTimer::singleShot(0, pantallaJuego, &PantallaJuego::ajustarVista);
    pantallaJuego->setFocus();
}

void MainWindow::irAInstrucciones(){
    continuarMusicaMenu();
    stack->setCurrentIndex(INDICE_INSTRUCCIONES);
}

void MainWindow::irARanking(){
    continuarMusicaMenu();
    pantallaRanking->recargar();
    stack->setCurrentIndex(INDICE_RANKING);
}

void MainWindow::irAMisiones(){
    // Si viene de un nivel -> reinicia a 0, si viene del menu -> continua.
    if (qobject_cast<PantallaJuego*>(sender()) != nullptr)
        reiniciarMusicaMenu();
    else
        continuarMusicaMenu();
    stack->setCurrentIndex(INDICE_MISIONES);
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
    if (nivel == 4) {
        // Supervivencia: ademas del puntaje se guarda el mejor tiempo
        // y la mayor distancia (el ranking ordena por esos valores).
        PantallaJuego *juego = qobject_cast<PantallaJuego*>(sender());
        int tiempoSeg = 0;
        int distanciaM = 0;
        if (juego != nullptr) {
            tiempoSeg = juego->tiempoSupervivenciaSegundos();
            distanciaM = juego->distanciaSupervivenciaMetros();
        }
        bool recordSup = GestorUsuarios::guardarMejorSupervivencia(
            m_usuarioActual, tiempoSeg, distanciaM, puntos);
        if (recordSup) {
            cout << "[INFO] record supervivencia: " << m_usuarioActual.toStdString()
            << " tiempo " << tiempoSeg << "s distancia " << distanciaM << "m" << endl;
        }
    }
}


void MainWindow::irAMenu(){
    if (qobject_cast<PantallaJuego*>(sender()) != nullptr)
        reiniciarMusicaMenu();
    else
        continuarMusicaMenu();
    stack->setCurrentIndex(INDICE_MENU);
}

void MainWindow::salirDelJuego(){
    QApplication::quit();
}

void MainWindow::mision1Elegida(){
    // La Mision 1 es el nivel del helicoptero que ya esta implementado.
    musicaMenu->stop();
    pantallaJuego->reiniciarNivel();
    stack->setCurrentIndex(INDICE_JUEGO);
    QTimer::singleShot(0, pantallaJuego, &PantallaJuego::ajustarVista);
    pantallaJuego->setFocus();
}

void MainWindow::mision2Elegida(){
    musicaMenu->stop();
    pantallaNivel2->reiniciarNivel();
    stack->setCurrentIndex(INDICE_NIVEL2);
    QTimer::singleShot(0, pantallaNivel2, &PantallaJuego::ajustarVista);
    pantallaNivel2->setFocus();
}

void MainWindow::mision3Elegida(){
    musicaMenu->stop();
    pantallaNivel3->reiniciarNivel();
    stack->setCurrentIndex(INDICE_NIVEL3);
    QTimer::singleShot(0, pantallaNivel3, &PantallaJuego::ajustarVista);
    pantallaNivel3->setFocus();
}

void MainWindow::misionExtraElegida(){
    // Supervivencia: nivel infinito aparte (sin civiles, mas enemigos
    // y estructuras que tapan la pantalla).
    musicaMenu->stop();
    pantallaNivelExtra->reiniciarNivel();
    stack->setCurrentIndex(INDICE_NIVELEXTRA);
    QTimer::singleShot(0, pantallaNivelExtra, &PantallaJuego::ajustarVista);
    pantallaNivelExtra->setFocus();
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

    // La supervivencia no tiene siguiente nivel: se vuelve a misiones.
    if(sender() == pantallaNivelExtra){
        reiniciarMusicaMenu();
        stack->setCurrentIndex(INDICE_MISIONES);
        return;
    }

    reiniciarMusicaMenu();
    stack->setCurrentIndex(INDICE_MISIONES);

    // Se completo el Nivel 3 (el ultimo por ahora): se vuelve a la
    musicaMenu->setPosition(0);
    musicaMenu->play();
    stack->setCurrentIndex(INDICE_MISIONES);
}