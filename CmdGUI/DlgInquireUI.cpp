// DlgInquireUI.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgInquireUI.h"
#include "afxdialogex.h"


// CDlgInquireUI �Ի���

IMPLEMENT_DYNAMIC(CDlgInquireUI, CDialogEx)

CDlgInquireUI::CDlgInquireUI(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInquireUI::IDD, pParent)
{
    m_bLBDown = false;
	iIsTwice = 0;
	img = NULL;
	tagImg = NULL;
}

CDlgInquireUI::~CDlgInquireUI()
{
	if (img)
	{
		cvReleaseImage(&img);
	}
	if (tagImg)
	{
		cvReleaseImage(&tagImg);
	}
}

void CDlgInquireUI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_DATETIMEPICKER1, m_ctlBeginDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_ctlEndDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_ctlBeginTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_ctlEndTime);*/
}


BEGIN_MESSAGE_MAP(CDlgInquireUI, CDialogEx)

	ON_BN_CLICKED(IDC_LoadImage, &CDlgInquireUI::OnBnClickedLoadimage)
	ON_STN_CLICKED(IDC_IMAGE, &CDlgInquireUI::OnStnClickedImage)
	/*ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()*/
	//ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgInquireUI ��Ϣ�������
void CDlgInquireUI::initImage(CImage myImage)
{
	/*img = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);
	cvCopyImage(myImage,img);*/
}
//void CDlgInquireUI::ResizeImage(CImage img)
//{;
//	// ��ȡͼƬ�Ŀ�͸�
//	int w = img.GetWidth();
//	int h = img.GetHeight();
//
//	// �ҳ���͸��еĽϴ�ֵ��
//	int max = (w > h)? w: h;
//	// �����ʾ�ؼ��� DC 
//	CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
//	// ��ȡ HDC(�豸���) �����л�ͼ����    
//	HDC hDC = pDC ->GetSafeHdc(); 
//	CRect rect; 
//	GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect ); 
//	// ���ͼƬ�ؼ��Ŀ�͸� 
//	int rw = rect.right - rect.left;  
//	int rh = rect.bottom - rect.top; 
//
//    float scale =.0;
//	// ���㽫ͼƬ���ŵ�picture control��������ı�������
//	if (max==h)
//	{
//        scale = (float) ( (float) max /(float)rh );
//	} 
//	else
//	{
//       scale = (float) ( (float) max / (float)rw );
//	}
//	
//
//	// ���ź�ͼƬ�Ŀ�͸�
//	int nw = (int)( w/scale );
//	int nh = (int)( h/scale );
//	
//
//}

void CDlgInquireUI::OnBnClickedLoadimage()
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


BOOL CDlgInquireUI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void RGBtoHSV( BYTE r, BYTE g, BYTE b, float &h, float &s, float &v )
{
	BYTE min, max;
	float delta;

	min = MIN( r, g, b );
	max = MAX( r, g, b );
	v = max; // v
	delta = max - min;

	if( max != 0 )
	{
		s = delta / max; // s
	}
	else
	{
		// r = g = b = 0 // s = 0, v is undefined
		s = 0;
		h = -1;
		return;
	}

	if( r == max )
	{
		h = ( g - b ) / delta; // between yellow & magenta
	}
	else if( g == max )
	{
		h = 2 + ( b - r ) / delta; // between cyan & yellow
	}
	else
	{
		h = 4 + ( r - g ) / delta; // between magenta & cyan
	}

	h *= 60; // degrees

	if( h < 0 )
	{
		h += 360;
	}
} 


void CDlgInquireUI::HistogramProcess(IplImage* tagImg)
{
	IplImage* hsv = cvCreateImage( cvGetSize(tagImg), 8, 3 );//hsvɫ�ʿռ�(hueɫ�� saturation���Ͷ� value����)
	IplImage* h_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane, v_plane };//
	//** H ��������Ϊ16���ȼ���S��������Ϊ8���ȼ� */  
	int h_bins =32,s_bins =4;  //,v_bins = 4
	int hist_size[] = { h_bins,s_bins  };// ,v_bins 


	//** H �����ı仯��Χ */  
	float h_ranges[] = { 0, 180 };   
	//** S �����ı仯��Χ*/  
	float s_ranges[] = { 0, 255 };  
	float* ranges[] = { h_ranges, s_ranges };// ,v_ranges
	//** ����ͼ��ת����HSV��ɫ�ռ� */  
	cvCvtColor( tagImg, hsv, CV_BGR2HSV );  
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 ); //
	//** ����ֱ��ͼ����ά, ÿ��ά���Ͼ��� */  
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 ); //2 
	//** ����H,S����ƽ������ͳ��ֱ��ͼ */  
	cvCalcHist( planes, hist, 0, 0 );  
	//��һ������
	cvNormalizeHist(hist,1.0);

	//** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ */  
	float max_value;
	cvGetMinMaxHistValue( hist,0, &max_value, 0, 0 );
	//** ����ֱ��ͼ��ʾͼ�� */  
	int height = 240;//3000
	int width = (h_bins*s_bins*6);//h_bins*6   h_bins*s_bins*v_bins*10
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );
	IplImage* histImg = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( histImg );
	//** ��������HSV��RGB��ɫת������ʱ��λͼ�� */  
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width/(h_bins*s_bins );//*v_bins 
	if(max_value<0.000001)
	{
		return;
	}
//	int Maxtensy,Sectensy,Thirdtensy;

	memset(tColor.nH,0,sizeof(struct Hist));
	//memset(tColor.nS,0,sizeof(struct Hist));
	//memset(tColor.nT,0,sizeof(struct Hist));
	int i=0,j=0;
	bool IsFirst = true;
	memset(histinfo,0,sizeof(struct Hist)*128);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			    int i = h*s_bins+s;
				/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶� */ 
				float bin_val = cvQueryHistValue_2D( hist, h, s );
				int intensity = cvRound(bin_val*height/max_value);

				/** ��õ�ǰֱ��ͼ�������ɫ��ת����RGB���ڻ��� */  
				cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));//s*255.f/s_bins        v*255.f/v_bins
				cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
				CvScalar color = cvGet2D(rgb_color,0,0);
				cvRectangle( hist_img, cvPoint(i*bin_w,height),
					cvPoint((i+1)*bin_w,height - intensity),
					color, -1, 8, 0 );
				////��ȡ��������ɫ��Ϣ��ֱ����
				//if ((color.val[0]!=color.val[1]
				//||color.val[1]!=color.val[2]
				//||color.val[2]!=color.val[0])
				//	&&intensity!=0)
				histinfo[i].hsv.val[0]= h*180.f / h_bins;
				//	histinfo[i].bgr = color;
				histinfo[i].value = intensity;

					
		}

	}
	histGroup histgroup[32];
	memset (histgroup,0,sizeof(struct Group)*32);
	for (int i =0;i<31;i++)
	{
		histgroup[i+1].h=histgroup[i].h+180.f/h_bins;
	}

	for (int j=0;j<128;j++)
	{
		for (int i =0;i<32;i++)
		{
			if (histinfo[j].hsv.val[0]==histgroup[i].h)
			{
				histgroup[i].value=histinfo[j].value+histgroup[i].value;
				//histgroup[i].bgr = histinfo[j].bgr;
			}
		}
	}
	for (int i=0;i<31;i++)
	{
		//	printf("%d\n",histgroup[i].value);
	}
	//������
	int max,tmp;
	float temp;
	for(i=0;i<32;i++)
	{
		max =i;
		for (j=i+1;j<32;j++)
		{
			if(histgroup[max].value<histgroup[j].value)
				max=j;
		}
		if(i!=max)
		{
			tmp =histgroup[max].value;
			histgroup[max].value=histgroup[i].value;
			histgroup[i].value= tmp;
			temp =histgroup[max].h;
			histgroup[max].h=histgroup[i].h;
			histgroup[i].h= temp;
		}
	}
	//��ȡ�ۼӺ���������

	tColor.nH[0].hsv.val[0] = histgroup[0].h;
	tColor.nH[1].hsv.val[0] = histgroup[1].h;
	tColor.nH[2].hsv.val[0] = histgroup[2].h;
	tColor.nH[3].hsv.val[0] = histgroup[3].h;
	tColor.nH[4].hsv.val[0] = histgroup[4].h;
	tColor.nH[5].hsv.val[0] = histgroup[5].h;
	////��ȡ�ۼӺ���������

	//for (int i=1;i<32;i++)
	//{
	//	if (i==1)
	//	{
	//		Maxtensy =  histgroup[i].value;
	//		Sectensy = Thirdtensy = 0;
	//		tColor.nM[0].nC=tColor.nS[0].nC=tColor.nT[0].nC=i;
	//		tColor.nM[0].hsv.val[0]=tColor.nS[0].hsv.val[0]=tColor.nT[0].hsv.val[0]=histgroup[i].h;
	//	}
	//	else
	//	{
	//		if ( histgroup[i].value!=0)
	//		{
	//			if ( histgroup[i].value>Maxtensy)
	//			{
	//				Thirdtensy = Sectensy;tColor.nT[0] = tColor.nS[0];
	//				Sectensy = Maxtensy;tColor.nS[0] = tColor.nM[0];
	//				Maxtensy =  histgroup[i].value;tColor.nM[0].nC = i;tColor.nM[0].hsv.val[0]=histgroup[i].h;
	//			}

	//			else if ( histgroup[i].value > Sectensy)
	//			{
	//				Thirdtensy = Sectensy;tColor.nT[0] = tColor.nS[0];
	//				Sectensy =  histgroup[i].value;tColor.nS[0].nC = i;tColor.nS[0].hsv.val[0]=histgroup[i].h;
	//			}
	//			else if ( histgroup[i].value > Thirdtensy)
	//			{
	//				Thirdtensy =  histgroup[i].value;tColor.nT[0].nC = i;tColor.nT[0].hsv.val[0]=histgroup[i].h;
	//			}
	//		}
	//	}
	//}
	/** ��õ�ǰֱ��ͼ��ߵ������������ɫ��ת����RGB���ڻ��� */
	//�����
	/*cvZero(rgb_color);
	cvZero(hsv_color);
	cvSet2D(hsv_color,0,0,tColor.nM[0].hsv);
	cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
	CvScalar color = cvGet2D(rgb_color,0,0);
	cvRectangle( histImg, cvPoint(tColor.nM[0].nC*bin_w,height),
		cvPoint((tColor.nM[0].nC+1)*bin_w,height - Maxtensy),color, -1, 8, 0 );
	tColor.nM[0].bgr.val[0]=color.val[2];
	tColor.nM[0].bgr.val[1]=color.val[1];
	tColor.nM[0].bgr.val[2]=color.val[0];*/
	cvNamedWindow("hist",1);
	cvMoveWindow("hist",420,120);
	cvShowImage("hist",hist_img);
	/*cvNamedWindow("hista",1);
	cvMoveWindow("hista",420,320);
	cvShowImage("hista",histImg);
	cvReleaseImage(&hist_img);*/
	cvReleaseImage(&histImg);
	cvReleaseImage(&hsv);
	cvReleaseImage(&h_plane);
	cvReleaseImage(&s_plane);
	cvReleaseImage(&v_plane);
	cvReleaseImage(&hsv_color);
	cvReleaseImage(&rgb_color);
}
void CDlgInquireUI::OnStnClickedImage()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	iIsTwice++;
	GetCursorPos(&position);
	ScreenToClient(&position);
	//COLORREF m_Color;
	////m_Color = m_btColor.GetColor();
	//CString strT,str;
 //   BYTE   bRed;
 //   BYTE   bGreen;
 //   BYTE   bBlue;
 //
 //   bRed = GetRValue(m_Color);
 //   bGreen = GetGValue(m_Color);
 //   bBlue = GetBValue(m_Color);
 //
 //   strT.Format("%d",bRed);
 //   str += "Red: " + strT + "\r\n";
 //   strT.Format("%d",bGreen);
 //   str += "Green: " + strT + "\r\n";
 //   strT.Format("%d",bBlue);
 //   str += "Blue: " + strT + "\r\n";
	//float h=0;
	//float s=0;
	//float v=0;
	//RGBtoHSV(bRed,bGreen,bBlue,h,s,v);
	//strT.Format("%f",h);
	//str += "H: " + strT + "\r\n";
	//strT.Format("%f",s);
	//str += "S: " + strT + "\r\n";
	//strT.Format("%f",v);
	//str += "V: " + strT + "\r\n";
	//AfxMessageBox(str);

	CClientDC ClientDC(this); 
	CBrush brush,*oldbrush; 
	brush.CreateSolidBrush(RGB(0,0,0)); 
	oldbrush=ClientDC.SelectObject(&brush); 
	ClientDC.Ellipse(position.x-2,position.y-2,position.x+2,position.y+2);
	//ClientDC.TextOutA(position.x+10,position.y+10,"hello");
	
	ClientDC.SelectObject(oldbrush);
	

	//ͨ���Խ����ϵ������㻭���ο�
	if (iIsTwice>0
		&&(iIsTwice%2==0))
	{
		CDC *pDc; pDc = GetDC(); 
		CBrush cbr; 
		cbr.CreateStockObject(NULL_BRUSH);  //����һ���ջ�ˢ 
		CBrush *pOldBr = pDc->SelectObject(&cbr); 
		CPen cpen; 
		cpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //����һ����ɫ�Ļ���
		CPen *pOldPen = pDc->SelectObject(&cpen);
		pDc->Rectangle(position.x,position.y,pos.x,pos.y);

		
		//// �����ʾ�ؼ��� DC 
		//CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
		//// ��ȡ HDC(�豸���) �����л�ͼ����    
		//HDC hDC = pDC ->GetSafeHdc(); 
		//CRect rect; 
		//GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect );  
		
		//���ڸ����ľ������ø���Ȥ����ROI
		roi_rect_src.x=position.x>pos.x?pos.x:position.x;
		roi_rect_src.y=position.y>pos.y?pos.y:position.y;
		roi_rect_src.width=abs(position.x-pos.x);
		roi_rect_src.height=abs(position.y-pos.y);
		cvSetImageROI(img,roi_rect_src);
		tagImg = cvCreateImage(cvSize(roi_rect_src.width,roi_rect_src.height),IPL_DEPTH_8U,3);
		cvCopy(img,tagImg);
		meas.box= cvRect(roi_rect_src.x,roi_rect_src.y,roi_rect_src.width,roi_rect_src.height);
		HistogramProcess(tagImg);
		CString sInfo;
		//sInfo.Format("Mb:%f,Mg:%f,Mr:%f,Sb:%f,Sg:%f,Sr:%f,Tb:%f,Tg:%f,Tr:%f,Mh:%f,Sh:%f,Th:%f",tColor.nM[0].bgr.val[0],tColor.nM[0].bgr.val[1],tColor.nM[0].bgr.val[2],tColor.nS[0].bgr.val[0],tColor.nS[0].bgr.val[1],tColor.nS[0].bgr.val[2],
		//	tColor.nT[0].bgr.val[0],tColor.nT[0].bgr.val[1],tColor.nT[0].bgr.val[2],tColor.nM[0].hsv.val[0],tColor.nS[0].hsv.val[0],tColor.nT[0].hsv.val[0]);
		sInfo.Format("YH:%f\nEH:%f\nSH:%f\nFH:%f\nWH:%f\nLH:%f\n",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0],tColor.nH[3].hsv.val[0],tColor.nH[4].hsv.val[0],tColor.nH[5].hsv.val[0]);
		/*sInfo=sInfo+ssInfo;*/
		//AfxMessageBox(sInfo);
		this->SetDlgItemTextA(IDC_EDIT2,sInfo);
		CString colorValue;
		CString  tText;
		tText =sText;
		int idex = tText.Find("limit");
		colorValue.Format("where yh=%.3f or eh=%.3f or  sh=%.3f or fh=%.3f or wh=%.3f or  lh=%.3f ",tColor.nH[0].hsv.val[0],tColor.nH[1].hsv.val[0],tColor.nH[2].hsv.val[0],tColor.nH[3].hsv.val[0],tColor.nH[4].hsv.val[0],tColor.nH[5].hsv.val[0]);
		tText.Insert(idex,colorValue);
		SetDlgItemTextA(IDC_ETEXT,tText); //��ӵ��༭����ʾ
		pDc->SelectObject(pOldPen); 
		cbr.DeleteObject(); 
		pDc->SelectObject(pOldBr); 
		cbr.DeleteObject();
		cvResetImageROI(img);
	}
	else
	{
		pos.x = position.x;
		pos.y = position.y;
	}
}


//void CDlgInquireUI::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// m_bLBDown = true;
//	if (m_bLBDown)
//	{
//		m_StartPoint = point;  
//		m_EndPoint = point;  
//		SetCapture();//������겶�� 
//
//		CRect rect;  
//		GetClientRect(&rect); 
//		ClientToScreen(&rect); 
//		ClipCursor(&rect); 
//
//	}
//	CDialogEx::OnLButtonDown(nFlags, point);
//}
//
//
//void CDlgInquireUI::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if (m_bLBDown)
//	{
//       m_bLBDown = false;
//	   ReleaseCapture();   
//	   ClipCursor( NULL );   
//	   CWindowDC dc(this);   
//	   dc.SetROP2( R2_NOT ); 
//	   //dc.MoveTo( m_ptStart );       //�����д����������㣨��갴�µ㣩��   
//	   //dc.LineTo( m_ptOld ); 
//	   dc.Rectangle(m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y);//�ϴ�����ƶ�����λ��֮�����ʱ��  
//	   dc.SetROP2( R2_COPYPEN ); 
//	   //dc.MoveTo( m_ptStart );      //�����д������㵽��굱ǰλ�û���  
//	   //dc.LineTo( point ); // 
//	   dc.Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y);    
//	   //CDocViewDoc *pDoc = GetDocument();           //�õ��ĵ���ָ��   //pDoc->m_line = new CLine(m_ptStart,point);    //����CLine����   //pDoc->m_LineList.AddTail(  pDoc->m_line);    //��pLine���뵽������ 
//	   //pDoc->SetModifiedFlag();  // Mark the document as having been modified, for  
//	   // purposes of confirming File Close. 
//		   //pDoc->UpdateAllViews(this, 0L,pLine); 
//		   //SetCursor(m_hArrow);          //���������״Ϊ��׼��ͷ�� 
//	}
//
//	CDialogEx::OnLButtonUp(nFlags, point);
//}
//
//
//void CDlgInquireUI::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if (m_bLBDown)
//	{
//		
//		CWindowDC dc(this); 
//		dc.SetROP2( R2_NOT );//����ģʽ 
//		//dc.MoveTo( m_ptStart ); //�����д����������㣨��갴�µ㣩��   
//		//dc.LineTo( m_ptOld ); //�ϴ�����ƶ�����λ��֮�����ʱ��   
//		dc.Rectangle(m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y);   
//		dc.MoveTo( m_StartPoint ); //�����д������㵽��굱ǰλ�û���  
//		//dc.LineTo( point );   
//		dc.Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y); 
//		m_EndPoint = point; //��굱ǰλ������һ������ƶ��¼���������"��λ��" 
//	}
//
//	CDialogEx::OnMouseMove(nFlags, point);
//}


//void CDlgInquireUI::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	//���Ʊ���  
//	// �����ʾ�ؼ��� DC 
//	CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
//	// ��ȡ HDC(�豸���) �����л�ͼ����    
//	HDC hDC = pDC ->GetSafeHdc(); 
//	CRect rect; 
//	GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect );  
//	//�����ؼ��ڴ����е�λ�ã����ԸĶ���ֵ��ʹ�Ӵ���Ĵ�С���ʣ�
//	rect.top+=7;
//	rect.bottom+=7;
//	rect.left+=25;
//	rect.right+=25;
//	// ���ͼƬ�ؼ��Ŀ�͸� 
//	int rw = rect.right - rect.left;  
//	int rh = rect.bottom - rect.top; 
//	//�����϶�����  
//	 
//	//	CRect rect(m_StartPoint, m_EndPoint);  
//		//rect.NormalizeRect();//�淶������  
//		//CBrush bruPen(RGB(255, 0, 0));//���α߿���ɫ  
//		//dc.FrameRect(&rect, &bruPen);  
//	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//}
