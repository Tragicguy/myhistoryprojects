// DialogFile\SetBase.cpp : 实现文件
//

#include "stdafx.h"
#include "../DbGate.h"
#include "SetBase.h"
#include ".\setbase.h"
#include "..\ControlFile\UserFile.h"

// CSetBase 对话框

IMPLEMENT_DYNAMIC(CSetBase, CDialog)
CSetBase::CSetBase(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBase::IDD, pParent)
{
}

CSetBase::~CSetBase()
{
}

void CSetBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetBase, CDialog)

	ON_BN_CLICKED(IDC_RADIO_NORMAL, OnBnClickedRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_OWNSET, OnBnClickedRadioOwnset)
	ON_BN_CLICKED(IDC_RADIO_DBSTORESET, OnBnClickedRadioDbstoreset)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_MAINPORT, OnBnClickedCheckMainport)
	ON_BN_CLICKED(IDC_CHECK_HISTRY, OnBnClickedCheckHistry)
	ON_BN_CLICKED(IDC_CHECK_RECVTABLE, OnBnClickedCheckRecvtable)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL CSetBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);		// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标
	InitDialogs();
	//读取配置类型,然后分析显示哪一个
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//数据库信息
		pFile->ReadInitInfo(strIniFile,"opttype","type",strTemp,iRet,1);
		switch ( iRet )
		{
		case 1:	//常规
			{
				OnBnClickedRadioNormal();
				break;
			}
		case 2: //自定义
			{
				OnBnClickedRadioOwnset();
				break;
			}
		case 3: //存储过程
			{
				OnBnClickedRadioDbstoreset();
				break;
			}
		default:
			{
				MessageBox("无效的配置类型选择","警告");
				//OnBnClickedRadioNormal();
				OnBnClickedRadioDbstoreset();
			}			
		}
		pFile->ReadInitInfo(strIniFile,"status","recv",strTemp,iTemp,0);
		CString strArry[5] = {""};
		pFile->SplitMsg(strTemp,strArry,",",5);

		SetDlgItemText(IDC_EDIT_RECVSTATUS,strArry[0]);
		SetDlgItemText(IDC_EDIT_RECVSTATUS2,strArry[1]);
		SetDlgItemText(IDC_EDIT_RECVSTATUS3,strArry[2]);
		SetDlgItemText(IDC_EDIT_RECVSTATUS4,strArry[3]);
		SetDlgItemText(IDC_EDIT_RECVSTATUS5,strArry[4]);
		pFile->ReadInitInfo(strIniFile,"status","send",strTemp,iTemp,0);
		pFile->SplitMsg(strTemp,strArry,",",2);
		SetDlgItemText(IDC_EDIT_SENDSTATUS1,strArry[0]);
		SetDlgItemText(IDC_EDIT_SENDSTATUS2,strArry[1]);
		//main port
		pFile->ReadInitInfo(strIniFile,"mainport","type",strTemp,iTemp,1);
		if ( iTemp == 1 )
		{
			((CButton*)GetDlgItem(IDC_CHECK_MAINPORT))->SetCheck(1);
			GetDlgItem(IDC_EDIT_MAINPORT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_MAINPORT)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_CHECK_MAINPORT))->SetCheck(0);
		}
		pFile->ReadInitInfo(strIniFile,"mainport","format",strTemp,iTemp,0);
		SetDlgItemText(IDC_EDIT_MAINPORT,strTemp);
		//history format
		pFile->ReadInitInfo(strIniFile,"history","type",strTemp,iTemp,1);
		if ( iTemp == 1 )
		{
			((CButton*)GetDlgItem(IDC_CHECK_HISTRY))->SetCheck(1);
			GetDlgItem(IDC_EDIT_HISTRYFORMAT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_HISTRYFORMAT)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_CHECK_HISTRY))->SetCheck(0);
		}
		pFile->ReadInitInfo(strIniFile,"history","format",strTemp,iTemp,0);
		SetDlgItemText(IDC_EDIT_HISTRYFORMAT,strTemp);
		//recv format
		pFile->ReadInitInfo(strIniFile,"recv","type",strTemp,iTemp,1);
		if ( iTemp == 1 )
		{
			((CButton*)GetDlgItem(IDC_CHECK_RECVTABLE))->SetCheck(1);
			GetDlgItem(IDC_EDIT_RECVFORMAT)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_RECVFORMAT)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_CHECK_RECVTABLE))->SetCheck(0);
		}
		pFile->ReadInitInfo(strIniFile,"recv","format",strTemp,iTemp,0);
		SetDlgItemText(IDC_EDIT_RECVFORMAT,strTemp);
		pFile->ReadInitInfo(strIniFile,"spliter","char",strTemp,iTemp,0);
		SetDlgItemText(IDC_EDIT_SPLITERCHAR,strTemp);
		
	}	
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::InitDialogs()
{
	otherSet.Create(IDD_DLG_OTHERSET,GetDlgItem(IDC_FRAME_BASE));
	otherSetEx.Create(IDD_DLG_OTHERSETEX,GetDlgItem(IDC_FRAME_BASE));
	storeSet.Create(IDD_DLG_STORESET,GetDlgItem(IDC_FRAME_BASE));
	otherSet.MoveWindow(1,8,673,500);
	otherSetEx.MoveWindow(1,8,673,500);
	storeSet.MoveWindow(1,8,673,500);
	otherSet.ShowWindow(SW_HIDE);
	otherSetEx.ShowWindow(SW_HIDE);
	storeSet.ShowWindow(SW_HIDE);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedRadioNormal()
{
	otherSet.ShowWindow(SW_HIDE);
	otherSetEx.ShowWindow(SW_HIDE);
	storeSet.ShowWindow(SW_HIDE);
	otherSetEx.ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO_NORMAL))->SetCheck(TRUE);	
	Invalidate();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedRadioOwnset()
{
	otherSet.ShowWindow(SW_HIDE);
	otherSetEx.ShowWindow(SW_HIDE);
	storeSet.ShowWindow(SW_HIDE);
	otherSet.ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO_OWNSET))->SetCheck(TRUE);
	Invalidate();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedRadioDbstoreset()
{
	otherSet.ShowWindow(SW_HIDE);
	otherSetEx.ShowWindow(SW_HIDE);
	storeSet.ShowWindow(SW_HIDE);
	storeSet.ShowWindow(SW_SHOW);
	((CButton*)GetDlgItem(IDC_RADIO_DBSTORESET))->SetCheck(TRUE);
	Invalidate();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::SaveParams()
{
	CString recvStr = "";
	CString sendStr = "";
	CString strTemp = "";
	GetDlgItemText(IDC_EDIT_RECVSTATUS,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "0";
	}
	recvStr+= strTemp+",";
	GetDlgItemText(IDC_EDIT_RECVSTATUS2,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "1";
	}
	recvStr+= strTemp+",";
	GetDlgItemText(IDC_EDIT_RECVSTATUS3,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "2";
	}
	recvStr+= strTemp+",";
	GetDlgItemText(IDC_EDIT_RECVSTATUS4,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "3";
	}
	recvStr+= strTemp+",";
	GetDlgItemText(IDC_EDIT_RECVSTATUS5,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "4";
	}
	recvStr+= strTemp;
	
	GetDlgItemText(IDC_EDIT_SENDSTATUS1,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "0";
	}
	sendStr+= strTemp+",";
	GetDlgItemText(IDC_EDIT_SENDSTATUS2,strTemp);
	strTemp.Trim();
	if( strTemp == "" )
	{
		strTemp = "1";
	}
	sendStr+= strTemp;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//数据库信息
		pFile->WriteInitInfo(strIniFile,"status","send",sendStr,0,0);
		pFile->WriteInitInfo(strIniFile,"status","recv",recvStr,0,0);
		int iTemp = 0;
		strTemp = "";
		//main port
		iTemp = ((CButton*)GetDlgItem(IDC_CHECK_MAINPORT))->GetCheck();
		GetDlgItemText(IDC_EDIT_MAINPORT,strTemp);
		pFile->WriteInitInfo(strIniFile,"mainport","type",strTemp,iTemp,1);
		pFile->WriteInitInfo(strIniFile,"mainport","format",strTemp,iTemp,0);
		//history
		iTemp = ((CButton*)GetDlgItem(IDC_CHECK_HISTRY))->GetCheck();
		GetDlgItemText(IDC_EDIT_HISTRYFORMAT,strTemp);
		pFile->WriteInitInfo(strIniFile,"history","type",strTemp,iTemp,1);
		pFile->WriteInitInfo(strIniFile,"history","format",strTemp,iTemp,0);
		//recv
		iTemp = ((CButton*)GetDlgItem(IDC_CHECK_RECVTABLE))->GetCheck();
		GetDlgItemText(IDC_EDIT_RECVFORMAT,strTemp);
		pFile->WriteInitInfo(strIniFile,"recv","type",strTemp,iTemp,1);
		pFile->WriteInitInfo(strIniFile,"recv","format",strTemp,iTemp,0);
		GetDlgItemText(IDC_EDIT_SPLITERCHAR,strTemp);
		pFile->WriteInitInfo(strIniFile,"spliter","char",strTemp,iTemp,0);
		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnClose()
{
	SaveParams();
	CDialog::OnClose();
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedCheckMainport()
{
	int iTemp = 0;
	iTemp = ((CButton*)GetDlgItem(IDC_CHECK_MAINPORT))->GetCheck();
	if ( iTemp == 1 )
	{
		GetDlgItem(IDC_EDIT_MAINPORT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MAINPORT)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedCheckHistry()
{
	int iTemp = 0;
	iTemp = ((CButton*)GetDlgItem(IDC_CHECK_HISTRY))->GetCheck();
	if ( iTemp == 1 )
	{
		GetDlgItem(IDC_EDIT_HISTRYFORMAT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_HISTRYFORMAT)->EnableWindow(FALSE);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CSetBase::OnBnClickedCheckRecvtable()
{
	int iTemp = 0;
	iTemp = ((CButton*)GetDlgItem(IDC_CHECK_RECVTABLE))->GetCheck();
	if ( iTemp == 1 )
	{
		GetDlgItem(IDC_EDIT_RECVFORMAT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_RECVFORMAT)->EnableWindow(FALSE);
	}
}
