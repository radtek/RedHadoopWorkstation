#pragma once
#include "resource.h"
#include "vehicle/ItsVehicleTypeRec.h"
// CDlgVehInquireUI 对话框
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
class CDlgVehInquireUI : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVehInquireUI)

public:
	CDlgVehInquireUI(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVehInquireUI();

// 对话框数据
	enum { IDD = IDD_VEHINQUIREUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLoadimage();
	afx_msg void OnStnClickedImage();
	void VehicleProc(IplImage* src);	
	CPoint position;
	int iIsTwice;
	CPoint pos;
	CString sText;
	IplImage* img;
	IplImage* tagImg;
	CvRect roi_rect_src;
	void* pInstance;
	VehicleInfo vehinfo[MAX_VEHICLE_NUM];
	//外部声明
	typedef void (*ThreadInit)();
	typedef void* (*VehicleRecInit)(char*,int&);
	typedef void (*VehicleRecRelease)(void*);
	typedef void* (*VehicleTypeRec)(void* ,V_Image* ,ITS_Vehicle_Result* );
	afx_msg void OnBnClickedBvehicle();
};
