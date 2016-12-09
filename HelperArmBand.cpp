#include "HelperArmBand.h"
#include <iostream>

/*******************************************
静态全局函数: BEGIN
********************************************/
// 默认使用的数据更新线程
static DWORD WINAPI DataThreadEntry_EMG(PVOID arg) 
{
	((SJTArmBand *)arg)->OpenSerial();
	while(((SJTArmBand *)arg)->thread_state!=END)
	{
		if(((SJTArmBand *)arg)->mSerialSuccessful)
			((SJTArmBand *)arg)->SendData_EMG();
		else
			((SJTArmBand *)arg)->thread_state=END;
	}
	((SJTArmBand *)arg)->CloseSerial();
	return 0;
}
/*******************************************
静态全局函数: END
********************************************/

/*******************************************
CFilter: BEGIN
********************************************/
CFilter::CFilter(FilterType type/*=COMB*/)
{
	filter_index=-1;
	for(int i=0;i<100;i++)
	{
		for(int j=0;j<8;j++)
		{
			data_src[i][j]=0;
			data_filter[i][j]=0;
		}
	}
	for(int i=0;i<100;i++)
	{
		filter_a[i]=0;
		filter_b[i]=0;
	}


	switch(type)
	{
	case HIGH_PASS:
		filter_n=4;
		filter_a[0]=1.000000000000000;
		filter_a[1]= -3.917907865391987;
		filter_a[2]=5.757076379118066;
		filter_a[3]=-3.760349507694526;
		filter_a[4]=0.921181929191236;

		filter_b[0]=0.959782230087239;
		filter_b[1]= -3.839128920348954;
		filter_b[2]= 5.758693380523431;
		filter_b[3]=-3.839128920348954;
		filter_b[4]=0.959782230087239;
		break;
	case NOTCH:
		//Fs = 1000;      % Sampling Frequency
		//Fnotch = 50;    % Notch Frequency
		//BW     = 2;     % Bandwidth
		//Apass  = 20;    % Bandwidth Attenuation
		//[filter_b, filter_a] = iirnotch(Fnotch/(Fs/2), BW/(Fs/2), Apass);
		filter_n=2;
		filter_a[0]=1.000000000000000;
		filter_a[1]=-1.790194162384630;
		filter_a[2]=0.882321535799305;
		filter_b[0]=0.941160767899653;
		filter_b[1]=-1.790194162384630;
		filter_b[2]=0.941160767899653;
		break;
	case COMB:
		//filt_n=20;
		//fs = 1000; fo = 50;  q = 35; bw = (fo/(fs/2))/q;
		//[filter_b, filter_a]= iircomb(fs/fo,bw,'notch');
		filter_n=20;
		filter_a[0]=1.000000000000000;
		filter_a[20]=-0.809784033195007;
		filter_b[0]=0.904892016597504;
		filter_b[20]=-0.904892016597504;
		break;
	}		
}

double CFilter::DoFilter(double temp)
{
	filter_index=(filter_index+1)%8;    // filter_index=-1; initialized in ctor
	for(int i=0;i<filter_n;i++)         // filter_n=20; initialized in ctor
	{
		data_src[filter_n-i][filter_index]=data_src[filter_n-i-1][filter_index];
		data_filter[filter_n-i][filter_index]=data_filter[filter_n-i-1][filter_index];
	}
	data_src[0][filter_index]=temp;
	data_filter[0][filter_index]=0;
	for(int i=0;i<filter_n+1;i++)
		data_filter[0][filter_index]+=filter_b[i]*data_src[i][filter_index];
	for(int i=1;i<filter_n+1;i++)
		data_filter[0][filter_index]-=filter_a[i]*data_filter[i][filter_index];
	return data_filter[0][filter_index];
}
/*******************************************
CFilter: END
********************************************/


/*******************************************
SJTArmBand: BEGIN
********************************************/
SJTArmBand::SJTArmBand(int nChannel/*=8*/, DeviceType dt/*=EMG*/)
	: DATA_CHANNEL(nChannel)
	, mDevice(dt)
	, data_buff(nullptr)
{
	// 数据格式：8通道，前4个是EMG信号，后4个是NIR信号
	data_buff = new double[DATA_CHANNEL]();

	// 串口数据解析相关：数据状态
	data_cnt_state = 0;

	// 初始化供外部访问的数据容器
	_mDataReadyToUse.resize(DATA_CHANNEL,0);

	// 初始化临界区结构体
	InitializeCriticalSection(&_mCriticalSection);
}

SJTArmBand::~SJTArmBand()
{
	delete [] data_buff;
	data_buff = nullptr;
	switch (mDevice)
	{
		case EMG_NIR:
			thread_state=END;
			Sleep(100);
			break;

		case EMG:
			thread_state=END;
			Sleep(100);
			break;
	}
}

//void SJTArmBand::OpenSerial(int nCOM/*=5*/, int Baudrate/*=57600*/)
void SJTArmBand::OpenSerial()
{
	mSerialSuccessful=true;
	char	tmpCOM[100];
	sprintf_s(tmpCOM,"\\\\.\\COM%d",nCOM); // tmpCOM="\\.\COM5"

	// 把char* 转换为LPWSTR
	wchar_t wtext[100];
	std::mbstowcs(wtext, tmpCOM, strlen(tmpCOM)+1);//Plus null
	LPWSTR COM_NUM = wtext;

	//做5次打开串口的尝试
	for(int i=0;i<5;i++)
	{
		hCom =::CreateFile(COM_NUM,
			GENERIC_READ|GENERIC_WRITE, 
			0,
			0,
			OPEN_EXISTING,
			0,//FILE_FLAG_OVERLAPPED,
			0
			);
		if(hCom ==INVALID_HANDLE_VALUE)
		{
			::CloseHandle(hCom);
			mSerialSuccessful=false;
		}
		else
		{
			mSerialSuccessful=true;
			break;
		}
	}
	if(mSerialSuccessful&&!::SetupComm(hCom, 32000, 32000))
	{
		::CloseHandle(hCom);
		mSerialSuccessful=false;
	}

	COMMTIMEOUTS CommTimeOuts;// set up for overlapped I/O
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 0;		
	if(mSerialSuccessful&&!::SetCommTimeouts(hCom, &CommTimeOuts))
	{
		::CloseHandle(hCom);
		mSerialSuccessful=false;
	}
	if(mSerialSuccessful&&!::SetCommMask(hCom, 0))
	{
		::CloseHandle(hCom);
		mSerialSuccessful=false;
	}

	DCB dcb={0};
	if(mSerialSuccessful)
	{
		dcb.DCBlength = sizeof(DCB);
		if(!::GetCommState(hCom,&dcb))
			mSerialSuccessful=false;
	}
	if(mSerialSuccessful)
	{
		dcb.BaudRate = Baudrate;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits =ONESTOPBIT;
		if(!::SetCommState(hCom,&dcb))
			mSerialSuccessful=false;
	}
	if( mSerialSuccessful&&!::PurgeComm( hCom, PURGE_TXABORT|PURGE_RXABORT| PURGE_TXCLEAR|PURGE_RXCLEAR ) )
		mSerialSuccessful=false;
}

void SJTArmBand::CloseSerial()
{
	if(mSerialSuccessful)
	{
		::PurgeComm(hCom, PURGE_TXABORT|PURGE_RXABORT| PURGE_TXCLEAR|PURGE_RXCLEAR);
		::CloseHandle(hCom);
		mSerialSuccessful=false;
	}
}

bool SJTArmBand::Connect(int _nCOM/*=5*/, int _BaudRate/*=57600*/)
{
	this->nCOM = _nCOM;
	this->Baudrate = _BaudRate;

	switch (mDevice)
	{
	case EMG_NIR:
		thread_state=REST;
		//m_hThread=CreateThread(NULL,0,DataThreadEntry,(PVOID)this,0,NULL);
		break;

	case EMG:
		thread_state=REST;
		m_hThread=CreateThread(NULL,0,DataThreadEntry_EMG,(PVOID)this,0,NULL);
		break;

	default:
		std::cout << "Unknown DeviceType!" << std::endl;
		return false;
	}

	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		std::cout << "Device Connect Failed!" << std::endl;
		return false;
	}
	else
		return true;
}

bool SJTArmBand::SendData_EMG()
{
	DWORD BytesRead;                               // the number of bytes that have been read
	::ReadFile(hCom,buff,3000,&BytesRead,NULL);    //LB//从文件指针指向的位置开始将数据读出到一个文件中,buff为保存读入数据的缓冲区。3000要读入的字节数。
	for(DWORD index=0; index<BytesRead; index++)       
	{
		switch(data_cnt_state)              //LB//数据状态
		{
		case 0:
			if(buff[index]==0x55)           //LB//许多串口通讯中测试或握手信号使用AA或55这两个特殊的十六进制数
				data_cnt_state++;           
			break;
		case 1:
			if(buff[index]==0xAA)           //LB//AA展开为10101010,55展开为01010101,变成串行电平的话就是一个占空比为50%的方波
			{                               //LB//这种方波在电路中最容易被分辨是否受干扰或者畸变，在实际波形的观察中也最容易看出毛病所在
				data_cnt_state++;
				data_cnt_count=0;           //LB//数据计数
				data_cnt_max=16;
				data_cnt_check=0;           //LB//数据校验
			}
			else
				data_cnt_state=0;
			break;
		case 2:
			data_cnt_check+=buff[index];
			if(data_cnt_count%2==0)//每两个数据存储一次（一个数据分高低两字节传输）
				data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]=double(buff[index]*256);//LB//高八位存储,乘256复原    %与/同级，从左往右
			else
			{
				data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]+=double(buff[index]); //LB//低八位存储，与高八位的数据相加
				if(data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]>=65534/2)   //LB//2^16=65536  
					data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]-=65535;
				data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]=mFilter.DoFilter(data_buff[data_cnt_count%(DATA_CHANNEL*2)/2]);
			}
			data_cnt_count++;
			if(data_cnt_count==data_cnt_max)
				data_cnt_state++;
			break;
		case 3:
			if(data_cnt_check%256==buff[index])    //校验正确，则执行既定逻辑；否则将数据丢弃
			{
				EnterCriticalSection(&_mCriticalSection);
				for (int i=0; i<DATA_CHANNEL; i++)
				{
					_mDataReadyToUse[i] = data_buff[i];
				}
				LeaveCriticalSection(&_mCriticalSection);
			}
			data_cnt_state=0;
			break;
		}
	}
	Sleep(1);//LB//"目的不是要达到1个毫秒，只不过是想让CPU有时间去对其它进程进行调度而已，即不要让CPU在这个时间段内占用到100%"
	return false;
}

std::vector<double> SJTArmBand::GetDataVector()
{
	return _mDataReadyToUse;
}

/*******************************************
SJTArmBand: END
********************************************/
