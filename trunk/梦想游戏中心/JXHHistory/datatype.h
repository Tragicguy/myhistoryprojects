#pragma once
extern CListCtrl g_ShowResult;		//历史显示列表
extern CListCtrlColore g_ShowMsg;			//消息显示列表
extern BOOL g_bExitFlag;			//线程运行控制
extern CCriticalSection g_ShowLock;		//消息显示锁
extern CString g_dbServer;//数据库
extern CString g_dbPwd;				//数据库密码
extern CString g_strCurrRecord;		//当前输赢结果
extern CString g_strCurrRecordList;	//当前历史记录链
extern int g_iLastRecordID;			//最新的记录ID
extern CString  g_strAppPath;
extern CString  g_srcPicPath;
extern CString  g_dstPicPath11;
extern CString  g_dstPicPath12;
extern CString  g_dstPicPath13;
extern CString g_cmpPicPath1;
extern CString g_srcPicPath2;
extern CString g_cmpPicPath2;
extern void ShowMessageEx(CString strMsg);
extern CCriticalSection g_QueryFlagLock;		//匹配控制锁
extern int g_iQueryFlag;