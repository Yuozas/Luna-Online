// LogSiegeWarPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogSiegeWarPage.h"
#include "RMNetwork.h"
#include "LogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cLogSiegeWarPage property page

IMPLEMENT_DYNCREATE(cLogSiegeWarPage, CLDPropPage)

/* --------------------------------------------------------------------------
* 함수이름 : cLogSiegeWarPage
* 목    적 : cLogSiegeWarPage 클래스 맴버 함수 초기화 (생성자)
* 주의사항 :
*---------------------------------------------------------------------------*/
cLogSiegeWarPage::cLogSiegeWarPage() : CLDPropPage(cLogSiegeWarPage::IDD)
{
	//{{AFX_DATA_INIT(cLogSiegeWarPage)
	mChkGuildName = FALSE;
	mOneDay = FALSE;
	mTime = FALSE;
	//}}AFX_DATA_INIT
	mSiegeWarLogTable.Initialize( 1000 );
	mpDocument = NULL;
	mStart = FALSE;

	for( int i = 0; i < 14; ++i )
		mOption[i] = TRUE;
}

/* --------------------------------------------------------------------------
* 함수이름 : ~cLogSiegeWarPage
* 목    적 : 소멸자
* 주의사항 :
*---------------------------------------------------------------------------*/
cLogSiegeWarPage::~cLogSiegeWarPage()
{
	releaseLogTable();
}

/* --------------------------------------------------------------------------
* 함수이름 : DoDataExchange
* 목    적 : 콘트롤 셋팅
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cLogSiegeWarPage)
	DDX_Control(pDX, IDC_EDIT_E_MIN, mEndMin);
	DDX_Control(pDX, IDC_EDIT_E_HOUR, mEndHour);
	DDX_Control(pDX, IDC_EDIT_S_MIN, mStartMin);
	DDX_Control(pDX, IDC_EDIT_S_HOUR, mStartHour);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, mStartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, mEndTime);
	DDX_Control(pDX, IDC_EDIT_GUILDIDX, mGuildIdxEdit);
	DDX_Control(pDX, IDC_EDIT_LOG_GUILDNAME, mGuildName);
	DDX_Control(pDX, IDC_LISTCTRL_SIEGEWARLOG, mSiegeWarLog);
	DDX_Check(pDX, IDC_CHECK_LOG_GUILDNAME, mChkGuildName);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, mOneDay);
	DDX_Check(pDX, IDC_CHECK_TIME, mTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cLogSiegeWarPage, CPropertyPage)
	//{{AFX_MSG_MAP(cLogSiegeWarPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_LOG_GUILDNAME, OnCheckLogGuildname)
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, OnBtnSavetofile)
	ON_BN_CLICKED(IDC_CHECK_TIME, OnCheckTime)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cLogSiegeWarPage message handlers


/* --------------------------------------------------------------------------
* 함수이름 : OnCreate
* 목    적 : 윈도우생성
* 주의사항 :
*---------------------------------------------------------------------------*/
int cLogSiegeWarPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


/* --------------------------------------------------------------------------
* 함수이름 : OnInitDialog
* 목    적 : 다이얼로그 초기화
* 주의사항 :
*---------------------------------------------------------------------------*/
BOOL cLogSiegeWarPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// 
	mOneDay = TRUE;
	mChkGuildName = TRUE;
	mGuildName.EnableWindow( mChkGuildName );

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/* --------------------------------------------------------------------------
* 함수이름 : OnCheckLogOneday
* 목    적 : 조회시 하루만 검색할경우 콘트롤 상태 변경
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnCheckLogOneday() 
{
	mOneDay = !mOneDay;
//	mStartTime.EnableWindow( !mOneDay );
	mEndTime.EnableWindow( !mOneDay );

	if( mOneDay == FALSE )
	{
		mTime = FALSE;
		mStartHour.EnableWindow( FALSE );
		mStartMin.EnableWindow( FALSE );
		mEndHour.EnableWindow( FALSE );
		mEndMin.EnableWindow( FALSE );
	}
	UpdateData( FALSE );
}


/* --------------------------------------------------------------------------
* 함수이름 : OnCheckLogGuildname
* 목    적 : 조회시 문파명으로 조회할경우
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnCheckLogGuildname() 
{
	mChkGuildName = !mChkGuildName;
	mGuildName.EnableWindow( mChkGuildName );
	UpdateData( FALSE );	
}


/* --------------------------------------------------------------------------
* 함수이름 : OnButtonLogSearch
* 목    적 : 검색버튼 클릭시 검색
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnButtonLogSearch() 
{
	CString name;
	DWORD dwItem = 0;

	// logtype
	unsigned long logType;

	/// 060831 PKH 길드별 검색 안함
	logType = eNONESIEGEWAR;

	if( mChkGuildName == TRUE  )		
	{
		/// 060831 PKH 길드인덱스 검색
		logType = eSIEGEWAR;
		/// 060831 PKH 이름내용이 없는 경우 - 오류
		if( mGuildName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Guild Name!!", "Error" );
			return;
		}
		else
		{
			mGuildName.GetWindowText( name );

		}
	}

	/// 060831 PKH 길드체크가 안된경우
	if( logType == eNONESIEGEWAR )
	{
		setGuildIdx( 0 );
	}
	else
	{
		mGuildName.GetWindowText( name );
	
		// Search
		TMSG_CHECKNAME Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_SIEGEWARIDX_SYN;
		Msg.dwTemplateIdx = mpDocument->GetDocumentIdx();
		strcpy( Msg.sName, (char*)(LPCTSTR)name );

		NETWORK->Send( &Msg, sizeof(TMSG_CHECKNAME) );

		mpDocument->m_dwPage = 4;
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : setGuildIdx
* 목    적 : 문파 index로 문파 토너먼트 데이타 요청
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::setGuildIdx( unsigned long guildidx )
{
//	if( guildidx == 0 )
//		return;

	char temp[128] = {0, };
	sprintf( temp, "%d", guildidx );
	mGuildIdxEdit.SetWindowText( temp );
	mGuildIdxEdit.EnableWindow( TRUE );
	mGuildIdx = guildidx;

	CString sName, sSDate, sEDate;
	DWORD dwItem = 0;
	// Date
	if( mOneDay )
	{
		mStartTime.GetWindowText( sSDate );
		sEDate.Empty();		
	}
	else
	{
		mStartTime.GetWindowText( sSDate );
		mEndTime.GetWindowText( sEDate );
	}


	// logtype
	unsigned long logType;

	/// 060831 PKH 길드별 검색 안함
	logType = eNONESIEGEWAR;

	if( mChkGuildName == TRUE )		// 이름만,
	{
		logType = eSIEGEWAR;
		if( mGuildName.GetWindowTextLength() == 0 || mGuildIdxEdit.GetWindowTextLength() == 0 ||
			mGuildIdxEdit.GetWindowTextLength() == 0)
		{
			MessageBox( "Guild Date Input Error!!", "Error" );
			return;
		}
		else
			mGuildName.GetWindowText( sName );
	}

	
	TMSG_GAMELOGSEARCH GameLogSearch;

	if( mTime )
	{
		CString time;
		int nSHour, nSMin, nEHour, nEMin;
		mStartHour.GetWindowText( time );
		nSHour = atoi( (char*)(LPCTSTR)time );
		mStartMin.GetWindowText( time );
		nSMin = atoi( (char*)(LPCTSTR)time );
		mEndHour.GetWindowText( time );
		nEHour = atoi( (char*)(LPCTSTR)time );
		mEndMin.GetWindowText( time );
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
		GameLogSearch.Protocol = MP_RMTOOL_SIEGEWARLOGFROMTYPE_SYN;
		GameLogSearch.dwTemplateIdx = mpDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = (WORD)logType;
		GameLogSearch.dwChrIdx = mGuildIdx;
		GameLogSearch.dwItem = dwItem;
		GameLogSearch.bDayOnly = FALSE;
		strcpy( GameLogSearch.sSDate, sTime );
		strcpy( GameLogSearch.sEDate, eTime );
	}
	else
	{
		// Search
		GameLogSearch.Category = MP_RMTOOL_GAMELOG;
		GameLogSearch.Protocol = MP_RMTOOL_SIEGEWARLOGFROMTYPE_SYN;
		GameLogSearch.dwTemplateIdx = mpDocument->GetDocumentIdx();
		GameLogSearch.wServer = 0;
		GameLogSearch.wType = (WORD)logType;
		GameLogSearch.dwChrIdx = mGuildIdx;
		GameLogSearch.dwItem = dwItem;
		GameLogSearch.bDayOnly = mOneDay;
		strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
		strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );
	}

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	releaseLogTable();

	mpDocument->StartDlg();
}


/* --------------------------------------------------------------------------
* 함수이름 : setLogSiegeWAr
* 목    적 : DB에서 받아온 길드 로그 정보 셋팅
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::setLogSiegeWar( MSG_LOGSIEGEWAR* pMsg )
{
//	if( !pMsg->bEnd )
//		releaseLogTable();
	for( int i = 0; i < pMsg->count; ++i )
	{
		sSiegeWar* pData = new sSiegeWar;
		memcpy( pData, &pMsg->logdata[i], sizeof(sSiegeWar) );

		mSiegeWarLogTable.Add( pData, pData->idx );
	}

	initLCSiegeWarLog();
	updateLCSiegeWarLog( eLog_Max );

}


/* --------------------------------------------------------------------------
* 함수이름 : releaseLogTable
* 목    적 : log테이블 릴리즈
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::releaseLogTable()
{
	sSiegeWar* pData = NULL;
	mSiegeWarLogTable.SetPositionHead();
	while( pData = mSiegeWarLogTable.GetData() )
		delete pData;
	mSiegeWarLogTable.RemoveAll();
}


/* --------------------------------------------------------------------------
* 함수이름 : initLCSiegeWarLog
* 목    적 : 리스트콘트롤 컬럼 셋팅
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::initLCSiegeWarLog()
{
	mSiegeWarLog.DeleteAllItems();

	// Delete all of the columns.
	int i = 0;
	int nColumnCount = mSiegeWarLog.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		mSiegeWarLog.DeleteColumn( 0 );

	char* tcolumn[7] = { "Idx", "SiegeWarIdx", "MapNum", "GuildIdx", "LogKind", "EngraveIdx", "LogDate" };
	int tcolumnsize[7] = { 70, 100, 70, 70, 200, 100, 130};
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	int index = 0;
	for( i = 0; i < 7; ++i )
	{
		if( mOption[i] )
		{
			lvcolumn.iSubItem = index;
			lvcolumn.cx = tcolumnsize[i];
			lvcolumn.pszText = tcolumn[i];
			mSiegeWarLog.InsertColumn( index, &lvcolumn );
			++index;
		}
	}
	// style
	DWORD dwExStyle = mSiegeWarLog.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	mSiegeWarLog.SetExtendedStyle( dwExStyle );
}


/* --------------------------------------------------------------------------
* 함수이름 : updateLCSiegeWarLog
* 목    적 : 콘트롤 리스트에 db에서 받아온 데이타 출력
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::updateLCSiegeWarLog( unsigned long how )
{
	mSiegeWarLog.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;
	sSiegeWar* pData = NULL;
	mSiegeWarLogTable.SetPositionHead();
	while( pData = mSiegeWarLogTable.GetData() )
	{
		int i = 0;
		int index = 0;
		if( pData->logkind == how || how == eLog_Max )
		{
			lvitem.iItem = mSiegeWarLog.GetItemCount();	// 행

			lvitem.mask = LVIF_TEXT;
			lvitem.stateMask = LVIS_SELECTED;
			lvitem.state = INDEXTOSTATEIMAGEMASK(1);
			lvitem.iSubItem = i;
			
			for( i = 0; i < 7; ++i )
			{
				if( mOption[i] )
					break;
			}
			lvitem.pszText = getLCData( pData, i, temp );
			mSiegeWarLog.InsertItem( &lvitem );
	
			for( int j = i+1; j < 7; ++j )
			{
				if( mOption[j] )
					mSiegeWarLog.SetItemText( lvitem.iItem, ++index, getLCData( pData, j, temp ) );
			}
		}
	}
}


/* --------------------------------------------------------------------------
* 함수이름 : getLCData
* 목    적 : 데이타를 컬럼에 맞게 수정
* 주의사항 :
*---------------------------------------------------------------------------*/
char* cLogSiegeWarPage::getLCData( sSiegeWar* pData, DWORD dwIndex, char* temp )
{
	switch( dwIndex )
	{
	case 0:		return itoa( pData->idx, temp, 10 );
	case 1:		return itoa( pData->siegewaridx, temp, 10);
	case 2:		return itoa( pData->mapnum, temp, 10);
	case 3:		return itoa( pData->guildidx, temp, 10);
	case 4:	
		{
			if( pData->logkind == 0 )
			{
				sprintf( temp, "-");
				return temp;
			}
			else
				return GAMERESRCMNGR->GetSiegeWarLogType( pData->logkind, temp );
		}
		break;
	case 5:		return itoa( pData->engraveidx, temp, 10);
	case 6:		return pData->registtime;

	default:	sprintf( temp, "");	return temp;
	}	
}

/* --------------------------------------------------------------------------
* 함수이름 : OnBtnSavetofile
* 목    적 : 저장버튼 클릭시 파일로 저장
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnBtnSavetofile() 
{

	DeleteFile( "SiegeWarLog.txt" );

	FILE* fp = fopen( "SiegeWarLog.txt", "a+" );
	if( !fp )
		return;

	char* tcolumn[7] = { "Idx", "SiegeWarIdx", "MapNum", "GuildIdx", "LogKind", "EngraveIdx", "LogDate" };

	fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2],
		tcolumn[3], tcolumn[4], tcolumn[5], tcolumn[6]);

	char temp1[128] = {0, };
	sSiegeWar* pData = NULL;
	mSiegeWarLogTable.SetPositionHead();
	while( pData = mSiegeWarLogTable.GetData() )
	{
		fprintf( fp, "%d\t%d\t%d\t%d\t%s\t%d\t%s\n", 
			pData->idx,
			pData->siegewaridx,
			pData->mapnum,
			pData->guildidx,
			GAMERESRCMNGR->GetSiegeWarLogType( pData->logkind, temp1 ),
			pData->engraveidx,
			pData->registtime);
	}

	fclose( fp );

}


/* --------------------------------------------------------------------------
* 함수이름 : ChangeFont
* 목    적 : 글씨 폰트 설정
* 주의사항 :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::ChangeFont()
{
	mFont.CreatePointFont(100, "Arial");
	mSiegeWarLog.SetFont( &mFont );
}

void cLogSiegeWarPage::OnCheckTime() 
{
	if( mOneDay )
	{
		mTime = !mTime;	

		mStartHour.EnableWindow( mTime );
		mStartMin.EnableWindow( mTime );
		mEndHour.EnableWindow( mTime );
		mEndMin.EnableWindow( mTime );
	}
	else
	{
		mTime = FALSE;	

		mStartHour.EnableWindow( mTime );
		mStartMin.EnableWindow( mTime );
		mEndHour.EnableWindow( mTime );
		mEndMin.EnableWindow( mTime );
	}
	
	UpdateData( FALSE );
}

