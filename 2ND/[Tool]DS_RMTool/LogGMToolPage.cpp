#include "stdafx.h"
#include "ds_rmtool.h"
#include "loggmtoolpage.h"
#include "RMNetwork.h"
#include "LogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cLogGMToolPage property page

IMPLEMENT_DYNCREATE(cLogGMToolPage, CLDPropPage)


cLogGMToolPage::cLogGMToolPage(void) : CLDPropPage(cLogGMToolPage::IDD)
{
	//{{AFX_DATA_INIT(cLogGMToolPage)
	m_bCharName = FALSE;
	m_bLogType = FALSE;
	m_bOneDay = FALSE;
	m_bTime = FALSE;
	//}}AFX_DATA_INIT
	m_LogTable.Initialize( 1000 );
	m_pDocument = NULL;
	m_bStart = FALSE;

	for( int i = 0; i < 14; ++i )
		m_bVOption[i] = TRUE;
}

cLogGMToolPage::~cLogGMToolPage(void)
{
	ReleaseLogTable();
}

void cLogGMToolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cLogGMToolPage)
	DDX_Control(pDX, IDC_EDIT_E_MIN, m_etEMin);
	DDX_Control(pDX, IDC_EDIT_E_HOUR, m_etEHour);
	DDX_Control(pDX, IDC_EDIT_S_MIN, m_etSMin);
	DDX_Control(pDX, IDC_EDIT_S_HOUR, m_etSHour);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Control(pDX, IDC_EDIT_CHARIDX, m_etCharIdx);
	DDX_Control(pDX, IDC_COMBO_LOG_LOGTYPE, m_cbLogType);
	DDX_Control(pDX, IDC_EDIT_LOG_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_LISTCTRL_GMTOOL, m_lcGMToolLog);
	DDX_Check(pDX, IDC_CHECK_LOG_CHARNAME, m_bCharName);
	DDX_Check(pDX, IDC_CHECK_LOG_ITEMNAME, m_bLogType);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	DDX_Check(pDX, IDC_CHECK_TIME, m_bTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cLogGMToolPage, CPropertyPage)
	//{{AFX_MSG_MAP(cLogGMToolPage)
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
// cLogGMToolPage message handlers

int cLogGMToolPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL cLogGMToolPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// 
	m_bOneDay = TRUE;
	m_bCharName = TRUE;
	m_etCharName.EnableWindow( m_bCharName );
	InitComboItemName();

	//	m_Font.CreatePointFont(100, "Arial");
	//	m_lcGMToolLog.SetFont( &m_Font );


	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cLogGMToolPage::InitComboItemName()
{
	m_cbLogType.ResetContent();

	CYHHashTable<char>* pItemInfoList = GAMERESRCMNGR->GetGMToolLogTypeTable();
	char* pData = NULL;
	int index = 0;
	pItemInfoList->SetPositionHead();
	while( pData = pItemInfoList->GetData() )
	{
		m_cbLogType.AddString( pData );
		m_cbLogType.SetItemData( index, index );
		++index;			
	}
	m_cbLogType.SetCurSel( 0 );
}

void cLogGMToolPage::OnCheckLogOneday() 
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

void cLogGMToolPage::OnCheckLogCharname() 
{
	m_bCharName = !m_bCharName;
	m_etCharName.EnableWindow( m_bCharName );
	UpdateData( FALSE );	
}

void cLogGMToolPage::OnCheckLogItemname() 
{
	m_bLogType = !m_bLogType;
	m_cbLogType.EnableWindow( m_bLogType );
	UpdateData( FALSE );	
}

void cLogGMToolPage::OnCheckLogDbidx() 
{
	m_cbLogType.EnableWindow( m_bLogType );	
	UpdateData( FALSE );	
}

void cLogGMToolPage::OnButtonLogSearch() 
{
	CString sName;
	DWORD dwItem = 0;

	// logtype
	DWORD dwLogType;
	if( m_bCharName == FALSE && m_bLogType == FALSE )
	{
		/// 060914 PKH 날짜에 해당하는 모든 캐릭터 조회
		dwLogType = eSearchType_GMToolNone;
	}
	else if( m_bCharName == TRUE  && m_bLogType == FALSE )		// 이름만,
	{
		/// 060914 PKH 날짜에 해당하는 입력된 캐릭터 조회
		dwLogType = eSearchType_GMToolChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bLogType == TRUE )
	{
		/// 060914 PKH 날짜에 해당하는 입력된 캐릭터와 로그타입으로 조회
		dwLogType = eSearchType_GMToolChrNameLogType;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbLogType.GetItemData( m_cbLogType.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bLogType == TRUE )
	{
		/// 060914 PKH 날짜에 해당하는 입력된 로그타입으로 조회
		dwLogType = eSearchType_GMToolLogType;
		dwItem = m_cbLogType.GetItemData( m_cbLogType.GetCurSel() );

	}

	// Search
	if( dwLogType == eSearchType_GMToolNone || dwLogType == eSearchType_GMToolLogType )
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

		m_pDocument->m_dwPage = 6;
	}
}

void cLogGMToolPage::SetCharacterIdx( DWORD dwCharIdx )
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
	if( m_bCharName == FALSE && m_bLogType == FALSE )
	{
		/// 060914 PKH 날짜에 해당하는 모든 캐릭터 조회
		dwLogType = eSearchType_GMToolNone;
	}
	else if( m_bCharName == TRUE  && m_bLogType == FALSE )		// 이름만,
	{
		/// 060914 PKH 날짜에 해당하는 입력된 캐릭터 조회
		dwLogType = eSearchType_GMToolChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bLogType == TRUE )
	{
		/// 060914 PKH 날짜에 해당하는 입력된 캐릭터와 로그타입으로 조회
		dwLogType = eSearchType_GMToolChrNameLogType;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbLogType.GetItemData( m_cbLogType.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bLogType == TRUE )
	{
		/// 060914 PKH 날짜에 해당하는 입력된 로그타입으로 조회
		dwLogType = eSearchType_GMToolLogType;
		dwItem = m_cbLogType.GetItemData( m_cbLogType.GetCurSel() );

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
		GameLogSearch.Protocol = MP_RMTOOL_GMTOOLLOGFROMTYPE_SYN;
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = (WORD)dwLogType;
		GameLogSearch.dwChrIdx = m_dwCharIdx;
		GameLogSearch.dwItem = dwItem;
		GameLogSearch.bDayOnly = FALSE;
		//		strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
		//		strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );
		strcpy( GameLogSearch.sSDate, sTime );
		strcpy( GameLogSearch.sEDate, eTime );
	}
	else
	{
		// Search
		GameLogSearch.Category = MP_RMTOOL_GAMELOG;
		GameLogSearch.Protocol = MP_RMTOOL_GMTOOLLOGFROMTYPE_SYN;
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

void cLogGMToolPage::SetLogGMTool( MSG_LOGGMTOOL* pMsg )
{
	//	if( !pMsg->bEnd )
	//		ReleaseLogTable();
	for( int i = 0; i < pMsg->count; ++i )
	{
		sGMLogType* pData = new sGMLogType;
		memcpy( pData, &pMsg->logdata[i], sizeof(sGMLogType) );

		m_LogTable.Add( pData, pData->dbidx );
	}

	InitLCGMToolLog();
	UpdateLCGMToolLog( eLog_Max );

}

void cLogGMToolPage::ReleaseLogTable()
{
	sGMLogType* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
		delete pData;
	m_LogTable.RemoveAll();
}

void cLogGMToolPage::InitLCGMToolLog()
{
	m_lcGMToolLog.DeleteAllItems();

	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcGMToolLog.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcGMToolLog.DeleteColumn( 0 );

	char* tcolumn[7] = { "DBIdx", "Characteridx", "LogType", "Logkind", "param1", "param2", "date"};
	int tcolumnsize[7] = { 50, 80, 180, 180, 80, 80, 160 };

	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	int index = 0;
	for( i = 0; i < 7; ++i )
	{
		if( m_bVOption[i] )
		{
			lvcolumn.iSubItem = index;
			lvcolumn.cx = tcolumnsize[i];
			lvcolumn.pszText = tcolumn[i];
			m_lcGMToolLog.InsertColumn( index, &lvcolumn );
			++index;
		}
	}
	// style
	DWORD dwExStyle = m_lcGMToolLog.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcGMToolLog.SetExtendedStyle( dwExStyle );
}

void cLogGMToolPage::UpdateLCGMToolLog( DWORD dwHow )
{
	m_lcGMToolLog.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;
	sGMLogType* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
	{
		int i = 0;
		int index = 0;
		if( pData->logkind == dwHow || dwHow == eLog_Max )
		{
			lvitem.iItem = m_lcGMToolLog.GetItemCount();	// 행

			lvitem.mask = LVIF_TEXT;
			lvitem.stateMask = LVIS_SELECTED;
			lvitem.state = INDEXTOSTATEIMAGEMASK(1);
			lvitem.iSubItem = i;

			for( i = 0; i < 7; ++i )
			{
				if( m_bVOption[i] )
					break;
			}
			lvitem.pszText = GetLCData( pData, i, temp );
			m_lcGMToolLog.InsertItem( &lvitem );

			for( int j = i+1; j < 7; ++j )
			{
				if( m_bVOption[j] )
					m_lcGMToolLog.SetItemText( lvitem.iItem, ++index, GetLCData( pData, j, temp ) );
			}
		}
	}
}

char* cLogGMToolPage::GetLCData( sGMLogType* pData, DWORD dwIndex, char* temp )
{

	switch( dwIndex )
	{
	case 0:		return itoa( pData->dbidx, temp, 10 );
	case 1:		return itoa( pData->character_idx, temp, 10 );
	case 2:	
		{
			if( pData->gmlogtype == 0 )
			{
				sprintf( temp, "-");
				return temp;
			}
			else
				return GAMERESRCMNGR->GetGMToolLogType( pData->gmlogtype, temp );
		}
		break;
	case 3:		return GetKindName( pData->gmlogtype, pData->logkind, temp );
	case 4:		return itoa( pData->param1, temp, 10 );
	case 5:		return itoa( pData->param2, temp, 10 );
	case 6:		return pData->registtime;
	default:	sprintf( temp, "");	return temp;
	}	
}

void cLogGMToolPage::OnBtnSavetofile() 
{
	DeleteFile( "GMToolLog.txt" );

	FILE* fp = fopen( "GMToolLog.txt", "a+" );
	if( !fp )
		return;

	char* tcolumn[7] = { "DBIdx", "Characteridx", "LogType", "Logkind", "param1", "param2", "date"};

	fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2],
		tcolumn[3], tcolumn[4], tcolumn[5], tcolumn[6] );

	char temp1[128] = {0, };
	char temp2[128] = {0, };
	sGMLogType* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
	{
		fprintf( fp, "%d\t%d\t%s\t%s\t%d\t%d\t%s\n", 
			pData->dbidx, 
			pData->character_idx, 
			GAMERESRCMNGR->GetGMToolLogType(pData->gmlogtype, temp1),
			GetKindName( pData->gmlogtype, pData->logkind, temp2 ), 
			pData->param1, 
			pData->param2,
			pData->registtime);
	}

	fclose( fp );
}

void cLogGMToolPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcGMToolLog.SetFont( &m_Font );
}

void cLogGMToolPage::OnCheckTime() 
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


char* cLogGMToolPage::GetKindName( unsigned long logtype, unsigned long logkind, char* temp )
{
	switch( logtype )
	{
	case eGMLog_LogIn:					return temp;
	case eGMLog_Move:					return temp;
	case eGMLog_Item:					return temp;
		switch( logkind )
		{
			case 209:					sprintf( temp, "OptionItem" );		break;
			case 1506:					sprintf( temp, "ItemMallItem" );		break;
			case 202:					sprintf( temp, "Item" );		break;
			case 208:					sprintf( temp, "RareItem" );		break;
		}break;

	case eGMLog_Money:					
		switch( logkind )
		{
		case 116:						sprintf( temp, "ExpensesMoney" );		break;
		case 13:						sprintf( temp, "GetMoney" );		break;
		}break;

	case eGMLog_Summon:					return temp;
	case eGMLog_SummonDelete:			return temp;
	case eGMLog_MonsterKill:			return temp;
	case eGMLog_PKAllow:				return temp;
	case eGMLog_Disconnect_Map:			return temp;
	case eGMLog_Disconnect_User:		return temp;
	case eGMLog_Block:					return temp;
	case eGMLog_Event:					return temp;
		switch( logkind )
		{
			case 1:						sprintf( temp, "EXP" );		break;
			case 2:						sprintf( temp, "ItemDropPercent" );		break;
			case 3:						sprintf( temp, "MoneyDropPercent" );		break;
			case 4:						sprintf( temp, "GetDamage" );		break;
			case 5:						sprintf( temp, "AttackDamage" );		break;
			case 6:						sprintf( temp, "ManaUsePercent" );		break;
			case 7:						sprintf( temp, "RestSpeed" );		break;
			case 8:						sprintf( temp, "PartyEXP" );		break;
			case 9:						sprintf( temp, "AbilityEXP" );		break;
			case 10:					sprintf( temp, "GetMoney" );		break;
			case 11:					sprintf( temp, "SkillEXP" );		break;
		}break;
	case eGMLog_Jackpot:				return temp;
	case eGMLog_PlusTime:				
		switch( logkind )
		{
			case 106:					sprintf( temp, "PlusTimeON" );		break;
			case 99:					sprintf( temp, "PlusTimeOFF" );		break;
			case 105:					sprintf( temp, "PlaingPlusTimeOFF" );		break;
		}break;

	default:							return temp;
	}

	return temp;
}