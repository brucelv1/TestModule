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
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

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
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *cbBox_Armband;
    QPushButton *Btn_Connect;
    QLabel *label_2;
    QLineEdit *LE_Armband_Status;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpinBox *spinB_ActionTimes;
    QLabel *label_4;
    QSpinBox *spinB_ActionDuration;
    QPushButton *Btn_StartTest;
    QProgressBar *progressBar;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QTableView *tbView_Report;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *Btn_CreateReport;
    QPushButton *BtnExportReport;
    QPushButton *BtnSaveClassifier;
    QLineEdit *LEClassifierSaveStatus;

    void setupUi(QDialog *TestModule)
    {
        if (TestModule->objectName().isEmpty())
            TestModule->setObjectName(QString::fromUtf8("TestModule"));
        TestModule->resize(535, 720);
        TestModule->setMinimumSize(QSize(535, 720));
        TestModule->setMaximumSize(QSize(535, 720));
        BtnImportConfig = new QPushButton(TestModule);
        BtnImportConfig->setObjectName(QString::fromUtf8("BtnImportConfig"));
        BtnImportConfig->setGeometry(QRect(10, 20, 121, 31));
        BtnImportData = new QPushButton(TestModule);
        BtnImportData->setObjectName(QString::fromUtf8("BtnImportData"));
        BtnImportData->setGeometry(QRect(10, 70, 121, 31));
        LEConfigPath = new QLineEdit(TestModule);
        LEConfigPath->setObjectName(QString::fromUtf8("LEConfigPath"));
        LEConfigPath->setGeometry(QRect(150, 30, 371, 21));
        LEConfigPath->setReadOnly(true);
        LEDataPath = new QLineEdit(TestModule);
        LEDataPath->setObjectName(QString::fromUtf8("LEDataPath"));
        LEDataPath->setGeometry(QRect(150, 80, 371, 21));
        LEDataPath->setReadOnly(true);
        BtnCreateClassifier = new QPushButton(TestModule);
        BtnCreateClassifier->setObjectName(QString::fromUtf8("BtnCreateClassifier"));
        BtnCreateClassifier->setGeometry(QRect(10, 120, 121, 31));
        LEClassifierStatus = new QLineEdit(TestModule);
        LEClassifierStatus->setObjectName(QString::fromUtf8("LEClassifierStatus"));
        LEClassifierStatus->setGeometry(QRect(150, 130, 371, 21));
        groupBox = new QGroupBox(TestModule);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 220, 511, 81));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        cbBox_Armband = new QComboBox(groupBox);
        cbBox_Armband->setObjectName(QString::fromUtf8("cbBox_Armband"));

        horizontalLayout->addWidget(cbBox_Armband);

        Btn_Connect = new QPushButton(groupBox);
        Btn_Connect->setObjectName(QString::fromUtf8("Btn_Connect"));

        horizontalLayout->addWidget(Btn_Connect);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout->addWidget(label_2);

        LE_Armband_Status = new QLineEdit(groupBox);
        LE_Armband_Status->setObjectName(QString::fromUtf8("LE_Armband_Status"));

        horizontalLayout->addWidget(LE_Armband_Status);


        horizontalLayout_2->addLayout(horizontalLayout);

        groupBox_2 = new QGroupBox(TestModule);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 310, 511, 71));
        horizontalLayout_4 = new QHBoxLayout(groupBox_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        spinB_ActionTimes = new QSpinBox(groupBox_2);
        spinB_ActionTimes->setObjectName(QString::fromUtf8("spinB_ActionTimes"));
        spinB_ActionTimes->setMinimum(1);
        spinB_ActionTimes->setMaximum(99);

        horizontalLayout_3->addWidget(spinB_ActionTimes);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        spinB_ActionDuration = new QSpinBox(groupBox_2);
        spinB_ActionDuration->setObjectName(QString::fromUtf8("spinB_ActionDuration"));
        spinB_ActionDuration->setMinimum(5);
        spinB_ActionDuration->setMaximum(99);

        horizontalLayout_3->addWidget(spinB_ActionDuration);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        Btn_StartTest = new QPushButton(TestModule);
        Btn_StartTest->setObjectName(QString::fromUtf8("Btn_StartTest"));
        Btn_StartTest->setGeometry(QRect(10, 400, 81, 31));
        progressBar = new QProgressBar(TestModule);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(110, 400, 411, 31));
        progressBar->setValue(0);
        groupBox_3 = new QGroupBox(TestModule);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 440, 511, 261));
        horizontalLayout_5 = new QHBoxLayout(groupBox_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tbView_Report = new QTableView(groupBox_3);
        tbView_Report->setObjectName(QString::fromUtf8("tbView_Report"));

        verticalLayout->addWidget(tbView_Report);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        Btn_CreateReport = new QPushButton(groupBox_3);
        Btn_CreateReport->setObjectName(QString::fromUtf8("Btn_CreateReport"));

        horizontalLayout_6->addWidget(Btn_CreateReport);

        BtnExportReport = new QPushButton(groupBox_3);
        BtnExportReport->setObjectName(QString::fromUtf8("BtnExportReport"));

        horizontalLayout_6->addWidget(BtnExportReport);


        verticalLayout->addLayout(horizontalLayout_6);


        horizontalLayout_5->addLayout(verticalLayout);

        BtnSaveClassifier = new QPushButton(TestModule);
        BtnSaveClassifier->setObjectName(QString::fromUtf8("BtnSaveClassifier"));
        BtnSaveClassifier->setGeometry(QRect(10, 170, 121, 31));
        LEClassifierSaveStatus = new QLineEdit(TestModule);
        LEClassifierSaveStatus->setObjectName(QString::fromUtf8("LEClassifierSaveStatus"));
        LEClassifierSaveStatus->setGeometry(QRect(150, 180, 371, 21));
        LEClassifierSaveStatus->setReadOnly(true);

        retranslateUi(TestModule);

        QMetaObject::connectSlotsByName(TestModule);
    } // setupUi

    void retranslateUi(QDialog *TestModule)
    {
        TestModule->setWindowTitle(QApplication::translate("TestModule", "TestModule", 0, QApplication::UnicodeUTF8));
        BtnImportConfig->setText(QApplication::translate("TestModule", "\345\257\274\345\205\245\350\256\255\347\273\203\351\205\215\347\275\256\346\226\207\344\273\266...", 0, QApplication::UnicodeUTF8));
        BtnImportData->setText(QApplication::translate("TestModule", "\345\257\274\345\205\245\350\256\255\347\273\203\346\225\260\346\215\256\346\226\207\344\273\266...", 0, QApplication::UnicodeUTF8));
        BtnCreateClassifier->setText(QApplication::translate("TestModule", "\347\224\237\346\210\220\345\210\206\347\261\273\345\231\250", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TestModule", "\350\202\214\347\224\265\350\207\202\345\270\246\350\277\236\346\216\245\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TestModule", "\344\270\262\345\217\243\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        cbBox_Armband->clear();
        cbBox_Armband->insertItems(0, QStringList()
         << QApplication::translate("TestModule", "COM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("TestModule", "COM9", 0, QApplication::UnicodeUTF8)
        );
        Btn_Connect->setText(QApplication::translate("TestModule", "\350\277\236\346\216\245", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TestModule", "\350\277\236\346\216\245\347\212\266\346\200\201\357\274\232", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("TestModule", "\346\265\213\350\257\225\347\233\270\345\205\263\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TestModule", "\346\257\217\344\270\252\345\212\250\344\275\234\351\207\215\345\244\215\346\254\241\346\225\260", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TestModule", "\345\212\250\344\275\234\346\214\201\347\273\255\346\227\266\351\227\264", 0, QApplication::UnicodeUTF8));
        Btn_StartTest->setText(QApplication::translate("TestModule", "\345\274\200\345\247\213\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("TestModule", "\346\265\213\350\257\225\346\212\245\345\221\212", 0, QApplication::UnicodeUTF8));
        Btn_CreateReport->setText(QApplication::translate("TestModule", "\347\224\237\346\210\220\346\212\245\345\221\212", 0, QApplication::UnicodeUTF8));
        BtnExportReport->setText(QApplication::translate("TestModule", "\345\257\274\345\207\272\346\212\245\345\221\212", 0, QApplication::UnicodeUTF8));
        BtnSaveClassifier->setText(QApplication::translate("TestModule", "\344\277\235\345\255\230\345\210\206\347\261\273\345\231\250", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TestModule: public Ui_TestModule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTMODULE_H
