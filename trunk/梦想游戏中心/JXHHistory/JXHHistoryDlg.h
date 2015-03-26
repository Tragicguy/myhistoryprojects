// JXHHistoryDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "WindowFind.h"
#include "MyImage.h"
// CJXHHistoryDlg 对话框
class CJXHHistoryDlg : public CDialog
{
// 构造
public:
	CJXHHistoryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_JXHHISTORY_DIALOG };

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

	static void ThreadFunc(LPVOID lParam);
	void GetFunc();
	static void ThreadQueryFunc(LPVOID lParam);
	void QueryFunc();
	void SetTableStyle();
	void InitListCtrl();
public:
	
	afx_msg void OnBnClickedBtnBegin();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnQuery();
	afx_msg void OnBnClickedCheckAutoquery();
	bool SaveHistory();
	void GetRecordPic();
	CString GetCurrRecord();
	bool IsNeedToGetRecord();
	void DeleteFile(CString strPath);
	void SplitPic();
	void SaveSubRectPic(IplImage *srcImg,CString strPath,int x,int y,int iWidth,int iHeight);
	CWindowFind m_myWind;
	CMyImage m_myImg;
	afx_msg void OnClose();
	bool IsSelectResult(int iFlag);
	afx_msg void OnBnClickedButton1();
	void SplitPicEx();
	void Delay(int iMiniSecond,BOOL *bExit);
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnSave();
	void InitParams();
	int QueryHistoryRecord(CString strQuery,CStringList &strResultList);
	int GetPower();
	void ShowRecords();
	void ComputeResult();
	void ComputeResult(CString strDstString );
	afx_msg void OnBnClickedBtnShowall();
	afx_msg void OnDeletethis();
	afx_msg void OnDeleteall();
	afx_msg void OnNMRclickListShowresult(NMHDR *pNMHDR, LRESULT *pResult);
	void ExcuteSQL(CString strSQL);
};
