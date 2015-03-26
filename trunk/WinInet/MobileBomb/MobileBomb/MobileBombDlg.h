// MobileBombDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <afxmt.h>
#include <map>
using namespace std;
#include "CtrlFile/CodeCheck.h"
class A{};
// CMobileBombDlg 对话框
class CMobileBombDlg : public CDialog
{
// 构造
public:
	CMobileBombDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MOBILEBOMB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	void InitCtrList();
public:
	CListCtrl m_PhoneList;
public:
	afx_msg void OnBnClickedBtnImport();
	static void RunThreadProc(LPVOID lpParam);		//回存线程函数
	void RunProc();			//回存线程执行函数
	bool m_bRunFlag;
	CWinThread* m_pRumThread[20];
	CArray<CString,CString> m_SendList;		//真正发送链表
	CCriticalSection m_SendLock;		//真正发送链表锁
public:
	afx_msg void OnBnClickedBtnStop();
	void SendMessageEx();
public:
	afx_msg void OnClose();
	int GetSendPack(CString &sendPack);
	void AddRealSendPack(CString &sendPack);
	void AddSendResult(CString strPhone,bool bSuc,CString strResult);
	void MobileMusicSend(CString strMobile);
	map<CString,int> m_mapPhoneList;
public:
	afx_msg void OnBnClickedButton1();
	void ConvertUtf8ToGBK(CString& strUtf8);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
