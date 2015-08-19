// AutoUpdateClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "AutoUpdateClient.h"
#include "AutoUpdateClientDlg.h"
#include ".\autoupdateclientdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoUpdateClientDlg �Ի���



CAutoUpdateClientDlg::CAutoUpdateClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoUpdateClientDlg::IDD, pParent)
	, m_dwCount(0)
	, m_dwTotalLen(0)
	, m_nMaxDownThread(5)
	, m_nCurThreadCount(0)
	, m_bTray(false)
	, m_dwAlreadyDownLen(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CAutoUpdateClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BTN_AUTOSELSERVER, m_btnAutoSelServer);
	DDX_Control(pDX, IDC_BTN_NOWUPDATE, m_btnNowUpdate);
	DDX_Control(pDX, IDC_PRS_CUR, m_prsCur);
	DDX_Control(pDX, IDC_PRS_TOTAL, m_prsTotal);
	DDX_Control(pDX, IDC_CMB_SELSERVER, m_cmbSelServer);
	DDX_Control(pDX, IDC_LST_LOG, m_lstLog);
}

BEGIN_MESSAGE_MAP(CAutoUpdateClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_NOWUPDATE, OnBnClickedBtnNowupdate)
	ON_MESSAGE(WM_DOWNLOADFILE, OnDownLoadFile) 
	ON_MESSAGE(WM_DOWNLOADFILEEND, OnDownLoadFileEnd) 
	//ON_BN_CLICKED(IDC_BTN_AUTOSELSERVER, OnBnClickedBtnAutoselserver)
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CAutoUpdateClientDlg ��Ϣ�������

BOOL CAutoUpdateClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, true);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_prsCur.SetRange(0, 100);	
	m_prsTotal.SetRange(0, 100);
	
	//m_cmbSelServer.AddString("�������� www.hadoop.com");

	SetTimer(PUBLIC_TIMER, 100, NULL);

	if ( ! m_bTray )
	{
		m_pTray.cbSize				= sizeof(m_pTray);
		m_pTray.hWnd				= GetSafeHwnd();
		m_pTray.uID					= 0;
		m_pTray.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_pTray.uCallbackMessage	= WM_TRAY;
		//m_pTray.hIcon				= theApp.LoadIcon( IDI_ICON1);
		_tcscpy( m_pTray.szTip, _T("�����Զ�����") );
		Shell_NotifyIcon( NIM_ADD, &m_pTray );
		ShowWindow( SW_HIDE );
		m_bTray = true;
	}
	//m_sServerListURL="";
	//AddDownServerListQueue(m_sServerListURL);
	
	return true; 
}



void CAutoUpdateClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CAutoUpdateClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoUpdateClientDlg::RefreshCtrl(void)
{
	while (LB_ERR != m_cmbSelServer.DeleteString(0));
	//m_Servers.FillCmbCtrl(m_cmbSelServer);
}

void CAutoUpdateClientDlg::OnBnClickedBtnNowupdate()
{
	m_cmbSelServer.SetCurSel(0);
	CString serverName="�������� www.redhadoop.com";
	CString sLog; sLog.Format(_T("�������ӷ�������%s"), serverName); AddLog(sLog);
		TCHAR szTempPath[MAX_PATH], szTempName[MAX_PATH];
		::GetTempPath(
			MAX_PATH,
			szTempPath);

		GetTempFileName(szTempPath, __TEXT("Xml"), 0, szTempName);

		m_RemoteFiles.ClearAllList();

		m_prsCur.SetPos(0);
		m_prsTotal.SetPos(0);

		m_dwAlreadyDownLen = 0;
		CInetFile *pInetFileNew = new CInetFile();
		if (pInetFileNew) {
			pInetFileNew->m_bProgressMode = true;
			pInetFileNew->m_dlFileType = FILELIST_XML;
			pInetFileNew->m_hWnd = m_hWnd;
			pInetFileNew->m_sURL = "http://127.0.0.1/demo/RedHadoop.xml";
			pInetFileNew->m_sServerName =serverName;
			pInetFileNew->m_sPath = szTempName;
			pInetFileNew->m_dwTotalLen = 0;
			pInetFileNew->m_sFileName = _T("");
			m_btnNowUpdate.EnableWindow(false);
			//m_cmbSelServer.EnableWindow(false);
			m_InetFiles.Push(pInetFileNew);
		}

}

void CAutoUpdateClientDlg::AddDownServerListQueue(CString sServerListURL)
{
	/*TCHAR szTempPath[MAX_PATH], szTempName[MAX_PATH];
	::GetTempPath(
	MAX_PATH,
	szTempPath);

	GetTempFileName(szTempPath, __TEXT("Xml"), 0, szTempName);

	m_RemoteFiles.ClearAllList();

	m_prsCur.SetPos(0);
	m_prsTotal.SetPos(0);
	CString sLog; sLog.Format(_T("�������ӻ�ȡ�������б�")); AddLog(sLog);
	m_dwAlreadyDownLen = 0;
	CInetFile *pInetFileNew = new CInetFile();
	if (pInetFileNew) {
	pInetFileNew->m_bProgressMode = true;
	pInetFileNew->m_dlFileType = SERVERLIST_XML;
	pInetFileNew->m_hWnd = m_hWnd;
	pInetFileNew->m_sURL = sServerListURL;
	pInetFileNew->m_sServerName = _T("");
	pInetFileNew->m_sPath = szTempName;
	pInetFileNew->m_dwTotalLen = 0;
	pInetFileNew->m_sFileName = _T("");
	m_btnNowUpdate.EnableWindow(false);
	m_cmbSelServer.EnableWindow(false);
	m_InetFiles.Push(pInetFileNew);
	}*/
}

LRESULT CAutoUpdateClientDlg::OnDownLoadFile(WPARAM wParam, LPARAM lParam) 
{ 
	CKevinAutoLock  lock(&m_lock);

	int nLen = LOWORD(wParam);
	DOWNLOAD_FILE_TYPE llFileType = (DOWNLOAD_FILE_TYPE)HIWORD(wParam);
	CInetFile *pInetFile = (CInetFile *)lParam;
	ASSERT(pInetFile);

	TRACE("wParam = %u, lParam = %u\n", wParam, lParam);
	if (pInetFile) {
		TCHAR szText[64];		_tcscpy(szText, _T(""));
		if (pInetFile->m_dwTotalLen > 0) {
			int nPercent = pInetFile->m_nAlreadyDown * 100 / pInetFile->m_dwTotalLen;
			m_prsCur.SetPos(nPercent);
		}

		if (GENERAL_FILE == llFileType) {
			m_dwAlreadyDownLen += nLen;
			if (m_dwTotalLen > 0) {
				int nPercent = m_dwAlreadyDownLen * 100 / m_dwTotalLen;
				m_prsTotal.SetPos(nPercent);
				_stprintf(szText, _T("%u%%"), nPercent);
			}
		}

		if ( m_bTray )
		{			
			static int nCount = 0;
			nCount ++;
			switch (nCount % 3)
			{
			case 0:
				//m_pTray.hIcon = LoadIcon( IDI_ICON1);
				break;
			case 1:
				//m_pTray.hIcon = LoadIcon( IDI_ICON);
				break;
			case 2:
				//m_pTray.hIcon = LoadIcon( IDI_ICON4);
				break;
			}

			m_pTray.uFlags = NIF_ICON;
			Shell_NotifyIcon( NIM_MODIFY, &m_pTray );

			if ( _tcsncmp( m_pTray.szTip, szText, 63 ) )
			{				
				m_pTray.uFlags = NIF_TIP;
				_tcsncpy( m_pTray.szTip, szText, 63 );
				Shell_NotifyIcon( NIM_MODIFY, &m_pTray );
			}
		}
	}

	return 0; 
} 

bool FileExist(CString sFileName)
{
	HANDLE hFile = CreateFile(sFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE) 
		return false;

	::CloseHandle(hFile);

	return true;
}

LRESULT CAutoUpdateClientDlg::OnDownLoadFileEnd(WPARAM wParam, LPARAM lParam) 
{ 
	// wParam 0 �ɹ�, 2, SESSION �쳣 3, ����ֹͣ
	CKevinAutoLock  lock(&m_lock);
	CString sLog; 

	int nRetCode = LOWORD(wParam);
	DOWNLOAD_FILE_TYPE llFileType = (DOWNLOAD_FILE_TYPE)HIWORD(wParam);
	CInetFile *pInetFile = (CInetFile *)lParam;
	ASSERT(pInetFile);

	switch(llFileType)
	{
	case SERVERLIST_XML:
		/*	{
		CStdioFile sf;
		if (sf.Open(pInetFile->m_sPath, CFile::modeRead)) {
		CString sRow;
		int nSucceedCount = 0;
		while (sf.ReadString(sRow)) {
		TRACE("%S\n", sRow);
		char szRow[1024];
		USES_CONVERSION;
		sprintf(szRow, "%S", sRow);
		sRow = szRow;	sRow.Trim();
		int nPos = sRow.Find(_T("|"));
		if (nPos > 0) {
		CString sServerName, sFileListURL;
		sServerName = sRow.Left(nPos);	
		sServerName.Trim();
		sFileListURL = sRow.Right(sRow.GetLength() - nPos - 1);
		sFileListURL.Trim();
		if (!sServerName.IsEmpty() && !sFileListURL.IsEmpty()) {
		if (m_Servers.AddServer(sServerName, sFileListURL)) {
		nSucceedCount ++;
		}
		}
		}
		}

		if (nSucceedCount > 0) {
		sLog.Format(_T("��ȡ�������б�ɹ�,��ȡ�� %u ����ѡ������"), nSucceedCount);
		AddLog(sLog, nRetCode);
		RefreshCtrl(); 
		if (theApp.m_bAutoRun) {
		OnBnClickedBtnNowupdate();
		}
		}
		else {
		AddLog(_T("��÷������б�ʧ��"), 0);
		}

		sf.Close();
		::DeleteFile(pInetFile->m_sPath);
		}
		}*/
		break;

	case FILELIST_XML:		
		{
			if (0 == nRetCode) {
				sLog.Format(_T("��ȡ�����ļ��б�ɹ���%s"),pInetFile->m_sServerName );
				AddLog(sLog, nRetCode);
				if (m_RemoteFiles.ReadFromXMLFile((LPTSTR)(LPCTSTR)pInetFile->m_sPath)) {
					m_dwCount = m_RemoteFiles.CheckNeedUpdate();
					m_dwTotalLen = m_RemoteFiles.GetNeedUpdateTotalLength();
					if (0 == m_dwCount) {
						sLog.Format(_T("�㵱ǰ�İ汾�����µ�"));
						AddLog(sLog, nRetCode);
					}
					else {
						sLog.Format(_T("������ļ�����: %u, �ļ��ܳ���: %u KB"), m_dwCount, m_dwTotalLen / 1024);
						AddLog(sLog, nRetCode);
					}

					CUpdateFile *pUpdateFile = m_RemoteFiles.PopUpdateFile();
					while(pUpdateFile) {
						CInetFile *pInetFileNew = new CInetFile();
						if (pInetFileNew) {
							pInetFileNew->m_bProgressMode = true;
							pInetFileNew->m_dlFileType = GENERAL_FILE;
							pInetFileNew->m_hWnd = m_hWnd;
							pInetFileNew->m_sURL = pUpdateFile->m_sURL;
							pInetFileNew->m_sServerName = pInetFile->m_sServerName;
							pInetFileNew->m_sPath = pUpdateFile->m_sFilePath;
							pInetFileNew->m_dwTotalLen = (DWORD)pUpdateFile->m_ullSize;
							pInetFileNew->m_sFileName = pUpdateFile->m_sFileName;

							pInetFileNew->m_bRegister = pUpdateFile->m_bRegister;
							pInetFileNew->m_bZipFile = pUpdateFile->m_bZipFile;
							pInetFileNew->m_bForceOverwrite = pUpdateFile->m_bForceOverwrite;
							pInetFileNew->m_bExecute = pUpdateFile->m_bExecute;
							pInetFileNew->m_sExecutePar = pUpdateFile->m_sExecutePar;

							m_btnNowUpdate.EnableWindow(false);
							//m_cmbSelServer.EnableWindow(false);
							m_InetFiles.Push(pInetFileNew);

							SAFE_DELETE(pUpdateFile);
							pUpdateFile = m_RemoteFiles.PopUpdateFile();
						}
					}

					// ��Ҫ�����ļ���ӵ� m_InetFiles �б���
				}
				//::ShellExecute(m_hWnd, NULL, _T("NOTEPAD"), pInetFile->m_sPath, NULL, SW_SHOW);
				::DeleteFile(pInetFile->m_sPath);
			}
			else {
				sLog.Format(_T("%s��%s"), pInetFile->GetLastError(), pInetFile->m_sServerName);
				AddLog(sLog, nRetCode);
				//MessageBox(pInetFile->GetLastError(), _T("ϵͳ��ʾ"), MB_ICONERROR);
			}
		}
		break;

	case GENERAL_FILE:
		{
			if (0 == nRetCode) {			// ���سɹ�
				// ɾ��ԭ�����ļ�������
				USES_CONVERSION;
				if (FileExist(pInetFile->m_sPath)) {
					if (::DeleteFile(pInetFile->m_sPath)) {
						RenameAndExeFile(pInetFile);
					}
					else {
						sLog.Format(_T("ɾ���ļ�ʧ�� [%s]"), pInetFile->m_sFileName);
						AddLog(sLog, nRetCode);
					}
				}
				else {
					RenameAndExeFile(pInetFile);
				}
			}
			else{							// ����ʧ��
				sLog.Format(_T("�����ļ�ʧ�� [%s], ������� [%u]"), pInetFile->m_sFileName, nRetCode);
				AddLog(sLog, nRetCode);
			}
		}
		break;
	}

	m_InetFiles.DeleteItem(pInetFile);
	if (m_InetFiles.Count() == 0) {
		if (GENERAL_FILE == llFileType) {
			sLog.Format(_T("�������"));
			AddLog(sLog, nRetCode);
		}
		m_btnNowUpdate.EnableWindow();
		//m_cmbSelServer.EnableWindow();
	}

	m_nCurThreadCount --;
	return 0; 
} 




void CAutoUpdateClientDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case PUBLIC_TIMER:
		{
			CKevinAutoLock  lock(&m_lock);
			if (m_nCurThreadCount < m_nMaxDownThread) {
				CInetFile* pInetFile = m_InetFiles.Front();
				if (pInetFile && 0 == pInetFile->m_nDownloadTime) {	// ֻ���������ش���Ϊ  0 ���б�
					if (GENERAL_FILE == pInetFile->m_dlFileType) {
						CString sLog;
						sLog.Format(_T("���ڸ����ļ���%s"), pInetFile->m_sFileName);
						AddLog(sLog, 0);
					}
					if (pInetFile->GetInetFile())
						m_nCurThreadCount ++;
				}
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CAutoUpdateClientDlg::AddLog(CString sLog, int nErrCode)
{
	CString sAddString;
	CTime t = CTime::GetCurrentTime();
	sAddString.Format(_T("%.2u:%.2u:%.2u %s"), t.GetHour(), t.GetMinute(), t.GetSecond(), sLog);
	//sAddString.Format(_T("%s Servers:%u, InetFiles:%u, RemoteFiles:%u, %.2u:%.2u:%.2u"),
	//	sLog, m_Servers.Count(), m_InetFiles.Count(), m_RemoteFiles.Count(),
	//	t.GetHour(), t.GetMinute(), t.GetSecond());
	USES_CONVERSION;
	TRACE("%s\n", (sAddString));
	int nItem = m_lstLog.AddString(sAddString);
	m_lstLog.SetItemData(nItem, nErrCode);
	m_lstLog.SetCurSel(nItem);
}

void CAutoUpdateClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	UINT nCommand = nID & 0xFFF0;
	BOOL bShift = GetAsyncKeyState( VK_SHIFT ) & 0x8000;

	if ( nCommand == SC_MINIMIZE || ( nCommand == SC_MINIMIZE && bShift ) )
	{
		return;
	}

	CDialog::OnSysCommand(nID, lParam);
}

BOOL CAutoUpdateClientDlg::DestroyWindow()
{
	if ( m_bTray )
	{
		Shell_NotifyIcon( NIM_DELETE, &m_pTray );
		m_bTray = false;
	}

	return CDialog::DestroyWindow();
}

void CAutoUpdateClientDlg::RenameAndExeFile(CInetFile* pInetFile)
{
	ASSERT(pInetFile);

	CString sLog;
	USES_CONVERSION;
	if (0 == ::rename((pInetFile->m_sPath + _T(".update")), (pInetFile->m_sPath))) {
		sLog.Format(_T("�����ļ��ɹ� [%s]"), pInetFile->m_sFileName);
		AddLog(sLog, 0);
		if (pInetFile->m_bExecute) {
			::ShellExecute(NULL, NULL, pInetFile->m_sPath, pInetFile->m_sExecutePar, NULL, SW_SHOW);
		}
		if (pInetFile->m_bRegister) {
			::ShellExecute(NULL, NULL, _T("regsvr32"), _T("/s ") + pInetFile->m_sPath, NULL, SW_SHOW);
		}
	}
	else {
		sLog.Format(_T("�������ļ�ʧ�� [%s]"), pInetFile->m_sFileName);
		AddLog(sLog, 0);
	}
}
