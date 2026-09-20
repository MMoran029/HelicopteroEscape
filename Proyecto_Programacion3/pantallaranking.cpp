#include "pantallaranking.h"
#include "gestorusuarios.h"
#include <QPainter>
#include <QHeaderView>
#include <iostream>
using namespace std;

PantallaRanking::PantallaRanking(QWidget *parent) : QWidget(parent) {
    // Fondo ranking.png (se prueban ambas rutas del .qrc).
    if (!m_fondo.load(":/imagenes/Imagenes/ranking.png")) {
        m_fondo.load(":/imagenes/ranking.png");
    }
    if (m_fondo.isNull()) {
        cout << "[ERROR] no se pudo cargar la imagen de ranking" << endl;
    }
    configurarElementos();
    mostrarCategoria(1);
}

void PantallaRanking::configurarElementos() {
    m_botonNivel1 = new CrearBoton("NIVEL 1", this);
    m_botonNivel2 = new CrearBoton("NIVEL 2", this);
    m_botonNivel3 = new CrearBoton("NIVEL 3", this);
    m_botonGeneral = new CrearBoton("GENERAL", this);
    m_botonSupervivencia = new CrearBoton("SUPERVIV.", this);
    m_botonVolver = new CrearBoton("VOLVER", this);

    connect(m_botonNivel1, &QPushButton::clicked, this, &PantallaRanking::mostrarNivel1);
    connect(m_botonNivel2, &QPushButton::clicked, this, &PantallaRanking::mostrarNivel2);
    connect(m_botonNivel3, &QPushButton::clicked, this, &PantallaRanking::mostrarNivel3);
    connect(m_botonGeneral, &QPushButton::clicked, this, &PantallaRanking::mostrarGeneral);
    connect(m_botonSupervivencia, &QPushButton::clicked, this, &PantallaRanking::mostrarSupervivencia);
    connect(m_botonVolver, &QPushButton::clicked, this, &PantallaRanking::volverPresionado);

    m_tabla = new QTableWidget(this);
    m_tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tabla->setSelectionMode(QAbstractItemView::NoSelection);
    m_tabla->setFocusPolicy(Qt::NoFocus);
    m_tabla->verticalHeader()->setVisible(false);
    m_tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    aplicarEstiloTabla();
}

void PantallaRanking::aplicarEstiloTabla() {
    m_tabla->setStyleSheet(
        "QTableWidget { background-color: rgba(0, 0, 0, 170); color: white; "
        "font-size: 15px; border: 2px solid rgb(230,170,30); border-radius: 8px; }"
        "QHeaderView::section { background-color: rgb(230,170,30); color: black; "
        "font-size: 15px; font-weight: bold; padding: 4px; border: none; }"
        "QTableWidget::item { padding: 4px; }");
    m_tabla->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background-color: rgb(230,170,30); color: black; "
        "font-size: 15px; font-weight: bold; padding: 4px; border: none; }");
}

void PantallaRanking::mostrarNivel1() { mostrarCategoria(1); }
void PantallaRanking::mostrarNivel2() { mostrarCategoria(2); }
void PantallaRanking::mostrarNivel3() { mostrarCategoria(3); }
void PantallaRanking::mostrarGeneral() { mostrarCategoria(4); }
void PantallaRanking::mostrarSupervivencia() { mostrarCategoria(5); }

void PantallaRanking::mostrarCategoria(int categoria) {
    if (categoria < 1 || categoria > 5) {
        categoria = 1;
    }
    m_categoria = categoria;
    llenarTabla();
}

void PantallaRanking::recargar() {
    llenarTabla();
}

void PantallaRanking::llenarTabla() {
    if (m_categoria == 4) {
        // Apartado general: desglose N1, N2, N3 y total (N1+N2+N3).
        m_tabla->setColumnCount(6);
        m_tabla->setHorizontalHeaderLabels({"POS", "USUARIO", "NIVEL 1", "NIVEL 2", "NIVEL 3", "TOTAL"});
        QVector<GestorUsuarios::EntradaRanking> rank = GestorUsuarios::rankingGeneral();
        if (rank.isEmpty()) {
            m_tabla->setRowCount(1);
            m_tabla->setItem(0, 0, new QTableWidgetItem("-"));
            m_tabla->setItem(0, 1, new QTableWidgetItem("Sin usuarios registrados"));
            m_tabla->setItem(0, 2, new QTableWidgetItem("0"));
            m_tabla->setItem(0, 3, new QTableWidgetItem("0"));
            m_tabla->setItem(0, 4, new QTableWidgetItem("0"));
            m_tabla->setItem(0, 5, new QTableWidgetItem("0"));
        } else {
            m_tabla->setRowCount(rank.size());
            int pos = 1;
            for (int i = 0; i < rank.size(); ++i) {
                int n1 = 0, n2 = 0, n3 = 0;
                GestorUsuarios::obtenerPuntajes(rank[i].nombre, n1, n2, n3);
                m_tabla->setItem(i, 0, new QTableWidgetItem(QString::number(pos++)));
                m_tabla->setItem(i, 1, new QTableWidgetItem(rank[i].nombre));
                m_tabla->setItem(i, 2, new QTableWidgetItem(QString::number(n1)));
                m_tabla->setItem(i, 3, new QTableWidgetItem(QString::number(n2)));
                m_tabla->setItem(i, 4, new QTableWidgetItem(QString::number(n3)));
                m_tabla->setItem(i, 5, new QTableWidgetItem(QString::number(rank[i].puntaje)));
            }
        }
    } else if (m_categoria == 5) {
        // Supervivencia: el primero es el que mas tiempo duro, luego
        // el de mayor distancia y luego el de mas puntos.
        m_tabla->setColumnCount(5);
        m_tabla->setHorizontalHeaderLabels({"POS", "USUARIO", "TIEMPO", "DISTANCIA", "PUNTOS"});
        QVector<GestorUsuarios::EntradaSupervivencia> rank =
            GestorUsuarios::rankingSupervivencia();
        if (rank.isEmpty()) {
            m_tabla->setRowCount(1);
            m_tabla->setItem(0, 0, new QTableWidgetItem("-"));
            m_tabla->setItem(0, 1, new QTableWidgetItem("Sin usuarios registrados"));
            m_tabla->setItem(0, 2, new QTableWidgetItem("-"));
            m_tabla->setItem(0, 3, new QTableWidgetItem("-"));
            m_tabla->setItem(0, 4, new QTableWidgetItem("0"));
        } else {
            m_tabla->setRowCount(rank.size());
            for (int i = 0; i < rank.size(); ++i) {
                m_tabla->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
                m_tabla->setItem(i, 1, new QTableWidgetItem(rank[i].nombre));
                m_tabla->setItem(i, 2, new QTableWidgetItem(QString("%1s").arg(rank[i].tiempoSeg)));
                m_tabla->setItem(i, 3, new QTableWidgetItem(QString("%1m").arg(rank[i].distanciaM)));
                m_tabla->setItem(i, 4, new QTableWidgetItem(QString::number(rank[i].puntaje)));
            }
        }
    } else {
        m_tabla->setColumnCount(3);
        m_tabla->setHorizontalHeaderLabels({"POS", "USUARIO", "PUNTOS"});
        QVector<GestorUsuarios::EntradaRanking> rank =
            GestorUsuarios::rankingPorNivel(m_categoria);
        if (rank.isEmpty()) {
            m_tabla->setRowCount(1);
            m_tabla->setItem(0, 0, new QTableWidgetItem("-"));
            m_tabla->setItem(0, 1, new QTableWidgetItem("Sin usuarios registrados"));
            m_tabla->setItem(0, 2, new QTableWidgetItem("0"));
        } else {
            m_tabla->setRowCount(rank.size());
            for (int i = 0; i < rank.size(); ++i) {
                m_tabla->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
                m_tabla->setItem(i, 1, new QTableWidgetItem(rank[i].nombre));
                m_tabla->setItem(i, 2, new QTableWidgetItem(QString::number(rank[i].puntaje)));
            }
        }
    }
    // Centrar el texto de todas las celdas.
    for (int f = 0; f < m_tabla->rowCount(); ++f) {
        for (int c = 0; c < m_tabla->columnCount(); ++c) {
            if (QTableWidgetItem *item = m_tabla->item(f, c)) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

void PantallaRanking::acomodarElementos() {
    int yFranja = 25;
    int altoFranja = 40;
    int yTabla = yFranja + altoFranja + 15;

    int margenX = 60;
    int altoVolver = 40;
    int yFilaBotones = height() - altoVolver - 20;
    int altoTabla = yFilaBotones - yTabla - 15;
    if (altoTabla < 100) altoTabla = 100;

    int anchoTabla = static_cast<int>((width() - margenX * 2) * 0.9);
    int altoTablaFinal = static_cast<int>(altoTabla * 0.9);
    int xTabla = (width() - anchoTabla) / 2;
    int yTablaCentrada = yTabla + (altoTabla - altoTablaFinal) / 2;

    m_tabla->move(xTabla, yTablaCentrada);
    m_tabla->resize(anchoTabla, altoTablaFinal);

    int anchoTab = 100;
    int altoTab = 32;
    int espacio = 6;
    int totalTabs = 5 * anchoTab + 4 * espacio;
    int x0 = (width() - totalTabs) / 2;
    // El VOLVER va en la esquina inferior izquierda (x=20, ancho 120)
    // en la misma fila: la primera pestaña nunca debe montarse sobre el.
    int limiteVolver = 20 + 120 + 8;
    if (x0 < limiteVolver) {
        x0 = limiteVolver;
    }
    int yTabs = yFilaBotones + (altoVolver - altoTab) / 2;

    m_botonNivel1->resize(anchoTab, altoTab);
    m_botonNivel2->resize(anchoTab, altoTab);
    m_botonNivel3->resize(anchoTab, altoTab);
    m_botonGeneral->resize(anchoTab, altoTab);
    m_botonSupervivencia->resize(anchoTab, altoTab);
    m_botonNivel1->move(x0, yTabs);
    m_botonNivel2->move(x0 + (anchoTab + espacio), yTabs);
    m_botonNivel3->move(x0 + (anchoTab + espacio) * 2, yTabs);
    m_botonGeneral->move(x0 + (anchoTab + espacio) * 3, yTabs);
    m_botonSupervivencia->move(x0 + (anchoTab + espacio) * 4, yTabs);

    // VOLVER en la esquina inferior izquierda, en la misma fila.
    int anchoVolver = 120;
    m_botonVolver->resize(anchoVolver, altoVolver);
    m_botonVolver->move(20, yFilaBotones);
}

void PantallaRanking::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    if (!m_fondo.isNull()) {
        painter.drawPixmap(rect(), m_fondo);
    } else {
        painter.fillRect(rect(), Qt::black);
    }
}

void PantallaRanking::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);
    acomodarElementos();
}
