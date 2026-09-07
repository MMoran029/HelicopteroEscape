#include "pantallajuego.h"
#include <QPainter>
#include <QBrush>
#include <QVBoxLayout>
#include <cstdlib>
#include <ctime>

PantallaJuego::PantallaJuego(QWidget *parent) : QWidget(parent),
    obstaculos(nullptr), numObstaculos(0), capacidadObstaculos(0),
    civiles(nullptr), numCiviles(0), capacidadCiviles(0),
    vidas(3), civilesRescatados(0), civilesTotalNivel(0), civilesPerdidos(0),
    distanciaRecorrida(0), distanciaMeta(6000), velocidadScroll(1.6),
    estado(EstadoJuego::Jugando), juegoIniciado(false),
    contadorFramesEdificio(0), intervaloEdificio(110),
    contadorFramesEnemigo(0), intervaloEnemigo(240)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    configurarEscena();
    configurarHUD();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(hud);
    layout->addWidget(vista);
    setLayout(layout);

    configurarPanelResultado();

    timerJuego = new QTimer(this);
    connect(timerJuego, &QTimer::timeout, this, &PantallaJuego::actualizarJuego);
    timerJuego->start(16);

    setFocusPolicy(Qt::StrongFocus);
}

PantallaJuego::~PantallaJuego(){
    // Se libera manualmente toda la memoria dinamica de obstaculos y
    // civiles que aun siguiera viva al cerrar la pantalla de juego.
    limpiarNivel();
}

void PantallaJuego::configurarEscena(){
    escena = new QGraphicsScene(0, 0, ANCHO_ESCENA, ALTO_ESCENA, this);
    escena->setBackgroundBrush(QBrush(QColor(30, 30, 60)));

    vista = new QGraphicsView(escena, this);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    helicoptero = new Helicoptero();
    helicoptero->setPos(100, ALTO_ESCENA / 2);
    escena->addItem(helicoptero);
}

void PantallaJuego::configurarHUD(){
    hud = new QLabel(this);
    hud->setStyleSheet("background-color: rgb(20,20,20); color: white; padding: 4px; font-weight: bold;");
    hud->setFixedHeight(26);
    actualizarHUD();
}

void PantallaJuego::configurarPanelResultado(){
    panelResultado = new PanelResultado(this);
    panelResultado->setGeometry(rect());

    connect(panelResultado, &PanelResultado::reiniciarPresionado, this, &PantallaJuego::reiniciarNivel);
    connect(panelResultado, &PanelResultado::menuPresionado, this, &PantallaJuego::solicitaMenu);
    connect(panelResultado, &PanelResultado::misionesPresionado, this, &PantallaJuego::solicitaMisiones);
    connect(panelResultado, &PanelResultado::siguienteNivelPresionado, this, &PantallaJuego::solicitaSiguienteNivel);
}

void PantallaJuego::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    if (panelResultado != nullptr) {
        panelResultado->setGeometry(rect());
    }
}

// ---------------------------------------------------------------
// Generacion de obstaculos y civiles
// ---------------------------------------------------------------
void PantallaJuego::generarEdificio(){
    qreal alto = 80 + (rand() % 180);      // alto entre 80 y 260
    qreal ancho = 60 + (rand() % 30);      // ancho entre 60 y 90
    qreal posX = ANCHO_ESCENA + ancho;     // aparece justo fuera de la vista
    qreal posY = ALTO_ESCENA - alto / 2;   // apoyado sobre el piso de la escena

    ObstaculoEstatico *edificio = new ObstaculoEstatico(posX, posY, ancho, alto);
    agregarObstaculo(edificio);

    // No todos los edificios tienen civiles: 40% de probabilidad.
    if (rand() % 100 < 40) {
        qreal topY = posY - alto / 2; // borde superior (techo) del edificio
        generarCivilesSobreEdificio(posX, topY);
    }
}

void PantallaJuego::generarEnemigo(){
    qreal ancho = 50;
    qreal alto = 30;
    qreal posX = ANCHO_ESCENA + ancho;
    qreal posY = 60 + (rand() % (ALTO_ESCENA - 140)); // banda de vuelo intermedia
    qreal amplitud = 30 + (rand() % 40);
    qreal velocidadVertical = 0.04 + (rand() % 5) / 100.0;

    ObstaculoMovil *enemigo = new ObstaculoMovil(posX, posY, ancho, alto, amplitud, velocidadVertical);
    agregarObstaculo(enemigo);
}

void PantallaJuego::generarCivilesSobreEdificio(qreal posXEdificio, qreal topYEdificio){
    // Un solo objeto Civil representa a todo el grupo (se dibuja como
    // un unico rectangulo grande). Evita crear un objeto por persona.
    int cantidad = 1 + (rand() % 3); // grupo de 1 a 3 personas
    Civil *civil = new Civil(posXEdificio, topYEdificio - 16, cantidad);
    agregarCivil(civil);
    civilesTotalNivel += cantidad;
}

// ---------------------------------------------------------------
// Manejo de memoria: arreglos dinamicos de punteros
// ---------------------------------------------------------------
void PantallaJuego::agregarObstaculo(Obstaculo *obs){
    if (numObstaculos == capacidadObstaculos) {
        int nuevaCapacidad = (capacidadObstaculos == 0) ? 8 : capacidadObstaculos * 2;
        Obstaculo **nuevoArreglo = new Obstaculo*[nuevaCapacidad];

        for (int i = 0; i < numObstaculos; ++i) {
            nuevoArreglo[i] = obstaculos[i];
        }

        delete[] obstaculos; // se libera el arreglo viejo (los objetos apuntados siguen vivos)
        obstaculos = nuevoArreglo;
        capacidadObstaculos = nuevaCapacidad;
    }

    obstaculos[numObstaculos] = obs;
    numObstaculos++;
    escena->addItem(obs);
}

void PantallaJuego::agregarCivil(Civil *civil){
    if (numCiviles == capacidadCiviles) {
        int nuevaCapacidad = (capacidadCiviles == 0) ? 8 : capacidadCiviles * 2;
        Civil **nuevoArreglo = new Civil*[nuevaCapacidad];

        for (int i = 0; i < numCiviles; ++i) {
            nuevoArreglo[i] = civiles[i];
        }

        delete[] civiles;
        civiles = nuevoArreglo;
        capacidadCiviles = nuevaCapacidad;
    }

    civiles[numCiviles] = civil;
    numCiviles++;
    escena->addItem(civil);
}

void PantallaJuego::eliminarObstaculoEnIndice(int indice){
    Obstaculo *obs = obstaculos[indice];
    escena->removeItem(obs); // se retira de la escena antes de liberar memoria
    delete obs;               // delete explicito: aqui se invoca el destructor virtual

    for (int i = indice; i < numObstaculos - 1; ++i) {
        obstaculos[i] = obstaculos[i + 1];
    }
    numObstaculos--;
}

void PantallaJuego::eliminarCivilEnIndice(int indice){
    Civil *civil = civiles[indice];
    escena->removeItem(civil);
    delete civil;

    for (int i = indice; i < numCiviles - 1; ++i) {
        civiles[i] = civiles[i + 1];
    }
    numCiviles--;
}

void PantallaJuego::eliminarObstaculosFuera(){
    // De atras hacia adelante: al eliminar, el arreglo se compacta
    // y correr de adelante hacia atras se saltaria elementos.
    for (int i = numObstaculos - 1; i >= 0; --i) {
        if (obstaculos[i]->fueraDePantalla()) {
            eliminarObstaculoEnIndice(i);
        }
    }
}

void PantallaJuego::eliminarCivilesFuera(){
    for (int i = numCiviles - 1; i >= 0; --i) {
        bool seFue = civiles[i]->fueraDePantalla();
        bool yaResuelto = civiles[i]->estaRescatado() || civiles[i]->estaAplastado();

        // Si el civil sale de pantalla sin haber sido rescatado ni aplastado,
        // se cuenta como perdido (se dejo atras).
        if (seFue && !yaResuelto) {
            civilesPerdidos += civiles[i]->getCantidadPersonas();
        }

        if (seFue || yaResuelto) {
            eliminarCivilEnIndice(i);
        }
    }
}

void PantallaJuego::limpiarNivel(){
    if (obstaculos != nullptr) {
        for (int i = 0; i < numObstaculos; ++i) {
            escena->removeItem(obstaculos[i]);
            delete obstaculos[i];
        }
        delete[] obstaculos;
        obstaculos = nullptr;
    }
    numObstaculos = 0;
    capacidadObstaculos = 0;

    if (civiles != nullptr) {
        for (int i = 0; i < numCiviles; ++i) {
            escena->removeItem(civiles[i]);
            delete civiles[i];
        }
        delete[] civiles;
        civiles = nullptr;
    }
    numCiviles = 0;
    capacidadCiviles = 0;
}

// ---------------------------------------------------------------
// Actualizacion por frame
// ---------------------------------------------------------------
void PantallaJuego::actualizarObstaculos(){
    for (int i = 0; i < numObstaculos; ++i) {
        obstaculos[i]->actualizar(velocidadScroll, ALTO_ESCENA);
    }
}

void PantallaJuego::actualizarCiviles(){
    for (int i = 0; i < numCiviles; ++i) {
        civiles[i]->actualizar(velocidadScroll);
    }
}

void PantallaJuego::revisarColisiones(){
    const qreal TOLERANCIA_ATERRIZAJE = 12.0;      // penetracion maxima que se considera "aterrizaje", no choque
    const qreal VELOCIDAD_MAXIMA_ATERRIZAJE = 4.5; // si baja mas rapido que esto, el aterrizaje se considera brusco

    for (int i = 0; i < numObstaculos; ++i) {
        QRectF rectHelicoptero = helicoptero->sceneBoundingRect();
        ObstaculoEstatico *edificio = dynamic_cast<ObstaculoEstatico*>(obstaculos[i]);

        if (edificio != nullptr) {
            // Los edificios se pueden "pisar": si el helicoptero llega desde
            // arriba, apenas roza el techo y no viene cayendo muy rapido,
            // se posa en vez de destruirse.
            QRectF rectEdificio = edificio->sceneBoundingRect();
            bool solapaHorizontal = (rectHelicoptero.right() > rectEdificio.left()) &&
                                     (rectHelicoptero.left() < rectEdificio.right());

            if (solapaHorizontal && rectHelicoptero.bottom() >= rectEdificio.top()) {
                qreal penetracion = rectHelicoptero.bottom() - rectEdificio.top();
                bool cayendoMuyRapido = helicoptero->getVelocidadY() > VELOCIDAD_MAXIMA_ATERRIZAJE;

                if (penetracion <= TOLERANCIA_ATERRIZAJE && !cayendoMuyRapido) {
                    // Aterrizaje seguro sobre el techo: se ajusta la posicion
                    // y se arrastra junto con el scroll, como si fuera piso.
                    qreal nuevaY = helicoptero->y() - penetracion;
                    helicoptero->aterrizarSobre(helicoptero->x() - velocidadScroll, nuevaY);
                    continue; // no hay daño, se revisa el siguiente obstaculo
                }

                // penetro demasiado o venia cayendo muy rapido: aterrizaje brusco = choque
                vidas--;
                eliminarObstaculoEnIndice(i);
                if (vidas <= 0) {
                    finalizarJuego(EstadoJuego::Derrota);
                }
                break; // solo se procesa una colision por frame
            }
            // si no hay solapamiento, o el helicoptero sigue arriba del techo: nada que hacer
        } else {
            // Obstaculo movil (enemigo): cualquier contacto es dano, sin excepciones.
            if (rectHelicoptero.intersects(obstaculos[i]->sceneBoundingRect())) {
                vidas--;
                eliminarObstaculoEnIndice(i);

                if (vidas <= 0) {
                    finalizarJuego(EstadoJuego::Derrota);
                }
                break;
            }
        }
    }
}

void PantallaJuego::revisarRescates(){
    qreal helicX = helicoptero->x();
    qreal helicY = helicoptero->y();
    const qreal DISTANCIA_RESCATE = 35.0;
    const qreal VELOCIDAD_MAXIMA_RESCATE = 4.0; // si baja mas rapido que esto sobre un civil, lo aplasta

    QRectF rectHelicoptero = helicoptero->sceneBoundingRect();

    for (int i = 0; i < numCiviles; ++i) {
        if (civiles[i]->estaActivo() && !civiles[i]->estaRescatado() && !civiles[i]->estaAplastado()) {
            bool solapa = rectHelicoptero.intersects(civiles[i]->sceneBoundingRect());
            bool cayendoMuyRapido = helicoptero->getVelocidadY() > VELOCIDAD_MAXIMA_RESCATE;

            if (solapa && cayendoMuyRapido) {
                // El helicoptero les cayo encima demasiado rapido: se aplastan.
                civiles[i]->aplastar();
                civilesPerdidos += civiles[i]->getCantidadPersonas();
            } else if (civiles[i]->verificarCercania(helicX, helicY, DISTANCIA_RESCATE)) {
                civiles[i]->rescatar();
                civilesRescatados += civiles[i]->getCantidadPersonas();
            }
        }
    }
}

void PantallaJuego::actualizarHUD(){
    int progreso = static_cast<int>(qMin(100.0, (distanciaRecorrida / distanciaMeta) * 100.0));
    QString texto = QString("Vidas: %1   |   Rescatados: %2   |   Perdidos: %3   |   Progreso: %4%")
                        .arg(vidas)
                        .arg(civilesRescatados)
                        .arg(civilesPerdidos)
                        .arg(progreso);
    hud->setText(texto);
}

void PantallaJuego::finalizarJuego(EstadoJuego resultado){
    estado = resultado;
    timerJuego->stop();

    bool victoria = (resultado == EstadoJuego::Victoria);
    int progreso = static_cast<int>(qMin(100.0, (distanciaRecorrida / distanciaMeta) * 100.0));

    QString info = QString("Civiles rescatados: %1\nCiviles perdidos: %2\nProgreso alcanzado: %3%\nVidas restantes: %4")
                       .arg(civilesRescatados)
                       .arg(civilesPerdidos)
                       .arg(progreso)
                       .arg(vidas);

    hud->setText(victoria ? "Escape exitoso" : "Helicoptero destruido");
    panelResultado->mostrarResultado(victoria, info);
}

void PantallaJuego::reiniciarNivel(){
    limpiarNivel();
    panelResultado->hide();

    helicoptero->setPos(100, ALTO_ESCENA / 2);
    helicoptero->setSubiendo(false);
    helicoptero->setBajando(false);
    helicoptero->setIzquierda(false);
    helicoptero->setDerecha(false);

    vidas = 3;
    civilesRescatados = 0;
    civilesTotalNivel = 0;
    civilesPerdidos = 0;
    distanciaRecorrida = 0;
    contadorFramesEdificio = 0;
    contadorFramesEnemigo = 0;
    estado = EstadoJuego::Jugando;
    juegoIniciado = false;

    actualizarHUD();
    timerJuego->start(16);
}

void PantallaJuego::actualizarJuego(){
    if (estado != EstadoJuego::Jugando) {
        return;
    }

    if (!juegoIniciado) {
        // El helicoptero se mantiene quieto (sin gravedad, sin obstaculos)
        // hasta que el jugador presione una tecla de control por primera vez.
        hud->setText("Presiona una flecha (o WASD) para iniciar la mision");
        return;
    }

    helicoptero->actualizarFisica(ANCHO_ESCENA, ALTO_ESCENA);

    // El piso destruye el helicoptero de inmediato, sin importar las vidas.
    if (helicoptero->tocoElSuelo()) {
        finalizarJuego(EstadoJuego::Derrota);
        return;
    }

    distanciaRecorrida += velocidadScroll;

    contadorFramesEdificio++;
    if (contadorFramesEdificio >= intervaloEdificio) {
        generarEdificio();
        contadorFramesEdificio = 0;
        intervaloEdificio = 70 + (rand() % 70);
    }

    contadorFramesEnemigo++;
    if (contadorFramesEnemigo >= intervaloEnemigo) {
        generarEnemigo();
        contadorFramesEnemigo = 0;
        intervaloEnemigo = 150 + (rand() % 150);
    }

    actualizarObstaculos();
    actualizarCiviles();

    eliminarObstaculosFuera();
    eliminarCivilesFuera();

    revisarColisiones();
    if (estado != EstadoJuego::Jugando) {
        return;
    }

    revisarRescates();

    if (distanciaRecorrida >= distanciaMeta) {
        finalizarJuego(EstadoJuego::Victoria);
        return;
    }

    actualizarHUD();
}

void PantallaJuego::keyPressEvent(QKeyEvent *event){
    if (estado != EstadoJuego::Jugando) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            reiniciarNivel();
        }
        return;
    }

    if (event->isAutoRepeat()) {
        return;
    }

    juegoIniciado = true;

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(true);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(true);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(true);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(true);
        break;
    }
    QWidget::keyPressEvent(event);
}

void PantallaJuego::keyReleaseEvent(QKeyEvent *event){
    if (event->isAutoRepeat()) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_W:
        helicoptero->setSubiendo(false);
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        helicoptero->setBajando(false);
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
        helicoptero->setIzquierda(false);
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        helicoptero->setDerecha(false);
        break;
    }
    QWidget::keyReleaseEvent(event);
}
