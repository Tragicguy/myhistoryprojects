// CheckCodeCatchDlg.h : 头文件
//

#pragma once
#include <gdiplus.h>
#include "afxwin.h"
#include <shlwapi.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

// CCheckCodeCatchDlg 对话框
class CCheckCodeCatchDlg : public CDialog
{
// 构造
public:
	CCheckCodeCatchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHECKCODECATCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBegin();
	static void ThreadFunc(LPVOID lParam);
	void GetCheckCode();
	BOOL GetImageCLSID(const WCHAR* format, CLSID* pCLSID);
	BOOL TranstToBMP(CString srcPath,CString destPath);
	~CCheckCodeCatchDlg();
	CString m_strHeader;
	BOOL m_ExitFlag;
	int m_iGetCount;
	int m_iTotalCount;
	CString m_strDirect;

};
