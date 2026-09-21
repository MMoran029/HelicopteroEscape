#include "barrahud.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPen>

BarraHUD::BarraHUD(const QString &etiqueta, const QColor &colorRelleno, QWidget *parent)
    : QWidget(parent), etiqueta(etiqueta), colorRelleno(colorRelleno), porcentaje(0)
{
    // No debe robar clics: los eventos de raton pasan a la vista de juego.
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void BarraHUD::setValor(int valor)
{
    // Recorta el valor al rango 0-100.
    if (valor < 0) {
        valor = 0;
    }
    if (valor > 100) {
        valor = 100;
    }
    if (valor != porcentaje) {
        porcentaje = valor;
        update();
    }
}

void BarraHUD::setEtiqueta(const QString &nuevaEtiqueta)
{
    etiqueta = nuevaEtiqueta;
    update();
}

void BarraHUD::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect area = rect();

    // Fondo negro semitransparente.
    painter.fillRect(area, QColor(0, 0, 0, 170));

    // Relleno de color proporcional al valor.
    int anchoRelleno = static_cast<int>((area.width() * porcentaje) / 100.0);
    if (anchoRelleno > 0) {
        painter.fillRect(QRect(area.left(), area.top(), anchoRelleno, area.height()), colorRelleno);
    }

    // Contorno negro de 3px.
    QPen pen(Qt::black, 3);
    painter.setPen(pen);
    painter.drawRect(area.adjusted(1, 1, -1, -1));

    // Texto centrado "ETIQUETA 45%", siempre blanco y en negrita.
    painter.setPen(Qt::white);
    QFont fuente = painter.font();
    fuente.setBold(true);
    painter.setFont(fuente);
    QString texto = QString("%1 %2%").arg(etiqueta).arg(porcentaje);
    painter.drawText(area, Qt::AlignCenter, texto);
}
