#pragma once 
 
class CWindowsService 
{ 
public: 
 CWindowsService(void); 
public: 
 ~CWindowsService(void); 
 enum SEV_STATUS 
 { 
  SEV_ERROR = 0x00, 
  SEV_NO  = 0x01, 
  SEV_HAVE = 0x02, 
  SEV_RUNING = 0x03, 
  SEV_STOPED = 0x04 
 }; 
public: 
static  BOOL AddService(const char*pSourceName,const char*pServiceName,const char*pDisName,const char*pPara); 
 static BOOL RemoveService(const char*pServiceName); 
 static BYTE CheckServiceStatus(const char*pServiceName); 
 static BOOL StartSevice(const char*pServiceName); 
 static BOOL StopSevice(const char*pServiceName); 
 static BOOL GetSevicePath(const char*pServiceName,CString &strServicePath); 
 static BOOL GetCurPath(CString &strCurPath); 
}; 