// DialAdslDlg.cpp : 实现文件
//
//本程序由唐添麒编写，现将代码发布供学习交流之用
//本程序的发布遵守GPL协议，您可在此协议的规定下使用本程序
//如果您在使用本程序的途中有什么疑问或建议请访问http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//或者联系作者：qq<254909662> 并说明来意，本人非常乐意与您交流

#include "stdafx.h"
#include "DialAdsl.h"
#include "DialAdslDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CListBox CDialAdslDlg::dialStatusListBox;
CONNNECT_STATUS CDialAdslDlg::dialStatus = MYADSL_INIALTED;
DialManage* CDialAdslDlg::lpDialManage = NULL;

// CDialAdslDlg 对话框




CDialAdslDlg::CDialAdslDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDialAdslDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDialAdslDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTDIALSTATUS, dialStatusListBox);
}

BEGIN_MESSAGE_MAP(CDialAdslDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_DIAL, &CDialAdslDlg::OnBnClickedDial)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDOK, &CDialAdslDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialAdslDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLOSE, &CDialAdslDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_MINI, &CDialAdslDlg::OnBnClickedMini)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialAdslDlg 消息处理程序

BOOL CDialAdslDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
#pragma region //设置按钮图片
	m_btnTest.LoadBitmaps(IDB_DIALMAP);
	m_btnTest.SubclassDlgItem(IDC_DIAL,this);
	m_btnTest.SizeToContent();
	//m_btnTest.AutoLoad(IDC_DIAL, this);IDB_MIN IDB_CLOSE
	m_btnMin.LoadBitmaps(IDB_MIN);
	m_btnMin.SubclassDlgItem(IDC_MINI,this);
	m_btnMin.SizeToContent();

	m_btnClose.LoadBitmaps(IDB_CLOSE);
	m_btnClose.SubclassDlgItem(IDC_CLOSE,this);
	m_btnClose.SizeToContent();
#pragma endregion

	dialStatus = MYADSL_INIALTED;

#pragma region //获取系统中第一个adsl连接的名称
	lpDialManage = new DialManage();
	ASSERT( NULL != lpDialManage );
	if( 1 != lpDialManage ->FindDefaultAdslConnect())
	{
		AfxMessageBox(_T("系统中找不到adsl连接"));
		return TRUE;
	}
	lpDialManage ->GetDialInfo(&dialInformation);
	AfxGetMainWnd() ->SetDlgItemTextA(IDC_EDITUSERNAME, dialInformation.szUserName);
	AfxGetMainWnd() ->SetDlgItemTextA(IDC_EDITPASSWORD, dialInformation.szPassword);
#pragma endregion

	//HBITMAP hBitmap;
	////关联图片ID
	//hBitmap =(HBITMAP)LoadImage(AfxGetInstanceHandle(),
	//	MAKEINTRESOURCE(IDB_BITMAP1),
	//	IMAGE_BITMAP,0,0,
	//	LR_LOADMAP3DCOLORS);
	////获得指向静态控件的指针
	//CStatic *pStatic=(CStatic *)GetDlgItem(IDC_PICTURE);//控件ID
	////设置静态控件的样式，使其可以使用位图，并试位标显示使居中
	//pStatic->ModifyStyle(0xF,SS_BITMAP|SS_CENTERIMAGE);
	////设置静态控件显示位图
	//pStatic->SetBitmap(hBitmap); 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDialAdslDlg::SetBitmap(UINT uBmpResource, UINT uCtrlResource)
{  
	HBITMAP hBitmap;  
	CStatic *pStatic = (CStatic*)GetDlgItem(uCtrlResource);  
	hBitmap = (HBITMAP)LoadImage(  
		AfxGetInstanceHandle(),    
		MAKEINTRESOURCE(uBmpResource),    
		IMAGE_BITMAP,    
		0,    
		0,    
		LR_LOADMAP3DCOLORS);  
	pStatic->ModifyStyle(0xF,   SS_BITMAP|SS_CENTERIMAGE);  
	pStatic->SetBitmap(hBitmap);  
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDialAdslDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	if( MYADSL_FAILED == dialStatus )
	{
		lpDialManage ->RasDialReset();
		dialStatus = MYADSL_INIALTED;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CDialAdslDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDialAdslDlg::OnBnClickedDial()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString buffer;
	//CHAR buffer[1024];
	//ConnectByCmd();
	CHAR buffer[128];
	::RtlZeroMemory(buffer, 128);
	AfxGetMainWnd() ->GetDlgItemTextA(IDC_EDITPASSWORD, buffer, 127);
	lstrcpy(dialInformation.szPassword, buffer);
	::RtlZeroMemory(buffer, 128);
	AfxGetMainWnd() ->GetDlgItemTextA(IDC_EDITUSERNAME, buffer, 127);
	lstrcpy(dialInformation.szUserName, buffer);
	lpDialManage ->SetDialInfo(&dialInformation);

	ConnectByRas();
	//AfxMessageBox("hello");
}


// 拖拽
LRESULT CDialAdslDlg::OnNcHitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	return rc.PtInRect(pt) ? HTCAPTION : CDialog::OnNcHitTest(pt);
}

void CDialAdslDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
	//OnOK();
}

void CDialAdslDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDialAdslDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDialAdslDlg::OnBnClickedMini()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->ShowWindow(SW_SHOWMINIMIZED);
}

HBRUSH CDialAdslDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);   
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDialAdslDlg::ConnectByRas()
{
	if(NULL == lpDialManage)
	{
		return;
	}
	dialStatusListBox.ResetContent();
	int iRet = lpDialManage ->RasDialConnect(RasDialFunc);
	if( 1 == iRet)
	{
		CString temp;
		temp.Format(_T("正在连接到 %s...请稍候"), dialInformation.szEntryName);
		dialStatusListBox.AddString(temp);
		return;
	}
	if(iRet == 99)
	{
		CString temp;
		temp.Format(_T("已经连接成功"));
		dialStatusListBox.AddString(temp);
		return;
	}
	if(iRet == 100)
	{
		CString temp;
		temp.Format(_T("抱歉，系统中没找到拨号连接"));
		dialStatusListBox.AddString(temp);
		return;
	}
	if( iRet > 1)
	{
		CString temp;
		lpDialManage ->GetErrorMessage(iRet, temp.GetBuffer(128), 127);
		//AfxMessageBox(temp.GetBuffer());
		dialStatusListBox.AddString(temp);
		temp.Format(_T("请稍候再试，休息一分钟先"));
		dialStatusListBox.AddString(temp);
		
		//lpDialManage ->RasDialReset();
	}
}

void CDialAdslDlg::ConnectByCmd()
{
	CString strBuffer;
	HANDLE hReadHandle;
	DWORD byteRead;
	if(NULL == lpDialManage)
	{
		return;
	}
	int listIndex = 0;
	int result = lpDialManage ->CmdDialConnect(hReadHandle);
	if( 100 == result)
	{
		AfxMessageBox(_T("系统中没有找到连接"));
		return;
	}
	dialStatusListBox.InsertString(listIndex, _T("正在连接请稍候..."));
	Invalidate();
	if( 1 == result )
	{
		while(true)  
		{  
			strBuffer = "";
			if(ReadFile(hReadHandle,strBuffer.GetBuffer(1024),1023,&byteRead,NULL)==NULL)  
				break;  
			int index = strBuffer.Find(_T("..."), 0);
			if( index > 0 )
			{
				AfxMessageBox(_T("finded ..."));
				AfxMessageBox(strBuffer.GetBuffer());
				CString tempStr;
				tempStr = strBuffer.Mid(0, 14 + 3);
				listIndex++;
				dialStatusListBox.InsertString(listIndex, tempStr);
				int lateIndex;
				tempStr.Format("%d", index);
				AfxMessageBox(tempStr);
				/*while(index > 0)
				{
					lateIndex = strBuffer.Find(_T("..."), index + 3 );
					if(lateIndex > 0)
					{
						AfxMessageBox(_T("while ..."));
						tempStr = strBuffer.Mid(index+3, lateIndex - index);
						listIndex++;
						dialStatusListBox.InsertString(listIndex, tempStr);
					}
					index = lateIndex;
				}*/
				//dialStatusListBox.AddString(_T("\n"));
			}
		}  
	}
}
VOID WINAPI CDialAdslDlg::RasDialFunc(UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	//AfxMessageBox(_T("arrived RasDialFunc"));
	if (unMsg != WM_RASDIALEVENT) return; 
	if( MYADSL_INIALTED == dialStatus)
	{
		dialStatus = MYADSL_DIALING;
	}
	if(dwError != 0)
	{
		ASSERT( MYADSL_DIALING == dialStatus );
		dialStatus = MYADSL_FAILED;
		CHAR buffer[1024];
		lpDialManage ->GetErrorMessage(dwError, buffer, 1023);
		//AfxMessageBox(buffer);
		dialStatusListBox.AddString(buffer);
		return;
	}
	CHAR buffer[128];
	if( RASCS_Authenticate == rasconnstate)
	{
		lstrcpy(buffer, _T("正在验证用户名和密码...") );
		dialStatusListBox.AddString(buffer);
		return;
	}
	if( RASCS_Projected == rasconnstate)
	{
		lstrcpy(buffer, _T("正在网络上注册您的计算机") );
		dialStatusListBox.AddString(buffer);
		return;
	}
	if( RASCS_Connected == rasconnstate)
	{
		lstrcpy(buffer, _T("成功连接") );
		dialStatusListBox.AddString(buffer);
		dialStatus = MYADSL_CONNECTED;
		return;
	}
	return VOID();
}
