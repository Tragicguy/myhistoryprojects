#pragma once
#include <afxmt.h>
#define			ALWAYSWAITTYPE     -2
#define			ONETIMEJUSTTYPE    -1
#define			ALWAYSWORKTYPE      0

class CThreadManager
{
public:
	CThreadManager(void);
	~CThreadManager(void);

	typedef void (*WorkFuncProc)(void * param);			//定义线程函数
	

	void StartThread();								//启动新线程
	void StopThread();								//结束线程
	void ThreadDoWork();							//使线程立即进入工作状态

	void Set(WorkFuncProc funcProc, void * lparameter, int cycleTime);		//设置线程函数地址、参数及执行周期
	void SetWorkFunction( WorkFuncProc funcProc );							//设置线程函数地址
	void SetWorkParameter( void * lparameter);								//设置线程函数参数
	void SetCycleTime( int cycleTime );										//设置线程执行周期
	
	void SuspendThread();	//挂起线程（设置m_CycleTime标志为 ALWAYSWAIT）
	void ResumeThread();	//恢复线程（设置m_CycleTime标志为之前设置的m_OldCycleTime,并调用ThreadDoWok函数触发）
	unsigned long GetThreadId();											//获取线程ID

protected:
	enum CycleTimeEnum{ ALWAYSWAIT = -2 , ONETIMEJUST = -1 , ALWAYSWORK = 0 };			//执行周期

	enum ThreadCmd{ NONE = 0 , STOP = 1 };

	static unsigned __stdcall WorkThread( void * lpParameter );	//线程工作函数

	void WorkThreadProc();

	ThreadCmd m_ThreadCmd;			//线程是否结束标志
	
	HANDLE   m_hThread;			//线程句柄	
	HANDLE   m_hEvent;			//事件句柄

	unsigned int m_ThreadId;		//线程ID
	
	int      m_CycleTime;			//执行周期
	int      m_OldCycleTime;		//旧的执行周期,用于还原
	void *   m_lpParameter;			//线程函数参数
	WorkFuncProc m_FunctionProc;		//线程函数地址

	CCriticalSection m_CycleTimeSection;		//周期设定锁
};	
