/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    const uint offsetsAndSize[40];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 10), // "MainWindow"
QT_MOC_LITERAL(11, 15), // "createNewFolder"
QT_MOC_LITERAL(27, 0), // ""
QT_MOC_LITERAL(28, 14), // "deleteSelected"
QT_MOC_LITERAL(43, 12), // "copySelected"
QT_MOC_LITERAL(56, 17), // "pasteToCurrentDir"
QT_MOC_LITERAL(74, 22), // "handleSelectionChanged"
QT_MOC_LITERAL(97, 15), // "updateStatusBar"
QT_MOC_LITERAL(113, 8), // "handleLs"
QT_MOC_LITERAL(122, 8), // "handleCd"
QT_MOC_LITERAL(131, 4), // "path"
QT_MOC_LITERAL(136, 11), // "handleMkdir"
QT_MOC_LITERAL(148, 8), // "handleRm"
QT_MOC_LITERAL(157, 11), // "handleChmod"
QT_MOC_LITERAL(169, 8), // "handleCp"
QT_MOC_LITERAL(178, 12), // "handleRename"
QT_MOC_LITERAL(191, 8), // "handleLn"
QT_MOC_LITERAL(200, 9), // "handleCat"
QT_MOC_LITERAL(210, 8), // "handlePs"
QT_MOC_LITERAL(219, 10) // "handleKill"

    },
    "MainWindow\0createNewFolder\0\0deleteSelected\0"
    "copySelected\0pasteToCurrentDir\0"
    "handleSelectionChanged\0updateStatusBar\0"
    "handleLs\0handleCd\0path\0handleMkdir\0"
    "handleRm\0handleChmod\0handleCp\0"
    "handleRename\0handleLn\0handleCat\0"
    "handlePs\0handleKill"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  116,    2, 0x08,    1 /* Private */,
       3,    0,  117,    2, 0x08,    2 /* Private */,
       4,    0,  118,    2, 0x08,    3 /* Private */,
       5,    0,  119,    2, 0x08,    4 /* Private */,
       6,    0,  120,    2, 0x08,    5 /* Private */,
       7,    0,  121,    2, 0x08,    6 /* Private */,
       8,    0,  122,    2, 0x08,    7 /* Private */,
       9,    1,  123,    2, 0x08,    8 /* Private */,
      11,    0,  126,    2, 0x08,   10 /* Private */,
      12,    0,  127,    2, 0x08,   11 /* Private */,
      13,    0,  128,    2, 0x08,   12 /* Private */,
      14,    0,  129,    2, 0x08,   13 /* Private */,
      15,    0,  130,    2, 0x08,   14 /* Private */,
      16,    0,  131,    2, 0x08,   15 /* Private */,
      17,    0,  132,    2, 0x08,   16 /* Private */,
      18,    0,  133,    2, 0x08,   17 /* Private */,
      19,    0,  134,    2, 0x08,   18 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->createNewFolder(); break;
        case 1: _t->deleteSelected(); break;
        case 2: _t->copySelected(); break;
        case 3: _t->pasteToCurrentDir(); break;
        case 4: _t->handleSelectionChanged(); break;
        case 5: _t->updateStatusBar(); break;
        case 6: _t->handleLs(); break;
        case 7: _t->handleCd((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->handleMkdir(); break;
        case 9: _t->handleRm(); break;
        case 10: _t->handleChmod(); break;
        case 11: _t->handleCp(); break;
        case 12: _t->handleRename(); break;
        case 13: _t->handleLn(); break;
        case 14: _t->handleCat(); break;
        case 15: _t->handlePs(); break;
        case 16: _t->handleKill(); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.offsetsAndSize,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWindow_t
, QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
