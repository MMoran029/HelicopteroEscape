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
//       datos.txt -> nombre, contrasena, mejorPuntajeNivel1/2/3,
//                    mejorPuntajeSupervivencia, mejorSupervivenciaTiempo,
//                    mejorSupervivenciaDistancia
//
// El datos.txt es un archivo de texto simple "clave=valor" para que
// sea facil de leer y sirva para el apartado de rankings.
// El puntaje GENERAL de un usuario es la suma de sus 3 niveles.
// Todos los puntajes tienen minimo 0 (nunca negativos).
class GestorUsuarios {
public:
    struct EntradaRanking {
        QString nombre;
        int puntaje = 0;
    };

    // Una fila del ranking de supervivencia: se ordena por mas
    // tiempo, luego por mayor distancia y luego por mas puntos.
    struct EntradaSupervivencia {
        QString nombre;
        int tiempoSeg = 0;
        int distanciaM = 0;
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

    // ---- Puntajes por nivel (1, 2, 3 y 4=SUPERVIVENCIA) ----
    static int obtenerMejorPuntaje(const QString &nombre, int nivel);
    // Solo guarda si el nuevo puntaje supera al anterior. Devuelve true
    // si se guardo un record nuevo.
    static bool guardarMejorPuntaje(const QString &nombre, int nivel, int puntaje);
    static void obtenerPuntajes(const QString &nombre, int &n1, int &n2, int &n3);
    static void obtenerPuntajes(const QString &nombre, int &n1, int &n2, int &n3, int &nSupervivencia);
    static int puntajeGeneral(const QString &nombre);

    // Compatibilidad con la version anterior (un solo mejorPuntaje).
    static int obtenerMejorPuntaje(const QString &nombre);
    static bool guardarMejorPuntaje(const QString &nombre, int puntaje);

    // ---- Supervivencia (nivel extra infinito) ----
    // Lee el mejor tiempo (segundos), distancia (metros) y puntaje.
    static void obtenerSupervivencia(const QString &nombre, int &tiempoSeg,
                                     int &distanciaM, int &puntaje);
    // Solo guarda si la marca nueva supera a la anterior: mas tiempo;
    // a igual tiempo, mas distancia; a igual distancia, mas puntos.
    // Devuelve true si se guardo un record nuevo.
    static bool guardarMejorSupervivencia(const QString &nombre, int tiempoSeg,
                                          int distanciaM, int puntaje);

    // ---- Rankings ordenados de mayor a menor ----
    static QVector<EntradaRanking> rankingPorNivel(int nivel);
    static QVector<EntradaRanking> rankingGeneral();
    // Ordenado por mas tiempo, luego mayor distancia, luego mas puntos.
    static QVector<EntradaSupervivencia> rankingSupervivencia();

private:
    static QString rutaCarpetaUsuario(const QString &nombreSanitizado);
    static QString rutaDatosUsuario(const QString &nombreSanitizado);
    static QString claveNivel(int nivel);
    static int leerClave(const QMap<QString, QString> &datos, const QString &clave);
};
#endif // GESTORUSUARIOS_H
