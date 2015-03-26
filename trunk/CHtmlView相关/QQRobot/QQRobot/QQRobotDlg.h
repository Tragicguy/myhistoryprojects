// QQRobotDlg.h : 头文件
//

#pragma once
#include "WebUserHtmlView.h"
#include <map>
using namespace std;

typedef struct _HInternetStr
{
//	HINTERNET hRequest;
	char* szContent;

}HInternetStr;
// CQQRobotDlg 对话框
class CQQRobotDlg : public CDialog
{
// 构造
public:
	CQQRobotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QQROBOT_DIALOG };

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
	CUserHtmlView m_wndHtmlView;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	void OnRecvContent(BufferStr &contentStr);
	void OnCloseHandle(HINTERNET hHandle);
	void OnInternetConnect(InternetConnectInfo &connInfo);
	void OnOpenRequesy(RequestStr &requestStr);
	void SendData();
	void ConvertUtf8ToGBK(CString& strUtf8);
	void ConvertGBKToUtf8(CString& strGBK);
	CString GetKeyString(CString &srcStr,CString strBefore,CString strAfter);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	map<HINTERNET,RequestStr> m_mapRequestList;
	std::map< HINTERNET, InternetConnectInfo>	m_mpConnection;
	CString m_strCookie;
};
