// GuildInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "GuildInfo.h"
#include "MunpaDoc.h"
#include "RMNetwork.h"
#include "ItemDlg.h"
#include "ChangeValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuildInfo property page

IMPLEMENT_DYNCREATE(CGuildInfo, CLDPropPage)

CGuildInfo::CGuildInfo() : CLDPropPage(CGuildInfo::IDD)
{
	//{{AFX_DATA_INIT(CGuildInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDocument = NULL;
	m_GuildTable.Initialize( 500 );
	m_GuildMemberTable.Initialize( 100 );

	memset( &m_WareItemInfo_org, 0, sizeof(m_WareItemInfo_org) );
	memset( m_WareItemOptionInfo_org, 0, sizeof(ITEM_OPTION_INFO)*TABCELL_GUILDWAREHOUSE_NUM );
	memset( &m_WareItemInfo, 0, sizeof(m_WareItemInfo) );
	memset( m_WareItemOptionInfo, 0, sizeof(ITEM_OPTION_INFO)*TABCELL_GUILDWAREHOUSE_NUM );

	m_pItemDlg = NULL;
}

CGuildInfo::~CGuildInfo()
{
	ReleaseGuildTable();
	ReleaseGuildMemberTable();
}

void CGuildInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuildInfo)
	DDX_Control(pDX, IDC_EDIT_GUILDMONEY, m_etGuildMoney);
	DDX_Control(pDX, IDC_COMBO_GUILDWARE, m_cbGuildWare);
	DDX_Control(pDX, IDC_EDIT_MEMBERNUM, m_etMemberNum);
	DDX_Control(pDX, IDC_EDIT_MASTERNAME, m_etGuildMasterName);
	DDX_Control(pDX, IDC_EDIT_GUILDNAME, m_etGuildName);
	DDX_Control(pDX, IDC_EDIT_GUILDMAP, m_etGuildMap);
	DDX_Control(pDX, IDC_EDIT_GUILDLEVEL, m_etGuildLevel);
	DDX_Control(pDX, IDC_EDIT_SEARCH_GUILDNAME, m_etSearchGuildName);
	DDX_Control(pDX, IDC_LIST_GUILDMEMBER, m_lcGuildMember);
	DDX_Control(pDX, IDC_LIST_GUILD, m_lcGuild);
	//}}AFX_DATA_MAP

	for( int i = 0; i < TABCELL_GUILDWAREHOUSE_NUM; ++i )
		DDX_Control(pDX, IDC_BTN_GUILDWARE1+i, m_GuildWareItem[i]);
}


BEGIN_MESSAGE_MAP(CGuildInfo, CLDPropPage)
	//{{AFX_MSG_MAP(CGuildInfo)
	ON_BN_CLICKED(IDC_SEARCH_GUILD_TOTAL, OnSearchGuildTotal)
	ON_BN_CLICKED(IDC_SEARCH_GUILD, OnSearchGuild)
	ON_CBN_SELCHANGE(IDC_COMBO_GUILDWARE, OnSelchangeComboGuildware)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuildInfo message handlers
BOOL CGuildInfo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	switch( nCode )
	{
	case BN_CLICKED:
		{
			ButtonControlCommand( nID );
		}
		break;
	}

	return CLDPropPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CGuildInfo::ReleaseGuildTable()
{
	GUILDBASEINFO* pData = NULL;
	m_GuildTable.SetPositionHead();
	while( pData = m_GuildTable.GetData() )
		delete pData;
	m_GuildTable.RemoveAll();
}

void CGuildInfo::ReleaseGuildMemberTable()
{
	TGUILDMEMBERINFO* pData = NULL;
	m_GuildMemberTable.SetPositionHead();
	while( pData = m_GuildMemberTable.GetData() )
		delete pData;
	m_GuildMemberTable.RemoveAll();
}

void CGuildInfo::SetGuildList( TMSG_GUILD* pMsg )
{
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		GUILDBASEINFO* pData = new GUILDBASEINFO;
		memcpy( pData, &pMsg->GuildInfo[i], sizeof(GUILDBASEINFO) );

		m_GuildTable.Add( pData, pData->dwGuildIdx );
	}

	InitGuildListControl();
	UpdateGuildListControl();
}

void CGuildInfo::SetGuildInfo( TMSG_GUILD* pMsg )
{
	GUILDBASEINFO* pData = &pMsg->GuildInfo[0];

	char temp[256] = {0, };
	m_etGuildName.SetWindowText( (LPCTSTR)pData->sGuildName );
	m_etGuildLevel.SetWindowText( (LPCTSTR)itoa( pData->dwGuildLevel, temp, 10 ) );
	m_etGuildMasterName.SetWindowText( (LPCTSTR)pData->sMasterName );
	m_etMemberNum.SetWindowText( "" );
	m_etGuildMap.SetWindowText( (LPCTSTR)itoa( pData->dwGuildMap, temp, 10 ) );

	m_etGuildMoney.EnableWindow( TRUE );
	m_etGuildMoney.SetWindowText( (LPCTSTR)itoa( pData->dwGuildMoney, temp, 10 ) );
}

void CGuildInfo::SetGuildMemberList( TMSG_GUILDMEMBERINFO* pMsg )
{
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		TGUILDMEMBERINFO* pData = new TGUILDMEMBERINFO;
		memcpy( pData, &pMsg->MemberInfo[i], sizeof(TGUILDMEMBERINFO) );

		m_GuildMemberTable.Add( pData, pData->dwChrIdx );
	}

	char temp[256] = {0, };
	m_etMemberNum.SetWindowText( (LPCTSTR)itoa( m_GuildMemberTable.GetDataNum(), temp, 10 ) );

	InitGuildMemberListControl();
	UpdateGuildMemberListControl();	
}

void CGuildInfo::SetGuildWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg )
{
	DWORD pos = 0;
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		pos = pMsg->ItemInfo[i].Position;
		if( pos >= TP_GUILDWAREHOUSE_START && pos < TP_GUILDWAREHOUSE_END )
		{
			pos = pMsg->ItemInfo[i].Position - TP_GUILDWAREHOUSE_START;
			m_WareItemInfo_org.WarehouseItem[pos] = pMsg->ItemInfo[i];
		}
		else
		{
			MessageBox( "GuildWareItem Position Error!!", "Error!" );
		}
	}

	m_WareItemInfo = m_WareItemInfo_org;

	m_cbGuildWare.EnableWindow( TRUE );
	OnSelchangeComboGuildware();
}

void CGuildInfo::SetGuildWareItemOptionInfo( TMSG_ITEMOPTIONINFO2* pMsg )
{
	DWORD pos = 0;
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		int pos = GetItemPositionFromOptionDBIdx( pMsg->ItemOptionInfo[i].dwOptionIdx );
		if( pos != -1 )
		{
			m_WareItemOptionInfo_org[pos] = pMsg->ItemOptionInfo[i];
		}
	}

	memcpy( m_WareItemOptionInfo, m_WareItemOptionInfo_org, sizeof(ITEM_OPTION_INFO)*TABCELL_GUILDWAREHOUSE_NUM );
}

int CGuildInfo::GetItemPositionFromOptionDBIdx( DWORD dwOptionDBIdx )
{
	for( int i = 0; i < TABCELL_GUILDWAREHOUSE_NUM; ++i )
	{
		if( m_WareItemInfo_org.WarehouseItem[i].Durability == dwOptionDBIdx )
			return i;
	}
	return -1;
}

void CGuildInfo::InitGuildListControl()
{
	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcGuild.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcGuild.DeleteColumn( 0 );
	m_lcGuild.DeleteAllItems();

	char* tcolumn[6] = { "GuildIdx", "GuildName", "MasterName", "GuildLevel", "Location", "CreateDate" };
	int tcolumnsize[6] = { 50, 80, 80, 50, 50, 120 };

	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 6; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcGuild.InsertColumn( i, &lvcolumn );
	}
}

void CGuildInfo::UpdateGuildListControl()
{
	m_lcGuild.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	GUILDBASEINFO* pData = NULL;
	m_GuildTable.SetPositionHead();
	while( pData = m_GuildTable.GetData() )
	{
		lvitem.iItem = m_lcGuild.GetItemCount();	// 행
				
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.iSubItem = 0;	
		lvitem.pszText = itoa( pData->dwGuildIdx, temp, 10 );

		m_lcGuild.InsertItem( &lvitem );
		m_lcGuild.SetItemText( lvitem.iItem, 1, pData->sGuildName );
		m_lcGuild.SetItemText( lvitem.iItem, 2, pData->sMasterName );
		m_lcGuild.SetItemText( lvitem.iItem, 3, itoa( pData->dwGuildLevel, temp, 10 ) );
		m_lcGuild.SetItemText( lvitem.iItem, 4, itoa( pData->dwGuildMap, temp, 10 ) );
		m_lcGuild.SetItemText( lvitem.iItem, 5, pData->sCreateDate );
	}
	// style
	DWORD dwExStyle = m_lcGuild.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcGuild.SetExtendedStyle( dwExStyle );
}

void CGuildInfo::InitGuildMemberListControl()
{
	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcGuildMember.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcGuildMember.DeleteColumn( 0 );
	m_lcGuildMember.DeleteAllItems();

	char* tcolumn[3] = { "MemberName", "GuildPosition", "Level" };
	int tcolumnsize[3] = { 100, 100, 50 };

	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 3; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcGuildMember.InsertColumn( i, &lvcolumn );
	}
}

void CGuildInfo::UpdateGuildMemberListControl()
{
	m_lcGuildMember.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	TGUILDMEMBERINFO* pData = NULL;
	m_GuildMemberTable.SetPositionHead();
	while( pData = m_GuildMemberTable.GetData() )
	{
		lvitem.iItem = m_lcGuildMember.GetItemCount();	// 행
				
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.iSubItem = 0;	
		lvitem.pszText = pData->sName;

		m_lcGuildMember.InsertItem( &lvitem );
		m_lcGuildMember.SetItemText( lvitem.iItem, 1, itoa( pData->dwPosition, temp, 10 ) );
		m_lcGuildMember.SetItemText( lvitem.iItem, 2, itoa( pData->dwLevel, temp, 10 ) );
	}
	// style
	DWORD dwExStyle = m_lcGuildMember.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcGuildMember.SetExtendedStyle( dwExStyle );
}

void CGuildInfo::OnSearchGuildTotal() 
{
	ReleaseGuildTable();

	TMSGBASE Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILD_SYN;
	Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CGuildInfo::OnSearchGuild() 
{
	CString str;
	m_etSearchGuildName.GetWindowText( str );

	if( str.GetLength() == 0 || str.GetLength() > 16 )
		return;

	ReleaseGuildMemberTable();

	TMSG_MUNPASEARCH Msg;
	Msg.Category = MP_RMTOOL_MUNPA;
	Msg.Protocol = MP_RMTOOL_GUILDIFNO_SYN;
	Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	strcpy( Msg.strSearch, str.GetBuffer(0) );

	NETWORK->Send( &Msg, sizeof(Msg) );
}


void CGuildInfo::ButtonControlCommand( UINT nId )
{
	CMRButton* pBtn = (CMRButton*)GetDlgItem( nId );
	if( pBtn )	return;

	if( nId >= IDC_BTN_GUILDWARE1 && nId < IDC_BTN_GUILDWARE30+1 )
	{
		int tab = m_cbGuildWare.GetCurSel()*TABCELL_GUILDWAREHOUSE_NUM;
		WORD wPos = tab + nId - IDC_BTN_GUILDWARE1;

		ITEMBASE* origin = &m_WareItemInfo_org.WarehouseItem[wPos];
		ITEMBASE* base_before = &m_WareItemInfo.WarehouseItem[wPos];
		ITEMBASE base_after;
		ITEM_OPTION_INFO* option_before = NULL;
		ITEM_OPTION_INFO option_after;

//		if( GAMERESRCMNGR->IsOptionItem( base_before->wIconIdx, base_before->Durability ) )
			option_before = &m_WareItemOptionInfo[wPos];
/*
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
		if( GAMERESRCMNGR->IsOptionItem(origin->wIconIdx, origin->Durability) )
			pOptionInfoOrigin = m_pDocument->GetItemOptionInfoOrigin(origin->Durability);
		if( GAMERESRCMNGR->IsOptionItem(base_before->wIconIdx, base_before->Durability) )
			pOptionInfo = m_pDocument->GetItemOptionInfo(base_before->Durability);
*/

		int result = ItemEdit( eIF_MUNPA, wPos, base_before, base_after, option_before, option_after );

		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_GUILDWAREHOUSE_START + wPos;
//		ResultItemEdit( eIF_MUNPA, result, TP_MUNPAWAREHOUSE_START+wPos, nId, base_before, option_before, base_before,
//						&base_after, option_before, &option_after );
		ResultItemEdit( eIF_MUNPA, result, TP_PYOGUK_END+wPos, nId, base_before, option_before, base_before,
			&base_after, option_before, &option_after );
	}
}

int CGuildInfo::ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after,
						  const ITEM_OPTION_INFO* option_before, ITEM_OPTION_INFO& option_after )
{
	int result = 0;

	m_pItemDlg = new CItemDlg;
	m_pItemDlg->Init( field, base_before, option_before, wControlId );

	result = m_pItemDlg->DoModal();

	memcpy( &base_after, m_pItemDlg->GetItemBase(), sizeof(ITEMBASE) );
	memcpy( &option_after, m_pItemDlg->GetItemOption(), sizeof(ITEM_OPTION_INFO) );

	SAFE_DELETE(m_pItemDlg);

	return result;
}

BOOL CGuildInfo::ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
								 ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption )
{
/*
	CChangeManager* pManager = m_pDocument->GetChangeManager();

	TABLE_KIND kind = pManager->GetTableKind_ByField( field );
	if( kind == eTK_None )	return FALSE;

	BOOL bAdd = FALSE;
	CChangeItem* pItem = (CChangeItem*)pManager->ExistChangeInfo( wPos, FALSE );

	if( !pItem )				// 기존 수정 정보가 있는지 검사한다. (있어도 무시!)
	{
		pItem = new CChangeItem;
		bAdd = TRUE;
	}

	if( result == eMR_Delete )	// Delete인 경우
	{
		memset( pBItem, 0, sizeof(ITEMBASE) );
		memset( pAItem, 0, sizeof(ITEMBASE) );
		memset( pBOption, 0, sizeof(ITEM_OPTION_INFO) );
		memset( pAOption, 0, sizeof(ITEM_OPTION_INFO) );

		pItem->Init( eOp_Delete, kind, wPos );
		pItem->SetData( pBItem, pAItem, pBOption, pAOption );
	}
	else if( result == eMR_Edit )
	{
		memcpy( pBItem, pAItem, sizeof(ITEMBASE) );				// 기본 아이템 변경
		memcpy( pBOption, pAOption, sizeof(ITEM_OPTION_INFO) );	//

		if( pAItem->Durability == 9999 ) // 옵션 셋팅
		{
			if( OriginOption && OriginOption->dwOptionIdx != 0 )
			{
				pAOption->dwOptionIdx = OriginOption->dwOptionIdx;
				m_pDocument->AddLocalOptionInfo(pAOption, FALSE);
			}
			else
			{
				WORD wOptionIdx = m_pDocument->AddLocalOptionInfo(pAOption, TRUE);
				pAOption->dwOptionIdx = wOptionIdx;
			}
			pAItem->Durability = (WORD)pAOption->dwOptionIdx;
		}

		if( Origin->dwDBIdx == 0 )	// Insert인 경우
		{
			pItem->Init( eOp_Insert, kind, wPos );
			pItem->SetData( pBItem, pAItem, pBOption, pAOption );
		}
		else						// Update인 경우
		{
			pItem->Init( eOp_Update, kind, wPos );
			pItem->SetData( pBItem, pAItem, pBOption, pAOption );
		}
	}

	if( bAdd )	pManager->Add( pItem, wPos, FALSE );
*/

	// Button Text Setting
	char buff[256] = {0,};
	ITEM_INFO* pInfo;
	if( pAItem->wIconIdx != 0 )
	{
		pInfo = GAMERESRCMNGR->GetItemInfo(pAItem->wIconIdx);
		if( pInfo )	sprintf(buff, pInfo->ItemName);
		else		sprintf(buff, "%d", pAItem->wIconIdx);
	}
	else	sprintf(buff, "");

	CMRButton* btn = (CMRButton*)GetDlgItem(nControlIdx);
	btn->SetWindowText(buff);
	btn->SetData(pAItem->wIconIdx);
	btn->SetColorText(COLOR_BUTTON_CHANGED);

	return TRUE;
}


void CGuildInfo::OnSelchangeComboGuildware() 
{
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	int nCurSel = m_cbGuildWare.GetCurSel();
	int tab = nCurSel*TABCELL_GUILDWAREHOUSE_NUM;

	// Item Setting
	for(int i = 0; i < TABCELL_GUILDWAREHOUSE_NUM; ++i )
	{
		if( m_WareItemInfo.WarehouseItem[tab+i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(m_WareItemInfo.WarehouseItem[tab+i].wIconIdx);
			if( pInfo )	sprintf(buff, pInfo->ItemName);
			else		sprintf(buff, "%d", m_WareItemInfo.WarehouseItem[tab+i].wIconIdx);
		}
		else	sprintf(buff, "");

		m_GuildWareItem[i].SetWindowText(buff);
		m_GuildWareItem[i].SetData(m_WareItemInfo.WarehouseItem[tab+i].wIconIdx);
	}	
}

BOOL CGuildInfo::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

	char temp[256] = {0, };
	for( int i = 0; i < TAB_GUILDWAREHOUSE_NUM; ++i )
	{
		sprintf( temp, "Tab%d", i+1 );
		m_cbGuildWare.AddString( temp );
	}
	m_cbGuildWare.SetCurSel( 0 );
	
	m_cbGuildWare.EnableWindow( FALSE );
	m_etGuildMoney.EnableWindow( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
