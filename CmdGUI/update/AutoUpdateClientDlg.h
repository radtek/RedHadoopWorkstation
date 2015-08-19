// AutoUpdateClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "..\resource.h"


#include "updatefiles.h"
#include ".\inetfiles.h"

#define PUBLIC_TIMER 1000

// CAutoUpdateClientDlg 对话框
class CAutoUpdateClientDlg : public CDialog
{
// 构造
public:
	CAutoUpdateClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DIGUPDATE };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDownLoadFile(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
private:
	//CButton m_btnAutoSelServer;
	CButton m_btnNowUpdate;
	CProgressCtrl m_prsCur;
	CProgressCtrl m_prsTotal;

	CComboBox m_cmbSelServer;

	DWORD m_dwCount;
	DWORD m_dwTotalLen;
	DWORD m_dwAlreadyDownLen;

	CKevinCritSec m_lock;

	//CServers m_Servers;
	CUpdateFiles m_RemoteFiles;
	CInetFiles m_InetFiles;
	CListBox m_lstLog;
	UINT m_nMaxDownThread;
	UINT m_nCurThreadCount;
	bool m_bTray;
	NOTIFYICONDATA	m_pTray;

	void RenameAndExeFile(CInetFile* pInetFile);
	void AddDownServerListQueue(CString sServerListURL);

public:
	void RefreshCtrl(void);
	afx_msg void OnBnClickedBtnNowupdate();
	//afx_msg void OnBnClickedBtnAutoselserver();
	afx_msg void OnTimer(UINT nIDEvent);
	void AddLog(CString sLog, int nErrCode = 0);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL DestroyWindow();
	CString m_sServerListURL;
};
