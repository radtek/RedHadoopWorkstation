#pragma once
#if   _MSC_VER   >   1000   
#pragma   once   
#endif   //   _MSC_VER   >   1000   
#include   "wininet.h"   
#include   "afxinet.h" 
#include "..\update\KevinThread.h"
#include "..\..\common\KevinAutoLock.h"
#include <list>
#include <string>
class   CHttpClient : 
public CKevinThread       
{   
public: 
	static CHttpClient * getinstance();
	static void destory();
	void start();
	void stop();
	bool  startdoget(CString   href);
	bool  startdopost(CString   href);
	void   addParam(CString   name,CString   value);   
	CString   doPost(CString   href);   
	CString   doGet(CString   href);   
 
	virtual   ~CHttpClient();   
private:
	CHttpClient();  
	virtual void ThreadProcMain(void);
private:   
	CString   CONTENT;   
	int   CL;   
	CStringList   values;   
	CStringList   names; 
	bool getorpost;
	std::string href;
	static  CHttpClient * pinstance;
	 CKevinEvent m_event;
	 CKevinCritSec m_sec;

	typedef struct _httprequest {
		_httprequest(bool getorpost1,std::string & href1)
			:getorpost(getorpost1),href(href1){};
		bool getorpost;
		std::string href;
	}httprequest;

	std::list<httprequest*> m_hreflist;

	bool bstart;
	bool bexit;
	int bruning;
	
};   


