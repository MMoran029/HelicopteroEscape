#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>

// Manejo de archivos y usuarios del juego.
//
// Estructura en disco (junto al .exe):
//   USUARIOS/
//     <nombre_usuario>/
//       datos.txt -> nombre, contrasena, mejorPuntajeNivel1/2/3
//
// El datos.txt es un archivo de texto simple "clave=valor" para que
// sea facil de leer y sirva para el apartado de rankings.
// El puntaje GENERAL de un usuario es la suma de sus 3 niveles.
class GestorUsuarios {
public:
    struct EntradaRanking {
        QString nombre;
        int puntaje = 0;
    };

    // Carpeta base donde se guardan los usuarios ("USUARIOS").
    // Se crea automaticamente si no existe.
    static QString rutaBase();

    // Lista los nombres de usuario registrados (una carpeta por usuario).
    static QStringList listaUsuarios();

    static bool hayUsuariosRegistrados();
    static bool existeUsuario(const QString &nombre);

    // Limpia el nombre para usarlo como nombre de carpeta en Windows:
    // quita espacios de los extremos y caracteres prohibidos \ / : * ? " < > |
    static QString sanitizarNombre(const QString &nombre);

    // Registra un usuario nuevo. Crea USUARIOS/<nombre>/datos.txt con
    // los 3 puntajes en 0. Devuelve false con mensaje de error si falla.
    static bool registrarUsuario(const QString &nombre, const QString &contrasena,
                                 QString &mensajeError);

    // Verifica nombre + contrasena contra el datos.txt guardado.
    static bool validarCredenciales(const QString &nombre, const QString &contrasena,
                                    QString &mensajeError);

    // ---- Puntajes por nivel (1, 2 o 3) ----
    static int obtenerMejorPuntaje(const QString &nombre, int nivel);
    // Solo guarda si el nuevo puntaje supera al anterior. Devuelve true
    // si se guardo un record nuevo.
    static bool guardarMejorPuntaje(const QString &nombre, int nivel, int puntaje);
    static void obtenerPuntajes(const QString &nombre, int &n1, int &n2, int &n3);
    static int puntajeGeneral(const QString &nombre);

    // Compatibilidad con la version anterior (un solo mejorPuntaje).
    static int obtenerMejorPuntaje(const QString &nombre);
    static bool guardarMejorPuntaje(const QString &nombre, int puntaje);

    // ---- Rankings ordenados de mayor a menor ----
    static QVector<EntradaRanking> rankingPorNivel(int nivel);
    static QVector<EntradaRanking> rankingGeneral();

private:
    static QString rutaCarpetaUsuario(const QString &nombreSanitizado);
    static QString rutaDatosUsuario(const QString &nombreSanitizado);
    static QString claveNivel(int nivel);
    static int leerClave(const QMap<QString, QString> &datos, const QString &clave);
};
#endif // GESTORUSUARIOS_H
