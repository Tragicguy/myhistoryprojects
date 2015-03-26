#pragma once
#include <map>
#include "ComboListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
using   namespace   std;
// COtherSet 对话框

class COtherSet : public CDialog
{
	DECLARE_DYNAMIC(COtherSet)

public:
	COtherSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COtherSet();

// 对话框数据
	enum { IDD = IDD_DLG_OTHERSET };

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

	CString m_strReadSql;			//读取待发信息
	CString m_strReadIndexList;		//待发映射
	CString m_strSucAddResultSql;		//发送后添加发送结果[成功]
	CString m_strSucAddResultIndexList;	//添加发送结果映射[成功]
	CString m_strFailAddResultSql;			//发送后添加发送结果[失败]
	CString m_strFailAddResultIndexList;	//添加发送结果映射[失败]
	CString m_strSucCleanSql;			//发送成功更新待发表
	CString m_strSucCleanIndexList;		//成功更新待发表映射
	CString m_strFailCleanSql;			//发送失败更新待发表
	CString m_strFailCleanIndexList;	//失败更新待发表映射
	CString m_strUpdateSql;				//状态报告更新发送结果表
	CString m_strUpdateIndexList;		//状态报告更新映射
	CString m_strAddMoSql;				//保存上行信息	
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
	afx_msg void OnBnClickedBtnAddField();
};
