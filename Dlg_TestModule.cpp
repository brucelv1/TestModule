#include "Dlg_TestModule.h"
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <QtGui/QMessageBox>

Dlg_TestModule::Dlg_TestModule(unsigned char* nameSharedMem, size_t lenSharedMem, QWidget* parent /*= NULL*/ )
	: QDialog(parent)
	, _armBand(new SJTArmBand())
	, _mLDA(new LDA_Bayesian())
	, _ucpNameSharedMem(nameSharedMem)
	, _stLenSharedMem(lenSharedMem)
{
	setupUi(this);
}

Dlg_TestModule::~Dlg_TestModule()
{
	_mThread.join();
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

	
	_mLDA->FeatureExtract(trainData,labelVec);
	if(_mLDA->GenerateModel()==true)
	{
		LEClassifierStatus->setText("LDA model is generated.");
		return;
	}
	else
	{
		QMessageBox::information(NULL, "Information", "Fail to generate LDA model.", QMessageBox::Ok);
		LEClassifierStatus->setText("Failed, please try again.");
		return;
	}
}

void Dlg_TestModule::on_Btn_Connect_clicked()
{
	std::string strCom = cbBox_Armband->currentText().toStdString();
	int nCOM = std::atoi( (strCom.substr(3,strCom.size()-3)).c_str() );

	if(_armBand->Connect(nCOM)==false)
	{
		LE_Armband_Status->setText("Connection Failed.");
		return;
	}
	else
	{
		LE_Armband_Status->setText("Connection Successful.");
	}
}

void Dlg_TestModule::on_Btn_StartTest_clicked()
{
	std::vector<int> testSeries;
	std::vector<int> classLabel = _mLDA->GetClassVector();
	for(int i=1; i<=spinB_ActionTimes->value(); i++)
	{
		testSeries.insert(testSeries.end(), classLabel.begin(), classLabel.end());
	}
	_mThread = boost::thread(boost::bind(&(Dlg_TestModule::_threadSend),this,testSeries));
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

void Dlg_TestModule::_threadSend( Dlg_TestModule* dtm, std::vector<int> testSeries)
{
	using namespace boost::chrono;

	// timing
	duration<double> time_span;
	steady_clock::time_point t1, t2;

	// 臂带一开始不能正常读数，先等待1000ms
	//boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));

	// 读秒准备：3秒
	t1 = steady_clock::now();
	do 
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
		t2 = steady_clock::now();
		time_span = duration_cast<duration<double> > (t2-t1);

		if (time_span.count() <= 1)
			dtm->_ucpNameSharedMem[5] = 3;// set memory to 3
		else if (time_span.count() <= 2)
			dtm->_ucpNameSharedMem[5] = 2;// set memory to 2
		else if (time_span.count() <= 3)
			dtm->_ucpNameSharedMem[5] = 1;// set memory to 1
		else
		{
			dtm->_ucpNameSharedMem[5] = 0;// set memory to 0
			break;
		}
	} while (1);

	// counting
	int progress = 0;
	int total_num = testSeries.size();
		
	for (int i=0; i<total_num; i++)
	{
		t1 = steady_clock::now();

		int command = testSeries[i];
		std::cout << "command: " << command << "  ";
		int cnt = 0;
		int right = 0;

		unsigned char byte0 = command % 256;
		unsigned char byte1 = (command >> 8) % 256;
		unsigned char byte2 = 0; // not used yet
		unsigned char byte3 = 0; // not used yet

		// set command
		dtm->_ucpNameSharedMem[4] = byte0;
		dtm->_ucpNameSharedMem[3] = byte1;
		dtm->_ucpNameSharedMem[2] = byte2; // not used yet
		dtm->_ucpNameSharedMem[1] = byte3; // not used yet

		// sampling, wait for duration
		int sampleIdx = 0;
		do 
		{
			// data query
			if(dtm->_armBand->IsConnected())
			{
				dtm->_armBandData.push_back(dtm->_armBand->GetDataVector());
			}

			// 凑够了一个窗长，做一次预测
			if (dtm->_armBandData.size() == dtm->_mLDA->_mFeaWinWidth)
			{
				std::vector<double> fea = dtm->_mLDA->FeatureExtractToVec(dtm->_armBandData);
				int predict = dtm->_mLDA->Predict(fea);
				
				cnt++;
				if (predict == command)
					right++;

				dtm->_armBandData.clear();
			}

			boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

			t2 = steady_clock::now();
			time_span = duration_cast<duration<double> > (t2-t1);
			sampleIdx+=1;
		} while (time_span.count()<6);

		std::cout << "count: " << cnt << "   ";
		std::cout << "rate: " << right*1.0 / cnt << std::endl;

		progress+=1;
		dtm->processingBarVal = 100*progress/total_num;
	}
	return;
}
