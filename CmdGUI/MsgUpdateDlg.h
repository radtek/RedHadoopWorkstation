#pragma once

#include "Resource.h"
// CMsgUpdateDlg �Ի���

class CMsgUpdateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMsgUpdateDlg)

public:
	CMsgUpdateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgUpdateDlg();

// �Ի�������
	enum { IDD = IDD_MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUpdate();
};
