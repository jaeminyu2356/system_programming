/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata0[261];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 22), // "handleSelectionChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 8), // "handleLs"
QT_MOC_LITERAL(4, 44, 11), // "handleMkdir"
QT_MOC_LITERAL(5, 56, 8), // "handleRm"
QT_MOC_LITERAL(6, 65, 11), // "handleChmod"
QT_MOC_LITERAL(7, 77, 8), // "handleCp"
QT_MOC_LITERAL(8, 86, 12), // "handleRename"
QT_MOC_LITERAL(9, 99, 8), // "handleLn"
QT_MOC_LITERAL(10, 108, 9), // "handleCat"
QT_MOC_LITERAL(11, 118, 8), // "handlePs"
QT_MOC_LITERAL(12, 127, 10), // "handleKill"
QT_MOC_LITERAL(13, 138, 3), // "pid"
QT_MOC_LITERAL(14, 142, 15), // "createNewFolder"
QT_MOC_LITERAL(15, 158, 14), // "deleteSelected"
QT_MOC_LITERAL(16, 173, 12), // "copySelected"
QT_MOC_LITERAL(17, 186, 17), // "pasteToCurrentDir"
QT_MOC_LITERAL(18, 204, 15), // "showContextMenu"
QT_MOC_LITERAL(19, 220, 3), // "pos"
QT_MOC_LITERAL(20, 224, 15), // "showFileDetails"
QT_MOC_LITERAL(21, 240, 8), // "fileName"
QT_MOC_LITERAL(22, 249, 11) // "handleRmdir"

    },
    "MainWindow\0handleSelectionChanged\0\0"
    "handleLs\0handleMkdir\0handleRm\0handleChmod\0"
    "handleCp\0handleRename\0handleLn\0handleCat\0"
    "handlePs\0handleKill\0pid\0createNewFolder\0"
    "deleteSelected\0copySelected\0"
    "pasteToCurrentDir\0showContextMenu\0pos\0"
    "showFileDetails\0fileName\0handleRmdir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x08 /* Private */,
       3,    0,  105,    2, 0x08 /* Private */,
       4,    0,  106,    2, 0x08 /* Private */,
       5,    0,  107,    2, 0x08 /* Private */,
       6,    0,  108,    2, 0x08 /* Private */,
       7,    0,  109,    2, 0x08 /* Private */,
       8,    0,  110,    2, 0x08 /* Private */,
       9,    0,  111,    2, 0x08 /* Private */,
      10,    0,  112,    2, 0x08 /* Private */,
      11,    0,  113,    2, 0x08 /* Private */,
      12,    1,  114,    2, 0x08 /* Private */,
      14,    0,  117,    2, 0x08 /* Private */,
      15,    0,  118,    2, 0x08 /* Private */,
      16,    0,  119,    2, 0x08 /* Private */,
      17,    0,  120,    2, 0x08 /* Private */,
      18,    1,  121,    2, 0x08 /* Private */,
      20,    1,  124,    2, 0x08 /* Private */,
      22,    0,  127,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->handleSelectionChanged(); break;
        case 1: _t->handleLs(); break;
        case 2: _t->handleMkdir(); break;
        case 3: _t->handleRm(); break;
        case 4: _t->handleChmod(); break;
        case 5: _t->handleCp(); break;
        case 6: _t->handleRename(); break;
        case 7: _t->handleLn(); break;
        case 8: _t->handleCat(); break;
        case 9: _t->handlePs(); break;
        case 10: _t->handleKill((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->createNewFolder(); break;
        case 12: _t->deleteSelected(); break;
        case 13: _t->copySelected(); break;
        case 14: _t->pasteToCurrentDir(); break;
        case 15: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 16: _t->showFileDetails((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->handleRmdir(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
