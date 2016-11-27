/********************************************************************************
** Form generated from reading UI file 'TestModule.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTMODULE_H
#define UI_TESTMODULE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_TestModule
{
public:
    QPushButton *BtnImportConfig;
    QPushButton *BtnImportData;
    QLineEdit *LEConfigPath;
    QLineEdit *LEDataPath;
    QPushButton *BtnCreateClassifier;
    QLineEdit *LEClassifierStatus;

    void setupUi(QDialog *TestModule)
    {
        if (TestModule->objectName().isEmpty())
            TestModule->setObjectName(QString::fromUtf8("TestModule"));
        TestModule->resize(406, 241);
        BtnImportConfig = new QPushButton(TestModule);
        BtnImportConfig->setObjectName(QString::fromUtf8("BtnImportConfig"));
        BtnImportConfig->setGeometry(QRect(10, 20, 121, 31));
        BtnImportData = new QPushButton(TestModule);
        BtnImportData->setObjectName(QString::fromUtf8("BtnImportData"));
        BtnImportData->setGeometry(QRect(10, 70, 121, 31));
        LEConfigPath = new QLineEdit(TestModule);
        LEConfigPath->setObjectName(QString::fromUtf8("LEConfigPath"));
        LEConfigPath->setGeometry(QRect(150, 30, 251, 21));
        LEConfigPath->setReadOnly(true);
        LEDataPath = new QLineEdit(TestModule);
        LEDataPath->setObjectName(QString::fromUtf8("LEDataPath"));
        LEDataPath->setGeometry(QRect(150, 80, 251, 21));
        LEDataPath->setReadOnly(true);
        BtnCreateClassifier = new QPushButton(TestModule);
        BtnCreateClassifier->setObjectName(QString::fromUtf8("BtnCreateClassifier"));
        BtnCreateClassifier->setGeometry(QRect(10, 120, 121, 31));
        LEClassifierStatus = new QLineEdit(TestModule);
        LEClassifierStatus->setObjectName(QString::fromUtf8("LEClassifierStatus"));
        LEClassifierStatus->setGeometry(QRect(150, 130, 251, 21));

        retranslateUi(TestModule);

        QMetaObject::connectSlotsByName(TestModule);
    } // setupUi

    void retranslateUi(QDialog *TestModule)
    {
        TestModule->setWindowTitle(QApplication::translate("TestModule", "TestModule", 0, QApplication::UnicodeUTF8));
        BtnImportConfig->setText(QApplication::translate("TestModule", "\345\257\274\345\205\245\350\256\255\347\273\203\351\205\215\347\275\256\346\226\207\344\273\266...", 0, QApplication::UnicodeUTF8));
        BtnImportData->setText(QApplication::translate("TestModule", "\345\257\274\345\205\245\350\256\255\347\273\203\346\225\260\346\215\256\346\226\207\344\273\266...", 0, QApplication::UnicodeUTF8));
        BtnCreateClassifier->setText(QApplication::translate("TestModule", "\347\224\237\346\210\220\345\210\206\347\261\273\345\231\250", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TestModule: public Ui_TestModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTMODULE_H
