#include "civil.h"
#include <cmath>
#include <iostream>
using namespace std;

Civil::Civil(qreal posX, qreal posY, int cantidadPersonas)
    : rescatado(false), activo(true), aplastado(false), puntosRescate(100 * cantidadPersonas),
    cantidadPersonas(cantidadPersonas), tiempoParpadeo(0), contadorRescate(0){
    setPos(posX, posY);

    imagen = obtenerImagen(cantidadPersonas);

    const qreal ALTO_OBJETIVO = alturaVisual();
    if (imagen.height() > 0) {
        qreal proporcion = static_cast<qreal>(imagen.width()) / static_cast<qreal>(imagen.height());
        altoImagen = ALTO_OBJETIVO;
        anchoImagen = ALTO_OBJETIVO * proporcion;
    } else {
        anchoImagen = 28;
        altoImagen = 32;
    }
}

QPixmap Civil::obtenerImagen(int cantidadPersonas){
    if (cantidadPersonas >= 3) {
        return QPixmap(":/imagenes/Imagenes/Persona_3.png");
    }
    if (cantidadPersonas == 2) {
        return QPixmap(":/imagenes/Imagenes/Persona_2.png");
    }
    return QPixmap(":/imagenes/Imagenes/Persona_1.png");
}

QRectF Civil::boundingRect() const{
    const qreal MARGEN_CAJA = 6.0;
    return QRectF(-anchoImagen / 2 - MARGEN_CAJA, -altoImagen / 2 - MARGEN_CAJA,anchoImagen + MARGEN_CAJA * 2, altoImagen + MARGEN_CAJA * 2);
}
void Civil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    QRectF destino = boundingRect();

    if (aplastado == true) {
        // Se dibuja atenuado, como si hubiese quedado aplastado.
        painter->setOpacity(0.45);
        painter->drawPixmap(destino.toRect(), imagen);
        painter->setOpacity(1.0);
        return;
    }

    painter->drawPixmap(destino.toRect(), imagen);
    if(estaSiendoRescatado()==true){
        QColor colorCaja;

        if(contadorRescate < FRAMES_PARA_RESCATE / 3){
            colorCaja = QColor(255, 140, 0); // color naranja
        }else if(contadorRescate < (FRAMES_PARA_RESCATE * 2) / 3){
            colorCaja = QColor(255, 220, 0); //color amarillo
        }else{
            colorCaja = QColor(90, 220, 90); // color verde
        }

        painter->setPen(QPen(colorCaja, 3));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(destino.adjusted(-3, -3, 3, 3), 6, 6); //aqui se crea la cajita alrededor del civil
    }

    if (rescatado == true) {
        // Contorno verde de confirmacion (visible el ultimo frame antes
        // de que PantallaJuego lo retire de la escena).
        painter->setPen(QPen(QColor(90, 220, 90), 3));
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(destino.adjusted(-3, -3, 3, 3), 6, 6);
    }
}

bool Civil::estaRescatado() const{
    return rescatado;
}

bool Civil::estaActivo() const{
    return activo;
}

bool Civil::estaAplastado() const{
    return aplastado;
}

void Civil::rescatar(){
    if(activo == true && rescatado == false && aplastado == false){
        rescatado = true;
        activo = false;
        cout<<"Civil rescatado, puntos ganados: "<<puntosRescate<<endl;
    }
}

void Civil::aplastar(){
    if(activo == true && rescatado == false && aplastado == false){
        aplastado = true;
        activo = false;
        cout<<"Civil aplastado por el helicoptero, personas perdidas: "<<cantidadPersonas<<endl;
    }
}

void Civil::desactivar(){
    activo = false;
}

bool Civil::verificarCercania(qreal helicX, qreal helicY, qreal distanciaMax){
    double dx = helicX - x();
    double dy = helicY - y();
    double distancia = sqrt(dx * dx + dy * dy);
    if(distancia < distanciaMax){
        return true;
    }
    return false;
}

void Civil::actualizarRescate(bool helicopteroCerca){
    if(activo==false || rescatado==true || aplastado==true){
        return;
    }

    if(helicopteroCerca==true){
        contadorRescate++;
        if(contadorRescate >= FRAMES_PARA_RESCATE){//si se cumplen los 1.5 segundos se rescatara los o el civil
            rescatar();
        }
    }else{
        contadorRescate = 0;
    }
}

bool Civil::estaSiendoRescatado() const{
    if(contadorRescate > 0 && rescatado == false && aplastado == false){
        return true;
    }
    return false;
}

void Civil::actualizar(qreal velocidadScroll){
    setPos(x() - velocidadScroll, y());
}

bool Civil::fueraDePantalla() const{
    return x() < -anchoImagen;
}