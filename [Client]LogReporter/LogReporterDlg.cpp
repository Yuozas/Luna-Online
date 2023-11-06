// LogReporterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LogReporterGlobal.h"
#include "LogReporter.h"
#include "LogReporterDlg.h"

/*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogReporterDlg dialog

CLogReporterDlg::CLogReporterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogReporterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogReporterDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLogReporterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogReporterDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLogReporterDlg, CDialog)
	//{{AFX_MSG_MAP(CLogReporterDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogReporterDlg message handlers




#include ".\zbugziller.h"
#include <process.h>
#include <assert.h>
#include "HardwareInfo.h"
/*
struct FAILPAIR
{
	char oldfilename[MAX_PATH];
	char newfilename[MAX_PATH];
	void Set(char* oldf,char* newf)
	{
		strcpy(oldfilename,oldf);
		strcpy(newfilename,newf);
	}
};

#define AUTOPATCHEXEC	"MHAutoPatch.exe"

BOOL IsSupportSSE()
{
	BOOL	bResult = TRUE;
	__try 
	{
		// 	xorps	xmm0,xmm0
        __asm _emit 0x0f 
		__asm _emit 0x57 
		__asm _emit 0xc0

    }
	__except ( EXCEPTION_EXECUTE_HANDLER) 
	{
		bResult = FALSE;
		
	}
	return bResult;
}

BOOL SelectGFunc()
{
	if (IsSupportSSE())
		CopyFile("SS3DGFuncSSE.dll","SS3DGFunc.dll",FALSE);
	else
		CopyFile("SS3DGFuncN.dll","SS3DGFunc.dll",FALSE);
	return TRUE;
}


  */


BOOL CLogReporterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
/*
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
*/
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

/////
	//SetWindowText( "DarkStoryOnline" );
	SetWindowText( CAPTION );

	int iX = GetSystemMetrics( SM_CXSCREEN );
	int iY = GetSystemMetrics( SM_CYSCREEN );
	int cx = 171;
	int cy = 54;

	SetWindowPos( NULL, ( iX - cx ) / 2, ( iY - cy ) / 2, cx, cy, SWP_NOZORDER );

//	m_txtContext.SetWindowPos( NULL, 5, 15, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
//	m_txtPeriod.SetWindowPos( NULL, 155, 15, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

	// TODO: Add extra initialization here
	SelectGFunc();

	m_dwStartTime = GetTickCount();
	m_bEnd = FALSE;
	IngValue = 1;
	//OnStartTimer();
	OnStartTimer(m_hWnd);
	StartLog(this);
	//m_strIngStatic = "gfdgfdsg";
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
/*
void CLogReporterDlg::ExecuteMHAutoPatchEXE()
{
	//////////////////////////////////////////////////////////////////////////
	// 바꿀파일의 갯수
	int maxchangefile = 2;
	//////////////////////////////////////////////////////////////////////////
	
	int temp = 0;
	FAILPAIR files[2];
	files[temp++].Set("MHAutoPatch.exe","MHAutoPatch.new");
	files[temp++].Set("SS3DGFunc.dll","SS3DGFunc.new");	

	for(int n=0;n<maxchangefile;++n)
	{
		// TODO: Place code here.
		FILE* fp = fopen(files[n].newfilename,"rb");
		if(fp != NULL)
		{
			fclose(fp);
			while(1)
			{
				if(DeleteFile(files[n].oldfilename) == TRUE)
					break;
				Sleep(500);
			}
			BOOL rt = MoveFile(files[n].newfilename,files[n].oldfilename);
			assert(rt);
		}
	}

	char szDir[256];
	GetCurrentDirectory(256, szDir);
	CString strFullPath;
	strFullPath.Format("%s\\%s", szDir,AUTOPATCHEXEC);
	int rt = _execl((LPTSTR)(LPCTSTR)strFullPath,"a",NULL);

}

void CLogReporterDlg::OnStartTimer() 
{
	m_nTimer = SetTimer(1, 1000, 0);
}

void CLogReporterDlg::OnStopTimer() 
{
	KillTimer(m_nTimer);  
}

void CLogReporterDlg::StartLog()
{
	//CLogZiller logZ;
	//logZ.FtpConnect();
	//logZ.Upload("aa",CLIENTBUGFILE);
	//logZ.FtpDisconnect();
	DWORD id = 0;
	HANDLE h = CreateThread(0, 0, ftpThread, this, 0, &id);
	CloseHandle(h);
}

void CLogReporterDlg::WriteFile(const char *fn, int type, char * buffer)
{
	FILE	*fp;
	if(type == 0)
	{
		fp = fopen(fn, "wt");
		fwrite(buffer, strlen(buffer), 1, fp);
		fclose(fp);
	}
	else if(type == 1)
	{
		fp = fopen(fn,"a+t");
		fprintf(fp,"\n%s", buffer);
		fclose(fp);
	}
}

void ConvSpecFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile)
{
	CFileStatus aStatus;
	if ( CFile::GetStatus(szLocalFile, aStatus) == TRUE )
	{
		CTime creationTime = aStatus.m_ctime;
		CString strTime = "[";
		strTime += szKey;
		strTime += "].txt";
		strcpy(szRemoteFile, (LPTSTR)(LPCTSTR)strTime);
	}
}
void ConvBugFormatFile(char * szKey, char * szLocalFile, char * szRemoteFile)
{
	CFileStatus aStatus;
	if ( CFile::GetStatus(szLocalFile, aStatus) == TRUE )
	{
		CTime creationTime = aStatus.m_ctime;
		CString strTime = "[";
		strTime += szKey;
		strTime += "]";
		strTime += creationTime.Format("(%Y-%m-%d)%H-%M-%S");
		strTime += szLocalFile;
		strcpy(szRemoteFile, (LPTSTR)(LPCTSTR)strTime);
	}
} 
DWORD WINAPI CLogReporterDlg::ftpThread(LPVOID p)
{
	//CString commandLine = AfxGetApp()->m_lpCmdLine;
	//if ( commandLine == "" ) commandLine = "_NO_ID_";

	
	CHardwareInfo HWInfo;

	//BYTE MacAddress[64];
	//HWInfo.GetMacAddressFromNetBIOS(MacAddress);
	//HWInfo.GetMacAddressFromSock(MacAddress);

	CString commandLine = HWInfo.GetLocalIP();

	CString str = HWInfo.GetInfoToString();
	((CLogReporterDlg*)p)->WriteFile(SPECFILE, 0, (LPTSTR)(LPCTSTR)str);

	ZBugZiller aBug;
	aBug.SetFTPInfo(FTPSERVERIP, FTPSERVERPORT);
	aBug.SetAccount(FTPUSERID, FTPUSERPWD);
//	aBug.ConnectFTP();

	
	int cac = 0;//_access(CLIENTBUGFILE,0);
	int mac = _access(MAPCHANGEERRORFILE,0);
	int rac = _access(RECVERRORFILE,0);

	if(cac != -1 || mac != -1 || rac != -1)
	{
		char szConvFile[256];
		ConvSpecFormatFile((LPTSTR)(LPCTSTR)commandLine, SPECFILE, szConvFile);
		aBug.Upload(SPECFILE, szConvFile);
		//ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, CLIENTBUGFILE, szConvFile);
		//aBug.Upload(CLIENTBUGFILE, szConvFile);
		ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, MAPCHANGEERRORFILE, szConvFile);
		aBug.Upload(MAPCHANGEERRORFILE, szConvFile);
		ConvBugFormatFile((LPTSTR)(LPCTSTR)commandLine, RECVERRORFILE, szConvFile);
		aBug.Upload(RECVERRORFILE, szConvFile);
	}
	
	DeleteFile(CLIENTBUGFILE);
	//((CLogReporterDlg*)p)->OnStopTimer();
	//((CLogReporterDlg*)p)->ExecuteMHAutoPatchEXE();
	//((CLogReporterDlg*)p)->PostMessage(WM_CLOSE);

	((CLogReporterDlg*)p)->m_bEnd = TRUE;

	return 0;
}

  */
void CLogReporterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLogReporterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLogReporterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLogReporterDlg::OnTimer(UINT nIDEvent) 
{
	DWORD curTick = GetTickCount();
	if(curTick - m_dwStartTime > 30000)
	{
		//MessageBox("서버에 로그를 보내는 데 실패했습니다.", NULL, MB_OK);
		m_bEnd = TRUE;
	}

	if(m_bEnd)
	{
		OnStopTimer(m_hWnd);
#ifdef _DEBUG
		ExecuteMHAutoPatchEXE(LAUNCHER_DEBUG, "a");
#else
		ExecuteMHAutoPatchEXE(LAUNCHER_RELEASE, "a");
#endif
		PostMessage(WM_CLOSE);
		m_bEnd = FALSE;
	}

//	m_strIngStatic = "";
//	++IngValue;
//	if(IngValue>5)
//		IngValue = 1;
//	int i = 0;
//	while(i<IngValue)
//	{
//		m_strIngStatic += ".";
//		++i;
//	}
//	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CLogReporterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
