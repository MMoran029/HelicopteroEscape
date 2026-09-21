#ifndef BARRAHUD_H
#define BARRAHUD_H

#include <QWidget>
#include <QColor>
#include <QString>

// Barra de HUD pintada con QPainter (sin Q_OBJECT): fondo negro
// semitransparente, relleno de color proporcional al valor 0-100,
// contorno negro de 3px y texto centrado "ETIQUETA 45%" siempre en
// blanco y en negrita. No intercepta clics (WA_TransparentForMouseEvents).
class BarraHUD : public QWidget
{
public:
    explicit BarraHUD(const QString &etiqueta, const QColor &colorRelleno,
                      QWidget *parent = nullptr);
    void setValor(int valor);
    int valor() const { return porcentaje; }
    void setEtiqueta(const QString &nuevaEtiqueta);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString etiqueta;
    QColor colorRelleno;
    int porcentaje;
};

#endif // BARRAHUD_H
