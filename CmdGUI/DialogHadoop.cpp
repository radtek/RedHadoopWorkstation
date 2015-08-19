// DialogHadoop.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "DialogHadoop.h"
#include ".\servicectrl\WindowsService.h"
#include ".\logreport\HttpClient.h"
//#include "..\Client\Client\Serv.h"
//#include "..\Server\Server\Serv.h"
//#include <thrift/transport/TSocket.h>  
//#include <thrift/transport/TBufferTransports.h>  
//#include <thrift/protocol/TBinaryProtocol.h>   
//
//using namespace apache::thrift;  
//using namespace apache::thrift::protocol;  
//using namespace apache::thrift::transport;  
//using boost::shared_ptr; 

// CDialogHadoop 对话框

IMPLEMENT_DYNAMIC(CDialogHadoop, CDialog)

CDialogHadoop::CDialogHadoop(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHadoop::IDD, pParent)

{
    //m_pStartY.EnableWindow(0);
	//m_pStopY.EnableWindow(0);
	IsStartH = false;
	IsStopH = false;
	IsStartY = false;
	IsStopY = false;

}
//启动一个程序，直到它运行结束
BOOL HadoopRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
{    //  DWORD Sk=GetTickCount();

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow =SW_SHOWMINIMIZED; // SW_HIDE
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);
	//WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds);
	// DWORD Ek=GetTickCount();
	//printf("################################k:%dms\n",Ek-Sk);
	if (wait) {  
		HANDLE hProcess = ShExecInfo.hProcess;  
		if (hProcess != 0) {  
			WaitForSingleObject(hProcess, INFINITE);  
			CloseHandle(hProcess);  
		}  
	}  
	return suc;
}

CDialogHadoop::~CDialogHadoop()
{

}


void CDialogHadoop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BStartH, m_bStartH);
	DDX_Control(pDX, IDC_BStopH, m_bStopH);
	DDX_Control(pDX, IDC_BStartY, m_pStartY);
	DDX_Control(pDX, IDC_BStopY, m_pStopY);
}

 
BEGIN_MESSAGE_MAP(CDialogHadoop, CDialog)
	ON_BN_CLICKED(IDC_BStartH, &CDialogHadoop::OnBnClickedBstarth)
	ON_BN_CLICKED(IDC_BStopH, &CDialogHadoop::OnBnClickedBstoph)
	ON_BN_CLICKED(IDC_BStartY, &CDialogHadoop::OnBnClickedBstarty)
	ON_BN_CLICKED(IDC_BStopY, &CDialogHadoop::OnBnClickedBstopy)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL IsAdministrator(HANDLE p) {//判断是否管理员模式  
  
    BOOL bIsElevated = FALSE;  
    HANDLE hToken = NULL;  
    UINT16 uWinVer = LOWORD(GetVersion());  
    uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer));  
  
    if (uWinVer < 0x0600)//不是VISTA、Windows7  
        return(FALSE);  
  
    if (OpenProcessToken(p,TOKEN_QUERY,&hToken)) {  
  
        struct {  
            DWORD TokenIsElevated;  
        } /*TOKEN_ELEVATION*/te;  
        DWORD dwReturnLength = 0;  
  
        if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength)) {  
            if (dwReturnLength == sizeof(te))  
                bIsElevated = te.TokenIsElevated;  
        }  
        CloseHandle( hToken );  
    }  
    return bIsElevated;  
}  


// CDialogHadoop 消息处理程序
void CDialogHadoop::OnBnClickedBstarth()
{
	//SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};
 //sei.lpVerb = TEXT("runas");
 //sei.lpFile = TEXT("cmd.exe");
 //sei.nShow = SW_NORMAL;
 //if (!ShellExecuteEx(&sei))
 //{
 // DWORD dwStatus = GetLastError();
 // if (dwStatus == ERROR_CANCELLED)
 // {
 //  printf("没有以管理员权限运行");
 // }
 // else
 //  if(dwStatus == ERROR_FILE_NOT_FOUND)
 // {
 //  printf("没有找到该文件");
 // }
 //}
 #ifndef START_IN_SERVICE
	IsStartH = HadoopRunExec(_T("cmd.exe"),_T("/c start-dfs"), INFINITE);//

	if(IsStartH) 
	{	
		m_bStartH.EnableWindow(0);
		m_bStopH.EnableWindow(1);
		m_pStartY.EnableWindow(1);
		m_pStopY.EnableWindow(0);	
	}
	else
		AfxMessageBox("hdfs start failed! ",MB_ICONERROR,NULL);
#else


	IsStartH = CWindowsService::StartSevice("Hadoop-NameNode");
	if(IsStartH) 
	{
		IsStartH = CWindowsService::StartSevice("Hadoop-DataNode");
	    
		if(IsStartH)
		{
			m_bStartH.EnableWindow(0);
			m_bStopH.EnableWindow(1);
			m_pStartY.EnableWindow(1);
			m_pStopY.EnableWindow(0);
			AfxMessageBox("HDFS start sucessfully! ");
		}
		else
		{
			CWindowsService::StopSevice("Hadoop-NameNode");
			AfxMessageBox("HDFS start failed! ");
			return;
		}

		return;
#endif
	this->PostMessageA(WM_TIMER);
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRHAid::CRH_USE,theApp.CRHServiceType::CRH_HDFS);
	CHttpClient::getinstance()->startdoget(info);
}

void CDialogHadoop::OnBnClickedBstoph()
{
	/*if(FALSE == IsAdministrator(GetCurrentProcess))
	{
		AfxMessageBox("CURRENT PROCESS NOT RUN AS ADMINISTRATOR");
	}*/
	HWND handle1 = ::FindWindow(NULL,_T("Apache Hadoop Distribution - yarn   nodemanager"));
	DWORD p=NULL;
	::GetWindowThreadProcessId(handle1,&p);
	HANDLE hSpy = ::OpenProcess (PROCESS_DUP_HANDLE, FALSE, p);
	/*if(FALSE == IsAdministrator(hSpy))
	{
		AfxMessageBox("NODEMANAGER PROCESS NOT RUN AS ADMINISTRATOR");
	}*/
	::CloseHandle(hSpy);
#ifndef START_IN_SERVICE
	if(IsStartH)
	{
		IsStopH = HadoopRunExec(_T("cmd"), _T("/c stop-dfs"), INFINITE);
		if (IsStopH)
		{
          m_bStopH.EnableWindow(0);
		  m_bStartH.EnableWindow(1);
		  m_pStartY.EnableWindow(0);
		  IsStartH = false;

		  if(!IsStartY)
		  m_pStopY.EnableWindow(0);
		}
		else
		AfxMessageBox("hdfs stop failed! ",MB_ICONERROR,NULL);
	}
	
#else
	if(IsStartH)
	{
		CWindowsService::StopSevice("Hadoop-DataNode");
		CWindowsService::StopSevice("Hadoop-NameNode");
		HadoopRunExec(_T("cmd"), _T("/c stop-dfs"), INFINITE);
		m_bStopH.EnableWindow(0);
		m_bStartH.EnableWindow(1);
		m_pStartY.EnableWindow(0);
		IsStartH = FALSE;

		if(!IsStartY)
		m_pStopY.EnableWindow(0);
		AfxMessageBox("HDFS stop sucessfully! ");
	}
#endif
	this->PostMessageA(WM_TIMER);
}

void CDialogHadoop::OnBnClickedBstarty()
{
#ifndef START_IN_SERVICE
	//if(IsStartH)
	//{
		IsStartY = HadoopRunExec(_T("cmd.exe"),_T("/c start-yarn"), INFINITE);
		if(IsStartY)
		{
		  m_pStartY.EnableWindow(0);
		  m_pStopY.EnableWindow(1);
		}
		else
		{
          AfxMessageBox("yarn start failed! ",MB_ICONERROR,NULL);
		}
		
	//}
		
#else
	IsStartY = CWindowsService::StartSevice("YARN-ResourceManager");
	if(IsStartY)
	{
		IsStartY = CWindowsService::StartSevice("YARN-NodeManager");
		if(IsStartY)
		{
			m_pStartY.EnableWindow(0);
			m_pStopY.EnableWindow(1);
			AfxMessageBox("YARN start sucessfully! ");
		}else
		{
			CWindowsService::StopSevice("YARN-ResourceManager");
			AfxMessageBox("YARN start failed! ");
			return;
		}
		return;
	}
#endif
	this->PostMessageA(WM_TIMER);
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRHAid::CRH_USE,theApp.CRHServiceType::CRH_YARN);
	CHttpClient::getinstance()->startdoget(info);	

}

void CDialogHadoop::OnBnClickedBstopy()
{
#ifndef START_IN_SERVICE
	if(IsStartY)
	{
		IsStopY = HadoopRunExec(_T("cmd"), _T("/c stop-yarn"), INFINITE);
		if (IsStopY)
		{
			m_pStopY.EnableWindow(0);
			IsStartY = false;
			if(IsStartH)
				m_pStartY.EnableWindow(1);
		}
		else
          AfxMessageBox("yarn stop failed! ",MB_ICONERROR,NULL);

	}
#else
	if(IsStartY)
	{
		CWindowsService::StopSevice("YARN-ResourceManager");
		CWindowsService::StopSevice("YARN-NodeManager");
		HadoopRunExec(_T("cmd"), _T("/c stop-yarn"), INFINITE);
		m_pStopY.EnableWindow(0);
		IsStartY = false;
		if(IsStartH)
		m_pStartY.EnableWindow(1);
		AfxMessageBox("YARN stop sucessfully! ");
	}
#endif
	this->PostMessageA(WM_TIMER);
}


BOOL CDialogHadoop::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->SetTimer(1,2000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogHadoop::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bRunningh= FALSE;
	BOOL bRunningy= FALSE;
#ifndef START_IN_SERVICE
	HWND handle = ::FindWindow(NULL,_T("Apache Hadoop Distribution - hadoop   namenode"));
	HWND handle1 = ::FindWindow(NULL,_T("Apache Hadoop Distribution - hadoop   datanode"));
	if(NULL!=handle|| NULL!=handle1)
		bRunningh= TRUE;
	handle = ::FindWindow(NULL,_T("Apache Hadoop Distribution - yarn   resourcemanager"));
	handle1 = ::FindWindow(NULL,_T("Apache Hadoop Distribution - yarn   nodemanager"));
	if(NULL!=handle|| NULL!=handle1)
		bRunningy= TRUE;
#else

	if(CWindowsService::CheckServiceStatus("Hadoop-NameNode")
		==CWindowsService::SEV_STATUS::SEV_RUNING)
	{
		bRunningh= TRUE;
	}
	if(CWindowsService::CheckServiceStatus("YARN-ResourceManager")
		==CWindowsService::SEV_STATUS::SEV_RUNING)
	{
		bRunningy= TRUE;
	}

#endif
	m_bStopH.EnableWindow(bRunningh);
	m_bStartH.EnableWindow(!bRunningh);

	m_pStartY.EnableWindow(!bRunningy);
	m_pStopY.EnableWindow(bRunningy);
	IsStartY= bRunningy;
	IsStartH= bRunningh;

	CDialog::OnTimer(nIDEvent);
}


//BOOL CDialogHadoop::DestroyWindow()
//{
//
//
//	return CDialog::DestroyWindow();
//}


//void CDialogHadoop::OnFinalRelease()
//{
//
//	CDialog::OnFinalRelease();
//}


void CDialogHadoop::DestroyWin()
{
#ifndef START_IN_SERVICE
	if(IsStartY)
	{
		HadoopRunExec(_T("cmd"), _T("/c stop-yarn"), INFINITE);
	}
	if(IsStartH)
		HadoopRunExec(_T("cmd"), _T("/c stop-dfs"), INFINITE);
#else
#endif
}
