// zhaochaDlg.h : 头文件
//

#pragma once
#include "BtnST.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "afxwin.h"
// CzhaochaDlg 对话框
#define  WM_MY_MESSAGE WM_USER + 33
class CzhaochaDlg : public CDialog
{

	struct myParam
	{
		IplImage* img;
		int x;
		int y;
		HWND hwnd;

	};
// 构造
public:
	CzhaochaDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ZHAOCHA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	int showFlag;
	CButtonST	m_btn;
	CButtonST	m_btnshow;
	CRect oltBtnRect;

	HHOOK m_lowHook;			//键盘HOOK

	// 生成的消息映射函数
	BOOL PreTranslateMessage( MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT ButtonMove(WPARAM wParam,LPARAM lParam);
	//键盘消息处理
	afx_msg LRESULT GoToGetDiffrent(WPARAM wParam,LPARAM lParam);		//找茬
	afx_msg LRESULT CloseFindWindow(WPARAM wParam,LPARAM lParam);		//找茬

	void GetSelectWindowPic(HWND hwnd);
	bool SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength);
	void GetLocation(IplImage* Img,int &x1,int &x2,int &x3,int tempY);
	static void on_mouseProc( int event, int x, int y, int flags, void* param);
	
	bool IsInRect(CPoint point,CRect &rect);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFind();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStnClickedShow();
	afx_msg void OnBnClickedBtnShowwawa();
	afx_msg void OnClose();
};
