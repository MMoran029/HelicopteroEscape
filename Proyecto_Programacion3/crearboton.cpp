#include "crearboton.h"

CrearBoton::CrearBoton(const QString &texto, QWidget *parent) : QPushButton(texto, parent){
    estiloNormal =
        "QPushButton { background-color: rgb(230,170,30); color: black; "
        "font-size: 18px; font-weight: bold; border-radius: 10px; "
        "padding: 8px; border: 2px solid rgb(180,130,20); }";

    estiloHover =
        "QPushButton { background-color: rgb(255,200,60); color: black; "
        "font-size: 18px; font-weight: bold; border-radius: 10px; "
        "padding: 8px; border: 2px solid rgb(230,170,30); }";

    estiloPresionado =
        "QPushButton { background-color: rgb(200,140,10); color: black; "
        "font-size: 18px; font-weight: bold; border-radius: 10px; "
        "padding: 8px; border: 2px solid rgb(150,100,10); }";

    setStyleSheet(estiloNormal);
    setCursor(Qt::PointingHandCursor);

    animacion = new QPropertyAnimation(this, "geometry");
}

void CrearBoton::enterEvent(QEnterEvent *event){
    Q_UNUSED(event);
    geometriaOriginal = geometry();
    QRect destino = geometriaOriginal.adjusted(-4, -4, 4, 4);
    setStyleSheet(estiloHover);
    animacion->setDuration(120);
    animarA(destino);
}

void CrearBoton::leaveEvent(QEvent *event){
    Q_UNUSED(event);
    setStyleSheet(estiloNormal);
    animacion->setDuration(120);
    animarA(geometriaOriginal);
}

void CrearBoton::mousePressEvent(QMouseEvent *event){
    setStyleSheet(estiloPresionado);
    QRect destino = geometriaOriginal.adjusted(2, 2, -2, -2);
    animacion->setDuration(60);
    animarA(destino);
    QPushButton::mousePressEvent(event);
}

void CrearBoton::mouseReleaseEvent(QMouseEvent *event){
    setStyleSheet(estiloHover);
    QRect destino = geometriaOriginal.adjusted(-4, -4, 4, 4);
    animacion->setDuration(60);
    animarA(destino);
    QPushButton::mouseReleaseEvent(event);
}

void CrearBoton::animarA(const QRect &destino){
    animacion->stop();
    animacion->setStartValue(geometry());
    animacion->setEndValue(destino);
    animacion->start();
}