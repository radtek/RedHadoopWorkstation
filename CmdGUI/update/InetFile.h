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

// 每个实例仅开启一个 GET 线程
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
	int m_nDownloadTime;	// 下载次数
	UINT m_nAlreadyDown;
	DWORD m_dwTotalLen;

	bool	m_bRegister;		// 是否注册，如果是组件，要注册
	bool	m_bZipFile;			// 是否为ZIP文件，如果是，要解压 (目前保留)
	bool	m_bForceOverwrite;	// 是否覆盖 如果是新版本，无条件覆盖；如果此值为真，无条件覆盖 (目前保留)
	bool	m_bExecute;			// 是否执行
	CString m_sExecutePar;		// 如果执行，执行参数
};
