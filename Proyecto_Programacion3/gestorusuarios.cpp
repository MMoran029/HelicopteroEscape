#include "gestorusuarios.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QCoreApplication>
#include <algorithm>

QString GestorUsuarios::rutaBase() {
    // Carpeta "USUARIOS" junto al ejecutable.
    QString ruta = QCoreApplication::applicationDirPath() + "/USUARIOS";
    QDir dir(ruta);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    return ruta;
}

QString GestorUsuarios::sanitizarNombre(const QString &nombre) {
    QString limpio = nombre.trimmed();
    const QString prohibidos = "\\/:*?\"<>|";
    for (QChar c : prohibidos) {
        limpio.remove(c);
    }
    // No permitir "." ni ".." como nombre de carpeta.
    if (limpio == "." || limpio == "..") {
        limpio.clear();
    }
    return limpio;
}

QString GestorUsuarios::rutaCarpetaUsuario(const QString &nombreSanitizado) {
    return rutaBase() + "/" + nombreSanitizado;
}

QString GestorUsuarios::rutaDatosUsuario(const QString &nombreSanitizado) {
    return rutaCarpetaUsuario(nombreSanitizado) + "/datos.txt";
}

QString GestorUsuarios::claveNivel(int nivel) {
    if (nivel == 4) {
        return QString("mejorPuntajeSupervivencia");
    }
    if (nivel < 1) nivel = 1;
    if (nivel > 4) nivel = 4;
    return QString("mejorPuntajeNivel%1").arg(nivel);
}

static QMap<QString, QString> leerDatos(const QString &ruta) {
    QMap<QString, QString> datos;
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return datos;
    }
    QTextStream in(&archivo);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        int idx = linea.indexOf('=');
        if (idx > 0) {
            datos[linea.left(idx).trimmed()] = linea.mid(idx + 1).trimmed();
        }
    }
    archivo.close();
    return datos;
}

static bool escribirDatos(const QString &ruta, const QMap<QString, QString> &datos) {
    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }
    QTextStream out(&archivo);
    // Orden fijo para que el archivo siempre se vea igual.
    static const QStringList orden = {
        "nombre", "contrasena",
        "mejorPuntajeNivel1", "mejorPuntajeNivel2", "mejorPuntajeNivel3",
        "mejorPuntajeSupervivencia",
        "mejorSupervivenciaTiempo", "mejorSupervivenciaDistancia"
    };
    for (const QString &clave : orden) {
        if (datos.contains(clave)) {
            out << clave << "=" << datos.value(clave) << "\n";
        }
    }
    // Cualquier clave extra que exista se conserva al final.
    for (auto it = datos.constBegin(); it != datos.constEnd(); ++it) {
        if (!orden.contains(it.key())) {
            out << it.key() << "=" << it.value() << "\n";
        }
    }
    archivo.close();
    return true;
}

int GestorUsuarios::leerClave(const QMap<QString, QString> &datos, const QString &clave) {
    bool ok = false;
    int valor = datos.value(clave, "0").toInt(&ok);
    return ok ? valor : 0;
}

QStringList GestorUsuarios::listaUsuarios() {
    QDir dir(rutaBase());
    QStringList carpetas = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    // Solo contar carpetas que tengan su datos.txt (usuarios validos).
    QStringList usuarios;
    for (const QString &carpeta : carpetas) {
        QFile datos(rutaDatosUsuario(carpeta));
        if (datos.exists()) {
            usuarios << carpeta;
        }
    }
    return usuarios;
}

bool GestorUsuarios::hayUsuariosRegistrados() {
    return !listaUsuarios().isEmpty();
}

bool GestorUsuarios::existeUsuario(const QString &nombre) {
    QString limpio = sanitizarNombre(nombre);
    if (limpio.isEmpty()) {
        return false;
    }
    return QFile::exists(rutaDatosUsuario(limpio));
}

bool GestorUsuarios::registrarUsuario(const QString &nombre, const QString &contrasena,
                                      QString &mensajeError) {
    QString limpio = sanitizarNombre(nombre);
    QString clave = contrasena.trimmed();

    if (limpio.isEmpty()) {
        mensajeError = "Ingrese un nombre de usuario valido.";
        return false;
    }
    if (clave.isEmpty()) {
        mensajeError = "Ingrese una contrasena valida.";
        return false;
    }
    if (limpio != nombre.trimmed()) {
        mensajeError = "El nombre contiene caracteres no permitidos (\\ / : * ? \" < > |).";
        return false;
    }
    if (existeUsuario(limpio)) {
        mensajeError = "Ese nombre de usuario ya esta registrado.";
        return false;
    }

    QDir dir;
    if (!dir.mkpath(rutaCarpetaUsuario(limpio))) {
        mensajeError = "No se pudo crear la carpeta del usuario.";
        return false;
    }

    QMap<QString, QString> datos;
    datos["nombre"] = limpio;
    datos["contrasena"] = clave;
    datos["mejorPuntajeNivel1"] = "0";
    datos["mejorPuntajeNivel2"] = "0";
    datos["mejorPuntajeNivel3"] = "0";
    datos["mejorPuntajeSupervivencia"] = "0";
    datos["mejorSupervivenciaTiempo"] = "0";
    datos["mejorSupervivenciaDistancia"] = "0";
    if (!escribirDatos(rutaDatosUsuario(limpio), datos)) {
        mensajeError = "No se pudo crear el archivo de datos del usuario.";
        return false;
    }
    return true;
}

bool GestorUsuarios::validarCredenciales(const QString &nombre, const QString &contrasena,
                                         QString &mensajeError) {
    QString limpio = sanitizarNombre(nombre);

    if (limpio.isEmpty()) {
        mensajeError = "Ingrese su nombre de usuario.";
        return false;
    }
    if (!existeUsuario(limpio)) {
        mensajeError = "Usuario no encontrado. Registrese primero.";
        return false;
    }

    QMap<QString, QString> datos = leerDatos(rutaDatosUsuario(limpio));
    if (datos.isEmpty() || !datos.contains("contrasena")) {
        mensajeError = "No se pudo leer los datos del usuario.";
        return false;
    }

    if (datos.value("contrasena") != contrasena.trimmed()) {
        mensajeError = "Contrasena incorrecta.";
        return false;
    }
    return true;
}

void GestorUsuarios::obtenerPuntajes(const QString &nombre, int &n1, int &n2, int &n3) {
    int nSup = 0;
    obtenerPuntajes(nombre, n1, n2, n3, nSup);
}

void GestorUsuarios::obtenerPuntajes(const QString &nombre, int &n1, int &n2, int &n3, int &nSupervivencia) {
    QString limpio = sanitizarNombre(nombre);
    QMap<QString, QString> datos = leerDatos(rutaDatosUsuario(limpio));
    n1 = leerClave(datos, "mejorPuntajeNivel1");
    n2 = leerClave(datos, "mejorPuntajeNivel2");
    n3 = leerClave(datos, "mejorPuntajeNivel3");
    nSupervivencia = leerClave(datos, "mejorPuntajeSupervivencia");
    // Migracion: archivos viejos solo traian "mejorPuntaje".
    // Se toma como Nivel 1 para no perder el record anterior.
    if (n1 == 0 && n2 == 0 && n3 == 0 && datos.contains("mejorPuntaje")) {
        n1 = leerClave(datos, "mejorPuntaje");
    }
}

int GestorUsuarios::obtenerMejorPuntaje(const QString &nombre, int nivel) {
    int n1 = 0, n2 = 0, n3 = 0, nSup = 0;
    obtenerPuntajes(nombre, n1, n2, n3, nSup);
    int resultado = n1;
    if (nivel == 2) resultado = n2;
    else if (nivel == 3) resultado = n3;
    else if (nivel == 4) resultado = nSup;
    return resultado < 0 ? 0 : resultado;
}

int GestorUsuarios::puntajeGeneral(const QString &nombre) {
    int n1 = 0, n2 = 0, n3 = 0;
    obtenerPuntajes(nombre, n1, n2, n3);
    // El puntaje general nunca es negativo: los archivos viejos
    // podrian traer valores bajo cero y se normalizan a 0.
    if (n1 < 0) n1 = 0;
    if (n2 < 0) n2 = 0;
    if (n3 < 0) n3 = 0;
    return n1 + n2 + n3;
}

bool GestorUsuarios::guardarMejorPuntaje(const QString &nombre, int nivel, int puntaje) {
    QString limpio = sanitizarNombre(nombre);
    if (limpio.isEmpty() || !existeUsuario(limpio)) {
        return false;
    }
    if (nivel < 1) nivel = 1;
    if (nivel > 4) nivel = 4;
    // El puntaje minimo es 0: nunca se guarda un valor negativo.
    if (puntaje < 0) puntaje = 0;

    QMap<QString, QString> datos = leerDatos(rutaDatosUsuario(limpio));
    if (!datos.contains("nombre")) {
        datos["nombre"] = limpio;
    }
    int n1 = 0, n2 = 0, n3 = 0, nSup = 0;
    obtenerPuntajes(nombre, n1, n2, n3, nSup);
    if (n1 < 0) n1 = 0;
    if (n2 < 0) n2 = 0;
    if (n3 < 0) n3 = 0;
    if (nSup < 0) nSup = 0;
    int tSup = 0, dSup = 0, pSup = 0;
    obtenerSupervivencia(nombre, tSup, dSup, pSup);
    if (tSup < 0) tSup = 0;
    if (dSup < 0) dSup = 0;
    datos["mejorPuntajeNivel1"] = QString::number(n1);
    datos["mejorPuntajeNivel2"] = QString::number(n2);
    datos["mejorPuntajeNivel3"] = QString::number(n3);
    datos["mejorPuntajeSupervivencia"] = QString::number(nSup);
    datos["mejorSupervivenciaTiempo"] = QString::number(tSup);
    datos["mejorSupervivenciaDistancia"] = QString::number(dSup);

    QString clave = claveNivel(nivel);
    if (puntaje <= leerClave(datos, clave)) {
        // No hay record nuevo, pero igual normalizamos el archivo
        // (migracion de "mejorPuntaje" viejo a las 3 claves).
        escribirDatos(rutaDatosUsuario(limpio), datos);
        return false;
    }
    datos[clave] = QString::number(puntaje);
    return escribirDatos(rutaDatosUsuario(limpio), datos);
}

// Compatibilidad: el puntaje unico viejo equivale al Nivel 1.
int GestorUsuarios::obtenerMejorPuntaje(const QString &nombre) {
    return obtenerMejorPuntaje(nombre, 1);
}

bool GestorUsuarios::guardarMejorPuntaje(const QString &nombre, int puntaje) {
    return guardarMejorPuntaje(nombre, 1, puntaje);
}

void GestorUsuarios::obtenerSupervivencia(const QString &nombre, int &tiempoSeg,
                                         int &distanciaM, int &puntaje) {
    QString limpio = sanitizarNombre(nombre);
    QMap<QString, QString> datos = leerDatos(rutaDatosUsuario(limpio));
    tiempoSeg = leerClave(datos, "mejorSupervivenciaTiempo");
    distanciaM = leerClave(datos, "mejorSupervivenciaDistancia");
    puntaje = leerClave(datos, "mejorPuntajeSupervivencia");
    if (tiempoSeg < 0) tiempoSeg = 0;
    if (distanciaM < 0) distanciaM = 0;
    if (puntaje < 0) puntaje = 0;
}

bool GestorUsuarios::guardarMejorSupervivencia(const QString &nombre, int tiempoSeg,
                                              int distanciaM, int puntaje) {
    QString limpio = sanitizarNombre(nombre);
    if (limpio.isEmpty() || !existeUsuario(limpio)) {
        return false;
    }
    if (tiempoSeg < 0) tiempoSeg = 0;
    if (distanciaM < 0) distanciaM = 0;
    if (puntaje < 0) puntaje = 0;

    int tViejo = 0, dViejo = 0, pViejo = 0;
    obtenerSupervivencia(nombre, tViejo, dViejo, pViejo);

    // La marca nueva solo supera a la anterior si dura mas tiempo;
    // a igual tiempo vale mas distancia, y a igual distancia mas puntos.
    bool esRecord = false;
    if (tiempoSeg > tViejo) {
        esRecord = true;
    } else if (tiempoSeg == tViejo && distanciaM > dViejo) {
        esRecord = true;
    } else if (tiempoSeg == tViejo && distanciaM == dViejo && puntaje > pViejo) {
        esRecord = true;
    }

    QMap<QString, QString> datos = leerDatos(rutaDatosUsuario(limpio));
    if (!datos.contains("nombre")) {
        datos["nombre"] = limpio;
    }
    if (esRecord == false) {
        return false;
    }
    datos["mejorSupervivenciaTiempo"] = QString::number(tiempoSeg);
    datos["mejorSupervivenciaDistancia"] = QString::number(distanciaM);
    datos["mejorPuntajeSupervivencia"] = QString::number(puntaje);
    return escribirDatos(rutaDatosUsuario(limpio), datos);
}

QVector<GestorUsuarios::EntradaRanking> GestorUsuarios::rankingPorNivel(int nivel) {
    QVector<EntradaRanking> lista;
    for (const QString &usuario : listaUsuarios()) {
        EntradaRanking e;
        e.nombre = usuario;
        e.puntaje = obtenerMejorPuntaje(usuario, nivel);
        lista.append(e);
    }
    std::sort(lista.begin(), lista.end(), [](const EntradaRanking &a, const EntradaRanking &b) {
        if (a.puntaje != b.puntaje) return a.puntaje > b.puntaje;
        return a.nombre.toLower() < b.nombre.toLower();
    });
    return lista;
}

QVector<GestorUsuarios::EntradaRanking> GestorUsuarios::rankingGeneral() {
    QVector<EntradaRanking> lista;
    for (const QString &usuario : listaUsuarios()) {
        EntradaRanking e;
        e.nombre = usuario;
        e.puntaje = puntajeGeneral(usuario);
        lista.append(e);
    }
    std::sort(lista.begin(), lista.end(), [](const EntradaRanking &a, const EntradaRanking &b) {
        if (a.puntaje != b.puntaje) return a.puntaje > b.puntaje;
        return a.nombre.toLower() < b.nombre.toLower();
    });
    return lista;
}

QVector<GestorUsuarios::EntradaSupervivencia> GestorUsuarios::rankingSupervivencia() {
    QVector<EntradaSupervivencia> lista;
    for (const QString &usuario : listaUsuarios()) {
        EntradaSupervivencia e;
        e.nombre = usuario;
        obtenerSupervivencia(usuario, e.tiempoSeg, e.distanciaM, e.puntaje);
        lista.append(e);
    }
    // Primero el que mas tiempo duro, luego mayor distancia,
    // luego mas puntos, luego orden alfabetico.
    std::sort(lista.begin(), lista.end(), [](const EntradaSupervivencia &a, const EntradaSupervivencia &b) {
        if (a.tiempoSeg != b.tiempoSeg) return a.tiempoSeg > b.tiempoSeg;
        if (a.distanciaM != b.distanciaM) return a.distanciaM > b.distanciaM;
        if (a.puntaje != b.puntaje) return a.puntaje > b.puntaje;
        return a.nombre.toLower() < b.nombre.toLower();
    });
    return lista;
}
