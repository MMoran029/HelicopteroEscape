/****************************************************************************
** Meta object code from reading C++ file 'pantalla_inicio_sesion.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../pantalla_inicio_sesion.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pantalla_inicio_sesion.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN22pantalla_inicio_sesionE_t {};
} // unnamed namespace

template <> constexpr inline auto pantalla_inicio_sesion::qt_create_metaobjectdata<qt_meta_tag_ZN22pantalla_inicio_sesionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "pantalla_inicio_sesion",
        "sesionIniciada",
        "",
        "nombreUsuario",
        "salirPresionado",
        "onBotonIniciarSesion",
        "onBotonRegistrarse",
        "onBotonAceptar",
        "onBotonVolver"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sesionIniciada'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'salirPresionado'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onBotonIniciarSesion'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBotonRegistrarse'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBotonAceptar'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBotonVolver'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<pantalla_inicio_sesion, qt_meta_tag_ZN22pantalla_inicio_sesionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject pantalla_inicio_sesion::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22pantalla_inicio_sesionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22pantalla_inicio_sesionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22pantalla_inicio_sesionE_t>.metaTypes,
    nullptr
} };

void pantalla_inicio_sesion::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<pantalla_inicio_sesion *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sesionIniciada((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->salirPresionado(); break;
        case 2: _t->onBotonIniciarSesion(); break;
        case 3: _t->onBotonRegistrarse(); break;
        case 4: _t->onBotonAceptar(); break;
        case 5: _t->onBotonVolver(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (pantalla_inicio_sesion::*)(const QString & )>(_a, &pantalla_inicio_sesion::sesionIniciada, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (pantalla_inicio_sesion::*)()>(_a, &pantalla_inicio_sesion::salirPresionado, 1))
            return;
    }
}

const QMetaObject *pantalla_inicio_sesion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pantalla_inicio_sesion::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22pantalla_inicio_sesionE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int pantalla_inicio_sesion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void pantalla_inicio_sesion::sesionIniciada(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void pantalla_inicio_sesion::salirPresionado()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
