#pragma once

#include "Resource.h"
#include "afxcmn.h"
#include "PictureButton\PicButton.h"
#include "afxwin.h"
#include "FolderNameBuilder.h"
#include "UIHelper.h"
#include "DestFolderHelper.h"
#include "PromptDlg.h"

//#include "DragDrop.h"
//#include "MyProgressCtrl.h"
#include "SortListCtrl.h"

#include "BaseDlg.h"

#include "FileClient.h"

#define MAXCONNECTFAILED 10

typedef struct SortItemData
{
	CString strFileName;
	ULONGLONG ullSize;
	FILETIME FileTime;
	SortItemData()
	{
		ullSize = -1;
	}
}SortItemData;

typedef CArray<SortItemData*, SortItemData*> DataArray;
int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

extern bool gl_bPrompt;
extern CCriticalSection gl_PromptSection;

// CFileClientDialog 对话框

class CFileClientDialog : public CBaseDlg
{
	DECLARE_DYNAMIC(CFileClientDialog)

public:
	CFileClientDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileClientDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

public:
	SOCKET m_Socket;
	virtual BOOL OnInitDialog();
	void SendFile(); 

private:
	bool	SendListFileRequest();
	void	SetListHeader(CListCtrl* pListCtrl);
	void	SetBackPicture(CDC* pDC);
	void	SetPictureButtonStyle();
	void	SetListControlStyle(CListCtrl* pSkinListCtrl);
	void	ListLocalDrives();
	void	ListContentInFolder(char* pFolderName);
	void	DisplayFileCount(bool bLocal, int nCount);
	void	SetDlgReign();
	bool	SendFileC2SRequest();
	bool	SendFileS2CRequest();
	bool	PackAndSendRequest(short nParam);
	void	HighLightItemInList(CListCtrl* pListCtrl, char* pItemName);
	void	EnableButtonsWhenTransferFile(bool bEnable);
	void	InsertLogItem(CString LogItem);
	void	SetForegroundWindowB();
	bool	WriteExact(SOCKET sock, char *buf, int bytes);
	BOOL	DeleteDirectory(char *strDirectory);

	//***pan peng 2008-11-28
	bool	SendGetDriverRequest();
	bool	CreateTestFile(LPCTSTR szFileName);
	void	InitUi();
	void	SetStatusText(LPCTSTR strText);
	void	ClearItemDatas(DataArray &datas);
public:
	//2009-03-11
	bool	ExitBySp(){ return m_bExitBySupporter;}
	//end***

private: //Messages
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnFileTransferMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileToServerMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileToServerFinishMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileToClientFinishMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFileToServerProgessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetLocalPathMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRefreshLocalPathMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetRangeMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetRecvStatusMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnableButtonsMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisableButtonsMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddLogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSendingStopMessage(WPARAM wParam, LPARAM lParam);

	//***Add by pan peng 2008-11-28
	afx_msg LRESULT OnReceiveDrivers(WPARAM wParam, LPARAM lParam);
	//end***
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); 
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point); 
	afx_msg void OnBegindragListLeft(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnBegindragListRight(NMHDR* pNMHDR, LRESULT* pResult); 

	//afx_msg LRESULT OnDragEnter(WPARAM wp, LPARAM lp);
	//afx_msg LRESULT OnDragDrop(WPARAM wp, LPARAM lp);
	//afx_msg LRESULT OnDragAbort(WPARAM wp, LPARAM lp);

	//int GetLBItemUnderPt(CListCtrl* pListCtrl, CPoint pt);
private:
	//CDragDropMgr	m_ddm;						 // drag-drop manager
	CString			m_strFileNameToSend;
	CString			m_strFileNameToReceive;
	
	CArray<SortItemData*, SortItemData*> m_DataArray;

	CSortListCtrl	m_RemoteFileCtrl;
	CSortListCtrl	m_LocalFileCtrl;
	CComboBox		m_ComboLocalDrive;

	CStatic			m_StaticRemotePath;
	CProgressCtrl	m_FileProgress;
	CStatic			m_LocalFileCount;
	CStatic			m_RemoteFileCount;
	CComboBox		m_ComboHistory;

	//***Add by Pan peng 2008-11-27
	CComboBox		m_ComboDestDriver;
	//end***

	CPicButton		m_btClose;
	CPicButton		m_btMin;
	CPicButton		m_btNew;
	CPicButton		m_btDelete;
	CPicButton		m_btSend;
	CPicButton		m_btStop;
	CPicButton		m_btUp;
	CPicButton		m_btRoot;
	CPicButton		m_btRecv;
	CPicButton		m_btRefresh;
	CPicButton		m_btRefreshRemote;
	//panpeng 
	CPicButton		m_btRoot1;
	CPicButton		m_btUp1;
	//

	CString			m_LocalPath;
	CString			m_strDistinationFolder;
	//***Pan peng 2008-11-28
	CString			m_strRemoteFolderPath;
	//end***
	CString			m_RemoteItemToHightLight;
	CString			m_LocalItemToHightLight;

	DataArray		m_RemoteDatas;
	DataArray		m_LocalDatas;
private:
	CUIHelper*		m_UIHelper;
	bool			m_bSending;
	bool			m_bReceive;
	bool			m_ContinueReceiving;
	bool			m_bContinue;
	bool			m_bCancel;
	bool			m_StopByClient;

	bool			m_bDropFile;
	CString			m_strDropPathFileName;

	bool			m_bExitBySupporter;

	CPromptDlg		m_PromptDlg;
	CString			m_strMsg;

public:
	bool			m_bExit;
	bool			m_bEnableRemotePath;
public:				
	CString			GetCurFileName();
private:

	afx_msg void	OnBnClickedButtonMin();
	afx_msg void	OnCbnSelchangeComboLocaldrive();
	afx_msg void	OnNMDblclkListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnBnClickedButtonRoot();
	afx_msg void	OnBnClickedButtonUp();
	afx_msg void	OnBnClickedButtonRoot1();
	afx_msg void	OnBnClickedButtonUp1();
	afx_msg HBRUSH	OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void	OnBnClickedButtonSend();
	afx_msg void	OnBnClickedButtonDelete();
	afx_msg void	OnBnClickedButtonNew();
	afx_msg void	OnClose();
	afx_msg void	OnBnClickedButtonStop();
	afx_msg void	OnBnClickedButtonReceive();
	afx_msg void	OnBnClickedCancel();
	afx_msg void	OnBnClickedButtonRefresh();
	afx_msg void	OnBnClickedButtonRefRemote();
	afx_msg void	OnLvnItemchangedListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnLvnItemchangedListRemote(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboDestdriver();
	afx_msg void OnNMDblclkListRemote(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CEdit m_LocalPathCtrl;
	afx_msg void OnLvnColumnclickListLocal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickListRemote(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditLocalfpath();

	CFileClient m_ControlThread;

protected:
	void OnMakeTunnelEnd(bool bSuccess, TunnelMode mode);

	int m_nConnectFailed;
	int m_nTest;

	CTime m_LastRebuildTime;
	bool bTest;
protected: 
	CListCtrl* m_pDragList;		//Which ListCtrl we are dragging FROM 
	CListCtrl* m_pDropList;		//Which ListCtrl we are dropping ON 
	CImageList* m_pDragImage;	//For creating and managing the drag-image 
	BOOL		m_bDragging;	//T during a drag operation 
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM 
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON 
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type) 
 
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList); 
};
