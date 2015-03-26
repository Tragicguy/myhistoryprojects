// FileServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "FileServerDlg.h"
#include "FileDefines.h"

#include "GlobalFun.h"

#pragma warning(disable:4244)
#pragma warning(disable:4800)

// CFileServerDlg 对话框

IMPLEMENT_DYNAMIC(CFileServerDlg, CDialog)

CFileServerDlg::CFileServerDlg(CWnd* pParent /*=NULL*/)
	: CBaseDlg(CFileServerDlg::IDD, pParent)
{

	m_UIHelper = NULL;
	//m_pListWnd = NULL;

	m_hIcon = NULL;

	m_bReceive = false;
	m_bDropFile = false;
	m_bSending = false;
	m_bContinue = false;

	m_nListItemIndex = -1;
	m_bAddFile = false;
	//m_bExit = false;

	m_bAskExit = false;	

	m_pFileDlg = new CFileDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_EXPLORER, //| OFN_ALLOWMULTISELECT
						"All files(*.*)|*.*||", NULL);
}

CFileServerDlg::~CFileServerDlg()
{
	try
	{
		if (m_UIHelper != NULL)
		{
			delete m_UIHelper;
			m_UIHelper = NULL;
		}

		//ReleaseFileList();

		if (m_hIcon != NULL)
		{
			DestroyIcon(m_hIcon);
		}
		m_PromptDlg.DestroyWindow();
		m_ExitAskDlg.DestroyWindow();

		if (m_pFileDlg)
		{
			delete m_pFileDlg;
		}
	}
	catch(...){}
}

void CFileServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADDFILE, m_btAddFile);
	DDX_Control(pDX, IDC_BUTTON_SELALL, m_btSelAll);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BUTTON_STOPSERVER, m_btStop);
	DDX_Control(pDX, IDC_LIST_FILES, m_wndFileList);
	DDX_Control(pDX, IDC_PROGRESS1, m_FileProgress);
	DDX_Control(pDX, IDC_BUTTON_MINIMIZE, m_btMinimize);
	DDX_Control(pDX, IDC_BUTTON_CCLOSE, m_btCClose);
}


BEGIN_MESSAGE_MAP(CFileServerDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_ADDFILE, &CFileServerDlg::OnBnClickedButtonAddfile)
	//ON_BN_CLICKED(IDC_BUTTON_SELALL, &CFileServerDlg::OnBnClickedButtonSelall)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CFileServerDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CFileServerDlg::OnBnClickedButtonStop)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CFileServerDlg::OnHdnItemclickListFiles)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILES, &CFileServerDlg::OnNMClickListFiles)
	ON_MESSAGE(WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, &CFileServerDlg::OnFileToServerProgessMessage)
	ON_MESSAGE(WM_FILE_SETRANGE_MESSAGE, &CFileServerDlg::OnSetRangeMessage)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_STOPSERVER, &CFileServerDlg::OnBnClickedButtonStopserver)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, &CFileServerDlg::OnBnClickedButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_CCLOSE, &CFileServerDlg::OnBnClickedButtonCclose)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFileServerDlg 消息处理程序

void CFileServerDlg::OnCancel()
{
	this->OnClose();
}

void CFileServerDlg::OnClose()
{
	try
	{
		/*if(GetPrompt(m_hWnd))
		{
			GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_CH);
			else
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_EN);
			SetForegroundWindow();
			return;
		}*/

		//if (m_bAskExit)
		//	return;
		////if(this->m_bSending || this->m_bReceive)
		//{
		//	CString strMsg;
		//	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		//		strMsg = szExitAsk_CH;
		//	else
		//		strMsg = szExitAsk_EN;
		//	CPromptDlg ExitAskDlg;
		//	ExitAskDlg.SetType(OkCancelDlg);
		//	ExitAskDlg.SetMsgText(strMsg);
		//	m_bAskExit = true;
		//	int nRes = ExitAskDlg.DoModal();
		//	m_bAskExit = false;
		//	if (nRes != IDOK)
		//	{
		//		return;
		//	}
		//}

		/*gl_bInitiActiveExit = true;
		closesocket(this->m_Socket);
		this->m_bContinue = false;
		Sleep(500);
		m_bExit = true;
		this->EndDialog(0);*/
		m_bContinue = false;
		gl_mapSenderStop[m_hWnd] = true;
		closesocket(this->m_Socket);
		m_Socket = INVALID_SOCKET;		
		CBaseDlg::OnClose();
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnClose unknown exception.");
	}
}

BOOL CFileServerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		//return CDialog::OnEraseBkgnd(pDC);
		CRect rc;
		GetWindowRect(&rc);
		ScreenToClient(&rc);

		if (m_UIHelper != NULL)
		{
			m_UIHelper->SetBackPicture(pDC, rc);
		}

		return TRUE;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnEraseBkgnd unknown exception.");
	}
	return CDialog::OnEraseBkgnd(pDC);
}


BOOL CFileServerDlg::OnInitDialog()
{
	try
	{
		CBaseDlg::OnInitDialog();

		SetControlThread(&m_ControlThread);
		m_ControlThread.SetServerSocket(&m_ServerCtrlSocket);
		m_ControlThread.m_MSGNotifyWnd = m_MsgNotifyWnd;
		m_ControlThread.m_nQuitMessageID = m_nQuitMessageID;
		BuildConnect(m_nCtrlPort);

		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			SetWindowText(szFileTransferText_CH);
		else
			SetWindowText(szFileTransferText_EN);
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO:  在此添加额外的初始化

		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			m_UIHelper = new CUIHelper(IDR_GIFMAP_SSBK);
		else
			m_UIHelper = new CUIHelper(IDR_GIFMAP_SSBKEN);

		this->InitUI();
		this->InitList();
		SetPrompt(false,m_hWnd);
		SetForegroundWindow();

		SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

		SetTimer(1, 500, NULL);

		if (gl_pLogger)
			gl_pLogger->log_info("CFileServerDlg::OnInitDialog");
		return TRUE;  // return TRUE unless you set the focus to a control
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnInitDialog unknown exception.");
	}
	return FALSE;
	// 异常: OCX 属性页应返回 FALSE
}

void CFileServerDlg::InitUI()
{
	try
	{
		this->MoveWindow(0, 0, 468, 330);

		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			m_btAddFile.SetIcon(IDB_BITMAP_ADDFILE02, IDB_BITMAP_ADDFILE01);
			m_btSelAll.SetIcon(IDB_BITMAP_SELALL02, IDB_BITMAP_SELALL01);
			m_btCancel.SetIcon(IDB_BITMAP_CCANCEL02, IDB_BITMAP_CCANCEL01);
			m_btStop.SetIcon(IDB_BITMAP_STOP02, IDB_BITMAP_STOP01);
		}
		else
		{
			m_btAddFile.SetIcon(IDB_BITMAP_ADDFILEEN02, IDB_BITMAP_ADDFILEEN01);
			m_btSelAll.SetIcon(IDB_BITMAP_SELALLEN02, IDB_BITMAP_SELALLEN01);
			m_btCancel.SetIcon(IDB_BITMAP_CCANCELEN02, IDB_BITMAP_CCANCELEN01);
			m_btStop.SetIcon(IDB_BITMAP_STOPEN02, IDB_BITMAP_STOPEN01);
		}

		m_btMinimize.SetIcon(IDB_BITMAP_MINIMIZE02, IDB_BITMAP_MINIMIZE01);
		m_btCClose.SetIcon(IDB_BITMAP_CCLOSE02, IDB_BITMAP_CCLOSE01);

		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			CString strText;
			strText = szMinimize_CH;
			m_btMinimize.SetToolTipText(&strText);
			strText = szCloseTransfer_CH;
			m_btCClose.SetToolTipText(&strText);
		}
		else
		{
			CString strText;
			strText = szMinimize_EN;
			m_btMinimize.SetToolTipText(&strText);
			strText = szCloseTransfer_EN;
			m_btCClose.SetToolTipText(&strText);
		}

		m_btAddFile.MoveWindow(13, 40, 98, 27);
		m_btSelAll.MoveWindow(120, 40, 81, 27);
		m_btCancel.MoveWindow(374, 40, 81, 27);
		m_btStop.MoveWindow(330, 295, 125, 27);
		m_btMinimize.MoveWindow(403, 6, 21, 21);
		m_btCClose.MoveWindow(430, 6, 21, 21);

		m_btCancel.ShowWindow(SW_HIDE);

		m_wndFileList.MoveWindow(14, 76, 440, 182);
		m_FileProgress.MoveWindow(14, 268, 441, 19);

		//m_wndFileList.ShowWindow(SW_HIDE);
		m_PromptDlg.SetType(MessageDlg);
		m_PromptDlg.Create(IDD_DIALOG_PROMPT, this);
		m_PromptDlg.SetHwnd(m_hWnd);
		m_ExitAskDlg.SetType(AcceptAskDlg);
		m_ExitAskDlg.Create(IDD_DIALOG_PROMPT, this);
		m_PromptDlg.SetHwnd(m_hWnd);
		GetDlgItem(IDC_STATIC_STATUS)->MoveWindow(14, 300, 300, 19);
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::InitUI unknown exception.");
	}
}

void CFileServerDlg::InitList()
{
	try
	{
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			m_wndFileList.InsertColumn(0, szState_CH, 0, 200);
			m_wndFileList.InsertColumn(0, szSize_CH, 0, 50);
			m_wndFileList.InsertColumn(0, szFileName_CH, 0, 200);
		}
		else
		{
			m_wndFileList.InsertColumn(0, szState_EN, 0, 200);
			m_wndFileList.InsertColumn(0, szSize_EN, 0, 50);
			m_wndFileList.InsertColumn(0, szFileName_EN, 0, 200);
		}

		//m_wndFileList.ModifyStyleEx(NULL, LVS_EX_CHECKBOXES, 0);
		m_wndFileList.SetExtendedStyle(m_wndFileList.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
		m_wndFileList.SetExtendedStyle(m_wndFileList.GetExtendedStyle() | LVS_EX_GRIDLINES );
		//m_wndFileList.SetExtendedStyle(m_wndFileList.GetExtendedStyle() | LVS_EX_CHECKBOXES );

		m_wndFileList.ShowWindow(SW_HIDE);
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::InitList unknown exception.");
	}
}

void CFileServerDlg::AddListItem(BasicFileInfo fileinfo, State state)
{
	try
	{
		CString strState;
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			switch(state)
			{
			case Sending:
				strState = szSending_CH;
				break;
			case Receiving:
				strState = szReceiving_CH;
				break;
			case CRequestSend:
				strState = szERequest_CH;
				m_bContinue = true;
				break;
			case FinishSend:
				strState = szFinishedSending_CH;
				break;
			case FinishReceive:
				strState = szFinishedReceiving_CH;
				break;
			case CRejectReceive:
				strState = szERejectReceiving_CH;
				break;
			case StopSend:
				strState = szStopSending_CH;
				break;
			case StopReceive:
				strState = szStopReceiving_CH;
				break;
			case SRejectReceive:
				strState = szYRejectReceiving_CH;
				break;
			case SRequestSend:
				strState = szYRequestSending_CH;
				m_bContinue = true;
				break;

			case RequestGet:
				strState = szERequestGetFile_CH;
				m_bContinue = true;
				break;

			case RejectGet:
				strState = szYRejectGetFile_CH;
				break;
			}
		}
		else
		{
			switch(state)
			{
			case Sending:
				strState = szSending_EN;
				break;
			case Receiving:
				strState = szReceiving_EN;
				break;
			case CRequestSend:
				strState = szERequest_EN;
				m_bContinue = true;
				break;
			case FinishSend:
				strState = szFinishedSending_EN;
				break;
			case FinishReceive:
				strState = szFinishedReceiving_EN;
				break;
			case CRejectReceive:
				strState = szERejectReceiving_EN;
				break;
			case StopSend:
				strState = szStopSending_EN;
				break;
			case StopReceive:
				strState = szStopReceiving_EN;
				break;
			case SRejectReceive:
				strState = szYRejectReceiving_EN;
				break;
			case SRequestSend:
				strState = szYRequestSending_EN;
				m_bContinue = true;
				break;

			case RequestGet:
				strState = szERequestGetFile_EN;
				m_bContinue = true;
				break;

			case RejectGet:
				strState = szYRejectGetFile_EN;
				break;
			}
		}
		int nCount = this->m_wndFileList.GetItemCount();

		LVITEM item;
		item.mask = LVIF_TEXT;
		item.iItem  = nCount;
		item.iSubItem = 0;
		item.pszText = fileinfo.FileName;

		this->m_wndFileList.InsertItem(&item);

		item.iSubItem = 1;
		CString strUSize;
		if(fileinfo.Size < 1024)
			strUSize.Format("%dByte", fileinfo.Size);
		else if(fileinfo.Size > 1024 && fileinfo.Size < 1024 * 1024)
			strUSize.Format("%.2fK", (float)fileinfo.Size / 1024);
		else if(fileinfo.Size > 1024 * 1024)
			strUSize.Format("%.2fM", (float)fileinfo.Size / (1024 * 1024));

		item.pszText = strUSize.GetBuffer();
		m_wndFileList.SetItem(&item);

		item.iSubItem = 2;
		item.pszText = strState.GetBuffer();
		m_wndFileList.SetItem(&item);
		m_wndFileList.ShowWindow(SW_SHOW);

		m_nListItemIndex ++;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::AddListItem unknown exception.");
	}
}

void CFileServerDlg::SetListItem(BasicFileInfo fileinfo, State state)
{
	try
	{
		int nCount = this->m_wndFileList.GetItemCount();
		if(nCount - 1 < 0)
			return;
		CString strState;
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			switch(state)
			{
			case Sending:
				strState = szSending_CH;
				break;
			case Receiving:
				strState = szReceiving_CH;
				break;
			case CRequestSend:
				strState = szERequest_CH;
				m_bContinue = true;
				break;
			case FinishSend:
				strState = szFinishedSending_CH;
				m_bContinue = false;
				break;
			case FinishReceive:
				strState = szFinishedReceiving_CH;
				m_bContinue = false;
				break;
			case CRejectReceive:
				strState = szERejectReceiving_CH;
				m_bContinue = false;
				break;
			case SRejectReceive:
				strState = szYRejectReceiving_CH;
				m_bContinue = false;
				break;
			case StopSend:
				strState = szStopSending_CH;
				m_bContinue = false;
				break;
			case StopReceive:
				strState = szStopReceiving_CH;
				m_bContinue = false;
				break;

			case SRequestSend:
				strState = szYRequestSending_CH;
				m_bContinue = true;
				break;

			case RequestGet:
				strState = szERequestGetFile_CH;
				m_bContinue = true;
				break;

			case RejectGet:
				strState = szYRejectGetFile_CH;
				m_bContinue = false;
				break;
			}
		}
		else
		{
			switch(state)
			{
			case Sending:
				strState = szSending_EN;
				break;
			case Receiving:
				strState = szReceiving_EN;
				break;
			case CRequestSend:
				strState = szERequest_EN;
				m_bContinue = true;
				break;
			case FinishSend:
				strState = szFinishedSending_EN;
				m_bContinue = false;
				break;
			case FinishReceive:
				strState = szFinishedReceiving_EN;
				m_bContinue = false;
				break;
			case CRejectReceive:
				strState = szERejectReceiving_EN;
				m_bContinue = false;
				break;
			case SRejectReceive:
				strState = szYRejectReceiving_EN;
				m_bContinue = false;
				break;
			case StopSend:
				strState = szStopSending_EN;
				m_bContinue = false;
				break;
			case StopReceive:
				strState = szStopReceiving_EN;
				m_bContinue = false;
				break;

			case SRequestSend:
				strState = szYRequestSending_EN;
				m_bContinue = true;
				break;

			case RequestGet:
				strState = szERequestGetFile_EN;
				m_bContinue = true;
				break;

			case RejectGet:
				strState = szYRejectGetFile_EN;
				m_bContinue = false;
				break;
			}
		}

		this->m_wndFileList.SetItemText(m_nListItemIndex, 2, strState);

		m_wndFileList.ShowWindow(SW_SHOW);
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::SetListItem unknown exception.");
	}

}

void CFileServerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		if (point.y <= 30)
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
		}
	}
	catch(...){}
	CDialog::OnLButtonDown(nFlags, point);
}


//CFileDialog Filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_EXPLORER, //| OFN_ALLOWMULTISELECT
//						"All files(*.*)|*.*||", NULL);
void CFileServerDlg::OnBnClickedButtonAddfile()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		if (m_bAddFile)
		{
			try
			{
				try
				{
					//static CFileDialog Filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_EXPLORER, //| OFN_ALLOWMULTISELECT
					//	"All files(*.*)|*.*||", NULL);  
					if (m_pFileDlg)
					{
					if (IsWindow(m_pFileDlg->m_hWnd))
					{
						m_pFileDlg->SetFocus();
					}
					}
				}
				catch(...){}
			}
			catch(...){}
			return;
		}
		m_bAddFile = true;

		if(GetPrompt(m_hWnd))
		{
			GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_CH);
			else
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_EN);
			SetForegroundWindow();
			m_bAddFile = false;
			return;
		}

		CString filepath;      
		//static CFileDialog Filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_EXPLORER, //| OFN_ALLOWMULTISELECT
		//	"All files(*.*)|*.*||", NULL);   
		DWORD MAXFILE = 4000;
		m_pFileDlg->m_ofn.nMaxFile = MAXFILE;
		char* pc = new char[MAXFILE];
		m_pFileDlg->m_ofn.lpstrFile = pc;
		m_pFileDlg->m_ofn.lpstrFile[0] = NULL;

		CString strInitPath = GetLastFileDialogPath();
		m_pFileDlg->m_ofn.lpstrInitialDir = strInitPath.GetBuffer();
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

		if(IDOK == m_pFileDlg->DoModal())
		{
			if(m_bSending)
			{
				CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					strMsg = szWaiting_CH;
				else
					strMsg = szWaiting_EN;
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
				this->Invalidate();
				m_bAddFile = false;
				return;
			}
			else if(m_bReceive)
			{
				CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					strMsg = szWaiting_CH;
				else
					strMsg = szWaiting_EN;
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));
				this->Invalidate();
				m_bAddFile = false;
				return;
			}

			CString strSendFilePathName = m_pFileDlg->GetPathName();

			ProgramConfig pConfig;
			pConfig.SetConfigFileName("FileSetting.ini");
			pConfig.WriteStringConfigParameter("File Setting", "LastFileDialogPath", strSendFilePathName);

			int nFind = strSendFilePathName.ReverseFind('\\');
			if(nFind != -1)
			{
				m_strSendPath = strSendFilePathName.Left(nFind);
				m_strSendFileName = strSendFilePathName.Right(strSendFilePathName.GetLength() - nFind - 1);
			}

			BasicFileInfo info;
			CString strFileName = m_pFileDlg->GetFileName();
			strcpy_s(info.FileName, sizeof(info.FileName), strSendFilePathName);
			info.Size = 0;
			CFile file;
			if(file.Open(m_pFileDlg->GetPathName(), CFile::modeRead))
			{
				info.Size = file.GetLength();
				file.Close();
			}
			else
			{
				CString strMsg = _T("无法打开文件，该文件可能正被其他程序使用。无法发送文件。");
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					strMsg = szCannotOpenFile_CH;
				else
					strMsg = szCannotOpenFile_EN;
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				m_bSending = false;
				m_bAddFile = false;
				return;
			}

			if (info.Size == 0)
			{
				CString strMsg = _T("不支持大小为0文件传输。");
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					strMsg = szNotZeroFile_CH;
				else
					strMsg = szNotZeroFile_EN;
				m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();
				m_bSending = false;
				m_bAddFile = false;
				return;
			}

			if(!this->SendFileRequest(m_pFileDlg->GetPathName()))
			{
				m_bSending = false;
				m_bAddFile = false;
				return;
			}

			State state;
			state = State::SRequestSend;
			SendMessage(WM_ADDLISTITEM, (WPARAM)&info, (LPARAM)&state);
			m_bSending = true;
		}
		delete []pc;
		m_bAddFile = false;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnBnClickedButtonAddfile unknown exception.");
	}
	m_bAddFile = false;
}

void CFileServerDlg::OnBnClickedButtonCancel()
{
	this->OnCancel();
}

void CFileServerDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	/*gl_StopSection.Lock();
	gl_bStop = true;
	gl_StopSection.Unlock();*/
	
	if (m_bReceive || m_bSending)
	{
		CString strMsg;
		strMsg = _T("正在停止...");
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			strMsg = szStoping_CH;
		else
			strMsg = szStoping_EN;
		m_wndFileList.SetItemText(m_nListItemIndex, 2, strMsg);
	}

	m_bContinue = false;
	m_bSending = false;
	m_bReceive = false;
}


void CFileServerDlg::OnHdnItemclickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CFileServerDlg::OnNMClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CFileServerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		if(GetPrompt(m_hWnd))
		{
			GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_CH);
			else
				SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_EN);
			SetForegroundWindow();
			return;
		}

		CDialog::OnDropFiles(hDropInfo);

		if(m_bSending)
		{
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				strMsg = szWaiting_CH;
			else
				strMsg = szWaiting_EN;
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			return;
		}
		else if(m_bReceive)
		{
			CString strMsg = _T("正等待响应或已经有文件正在传输,请稍候重试。");
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				strMsg = szWaiting_CH;
			else
				strMsg = szWaiting_EN;
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
			CString strMsg = _T("一次只能传送单个文件。");
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				strMsg = szOneFileOnly_CH;
			else
				strMsg = szOneFileOnly_EN;
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			m_bSending = false;
			return;
		}

		if(foldercount > 0)
		{
			CString strMsg = _T("不支持文件夹传输。");
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				strMsg = szNotFolder_CH;
			else
				strMsg = szNotFolder_EN;
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			m_bSending = false;
			return;
		}

		BasicFileInfo info;
		CString strFileName = strPathFileName;
		strcpy_s(info.FileName, sizeof(info.FileName), strFileName);
		info.Size = 0;
		CFile file;
		if(file.Open(strPathFileName, CFile::modeRead))
		{
			info.Size = file.GetLength();
			file.Close();
		}

		if (info.Size == 0)
		{
			CString strMsg = _T("不支持大小为0文件传输。");
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				strMsg = szNotZeroFile_CH;
			else
				strMsg = szNotZeroFile_EN;
			m_PromptDlg.SetMsgText(strMsg);
			m_PromptDlg.Show();
			m_bSending = false;
			return;
		}

		State state;
		state = State::SRequestSend;
		SendMessage(WM_ADDLISTITEM, (WPARAM)&info, (LPARAM)&state);

		//this->m_strSendFileName = strPathFileName;

		CString strSendFilePathName = strPathFileName;
		int nFind = strSendFilePathName.ReverseFind('\\');
		if(nFind != -1)
		{
			m_strSendPath = strSendFilePathName.Left(nFind);
			m_strSendFileName = strSendFilePathName.Right(strPathFileName.GetLength() - nFind - 1);
		}


		m_bSending = true;
		if(!SendFileRequest(strPathFileName))
		{
			m_bSending = false;
		}
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnDropFiles unknown exception.");
	}
}

BOOL CFileServerDlg::UpdateListCtrl(CListCtrl *pListCtrl, BOOL bReceiveList/* = TRUE */)
{
	

	return TRUE;
}

LRESULT CFileServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
		switch(message)
		{
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
			m_bSending = false;
			m_bReceive = false;

			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
				SetDlgItemText(IDC_STATIC_STATUS, szAcceptSameTime_CH);
			else
				SetDlgItemText(IDC_STATIC_STATUS, szAcceptSameTime_EN);
			break;
		case WM_GETSENDPATH:
			{
				CString *strFilePath = (CString *)lParam;
				if(strFilePath != NULL)
					*strFilePath = this->m_strSendPath;
			}
			break;
		case WM_GETFILENAMETOSEND:
			{
				CString *strFileToSend = (CString *)lParam;
				if(strFileToSend != NULL)
					*strFileToSend = this->m_strSendFileName;
			}
			break;
		case WM_FILE_OPERATE_BEGIN:
			{
				CString strMsg;
				if(wParam == 1) 
				{
					m_bSending = true;
					strMsg = _T("正在发送...");
					if(StrCmp(gl_strLanguageId, CHINESE) == 0)
						strMsg = szSending_CH;
					else
						strMsg = szSending_EN;
				}
				else
				{
					m_bReceive = true;
					strMsg = _T("正在接收...");
					if(StrCmp(gl_strLanguageId, CHINESE) == 0)
						strMsg = szReceiving_CH;
					else
						strMsg = szReceiving_EN;
				}
				m_wndFileList.SetItemText(m_nListItemIndex, 2, strMsg);
			}
			break;
		case WM_FILE_OPERATE_END:
			{
				if (gl_pLogger) gl_pLogger->log_info("CFileServerDlg::WindowProc WM_FILE_OPERATE_END %d, %d", wParam, lParam);
				PostMessage(WM_FILE_SENDFILETOSERVER_PROGRESS_MESSAGE, 0, 0);
				this->m_bContinue = false;
				this->m_bSending = false;
				this->m_bReceive = false;
				CString strMsg = GenNoticeMsg((FileState)wParam, lParam);
				if (gl_pLogger) gl_pLogger->log_info("SendFile return %s", strMsg);
				if(strMsg != "")
				{
					m_wndFileList.SetItemText(m_nListItemIndex, 2, strMsg);
					CString strFilePathName = m_wndFileList.GetItemText(m_nListItemIndex, 0);
					if(lstrcmp(gl_strLanguageId, CHINESE) == 0)
						strFilePathName = _T(" 文件: ") + strFilePathName;
					else
						strFilePathName = _T(" File: ") + strFilePathName;
					m_PromptDlg.SetMsgText(strMsg + strFilePathName);
					m_PromptDlg.Show();
				}
				PostMessage(WM_CLOSEPROMPTDLG, MessageDlg, 0);
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
			{
				DlgType dlgtype = (DlgType)wParam;
				if (dlgtype == MessageDlg)
				{
					if (this->m_bSending || this->m_bReceive)
					{
						if(lstrcmp(gl_strLanguageId, CHINESE) == 0)
							this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(szTransfering_CH);
						else
							this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(szTransfering_EN);
					}
					else
						this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T(""));

					this->Invalidate();
				}

				else if (dlgtype == AcceptAskDlg)
				{
				}
				else if (dlgtype == OkCancelDlg)
				{

				}
			}
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
			break;
		case WM_REJECT_SENDREQUEST:
			{
				CString strMsg;
				if(wParam == 0)
				{
					strMsg = _T("对方拒绝了您的发送请求");
					if(StrCmp(gl_strLanguageId, CHINESE) == 0)
						strMsg = szERejectYSendingFile_CH;
					else
						strMsg = szERejectYSendingFile_EN;
					m_bSending = false;
				}
				else if(wParam == 1)
				{
					strMsg = _T("对方拒绝了您的提取请求");
					if(StrCmp(gl_strLanguageId, CHINESE) == 0)
						strMsg = szERejectYGettingFile_CH;
					else
						strMsg = szERejectYGettingFile_EN;
					m_bReceive = false;
				}
				/*m_PromptDlg.SetMsgText(strMsg);
				m_PromptDlg.Show();*/
			}
			break;
		case WM_GETCONTINUESTATUS:
			{
				bool *bContinue = (bool *)lParam;
				*bContinue = this->m_bContinue;
			}
			break;
		case WM_SETSTOPSTATE:
			{
				m_bContinue = lParam == 1 ? true : false;
			}
			break;
		case WM_GETSTOPSTATE:
			{
				bool *pCoutinue = (bool *)lParam;
				*pCoutinue = m_bContinue;
			}
			break;
		case WM_ADDLISTITEM:
			{
				BasicFileInfo *pInfo = (BasicFileInfo *)wParam;
				State *pState = (State *)lParam;
				this->AddListItem(*pInfo, *pState);
				memcpy(&m_FileInfo, pInfo, sizeof(BasicFileInfo));
			}
			break;
		case WM_SETLISTITEM:
			{
				//BasicFileInfo *pInfo = (BasicFileInfo *)wParam;
				State *pState = (State *)lParam;
				this->SetListItem(m_FileInfo, *pState);
				
				//memcpy(&m_FileInfo, pInfo, sizeof(BasicFileInfo));
			}
			break;
		case WM_SETSTSTUS_TEXT:
			{
				CString *strStatusText = (CString *)lParam;
				if (strStatusText)
				{
					this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(*strStatusText);
					this->Invalidate();
				}
			}
			break;
		case WM_SETSENDING:
			{
				m_bSending = wParam == 1 ? true : false;
			}
			break;
		case WM_SETRECEIVE:
			{
				m_bReceive = wParam == 1 ? true : false;
			}
			break;
		case WM_GETSENDFILEPATHNAME:
			{
				char *p = (char *)wParam;
#pragma warning(push)
#pragma warning(disable : 4996) // warning C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	
				strcpy(p, m_strSendFileName);
#pragma warning(pop)
			}
			break;

		case WM_MAKETUNNELEND:
			{
				bool bRes = (bool)wParam;
				TunnelMode mode = (TunnelMode)lParam;
				OnMakeTunnelEnd(bRes, mode);
				return 0;
			}
			break;
		}
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::WindowProc unknown exception. message: %d", message);
	}
	return CBaseDlg::WindowProc(message, wParam, lParam);
}

LRESULT CFileServerDlg::OnFileToServerProgessMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		ULONGLONG nProgress = (ULONGLONG) wParam;

		if (IsWindow(m_FileProgress.GetSafeHwnd()))
		{
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
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnFileToServerProgessMessage unknown exception");
	}
	return 0;
}

LRESULT CFileServerDlg::OnSetRangeMessage(WPARAM wParam, LPARAM lParam)
{
	try
	{
		if (IsWindow(m_FileProgress.GetSafeHwnd()))
		{
			m_FileProgress.SetRange32(0, (int) wParam);
			m_FileProgress.SetPos(0);
		}

		return 0;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnSetRangeMessage unknown exception");
	}
	return 0;
}

bool CFileServerDlg::SendFileRequest(LPCTSTR lpFilePathName)
{
	try
	{
		if(!PackAndSendHeader(this->m_Socket, CMD_PARAM_TRANSFILE_S2C_SENDFILEREQUEST, true))
			return false;
		FileToSend filesend;
		strcpy_s(filesend.FileName, sizeof(filesend.FileName), m_strSendFileName);
		if(!::SendData(this->m_Socket, (char*)(&filesend), sizeof(FileToSend)))
			return false;

		return true;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::SendFileRequest unknown exception");
	}
	return false;
}


bool IsActive()
{
	try
	{
		bool bactive = false;
		::gl_ActiveSection.Lock();
		bactive = gl_bActive;
		gl_ActiveSection.Unlock();
		return bactive;
	}
	catch(...)
	{
	}
	return false;
}

void SetNotActive()
{
	::gl_ActiveSection.Lock();
	gl_bActive = false;
	gl_ActiveSection.Unlock();
}



//end
void CFileServerDlg::OnBnClickedButtonStopserver()
{
	// TODO: 在此添加控件通知处理程序代码
	if(GetPrompt(m_hWnd))
	{
		GetDlgItem(IDC_STATIC_STATUS)->ShowWindow(SW_SHOW);
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_CH);
		else
			SetDlgItemText(IDC_STATIC_STATUS, szStatTermiate_EN);
		SetForegroundWindow();
		return;
	}
	m_bContinue = false;
	//gl_SenderStop = true;
	gl_mapSenderStop[m_hWnd] = true;
}

void CFileServerDlg::OnBnClickedButtonMinimize()
{
	// TODO: 在此添加控件通知处理程序代码
	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);   
}

void CFileServerDlg::OnBnClickedButtonCclose()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowWindow(SW_HIDE);	
	PostMessage(WM_CLOSE,0,0);
}

void CFileServerDlg::OnTimer(UINT_PTR nIDEvent)
{

	/*try
	{
		if(!IsActive())
		{
			this->OnBnClickedButtonStop();
			int count = 0;
			while((this->m_bSending || this->m_bReceive) && count < 30)
			{
			
				Sleep(100);
				count ++;
			}
			this->OnOK();
		}
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnTimer unknown exception.");
	}*/
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CFileServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_STATUS)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 0, 0));
		hbr = (HBRUSH) GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

BOOL CFileServerDlg::PreTranslateMessage(MSG* pMsg)
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

	return CDialog::PreTranslateMessage(pMsg);
}

CString CFileServerDlg::GetLastFileDialogPath()
{
	try
	{
		char lpszDefaultDir[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, lpszDefaultDir);

		char szDocument[MAX_PATH]={0};   
		memset(lpszDefaultDir,0,_MAX_PATH);   

		LPITEMIDLIST pidl = NULL;   
		SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);   
		if (pidl && SHGetPathFromIDList(pidl, szDocument))   
		{   
			//GetShortPathName(szDocument, lpszDefaultDir, _MAX_PATH);   
			lstrcpy(lpszDefaultDir, szDocument);
		}

		char lpszPath[MAX_PATH];
		ProgramConfig pConfig;
		pConfig.SetConfigFileName("FileSetting.ini");
		pConfig.ReadStringConfigParameter("File Setting", "LastFileDialogPath", lpszPath, lpszDefaultDir);

		CString strPath;
		strPath = lpszPath;
		return strPath;
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::GetLastFileDialogPath unknown exception");
	}
	return "";
}

void CFileServerDlg::OnMakeTunnelEnd(bool bSuccess, TunnelMode mode)
{
	CBaseDlg::OnMakeTunnelEnd(bSuccess, mode);

	try
	{
		if (bSuccess)
		{
			if (mode == CONTROL)
			{
				this->m_Socket = this->m_ServerCtrlSocket.GetWorkingSocket();
				m_ControlThread.SetWnd(m_hWnd);
				m_ControlThread.StartWorking();
			}
			else if(mode == SEND)
			{

			}
			else if(mode == RECEIVE)
			{

			}
			if (gl_pLogger) gl_pLogger->log_info("CFileServerDlg WM_MAKETUNNELEND Ok mode: %d", mode);
		}
		else
		{
			if (gl_pLogger) gl_pLogger->log_info("CFileServerDlg WM_MAKETUNNELEND failed mode: %d", mode);
		}
	}
	catch(...)
	{
		if (gl_pLogger) gl_pLogger->log_error("CFileServerDlg::OnMakeTunnelEnd unkown exception. ");
	}
}
