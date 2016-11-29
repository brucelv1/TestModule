/*************************************************
Copyright:
Author:
Date:2013-08-17
Description:定义了CFilter、SJTArmBand类，完成从串口读数据、滤波功能
**************************************************/
#pragma once
#include <Windows.h>
#include <vector>

// EMG臂带接收数据线程，打开串口、发送数据、关闭串口的动作。
static DWORD WINAPI DataThreadEntry_EMG(PVOID arg);

enum STATE {REST=1,START,END};

/*******************************************
类名称：CFilter
功    能：定义了三个滤波器：高通、陷波、梳状滤波器（但仅仅使用了梳状滤波器），定义了滤波函数
********************************************/
class  CFilter
{
public:
	enum FilterType {HIGH_PASS=1,NOTCH,COMB};
	int					filter_n;
	double				data_src[100][8];
	double				data_filter[100][8];
	double				filter_a[100];
	double				filter_b[100];
	int					filter_index;

	CFilter(FilterType type=COMB);
	
/*******************************************
函数名称：DoFilter
功    能：滤波函数，滤波函数算法：y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(nb+1)*x(n-nb)- a(2)*y(n-1) - ... - a(na+1)*y(n-na)//LB//一维数字滤波器，同Matlab filter
参    数：无       
返回值  ：无
********************************************/
	double DoFilter(double temp);
	
};

/*******************************************
类名称：SJTArmBand
功    能：打开串口，读取串口传输过来的数据、滤波
********************************************/
class  SJTArmBand
{
public:
	enum DeviceType {EMG_NIR=1,EMG};
	int					thread_state;
	bool				mSerialSuccessful;

private:
	DeviceType          mDevice;
	HANDLE				hCom;
	HANDLE				m_hThread;
	
	unsigned char		buff[8192];         //LB//8192=2^13    unsigned char 0~255 2^8
	const int           DATA_CHANNEL;      
	double*				data_buff;          //通道数据缓冲区,在构造函数中初始化
	int					data_cnt_state;		//数据状态
	int					data_cnt_count;		//数据计数
	int					data_cnt_max;		//数据计数
	int					data_cnt_check;		//数据校验
	std::vector<double> _mDataReadyToUse;

	CFilter				mFilter;

	CRITICAL_SECTION    _mCriticalSection;

	// Serial port
	int nCOM;
	int Baudrate;

public:
	SJTArmBand(int nChannel=8, DeviceType dt=EMG);	
	~SJTArmBand();

	// 串口操作
	//void OpenSerial(int nCOM=5, int Baudrate=57600);
	void OpenSerial();
	void CloseSerial();
	
	bool Connect(int _nCOM=5, int _BaudRate=57600);
	bool IsConnected()
	{
		return mSerialSuccessful;
	}
	
	/*******************************************
	函数名称：SendData_EMG
	功    能：读取EMG臂带串口数据，滤波、校验、存储数据
	参    数：无       
	返回值  ：无
	********************************************/	
	bool SendData_EMG();

	std::vector<double> GetDataVector(); 
};

