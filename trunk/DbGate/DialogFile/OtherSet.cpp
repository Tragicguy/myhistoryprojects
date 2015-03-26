#include "stdafx.h"
#include "../DbGate.h"
#include "../resource.h"
#include "OtherSet.h"
#include "../ControlFile/UserFile.h"
#include ".\otherset.h"

//设置自定义数据库操作的对话框
IMPLEMENT_DYNAMIC(COtherSet, CDialog)
COtherSet::COtherSet(CWnd* pParent /*=NULL*/)
	: CDialog(COtherSet::IDD, pParent)
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
COtherSet::~COtherSet()
{
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPTTYPE, m_ComboSql);
	DDX_Control(pDX, IDC_LIST2, m_List);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
BEGIN_MESSAGE_MAP(COtherSet, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedBtnSave)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, OnNMRclickListMapping)
	ON_MESSAGE(WM_SET_ITEMS, PopulateComboList)
	ON_MESSAGE(WM_VALIDATE,UpdateValue)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnNMDblclkListMapping)
	ON_COMMAND(ID_MENU_INSERTOWN, OnMenuInsertown)
	ON_COMMAND(ID_LST_DELETEOWN, OnLstDeleteown)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTTYPE, OnCbnSelchangeComboOpttype)
	ON_BN_CLICKED(IDC_BTN_ADD_FIELD, OnBnClickedBtnAddField)
END_MESSAGE_MAP()

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL COtherSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_iOptFlag = 0;		//读取
	InitKeyMap();
	InitParams();
	InitListCtrl();		//初始化配置
	InitSqlCombo();
	GetDlgItem(IDC_COMBO_OPTTYPE)->MoveWindow(80,15,150,300);
	m_ComboSql.SetCurSel(0);
	InitSqlString();
	return TRUE;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnBnClickedBtnSave()
{
	switch( m_iOptFlag )
	{	
	case 0:		//读取
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //保存成功结果
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
			SaveMapping(m_strSucAddResultIndexList);
			break;
		}
// 	case 2://保存失败结果
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 			SaveMapping(m_strFailAddResultIndexList);
// 			break;
// 		}
	case 2://成功清理等待发送
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
			SaveMapping(m_strSucCleanIndexList);
			break;
		}
// 	case 4:		//失败处理
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 			SaveMapping(m_strFailCleanIndexList);
// 			break;
// 		}
	case 3:	//上行信息
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 4://状态报告
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	CString strTemp = "";
	int iTemp = 0;
	CString strFurtherName = "own";
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{
		m_strReadSql = pFile->StrEncode(m_strReadSql);	
		m_strReadIndexList = pFile->StrEncode(m_strReadIndexList);

		m_strSucAddResultSql = pFile->StrEncode(m_strSucAddResultSql);	
		m_strSucAddResultIndexList = pFile->StrEncode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrEncode(m_strFailAddResultSql);	
		m_strFailAddResultIndexList = pFile->StrEncode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrEncode(m_strSucCleanSql);
		m_strSucCleanIndexList = pFile->StrEncode(m_strSucCleanIndexList);
		m_strFailCleanSql = pFile->StrEncode(m_strFailCleanSql);	
		m_strFailCleanIndexList = pFile->StrEncode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrEncode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrEncode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrEncode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrEncode(m_strAddMoIndexList);

		int iRet = -1;
		CString strAppPath = "";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//读取
		pFile->WriteInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//保存发送结果
		pFile->WriteInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//清理待发信息
		pFile->WriteInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		pFile->WriteInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//状态报告处理
		pFile->WriteInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);			pFile->WriteInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//上行信息处理
		pFile->WriteInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		pFile->WriteInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);
		//数据库操作类型设置
		pFile->WriteInitInfo(strIniFile,"opttype","type",strTemp,2,1);
	}
	::PostMessage(this->GetParent()->GetParent()->m_hWnd,WM_CLOSE,0,0);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitParams()
{	
	CString strTemp = "";
	int iTemp = 0;
	CUserFile *pFile = NULL;
	pFile = CUserFile::GetInstance();
	if (pFile)
	{	
		int iRet = -1;
		CString strAppPath = "";
		CString strFurtherName = "own";
		pFile->GetAppPath(strAppPath);
		CString strIniFile = strAppPath + DBSETINITFILE;
		//读取
		pFile->ReadInitInfo(strIniFile,"db","read"+strFurtherName,m_strReadSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","readindex"+strFurtherName,m_strReadIndexList,iTemp,0);
		//发送成功后保存
		pFile->ReadInitInfo(strIniFile,"db","sucadd"+strFurtherName,m_strSucAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","sucaddindex"+strFurtherName,m_strSucAddResultIndexList,iTemp,0);
		//发送失败后处理
		pFile->ReadInitInfo(strIniFile,"db","failadd"+strFurtherName,m_strFailAddResultSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failaddindex"+strFurtherName,m_strFailAddResultIndexList,iTemp,0);
		//发送成功处理待发信息
		pFile->ReadInitInfo(strIniFile,"db","succlean"+strFurtherName,m_strSucCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","succleanindex"+strFurtherName,m_strSucCleanIndexList,iTemp,0);		
		//发送失败处理待发信息
		pFile->ReadInitInfo(strIniFile,"db","failclean"+strFurtherName,m_strFailCleanSql,iTemp,0);
		pFile->ReadInitInfo(strIniFile,"db","failcleanindex"+strFurtherName,m_strFailCleanIndexList,iTemp,0);
		//状态报告处理
		pFile->ReadInitInfo(strIniFile,"db","update"+strFurtherName,m_strUpdateSql,iTemp,0);			pFile->ReadInitInfo(strIniFile,"db","updateindex"+strFurtherName,m_strUpdateIndexList,iTemp,0);
		//上行信息处理
		pFile->ReadInitInfo(strIniFile,"db","addmo"+strFurtherName,m_strAddMoSql,iTemp,0);		
		pFile->ReadInitInfo(strIniFile,"db","addmoindex"+strFurtherName,m_strAddMoIndexList,iTemp,0);

		m_strReadSql = pFile->StrDecode(m_strReadSql);	
		m_strReadIndexList = pFile->StrDecode(m_strReadIndexList);

		m_strSucAddResultSql = pFile->StrDecode(m_strSucAddResultSql);	
		m_strSucAddResultIndexList = pFile->StrDecode(m_strSucAddResultIndexList);

		m_strFailAddResultSql = pFile->StrDecode(m_strFailAddResultSql);	
		m_strFailAddResultIndexList = pFile->StrDecode(m_strFailAddResultIndexList);

		m_strSucCleanSql = pFile->StrDecode(m_strSucCleanSql);
		m_strSucCleanIndexList = pFile->StrDecode(m_strSucCleanIndexList);
		m_strFailCleanSql = pFile->StrDecode(m_strFailCleanSql);	
		m_strFailCleanIndexList = pFile->StrDecode(m_strFailCleanIndexList);

		m_strUpdateSql = pFile->StrDecode(m_strUpdateSql);	
		m_strUpdateIndexList = pFile->StrDecode(m_strUpdateIndexList);	

		m_strAddMoSql = pFile->StrDecode(m_strAddMoSql);	
		m_strAddMoIndexList = pFile->StrDecode(m_strAddMoIndexList);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitComString( CStringList* pComboList)
{
	for (POSITION Pos_ = m_cstringList.GetHeadPosition(); Pos_ != NULL;)
	{
		pComboList->AddTail(m_cstringList.GetNext(Pos_));
	}
	return;	
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu obMenu;
	obMenu.LoadMenu(IDR_MENU2);
	CMenu* pPopupMenu = obMenu.GetSubMenu(0);
	ASSERT(pPopupMenu); 
	// Get the cursor position
	CPoint obCursorPoint = (0, 0);
	GetCursorPos(&obCursorPoint);
	if (0 >= m_List.GetSelectedCount())
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETEOWN, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED); 
	}
	else
	{
		pPopupMenu->EnableMenuItem(ID_LST_DELETEOWN, MF_BYCOMMAND | MF_ENABLED);
	}
	// Track the popup menu
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, obCursorPoint.x,obCursorPoint.y, this);
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitKeyMap()
{
	m_keyMap.clear();
	m_cstringList.RemoveAll();
	switch ( m_iOptFlag )
	{
	case 0:		//读取
	case 1:		//发送
	case 2:		//处理等待发送信息
	{
			m_keyMap["本地ID[int]"] = 8;//			
			m_keyMap["信息内容"] = 19;//
			m_keyMap["接收号码[预处理]"] = 16;//
			m_keyMap["接收号码"] = 15;//
			m_keyMap["发送方号码"] = 6;
			//m_keyMap["发送类型[int]"] = 1;	//
			m_keyMap["重试次数"] = 5;		//
			m_keyMap["信息长度"] = 6;		//
			m_keyMap["短信优先级"] = 7;		//	
			m_keyMap["保留字段1[int]"] = 9;//
			m_keyMap["保留字段2[int]"] = 10;//
			m_keyMap["保留字段3[int]"] = 11;//
			m_keyMap["保留字段4[int]"] = 12;//
			m_keyMap["保留字段5[int]"] = 13;//
			m_keyMap["发送类型[string]"] = 14;
			m_keyMap["消息本地标识"] = 17;//
			m_keyMap["消息ID"]	= 18;
			m_keyMap["信息生成时间"] = 20;//
			m_keyMap["信息提交时间"] = 21;//
			m_keyMap["信息发送时间"] = 22;//
			m_keyMap["操作员ID"] = 23;//
			m_keyMap["服务ID"] = 24;
			m_keyMap["任务名"] = 25;
			m_keyMap["本地ID[string]"] = 26;
			m_keyMap["子发送类型"] = 27;
			m_keyMap["子端口"] = 28;
			m_keyMap["用户ID"] = 29;
			m_keyMap["短信发送状态"] = 30;
			m_keyMap["短信接收状态"] = 31;
			m_keyMap["主端口"] = 32;
			m_keyMap["失败原因"] = 33;
			m_keyMap["保留字段1[string]"] = 34;
			m_keyMap["保留字段2[string]"] = 35;	
			m_keyMap["接收号码数"] = 3;

			m_cstringList.AddTail("本地ID[int]");
			m_cstringList.AddTail("本地ID[string]");//26;
			m_cstringList.AddTail("信息内容");//19;//
			//m_cstringList.AddTail("接收号码[预处理]");//16;//
			m_cstringList.AddTail("接收号码");//15;//
			m_cstringList.AddTail("接收号码数");//3;
			m_cstringList.AddTail("发送方号码");//6;			
			m_cstringList.AddTail("短信优先级");//7;	//
			//m_cstringList.AddTail("发送类型[int]");//1;	//
			m_cstringList.AddTail("发送类型[string]");//14;
			m_cstringList.AddTail("子发送类型");//27;
			m_cstringList.AddTail("消息ID");// 18;
			m_cstringList.AddTail("消息本地标识");//17;//		
			m_cstringList.AddTail("信息生成时间");//20;//
			m_cstringList.AddTail("信息提交时间");//21;//
			m_cstringList.AddTail("信息发送时间");//22;//
			m_cstringList.AddTail("操作员ID");//23;//
			m_cstringList.AddTail("服务ID");//24;
			m_cstringList.AddTail("任务名");//25;						
			m_cstringList.AddTail("用户ID");//29;
			m_cstringList.AddTail("短信发送状态");//30;
			m_cstringList.AddTail("短信接收状态");//31;
			m_cstringList.AddTail("重试次数");//5;		//
			m_cstringList.AddTail("信息长度");//6;		//
			m_cstringList.AddTail("子端口");//28;
			m_cstringList.AddTail("主端口");//32;
			m_cstringList.AddTail("失败原因");//33;
			m_cstringList.AddTail("保留字段1[int]");//9;//
			m_cstringList.AddTail("保留字段2[int]");//10;//
			m_cstringList.AddTail("保留字段3[int]");//11;//
			m_cstringList.AddTail("保留字段4[int]");//12;//
			m_cstringList.AddTail("保留字段5[int]");//13;//
			m_cstringList.AddTail("保留字段1[string]");//34;
			m_cstringList.AddTail("保留字段2[string]");//35;			
			break;
		}
	case 3:		//上行
		{
			m_keyMap["接收日期"] = 1;
			m_keyMap["接收时间"] = 2;
			m_keyMap["手机号码"] = 3;
			m_keyMap["通道号码"] = 4;
			m_keyMap["信息内容"] = 6;
			m_keyMap["保留字段"] = 5;
			//m_cstringList.AddTail("接收日期");
			m_cstringList.AddTail("接收时间");
			m_cstringList.AddTail("手机号码");
			m_cstringList.AddTail("通道号码");
			//m_cstringList.AddTail("发送方号码");
			m_cstringList.AddTail("信息内容");
			m_cstringList.AddTail("保留字段");
			break;
		}
	case 4:		//状态报告
		{
			m_keyMap["接收日期"] = 1;
			m_keyMap["接收时间"] = 2;
			m_keyMap["信息编号"] = 3;
			m_keyMap["信息状态"] = 5;
			m_keyMap["信息详细错误"] = 6;
			m_keyMap["保留字段"] = 4;
			//m_cstringList.AddTail("接收日期");
			m_cstringList.AddTail("接收时间");
			m_cstringList.AddTail("信息编号");
			m_cstringList.AddTail("信息状态");
			m_cstringList.AddTail("信息详细错误");
			m_cstringList.AddTail("保留字段");
			break;
		}
	default:
		{
			m_keyMap.clear();
			break;
		}
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitListCtrl()
{
	DWORD dwSytle=::GetWindowLong(m_List.m_hWnd,GWL_STYLE);
	//设置为报表形式
	SetWindowLong(m_List.m_hWnd,GWL_STYLE,dwSytle|LVS_REPORT);
	DWORD ExStyle=m_List.GetExtendedStyle();
	m_List.SetExtendedStyle(ExStyle|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_List.InsertColumn(0,_T("字段名"),LVCFMT_LEFT,180);
	m_List.InsertColumn(1,_T("映射到结构体名"),LVCFMT_LEFT,120);
	m_List.SetComboColumns(1);
	m_List.SetReadOnlyColumns(1);
	//m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitSqlCombo()
{
	m_ComboSql.AddString("读取待发信息");
	m_ComboSql.AddString("保存发送结果");
	//m_ComboSql.AddString("保存发送结果[失败]");
	m_ComboSql.AddString("处理待发信息");
	//m_ComboSql.AddString("发送[失败]后处理待发信息");
	m_ComboSql.AddString("保存上行信息");
	m_ComboSql.AddString("更新发送结果");
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSet::UpdateValue(WPARAM wParam, LPARAM lParam)
{
	// 	POSITION SelectedItemPos = m_List.GetFirstSelectedItemPosition();
	// 	int iIndex = m_List.GetNextSelectedItem(SelectedItemPos);
	// 	CString strMapValue = m_List.GetItemText(iIndex,1);
	// 	m_List.SetItemText(iIndex,2,CStringToIntCString(strMapValue));
	return 0;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString COtherSet::IntToCString( int iIndex )
{
	map<CString,int>::iterator it;   
	for(it=m_keyMap.begin();it!=m_keyMap.end();++it)   
	{
		int iFlag = it->second;
		if (iFlag == iIndex)
		{
			return it->first;
		}
	}
	return "";
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CString COtherSet::CStringToIntCString( CString strKey )
{
	CString strRet = "";
	int iRet = m_keyMap[strKey];
	strRet.Format("%d",iRet);
	return strRet;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
LRESULT COtherSet::PopulateComboList(WPARAM wParam, LPARAM lParam)
{
	// Get the Combobox window pointer
	CComboBox* pInPlaceCombo = static_cast<CComboBox*> (GetFocus());
	//去掉排序属性
	// 	DWORD nStyle   =   GetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE);   
	// 	nStyle   =   nStyle &(~ LBS_SORT);   
	// 	SetWindowLong(pInPlaceCombo->m_hWnd,GWL_STYLE,nStyle);   

	CRect obWindowRect;
	pInPlaceCombo->GetWindowRect(&obWindowRect);
	//pInPlaceCombo->MoveWindow(obWindowRect.left,obWindowRect.top,obWindowRect.Width(),500);
	CPoint obInPlaceComboTopLeft(obWindowRect.TopLeft()); 
	// Get the active list
	// Get the control window rect
	// If the inplace combobox top left is in the rect then
	// The control is the active control
	m_List.GetWindowRect(&obWindowRect);
	
	int iColIndex = wParam;
	CStringList* pComboList = reinterpret_cast<CStringList*>(lParam);
	pComboList->RemoveAll();
	if (obWindowRect.PtInRect(obInPlaceComboTopLeft)) 
	{
		if (1 == iColIndex)
		{
			InitComString(pComboList);
		}		
	}
	return 0;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnMenuInsertown()
{
	int iCount = m_List.GetItemCount();
	int iRow = -1;
	POSITION SelectedItemPos = m_List.GetFirstSelectedItemPosition();
	int iIndex = -1;
	int iLastSelectedIndex = -1;

	while (SelectedItemPos)
	{
		iIndex = m_List.GetNextSelectedItem(SelectedItemPos);

		if ((LVIS_SELECTED == m_List.GetItemState(iIndex, LVIS_SELECTED)) && 
			(LVIS_FOCUSED == m_List.GetItemState(iIndex, LVIS_FOCUSED)))
		{
			iRow = iIndex;
		}
		m_List.SetItemState(iIndex, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
	if (-1 == iRow)
	{
		m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iCount, 
			"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	}
	else
	{
		m_List.InsertItem(LVIF_TEXT|LVIF_STATE, iRow, 
			"fieldname", LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED, 0, 0);
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnLstDeleteown()
{
	POSITION SelectionPos = m_List.GetFirstSelectedItemPosition();
	int iCurSel = -1;
	// Delete the selected item from the list control 
	while (NULL != SelectionPos)
	{
		SelectionPos = m_List.GetFirstSelectedItemPosition();
		iCurSel = m_List.GetNextSelectedItem(SelectionPos);
		m_List.DeleteItem(iCurSel);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::SaveMapping(CString &strList)
{
	strList.Empty();
	CString strTemp = "";
	int iCount = m_List.GetItemCount();
	for ( int i=0;i<iCount;i++)
	{
		strTemp = m_List.GetItemText(i,0);
		strTemp.Trim();
		if ( strTemp != "" )
		{
			strList += strTemp+",";
			strTemp = CStringToIntCString(m_List.GetItemText(i,1));
			strList +=strTemp+";";
		}		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitMapping( CString strMap )
{
	//strMap += ";";
	CString strTemp = "";
	int iPos = 0 ;
	int idx = 0;
	m_List.DeleteAllItems();
	int iCount = 0;
	while( true )
	{
		try
		{
			iPos = strMap.Find( ';' );
			if ( iPos != -1 )
			{
				strTemp = strMap.Left( iPos );
				idx = strTemp.Find(',');
				iCount = m_List.GetItemCount();
				m_List.InsertItem(iCount,strTemp.Left(idx));
				m_List.SetItemText( iCount,1,IntToCString(atoi(strTemp.Mid(idx+1))) );
				strMap = strMap.Mid(iPos+1);
			}
			else
			{
				break;
			}
		}
		catch (...)
		{
		}		
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::InitSqlString()
{
	switch( m_iOptFlag )
	{	
		case 0:		//读取
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
				InitMapping(m_strReadIndexList);
				break;
			}
		case 1: //保存成功结果
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
				InitMapping(m_strSucAddResultIndexList);
				break;
			}
// 		case 2://保存失败结果
// 			{
// 				SetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 				InitMapping(m_strFailAddResultIndexList);
// 				break;
// 			}
		case 2://成功清理等待发送
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
				InitMapping(m_strSucCleanIndexList);
				break;
			}
// 		case 4:		//失败处理
// 			{
// 				SetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 				InitMapping(m_strFailCleanIndexList);
// 				break;
// 			}
		case 3:	//上行信息
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
				InitMapping(m_strAddMoIndexList);
				break;
			}
		case 4://状态报告
			{
				SetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
				InitMapping(m_strUpdateIndexList);
				break;
			}
	}
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnCbnSelchangeComboOpttype()
{
	switch(m_iOptFlag)
	{	
	case 0: //读取
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strReadSql);
			SaveMapping(m_strReadIndexList);
			break;
		}
	case 1: //保存成功结果
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucAddResultSql);
			SaveMapping(m_strSucAddResultIndexList);
			break;
		}
// 	case 2: //保存失败结果
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailAddResultSql);
// 			SaveMapping(m_strFailAddResultIndexList);
// 			break;
// 		}
	case 2: //成功清理等待发送
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strSucCleanSql);
			SaveMapping(m_strSucCleanIndexList);
			break;
		}
// 	case 4:	//失败处理
// 		{
// 			GetDlgItemText(IDC_EDIT_SQL,m_strFailCleanSql);
// 			SaveMapping(m_strFailCleanIndexList);
// 			break;
// 		}
	case 3:	//上行信息
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strAddMoSql);
			SaveMapping(m_strAddMoIndexList);
			break;
		}
	case 4://状态报告
		{
			GetDlgItemText(IDC_EDIT_SQL,m_strUpdateSql);
			SaveMapping(m_strUpdateIndexList);
			break;
		}
	}
	m_iOptFlag = m_ComboSql.GetCurSel();
	InitKeyMap();
	InitSqlString();	
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void COtherSet::OnBnClickedBtnAddField()
{
	CString strSql = "";
	GetDlgItemText(IDC_EDIT_SQL,strSql);
	int idx1 = 0;
	int idx2 = 0;
	CString strTemp = "";
	int iIndex = 0;
	m_List.DeleteAllItems();
	while(true)
	{
		idx1 = strSql.Find('{');
		idx2 = strSql.Find('}');
		if ( idx1 != -1 && idx2 != -1)
		{
			strTemp = strSql.Mid(idx1+1,idx2-idx1-1);
			strSql = strSql.Mid(idx2 + 1);
			int iCount = m_List.GetItemCount();
			m_List.InsertItem(iCount,strTemp);
		}
		else
		{
			break;
		}
	}
}
