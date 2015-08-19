// CmdGUI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCmdGUIApp:
// 有关此类的实现，请参阅 CmdGUI.cpp
//

class CCmdGUIApp : public CWinApp
{
public:
	CCmdGUIApp();
	enum CRHAid
	{
		CRH_INSTALL=0,
		CRH_START,
		CRH_USE,
		CRH_UPDATE,
		CRH_UNINSTALL
	};
	enum CRHServiceType
	{
		CRH_HDFS=0,
		CRH_YARN,
		CRH_HIVE,
		CRH_HBASE,
		CRH_SPARK
	};

// 重写
	public:
	virtual BOOL InitInstance();
	
	CString href;
	
// 实现

	DECLARE_MESSAGE_MAP()
	
	virtual int ExitInstance();
	
};


extern CCmdGUIApp theApp;
