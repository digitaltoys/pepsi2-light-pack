/****************************************************************************
** Meta object code from reading C++ file 'grabdesktopwindowleds.h'
**
** Created: Wed Oct 27 21:03:45 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../inc/grabdesktopwindowleds.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'grabdesktopwindowleds.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GrabDesktopWindowLeds[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   23,   22,   22, 0x05,
      73,   70,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
     117,  111,   22,   22, 0x0a,
     140,  138,   22,   22, 0x0a,
     165,  163,   22,   22, 0x0a,
     189,   70,   22,   22, 0x0a,
     233,  221,   22,   22, 0x0a,
     263,  261,   22,   22, 0x0a,
     299,  297,   22,   22, 0x0a,
     335,  333,   22,   22, 0x0a,
     369,  111,   22,   22, 0x0a,
     401,   22,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GrabDesktopWindowLeds[] = {
    "GrabDesktopWindowLeds\0\0colorsNew\0"
    "updateLedsColors(int[LEDS_COUNT][3])\0"
    "ms\0ambilightTimeOfUpdatingColors(double)\0"
    "state\0setAmbilightOn(bool)\0w\0"
    "setAmbilightWidth(int)\0h\0"
    "setAmbilightHeight(int)\0"
    "setAmbilightRefreshDelayMs(int)\0"
    "color_depth\0setAmbilightColorDepth(int)\0"
    "r\0setAmbilightWhiteBalanceR(double)\0"
    "g\0setAmbilightWhiteBalanceG(double)\0"
    "b\0setAmbilightWhiteBalanceB(double)\0"
    "setVisibleGrabPixelsRects(bool)\0"
    "updateLedsColorsIfChanged()\0"
};

const QMetaObject GrabDesktopWindowLeds::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GrabDesktopWindowLeds,
      qt_meta_data_GrabDesktopWindowLeds, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GrabDesktopWindowLeds::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GrabDesktopWindowLeds::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GrabDesktopWindowLeds::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GrabDesktopWindowLeds))
        return static_cast<void*>(const_cast< GrabDesktopWindowLeds*>(this));
    return QWidget::qt_metacast(_clname);
}

int GrabDesktopWindowLeds::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateLedsColors((*reinterpret_cast< int(*)[LEDS_COUNT][3]>(_a[1]))); break;
        case 1: ambilightTimeOfUpdatingColors((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: setAmbilightOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: setAmbilightWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setAmbilightHeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setAmbilightRefreshDelayMs((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setAmbilightColorDepth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: setAmbilightWhiteBalanceR((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: setAmbilightWhiteBalanceG((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: setAmbilightWhiteBalanceB((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: setVisibleGrabPixelsRects((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: updateLedsColorsIfChanged(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GrabDesktopWindowLeds::updateLedsColors(int _t1[LEDS_COUNT][3])
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GrabDesktopWindowLeds::ambilightTimeOfUpdatingColors(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
