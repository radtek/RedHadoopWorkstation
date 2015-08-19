#pragma once

#include<msxml.h>
#include "updatefile.h"

#include ".\kevinautolock.h"
#include <list>
#include <tchar.h>
#include "atlconv.h"
#include <afxstr.h>
#include "afxcmn.h"
using namespace std;

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"GB2312\" ?> \r\n<xmlRoot keyname=\"KeyValue\">\r\n"
#define XML_TAIL   "</xmlRoot>\r\n"

#define XML_PRODUCT_HEADER "\t<Product productname=\"%s\" createdate=\"%s\" filenum=\"%u\">\r\n"
#define XML_PRODUCT_TAIL   "\t</Product>\r\n"

class CUpdateFiles
{
public:
	CUpdateFiles(void);
	virtual ~CUpdateFiles(void);

private:
	list<CUpdateFile *> m_listFile;
	CKevinCritSec m_lock;
	TCHAR m_szMainDir[_MAX_PATH]; 
	TCHAR m_szWindowsDir[_MAX_PATH]; 
	TCHAR m_szSystemDir[_MAX_PATH]; 
	bool BuildXMLStr(CUpdateFile* pUpdateFile, char szXMLStr[1024]);
	bool GetFileList(IXMLDOMNodeList* pDOMNodeList);
public:
	void ClearAllList(void);
	bool ReadFromXMLFile(TCHAR* pszFileName);
	bool SaveToXMLFile(TCHAR* pszFileName);
	bool AddUpdateFile(TCHAR* pszFileName);
	//void FillListCtrl(CListCtrl& m_lstCtrl);
	//void DeleteItem(CUpdateFile * pUpdateFile);
	CUpdateFile* PopUpdateFile(void);
	void PushUpdateFile(CUpdateFile* pUpdateFile);

	// only client
	DWORD CheckNeedUpdate(void);
	DWORD GetNeedUpdateTotalLength(void);
	DWORD Count(void);

	CString m_sCommonURL;
};
