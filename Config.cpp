#include "Config.h"

#ifdef SAMSUNG_EXPORT_LIBRARY
	// 使用方法：测试状态注释掉下方一行代码，训练状态则保留
	#define TRAIN_STATE

	#ifdef TRAIN_STATE
		int IS_TRAIN = 1;           //0:测试状态，1:训练状态
		int IS_OnLineTest = 0;      //
	#else
		int IS_TRAIN = 0;
		int IS_OnLineTest = 1;
	#endif
#endif

bool Is_Action = false;
