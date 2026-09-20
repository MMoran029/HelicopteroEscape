#ifndef PANTALLARANKING_H
#define PANTALLARANKING_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTableWidget>
#include "crearboton.h"

// Pantalla de ranking con fondo ranking.png.
// Tiene 4 apartados (pestañas):
//   NIVEL 1, NIVEL 2, NIVEL 3 -> mejor puntaje de cada nivel.
//   GENERAL -> suma de los 3 puntajes de cada usuario.
// Los datos salen de USUARIOS/<nombre>/datos.txt via GestorUsuarios.
class PantallaRanking : public QWidget {
    Q_OBJECT
public:
    explicit PantallaRanking(QWidget *parent = nullptr);

public slots:
    // Recarga la tabla con los datos actuales. Llamarla cada vez
    // que se vaya a mostrar la pantalla.
    void recargar();

signals:
    void volverPresionado();

private slots:
    void mostrarNivel1();
    void mostrarNivel2();
    void mostrarNivel3();
    void mostrarGeneral();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    // 1, 2, 3 = nivel; 4 = general.
    int m_categoria = 1;

    QPixmap m_fondo;
    CrearBoton *m_botonNivel1 = nullptr;
    CrearBoton *m_botonNivel2 = nullptr;
    CrearBoton *m_botonNivel3 = nullptr;
    CrearBoton *m_botonGeneral = nullptr;
    QTableWidget *m_tabla = nullptr;
    CrearBoton *m_botonVolver = nullptr;

    void configurarElementos();
    void aplicarEstiloTabla();
    void acomodarElementos();
    void mostrarCategoria(int categoria);
    void llenarTabla();
};
#endif // PANTALLARANKING_H
