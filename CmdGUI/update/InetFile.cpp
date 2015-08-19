#include "StdAfx.h"
#include ".\inetfile.h"

CInetFile::CInetFile(void)
: sLastErr(_T(""))
, m_bProgressMode(false)
, m_sURL(_T(""))
, m_sPath(_T(""))
, m_sServerName(_T(""))
, m_sFileName(_T(""))
, m_hWnd(NULL)
, m_bWantStop(false)
, m_dlFileType(GENERAL_FILE)
, m_nDownloadTime(0)
, m_nAlreadyDown(0)
, m_dwTotalLen(0)
{
	m_bRegister = false;		// �Ƿ�ע�ᣬ����������Ҫע��
	m_bZipFile = false;			// �Ƿ�ΪZIP�ļ�������ǣ�Ҫ��ѹ
	m_bForceOverwrite = false;	// �Ƿ񸲸� ������°汾�����������ǣ������ֵΪ�棬����������
	m_bExecute = false;			// �Ƿ�ִ��
	m_sExecutePar = _T("");		// ���ִ�У�ִ�в���

	sLastErr = _T("���ӷ�����ʧ��, ��ѡ���������������г���");
}

CInetFile::~CInetFile(void)
{
	m_bWantStop = true;
	WaitForStop();
}

bool CInetFile::GetInetFile()
{
	m_nDownloadTime ++;
	WaitForStop();
	m_bWantStop = false;
	return StartThread();
}

CString CInetFile::GetLastError(void)
{
	return sLastErr;
}

void CInetFile::ThrowInetFileException(int nCode)
{
	switch(nCode) {
		case 1:
			sLastErr = _T("URL��������, ��ѡ���������������г���");
			break;
		case 2:
			sLastErr = _T("�������ܾ�����, ��ѡ���������������г���");
			break;
		case 3:
			sLastErr = _T("�ض���ʧ��, ��ѡ���������������г���");
			break;
		case 4:
			sLastErr = _T("�ض���URL��������, ��ѡ���������������г���");
			break;
		case 5:
			sLastErr = _T("����HTTP����, ��ѡ���������������г���");
			break;
		case 6:
			sLastErr = _T("ȡ�÷������ļ�ʧ��, ��ѡ���������������г���");
			break;
		case 7:
			sLastErr = _T("���������ļ�ʧ��");
			break;			
		case 8:
			sLastErr = _T("����ȡ������");
			break;	
		case 9:
			sLastErr = _T("URL����ȷ");
			break;
		default:
			sLastErr = _T("���ӷ�����ʧ��, ��ѡ���������������г���");
			break;
	}
	
	CInetFileException* pEx = new CInetFileException(nCode);
	throw pEx;
}

void CInetFile::ThreadProcMain(void)
{
	//ASSERT(!m_sURL.IsEmpty());
	ASSERT(!m_sPath.IsEmpty());

	int nRetCode = 0;

	CInetFileSession session(_T("AutoUpdate - MFC App"), PRE_CONFIG_INTERNET_ACCESS);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;
		const TCHAR szHeaders[] = _T("Accept: */*\r\nUser-Agent: AutoUpdate_Kevin\r\n");
		DWORD dwHttpRequestFlags = INTERNET_FLAG_RELOAD;

		//if (m_sURL.IsEmpty())
		//	ThrowInetFileException(9);

		if (!AfxParseURL(m_sURL, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			ThrowInetFileException(1);
		}

		if (m_bProgressMode)
		{
			VERIFY(session.EnableStatusCallback(true));
		}

		pServer = session.GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);

		if (dwRet != HTTP_STATUS_OK)
		{
			ThrowInetFileException(6);
		}

		CString strNewLocation;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

		CString sTotalLen;
		pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH , sTotalLen);
		USES_CONVERSION;
		DWORD dwTotalLen = (DWORD)atoi((sTotalLen));

		CFile fLocal;
		CString sUpdateFileName = m_sPath;
		if (GENERAL_FILE == m_dlFileType) {
			sUpdateFileName += _T(".update");
		}

		if (!fLocal.Open(sUpdateFileName, CFile::modeCreate | CFile::modeWrite)) {
			ThrowInetFileException(7);
		}

		BYTE szBuf[1024];
		UINT nLen = 0;
		m_nAlreadyDown = 0;

		while ( nLen = pFile->Read(szBuf, sizeof(szBuf)) )
		{
			fLocal.Write(szBuf, nLen);
			m_nAlreadyDown += nLen;
			if (m_hWnd) 
				PostMessage(m_hWnd, WM_DOWNLOADFILE, MAKEWPARAM(nLen, m_dlFileType), (LPARAM)this);

			if (m_bWantStop) {
				nRetCode = 8;
				break;
			}
		}

		fLocal.Close();
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		nRetCode = 2;
		pEx->Delete();
	}
	catch (CInetFileException* pEx)
	{
		nRetCode = pEx->m_nErrorCode;
		TRACE("Error: Exiting with CInetFileException(%d)\n", nRetCode);
		pEx->Delete();
	}

	if (pFile != NULL) {
		pFile->Close();
		delete pFile;
	}

	if (pServer != NULL) {
		pServer->Close();
		delete pServer;
	}

	session.Close();

	if (m_hWnd) 
		PostMessage(m_hWnd, WM_DOWNLOADFILEEND, MAKEWPARAM(nRetCode, m_dlFileType), (LPARAM)this);
}

