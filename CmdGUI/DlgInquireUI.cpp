// DlgInquireUI.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInquireUI.h"
#include "afxdialogex.h"


// CDlgInquireUI 对话框

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


// CDlgInquireUI 消息处理程序
void CDlgInquireUI::initImage(CImage myImage)
{
	/*img = cvCreateImage(cvSize(myImage.GetWidth(),myImage.GetHeight()),IPL_DEPTH_8U,3);
	cvCopyImage(myImage,img);*/
}
//void CDlgInquireUI::ResizeImage(CImage img)
//{;
//	// 读取图片的宽和高
//	int w = img.GetWidth();
//	int h = img.GetHeight();
//
//	// 找出宽和高中的较大值者
//	int max = (w > h)? w: h;
//	// 获得显示控件的 DC 
//	CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
//	// 获取 HDC(设备句柄) 来进行绘图操作    
//	HDC hDC = pDC ->GetSafeHdc(); 
//	CRect rect; 
//	GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect ); 
//	// 求出图片控件的宽和高 
//	int rw = rect.right - rect.left;  
//	int rh = rect.bottom - rect.top; 
//
//    float scale =.0;
//	// 计算将图片缩放到picture control区域所需的比例因子
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
//	// 缩放后图片的宽和高
//	int nw = (int)( w/scale );
//	int nh = (int)( h/scale );
//	
//
//}

void CDlgInquireUI::OnBnClickedLoadimage()
{
	// TODO: 在此添加控件通知处理程序代码
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
	
	//ResizeImage( myImage );    // 对读入的图片进行缩放
	
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_IMAGE); // (这是在此资源创建的类的内部, 若是在外部, 可先通过获得CMainFrame的指针, 再通过pMianFrame->GetDlgItem(IDCk_MY_PIC)来获取)
	CDC *pDC = pWnd->GetDC();
	pWnd->GetClientRect(&rect);
	pDC->SetStretchBltMode(STRETCH_HALFTONE);
	myImage.Draw(pDC->m_hDC, rect);
	
	//调整控件在窗口中的位置，可以改动数值，使子窗体的大小合适；
	rect.top+=7;
	rect.bottom+=7;
	rect.left+=25;
	rect.right+=25;
	// 求出图片控件的宽和高 
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

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
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
	IplImage* hsv = cvCreateImage( cvGetSize(tagImg), 8, 3 );//hsv色彩空间(hue色相 saturation饱和度 value明度)
	IplImage* h_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(tagImg), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane, v_plane };//
	//** H 分量划分为16个等级，S分量划分为8个等级 */  
	int h_bins =32,s_bins =4;  //,v_bins = 4
	int hist_size[] = { h_bins,s_bins  };// ,v_bins 


	//** H 分量的变化范围 */  
	float h_ranges[] = { 0, 180 };   
	//** S 分量的变化范围*/  
	float s_ranges[] = { 0, 255 };  
	float* ranges[] = { h_ranges, s_ranges };// ,v_ranges
	//** 输入图像转换到HSV颜色空间 */  
	cvCvtColor( tagImg, hsv, CV_BGR2HSV );  
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 ); //
	//** 创建直方图，二维, 每个维度上均分 */  
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 ); //2 
	//** 根据H,S两个平面数据统计直方图 */  
	cvCalcHist( planes, hist, 0, 0 );  
	//归一化处理
	cvNormalizeHist(hist,1.0);

	//** 获取直方图统计的最大值，用于动态显示直方图 */  
	float max_value;
	cvGetMinMaxHistValue( hist,0, &max_value, 0, 0 );
	//** 设置直方图显示图像 */  
	int height = 240;//3000
	int width = (h_bins*s_bins*6);//h_bins*6   h_bins*s_bins*v_bins*10
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );
	IplImage* histImg = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( histImg );
	//** 用来进行HSV到RGB颜色转换的临时单位图像 */  
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
				/** 获得直方图中的统计次数，计算显示在图像中的高度 */ 
				float bin_val = cvQueryHistValue_2D( hist, h, s );
				int intensity = cvRound(bin_val*height/max_value);

				/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */  
				cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));//s*255.f/s_bins        v*255.f/v_bins
				cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
				CvScalar color = cvGet2D(rgb_color,0,0);
				cvRectangle( hist_img, cvPoint(i*bin_w,height),
					cvPoint((i+1)*bin_w,height - intensity),
					color, -1, 8, 0 );
				////提取出带有颜色信息的直方条
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
	//简单排序
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
	//求取累加后的最高六条

	tColor.nH[0].hsv.val[0] = histgroup[0].h;
	tColor.nH[1].hsv.val[0] = histgroup[1].h;
	tColor.nH[2].hsv.val[0] = histgroup[2].h;
	tColor.nH[3].hsv.val[0] = histgroup[3].h;
	tColor.nH[4].hsv.val[0] = histgroup[4].h;
	tColor.nH[5].hsv.val[0] = histgroup[5].h;
	////求取累加后的最高三条

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
	/** 获得当前直方图最高的三条代表的颜色，转换成RGB用于绘制 */
	//最高条
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
	// TODO: 在此添加控件通知处理程序代码
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
	

	//通过对角线上的两个点画矩形框
	if (iIsTwice>0
		&&(iIsTwice%2==0))
	{
		CDC *pDc; pDc = GetDC(); 
		CBrush cbr; 
		cbr.CreateStockObject(NULL_BRUSH);  //创建一个空画刷 
		CBrush *pOldBr = pDc->SelectObject(&cbr); 
		CPen cpen; 
		cpen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0)); //创建一个红色的画笔
		CPen *pOldPen = pDc->SelectObject(&cpen);
		pDc->Rectangle(position.x,position.y,pos.x,pos.y);

		
		//// 获得显示控件的 DC 
		//CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
		//// 获取 HDC(设备句柄) 来进行绘图操作    
		//HDC hDC = pDC ->GetSafeHdc(); 
		//CRect rect; 
		//GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect );  
		
		//基于给定的矩形设置感兴趣区域ROI
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
		SetDlgItemTextA(IDC_ETEXT,tText); //添加到编辑框显示
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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
// m_bLBDown = true;
//	if (m_bLBDown)
//	{
//		m_StartPoint = point;  
//		m_EndPoint = point;  
//		SetCapture();//设置鼠标捕获 
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
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (m_bLBDown)
//	{
//       m_bLBDown = false;
//	   ReleaseCapture();   
//	   ClipCursor( NULL );   
//	   CWindowDC dc(this);   
//	   dc.SetROP2( R2_NOT ); 
//	   //dc.MoveTo( m_ptStart );       //这两行代码擦除从起点（鼠标按下点）到   
//	   //dc.LineTo( m_ptOld ); 
//	   dc.Rectangle(m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y);//上次鼠标移动到的位置之间的临时线  
//	   dc.SetROP2( R2_COPYPEN ); 
//	   //dc.MoveTo( m_ptStart );      //这两行代码从起点到鼠标当前位置画线  
//	   //dc.LineTo( point ); // 
//	   dc.Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y);    
//	   //CDocViewDoc *pDoc = GetDocument();           //得到文档类指针   //pDoc->m_line = new CLine(m_ptStart,point);    //创建CLine对象   //pDoc->m_LineList.AddTail(  pDoc->m_line);    //将pLine加入到链表中 
//	   //pDoc->SetModifiedFlag();  // Mark the document as having been modified, for  
//	   // purposes of confirming File Close. 
//		   //pDoc->UpdateAllViews(this, 0L,pLine); 
//		   //SetCursor(m_hArrow);          //设置鼠标形状为标准箭头形 
//	}
//
//	CDialogEx::OnLButtonUp(nFlags, point);
//}
//
//
//void CDlgInquireUI::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	if (m_bLBDown)
//	{
//		
//		CWindowDC dc(this); 
//		dc.SetROP2( R2_NOT );//绘制模式 
//		//dc.MoveTo( m_ptStart ); //这两行代码擦除从起点（鼠标按下点）到   
//		//dc.LineTo( m_ptOld ); //上次鼠标移动到的位置之间的临时线   
//		dc.Rectangle(m_StartPoint.x,m_StartPoint.y,m_EndPoint.x,m_EndPoint.y);   
//		dc.MoveTo( m_StartPoint ); //这两行代码从起点到鼠标当前位置画线  
//		//dc.LineTo( point );   
//		dc.Rectangle(m_StartPoint.x,m_StartPoint.y,point.x,point.y); 
//		m_EndPoint = point; //鼠标当前位置在下一次鼠标移动事件看来就是"旧位置" 
//	}
//
//	CDialogEx::OnMouseMove(nFlags, point);
//}


//void CDlgInquireUI::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 在此处添加消息处理程序代码
//	//绘制背景  
//	// 获得显示控件的 DC 
//	CDC* pDC = GetDlgItem( IDC_IMAGE ) ->GetDC();  
//	// 获取 HDC(设备句柄) 来进行绘图操作    
//	HDC hDC = pDC ->GetSafeHdc(); 
//	CRect rect; 
//	GetDlgItem(IDC_IMAGE) ->GetClientRect( &rect );  
//	//调整控件在窗口中的位置，可以改动数值，使子窗体的大小合适；
//	rect.top+=7;
//	rect.bottom+=7;
//	rect.left+=25;
//	rect.right+=25;
//	// 求出图片控件的宽和高 
//	int rw = rect.right - rect.left;  
//	int rh = rect.bottom - rect.top; 
//	//绘制拖动矩形  
//	 
//	//	CRect rect(m_StartPoint, m_EndPoint);  
//		//rect.NormalizeRect();//规范化矩形  
//		//CBrush bruPen(RGB(255, 0, 0));//矩形边框颜色  
//		//dc.FrameRect(&rect, &bruPen);  
//	// 不为绘图消息调用 CDialogEx::OnPaint()
//}
