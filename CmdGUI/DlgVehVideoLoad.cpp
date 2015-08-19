// DlgVehVideoLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVehVideoLoad.h"
#include "afxdialogex.h"


// CDlgVehVideoLoad 对话框

IMPLEMENT_DYNAMIC(CDlgVehVideoLoad, CDialogEx)

CDlgVehVideoLoad::CDlgVehVideoLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgVehVideoLoad::IDD, pParent)
	, m_radiobtngroup1(0)
{
	 strStore="TEXTFILE";
}

CDlgVehVideoLoad::~CDlgVehVideoLoad()
{
}

void CDlgVehVideoLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Radio(pDX, IDC_RADIOPARQUET, m_radiobtngroup1);
}


BEGIN_MESSAGE_MAP(CDlgVehVideoLoad, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgVehVideoLoad::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTNVIDEOBROWSER, &CDlgVehVideoLoad::OnBnClickedBtnvideobrowser)
END_MESSAGE_MAP()


// CDlgVehVideoLoad 消息处理程序
BOOL CDlgVehVideoLoad::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_progress.SetRange(0,100);
	m_radiobtngroup1 = 0;
	SetDlgItemTextA(IDC_SPLITSIZE,"16");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

static BOOL SeasbaseRunExec(const TCHAR *cmd, const TCHAR *para, DWORD dwMilliseconds,bool wait=false)
{    //  DWORD Sk=GetTickCount();

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd;
	ShExecInfo.lpParameters = para;
	ShExecInfo.lpDirectory = NULL;//_T("D:\\Redhadoop\\CRH3\\hadoop\\bin");
	ShExecInfo.nShow =SW_NORMAL; //SW_SHOWMINIMIZED; // SW_HIDE
	ShExecInfo.hInstApp = NULL;
	BOOL suc = ShellExecuteEx(&ShExecInfo);

	if (wait) {  
		HANDLE hProcess = ShExecInfo.hProcess;  
		if (hProcess != 0) {  
			WaitForSingleObject(hProcess, INFINITE);  
			CloseHandle(hProcess);  
		}  
	}  

	return suc;
}
void CDlgVehVideoLoad::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	CString strTable;
	CString strSize;
	/*const char createTemplate[] =
	"CREATE EXTERNAL TABLE IF NOT EXISTS RAW%s \
	(frameid int,targetnum int,sx int,sy int,ex int,ey int,number string,type string,color string,frame BINARY) \
	ROW FORMAT SERDE \'com.seasbase.video.serde.ImageInfoSerDe\' \
	STORED AS \
	INPUTFORMAT \'com.redhadoop.io.input.VideoInputFormat\' \
	OUTPUTFORMAT \'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat\' " 
	"LOCATION \'/user/hive/warehouse/Video%s\'";

	const char ctasTemplate[] ="set mapred.max.split.size =%d000000;create  \
	table %s(frameid int,targetnum int,sx int,sy int,ex int,ey int,number string,type string,color string,frame string) STORED \
	AS %s;insert overwrite table  %s select frameid, targetnum,sx,sy,ex,ey,number,type,color,base64(frame) from RAW%s";*/
	const char createTemplate[] =
		"CREATE EXTERNAL TABLE IF NOT EXISTS RAW%s \
		(frameid int,targetnum int,x int,y int,width int,height int,number string,type string,color string,frame BINARY) \
		ROW FORMAT SERDE \'com.seasbase.video.serde.ImageInfoSerDe\' \
		STORED AS \
		INPUTFORMAT \'com.redhadoop.io.input.VideoInputFormat\' \
		OUTPUTFORMAT \'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat\' " 
		"LOCATION \'/user/hive/warehouse/Video%s\'";

	const char ctasTemplate[] ="set mapred.max.split.size =%d000000;create  \
							   table %s(frameid int,targetnum int,x int,y int,width int,height int,number string,type string,color string,frame string) STORED \
							   AS %s;insert overwrite table  %s select frameid, targetnum,x,y,width,height,number,type,color,base64(frame) from RAW%s";
	this->GetDlgItemText(IDC_VIDEOTABLE,strTable);
	this->GetDlgItemText(IDC_VIDEOFILEPATH,path);
	strPath=path;
	this->GetDlgItemText(IDC_SPLITSIZE,strSize);
	int size =atoi(strSize);
	if(strTable.GetLength()<1)
	{
		::AfxMessageBox("Destination Table must not be Empty!");
		return;
	}
	if(path.GetLength()<3)
	{
		::AfxMessageBox("Video File Full Path must not be Empty!");
		return;
	}
	if (size<16)
	{
		::AfxMessageBox("split size is too small");
		return;
	}
	char buff[1024]={0};
	char command[1024]={0};
	m_progress.SetPos(0);
	sprintf(command,createTemplate, strTable,strTable);
	sprintf(buff,"/c hive -e \"%s\"  ",command);
	SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	m_progress.SetPos(10);
	sprintf(buff,"/c hadoop fs -mkdir -p /user/hive/warehouse/Video%s",strTable);
	SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	m_progress.SetPos(15);
	sprintf(buff,"/c hadoop fs -put %s /user/hive/warehouse/Video%s",path,strTable);
	//sprintf(buff,"/c hadoop fs -put %s /movie/detect%s>err.txt 2>&1",path,strTable);
	SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	m_progress.SetPos(40);
	// TODO: 在此添加控件通知处理程序代码
	sprintf(command,ctasTemplate,size,strTable,strStore,strTable,strTable);
	sprintf(buff,"/c hive -e \"%s\"  2>data.txt",command);
	SeasbaseRunExec(_T("cmd"),buff, INFINITE,true);
	m_progress.SetPos(100);
	CDialogEx::OnOK();
}


void CDlgVehVideoLoad::OnBnClickedBtnvideobrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, 
		"Video Files (*avi)", 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ENABLESIZING,
		_T("Text Files (*.avi;*.mpeg;*.mp4;*.ts)|*.avi;*.mpeg;*.mp4;*.ts||"),
		NULL);
	dlg.m_ofn.lpstrTitle=_T("Please select Video File");

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

	this->SetDlgItemTextA(IDC_VIDEOFILEPATH,ary_filename.GetAt(0));
}


