// FileClientDialog.cpp : ????
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "FileClientDialog.h"
#include "../FileTransferLib/FileDefines.h"
#include "FileClient.h"
#include "FileServerDlg.h"
#include "GlobalFun.h"

#pragma warning(disable:4800)

IMPLEMENT_DYNAMIC(CFileClientDialog, CDialog)

CFileClientDialog::CFileClientDialog(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CFileClientDialog::IDD, pParent)
{
	char strSystemDirectory[MAX_PATH];
	GetSystemDirectory(strSystemDirectory, MAX_PATH);
	m_strDistinationFolder.Format("%c:\\Web1800S", strSystemDirectory[0]);


	m_UIHelper = NULL;
	m_hIcon = NULL;
	m_bCancel = false;

	m_bSending = false;
	m_bReceive = false;
	m_bDropFile = false;
	m_StopByClient = false;

	m_bExitBySupporter = false;
	m_strDropPathFileName = _T("");
	m_strMsg = _T("");

	m_bExit = false;

	m_strFileNameToSend = _T("");

	m_bEnableRemotePath = true;
	m_nConnectFailed = 0;

	m_LastRebuildTime = CTime::GetTickCount();
	bTest = true;

	m_Socket = INVALID_SOCKET;
	//m_nTest = 0;
}

CFileClientDialog::~CFileClientDialog()
{
	try
	{
		for(int i = 0; i < m_DataArray.GetSize(); i++)
		{
			SortItemData* pData = m_DataArray.GetAt(i);
			if (pData != NULL)
			{
				delete pData;
				pData = NULL;
			}
		}
		m_DataArray.RemoveAll();

		ClearItemDatas(this->m_RemoteDatas);
		ClearItemDatas(this->m_LocalDatas);

		if (m_UIHelper != NULL)
		{
			delete m_UIHelper;
			m_UIHelper = NULL;
		}

		if (m_hIcon != NULL)
		{
			DestroyIcon(m_hIcon);
		}

		m_PromptDlg.DestroyWindow();
	}
	catch (...) {}
}

void CFileClientDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REMOTE, m_RemoteFileCtrl);
	DDX_Control(pDX, IDC_LIST_LOCAL, m_LocalFileCtrl);
	DDX_Control(pDX, IDC_COMBO_LOCALDRIVE, m_ComboLocalDrive);
	DDX_Control(pDX, IDC_STATIC_REMOTEPATH, m_StaticRemotePath);
	DDX_Control(pDX, IDC_STATIC_REMOTEPATH2, m_LocalFileCount);
	DDX_Control(pDX, IDC_STATIC_REMOTEPATH3, m_RemoteFileCount);
	DDX_Control(pDX, IDC_PROGRESS, m_FileProgress);
	DDX_Control(pDX, IDC_COMBO_HISTORY, m_ComboHistory);
	DDX_Control(pDX, IDC_COMBO_DESTDRIVER, m_ComboDestDriver);
	DDX_Control(pDX, IDC_EDIT_LOCALFPATH, m_LocalPathCtrl);
}

BEGIN_MESSAGE_MAP(CFileClientDialog, CDialog)
	//ON_MESSAGE(WM_FILE_LISTFILE_MESSAGE, OnFileTransferMessage) 
	ON_MESSAGE(WM_FILE_GETLOCALPATH_MESSAGE, OnGetLocalPathMessage)
	ON_MESSAGE(WM_FILE_SENDFILETOSERVER_MESSAGE, OnFileToServerMessage)
	ON_MESSAGE(WM_FILE_SENDFILETOSERVER_FINISH_MESSAGE, OnFileToServerFinishMessage)
	ON_MESSAGE(WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, OnFileToServerProgessMessage)
	ON_MESSAGE(WM_FILE_REFRESHLOCAL_MESSAGE, OnRefreshLocalPathMessage)
	ON_MESSAGE(WM_FILE_SETRANGE_MESSAGE, OnSetRangeMessage)
	ON_MESSAGE(WM_FILE_GETRECVSTATUS_MESSAGE, OnGetRecvStatusMessage)
	ON_MESSAGE(WM_FILE_ENABLEBUTTON_MESSAGE, OnEnableButtonsMessage)
	ON_MESSAGE(WM_FILE_DISABLEBUTTON_MESSAGE, OnDisableButtonsMessage)
	ON_MESSAGE(WM_FILE_ADDLOG_MESSAGE, OnAddLogMessage)
	ON_MESSAGE(WM_FILE_FILETOCLIENTFINISH_MESSAGE, OnFileToClientFinishMessage)
	ON_MESSAGE(WM_FILE_FILETOSERVERSTOP_MESSAGE, OnSendingStopMessage)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CFileClientDialog::OnBnClickedButtonMin)
	ON_CBN_SELCHANGE(IDC_COMBO_LOCALDRIVE, &CFileClientDialog::OnCbnSelchangeComboLocaldrive)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOCAL, &CFileClientDialog::OnNMDblclkListLocal)
	ON_BN_CLICKED(IDC_BUTTON_ROOT, &CFileClientDialog::OnBnClickedButtonRoot)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CFileClientDialog::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_ROOT1, &CFileClientDialog::OnBnClickedButtonRoot1)
	ON_BN_CLICKED(IDC_BUTTON_UP1, &CFileClientDialog::OnBnClickedButtonUp1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BUTTON_SEND, &CFileClientDialog::OnBnClickedButtonSend)
	ON_BN_CLICKED(ID_BUTTON_DELETE, &CFileClientDialog::OnBnClickedButtonDelete)
	ON_BN_CLICKED(ID_BUTTON_NEW, &CFileClientDialog::OnBnClickedButtonNew)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BUTTON_STOP, &CFileClientDialog::OnBnClickedButtonStop)
	ON_BN_CLICKED(ID_BUTTON_RECEIVE, &CFileClientDialog::OnBnClickedButtonReceive)
	ON_BN_CLICKED(IDCANCEL, &CFileClientDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CFileClientDialog::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_REF_REMOTE, &CFileClientDialog::OnBnClickedButtonRefRemote)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOCAL, &CFileClientDialog::OnLvnItemchangedListLocal)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REMOTE, &CFileClientDialog::OnLvnItemchangedListRemote)
	ON_CBN_SELCHANGE(IDC_COMBO_DESTDRIVER, &CFileClientDialog::OnCbnSelchangeComboDestdriver)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REMOTE, &CFileClientDialog::OnNMDblclkListRemote)
	ON_WM_DROPFILES()
	ON_WM_TIMER()
	//ON_REGISTERED_MESSAGE(WM_DD_DRAGENTER, OnDragEnter)
	//ON_REGISTERED_MESSAGE(WM_DD_DRAGDROP,	OnDragDrop)
	//ON_REGISTERED_MESSAGE(WM_DD_DRAGABORT,	OnDragAbort)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_LOCAL, &CFileClientDialog::OnLvnColumnclickListLocal)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_REMOTE, &CFileClientDialog::OnLvnColumnclickListRemote)
	ON_EN_KILLFOCUS(IDC_EDIT_LOCALFPATH, &CFileClientDialog::OnEnKillfocusEditLocalfpath)

	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_LOCAL, OnBegindragListLeft)    
    ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_REMOTE, OnBegindragListRight)
	ON_WM_MOUSEMOVE()   
    ON_WM_LBUTTONUP()   
END_MESSAGE_MAP()



BOOL CFileClientDialog::OnInitDialog()
{
	try
	{
		CBaseDlg::OnInitDialog();

		this->SetControlThread(&m_ControlThread);
		m_ControlThread.SetClientSocket(&m_ClientCtrlSocket);
		BuildConnect(m_nCtrlPort);

		// TODO:  在此添加额外的初始化
		char strEuName[MAX_PATH];		
		CString strEuNameTemp;
		::GetWindowText(m_MsgNotifyWnd,strEuName,MAX_PATH);		
		strEuNameTemp = strEuName;
		int index = strEuNameTemp.Find('-');
		if(index>0)
		{
			strEuNameTemp = strEuNameTemp.Left(index);
		}
		SetDlgItemText(IDC_STATIC_EUNAME,strEuNameTemp);
		SetWindowText("文件传输-"+strEuNameTemp);
		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon
		ModifyStyle(0, WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		
		SetPrompt(false,m_hWnd);
		InitUi();
		m_StaticRemotePath.SetWindowTextA(m_strDistinationFolder);

		SetListControlStyle(&m_RemoteFileCtrl);
		SetListControlStyle(&m_LocalFileCtrl);

		SetListHeader(&m_RemoteFileCtrl);
		SetListHeader(&m_LocalFileCtrl);

		DisplayFileCount(false, 0);

		SetPictureButtonStyle();

		ListLocalDrives();

		//***pan peng 2008-11-28
		
		//end***
		//SendListFileRequest();

		//m_UIHelper = new CUIHelper(IDR_GIFMAPBG);

		SetForegroundWindow();
		m_PromptDlg.SetSpType(1);
		m_PromptDlg.SetType(MessageDlg);
		m_PromptDlg.Create(IDD_DIALOG_PROMPT, this);
		m_PromptDlg.SetHwnd(m_hWnd);
		/*static DRAGDROPWND MyDragDropWindows[] = {
			{ IDC_LIST_LOCAL, DDW_SOURCE | DDW_TARGET },
			{ IDC_LIST_REMOTE, DDW_SOURCE | DDW_TARGET },
			{ 0, 0 },
		};
		m_ddm.Install(this, MyDragDropWindows);*/

		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
		SetTimer(1, 500, NULL);

		return TRUE;  // return TRUE unless you set the focus to a control
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnInitDialog unknow exception.");
	}
	return FALSE;
	// 异常: OCX 属性页应返回 FALSE
}

void CFileClientDialog::SetForegroundWindowB()
{
	try
	{
		HWND hWnd = this->GetSafeHwnd();
		HWND pHWnd = ::GetForegroundWindow();
		if (pHWnd != hWnd)
		{
			DWORD dProcessID;
			DWORD dFocusWndThrID = ::GetWindowThreadProcessId(pHWnd, &dProcessID);
			DWORD dCurWndThrID = ::GetWindowThreadProcessId(hWnd, &dProcessID);
			::AttachThreadInput(dCurWndThrID, dFocusWndThrID, TRUE);
			::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			::AttachThreadInput(dCurWndThrID, dFocusWndThrID, FALSE);
		}
		else
		{
			::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SetForegroundWindowB unknow exception.");
	}
}

void CFileClientDialog::SetDlgReign()
{
	try
	{
		int UI_ROUNDRECT_WIDTH = 16;
		int UI_ROUNDRECT_HEIGHT = 16;

		CRect rc;
		POINT Points[6];
		GetClientRect(&rc);

		if (m_bEnableRemotePath)
			this->MoveWindow(0, 0, 701, 478);
		else
			this->MoveWindow(0, 0, 495, 460);

		HRGN rgn1 = CreateRoundRectRgn(0, 0, rc.Width() / 2, rc.Height() / 2, UI_ROUNDRECT_WIDTH, UI_ROUNDRECT_HEIGHT);
		Points[0].x = 0;
		Points[0].y = UI_ROUNDRECT_HEIGHT;
		Points[1].x = 0;
		Points[1].y = rc.Height() + UI_ROUNDRECT_HEIGHT;
		Points[2].x = rc.Width() + UI_ROUNDRECT_WIDTH;
		Points[2].y = rc.Height() + UI_ROUNDRECT_HEIGHT;
		Points[3].x = rc.Width() + UI_ROUNDRECT_WIDTH;
		Points[3].y = 0;
		Points[4].x = UI_ROUNDRECT_WIDTH;
		Points[4].y = 0;
		Points[5].x = UI_ROUNDRECT_WIDTH;
		Points[5].y = UI_ROUNDRECT_HEIGHT;
		HRGN rgn2 = CreatePolygonRgn(Points, 6, ALTERNATE);
		CombineRgn(rgn1, rgn1, rgn2, RGN_OR);
		SetWindowRgn(rgn1, TRUE);

		DeleteObject(rgn2);
		DeleteObject(rgn1);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SetDlgReign unknow exception.");
	}
}

void CFileClientDialog::SetPictureButtonStyle()
{
	try
	{
		CString strTip;

		m_btClose.SubclassDlgItem(IDCANCEL, this);
		m_btClose.SetIcon(IDB_BITMAP_CLOSE2, IDB_BITMAP_CLOSE1);
		strTip = "关闭文件传输";
		m_btClose.SetToolTipText(&strTip);

		m_btMin.SubclassDlgItem(IDC_BUTTON_MIN, this);
		m_btMin.SetIcon(IDB_BITMAP_MIN2, IDB_BITMAP_MIN1);
		strTip = "最小化";
		m_btMin.SetToolTipText(&strTip);

		m_btNew.SubclassDlgItem(ID_BUTTON_NEW, this);
		m_btNew.SetIcon(IDB_BITMAP_NEW2, IDB_BITMAP_NEW1);
		strTip = "新建本地目录";
		m_btNew.SetToolTipText(&strTip);

		m_btDelete.SubclassDlgItem(ID_BUTTON_DELETE, this);
		m_btDelete.SetIcon(IDB_BITMAP_DELETE2, IDB_BITMAP_DELETE1);
		strTip = "删除选中的本地文件";
		m_btDelete.SetToolTipText(&strTip);

		m_btSend.SubclassDlgItem(ID_BUTTON_SEND, this);
		m_btSend.SetIcon(IDB_BITMAP_SEND2, IDB_BITMAP_SEND1);
		strTip = "发送文件至远端";
		m_btSend.SetToolTipText(&strTip);

		m_btRecv.SubclassDlgItem(ID_BUTTON_RECEIVE, this);
		m_btRecv.SetIcon(IDB_BITMAP_RECV2, IDB_BITMAP_RECV1);
		strTip = "获取文件至本地";
		m_btRecv.SetToolTipText(&strTip);

		m_btStop.SubclassDlgItem(ID_BUTTON_STOP, this);
		m_btStop.SetIcon(IDB_BITMAP_STOP2, IDB_BITMAP_STOP1);
		strTip = "停止发送/接收";
		m_btStop.SetToolTipText(&strTip);

		m_btUp.SubclassDlgItem(IDC_BUTTON_UP, this);
		m_btUp.SetIcon(IDB_BITMAP_UP2, IDB_BITMAP_UP1);
		strTip = "返回上一级目录";
		m_btUp.SetToolTipText(&strTip);

		m_btRoot.SubclassDlgItem(IDC_BUTTON_ROOT, this);
		m_btRoot.SetIcon(IDB_BITMAP_ROOT2, IDB_BITMAP_ROOT1);
		strTip = "返回根目录";
		m_btRoot.SetToolTipText(&strTip);

		m_btRefresh.SubclassDlgItem(IDC_BUTTON_REFRESH, this);
		m_btRefresh.SetIcon(IDB_BITMAP_Refresh2, IDB_BITMAP_Refresh1);
		strTip = "刷新本地目录内容";
		m_btRefresh.SetToolTipText(&strTip);

		m_btRefreshRemote.SubclassDlgItem(IDC_BUTTON_REF_REMOTE, this);
		m_btRefreshRemote.SetIcon(IDB_BITMAP_Refresh2, IDB_BITMAP_Refresh1);
		strTip = "刷新远程目录内容";
		m_btRefreshRemote.SetToolTipText(&strTip);

		//panpeng 
		m_btUp1.SubclassDlgItem(IDC_BUTTON_UP1, this);
		m_btUp1.SetIcon(IDB_BITMAP_UP2, IDB_BITMAP_UP1);
		strTip = "返回远程上一级目录";
		m_btUp1.SetToolTipText(&strTip);

		m_btRoot1.SubclassDlgItem(IDC_BUTTON_ROOT1, this);
		m_btRoot1.SetIcon(IDB_BITMAP_ROOT2, IDB_BITMAP_ROOT1);
		strTip = "返回远程根目录";
		m_btRoot1.SetToolTipText(&strTip);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SetPictureButtonStyle unknow exception.");
	}
	//
}

void CFileClientDialog::SetListControlStyle(CListCtrl* pSkinListCtrl)
{
	//pSkinListCtrl->Init();
	try
	{
		if (pSkinListCtrl && IsWindow(pSkinListCtrl->m_hWnd))
			ListView_SetExtendedListViewStyle(pSkinListCtrl->m_hWnd, LVS_EX_FULLROWSELECT);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SetListControlStyle unknow exception.");
	}
}

bool CFileClientDialog::SendListFileRequest()
{
	try
	{
		OutputDebugString("CFileClientDialog::SendListFileRequest()\n");
		if (this->m_bEnableRemotePath)
			InsertLogItem("获取远程文件列表 ");
		bool bRes = PackAndSendRequest(CMD_PARAM_LISTFILE);
		//***Pan peng 2008-11-28
		if(bRes && m_Socket != INVALID_SOCKET)
		{
			RemoteFolderPath path;
			strncpy(path.RemotePath, m_strRemoteFolderPath, MAX_PATH-1);
			int nSent = send(m_Socket, (char*)(&path), sizeof(RemoteFolderPath), 0);
			if (nSent <= 0 || MAX_PATH != nSent)
				return false;
		}
		//end***
		return bRes;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SendListFileRequest unknow exception.");
	}
	return false;
}

LRESULT CFileClientDialog::OnFileTransferMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		char* pFileList = (char*) wParam;
		int nCount = (int) lParam;

		m_RemoteFileCtrl.DeleteAllItems();

		ClearItemDatas(this->m_RemoteDatas);

		int i = 0;

		for (int i = 0 ; i < nCount; i++)
		{
			BasicFileInfo* pFileInfo = (BasicFileInfo*)(pFileList + i * sizeof(BasicFileInfo));
			// Add it to the ListView
			LVITEM Item;
			Item.mask = LVIF_TEXT;
			Item.iItem = nCount;
			Item.iSubItem = 0;
			Item.pszText = pFileInfo->FileName;
			int nItem = m_RemoteFileCtrl.InsertItem(&Item);

			char szText[256];
			szText[0] = '\0';

			ULONGLONG lSize = pFileInfo->Size;

			//***modify by pan peng 2008-11-27 
			if(lSize == -1)
			{
				wsprintf(szText, "文件夹");
			}
			else
			{
				if (lSize > 1024)
				{
					lSize /= 1024;
					wsprintf(szText, "%dKb", lSize);
				}
				else
				{
					wsprintf(szText, "%ubytes", lSize);
				}
			}
			//end***
			Item.mask = LVIF_TEXT;
			Item.iItem = nItem;
			Item.iSubItem = 1;
			Item.pszText = szText;
			m_RemoteFileCtrl.SetItem(&Item);


			SYSTEMTIME FileTime;
			FileTimeToSystemTime(&(pFileInfo->CreationTime), &FileTime);
			wsprintf(szText, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", FileTime.wMonth, FileTime.wDay, FileTime.wYear, FileTime.wHour, FileTime.wMinute);

			Item.mask = LVIF_TEXT;
			Item.iItem = nItem;
			Item.iSubItem = 2;
			Item.pszText = szText;
			m_RemoteFileCtrl.SetItem(&Item);

			SortItemData* pData = new SortItemData;
			pData->strFileName = pFileInfo->FileName;
			pData->ullSize = lSize;
			pData->FileTime = pFileInfo->CreationTime;
			m_RemoteFileCtrl.SetItemData(nItem, (DWORD_PTR)pData);
			m_DataArray.Add(pData);
		}

		DisplayFileCount(false, nCount);

		if (m_RemoteItemToHightLight.GetLength() > 0)
		{
			HighLightItemInList(&m_RemoteFileCtrl, (LPSTR)(LPCTSTR) m_RemoteItemToHightLight);
		}
		m_RemoteFileCtrl.SortItems(ListCompare, (DWORD_PTR)&m_RemoteFileCtrl);
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnFileTransferMessage unknow exception.");
	}
	return 0;
}

void CFileClientDialog::SetListHeader(CListCtrl* pListCtrl)
{
	pListCtrl->InsertColumn(0, "名称", 0, 150);
	pListCtrl->InsertColumn(1, "大小", 0, 60);
	pListCtrl->InsertColumn(2, "修改时间", 0, 115);
}

BOOL CFileClientDialog::OnEraseBkgnd(CDC* pDC)
{
	try
	{
		//CDialog::OnEraseBkgnd(pDC);

		CRect rc;
		GetWindowRect(&rc);
		ScreenToClient(&rc);

		if (m_UIHelper != NULL)
		{
			m_UIHelper->SetBackPicture(pDC, rc);
		}

		return true;
	}
	catch(...)
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
	return true;
}

LRESULT CFileClientDialog::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return HTCAPTION;
}

void CFileClientDialog::OnBnClickedButtonMin()
{
	if (IsWindow(m_hWnd))
		this->ShowWindow(SW_SHOWMINIMIZED);
}

//
// List local drives
//
void CFileClientDialog::ListLocalDrives()
{
	try
	{
		TCHAR szDrivesList[256]; // Format when filled : "C:\<NULL>D:\<NULL>....Z:\<NULL><NULL>"
		TCHAR szDrive[4];
		TCHAR szTheDrive[32];
		// TCHAR szName[255];
		TCHAR szType[16];
		UINT nType = 0;
		DWORD dwLen;
		int nIndex = 0;
		dwLen = GetLogicalDriveStrings(256, szDrivesList);

		HWND hWndLocalList = GetDlgItem(IDC_LIST_LOCAL)->m_hWnd;
		SendDlgItemMessage(IDC_COMBO_LOCALDRIVE, LB_RESETCONTENT, 0, 0L);

		ListView_DeleteAllItems(hWndLocalList);
		//SetDlgItemText(hWnd, IDC_CURR_LOCAL, "");

		int k = 0;

		//Add the desktop to the driver list
		sprintf_s(szTheDrive, "%s", " 桌面");
		sprintf_s(szType, "%s", "本地磁盘");

		LVITEM Item;
		Item.mask = LVIF_TEXT;
		Item.iItem = k;
		Item.iSubItem = 0;
		Item.pszText = szTheDrive;
		int nItem = ListView_InsertItem(hWndLocalList, &Item);

		Item.mask = LVIF_TEXT;
		Item.iItem = nItem;
		Item.iSubItem = 1;
		Item.pszText = szType;
		ListView_SetItem(hWndLocalList, &Item);

		SendDlgItemMessageA(IDC_COMBO_LOCALDRIVE, CB_ADDSTRING, 0, (LPARAM)szTheDrive);

		// Parse the list of drives	
		while (nIndex < (int)(dwLen - 3))
		{
			strcpy_s(szDrive, szDrivesList + nIndex);
			nIndex += 4;
			szDrive[2] = '\0'; // remove the '\'
			sprintf_s(szTheDrive, "(%s)", szDrive);

			// szName[0] = '\0';
			szType[0] = '\0';

			strcat_s(szDrive, "\\");

			// GetVolumeInformation(szDrive, szName, sizeof(szName), NULL, NULL, NULL, NULL, NULL);

			// Get infos on the Drive (type and Name)
			nType = GetDriveType(szDrive);
			switch (nType)
			{
			case DRIVE_FIXED:
				sprintf_s(szType, "%s", "本地磁盘");
				break;

			case DRIVE_REMOVABLE:
				sprintf_s(szType, "%s", "软驱");
				break;

			case DRIVE_CDROM:
				sprintf_s(szType, "%s", "CD-ROM");
				break;

			case DRIVE_REMOTE:
				sprintf_s(szType, "%s", "网络驱动器");
				break;

			default:
				sprintf_s(szType, "%s", "未知");
				break;
			}

			// Add it to the ListView
			LVITEM Item;
			Item.mask = LVIF_TEXT;
			Item.iItem = k;
			Item.iSubItem = 0;
			Item.pszText = szTheDrive;
			int nItem = ListView_InsertItem(hWndLocalList, &Item);

			Item.mask = LVIF_TEXT;
			Item.iItem = nItem;
			Item.iSubItem = 1;
			Item.pszText = szType;
			ListView_SetItem(hWndLocalList, &Item);

			// Prepare it for Combo Box and add it
			strcat_s(szTheDrive, " - ");
			strcat_s(szTheDrive, szType);

			SendDlgItemMessageA(IDC_COMBO_LOCALDRIVE, CB_ADDSTRING, 0, (LPARAM) szTheDrive);
			k++;
		}

		SendDlgItemMessageA(IDC_COMBO_LOCALDRIVE, CB_SETCURSEL, 0, 0);
		OnCbnSelchangeComboLocaldrive();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::ListLocalDrives unknow exception.");
	}
}

void CFileClientDialog::OnCbnSelchangeComboLocaldrive()
{
	try
	{
		char ofDir[MAX_PATH];
		m_ComboLocalDrive.GetWindowTextA(ofDir, MAX_PATH - 1);

		if (strcmp(ofDir, " 桌面"))
		{
			ofDir[4] = '\0'; // Hum...
			m_LocalPath.Format("%s", ofDir);
			m_LocalPath.Replace("(", "");
			m_LocalPath.Replace(")", "");
		}
		else
		{
			char strDesktop[MAX_PATH];
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
			SHGetPathFromIDList(pidl, strDesktop);
			m_LocalPath.Format("%s", strDesktop);
		}

		ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnCbnSelchangeComboLocaldrive unknow exception.");
	}
}

void CFileClientDialog::ListContentInFolder(char* pFolderName)
{
	try
	{
		this->m_LocalFileCtrl.DeleteAllItems();

		this->ClearItemDatas(this->m_LocalDatas);

		m_LocalPathCtrl.SetWindowText(pFolderName);
		m_LocalPathCtrl.SetSel(-1);
		CRect rect;
		m_LocalPathCtrl.GetWindowRect(&rect);
		ScreenToClient(&rect);
		this->InvalidateRect(rect);

		m_LocalFileCtrl.SetRedraw(FALSE);

		CString m_DestFolder(pFolderName);
		m_DestFolder += ("\\*.*");

		CFileFind m_FileFind;
		int nCount = 0;

		BOOL bWorking = m_FileFind.FindFile(m_DestFolder);

		if (!bWorking)
		{
			DisplayFileCount(true, 0);
			return;
		}
		else
		{
			while (bWorking)
			{
				bWorking = m_FileFind.FindNextFile();

				if (m_FileFind.IsDots())
					continue;

				ULONGLONG ullSize = 0;
				CString strName = _T("");
				char szText[256];
				szText[0] = '\0';
				if (!m_FileFind.IsDirectory())
				{
					CString FileName = m_FileFind.GetFileName();
					strName = FileName;
					// Add it to the ListView
					LVITEM Item;
					Item.mask = LVIF_TEXT;
					Item.iItem = nCount;
					Item.iSubItem = 0;
					Item.pszText = (LPSTR)(LPCTSTR) FileName;
					int nItem = m_LocalFileCtrl.InsertItem(&Item);


					ULONGLONG fileSize = m_FileFind.GetLength();
					ullSize = fileSize;
					if (fileSize > 1024)
					{
						fileSize /= 1024;
						wsprintf(szText,"%dKb", fileSize);
					}
					else
					{
						wsprintf(szText,"%ubytes", fileSize);
					}

					Item.mask = LVIF_TEXT;
					Item.iItem = nItem;
					Item.iSubItem = 1;
					Item.pszText = szText;
					m_LocalFileCtrl.SetItem(&Item);

					ullSize = fileSize;

					FILETIME fileTime;
					SYSTEMTIME FileTime;
					m_FileFind.GetLastWriteTime(&fileTime);
					FileTimeToSystemTime(&fileTime, &FileTime);
					wsprintf(szText, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", FileTime.wMonth, FileTime.wDay, FileTime.wYear,
						FileTime.wHour, FileTime.wMinute);
					Item.mask = LVIF_TEXT;
					Item.iItem = nItem;
					Item.iSubItem = 2;
					Item.pszText = szText;
					m_LocalFileCtrl.SetItem(&Item);

					SortItemData* pData = new SortItemData;
					pData->strFileName = strName;
					pData->ullSize = ullSize;
					pData->FileTime = fileTime;
					m_LocalFileCtrl.SetItemData(nItem, (DWORD_PTR)pData);
					m_DataArray.Add(pData);
				}
				else
				{
					CString FileName = "(" + m_FileFind.GetFileName() + ")";
					strName = FileName;
					// Add it to the ListView
					LVITEM Item;
					Item.mask = LVIF_TEXT;
					Item.iItem = nCount;
					Item.iSubItem = 0;
					Item.pszText = (LPSTR)(LPCTSTR) FileName;
					int nItem = m_LocalFileCtrl.InsertItem(&Item);

					Item.mask = LVIF_TEXT;
					Item.iItem = nItem;
					Item.iSubItem = 1;
					Item.pszText = "文件夹";
					m_LocalFileCtrl.SetItem(&Item);

					FILETIME fileTime;
					SYSTEMTIME FileTime;
					m_FileFind.GetLastWriteTime(&fileTime);
					FileTimeToSystemTime(&fileTime, &FileTime);
					wsprintf(szText, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", FileTime.wMonth, FileTime.wDay, FileTime.wYear,
						FileTime.wHour, FileTime.wMinute);
					Item.mask = LVIF_TEXT;
					Item.iItem = nItem;
					Item.iSubItem = 2;
					Item.pszText = szText;
					m_LocalFileCtrl.SetItem(&Item);

					SortItemData* pData = new SortItemData;
					pData->strFileName = strName;
					pData->ullSize = ullSize;
					pData->FileTime = fileTime;
					m_LocalFileCtrl.SetItemData(nItem, (DWORD_PTR)pData);
					m_DataArray.Add(pData);
				}

				nCount++;
			}
		}

		m_FileFind.Close();

		m_LocalFileCtrl.SetRedraw(TRUE);

		DisplayFileCount(true, nCount);

		if (m_LocalItemToHightLight.GetLength() > 0)
		{
			HighLightItemInList(&m_LocalFileCtrl, (LPSTR)(LPCTSTR) m_LocalItemToHightLight);
		}

		m_LocalFileCtrl.SortItems(ListCompare, (DWORD_PTR)&m_LocalFileCtrl);
		return;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::ListContentInFolder unknow exception.");
	}
}

void CFileClientDialog::OnNMDblclkListLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		// TODO: 在此添加控件通知处理程序代码
		if (pNMHDR->hwndFrom == this->m_LocalFileCtrl.GetSafeHwnd())
		{
			POSITION pos = m_LocalFileCtrl.GetFirstSelectedItemPosition();
			if (!pos) return;

			int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
			CString strItem = m_LocalFileCtrl.GetItemText(iItem, 0);

			if (strItem.Left(1) == '(' && strItem.Right(1) == ')')
			{
				strItem = strItem.Mid(1, strItem.GetLength() - 2);
			}
			else
				return;

			if (m_LocalPath.GetLength() > 1)
			{
				m_LocalPath += "\\";
			}

			m_LocalPath += strItem;

			ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
		}

		*pResult = 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnNMDblclkListLocal unknow exception.");
	}
	*pResult = 0;
}

void CFileClientDialog::OnBnClickedButtonRoot()
{
	OnCbnSelchangeComboLocaldrive();
}

void CFileClientDialog::OnBnClickedButtonUp()
{
	int nIndex = m_LocalPath.ReverseFind('\\');
	if (nIndex > 1)
	{
		m_LocalPath = m_LocalPath.Left(nIndex);
		ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
	}
}

void CFileClientDialog::OnBnClickedButtonRoot1()
{
	try
	{
		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		char strDesktop[MAX_PATH];
		LPITEMIDLIST pidl;
		SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
		SHGetPathFromIDList(pidl, strDesktop);
		m_strRemoteFolderPath.Format("%s", strDesktop);
		m_ComboDestDriver.SetCurSel(0);

		m_StaticRemotePath.SetWindowTextA(m_strRemoteFolderPath);
		SendListFileRequest();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonRoot1 unknow exception.");
	}
}

void CFileClientDialog::OnBnClickedButtonUp1()
{
	try
	{
		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		int nIndex = m_strRemoteFolderPath.ReverseFind('\\');
		if (nIndex > 1)
		{
			m_strRemoteFolderPath = m_strRemoteFolderPath.Left(nIndex);
			this->SendListFileRequest();
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonUp1 unknow exception.");
	}
}


void CFileClientDialog::DisplayFileCount(bool bLocal, int nCount)
{
	try
	{
		CString strFileCount;
		strFileCount.Format("共 %d 个文件/文件夹", nCount);

		if (bLocal)
		{
			m_LocalFileCount.SetWindowTextA(strFileCount);

			RECT rc;
			m_LocalFileCount.GetWindowRect(&rc);

			ScreenToClient(&rc);
			m_LocalFileCount.GetParent()->InvalidateRect(&rc);
		}
		else
		{
			m_RemoteFileCount.SetWindowTextA(strFileCount);
			m_RemoteFileCount.GetParent()->RedrawWindow();

			RECT rc;
			m_RemoteFileCount.GetWindowRect(&rc);
			ScreenToClient(&rc);
			m_RemoteFileCount.GetParent()->InvalidateRect(&rc);
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_error("CFileClientDialog::DisplayFileCount unknow exception. bLocal:%d,nCount:%d",bLocal==true?1:0,nCount);
	}
}

HBRUSH CFileClientDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		pDC->SetBkMode(TRANSPARENT);
	}
	else
	{
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS )//|| pWnd->GetDlgCtrlID() == IDC_EDIT_LOCALFPATH
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		hbr = (HBRUSH) GetStockObject(NULL_BRUSH);
	}


	if (pWnd->GetDlgCtrlID() == IDC_STATIC_EUNAME )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(37, 45, 107));
		hbr = (HBRUSH) GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

void CFileClientDialog::OnBnClickedButtonSend()
{
	// 发送文件到服务器
	try
	{
		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		if (m_bSending || m_bReceive)
		{
			CString strMsg = _T("已经有文件正在传输，请停止后再重试。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		m_bSending = true;

		CString strFileName = _T("");
		CString strItem = _T("");
		if(!m_bDropFile)
		{
			POSITION pos = m_LocalFileCtrl.GetFirstSelectedItemPosition();
			if (!pos)
			{
				m_bSending = false;
				CString strMsg = _T("请选择要发送的文件。");
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				return;
			}

			int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
			strItem = m_LocalFileCtrl.GetItemText(iItem, 0);
			int nLeft = strItem.Find("(");
			int nRight = strItem.Find(")");

			if (nLeft == 0 && nRight == strItem.GetLength() - 1)
			{
				m_bSending = false;
				if (nLeft == 0 && nRight == strItem.GetLength() - 1)
				{
					CString strMsg = _T("不支持文件夹传输。");
					m_PromptDlg.SetMsgText(strMsg);
					m_PromptDlg.Show();
				}
				return;
			}

			strFileName = this->m_LocalPath + "\\" + strItem;
		}
		else
		{
			strFileName = this->m_strDropPathFileName;
			int nFind = strFileName.ReverseFind('\\');
			if (nFind > 0)
				strItem = strFileName.Right(strFileName.GetLength() - nFind - 1);
		}

		LVFINDINFO findInfo;
		findInfo.flags = LVFI_STRING;
		findInfo.psz = (LPSTR)(LPCTSTR) strItem;

		//gl_bOverWrite = false;
		gl_mapOverWrite[m_hWnd] = false;
		//if (m_RemoteFileCtrl.FindItem(&findInfo) != -1)
		if (FindMyItem(&m_RemoteFileCtrl, strItem) != -1)
		{
			CString strInfo = "已经存在同名文件，点“是”覆盖原文件，\n\r点“否”开始断点续传，点“取消”终止本次操作。";
			int nResult = MessageBox(strInfo, "提示", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);

			if (nResult == IDYES)
			{
				//gl_bOverWrite = true;
				gl_mapOverWrite[m_hWnd] = true;
			}
			else if (nResult == IDNO)
			{
				//gl_bOverWrite = false;
				gl_mapOverWrite[m_hWnd] = false;
			}
			else if (nResult == IDCANCEL)
			{
				m_bDropFile = false;
				m_bSending = false;
				return;
			}
		}

		m_strFileNameToSend = strFileName;

		CFileFind findfile;
		if (!findfile.FindFile(strFileName))
		{
			CString strMsg = _T("找不到本地文件，该文件可能已经被删除。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			m_bDropFile = false;
			m_bSending = false;
			return;
		}

		CFile LocalFile;

		BOOL bResult = LocalFile.Open(strFileName, CFile::modeRead | CFile::typeBinary);
		if (!bResult)
		{
			//MessageBox("打开本地文件失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("打开本地文件失败，该文件可能正被其他程序使用。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();

			m_bDropFile = false;
			m_bSending = false;
			return;
		}

		ULONGLONG length = LocalFile.GetLength();
		if (length == 0)
		{
			CString strMsg = _T("不支持发送0字节的文件。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();

			m_bDropFile = false;
			m_bSending = false;
			return;
		}

		FILETIME fileTime;
		bResult = GetFileTime(LocalFile.m_hFile, NULL, NULL, &fileTime);
		if (!bResult)
		{
			//MessageBox("", "提示", MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("获取本地文件信息失败，该文件可能正被其他程序使用。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			LocalFile.Close();
			m_bDropFile = false;
			m_bSending = false;
			return;
		}

		InsertLogItem("发送本地文件 " + strFileName + " ");
		if (!this->SendFileC2SRequest())
		{
			//MessageBox("发送文件请求失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("发送文件请求失败。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			m_bDropFile = false;
			m_bSending = false;
			LocalFile.Close();
			return;
		}

		BasicFileInfo basicFileInfo;
		basicFileInfo.CreationTime = fileTime;
		basicFileInfo.Size = LocalFile.GetLength();
		//basicFileInfo.bOverWrite = gl_bOverWrite;
		if (gl_mapOverWrite.find(m_hWnd) != gl_mapOverWrite.end())
		{
			basicFileInfo.bOverWrite = gl_mapOverWrite[m_hWnd];
		}
		else
		{
			basicFileInfo.bOverWrite = false;
		}
		char* pFileName = (LPSTR)(LPCTSTR) (LocalFile.GetFileName());
		memset(&(basicFileInfo.FileName), 0, MAX_PATH);
		CopyMemory(&(basicFileInfo.FileName), pFileName, LocalFile.GetFileName().GetLength());
		int nSend = send(m_Socket, (char*)(&basicFileInfo), sizeof(basicFileInfo), 0);
		if (nSend <= 0)
		{
			//MessageBox("发送本地文件信息失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("发送本地文件信息失败。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			LocalFile.Close();
			m_bDropFile = false;
			m_bSending = false;
			return;
		}
		else
		{
			LocalFile.Close();
			if (IsWindow(m_FileProgress.GetSafeHwnd()))
			{
				ULONGLONG nDes = basicFileInfo.Size > 1024 * 1024 * 1024 ? 1024 * 1024 : 1024;	//file's size larger than 1G.
				m_FileProgress.SetRange32(0, (int)(basicFileInfo.Size / nDes));
				m_FileProgress.SetPos(0);
			}
			return;
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonSend unknow exception.");
	}
}

bool CFileClientDialog::SendFileS2CRequest()
{
	try
	{
		PostMessage(WM_FILE_DISABLEBUTTON_MESSAGE, (WPARAM)0, 0);
		PostMessage(WM_SHOWSTOPBUTTON, (WPARAM)false, 0);
		bool bRes = PackAndSendRequest(CMD_PARAM_TRANSFILE_S2C_REQUEST);
		//***Pan peng 2008-12-01
		if(bRes)
		{
			FileRequest pathfile;
			strncpy(pathfile.FilePath, m_strRemoteFolderPath, MAX_PATH-1);
			POSITION pos = m_RemoteFileCtrl.GetFirstSelectedItemPosition();
			if (!pos) return false;
			int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
			CString strItem = m_RemoteFileCtrl.GetItemText(iItem, 0);

			strncpy(pathfile.FilePath, m_strRemoteFolderPath, MAX_PATH-1);
			strncpy(pathfile.FileName, strItem, MAX_PATH-1);
			int nSent = send(m_Socket, (char*)(&pathfile), sizeof(pathfile), 0);
			if (nSent <= 0 || sizeof(pathfile) != nSent)
				return false;
		}
		return true;
		//end***
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SendFileS2CRequest unknow exception.");
	}
	return false;
}

bool CFileClientDialog::SendFileC2SRequest()
{
	try
	{
		PostMessage(WM_FILE_DISABLEBUTTON_MESSAGE, (WPARAM)0, 0);
		PostMessage(WM_SHOWSTOPBUTTON, (WPARAM)false, 0);
		bool bRes = PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_REQUEST);
		//***Pan peng 2008-12-01
		if(bRes)
		{
			FileRequest pathfile;
			strncpy(pathfile.FilePath, m_strRemoteFolderPath, MAX_PATH-1);
			if(!m_bDropFile)
			{
				POSITION pos = m_LocalFileCtrl.GetFirstSelectedItemPosition();
				if (!pos) return false;
				int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
				CString strItem = m_LocalFileCtrl.GetItemText(iItem, 0);
				strncpy(pathfile.FilePath, m_strRemoteFolderPath, MAX_PATH-1);
				strncpy(pathfile.FileName, strItem, MAX_PATH-1);
			}
			else
			{
				m_bDropFile = false;
				int nfind = this->m_strDropPathFileName.ReverseFind('\\');
				if(nfind != -1)
				{
					CString strFileName = m_strDropPathFileName.Right(m_strDropPathFileName.GetLength() - nfind - 1);
					CString strPathFile = m_strRemoteFolderPath;
					strcpy_s(pathfile.FileName, sizeof(pathfile.FileName), strFileName);
					strcpy_s(pathfile.FilePath, sizeof(pathfile.FilePath), strPathFile);
				}
				else
					return false;
			}
			int nSent = send(m_Socket, (char*)(&pathfile), sizeof(pathfile), 0);
			if (nSent <= 0 || sizeof(pathfile) != nSent)
				return false;
		}
		return true;
		//end***
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SendFileC2SRequest unknow exception.");
	}
	return false;
}

bool CFileClientDialog::PackAndSendRequest(short nParam)
{
	try
	{
		FilePackHeader header;
		header.nType = FILE_REQUEST;
		header.nParam = nParam;

		int nSent = send(m_Socket, (char*)(&header), sizeof(header), 0);
		if (nSent <= 0)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("CFileClientDialog::PackAndSendRequest nSent <= 0, nParam: %d", nParam);
			return false;
		}

		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::PackAndSendRequest unknow exception.");
	}
	return false;
}

void CFileClientDialog::OnBnClickedButtonDelete()
{
	try
	{
		POSITION pos = m_LocalFileCtrl.GetFirstSelectedItemPosition();
		if (!pos) return;

		int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
		CString strItem = m_LocalFileCtrl.GetItemText(iItem, 0);
		int nLeft = strItem.Find("(");
		int nRight = strItem.Find(")");

		if (nLeft == 0 && nRight == strItem.GetLength() - 1)
		{
			strItem = strItem.Mid(1, strItem.GetLength() - 2);
		}

		CString strInfo = "您确定要删除 " + strItem + " 吗？";
		int nResult = MessageBox(strInfo, "提示", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);

		if (nResult == IDNO)
		{
			return;
		}
		else
		{
			CString strFileName = this->m_LocalPath + "\\" + strItem;

			int nFileType = GetFileAttributes(strFileName);
			if (nFileType == FILE_ATTRIBUTE_DIRECTORY)//目录是文件夹
			{
				if (!DeleteDirectory((LPSTR)(LPCTSTR) strFileName))
				{
					//MessageBox("删除目录失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
					CString strMsg = _T("删除目录失败。");
					m_PromptDlg.SetMsgText(strMsg);
					m_PromptDlg.Show();
					InsertLogItem("删除本地目录失败 " + strFileName + " ");
				}
				else
				{
					InsertLogItem("删除本地目录 " + strFileName + " ");
					ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
				}
			}
			else
			{
				if (!DeleteFile(strFileName))
				{
					//MessageBox("删除文件失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
					CString strMsg = _T("删除文件失败。");
					m_PromptDlg.SetMsgText(strMsg);
					m_PromptDlg.Show();
					InsertLogItem("删除本地文件失败 " + strFileName + " ");
				}
				else
				{
					InsertLogItem("删除本地文件 " + strFileName + " ");
					ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
				}
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonDelete unknow exception.");
	}
}

void CFileClientDialog::HighLightItemInList(CListCtrl* pListCtrl, char* pItemName)
{
	try
	{
		LVFINDINFO findInfo;
		findInfo.flags = LVFI_STRING;
		findInfo.psz = pItemName;

		//int iIndex = pListCtrl->FindItem(&findInfo);
		CString strItem = pItemName;
		int iIndex = FindMyItem(pListCtrl, strItem);
		if (iIndex != -1)
		{
			pListCtrl->SetItemState(iIndex, LVIS_SELECTED, LVIS_SELECTED);
			pListCtrl->EnsureVisible(iIndex, FALSE);
			pListCtrl->SetFocus();
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::HighLightItemInList unknow exception.");
	}
}

void CFileClientDialog::OnBnClickedButtonNew()
{
	try
	{
		CFolderNameBuilder m_NameBuilder;
		if (m_NameBuilder.DoModal() == IDCANCEL) return;

		CString FolderName = m_NameBuilder.GetFolderName();
		if (FolderName.GetLength() > 0)
		{
			CString strFullPath = this->m_LocalPath + "\\" + FolderName;
			CDestFolderHelper folderHelper;
			folderHelper.SetDestFolder((LPSTR)(LPCTSTR) strFullPath);
			if (folderHelper.IsDestFolderExist())
			{
				//MessageBox("指定目录 " + strFullPath + " 已经存在。", "提示", MB_OK | MB_ICONINFORMATION);
				CString strMsg = _T("指定目录 ") + strFullPath + _T(" 已经存在。");
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				return;
			}
			else
			{
				int nResult = ::CreateDirectory(strFullPath, NULL);
				if (nResult == 0)
				{
					//MessageBox("创建指定目录失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
					CString strMsg = _T("创建指定目录失败。 ");
					m_PromptDlg.SetMsgText(strMsg);
					m_PromptDlg.Show();
					InsertLogItem("创建指定目录失败 " + strFullPath + " ");
					return;
				}
				else
				{
					char* pFolderName = (LPSTR)(LPCTSTR) m_LocalPath;
					this->ListContentInFolder(pFolderName);

					CString folder = "(" + FolderName + ")";

					InsertLogItem("创建本地目录 " + strFullPath + " ");

					this->HighLightItemInList(&m_LocalFileCtrl, (LPSTR)(LPCTSTR) folder);
				}
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonNew unknow exception.");
	}
}

void CFileClientDialog::InsertLogItem(CString LogItem)
{
	try
	{
		CTime time = CTime::GetCurrentTime();

		CString strTime = time.Format("--- %H:%M:%S");

		int nIndex = m_ComboHistory.AddString(LogItem + strTime);
		m_ComboHistory.SetCurSel(nIndex);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::InsertLogItem unknow exception.");
	}
}

LRESULT CFileClientDialog::OnFileToServerMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		EnableButtonsWhenTransferFile(false);
		// SendFile(); 
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnFileToServerMessage unknow exception.");
	}
	return 0;
}

void CFileClientDialog::SendFile()
{
	////***Pan peng 2008-12-01
	bool bRes = PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S);
	if(!bRes)
		return;
}


LRESULT CFileClientDialog::OnFileToServerFinishMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CString* strItem = (CString*) wParam;
		this->m_RemoteItemToHightLight = strItem->GetString();

		EnableButtonsWhenTransferFile(true);

		//this->OnBnClickedButtonRefRemote();

		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnFileToServerFinishMessage unknow exception.");
	}
	return 0;
}

LRESULT CFileClientDialog::OnFileToClientFinishMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		CString* strItem = (CString*) wParam;
		this->m_LocalItemToHightLight = strItem->GetString();
		//panpeng 2008-11-28
		m_FileProgress.SetPos(0);
		InsertLogItem("结束文件接收 ");
		//
		m_bDropFile = false;
		m_bSending = false;
		m_bReceive = false;
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnFileToClientFinishMessage unknow exception.");
	}
	return 0;
}

LRESULT CFileClientDialog::OnEnableButtonsMessage(WPARAM wParam, LPARAM lParam)
{
	EnableButtonsWhenTransferFile(true);
	return 0;
}

LRESULT CFileClientDialog::OnDisableButtonsMessage(WPARAM wParam, LPARAM lParam)
{
	m_bContinue = true;
	this->m_ContinueReceiving = true;
	EnableButtonsWhenTransferFile(false);
	return 0;
}

LRESULT CFileClientDialog::OnFileToServerProgessMessage(WPARAM wParam, LPARAM lParam)
{
	ULONGLONG nProgress = (ULONGLONG) wParam;

	if (IsWindow(m_FileProgress.GetSafeHwnd()))
	{
		//SetStatusText(szTransfering_CH);
		bool bRun = m_bReceive || m_bSending;
		this->m_FileProgress.SetPos(bRun==true?nProgress:0);
		if (!bRun)
		{
			if (gl_pLogger)
				gl_pLogger->log_info("OnFileToServerProgessMessage : has been stopped :%d.",nProgress);
		}
	}
	return 0;
}

LRESULT CFileClientDialog::OnGetLocalPathMessage(WPARAM wParam, LPARAM lParam)
{
	char* pBuf = (char*) wParam;
	CopyMemory(pBuf, this->m_LocalPath, m_LocalPath.GetLength());
	return 0;
}

void CFileClientDialog::EnableButtonsWhenTransferFile(bool bEnable)
{
	try
	{
		if (!bEnable)
		{
			//m_ComboLocalDrive.EnableWindow(bEnable);
			m_ComboDestDriver.EnableWindow(bEnable);
			//m_btNew.ShowWindow(SW_HIDE);
			//m_btDelete.ShowWindow(SW_HIDE);
			m_btSend.ShowWindow(SW_HIDE);
			//m_btUp.ShowWindow(SW_HIDE);
			//m_btRoot.ShowWindow(SW_HIDE);
			m_btRecv.ShowWindow(SW_HIDE);
			//m_btRefresh.ShowWindow(SW_HIDE);
			m_btRefreshRemote.ShowWindow(SW_HIDE);
			m_btUp1.ShowWindow(SW_HIDE);
			m_btRoot1.ShowWindow(SW_HIDE);
			m_StaticRemotePath.ShowWindow(SW_HIDE);
		}
		else
		{
			m_ComboLocalDrive.EnableWindow(bEnable);
			m_ComboDestDriver.EnableWindow(bEnable);
			m_btNew.ShowWindow(SW_SHOW);
			m_btDelete.ShowWindow(SW_SHOW);
			m_btSend.ShowWindow(SW_SHOW);
			m_btUp.ShowWindow(SW_SHOW);
			m_btRoot.ShowWindow(SW_SHOW);
			m_btRecv.ShowWindow(SW_SHOW);
			m_btRefresh.ShowWindow(SW_SHOW);
			m_btRefreshRemote.ShowWindow(SW_SHOW);
			m_btUp1.ShowWindow(SW_SHOW);
			m_btRoot1.ShowWindow(SW_SHOW);
			m_StaticRemotePath.ShowWindow(SW_SHOW);

			m_bSending = false;
			m_bReceive = false;
			m_bDropFile = false;
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::EnableButtonsWhenTransferFile unknow exception.");
	}
}

void CFileClientDialog::OnClose()
{
	try
	{
		gl_mapSenderStop[m_hWnd] = true;
		m_bSending = false;
		m_bReceive = false;
		this->m_ContinueReceiving = false;
		m_bContinue = false;
		CBaseDlg::OnClose();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnClose unknow exception.");
	}
}

LRESULT CFileClientDialog::OnSendingStopMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP);

		if (IsWindow(m_FileProgress.GetSafeHwnd()))
		{
			m_FileProgress.SetPos(0);
		}

		if (m_bCancel)
		{
			InsertLogItem("停止文件发送 ");

		}
		else
			InsertLogItem("结束文件发送 ");
		m_bCancel = false;
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnSendingStopMessage unknow exception.");
	}
	return 0;
}

void CFileClientDialog::OnBnClickedButtonStop()
{
	try
	{
		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		m_bCancel = true;

		if (IsWindow(m_FileProgress.GetSafeHwnd()))
		{
			m_FileProgress.SetPos(0);
			if (gl_pLogger)
				gl_pLogger->log_info("OnBnClickedButtonStop Init m_FileProgress.");
		}
		if (m_bSending == true)
		{
			//PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_STOP);
			//gl_SenderStop = true;
			gl_mapSenderStop[m_hWnd] = true;
			InsertLogItem("停止文件发送 ");
		}
		else
		{
			m_ContinueReceiving = false;			
			if (this->m_bReceive)
				InsertLogItem("停止文件接收 ");
		}
		m_bSending = false;
		m_bReceive = false;
		this->m_ContinueReceiving = false;
		//m_StopByClient = true;

		m_bContinue = false;		
		//SetStatusText(_T("状态：正在停止，请稍候..."));
		SetStatusText(_T(""));
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonStop unknow exception.");
	}
}

bool CFileClientDialog::WriteExact(SOCKET sock, char *buf, int bytes)
{
	try
	{
		if (sock == INVALID_SOCKET)
		{
			return false;
		}

		int nBuffered = 0;
		int nWrite;

		while (bytes > 0)
		{
			nWrite = 0;

			nWrite = send(sock, &buf[nBuffered], bytes, 0);

			if (nWrite <= 0)
			{
				return false;
			}

			bytes -= nWrite;
			nBuffered += nWrite;
		}

		return true;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::WriteExact unknow exception.");
	}
	return false;
}

void CFileClientDialog::OnBnClickedButtonReceive()
{
	try
	{
		// 从服务器接收文件

		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		if (m_bSending || m_bReceive)
		{
			CString strMsg = _T("已经有文件正在传输，请停止后再重试。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		//***Add by Pan peng 2008-12-05
		CString strLocalPathFile = this->m_LocalPath + _T("\\Web1800_TestDriverAvaliable.txt");
		if(!CreateTestFile(strLocalPathFile))
		{
			//AfxMessageBox("指定的本地路径不可用。");
			CString strMsg = _T("指定的本地路径不可用。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}
		//end***

		m_bSending = false;
		m_ContinueReceiving = true;
		m_bContinue = true;
		POSITION pos = m_RemoteFileCtrl.GetFirstSelectedItemPosition();
		if (!pos) 
		{
			CString strMsg = _T("请选择要接收的文件。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		int iItem = m_RemoteFileCtrl.GetNextSelectedItem(pos);
		CString strItem = m_RemoteFileCtrl.GetItemText(iItem, 0);
		int nLeft = strItem.Find("(");
		int nRight = strItem.Find(")");

		if (nLeft == 0 && nRight == strItem.GetLength() - 1)
		{
			CString strMsg = _T("不支持文件夹传输。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		SortItemData* pData = (SortItemData*)m_RemoteFileCtrl.GetItemData(iItem);
		if (pData == NULL) return;
		ULONGLONG uSize = pData->ullSize;
		if (uSize == 0)
		{
			CString strMsg = _T("不支持传输0大小文件。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		LVFINDINFO findInfo;
		findInfo.flags = LVFI_STRING;
		findInfo.psz = (LPSTR)(LPCTSTR)strItem;

		//if (m_LocalFileCtrl.FindItem(&findInfo) != -1)
		if (FindMyItem(&m_LocalFileCtrl, strItem) != -1)
		{
			CString strInfo = "已经存在同名文件，点“是”覆盖原文件，\n\r点“否”开始断点续传，点“取消”终止本次操作。";
			int nResult = MessageBox(strInfo, "提示", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);

			if (nResult == IDYES)
			{
				//gl_llSize = 0;
				gl_mapllSize[m_hWnd] = 0;
			}
			else if (nResult == IDNO)
			{
				CString strFileName = m_LocalPath;
				strFileName = strFileName + _T("\\");
				strFileName = strFileName + strItem;
				CFile file;
				if (!file.Open(strFileName, CFile::modeRead))
					//gl_llSize = 0;
					gl_mapllSize[m_hWnd] = 0;
				else
				{
					//gl_llSize = file.GetLength();
					gl_mapllSize[m_hWnd] = file.GetLength();
					file.Close();
				}
			}
			else if (nResult == IDCANCEL)
			{
				return;
			}
		}

		InsertLogItem("接收远程文件 " + strItem + " ");

		if (!this->SendFileS2CRequest())
		{
			//MessageBox("发送文件请求失败。","提示",MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("发送文件请求失败。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		FileToSend fileInfo;
		memset(fileInfo.FileName,0,MAX_PATH);
		CopyMemory(fileInfo.FileName, (LPSTR)(LPCTSTR)strItem, strItem.GetLength());
		ULONGLONG ullSize = 0;
		if (gl_mapllSize.find(m_hWnd) != gl_mapllSize.end())
		{
			ullSize = gl_mapllSize[m_hWnd];
		}
		fileInfo.llStart = ullSize;
		//fileInfo.llStart = gl_llSize;
		if (!WriteExact(m_Socket, (char*)(&fileInfo), sizeof(fileInfo)))
		{
			//MessageBox("发送远程文件信息失败。", "提示", MB_OK | MB_ICONEXCLAMATION);
			CString strMsg = _T("发送远程文件信息失败。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}
		else
		{
			m_strFileNameToReceive = fileInfo.FileName;
			return;
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonReceive unknow exception.");
	}
}

LRESULT CFileClientDialog::OnRefreshLocalPathMessage(WPARAM wParam, LPARAM lParam)
{
	ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
	return 0;
}

LRESULT CFileClientDialog::OnGetRecvStatusMessage(WPARAM wParam, LPARAM lParam)
{
	RecvFileResponse* response = (RecvFileResponse*) wParam;
	response->ReadToGo = this->m_ContinueReceiving;
	return 0;
}

LRESULT CFileClientDialog::OnSetRangeMessage(WPARAM wParam, LPARAM lParam)
{
	if (IsWindow(m_FileProgress.GetSafeHwnd()))
	{
		m_FileProgress.SetRange32(0, (int) wParam);
		m_FileProgress.SetPos(0);
	}

	return 0;
}

void CFileClientDialog::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);	
	PostMessage(WM_CLOSE,0,0);
}

LRESULT CFileClientDialog::OnAddLogMessage(WPARAM wParam, LPARAM lParam)
{
	CString* pString = (CString*) wParam;
	this->InsertLogItem(pString->GetString());

	return 0;
}
void CFileClientDialog::OnBnClickedButtonRefresh()
{
	try
	{
		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		ListContentInFolder((LPSTR)(LPCTSTR) m_LocalPath);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnBnClickedButtonRefresh unknow exception.");
	}
}

void CFileClientDialog::OnBnClickedButtonRefRemote()
{
	SendListFileRequest();
	if(GetPrompt(m_hWnd))
	{
		SetStatusText(_T("状态：先响应提示再操作。"));
		SetForegroundWindow();
		return;
	}
}

void CFileClientDialog::OnLvnItemchangedListLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: 在此添加控件通知处理程序代码
		POSITION pos = m_LocalFileCtrl.GetFirstSelectedItemPosition();
		if (!pos) return;

		int iItem = m_LocalFileCtrl.GetNextSelectedItem(pos);
		CString strItem = m_LocalFileCtrl.GetItemText(iItem, 0);

		this->m_LocalItemToHightLight = strItem;

		*pResult = 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnLvnItemchangedListLocal unknow exception.");
	}
	*pResult = 0;
}

void CFileClientDialog::OnLvnItemchangedListRemote(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: 在此添加控件通知处理程序代码
		POSITION pos = m_RemoteFileCtrl.GetFirstSelectedItemPosition();
		if (!pos) return;

		int iItem = m_RemoteFileCtrl.GetNextSelectedItem(pos);
		CString strItem = m_RemoteFileCtrl.GetItemText(iItem, 0);

		this->m_RemoteItemToHightLight = strItem;
		*pResult = 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnLvnItemchangedListRemote unknow exception.");
	}
	*pResult = 0;
}

BOOL CFileClientDialog::DeleteDirectory(char* strDirectory)
{
	try
	{
		CFileFind fileFind;
		char strFileFind[MAX_PATH];
		sprintf_s(strFileFind, "%s\\*.*", strDirectory);

		BOOL bFinded = fileFind.FindFile(strFileFind);
		while (bFinded)
		{
			bFinded = fileFind.FindNextFile();

			if (!fileFind.IsDots())
			{
				char strFoundFile[MAX_PATH];
				sprintf_s(strFoundFile, "%s\\%s", strDirectory, fileFind.GetFileName());

				if (fileFind.IsDirectory())
				{
					DeleteDirectory(strFoundFile);
				}
				else
				{
					DeleteFile(strFoundFile);
				}
			}
		}

		fileFind.Close();

		return RemoveDirectory(strDirectory);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::DeleteDirectory unknow exception.");
	}
	return false;
}

CString CFileClientDialog::GetCurFileName()
{
	CString str;
	str.Empty();
	if (m_RemoteItemToHightLight.GetLength() > 0)
	{
		str = m_strDistinationFolder;
		str += "\\";
		str += m_RemoteItemToHightLight;
	}
	return str;
}

//***Add by Pan peng 2008-11-28
LRESULT CFileClientDialog::OnReceiveDrivers(WPARAM wParam, LPARAM lParam)
{
	try
	{
		Driver *driver = (Driver *)wParam;
		int nCount = (int)lParam;
		if(NULL == driver || nCount <= 0)
			return -1;

		while(m_ComboDestDriver.DeleteString(0) != -1)
			m_ComboDestDriver.DeleteString(0);
		int mainDisk = 0;
		for(int i = 0; i < nCount; i ++)
		{
			CString strDriver = driver[i].DriverName;
			m_ComboDestDriver.AddString(strDriver);
			int f = strDriver.Find(_T(")"),0);
			strDriver = strDriver.Left(f);
			if(strDriver.Find("C") != -1)
				mainDisk = i;
		}
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnReceiveDrivers unknow exception.");
	}

	return 0;
}

bool CFileClientDialog::SendGetDriverRequest()
{
	if (this->m_bEnableRemotePath)
		InsertLogItem("获取远程驱动列表 ");
	OutputDebugString("CFileClientDialog::SendGetDriverRequest()\n");
	return PackAndSendRequest(CMD_PARAM_DRIVER);
}

LRESULT CFileClientDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch(message)
		{
		case WM_REFRESHLOCALLIST:
			{
				CString *str = (CString *)lParam;
				CString strFullName = *str;
				CString strDrive = strFullName;
				strDrive.SetAt(2, 0);
				for (int i = 0; i < this->m_ComboLocalDrive.GetCount(); i ++)
				{
					CString strItem;
					m_ComboLocalDrive.GetLBText(i, strItem);
					if (strItem.Find(strDrive) == 1)
					{
						m_ComboLocalDrive.SetCurSel(i);
						break;
					}
				}
				CString strLog = _T("接收远程文件 ");
				strLog = strLog + strFullName;
				this->InsertLogItem(strLog);
				int nFind = strFullName.ReverseFind('\\');
				if (nFind > 0)
				{
					strFullName = strFullName.Left(nFind);
					this->ListContentInFolder(strFullName.GetBuffer());
				}
				this->m_LocalPath = strFullName + _T("\\");

			}
			break;
		case WM_GETSENDING:
			{
				bool *bSend = (bool *)wParam;
				if(bSend)
					*bSend = m_bSending;
			}
			break;
		case WM_GETRECEIVE:
			{
				bool *bReceive = (bool *)wParam;
				if(bReceive)
					*bReceive = m_bReceive;
			}
			break;
		case WM_FREEUI:
			{
				m_bSending = false;
				m_bReceive = false;
				SetStatusText(_T("状态：同时请求，默认拒绝。"));
				EnableButtonsWhenTransferFile(true);
			}
			break;
		case WM_FILE_LISTFILE_MESSAGE:
			return OnFileTransferMessage(wParam, lParam);
		case WM_GETFILENAMETORECEIVE:
			{
				CString *strFileNameToReceive = (CString *)lParam;
				if(strFileNameToReceive != NULL)
					*strFileNameToReceive = m_strFileNameToReceive;
			}
			break;
		case WM_GETCURRENTPATH:
			{
				CString *strPath = (CString *)lParam;
				if(strPath != NULL)
					*strPath = m_LocalPath;
			}
			break;
		case WM_FILE_OPERATE_BEGIN:
			{
				EnableButtonsWhenTransferFile(false);
				SetStatusText(szTransfering_CH);
				if(wParam == 1)
					m_bSending = true;
				else
					m_bReceive = true;
			}
			break;
		case WM_FILE_OPERATE_END:
			{
				SetStatusText(_T(""));
				EnableButtonsWhenTransferFile(true);
				OnFileToServerProgessMessage(0, 0);
				this->m_bSending = false;
				this->m_bReceive = false;
				this->OnBnClickedButtonRefRemote();
				this->OnBnClickedButtonRefresh();
				this->Invalidate();
				CString strMsg = GenNoticeMsg((FileState)wParam, lParam);
				if (gl_pLogger) gl_pLogger->log_info("SendFile return %s", strMsg);
				if (strMsg != "")
				{
					m_PromptDlg.SetMsgText(strMsg);
					m_PromptDlg.Show();
				}
			}
			break;
		case WM_GETFILENAMETOSEND:
			{
				CString *pFileName = (CString *)lParam;
				if(pFileName != NULL)
					*pFileName = m_strFileNameToSend;
			}
			break;
		case WM_GETSENDSTATE:
			{
				bool **pState = (bool **)lParam;
				if(pState != NULL)
					*pState  = &m_bContinue;
			}
			break;
		case WM_CLOSEPROMPTDLG:
			if (this->m_bSending || this->m_bReceive)
				SetStatusText(szTransfering_CH);
			else
				SetStatusText(_T(""));
			GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
			break;
		case WM_SETMSGTEXT:
			{
				m_strMsg = _T("");
				CString *pStringMsg = (CString *)lParam;
				m_strMsg = *pStringMsg;
			}
			break;
		case WM_SHOWMSGTEXT:
			m_PromptDlg.SetMsgText(m_strMsg);
			m_PromptDlg.Show();
			InsertLogItem(m_strMsg);
			break;
		case WM_FILE_STOP:
			{
				if(m_bCancel)
					break;
				CString strMsg = _T("对方停止了当前文件传输");
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
			}
			break;
		case WM_FILE_ERROR:
			{
				CString strMsg;
				if(wParam == 0)
				{
					strMsg = _T("无法发送文件至远端，请确认媒体可写。");
				}
				else if(wParam == 1)
				{
					strMsg = _T("发送本地文件失败(对方文件访问出错)。");
				}
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
			}
			break;
		case WM_REJECT_SENDREQUEST:
			{
				CString strMsg;
				if(wParam == 0)
					strMsg = _T("对方拒绝了您的提取请求");
				else if(wParam == 1)
					strMsg = _T("对方拒绝了您的发送请求");
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
			}
			break;

		case WM_FILE_DESKTOP_MESSAGE:
			{
				FileToSend *pDesktop = (FileToSend *)lParam;
				if(pDesktop != NULL)
				{
					this->m_strRemoteFolderPath = pDesktop->FileName;
					m_StaticRemotePath.SetWindowTextA(m_strRemoteFolderPath);

					m_ComboDestDriver.SetCurSel(0);
					if (m_Socket == INVALID_SOCKET)
						m_Socket = (SOCKET)wParam;

					SendListFileRequest();
				}
			}
			break;
		case WM_SETSTOPBYMYSEFLT:
			{
				m_StopByClient = lParam == 1 ? true : false;
			}
			break;
		case WM_GETSTOPBYMYSEFLT:
			{
				bool *pStopByClient = (bool *)lParam;
				*pStopByClient = m_StopByClient;
			}
			break;
		case WM_SETSENDING:
			m_bSending = wParam == 1 ? true : false;
			this->EnableButtonsWhenTransferFile(!m_bSending);
			break;
		case WM_SETRECEIVE:
			{
				m_bReceive = wParam == 1 ? true : false;
				this->EnableButtonsWhenTransferFile(!m_bReceive);
			}
			break;
		case WM_FILE_LISTDRIVERS_MESSAGE:
			PostMessage(WM_SHOWSTOPBUTTON, 0, 0);
			OnReceiveDrivers(wParam, lParam);
			PostMessage(WM_SHOWSTOPBUTTON, 1, 0);
			break;
		case WM_FILE_SENDFILETOSERVER_FINISHED_MESSAGE:
			//Sleep(500);
			//this->OnBnClickedButtonRefRemote();
			m_FileProgress.SetPos(0);
			break;
		case WM_SHOWSTOPBUTTON:
			{
				bool bShow = (wParam != 0);
				//SetStatusText(_T("状态:已经发送请求，等待用户回应。"));
				CWnd *pWnd = GetDlgItem(ID_BUTTON_STOP);
				if (pWnd && IsWindow(pWnd->m_hWnd))
					pWnd->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
			}
			break;
		case WM_MAKETUNNELEND:
			{
				bool bSuccess = (bool)wParam;
				TunnelMode mode = (TunnelMode)lParam;
				OnMakeTunnelEnd(bSuccess, mode);
				return 0;
			}
			break;
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::WindowProc unknow exception. message: %d", message);
	}
	return CBaseDlg::WindowProc(message, wParam, lParam);
}

void CFileClientDialog::OnCbnSelchangeComboDestdriver()
{
	try
	{
		if(GetPrompt(m_hWnd))
		{
			GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		char ofDir[MAX_PATH];
		m_ComboDestDriver.GetWindowTextA(ofDir, MAX_PATH - 1);

		if (strcmp(ofDir, " 桌面"))
		{
			ofDir[4] = '\0'; // Hum...
			m_strRemoteFolderPath.Format("%s", ofDir);
			m_strRemoteFolderPath.Replace("(", "");
			m_strRemoteFolderPath.Replace(")", "");
		}
		else
		{
			char strDesktop[MAX_PATH];
			LPITEMIDLIST pidl;
			SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &pidl);
			SHGetPathFromIDList(pidl, strDesktop);
			m_strRemoteFolderPath.Format("%s", strDesktop);
		}

		m_StaticRemotePath.SetWindowTextA(m_strRemoteFolderPath);

		SendListFileRequest();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnCbnSelchangeComboDestdriver unknow exception.");
	}
}

void CFileClientDialog::OnNMDblclkListRemote(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		if (pNMHDR->hwndFrom == this->m_RemoteFileCtrl.GetSafeHwnd())
		{
			POSITION pos = m_RemoteFileCtrl.GetFirstSelectedItemPosition();
			if (!pos) return;

			int iItem = m_RemoteFileCtrl.GetNextSelectedItem(pos);
			CString strItem = m_RemoteFileCtrl.GetItemText(iItem, 0);

			if (strItem.Left(1) == '(' && strItem.Right(1) == ')')
			{
				strItem = strItem.Mid(1, strItem.GetLength() - 2);
			}
			else
				return;

			if (m_strRemoteFolderPath.GetLength() > 1)
			{
				m_strRemoteFolderPath += "\\";
			}

			m_strRemoteFolderPath += strItem;
			m_StaticRemotePath.SetWindowTextA(m_strRemoteFolderPath);

			this->SendListFileRequest();
		}
		*pResult = 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnNMDblclkListRemote unknow exception.");
	}
}

bool CFileClientDialog::CreateTestFile(LPCTSTR szFileName)
{
	CFile file;
	if(file.Open(szFileName, CFile::modeCreate))
	{
		file.Close();
		::DeleteFile(szFileName);
		return true;
	}
	return false;
}
//end***

void CFileClientDialog::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	try
	{
		CDialog::OnDropFiles(hDropInfo);

		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		if(m_bSending)
		{
			//AfxMessageBox("已经有文件正在发送,请稍候发送。");
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}
		if(m_bReceive)
		{
			//AfxMessageBox("已经有文件正在接收,请稍候发送。");
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		char lpszFileName[MAX_PATH]; 
		int nFileCount, k; 

		nFileCount = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);

		CString strPathFileName;
		int filecount = 0;
		int foldercount = 0;
		for(k = 0; k < nFileCount; k++) 
		{ 
			DragQueryFile(hDropInfo, k, lpszFileName, MAX_PATH); 
			CFileFind filefind;
			if(filefind.FindFile(lpszFileName))
			{
				filefind.FindNextFile();
				if(filefind.IsDirectory())
				{
					foldercount ++;
					continue;
				}
				strPathFileName = filefind.GetFilePath();
				filecount ++;
			}
		} 
		if(filecount > 1)
		{
			//MessageBox("一次只能传送单个文件。");
			CString strMsg = _T("一次只能传送单个文件。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		if(foldercount > 0)
		{
			//MessageBox("不支持文件夹传输。");
			CString strMsg = _T("不支持文件夹传输。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		m_bDropFile = true;
		m_strDropPathFileName = strPathFileName;

		this->OnBnClickedButtonSend();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnDropFiles unknow exception.");
	}
}

void CFileClientDialog::OnTimer(UINT_PTR nIDEvent)
{
	try
	{
		/*if(!IsActive())
		{
			this->OnBnClickedButtonStop();
			int count = 0;
			while((this->m_bSending || this->m_bReceive) && count < 30)
			{
			
				Sleep(100);
				count ++;
			}
			this->OnOK();
		}*/
		/*CTime t = CTime::GetTickCount();
		if (t - m_LastRebuildTime > 20 && bTest)
		{
			::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_REBUILDCHANNEL, 0);
			OutputDebugString("::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_REBUILDCHANNEL, 0);");
			this->Stop();
			bTest = false;
		}*/
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnTimer unknow exception.");
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CFileClientDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)   
	{   
		int nVirtKey = (int)pMsg->wParam;           
		if(nVirtKey == VK_RETURN)   
		{   
			return TRUE;   
		}   
		if(nVirtKey == VK_ESCAPE)   
		{    
			return TRUE;   
		}   
	} 

	if (pMsg->message == WM_CHAR || pMsg->message == WM_KEYDOWN || pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_MOUSEMOVE)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_EDIT_LOCALFPATH)->m_hWnd)
		{
			CRect rect;
			GetDlgItem(IDC_EDIT_LOCALFPATH)->GetWindowRect(&rect);
			this->ScreenToClient(&rect);
			this->InvalidateRect(rect);
		}
	}
	
	//return m_ddm.PreTranslateMessage(pMsg) ? TRUE :
	return CDialog::PreTranslateMessage(pMsg);
}

void CFileClientDialog::InitUi()
{
	try
	{
		SetDlgReign();

		if (m_UIHelper)
		{
			delete m_UIHelper;
			m_UIHelper = NULL;
		}
		UINT uId = m_bEnableRemotePath ? IDR_GIFMAPBG : IDR_GIFMAPBG_EX;
		m_UIHelper = new CUIHelper(uId);
		if (m_bEnableRemotePath)
		{
			this->GetDlgItem(IDC_BUTTON_ROOT1)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDC_BUTTON_UP1)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BUTTON_ROOT1)->MoveWindow(1000, 1000, 10, 10);
			GetDlgItem(IDC_BUTTON_UP1)->MoveWindow(1000, 1000, 10, 10);
		}
		else
		{
			this->GetDlgItem(IDC_COMBO_DESTDRIVER)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDC_LIST_REMOTE)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDC_BUTTON_ROOT1)->ShowWindow(SW_HIDE);
			this->GetDlgItem(IDC_BUTTON_UP1)->ShowWindow(SW_HIDE);
			CRect rect;
			GetDlgItem(IDC_LIST_LOCAL)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_LIST_LOCAL)->MoveWindow(rect.left, rect.top, rect.Width() + 162, rect.Height()); 
			this->GetDlgItem(IDC_BUTTON_REF_REMOTE)->ShowWindow(SW_HIDE);

			this->GetDlgItem(IDC_COMBO_LOCALDRIVE)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_COMBO_LOCALDRIVE)->MoveWindow(rect.left, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(IDC_BUTTON_ROOT)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_BUTTON_ROOT)->MoveWindow(rect.left + 132, rect.top - 6, rect.Width(), rect.Height());
			this->GetDlgItem(IDC_BUTTON_UP)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_BUTTON_UP)->MoveWindow(rect.left + 132, rect.top - 6, rect.Width(), rect.Height());
			this->GetDlgItem(IDC_BUTTON_REFRESH)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_BUTTON_REFRESH)->MoveWindow(rect.left + 132, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(IDC_LIST_LOCAL)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_LIST_LOCAL)->MoveWindow(rect.left, rect.top - 6, 465, rect.Height());

			this->GetDlgItem(IDC_STATIC_REMOTEPATH2)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_STATIC_REMOTEPATH2)->MoveWindow(rect.left, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(IDC_PROGRESS)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_PROGRESS)->MoveWindow(18, rect.top - 6, 462, rect.Height());

			this->GetDlgItem(IDC_BUTTON_MIN)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_BUTTON_MIN)->MoveWindow(rect.left - 206, rect.top, rect.Width(), rect.Height());

			this->GetDlgItem(IDCANCEL)->GetWindowRect(&rect);
			this->GetDlgItem(IDCANCEL)->MoveWindow(rect.left - 206, rect.top, rect.Width(), rect.Height());

			this->GetDlgItem(ID_BUTTON_NEW)->GetWindowRect(&rect);
			this->GetDlgItem(ID_BUTTON_NEW)->MoveWindow(18, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(ID_BUTTON_DELETE)->GetWindowRect(&rect);
			this->GetDlgItem(ID_BUTTON_DELETE)->MoveWindow(137, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(ID_BUTTON_SEND)->GetWindowRect(&rect);
			this->GetDlgItem(ID_BUTTON_SEND)->MoveWindow(305, rect.top - 6, rect.Width(), rect.Height());

			this->GetDlgItem(ID_BUTTON_STOP)->GetWindowRect(&rect);
			this->GetDlgItem(ID_BUTTON_STOP)->MoveWindow(400, rect.top - 6, rect.Width(), rect.Height());

			int nStatusTop = rect.top - 6;
			this->GetDlgItem(IDC_STATIC_STATUS)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_STATIC_STATUS)->MoveWindow(18, nStatusTop, rect.Width(), rect.Height());

			this->GetDlgItem(IDC_COMBO_HISTORY)->GetWindowRect(&rect);
			this->GetDlgItem(IDC_COMBO_HISTORY)->MoveWindow(rect.left, rect.top - 18, 385, rect.Height());

			this->GetDlgItem(IDC_STATIC_REMOTEPATH3)->ShowWindow(SW_HIDE);
			this->GetDlgItem(ID_BUTTON_RECEIVE)->ShowWindow(SW_HIDE);

			this->GetDlgItem(IDC_STATIC_REMOTEPATH)->ShowWindow(SW_HIDE);

			this->m_LocalPathCtrl.GetWindowRect(&rect);
			this->m_LocalPathCtrl.MoveWindow(rect.left, rect.top - 6, rect.Width() + 132, rect.Height());

		}
		this->Invalidate();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::InitUi unknow exception.");
	}
}

void CFileClientDialog::SetStatusText(LPCTSTR strText)
{
	try
	{
		CWnd *pWnd = this->GetDlgItem(IDC_STATIC_STATUS);
		if (pWnd == NULL || strText == NULL)
			return;

		SetDlgItemText(IDC_STATIC_STATUS, strText);

		CRect rect;
		pWnd->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		this->InvalidateRect(&rect);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::SetStatusText unknow exception.");
	}
}

/*LRESULT CFileClientDialog::OnDragEnter(WPARAM wp, LPARAM lp)
{
	try
	{
		TRACE(_T(_T("CMyDlg::OnDragEnter, wp=%d\n")),wp);
		DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
		CListCtrl* plb = DYNAMIC_DOWNCAST(CListCtrl, GetDlgItem((int)wp));
		ASSERT(plb!=NULL);
		int item = GetLBItemUnderPt(plb, ddi.pt);
		if (item>=0) {
			CString text = plb->GetItemText(item, 0);
			ddi.data = new CDragDropText(text);
			return TRUE; // do drag-drop
		}
		return FALSE; // no item under mouse: nothing to drag
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnDragEnter unknow exception.");
	}
	return FALSE;
}

//////////////////
// User let go the mouse: drop data into child control.
//
LRESULT CFileClientDialog::OnDragDrop(WPARAM wp, LPARAM lp)
{
	try
	{
		TRACE(_T("CFileClientDialog::OnDragDrop\n"));
		DRAGDROPINFO& ddi = *(DRAGDROPINFO*)lp;
		LPCTSTR text = (LPCTSTR)ddi.data->OnGetData();

		if (wp == IDC_LIST_REMOTE) 
		{
			this->OnBnClickedButtonSend();
			//CListCtrl* plb = DYNAMIC_DOWNCAST(CListCtrl, GetDlgItem((int)wp));
			//ASSERT(plb!=NULL);

			//int iNew = GetLBItemUnderPt(plb, ddi.pt);
			//int iOld = plb->getlbt
			//if (iOld>=0) {
			//plb->DeleteString(iOld);
			//if (iOld < iNew)
			//iNew--;
			//}
			//if (iNew>=0)
			//plb->InsertString(iNew, text);
			//else
			//iNew = plb->AddString(text);

			//plb->SetCurSel(iNew);

		}
		else if (wp == IDC_LIST_LOCAL)
		{
			this->OnBnClickedButtonReceive();
		}
		else 
		{
			ASSERT(FALSE);
		}
		return 0;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnDragDrop unknow exception.");
	}
	return 0;
}

//////////////////
// Drag aborted (Eg, user pressed Esc).
//
LRESULT CFileClientDialog::OnDragAbort(WPARAM wp, LPARAM lp)
{
	TRACE(_T("CMyDlg::OnDragAbort\n"));
	return 0;
}

int CFileClientDialog::GetLBItemUnderPt(CListCtrl* pListCtrl, CPoint pt)
{
	try
	{
		UINT item = -1;
		for (int i = 0; i < pListCtrl->GetItemCount(); i ++)
		{
			CRect rect;
			if (pListCtrl->GetItemRect(i, &rect, LVIR_BOUNDS))
			{
				if (rect.PtInRect(pt))
				{
					item = i;
					break;
				}
			}
		}

		return item;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::GetLBItemUnderPt unknow exception.");
	}
	return -1;
}
*/
int gl_nRemoteIncrease[3] = {1,1,1};
int gl_nLocalIncrease[3] = {1,1,1};
LPARAM gl_lRemoteAddr = NULL;
int CALLBACK ListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	try
	{
		CSortListCtrl* pListCtrl = (CSortListCtrl*)lParamSort;
		SortItemData* pData1 = (SortItemData*)lParam1;
		SortItemData* pData2 = (SortItemData*)lParam2;
		int iFlag = 1;
		if (gl_lRemoteAddr == lParamSort)	//表示是远程列表排序
		{
			iFlag = gl_nRemoteIncrease[pListCtrl->m_nSortedCol];
		}
		else	//本地列表排序
		{
			iFlag = gl_nLocalIncrease[pListCtrl->m_nSortedCol];
		}
		switch(pListCtrl->m_nSortedCol)
		{
		case 0:
			{
				if (pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0) == 0)
				{
					if (pData1->ullSize - pData2->ullSize == 0)
						return CompareFileTime(&pData1->FileTime ,&pData2->FileTime);
					else 
						return pData1->ullSize - pData2->ullSize;
				}
				else
					return iFlag * (pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0));
			}
			break;
		case 1:
			{
				if (pData1->ullSize - pData2->ullSize == 0)
				{
					if (pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0) == 0)
						return CompareFileTime(&pData1->FileTime ,&pData2->FileTime);
					else
						return pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0);
				}
				else
					return iFlag * (pData1->ullSize - pData2->ullSize);
			}
			break;
		case 2:
			{
				if (CompareFileTime(&pData1->FileTime ,&pData2->FileTime) == 0)
				{
					if (pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0) == 0)
						return pData1->ullSize - pData2->ullSize;
					else
						return pData1->strFileName.GetAt(0) - pData2->strFileName.GetAt(0);
				}	
				else
					return iFlag * (CompareFileTime(&pData1->FileTime ,&pData2->FileTime));
			}
			break;
		default:
			break;
		}
		return 1;
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("ListCompare unknow exception.");
	}
	return 1;
}

void CFileClientDialog::OnLvnColumnclickListLocal(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		//if (pNMLV->iSubItem != this->m_LocalFileCtrl.m_nSortedCol)
		{
			m_LocalFileCtrl.m_fAsc = TRUE;
			m_LocalFileCtrl.m_nSortedCol = pNMLV->iSubItem;
			gl_nLocalIncrease[pNMLV->iSubItem] = gl_nLocalIncrease[pNMLV->iSubItem] * -1;
			m_LocalFileCtrl.SortItems(ListCompare, (DWORD_PTR)&m_LocalFileCtrl);
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnLvnColumnclickListLocal unknow exception.");
	}
	*pResult = 0;
}

void CFileClientDialog::OnLvnColumnclickListRemote(NMHDR *pNMHDR, LRESULT *pResult)
{
	try
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		//if (pNMLV->iSubItem != this->m_RemoteFileCtrl.m_nSortedCol)
		{
			m_RemoteFileCtrl.m_fAsc = TRUE;
			m_RemoteFileCtrl.m_nSortedCol = pNMLV->iSubItem;
			gl_lRemoteAddr = (LPARAM)&m_RemoteFileCtrl;
			gl_nRemoteIncrease[pNMLV->iSubItem] = gl_nRemoteIncrease[pNMLV->iSubItem] * -1;
			m_RemoteFileCtrl.SortItems(ListCompare, (DWORD_PTR)&m_RemoteFileCtrl);
		}
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnLvnColumnclickListRemote unknow exception.");
	}
	*pResult = 0;
}

void CFileClientDialog::ClearItemDatas(DataArray &datas)
{
	try
	{
		for(int i = 0; i < datas.GetSize(); i ++)
		{
			SortItemData* pData = datas.GetAt(i);
			delete pData;
		}
		datas.RemoveAll();
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::ClearItemDatas unknow exception.");
	}
}
void CFileClientDialog::OnEnKillfocusEditLocalfpath()
{
	try
	{
		CRect rect;
		GetDlgItem(IDC_EDIT_LOCALFPATH)->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		this->InvalidateRect(rect);
	}
	catch(...)
	{
		if (gl_pLogger)
			gl_pLogger->log_info("CFileClientDialog::OnEnKillfocusEditLocalfpath unknow exception.");
	}
}

void CFileClientDialog::OnMakeTunnelEnd(bool bSuccess, TunnelMode mode)
{
	try
	{
		CBaseDlg::OnMakeTunnelEnd(bSuccess, mode);
		if (bSuccess)
		{
			if (mode == CONTROL)
			{
				m_ControlThread.SetWnd(m_hWnd);
				if (!m_ControlThread.StartWorking())
				{
					if (gl_pLogger) gl_pLogger->log_info("!m_ControlThread.StartWorking()");
					OutputDebugString("!m_ControlThread.StartWorking()\n");
				}
				this->m_Socket = this->m_ClientCtrlSocket.GetWorkingSocket();
				SendGetDriverRequest();
			}
			else if(mode == SEND)
			{

			}
			else if(mode == RECEIVE)
			{

			}
			if (gl_pLogger) gl_pLogger->log_info("CFileClientDialog WM_MAKETUNNELEND Ok mode: %d", mode);
		}
		else
		{
			if (gl_pLogger) gl_pLogger->log_info("CFileClientDialog WM_MAKETUNNELEND failed mode: %d", mode);
			if (mode == CONTROL)
			{
				::PostMessage(m_MsgNotifyWnd, m_nQuitMessageID, FF_REBUILDCHANNEL, 0);
				this->Stop();
			}
		}
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileClientDialog::OnMakeTunnelEnd unkown exception. ");
	}
}

void CFileClientDialog::OnBegindragListLeft(NMHDR* pNMHDR, LRESULT* pResult)    
{   
    //This routine sets the parameters for a Drag and Drop operation.    
    //It sets some variables to track the Drag/Drop as well    
    // as creating the drag image to be shown during the drag.    
   
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;   
   
    //// Save the index of the item being dragged in m_nDragIndex    
    ////  This will be used later for retrieving the info dragged    
    int m_nDragIndex = pNMListView->iItem;   
   
    //// Create a drag image    
    POINT pt;   
    int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)    
    if(m_LocalFileCtrl.GetSelectedCount() > 1) //more than one item is selected    
    pt.x = nOffset;   
    pt.y = nOffset;   
   
    m_pDragImage = m_LocalFileCtrl.CreateDragImage(m_nDragIndex, &pt);   
    ASSERT(m_pDragImage); //make sure it was created    
    //We will call delete later (in LButtonUp) to clean this up    
   
    //CBitmap bitmap;   
    //if(m_LocalFileCtrl.GetSelectedCount() > 1) //more than 1 item in list is selected    
    //    //bitmap.LoadBitmap(IDB_BITMAP_MULTI);    
    //    bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);   
    //else   
    //    bitmap.LoadBitmap(IDB_BITMAP_BOX);   
    //m_pDragImage->Replace(0, &bitmap, &bitmap);   
   
    //// Change the cursor to the drag image    
    ////    (still must perform DragMove() in OnMouseMove() to show it moving)    
    m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));   
    m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);   
       
    //// Set dragging flag and others    
    m_bDragging = TRUE; //we are in a drag and drop operation    
    m_nDropIndex = -1;  //we don't have a drop index yet    
    m_pDragList = &m_LocalFileCtrl; //make note of which list we are dragging from    
    m_pDropWnd = &m_LocalFileCtrl;  //at present the drag list is the drop list    
   
    //// Capture all mouse messages    
    SetCapture ();   
   
    *pResult = 0;   
}

void CFileClientDialog::OnBegindragListRight(NMHDR* pNMHDR, LRESULT* pResult)    
{   
    //This routine sets the parameters for a Drag and Drop operation.    
    //It sets some variables to track the Drag/Drop as well    
    // as creating the drag image to be shown during the drag.    
   
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;   
   
    //// Save the index of the item being dragged in m_nDragIndex    
    ////  This will be used later for retrieving the info dragged    
    m_nDragIndex = pNMListView->iItem;   
   
    //// Create a drag image    
    POINT pt;   
    int nOffset = -10; //offset in pixels for drag image    
    pt.x = nOffset;   
    pt.y = nOffset;   
   
    m_pDragImage = m_RemoteFileCtrl.CreateDragImage(m_nDragIndex, &pt);   
    ASSERT(m_pDragImage); //make sure it was created    
    //We will call delete later (in LButtonUp) to clean this up    
   
    //    CBitmap bitmap;   
    //if(m_RemoteFileCtrl.GetSelectedCount() > 1) //more than 1 item in list is selected    
    //    //bitmap.LoadBitmap(IDB_BITMAP_MULTI);    
    //    bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);   
    //else   
    //    bitmap.LoadBitmap(IDB_BITMAP_BOX);   
    //m_pDragImage->Replace(0, &bitmap, &bitmap);   
   
    //// Change the cursor to the drag image    
    ////    (still must perform DragMove() in OnMouseMove() to show it moving)    
    m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));   
    m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);   
       
    //// Set dragging flag and others    
    m_bDragging = TRUE; //we are in a drag and drop operation    
    m_nDropIndex = -1;  //we don't have a drop index yet    
    m_pDragList = &m_RemoteFileCtrl; //make note of which list we are dragging from    
    m_pDropWnd = &m_RemoteFileCtrl;  //at present the drag list is the drop list    
   
    //// Capture all mouse messages    
    SetCapture ();   
   
    *pResult = 0;   
} 

void CFileClientDialog::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)   
{   
    //This routine performs the actual drop of the item dragged.    
    //It simply grabs the info from the Drag list (pDragList)    
    // and puts that info into the list dropped on (pDropList).    
    //Send: pDragList = pointer to CListCtrl we dragged from,    
    //      pDropList = pointer to CListCtrl we are dropping on.    
    //Return: nothing.    
   
    ////Variables    
   
   
    // Unhilight the drop target    
    //pDropList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);   
   
    ////Set up the LV_ITEM for retrieving item from pDragList and adding the new item to the pDropList    
    //char szLabel[256];   
    //LVITEM lviT;   
    //LVITEM* plvitem;   
   
    //ZeroMemory(&lviT, sizeof (LVITEM)); //allocate and clear memory space for LV_ITEM    
    //lviT.iItem      = m_nDragIndex;   
    //lviT.mask       = LVIF_TEXT;   
    //lviT.pszText        = szLabel;   
    //lviT.cchTextMax = 255;   
   
    //lvItem* pItem;   
    //lvItem lvi;   
    //lvi.plvi = &lviT;   
    //lvi.plvi->iItem = m_nDragIndex;   
    //lvi.plvi->mask = LVIF_TEXT;   
    //lvi.plvi->pszText = szLabel;   
    //lvi.plvi->cchTextMax = 255;   
   
    //if(pDragList->GetSelectedCount() == 1)   
    //{   
    //    // Get item that was dragged    
    //    pDragList->GetItem (lvi.plvi);   
    //    lvi.sCol2 = pDragList->GetItemText(lvi.plvi->iItem, 1);   
   
    //    // Delete the original item (for Move operation)    
    //    // This is optional. If you want to implement a Copy operation, don't delete.    
    //    // This works very well though for re-arranging items within a CListCtrl.    
    //    // It is written at present such that when dragging from one list to the other    
    //    //  the item is copied, but if dragging within one list, the item is moved.    
    //    if(pDragList == pDropList)   
    //    {   
    //        pDragList->DeleteItem (m_nDragIndex);   
    //        if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //decrement drop index to account for item    
    //                                                        //being deleted above it    
    //    }   
   
    //    // Insert item into pDropList    
    //    // if m_nDropIndex == -1, iItem = GetItemCount() (inserts at end of list), else iItem = m_nDropIndex    
    //    lvi.plvi->iItem = (m_nDropIndex == -1) ? pDropList->GetItemCount () : m_nDropIndex;   
    //    pDropList->InsertItem (lvi.plvi);   
    //    pDropList->SetItemText(lvi.plvi->iItem, 1, (LPCTSTR)lvi.sCol2);   
   
    //    // Select the new item we just inserted    
    //    pDropList->SetItemState (lvi.plvi->iItem, LVIS_SELECTED, LVIS_SELECTED);   
    //}   
    //else //more than 1 item is being dropped    
    //{   
    //    //We have to parse through all of the selected items from the DragList    
    //    //1) Retrieve the info for the items and store them in memory    
    //    //2) If we are reordering, delete the items from the list    
    //    //3) Insert the items into the list (either same list or different list)    
   
    //    CList<lvItem*, lvItem*> listItems;   
    //    POSITION listPos;   
    //           
    //    //Retrieve the selected items    
    //    POSITION pos = pDragList->GetFirstSelectedItemPosition(); //iterator for the CListCtrl    
    //    while(pos) //so long as we have a valid POSITION, we keep iterating    
    //    {   
    //        plvitem = new LVITEM;   
    //        ZeroMemory(plvitem, sizeof(LVITEM));   
    //        pItem = new lvItem;   
    //        //ZeroMemory(pItem, sizeof(lvItem)); //If you use ZeroMemory on the lvItem struct, it creates an error when you try to set sCol2    
    //        pItem->plvi = plvitem;   
    //        pItem->plvi->iItem = m_nDragIndex;   
    //        pItem->plvi->mask = LVIF_TEXT;   
    //        pItem->plvi->pszText = new char; //since this is a pointer to the string, we need a new pointer to a new string on the heap    
    //        pItem->plvi->cchTextMax = 255;   
   
    //        m_nDragIndex = pDragList->GetNextSelectedItem(pos);   
   
    //        //Get the item    
    //        pItem->plvi->iItem = m_nDragIndex; //set the index in the drag list to the selected item    
    //        pDragList->GetItem(pItem->plvi); //retrieve the information    
    //        pItem->sCol2 = pDragList->GetItemText(pItem->plvi->iItem, 1);   
   
    //        //Save the pointer to the new item in our CList    
    //        listItems.AddTail(pItem);   
    //    } //EO while(pos) -- at this point we have deleted the moving items and stored them in memory    
   
    //    if(pDragList == pDropList) //we are reordering the list (moving)    
    //    {   
    //        //Delete the selected items    
    //        pos = pDragList->GetFirstSelectedItemPosition();   
    //        while(pos)   
    //        {   
    //            pos = pDragList->GetFirstSelectedItemPosition();   
    //            m_nDragIndex = pDragList->GetNextSelectedItem(pos);   
   
    //            pDragList->DeleteItem(m_nDragIndex); //since we are MOVING, delete the item    
    //            if(m_nDragIndex < m_nDropIndex) m_nDropIndex--; //must decrement the drop index to account    
    //                                                            //for the deleted items    
    //        } //EO while(pos)    
    //    } //EO if(pDragList...    
   
    //    //Iterate through the items stored in memory and add them back into the CListCtrl at the drop index    
    //    listPos = listItems.GetHeadPosition();   
    //    while(listPos)   
    //    {   
    //        pItem = listItems.GetNext(listPos);   
   
    //        m_nDropIndex = (m_nDropIndex == -1) ? pDropList->GetItemCount() : m_nDropIndex;   
    //        pItem->plvi->iItem = m_nDropIndex;   
    //        pDropList->InsertItem(pItem->plvi); //add the item    
    //        pDropList->SetItemText(pItem->plvi->iItem, 1, pItem->sCol2);   
   
    //        pDropList->SetItemState(pItem->plvi->iItem, LVIS_SELECTED, LVIS_SELECTED); //highlight/select the item we just added    
   
    //        m_nDropIndex++; //increment the index we are dropping at to keep the dropped items in the same order they were in in the Drag List    
    //        //If we dont' increment this, the items are added in reverse order    
    //    } //EO while(listPos)    
   
    //}   

	if(pDragList != pDropList)
	{

		if(GetPrompt(m_hWnd))
		{
			SetStatusText(_T("状态：先响应提示再操作。"));
			SetForegroundWindow();
			return;
		}

		if(m_bSending)
		{
			//AfxMessageBox("已经有文件正在发送,请稍候发送。");
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。 ");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}
		if(m_bReceive)
		{
			//AfxMessageBox("已经有文件正在接收,请稍候发送。");
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		if (!(::GetWindowLong(this->m_btSend.m_hWnd, GWL_STYLE) & WS_VISIBLE))
		{
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}

		POSITION pos = pDragList->GetFirstSelectedItemPosition();
		m_nDragIndex = pDragList->GetNextSelectedItem(pos); 
		CString strText = pDragList->GetItemText(m_nDragIndex, 0);
		if (pDropList == &m_LocalFileCtrl)
			OnBnClickedButtonReceive();
		else
			this->OnBnClickedButtonSend();
	}
}  

void CFileClientDialog::OnMouseMove(UINT nFlags, CPoint point)    
{   
    //While the mouse is moving, this routine is called.    
    //This routine will redraw the drag image at the present    
    // mouse location to display the dragging.    
    //Also, while over a CListCtrl, this routine will highlight    
    // the item we are hovering over.    
   
    //// If we are in a drag/drop procedure (m_bDragging is true)    
    if (m_bDragging)   
    {   
        //// Move the drag image    
        CPoint pt(point);   //get our current mouse coordinates    
        ClientToScreen(&pt); //convert to screen coordinates    
        m_pDragImage->DragMove(pt); //move the drag image to those coordinates    
        // Unlock window updates (this allows the dragging image to be shown smoothly)    
        m_pDragImage->DragShowNolock(false);   
   
        ////// Get the CWnd pointer of the window that is under the mouse cursor    
        CWnd* pDropWnd = WindowFromPoint (pt);   
        //ASSERT(pDropWnd); //make sure we have a window    
   
        ////// If we drag outside current window we need to adjust the highlights displayed    
        if (pDropWnd != m_pDropWnd)   
        {   
            if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight    
            {   
                TRACE("m_nDropIndex is -1\n");   
                CListCtrl* pList = (CListCtrl*)m_pDropWnd;   
                VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));   
                // redraw item    
                VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));   
                pList->UpdateWindow ();   
                m_nDropIndex = -1;   
            }   
            else //If we drag out of the CListCtrl altogether    
            {   
                TRACE("m_nDropIndex is not -1\n");   
                CListCtrl* pList = (CListCtrl*)m_pDropWnd;   
                int i = 0;   
                int nCount = pList->GetItemCount();   
                for(i = 0; i < nCount; i++)   
                {   
                    pList->SetItemState(i, 0, LVIS_DROPHILITED);   
                }   
                pList->RedrawItems(0, nCount);   
                pList->UpdateWindow();   
            }   
        }   
   
        //// Save current window pointer as the CListCtrl we are dropping onto    
        m_pDropWnd = pDropWnd;   
   
        //// Convert from screen coordinates to drop target client coordinates    
        pDropWnd->ScreenToClient(&pt);   
   
        //If we are hovering over a CListCtrl we need to adjust the highlights    
        if(pDropWnd->IsKindOf(RUNTIME_CLASS (CListCtrl)))   
        {              
            //Note that we can drop here    
            SetCursor(LoadCursor(NULL, IDC_ARROW));   
            UINT uFlags;   
            CListCtrl* pList = (CListCtrl*)pDropWnd;   
   
            // Turn off hilight for previous drop target    
           // pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);   
            // Redraw previous item    
            //pList->RedrawItems (m_nDropIndex, m_nDropIndex);   
               
            // Get the item that is below cursor    
            m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);   
            // Highlight it    
            //pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);   
            //// Redraw item    
            //pList->RedrawItems(m_nDropIndex, m_nDropIndex);   
            //pList->UpdateWindow();   
        }   
        else   
        {   
            //If we are not hovering over a CListCtrl, change the cursor    
            // to note that we cannot drop here    
            SetCursor(LoadCursor(NULL, IDC_NO));   
        }   
        // Lock window updates    
       m_pDragImage->DragShowNolock(true);   
    }   
           
    CDialog::OnMouseMove(nFlags, point);   
}   

void CFileClientDialog::OnLButtonUp(UINT nFlags, CPoint point)    
{   
    //This routine is the end of the drag/drop operation.    
    //When the button is released, we are to drop the item.    
    //There are a few things we need to do to clean up and    
    // finalize the drop:    
    //  1) Release the mouse capture    
    //  2) Set m_bDragging to false to signify we are not dragging    
    //  3) Actually drop the item (we call a separate function to do that)    
   
    //If we are in a drag and drop operation (otherwise we don't do anything)    
    if (m_bDragging)   
    {   
        // Release mouse capture, so that other controls can get control/messages    
        ReleaseCapture();   
   
        // Note that we are NOT in a drag operation    
        m_bDragging = FALSE;   
   
        // End dragging image    
        m_pDragImage->DragLeave (GetDesktopWindow ());   
        m_pDragImage->EndDrag ();   
        delete m_pDragImage; //must delete it because it was created at the beginning of the drag    
   
        CPoint pt (point); //Get current mouse coordinates    
        ClientToScreen (&pt); //Convert to screen coordinates    
        // Get the CWnd pointer of the window that is under the mouse cursor    
        CWnd* pDropWnd = WindowFromPoint (pt);   
        ASSERT (pDropWnd); //make sure we have a window pointer    
        // If window is CListCtrl, we perform the drop    
        if (pDropWnd->IsKindOf (RUNTIME_CLASS (CListCtrl)))   
        {   
            m_pDropList = (CListCtrl*)pDropWnd; //Set pointer to the list we are dropping on    
            DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop    
        }   
    }   
       
    CDialog::OnLButtonUp(nFlags, point);   
}  