#ifndef CREARBOTON_H
#define CREARBOTON_H
#include <QPushButton>
#include <QPropertyAnimation>
#include <QEnterEvent>
#include <QMouseEvent>

class CrearBoton : public QPushButton {
    Q_OBJECT
public:
    CrearBoton(const QString &texto, QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPropertyAnimation *animacion;
    QRect geometriaOriginal;
    QString estiloNormal;
    QString estiloHover;
    QString estiloPresionado;

    void animarA(const QRect &destino);
};
#endif // CREARBOTON_H