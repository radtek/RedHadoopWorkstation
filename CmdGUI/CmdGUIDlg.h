// CmdGUIDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "dialoghadoop.h"
#include "dialoghive.h"
#include "dialogspark.h"
#include <stdlib.h>
#include <windows.h>//获取环境变量
#include <stdio.h>
#include "updatefiles.h"
#include ".\\update\\inetfiles.h"
#include "SeasBaseVideoDlg.h"
#include "VehicleInquireDlg.h"
#define PUBLIC_TIMER 1000
#define WM_NOTIFYICON WM_USER+1

// CCmdGUIDlg 对话框
class CCmdGUIDlg : public CDialog
{
// 构造
public:
	CCmdGUIDlg(CWnd* pParent = NULL);	// 标准构造函数
    virtual ~CCmdGUIDlg();
// 对话框数据
	enum { IDD = IDD_CMDGUI_DIALOG };//
    
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam); 
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedBcmd();
	afx_msg void OnBnClickedBbrowsehdfs();
	afx_msg void OnBnClickedBbrowseyarn();
	afx_msg void OnHadoopenvcmd();
	afx_msg void OnCoresite();
	afx_msg void OnHdfssite();
	afx_msg void OnYarnsite();
	afx_msg void OnMapredsite();
	afx_msg void OnHdfsnamenodeformat();
	afx_msg void OnHivesite();
	afx_msg void OnHosts();
	afx_msg void OnPath();
	CTabCtrl m_tab;
	CDialogHadoop m_paraHadoop;
	CDialogHive m_paraHive;
	CDialogSpark m_paraSpark;
	//CSeasbaseVideoDlg m_paraSeasbase;
	
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnJavaHome();
	afx_msg void OnHadoopHome();

	CEdit m_editPath;
	afx_msg void OnHiveHome();
	afx_msg void OnBnClickedBclear();
	
	afx_msg void OnCmd();
	afx_msg void OnBrowsehdfs();
	afx_msg void OnBrowseyarn();
	afx_msg void OnAbout();
	afx_msg void OnExit();
	int nItem;

private:
	CString m_strConfPath;
public:
	afx_msg void OnUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void checkUpdate();

	CUpdateFiles m_RemoteFiles;
	CInetFiles m_InetFiles;
	CKevinCritSec m_lock;
	DWORD m_dwCount;
	UINT m_nMaxDownThread;
	UINT m_nCurThreadCount;
	NOTIFYICONDATA nld;
	afx_msg void OnHelp();
	afx_msg void OnNnserviceinstall();
	afx_msg void OnUpdateNnserviceinstall(CCmdUI *pCmdUI);
	afx_msg void OnExpWc();
	afx_msg void OnRmserviceinstall();
	afx_msg void OnUpdateRmserviceinstall(CCmdUI *pCmdUI);
	afx_msg void OnExpPi();

	LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);

	afx_msg void OnShow();
	afx_msg void OnHide();
	afx_msg void OnHdfsexitsafemode();
	afx_msg void OnBnClickedBeclipse();
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSeasbaseInquire();
	afx_msg void OnSeasbaseLoadvideo();
	afx_msg void OnSeasbaseHdfsexplorer();
	afx_msg void OnSparkmaster();
	afx_msg void OnSparkslaves();
	afx_msg void OnBnClickedBbrowsespark();
	afx_msg void OnExpSpi();
	CString videoPath;
	afx_msg void OnBnClickedSeasbaseInquire();
	afx_msg void OnBnClickedVehicle();

	void* pInstance;
	VehicleInfo vehinfo[MAX_VEHICLE_NUM];
	HMODULE dlh;
	//外部声明
	typedef void (*ThreadInit)();
	typedef void* (*VehicleRecInit)(char*,int&);
	typedef void (*VehicleRecRelease)(void*);
	typedef void* (*VehicleTypeRec)(void* ,V_Image* ,ITS_Vehicle_Result* );
};
