#ifndef _DLG_TEST_MODULE_
#define _DLG_TEST_MODULE_

#include <QtGui/QDialog>
#include "GUI/ui_TestModule.h"
#include <deque>

class Dlg_TestModule : public QDialog, public Ui_TestModule
{
	Q_OBJECT
private:
	std::string _mTrainConfigPath;
	std::string _mTrainDataPath;
	std::deque<std::vector<double> > _mDataBuffer;
public:
	Dlg_TestModule(QWidget* parent = NULL);
	~Dlg_TestModule();

public slots:
	void on_BtnImportConfig_clicked();
	void on_BtnImportData_clicked();
	void on_BtnCreateClassifier_clicked();
};

#endif // _DLG_TEST_MODULE_