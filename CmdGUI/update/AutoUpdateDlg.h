#pragma once
#include "afxcmn.h"
#include "Resource.h"
#include "afxwin.h"
#include ".\servers.h"
#include "updatefiles.h"
#include ".\inetfiles.h"
#define PUBLIC_TIMER 1000

// CAutoUpdateDlg 对话框

class CAutoUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoUpdateDlg)

public:
	CAutoUpdateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoUpdateDlg();

// 对话框数据
	enum { IDD = IDD_DIGUPDATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_prsTotal;
	CProgressCtrl m_prsCur;
	CListBox m_lstLog;
	CEdit m_servername;
	bool m_bTray;
	NOTIFYICONDATA	m_pTray;

};
