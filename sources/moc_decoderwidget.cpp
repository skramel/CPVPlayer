/****************************************************************************
** Meta object code from reading C++ file 'decoderwidget.h'
**
** Created: Tue Sep 18 15:34:57 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "decoderwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'decoderwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DecoderWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x08,
      26,   14,   14,   14, 0x08,
      40,   14,   14,   14, 0x08,
      58,   14,   14,   14, 0x08,
      68,   14,   14,   14, 0x0a,
      86,   14,   14,   14, 0x0a,
      95,   14,   14,   14, 0x0a,
     102,   14,   14,   14, 0x0a,
     109,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DecoderWidget[] = {
    "DecoderWidget\0\0writeGDF()\0writeGDFall()\0"
    "writeClusterJob()\0gdfDone()\0"
    "decodeNextFrame()\0rewind()\0shot()\0"
    "seek()\0reDecode()\0"
};

void DecoderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DecoderWidget *_t = static_cast<DecoderWidget *>(_o);
        switch (_id) {
        case 0: _t->writeGDF(); break;
        case 1: _t->writeGDFall(); break;
        case 2: _t->writeClusterJob(); break;
        case 3: _t->gdfDone(); break;
        case 4: _t->decodeNextFrame(); break;
        case 5: _t->rewind(); break;
        case 6: _t->shot(); break;
        case 7: _t->seek(); break;
        case 8: _t->reDecode(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData DecoderWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DecoderWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DecoderWidget,
      qt_meta_data_DecoderWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DecoderWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DecoderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DecoderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DecoderWidget))
        return static_cast<void*>(const_cast< DecoderWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DecoderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
