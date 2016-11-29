/****************************************************************************
** Meta object code from reading C++ file 'Dlg_TestModule.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Dlg_TestModule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dlg_TestModule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dlg_TestModule[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      45,   15,   15,   15, 0x0a,
      72,   15,   15,   15, 0x0a,
     105,   15,   15,   15, 0x0a,
     130,   15,   15,   15, 0x0a,
     157,   15,   15,   15, 0x0a,
     187,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Dlg_TestModule[] = {
    "Dlg_TestModule\0\0on_BtnImportConfig_clicked()\0"
    "on_BtnImportData_clicked()\0"
    "on_BtnCreateClassifier_clicked()\0"
    "on_Btn_Connect_clicked()\0"
    "on_Btn_StartTest_clicked()\0"
    "on_Btn_CreateReport_clicked()\0"
    "_qTimer_timeout()\0"
};

void Dlg_TestModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Dlg_TestModule *_t = static_cast<Dlg_TestModule *>(_o);
        switch (_id) {
        case 0: _t->on_BtnImportConfig_clicked(); break;
        case 1: _t->on_BtnImportData_clicked(); break;
        case 2: _t->on_BtnCreateClassifier_clicked(); break;
        case 3: _t->on_Btn_Connect_clicked(); break;
        case 4: _t->on_Btn_StartTest_clicked(); break;
        case 5: _t->on_Btn_CreateReport_clicked(); break;
        case 6: _t->_qTimer_timeout(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Dlg_TestModule::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Dlg_TestModule::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dlg_TestModule,
      qt_meta_data_Dlg_TestModule, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dlg_TestModule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dlg_TestModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dlg_TestModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dlg_TestModule))
        return static_cast<void*>(const_cast< Dlg_TestModule*>(this));
    if (!strcmp(_clname, "Ui_TestModule"))
        return static_cast< Ui_TestModule*>(const_cast< Dlg_TestModule*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dlg_TestModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
