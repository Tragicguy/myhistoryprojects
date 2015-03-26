#pragma once

class CThreadBase 
{
public:
	CThreadBase();
	virtual ~CThreadBase();

public:
	bool StartWorking();      //开始执行
	HANDLE StartWorkingEx();
	void StopWorking(HANDLE threadHandle = NULL, DWORD dwWait = 3000);			//结束执行
	void MarkTheadToStop();   //暂停线程
	virtual void ThreadProc(); //OVERWRITE
	virtual void OnThreadStop(); 
	void SetRunning(bool bRunning = true);
	inline bool IsRunning(){ return m_bRunning; }

	//CString		m_DebugID;
protected:
	static	UINT WINAPI StartThread(LPVOID pParam);
	void ThreadLoop();

public:
	HANDLE	m_hThread;
	bool	m_bRunning;
public:
	UINT	m_uiThreadId;
protected:
};