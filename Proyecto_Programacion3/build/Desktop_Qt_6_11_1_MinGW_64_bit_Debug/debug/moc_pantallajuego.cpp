/****************************************************************************
** Meta object code from reading C++ file 'pantallajuego.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pantallajuego.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pantallajuego.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13PantallaJuegoE_t {};
} // unnamed namespace

template <> constexpr inline auto PantallaJuego::qt_create_metaobjectdata<qt_meta_tag_ZN13PantallaJuegoE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "PantallaJuego",
        "solicitaMenu",
        "",
        "solicitaMisiones",
        "solicitaSiguienteNivel",
        "partidaTerminada",
        "nivel",
        "puntos",
        "victoria",
        "actualizarJuego"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'solicitaMenu'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'solicitaMisiones'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'solicitaSiguienteNivel'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'partidaTerminada'
        QtMocHelpers::SignalData<void(int, int, bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 }, { QMetaType::Int, 7 }, { QMetaType::Bool, 8 },
        }}),
        // Slot 'actualizarJuego'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<PantallaJuego, qt_meta_tag_ZN13PantallaJuegoE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject PantallaJuego::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PantallaJuegoE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PantallaJuegoE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13PantallaJuegoE_t>.metaTypes,
    nullptr
} };

void PantallaJuego::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<PantallaJuego *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->solicitaMenu(); break;
        case 1: _t->solicitaMisiones(); break;
        case 2: _t->solicitaSiguienteNivel(); break;
        case 3: _t->partidaTerminada((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[3]))); break;
        case 4: _t->actualizarJuego(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (PantallaJuego::*)()>(_a, &PantallaJuego::solicitaMenu, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (PantallaJuego::*)()>(_a, &PantallaJuego::solicitaMisiones, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (PantallaJuego::*)()>(_a, &PantallaJuego::solicitaSiguienteNivel, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (PantallaJuego::*)(int , int , bool )>(_a, &PantallaJuego::partidaTerminada, 3))
            return;
    }
}

const QMetaObject *PantallaJuego::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PantallaJuego::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13PantallaJuegoE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PantallaJuego::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void PantallaJuego::solicitaMenu()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PantallaJuego::solicitaMisiones()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PantallaJuego::solicitaSiguienteNivel()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PantallaJuego::partidaTerminada(int _t1, int _t2, bool _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
