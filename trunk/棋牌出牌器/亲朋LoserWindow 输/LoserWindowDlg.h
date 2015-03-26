// LoserWindowDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ListCtrlColore.h"
#include "afxwin.h"
#define	WM_KILLTHREAD	WM_USER+547
#include "MyImage.h"
#include <map>
using namespace std;
#include "ProcessManager.h"
typedef struct _RegisterPack
{
	CString strAcc;		//接收日期
	CString strPwd;		//接收时间
	CString strWinnerName;	//信息编号
	int iWinnerDesk;
	CString strBalance;
	_RegisterPack()
	{
		strAcc.Empty();
		strPwd.Empty();
		strWinnerName.Empty();
		iWinnerDesk =0;
		strBalance.Empty();
	}
}RegisterPack;
struct HookStruct
{
	HHOOK hKeyBoardHook;
	HHOOK hLowKeyBoardHook;
};
// CLoserWindowDlg 对话框
class CLoserWindowDlg : public CDialog
{
// 构造
public:
	CLoserWindowDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LOSERWINDOW_DIALOG };

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
	static void ThreadFunc(LPVOID lParam);
	void RunFunc();
	void ShowMessage(CString strMsg);
	int Login(CString strUser,CString strPwd);
	afx_msg void OnBnClickedButton2();

	bool CopyStringToClipboard(HWND hWnd, LPCTSTR lpszText);

	int TryIntoRoom();
	int TryIntoDesk(int iIndex,int iSetIndex);
	CString GetItemNodeText(HWND hwnd,HTREEITEM hItem);
	HTREEITEM GetSingleItem(HWND hwnd,HTREEITEM hItem,CString strItemText);
	bool GetTreeItemRect(HWND TreeView, HTREEITEM TreeItem, RECT& Rect);
	afx_msg void OnBnClickedButton1();
	int DblClickItemNode(HWND hwnd,HTREEITEM hItem);
	afx_msg void OnBnClickedButton3();
	void WriteLog(CString srcStr,int iFlag);
	afx_msg void OnBnClickedButton4();
	int SetPassWord(CString strPwd);
	CPoint DestIndexToPoint(int iIndex);
	afx_msg void OnBnClickedButton5();
	int ComputeScrollUpValue(int iIndex);
	CPoint DestIndexToPointEx(int iIndex,int iSetIndex);
	void MoveScrollBar(int iIndex);
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	void SetTableStyle();
	void InitListCtrl();
	LRESULT AddToList(WPARAM wParam,LPARAM lParam);
	CListCtrl m_ListAcc;
	CListCtrlColore m_message;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnInport();
	void AddToAccList(CString strAcc,CString strPwd);
	int GetRegisterPack(RegisterPack &submitPack);
	afx_msg void OnBnClickedRadioLose();
	afx_msg void OnBnClickedRadioWin();
	int SaveParams();
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnBnClickedBtnSave();
	void ClosePlayWindow();
	void BackToLoginStatus();
	CString GetListItemText(HWND hwnd,int iItem,int iSubItem);
	int GetBalance(HWND hwnd);
	CString GetPreUsers(HWND hwnd,int iCount);
	int GetInitBtnInfo(HWND hwndTopParant);
	afx_msg void OnBnClickedButton8();
	bool IsGmHere(HWND hwnd);
	afx_msg void OnBnClickedButton9();
	int PlayGame();
	void UnloadHook();
	HookStruct m_KeyHook;
	afx_msg void OnClose();
	void SetHook();
	afx_msg LRESULT StopThread(WPARAM wParam,LPARAM lParam);
	int DblClickItemNodeEx(HWND hwnd);
	HTREEITEM FindItem(HWND hwnd,HTREEITEM hItem,CString strText);
	CSliderCtrl m_SpeedCtrl;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckGmcheck();
	CButton m_CheckGM;
	CButton m_HideWindow;
	afx_msg void OnBnClickedCheckHidewindow();
	void AddFreshResult(RegisterPack rgPack,int iFlag);
	void ShutUpWindow();
	int GetPower();
	void SetLastWaitCheckTime();
	bool IsOverCheckTime(int iIntervalSeconds);
	long m_iCurrTime;
	afx_msg void OnBnClickedCheckGmcheckwait();
	void SetRemoteWindowText(HWND hwnd, CString strText);
	HTREEITEM FindItemEx(HWND hwnd,HTREEITEM hItem,CString strText);
	int InitPlayData();
	int IsBtnEnable(CString strSource,bool &bPassEnable,bool &bGiveUpEnable);
	void ClickBtnPass();
	void ClickBtnGiveUp();
	void CloseCheckWindow();
	void RestartGame();
	void CloseWarnWindow();
	void WriteOldAcc(RegisterPack &rgPack,int iFlag);
	CButton m_gmCheckWait;
	
	CMyImage m_myImg;

	void LoadOldAcc();
	void StartGame(HWND hwnd);
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool IsPlayerOut();
	int GetBalanceEx(HWND hwnd);
	map<CString,int> m_mapOldAcc;
};