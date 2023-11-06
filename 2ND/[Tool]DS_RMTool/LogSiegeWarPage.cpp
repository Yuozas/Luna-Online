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
* �Լ��̸� : cLogSiegeWarPage
* ��    �� : cLogSiegeWarPage Ŭ���� �ɹ� �Լ� �ʱ�ȭ (������)
* ���ǻ��� :
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
* �Լ��̸� : ~cLogSiegeWarPage
* ��    �� : �Ҹ���
* ���ǻ��� :
*---------------------------------------------------------------------------*/
cLogSiegeWarPage::~cLogSiegeWarPage()
{
	releaseLogTable();
}

/* --------------------------------------------------------------------------
* �Լ��̸� : DoDataExchange
* ��    �� : ��Ʈ�� ����
* ���ǻ��� :
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
* �Լ��̸� : OnCreate
* ��    �� : ���������
* ���ǻ��� :
*---------------------------------------------------------------------------*/
int cLogSiegeWarPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


/* --------------------------------------------------------------------------
* �Լ��̸� : OnInitDialog
* ��    �� : ���̾�α� �ʱ�ȭ
* ���ǻ��� :
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
* �Լ��̸� : OnCheckLogOneday
* ��    �� : ��ȸ�� �Ϸ縸 �˻��Ұ�� ��Ʈ�� ���� ����
* ���ǻ��� :
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
* �Լ��̸� : OnCheckLogGuildname
* ��    �� : ��ȸ�� ���ĸ����� ��ȸ�Ұ��
* ���ǻ��� :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnCheckLogGuildname() 
{
	mChkGuildName = !mChkGuildName;
	mGuildName.EnableWindow( mChkGuildName );
	UpdateData( FALSE );	
}


/* --------------------------------------------------------------------------
* �Լ��̸� : OnButtonLogSearch
* ��    �� : �˻���ư Ŭ���� �˻�
* ���ǻ��� :
*---------------------------------------------------------------------------*/
void cLogSiegeWarPage::OnButtonLogSearch() 
{
	CString name;
	DWORD dwItem = 0;

	// logtype
	unsigned long logType;

	/// 060831 PKH ��庰 �˻� ����
	logType = eNONESIEGEWAR;

	if( mChkGuildName == TRUE  )		
	{
		/// 060831 PKH ����ε��� �˻�
		logType = eSIEGEWAR;
		/// 060831 PKH �̸������� ���� ��� - ����
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

	/// 060831 PKH ���üũ�� �ȵȰ��
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
* �Լ��̸� : setGuildIdx
* ��    �� : ���� index�� ���� ��ʸ�Ʈ ����Ÿ ��û
* ���ǻ��� :
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

	/// 060831 PKH ��庰 �˻� ����
	logType = eNONESIEGEWAR;

	if( mChkGuildName == TRUE )		// �̸���,
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
* �Լ��̸� : setLogSiegeWAr
* ��    �� : DB���� �޾ƿ� ��� �α� ���� ����
* ���ǻ��� :
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
* �Լ��̸� : releaseLogTable
* ��    �� : log���̺� ������
* ���ǻ��� :
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
* �Լ��̸� : initLCSiegeWarLog
* ��    �� : ����Ʈ��Ʈ�� �÷� ����
* ���ǻ��� :
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
	
	// listctrl column ����
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
* �Լ��̸� : updateLCSiegeWarLog
* ��    �� : ��Ʈ�� ����Ʈ�� db���� �޾ƿ� ����Ÿ ���
* ���ǻ��� :
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
			lvitem.iItem = mSiegeWarLog.GetItemCount();	// ��

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
* �Լ��̸� : getLCData
* ��    �� : ����Ÿ�� �÷��� �°� ����
* ���ǻ��� :
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
* �Լ��̸� : OnBtnSavetofile
* ��    �� : �����ư Ŭ���� ���Ϸ� ����
* ���ǻ��� :
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
* �Լ��̸� : ChangeFont
* ��    �� : �۾� ��Ʈ ����
* ���ǻ��� :
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

