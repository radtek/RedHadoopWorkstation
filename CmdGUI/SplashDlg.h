#pragma once
#include "afxwin.h"


// CSplashDlg �Ի���

class CSplashDlg : public CDialog
{
	DECLARE_DYNAMIC(CSplashDlg)

public:
	CSplashDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSplashDlg();

// �Ի�������
	enum { IDD = IDD_SPLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CStatic m_sinfo;
	CStatic m_pinfo;
};
