#include <QtGui/QApplication>
#include "Dlg_TestModule.h"

int main(int argc, char** argv)
{
	QApplication qapp(argc, argv);
	Dlg_TestModule dtm;
	dtm.setModal(false);
	dtm.show();
	qapp.exec();
	return 0;
}