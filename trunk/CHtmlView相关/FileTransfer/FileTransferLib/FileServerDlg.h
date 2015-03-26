#pragma once
#include "UIHelper.h"
#include "Resource.h"
#include "PictureButton\PicButton.h"
#include "afxcmn.h"

#include "FileDefines.h"
#include "SkinScrollWnd.h"

#include "FileDefines.h"
#include "PromptDlg.h"

//#include "TooltipListCtrl.h"
#include "BaseDlg.h"

// CFileServerDlg 对话框

#include "FileServer.h"

class CFileServerDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CFileServerDlg)

public:
	CFileServerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileServerDlg();
	void OnCancel();
// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	void InitUI();
	void InitList();
public:
	SOCKET			m_Socket;

private:
	CUIHelper*		m_UIHelper;
	HICON			m_hIcon;
public:
	CPicButton		m_btAddFile;
	CPicButton		m_btSelAll;
	CPicButton		m_btCancel;
	CPicButton		m_btStop;
	CPicButton		m_btMinimize;
	CPicButton		m_btCClose;
	
	CListCtrl		m_wndFileList;
	CProgressCtrl	m_FileProgress;

	bool			m_bExit;

	CPromptDlg		m_PromptDlg;
	CPromptDlg		m_ExitAskDlg;
private:
	//FileListNode	m_CurrentFileInfo;

	CString			m_strSendPath;
	CString			m_strSendFileName;

	int				m_nListItemIndex;

	BasicFileInfo	m_FileInfo;

	CString			m_strMsg;

	bool			m_bAskExit;

	CFileDialog*	m_pFileDlg;
	bool			m_bAddFile;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonAddfile();
	afx_msg void OnBnClickedButtonSelall();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnClose();
	afx_msg void OnHdnItemclickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg LRESULT OnFileToServerProgessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetRangeMessage(WPARAM wParam, LPARAM lParam);

	BOOL UpdateListCtrl(CListCtrl *pListCtrl, BOOL bReceiveList = TRUE);
	//void RefreshListItemState(int nItem, FileListNode *pFindNode);
private:
	bool m_bReceive;
	bool m_bDropFile;
	bool m_bSending;

	bool m_bContinue;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool SendFileRequest(LPCTSTR lpFilePathName);
	void AddListItem(BasicFileInfo fileinfo, State state);
	void SetListItem(BasicFileInfo fileinfo, State state);
	CString GetLastFileDialogPath();
public:
	afx_msg void OnBnClickedButtonStopserver();
public:
	afx_msg void OnBnClickedButtonMinimize();
public:
	afx_msg void OnBnClickedButtonCclose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CFileServer m_ControlThread;

protected:
	virtual void OnMakeTunnelEnd(bool bSuccess, TunnelMode mode);
};
