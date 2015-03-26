// CountGen.cpp : 实现文件
//

#include "stdafx.h"
#include "CountGen.h"
#include ".\countgen.h"
#include "UserMessagePump.h"
#include "datatype.h"
#include "UserFile.h"
// CCountGen 对话框
CArray<CString, CString> g_ContentList;
IMPLEMENT_DYNAMIC(CCountGen, CDialog)
CCountGen::CCountGen(CWnd* pParent /*=NULL*/)
	: CDialog(CCountGen::IDD, pParent)
{
}

CCountGen::~CCountGen()
{
}

void CCountGen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTOCHECKCODE2, m_AutoCheck);
	DDX_Control(pDX, IDC_CHECK_AUTOIP2, m_AutoIP);
}

BEGIN_MESSAGE_MAP(CCountGen, CDialog)
	ON_BN_CLICKED(IDC_BTN_GENCOUNT, OnBnClickedBtnGencount)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_CHECK_AUTOIP2, OnBnClickedCheckAutoip2)
	ON_BN_CLICKED(IDC_CHECK_AUTOCHECKCODE2, OnBnClickedCheckAutocheckcode2)
	ON_BN_CLICKED(IDC_RADIO_IPCHANGE1, OnBnClickedRadioIpchange1)
	ON_BN_CLICKED(IDC_RADIO_IPCHANGE2, OnBnClickedRadioIpchange2)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP1, OnBnClickedRadioCheckip1)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP2, OnBnClickedRadioCheckip2)
	ON_BN_CLICKED(IDC_RADIO_CHECKIP3, OnBnClickedRadioCheckip3)
	ON_BN_CLICKED(IDC_BTN_FIND, OnBnClickedBtnFind)
	ON_BN_CLICKED(IDC_RADIO_LONGYAN, OnBnClickedRadioLongyan)
	ON_BN_CLICKED(IDC_RADIO_SHAOWU, OnBnClickedRadioShaowu)
	ON_BN_CLICKED(IDC_RADIO_PUTIAN, OnBnClickedRadioPutian)
	ON_BN_CLICKED(IDC_CHECK_CHANGEMAC, OnBnClickedCheckChangemac)
	ON_BN_CLICKED(IDC_BTN_IMPORT, OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_IMPORT2, OnBnClickedBtnImport2)
END_MESSAGE_MAP()


BOOL CCountGen::PreTranslateMessage( MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;      //屏蔽掉Esc键
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;      //屏蔽掉Enter键
	}

	return CDialog::PreTranslateMessage( pMsg );
}

BOOL CCountGen::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON hIcon;
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标
	int generateNum = 0;
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";
	CString strIntroMan = "";
	int iTemp = 0;
	//读取配置
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strAccType",strAccType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strPwdType",strPwdType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strMailType",strMailType,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","genNum",strTemp,generateNum,1);
		pFile->CheckInitValue(generateNum,1,1000000);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","type",strTemp,g_iDeleteFlag,1);
		pFile->CheckInitValue(g_iDeleteFlag,0,1);

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","strIntro",strIntroMan,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","width",strTemp,g_iShowWidth,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","height",strTemp,g_iShowHeight,1);
		pFile->CheckInitValue(g_iShowWidth,1,10000);
		pFile->CheckInitValue(g_iShowHeight,1,10000);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","autochangeip",strTemp,g_iAotuChangeIp,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","autocheck",strTemp,g_iAotuCheckCode,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","ipregnum",strTemp,g_iIpRegNum,1);
		pFile->CheckInitValue(g_iAotuChangeIp,0,1);
		pFile->CheckInitValue(g_iAotuCheckCode,0,1);
		pFile->CheckInitValue(g_iIpRegNum,1,100000000);
		m_AutoCheck.SetCheck(g_iAotuCheckCode);
		m_AutoIP.SetCheck(g_iAotuChangeIp);		
		SetDlgItemInt(IDC_EDIT_IPREGNUM,g_iIpRegNum);
		GetDlgItem(IDC_EDIT_IPREGNUM)->EnableWindow(g_iAotuChangeIp);
		CString strRasName = "";
		CString strRasAcc = "";
		CString strRasPwd = "";		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","rasname",strRasName,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","rasacc",strRasAcc,iTemp,0);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","raspwd",strRasPwd,iTemp,0);
		SetDlgItemText(IDC_EDIT_RASNAME,strRasName);
		SetDlgItemText(IDC_EDIT_RASACC,strRasAcc);
		SetDlgItemText(IDC_EDIT_RASPWD,strRasPwd);

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","ipchangetype",strTemp,g_iIpChangeType,1);
		pFile->CheckInitValue(g_iIpChangeType,0,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","timeout",strTemp,g_iTimeOut,1);
		pFile->CheckInitValue(g_iTimeOut,1,50);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","recheckipnum",strTemp,g_iReCheckIpCount,1);
		pFile->CheckInitValue(g_iReCheckIpCount,0,50);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","checkipfalg",strTemp,g_iCheckIpFlag,1);
		pFile->CheckInitValue(g_iCheckIpFlag,0,2);
		SetDlgItemInt(IDC_EDIT_TIMEOUT,g_iTimeOut);
		SetDlgItemInt(IDC_EDIT_CHECKIPCHANGENUM,g_iReCheckIpCount);
		if (g_iIpChangeType == 0)
		{
			OnBnClickedRadioIpchange1();
		}
		else
		{
			OnBnClickedRadioIpchange2();
		}
		switch (g_iCheckIpFlag)
		{
		case 1:
			OnBnClickedRadioCheckip1();
			break;
		case 2:
			OnBnClickedRadioCheckip2();
			break;
		case 0:
		default:
			OnBnClickedRadioCheckip3();
			break;
		}
		OnBnClickedCheckAutoip2();

		pFile->ReadInitInfo(strAppPath+"set.ini","Register","codechecktype",strTemp,g_iCheckCodeType,1);
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","codeerrornum",strTemp,g_iCodeErrorNum,1);
		pFile->CheckInitValue(g_iCheckCodeType,0,1);
		pFile->CheckInitValue(g_iCodeErrorNum,1,100);
		((CButton*)GetDlgItem(IDC_CHECK_TEST))->SetCheck(g_iCheckCodeType);
		SetDlgItemInt(IDC_EDIT_CODEERRORNUM,g_iCodeErrorNum);
		OnBnClickedCheckAutocheckcode2();

		int iCheck = 0;
		CString strEailTail = "";
		CString strNickName = "";
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","nicknametype",strTemp,iCheck,1);
		pFile->CheckInitValue(iCheck,0,1);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","emailtail",strEailTail,iTemp,0);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","nickname",strNickName,iTemp,0);
		SetDlgItemText(IDC_EDIT_MAILTAIL,strEailTail);
		SetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
		((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->SetCheck(iCheck);
		CString strDirect = "";
		CString strFileName = "";		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","direct",strDirect,iTemp,0);		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","filename",strFileName,iTemp,0);
		if (strFileName == "")
		{
			strFileName = "checkcode.bmp";
		}
		if (strDirect == "")
		{
			strDirect = "c:\\";
		}
		SetDlgItemText(IDC_EDIT_PICSAVENAME,strFileName);
		SetDlgItemText(IDC_EDIT_PICPATH,strDirect);
		
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","webflag",strTemp,g_iRegWebFlag,1);
		pFile->CheckInitValue(g_iRegWebFlag,0,2);
		if (g_iRegWebFlag == 0)
		{
			OnBnClickedRadioLongyan();
		}
		else if (g_iRegWebFlag == 1)
		{
			OnBnClickedRadioPutian();
		}
		else
		{
			OnBnClickedRadioShaowu();
		}
		pFile->ReadInitInfo(strAppPath+"set.ini","Register","macchange",strTemp,g_iIsChangeMac,1);
		pFile->CheckInitValue(g_iIsChangeMac,0,1);
		((CButton*)GetDlgItem(IDC_CHECK_CHANGEMAC))->SetCheck(g_iIsChangeMac);		
	}
	SetDlgItemText(IDC_EDIT_ACC,strAccType);
	SetDlgItemText(IDC_EDIT_PWD,strPwdType);
	SetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	SetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	SetDlgItemInt(IDC_EDIT_COUNTNUM,generateNum);
	SetDlgItemInt(IDC_EDIT_SHOWWIDTH,g_iShowWidth);
	SetDlgItemInt(IDC_EDIT_SHOWHEIGHT,g_iShowHeight);
	((CButton*)GetDlgItem(IDC_CHECK_CHECKERROR))->SetCheck(g_iDeleteFlag);

	pFile->ReadInitInfo(strAppPath+"set.ini","Register","timestart",strTemp,g_iTimeStart,1);
	pFile->ReadInitInfo(strAppPath+"set.ini","Register","timesend",strTemp,g_iTimeEnd,1);
	SetDlgItemInt(IDC_EDIT_TIMEOUT2,g_iTimeStart);
	SetDlgItemInt(IDC_EDIT_TIMEOUT3,g_iTimeEnd);

	pFile->ReadInitInfo(strAppPath+"set.ini","Register","freshdelay",strTemp,g_iFreshDelay,1);
	SetDlgItemInt(IDC_EDIT_TIMEOUT4,g_iFreshDelay);
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

//生成账号
void CCountGen::OnBnClickedBtnGencount()
{
	if(GenerateCounts()<0)
	{
		return;
	}
	CString msgBuf	 = "账号生成成功";
	CUserMessagePump *pMsgPump = NULL;
	pMsgPump = CUserMessagePump::GetInstance();
	if (!pMsgPump)
	{
		return;
	}
	pMsgPump->AddToMessagePump(USERWINDOWMSGSUCCESS,msgBuf);
	PostMessage(WM_CLOSE,0,0);
}


/*********************************
 *表示字母
 ?表示数字
 其他为0 (账号仅限用下划线，且不能在第一位)
 密码尽量采用数字加字母
****************************************/
int CCountGen::GenerateCounts()
{
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";
	CString strTemp = "";
	CString strAcc = "";
	CString strPwd = "";
	CString strMail = "";
	CString strIntroMan = "";
	CString strIntroManEx = "";
	CString strNickNameEx = "";
	int generateNum = 0;	
	CString mailtype = "@china.com.cn";
	GetDlgItemText(IDC_EDIT_ACC,strAccType);
	GetDlgItemText(IDC_EDIT_PWD,strPwdType);
	GetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	generateNum = GetDlgItemInt(IDC_EDIT_COUNTNUM);
	g_iShowWidth = GetDlgItemInt(IDC_EDIT_SHOWWIDTH);
	g_iShowHeight = GetDlgItemInt(IDC_EDIT_SHOWHEIGHT);
	GetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	strAccType.Trim();
	strPwdType.Trim();
	strMailType.Trim();
	strIntroMan.Trim();
	int iCheck = ((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->GetCheck();
	GetDlgItemText(IDC_EDIT_MAILTAIL,mailtype);
	CString strNickName = "";
	GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
	strNickName.Trim();
	mailtype.Trim();	
	//保存配置
	if (SaveParams() == -1)
	{
		return -1;
	}
	CString tempStr = "";		//临时变量
	srand(time(NULL));
	for (int i=0;i< generateNum;)
	{
		char inChar;
		char outChar;
		int iLen = 0;
		strAcc = "";
		strPwd = "";
		strMail = "";
		strIntroManEx = "";
		strNickNameEx = "";
		int j = 0;
		iLen = strIntroMan.GetLength();
		for (j=0;j<iLen;j++)		//身份证
		{
			inChar = strIntroMan.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strIntroManEx += tempStr;
		}
		strTemp.Format("%c",DoVerify(strIntroManEx));
		if (strTemp.MakeLower() =="x")
		{
			continue;
		}
		strIntroManEx += strTemp;

		iLen = strAccType.GetLength();
		for (j=0;j<iLen;j++)		//账号
		{
			inChar = strAccType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strAcc += tempStr;
		}
		iLen = strPwdType.GetLength();
		for (j=0;j<iLen;j++)		//密码
		{

			inChar = strPwdType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strPwd += tempStr;
		}
		iLen = strMailType.GetLength();
		for (j=0;j<iLen;j++)		//邮箱
		{

			inChar = strMailType.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strMail += tempStr;
		}
		strMail += mailtype;
		
		iLen = strNickName.GetLength();
		for (j=0;j<iLen;j++)		//邮箱
		{

			inChar = strNickName.GetAt(j);
			outChar = RandChar(inChar);
			tempStr.Format("%c",outChar);
			strNickNameEx += tempStr;
		}
		if (iCheck == 1)
		{
			strNickNameEx = strAcc + strNickNameEx;
		}		
		RegisterPack rgPack;
		rgPack.strAcc = strAcc;					//账号
		rgPack.strPwd = strPwd;					//密码
		rgPack.strMail = strMail;				//邮箱
		rgPack.strNickName = strNickNameEx;		//昵称
		rgPack.strIntroMan = strIntroManEx;		//身份证号
		AddRegisterPack(rgPack);
		int nCount = m_pList->GetItemCount();
		CString listIndex = "";
		listIndex.Format("%d",nCount);
		m_pList->InsertItem(nCount,"",0);
		m_pList->SetItemText(nCount,0,listIndex);
		m_pList->SetItemText(nCount,1,strAcc);
		m_pList->SetItemText(nCount,2,strPwd);
		m_pList->SetItemText(nCount,3,strMail);
		m_pList->SetItemText(nCount,4,strIntroManEx);
		m_pList->SetItemText(nCount,7,strNickNameEx);
		m_pList->SetItemText(nCount,8,"");
		i++;
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
char CCountGen::RandChar(char inChar)
{
	char outChar;
	if (inChar == '?')	//数字
	{
		outChar = rand() % 10 + 48;
	}
	else if (inChar == '*') //字母
	{
		int num = rand() % 26;
		outChar = num + 97;
	}
	else
	{
		int num = (int)inChar;
		if ((num >= 48 && num <=57)   ||
			(num >= 65 && num <=90) ||
			(num >= 97 && num <=122)||
			inChar == '_')
		{
			outChar = inChar;
		}
		else
		{
			outChar = 48;
		}
	}
	return outChar;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnSave()
{
	if (SaveParams() < 0)
	{
		return;
	}	
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
int CCountGen::SaveParams()
{
	CString strAccType = "";
	CString strPwdType = "";
	CString strMailType = "";

	CString strAcc = "";
	CString strPwd = "";
	CString strMail = "";
	CString strIntroMan = "";	
	int generateNum = 0;
	CString mailtype = "@china.com.cn";
	GetDlgItemText(IDC_EDIT_ACC,strAccType);
	GetDlgItemText(IDC_EDIT_PWD,strPwdType);
	GetDlgItemText(IDC_EDIT_MAILACC,strMailType);
	generateNum = GetDlgItemInt(IDC_EDIT_COUNTNUM);
	g_iShowWidth = GetDlgItemInt(IDC_EDIT_SHOWWIDTH);
	g_iShowHeight = GetDlgItemInt(IDC_EDIT_SHOWHEIGHT);
	GetDlgItemText(IDC_EDIT_INTROMAN,strIntroMan);
	strAccType.Trim();
	strPwdType.Trim();
	strMailType.Trim();
	strIntroMan.Trim();
	g_iFreshDelay = GetDlgItemInt(IDC_EDIT_TIMEOUT4);
	if (g_iFreshDelay < 1 || g_iFreshDelay > 5)
	{
		MessageBox("搜索深度最大只能设置为5","错误");
		return -1;
	}
	CString strNickName = "";
	GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
	GetDlgItemText(IDC_EDIT_MAILTAIL,mailtype);
	strNickName.Trim();
	mailtype.Trim();
	if (strAccType == "" || strPwdType == "" || strMailType == "" || mailtype=="" )
	{
		MessageBox("请输入生成格式信息","错误");
		return -1;
	}
	if (strIntroMan.GetLength() != 17)
	{
		MessageBox("请填写身份证的前17位","错误");
		return -1;
	}
	CString strDirect = "";
	CString strFileName = "";
	GetDlgItemText(IDC_EDIT_PICPATH,strDirect);
	GetDlgItemText(IDC_EDIT_PICSAVENAME,strFileName);
	strDirect.Trim();
	strFileName.Trim();
	if (strFileName == BMPFILE_PATH)
	{
		MessageBox("请填写其他文件名","错误");
		return -1;
	}
	CString strRasName = "";
	CString strRasAcc = "";
	CString strRasPwd = "";
	GetDlgItemText(IDC_EDIT_RASNAME,strRasName);
	GetDlgItemText(IDC_EDIT_RASACC,strRasAcc);
	GetDlgItemText(IDC_EDIT_RASPWD,strRasPwd);
	g_iDeleteFlag = ((CButton*)GetDlgItem(IDC_CHECK_CHECKERROR))->GetCheck();
	g_iIpRegNum = GetDlgItemInt(IDC_EDIT_IPREGNUM);
	//保存配置
	CString strAppPath = "";
	CString strTemp = "";
	CUserFile *pFile = CUserFile::GetInstance();
	if (pFile)
	{
		pFile->GetAppPath(strAppPath);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strAccType",strAccType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strPwdType",strPwdType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strMailType",strMailType,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","strIntro",strIntroMan,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","genNum","",generateNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","type","",g_iDeleteFlag,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","width","",g_iShowWidth,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","height","",g_iShowHeight,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","autochangeip",strTemp,g_iAotuChangeIp,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","autocheck",strTemp,g_iAotuCheckCode,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","ipregnum",strTemp,g_iIpRegNum,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","rasname",strRasName,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","rasacc",strRasAcc,0,0);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","raspwd",strRasPwd,0,0);
		//获取IP检测方式
		int iCheck = 0;
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->GetCheck();
		g_iIpChangeType = iCheck;		
        g_iTimeOut = GetDlgItemInt(IDC_EDIT_TIMEOUT);
		g_iReCheckIpCount = GetDlgItemInt(IDC_EDIT_CHECKIPCHANGENUM);
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->GetCheck();
		if (iCheck == 1)
		{
			g_iCheckIpFlag = 1;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->GetCheck()==1)
		{
			g_iCheckIpFlag = 2;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->GetCheck()==1)
		{
			g_iCheckIpFlag = 0;
		}
		else
		{
			g_iCheckIpFlag = 0;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","ipchangetype",strTemp,g_iIpChangeType,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","timeout",strTemp,g_iTimeOut,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","recheckipnum",strTemp,g_iReCheckIpCount,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","checkipfalg",strTemp,g_iCheckIpFlag,1);
		g_iCheckCodeType = ((CButton*)GetDlgItem(IDC_CHECK_TEST))->GetCheck();
		g_iCodeErrorNum = GetDlgItemInt(IDC_EDIT_CODEERRORNUM);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","codechecktype",strTemp,g_iCheckCodeType,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","codeerrornum",strTemp,g_iCodeErrorNum,1);
		iCheck = ((CButton*)GetDlgItem(IDC_CHECK_NAMEADD))->GetCheck();
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","nicknametype",strTemp,iCheck,1);
		CString strEailTail = "";
		GetDlgItemText(IDC_EDIT_MAILTAIL,strEailTail);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","emailtail",strEailTail,0,0);
		CString strNickName = "";
		GetDlgItemText(IDC_EDIT_NICKNAME,strNickName);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","nickname",strNickName,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","direct",strDirect,0,0);		
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","filename",strFileName,0,0);
		iCheck = ((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->GetCheck();
		if (iCheck == 1)
		{
			g_iRegWebFlag = 0;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->GetCheck()==1)
		{
			g_iRegWebFlag = 1;
		}
		else if (((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->GetCheck()==1)
		{
			g_iRegWebFlag = 2;
		}
		else
		{
			g_iRegWebFlag = 2;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","webflag","",g_iRegWebFlag,1);
		g_strDirect = strDirect;
		g_strFileName = strFileName;
		g_iIsChangeMac = ((CButton*)GetDlgItem(IDC_CHECK_CHANGEMAC))->GetCheck();
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","macchange","",g_iIsChangeMac,1);

		g_iTimeStart = GetDlgItemInt(IDC_EDIT_TIMEOUT2);
		g_iTimeEnd = GetDlgItemInt(IDC_EDIT_TIMEOUT3);
		if (g_iTimeStart >= g_iTimeEnd)
		{
			AfxMessageBox("时间设置非法");
			return -1;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","timestart","",g_iTimeStart,1);
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","timesend","",g_iTimeEnd,1);
		g_iFreshDelay = GetDlgItemInt(IDC_EDIT_TIMEOUT4);
		if (g_iFreshDelay < 0)
		{
			AfxMessageBox("页面刷新时间设置非法");
			return -1;
		}
		pFile->WriteInitInfo(strAppPath+"set.ini","Register","freshdelay","",g_iFreshDelay,1);
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckAutoip2()
{
	g_iAotuChangeIp = m_AutoIP.GetCheck();
	int iCheck = ((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->GetCheck();
	GetDlgItem(IDC_RADIO_IPCHANGE1)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_RADIO_IPCHANGE2)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_IPREGNUM)->EnableWindow(g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASNAME)->EnableWindow(g_iAotuChangeIp&&iCheck);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(g_iAotuChangeIp&&iCheck);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(g_iAotuChangeIp&&iCheck);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckAutocheckcode2()
{
	g_iAotuCheckCode = m_AutoCheck.GetCheck();
	GetDlgItem(IDC_CHECK_TEST)->EnableWindow(g_iAotuCheckCode);
}
/************************************************************************/
/*生成最后一位校验码                                                    */
/************************************************************************/
char CCountGen::DoVerify(const char* pszSrc)
{
	int iS = 0;
	int iW[]={7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
	static char szVerCode[]="10X98765432";
	int i;
	for(i=0;i<17;i++)
	{
		iS += (int)(pszSrc[i]-'0') * iW[i];
	}
	int iY = iS%11;
	return szVerCode[iY];
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioIpchange1()
{
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->SetCheck(0);
	GetDlgItem(IDC_EDIT_RASNAME)->EnableWindow(1&&g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(1&&g_iAotuChangeIp);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(1&&g_iAotuChangeIp);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioIpchange2()
{
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_IPCHANGE2))->SetCheck(1);	
	GetDlgItem(IDC_EDIT_RASNAME)->EnableWindow(0);
	GetDlgItem(IDC_EDIT_RASACC)->EnableWindow(0);
	GetDlgItem(IDC_EDIT_RASPWD)->EnableWindow(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip1()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(0);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip2()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(0);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioCheckip3()
{
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_CHECKIP3))->SetCheck(1);
	GetDlgItem(IDC_EDIT_CHECKIPCHANGENUM)->EnableWindow(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnFind()
{
	CString sFolderPath;
	BROWSEINFO bi;
	char Buffer[MAX_PATH]; //初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer; //此参数如为NULL则不能显示对话框
	bi.lpszTitle = "选择路径";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;   //初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//取得文件夹路径到Buffer里
		sFolderPath.Format("%s",Buffer);//将路径保存在一个CString对象里
		if (sFolderPath.GetLength() > 0)
		{
			if (sFolderPath.Right(1) != "\\")
			{
				sFolderPath +="\\";
			}
		}
	}
	SetDlgItemText(IDC_EDIT_PICPATH,sFolderPath);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioLongyan()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioShaowu()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedRadioPutian()
{
	((CButton*)GetDlgItem(IDC_RADIO_LONGYAN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_SHAOWU))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_PUTIAN))->SetCheck(1);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedCheckChangemac()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnImport()
{
	if (SaveParams() < 0)
	{
		return;
	}
	ImportUrlData();
	AfxMessageBox("账号已导入,接下来导入顶帖内容");
	ImportSubContent();
	PostMessage(WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::ImportUrlData()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int idx1=0;
		int idx2=0;
		srand(time(NULL));
		while (file.ReadString(cstr))
		{
			strAcc = "";
			strPwd = "";
			strPhone = "";
			strRecommandPhone = "";
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strAcc = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPwd = cstr.Left(idx2);
			cstr = cstr.Right(cstr.GetLength()-idx2-1);
			idx2 =cstr.Find(",");
			if (idx2==-1)
			{
				continue;
			}
			strPhone = cstr.Left(idx2);
			//已经找到所有需要的数据
			if (strAcc !="" && strPwd != "" && strPhone != "")
			{
				int nCount = m_pList->GetItemCount();
				CString listIndex = "";
				int iIsExist = 0;
				for (int i=0;i<nCount;i++)
				{
					if (strAcc == m_pList->GetItemText(i,1) && strPhone == m_pList->GetItemText(i,4))
					{
						CString strShow = "";
						strShow.Format("%s 重复导入",strAcc);
						CUserMessagePump *pMsgPump = NULL;
						pMsgPump = CUserMessagePump::GetInstance();
						if (!pMsgPump)
						{
							return;
						}
						pMsgPump->AddToMessagePump(USERWINDOWMSG_1,strShow);
						iIsExist = 1;
						break;
					}
				}
				if (iIsExist == 1)
				{
					continue;
				}

				RegisterPack rgPack;
				rgPack.strAcc = strAcc;					//账号
				rgPack.strPwd = strPwd;					//密码
				rgPack.strMail = strPhone;				//邮箱		
				rgPack.iLastTime = -10000;
				AddRegisterPack(rgPack);
				nCount = m_pList->GetItemCount();
				listIndex.Format("%d",nCount);
				m_pList->InsertItem(nCount,"",0);
				m_pList->SetItemText(nCount,0,listIndex);
				m_pList->SetItemText(nCount,1,strAcc);
				m_pList->SetItemText(nCount,2,strPwd);
				m_pList->SetItemText(nCount,4,strPhone);
				m_pList->SetItemText(nCount,3,"0");
			}
		}
		file.Close();
	}	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::ImportSubContent()
{
	CString strAcc="";
	CString strPwd="";
	CString strPhone = "";
	CString strRecommandPhone = "";
	int   lines   =   0;   
	CStdioFile   file;
	CString   cstr;
	CFileException   fe;
	CString strFilePath = "";
	char szFilter[]="Text files(*.txt)|*.txt";
	CFileDialog dlg(1,NULL,NULL,NULL,szFilter); 
	if(dlg.DoModal()==IDOK) 
	{
		strFilePath = dlg.GetPathName();
	}
	if (strFilePath=="")
	{
		return;
	}
	if(file.Open(strFilePath,CFile::modeRead,&fe))   
	{ 
		file.SeekToBegin();
		int iCount = 0;
		while (file.ReadString(cstr))
		{
			cstr.Trim();
			if (cstr == "")
			{
				continue;
			}
			g_ContentList.Add(cstr);
			iCount++;
		}
		file.Close();
		strPhone.Format("共导入%d条内容",iCount);
		ShowMessage(strPhone);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCountGen::OnBnClickedBtnImport2()
{
	
}
