#include "Dlg_TestModule.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <iostream>

Dlg_TestModule::Dlg_TestModule( QWidget* parent /*= NULL*/ )
	: QDialog(parent)
{
	setupUi(this);
}

Dlg_TestModule::~Dlg_TestModule()
{

}

void Dlg_TestModule::on_BtnImportConfig_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Import Training Configuration File"), 
		"",
		tr("Config Files (*.json)"), 
		0);

	if (!filename.isNull())
	{
		LEConfigPath->setText(filename);
		_mTrainConfigPath = filename.toStdString();
	}
}

void Dlg_TestModule::on_BtnImportData_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Import Training Data File"), 
		"",
		tr("Data Files (*.txt)"), 
		0);

	if (!filename.isNull())
	{
		LEDataPath->setText(filename);
		_mTrainDataPath = filename.toStdString();
	}
}

void Dlg_TestModule::on_BtnCreateClassifier_clicked()
{
	std::vector<std::vector<double> > trainData;
}
