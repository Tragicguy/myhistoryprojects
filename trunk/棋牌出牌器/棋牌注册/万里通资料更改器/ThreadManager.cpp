#include "StdAfx.h"
#include <process.h>
#include ".\threadmanager.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
//构造函数,初始化线程标识
CThreadManager::CThreadManager(void)
{
        m_ThreadCmd = NONE;										//线程是否结束标识

        m_hThread = NULL;										//线程句柄
        m_hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		//创建事件对象

        m_ThreadId = 0;							//线程ID

        m_CycleTime = ONETIMEJUST;				//一次执行线程
        m_FunctionProc = NULL;					//函数地址为空
        m_lpParameter = NULL;					//线程函数参数为空
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//析构,如果线程未结束，结束之
CThreadManager::~CThreadManager(void)
{
        StopThread();
        CloseHandle(m_hEvent);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//创建一个新线程
void CThreadManager::StartThread()
{
        if ( m_ThreadId !=0 )		//如果线程已经建立，直接返回
        {
                return;
        }
        m_ThreadCmd = NONE;			//先设置线程结束标志为不结束
        m_hThread = (HANDLE)_beginthreadex(NULL,0, WorkThread, (void *)this, 0, &m_ThreadId);	//创建线程

        TRACE("创建线程%d\r\n",m_ThreadId);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//结束线程
void CThreadManager::StopThread()
{
        if ( m_ThreadId == 0 )		//如果线程未创建，直接返回
        {
                return;
        }
        m_ThreadCmd = STOP;			//设置线程结束标志为结束

        ThreadDoWork();				//触发线程执行
        WaitForSingleObject(m_hThread, 3000);	//等待线程结束
        if ( m_ThreadId != 0)
        {
                TRACE("未等到线程自动结束,强制结束它\r\n");                   
		m_ThreadCmd = NONE;
		m_ThreadId = 0;
		::TerminateThread(m_hThread,STILL_ACTIVE);
		m_hThread = NULL;
        }
	m_ThreadCmd = NONE;
	m_ThreadId = 0;
	m_hThread = NULL;

}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//使线程立即进入工作状态
void CThreadManager::ThreadDoWork()
{
        if ( m_hThread == 0 )
        {
                StartThread();
                SetEvent(m_hEvent);
        }
        else
        {
                SetEvent(m_hEvent);
        }
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//挂起线程
void CThreadManager::SuspendThread()
{
        if ( m_ThreadId == 0 )
        {
                return;
        }
        TRACE("挂起线程%d\r\n",m_ThreadId);
        ::SuspendThread(m_hThread);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//恢复线程
void CThreadManager::ResumeThread()
{
        if ( m_ThreadId == 0 )
        {
                return;
        }
        TRACE("恢复线程%d\r\n",m_ThreadId);
        ::ResumeThread(m_hThread);
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程执行过程
unsigned CThreadManager::WorkThread( void * lpParameter )
{
        CThreadManager * pThis = (CThreadManager*) lpParameter;
        if ( pThis == NULL )
        {
                return -1;		//参数无效
        }
        pThis->WorkThreadProc();
        return 0;			//退出
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//线程执行过程
void CThreadManager::WorkThreadProc()
{
        while( true )		//循环执行
        {
                if ( m_ThreadCmd == STOP )				//如果线程即将结束
                {
                        TRACE("线程%d结束\r\n",m_ThreadId);
                        m_ThreadCmd = NONE;
                        m_ThreadId = 0;
                        m_hThread = NULL;
                        return;			//线程终止
                }
                if ( m_FunctionProc != NULL )			//执行真正的线程过程
                {
                        try
                        {          
                                ( *m_FunctionProc )( m_lpParameter );
                        }
                        catch ( ... )
                        {
                                //.......异常
                        }
                }

                int  cycleFlag = m_CycleTime;

                if ( cycleFlag == ONETIMEJUST )			//如果线程仅为一次性执行线程
                {
                        m_ThreadCmd = NONE;
                        m_ThreadId = 0;
                        m_hThread = NULL;
                        return;			//正常退出
                }

                if ( cycleFlag > ALWAYSWORK )		//如果工作标志为定时执行
                {
                        WaitForSingleObject( m_hEvent, m_CycleTime );
                }
                if ( cycleFlag <= ALWAYSWAIT )		//如果工作标志为执行一次后等待
                {
                        WaitForSingleObject( m_hEvent , INFINITE );
                }

        }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置线程函数地址、参数及执行周期
void CThreadManager::Set( WorkFuncProc funcProc, void * lparameter, int cycleTime )
{
        m_FunctionProc  = funcProc;
        m_lpParameter	= lparameter;
        m_CycleTime = cycleTime;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置线程函数地址
void CThreadManager::SetWorkFunction( WorkFuncProc funcProc )
{
        m_FunctionProc = funcProc;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置线程函数参数
void CThreadManager::SetWorkParameter( void * lparameter)
{
        m_lpParameter = lparameter;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//设置周期
void CThreadManager::SetCycleTime( int cycleTime )
{
        m_CycleTime = cycleTime;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
//获取线程ID
unsigned long CThreadManager::GetThreadId()
{
        return m_ThreadId;
}