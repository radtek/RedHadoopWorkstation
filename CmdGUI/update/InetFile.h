#pragma once

#include <afxinet.h>
#include "KevinThread.h"
#include "updatefile.h"

#define WM_DOWNLOADFILE			(WM_USER + 100) 
#define WM_DOWNLOADFILEEND		(WM_USER + 101) 
#define WM_TRAY					(WM_USER + 102)

typedef enum DOWNLOAD_FILE_TYPE
{
	SERVERLIST_XML = 0,
	FILELIST_XML,
	GENERAL_FILE
};

// ÿ��ʵ��������һ�� GET �߳�
class CInetFile : 
	public CKevinThread
{
public:
	class CInetFileSession : public CInternetSession
	{
	public:
		CInetFileSession(LPCTSTR pszAppName, int nMethod): CInternetSession(pszAppName, 1, nMethod) { };

		virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
			LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen) 
		{
			if (dwInternetStatus == INTERNET_STATUS_CONNECTED_TO_SERVER)
				return;
		};
	};

	class CInetFileException : public CException
	{
	public:
		CInetFileException(int nCode = 0) : m_nErrorCode(nCode) { };
		~CInetFileException() { }

		int m_nErrorCode;
	};

	CInetFile(void);
	virtual ~CInetFile(void);
	CString GetLastError(void);
	bool GetInetFile();
private:
	CString sLastErr;
	void ThrowInetFileException(int nCode);
	virtual void ThreadProcMain(void);
public:
	bool m_bProgressMode;
	DOWNLOAD_FILE_TYPE m_dlFileType;
	CString m_sPath;
	CString m_sFileName;
	CString m_sURL;
	HWND m_hWnd;
	CString m_sServerName;
	bool m_bWantStop;
	int m_nDownloadTime;	// ���ش���
	UINT m_nAlreadyDown;
	DWORD m_dwTotalLen;

	bool	m_bRegister;		// �Ƿ�ע�ᣬ����������Ҫע��
	bool	m_bZipFile;			// �Ƿ�ΪZIP�ļ�������ǣ�Ҫ��ѹ (Ŀǰ����)
	bool	m_bForceOverwrite;	// �Ƿ񸲸� ������°汾�����������ǣ������ֵΪ�棬���������� (Ŀǰ����)
	bool	m_bExecute;			// �Ƿ�ִ��
	CString m_sExecutePar;		// ���ִ�У�ִ�в���
};
