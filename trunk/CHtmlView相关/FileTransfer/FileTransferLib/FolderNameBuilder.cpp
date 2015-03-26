// FolderNameBuilder.cpp : 实现文件
//

#include "stdafx.h"
#include "FileTransferLib.h"
#include "FolderNameBuilder.h"


// CFolderNameBuilder 对话框

IMPLEMENT_DYNAMIC(CFolderNameBuilder, CDialog)

CFolderNameBuilder::CFolderNameBuilder(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderNameBuilder::IDD, pParent)
{
}

CFolderNameBuilder::~CFolderNameBuilder()
{
}

void CFolderNameBuilder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_EditName);
}


BEGIN_MESSAGE_MAP(CFolderNameBuilder, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CFolderNameBuilder::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFolderNameBuilder::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CANCEL2, &CFolderNameBuilder::OnBnClickedCancel2)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CFolderNameBuilder 消息处理程序

CString CFolderNameBuilder::GetFolderName()
{
	return this->m_FolderName;
}

void CFolderNameBuilder::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_FolderName = "";
	OnCancel();
}

void CFolderNameBuilder::OnBnClickedOk()
{
	CString strName;
	m_EditName.GetWindowTextA(strName);

	strName.Trim();

	if (strName.GetLength() == 0)
	{
		m_EditName.SetFocus();
		return;
	}
	else
	{
		CString strInfo = "您所输入的目录名称中含有非法字符，请检查。";

		if (strName.Find('\\') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('/') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find(':') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('*') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('?') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find("'") >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('\"') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('<') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('>') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		if (strName.Find('|') >= 0)
		{
			MessageBox(strInfo, "错误", MB_OK | MB_ICONEXCLAMATION);
			m_EditName.SetFocus();
			return;
		}

		this->m_FolderName = strName;
		OnOK();
	}
}

BOOL CFolderNameBuilder::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);

	CUIHelper UIHelper(IDR_GIFMAP_FOLDERBG);
	UIHelper.SetBackPicture(pDC, rc);

	return true;
}

BOOL CFolderNameBuilder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->MoveWindow(0, 0, 282, 168);
	SetWindowText("文件传输");

	SetDlgReign();

	m_btCancel2.SubclassDlgItem(IDC_CANCEL2, this);
	m_btCancel2.SetIcon(IDB_BITMAP_CLOSE2, IDB_BITMAP_CLOSE1);

	m_btCancel.SubclassDlgItem(IDCANCEL, this);
	m_btCancel.SetIcon(IDB_BITMAP_CANCEL2, IDB_BITMAP_CANCEL1);

	m_btOK.SubclassDlgItem(IDOK,this);
	m_btOK.SetIcon(IDB_BITMAP_OK2, IDB_BITMAP_OK1);

	m_EditName.SetFocus();
	SetForegroundWindow();
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	this->CenterWindow();
	return FALSE;  // return TRUE unless you set the focus to a control
}

void CFolderNameBuilder::SetDlgReign()
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

void CFolderNameBuilder::OnBnClickedCancel2()
{
	this->m_FolderName = "";
	OnCancel();
}

LRESULT CFolderNameBuilder::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
}
