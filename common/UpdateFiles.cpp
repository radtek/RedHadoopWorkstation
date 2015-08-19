#include "StdAfx.h"
#include ".\updatefiles.h"

CUpdateFiles::CUpdateFiles(void)
{
	::GetModuleFileName(NULL, m_szMainDir, sizeof(m_szMainDir));
	TCHAR *p = _tcsrchr(m_szMainDir, _T('\\'));	if (p) 		*p = 0;
	::GetWindowsDirectory(m_szWindowsDir, sizeof(m_szWindowsDir));
	::GetSystemDirectory(m_szSystemDir, sizeof(m_szSystemDir));
}

CUpdateFiles::~CUpdateFiles(void)
{
	ClearAllList();
}

void CUpdateFiles::ClearAllList(void)
{
	CKevinAutoLock  lock(&m_lock);

	while (m_listFile.size() > 0) {
		list<CUpdateFile*>::iterator it = m_listFile.begin();
		CUpdateFile* pUpdateFile = *it;
		m_listFile.erase(it);
		SAFE_DELETE(pUpdateFile);
	}
}

bool CUpdateFiles::ReadFromXMLFile(TCHAR* pszFileName)
{
	ClearAllList();

	CKevinAutoLock  lock(&m_lock);

	bool bRet = false;

	IXMLDOMDocument* pDOMDocument = NULL;
	IXMLDOMElement*  pDOMElement = NULL;
	IXMLDOMNodeList* pDOMNodeList = NULL;

	BSTR bstrBaseName = ::SysAllocString(_com_util::ConvertStringToBSTR("File"));
	CComVariant vFile(pszFileName);	VARIANT_BOOL vBool;

	if(SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pDOMDocument) )) {	
		if(SUCCEEDED(pDOMDocument->put_async(VARIANT_FALSE)) && SUCCEEDED(pDOMDocument->load(vFile, &vBool))) {
			if(VARIANT_TRUE == vBool && SUCCEEDED(pDOMDocument->get_documentElement(&pDOMElement))) {
				if (SUCCEEDED(pDOMElement->getElementsByTagName(bstrBaseName, &pDOMNodeList))) {
					return GetFileList(pDOMNodeList);
				}
			}
		}
	}

	::SysFreeString(bstrBaseName);
	SAFE_RELEASE(pDOMDocument);
	SAFE_RELEASE(pDOMElement);
	SAFE_RELEASE(pDOMNodeList);

	return bRet;
}
//
bool CUpdateFiles::GetFileList(IXMLDOMNodeList* pDOMNodeList)
{
	long lLen = 0;
	if (SUCCEEDED(pDOMNodeList->get_length(&lLen))) {
		for (int i = 0; i < lLen; i ++) {
			IXMLDOMNode* pNode = NULL;
			if (SUCCEEDED(pDOMNodeList->get_item(i, &pNode))) {
				CUpdateFile* pUpdateFile = new CUpdateFile(_T(""));
				if (!pUpdateFile) return false;

				IXMLDOMNode* pNext=NULL;
				IXMLDOMNode* pChild;
				pNode->get_firstChild(&pChild);

				USES_CONVERSION;
				while(pChild)
				{					
					TCHAR szPathName[_MAX_PATH];
					memset(szPathName, 0, sizeof(szPathName));
					BSTR bstrNodeName, bstrText;
					pChild->get_nodeName(&bstrNodeName);
					pChild->get_text(&bstrText);

					if (0 == strcmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("FileName"))) {
						pUpdateFile->m_sFileName = bstrText;
					} else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("FileVersion"))){
						pUpdateFile->m_sFileVersion = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("ProductVersion"))){
						pUpdateFile->m_sProductVersion = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("Length"))){
						pUpdateFile->m_ullSize = atoi(T2A((LPTSTR)bstrText));
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("FileDateLow"))){
						pUpdateFile->m_fileTime.dwLowDateTime = atoi(T2A((LPTSTR)bstrText));
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("FileDateHigh"))){
						pUpdateFile->m_fileTime.dwHighDateTime = atoi(T2A((LPTSTR)bstrText));
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("InstallDir"))){
						pUpdateFile->m_sInstallDir = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("Register"))){
						pUpdateFile->m_bRegister = (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrText), _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("ZipFile"))){
						pUpdateFile->m_bZipFile = (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrText), _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("ForceOverwrite"))){
						pUpdateFile->m_bForceOverwrite = (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrText), _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("Execute"))){
						pUpdateFile->m_bExecute = (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrText), _T("YES"))) ? true : false;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("ExecutePar"))){
						pUpdateFile->m_sExecutePar = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("LocalPath"))){
						pUpdateFile->m_sLocalPath = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("URL"))){
						pUpdateFile->m_sURL = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("UserName"))){
						pUpdateFile->m_sUserName = bstrText;
					}
					else if (0 == _tcsicmp(_com_util::ConvertBSTRToString(bstrNodeName), _T("Password"))){
						pUpdateFile->m_sPassword = bstrText;
					}

					::SysFreeString(bstrNodeName);
					::SysFreeString(bstrText);

					pChild->get_nextSibling(&pNext);
					pChild->Release();
					pChild = pNext;
				}
				
				if (pUpdateFile->m_sLocalPath.IsEmpty()) {
					if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("WINDOWSDIR"))) {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szWindowsDir, pUpdateFile->m_sFileName);
					} else if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("SYSTEMDIR"))) {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szSystemDir, pUpdateFile->m_sFileName);
					} else {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s"),m_szMainDir, pUpdateFile->m_sFileName);
					}
				}
				else {
					if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("WINDOWSDIR"))) {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szWindowsDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					} else if (0 == pUpdateFile->m_sInstallDir.CollateNoCase(_T("SYSTEMDIR"))) {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szSystemDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					} else {
						pUpdateFile->m_sFilePath.Format(_T("%s\\%s\\%s"),m_szMainDir, pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);
					}
				}
				
				m_listFile.push_back(pUpdateFile);
				TRACE("新文件对象添加：%s, 对象总数：%u\n", pUpdateFile->m_sFileName, m_listFile.size());
				SAFE_RELEASE(pNode);
			}
		}
		return true;
	}

	return false;
}

bool CUpdateFiles::SaveToXMLFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	bool bRet = false;

	CFile f;
	if (f.Open(pszFileName, CFile::modeCreate | CFile::modeWrite)) {
		f.Write(XML_HEADER, (UINT)strlen(XML_HEADER));

		char szText[512], szDate[256];
		CTime t = CTime::GetCurrentTime();
		sprintf(szDate, "%.4u-%.2u-%.2u %.2u:%.2u:%.2u", t.GetYear(), t.GetMonth(), t.GetDay(),
			t.GetHour(), t.GetMinute(), t.GetSecond());
		sprintf(szText, XML_PRODUCT_HEADER, "Product-Name", szDate, m_listFile.size());
		f.Write(szText, (UINT)strlen(szText));

		char szXMLStr[1024];
		m_listFile.sort();
		list<CUpdateFile*>::iterator it = m_listFile.begin();
		while (it != m_listFile.end()) {
			CUpdateFile* pUpdateFile = *it;
			if (BuildXMLStr(pUpdateFile, szXMLStr)) {
				f.Write(szXMLStr, (UINT)strlen(szXMLStr));
				it ++;
			}
		}

		bRet = true;
		f.Write(XML_PRODUCT_TAIL, (UINT)strlen(XML_PRODUCT_TAIL));
		f.Write(XML_TAIL, (UINT)strlen(XML_TAIL));
		f.Close();
	}

	return bRet;
}

bool CUpdateFiles::AddUpdateFile(TCHAR* pszFileName)
{
	CKevinAutoLock  lock(&m_lock);

	//删除重复
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) {
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile) {
			if (pUpdateFile->m_sFilePath.CompareNoCase(pszFileName) == 0) {
				it = m_listFile.erase(it);
				SAFE_DELETE(pUpdateFile)
				continue;
			}
		}
		it ++;
	}

	CUpdateFile* pUpdateFile = new CUpdateFile(pszFileName);
	if (pUpdateFile) {
		if (pUpdateFile->InitUpdateFile()) {
			m_listFile.push_back(pUpdateFile);
			return true;
		}
		else SAFE_DELETE(pUpdateFile);
	}

	return false;
}


bool CUpdateFiles::BuildXMLStr(CUpdateFile* pUpdateFile, char szXMLStr[1024])
{
	bool bRet = false;

	if (pUpdateFile) {

		pUpdateFile->m_sURL.Format(_T("%s/%s/%s"), m_sCommonURL, 
			pUpdateFile->m_sLocalPath, pUpdateFile->m_sFileName);

		USES_CONVERSION;
		sprintf(
			szXMLStr, 

			"\t\t<File>\r\n"\
			"\t\t\t<FileName>%s</FileName>\r\n"\
			"\t\t\t<FileVersion>%s</FileVersion>\r\n"\
			"\t\t\t<ProductVersion>%s</ProductVersion>\r\n"\
			"\t\t\t<Length>%u</Length>\r\n"\
			"\t\t\t<FileDateLow>%u</FileDateLow>\r\n"\
			"\t\t\t<FileDateHigh>%u</FileDateHigh>\r\n"\
			"\t\t\t<InstallDir>%s</InstallDir>\r\n"\
			"\t\t\t<Register>%s</Register>\r\n"\
			"\t\t\t<ZipFile>%s</ZipFile>\r\n"\
			"\t\t\t<ForceOverwrite>%s</ForceOverwrite>\r\n"\
			"\t\t\t<Execute>%s</Execute>\r\n"\
			"\t\t\t<ExecutePar>%s</ExecutePar>\r\n"\
			"\t\t\t<LocalPath>%s</LocalPath>\r\n"\
			"\t\t\t<URL>%s</URL>\r\n"\
			"\t\t\t<UserName>%s</UserName>\r\n"\
			"\t\t\t<Password>%s</Password>\r\n"\
			"\t\t</File>\r\n",
			pUpdateFile->m_sFileName,
			pUpdateFile->m_sFileVersion,
			pUpdateFile->m_sProductVersion,  
			(DWORD)pUpdateFile->m_ullSize, 
			pUpdateFile->m_fileTime.dwLowDateTime,
			pUpdateFile->m_fileTime.dwHighDateTime,
			pUpdateFile->m_sInstallDir,
			pUpdateFile->m_bRegister ? "REGISTER" : "NO",
			pUpdateFile->m_bZipFile ? "ZIPFILE" : "NO",
			pUpdateFile->m_bForceOverwrite ? "FORCEOVERWRITE" : "NO",
			pUpdateFile->m_bExecute ? "EXECUTE" : "NO",
			pUpdateFile->m_sExecutePar,
			pUpdateFile->m_sLocalPath,
			pUpdateFile->m_sURL,
			pUpdateFile->m_sUserName,
			pUpdateFile->m_sPassword
			);
		bRet = true;
	}

	return bRet;
}

//void CUpdateFiles::FillListCtrl(CListCtrl& m_lstCtrl)
//{
//	CKevinAutoLock  lock(&m_lock);
//
//	list<CUpdateFile*>::iterator it = m_listFile.begin();
//	int iCount = 0; 
//	TCHAR szText[16];
//	while (it != m_listFile.end()) {
//		CUpdateFile* pUpdateFile = *it;
//		if (pUpdateFile) {
//			iCount ++;
//			_stprintf(szText, _T("%u"), iCount);
//			int iItem = m_lstCtrl.InsertItem(
//				LVIF_TEXT|LVIF_STATE, 0, szText, 
//				LVIS_SELECTED, LVIS_SELECTED,
//				0, 0);
//			if (iItem != -1) {
//				m_lstCtrl.SetItemText(iItem, 1, pUpdateFile->m_sFileVersion);
//				_stprintf(szText, _T("%u"), (DWORD)pUpdateFile->m_ullSize);
//				m_lstCtrl.SetItemText(iItem, 2, szText);
//				m_lstCtrl.SetItemText(iItem, 3, pUpdateFile->m_sFileName);
//				m_lstCtrl.SetItemText(iItem, 4, pUpdateFile->m_sFilePath);
//				m_lstCtrl.SetItemData(iItem, (DWORD_PTR)pUpdateFile);
//			}
//		}
//		it ++;
//	}
//}

//void CUpdateFiles::DeleteItem(CUpdateFile * pUpdateFile)
//{
//	CKevinAutoLock  lock(&m_lock);
//	m_listFile.remove(pUpdateFile);
//	SAFE_DELETE(pUpdateFile);
//}
//
CUpdateFile* CUpdateFiles::PopUpdateFile(void)
{
	CKevinAutoLock  lock(&m_lock);
	CUpdateFile* pUpdateFile = NULL;
	if (m_listFile.size() > 0) {
		pUpdateFile = m_listFile.front();
		m_listFile.pop_front();
	}
	return pUpdateFile;
}

void CUpdateFiles::PushUpdateFile(CUpdateFile* pUpdateFile)
{
	CKevinAutoLock  lock(&m_lock);
	m_listFile.push_back(pUpdateFile);
}

DWORD CUpdateFiles::CheckNeedUpdate(void)
{
	CKevinAutoLock  lock(&m_lock);
	DWORD dwUpdateCount = 0;
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) {
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile) {
			if (pUpdateFile->CheckNeedUpdate())
				dwUpdateCount ++;
			else {		// delete it
				it = m_listFile.erase(it);
				SAFE_DELETE(pUpdateFile);
				continue;
			}
		}
		it ++;
	}

	return dwUpdateCount;
}

// 调用此函数前请先调用 CheckNeedUpdate() 以初始化 pUpdateFile->m_bLocalNeedUpdate
DWORD CUpdateFiles::GetNeedUpdateTotalLength(void)
{
	CKevinAutoLock  lock(&m_lock);
	DWORD dwSize = 0;
	list<CUpdateFile*>::iterator it = m_listFile.begin();
	while (it != m_listFile.end()) {
		CUpdateFile* pUpdateFile = *it;
		if (pUpdateFile && pUpdateFile->m_bLocalNeedUpdate) {
			dwSize += (DWORD)pUpdateFile->m_ullSize;
		}
		it ++;
	}

	return dwSize;
}

DWORD CUpdateFiles::Count(void)
{
	CKevinAutoLock  lock(&m_lock);

	return (DWORD)m_listFile.size();
}