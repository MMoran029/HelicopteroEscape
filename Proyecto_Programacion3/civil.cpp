#include "civil.h"
#include <cmath>
#include <iostream>
using namespace std;

Civil::Civil(qreal posX, qreal posY, int cantidadPersonas)
    : rescatado(false), activo(true), aplastado(false), puntosRescate(100 * cantidadPersonas),
    cantidadPersonas(cantidadPersonas), tiempoParpadeo(0){
    setPos(posX, posY);
}

QRectF Civil::boundingRect() const{
    return QRectF(-20, -16, 40, 32);
}

void Civil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    if(aplastado == true){
        painter->setBrush(QColor(120, 30, 30));
    } else if(rescatado == true){
        painter->setBrush(QColor(80, 200, 80));
    } else {
        painter->setBrush(QColor(200, 200, 40));
    }
    painter->setPen(Qt::black);
    // Un solo rectangulo grande representa a todo el grupo de civiles.
    // RECORDATORIO: aqui se puede reemplazar por painter->drawPixmap(...)
    // cuando se tenga la imagen del grupo de civiles.
    painter->drawRoundedRect(-18, -14, 36, 28, 4, 4);

    painter->setPen(Qt::black);
    painter->drawText(QRectF(-18, -14, 36, 28), Qt::AlignCenter, QString::number(cantidadPersonas));
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

void Civil::actualizar(qreal velocidadScroll){
    setPos(x() - velocidadScroll, y());
}

bool Civil::fueraDePantalla() const{
    return x() < -20;
}