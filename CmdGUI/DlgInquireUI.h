#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcolorbutton.h"
#include "afxdtctl.h"
// CDlgInquireUI �Ի���

class CDlgInquireUI : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInquireUI)

public:
	CDlgInquireUI(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInquireUI();

// �Ի�������
	enum { IDD = IDD_INQUIREUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownCombo1();
	afx_msg void OnBnClickedLoadimage();
	virtual BOOL OnInitDialog();

	afx_msg void OnStnClickedImage();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnPaint();
	CPoint m_StartPoint;//��ʼ��   
	CPoint m_EndPoint;//������   
	BOOL m_bLBDown;//����Ƿ���
	CPoint position;
	int iIsTwice;
	CPoint pos;
	CString sText;
	//CString sInfo;
	typedef struct Measure
	{
		CvPoint p_c;
		CvRect box;
		CvPoint ppt;
		IplImage* frame;
	}mesure;
	//void ResizeImage(CImage img);
	void ProcImage(CImage img);
	void initImage(CImage myImage);
	void HistogramProcess(IplImage* tagImg);
	IplImage* img;
	IplImage* tagImg;
	CvRect roi_rect_src;


	/*CDateTimeCtrl m_ctlBeginDate;
	CDateTimeCtrl m_ctlEndDate;
	CDateTimeCtrl m_ctlBeginTime;
	CDateTimeCtrl m_ctlEndTime;*/
	Measure meas;
	typedef struct Group
	{
		float h;
		CvScalar bgr;
		int nC;
		int value;
	}histGroup;
	typedef struct Hist
	{
		CvScalar hsv;
		CvScalar bgr;
		int nC;
		int value;
	}histG;
	Hist histinfo[128];
	typedef struct _targetColor
	{
		/*Hist nM[1];
		Hist nS[1];	
		Hist nT[1];*/
		Hist nH[6];
		int nm,ns,nt;

	}targetColor;
	targetColor  tColor;
	
};
