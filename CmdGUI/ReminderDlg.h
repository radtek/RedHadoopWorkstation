#pragma once

#include "Resource.h"
// CReminderDlg �Ի���

class CReminderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReminderDlg)

public:
	CReminderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReminderDlg();

// �Ի�������
	enum { IDD = IDD_REMINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
