// DialAdsl.cpp : 定义应用程序的类行为。
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


// CDialAdslApp

BEGIN_MESSAGE_MAP(CDialAdslApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDialAdslApp 构造

CDialAdslApp::CDialAdslApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDialAdslApp 对象

CDialAdslApp theApp;


// CDialAdslApp 初始化

BOOL CDialAdslApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CDialAdslDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
