#ifndef _DLG_TEST_MODULE_
#define _DLG_TEST_MODULE_

#include <QtGui/QDialog>
#include "GUI/ui_TestModule.h"
#include <deque>
#include "HelperArmBand.h"
#include <boost/thread.hpp>
#include <QtCore/QTimer>
#include "ClassifierBase.h"
#include <QtGui/QAbstractItemView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>

class Dlg_TestModule : public QDialog, public Ui_TestModule
{
	Q_OBJECT
private:
	// use int to store command
	// 0: rest
	// 1: thumb, 2: index, 4: middle, 8: ring, 16: little
	// 256: shangqie, 512: xiaqie
	// 1024: neifan, 2048: waifan
	// 4096: neixuan, 8192: waixuan
	// 16384: shenzhou, 32768: quzhou
	class _commandInfo
	{
	public:
		std::string Name;
		int         Command;
		size_t      Duration;
		size_t      SampleRate;
		size_t      TotalSample;
		_commandInfo() 
			: Name("")
			, Command(0)
			, Duration(0)
			, SampleRate(0)
			, TotalSample(0)
		{}
	};
	std::vector< std::shared_ptr<_commandInfo> > _commandVec;

	// file path
	std::string _mTrainConfigPath;
	std::string _mTrainDataPath;
	std::deque<std::vector<double> > _mDataBuffer;

	// ArmBand
	SJTArmBand* _armBand;
	std::vector<std::vector<double> > _armBandData;

	// thread: get data, send command, ...
	boost::thread _mThread;
	// single action duration, in second, used in thread
	// default: 6s
	size_t _mSingleDuration;

	// Timer, for processing bar
	QTimer* qTimer;
	int processingBarVal;

	// Classifier
	ClassifierBase* _mClassifier;
	typedef ClassifierBase* (*CreateClassifier)();
	typedef void (*DestroyClassifier)(ClassifierBase*);
	CreateClassifier _mCreateClassifierFunc;
	DestroyClassifier _mDestroyClassifierFunc;

	// Shared memory
	unsigned char* _ucpNameSharedMem;
	size_t _stLenSharedMem;

	// Statistics
	// 1 ÿ��������Ӧ����Ԥ����� <command, predict>
	std::vector< std::pair<int, int> > _predictPerAction;
	// 2 ÿ��������ȷ��Ԥ�����
	std::vector<int> _rightPrdtPerAction;
	// 3 ��һ��Ԥ����ȷ��Ӧ��ʱ���ӳ�
	std::vector<int> _firstHitDelay;
	// 4 ��һ��Ԥ����ȷ�󣬶�������׼ȷ��
	std::vector<float> _holdStability;

	// Table model
	QStandardItemModel* _tableModel;

public:
	Dlg_TestModule(unsigned char* nameSharedMem, size_t lenSharedMem, QWidget* parent = NULL);
	~Dlg_TestModule();

private:
	void _parseTrainConfig();

	static void _threadSend(Dlg_TestModule* dtm, std::vector<int> testSeries);

	void _initTableView();

	void _updateGUI();

public slots:
	void on_BtnImportConfig_clicked();
	void on_BtnImportData_clicked();
	void on_BtnCreateClassifier_clicked();
	void on_BtnSaveClassifier_clicked();
	void on_Btn_Connect_clicked();
	void on_Btn_StartTest_clicked();
	void on_Btn_CreateReport_clicked();
	void on_BtnExportReport_clicked();
	void on_BtnOpenPlugin_clicked();

private slots:
	void _qTimer_timeout();
};

#endif // _DLG_TEST_MODULE_