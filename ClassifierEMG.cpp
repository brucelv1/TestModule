#include "ClassifierEMG.h"
#include <ctime>

static DWORD WINAPI MatlabThreadEntry(PVOID arg)
{		
	((CEMG *)arg)-> MatlabEng();
	return 0;
}

int* create_randperm(int length){//LB//产生length长度的随机数组

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
函数名称：构造函数
功    能：从文本中读取串口号、通道数等参数，并创建数据文本
参    数：无       
返回值  ：无
********************************************/
CEMG::CEMG()
{
	std::ifstream file_temp(".\\data\\config.txt"); //LB// 读inc/config.txt中的串口号及通道信息

	data_predict[0]=0;

	char buff[9];
	for(int i=0;i<3;i++)             //LB// COM		5  信号采集设备的串口号
		file_temp>>buff[0];
	file_temp>>com_number;           //LB// com_number=5


	for(int i=0;i<6;i++)             //LB// ACTION		13:	1 2 3 4 5 6 7 8 9 10 11 12 13
		file_temp>>buff[0];
	file_temp>>class_num;            //LB// class_num=13
	file_temp>>buff[0];
	for(int i=0;i<class_num;i++)
	{
		file_temp>>class_used[i];    //LB// 用到的动作数 class_used[0]=1 2 3 4 5 6 7 8 9 10 11 12 13
		class_used[i]--;             //LB// 减一 class_used[0]=0,......
	}
	for(int i=0;i<7;i++)             //LB// CHANNEL 8     :1 2 3 4 5 6 7 8
		file_temp>>buff[0];
	file_temp>>channel_num;          //LB// channel_num=8    
	file_temp>>buff[0];
	for(int i=0;i<channel_num;i++)   
	{
		file_temp>>channel_used[i];  //LB// 用到的通道数 channel_used[i]=1 2 3 4 5 6 7 8
		channel_used[i]--;           //LB// 减一 channel_used[i]=0 1 2 3 4 5 6 7
	}

	for(int i=0;i<13;i++)            //LB//除去ACTION_SERIAL 
		file_temp>>buff[0];
	file_temp>>action_num;          //LB//action_num=14    
	file_temp>>buff[0];				//LB//除去: 
	for(int i=0;i<action_num;i++)   
	{
		file_temp>>action_serial[i];  //LB// 用到的通道数 action_serial[i]=13 1 2 3 4 5 6 7 8
		action_serial[i]--;           //LB// 减一 action_serial[i]=12 0 1 2 3 4 5 6 7
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


	//文件记录
	sprintf_s(file_date,"2008.12.26_09.32"); //LB// 将file_data改成系统时间

	_strdate_s( buff, 9 ); //LB// 复制当前系统日期到缓冲区 _strdate_s( tmpbuf, 128 ); printf( "OS date:\t\t\t\t%s\n", tmpbuf ); OS date: 04/25/03  2003年4月25日
	file_date[2]=buff[6];
	file_date[3]=buff[7];
	file_date[5]=buff[0];
	file_date[6]=buff[1];
	file_date[8]=buff[3];
	file_date[9]=buff[4];
	_strtime_s( buff, 9 ); //LB// 写入时间到缓冲区      _strtime_s( tmpbuf, 128 );   printf( "OS time:\t\t\t\t%s\n", tmpbuf );    OS time: 13:51:23
	file_date[11]=buff[0];
	file_date[12]=buff[1];
	file_date[14]=buff[3];
	file_date[15]=buff[4]; //LB//将file_date改为系统当前时间

	sprintf_s(file_label,".\\data\\%s_label.txt",file_date);   //LB// data文件夹  “日期_label.txt”文件
	sprintf_s(file_fea,".\\data\\%s_fea.txt",file_date);       //LB// data文件夹  “日期_fea.txt”文件
	sprintf_s(file_src,".\\data\\%s_data.txt",file_date);      //LB// data文件夹  “日期_data.txt”文件
	outfile.open(file_src);                               //LB// 打开“data.txt”文件

	switch (CLASSIFIER)
	{
	case SVM:{
		sprintf_s(file_online_label,".\\data\\%s_online_label_SVM.txt",file_date);   //LB// data文件夹  “日期_oneline_label.txt”文件
		sprintf_s(file_online_set_label,".\\data\\%s_online_set_label_SVM.txt",file_date);   //LB// data文件夹  “日期_oneline_label.txt”文件
		break;
			 }
	case LDA:{
		sprintf_s(file_online_label,".\\data\\%s_online_label_LDA.txt",file_date);   //LB// data文件夹  “日期_oneline_label.txt”文件
		sprintf_s(file_online_set_label,".\\data\\%s_online_set_label_LDA.txt",file_date);   //LB// data文件夹  “日期_oneline_label.txt”文件
		break;
			 }
	}

	for(int i=0;i<DATA_LENGTH;i++)
		data_predict[i]=-1;

	if(IS_TRAIN)        //LB// 训练时不做任何动作
	{

	}
	else                //LB// 测试时，调用分类器模型
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

	m_hThread=CreateThread(NULL,0,MatlabThreadEntry,(PVOID)this,0,NULL);  //LB// MatlabThreadEntry表示新线程执行的线程函数地址  后一个0表示线程创建后立即调度

	state=REST;
	dur=-1;             //LB// REST状态下dur=-1
	memset(data,0,sizeof(data));          //LB// 将已经开辟内存空间data的sizeof(data)个字节的值设为0，对较大的结构体或数组进行清零的最快方法。
	memset(data_rms,0,sizeof(data_rms));
	memset(data_rms_max,1,sizeof(data_rms_max));
	rms_index=-1;
	Is_Correct_Num = 0;

}
/*******************************************
函数名称：析构函数
功    能：关闭文件、释放内存
参    数：无       
返回值  ：无
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
	if(online_label.size()>0)   //LB//在线测试部分   
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

	if(IS_TRAIN)         //LB// 训练的时候给特征文件夹“fea.txt”增加换行，并调用train()函数
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
函数名称：Continue
功    能：将程序状态由REST转变为TRAIN，按下空格键时调用//LB//由CVR.h中触发
参    数：无       
返回值  ：无
********************************************/
void CEMG::Continue()
{
	if(state==REST&&mat_index==data_index)
	{
		memset(data,0,sizeof(data));
		dur=-2000;         //LB// 程序状态由REST转变为START后，dur=-2000
		state=START;
		if(IS_TRAIN)
		{
			//八个类别的标签
			/*for(int i=0;i<DATA_LENGTH/5000;i++)
			{
			static int j0=0;

			for(int j=0;j<5000;j++)
			data_predict[j+i*5000]=class_used[j0%class_num];
			j0++;
			}*/
			//十二个动作，5s休息标签
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
			/*NIR40个动作范式
			for(int i=0;i<DATA_LENGTH/5000;i+=3)    //LB// data_predict初始化 前5000个数为13，中间5000个数为13，后5000个数为0;5000*13 5000*13 5000*1；……
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
			random_figure=class_num-1;//LB//random_figure初始值赋为REST
		}
	}
}
/*******************************************
函数名称：SetData
功    能：将数据写入到EMG的data成员中（去掉前2000个数据），并输出到文本
参    数：要写入的数据指针       
返回值  ：无
********************************************/
void CEMG::SetData(double *p)
{
	if(dur>0)   //LB// 在去掉2000个数据后dur=DATA_LENGTH
	{
		dur--;
		int index_temp=(data_index+1)%DATA_LENGTH;   //LB// data_index=-1
		for (int j=0;j<DATA_CHANNEL;j++)
		{
			data[index_temp][j]=*p;       //LB// 将EMG&NIR信号存储到data中，前4位为EMG信号，后4位为NIR信号
			p++;
		}			
		for (int j=0;j<channel_num;j++)
			outfile<<data[index_temp][channel_used[j]]<<'\t';   //LB// outfile为file_src即“data.txt”写入数据
		outfile<<'\n';
		data_index++;        //LB// “data.txt”中写入数据的行数
		if(IS_TRAIN&&data_index%(1000*ACTION_TIME_TRAINING)==0)
		{
			//PlaySound("inc/3.wav",NULL, SND_ASYNC); //LB//(1000*ACTION_TIME_TRAINING)个数据有提示音
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
			else        //如果未采集完成需要的Trial数目，则将data与控制变量dur重新初始化 
			{
				memset(data,0,sizeof(data));
				dur=DATA_LENGTH;
				//此处将按下空格的处理函数continue的代码放在此处，实现数据的长时间采集。将dur修改为DATA_LENGTH，解决了近红外采集数据不连续的Bug
				//state=START; 
			}
		}

	}
	if(dur<0)
	{
		dur++;     //LB// 变为训练状态TRAIN后，dur=-2000，即去掉前2000个数据
		if(dur%1000==-999)					
			;//PlaySound("inc/2.wav",NULL, SND_ASYNC);//LB//每1s钟提示一下
		if(dur==0&&state==START)
			dur=DATA_LENGTH;    //LB// 2000个数据后，dur变成DATA_LENGHTH，即200s
	}
}


/*******************************************
函数名称：MatlabEng
功    能：处理数据，判断目前实验处于哪一步骤，并给出相应处理
参    数：无     
返回值  ：无
********************************************/
void CEMG::MatlabEng()
{
	mat_over=false;
	while(!mat_over)
	{
		if(data_index>mat_index)    //LB// “data.txt”写入数据后，data_index为原始写入数据的行数，mat_index为数据行索引（初始值为-1）。
		{		
			mat_index++;
			if(mat_index%100==0)        //LB// mat_index=100,200,300……
			{
				rms_index++;                           //LB// 每100行数据，rms_index加一（初始值为-1）
				for(int i=0;i<channel_num;i++)
					data_rms[i][rms_index%50]=0;       //LB// data_rms为channel_num行50列的数组，赋用到的列初值为0
				for(int j=0;j<50;j++)
				{
					int j0=(mat_index+DATA_LENGTH-j)%DATA_LENGTH;
					for(int i=0;i<channel_num;i++)
						data_rms[i][rms_index%50]+=data[j0][channel_used[i]]*data[j0][channel_used[i]];//LB//前50行各个通道数据的平方和
				}
				for(int i=0;i<channel_num;i++)
				{
					data_rms[i][rms_index%50]=sqrt(data_rms[i][rms_index%50]);
					if(data_rms[i][rms_index%50]>data_rms_max[i])
						data_rms_max[i]=data_rms[i][rms_index%50];
					data_rms[i][rms_index%50]=(data_rms[i][rms_index%50]/data_rms_max[i]); //LB// data_rms是用来画能量条用的
				}
			}

			if(IS_TRAIN && mat_index%TIME_INCEMENT==0 && mat_index%(1000*ACTION_TIME_TRAINING)>=TIME_INTERVAL)//LB// 训练状态下 每(1000*ACTION_TIME_TRAINING)个数据中的后2000个 每100个数据做一次特征提取
			{
				feature_extract(); //LB// mat_index=3000，3100，……
			}
			if(!IS_TRAIN)//LB//包含OnLineTest
			{
				if(mat_index>=TIME_INTERVAL && mat_index%TIME_INCEMENT==0)//LB// 测试状态下 每100个数据做一次特征提取(即0.1秒提取一个特征)。
				{
					feature_extract();//LB// TD依据svm判断出结果data_predict     mat_index=300，400，……
				}
				else
					data_predict[mat_index%DATA_LENGTH]=data_predict[(mat_index-1)%DATA_LENGTH];//LB// 测试状态下 每100个数据都一样(即0.1秒提取一个特征)
			}
		}
		else
			Sleep(1);    //LB// mat_index数据行索引比data_index数据行数大的话，延迟一下。
	}
}
/*******************************************
函数名称：feature_extract
功    能：提取特征：TD、DFT、AR，并根据SVM给出判断结果data_predict
参    数：无     
返回值  ：无
********************************************/
void CEMG::feature_extract()       //LB// 训练状态下mat_index=3000，3100，……；测试状态下mat_index=300，400，……
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
				int j1=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-1)%DATA_LENGTH;  //LB// ???问题??? mat_index=300时怎么办 答：后有j>0 j>1条件
				int j2=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-2)%DATA_LENGTH;

				fea_temp[i*4]+=abs(data[j0][channel_index]);   //LB// fea_temp[0 4 8……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据绝对值的和
				if(j>0)
				{
					fea_temp[i*4+1]+=int(data[j0][channel_index]*data[j1][channel_index]>0);//LB// fea_temp[1 5 9……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据相邻两个相乘是否大于0的状态的总和
					fea_temp[i*4+2]+=abs(data[j0][channel_index]-data[j1][channel_index]);//LB// fea_temp[2 6 10……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据相邻两个之差的绝对值的总和
				}
				if(j>1)					
					fea_temp[i*4+3]+=int((data[j0][channel_index]-data[j1][channel_index])*(data[j1][channel_index]-data[j2][channel_index])>0);//LB// fea_temp[3 8 11……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据相邻两个数据之差的乘积是否大于0的状态的总和
			}
		}
		for(int i=4;i<channel_num;i++)//LB// channel_num=8 here
		{
			channel_index=channel_used[i];
			fea_temp[4*4+(i-4)*2]=0;fea_temp[4*4+(i-4)*2+1]=0;			
			for(int j=0;j<TIME_INTERVAL;j++)
			{
				int j0=(mat_index+DATA_LENGTH-TIME_INTERVAL+j)%DATA_LENGTH;   
				int j1=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-1)%DATA_LENGTH;  //LB// ???问题??? mat_index=300时怎么办 答：后有j>0 j>1条件
				//int j2=(mat_index+DATA_LENGTH-TIME_INTERVAL+j-2)%DATA_LENGTH;

				fea_temp[4*4+(i-4)*2]+=data[j0][channel_index];   //LB// fea_temp[0 4 8……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据绝对值的和
				if(j>0)
				{
					fea_temp[4*4+(i-4)*2+1]+=abs(data[j0][channel_index]-data[j1][channel_index]);//LB// fea_temp[2 6 10……]位上分别是0 1 2通道上前300(TIME_INTERVAL)个数据相邻两个之差的绝对值的总和
				}

			}
			fea_temp[4*4+(i-4)*2]=fea_temp[4*4+(i-4)*2]/TIME_INTERVAL;
		}
	}

	// 这里有其他的分类器策略，暂时放到资源文件夹中

	//finish = clock();
	//double totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
	//double ttime = 100;
	if(IS_TRAIN)        //LB// 训练的时候将特征和预测写入"fea.txt"和"label.txt"文件
	{
		fea.push_back(fea_temp); //LB// 将fea_temp加入fea的尾部
		label.push_back(data_predict[(mat_index-1)%DATA_LENGTH]);//LB// 将data_predict加入label的尾部   为什么减50？
	}
	else                
	{
		//LB// 测试时根据SVM给出判断结果
		if(CLASSIFIER==SVM)
		{
			//clock_t start;
			//clock_t finish;
			//start = clock();
			//根据SVM给出判断结果
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
		}//LB// 定义double data_predict[DATA_LENGTH]是每行数据的模式状态

		if(IS_OnLineTest)
		{
			random_figure=class_num-1;//LB//一般状态下random_figure为REST
			//if((mat_index/1000)%ACTION_TIME_ONLINE_TESTING==0)
			//{
			//	Is_Correct_Num = 0;
			//	Is_Action = false;
			//}
			if((mat_index/1000)%ACTION_TIME_ONLINE_TESTING>=REST_TIME_ONLINE_TESTING)//LB//在线测试状态下，前3s为REST，后6s为随机动作
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
			online_label.push_back(data_predict[mat_index%DATA_LENGTH]);//LB// 将data_predict加入label的尾部   为什么减50？改回
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
