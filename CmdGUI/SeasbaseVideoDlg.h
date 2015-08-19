#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ListDescInfoDlg.h"
#include "ListViewInfoDlg.h"
#include "ListImagesDlg.h"


//#include "easysize.h"
// CSeasbaseVideoDlg 对话框

class CSeasbaseVideoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSeasbaseVideoDlg)
	//DECLARE_EASYSIZE

public:
	CSeasbaseVideoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSeasbaseVideoDlg();

// 对话框数据
	enum { IDD = IDD_SEASBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetControlInfo(WORD CtrlId);
	int SaveData(char *buff,int len,int* fno);
	void HistogramProcess(IplImage* tagImg);
	BOOL HiveRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBinquire();
	afx_msg void OnBnClickedBload();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBloadvideo();
	afx_msg void OnBnClickedLoadimage();
	afx_msg void OnStnClickedImage();
	CTreeCtrl m_treeview;
	HTREEITEM hRoot ;
	int ColIndex;
	CTabCtrl m_subtab;
	CListViewInfoDlg m_listviewinfo;
	CListDescInfoDlg m_listdescinfo;
	CListImagesDlg  m_listimages;
	int nCnt;
	CRect m_rect;//用于保存对话框大小变化前的大小
	int   m_old_cx,m_old_cy; 
	CDWordArray   m_control_info; 
	CImageList img;
    int framenum ;
	ULONG_PTR m_gdiplusToken;
	CString tablename;
	CString strVideoPath;
	int frameID;
	int tagID;
	int imgWidthID;
	int imgHeightID;
	CString sEditText;

	typedef struct Measure
	{
		CvPoint p_c;
		CvRect box;
		CvPoint ppt;
		IplImage* frame;
	}mesure;
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
		Hist nH[6];
		int nm,ns,nt;

	}targetColor;
	targetColor  tColor;
 bool bSelRect;
 IplImage* pImg;
 IplImage* tagImg;
 CvRect roi_rect_src;
 CPoint position;
 int iIsTwice;
 CPoint pos;
 CString sText;
 CRect windowRect;
 IplImage* subTarImg;
 afx_msg void OnLoadLoadvideo();
 afx_msg void OnLoadLoadtables();
 afx_msg void OnLoadLoadimage();
 afx_msg void OnHiveCmd();
 afx_msg void OnHelpHelp();

 afx_msg void OnBnClickedBrowerdata();
 afx_msg void OnLoadExecute();
};
