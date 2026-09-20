#ifndef PANTALLA_INICIO_SESION_H
#define PANTALLA_INICIO_SESION_H
#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QLineEdit>
#include <QLabel>
#include "crearboton.h"

// Pantalla de acceso: es la primera que se muestra al abrir el juego.
// Tiene 3 modos internos:
//   Menu          -> 3 botones: INICIAR SESION / REGISTRARSE / SALIR
//   IniciarSesion -> formulario (usuario + contrasena) con fondo inicio_sesion.png
//   Registrarse   -> formulario (usuario + contrasena) con fondo registrarse.png
class pantalla_inicio_sesion : public QWidget {
    Q_OBJECT
public:
    enum class Modo { Menu, IniciarSesion, Registrarse };

    explicit pantalla_inicio_sesion(QWidget *parent = nullptr);

    QString usuarioActual() const { return m_usuarioActual; }
    void mostrarMenu(); // vuelve al menu de los 3 botones (la usa MainWindow al entrar)

signals:
    void sesionIniciada(const QString &nombreUsuario);
    void salirPresionado();

private slots:
    void onBotonIniciarSesion();
    void onBotonRegistrarse();
    void onBotonAceptar();
    void onBotonVolver();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Modo m_modo = Modo::Menu;
    QString m_usuarioActual;

    QPixmap m_fondoInicio;
    QPixmap m_fondoRegistro;

    // Modo Menu
    CrearBoton *m_botonIniciarSesion = nullptr;
    CrearBoton *m_botonRegistrarse = nullptr;
    CrearBoton *m_botonSalir = nullptr;

    // Modo formulario (login / registro)
    QLabel *m_etiquetaUsuario = nullptr;
    QLineEdit *m_campoUsuario = nullptr;
    QLabel *m_etiquetaContrasena = nullptr;
    QLineEdit *m_campoContrasena = nullptr;
    CrearBoton *m_botonAceptar = nullptr;
    CrearBoton *m_botonVolver = nullptr;

    void configurarBotones();
    void configurarFormulario();
    void cambiarModo(Modo nuevo);
    void acomodarBotones();
    void acomodarFormulario();
    void aplicarEstiloCampos();
};
#endif // PANTALLA_INICIO_SESION_H
