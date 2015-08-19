// DlgVehInquireUI.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgVehInquireUI.h"
#include "afxdialogex.h"


// CDlgVehInquireUI �Ի���

IMPLEMENT_DYNAMIC(CDlgVehInquireUI, CDialogEx)

CDlgVehInquireUI::CDlgVehInquireUI(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVehInquireUI::IDD, pParent)
{
	iIsTwice = 0;
	img = NULL;
	tagImg = NULL;
	memset(vehinfo,0,sizeof(VehicleInfo));
	
}

CDlgVehInquireUI::~CDlgVehInquireUI()
{
	if (img)
	{
		cvReleaseImage(&img);
	}
	if (tagImg)
	{
		cvReleaseImage(&tagImg);
	}
	//ITS_VehicleRecRelease(pInstance); //�ͷ�
}

void CDlgVehInquireUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgVehInquireUI, CDialogEx)
	ON_BN_CLICKED(IDC_LoadImage, &CDlgVehInquireUI::OnBnClickedLoadimage)
	ON_STN_CLICKED(IDC_IMAGE, &CDlgVehInquireUI::OnStnClickedImage)
	ON_BN_CLICKED(IDC_BVehicle, &CDlgVehInquireUI::OnBnClickedBvehicle)
END_MESSAGE_MAP()


// CDlgVehInquireUI ��Ϣ�������


void CDlgVehInquireUI::OnBnClickedLoadimage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, 
		"Image Files (*bmp)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
		_T("Image Files (*.jpg;*.bmp;*.png;*.icon;*.gif)|*.jpg;*.bmp;*.png;*.icon;*.gif||"),
		NULL);
	dlg.m_ofn.lpstrTitle=_T("Please select an image");

	dlg.m_ofn.nMaxFile = MAX_PATH; 
	dlg.m_ofn.lpstrFile = new TCHAR[dlg.m_ofn.nMaxFile];  
	ZeroMemory(dlg.m_ofn.lpstrFile, sizeof(TCHAR) * dlg.m_ofn.nMaxFile); 
	int retval = dlg.DoModal();
	if(retval==IDCANCEL)
		return;
	POSITION pos_file;
	pos_file = dlg.GetStartPosition();
	CArray<CString, CString> ary_filename;
	while(pos_file != NULL)
		ary_filename.Add(dlg.GetNextPathName(pos_file));
	this->SetDlgItemTextA(IDC_ImagePath,ary_filename.GetAt(0));
	//sInfo= ary_filename.GetAt(0);
	CImage myImage;
	myImage.Load(ary_filename.GetAt(0));
	IplImage* copyImg = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);

	copyImg = cvLoadImage(ary_filename.GetAt(0),1);

	//ResizeImage( myImage );    // �Զ����ͼƬ��������

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (�����ڴ���Դ����������ڲ�, �������ⲿ, ����ͨ�����CMainFrame��ָ��, ��ͨ��pMianFrame->GetDlgItem(IDCk_MY_PIC)����ȡ)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);
	tagImg = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);
	cvCopyImage(copyImg,tagImg);
	
	//�����ؼ��ڴ����е�λ�ã����ԸĶ���ֵ��ʹ�Ӵ���Ĵ�С���ʣ�
	rect.top+=7;
	rect.bottom+=7;
	rect.left+=25;
	rect.right+=25;
	// ���ͼƬ�ؼ��Ŀ�͸� 
	int rw = rect.right - rect.left;  
	int rh = rect.bottom - rect.top; 
	img = cvCreateImage(cvSize(rw,rh),IPL_DEPTH_8U,3);
	cvResize(copyImg,img,CV_INTER_LINEAR);
	ReleaseDC(pDC);
	myImage.Destroy();
	cvReleaseImage(&copyImg);
}


void CDlgVehInquireUI::OnStnClickedImage()
{
////	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	iIsTwice++;
//	GetCursorPos(&position);
//	ScreenToClient(&position);
//	CClientDC ClientDC(this); 
//	CBrush brush,*oldbrush; 
//	brush.CreateSolidBrush(RGB(0,0,0)); 
//	oldbrush=ClientDC.SelectObject(&brush); 
//	ClientDC.Ellipse(position.x-2,position.y-2,position.x+2,position.y+2);
//
//	ClientDC.SelectObject(oldbrush);
//
//	//ͨ���Խ����ϵ������㻭���ο�
//	if (iIsTwice>0
//		&&(iIsTwice%2==0))
//	{
//		CDC *pDc; pDc = GetDC(); 
//		CBrush cbr; 
//		cbr.CreateStockObject(NULL_BRUSH);  //����һ���ջ�ˢ 
//		CBrush *pOldBr = pDc->SelectObject(&cbr); 
//		CPen cpen; 
//		cpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //����һ����ɫ�Ļ���
//		CPen *pOldPen = pDc->SelectObject(&cpen);
//		pDc->Rectangle(position.x,position.y,pos.x,pos.y);
//		roi_rect_src.x=position.x>pos.x?pos.x:position.x;
//		roi_rect_src.y=position.y>pos.y?pos.y:position.y;
//		roi_rect_src.width=abs(position.x-pos.x);
//		roi_rect_src.height=abs(position.y-pos.y);
//		cvSetImageROI(img,roi_rect_src);
//		tagImg = cvCreateImage(cvSize(roi_rect_src.width,roi_rect_src.height),IPL_DEPTH_8U,3);
//		cvCopy(img,tagImg);
//		VehicleProc(tagImg);
//		//meas.box= cvRect(roi_rect_src.x,roi_rect_src.y,roi_rect_src.width,roi_rect_src.height);
//		CString sInfo;
//		sInfo.Format("plate position:left:%d,top:%d,right:%d,bottom:%d,number:%s\ncolor:%s\ntype:%s\n",vehinfo[0].pointLT.x,vehinfo[0].pointLT.y,vehinfo[0].pointRB.x,vehinfo[0].pointRB.y,vehinfo[0].platenumber,vehinfo[0].eVehicleColor,vehinfo[0].tempVehicleType);
//		this->SetDlgItemTextA(IDC_ETEXT,sInfo);
//		CString colorValue;
//		CString  tText;
//		tText =sText;
//		int idex = tText.Find("limit");
////		colorValue.Format("where yh=%.3f or eh=%.3f or  sh=%.3f or fh=%.3f or wh=%.3f or  lh=%.3f ",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0],tColor.nH[3].hsv.val[0],tColor.nH[4].hsv.val[0],tColor.nH[5].hsv.val[0]);
//		//tText.Insert(idex,colorValue);
//		//SetDlgItemTextA(IDC_ETEXT,tText); //��ӵ��༭����ʾ
//		pDc->SelectObject(pOldPen); 
//		cbr.DeleteObject(); 
//		pDc->SelectObject(pOldBr); 
//		cbr.DeleteObject();
//		cvResetImageROI(img);
//	}
//	else
//	{
//		pos.x = position.x;
//		pos.y = position.y;
//	}
}
//void CDlgVehInquireUI::VehicleProc(IplImage* pSrcImage)
//{
//	//���ô���
//	HMODULE dlh = NULL;
//	if (!(dlh=LoadLibrary("VehicleTypeRecognition_D.dll")))      
//	{
//		printf("LoadLibrary() failed: %d\n", GetLastError()); 
//	}
//	// ITS_ThreadInit(); //��������ʱ����
//	ThreadInit threadInit;
//	threadInit = (ThreadInit)GetProcAddress(dlh, "ITS_ThreadInit");//ITS_ThreadInit(); ��������ʱ����
//	(*threadInit)();
//
//	
//	int iInitFlag;
//	char* modePath = "E:\\RedHadoop Project\\RedHadoop Workstation\\RedHadoopWorkstation0.1\\RedHadoopWorkstation0.3\\model";//..\\model	..\\bin\\model
//	VehicleRecInit vehicleRecInit;
//	vehicleRecInit = (VehicleRecInit)GetProcAddress(dlh, "ITS_VehicleRecInit");
//	void* pInstance  = (*vehicleRecInit)(modePath, iInitFlag);//��ʼ������
//	//pInstance = ITS_VehicleRecInit(modePath, iInitFlag);//��ʼ������
//	ITS_Vehicle_Result sResult;
//	V_Image* pVImage = (V_Image*)malloc(sizeof(V_Image)*1);
//	pVImage->eType =E_BGR ;
//	pVImage->iImageHeight = pSrcImage->height;
//	pVImage->iImageWidth = pSrcImage->width;
//	pVImage->pImageData = (unsigned char*)malloc(sizeof(unsigned char)*pVImage->iImageHeight*pVImage->iImageWidth*3);
//	int iIndex = 0;
//	for (int i = 0; i < pVImage->iImageHeight; i ++)
//	{
//		unsigned char* p = (unsigned char*)(pSrcImage->imageData + pSrcImage->widthStep*i);
//		for (int j = 0; j < pVImage->iImageWidth; j ++)
//		{
//			pVImage->pImageData[iIndex ++] = p[j*3 + 0];
//			pVImage->pImageData[iIndex ++] = p[j*3 + 1];
//			pVImage->pImageData[iIndex ++] = p[j*3 + 2];
//		}
//	}
//	VehicleTypeRec vehicleTypeRec;
//	vehicleTypeRec = (VehicleTypeRec)GetProcAddress(dlh, "ITS_VehicleTypeRec");
//	(*vehicleTypeRec)(pInstance,pVImage,&sResult);
//	//ITS_VehicleTypeRec(pInstance,pVImage,&sResult);
//	CvPoint point1,point2;
//	
//	    int inum = sResult.iNum;
//		point1.x = sResult.tempResult[0].plateRect.iLeft;
//		point1.y = sResult.tempResult[0].plateRect.iTop;
//		point2.x = sResult.tempResult[0].plateRect.iRight;
//		point2.y = sResult.tempResult[0].plateRect.iBottom;
//		vehinfo[0].pointLT = point1;
//		vehinfo[0].pointRB = point2; //����λ��
//		strcpy(vehinfo[0].platenumber ,sResult.tempResult[0].plateInfor.platenumber); //���ƺ�
//		strcpy(vehinfo[0].tempVehicleType,sResult.tempResult[0].tempVehicleType);
//		switch (sResult.tempResult[0].eVehicleColor1)                            //������ɫ��Ϣ
//		{
//		case E_VEHICLE_RED:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_GREEN:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_BLUE:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_PINK:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_BROWN:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_YELLOW:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_WHITE:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_BLACK:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_GRAY:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		case E_VEHICLE_CYAN:
//			//printf("��������ɫ����ɫ");
//			vehinfo[0].eVehicleColor ="��ɫ";
//			break;
//		default:
//			//printf("��������ɫ��δ֪");
//			vehinfo[0].eVehicleColor ="δ֪";
//			break;
//		}
//
//		VehicleRecRelease vehicleRecRelease;
//		vehicleRecRelease = (VehicleRecRelease)GetProcAddress(dlh, "ITS_VehicleRecRelease");
//		(*vehicleRecRelease)(pInstance);
//}

void CDlgVehInquireUI::OnBnClickedBvehicle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ô���
	HMODULE dlh = NULL;
	if (!(dlh=LoadLibrary("VehicleTypeRecognition_D.dll")))      
	{
		printf("LoadLibrary() failed: %d\n", GetLastError()); 
	}
	// ITS_ThreadInit(); //��������ʱ����
	ThreadInit threadInit;
	threadInit = (ThreadInit)GetProcAddress(dlh, "ITS_ThreadInit");//ITS_ThreadInit(); ��������ʱ����
	(*threadInit)();


	int iInitFlag;
	char* modePath = "..\\model";//..\\model	..\\bin\\model
	VehicleRecInit vehicleRecInit;
	vehicleRecInit = (VehicleRecInit)GetProcAddress(dlh, "ITS_VehicleRecInit");
	void* pInstance  = (*vehicleRecInit)(modePath, iInitFlag);//��ʼ������
	//pInstance = ITS_VehicleRecInit(modePath, iInitFlag);//��ʼ������
	ITS_Vehicle_Result sResult;
	V_Image* pVImage = (V_Image*)malloc(sizeof(V_Image)*1);
	pVImage->eType =E_BGR ;
	pVImage->iImageHeight = tagImg->height;
	pVImage->iImageWidth = tagImg->width;
	
	pVImage->pImageData = (unsigned char*)malloc(sizeof(unsigned char)*pVImage->iImageHeight*pVImage->iImageWidth*3);
	int iIndex = 0;
	for (int i = 0; i < pVImage->iImageHeight; i ++)
	{
		unsigned char* p = (unsigned char*)(tagImg->imageData + tagImg->widthStep*i);
		for (int j = 0; j < pVImage->iImageWidth; j ++)
		{
			pVImage->pImageData[iIndex ++] = p[j*3 + 0];
			pVImage->pImageData[iIndex ++] = p[j*3 + 1];
			pVImage->pImageData[iIndex ++] = p[j*3 + 2];
		}
	}
	VehicleTypeRec vehicleTypeRec;
	vehicleTypeRec = (VehicleTypeRec)GetProcAddress(dlh, "ITS_VehicleTypeRec");
	(*vehicleTypeRec)(pInstance,pVImage,&sResult);
	//ITS_VehicleTypeRec(pInstance,pVImage,&sResult);
	CvPoint point1,point2;
	for (int j = 0; j < sResult.iNum; j++)
		{
			point1.x = sResult.tempResult[j].plateRect.iLeft;
			point1.y = sResult.tempResult[j].plateRect.iTop;
			point2.x = sResult.tempResult[j].plateRect.iRight;
			point2.y = sResult.tempResult[j].plateRect.iBottom;

			vehinfo[j].pointLT = point1;
            vehinfo[j].pointRB = point2; //����λ��
			strcpy(vehinfo[j].platenumber ,sResult.tempResult[j].plateInfor.platenumber); //���ƺ�
			strcpy(vehinfo[j].tempVehicleType,sResult.tempResult[j].tempVehicleType);
			switch (sResult.tempResult[j].eVehicleColor1)                            //������ɫ��Ϣ
			{
			case E_VEHICLE_RED:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_GREEN:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_BLUE:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_PINK:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_BROWN:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_YELLOW:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_WHITE:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_BLACK:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_GRAY:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			case E_VEHICLE_CYAN:
				//printf("��������ɫ����ɫ");
				vehinfo[j].eVehicleColor ="��ɫ";
				break;
			default:
				//printf("��������ɫ��δ֪");
				vehinfo[j].eVehicleColor ="δ֪";
				break;
			}

		}
	CString sInfo;
	sInfo.Format("plate position:\t\nleft:%d,top:%d\t\nright:%d,bottom:%d\t\nnumber:%s\t\ncolor:%s\t\ntype:%s\t\n",vehinfo[0].pointLT.x,vehinfo[0].pointLT.y,vehinfo[0].pointRB.x,vehinfo[0].pointRB.y,vehinfo[0].platenumber,vehinfo[0].eVehicleColor,vehinfo[0].tempVehicleType);
	this->SetDlgItemTextA(IDC_EDIT2,sInfo);
	CString vehValue;
	CString  tText;
	tText =sText;
	int idex = tText.Find("limit");
	vehValue.Format("where number='%s' and color='%s' and  type='%s' ",vehinfo[0].platenumber,vehinfo[0].eVehicleColor,vehinfo[0].tempVehicleType);
	tText.Insert(idex,vehValue);
	SetDlgItemTextA(IDC_ETEXT,tText); //��ӵ��༭����ʾ

	VehicleRecRelease vehicleRecRelease;
	vehicleRecRelease = (VehicleRecRelease)GetProcAddress(dlh, "ITS_VehicleRecRelease");
	(*vehicleRecRelease)(pInstance);
}
