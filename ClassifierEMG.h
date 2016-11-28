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

	//// ֻ��������ͼ��
	//double	data_rms[DATA_CHANNEL][100]; // ֻ��MatlabEng�и���
	//double	data_rms_max[DATA_CHANNEL];	 // ֻ��MatlabEng�и���
	//int		rms_index;                   // ֻ��MatlabEng�и���


	int		data_index; // ԭʼ�����кţ��������
	int		mat_index;  // ���ݴ����кţ��������

	HANDLE	m_hThread;
	bool	mat_over;
	int		dur;

	// NIR
	double total_nirs;
	double average_nirs;

	int		state;
	
	// �ļ��洢
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


	vector<int> online_label;//LB//���߲���
	vector<int> online_set_label;//LB//���߲���
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
�������ƣ����캯��
��    �ܣ����ı��ж�ȡ���ںš�ͨ�����Ȳ����������������ı�
��    ������       
����ֵ  ����
********************************************/
	CEMG();
	
/*******************************************
�������ƣ���������
��    �ܣ��ر��ļ����ͷ��ڴ�
��    ������       
����ֵ  ����
********************************************/
	~CEMG();
	
/*******************************************
�������ƣ�Continue
��    �ܣ�������״̬��RESTת��ΪTRAIN�����¿ո��ʱ����//LB//��CVR.h�д���
��    ������       
����ֵ  ����
********************************************/
	void Continue();
	
/*******************************************
�������ƣ�SetData
��    �ܣ�������д�뵽EMG��data��Ա�У�ȥ��ǰ2000�����ݣ�����������ı�
��    ����Ҫд�������ָ��       
����ֵ  ����
********************************************/
	void SetData(double *p);
	
/*******************************************
�������ƣ�MatlabEng
��    �ܣ��������ݣ��ж�Ŀǰʵ�鴦����һ���裬��������Ӧ����
��    ������     
����ֵ  ����
********************************************/
	void MatlabEng();
	
/*******************************************
�������ƣ�feature_extract
��    �ܣ���ȡ������TD��DFT��AR��������SVM�����жϽ��data_predict
��    ������     
����ֵ  ����
********************************************/
	void feature_extract();       //LB// ѵ��״̬��mat_index=3000��3100������������״̬��mat_index=300��400������
	

	void SVMtrain();
};
