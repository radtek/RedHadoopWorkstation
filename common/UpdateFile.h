#pragma once

#pragma comment(lib,"version.lib")

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

class CUpdateFile
{
public:
	CUpdateFile(TCHAR* pszFilePath);
	virtual ~CUpdateFile(void);
	bool InitUpdateFile();
	//   int newer = WhichIsNewer(argv[1],argv[2]);
	//   switch(newer) {
	//      case 1:
	//      case 2: printf("%s is newer\n",argv[newer]); break;
	//      case 3: printf("they are the same version\n"); break;
	//      case 0:
	//      default: printf("there was an error\n"); break;
	//   }
	int WhichIsNewer (TCHAR *fname1, TCHAR *fname2);

private:
	void EmitErrorMsg (HRESULT hr);
	HRESULT GetFileVersion (TCHAR *szFileName, VS_FIXEDFILEINFO *vsf);
	HRESULT GetFileDate (TCHAR *szFileName, FILETIME *pft);
	HRESULT LastError();
	//bool GetVersion(TCHAR* pszFileName);
	VS_FIXEDFILEINFO m_vfixedFileInfo;
	bool FileExist(CString sFileName);

public:
	ULONGLONG m_ullSize;
	CString m_sFileName;
	CString m_sFileVersion;
	CString m_sProductVersion;

	bool	m_bRegister;		// 是否注册，如果是组件，要注册
	CString m_sInstallDir;		// WINDOWSDIR SYSTEMDIR CURRENTDIR 安装目录
	bool	m_bZipFile;			// 是否为ZIP文件，如果是，要解压
	bool	m_bForceOverwrite;	// 是否覆盖 如果是新版本，无条件覆盖；如果此值为真，无条件覆盖
	bool	m_bExecute;			// 是否执行
	CString m_sExecutePar;		// 如果执行，执行参数
	CString m_sLocalPath;		// 安装的相对路径
	FILETIME m_fileTime;		// 文件日期
	CString m_sFilePath;		// 本地文件绝对路径
	CString m_sURL;				// 供下载URL地址，HTTP，文件名可以与 m_sFileName 不同
	CString m_sUserName;		// 如果提供登录，登录用户名
	CString m_sPassword;		// 如果提供登录，登录密码

	// 仅适用于客户端的更新，或重载此类
	bool CheckNeedUpdate();
	bool m_bLocalNeedUpdate;
};
