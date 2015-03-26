// DialAdsl.h : PROJECT_NAME 应用程序的主头文件
//
//本程序由唐添麒编写，现将代码发布供学习交流之用
//本程序的发布遵守GPL协议，您可在此协议的规定下使用本程序
//如果您在使用本程序的途中有什么疑问或建议请访问http://digua254909662.blog.163.com/blog/static/74805072200922323951508/edit/
//或者联系作者：qq<254909662> 并说明来意，本人非常乐意与您交流

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CDialAdslApp:
// 有关此类的实现，请参阅 DialAdsl.cpp
//

class CDialAdslApp : public CWinApp
{
public:
	CDialAdslApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CDialAdslApp theApp;