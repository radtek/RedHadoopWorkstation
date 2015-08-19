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

	bool	m_bRegister;		// �Ƿ�ע�ᣬ����������Ҫע��
	CString m_sInstallDir;		// WINDOWSDIR SYSTEMDIR CURRENTDIR ��װĿ¼
	bool	m_bZipFile;			// �Ƿ�ΪZIP�ļ�������ǣ�Ҫ��ѹ
	bool	m_bForceOverwrite;	// �Ƿ񸲸� ������°汾�����������ǣ������ֵΪ�棬����������
	bool	m_bExecute;			// �Ƿ�ִ��
	CString m_sExecutePar;		// ���ִ�У�ִ�в���
	CString m_sLocalPath;		// ��װ�����·��
	FILETIME m_fileTime;		// �ļ�����
	CString m_sFilePath;		// �����ļ�����·��
	CString m_sURL;				// ������URL��ַ��HTTP���ļ��������� m_sFileName ��ͬ
	CString m_sUserName;		// ����ṩ��¼����¼�û���
	CString m_sPassword;		// ����ṩ��¼����¼����

	// �������ڿͻ��˵ĸ��£������ش���
	bool CheckNeedUpdate();
	bool m_bLocalNeedUpdate;
};
