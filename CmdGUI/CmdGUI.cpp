// CmdGUI.cpp : ����Ӧ�ó��������Ϊ��
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


// CCmdGUIApp ����

CCmdGUIApp::CCmdGUIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCmdGUIApp ����

CCmdGUIApp theApp;


// CCmdGUIApp ��ʼ��

BOOL CCmdGUIApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	CMachineID user;
	user.Init();
	std::string userID=user.GetUserID();
	CString  uniqueID(userID.c_str()); 

	href.Format("http://logger.redhadoop.com/studio/logger.php?uid=%s",uniqueID);
//��װ
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
		if(param.CompareNoCase("install")==0)//��Ϊ�����ִ�Сд�Ƚϣ�����������ȫһ���򷵻�0,С�ڷ���-1
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
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    CSplashWindow *m_pSplashWindow = new CSplashWindow;
	m_pSplashWindow->CreateSplash();
	m_pSplashWindow->CenterWindow();
	m_pSplashWindow->ShowWindow(SW_SHOW);
	//m_pSplashWindow->ShowText("������������");
	m_pSplashWindow->UpdateWindow();
	Sleep(3000); //Delay 3 Second
	m_pSplashWindow->DestroyWindow(); //Destroy Splash Window
	delete m_pSplashWindow;

	CCmdGUIDlg dlg;
	m_pMainWnd = &dlg;
	
	INT_PTR nResponse = dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	//LoadSkin(_T("Minimized.ssk"));
	//preproc();
	
	//��ʼ��GID+

	

	return FALSE;
}



int CCmdGUIApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	CHttpClient::getinstance()->stop();
	CHttpClient::destory();
	return CWinApp::ExitInstance();
	
	
}
