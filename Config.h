#pragma once

/***********������ѡ�������豸��ע���0��ʼ**************/
enum DEVICES{EMG_NIR,BIOMETRICS,DELSYS,EMG}; 
#define DEVICE DEVICES(3) 

/***********������ѡ�񴮿ں�**************/
//#define COM_NUM   11

/***********������ѡ��������ÿ�������ĳ���ʱ��**************/
//#define class_num			13	  //�õ���������
//#define class_used		{1 2 3 4 5 6 7 8 9 10 11 12 13}  //�õ�������
#define ACTION_TIME_TRAINING	5    //ÿ����������ʱ��
#define ACTION_TIME_ONLINE_TESTING 10
#define REST_TIME_ONLINE_TESTING   5


#define ACTION_NUM          4/*8*/    //1��trial���ܶ�����
#define ACTION_NUM_ONLINE_TESTING  5

//#define ACTION_SERIAL       {13,1,2,3,4,5,6,7,8,9,10,11,12}

/***********������ѡ��ͨ������ע����config.txt�ﱣ��һ��**************/
#define DATA_CHANNEL		8
//#define channel_num         4
//#define channel_used		{1,2,3,4};

/***********������ѡ��ѵ�������ԡ����߲���״̬**************/
#define	IS_TRAIN			1
#define IS_OnLineTest		0

/***********������ѡ������豸״̬**************/
#define IS_CONNECT			0

/***********������ѡ��������ȡ�취��ע���0��ʼ**************/
#define ARORDER             6
enum FEATURES{EMG_TD,EMG_AR,EMG_TDAR,NIR,EMGTD_NIR,EMGTDAR_NIR,DFT}; 
#define FEATUREN FEATURES(0)

enum CLASSIFIERS{SVM,LDA,LDA_SE,LDA_CA}; 
#define CLASSIFIER CLASSIFIERS(1)

enum TESTDAYS{FIRST,SECOND,OTHERS};
#define TESTDAY TESTDAYS(0)

/***********������ѡ��Trial��**************/
#define TRIAL_NUM			1


#if(IS_OnLineTest)
#define DATA_LENGTH		(ACTION_NUM_ONLINE_TESTING*ACTION_TIME_ONLINE_TESTING*1000) // used to be 1000
#else
#define DATA_LENGTH			(ACTION_NUM*ACTION_TIME_TRAINING*1000)
#endif

#define PI					3.1415926535
#define BAUD_RATE			57600
#define TIME_INTERVAL		200				//������ȡ��ʱ�䳤��
#define	TIME_INCEMENT		100				//��ȡ����ʱ������
enum	STATE { REST,START,END,HIGH_PASS,NOTCH,COMB,FIG1,FIG2,FIG3}; //�����״̬

#define DELETE_ARRAY(x) if (x != NULL) {delete[] x; x = NULL;}
#define DELETE_OBJECT(x) if (x != NULL) {delete   x; x = NULL;}
#define NEL(x)  (sizeof(x)/sizeof(x[0]))
#define Malloc(type,n) (type *)malloc((n)*sizeof(type)) 

extern bool Is_Action;
