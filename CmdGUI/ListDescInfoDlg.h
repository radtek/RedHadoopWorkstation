#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
// CListDescInfoDlg �Ի���

class CListDescInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDescInfoDlg)

public:
	CListDescInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListDescInfoDlg();

// �Ի�������
	enum { IDD = IDD_DESCINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//CListCtrl m_listdescinfo;
	CListBox m_listmessage;
	
};
