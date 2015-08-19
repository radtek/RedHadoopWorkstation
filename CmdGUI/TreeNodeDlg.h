#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTreeNodeDlg 对话框

class CTreeNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeNodeDlg)

public:
	CTreeNodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTreeNodeDlg();

// 对话框数据
	enum { IDD = IDD_DLGTREENODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString node_IP;
	CString node_Name;
 
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_ipaddress;
};
