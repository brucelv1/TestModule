#include "Dlg_TestModule.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <iostream>
#include "ClassifierEMG.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <QtGui/QMessageBox>
#include "LDA_Bayesian.h"

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
	// read config file, and generate label vector
	_parseTrainConfig();
	std::vector<int> labelVec;
	for (size_t i=0; i<_commandVec.size(); i++)
	{
		size_t tsp = _commandVec[i]->TotalSample;
		int label = _commandVec[i]->Command;
		for (size_t j=0; j<tsp; j++)
		{
			labelVec.push_back(label);
		}
	}

	// read raw data
	std::vector<std::vector<double> > trainData;
	std::ifstream raw_data(_mTrainDataPath);

	std::vector<double> data_tmp(8,0);
	std::string val;
	while(raw_data.good())
	{
		for (int i=0; i<8; i++)
		{
			raw_data >> val;
			data_tmp[i] = std::atof(val.c_str());
		}
		trainData.push_back(data_tmp);
	}

	LDA_Bayesian lb;
	lb.FeatureExtract(trainData,labelVec);
	if(lb.GenerateModel()==true)
	{
		QMessageBox::information(NULL, "Information", "LDA model is generated.", QMessageBox::Ok);
		return;
	}
	else
	{
		QMessageBox::information(NULL, "Information", "Fail to generate LDA model.", QMessageBox::Ok);
		return;
	}
}

void Dlg_TestModule::_parseTrainConfig()
{
	using namespace boost::property_tree;
	ptree root;

	try
	{
		// load the json file in this ptree
		read_json(_mTrainConfigPath,root);
	}
	catch (json_parser::json_parser_error& e)
	{
		std::cout << "Error: Invalid JSON file!" << std::endl;
		std::cout << "[Info] " << e.what() << std::endl;
		system("pause");
		return;
	}

	std::string _type = root.get<std::string>("type","");
	if(_type != "Training Series")
	{
		QMessageBox::warning(NULL, "warning", "Invalid configuration file.", QMessageBox::Ok);
		return;
	}

	ptree Actions = root.get_child("actions");
	std::vector< std::shared_ptr<_commandInfo> > InfoVec;
	for (ptree::iterator it=Actions.begin(); it!=Actions.end(); it++)
	{
		std::string name = it->second.get<std::string>("name");
		int command = it->second.get<int>("command");
		size_t dura = it->second.get<size_t>("duration");
		size_t smRt = it->second.get<size_t>("sample_rate");
		size_t tSp = it->second.get<size_t>("total_sample");
		std::shared_ptr<_commandInfo> cinfo(new _commandInfo());
		cinfo->Name = name;
		cinfo->Command = command;
		cinfo->Duration = dura;
		cinfo->SampleRate = smRt;
		cinfo->TotalSample = tSp;
		InfoVec.push_back(cinfo);
	}

	_commandVec.clear();
	_commandVec = InfoVec;
}
