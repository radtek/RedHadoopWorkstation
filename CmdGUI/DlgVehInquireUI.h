#pragma once
#include "resource.h"
#include "vehicle/ItsVehicleTypeRec.h"
// CDlgVehInquireUI �Ի���
typedef struct
{
	CvPoint pointLT;            //����λ������
	CvPoint pointRB;           //����λ������
	IplImage* plateImg;        //����ͼ��
	IplImage* vehicleImg;        //����ͼ��
	char tempVehicleType[1024]; //��������
	char platenumber[20];         //���ƺ���
	char* eVehicleColor;      //������ɫ
} VehicleInfo;
class CDlgVehInquireUI : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVehInquireUI)

public:
	CDlgVehInquireUI(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVehInquireUI();

// �Ի�������
	enum { IDD = IDD_VEHINQUIREUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	//�ⲿ����
	typedef void (*ThreadInit)();
	typedef void* (*VehicleRecInit)(char*,int&);
	typedef void (*VehicleRecRelease)(void*);
	typedef void* (*VehicleTypeRec)(void* ,V_Image* ,ITS_Vehicle_Result* );
	afx_msg void OnBnClickedBvehicle();
};
