// DialogHive.cpp : 实现文件
//

#include "stdafx.h"
#include "CmdGUI.h"
#include "DialogHive.h"
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

// CDialogHive 对话框

IMPLEMENT_DYNAMIC(CDialogHive, CDialog)

CDialogHive::CDialogHive(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHive::IDD, pParent)
{
  //m_bStopHive.EnableWindow(0);
//  m_bStopHiveServer.EnableWindow(0);
  IsStartHive =false;
  IsStartHiveServer =false;
}

CDialogHive::~CDialogHive()
{
}

void CDialogHive::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BStartHive, m_bStartHive);
	DDX_Control(pDX, IDC_BStartHiveServer, m_bStartHiveServer);
}


BEGIN_MESSAGE_MAP(CDialogHive, CDialog)
	
	ON_BN_CLICKED(IDC_BStartHive, &CDialogHive::OnBnClickedBstarthive)
	ON_BN_CLICKED(IDC_BStartHiveServer, &CDialogHive::OnBnClickedBstarthiveserver)
END_MESSAGE_MAP()

//启动一个程序，直到它运行结束
BOOL HiveRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds)
{    //  DWORD Sk=GetTickCount();
	   
       SHELLEXECUTEINFO ShExecInfo = {0};
       ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
       ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
       ShExecInfo.hwnd = NULL;
       ShExecInfo.lpVerb = NULL;
       ShExecInfo.lpFile = cmd;
       ShExecInfo.lpParameters = para;
       ShExecInfo.lpDirectory = NULL;
       ShExecInfo.nShow =SW_SHOWMAXIMIZED; // SW_HIDE
       ShExecInfo.hInstApp = NULL;
       BOOL suc = ShellExecuteEx(&ShExecInfo);
       //WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds);
	  /* DWORD Ek=GetTickCount();
	   printf("################################k:%dms\n",Ek-Sk);*/
       return suc;
}
// CDialogHive 消息处理程序


void CDialogHive::OnBnClickedBstarthive()
{
	// TODO: 在此添加控件通知处理程序代码
	//boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));  
	//boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));  
	//boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); 
	//transport->open(); 
	//// 再定义一个对象client，又是以"Serv"开头的类
	//ServClient client(protocol);
	//client.ping();
	//std::string s ="/c hive.cmd";
	//client.run(s);
	//transport->close(); 
	CString info;
	info.Format("%s&type=active&ver=0.2&aid=%d&service=%d&cid=0",theApp.href,theApp.CRH_USE,theApp.CRH_HIVE);
	CHttpClient::getinstance()->startdoget(info);
	 IsStartHive = HiveRunExec(_T("cmd"),_T("/c hive.cmd"), INFINITE);
	if(!IsStartHive) 
	{
		AfxMessageBox("hive start failed! ");
	}

}
void CDialogHive::OnBnClickedBstarthiveserver()
{
	// TODO: 在此添加控件通知处理程序代码hiveserver.cmd
	//boost::shared_ptr<TSocket> socket(new TSocket("localhost", 9090));  
	//boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));  
	//boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); 
	//transport->open(); 
	//// 再定义一个对象client，又是以"Serv"开头的类
	//ServClient client(protocol);
	//client.ping();
	//std::string s ="/c hiveserver.cmd";
	//client.run(s);
	//transport->close(); 
	 IsStartHiveServer = HiveRunExec(_T("cmd"),_T("/c hiveserver.cmd"), INFINITE);//=true;//hdfs
	if(!IsStartHiveServer) 
	{
		
		AfxMessageBox("hiveserver start failed! ");
	}

}

