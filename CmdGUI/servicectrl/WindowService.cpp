#include "StdAfx.h"    
#include "WindowsService.h"    
#include "winsvc.h"    
   
CWindowsService::CWindowsService(void)   
{   
}   
   
CWindowsService::~CWindowsService(void)   
{   
}   

/*****************************************************  
��������˵��  
�� m_Test.AddService("C://Test//Test.exe","MyServiceName","DisplayName",NULL);  
pPara Ϊ������Щ������
*****************************************************/  
   
BOOL CWindowsService::AddService(const char*pSourceName,const char*pServiceName,const char*pDisName,const char*pPara)   
{   
    SC_HANDLE hSC = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );   
    if ( hSC == NULL )   
    {   
        return FALSE;//_ERROR_;    
    }   
    CString strBinnaryPathName;   
    //strBinnaryPathName.Format("%s%s%s %s","/"",pSourceName,"/"",pPara);   
   
    SC_HANDLE hSvc = CreateService( hSC,   
                              pServiceName,       //��������    
                                    pDisName,          //��ʾ����    
                                    SERVICE_ALL_ACCESS,   
                                    SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,   
                                    SERVICE_AUTO_START,   
                                    SERVICE_ERROR_NORMAL,   
                                    strBinnaryPathName,    
                                    NULL,   
                                    NULL,   
                                    NULL,   
                                    NULL,   
                                    NULL );   
    if( hSvc == NULL )   
 {   
        ::CloseServiceHandle( hSC );   
        return FALSE;//_ERROR_;    
    }   
    ::CloseServiceHandle( hSvc );   
    ::CloseServiceHandle( hSC );   
    return TRUE;   
}   
   
   
/*  
**************************************************  
ɾ������  
**************************************************  
*/   
BOOL CWindowsService::RemoveService(const char*pServiceName)   
{   
    // UnInstall the XService from Service Control Manager Database    
    SC_HANDLE hSC = ::OpenSCManager( NULL,NULL, SC_MANAGER_ALL_ACCESS);   
    if( hSC == NULL)   
    {   
        return FALSE;   
    }   
   
    SC_HANDLE hSvc = ::OpenService( hSC, pServiceName, SERVICE_STOP | DELETE/*SERVICE_ALL_ACCESS*/);    
    if( hSvc == NULL)   
    {   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }

 SERVICE_STATUS status;   
    ::ControlService(hSvc, SERVICE_CONTROL_STOP, &status);   
        
    if ( !::DeleteService(hSvc))
 {
  DWORD _error =GetLastError();//ERROR_ACCESS_DENIED
  return FALSE;
 }  

 ::CloseServiceHandle(hSvc); 
 ::CloseServiceHandle(hSC);
 return TRUE;  
}   
   
/*  
**************************************************  
���״̬  
**************************************************  
*/   
BYTE CWindowsService::CheckServiceStatus(const char*pServiceName)   
{   
    SC_HANDLE hSC = ::OpenSCManager( NULL,NULL, GENERIC_EXECUTE);   
    if( hSC == NULL)   
    {   
        return SEV_ERROR;   
    }   
    // �򿪷���    
    SC_HANDLE hSvc = ::OpenService( hSC, pServiceName, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);   
    if( hSvc == NULL)   
    {   
        ::CloseServiceHandle( hSC);   
        return SEV_NO;   
    }   
    // ��÷����״̬    
    SERVICE_STATUS status;   
    if( ::QueryServiceStatus( hSvc, &status) == FALSE)   
    {   
       // TRACE( "Get Service state error��");    
        ::CloseServiceHandle( hSvc);   
        ::CloseServiceHandle( hSC);   
        return SEV_ERROR;   
    }   
    if(status.dwCurrentState == SERVICE_RUNNING)   
    {   
        ::CloseServiceHandle( hSvc);   
        ::CloseServiceHandle( hSC);   
        return SEV_RUNING;   
    }   
    else if(status.dwCurrentState == SERVICE_STOPPED)   
    {   
        ::CloseServiceHandle( hSvc);   
        ::CloseServiceHandle( hSC);   
        return SEV_STOPED;   
    }   
    ::CloseServiceHandle( hSvc);   
    ::CloseServiceHandle( hSC);   
    return SEV_ERROR;   
}   
/*  
**************************************************  
��������  
**************************************************  
*/   
BOOL CWindowsService::StartSevice(const char*pServiceName)   
{   
    SC_HANDLE hSC = ::OpenSCManager( NULL,NULL, GENERIC_EXECUTE);   
    if( hSC == NULL)   
    {   
        return FALSE;   
    }   
    // �򿪷���    
    SC_HANDLE hSvc = ::OpenService( hSC, pServiceName, SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);   
    if( hSvc == NULL)   
    {   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }   
    // ��÷����״̬    
    SERVICE_STATUS status;   
    if( ::QueryServiceStatus( hSvc, &status) == FALSE)   
    {   
        //TRACE( "Get Service state error��");    
        ::CloseServiceHandle( hSvc);   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }
 
 if( status.dwCurrentState != SERVICE_RUNNING) //�������ֹͣ״̬���������񣬷��򷵻�      
    {
  // ��������
  if( ::StartService( hSvc, NULL, NULL) == FALSE)
  { 
   //TRACE( "start service error��");
   ::CloseServiceHandle( hSvc);   
            ::CloseServiceHandle( hSC);   
            return FALSE;   
        }   
        // �ȴ���������    
        while( ::QueryServiceStatus( hSvc, &status) == TRUE)   
        {   
            ::Sleep(500);   
            QueryServiceStatus( hSvc, &status);   
            if( status.dwCurrentState == SERVICE_START_PENDING)   
            {   
                //AfxMessageBox( "start success��");    
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return TRUE;   
            }   
            ::Sleep( status.dwWaitHint);   
            QueryServiceStatus( hSvc, &status);   
            if( status.dwCurrentState == SERVICE_RUNNING)   
            {   
                //AfxMessageBox( "start success��");    
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return TRUE;   
            }   
            else   
            {   
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return FALSE;   
            } 
  }
 }   
    ::CloseServiceHandle( hSvc);   
    ::CloseServiceHandle( hSC);   
    return TRUE;   
}   
/*  
**************************************************  
ֹͣ����  
**************************************************  
*/   
BOOL CWindowsService::StopSevice(const char*pServiceName)   
{   
    SC_HANDLE hSC = ::OpenSCManager( NULL,NULL, GENERIC_EXECUTE);   
    if( hSC == NULL)   
    {   
        return FALSE;   
    }   
    // �򿪷���    
    SC_HANDLE hSvc = ::OpenService( hSC, pServiceName,   
        SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);   
    if( hSvc == NULL)   
    {   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }   
    // ��÷����״̬    
    SERVICE_STATUS status;   
    if( ::QueryServiceStatus( hSvc, &status) == FALSE)   
    {   
        //TRACE( "Get Service state error��");    
        ::CloseServiceHandle( hSvc);   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }   
      //�������ֹͣ״̬���������񣬷���ֹͣ����    
    if( status.dwCurrentState == SERVICE_RUNNING)   
    {   
        // ֹͣ����    
        if( ::ControlService( hSvc, SERVICE_CONTROL_STOP, &status) == FALSE)   
        {   
            //TRACE( "stop service error��");    
            ::CloseServiceHandle( hSvc);   
            ::CloseServiceHandle( hSC);   
            return FALSE;   
        }   
        // �ȴ�����ֹͣ    
        while( ::QueryServiceStatus( hSvc, &status) == TRUE)   
        {   
             //::Sleep( status.dwWaitHint);    
            ::Sleep(1000);   
            QueryServiceStatus( hSvc, &status);   
            if( status.dwCurrentState == SERVICE_STOPPED)   
            {   
               //AfxMessageBox( "stop success��");    
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return TRUE;   
            }   
            ::Sleep(3000);   
            QueryServiceStatus( hSvc, &status);   
            if( status.dwCurrentState == SERVICE_STOPPED)   
            {   
               //AfxMessageBox( "stop success��");    
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return TRUE;   
            }   
            ::Sleep( status.dwWaitHint);   
            QueryServiceStatus( hSvc, &status);   
            if( status.dwCurrentState == SERVICE_STOPPED)   
            {   
               //AfxMessageBox( "stop success��");    
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return TRUE;   
            }   
            else   
            {   
                ::CloseServiceHandle( hSvc);   
                ::CloseServiceHandle( hSC);   
                return FALSE;   
            }   
        }   
    }   
    return TRUE;   
}   
   
   
   
   
   
BOOL CWindowsService::GetSevicePath(const char*pServiceName,CString & strServicePath)   
{    
    SC_HANDLE hSC = ::OpenSCManager( NULL,NULL, SC_MANAGER_ALL_ACCESS);   
    if( hSC == NULL)   
    {   
        return FALSE;   
    }   
    // �򿪷���    
    SC_HANDLE hSvc = ::OpenService( hSC, pServiceName,SERVICE_ALL_ACCESS);   
    if( hSvc == NULL)   
    {   
        ::CloseServiceHandle( hSC);   
        return FALSE;   
    }   
    LPQUERY_SERVICE_CONFIG  pServiceConfig;   
    DWORD   cbBufSize = 1024*8;//sizeof(SERVICE_DESCRIPTION_STRUCT);    
       
    DWORD   cbBytesNeeded;   
    CString strPath;   
    pServiceConfig = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, cbBufSize);      //ע������㹻�Ŀռ�     
    QueryServiceConfig( hSvc, pServiceConfig  ,cbBufSize, &cbBytesNeeded);    
    strPath = pServiceConfig->lpBinaryPathName;   
    strPath.Replace('"',' ');   
    //int iFind = strPath.Find("////");    
    //strPath = strPath.Left(iFind+1);    
    int iFind = strPath.Find("exe");   
    strPath = strPath.Left(iFind+3);   
    strServicePath = strPath;   
    ::LocalFree(pServiceConfig);   
    ::CloseServiceHandle( hSvc);   
    ::CloseServiceHandle( hSC);   
       
    return TRUE;   
}   
   
   
BOOL CWindowsService::GetCurPath(CString &strCurPath)   
{   
    char szTemp[MAX_PATH] = {0};   
    ::GetModuleFileName(NULL, szTemp, sizeof(szTemp)-1);   
    int iLen = (int)strlen(szTemp);   
    for(int i = iLen - 1; i >= 0; i--)   
    {   
        if (szTemp[i] == '//')   
        {   
            break;   
        }   
        else   
        {   
            szTemp[i] = 0;   
        }   
    }   
    strCurPath = szTemp;   
    return TRUE;   
}   
//3.���ð�����
//
//#include "WindowsService.h"
//void CDemoSvrDlg::OnOK() 
//{
// // TODO: Add extra validation here 
// CWindowsService ws;
// ws.AddService("D://demo.exe","demo","demo",NULL);//����
//
// ws.StartSevice("demo");//����
//
// if(ws.CheckServiceStatus("demo") == 0x03)
//  AfxMessageBox("����״̬");
//
// ws.StopSevice("demo");
//
// if(ws.CheckServiceStatus("demo") == 0x04)
//  AfxMessageBox("ֹͣ״̬");
//
// ws.RemoveService("demo");//ɾ��
// //CDialog::OnOK();
//}
