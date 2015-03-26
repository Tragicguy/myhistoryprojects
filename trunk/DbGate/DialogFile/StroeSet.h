#pragma once
#include <map>
#include "ComboListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
using   namespace   std;

class CStroeSet : public CDialog
{
	DECLARE_DYNAMIC(CStroeSet)

public:
	CStroeSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStroeSet();

// 对话框数据
	enum { IDD = IDD_DLG_STORESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSave();
	void InitParams();
	BOOL OnInitDialog();	
	afx_msg LRESULT PopulateComboList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMRclickListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT UpdateValue(WPARAM wParam, LPARAM lParam);
	void InitComString( CStringList* pComboList);
	afx_msg void OnNMDblclkListMapping(NMHDR *pNMHDR, LRESULT *pResult);
	CString IntToCString( int iIndex );
	CString CStringToIntCString( CString strKey );
	void InitKeyMap();
	void InitListCtrl();
	void InitSqlCombo();
private:
	CComboListCtrl m_List;
	map<CString, int>  m_keyMap;
	CStringList m_cstringList;
	int m_iOptFlag;

	CString m_strRead;			//读取待发信息
	CString m_strReadIndexList;		//待发映射
	CString m_strAddResult;		//发送后添加发送结果[成功]
	CString m_strAddResultIndexList;	//添加发送结果映射[成功]
	CString m_strUpdate;				//状态报告更新发送结果表
	CString m_strUpdateIndexList;		//状态报告更新映射
	CString m_strAddMo;				//保存上行信息	
	CString m_strAddMoIndexList;		//保存上行信息映射
public:
	CComboBox m_ComboSql;
	afx_msg void OnMenuInsertown();
	afx_msg void OnLstDeleteown();
	afx_msg void OnCbnDropdownComboOpttype();
	void InitSqlString();
	void InitMapping( CString strMap );
	void SaveMapping(CString &strList);
	afx_msg void OnCbnSelchangeComboOpttype();
};
