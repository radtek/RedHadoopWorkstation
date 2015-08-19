#pragma once
#include "afxcmn.h"
#include "Resource.h"
#include "afxwin.h"
#include ".\servers.h"
#include "updatefiles.h"
#include ".\inetfiles.h"
#define PUBLIC_TIMER 1000

// CAutoUpdateDlg �Ի���

class CAutoUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoUpdateDlg)

public:
	CAutoUpdateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAutoUpdateDlg();

// �Ի�������
	enum { IDD = IDD_DIGUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_prsTotal;
	CProgressCtrl m_prsCur;
	CListBox m_lstLog;
	CEdit m_servername;
	bool m_bTray;
	NOTIFYICONDATA	m_pTray;

};
