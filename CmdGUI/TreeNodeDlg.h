#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTreeNodeDlg �Ի���

class CTreeNodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTreeNodeDlg)

public:
	CTreeNodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTreeNodeDlg();

// �Ի�������
	enum { IDD = IDD_DLGTREENODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString node_IP;
	CString node_Name;
 
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl m_ipaddress;
};
