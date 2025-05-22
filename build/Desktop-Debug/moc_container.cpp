/****************************************************************************
** Meta object code from reading C++ file 'container.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.16)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../container.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'container.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.16. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Container_t {
    QByteArrayData data[3];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Container_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Container_t qt_meta_stringdata_Container = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Container"
QT_MOC_LITERAL(1, 10, 5), // "click"
QT_MOC_LITERAL(2, 16, 0) // ""

    },
    "Container\0click\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Container[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void Container::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Container *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->click(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject Container::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Container.data,
    qt_meta_data_Container,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Container::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Container::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Container.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Container::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_StatusContainer_t {
    QByteArrayData data[7];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_StatusContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_StatusContainer_t qt_meta_stringdata_StatusContainer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "StatusContainer"
QT_MOC_LITERAL(1, 16, 5), // "Ended"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 5), // "count"
QT_MOC_LITERAL(4, 29, 6), // "reveal"
QT_MOC_LITERAL(5, 36, 5), // "close"
QT_MOC_LITERAL(6, 42, 7) // "EmitEnd"

    },
    "StatusContainer\0Ended\0\0count\0reveal\0"
    "close\0EmitEnd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StatusContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    0,   42,    2, 0x0a /* Public */,
       6,    0,   43,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void StatusContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<StatusContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Ended(); break;
        case 1: _t->count(); break;
        case 2: _t->reveal(); break;
        case 3: _t->close(); break;
        case 4: _t->EmitEnd(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (StatusContainer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&StatusContainer::Ended)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject StatusContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<Container::staticMetaObject>(),
    qt_meta_stringdata_StatusContainer.data,
    qt_meta_data_StatusContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *StatusContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StatusContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_StatusContainer.stringdata0))
        return static_cast<void*>(this);
    return Container::qt_metacast(_clname);
}

int StatusContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Container::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void StatusContainer::Ended()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_PreviewContainer_t {
    QByteArrayData data[15];
    char stringdata0[173];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PreviewContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PreviewContainer_t qt_meta_stringdata_PreviewContainer = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PreviewContainer"
QT_MOC_LITERAL(1, 17, 16), // "tryConnectSignal"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 4), // "info"
QT_MOC_LITERAL(4, 40, 4), // "wifi"
QT_MOC_LITERAL(5, 45, 17), // "PreviewContainer*"
QT_MOC_LITERAL(6, 63, 6), // "sender"
QT_MOC_LITERAL(7, 70, 16), // "disconnectSignal"
QT_MOC_LITERAL(8, 87, 22), // "deleteConnectionSignal"
QT_MOC_LITERAL(9, 110, 5), // "click"
QT_MOC_LITERAL(10, 116, 12), // "remakeStatus"
QT_MOC_LITERAL(11, 129, 10), // "tryConnect"
QT_MOC_LITERAL(12, 140, 4), // "pswd"
QT_MOC_LITERAL(13, 145, 10), // "disconnect"
QT_MOC_LITERAL(14, 156, 16) // "deleteConnection"

    },
    "PreviewContainer\0tryConnectSignal\0\0"
    "info\0wifi\0PreviewContainer*\0sender\0"
    "disconnectSignal\0deleteConnectionSignal\0"
    "click\0remakeStatus\0tryConnect\0pswd\0"
    "disconnect\0deleteConnection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PreviewContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       7,    2,   59,    2, 0x06 /* Public */,
       8,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   69,    2, 0x0a /* Public */,
      10,    0,   70,    2, 0x0a /* Public */,
      11,    1,   71,    2, 0x0a /* Public */,
      13,    0,   74,    2, 0x0a /* Public */,
      14,    0,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PreviewContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PreviewContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tryConnectSignal((*reinterpret_cast< info(*)>(_a[1])),(*reinterpret_cast< PreviewContainer*(*)>(_a[2]))); break;
        case 1: _t->disconnectSignal((*reinterpret_cast< info(*)>(_a[1])),(*reinterpret_cast< PreviewContainer*(*)>(_a[2]))); break;
        case 2: _t->deleteConnectionSignal((*reinterpret_cast< info(*)>(_a[1])),(*reinterpret_cast< PreviewContainer*(*)>(_a[2]))); break;
        case 3: _t->click(); break;
        case 4: _t->remakeStatus(); break;
        case 5: _t->tryConnect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->disconnect(); break;
        case 7: _t->deleteConnection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PreviewContainer* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PreviewContainer* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< PreviewContainer* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PreviewContainer::*)(info , PreviewContainer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PreviewContainer::tryConnectSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PreviewContainer::*)(info , PreviewContainer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PreviewContainer::disconnectSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PreviewContainer::*)(info , PreviewContainer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PreviewContainer::deleteConnectionSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PreviewContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<Container::staticMetaObject>(),
    qt_meta_stringdata_PreviewContainer.data,
    qt_meta_data_PreviewContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PreviewContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PreviewContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PreviewContainer.stringdata0))
        return static_cast<void*>(this);
    return Container::qt_metacast(_clname);
}

int PreviewContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Container::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void PreviewContainer::tryConnectSignal(info _t1, PreviewContainer * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PreviewContainer::disconnectSignal(info _t1, PreviewContainer * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PreviewContainer::deleteConnectionSignal(info _t1, PreviewContainer * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_ConnectedContainer_t {
    QByteArrayData data[5];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConnectedContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConnectedContainer_t qt_meta_stringdata_ConnectedContainer = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ConnectedContainer"
QT_MOC_LITERAL(1, 19, 16), // "disconnectSignal"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "click"
QT_MOC_LITERAL(4, 43, 15) // "disconnectClick"

    },
    "ConnectedContainer\0disconnectSignal\0"
    "\0click\0disconnectClick"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectedContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConnectedContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConnectedContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->disconnectSignal(); break;
        case 1: _t->click(); break;
        case 2: _t->disconnectClick(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConnectedContainer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectedContainer::disconnectSignal)) {
                *result = 0;
                return;
            }
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject ConnectedContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<Container::staticMetaObject>(),
    qt_meta_stringdata_ConnectedContainer.data,
    qt_meta_data_ConnectedContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConnectedContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectedContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectedContainer.stringdata0))
        return static_cast<void*>(this);
    return Container::qt_metacast(_clname);
}

int ConnectedContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Container::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ConnectedContainer::disconnectSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_PasswordContainer_t {
    QByteArrayData data[7];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PasswordContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PasswordContainer_t qt_meta_stringdata_PasswordContainer = {
    {
QT_MOC_LITERAL(0, 0, 17), // "PasswordContainer"
QT_MOC_LITERAL(1, 18, 16), // "tryConnectSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "password"
QT_MOC_LITERAL(4, 45, 5), // "click"
QT_MOC_LITERAL(5, 51, 6), // "toggle"
QT_MOC_LITERAL(6, 58, 13) // "connectToWifi"

    },
    "PasswordContainer\0tryConnectSignal\0\0"
    "password\0click\0toggle\0connectToWifi"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PasswordContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       6,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PasswordContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PasswordContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tryConnectSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->click(); break;
        case 2: _t->toggle(); break;
        case 3: _t->connectToWifi(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PasswordContainer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PasswordContainer::tryConnectSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PasswordContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<Container::staticMetaObject>(),
    qt_meta_stringdata_PasswordContainer.data,
    qt_meta_data_PasswordContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PasswordContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PasswordContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PasswordContainer.stringdata0))
        return static_cast<void*>(this);
    return Container::qt_metacast(_clname);
}

int PasswordContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Container::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void PasswordContainer::tryConnectSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_SavedDisconnectedContainer_t {
    QByteArrayData data[8];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SavedDisconnectedContainer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SavedDisconnectedContainer_t qt_meta_stringdata_SavedDisconnectedContainer = {
    {
QT_MOC_LITERAL(0, 0, 26), // "SavedDisconnectedContainer"
QT_MOC_LITERAL(1, 27, 16), // "tryConnectSignal"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 4), // "pswd"
QT_MOC_LITERAL(4, 50, 9), // "tryDelete"
QT_MOC_LITERAL(5, 60, 13), // "connectToWifi"
QT_MOC_LITERAL(6, 74, 5), // "click"
QT_MOC_LITERAL(7, 80, 10) // "deleteWifi"

    },
    "SavedDisconnectedContainer\0tryConnectSignal\0"
    "\0pswd\0tryDelete\0connectToWifi\0click\0"
    "deleteWifi"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SavedDisconnectedContainer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   43,    2, 0x0a /* Public */,
       6,    0,   44,    2, 0x0a /* Public */,
       7,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SavedDisconnectedContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SavedDisconnectedContainer *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->tryConnectSignal((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->tryDelete(); break;
        case 2: _t->connectToWifi(); break;
        case 3: _t->click(); break;
        case 4: _t->deleteWifi(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SavedDisconnectedContainer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SavedDisconnectedContainer::tryConnectSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SavedDisconnectedContainer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SavedDisconnectedContainer::tryDelete)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SavedDisconnectedContainer::staticMetaObject = { {
    QMetaObject::SuperData::link<Container::staticMetaObject>(),
    qt_meta_stringdata_SavedDisconnectedContainer.data,
    qt_meta_data_SavedDisconnectedContainer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SavedDisconnectedContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SavedDisconnectedContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SavedDisconnectedContainer.stringdata0))
        return static_cast<void*>(this);
    return Container::qt_metacast(_clname);
}

int SavedDisconnectedContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Container::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SavedDisconnectedContainer::tryConnectSignal(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SavedDisconnectedContainer::tryDelete()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
