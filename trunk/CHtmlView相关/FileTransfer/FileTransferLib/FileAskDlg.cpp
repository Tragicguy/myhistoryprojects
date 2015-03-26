// FileAskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "FileAskDlg.h"

#include "SkinScrollWnd.h"
#include "PromptDlg.h"

#include "StringTable.h"
#include "GlobalFun.h"


// CFileAskDlg 对话框

IMPLEMENT_DYNAMIC(CFileAskDlg, CDialog)

CFileAskDlg::CFileAskDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFileAskDlg::IDD, pParent)
{

}

CFileAskDlg::~CFileAskDlg()
{
	DeleteObject(m_hBmpScrollBar); //2009-1-9
}

void CFileAskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_MSG, m_REMsg);
}


BEGIN_MESSAGE_MAP(CFileAskDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CFileAskDlg::OnBnClickedButtonSaveas)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CANCEL2, &CFileAskDlg::OnBnClickedCancel2)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDOK, &CFileAskDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT_MSG, &CFileAskDlg::OnEnSetfocusEditMsg)
END_MESSAGE_MAP()


// CFileAskDlg 消息处理程序

void CFileAskDlg::OnBnClickedButtonSaveas()
{
	while(1)
	{
		char   szDir[MAX_PATH];   
		memset(szDir, 0, MAX_PATH);
		BROWSEINFO   bi;   
		ITEMIDLIST   *pidl;   
		bi.hwndOwner   =   NULL;   
		bi.pidlRoot   =   NULL;   
		bi.pszDisplayName   =   szDir;   
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			bi.lpszTitle   =   szSelDir_CH;   
		else
			bi.lpszTitle   =   szSelDir_EN;    
		bi.ulFlags   =   BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN;   
		bi.lpfn   =   NULL;   
		bi.lParam   =   0;   
		bi.iImage   =   0;   
		pidl   =   SHBrowseForFolder(&bi);   
		if(pidl   ==   NULL)  
		{
			/*if(IDYES == MessageBox(NULL, _T("要取消接收文件？"), _T("询问提示"), MB_YESNO))
				return PackAndSendRequest(CMD_PARAM_TRANSFILE_C2S_REJECT);
			else
				continue;*/
			return;
		}
		SHGetPathFromIDList(pidl, szDir);  
		CString strDir = szDir;
		CString strFileName = strDir + _T("\\test.txt1");
		if(!strDir.IsEmpty())
		{
			m_strDistinationFolder = strDir;
			if(this->CreateTestFile(strFileName))
			{
				CString strFileName = m_strDistinationFolder + _T("\\") + this->m_strFileName;
				CFileStatus   status;   
				if(CFile::GetStatus(strFileName, status) == TRUE)  
				{
					int nRes = -1;
					if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					{
						nRes = MessageBox(szOverWrite_CH, szMsgTitle_CH, MB_YESNOCANCEL);
					}
					else
					{
						nRes = MessageBox(szOverWrite_EN, szMsgTitle_EN, MB_YESNOCANCEL);
					}	

					if(IDYES == nRes)
					{
						//gl_llSize = 0;
						gl_mapllSize[m_hRapperWnd] = 0;
					}
					else if (IDNO == nRes)
					{
						CFile file;
						if (!file.Open(strFileName, CFile::modeRead))
							//gl_llSize = 0;
							gl_mapllSize[m_hRapperWnd] = 0;
						else
						{
							//gl_llSize = file.GetLength();
							gl_mapllSize[m_hRapperWnd] = file.GetLength();
							file.Close();
						}
					}
					else if (IDCANCEL == nRes)
					{
						return;
					}
				}
				this->OnOK();
				break;
			}
			else
			{
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					AfxMessageBox(szInvalidDir_CH);
				else
					AfxMessageBox(szInvalidDir_EN);
			}
		}
	}
}

bool CFileAskDlg::CreateTestFile(LPCTSTR szFileName)
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

void CFileAskDlg::SetMsgText(LPCTSTR szMsgText)
{
	m_strMsg = szMsgText;
}

BOOL CFileAskDlg::OnInitDialog()
{
	

	CDialog::OnInitDialog();
	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
	{
		this->SetWindowText(szMsgTitle_CH);
		GetDlgItem(IDOK)->SetWindowText(szAgree_CH);
		GetDlgItem(IDCANCEL)->SetWindowText(szReject_CH);
		GetDlgItem(IDC_BUTTON_SAVEAS)->SetWindowText(szSaveAs_CH);
	}
	else
	{
		this->SetWindowText(szMsgTitle_EN);
		GetDlgItem(IDOK)->SetWindowText(szAgree_EN);
		GetDlgItem(IDCANCEL)->SetWindowText(szReject_EN);
		GetDlgItem(IDC_BUTTON_SAVEAS)->SetWindowText(szSaveAs_EN);
	}
	CString strFileName = this->m_strDistinationFolder + _T("\\test.txt1");
	

	m_btClose.SubclassDlgItem(IDC_CANCEL2, this);
	m_btClose.SetIcon(IDB_BITMAP_CLOSE2, IDB_BITMAP_CLOSE1);

	m_btAccept.SubclassDlgItem(IDOK, this);
	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		m_btAccept.SetIcon(IDB_BITMAP_ACCEPT2, IDB_BITMAP_ACCEPT1);
	else
		m_btAccept.SetIcon(IDB_BITMAP_ACCEPTEN2, IDB_BITMAP_ACCEPTEN1);
	
	m_btAccept.MoveWindow(30, 125, 62, 22);

	m_btSaveAs.SubclassDlgItem(IDC_BUTTON_SAVEAS, this);
	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		m_btSaveAs.SetIcon(IDB_BITMAP_SAVEAS2, IDB_BITMAP_SAVEAS1);
	else
		m_btSaveAs.SetIcon(IDB_BITMAP_SAVEASEN2, IDB_BITMAP_SAVEASEN1);
	m_btSaveAs.MoveWindow(105, 125, 67, 22);

	m_btReject.SubclassDlgItem(IDCANCEL, this);
	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		m_btReject.SetIcon(IDB_BITMAP_REJECT2, IDB_BITMAP_REJECT1);
	else
		m_btReject.SetIcon(IDB_BITMAP_REJECTEN2, IDB_BITMAP_REJECTEN1);
	m_btReject.MoveWindow(184, 125, 62, 22);

	//GetDlgItem(IDOK)->EnableWindow(CreateTestFile(strFileName));

	//SetDlgReign();

	

	CRect rect;
	GetClientRect(&rect);
	this->MoveWindow(rect.left, rect.top, 276, 176);

	//add by pan peng 2009-1-9 for ticket #1321
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_SCROLLBAR);
	m_hBmpScrollBar = (HBITMAP) bmp.Detach();
	CSkinScrollWnd *pWnd = SkinWndScroll(GetDlgItem(IDC_RICHEDIT2_MSG), m_hBmpScrollBar);
	bmp.DeleteObject();

	pWnd->MoveWindow(31, 48, 214, 70);
	//GetDlgItem(IDC_RICHEDIT2_MSG)->SetWindowText(m_strMsg);
	AddRichEditMsg(m_strMsg); //add colored text.
	//

	//
	GetDlgItem(IDC_STATIC_ASKMSG)->ShowWindow(SW_HIDE);
	/*CWnd *pParent = this->GetParent();
	if(pParent)*/
		GetDlgItem(IDC_RICHEDIT2_MSG)->SetFont(this->GetFont());
	//end

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control

}

BOOL CFileAskDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
	{
		CUIHelper UIHelper(IDR_GIFMAP_ASK);
		UIHelper.SetBackPicture(pDC, rc);
	}
	else
	{
		CUIHelper UIHelper(IDR_GIFMAP_ASK_EN);
		UIHelper.SetBackPicture(pDC, rc);
	}
	return true; //CDialog::OnEraseBkgnd(pDC);
}


void CFileAskDlg::SetDlgReign()
{
	int UI_ROUNDRECT_WIDTH = 16;
	int UI_ROUNDRECT_HEIGHT = 16;

	CRect rc;
	POINT Points[6];
	GetClientRect(&rc);

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
}
void CFileAskDlg::OnBnClickedCancel2()
{
	this->OnCancel();
}

HBRUSH CFileAskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ASKMSG || pWnd->GetDlgCtrlID() == IDC_RICHEDIT2_MSG)
	{
		pDC->SetBkMode(TRANSPARENT); 
   		//pDC->SetTextColor(RGB(69, 96, 152));		
		//pDC->SetTextColor(RGB(69, 96, 152));	
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}

void CFileAskDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.y <= 30)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
	}
    CDialog::OnLButtonDown(nFlags, point); 
}

void CFileAskDlg::OnBnClickedOk()
{
	CString strFileName = this->m_strDistinationFolder + _T("\\test.txt1");
	if(!CreateTestFile(strFileName))
	{
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			AfxMessageBox(szInvalidDir_CH);
		else
			AfxMessageBox(szInvalidDir_EN);
		
		return;
	}

	CString strFilePathName = m_strDistinationFolder + _T("\\") + this->m_strFileName;
	CFileStatus   status;   
	if(CFile::GetStatus(strFilePathName, status) == TRUE)  
	{
		int nRes = -1;
		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
		{
			nRes = MessageBox(szOverWrite_CH, szMsgTitle_CH, MB_YESNOCANCEL);
		}
		else
			nRes = MessageBox(szOverWrite_CH, szMsgTitle_EN, MB_YESNOCANCEL);
		if(IDYES == nRes)
		{
			//gl_llSize = 0;
			gl_mapllSize[m_hRapperWnd] = 0;
		}
		else if (IDNO == nRes)
		{
			CFile file;
			if (!file.Open(strFilePathName, CFile::modeRead))
				//gl_llSize = 0;
				gl_mapllSize[m_hRapperWnd] = 0;
			else
			{
				//gl_llSize = file.GetLength();
				gl_mapllSize[m_hRapperWnd] = file.GetLength();
				file.Close();
			}
		}
		else if (IDCANCEL == nRes)
		{
			return;
		}
	}
	OnOK();
}

void CFileAskDlg::OnEnSetfocusEditMsg()
{
	//GetDlgItem(IDC_EDIT_MSG)->SendMessage(WM_KILLFOCUS, (WPARAM)this, 0);
}

void CFileAskDlg::AddRichEditMsg(LPCTSTR strMsg)
{
	m_REMsg.SetSel(-1, -1);

	COLORREF clr = RGB(69, 96, 152);

	CHARFORMAT2 cfm;
	cfm.cbSize = sizeof(cfm);
	cfm.crTextColor = clr;
	cfm.dwEffects = CFE_BOLD;
	BOOL bBold = FALSE;
	cfm.dwMask = bBold ? (CFM_BOLD | CFM_COLOR) : CFM_COLOR;
	cfm.dwMask |= CFM_SIZE;
	cfm.yHeight = 200;
	m_REMsg.SetSelectionCharFormat(cfm);

	m_REMsg.SetSel(-1, -1);
	m_REMsg.ReplaceSel(strMsg);
	//m_REMsg.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}