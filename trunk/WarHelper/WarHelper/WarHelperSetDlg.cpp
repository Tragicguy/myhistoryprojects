// WarHelperSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WarHelper.h"
#include "WarHelperSetDlg.h"
#include ".\warhelpersetdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarHelperSetDlg dialog


CWarHelperSetDlg::CWarHelperSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarHelperSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarHelperSetDlg)
	m_vk1 = _T("");
	m_vk2 = _T("");
	m_vk4 = _T("");
	m_vk5 = _T("");
	m_vk7 = _T("");
	m_vk8 = _T("");
	//}}AFX_DATA_INIT
}


void CWarHelperSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarHelperSetDlg)
	DDX_Control(pDX, IDC_CHECKWIN, m_winflag);
	DDX_Text(pDX, IDC_NUM1, m_vk1);
	DDX_Text(pDX, IDC_NUM2, m_vk2);
	DDX_Text(pDX, IDC_NUM4, m_vk4);
	DDX_Text(pDX, IDC_NUM5, m_vk5);
	DDX_Text(pDX, IDC_NUM7, m_vk7);
	DDX_Text(pDX, IDC_NUM8, m_vk8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarHelperSetDlg, CDialog)
	//{{AFX_MSG_MAP(CWarHelperSetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarHelperSetDlg message handlers

void CWarHelperSetDlg::InitData(CString str)
{
   	int pos;
    pos =  str.Find(',')+1;
	m_vk7 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());
	
    pos =  str.Find(',')+1;
	m_vk4 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk1 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk8 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());

	pos =  str.Find(',')+1;
	m_vk5 = str.Mid(0,pos-1);
	str = str.Mid(pos,str.GetLength());
	
	pos =  str.Find(',')+1;
	m_vk2 = str.Mid(0,pos-1);
	
    wintemp = str.Mid(pos,str.GetLength());
		
}

void CWarHelperSetDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData();
	CString tempstr,winstr;
	if(m_vk7 == "")  m_vk7 = "0";
	if(m_vk4 == "")  m_vk4 = "0";
	if(m_vk1 == "")  m_vk1 = "0";
	if(m_vk8 == "")  m_vk8 = "0";
	if(m_vk5 == "")  m_vk5 = "0";
	if(m_vk2 == "")  m_vk2 = "0";
	
    
	if(m_winflag.GetCheck() == 1)  winstr = "1";
	else winstr = "0";
    
	tempstr = m_vk7+','+m_vk4+','+m_vk1+','+m_vk8+','+m_vk5+','+ m_vk2 + ',' + winstr;
	
	   
	CStdioFile   file;
	CFileException   fe;   
  if(file.Open("key.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite,&fe))   
  { 
	 file.Close();
	 file.Remove("key.txt");
  	 file.Open("key.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::shareDenyWrite,&fe);
     file.SeekToBegin();
	 file.WriteString(tempstr);
	 file.Close();
	 SetFileAttributes("key.txt",FILE_ATTRIBUTE_HIDDEN);
	 CWarHelperDlg *pHelper = (CWarHelperDlg *)AfxGetMainWnd();
     pHelper->GetData(m_vk7,m_vk4,m_vk1,m_vk8,m_vk5,m_vk2,winstr);
     if(winstr == "1") pHelper->m_WinFlag = TRUE;
	  else pHelper->m_WinFlag = FALSE;
	 pHelper->Begin();
	 pHelper->OnCancel();
	 CDialog::OnOK();
	}
  else  MessageBox("存储出错！");
	
}



 void CWarHelperSetDlg::OnCancel() 
 {
 	// TODO: Add extra cleanup here
 
 	CDialog::OnCancel();
 }

BOOL CWarHelperSetDlg::PreTranslateMessage(MSG*   pMsg)
{
  if(pMsg->message   ==   WM_KEYDOWN)   
  if(int(pMsg->wParam)   ==   VK_ESCAPE)   
  return   TRUE;     
  return   CDialog::PreTranslateMessage(pMsg);
}



BOOL CWarHelperSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if(wintemp == "1")  m_winflag.SetCheck(1);
	else m_winflag.SetCheck(0);
	return TRUE;

}

void CWarHelperSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
