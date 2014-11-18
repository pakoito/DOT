/****************************************************************************
** Meta object code from reading C++ file 'ObjectEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ObjectEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ObjectEditor_t {
    QByteArrayData data[8];
    char stringdata[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ObjectEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ObjectEditor_t qt_meta_stringdata_ObjectEditor = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 7),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 8),
QT_MOC_LITERAL(4, 31, 9),
QT_MOC_LITERAL(5, 41, 11),
QT_MOC_LITERAL(6, 53, 5),
QT_MOC_LITERAL(7, 59, 9)
    },
    "ObjectEditor\0OnClick\0\0OnButton\0hideEvent\0"
    "QHideEvent*\0event\0CreateNew"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ObjectEditor[] = {

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
       1,    1,   34,    2, 0x08 /* Private */,
       3,    0,   37,    2, 0x08 /* Private */,
       4,    1,   38,    2, 0x08 /* Private */,
       7,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

       0        // eod
};

void ObjectEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ObjectEditor *_t = static_cast<ObjectEditor *>(_o);
        switch (_id) {
        case 0: _t->OnClick((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: _t->OnButton(); break;
        case 2: _t->hideEvent((*reinterpret_cast< QHideEvent*(*)>(_a[1]))); break;
        case 3: _t->CreateNew(); break;
        default: ;
        }
    }
}

const QMetaObject ObjectEditor::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ObjectEditor.data,
      qt_meta_data_ObjectEditor,  qt_static_metacall, 0, 0}
};


const QMetaObject *ObjectEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ObjectEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectEditor.stringdata))
        return static_cast<void*>(const_cast< ObjectEditor*>(this));
    return QWidget::qt_metacast(_clname);
}

int ObjectEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
