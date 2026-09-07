#ifndef PANELRESULTADO_H
#define PANELRESULTADO_H
#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QResizeEvent>
#include "crearboton.h"

// Panel de resultado (victoria o derrota) que se superpone sobre
// PantallaJuego al terminar la partida. Muestra un titulo, un resumen
// de estadisticas, y botones para reiniciar, elegir mision, volver al
// menu o (si hubo victoria) pasar al siguiente nivel.
class PanelResultado : public QWidget {
    Q_OBJECT
public:
    PanelResultado(QWidget *parent = nullptr);

    // Configura el titulo/color/boton segun el resultado, actualiza el
    // texto de estadisticas y hace visible el panel.
    void mostrarResultado(bool victoria, const QString &infoTexto);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void reiniciarPresionado();
    void menuPresionado();
    void misionesPresionado();
    void siguienteNivelPresionado();

private:
    QLabel *labelTitulo;
    QLabel *labelInfo;
    CrearBoton *botonReiniciar;
    CrearBoton *botonMenu;
    CrearBoton *botonMisiones;
    CrearBoton *botonSiguienteNivel;

    void configurarElementos();
    void acomodarElementos();
};
#endif // PANELRESULTADO_H
