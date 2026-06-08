/****************************************************************************
** Meta object code from reading C++ file 'MyForm.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "MyForm.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyForm_t {
    QByteArrayData data[24];
    char stringdata0[257];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyForm_t qt_meta_stringdata_MyForm = {
    {
QT_MOC_LITERAL(0, 0, 6), // "MyForm"
QT_MOC_LITERAL(1, 7, 11), // "onStartGame"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 17), // "onMonedaRecollida"
QT_MOC_LITERAL(4, 38, 9), // "collected"
QT_MOC_LITERAL(5, 48, 5), // "total"
QT_MOC_LITERAL(6, 54, 10), // "onVictoria"
QT_MOC_LITERAL(7, 65, 10), // "onGameOver"
QT_MOC_LITERAL(8, 76, 12), // "onPsiChanged"
QT_MOC_LITERAL(9, 89, 5), // "value"
QT_MOC_LITERAL(10, 95, 14), // "onThetaChanged"
QT_MOC_LITERAL(11, 110, 13), // "onZoomChanged"
QT_MOC_LITERAL(12, 124, 14), // "onCameraToggle"
QT_MOC_LITERAL(13, 139, 19), // "onRotateCoinsToggle"
QT_MOC_LITERAL(14, 159, 15), // "onCameraChanged"
QT_MOC_LITERAL(15, 175, 3), // "psi"
QT_MOC_LITERAL(16, 179, 5), // "theta"
QT_MOC_LITERAL(17, 185, 4), // "zoom"
QT_MOC_LITERAL(18, 190, 12), // "onLightColor"
QT_MOC_LITERAL(19, 203, 11), // "resizeEvent"
QT_MOC_LITERAL(20, 215, 13), // "QResizeEvent*"
QT_MOC_LITERAL(21, 229, 5), // "event"
QT_MOC_LITERAL(22, 235, 9), // "showEvent"
QT_MOC_LITERAL(23, 245, 11) // "QShowEvent*"

    },
    "MyForm\0onStartGame\0\0onMonedaRecollida\0"
    "collected\0total\0onVictoria\0onGameOver\0"
    "onPsiChanged\0value\0onThetaChanged\0"
    "onZoomChanged\0onCameraToggle\0"
    "onRotateCoinsToggle\0onCameraChanged\0"
    "psi\0theta\0zoom\0onLightColor\0resizeEvent\0"
    "QResizeEvent*\0event\0showEvent\0QShowEvent*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyForm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    2,   80,    2, 0x08 /* Private */,
       6,    0,   85,    2, 0x08 /* Private */,
       7,    0,   86,    2, 0x08 /* Private */,
       8,    1,   87,    2, 0x08 /* Private */,
      10,    1,   90,    2, 0x08 /* Private */,
      11,    1,   93,    2, 0x08 /* Private */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    3,   98,    2, 0x08 /* Private */,
      18,    0,  105,    2, 0x08 /* Private */,
      19,    1,  106,    2, 0x08 /* Private */,
      22,    1,  109,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,   15,   16,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   21,

       0        // eod
};

void MyForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MyForm *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onStartGame(); break;
        case 1: _t->onMonedaRecollida((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->onVictoria(); break;
        case 3: _t->onGameOver(); break;
        case 4: _t->onPsiChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onThetaChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onZoomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onCameraToggle(); break;
        case 8: _t->onRotateCoinsToggle(); break;
        case 9: _t->onCameraChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->onLightColor(); break;
        case 11: _t->resizeEvent((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 12: _t->showEvent((*reinterpret_cast< QShowEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MyForm::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MyForm.data,
    qt_meta_data_MyForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyForm.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MyForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
