#pragma once
#include "afxwin.h"
#include "resource.h"

// CDialogSpark �Ի���

class CDialogSpark : public CDialog
{
	DECLARE_DYNAMIC(CDialogSpark)

public:
	CDialogSpark(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogSpark();

// �Ի�������
	enum { IDD = IDD_DIALOGSPARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBstartmaster();
	afx_msg void OnBnClickedBstarthiveserver();
	afx_msg void OnBnClickedBsttartshell();
};
