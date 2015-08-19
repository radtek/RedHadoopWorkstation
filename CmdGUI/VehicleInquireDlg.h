#pragma once
#include "Resource.h"
#include "ListDescInfoDlg.h"
#include "ListViewInfoDlg.h"
#include "ListImagesDlg.h"
#include "afxcmn.h"
//#include "DlgVehInquireUI.h"
#include "vehicle/ItsVehicleTypeRec.h"
typedef struct
{
	CvPoint pointLT;            //车牌位置左上
	CvPoint pointRB;           //车牌位置右下
	IplImage* plateImg;        //车牌图像
	IplImage* vehicleImg;        //车辆图像
	char tempVehicleType[1024]; //车辆类型
	char platenumber[20];         //车牌号码
	char* eVehicleColor;      //车身颜色
} VehicleInfo;
// CVehicleInquireDlg 对话框

class CVehicleInquireDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVehicleInquireDlg)

public:
	CVehicleInquireDlg(HMODULE  dlh,void *pInstance,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVehicleInquireDlg();

// 对话框数据
	enum { IDD = IDD_VEHICLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL HiveRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds);
	int SaveData(char *buff,int len,int* fno);
	void UTF8toANSI(CString &strUTF8);
	afx_msg void OnTvnSelchangedTreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBloadvideo();
	afx_msg void OnBnClickedBload();
	afx_msg void OnBnClickedBinquire();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLoadvideos();
	afx_msg void OnLoadLoadtables();
	afx_msg void OnLoadLoadimage();
	afx_msg void OnHelp();
	afx_msg void OnHiveCmd();
	CListViewInfoDlg m_listviewinfo;
	CListDescInfoDlg m_listdescinfo;
	CListImagesDlg  m_listimages;
	//CDlgVehInquireUI  m_vehinquireUI;
	ULONG_PTR m_gdiplusToken;
	HTREEITEM hRoot ;
	CImageList img;
	int ColIndex;
	int frameID;
	int tagID;
	int numID;
	int colorID;
	int typeID;
	int nCnt;
	int framenum ;
	CString strVideoPath;
	CTabCtrl m_subtab;
	CTreeCtrl m_treeview;
	//CTabCtrl m_subtab2;
	IplImage* pImg;
	
	afx_msg void OnBnClickedBvehicle();
	afx_msg void OnBnClickedLoadimage();
	CPoint position;
	CPoint pos;
	CString sText;
	IplImage* tagImg;
	CvRect roi_rect_src;
	void* pInstance;
	VehicleInfo vehinfo[MAX_VEHICLE_NUM];
	HMODULE dlh;
	//外部声明
	typedef void (*ThreadInit)();
	typedef void* (*VehicleRecInit)(char*,int&);
	typedef void (*VehicleRecRelease)(void*);
	typedef void* (*VehicleTypeRec)(void* ,V_Image* ,ITS_Vehicle_Result* );

};
