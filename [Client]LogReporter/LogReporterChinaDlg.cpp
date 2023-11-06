// LogReporterChinaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LogReporterGlobal.h"
#include "logreporter.h"
#include "SkinStatic.h"
#include "LogReporterChinaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CHINA_DIALOG_WIDTH	189
#define CHINA_DIALOG_HEIGHT 110

/////////////////////////////////////////////////////////////////////////////
// CLogReporterChinaDlg dialog


CLogReporterChinaDlg::CLogReporterChinaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogReporterChinaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogReporterChinaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	// Custom variables initializing.

	//CRect		m_rcCustomCaption;
	memset(&m_rcCustomCaption, 0, sizeof(m_rcCustomCaption));	

	m_hBmpBackScreen		= NULL;

	m_hBmpEnterBtn			= NULL;
	m_hBmpEnterBtnPress		= NULL;

	m_hBmpQuitBtn			= NULL;
	m_hBmpQuitBtnPress		= NULL;

	m_hBmpSouthCheck		= NULL;
	m_hBmpSouthCheckPress	= NULL;

	m_hBmpNorthCheck		= NULL;
	m_hBmpNorthCheckPress	= NULL;	
}


void CLogReporterChinaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogReporterChinaDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
//#ifdef _FOR_CLIENT_	
	DDX_Control(pDX, IDC_STATIC_ENTER, m_txtEnter);
	DDX_Control(pDX, IDC_STATIC_QUIT, m_txtQuit);

	DDX_Control(pDX, IDC_STATIC_TITLE, m_txtTitle);
	DDX_Control(pDX, IDC_STATIC_SOUTH_FTP, m_txtSouthFtp);
	DDX_Control(pDX, IDC_STATIC_NORTH_FTP, m_txtNorthFtp);

	DDX_Control(pDX, IDC_STATIC_BACKSCREEN, m_picBackScreen);	
	DDX_Control(pDX, IDC_BUTTON_SELECT, m_btnSelect);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_btnQuit);

	DDX_Control(pDX, IDC_CHECK_FTP_01, m_btnSouthCheck);	
	DDX_Control(pDX, IDC_CHECK_FTP_02, m_btnNorthCheck);		
//#endif
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogReporterChinaDlg, CDialog)
	//{{AFX_MSG_MAP(CLogReporterChinaDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)	
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_CHECK_FTP_01, OnCheckFtp01)
	ON_BN_CLICKED(IDC_CHECK_FTP_02, OnCheckFtp02)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogReporterChinaDlg message handlers

BOOL CLogReporterChinaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
		
	SetWindowText( CAPTION );	

	int iX = GetSystemMetrics( SM_CXSCREEN );
	int iY = GetSystemMetrics( SM_CYSCREEN );
	
	int cx = CHINA_DIALOG_WIDTH;
	int cy = CHINA_DIALOG_HEIGHT;
	SetWindowPos( NULL, ( iX - cx ) / 2, ( iY - cy ) / 2, cx, cy, SWP_NOZORDER );	


	LoadRequireImages();	
	SetRect(m_rcCustomCaption, 0, 0, 189, 21);
	
	// check box properties.	
		
	DWORD dwBStyle = m_btnSouthCheck.GetButtonStyle();
	dwBStyle |= BS_OWNERDRAW;
	dwBStyle |= BS_CHECKBOX;
	m_btnSouthCheck.SetButtonStyle(dwBStyle);

	m_btnSouthCheck.SetWindowPos(NULL, 16, 36, 140, 16, SWP_NOZORDER);

	dwBStyle = m_btnNorthCheck.GetButtonStyle();
	dwBStyle |= BS_OWNERDRAW;
	dwBStyle |= BS_CHECKBOX;
	m_btnNorthCheck.SetButtonStyle(dwBStyle);

	m_btnNorthCheck.SetWindowPos(NULL, 16, 56, 140, 16, SWP_NOZORDER);	

	m_btnSouthCheck.ShowWindow(SW_SHOW);
	m_btnNorthCheck.ShowWindow(SW_SHOW);

	// title
	m_txtTitle.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtTitle.SetWindowText( "请选择更新服务器" );	
	m_txtTitle.SetWindowPos(NULL, 45, 5, 120, 14, SWP_NOZORDER);
	
	// south ftp	
	m_txtSouthFtp.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtSouthFtp.SetWindowText( "更新服务器1（南方/电信）" );
	m_txtSouthFtp.SetWindowPos(NULL, 38, 36, 122, 14, SWP_NOZORDER);

	// north ftp
	m_txtNorthFtp.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtNorthFtp.SetWindowText( "更新服务器2（北方/网通）" );	
	m_txtNorthFtp.SetWindowPos(NULL, 38, 56, 122, 14, SWP_NOZORDER);

	// enter	
	m_txtEnter.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtEnter.SetWindowText( "确定" );	
	m_txtEnter.SetWindowPos(NULL, 50, 87, 60, 17, SWP_NOZORDER);

	// quit	
	m_txtQuit.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtQuit.SetWindowText( "退出" );
	m_txtQuit.SetWindowPos(NULL, 125, 87, 60, 17, SWP_NOZORDER);
	
	

	//CheckFirstRadioButton();
	m_bSouthSelect			= TRUE;
	SetCheckButton(m_bSouthSelect);	

	SelectGFunc();

	m_dwStartTime	= GetTickCount();
	m_bEnd			= FALSE;
	IngValue		= 1;

	OnStartTimer(m_hWnd);
	StartLog(this);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLogReporterChinaDlg::SetCheckButton(BOOL bSouthSelect)
{
	HWND hWnd;	

	if (TRUE == bSouthSelect)
	{
		hWnd = ::GetDlgItem(GetSafeHwnd(), IDC_CHECK_FTP_01);
		::SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_CHECKED, NULL);

		hWnd = ::GetDlgItem(GetSafeHwnd(), IDC_CHECK_FTP_02);
		::SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_UNCHECKED, NULL);	
	}
	else
	{
		hWnd = ::GetDlgItem(GetSafeHwnd(), IDC_CHECK_FTP_01);
		::SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_UNCHECKED, NULL);

		hWnd = ::GetDlgItem(GetSafeHwnd(), IDC_CHECK_FTP_02);
		::SendMessage(hWnd, BM_SETCHECK, (WPARAM)BST_CHECKED, NULL);		
	}	
	
	m_btnSouthCheck.Invalidate();
	m_btnNorthCheck.Invalidate();	

}

void CLogReporterChinaDlg::OnButtonSelect() 
{
	// First, Check Radio button.	

	/*
	UINT uiRet = IsDlgButtonChecked(IDC_CHECK_FTP_01);
	
	char szArgument[_MAX_PATH];
	memset(szArgument, 0, _MAX_PATH);

	// First radio button is checked!
	if (BST_CHECKED == uiRet)
	{
		strncpy(szArgument, "CHINA_FTP_01", _MAX_PATH);
	}
	else // Second radio button is checked!
	{
		strncpy(szArgument, "CHINA_FTP_02", _MAX_PATH);
	}*/

	char szArgument[_MAX_PATH];
	memset(szArgument, 0, _MAX_PATH);

	if (TRUE == m_bSouthSelect)
	{
		strncpy(szArgument, "CHINA_FTP_01", _MAX_PATH);
	}
	else
	{
		strncpy(szArgument, "CHINA_FTP_02", _MAX_PATH);
	}

	// Unload used resource.
	OnStopTimer(m_hWnd);
	UnloadRequireImages();

	// And execute Autopatch program.

#ifdef _DEBUG
	ExecuteMHAutoPatchEXE(LAUNCHER_DEBUG, szArgument);
#else
	ExecuteMHAutoPatchEXE(LAUNCHER_RELEASE, szArgument);
#endif
	

	OnOK();	
}

void CLogReporterChinaDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	/*
	DWORD curTick = GetTickCount();
	if(curTick - m_dwStartTime > 30000)
	{
		//MessageBox("辑滚俊 肺弊甫 焊郴绰 单 角菩沁嚼聪促.", NULL, MB_OK);
		m_bEnd = TRUE;
	}

	if(m_bEnd)
	{
		OnStopTimer(m_hWnd);
		ExecuteMHAutoPatchEXE();
		PostMessage(WM_CLOSE);
		m_bEnd = FALSE;
	}*/

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

void CLogReporterChinaDlg::OnButtonQuit() 
{
	OnStopTimer(m_hWnd);
	UnloadRequireImages();
	OnCancel();
}


void CLogReporterChinaDlg::LoadRequireImages(void)
{	
	// Back screen.
	m_hBmpBackScreen		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-01.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_picBackScreen.SetBitmap( m_hBmpBackScreen );
	m_picBackScreen.SetWindowPos(NULL, 0, 0, CHINA_DIALOG_WIDTH, CHINA_DIALOG_HEIGHT, SWP_NOZORDER);

	// Pressed Check box images.
	m_hBmpSouthCheckPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-02.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpNorthCheckPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-02.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// not pressed check box images.
	m_hBmpSouthCheck		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-03.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpNorthCheck		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-03.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// not pressed button images.
	m_hBmpEnterBtn			= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-04.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpQuitBtn			= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-04.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	// pressed button images.
	m_hBmpEnterBtnPress		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-05.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpQuitBtnPress		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-ch-05.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	
}

void CLogReporterChinaDlg::UnloadRequireImages(void)
{
	if (NULL != m_hBmpBackScreen)
	{
		DeleteObject(m_hBmpBackScreen);
		m_hBmpBackScreen = NULL;
	}

	if (NULL != m_hBmpSouthCheckPress)
	{
		DeleteObject(m_hBmpSouthCheckPress);
		m_hBmpSouthCheckPress = NULL;
	}

	if (NULL != m_hBmpNorthCheckPress)
	{
		DeleteObject(m_hBmpNorthCheckPress);
		m_hBmpNorthCheckPress = NULL;
	}

	if (NULL != m_hBmpSouthCheck)
	{
		DeleteObject(m_hBmpSouthCheck);
		m_hBmpSouthCheck = NULL;
	}


	if (NULL != m_hBmpNorthCheck)
	{
		DeleteObject(m_hBmpNorthCheck);
		m_hBmpNorthCheck = NULL;
	}


	if (NULL != m_hBmpEnterBtn)
	{
		DeleteObject(m_hBmpEnterBtn);
		m_hBmpEnterBtn = NULL;
	}


	if (NULL != m_hBmpQuitBtn)
	{
		DeleteObject(m_hBmpQuitBtn);
		m_hBmpQuitBtn = NULL;
	}

	if (NULL != m_hBmpEnterBtnPress)
	{
		DeleteObject(m_hBmpEnterBtnPress);
		m_hBmpEnterBtnPress = NULL;
	}

	if (NULL != m_hBmpQuitBtnPress)
	{
		DeleteObject(m_hBmpQuitBtnPress);
		m_hBmpQuitBtnPress = NULL;
	}
}

void CLogReporterChinaDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default

	CDC MemDC;
	
	m_txtTitle.Invalidate();	

	switch (nIDCtl)
	{
	case IDC_BUTTON_SELECT:
		{
			MemDC.CreateCompatibleDC( m_btnSelect.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )				
				MemDC.SelectObject( m_hBmpEnterBtnPress );
			else				
				MemDC.SelectObject( m_hBmpEnterBtn );

			m_btnSelect.GetDC()->BitBlt( 0, 0, 60, 17, &MemDC, 0, 0, SRCCOPY );
			m_txtEnter.Invalidate();
		}
		break;

	case IDC_BUTTON_QUIT:
		{
			MemDC.CreateCompatibleDC( m_btnQuit.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )				
				MemDC.SelectObject( m_hBmpEnterBtnPress );
			else
				MemDC.SelectObject( m_hBmpEnterBtn );

			m_btnQuit.GetDC()->BitBlt( 0, 0, 60, 17, &MemDC, 0, 0, SRCCOPY );
			m_txtQuit.Invalidate();
		}

		break;

	case IDC_CHECK_FTP_01:
		{
			
			MemDC.CreateCompatibleDC( m_btnSouthCheck.GetDC() );		

			if (TRUE == m_bSouthSelect)
			{
				MemDC.SelectObject( m_hBmpSouthCheckPress );

				m_txtSouthFtp.SetFontColor( RGB(255, 255, 0) );
				m_txtNorthFtp.SetFontColor( RGB(255, 255, 255) );

				m_txtSouthFtp.Invalidate();
				m_txtNorthFtp.Invalidate();
			}
			else
			{
				MemDC.SelectObject( m_hBmpSouthCheck );
			}

			m_btnSouthCheck.GetDC()->BitBlt( 0, 0, 14, 14, &MemDC, 0, 0, SRCCOPY );							
		}
		break;

	case IDC_CHECK_FTP_02:
		{	
			
			MemDC.CreateCompatibleDC( m_btnNorthCheck.GetDC() );		

			if (FALSE == m_bSouthSelect)
			{
				MemDC.SelectObject( m_hBmpNorthCheckPress );

				m_txtSouthFtp.SetFontColor( RGB(255, 255, 255) );
				m_txtNorthFtp.SetFontColor( RGB(255, 255, 0) );

				m_txtSouthFtp.Invalidate();
				m_txtNorthFtp.Invalidate();
			}
			else
			{
				MemDC.SelectObject( m_hBmpNorthCheck );
			}

			m_btnNorthCheck.GetDC()->BitBlt( 0, 0, 14, 14, &MemDC, 0, 0, SRCCOPY );									
		}		

		break;
		
	}
	
	MemDC.DeleteDC();
	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
	
}

void CLogReporterChinaDlg::OnCheckFtp01() 
{
	m_bSouthSelect = TRUE;
	SetCheckButton(m_bSouthSelect);
}

void CLogReporterChinaDlg::OnCheckFtp02() 
{
	m_bSouthSelect = FALSE;
	SetCheckButton(m_bSouthSelect);
}



HBRUSH CLogReporterChinaDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);    

    if(pWnd == &m_btnSouthCheck)
    {
        pDC->SetBkMode(TRANSPARENT); 

        CBrush MyBrush;

        MyBrush.CreateStockObject(NULL_BRUSH);

        hbr = MyBrush;
    }

	if (pWnd == &m_btnNorthCheck)
	{
		pDC->SetBkMode(TRANSPARENT); 

        CBrush MyBrush;

        MyBrush.CreateStockObject(NULL_BRUSH);

        hbr = MyBrush;
	} 

    return hbr;

}

void CLogReporterChinaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rcCustomCaption.PtInRect(point))
	{
		SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
		CDialog::OnLButtonDown(nFlags, point);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CLogReporterChinaDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if( m_rcCustomCaption.PtInRect( point ) )
	{
		SendMessage( WM_NCLBUTTONUP, HTCAPTION, 0 );
		CDialog::OnLButtonUp(nFlags, point);
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}
