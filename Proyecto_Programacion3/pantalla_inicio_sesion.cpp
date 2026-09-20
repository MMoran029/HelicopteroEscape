#include "pantalla_inicio_sesion.h"
#include "gestorusuarios.h"
#include <QPainter>
#include <QMessageBox>
#include <iostream>
using namespace std;

pantalla_inicio_sesion::pantalla_inicio_sesion(QWidget *parent) : QWidget(parent) {
    // Fondos: el de login es inicio_sesion.png y el de registro es registrarse.png.
    // Se prueban varias rutas porque el .qrc mezcla "Imagenes/" e "imagenes/".
    const char *candidatosInicio[] = {
        ":/imagenes/Imagenes/inicio_sesion.png",
        ":/imagenes/inicio_sesion.png",
        ":/imagenes/menu_principal.png"
    };
    const char *candidatosRegistro[] = {
        ":/imagenes/Imagenes/registrarse.png",
        ":/imagenes/registrarse.png",
        ":/imagenes/Imagenes/inicio_sesion.png"
    };
    for (const char *ruta : candidatosInicio) {
        if (m_fondoInicio.load(ruta)) break;
    }
    for (const char *ruta : candidatosRegistro) {
        if (m_fondoRegistro.load(ruta)) break;
    }
    if (m_fondoInicio.isNull()) {
        cout << "[ERROR] no se pudo cargar la imagen de fondo de inicio de sesion" << endl;
    }
    if (m_fondoRegistro.isNull()) {
        cout << "[ERROR] no se pudo cargar la imagen de fondo de registrarse" << endl;
        m_fondoRegistro = m_fondoInicio; // respaldo para no pintar vacio
    }

    configurarBotones();
    configurarFormulario();
    cambiarModo(Modo::Menu);
}

void pantalla_inicio_sesion::configurarBotones() {
    m_botonIniciarSesion = new CrearBoton("INICIAR SESION", this);
    m_botonRegistrarse = new CrearBoton("REGISTRARSE", this);
    m_botonSalir = new CrearBoton("SALIR", this);

    m_botonIniciarSesion->resize(200, 50);
    m_botonRegistrarse->resize(200, 50);
    m_botonSalir->resize(200, 50);

    connect(m_botonIniciarSesion, &QPushButton::clicked, this, &pantalla_inicio_sesion::onBotonIniciarSesion);
    connect(m_botonRegistrarse, &QPushButton::clicked, this, &pantalla_inicio_sesion::onBotonRegistrarse);
    connect(m_botonSalir, &QPushButton::clicked, this, &pantalla_inicio_sesion::salirPresionado);
}

void pantalla_inicio_sesion::aplicarEstiloCampos() {
    const QString estiloEtiqueta =
        "QLabel { color: white; font-size: 16px; font-weight: bold; "
        "background: transparent; }";
    const QString estiloCampo =
        "QLineEdit { background-color: rgba(0, 0, 0, 160); color: white; "
        "font-size: 16px; border: 2px solid rgb(230,170,30); "
        "border-radius: 8px; padding: 6px; }";

    m_etiquetaUsuario->setStyleSheet(estiloEtiqueta);
    m_etiquetaContrasena->setStyleSheet(estiloEtiqueta);
    m_campoUsuario->setStyleSheet(estiloCampo);
    m_campoContrasena->setStyleSheet(estiloCampo);
}

void pantalla_inicio_sesion::configurarFormulario() {
    m_etiquetaUsuario = new QLabel("USUARIO:", this);
    m_campoUsuario = new QLineEdit(this);
    m_campoUsuario->setPlaceholderText("Ingrese su nombre de usuario");
    m_campoUsuario->setMaxLength(20);

    m_etiquetaContrasena = new QLabel("CONTRASENA:", this);
    m_campoContrasena = new QLineEdit(this);
    m_campoContrasena->setPlaceholderText("Ingrese su contrasena");
    m_campoContrasena->setEchoMode(QLineEdit::Password);
    m_campoContrasena->setMaxLength(20);

    m_botonAceptar = new CrearBoton("ENTRAR", this);
    m_botonVolver = new CrearBoton("VOLVER", this);
    m_botonAceptar->resize(200, 50);
    m_botonVolver->resize(200, 50);

    aplicarEstiloCampos();

    connect(m_botonAceptar, &QPushButton::clicked, this, &pantalla_inicio_sesion::onBotonAceptar);
    connect(m_botonVolver, &QPushButton::clicked, this, &pantalla_inicio_sesion::onBotonVolver);
    // Enter en cualquier campo tambien acepta.
    connect(m_campoUsuario, &QLineEdit::returnPressed, this, &pantalla_inicio_sesion::onBotonAceptar);
    connect(m_campoContrasena, &QLineEdit::returnPressed, this, &pantalla_inicio_sesion::onBotonAceptar);
}

void pantalla_inicio_sesion::cambiarModo(Modo nuevo) {
    m_modo = nuevo;
    bool enMenu = (m_modo == Modo::Menu);

    m_botonIniciarSesion->setVisible(enMenu);
    m_botonRegistrarse->setVisible(enMenu);
    m_botonSalir->setVisible(enMenu);

    bool enForm = !enMenu;
    m_etiquetaUsuario->setVisible(enForm);
    m_campoUsuario->setVisible(enForm);
    m_etiquetaContrasena->setVisible(enForm);
    m_campoContrasena->setVisible(enForm);
    m_botonAceptar->setVisible(enForm);
    m_botonVolver->setVisible(enForm);

    if (enForm) {
        m_campoUsuario->clear();
        m_campoContrasena->clear();
        m_botonAceptar->setText(m_modo == Modo::Registrarse ? "CREAR CUENTA" : "ENTRAR");
        m_campoUsuario->setFocus();
    }

    if (enMenu) {
        acomodarBotones();
    } else {
        acomodarFormulario();
    }
    update(); // repintar con el fondo que toca
}

void pantalla_inicio_sesion::mostrarMenu() {
    cambiarModo(Modo::Menu);
}

void pantalla_inicio_sesion::onBotonIniciarSesion() {
    // Validacion pedida: si aun no hay ningun usuario registrado, avisar.
    if (!GestorUsuarios::hayUsuariosRegistrados()) {
        QMessageBox::information(this, "Sin usuarios",
            "Todavia no hay ningun usuario registrado.\nPor favor registrese primero.");
        return;
    }
    cambiarModo(Modo::IniciarSesion);
}

void pantalla_inicio_sesion::onBotonRegistrarse() {
    cambiarModo(Modo::Registrarse);
}

void pantalla_inicio_sesion::onBotonVolver() {
    cambiarModo(Modo::Menu);
}

void pantalla_inicio_sesion::onBotonAceptar() {
    QString nombre = m_campoUsuario->text().trimmed();
    QString clave = m_campoContrasena->text();

    if (nombre.isEmpty() || clave.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campos vacios",
            "Ingrese el nombre de usuario y la contrasena.");
        return;
    }

    if (m_modo == Modo::Registrarse) {
        QString error;
        if (!GestorUsuarios::registrarUsuario(nombre, clave, error)) {
            QMessageBox::warning(this, "No se pudo registrar", error);
            return;
        }
        QMessageBox::information(this, "Registro exitoso",
            QString("Usuario '%1' registrado.\nYa puede iniciar sesion.")
                .arg(GestorUsuarios::sanitizarNombre(nombre)));
        cambiarModo(Modo::IniciarSesion); // pasa directo al login
        return;
    }

    // Modo::IniciarSesion
    QString error;
    if (!GestorUsuarios::validarCredenciales(nombre, clave, error)) {
        QMessageBox::warning(this, "No se pudo iniciar sesion", error);
        return;
    }
    m_usuarioActual = GestorUsuarios::sanitizarNombre(nombre);
    emit sesionIniciada(m_usuarioActual);
}

void pantalla_inicio_sesion::acomodarBotones() {
    int totalBotones = 3;
    int alturaBoton = m_botonIniciarSesion->height();
    int espacio = 62;
    int alturaTotal = (totalBotones - 1) * espacio + alturaBoton;

    int posX = (width() - m_botonIniciarSesion->width()) / 2;
    int posYInicial = height() - alturaTotal - 40;

    if (posYInicial < 10) {
        posYInicial = 10;
    }

    m_botonIniciarSesion->move(posX, posYInicial);
    m_botonRegistrarse->move(posX, posYInicial + espacio);
    m_botonSalir->move(posX, posYInicial + espacio * 2);
}

void pantalla_inicio_sesion::acomodarFormulario() {
    // Formulario centrado: 2 filas etiqueta+campo y 2 botones debajo.
    int anchoCampo = 280;
    int altoCampo = 34;
    int anchoBoton = 200;
    int altoBoton = 50;

    int cx = width() / 2;
    int cy = height() / 2 - 20;

    m_etiquetaUsuario->resize(anchoCampo, 22);
    m_etiquetaUsuario->move(cx - anchoCampo / 2, cy - 70);
    m_campoUsuario->resize(anchoCampo, altoCampo);
    m_campoUsuario->move(cx - anchoCampo / 2, cy - 46);

    m_etiquetaContrasena->resize(anchoCampo, 22);
    m_etiquetaContrasena->move(cx - anchoCampo / 2, cy + 2);
    m_campoContrasena->resize(anchoCampo, altoCampo);
    m_campoContrasena->move(cx - anchoCampo / 2, cy + 26);

    m_botonAceptar->resize(anchoBoton, altoBoton);
    m_botonVolver->resize(anchoBoton, altoBoton);
    m_botonAceptar->move(cx - anchoBoton / 2, cy + 80);
    m_botonVolver->move(cx - anchoBoton / 2, cy + 80 + altoBoton + 12);
}

void pantalla_inicio_sesion::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    const QPixmap &fondo = (m_modo == Modo::Registrarse) ? m_fondoRegistro : m_fondoInicio;
    if (!fondo.isNull()) {
        painter.drawPixmap(rect(), fondo);
    } else {
        painter.fillRect(rect(), Qt::black);
    }
}

void pantalla_inicio_sesion::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    if (m_modo == Modo::Menu) {
        acomodarBotones();
    } else {
        acomodarFormulario();
    }
}
