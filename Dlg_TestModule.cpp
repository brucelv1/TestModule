#include "Dlg_TestModule.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <iostream>
#include "ClassifierEMG.h"

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
	std::ifstream raw_data(_mTrainDataPath);

	std::vector<double> data_tmp(8,0);
	std::string val;
	while(raw_data.good())
	{
		for (int i=0; i<8; i++)
		{
			//std::getline(raw_data,val/*,','*/);
			raw_data >> val;
			data_tmp[i] = std::atof(val.c_str());
		}
		trainData.push_back(data_tmp);
	}

}
