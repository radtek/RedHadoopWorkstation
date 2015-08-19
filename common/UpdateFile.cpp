#include "StdAfx.h"
#include ".\updatefile.h"

CUpdateFile::CUpdateFile(TCHAR* pszFilePath)
: m_ullSize(0)
, m_bRegister(false)
, m_bZipFile(false)
, m_bForceOverwrite(false)
, m_bExecute(false)
, m_bLocalNeedUpdate(false)
, m_sURL(_T(""))
, m_sUserName(_T(""))
, m_sPassword(_T(""))
{
	m_sFilePath = pszFilePath;
	m_sInstallDir = _T("CURRENTDIR");
	m_sLocalPath = _T("");
	ZeroMemory(&m_fileTime, sizeof(FILETIME));
	ZeroMemory(&m_vfixedFileInfo, sizeof(m_vfixedFileInfo));
}

CUpdateFile::~CUpdateFile(void)
{
}

bool CUpdateFile::InitUpdateFile()
{
	if (m_sFilePath.IsEmpty())
		return false;

	bool bRet = false;
	CFile f;
	if (f.Open(m_sFilePath, CFile::modeRead | CFile::shareDenyNone)) {
		m_ullSize = f.GetLength();
		m_sFileName = f.GetFileName();
		f.Close();
		if ( SUCCEEDED( GetFileVersion((TCHAR*)(LPCTSTR)m_sFilePath, &m_vfixedFileInfo)) ) {
			bRet = true;
		}
	}

	if ( SUCCEEDED(GetFileDate((TCHAR*)(LPCTSTR)m_sFilePath, &m_fileTime)) ) {
		bRet = true;
	}

	return bRet;
}

//void printtime (FILETIME *t) {
//
//   FILETIME lft;
//   FILETIME *ft = &lft;
//   FileTimeToLocalFileTime(t,ft);
//   printf("%08x %08x",ft->dwHighDateTime,ft->dwLowDateTime); {
//      SYSTEMTIME stCreate;
//      BOOL bret = FileTimeToSystemTime(ft,&stCreate);
//      printf("    %02d/%02d/%d  %02d:%02d:%02d\n",
//      stCreate.wMonth, stCreate.wDay, stCreate.wYear,
//      stCreate.wHour, stCreate.wMinute, stCreate.wSecond);
//   }
//}
//
//int main(int argc, TCHAR* argv[]) {
//    printf("usage: checkversion file1 file2\n"
//      "\tReports which file is newer, first by checking the file version in "
//      "\tthe version resource, then by checking the date\n\n" );
//
//   if (argc != 3) 
//      return 1;
//
//   int newer = WhichIsNewer(argv[1],argv[2]);
//   switch(newer) {
//      case 1:
//      case 2: printf("%s is newer\n",argv[newer]); break;
//      case 3: printf("they are the same version\n"); break;
//      case 0:
//      default: printf("there was an error\n"); break;
//   }
//
//   return !newer;
//}

int  CUpdateFile::WhichIsNewer (TCHAR *fname1, TCHAR *fname2) {
   // 1 if argv[1] is newer
   // 2 if argv[2] is newer
   // 3 if they are the same version
	// 0 if there is an error

	int ndxNewerFile;
	HRESULT ret;
	VS_FIXEDFILEINFO vsf1,vsf2;

	if ( SUCCEEDED((ret=GetFileVersion(fname1,&vsf1))) && SUCCEEDED((ret=GetFileVersion(fname2,&vsf2)))) {
		// both files have a file version resource
		// compare by file version resource
		if (vsf1.dwFileVersionMS > vsf2.dwFileVersionMS) {
			ndxNewerFile = 1;
		}
		else 
			if (vsf1.dwFileVersionMS < vsf2.dwFileVersionMS) {
				ndxNewerFile = 2;
			}
			else {   // if (vsf1.dwFileVersionMS == vsf2.dwFileVersionMS)
				if (vsf1.dwFileVersionLS > vsf2.dwFileVersionLS) {
					ndxNewerFile = 1;
				}
				else if (vsf1.dwFileVersionLS < vsf2.dwFileVersionLS) {
					ndxNewerFile = 2;
				}
				else {   // if (vsf1.dwFileVersionLS == vsf2.dwFileVersionLS)
					ndxNewerFile = 3;
				}
			}
	}

	else {
		// compare by date
		FILETIME ft1,ft2;
		if (SUCCEEDED((ret=GetFileDate(fname1,&ft1))) && SUCCEEDED((ret=GetFileDate(fname2,&ft2))))
		{
			LONG x = CompareFileTime(&ft1,&ft2);
			if (x == -1) 
				ndxNewerFile = 2;
			else 
				if (x == 0) 
					ndxNewerFile = 3;
				else 
					if (x == 1) ndxNewerFile = 1;
					else {
						EmitErrorMsg(E_FAIL);
						return 0;
					}
		}
		else {
			EmitErrorMsg(ret);
			return 0;
		}
	}
	return ndxNewerFile;
}

HRESULT  CUpdateFile::GetFileDate (TCHAR *szFileName, FILETIME *pft) {
	// we are interested only in the create time
	// this is the equiv of "modified time" in the 
	// Windows Explorer properties dialog
	FILETIME ct,lat;
	HANDLE hFile = CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
	if (hFile == INVALID_HANDLE_VALUE) 
		return LastError();
	BOOL bret = ::GetFileTime(hFile,&ct,&lat,pft);
	if (bret == 0) {
		::CloseHandle(hFile);
		return LastError();
	}

	::CloseHandle(hFile);
	return S_OK;
}

// This function gets the file version info structure
HRESULT  CUpdateFile::GetFileVersion (TCHAR *szFileName, VS_FIXEDFILEINFO *pvsf) {
	DWORD dwHandle;
	DWORD cchver = GetFileVersionInfoSize(szFileName,&dwHandle);
	if (cchver == 0) 
		return LastError();
	TCHAR* pver = new TCHAR[cchver];
	BOOL bret = GetFileVersionInfo(szFileName,dwHandle,cchver,pver);
	if (!bret) 
		return LastError();
	UINT uLen;
	void *pbuf;
	bret = VerQueryValue(pver,_T("\\"),&pbuf,&uLen);
	if (!bret) 
		return LastError();
	memcpy(pvsf,pbuf,sizeof(VS_FIXEDFILEINFO));
	delete[] pver;

	m_sFileVersion.Format(_T("%u.%u.%u.%u"), 
		HIWORD(m_vfixedFileInfo.dwFileVersionMS), 
		LOWORD(m_vfixedFileInfo.dwFileVersionMS), 
		HIWORD(m_vfixedFileInfo.dwFileVersionLS), 
		LOWORD(m_vfixedFileInfo.dwFileVersionLS)
		);

	m_sProductVersion.Format(_T("%u.%u.%u.%u"), 
		HIWORD(m_vfixedFileInfo.dwProductVersionMS), 
		LOWORD(m_vfixedFileInfo.dwProductVersionMS), 
		HIWORD(m_vfixedFileInfo.dwProductVersionLS), 
		LOWORD(m_vfixedFileInfo.dwProductVersionLS)
		);

	return S_OK;
}

HRESULT  CUpdateFile::LastError () {
	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
	if (SUCCEEDED(hr)) 
		return E_FAIL;
	return hr;
}

// This little function emits an error message based on WIN32 error messages
void  CUpdateFile::EmitErrorMsg (HRESULT hr) {
	TCHAR szMsg[1024];
	FormatMessage( 
		FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		szMsg,
		1024,
		NULL 
		);

	printf("%s\n",szMsg);
}
//
//bool CUpdateFile::GetVersion(TCHAR* pszFileName)
//{
//	DWORD dwHandle,InfoSize;
//	//首先获得版本信息资源的长度
//	InfoSize = GetFileVersionInfoSize(pszFileName,&dwHandle);
//	//将版本信息资源读入缓冲区
//	if(InfoSize==0) return false;
//	char *InfoBuf = new char[InfoSize];
//	GetFileVersionInfo(pszFileName,0,InfoSize,InfoBuf);
//	//获得生成文件使用的代码页及文件版本
//	unsigned int  cbTranslate = 0;
//	struct LANGANDCODEPAGE {
//		WORD wLanguage;
//		WORD wCodePage;
//	} *lpTranslate;
//	VerQueryValue(InfoBuf, TEXT("\\VarFileInfo\\Translation"),(LPVOID*)&lpTranslate,&cbTranslate);
//	// Read the file description for each language and code page.
//	for( UINT i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
//	{
//		TCHAR  SubBlock[200];
//		wsprintf( SubBlock, 
//			TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
//			lpTranslate[i].wLanguage,
//			lpTranslate[i].wCodePage);
//		void *lpBuffer=NULL;
//		unsigned int dwBytes=0;
//		VerQueryValue(InfoBuf, 
//			SubBlock, 
//			&lpBuffer, 
//			&dwBytes); 
//		m_sFileVersion = (TCHAR *)lpBuffer;
//		
//		wsprintf( SubBlock, 
//			TEXT("\\StringFileInfo\\%04x%04x\\ProductVersion"),
//			lpTranslate[i].wLanguage,
//			lpTranslate[i].wCodePage);
//		lpBuffer=NULL;
//		dwBytes=0;
//		VerQueryValue(InfoBuf, 
//			SubBlock, 
//			&lpBuffer, 
//			&dwBytes); 
//		m_sProductVersion = (TCHAR *)lpBuffer;
//	}
//
//	if (InfoBuf) delete[] InfoBuf; InfoBuf = NULL;
//
//	return true;
//}

bool CUpdateFile::CheckNeedUpdate()
{
	ASSERT(!m_sFilePath.IsEmpty());
	if (FileExist(m_sFilePath)) {
		CUpdateFile* plocalUpdateFile = new CUpdateFile((LPTSTR)(LPCTSTR)m_sFilePath);
		if (plocalUpdateFile) {
			if (plocalUpdateFile->InitUpdateFile()) {
				//–1 First file time is earlier than second file time. 
				//0 First file time is equal to second file time. 
				//1 First file time is later than second file time. 
				// 没有版本比较时间
				if (m_sFileVersion.IsEmpty()) {
					if( -1 == CompareFileTime(&plocalUpdateFile->m_fileTime, &m_fileTime))
						m_bLocalNeedUpdate = true;
				}
				else {
					if (plocalUpdateFile->m_sFileVersion.CompareNoCase(m_sFileVersion) < 0)
						m_bLocalNeedUpdate = true;
				}

				//if (plocalUpdateFile->m_ullSize != m_ullSize)
				//	m_bLocalNeedUpdate = true;

			}
			delete plocalUpdateFile;
			plocalUpdateFile = NULL;
		}
	}
	else {
		m_bLocalNeedUpdate = true;
	}

	return m_bLocalNeedUpdate;
}

bool CUpdateFile::FileExist(CString sFileName)
{
	HANDLE hFile = CreateFile(sFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	::CloseHandle(hFile);

	return true;
}