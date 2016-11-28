#pragma once

/***********在这里选择输入设备，注意从0开始**************/
enum DEVICES{EMG_NIR,BIOMETRICS,DELSYS,EMG}; 
#define DEVICE DEVICES(3) 

/***********在这里选择串口号**************/
//#define COM_NUM   11

/***********在这里选择动作数和每个动作的持续时间**************/
//#define class_num			13	  //用到的手势数
//#define class_used		{1 2 3 4 5 6 7 8 9 10 11 12 13}  //用到的手势
#define ACTION_TIME_TRAINING	5    //每个动作持续时间
#define ACTION_TIME_ONLINE_TESTING 10
#define REST_TIME_ONLINE_TESTING   5


#define ACTION_NUM          4/*8*/    //1个trial的总动作数
#define ACTION_NUM_ONLINE_TESTING  5

//#define ACTION_SERIAL       {13,1,2,3,4,5,6,7,8,9,10,11,12}

/***********在这里选择通道数，注意与config.txt里保持一致**************/
#define DATA_CHANNEL		8
//#define channel_num         4
//#define channel_used		{1,2,3,4};

/***********在这里选择训练、测试、在线测试状态**************/
#define	IS_TRAIN			1
#define IS_OnLineTest		0

/***********在这里选择外接设备状态**************/
#define IS_CONNECT			0

/***********在这里选择特征提取办法，注意从0开始**************/
#define ARORDER             6
enum FEATURES{EMG_TD,EMG_AR,EMG_TDAR,NIR,EMGTD_NIR,EMGTDAR_NIR,DFT}; 
#define FEATUREN FEATURES(0)

enum CLASSIFIERS{SVM,LDA,LDA_SE,LDA_CA}; 
#define CLASSIFIER CLASSIFIERS(1)

enum TESTDAYS{FIRST,SECOND,OTHERS};
#define TESTDAY TESTDAYS(0)

/***********在这里选择Trial数**************/
#define TRIAL_NUM			1


#if(IS_OnLineTest)
#define DATA_LENGTH		(ACTION_NUM_ONLINE_TESTING*ACTION_TIME_ONLINE_TESTING*1000) // used to be 1000
#else
#define DATA_LENGTH			(ACTION_NUM*ACTION_TIME_TRAINING*1000)
#endif

#define PI					3.1415926535
#define BAUD_RATE			57600
#define TIME_INTERVAL		200				//样本所取得时间长度
#define	TIME_INCEMENT		100				//提取样本时间增量
enum	STATE { REST,START,END,HIGH_PASS,NOTCH,COMB,FIG1,FIG2,FIG3}; //程序的状态

#define DELETE_ARRAY(x) if (x != NULL) {delete[] x; x = NULL;}
#define DELETE_OBJECT(x) if (x != NULL) {delete   x; x = NULL;}
#define NEL(x)  (sizeof(x)/sizeof(x[0]))
#define Malloc(type,n) (type *)malloc((n)*sizeof(type)) 

extern bool Is_Action;
