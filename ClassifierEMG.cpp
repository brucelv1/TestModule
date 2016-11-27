#include "ClassifierEMG.h"
#include <ctime>

static DWORD WINAPI MatlabThreadEntry(PVOID arg)
{		
	((CEMG *)arg)-> MatlabEng();
	return 0;
}

int* create_randperm(int length){//LB//����length���ȵ��������

	int *orgin=new int[length];
	int *result=new int[length];

	int i,temp;


	for(i=0;i<length;i++)
		orgin[i]=i;
	srand((int)std::time(0));
	i=0;
	while(1){
		temp=rand()%length;
		if(orgin[temp]==-1)
			continue;
		else{
			result[i]=orgin[temp];
			orgin[temp]=-1;
			i++;
		}
		if(i==length)
			break;
	}
	delete[] orgin;

	return result;
}


/*******************************************
�������ƣ����캯��
��    �ܣ����ı��ж�ȡ���ںš�ͨ�����Ȳ����������������ı�
��    ������       
����ֵ  ����
********************************************/
CEMG::CEMG()
{
	std::ifstream file_temp(".\\data\\config.txt"); //LB// ��inc/config.txt�еĴ��ںż�ͨ����Ϣ

	data_predict[0]=0;

	char buff[9];
	for(int i=0;i<3;i++)             //LB// COM		5  �źŲɼ��豸�Ĵ��ں�
		file_temp>>buff[0];
	file_temp>>com_number;           //LB// com_number=5


	for(int i=0;i<6;i++)             //LB// ACTION		13:	1 2 3 4 5 6 7 8 9 10 11 12 13
		file_temp>>buff[0];
	file_temp>>class_num;            //LB// class_num=13
	file_temp>>buff[0];
	for(int i=0;i<class_num;i++)
	{
		file_temp>>class_used[i];    //LB// �õ��Ķ����� class_used[0]=1 2 3 4 5 6 7 8 9 10 11 12 13
		class_used[i]--;             //LB// ��һ class_used[0]=0,......
	}
	for(int i=0;i<7;i++)             //LB// CHANNEL 8     :1 2 3 4 5 6 7 8
		file_temp>>buff[0];
	file_temp>>channel_num;          //LB// channel_num=8    
	file_temp>>buff[0];
	for(int i=0;i<channel_num;i++)   
	{
		file_temp>>channel_used[i];  //LB// �õ���ͨ���� channel_used[i]=1 2 3 4 5 6 7 8
		channel_used[i]--;           //LB// ��һ channel_used[i]=0 1 2 3 4 5 6 7
	}

	for(int i=0;i<13;i++)            //LB//��ȥACTION_SERIAL 
		file_temp>>buff[0];
	file_temp>>action_num;          //LB//action_num=14    
	file_temp>>buff[0];				//LB//��ȥ: 
	for(int i=0;i<action_num;i++)   
	{
		file_temp>>action_serial[i];  //LB// �õ���ͨ���� action_serial[i]=13 1 2 3 4 5 6 7 8
		action_serial[i]--;           //LB// ��һ action_serial[i]=12 0 1 2 3 4 5 6 7
	}

	//file_temp.close();

	switch (FEATUREN)
	{
	case EMG_TD:
		fea_per_channel=4;
		break;
	case EMG_AR:
		fea_per_channel=ARORDER;
		break;
	case EMG_TDAR:
		fea_per_channel=(4+ARORDER);
		break;
	case NIR:
		fea_per_channel=3;
		break;
	case EMGTD_NIR:
		fea_per_channel=4;
		break;
	case EMGTDAR_NIR:
		fea_per_channel=4;
		break;
	case DFT:
		fea_per_channel=6;
		break;
	}
	
	fea_num=channel_num*fea_per_channel/2+2*channel_num/2;           //fea_num
	if (FEATUREN==EMGTDAR_NIR)
		fea_num=52;


	//�ļ���¼
	sprintf_s(file_date,"2008.12.26_09.32"); //LB// ��file_data�ĳ�ϵͳʱ��

	_strdate_s( buff, 9 ); //LB// ���Ƶ�ǰϵͳ���ڵ������� _strdate_s( tmpbuf, 128 ); printf( "OS date:\t\t\t\t%s\n", tmpbuf ); OS date: 04/25/03  2003��4��25��
	file_date[2]=buff[6];
	file_date[3]=buff[7];
	file_date[5]=buff[0];
	file_date[6]=buff[1];
	file_date[8]=buff[3];
	file_date[9]=buff[4];
	_strtime_s( buff, 9 ); //LB// д��ʱ�䵽������      _strtime_s( tmpbuf, 128 );   printf( "OS time:\t\t\t\t%s\n", tmpbuf );    OS time: 13:51:23
	file_date[11]=buff[0];
	file_date[12]=buff[1];
	file_date[14]=buff[3];
	file_date[15]=buff[4]; //LB//��file_date��Ϊϵͳ��ǰʱ��

	sprintf_s(file_label,".\\data\\%s_label.txt",file_date);   //LB// data�ļ���  ������_label.txt���ļ�
	sprintf_s(file_fea,".\\data\\%s_fea.txt",file_date);       //LB// data�ļ���  ������_fea.txt���ļ�
	sprintf_s(file_src,".\\data\\%s_data.txt",file_date);      //LB// data�ļ���  ������_data.txt���ļ�
	outfile.open(file_src);                               //LB// �򿪡�data.txt���ļ�

	switch (CLASSIFIER)
	{
	case SVM:{
		sprintf_s(file_online_label,".\\data\\%s_online_label_SVM.txt",file_date);   //LB// data�ļ���  ������_oneline_label.txt���ļ�
		sprintf_s(file_online_set_label,".\\data\\%s_online_set_label_SVM.txt",file_date);   //LB// data�ļ���  ������_oneline_label.txt���ļ�
		break;
			 }
	case LDA:{
		sprintf_s(file_online_label,".\\data\\%s_online_label_LDA.txt",file_date);   //LB// data�ļ���  ������_oneline_label.txt���ļ�
		sprintf_s(file_online_set_label,".\\data\\%s_online_set_label_LDA.txt",file_date);   //LB// data�ļ���  ������_oneline_label.txt���ļ�
		break;
			 }
	}

	for(int i=0;i<DATA_LENGTH;i++)
		data_predict[i]=-1;

	if(IS_TRAIN)        //LB// ѵ��ʱ�����κζ���
	{

	}
	else                //LB// ����ʱ�����÷�����ģ��
	{
		switch (CLASSIFIER)
		{
			case SVM:
			{
				model=svm_load_model(".\\data\\svm_model.txt");
				x =Malloc(svm_node,fea_num+1);
				break;
			}
			case LDA:
			{
				std::ifstream openfile;
				openfile.open(".\\data\\LDA_mean.txt");
				double tempt;
				vector<double> tempvector;
				for(int i=0;i<class_num;i++)
				{
					for(int j=0;j<fea_num;j++)
					{
						openfile>>tempt;
						tempvector.push_back(tempt);
					}
					lda_model_mean.push_back(tempvector);
					tempvector.clear();
				}
				openfile.close();

				openfile.open(".\\data\\LDA_cov.txt");

				for(int i=0;i<fea_num;i++)
				{
					for(int j=0;j<fea_num;j++)
					{
						openfile>>tempt;
						tempvector.push_back(tempt);
					}
					lda_model_cov.push_back(tempvector);
					tempvector.clear();
				}
				openfile.close();
				break;
			}
		}
	}

	trial_num=0;
	data_index=-1;
	mat_index=-1;
	mat_over=true;

	m_hThread=CreateThread(NULL,0,MatlabThreadEntry,(PVOID)this,0,NULL);  //LB// MatlabThreadEntry��ʾ���߳�ִ�е��̺߳�����ַ  ��һ��0��ʾ�̴߳�������������

	state=REST;
	dur=-1;             //LB// REST״̬��dur=-1
	memset(data,0,sizeof(data));          //LB// ���Ѿ������ڴ�ռ�data��sizeof(data)���ֽڵ�ֵ��Ϊ0���Խϴ�Ľṹ�����������������췽����
	memset(data_rms,0,sizeof(data_rms));
	memset(data_rms_max,1,sizeof(data_rms_max));
	rms_index=-1;
	Is_Correct_Num = 0;

}
/*******************************************
�������ƣ���������
��    �ܣ��ر��ļ����ͷ��ڴ�
��    ������       
����ֵ  ����
********************************************/
CEMG::~CEMG()
{
	state=END;
	Sleep(1000);
	if(m_hThread!=NULL) 
	{
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
	outfile.close();

	// save label vector to disk
	if(label.size()>0)      
	{
		outfile.open(file_label);
		for(size_t i=0;i<label.size();i++)
		{
			outfile<<label[i]<<'\n';
		}
		outfile.close();
	}

	// save online label to disk
	if(online_label.size()>0)   //LB//���߲��Բ���   
	{
		outfile.open(file_online_label);
		for(size_t i=0;i<online_label.size();i++)
		{
			outfile<<online_label[i]<<'\n';
		}
		outfile.close();
	}
	if(online_set_label.size()>0)      
	{
		outfile.open(file_online_set_label);
		for(size_t i=0;i<online_set_label.size();i++)
		{
			outfile<<online_set_label[i]<<'\n';
		}
		outfile.close();
	}

	if(IS_TRAIN)         //LB// ѵ����ʱ��������ļ��С�fea.txt�����ӻ��У�������train()����
	{
		if(fea.size()>0)
		{
			outfile.open(file_fea);
			for(size_t i=0;i<fea.size();i++)
			{
				for(size_t j=0;j<fea[i].size();j++)
					outfile<<fea[i][j]<<'\t';
				outfile<<'\n';
			}
			outfile.close();
			if(CLASSIFIER==SVM)
			{
				// only be called here, and since using LDA, this method
				// actually does not get called at all.
				SVMtrain();
			}
			else
			{
				// only be called here.
				// this method will update two files
				Byestrain(fea,label);
			}
		}
	}
	else
	{
		free(x);
	}
}
/*******************************************
�������ƣ�Continue
��    �ܣ�������״̬��RESTת��ΪTRAIN�����¿ո��ʱ����//LB//��CVR.h�д���
��    ������       
����ֵ  ����
********************************************/
void CEMG::Continue()
{
	if(state==REST&&mat_index==data_index)
	{
		memset(data,0,sizeof(data));
		dur=-2000;         //LB// ����״̬��RESTת��ΪSTART��dur=-2000
		state=START;
		if(IS_TRAIN)
		{
			//�˸����ı�ǩ
			/*for(int i=0;i<DATA_LENGTH/5000;i++)
			{
			static int j0=0;

			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=class_used[j0%class_num];
			j0++;
			}*/
			//ʮ����������5s��Ϣ��ǩ
			/*for(int i=0;i<DATA_LENGTH/5000;i+=2)
			{
			static int j0=0;

			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=class_used[j0%class_num];
			j0++;
			}
			for(int i=1;i<DATA_LENGTH/5000;i+=2)
			{		
			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=13;
			}*/
			/*NIR40��������ʽ
			for(int i=0;i<DATA_LENGTH/5000;i+=3)    //LB// data_predict��ʼ�� ǰ5000����Ϊ13���м�5000����Ϊ13����5000����Ϊ0;5000*13 5000*13 5000*1������
			{
			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=13;
			}
			for(int i=1;i<DATA_LENGTH/5000;i+=3)
			{
			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=13;
			}
			for(int i=2;i<DATA_LENGTH/5000;i+=3)
			{
			static int j0=0;

			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=class_used[j0%class_num]; //LB// class_num=8,class_used[i]=0 1 2 3 4 5 6 7 
			j0++;
			}
			*/
			for(int i=0;i<DATA_LENGTH/(1000*ACTION_TIME_TRAINING);i++)    
			{
				for(int j=0;j<(1000*ACTION_TIME_TRAINING);j++)
					data_predict[j+i*(1000*ACTION_TIME_TRAINING)]=action_serial[i];
			}
		}
		else if (IS_OnLineTest)
		{
			random_hand_serial=create_randperm(class_num-1);//LB//
			//for(int i=0;i<class_num-1;i++)
			//random_hand_serial[i]=class_used[(random_serial[i])];//?????????????????????????????????????????????????//
			random_hand_serial[class_num-1]=class_num-1;
			random_figure=class_num-1;//LB//random_figure��ʼֵ��ΪREST
		}
	}
}
/*******************************************
�������ƣ�SetData
��    �ܣ�������д�뵽EMG��data��Ա�У�ȥ��ǰ2000�����ݣ�����������ı�
��    ����Ҫд�������ָ��       
����ֵ  ����
********************************************/
void CEMG::SetData(double *p)
{
	if(dur>0)   //LB// ��ȥ��2000�����ݺ�dur=DATA_LENGTH
	{
		dur--;
		int index_temp=(data_index+1)%DATA_LENGTH;   //LB// data_index=-1
		for (int j=0;j<DATA_CHANNEL;j++)
		{
			data[index_temp][j]=*p;       //LB// ��EMG&NIR�źŴ洢��data�У�ǰ4λΪEMG�źţ���4λΪNIR�ź�
			p++;
		}			
		for (int j=0;j<channel_num;j++)
			outfile<<data[index_temp][channel_used[j]]<<'\t';   //LB// outfileΪfile_src����data.txt��д������
		outfile<<'\n';
		data_index++;        //LB// ��data.txt����д�����ݵ�����
		if(IS_TRAIN&&data_index%(1000*ACTION_TIME_TRAINING)==0)
		{
			//PlaySound("inc/3.wav",NULL, SND_ASYNC); //LB//(1000*ACTION_TIME_TRAINING)����������ʾ��
		}
		if(dur==0)
		{
			trial_num++;
			if(trial_num==TRIAL_NUM)
			{
				state=REST;
				trial_num=0;
				//tcounter_flag=false;
			}
			else        //���δ�ɼ������Ҫ��Trial��Ŀ����data����Ʊ���dur���³�ʼ�� 
			{
				memset(data,0,sizeof(data));
				dur=DATA_LENGTH;
				//�˴������¿ո�Ĵ�����continue�Ĵ�����ڴ˴���ʵ�����ݵĳ�ʱ��ɼ�����dur�޸�ΪDATA_LENGTH������˽�����ɼ����ݲ�������Bug
				//state=START; 
			}
		}

	}
	if(dur<0)
	{
		dur++;     //LB// ��Ϊѵ��״̬TRAIN��dur=-2000����ȥ��ǰ2000������
		if(dur%1000==-999)					
			;//PlaySound("inc/2.wav",NULL, SND_ASYNC);//LB//ÿ1s����ʾһ��
		if(dur==0&&state==START)
			dur=DATA_LENGTH;    //LB// 2000�����ݺ�dur���DATA_LENGHTH����200s
	}
}


/*******************************************
�������ƣ�MatlabEng
��    �ܣ��������ݣ��ж�Ŀǰʵ�鴦����һ���裬��������Ӧ����
��    ������     
����ֵ  ����
********************************************/
void CEMG::MatlabEng()
{
	mat_over=false;
	while(!mat_over)
	{
		if(data_index>mat_index)    //LB// ��data.txt��д�����ݺ�data_indexΪԭʼд�����ݵ�������mat_indexΪ��������������ʼֵΪ-1����
		{		
			mat_index++;
			if(mat_index%100==0)        //LB// mat_index=100,200,300����
			{
				rms_index++;                           //LB// ÿ100�����ݣ�rms_index��һ����ʼֵΪ-1��
				for(int i=0;i<channel_num;i++)
					data_rms[i][rms_index%50]=0;       //LB// data_rmsΪchannel_num��50�е����飬���õ����г�ֵΪ0
				for(int j=0;j<50;j++)
				{
					int j0=(mat_index+DATA_LENGTH-j)%DATA_LENGTH;
					for(int i=0;i<channel_num;i++)
						data_rms[i][rms_index%50]+=data[j0][channel_used[i]]*data[j0][channel_used[i]];//LB//ǰ50�и���ͨ�����ݵ�ƽ����
				}
				for(int i=0;i<channel_num;i++)
				{
					data_rms[i][rms_index%50]=sqrt(data_rms[i][rms_index%50]);
					if(data_rms[i][rms_index%50]>data_rms_max[i])
						data_rms_max[i]=data_rms[i][rms_index%50];
					data_rms[i][rms_index%50]=(data_rms[i][rms_index%50]/data_rms_max[i]); //LB// data_rms���������������õ�
				}
			}

			if(IS_TRAIN && mat_index%TIME_INCEMENT==0 && mat_index%(1000*ACTION_TIME_TRAINING)>=TIME_INTERVAL)//LB// ѵ��״̬�� ÿ(1000*ACTION_TIME_TRAINING)�������еĺ�2000�� ÿ100��������һ��������ȡ
			{
				feature_extract(); //LB// mat_index=3000��3100������
			}
			if(!IS_TRAIN)//LB//����OnLineTest
			{
				if(mat_index>=TIME_INTERVAL && mat_index%TIME_INCEMENT==0)//LB// ����״̬�� ÿ100��������һ��������ȡ(��0.1����ȡһ������)��
				{
					feature_extract();//LB// TD����svm�жϳ����data_predict     mat_index=300��400������
				}
				else
					data_predict[mat_index%DATA_LENGTH]=data_predict[(mat_index-1)%DATA_LENGTH];//LB// ����״̬�� ÿ100�����ݶ�һ��(��0.1����ȡһ������)
			}
		}
		else
			Sleep(1);    //LB// mat_index������������data_index����������Ļ����ӳ�һ�¡�
	}
}
/*******************************************
�������ƣ�feature_extract
��    �ܣ���ȡ������TD��DFT��AR��������SVM�����жϽ��data_predict
��    ������     
����ֵ  ����
********************************************/
void CEMG::feature_extract()       //LB// ѵ��״̬��mat_index=3000��3100������������״̬��mat_index=300��400������
{
	vector<double> fea_temp;

	fea_temp.resize(fea_num);

	int channel_index;

	if(FEATUREN==EMG_TD||FEATUREN==EMGTD_NIR)
	{
		// feature TD
		for(int i=0;i<4;i++)//LB// channel_num=8 here
		{
			channel_index=channel_used[i];
			fea_temp[i*4]=0;fea_temp[i*4+1]=0;fea_temp[i*4+2]=0;fea_temp[i*4+3]=0;			
			for(int j=0;j<TIME_INTERVAL;j++)
			{
				int j0=(mat_index+DATA_LENGTH-TIME_INTERVAL+j)%DATA_LENGTH;   
				int j1=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-1)%DATA_LENGTH;  //LB// ???����??? mat_index=300ʱ��ô�� �𣺺���j>0 j>1����
				int j2=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-2)%DATA_LENGTH;

				fea_temp[i*4]+=abs(data[j0][channel_index]);   //LB// fea_temp[0 4 8����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)�����ݾ���ֵ�ĺ�
				if(j>0)
				{
					fea_temp[i*4+1]+=int(data[j0][channel_index]*data[j1][channel_index]>0);//LB// fea_temp[1 5 9����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)������������������Ƿ����0��״̬���ܺ�
					fea_temp[i*4+2]+=abs(data[j0][channel_index]-data[j1][channel_index]);//LB// fea_temp[2 6 10����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)��������������֮��ľ���ֵ���ܺ�
				}
				if(j>1)					
					fea_temp[i*4+3]+=int((data[j0][channel_index]-data[j1][channel_index])*(data[j1][channel_index]-data[j2][channel_index])>0);//LB// fea_temp[3 8 11����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)������������������֮��ĳ˻��Ƿ����0��״̬���ܺ�
			}
		}
		for(int i=4;i<channel_num;i++)//LB// channel_num=8 here
		{
			channel_index=channel_used[i];
			fea_temp[4*4+(i-4)*2]=0;fea_temp[4*4+(i-4)*2+1]=0;			
			for(int j=0;j<TIME_INTERVAL;j++)
			{
				int j0=(mat_index+DATA_LENGTH-TIME_INTERVAL+j)%DATA_LENGTH;   
				int j1=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-1)%DATA_LENGTH;  //LB// ???����??? mat_index=300ʱ��ô�� �𣺺���j>0 j>1����
				//int j2=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-2)%DATA_LENGTH;

				fea_temp[4*4+(i-4)*2]+=data[j0][channel_index];   //LB// fea_temp[0 4 8����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)�����ݾ���ֵ�ĺ�
				if(j>0)
				{
					fea_temp[4*4+(i-4)*2+1]+=abs(data[j0][channel_index]-data[j1][channel_index]);//LB// fea_temp[2 6 10����]λ�Ϸֱ���0 1 2ͨ����ǰ300(TIME_INTERVAL)��������������֮��ľ���ֵ���ܺ�
				}

			}
			fea_temp[4*4+(i-4)*2]=fea_temp[4*4+(i-4)*2]/TIME_INTERVAL;
		}
	}

	// �����������ķ��������ԣ���ʱ�ŵ���Դ�ļ�����

	//finish = clock();
	//double totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
	//double ttime = 100;
	if(IS_TRAIN)        //LB// ѵ����ʱ��������Ԥ��д��"fea.txt"��"label.txt"�ļ�
	{
		fea.push_back(fea_temp); //LB// ��fea_temp����fea��β��
		label.push_back(data_predict[(mat_index-1)%DATA_LENGTH]);//LB// ��data_predict����label��β��   Ϊʲô��50��
	}
	else                
	{
		//LB// ����ʱ����SVM�����жϽ��
		if(CLASSIFIER==SVM)
		{
			//clock_t start;
			//clock_t finish;
			//start = clock();
			//����SVM�����жϽ��
			for(int k=0;k<fea_num;k++)
			{
				x[k].index=k;
				x[k].value=fea_temp[k];
			}
			data_predict[mat_index%DATA_LENGTH]=int(svm_predict(model,x));
			//finish = clock();
			//double totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
			//double ttime = 100;
		}
		else
		{
			//clock_t start;
			//clock_t finish;
			//start = clock();


			// Byespredict is called only here
			data_predict[mat_index%DATA_LENGTH]=int(Byespredict(lda_model_cov,lda_model_mean,fea_temp));


			//finish = clock();
			//double totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
			//double ttime = 100;
			//lda_feat.clear();
		}//LB// ����double data_predict[DATA_LENGTH]��ÿ�����ݵ�ģʽ״̬

		if(IS_OnLineTest)
		{
			random_figure=class_num-1;//LB//һ��״̬��random_figureΪREST
			//if((mat_index/1000)%ACTION_TIME_ONLINE_TESTING==0)
			//{
			//	Is_Correct_Num = 0;
			//	Is_Action = false;
			//}
			if((mat_index/1000)%ACTION_TIME_ONLINE_TESTING>=REST_TIME_ONLINE_TESTING)//LB//���߲���״̬�£�ǰ3sΪREST����6sΪ�������
			{
				random_figure=random_hand_serial[mat_index/(1000*ACTION_TIME_ONLINE_TESTING)];
				//if((mat_index/(1000*ACTION_TIME_TRAINING))==12)
				//random_figure=12;
				if(data_predict[mat_index%DATA_LENGTH]==random_figure)
					Is_Correct_Num++;
			}
			else
			{
				Is_Correct_Num = 0;
				Is_Action = false;
			}
			online_set_label.push_back(random_figure);
			online_label.push_back(data_predict[mat_index%DATA_LENGTH]);//LB// ��data_predict����label��β��   Ϊʲô��50���Ļ�
		}
	}	
}

void CEMG::SVMtrain()
{		
	struct svm_parameter param;		// set by parse_command_line
	param.svm_type = C_SVC;
	param.kernel_type = POLY;
	param.degree = 2;
	param.gamma = 1.0/fea_num;// 1/k
	//param.gamma = 2;// gwc xiugai
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 10;
	//param.C = 5; // gwc xiugai
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	model=NULL;

	struct svm_problem prob;
	struct svm_node *x_space;
	prob.l=(int)fea.size();


	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,prob.l*(fea_num+1));
	int j=0;
	for(int i=0;i<prob.l;i++)
	{
		prob.x[i] = &x_space[j];
		prob.y[i] = label[i];
		for(int k=0;k<fea_num;k++)
		{
			x_space[j].index=k;
			x_space[j].value=fea[i][k];
			j++;
		}
		x_space[j++].index = -1;
	}

	if(!svm_check_parameter(&prob,&param))
	{
		model = svm_train(&prob,&param);
		svm_save_model(".\\data\\svm_model.txt",model);
		svm_destroy_model(model);
	}
	svm_destroy_param(&param);	
	free(prob.y);
	free(prob.x);
	free(x_space);
}
