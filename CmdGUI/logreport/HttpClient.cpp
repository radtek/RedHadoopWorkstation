  #include   "stdafx.h"     
  #include   "HttpClient.h"   
    
  #ifdef   _DEBUG   
  #undef   THIS_FILE   
  static   char   THIS_FILE[]=__FILE__;   
  #define   new   DEBUG_NEW   
  #endif   
    
  //////////////////////////////////////////////////////////////////////   
  //   Construction/Destruction   
  //////////////////////////////////////////////////////////////////////   
   CHttpClient * CHttpClient::pinstance=NULL;
  CHttpClient::CHttpClient()   
  {   
    getorpost = true;
	bexit = false;
	bruning=0;
  }   
    
  CHttpClient::~CHttpClient()   
  {   
    
  }   
    
  CString   CHttpClient::doGet(CString   href)   
  {   
	  CString   httpsource;   
	  CInternetSession   session1(NULL,0);   //类CInternetSession 创建并初始化一个或多个同时的Internet 会话。如果需要，还可描述与代理服务器的连接。
	  CHttpFile*   pHTTPFile=NULL;   
	  try{   
		  pHTTPFile=(CHttpFile*)session1.OpenURL(href);   
		  //session1.   
	  }catch(...){   
		  pHTTPFile=NULL;   
	  }   
	  if(pHTTPFile)   
	  {   
		  CString   text;   
		  for(int   i=0;pHTTPFile->ReadString(text);i++)   
		  {   
			  httpsource = (httpsource+text+"\r\n");   
		  }   
		  pHTTPFile->Close();   
		  delete   pHTTPFile;   
	  }
	  else   
	  {   

	  }   
	  return   httpsource;   
  }   

  CString   CHttpClient::doPost(CString   href)   
  {   
	  CString   httpsource;   
	  CInternetSession   session1;   
	  CHttpConnection*   conn1=NULL;   
	  CHttpFile*   pFile   =   NULL;       
	  CString   strServerName;   
	  CString   strObject;   
	  INTERNET_PORT   nPort;   
	  DWORD   dwServiceType;   
	  AfxParseURL((LPCTSTR)href,dwServiceType,   strServerName,   strObject,   nPort);   
	  DWORD   retcode;               
	  char   outBuff[1000];  
	  try       
	  {                   
		  conn1   =   session1.GetHttpConnection(strServerName,nPort);           
		  pFile   =   conn1->OpenRequest(0,strObject,NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);           
		  pFile->   AddRequestHeaders("Content-Type:   application/x-www-form-urlencoded");           
		  pFile   ->   AddRequestHeaders("Accept:   */*");                           
		  pFile   ->   SendRequest(NULL,0,outBuff,strlen(outBuff)+1);                   
		  pFile   ->   QueryInfoStatusCode(retcode);                   
	  }                             
	  catch   (CInternetException   *   e){};                   
	  if(pFile)   
	  {   
		  CString   text;   
		  for(int   i=0;pFile->ReadString(text);i++)   
		  {   
			  httpsource=httpsource+text+"\r\n";   
		  }   
		  pFile->Close();   
	  }else   
	  {   

	  }   
	  return   httpsource;   
	  delete   pFile;           
	  delete   conn1;           
	  session1.Close();   
  }   


  void   CHttpClient::addParam(CString   name,   CString   value)   
  {   
	  names.AddTail((LPCTSTR)name);   
	  values.AddTail((LPCTSTR)value);   
	  CString   eq="=";   
	  CString   an="&";   
	  CONTENT=CONTENT+name+eq+value+an;   
	  CL=CONTENT.GetLength();   
  }   


  	bool  CHttpClient::startdoget(CString   href1)
	{
		CKevinAutoLock lock(&m_sec);
		href=href1;
		getorpost = true;
		httprequest *hr= new httprequest(getorpost,href);
	
		m_hreflist.push_back(hr);
		m_event.Set();
		return true;
	}
	bool  CHttpClient::startdopost(CString   href1)
	{
		CKevinAutoLock lock(&m_sec);
		href=href1;
		getorpost = false;
		httprequest *hr= new httprequest(getorpost,href);
		m_hreflist.push_back(hr);
		m_event.Set();
		return true;
	}

	void CHttpClient::ThreadProcMain(void)
	{	
		while(bstart)
		{
			httprequest *hr=NULL;
			m_event.Wait(0);
			if(bstart==false)
				break;
			do {
				CKevinAutoLock lock(&m_sec);
				bruning++;
				if(m_hreflist.size()==0)
					continue;
				hr=m_hreflist.front();
				m_hreflist.pop_front();
			}while(0);
			if(NULL==hr)
				continue;
			CString href =hr->href.c_str();
			if(hr->getorpost)
				doGet(href);
			else
				doPost(href);
			delete hr;
		}
		bexit = true;

	}
	void CHttpClient::start()
	{
		bruning = 0;
		bexit = false;
		bstart=true;
		StartThread();
	}

	void CHttpClient::stop()
	{

		int waittimes= 10;
		while(bruning == 0 && waittimes-- >0)
		{
			::Sleep(100);
		}

		bstart=false;
		m_event.Set();
		while (false==bexit)::Sleep(1);
	}
	CHttpClient * CHttpClient::getinstance()
	{
		if(NULL==pinstance)
		{
			pinstance = new CHttpClient();
		}
		return pinstance;
	}
	void CHttpClient::destory()
	{
		if(NULL==pinstance)
			return;
		delete pinstance;
		pinstance=NULL;
	}