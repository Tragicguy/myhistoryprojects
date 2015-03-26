// DialAdslDlg.h : 头文件
//
//本程序由唐添麒编写，现将代码发布供学习交流之用
//本程序的发布遵守GPL协议，您可在此协议的规定下使用本程序
//如果您在使用本程序的途中有什么疑问或建议请访问http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//或者联系作者：qq<254909662> 并说明来意，本人非常乐意与您交流
#pragma once

#include "DialManage.h"
#include "afxwin.h"

typedef enum CONNNECT_STATUS
{
	MYADSL_DIALING = 1,
	MYADSL_CONNECTED = 2,
	MYADSL_FAILED   = 3,
	MYADSL_INIALTED = 4,
}CONNNECT_STATUS;

// CDialAdslDlg 对话框
class CDialAdslDlg  : public CDialog
{
// 构造
public:
	CDialAdslDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIALADSL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CBitmapButton m_btnTest;
	CBitmapButton m_btnMin;
	CBitmapButton m_btnClose;
	EntryNameInfo dialInformation;
	static DialManage *lpDialManage;
	static CONNNECT_STATUS dialStatus;

	void SetBitmap(UINT uBmpResource, UINT uCtrlResource);     
	afx_msg void OnBnClickedDial();
	void ConnectByCmd();
	void ConnectByRas();
private:
	// 拖拽
	LRESULT OnNcHitTest(CPoint pt);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMini();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static CListBox dialStatusListBox;
    static VOID WINAPI RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError);
};
