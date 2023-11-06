// LogPetPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogPetPage.h"
#include "RMNetwork.h"
#include "LogDoc.h"
#include ".\logpetpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogPetPage property page

IMPLEMENT_DYNCREATE(CLogPetPage, CLDPropPage)


CLogPetPage::CLogPetPage() : CLDPropPage(CLogPetPage::IDD)
{
	//{{AFX_DATA_INIT(CLogPetPage)
	m_bCharName = FALSE;
	m_bItemName = FALSE;
	m_bDBIdx = FALSE;
	m_bOneDay = FALSE;
	m_bTime = FALSE;
	//}}AFX_DATA_INIT
	m_PetLogTable.Initialize( 1000 );
	m_pDocument = NULL;
	m_bStart = FALSE;

	for( int i = 0; i < 14; ++i )
		m_bVOption[i] = TRUE;
}

CLogPetPage::~CLogPetPage()
{
	ReleaseLogTable();
}

void CLogPetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogPetPage)
	DDX_Control(pDX, IDC_EDIT_E_MIN, m_etEMin);
	DDX_Control(pDX, IDC_EDIT_E_HOUR, m_etEHour);
	DDX_Control(pDX, IDC_EDIT_S_MIN, m_etSMin);
	DDX_Control(pDX, IDC_EDIT_S_HOUR, m_etSHour);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Control(pDX, IDC_EDIT_CHARIDX, m_etCharIdx);
	DDX_Control(pDX, IDC_COMBO_LOG_ITEMNAME, m_cbItemName);
	DDX_Control(pDX, IDC_EDIT_LOG_DBIDX, m_etDBIdx);
	DDX_Control(pDX, IDC_EDIT_LOG_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_LISTCTRL_PETLOG, m_lcPetLog);
	DDX_Check(pDX, IDC_CHECK_LOG_CHARNAME, m_bCharName);
	DDX_Check(pDX, IDC_CHECK_LOG_ITEMNAME, m_bItemName);
	DDX_Check(pDX, IDC_CHECK_LOG_DBIDX, m_bDBIdx);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	DDX_Check(pDX, IDC_CHECK_TIME, m_bTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogPetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLogPetPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_LOG_CHARNAME, OnCheckLogCharname)
	ON_BN_CLICKED(IDC_CHECK_LOG_ITEMNAME, OnCheckLogItemname)
	ON_BN_CLICKED(IDC_CHECK_LOG_DBIDX, OnCheckLogDbidx)
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, OnBtnSavetofile)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogPetPage message handlers

int CLogPetPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CLogPetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// 
	m_bOneDay = TRUE;
	m_bCharName = TRUE;
	m_etCharName.EnableWindow( m_bCharName );
	InitComboItemName();

//	m_Font.CreatePointFont(100, "Arial");
//	m_lcPetLog.SetFont( &m_Font );


	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogPetPage::InitComboItemName()
{
	m_cbItemName.ResetContent();

	CYHHashTable<PET_NAME>* pPetName = GAMERESRCMNGR->GetPetName();
	PET_NAME* pData = NULL;
	int index = 0;
	pPetName->SetPositionHead();
	while( pData = pPetName->GetData() )
	{
		m_cbItemName.AddString( pData->ItemName );
		m_cbItemName.SetItemData( index, pData->PetIdx );
		++index;			
	}
	m_cbItemName.SetCurSel( 0 );
}

void CLogPetPage::OnCheckLogOneday() 
{
	m_bOneDay = !m_bOneDay;
//	m_StartTime.EnableWindow( !m_bOneDay );
	m_EndTime.EnableWindow( !m_bOneDay );

	if( m_bOneDay == FALSE )
	{
		m_bTime = FALSE;
		m_etSHour.EnableWindow( FALSE );
		m_etSMin.EnableWindow( FALSE );
		m_etEHour.EnableWindow( FALSE );
		m_etEMin.EnableWindow( FALSE );
	}
	UpdateData( FALSE );
}

void CLogPetPage::OnCheckLogCharname() 
{
	m_bCharName = !m_bCharName;
	m_etCharName.EnableWindow( m_bCharName );
	UpdateData( FALSE );	
}

void CLogPetPage::OnCheckLogItemname() 
{
	//log table에 kind 필드가 없는 관계로 우선 사용 안하기로 함
	// 검색 모듈은 다되어있음(테스트만 못했음)
	/*
	m_bItemName = !m_bItemName;
	if( m_bItemName )
		m_bDBIdx = FALSE;
	m_cbItemName.EnableWindow( m_bItemName );
	m_etDBIdx.EnableWindow( m_bDBIdx );
	UpdateData( FALSE );	
	*/
}

void CLogPetPage::OnCheckLogDbidx() 
{
	m_bDBIdx = !m_bDBIdx;
	if( m_bDBIdx )
		m_bItemName = FALSE;	
	m_etDBIdx.EnableWindow( m_bDBIdx );
	m_cbItemName.EnableWindow( m_bItemName );	
	UpdateData( FALSE );	
}

void CLogPetPage::OnButtonLogSearch() 
{
	CString sName;
	DWORD dwItem = 0;

	// logtype
	DWORD dwLogType;
	if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == FALSE )
	{
		MessageBox( "Enter The Search Data!!", "Error" );
		return;
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == FALSE )		// 이름만,
	{
		dwLogType = eSearchType_ChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ChrNameItemDBIdx;
		if( m_etCharName.GetWindowTextLength() == 0 || m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
			m_etCharName.GetWindowText( sName );
		}
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ChrNameItemIdx;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ItemDBIdx;
		if( m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ItemIdx;
		dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );		
	}

	// Search
	if( dwLogType == eSearchType_ItemDBIdx || dwLogType == eSearchType_ItemIdx )
	{
		SetCharacterIdx( 0 );
	}
	else
	{
		m_etCharName.GetWindowText( sName );
	
		// Search
		TMSG_CHECKNAME Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_SYN;
		Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		strcpy( Msg.sName, (char*)(LPCTSTR)sName );

		NETWORK->Send( &Msg, sizeof(TMSG_CHECKNAME) );

		m_pDocument->m_dwPage = 3;
	}
}

void CLogPetPage::SetCharacterIdx( DWORD dwCharIdx )
{
//	if( dwCharIdx == 0 )
//		return;

	char temp[128] = {0, };
	sprintf( temp, "%d", dwCharIdx );
	m_etCharIdx.SetWindowText( temp );
	m_etCharIdx.EnableWindow( TRUE );
	m_dwCharIdx = dwCharIdx;

	CString sName, sSDate, sEDate;
	DWORD dwItem = 0;
	// Date
	if( m_bOneDay )
	{
		m_StartTime.GetWindowText( sSDate );
		sEDate.Empty();		
	}
	else
	{
		m_StartTime.GetWindowText( sSDate );
		m_EndTime.GetWindowText( sEDate );
	}

	// logtype
	DWORD dwLogType;
	if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == FALSE )
	{
		MessageBox( "Enter The Search Data!!", "Error" );
		return;
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == FALSE )		// 이름만,
	{
		dwLogType = eSearchType_ChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ChrNameItemDBIdx;
		if( m_etCharName.GetWindowTextLength() == 0 || m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
			m_etCharName.GetWindowText( sName );
		}
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ChrNameItemIdx;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ItemDBIdx;
		if( m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ItemIdx;
		dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );		
	}
	
	TMSG_GAMELOGSEARCH GameLogSearch;

	if( m_bTime )
	{
		CString time;
		int nSHour, nSMin, nEHour, nEMin;
		m_etSHour.GetWindowText( time );
		nSHour = atoi( (char*)(LPCTSTR)time );
		m_etSMin.GetWindowText( time );
		nSMin = atoi( (char*)(LPCTSTR)time );
		m_etEHour.GetWindowText( time );
		nEHour = atoi( (char*)(LPCTSTR)time );
		m_etEMin.GetWindowText( time );
		nEMin = atoi( (char*)(LPCTSTR)time );
		if( nSHour >= 0 && nSHour < 24 && nSMin >= 0 && nSMin < 60 &&
			nEHour >= 0 && nEHour < 24 && nEMin >= 0 && nEMin < 60 )
		{
			if( nSHour > nEHour )
			{
				MessageBox( "Enter Valid Time!!", "Error" );
				return;
			}
			else if( nSHour == nEHour )
			{
				if( nSMin > nEMin )
				{
					MessageBox( "Enter Valid Time!!", "Error" );
					return;
				}
				else
				{
				}
			}
			else
			{				
			}
		}
		else
		{
			MessageBox( "Enter Valid Time!!", "Error" );
			return;
		}		

		char sTime[30] = {0,};
		char eTime[30] = {0,};

		sprintf( sTime, "%s %d:%d:0", (char*)(LPCTSTR)sSDate, nSHour, nSMin );
		sprintf( eTime, "%s %d:%d:0", (char*)(LPCTSTR)sSDate, nEHour, nEMin );

		// Search		
		GameLogSearch.Category = MP_RMTOOL_GAMELOG;
		GameLogSearch.Protocol = MP_RMTOOL_PETLOGFROMTYPE_SYN;
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = (WORD)dwLogType;
		GameLogSearch.dwChrIdx = m_dwCharIdx;
		GameLogSearch.dwItem = dwItem;
		GameLogSearch.bDayOnly = FALSE;
		strcpy( GameLogSearch.sSDate, sTime );
		strcpy( GameLogSearch.sEDate, eTime );
	}
	else
	{
		// Search
		GameLogSearch.Category = MP_RMTOOL_GAMELOG;
		GameLogSearch.Protocol = MP_RMTOOL_PETLOGFROMTYPE_SYN;
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = (WORD)dwLogType;
		GameLogSearch.dwChrIdx = m_dwCharIdx;
		GameLogSearch.dwItem = dwItem;
		GameLogSearch.bDayOnly = m_bOneDay;
		strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
		strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );
	}

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	ReleaseLogTable();

	m_pDocument->StartDlg();
}

void CLogPetPage::SetLogPet( TMSG_LOGPET* pMsg )
{
//	if( !pMsg->bEnd )
//		ReleaseLogTable();
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		LOGPET* pData = new LOGPET;
		memcpy( pData, &pMsg->sLog[i], sizeof(LOGPET) );

		m_PetLogTable.Add( pData, pData->dwIdx );
	}

	InitLCPetLog();
	UpdateLCPetLog( eLog_Max );

}

void CLogPetPage::ReleaseLogTable()
{
	LOGPET* pData = NULL;
	m_PetLogTable.SetPositionHead();
	while( pData = m_PetLogTable.GetData() )
		delete pData;
	m_PetLogTable.RemoveAll();
}

void CLogPetPage::InitLCPetLog()
{
	m_lcPetLog.DeleteAllItems();

	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcPetLog.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcPetLog.DeleteColumn( 0 );

	char* tcolumn[11] = { "Idx", "LogType", "PetIdx", "PetSummonItemDBIdx", "UserIdx", "Characteridx",
		"Grade", "FriendShip", "Stamina", "Alive", "LogDate" };
	int tcolumnsize[11] = { 70, 100, 70, 100, 70, 70, 50, 80, 70, 50, 130};
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	int index = 0;
	for( i = 0; i < 11; ++i )
	{
		if( m_bVOption[i] )
		{
			lvcolumn.iSubItem = index;
			lvcolumn.cx = tcolumnsize[i];
			lvcolumn.pszText = tcolumn[i];
			m_lcPetLog.InsertColumn( index, &lvcolumn );
			++index;
		}
	}
	// style
	DWORD dwExStyle = m_lcPetLog.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcPetLog.SetExtendedStyle( dwExStyle );
}

void CLogPetPage::UpdateLCPetLog( DWORD dwHow )
{
	m_lcPetLog.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;
	LOGPET* pData = NULL;
	m_PetLogTable.SetPositionHead();
	while( pData = m_PetLogTable.GetData() )
	{
		int i = 0;
		int index = 0;
		if( pData->dwLogType == dwHow || dwHow == eLog_Max )
		{
			lvitem.iItem = m_lcPetLog.GetItemCount();	// 행

			lvitem.mask = LVIF_TEXT;
			lvitem.stateMask = LVIS_SELECTED;
			lvitem.state = INDEXTOSTATEIMAGEMASK(1);
			lvitem.iSubItem = i;
			
			for( i = 0; i < 14; ++i )
			{
				if( m_bVOption[i] )
					break;
			}
			lvitem.pszText = GetLCData( pData, i, temp );
			m_lcPetLog.InsertItem( &lvitem );
	
			for( int j = i+1; j < 14; ++j )
			{
				if( m_bVOption[j] )
					m_lcPetLog.SetItemText( lvitem.iItem, ++index, GetLCData( pData, j, temp ) );
			}
		}
	}
}

char* CLogPetPage::GetLCData( LOGPET* pData, DWORD dwIndex, char* temp )
{
	switch( dwIndex )
	{
	case 0:		return itoa( pData->dwIdx, temp, 10 );
	case 1:	
		{
			if( pData->dwIdx == 0 )
			{
				sprintf( temp, "-");
				return temp;
			}
			else
				return GAMERESRCMNGR->GetPetLogType( pData->dwLogType, temp );
		}
		break;

	case 2:		return itoa( pData->dwPetidx, temp, 10 );
	case 3:		return itoa( pData->dwPetSummonItemDBIdx, temp, 10 );
	case 4:		return itoa( pData->dwUserIdx, temp, 10 );
	case 5:		return itoa( pData->dwCharacterIdx, temp, 10 );
	case 6:		return itoa( pData->wGrade, temp, 10 );
	case 7:		return itoa( pData->wFriendShip, temp, 10 );
	case 8:		return itoa( pData->wStamina, temp, 10 );
	case 9:		return itoa( pData->wAlive, temp, 10 );
	case 10:	return pData->sLogDate;
	default:	sprintf( temp, "");	return temp;
	}	
}

char* CLogPetPage::GetItemName( DWORD dwItemIdx, char* temp )
{
	if( dwItemIdx == 0 )
	{
		sprintf( temp, "Money" );
		return temp;
	}
	ITEM_INFO* pItem = GAMERESRCMNGR->GetItemInfo( (WORD)dwItemIdx );

	if( pItem == NULL )
		sprintf( temp, "???" );
	else
		sprintf( temp, "%s", pItem->ItemName );

	return temp;
}

void CLogPetPage::CommandMessage( UINT nID, int nCode )
{
}

void CLogPetPage::OnBtnSavetofile() 
{

	DeleteFile( "PetLog.txt" );

	FILE* fp = fopen( "PetLog.txt", "a+" );
	if( !fp )
		return;

	char* tcolumn[11] = { "Idx", "LogType", "PetIdx", "PetSummonItemDBIdx", "UserIdx", "Characteridx",
		"Grade", "FriendShip", "Stamina", "Alive", "LogDate" };

	fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2],
		tcolumn[3], tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7], tcolumn[8], tcolumn[9], tcolumn[10]);

	char temp1[128] = {0, };
	LOGPET* pData = NULL;
	m_PetLogTable.SetPositionHead();
	while( pData = m_PetLogTable.GetData() )
	{
		fprintf( fp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", 
			pData->dwIdx,
			GAMERESRCMNGR->GetPetLogType( pData->dwLogType, temp1 ),
			pData->dwPetidx,
			pData->dwPetSummonItemDBIdx,
			pData->dwUserIdx,
			pData->dwCharacterIdx,
			pData->wGrade,
			pData->wFriendShip,
			pData->wStamina,
			pData->wAlive,
			pData->sLogDate);
	}

	fclose( fp );

}

void CLogPetPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcPetLog.SetFont( &m_Font );
}

void CLogPetPage::OnCheckTime() 
{
	if( m_bOneDay )
	{
		m_bTime = !m_bTime;	

		m_etSHour.EnableWindow( m_bTime );
		m_etSMin.EnableWindow( m_bTime );
		m_etEHour.EnableWindow( m_bTime );
		m_etEMin.EnableWindow( m_bTime );
	}
	else
	{
		m_bTime = FALSE;	

		m_etSHour.EnableWindow( m_bTime );
		m_etSMin.EnableWindow( m_bTime );
		m_etEHour.EnableWindow( m_bTime );
		m_etEMin.EnableWindow( m_bTime );
	}
	
	UpdateData( FALSE );
}

