// LogTotalPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogTotalPage.h"
#include "RMNetwork.h"
#include "LogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogTotalPage dialog

IMPLEMENT_DYNCREATE(CLogTotalPage, CLDPropPage)

CLogTotalPage::CLogTotalPage() : CLDPropPage(CLogTotalPage::IDD)
{
	//{{AFX_DATA_INIT(CLogTotalPage)
	m_bOneDay = FALSE;
	m_nLogKind = -1;
	m_bCharName = TRUE;
	m_bItemDBIdx = FALSE;
	m_bTime = FALSE;
	//}}AFX_DATA_INIT
	m_bStart = FALSE;

	m_CharacterLogTable.Initialize( 500 );
	m_ExpPointLogTable.Initialize( 500 );
	m_MugongLogTable.Initialize( 500 );
	m_MugongExpLogTable.Initialize( 500 );
	m_MoneyLogTable.Initialize( 500 );
	m_MoneyWrongLogTable.Initialize( 500 );
	m_AbilityLogTable.Initialize( 500 );
	m_ItemOptionLogTable.Initialize( 500 );
}

CLogTotalPage::~CLogTotalPage()
{
	ReleaseCharacterLogTable();
	ReleaseExpPointLogTable();
	ReleaseMugongLogTable();
	ReleaseMugongExpLogTable();
	ReleaseMoneyLogTable();
	ReleaseMoneyWrongLogTable();
	ReleaseAbilityLogTable();
	ReleaseItemOptionLogTable();
}

void CLogTotalPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogTotalPage)
	DDX_Control(pDX, IDC_EDIT_E_MIN2, m_etEMin);
	DDX_Control(pDX, IDC_EDIT_E_HOUR2, m_etEHour);
	DDX_Control(pDX, IDC_EDIT_S_MIN2, m_etSMin);
	DDX_Control(pDX, IDC_EDIT_S_HOUR2, m_etSHour);
	DDX_Control(pDX, IDC_EDIT_ITEMDBIDX, m_etItemDBIdx);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Control(pDX, IDC_EDIT_CHARIDX, m_etCharIdx);
	DDX_Control(pDX, IDC_EDIT_LOG_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_LISTCTRL_TOTALLOG, m_lcTotal);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	DDX_Radio(pDX, IDC_RADIO1, m_nLogKind);
	DDX_Check(pDX, IDC_CHECK_CHARACTER_NAME, m_bCharName);
	DDX_Check(pDX, IDC_CHECK_ITEMDBIDX, m_bItemDBIdx);
	DDX_Check(pDX, IDC_CHECK_TIME2, m_bTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogTotalPage, CLDPropPage)
	//{{AFX_MSG_MAP(CLogTotalPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, OnBtnSavetofile)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	ON_BN_CLICKED(IDC_CHECK_CHARACTER_NAME, OnCheckCharacterName)
	ON_BN_CLICKED(IDC_CHECK_ITEMDBIDX, OnCheckItemdbidx)
	ON_BN_CLICKED(IDC_CHECK_TIME2, OnCheckTime2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogTotalPage message handlers

int CLogTotalPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CLDPropPage::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

BOOL CLogTotalPage::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

	m_bOneDay = TRUE;
	m_nLogKind = 0;	
	m_dwCharIdx = 0;

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogTotalPage::OnButtonLogSearch() 
{
	CString sName;
	
	if( m_nLogKind == eTotalLogKind_ItemOption && m_bItemDBIdx )
	{
		if( m_etItemDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search DBIndex!!", "Error" );
			return;
		}
		else
			m_etItemDBIdx.GetWindowText( sName );

		CString sSDate, sEDate;
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

		// Search
		TMSG_GAMELOGSEARCH GameLogSearch;
		GameLogSearch.Category = MP_RMTOOL_GAMELOG;
		GameLogSearch.Protocol = MP_RMTOOL_ITEMOPTIONLOG_SYN;
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 1;
		GameLogSearch.wType = m_nLogKind;
		GameLogSearch.dwItem = atoi( (char*)(LPCTSTR)sName );
		GameLogSearch.bDayOnly = m_bOneDay;
		strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
		strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );
		
		NETWORK->Send( &GameLogSearch, sizeof(GameLogSearch) );
		
		m_pDocument->StartDlg();

		ReleaseItemOptionLogTable();
	}
	else
	{
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );

		// Search
		TMSG_CHECKNAME Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_SYN;
		Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		strcpy( Msg.sName, (char*)(LPCTSTR)sName );

		NETWORK->Send( &Msg, sizeof(TMSG_CHECKNAME) );
	}

	m_pDocument->m_dwPage = 1;
}

void CLogTotalPage::SetCharacterIdx( DWORD dwCharIdx )
{
	if( dwCharIdx == 0 )
		return;

	char temp[128] = {0, };
	sprintf( temp, "%d", dwCharIdx );
	m_etCharIdx.SetWindowText( temp );
	m_etCharIdx.EnableWindow( TRUE );
	m_dwCharIdx = dwCharIdx;

	CString sName, sSDate, sEDate;
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
	
	// Search
	TMSG_GAMELOGSEARCH GameLogSearch;
	GameLogSearch.Category = MP_RMTOOL_GAMELOG;

	switch( m_nLogKind )
	{
	case eTotalLogKind_Character:
		{
			GameLogSearch.Protocol = MP_RMTOOL_CHARACTERLOG_SYN;
			ReleaseCharacterLogTable();
		}
		break;
	case eTotalLogKind_ExpPoint:
		{
			GameLogSearch.Protocol = MP_RMTOOL_EXPPOINTLOG_SYN;
			ReleaseExpPointLogTable();
		}
		break;
	case eTotalLogKind_Mugong:
		{
			GameLogSearch.Protocol = MP_RMTOOL_MUGONGLOG_SYN;
			ReleaseMugongLogTable();
		}
		break;
	case eTotalLogKind_MugongExp:
		{
			GameLogSearch.Protocol = MP_RMTOOL_MUGONGEXPLOG_SYN;
			ReleaseMugongExpLogTable();
		}
		break;
	case eTotalLogKind_Money:
		{
			GameLogSearch.Protocol = MP_RMTOOL_MONEYLOG_SYN;
			ReleaseMoneyLogTable();
		}
		break;
	case eTotalLogKind_MoneyWrong:
		{
			GameLogSearch.Protocol = MP_RMTOOL_MONEYWRONGLOG_SYN;
			ReleaseMoneyWrongLogTable();
		}
		break;
	case eTotalLogKind_Ability:
		{
			GameLogSearch.Protocol = MP_RMTOOL_ABILITYLOG_SYN;
			ReleaseAbilityLogTable();
		}
		break;
	case eTotalLogKind_ItemOption:
		{
			GameLogSearch.Protocol = MP_RMTOOL_ITEMOPTIONLOG_SYN;
			ReleaseItemOptionLogTable();
		}
		break;
	}

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
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = m_nLogKind;
		GameLogSearch.dwItem = m_dwCharIdx;
		GameLogSearch.bDayOnly = FALSE;
		strcpy( GameLogSearch.sSDate, sTime );
		strcpy( GameLogSearch.sEDate, eTime );
	}
	else
	{
		// Search
		GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = m_nLogKind;
		GameLogSearch.dwItem = m_dwCharIdx;
		GameLogSearch.bDayOnly = m_bOneDay;
		strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
		strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );
	}

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	m_pDocument->StartDlg();
}

void CLogTotalPage::OnCheckLogOneday() 
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

void CLogTotalPage::OnRadio1() 
{
	m_nLogKind = eTotalLogKind_Character;

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio2() 
{
	m_nLogKind = eTotalLogKind_ExpPoint;	

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio3() 
{
	m_nLogKind = eTotalLogKind_Mugong;	

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio4() 
{
	m_nLogKind = eTotalLogKind_MugongExp;

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio5() 
{
	m_nLogKind = eTotalLogKind_Money;

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio6() 
{
	m_nLogKind = eTotalLogKind_MoneyWrong;

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnRadio7() 
{
	m_nLogKind = eTotalLogKind_Ability;

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );
}

void CLogTotalPage::OnRadio8() 
{
	m_nLogKind = eTotalLogKind_ItemOption;	

	if( m_bItemDBIdx )
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}
 
void CLogTotalPage::SetCharacterLog( TMSG_CHARACTERLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		CHARACTERLOG* pData = new CHARACTERLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(CHARACTERLOG) );

		m_CharacterLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_Character );
	UpdateListControl( eTotalLogKind_Character );
}

void CLogTotalPage::SetExpPointLog( TMSG_EXPPOINTLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		EXPPOINTLOG* pData = new EXPPOINTLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(EXPPOINTLOG) );

		m_ExpPointLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_ExpPoint );
	UpdateListControl( eTotalLogKind_ExpPoint );
}

void CLogTotalPage::SetMugongLog( TMSG_MUGONGLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		MUGONGLOG* pData = new MUGONGLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(MUGONGLOG) );

		m_MugongLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_Mugong );
	UpdateListControl( eTotalLogKind_Mugong );
}

void CLogTotalPage::SetMugongExpLog( TMSG_MUGONGEXPLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		MUGONGEXPLOG* pData = new MUGONGEXPLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(MUGONGEXPLOG) );

		m_MugongExpLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_MugongExp );
	UpdateListControl( eTotalLogKind_MugongExp );
}

void CLogTotalPage::SetMoneyLog( TMSG_MONEYLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		MONEYLOG* pData = new MONEYLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(MONEYLOG) );

		m_MoneyLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_Money );
	UpdateListControl( eTotalLogKind_Money );
}

void CLogTotalPage::SetMoneyWrongLog( TMSG_MONEYWRONGLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		MONEYWRONGLOG* pData = new MONEYWRONGLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(MONEYWRONGLOG) );

		m_MoneyWrongLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_MoneyWrong );
	UpdateListControl( eTotalLogKind_MoneyWrong );
}

void CLogTotalPage::SetAbilityLog( TMSG_ABILITYLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		ABILITYLOG* pData = new ABILITYLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(ABILITYLOG) );

		m_AbilityLogTable.Add( pData, pData->dwAbilityIdx );
	}

	InitListControl( eTotalLogKind_Ability );
	UpdateListControl( eTotalLogKind_Ability );
}

void CLogTotalPage::SetItemOptionLog( TMSG_ITEMOPTIONLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		ITEMOPTIONLOG* pData = new ITEMOPTIONLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(ITEMOPTIONLOG) );

		m_ItemOptionLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eTotalLogKind_ItemOption );
	UpdateListControl( eTotalLogKind_ItemOption );
}

void CLogTotalPage::ReleaseCharacterLogTable()
{
	CHARACTERLOG* pData = NULL;
	m_CharacterLogTable.SetPositionHead();
	while( pData = m_CharacterLogTable.GetData() )
		delete pData;
	m_CharacterLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseExpPointLogTable()
{
	EXPPOINTLOG* pData = NULL;
	m_ExpPointLogTable.SetPositionHead();
	while( pData = m_ExpPointLogTable.GetData() )
		delete pData;
	m_ExpPointLogTable.RemoveAll();
}
	
void CLogTotalPage::ReleaseMugongLogTable()
{
	MUGONGLOG* pData = NULL;
	m_MugongLogTable.SetPositionHead();
	while( pData = m_MugongLogTable.GetData() )
		delete pData;
	m_MugongLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseMugongExpLogTable()
{
	MUGONGEXPLOG* pData = NULL;
	m_MugongExpLogTable.SetPositionHead();
	while( pData = m_MugongExpLogTable.GetData() )
		delete pData;
	m_MugongExpLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseMoneyLogTable()
{
	MONEYLOG* pData = NULL;
	m_MoneyLogTable.SetPositionHead();
	while( pData = m_MoneyLogTable.GetData() )
		delete pData;
	m_MoneyLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseMoneyWrongLogTable()
{
	MONEYWRONGLOG* pData = NULL;
	m_MoneyWrongLogTable.SetPositionHead();
	while( pData = m_MoneyWrongLogTable.GetData() )
		delete pData;
	m_MoneyWrongLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseAbilityLogTable()
{
	ABILITYLOG* pData = NULL;
	m_AbilityLogTable.SetPositionHead();
	while( pData = m_AbilityLogTable.GetData() )
		delete pData;
	m_AbilityLogTable.RemoveAll();
}

void CLogTotalPage::ReleaseItemOptionLogTable()
{
	ITEMOPTIONLOG* pData = NULL;
	m_ItemOptionLogTable.SetPositionHead();
	while( pData = m_ItemOptionLogTable.GetData() )
		delete pData;
	m_ItemOptionLogTable.RemoveAll();
}

void CLogTotalPage::InitListControl( DWORD dwKind )
{
	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcTotal.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcTotal.DeleteColumn( 0 );
	m_lcTotal.DeleteAllItems();

	switch( dwKind )
	{
	case eTotalLogKind_Character:
		{
			char* tcolumn[9] = { "LogIdx", "CharIdx", "Level", "Physique", "Agility", "Strength", "Heart", "Point", "LogDate" };
			int tcolumnsize[9] = { 80, 80, 80, 80, 80, 80, 80, 50, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 9; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_ExpPoint:
		{
			char* tcolumn[10] = { "LogIdx", "LogType", "CharIdx", "Level", "ChangeValue", "KillerKind", "KillerIdx", "Exp", "AbilityPoint", "LogDate" };
			int tcolumnsize[10] = { 80, 80, 50, 50, 50, 80, 80, 50, 50, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 10; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_Mugong:
		{
			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "SkillIdx", "SkillDBIdx", "Level", "Exp", "LogDate" };
			int tcolumnsize[8] = { 80, 50, 80, 50, 80, 50, 50, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 8; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_MugongExp:
		{
			char* tcolumn[9] = { "LogIdx", "CharIdx", "ChangeValue", "SkillIdx", "SkillDBIdx", "Level", "Exp", "Position", "LogDate" };
			int tcolumnsize[9] = { 50, 50, 50, 50, 80, 50, 50, 50, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 9; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_Money:
		{
			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "ChangeValue", "InvenMoney", "TargetIdx", "TargetMoney", "LogDate" };
			int tcolumnsize[8] = { 50, 80, 50, 50, 80, 50, 80, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 8; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_MoneyWrong:
		{
			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "ChangeValue", "TotalMoney", "WareMoney", "TargetIdx", "LogDate" };
			int tcolumnsize[8] = { 50, 80, 50, 50, 80, 80, 80, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 8; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_Ability:
		{
			char* tcolumn[7] = { "LogIdx", "CharIdx", "AbilityIdx", "AbilityLevelTo", "AbilityExpFrom", "AbilityExpTo", "LogDate" };
			int tcolumnsize[7] = { 50, 50, 50, 80, 80, 80, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 7; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eTotalLogKind_ItemOption:
		{
			char* tcolumn[25] = { "LogIdx", "OptionDBIdx", "ChrIdx", "ItemDBIdx", "Gengol", "Minchub", "Cheryuk",
				"Simmek", "Life", "Neryuk", "Shield", "FireRegis", "WaterRegist", "TreeRegist", "GoldRegist", "EarthRegist",
				"PhyAttack", "Critical", "FireAttack", "WaterAttack", "TreeAttack", "GoldAttack", "EarthAttack", "PhyDefense", "LogDate" };
			int tcolumnsize[25] = { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 25; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcTotal.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	}
	// style
	DWORD dwExStyle = m_lcTotal.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcTotal.SetExtendedStyle( dwExStyle );
}

void CLogTotalPage::UpdateListControl( DWORD dwKind )
{
	m_lcTotal.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	switch( dwKind )
	{
	case eTotalLogKind_Character:
		{
			CHARACTERLOG* pData = NULL;
			m_CharacterLogTable.SetPositionHead();
			while( pData = m_CharacterLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwGrade, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwGengoal, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwDex, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwStrength, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwSimmak, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, itoa( pData->dwPoint, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 8, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_ExpPoint:
		{
			EXPPOINTLOG* pData = NULL;
			m_ExpPointLogTable.SetPositionHead();
			while( pData = m_ExpPointLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
				if( pData->dwKillerIdx > 0 )
				{
//					m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwLogType, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetExpPointLogType( pData->dwLogType, temp ) );
					m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwGrade, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwChangeValue, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwKillerKind, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwKillerIdx, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 7, itoa( pData->dwExpPoint, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 8, itoa( pData->dwAbilPoint, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 9, pData->sLogDate );
				}
				else
				{
//					m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwLogType, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetExpPointLogType( pData->dwLogType, temp ) );
					m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwGrade, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwChangeValue, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 5, GetMonsterName( pData->dwKillerKind, temp ) );
					m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwKillerIdx, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 7, itoa( pData->dwExpPoint, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 8, itoa( pData->dwAbilPoint, temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 9, pData->sLogDate );
				}
			}
		}
		break;
	case eTotalLogKind_Mugong:
		{
			MUGONGLOG* pData = NULL;
			m_MugongLogTable.SetPositionHead();
			while( pData = m_MugongLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
				m_lcTotal.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetMugongLogType( pData->dwLogType, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, GetSkillName( pData->dwSkillIdx, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwSkillDBIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwSkillLevel, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwSkillExp, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_MugongExp:
		{
			MUGONGEXPLOG* pData = NULL;
			m_MugongExpLogTable.SetPositionHead();
			while( pData = m_MugongExpLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChangeValue, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, GetSkillName( pData->dwSkillIdx, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwSkillDBIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwSkillLevel, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwSkillExp, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, itoa( pData->dwSkillPos, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 8, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_Money:
		{
			MONEYLOG* pData = NULL;
			m_MoneyLogTable.SetPositionHead();
			while( pData = m_MoneyLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
//				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwLogType, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetMoneyLogType( pData->dwLogType, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwChangeValue, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwInvenMoney, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwTargetIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwTargetMoney, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_MoneyWrong:
		{
			MONEYWRONGLOG* pData = NULL;
			m_MoneyWrongLogTable.SetPositionHead();
			while( pData = m_MoneyWrongLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
//				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwLogType, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetMoneyLogType( pData->dwLogType, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwChangeValue, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwTotalMoney, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwPyogukMoney, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->dwTargetIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_Ability:
		{
			ABILITYLOG* pData = NULL;
			m_AbilityLogTable.SetPositionHead();
			while( pData = m_AbilityLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcTotal.InsertItem( &lvitem );
				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, GetAbilityName( pData->dwAbilityIdx, temp ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwAbilityLevelTo, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->dwAbilityExpFrom, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->dwAbilityExpTo, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, pData->sLogDate );
			}
		}
		break;
	case eTotalLogKind_ItemOption:
		{
			ITEMOPTIONLOG* pData = NULL;
			m_ItemOptionLogTable.SetPositionHead();
			while( pData = m_ItemOptionLogTable.GetData() )
			{
				lvitem.iItem = m_lcTotal.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );

				m_lcTotal.InsertItem( &lvitem );
				m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pData->dwOptionDBIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 2, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pData->dwItemDBIdx, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 4, itoa( pData->wGengol, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 5, itoa( pData->wMinchub, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 6, itoa( pData->wCheryuk, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 7, itoa( pData->wSimmek, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 8, itoa( pData->wLife, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 9, itoa( pData->wNeryuk, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 10, itoa( pData->wShield, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 11, itoa( pData->wFireRegis, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 12, itoa( pData->wWaterRegist, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 13, itoa( pData->wTreeRegist, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 14, itoa( pData->wGoldRegist, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 15, itoa( pData->wEarthRegist, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 16, itoa( pData->wPhyAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 17, itoa( pData->wCritical, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 18, itoa( pData->wFireAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 19, itoa( pData->wWaterAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 20, itoa( pData->wTreeAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 21, itoa( pData->wGoldAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 22, itoa( pData->wEarthAttack, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 23, itoa( pData->wPhyDefense, temp, 10 ) );
				m_lcTotal.SetItemText( lvitem.iItem, 24, pData->sLogDate );
			}			
		}
		break;
	}
}

char* CLogTotalPage::GetSkillLogType( DWORD dwLogType, char* temp )
{
	switch( dwLogType )
	{
	case eLog_MugongDiscard:			sprintf( temp, "SkillDiscard" );			break;
	case eLog_MugongDestroyByWanted:	sprintf( temp, "SkillDestroyByWanted" );	break;
	case eLog_MugongDestroyByBadFame:	sprintf( temp, "SkillDestroyByBadFame" );	break;
	default:							sprintf( temp, "???" );						break;
	}

	return temp;
}

char* CLogTotalPage::GetSkillName( DWORD dwSkillIdx, char* temp )
{
	SKILLINFO* pSkill = GAMERESRCMNGR->GetSkillInfo( (WORD)dwSkillIdx );
	if( pSkill == NULL )
		sprintf( temp, "%d", dwSkillIdx );
	else
		sprintf( temp, "%s", pSkill->SkillName );

	return temp;
}

char* CLogTotalPage::GetAbilityName( DWORD dwAbilityIdx, char* temp )
{
	CAbilityInfo* pInfo = GAMERESRCMNGR->GetAbilityInfo( (WORD)dwAbilityIdx ); 
	if( pInfo == NULL )
		sprintf( temp, "%d", dwAbilityIdx );
	else
		sprintf( temp, "%s", pInfo->GetInfo()->Ability_name );

	return temp;
}

void CLogTotalPage::OnBtnSavetofile() 
{
	switch( m_nLogKind )
	{
	case eTotalLogKind_Character:
		{
			DeleteFile( "CharacterLog.txt" );

			FILE* fp = fopen( "CharacterLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[9] = { "LogIdx", "CharIdx", "Level", "Physique", "Agility", "Strength", "Heart", "Point", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7], tcolumn[8] );

			char temp[128] = {0, };
			CHARACTERLOG* pData = NULL;
			m_CharacterLogTable.SetPositionHead();
			while( pData = m_CharacterLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwChrIdx, pData->dwGrade,
					pData->dwGengoal, pData->dwDex, pData->dwStrength, pData->dwSimmak, pData->dwPoint, pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_ExpPoint:
		{
			DeleteFile( "ExpPointLog.txt" );

			FILE* fp = fopen( "ExpPointLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[10] = { "LogIdx", "LogType", "CharIdx", "Level", "ChangeValue", "KillerKind", "KillerIdx", "Exp", "AbilityPoint", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7], tcolumn[8], tcolumn[9] );

			char temp[128] = {0, };
			EXPPOINTLOG* pData = NULL;
			m_ExpPointLogTable.SetPositionHead();
			while( pData = m_ExpPointLogTable.GetData() )
			{
				if( pData->dwKillerIdx > 0 )
				{
					fprintf( fp, "%d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, GAMERESRCMNGR->GetExpPointLogType( pData->dwLogType, temp ), pData->dwChrIdx, pData->dwGrade, pData->dwChangeValue,
						pData->dwKillerKind, pData->dwKillerIdx, pData->dwExpPoint, pData->dwAbilPoint, pData->sLogDate );
				}
				else
				{
					fprintf( fp, "%d\t%s\t%d\t%d\t%d\t%s\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, GAMERESRCMNGR->GetExpPointLogType( pData->dwLogType, temp ), pData->dwChrIdx, pData->dwGrade, pData->dwChangeValue,
						GetMonsterName( pData->dwKillerKind, temp ), pData->dwKillerIdx, pData->dwExpPoint, pData->dwAbilPoint, pData->sLogDate );
				}
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_Mugong:
		{
			DeleteFile( "SkillLog.txt" );

			FILE* fp = fopen( "SkillLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "SkillIdx", "SkillDBIdx", "Level", "Exp", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7] );

			char temp[128] = {0, };
			MUGONGLOG* pData = NULL;
			m_MugongLogTable.SetPositionHead();
			while( pData = m_MugongLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%s\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, GAMERESRCMNGR->GetMugongLogType( pData->dwLogType, temp ), pData->dwChrIdx,
					GetSkillName( pData->dwSkillIdx, temp ), pData->dwSkillDBIdx, pData->dwSkillLevel, pData->dwSkillExp, pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_MugongExp:
		{
			DeleteFile( "SkillExpLog.txt" );

			FILE* fp = fopen( "SkillExpLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[9] = { "LogIdx", "CharIdx", "ChangeValue", "SkillIdx", "SkillDBIdx", "Level", "Exp", "Position", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7], tcolumn[8] );

			char temp[128] = {0, };
			MUGONGEXPLOG* pData = NULL;
			m_MugongExpLogTable.SetPositionHead();
			while( pData = m_MugongExpLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%s\t%d\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwChrIdx, pData->dwChangeValue, 
					GetSkillName( pData->dwSkillIdx, temp ), pData->dwSkillLevel, pData->dwSkillExp, pData->dwSkillPos, pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_Money:
		{
			DeleteFile( "MoneyLog.txt" );

			FILE* fp = fopen( "MoneyLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "ChangeValue", "InvenMoney", "TargetIdx", "TargetMoney", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7] );

			char temp[128] = {0, };
			MONEYLOG* pData = NULL;
			m_MoneyLogTable.SetPositionHead();
			while( pData = m_MoneyLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwLogType, pData->dwChrIdx, pData->dwChangeValue, 
					pData->dwInvenMoney, pData->dwTargetIdx, pData->dwTargetMoney, pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_MoneyWrong:
		{
			DeleteFile( "MoneyWrongLog.txt" );

			FILE* fp = fopen( "MoneyWrongLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[8] = { "LogIdx", "LogType", "CharIdx", "ChangeValue", "TotalMoney", "WareMoney", "TargetIdx", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7] );

			char temp[128] = {0, };
			MONEYWRONGLOG* pData = NULL;
			m_MoneyWrongLogTable.SetPositionHead();
			while( pData = m_MoneyWrongLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwLogType, pData->dwChrIdx, pData->dwChangeValue,
					pData->dwTotalMoney, pData->dwPyogukMoney, pData->dwTargetIdx, pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eTotalLogKind_Ability:
		{
			DeleteFile( "AbilityLog.txt" );

			FILE* fp = fopen( "AbilityLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[7] = { "LogIdx", "CharIdx", "AbilityIdx", "AbilityLevelTo", "AbilityExpFrom", "AbilityExpTo", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6] );

			char temp[128] = {0, };
			ABILITYLOG* pData = NULL;
			m_AbilityLogTable.SetPositionHead();
			while( pData = m_AbilityLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%s\t%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwChrIdx, GetAbilityName( pData->dwAbilityIdx, temp ),
					pData->dwAbilityLevelTo, pData->dwAbilityExpFrom, pData->dwAbilityExpTo, pData->sLogDate );
			}

			fclose( fp );
		}
		break;
	}
}

char* CLogTotalPage::GetMonsterName( DWORD dwMonsterIdx, char* temp )
{
	if( dwMonsterIdx == 0 )
	{
		sprintf( temp, "0" );
		return temp;
	}

	MONSTEREX_LIST* pMonster = GAMERESRCMNGR->GetMonsterInfo( (WORD)dwMonsterIdx );
	if( pMonster == NULL )
		sprintf( temp, "Monster" );
	else
		sprintf( temp, "%s", pMonster->Name );

	return temp;
}

void CLogTotalPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcTotal.SetFont( &m_Font );
}

void CLogTotalPage::OnCheckCharacterName() 
{
	m_bCharName = !m_bCharName;
	if( m_bCharName )
		m_bItemDBIdx = FALSE;
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );
}

void CLogTotalPage::OnCheckItemdbidx() 
{
	if( m_nLogKind == eTotalLogKind_ItemOption )
	{
		m_bItemDBIdx = !m_bItemDBIdx;
		if( m_bItemDBIdx )
			m_bCharName = FALSE;
	}
	else
	{
		m_bItemDBIdx = FALSE;
		m_bCharName = TRUE;
	}
	m_etCharName.EnableWindow( m_bCharName );
	m_etItemDBIdx.EnableWindow( m_bItemDBIdx );
	UpdateData( FALSE );	
}

void CLogTotalPage::OnCheckTime2() 
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
