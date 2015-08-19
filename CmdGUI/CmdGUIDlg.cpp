// CmdGUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "CmdGUIDlg.h"
#include <shlobj.h> 
#include <shellapi.h>
#include "updatefiles.h"
#include "MsgUpdateDlg.h"
#include ".\servicectrl\WindowsService.h"
#include ".\logreport\HttpClient.h"
#include "DlgVideoLoad.h"
#include <fstream>
#include<iostream>



//#include ".\update\AutoUpdateClientDlg.h"
//#include "..\Server\Server\ServerAPI.h"
//#include "..\Server\Server\Serv.h"
//#include "..\Client\Client\ClientAPI.h"
//#include "..\Client\Client\Serv.h"
//#include <thrift/protocol/TBinaryProtocol.h>
//#include <thrift/server/TSimpleServer.h>
//#include <thrift/transport/TServerSocket.h>
//#include <thrift/transport/TBufferTransports.h>
//using namespace ::apache::thrift;
//using namespace ::apache::thrift::protocol;
//using namespace ::apache::thrift::transport;
//using namespace ::apache::thrift::server;
//
//using boost::shared_ptr;
//using namespace std;
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#pragma comment(lib,"..\\Lib\\ClientUD.lib")
//#pragma comment(lib,"..\\Lib\\ServerUD.lib")
//#else
//#pragma comment(lib,"..\\Lib\\ClientU.lib")
//#pragma comment(lib,"..\\Lib\\ServerU.lib")
//#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:

//	afx_msg void OnSize(UINT nType, int cx, int cy);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

//	ON_WM_SIZE()
END_MESSAGE_MAP()


// CCmdGUIDlg 对话框




CCmdGUIDlg::CCmdGUIDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCmdGUIDlg::IDD, pParent)
, m_nMaxDownThread(5)
, m_nCurThreadCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON);
	
}
CCmdGUIDlg::~CCmdGUIDlg()
{

}

void CCmdGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_EDITPATH, m_editPath);
}

BEGIN_MESSAGE_MAP(CCmdGUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP


	ON_BN_CLICKED(IDC_BCmd, &CCmdGUIDlg::OnBnClickedBcmd)
	ON_BN_CLICKED(IDC_BBrowseHDFS, &CCmdGUIDlg::OnBnClickedBbrowsehdfs)
	ON_BN_CLICKED(IDC_BBrowseYarn, &CCmdGUIDlg::OnBnClickedBbrowseyarn)
	ON_COMMAND(ID_HadoopEnvCmd, &CCmdGUIDlg::OnHadoopenvcmd)
	ON_COMMAND(ID_CoreSite, &CCmdGUIDlg::OnCoresite)
	ON_COMMAND(ID_HdfsSite, &CCmdGUIDlg::OnHdfssite)
	ON_COMMAND(ID_YarnSite, &CCmdGUIDlg::OnYarnsite)
	ON_COMMAND(ID_MapredSite, &CCmdGUIDlg::OnMapredsite)
	ON_COMMAND(ID_HdfsNameNodeFormat, &CCmdGUIDlg::OnHdfsnamenodeformat)
	ON_COMMAND(ID_HiveSite, &CCmdGUIDlg::OnHivesite)
	////ON_COMMAND(ID_NNSERVICEINSTALL, &CCmdGUIDlg::OnNnserviceinstall)
		
	ON_COMMAND(ID_Hosts, &CCmdGUIDlg::OnHosts)
	ON_COMMAND(ID_Path, &CCmdGUIDlg::OnPath)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CCmdGUIDlg::OnTcnSelchangeTab)
	ON_COMMAND(ID_Java_Home, &CCmdGUIDlg::OnJavaHome)
	ON_COMMAND(ID_Hadoop_Home, &CCmdGUIDlg::OnHadoopHome)
	ON_COMMAND(ID_Hive_Home, &CCmdGUIDlg::OnHiveHome)
	ON_BN_CLICKED(IDC_BClear, &CCmdGUIDlg::OnBnClickedBclear)
	//ON_UPDATE_COMMAND_UI(ID_NNSERVICEINSTALL, &CCmdGUIDlg::OnUpdateNnserviceinstall)
	ON_COMMAND(ID_Cmd, &CCmdGUIDlg::OnCmd)
	ON_COMMAND(ID_BrowseHDFS, &CCmdGUIDlg::OnBrowsehdfs)
	ON_COMMAND(ID_BrowseYarn, &CCmdGUIDlg::OnBrowseyarn)
	ON_COMMAND(ID_About, &CCmdGUIDlg::OnAbout)
	ON_COMMAND(ID_Exit, &CCmdGUIDlg::OnExit)
	ON_COMMAND(ID_Update, &CCmdGUIDlg::OnUpdate)
	ON_MESSAGE(WM_DOWNLOADFILEEND, OnDownLoadFileEnd) 
	ON_WM_TIMER()

	ON_COMMAND(ID_HELP, &CCmdGUIDlg::OnHelp)

	ON_COMMAND(ID_EXP_WC, &CCmdGUIDlg::OnExpWc)
	//ON_COMMAND(ID_RMSERVICEINSTALL, &CCmdGUIDlg::OnRmserviceinstall)
	//ON_UPDATE_COMMAND_UI(ID_RMSERVICEINSTALL, &CCmdGUIDlg::OnUpdateRmserviceinstall)
	ON_COMMAND(ID_EXP_PI, &CCmdGUIDlg::OnExpPi)

	ON_MESSAGE(WM_NOTIFYICON,OnNotifyIcon)
	ON_COMMAND(ID_SHOW, &CCmdGUIDlg::OnShow)
	ON_COMMAND(ID_HIDE, &CCmdGUIDlg::OnHide)
	ON_COMMAND(ID_HDFSEXITSAFEMODE, &CCmdGUIDlg::OnHdfsexitsafemode)
	ON_BN_CLICKED(IDC_BEclipse, &CCmdGUIDlg::OnBnClickedBeclipse)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SEASBASE_INQUIRE, &CCmdGUIDlg::OnSeasbaseInquire)
	ON_COMMAND(ID_SEASBASE_LOADVIDEO, &CCmdGUIDlg::OnSeasbaseLoadvideo)
	ON_COMMAND(ID_SEASBASE_HDFSEXPLORER, &CCmdGUIDlg::OnSeasbaseHdfsexplorer)
	ON_COMMAND(ID_SPARKMASTER, &CCmdGUIDlg::OnSparkmaster)
	ON_COMMAND(ID_SPARKSLAVES, &CCmdGUIDlg::OnSparkslaves)
	ON_BN_CLICKED(IDC_BBrowseSpark, &CCmdGUIDlg::OnBnClickedBbrowsespark)
	ON_COMMAND(ID_EXP_SPI, &CCmdGUIDlg::OnExpSpi)
	ON_BN_CLICKED(ID_VEHICLE, &CCmdGUIDlg::OnBnClickedVehicle)
END_MESSAGE_MAP()

void CCmdGUIDlg::checkUpdate()
{
	TCHAR szTempPath[MAX_PATH];TCHAR  szTempName[MAX_PATH];
	::GetTempPath(
		MAX_PATH,
		szTempPath);
	
	::GetTempFileNameA(szTempPath, __TEXT("Xml"), 16, szTempName);
	m_RemoteFiles.ClearAllList();

	CInetFile *pInetFileNew = new CInetFile();
	if (pInetFileNew) {
		pInetFileNew->m_bProgressMode = true;
		pInetFileNew->m_dlFileType = FILELIST_XML;
		pInetFileNew->m_hWnd = m_hWnd;
		pInetFileNew->m_sURL = "http://update.redhadoop.com/studio/0.1/RedHadoopStudio.xml";
		pInetFileNew->m_sServerName ="红象云腾 www.redhadoop.com";
		pInetFileNew->m_sPath = szTempName;
		pInetFileNew->m_dwTotalLen = 0;
		pInetFileNew->m_sFileName = _T("");
		m_InetFiles.Push(pInetFileNew);
	}
}
LRESULT CCmdGUIDlg::OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam) 
{
	// wParam 0 成功, 2, SESSION 异常 3, 主动停止
	CKevinAutoLock  lock(&m_lock);
	CString sLog; 
	int nRetCode = LOWORD(wParam);
	DOWNLOAD_FILE_TYPE llFileType = (DOWNLOAD_FILE_TYPE)HIWORD(wParam);
	CInetFile *pInetFile = (CInetFile *)lParam;
	ASSERT(pInetFile);

	switch(llFileType)
	{
	case FILELIST_XML:		
		{
			if (0 == nRetCode) {
				if (m_RemoteFiles.ReadFromXMLFile((LPTSTR)(LPCTSTR)pInetFile->m_sPath)) {
					m_dwCount = m_RemoteFiles.CheckNeedUpdate();
					KillTimer(PUBLIC_TIMER);
					if (0 != m_dwCount) //0 = m_dwCount {AfxMessageBox("你当前的版本是最新的",MB_OK,NULL);}
					{
						//AfxMessageBox("检测到有新版本，是否立即升级？",MB_YESNO,NULL);
						 CMsgUpdateDlg msg=new CMsgUpdateDlg();
						 msg.DoModal();
					}
				}
			}
		}
		break;
	}
	m_InetFiles.DeleteItem(pInetFile);
	m_nCurThreadCount --;
	return 0; 
}
void CCmdGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case PUBLIC_TIMER:
		{
			CKevinAutoLock  lock(&m_lock);
			if (m_nCurThreadCount < m_nMaxDownThread) {
				CInetFile* pInetFile = m_InetFiles.Front();
				if (pInetFile && 0 == pInetFile->m_nDownloadTime) {	// 只下载已下载次数为  0 的列表
					if (pInetFile->GetInetFile())
						m_nCurThreadCount ++;
				}
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
//HANDLE hMutex;
//UINT thread(LPVOID v)
//{
//	while (true) 
//	{
//		WaitForSingleObject(hMutex,INFINITE);
//		CCmdGUIDlg cmd;
//		cmd.preproc();
//		ReleaseMutex(hMutex);
//	}
//	AfxEndThread(0);
//	return 0;
//}
//class ServHandler : virtual public ServIf {
//public:
//	ServHandler() {
//		// Your initialization goes here
//	}
//
//	void ping() {
//		// Your implementation goes here
//		printf("ping()\n");
//	}
//
//	void run(const std::string& command) {
//		// Your implementation goes here
//		printf("command:%s\n",command.c_str());
//		//command ="/c start-dfs";
//		SHELLEXECUTEINFO ShExecInfo = {0};
//		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
//		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
//		ShExecInfo.hwnd = NULL;
//		ShExecInfo.lpVerb = NULL;
//		ShExecInfo.lpFile = "cmd";
//		ShExecInfo.lpParameters = command.c_str();
//		ShExecInfo.lpDirectory = NULL;
//		ShExecInfo.nShow =SW_SHOWMINIMIZED; // SW_HIDE
//		ShExecInfo.hInstApp = NULL;
//		BOOL suc = ShellExecuteEx(&ShExecInfo);
//
//	}
//
//};
//void CCmdGUIDlg::preproc(){
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int err;
//	wVersionRequested =MAKEWORD( 1, 1 );
//	err = WSAStartup( wVersionRequested, &wsaData );
//	if ( err != 0 ) {
//		/* Tell the user that we couldn't find a useable */
//		/* winsock.dll. */
//		//  return -1;
//	}
//	int port = 9090;
//	shared_ptr<ServHandler> handler(new ServHandler());
//	shared_ptr<TProcessor> processor(new ServProcessor(handler));
//	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
//	shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
//	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
//
//	TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
//	server.serve();
//
//}
// CCmdGUIDlg 消息处理程序

BOOL CCmdGUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
  
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	nItem=0;
	
	//添加选项卡
	m_tab.InsertItem(nItem++,_T("Hadoop"));//是sheet页的名字；
	m_tab.InsertItem(nItem++,_T("Hive"));
	m_tab.InsertItem(nItem++,_T("Spark"));


	//关联对话框,并且将IDC_TAB控件设为父窗口
	m_paraHadoop.Create(IDD_DIALOGHADOOP,GetDlgItem(IDC_TAB));
	m_paraHive.Create(IDD_DIALOGHIVE,GetDlgItem(IDC_TAB));
	m_paraSpark.Create(IDD_DIALOGSPARK,GetDlgItem(IDC_TAB));
	//初始化DialogHadoop子窗口上的按钮状态
	m_paraHadoop.GetDlgItem(IDC_BStopH)->EnableWindow(0);
	//m_paraHadoop.GetDlgItem(IDC_BStartY)->EnableWindow(0);
	m_paraHadoop.GetDlgItem(IDC_BStopY)->EnableWindow(0);
	m_paraHadoop.GetDlgItem(IDC_BStopY)->EnableWindow(0);
	//获得IDC_tabMain客户区大小
	CRect rs;
	m_tab.GetClientRect(&rs);

	//调整子对话框在父窗口中的位置，可以改动数值，使子窗体的大小合适；
	rs.top+=20;
	rs.bottom-=3;
	rs.left+=2;
	rs.right-=2;

	//设置子对话框尺寸并移动到指定位置
	m_paraHadoop.MoveWindow(&rs);
	m_paraHive.MoveWindow(&rs);
	m_paraSpark.MoveWindow(&rs);
	

	//分别设置隐藏和显示
	m_paraHadoop.ShowWindow(true);
	m_paraHive.ShowWindow(false);
	m_paraSpark.ShowWindow(false);
	
	//设置默认的选项卡
	m_tab.SetCurSel(0);
	
	//AfxBeginThread(thread,0);  
	CString des="";
	char tmp[4096];
	::GetCurrentDirectory(256,des.GetBuffer(256));
	des.ReleaseBuffer();
	des+="\\redhadoop.ini";

	GetPrivateProfileSectionNames(tmp,sizeof(tmp)-1,des);//读取一INI文件中所有的块名。
	int ret = GetPrivateProfileString("java","JAVA_HOME","C:\\Redhadoop\\RedhadoopWorkstation",tmp,sizeof(tmp)-1,des);
	CString str = "JAVA_HOME=";
	str+=tmp;
	//getenv("JAVA_HOME");
	_putenv(str.GetBuffer(512));
	str.ReleaseBuffer();

	GetPrivateProfileString("java","CLASSPATH","",tmp,sizeof(tmp)-1,des);
	str= "CLASSPATH=";
	str+=tmp;
	_putenv(str.GetBuffer(512));
	str.ReleaseBuffer();

	CString strh;
	GetPrivateProfileString("hadoop","HADOOP_HOME","",tmp,sizeof(tmp)-1,des);
	str= "HADOOP_HOME=";
	str+=tmp;
	strh+=tmp;
	strh+="\\bin;";
	strh+=tmp;
	strh+="\\sbin;";
	m_strConfPath = tmp;
	m_strConfPath+="\\etc\\hadoop";
	_putenv(str.GetBuffer(512));
	str.ReleaseBuffer();

	GetPrivateProfileString("hadoop","HIVE_HOME","",tmp,sizeof(tmp)-1,des);
	str= "HIVE_HOME=";
	str+=tmp;
	strh+=tmp;
	strh+="\\bin;";
	_putenv(str.GetBuffer(512));
	str.ReleaseBuffer();

	GetPrivateProfileString("hadoop","SPARK_HOME","",tmp,sizeof(tmp)-1,des);
	str= "SPARK_HOME=";
	str+=tmp;
	strh+=tmp;
	strh+="\\bin;";
	strh+=tmp;
	strh+="\\sbin;";

	_putenv(str.GetBuffer(512));
	str.ReleaseBuffer();

	GetPrivateProfileString("java","PATH","",tmp,sizeof(tmp)-1,des);
	str= "PATH=";
	char* pathvar = getenv("PATH");
	str+=tmp;
	str+=strh;
	str+=pathvar;
	_putenv(str.GetBuffer(1024));
	str.ReleaseBuffer();

    SetTimer(PUBLIC_TIMER, 2000, NULL);
    checkUpdate();
	nld.cbSize=sizeof(NOTIFYICONDATA);
	nld.hWnd=m_hWnd;    //当前窗口将收到托盘图标发出的消息
	nld.uID=IDI_ICON;
	nld.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;//nld 的成员hIcon,uCallbackMessage,szTip有效
	nld.uCallbackMessage=WM_NOTIFYICON;//用鼠标对托盘中的图标的操作发出消息WM_NOTIFYICON
	nld.hIcon=m_hIcon;    //托盘中的图标为m_hIcon，注意，这里的m_hIcon是事先加载了图标的
	strcpy(nld.szTip, "RedHadoopStudio");  //鼠标放在托盘图标上时显示的文字
	Shell_NotifyIcon(NIM_ADD, &nld); //添加图标到状态栏

	dlh = NULL;
	if (!(dlh=LoadLibrary("VehicleTypeRecognition_D.dll")))      
	{
		printf("LoadLibrary() failed: %d\n", GetLastError()); 
	}
	// ITS_ThreadInit(); //进程启动时调用
	ThreadInit threadInit;
	threadInit = (ThreadInit)GetProcAddress(dlh, "ITS_ThreadInit");//ITS_ThreadInit(); 进程启动时调用
	(*threadInit)();


	int iInitFlag;
	char* modePath = "..\\model";//..\\model	..\\bin\\model
	VehicleRecInit vehicleRecInit;
	vehicleRecInit = (VehicleRecInit)GetProcAddress(dlh, "ITS_VehicleRecInit");
	pInstance  = (*vehicleRecInit)(modePath, iInitFlag);//初始化操作
	//pInstance = ITS_VehicleRecInit(modePath, iInitFlag);//初始化操作
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
//-----------获取树型控件中项的数目------------------------//
//int GetItemCount(CItem * pItem)
//{
//	int		itemCount	= 0;
//	for(int i=1; i<maxItemCount; i++ )
//	{
//		if (pItem[i].ItemFlag != 0)
//			itemCount++;
//	}
//	return itemCount;
//}
void CCmdGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	//拦截最小化事件
	if(nID==SC_MINIMIZE) 
	{    
		//ToTray();
		//用自己的消息响应最小化事件   
		ShowWindow(SW_HIDE);
	} 
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCmdGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CCmdGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL RunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
{    //  DWORD Sk=GetTickCount();

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = NULL;//_T("D:\\Redhadoop\\CRH3\\hadoop\\bin");
	ShExecInfo.nShow =SW_NORMAL; //SW_SHOWMINIMIZED; // SW_HIDE
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);

	if (wait) {  
        HANDLE hProcess = ShExecInfo.hProcess;  
        if (hProcess != 0) {  
            WaitForSingleObject(hProcess, INFINITE);  
           CloseHandle(hProcess);  
        }  
    }  

	return suc;
}

void CCmdGUIDlg::OnBnClickedBcmd()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","cmd.exe",NULL,NULL,SW_SHOWNORMAL);
}


void CCmdGUIDlg::OnBnClickedBbrowsehdfs()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://localhost:50070",NULL,NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnBnClickedBbrowseyarn()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://localhost:8088",NULL,NULL,SW_SHOWNORMAL);
}


void CCmdGUIDlg::OnJavaHome()
{
	// TODO: 在此添加命令处理程序代码
	char buf[128];
	char* pathvar;
	pathvar = getenv("JAVA_HOME");
   if(pathvar != NULL)
   {
	sprintf(buf,"JAVA_HOME=%s",pathvar);
	 m_editPath.SetWindowTextA(buf);
   }
   else
   {
    sprintf(buf,"JAVA_HOME=");
	 m_editPath.SetWindowTextA(buf);
   }
}
void CCmdGUIDlg::OnHadoopHome()
{
	// TODO: 在此添加命令处理程序代码
	char buf[128];
	char* pathvar;
    pathvar = getenv("HADOOP_HOME");
	if(pathvar != NULL)
   {
	sprintf(buf,"HADOOP_HOME=%s",pathvar);
	 m_editPath.SetWindowTextA(buf);
   }
   else
   {
    sprintf(buf,"HADOOP_HOME=");
	 m_editPath.SetWindowTextA(buf);
   }
}
void CCmdGUIDlg::OnHiveHome()
{
	// TODO: 在此添加命令处理程序代码
	char buf[128];
	char* pathvar;
    pathvar = getenv("HIVE_HOME");
	if(pathvar != NULL)
   {
	sprintf(buf,"HIVE_HOME=%s",pathvar);
	 m_editPath.SetWindowTextA(buf);
   }
   else
   {
    sprintf(buf,"HIVE_HOME=");
	 m_editPath.SetWindowTextA(buf);
   }

}

void CCmdGUIDlg::OnHosts()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open","notepad.exe","C://Windows//System32//drivers//etc//hosts",NULL,SW_SHOWNORMAL);
}
 
void CCmdGUIDlg::OnPath()
{
	// TODO: 在此添加命令处理程序代码
	char buf[12800];
	char *pathvar;
    pathvar = getenv("path");
	if( pathvar != NULL )
	{
		sprintf(buf,"path=%s",pathvar);
	    m_editPath.SetWindowTextA(buf);
	}
	
}
void CCmdGUIDlg::OnHadoopenvcmd()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hadoop-2.4.0.2.1.1.0-1621.winpkg//resources//hadoop-2.4.0.2.1.1.0-1621//etc//hadoop//
	HINSTANCE hNewExe = ShellExecute(NULL,"open","notepad.exe",m_strConfPath+"\\hadoop-env.cmd",NULL,SW_SHOWNORMAL);
	if ((DWORD)hNewExe <= 32)
	{
		AfxMessageBox("failed",MB_OK,NULL);
	}
}

void CCmdGUIDlg::OnCoresite()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hadoop-2.4.0.2.1.1.0-1621.winpkg//resources//hadoop-2.4.0.2.1.1.0-1621//etc//hadoop//
	ShellExecute(NULL,"open","notepad.exe",m_strConfPath+"\\core-site.xml",NULL,SW_SHOWNORMAL);

}

void CCmdGUIDlg::OnHdfssite()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hadoop-2.4.0.2.1.1.0-1621.winpkg//resources//hadoop-2.4.0.2.1.1.0-1621//etc//hadoop//
	ShellExecute(NULL,"open","notepad.exe",m_strConfPath+"\\hdfs-site.xml",NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnYarnsite()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hadoop-2.4.0.2.1.1.0-1621.winpkg//resources//hadoop-2.4.0.2.1.1.0-1621//etc//hadoop//
	ShellExecute(NULL,"open","notepad.exe",m_strConfPath+"\\yarn-site.xml",NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnMapredsite()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hadoop-2.4.0.2.1.1.0-1621.winpkg//resources//hadoop-2.4.0.2.1.1.0-1621//etc//hadoop//
	ShellExecute(NULL,"open","notepad.exe",m_strConfPath+"\\mapred-site.xml",NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnHdfsnamenodeformat()
{
	// TODO: 在此添加命令处理程序代码
	bool IsStartH = RunExec(_T("cmd"),_T("/c hadoop namenode -format"), INFINITE,true);//hdfs
	if(!IsStartH) 
	{
		AfxMessageBox("Launch Hdfs Format Failed! ");
	}
}

void CCmdGUIDlg::OnHivesite()
{
	// TODO: 在此添加命令处理程序代码D://hdp-2.1.1.0-winpkg//resources//hive-0.13.0.2.1.1.0-1621.winpkg//template//conf//
	ShellExecute(NULL,"open","notepad.exe","hive-site.xml",NULL,SW_SHOWNORMAL);

}

void CCmdGUIDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel = m_tab.GetCurSel();
	switch(CurSel)
	{
	case 0:
		m_paraHadoop.ShowWindow(true);
		m_paraSpark.ShowWindow(false);
		m_paraHive.ShowWindow(false);
		break;
	case 1:
		m_paraHadoop.ShowWindow(false);
		m_paraSpark.ShowWindow(false);
		m_paraHive.ShowWindow(true);
		break;
	case 2:
		m_paraHadoop.ShowWindow(false);
		m_paraHive.ShowWindow(false);
		m_paraSpark.ShowWindow(true);
		break;

	default:;
	} 

	*pResult = 0;
}
void CCmdGUIDlg::OnBnClickedBclear()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDITPATH)->SetWindowText(_T(""));
}
void CCmdGUIDlg::OnCmd()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open","cmd.exe",NULL,NULL,SW_SHOWNORMAL);
}


void CCmdGUIDlg::OnBrowsehdfs()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open","http://localhost:50070",NULL,NULL,SW_SHOWNORMAL);
}


void CCmdGUIDlg::OnBrowseyarn()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open","http://localhost:8088",NULL,NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnBnClickedBbrowsespark()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://localhost:8080",NULL,NULL,SW_SHOWNORMAL);
}


void CCmdGUIDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg about ;
	INT_PTR nResponse = about.DoModal();
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

}

static int quitFlg=0;
void CCmdGUIDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	Shell_NotifyIcon(NIM_DELETE, &nld);//消除托盘图标
	quitFlg=1;
	WSACleanup();
	EndDialog(0);
}

void CCmdGUIDlg::OnUpdate()
{
	// TODO: 在此添加命令处理程序代码
	//"E:\\RedHadoop Project\\RedHadoop Workstation\\RedHadoopWorkstation0.1\\RedHadoopWorkstation0.1\\Debug"
	int iValue =MessageBox("即将进入升级程序，主程序将退出，是否继续？","升级确认",MB_YESNO|MB_ICONQUESTION);
	//int yes =AfxMessageBox("即将进入升级程序，主程序将退出，是否继续？",MB_YESNO|MB_ICONINFORMATION ,NULL);
	if (iValue==IDYES)
	{
		CString info;
		info.Format("%s&type=active&ver=0.2&aid=%d&&cid=0",theApp.href,theApp.CRHAid::CRH_UPDATE);
		CHttpClient::getinstance()->startdoget(info);

		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		CString    sPath;   
		GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
		sPath.ReleaseBuffer    ();   
		int    nPos;   
		nPos=sPath.ReverseFind('\\');   
		sPath=sPath.Left(nPos);   

		::ShellExecuteA(NULL,"open","AutoUpdateClient.exe",NULL,sPath,SW_SHOW);
		
		exit(0);
	}
}



void CCmdGUIDlg::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	::ShellExecuteA(NULL,"open","http://www.redhadoop.com/studio/help/",NULL,NULL,SW_SHOW);
}



void CCmdGUIDlg::OnNnserviceinstall()
{
	// TODO: 在此添加命令处理程序代码
	bool IsInstalled = RunExec(_T("cmd"),_T("/c setup-dfs.cmd"), INFINITE);//hdfs
	if(IsInstalled) 
	{
		AfxMessageBox("hdfs service install sucessfully! ");
	}
}


void CCmdGUIDlg::OnUpdateNnserviceinstall(CCmdUI *pCmdUI)
{
	if(CWindowsService::CheckServiceStatus("Hadoop-NameNode")
	==CWindowsService::SEV_STATUS::SEV_NO) {
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->Enable(FALSE);
}


void CCmdGUIDlg::OnExpWc()
{
	CFileDialog dlg(TRUE, 
		"Text Files (*txt)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
		_T("Text Files (*.txt;*.log)|*.txt;*.log||"),
		NULL);
	dlg.m_ofn.lpstrTitle=_T("请选择文本文件");

	dlg.m_ofn.nMaxFile = 100 * MAX_PATH; 
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];  
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile); 
	int retval = dlg.DoModal();
	if(retval==IDCANCEL)
		return;
	POSITION pos_file;
	pos_file = dlg.GetStartPosition();
	CArray<CString, CString> ary_filename;
	while(pos_file != NULL)
		ary_filename.Add(dlg.GetNextPathName(pos_file));



	std::ofstream outFile("run-hadoop-wordcount.bat",std::ios::trunc);
	std::string strcmd="@echo off  ";
	outFile << strcmd << endl;
	strcmd="cmd /c hadoop fs -rm -r /input  ";
	outFile << strcmd << endl;
	strcmd = "cmd /c hadoop fs -rm -r /output ";
	outFile << strcmd << endl;
	strcmd = "cmd /c hadoop fs -mkdir /input ";
	outFile << strcmd << endl;
	strcmd = "cmd /c hadoop fs -mkdir /output ";
	outFile << strcmd << endl;
	char buff[16];
	std::string filename;
	for(int i=0; i<ary_filename.GetSize(); i++)
	{
		ary_filename.GetAt(i);
		strcmd = "copy /y ";
		strcmd += ary_filename.GetAt(i).GetBuffer();
		ary_filename.GetAt(i).ReleaseBuffer();
		filename="wordcount";
		filename += itoa(i,buff,10);
		filename +=".txt";
		strcmd += " ";
		strcmd += filename;
		outFile << strcmd << endl;
		strcmd = "cmd /c hadoop fs  -put ";
		strcmd += filename;
		strcmd += " /input ";
		outFile << strcmd << endl;
		strcmd = "del ";
		strcmd += filename;
		outFile << strcmd << endl;
	}


	strcmd="cmd /c hadoop jar  ";
	strcmd += m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\share\\hadoop\\mapreduce\\hadoop-mapreduce-examples-*.jar  wordcount /input /output/out ";
	outFile << strcmd << endl;
	strcmd="echo ======================Output Result========================  ";
	outFile << strcmd << endl;	
	strcmd="cmd /c hadoop fs  -get /output/out/part-r-00000  wordcountresult.txt  ";
	outFile << strcmd << endl;

	strcmd="type  wordcountresult.txt  ";
	outFile << strcmd << endl;
	outFile << "pause" << endl;
	strcmd="del  wordcountresult.txt  ";
	outFile << strcmd << endl;
	
	outFile.close();
	delete dlg.m_ofn.lpstrFile;

	bool IsInstalled = RunExec(_T("cmd"),_T("/c run-hadoop-wordcount.bat"), INFINITE,true);//hdfs
	if(!IsInstalled) 
	{
		AfxMessageBox("Run Wordcount Example Failed! ");
	}
}



void CCmdGUIDlg::OnRmserviceinstall()
{
		// TODO: 在此添加命令处理程序代码
	bool IsInstalled = RunExec(_T("cmd"),_T("/c setup-yarn.cmd"), INFINITE);//hdfs
	if(IsInstalled) 
	{
		AfxMessageBox("yarn service install sucessfully! ");
	}
}


void CCmdGUIDlg::OnUpdateRmserviceinstall(CCmdUI *pCmdUI)
{
	if(CWindowsService::CheckServiceStatus("Yarn-ResourceManager")
	==CWindowsService::SEV_STATUS::SEV_NO) {
		pCmdUI->Enable(TRUE);
		return;
	}
	pCmdUI->Enable(FALSE);
}


void CCmdGUIDlg::OnExpPi()
{
	std::ofstream outFile("run-hadoop-pi.bat",std::ios::trunc);
	std::string strcmd="@echo off  ";
	outFile << strcmd << endl;
	strcmd="cmd /c hadoop jar  ";
	strcmd += m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\share\\hadoop\\mapreduce\\hadoop-mapreduce-examples-*.jar  pi 1 10000 ";

	outFile << strcmd << endl;
	outFile << "pause" << endl;

	outFile.close();

	//strcmd += "\\..\\..\\share\\hadoop\\mapreduce\\hadoop-mapreduce-examples-2.2.0.jar  pi 1 10000 ";
	//bool IsInstalled = RunExec(_T("cmd"),_T("/c hadoop jar D:\Redhadoop\CRH3\hadoop\share\hadoop\mapreduce\hadoop-mapreduce-examples-2.4.0.2.1.1.0-1621.jar  pi 1 10000 "), INFINITE);//hdfs
	bool IsInstalled = RunExec(_T("cmd"), _T("/c run-hadoop-pi.bat"), INFINITE,true);//hdfs

	if(!IsInstalled) 
	{
		AfxMessageBox("Run Pi Example Failed! ");
	}
}
LRESULT CCmdGUIDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)////wParam是响应消息的图标ID，lParam中是Windows消息
{
	CMenu menu;

	switch(lParam)
	{
	case WM_RBUTTONDOWN:  //在托盘图标上单击鼠标右键
		{
			if (menu.LoadMenu(IDR_MENU2))      
			{
				CMenu* pPopup = menu.GetSubMenu(0);
				CPoint point;
				if (IsWindowVisible())  //如果窗口没有隐藏
				{
					pPopup->EnableMenuItem(ID_SHOW,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				}
				else
					pPopup->EnableMenuItem(ID_HIDE,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
				GetCursorPos(&point);  //获得鼠标位置
				SetForegroundWindow();  //显示窗口
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
			}
			ShowWindow(SW_SHOWNORMAL);

		}
		break;
	case WM_LBUTTONDOWN:
		ModifyStyleEx(0,WS_EX_TOPMOST);
		OnShow();
		break;
	}
	return 0;

}
void CCmdGUIDlg::OnShow()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnHide()
{
	// TODO: 在此添加命令处理程序代码
	ShowWindow(SW_HIDE);
}

void CCmdGUIDlg::OnHdfsexitsafemode()
{
	bool IsStartH = RunExec(_T("cmd"),_T("/c  hdfs dfsadmin -safemode leave"), INFINITE,true);//hdfs
	if(IsStartH) 
	{
		AfxMessageBox("HDFS Safe Mode is Off! ");
	}

}

void CCmdGUIDlg::OnBnClickedBeclipse()
{
	std::string strcmd = m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\..\\eclipse\\eclipse.exe ";
	RunExec(strcmd.c_str(),NULL, INFINITE);//hdfs
}

void CCmdGUIDlg::OnDestroy()
{
	CDialog::OnDestroy();
	Shell_NotifyIcon(NIM_DELETE, &nld);//消除托盘图标
	m_paraHadoop.DestroyWin();
}

void CCmdGUIDlg::OnSeasbaseInquire()
{
	// TODO: 在此添加命令处理程序代码
	
	CSeasbaseVideoDlg video =new CSeasbaseVideoDlg();
	video.strVideoPath = videoPath;
    video.DoModal();

}

void CCmdGUIDlg::OnSeasbaseLoadvideo()
{
	CDlgVideoLoad video =new CDlgVideoLoad();
	int ibt =video.DoModal();
	if (ibt==IDOK)
	{
		videoPath=video.strPath;
	}
}

void CCmdGUIDlg::OnSeasbaseHdfsexplorer()
{
	// TODO: 在此添加命令处理程序代码
	std::string strcmd = m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	//strcmd.c_str()   D:\redhadoop\RedhadoopStudio\CRH3\hadoop\etc\hadoop
	strcmd += "\\..\\..\\..\\HDFS Explorer - beta.application ";
	RunExec(strcmd.c_str(),NULL, INFINITE);//hdfs

}

void CCmdGUIDlg::OnSparkmaster()
{
	// TODO: 在此添加命令处理程序代码
	std::string strcmd = m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\..\\spark\\conf\\master";
	ShellExecute(NULL,"open","notepad.exe",strcmd.c_str(),NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnSparkslaves()
{
	// TODO: 在此添加命令处理程序代码
	std::string strcmd = m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\..\\spark\\conf\\slaves";
	ShellExecute(NULL,"open","notepad.exe",strcmd.c_str(),NULL,SW_SHOWNORMAL);
}

void CCmdGUIDlg::OnExpSpi()
{
	// TODO: 在此添加命令处理程序代码
	std::ofstream outFile("run-spark-pi.bat",std::ios::trunc);
	std::string strcmd="cmd /c ";
	outFile << "@echo off  " << endl;
	
	strcmd += m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\..\\spark\\bin\\spark-submit.cmd --class org.apache.spark.examples.SparkPi --executor-memory 6G  --total-executor-cores 2  ";
	strcmd += m_strConfPath.GetBuffer();
	m_strConfPath.ReleaseBuffer();
	strcmd += "\\..\\..\\..\\spark\\lib\\spark-examples-1.1.0-hadoop2.4.0.jar 1000";
	outFile << strcmd << endl;

	outFile << "pause" << endl;
	
	outFile.close();

	bool IsInstalled = RunExec(_T("cmd"), _T("/c run-spark-pi.bat"), INFINITE,true);//hdfs

	if(!IsInstalled) 
	{
		AfxMessageBox("Run SparkPi Example Failed! ");
	}
}


void CCmdGUIDlg::OnBnClickedVehicle()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxMessageBox("interface is  initializing!");
	CVehicleInquireDlg vehicle =new CVehicleInquireDlg();
	vehicle.strVideoPath = videoPath;
	vehicle.DoModal();

}
