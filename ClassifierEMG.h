#pragma once

#include "Config.h"
#include "svm.h"
#include "MathUtility.h"
#include "Byesclassifier.h"

#include <Windows.h>

static DWORD WINAPI MatlabThreadEntry(PVOID arg);

int* create_randperm(int length);

class CEMG
{
public:
	int     k;
	int		trial_num;
	int		channel_used[8];
	int		channel_num;
	int		class_used[13];
	int		class_num;
	int		com_number;
	int		fea_num;
	int		fea_per_channel;

	int		action_num;
	int		action_serial[40];

	double	data[DATA_LENGTH][DATA_CHANNEL];
	double	data_predict[DATA_LENGTH];

	//// 只是用来画图的
	//double	data_rms[DATA_CHANNEL][100]; // 只在MatlabEng中更新
	//double	data_rms_max[DATA_CHANNEL];	 // 只在MatlabEng中更新
	//int		rms_index;                   // 只在MatlabEng中更新


	int		data_index; // 原始数据行号，可以溢出
	int		mat_index;  // 数据处理行号，可以溢出

	HANDLE	m_hThread;
	bool	mat_over;
	int		dur;

	// NIR
	double total_nirs;
	double average_nirs;

	int		state;
	
	// 文件存储
	char    file_src[100];
	char    file_label[100];
	char    file_online_label[100];
	char    file_online_set_label[100];
	char    file_fea[100];
	char    file_date[100];

	// TRAIN Part
	// feature matrix and label vector
	vector<vector<double> > fea;
	vector<int>	label;


	vector<int> online_label;//LB//在线测试
	vector<int> online_set_label;//LB//在线测试
	int		random_figure;
	int		*random_hand_serial;
	int		*random_serial;
	int     Is_Correct_Num;

	vector<vector<double> > lda_model_mean;
	vector<vector<double> > lda_model_cov;

	struct svm_model *model;
	struct svm_node  *x;
	std::ofstream outfile;

/*******************************************
函数名称：构造函数
功    能：从文本中读取串口号、通道数等参数，并创建数据文本
参    数：无       
返回值  ：无
********************************************/
	CEMG();
	
/*******************************************
函数名称：析构函数
功    能：关闭文件、释放内存
参    数：无       
返回值  ：无
********************************************/
	~CEMG();
	
/*******************************************
函数名称：Continue
功    能：将程序状态由REST转变为TRAIN，按下空格键时调用//LB//由CVR.h中触发
参    数：无       
返回值  ：无
********************************************/
	void Continue();
	
/*******************************************
函数名称：SetData
功    能：将数据写入到EMG的data成员中（去掉前2000个数据），并输出到文本
参    数：要写入的数据指针       
返回值  ：无
********************************************/
	void SetData(double *p);
	
/*******************************************
函数名称：MatlabEng
功    能：处理数据，判断目前实验处于哪一步骤，并给出相应处理
参    数：无     
返回值  ：无
********************************************/
	void MatlabEng();
	
/*******************************************
函数名称：feature_extract
功    能：提取特征：TD、DFT、AR，并根据SVM给出判断结果data_predict
参    数：无     
返回值  ：无
********************************************/
	void feature_extract();       //LB// 训练状态下mat_index=3000，3100，……；测试状态下mat_index=300，400，……
	

	void SVMtrain();
};
