#include "civil.h"
#include <cmath>
#include <iostream>
using namespace std;

Civil::Civil(qreal posX, qreal posY) : rescatado(false), activo(true), puntosRescate(100), tiempoParpadeo(0){
    setPos(posX, posY);
}

QRectF Civil::boundingRect() const{
    return QRectF(-10, -10, 20, 20);
}

void Civil::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    if(rescatado == true){
        painter->setBrush(QColor(80, 200, 80));
    } else {
        painter->setBrush(QColor(200, 200, 40));
    }
    painter->setPen(Qt::black);
    painter->drawEllipse(-8, -8, 16, 16);
}

bool Civil::estaRescatado() const{
    return rescatado;
}

bool Civil::estaActivo() const{
    return activo;
}

void Civil::rescatar(){
    if(activo == true && rescatado == false){
        rescatado = true;
        activo = false;
        cout<<"Civil rescatado, puntos ganados: "<<puntosRescate<<endl;
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

// RECORDATORIO: aun no esta conectado