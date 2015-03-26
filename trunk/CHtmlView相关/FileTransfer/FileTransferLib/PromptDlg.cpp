// PromptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "PromptDlg.h"
#include "SkinScrollWnd.h"
#include "GlobalFun.h"
#include <map>
using namespace std;
HWND gl_HPromptWnd = NULL;//没用，无需处理
bool gl_bPrompt = false;//不再使用
CCriticalSection gl_PromptSection;//没用，无需处理
//add by shaojun at 2010-09-28 添加MAP来区分是哪个窗口的弹出框标识
map<HWND,bool> gl_mapPromt;
void SetPrompt(bool bShow,HWND hwnd)
{
	gl_mapPromt[hwnd] = bShow;
}

bool GetPrompt(HWND hwnd)
{
	return gl_mapPromt[hwnd];
}

// CPromptDlg 对话框

IMPLEMENT_DYNAMIC(CPromptDlg, CDialog)

CPromptDlg::CPromptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPromptDlg::IDD, pParent)
{
	m_eType = AcceptAskDlg;
	m_iSpType = 0;
}

CPromptDlg::~CPromptDlg()
{
	DeleteObject(m_hBmpScrollBar); //2009-1-9
}

void CPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_MSG, m_REMsg);
}


BEGIN_MESSAGE_MAP(CPromptDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPromptDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDCANCEL, &CPromptDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CPromptDlg::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CPromptDlg 消息处理程序

void CPromptDlg::SetMsgText(LPCTSTR szMsgText)
{
	m_strMsg = _T("");
	m_strMsg = szMsgText;
}

void CPromptDlg::SetType(DlgType eType)
{
	m_eType = eType;
}
//
//void CPromptDlg::SetType()
//{
//
//}

BOOL CPromptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitUI();

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	
	return TRUE; 
}

void CPromptDlg::InitUI()
{
	try
	{
		m_btClose.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
		m_btOk.SubclassDlgItem(IDOK, this);
		m_btCancel.SubclassDlgItem(IDCANCEL, this);
		m_btMessageOk.SubclassDlgItem(IDC_BUTTON_OK, this);
		
		m_btClose.MoveWindow(255, 5, 21, 21);
		if (m_iSpType == 1)
		{
			m_btOk.MoveWindow(50, 136, 62, 22);
		}
		else
		{
			m_btOk.MoveWindow(50, 136, 81, 27);
		}
		m_btCancel.MoveWindow(150, 136, 81, 27);
		m_btMessageOk.MoveWindow(100, 136, 81, 27);

		m_btClose.SetIcon(IDB_BITMAP_CCLOSE02, IDB_BITMAP_CCLOSE01);

		if(m_eType == AcceptAskDlg)
		{
			m_btMessageOk.ShowWindow(SW_HIDE);
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			{
				m_btOk.SetIcon(IDB_BITMAP_CACCEPT02, IDB_BITMAP_CACCEPT01);
				m_btCancel.SetIcon(IDB_BITMAP_CREJECT02, IDB_BITMAP_CREJECT01);
			}
			else
			{
				m_btOk.SetIcon(IDB_BITMAP_CACCEPTEN02, IDB_BITMAP_CACCEPTEN01);
				m_btCancel.SetIcon(IDB_BITMAP_CREJECTEN02, IDB_BITMAP_CREJECTEN01);
			}
		}
		//目前只有这一种需要同时兼容支持端与客户端
		else if(m_eType == MessageDlg)
		{
			m_btClose.ShowWindow(SW_HIDE);
			m_btOk.ShowWindow(SW_HIDE);
			m_btCancel.ShowWindow(SW_HIDE);

			if (m_iSpType == 1)	//支持端
			{
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					m_btMessageOk.SetIcon(IDB_BITMAP_OK2, IDB_BITMAP_OK1);
				else
				m_btMessageOk.SetIcon(IDB_BITMAP_MSGOKEN02, IDB_BITMAP_MSGOKEN01);	//支持端无英文版，先不加
			}
			else
			{
				if(StrCmp(gl_strLanguageId, CHINESE) == 0)
					m_btMessageOk.SetIcon(IDB_BITMAP_MSGOK02, IDB_BITMAP_MSGOK01);
				else
					m_btMessageOk.SetIcon(IDB_BITMAP_MSGOKEN02, IDB_BITMAP_MSGOKEN01);
			}			
		}

		else if(m_eType == OkCancelDlg)
		{
			m_btMessageOk.ShowWindow(SW_HIDE);
			if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			{
				m_btOk.SetIcon(IDB_BITMAP_MSGOK02, IDB_BITMAP_MSGOK01);
				m_btCancel.SetIcon(IDB_BITMAP_CCANCEL02, IDB_BITMAP_CCANCEL01);
			}
			else
			{
				m_btOk.SetIcon(IDB_BITMAP_MSGOKEN02, IDB_BITMAP_MSGOKEN01);
				m_btCancel.SetIcon(IDB_BITMAP_CCANCELEN02, IDB_BITMAP_CCANCELEN01);
			}
		}

		gl_HPromptWnd = this->m_hWnd;
		//this->SetMsgText("提示信息提提示信息提示信息提示信息提示信息提示信息示信息提示信息提示信息提示信息提示信息提提示信息提示信息提示信息提示信息提示信息示信息提示信息提示信息提示信息");


		//add by pan peng 2009-1-9 for ticket #1321
		CBitmap bmp;
		bmp.LoadBitmap(IDB_BITMAP_SCROLLBAR);
		m_hBmpScrollBar = (HBITMAP) bmp.Detach();
		CSkinScrollWnd *pWnd = SkinWndScroll(GetDlgItem(IDC_RICHEDIT2_MSG), m_hBmpScrollBar);
		bmp.DeleteObject();

		pWnd->MoveWindow(30, 48, 224, 80);
		//GetDlgItem(IDC_RICHEDIT2_MSG)->SetWindowText(m_strMsg);
		AddRichEditMsg(m_strMsg); //add colored text.
		//

		GetDlgItem(IDC_RICHEDIT2_MSG)->SetFont(this->GetFont());
		//end


		CRect rect;
		GetClientRect(&rect);
		if (m_iSpType ==1)
		{
			this->MoveWindow(rect.left, rect.top, 276, 176);
		}
		else
		{
			this->MoveWindow(rect.left, rect.top, 286, 184);
		}

		if(StrCmp(gl_strLanguageId, CHINESE) == 0)
			SetWindowText(szMessage_EN);
		else
			SetWindowText(szMessage_CH);
		SetForegroundWindow();
	}
	catch(...){}
}

void CPromptDlg::Show()
{
	if (gl_bAutoControl)
		return;

	AddRichEditMsg(m_strMsg); //add colored text.
	this->CenterWindow();	
	this->ShowWindow(SW_SHOW);
	CRect rc;
	this->GetWindowRect(&rc);
	SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	SetForegroundWindow();
	SetPrompt(true,m_hwndParant);
}

BOOL CPromptDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);

	if(StrCmp(gl_strLanguageId, CHINESE) == 0)
	{
		if (m_iSpType ==1)
		{
			CUIHelper UIHelper(IDR_GIFMAP_ASK);
			UIHelper.SetBackPicture(pDC, rc);
		}
		else
		{
			CUIHelper UIHelper(IDR_GIFMAP_CASK);
			UIHelper.SetBackPicture(pDC, rc);
		}
		
	}
	else
	{
		if (m_iSpType ==1)
		{
			CUIHelper UIHelper(IDR_GIFMAP_ASK_EN);
			UIHelper.SetBackPicture(pDC, rc);
		}
		else
		{
			CUIHelper UIHelper(IDR_GIFMAP_CASKEN);
			UIHelper.SetBackPicture(pDC, rc);
		}		
	}
	return true; //CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CPromptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CPromptDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.y <= 30)
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y)); 
	}
    CDialog::OnLButtonDown(nFlags, point); 
}


void CPromptDlg::AddRichEditMsg(LPCTSTR strMsg)
{
	m_REMsg.SetWindowText(_T(""));
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
void CPromptDlg::OnBnClickedButtonClose()
{
	this->OnCancel();
	SetPrompt(false,m_hwndParant);
	if(this->GetParent())
		this->GetParent()->PostMessage(WM_CLOSEPROMPTDLG, (int)m_eType, 0);
}

void CPromptDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnCancel();
	SetPrompt(false,m_hwndParant);
	if(this->GetParent())
		this->GetParent()->PostMessage(WM_CLOSEPROMPTDLG, (int)m_eType, 0);
}
