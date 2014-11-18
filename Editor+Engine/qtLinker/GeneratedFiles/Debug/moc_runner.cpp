/****************************************************************************
** Meta object code from reading C++ file 'runner.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../runner.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runner.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Interop_t {
    QByteArrayData data[10];
    char stringdata[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Interop_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Interop_t qt_meta_stringdata_Interop = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 12),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 13),
QT_MOC_LITERAL(4, 36, 14),
QT_MOC_LITERAL(5, 51, 12),
QT_MOC_LITERAL(6, 64, 5),
QT_MOC_LITERAL(7, 70, 4),
QT_MOC_LITERAL(8, 75, 3),
QT_MOC_LITERAL(9, 79, 16)
    },
    "Interop\0handleButton\0\0handleButton2\0"
    "plotPressEvent\0QMouseEvent*\0event\0"
    "int*\0val\0plotReleaseEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Interop[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    2,   36,    2, 0x08 /* Private */,
       9,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void Interop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Interop *_t = static_cast<Interop *>(_o);
        switch (_id) {
        case 0: _t->handleButton(); break;
        case 1: _t->handleButton2(); break;
        case 2: _t->plotPressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2]))); break;
        case 3: _t->plotReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject Interop::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Interop.data,
      qt_meta_data_Interop,  qt_static_metacall, 0, 0}
};


const QMetaObject *Interop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Interop::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Interop.stringdata))
        return static_cast<void*>(const_cast< Interop*>(this));
    return QWidget::qt_metacast(_clname);
}

int Interop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
