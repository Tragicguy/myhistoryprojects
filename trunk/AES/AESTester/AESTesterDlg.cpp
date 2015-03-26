// AESTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AESTester.h"
#include "AESTesterDlg.h"
#include ".\aestesterdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAESTesterDlg dialog



CAESTesterDlg::CAESTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAESTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAESTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Source);
	DDX_Control(pDX, IDC_EDIT2, m_Destination);
	DDX_Control(pDX, IDC_COMBO1, m_Key);
}

BEGIN_MESSAGE_MAP(CAESTesterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CAESTesterDlg message handlers

BOOL CAESTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Key.InsertString(0,"128 bit");
	m_Key.InsertString(1,"192 bit");
	m_Key.InsertString(2,"256 bit");
	m_Key.SetCurSel(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAESTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAESTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAESTesterDlg::OnBnClickedOk()
{
	char m_SourceFile[MAX_PATH];
	memset(m_SourceFile,0,MAX_PATH);
	if (0==m_Source.GetWindowText(m_SourceFile,MAX_PATH-1))
	{
		AfxMessageBox("Please select source file to be encrypted!",MB_OK|MB_ICONSTOP);
		return;
	}

	char m_DestFolder[MAX_PATH];
	memset(m_DestFolder,0,MAX_PATH);
	if (0==m_Destination.GetWindowText(m_DestFolder,MAX_PATH-1))
	{
		AfxMessageBox("Please select destination folder!",MB_OK|MB_ICONSTOP);
		return;
	}

	int nKeyBit = m_Key.GetCurSel();

	switch(nKeyBit)
	{
	case 0:
		nKeyBit = 128;
		break;
	case 1:
		nKeyBit = 192;
		break;
	case 2:
		nKeyBit = 256;
		break;
	default:
		nKeyBit = 128;
		break;
	}

	const unsigned char key[] = "12345678901234561234567890123456";
	GenerateKey(nKeyBit,key);
	RestoreKey(nKeyBit,key);

	CFile testSourceFile;
	CFile testDesFile;
	try
	{
		testSourceFile.Open(m_SourceFile, CFile::modeRead | CFile::shareDenyNone);
	}
	catch(...)
	{
		AfxMessageBox("Can't open the source file!",MB_OK|MB_ICONSTOP);
		return;
	}

	CString strDesFile;

	try
	{
		strDesFile = CString(m_DestFolder) + "\\Decrypted_" + testSourceFile.GetFileName();
		testDesFile.Open((LPCTSTR)strDesFile, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive);
	}
	catch(...)
	{
		testSourceFile.Close();
		AfxMessageBox("Can't open the destination file!",MB_OK|MB_ICONSTOP);
		return;
	}

	ULONGLONG k = testSourceFile.GetLength();

	const int iLength = 128;
	char* pSource = new char[iLength];
	unsigned char* pEncrypt = new unsigned char[iLength];
	unsigned char* pDecrypt = new unsigned char[iLength];

	ULONGLONG i=0;

	while(i < k)
	{
		memset(pSource,0,iLength);
		memset(pEncrypt,0,iLength);
		memset(pDecrypt,0,iLength);

		int nRead = testSourceFile.Read(pSource,iLength);
		

		if(Encrypt((const unsigned char*)pSource,pEncrypt,iLength))
		{
			if(Decrypt(pEncrypt,pDecrypt,iLength))
			{
				testDesFile.Write(pDecrypt,nRead);

				i+=nRead;
			}
			else
			{
				break;
				AfxMessageBox("Fail!");
			}
		}
		else
		{
			break;
			AfxMessageBox("Fail!");
		}
		
		
	}

	testSourceFile.Close();
	testDesFile.Flush();
	testDesFile.Close();

	delete[] pSource;
	delete[] pEncrypt;
	delete[] pDecrypt;

	AfxMessageBox("Source file has been Encrypted->Decrypted to " + strDesFile,MB_OK);
}

void CAESTesterDlg::OnBnClickedButton1()
{
	
	CFileDialog fileDialog(true);
	fileDialog.DoModal();

	if(fileDialog.GetPathName().GetLength() > 0)
	{
		m_Source.SetWindowText((LPCTSTR)fileDialog.GetPathName());
	}
	
}

void CAESTesterDlg::OnBnClickedButton2()
{
	char szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl;

	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = "Destination Folder";//strDlgTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);
	if(pidl == NULL)	
	return;
	if(!SHGetPathFromIDList(pidl, szDir)) 
	return;

	m_Destination.SetWindowText(szDir);

}

void CAESTesterDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CAESTesterDlg::OnBnClickedButton3()
{
	const unsigned char key[] = "12345678901234561234567890123456";
	GenerateKey(128,key);
	RestoreKey(128,key);

	unsigned char pSource[] = "123456789012345678901234567890123456789012345678901234567890"; //30
	unsigned char pDes[32];
	//memset(pDes,0,21);

	Encrypt(pSource,pDes,32);
	Decrypt(pDes,pSource,32);

	//memset(pDes + 20,0,21);
	MessageBox((LPCTSTR)pSource);
}
