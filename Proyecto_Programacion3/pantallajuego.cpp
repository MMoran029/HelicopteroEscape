#include "pantallajuego.h"
#include <QPainter>
#include <QBrush>
#include <QVBoxLayout>
#include <QGraphicsPixmapItem>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;
PantallaJuego::PantallaJuego(QWidget *parent, int nivelJuego) : QWidget(parent),
    nivelJuego(nivelJuego),
    obstaculos(nullptr), numObstaculos(0), capacidadObstaculos(0),
    civiles(nullptr), numCiviles(0), capacidadCiviles(0),
    bidones(nullptr), numBidones(0), capacidadBidones(0),
    disparos(nullptr), numDisparos(0), capacidadDisparos(0),
    estructura(nullptr), estructuraGenerada(false),
    teclaEspacioPresionada(false), contadorEnfriamientoDisparo(0),
    contadorInvulnerable(0),
    vidas(3), civilesRescatados(0), civilesTotalNivel(0), civilesPerdidos(0),puntos(0),
    distanciaRecorrida(0), distanciaMeta(6000), velocidadScroll(1.6),
    estado(EstadoJuego::Jugando),
    combustible(100.0),
    contadorFramesBidon(0), intervaloBidon(280),
    juegoIniciado(false),
    contadorFramesEdificio(0), intervaloEdificio(150),
    contadorFramesEnemigo(0), intervaloEnemigo(260)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    configurarEscena();
    configurarHUD();
    configurarHUDCombustible();

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
    vista->setRenderHint(QPainter::SmoothPixmapTransform);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    escena->setItemIndexMethod(QGraphicsScene::NoIndex);
    vista->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    // Franja de piso: dos copias encadenadas para simular scroll
    // infinito. Cuando una sale completamente de la vista por la
    // izquierda, se reubica pegada detras de la otra.
    QPixmap fondoOriginal(":/imagenes/Imagenes/fondo.png");
    QPixmap fondoEscalado = fondoOriginal.scaled(ANCHO_ESCENA, ALTO_ESCENA,
                                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    fondo1 = escena->addPixmap(fondoEscalado);
    fondo1->setPos(0, 0);
    fondo1->setZValue(-2);

    fondo2 = escena->addPixmap(fondoEscalado);
    fondo2->setPos(ANCHO_ESCENA, 0);
    fondo2->setZValue(-2);

    QPixmap pisoOriginal(":/imagenes/Imagenes/Piso.png");
    QPixmap pisoEscalado = pisoOriginal.scaled(ANCHO_ESCENA, ALTURA_SUELO,
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    piso1 = escena->addPixmap(pisoEscalado);
    piso1->setPos(0, ALTO_ESCENA - ALTURA_SUELO);
    piso1->setZValue(-1);

    piso2 = escena->addPixmap(pisoEscalado);
    piso2->setPos(ANCHO_ESCENA, ALTO_ESCENA - ALTURA_SUELO);
    piso2->setZValue(-1);

    helicoptero = new Helicoptero();
    helicoptero->setPos(100, ALTO_ESCENA / 2);
    escena->addItem(helicoptero);
}

void PantallaJuego::actualizarSpriteJugador(){
    helicoptero->setConArmas(jugadorTieneArmas());
}

void PantallaJuego::configurarHUD(){
    hud = new QLabel(this);
    hud->setStyleSheet("background-color: rgb(20,20,20); color: white; padding: 4px; font-weight: bold;");
    hud->setFixedHeight(26);
    actualizarHUD();
}

void PantallaJuego::configurarHUDCombustible(){
    // Etiqueta flotante en la esquina superior derecha, separada del
    // HUD de estadisticas, para que el combustible resalte a simple
    // vista como en un tablero de instrumentos.
    hudCombustible = new QLabel(this);
    hudCombustible->setStyleSheet("background-color: rgba(20,20,20,200); color: rgb(240,210,80);"
                                  "padding: 4px 8px; font-weight: bold; border-radius: 4px;");
    hudCombustible->setAlignment(Qt::AlignCenter);
    actualizarHUDCombustible();
    hudCombustible->raise();
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
    if (hudCombustible != nullptr) {
        hudCombustible->adjustSize();
        int margen = 10;
        hudCombustible->move(width() - hudCombustible->width() - margen, margen + 26);
        hudCombustible->raise();
    }
}

// ---------------------------------------------------------------
// Generacion de obstaculos y civiles
// ---------------------------------------------------------------
void PantallaJuego::generarEdificio(){
    int variante = 1 + (rand() % 3);       // Edificio_1, Edificio_2 o Edificio_3

    qreal alto;
    if (variante == 2) {
        // Edificio_2 es mas angosto que los otros (imagen alta y
        // delgada). Un punto medio: ni tan chico que se pierda, ni tan
        // alto que termine dominando la pantalla.
        alto = 150 + (rand() % 90); // entre 150 y 240
    } else {
        alto = 120 + (rand() % 80);  // entre 120 y 200
    }

    qreal ancho = ObstaculoEstatico::calcularAncho(alto, variante);

    qreal posX = ANCHO_ESCENA + ancho;                          // aparece justo fuera de la vista
    qreal posY = (ALTO_ESCENA - ALTURA_SUELO) - alto / 2;       // apoyado sobre la franja de piso

    ObstaculoEstatico *edificio = new ObstaculoEstatico(posX, posY, alto, variante);
    agregarObstaculo(edificio);

    // No todos los edificios tienen civiles: 40% de probabilidad.
    if (rand() % 100 < 40) {
        qreal topY = posY - alto / 2; // borde superior (techo) del edificio
        Civil *civil = generarCivilesSobreEdificio(posX, topY);
        edificio->asociarCivil(civil);
    }
}

bool PantallaJuego::zonaOcupadaPorEdificio(qreal posYEnemigo, qreal altoEnemigo) const{
    // Solo interesan los edificios que siguen cerca del borde derecho
    // (donde tambien aparecen los enemigos nuevos). Un edificio que ya
    // avanzo hacia la izquierda no estorba mas en el punto de spawn.
    const qreal MARGEN_ZONA_SPAWN = 260.0;

    qreal topEnemigo = posYEnemigo - altoEnemigo / 2;
    qreal bottomEnemigo = posYEnemigo + altoEnemigo / 2;

    for(int i=0 ; i<numObstaculos ; i++){
        ObstaculoEstatico *edificio = dynamic_cast<ObstaculoEstatico*>(obstaculos[i]);
        if(edificio == nullptr){
            continue;
        }
        if(edificio->x() < ANCHO_ESCENA - MARGEN_ZONA_SPAWN){
            continue;
        }

        // Se extiende el techo hacia arriba por si hay civiles parados
        // encima del edificio, para que tampoco se aparezca sobre ellos.
        qreal topEdificio = edificio->sceneBoundingRect().top() - Civil::alturaVisual();
        qreal bottomEdificio = edificio->sceneBoundingRect().bottom();

        bool seSolapan = (topEnemigo < bottomEdificio) && (bottomEnemigo > topEdificio);
        if(seSolapan == true){
            return true;
        }
    }
    return false;
}

void PantallaJuego::generarEnemigo(){
    // Se decide primero si este enemigo trae armas, porque el ancho
    // correcto depende de que sprite se va a usar.
    bool conArmas = (rand() % 100) < static_cast<int>(probabilidadEnemigoArmado() * 100);

    qreal alto = 30 + (rand() % 13);       // alto entre 30 y 42 (mas pequenos que antes)
    qreal ancho = ObstaculoMovil::calcularAncho(alto, conArmas);
    qreal posX = ANCHO_ESCENA + ancho;

    qreal posY = 0;
    bool posicionValida = false;
    int intentos = 0;

    // Se prueban varias alturas al azar hasta encontrar una que no
    // choque con un edificio (o sus civiles) recien aparecido. Si
    // ninguna funciona, se cancela este intento y se genera el
    // enemigo mas adelante, en vez de incrustarlo a la fuerza.
    while(intentos < 6 && posicionValida == false){
        posY = 60 + (rand() % (ALTO_ESCENA - 140)); // banda de vuelo intermedia
        if(zonaOcupadaPorEdificio(posY, alto) == false){
            posicionValida = true;
        }
        intentos++;
    }

    if(posicionValida == false){
        return;
    }

    qreal amplitud = 30 + (rand() % 40);
    qreal velocidadVertical = 0.04 + (rand() % 5) / 100.0;

    ObstaculoMovil *enemigo = new ObstaculoMovil(posX, posY, ancho, alto, amplitud, velocidadVertical, conArmas);
    agregarObstaculo(enemigo);
}

void PantallaJuego::generarEstructura(){
    qreal alto = (ALTO_ESCENA - ALTURA_SUELO) * 1;
    qreal ancho = EstructuraBloqueadora::calcularAncho(alto);
    qreal posX = ANCHO_ESCENA + ancho;
    qreal posY = (ALTO_ESCENA - ALTURA_SUELO) / 2;

    estructura = new EstructuraBloqueadora(posX, posY, alto);
    escena->addItem(estructura);
    estructuraGenerada = true;
}

void PantallaJuego::generarBidon(){
    qreal posX = ANCHO_ESCENA + Bidon::alturaVisual();
    qreal alto = Bidon::alturaVisual();

    qreal posY = 0;
    bool posicionValida = false;
    int intentos = 0;

    // Mismo criterio que generarEnemigo(): se prueban varias alturas
    // hasta encontrar una que no caiga dentro de un edificio (o sus
    // civiles) recien aparecido, para que el bidon nunca quede
    // incrustado dentro de una estructura.
    while(intentos < 6 && posicionValida == false){
        posY = 70 + (rand() % (ALTO_ESCENA - ALTURA_SUELO - 140));
        if(zonaOcupadaPorEdificio(posY, alto) == false){
            posicionValida = true;
        }
        intentos++;
    }

    if(posicionValida == false){
        return;
    }

    Bidon *bidon = new Bidon(posX, posY);
    agregarBidon(bidon);
}

Civil* PantallaJuego::generarCivilesSobreEdificio(qreal posXEdificio, qreal topYEdificio){
    // Un solo objeto Civil representa a todo el grupo (dibuja la imagen
    // de 1, 2 o 3 personas segun corresponda). Se centra justo encima
    // del techo del edificio, usando el alto visual real de la imagen.
    int cantidad = 1 + (rand() % 3); // grupo de 1 a 3 personas
    qreal posY = topYEdificio - Civil::alturaVisual() / 2;
    Civil *civil = new Civil(posXEdificio, posY, cantidad);
    agregarCivil(civil);
    civilesTotalNivel += cantidad;
    return civil;
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

void PantallaJuego::agregarBidon(Bidon *bidon){
    if(numBidones == capacidadBidones){
        int nuevaCapacidad = (capacidadBidones == 0) ? 8 : capacidadBidones * 2;
        Bidon **nuevoArreglo = new Bidon*[nuevaCapacidad];

        for(int i=0 ; i<numBidones ; i++){
            nuevoArreglo[i] = bidones[i];
        }

        delete[] bidones;
        bidones = nuevoArreglo;
        capacidadBidones = nuevaCapacidad;
    }

    bidones[numBidones] = bidon;
    numBidones++;
    escena->addItem(bidon);
}

void PantallaJuego::agregarDisparo(Disparo *disparo){
    if(numDisparos == capacidadDisparos){
        int nuevaCapacidad = (capacidadDisparos == 0) ? 8 : capacidadDisparos * 2;
        Disparo **nuevoArreglo = new Disparo*[nuevaCapacidad];

        for(int i=0 ; i<numDisparos ; i++){
            nuevoArreglo[i] = disparos[i];
        }

        delete[] disparos;
        disparos = nuevoArreglo;
        capacidadDisparos = nuevaCapacidad;
    }

    disparos[numDisparos] = disparo;
    numDisparos++;
    escena->addItem(disparo);
}

void PantallaJuego::dispararJugador(){
    // El disparo sale desde el frente del helicoptero (borde derecho,
    // direccion de avance).
    qreal posX = helicoptero->x() + 34;
    qreal posY = helicoptero->y();

    Disparo *disparo = new Disparo(posX, posY, 9.0, true);
    agregarDisparo(disparo);
}

void PantallaJuego::dispararEnemigos(){
    for(int i=0 ; i<numObstaculos ; i++){
        ObstaculoMovil *enemigo = dynamic_cast<ObstaculoMovil*>(obstaculos[i]);
        if(enemigo == nullptr){
            continue;
        }
        if(enemigo->estaArmado() == false){
            continue;
        }
        // Solo disparan los que ya son visibles en pantalla.
        if(enemigo->x() < 0 || enemigo->x() > ANCHO_ESCENA){
            continue;
        }

        double probabilidad = probabilidadDisparoEnemigoPorFrame();
        int umbral = static_cast<int>(probabilidad * 10000);
        if((rand() % 10000) < umbral){
            qreal posX = enemigo->x() - 24; // borde izquierdo, direccion de avance del enemigo
            qreal posY = enemigo->y();
            Disparo *disparo = new Disparo(posX, posY, -7.0, false);
            agregarDisparo(disparo);
        }
    }
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

void PantallaJuego::eliminarBidonEnIndice(int indice){
    Bidon *bidon = bidones[indice];
    escena->removeItem(bidon);
    delete bidon;

    for(int i=indice ; i<numBidones - 1 ; i++){
        bidones[i] = bidones[i + 1];
    }
    numBidones--;
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
            puntos -= 100 * civiles[i]->getCantidadPersonas();
        }

        if (seFue || yaResuelto) {
            eliminarCivilEnIndice(i);
        }
    }
}

void PantallaJuego::eliminarBidonesFuera(){
    for(int i=numBidones - 1 ; i>=0 ; i--){
        bool seFue = bidones[i]->fueraDePantalla();
        bool yaRecogido = bidones[i]->estaRecogido();

        if(seFue || yaRecogido){
            eliminarBidonEnIndice(i);
        }
    }
}

void PantallaJuego::eliminarDisparoEnIndice(int indice){
    Disparo *disparo = disparos[indice];
    escena->removeItem(disparo);
    delete disparo;

    for(int i=indice ; i<numDisparos - 1 ; i++){
        disparos[i] = disparos[i + 1];
    }
    numDisparos--;
}

void PantallaJuego::eliminarDisparosFuera(){
    for(int i=numDisparos - 1 ; i>=0 ; i--){
        if(disparos[i]->fueraDePantalla(ANCHO_ESCENA) == true){
            eliminarDisparoEnIndice(i);
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

    if (bidones != nullptr) {
        for (int i = 0; i < numBidones; ++i) {
            escena->removeItem(bidones[i]);
            delete bidones[i];
        }
        delete[] bidones;
        bidones = nullptr;
    }
    numBidones = 0;
    capacidadBidones = 0;

    if (disparos != nullptr) {
        for (int i = 0; i < numDisparos; ++i) {
            escena->removeItem(disparos[i]);
            delete disparos[i];
        }
        delete[] disparos;
        disparos = nullptr;
    }
    numDisparos = 0;
    capacidadDisparos = 0;

    if (estructura != nullptr) {
        escena->removeItem(estructura);
        delete estructura;
        estructura = nullptr;
    }
    estructuraGenerada = false;
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

void PantallaJuego::actualizarBidones(){
    for(int i=0 ; i<numBidones ; i++){
        bidones[i]->actualizar(velocidadScroll);
    }
}

void PantallaJuego::actualizarDisparos(){
    for(int i=0 ; i<numDisparos ; i++){
        disparos[i]->actualizar();
    }
}

void PantallaJuego::actualizarEstructura(){
    if(estructura != nullptr){
        estructura->actualizar(velocidadScroll, ALTO_ESCENA);
        if(estructura->fueraDePantalla() == true){
            finalizarJuego(EstadoJuego::Derrota);
        }
    }
}
void PantallaJuego::actualizarPiso(){
    piso1->setPos(piso1->x() - velocidadScroll, piso1->y());
    piso2->setPos(piso2->x() - velocidadScroll, piso2->y());

    if(piso1->x() <= -ANCHO_ESCENA){
        piso1->setPos(piso2->x() + ANCHO_ESCENA, piso1->y());
    }
    if(piso2->x() <= -ANCHO_ESCENA){
        piso2->setPos(piso1->x() + ANCHO_ESCENA, piso2->y());
    }
}
void PantallaJuego::actualizarFondo(){
    fondo1->setPos(fondo1->x() - velocidadScroll, fondo1->y());
    fondo2->setPos(fondo2->x() - velocidadScroll, fondo2->y());

    if(fondo1->x() <= -ANCHO_ESCENA){
        fondo1->setPos(fondo2->x() + ANCHO_ESCENA, fondo1->y());
    }
    if(fondo2->x() <= -ANCHO_ESCENA){
        fondo2->setPos(fondo1->x() + ANCHO_ESCENA, fondo2->y());
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
                puntos -= 100;
                // Si el edificio tenia un grupo de civiles sobre el techo
                // y aun no habia sido resuelto, cae abatido junto con el
                // edificio y se cuenta como perdido.
                Civil *civilDelEdificio = edificio->obtenerCivilAsociado();
                if (civilDelEdificio != nullptr && civilDelEdificio->estaActivo() && !civilDelEdificio->estaRescatado() && !civilDelEdificio->estaAplastado()){
                    civilDelEdificio->aplastar();
                    civilesPerdidos += civilDelEdificio->getCantidadPersonas();
                    puntos -= 100 * civilDelEdificio->getCantidadPersonas();
                }

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
                puntos -= 100;
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
                puntos -= 100 * civiles[i]->getCantidadPersonas();
            }else{
                bool cerca = civiles[i]->verificarCercania(helicX, helicY, DISTANCIA_RESCATE); //ahora cuando nos acerquemos al civil empezara un timer, (cuenta frames)
                civiles[i]->actualizarRescate(cerca);                                          //pero a segundos son 1.5, hasta que termine se rescatara el civil

                if(civiles[i]->estaRescatado() == true){//en el mismo frame que rescatamos, activo=false, asi evitamos que nos sume tantos puntos
                    civilesRescatados += civiles[i]->getCantidadPersonas();
                    puntos += 100 * civiles[i]->getCantidadPersonas();
                }
            }
        }
    }
}

void PantallaJuego::revisarRecoleccionCombustible(){
    const qreal DISTANCIA_RECOLECCION = 40.0;
    qreal helicX = helicoptero->x();
    qreal helicY = helicoptero->y();

    for(int i=0 ; i<numBidones ; i++){
        if(bidones[i]->estaRecogido() == true){
            continue;
        }

        qreal dx = helicX - bidones[i]->x();
        qreal dy = helicY - bidones[i]->y();
        qreal distancia = sqrt(dx * dx + dy * dy);

        if(distancia < DISTANCIA_RECOLECCION){
            bidones[i]->recoger();
            combustible += bidones[i]->getCantidadCombustible();
            if(combustible > 100.0){
                combustible = 100.0;
            }
        }
    }
}

void PantallaJuego::actualizarCombustible(){
    combustible -= obtenerConsumoCombustible();
    if(combustible < 0.0){
        combustible = 0.0;
    }
}

void PantallaJuego::revisarColisionesDisparos(){
    for(int i=numDisparos - 1 ; i>=0 ; i--){
        Disparo *disparo = disparos[i];
        bool disparoConsumido = false;

        if(disparo->esDeJugador() == true){
            // Contra civiles: un disparo del jugador tambien puede
            // matarlos por accidente si les da.

            for(int j=0 ; j<numCiviles ; j++){
                if(civiles[j]->estaActivo() == false){
                    continue;
                }
                if(disparo->sceneBoundingRect().intersects(civiles[j]->sceneBoundingRect()) == true){
                    civiles[j]->aplastar();
                    civilesPerdidos += civiles[j]->getCantidadPersonas();
                    puntos -= 100 * civiles[j]->getCantidadPersonas();
                    disparoConsumido = true;
                    break;
                }
            }

            // Contra helicopteros enemigos.
            if(disparoConsumido == false){
                for(int j=0 ; j<numObstaculos ; j++){
                    ObstaculoMovil *enemigo = dynamic_cast<ObstaculoMovil*>(obstaculos[j]);
                    if(enemigo == nullptr){
                        continue;
                    }
                    if(disparo->sceneBoundingRect().intersects(enemigo->sceneBoundingRect()) == true){
                        puntos += 25;
                        eliminarObstaculoEnIndice(j);
                        disparoConsumido = true;
                        break;
                    }
                }
            }

            // Contra la estructura bloqueadora.
            if(disparoConsumido == false && estructura != nullptr){
                if(disparo->sceneBoundingRect().intersects(estructura->sceneBoundingRect()) == true){
                    bool destruida = estructura->recibirImpacto();
                    disparoConsumido = true;

                    if(destruida == true){
                        escena->removeItem(estructura);
                        delete estructura;
                        estructura = nullptr;
                    }
                }
            }
        } else {
            // Disparo enemigo: solo puede danar al helicoptero del jugador.
            if(disparo->sceneBoundingRect().intersects(helicoptero->sceneBoundingRect()) == true){
                vidas--;
                puntos -= 100;
                disparoConsumido = true;
                if(vidas <= 0){
                    finalizarJuego(EstadoJuego::Derrota);
                }
            }
        }

        if(disparoConsumido == true){
            eliminarDisparoEnIndice(i);
        }
    }
}

void PantallaJuego::revisarColisionEstructura(){
    if(estructura == nullptr){
        return;
    }

    if(contadorInvulnerable > 0){
        contadorInvulnerable--;
        return;
    }

    QRectF rectHelicoptero = helicoptero->sceneBoundingRect();
    if(rectHelicoptero.intersects(estructura->sceneBoundingRect()) == true){
        vidas--;
        contadorInvulnerable = 40;
        // Se separa al helicoptero de la estructura para que no siga
        // tocandola de inmediato.
        helicoptero->setPos(helicoptero->x() - 40, helicoptero->y());

        if(vidas <= 0){
            finalizarJuego(EstadoJuego::Derrota);
        }
    }
}

void PantallaJuego::actualizarHUD(){
    int progreso = static_cast<int>(qMin(100.0, (distanciaRecorrida / distanciaMeta) * 100.0));
    QString texto = QString("Vidas: %1   |   Rescatados: %2   |   Perdidos: %3   |   Progreso: %4%   |   Puntos: %5")
                        .arg(vidas)
                        .arg(civilesRescatados)
                        .arg(civilesPerdidos)
                        .arg(progreso)
                        .arg(puntos);
    hud->setText(texto);
}

void PantallaJuego::actualizarHUDCombustible(){
    int porcentaje = static_cast<int>(combustible);
    QString texto = QString("Combustible: %1%").arg(porcentaje);
    hudCombustible->setText(texto);

    if(porcentaje <= 20){
        hudCombustible->setStyleSheet("background-color: rgba(20,20,20,200); color: rgb(230,80,80);"
                                      "padding: 4px 8px; font-weight: bold; border-radius: 4px;");
    } else {
        hudCombustible->setStyleSheet("background-color: rgba(20,20,20,200); color: rgb(240,210,80);"
                                      "padding: 4px 8px; font-weight: bold; border-radius: 4px;");
    }
    hudCombustible->adjustSize();
}

int PantallaJuego::obtenerIntervaloBidon() const{
    if(nivelJuego == 3){
        // El nivel 3 ya gasta mas combustible por frame (formula de
        // obtenerConsumoCombustible) y el viento hace mas lento llegar
        // a cada bidon, asi que aqui se rompe el patron de "menos
        // bidones segun sube el nivel" y se dejan aparecer mas seguido.
        return 220;
    }


    // Menos bidones a medida que sube el nivel: el intervalo crece,
    // asi aparecen con menos frecuencia.
    return 280 + (nivelJuego - 1) * 140;
}

double PantallaJuego::obtenerConsumoCombustible() const{
    return 0.045 + (nivelJuego - 1) * 0.015;
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

    // Avisar a MainWindow para que guarde el record del usuario.
    emit partidaTerminada(nivelJuego, puntos, victoria);
}

void PantallaJuego::reiniciarNivel(){
    limpiarNivel();
    panelResultado->hide();

    helicoptero->setPos(100, ALTO_ESCENA / 2);
    helicoptero->setSubiendo(false);
    helicoptero->setBajando(false);
    helicoptero->setIzquierda(false);
    helicoptero->setDerecha(false);
    helicoptero->reiniciarFisica();

    vidas = 3;
    civilesRescatados = 0;
    civilesTotalNivel = 0;
    civilesPerdidos = 0;
    puntos = 0;
    distanciaRecorrida = 0;
    combustible = 100.0;
    contadorFramesBidon = 0;
    intervaloBidon = obtenerIntervaloBidon();
    contadorFramesEdificio = 0;
    contadorFramesEnemigo = 0;
    teclaEspacioPresionada = false;
    contadorEnfriamientoDisparo = 0;
    contadorInvulnerable = 0;
    estado = EstadoJuego::Jugando;
    juegoIniciado = false;

    actualizarHUD();
    actualizarHUDCombustible();
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

    qreal fuerzaEmpujeX = 0.0;
    qreal fuerzaEmpujeY = 0.0;
    obtenerFuerzaEmpuje(fuerzaEmpujeX, fuerzaEmpujeY);
    helicoptero->actualizarFisica(ANCHO_ESCENA, ALTO_ESCENA - ALTURA_SUELO, fuerzaEmpujeX, fuerzaEmpujeY);

    // El piso destruye el helicoptero de inmediato, sin importar las vidas.
    if (helicoptero->tocoElSuelo()) {
        finalizarJuego(EstadoJuego::Derrota);
        return;
    }

    distanciaRecorrida += velocidadScroll;
    if(usaEstructuraBloqueadora() == true && estructura != nullptr && estructura->estaDestruida() == false){
        // No se puede "avanzar" mas alla de la meta mientras la
        // estructura siga viva: se congela el progreso justo en el
        // limite hasta que la destruyan.
        if(distanciaRecorrida > distanciaMeta){
            distanciaRecorrida = distanciaMeta;
        }
    }

    contadorFramesEdificio++;
    if (contadorFramesEdificio >= intervaloEdificio) {
        generarEdificio();
        contadorFramesEdificio = 0;
        // Con velocidadScroll=1.6, 140-260 frames = 224 a 416 px de
        // separacion, siempre mayor que el ancho maximo posible de un
        // edificio (~160px), para que nunca se amontonen entre si.
        intervaloEdificio = 140 + (rand() % 120);
    }

    contadorFramesEnemigo++;
    if (contadorFramesEnemigo >= intervaloEnemigo) {
        generarEnemigo();
        contadorFramesEnemigo = 0;
        // 200-380 frames = 320 a 608 px de separacion, bastante mas que
        // el ancho maximo de un enemigo (~100px).
        intervaloEnemigo = 200 + (rand() % 180);
    }

    contadorFramesBidon++;
    if(contadorFramesBidon >= intervaloBidon){
        generarBidon();
        contadorFramesBidon = 0;
        intervaloBidon = obtenerIntervaloBidon() + (rand() % 100);
    }

    if(usaEstructuraBloqueadora() == true && estructuraGenerada == false && distanciaRecorrida >= distanciaMeta * 0.55){
        generarEstructura();
    }

    if(jugadorTieneArmas() == true){
        if(contadorEnfriamientoDisparo > 0){
            contadorEnfriamientoDisparo--;
        }
        if(teclaEspacioPresionada == true && contadorEnfriamientoDisparo <= 0){
            dispararJugador();
            contadorEnfriamientoDisparo = 14; // ~0.22s entre disparos a 60fps
        }
        dispararEnemigos();
    }

    actualizarObstaculos();
    actualizarCiviles();
    actualizarBidones();
    actualizarCombustible();
    actualizarFondo();
    actualizarPiso();
    actualizarDisparos();
    actualizarEstructura();

    eliminarObstaculosFuera();
    eliminarCivilesFuera();
    eliminarBidonesFuera();
    eliminarDisparosFuera();

    revisarColisiones();
    if (estado != EstadoJuego::Jugando) {
        return;
    }

    revisarRescates();
    revisarRecoleccionCombustible();
    revisarColisionEstructura();
    if (estado != EstadoJuego::Jugando) {
        return;
    }

    revisarColisionesDisparos();
    if (estado != EstadoJuego::Jugando) {
        return;
    }

    // Sin combustible, el helicoptero se queda sin poder de vuelo:
    // se cuenta como derrota, igual que chocar contra el suelo.
    if(combustible <= 0.0){
        finalizarJuego(EstadoJuego::Derrota);
        return;
    }

    bool puedeTerminar = true;
    if(usaEstructuraBloqueadora() == true && estructura != nullptr && estructura->estaDestruida() == false){
        puedeTerminar = false;
    }

    if (distanciaRecorrida >= distanciaMeta && puedeTerminar == true) {
        finalizarJuego(EstadoJuego::Victoria);
        return;
    }

    actualizarHUD();
    actualizarHUDCombustible();
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
    case Qt::Key_Space:
        teclaEspacioPresionada = true;
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
    case Qt::Key_Space:
        teclaEspacioPresionada = false;
        break;
    }
    QWidget::keyReleaseEvent(event);
}