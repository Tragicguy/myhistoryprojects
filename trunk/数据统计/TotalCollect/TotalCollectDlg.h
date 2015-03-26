// TotalCollectDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <afxmt.h>
#include <afxtempl.h>
#include <map>
#include "ListCtrlColore.h"
using namespace std;
class CTotalCollectDlg : public CDialog
{
// 构造
public:
	CTotalCollectDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TOTALCOLLECT_DIALOG };

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
	CListCtrl m_list_total;
	CListCtrl m_list_detail;
	afx_msg void OnBnClickedBtnImport();
	void SetTableStyle();
	void SetTableStyleEx();
	afx_msg void OnBnClickedBtnExit();
	static void ThreadProc(LPVOID lpParam);
	void OperateProc();
	afx_msg void OnBnClickedBtnStop();
	void AddToResultList(CString strTail,CString strSrcPos);
	bool IsRightData(CString strData);
	afx_msg void OnBnClickedBtnAnanyze();
	afx_msg void OnNMDblclkListTotal(NMHDR *pNMHDR, LRESULT *pResult);
	int GetPower();
	afx_msg void OnBnClickedBtnShowunion();
	int GetCountTotal(CString strSrc);
	CListCtrl m_listResult;
	afx_msg void OnBnClickedBtnImport2();
	void SetTableStyleExEx();
	CString GetExcelDriver();
	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded);
	BOOL GetDefaultXlsFileName(CString& sExcelFile);
};
