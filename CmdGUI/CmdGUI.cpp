// CmdGUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "CmdGUIDlg.h"
#include"SplashWindow.h"
#include ".\logreport\HttpClient.h"
#include ".\logreport\MachineID.h"
//#include "updatefiles.h"



// CCmdGUIApp

BEGIN_MESSAGE_MAP(CCmdGUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCmdGUIApp 构造

CCmdGUIApp::CCmdGUIApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCmdGUIApp 对象

CCmdGUIApp theApp;


// CCmdGUIApp 初始化

BOOL CCmdGUIApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	CMachineID user;
	user.Init();
	std::string userID=user.GetUserID();
	CString  uniqueID(userID.c_str()); 

	href.Format("http://logger.redhadoop.com/studio/logger.php?uid=%s",uniqueID);
//安装
	for (int i = 1; i < __argc; i++)   // extern int __argc;           /* count of cmd line args */
	{ 
		LPCTSTR pszParam = __targv[i];   //extern char ** __argv;       /* pointer to table of cmd line args */
		//extern wchar_t ** __wargv;   /* pointer to table of wide cmd line args */
		//	BOOL bFlag = FALSE; 
		//BOOL bLast = ((i + 1) == __argc); 
		//if (pszParam[0] == '-' || pszParam[0] == '/') 
		//{ 
		//	// remove flag specifier 
		//	bFlag = TRUE; 
		//	++pszParam; 
		//} 
		CString param = pszParam;
		CString info;
		if(param.CompareNoCase("install")==0)//它为不区分大小写比较，两个对象完全一致则返回0,小于返回-1
			info.Format("%s&type=active&ver=0.2&aid=%d&cid=0",href,CRH_INSTALL);
		else
		if(param.CompareNoCase("uninstall")==0)
			info.Format("%s&type=active&ver=0.2&aid=%d&cid=0",href,CRH_UNINSTALL);	
		else
			break;
	
		CHttpClient::getinstance()->start();
		CHttpClient::getinstance()->startdoget(info);
		CHttpClient::getinstance()->stop();
		exit(0);
	} 

	::CreateMutex(NULL,TRUE,"RedhadoopStudio");
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{    
		AfxMessageBox("Redhadoop Studio has been started!");
		exit(0);    
	}
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&cid=0",href,CRH_START);
	CHttpClient::getinstance()->start();
	CHttpClient::getinstance()->startdoget(info);

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
    //InitializeSkin(_T("Minimized.ssk"));//Minimized.ssk SoftCrystal
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    CSplashWindow *m_pSplashWindow = new CSplashWindow;
	m_pSplashWindow->CreateSplash();
	m_pSplashWindow->CenterWindow();
	m_pSplashWindow->ShowWindow(SW_SHOW);
	//m_pSplashWindow->ShowText("正在启动……");
	m_pSplashWindow->UpdateWindow();
	Sleep(3000); //Delay 3 Second
	m_pSplashWindow->DestroyWindow(); //Destroy Splash Window
	delete m_pSplashWindow;

	CCmdGUIDlg dlg;
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
	//LoadSkin(_T("Minimized.ssk"));
	//preproc();
	
	//初始化GID+

	

	return FALSE;
}



int CCmdGUIApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	CHttpClient::getinstance()->stop();
	CHttpClient::destory();
	return CWinApp::ExitInstance();
	
	
}
