// TESTDlg.h : 头文件
//

#pragma once
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#pragma comment(lib,"cxcore.lib")
#pragma comment(lib,"cv.lib")
#pragma comment(lib,"highgui.lib")
#pragma comment(lib,"cvaux.lib")
#pragma comment(lib,"cvcam.lib")
#include "include/AspriseOCR.h"
#pragma comment(lib,"lib\\AspriseOCR.lib")

#include <map>
#include <vector>
using namespace std;


typedef struct _InfoStruct 
{
	CString strTime;	//时间
	CString strPrice;	//价格
	CString strNum;		//次数
	CString strType;	//箭头
}InfoStruct;
// CTESTDlg 对话框
class CTESTDlg : public CDialog
{
// 构造
public:
	CTESTDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void GetLocation(IplImage* Img,int &ptY,CArray<int,int> &iPtXlist);
	void LoadSoucePicAndPreCal(CString strPath);
	
	CString ModifyOcrResult(CString strResult);
	void CaculatePic(CString strPath);
	void AddToMap(CString strKey,InfoStruct &infoValue);
	int StringSplit(CString srcStr,CArray<CString,CString> &strArray,char splitChar);
	map<CString,InfoStruct> m_mapInfoStruct;
	vector<InfoStruct> m_vecInfoList;
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnExport();
	afx_msg void OnBnClickedBtnFind();
	void OperateProc();
	static void ThreadProc(LPVOID lpParam);
	BOOL GetDefaultXlsFileName(CString &sExcelFile);
	BOOL MakeSurePathExists(CString &Path,bool FilenameIncluded);
	int CountDirectory(CString path,map<int,CString> &mapFileList);
	void CaculateFile(CString strFoldername,CString strSaveName,CString strParantFolder);
	int GetAppPath(CString &strAppPath);
	BOOL PreTranslateMessage( MSG* pMsg );
	CString GetExcelDriver();
	CString GetKeyString(CString srcStr,CString strBefore,CString strAfter);
	int GetPower();
	void GetSelectWindowPic(HWND hwnd,CString strPath);
	bool SaveNewPicture(CString filePath,LPBYTE m_newBmpData,long filelength);
	bool m_bExit;
	afx_msg void OnBnClickedButton3();
	void GetPutTypeAndClearNoiseEx(IplImage *pImage,CArray<CString,CString> &arrayTypeList);
	afx_msg void OnBnClickedBtnStop2();
	void SaveData(CString strXlsPath);
	CString m_strAppPath;
};
