#include "Config.h"

#ifdef SAMSUNG_EXPORT_LIBRARY
	// ʹ�÷���������״̬ע�͵��·�һ�д��룬ѵ��״̬����
	#define TRAIN_STATE

	#ifdef TRAIN_STATE
		int IS_TRAIN = 1;           //0:����״̬��1:ѵ��״̬
		int IS_OnLineTest = 0;      //
	#else
		int IS_TRAIN = 0;
		int IS_OnLineTest = 1;
	#endif
#endif

bool Is_Action = false;
