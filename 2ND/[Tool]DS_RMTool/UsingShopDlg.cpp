// UsingShopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "UsingShopDlg.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsingShopDlg dialog


CUsingShopDlg::CUsingShopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsingShopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsingShopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_dwDocumentIdx = 0;
	m_dwCharIdx = 0;
	m_bEdit = FALSE;
}


void CUsingShopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsingShopDlg)
	DDX_Control(pDX, IDC_USI_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_USI_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_USI_EDIT_ITEM_REMAINTIEM, m_etItemRemainTime);
	DDX_Control(pDX, IDC_USI_EDIT_ITEM_BEGINTIME, m_etItemBeginTime);
	DDX_Control(pDX, IDC_USI_EDIT_ITEM_PARAM, m_etItemParam);
	DDX_Control(pDX, IDC_USI_EDIT_ITEM_DBIDX, m_etItemDBIdx);
	DDX_Control(pDX, IDC_USI_EDIT_ITEM_NAME, m_etItemName);
	DDX_Control(pDX, IDC_LIST_USING_SHOP, m_lcUsingShop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsingShopDlg, CDialog)
	//{{AFX_MSG_MAP(CUsingShopDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_USING_SHOP, OnClickListUsingShop)
	ON_BN_CLICKED(IDC_USI_BTN_SAVE, OnUsiBtnSave)
	ON_BN_CLICKED(IDC_USI_BTN_DELETE, OnUsiBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsingShopDlg message handlers

BOOL CUsingShopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitUsingShopItemListCtrl();
	
	SendUsingShopItemInfoSyn();
	
	m_Font.CreatePointFont(100, "Arial");
	m_lcUsingShop.SetFont( &m_Font );

	if( m_bEdit )
	{
		m_btnDelete.EnableWindow( TRUE );
		m_btnSave.EnableWindow( TRUE );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUsingShopDlg::SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, BOOL bEdit )
{
	m_dwDocumentIdx = dwDocumentIdx;
	m_dwCharIdx = dwCharIdx;
	m_bEdit = bEdit;
}

void CUsingShopDlg::SendUsingShopItemInfoSyn()
{
	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_USINGSHOPITEMINFO_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData = m_dwCharIdx;	//캐릭터 인덱스

	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CUsingShopDlg::UpdateUsingShopItemInfo( TMSG_USINGSHOPITEMINFO* pMsg )
{
	m_lcUsingShop.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	ITEM_INFO* pItemInfo = NULL;
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		lvitem.iItem = m_lcUsingShop.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		pItemInfo = GAMERESRCMNGR->GetItemInfo( (WORD)pMsg->Info[i].dwItemIdx );
		if( pItemInfo )
			lvitem.pszText = pItemInfo->ItemName;
		else
			lvitem.pszText = itoa( pMsg->Info[i].dwItemIdx, temp, 10 );
		m_lcUsingShop.InsertItem( &lvitem );

		//
		sprintf( temp, "%d", pMsg->Info[i].dwItemIdx );
		m_lcUsingShop.SetItemText( lvitem.iItem, 1, temp );

		//
		sprintf( temp, "%d", pMsg->Info[i].dwItemDBIdx );
		m_lcUsingShop.SetItemText( lvitem.iItem, 2, temp );

		//
		if( pMsg->Info[i].dwItemParam == 1 )
		{
			m_lcUsingShop.SetItemText( lvitem.iItem, 3, GAMERESRCMNGR->GetEtcMsg(2) );
		}
		else if( pMsg->Info[i].dwItemParam == 2 )
		{
			m_lcUsingShop.SetItemText( lvitem.iItem, 3, GAMERESRCMNGR->GetEtcMsg(3) );
		}
		else if( pMsg->Info[i].dwItemParam == 4 )
		{
			m_lcUsingShop.SetItemText( lvitem.iItem, 3, GAMERESRCMNGR->GetEtcMsg(1) );
		}
		else if( pMsg->Info[i].dwItemParam == 10 )
		{
			m_lcUsingShop.SetItemText( lvitem.iItem, 3, GAMERESRCMNGR->GetEtcMsg(4) );
		}
		else
		{
			sprintf( temp, "%d", pMsg->Info[i].dwItemParam );		
			m_lcUsingShop.SetItemText( lvitem.iItem, 3, temp );
		}	

		//
		if( pMsg->Info[i].dwItemIdx == 55300 )
		{
			sprintf( temp, "%d", pMsg->Info[i].dwBeginTime );
			m_lcUsingShop.SetItemText( lvitem.iItem, 4, temp );
			
			sprintf( temp, "%d", pMsg->Info[i].dwRemainTime );
			m_lcUsingShop.SetItemText( lvitem.iItem, 5, temp );
		}
		else
		{
			stTIME time;
			time.SetTime( pMsg->Info[i].dwBeginTime );
			sprintf( temp, "%4d-%02d-%02d %02d:%02d", 2000+time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
			m_lcUsingShop.SetItemText( lvitem.iItem, 4, temp );
			
			if( pMsg->Info[i].dwItemParam == 2 )
			{
				sprintf( temp, "%d", pMsg->Info[i].dwRemainTime );
				m_lcUsingShop.SetItemText( lvitem.iItem, 5, temp );
			}
			else
			{
				time.SetTime( pMsg->Info[i].dwRemainTime );
				sprintf( temp, "%4d-%02d-%02d %02d:%02d", 2000+time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
				m_lcUsingShop.SetItemText( lvitem.iItem, 5, temp );
			}
		}
	}
}

void CUsingShopDlg::InitUsingShopItemListCtrl()
{
	char* tcolumn[6] = { "Item", "ItemIdx", "ItemDBIdx", "ItemParam", "BeginTime", "RemainTime" };
	int tcolumnsize[6] = { 180, 30, 80, 80, 120, 120 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 6; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcUsingShop.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcUsingShop.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcUsingShop.SetExtendedStyle( dwExStyle );
}
	

void CUsingShopDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_Font.DeleteObject();	
}

void CUsingShopDlg::OnClickListUsingShop(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lcUsingShop.GetFirstSelectedItemPosition();
	if( pos )
	{
		int index = m_lcUsingShop.GetNextSelectedItem( pos );		
		CString str = m_lcUsingShop.GetItemText( index, 1 );
		m_etItemName.SetWindowText( str );

		DWORD dwItemIdx = atoi(str.GetBuffer(0));
		ITEM_INFO* pItemInfo = GAMERESRCMNGR->GetItemInfo( (WORD)dwItemIdx );
		if( pItemInfo )
		{
			if( dwItemIdx == 55311 || dwItemIdx == 55312 || dwItemIdx == 55313 )
			{
				m_etItemParam.EnableWindow( TRUE );
			}
			else
			{
				m_etItemParam.EnableWindow( FALSE );
			}
		}
		else
		{
			m_etItemParam.EnableWindow( FALSE );
		}

		str = m_lcUsingShop.GetItemText( index, 2 );
		m_etItemDBIdx.SetWindowText( str );
		str = m_lcUsingShop.GetItemText( index, 3 );
		m_etItemParam.SetWindowText( str );
		str = m_lcUsingShop.GetItemText( index, 4 );
		m_etItemBeginTime.SetWindowText( str );
		str = m_lcUsingShop.GetItemText( index, 5 );
		m_etItemRemainTime.SetWindowText( str );
	}
	
	*pResult = 0;
}

void CUsingShopDlg::OnUsiBtnSave() 
{
	CString str;
	m_etItemDBIdx.GetWindowText( str );
	DWORD dwDBIdx = atoi(str.GetBuffer(0));

	if( dwDBIdx == 0 )
	{
		MessageBox( "Select Save Item!!", "Error" );
		return;
	}

	if( dwDBIdx )
	{
		m_etItemName.GetWindowText( str );
		DWORD dwItemIdx = atoi(str.GetBuffer(0));
		if( dwItemIdx == 55311 || dwItemIdx == 55312 || dwItemIdx == 55313 )
		{
			m_etItemParam.GetWindowText( str );
			DWORD dwItemParam = atoi(str.GetBuffer(0));

			char temp[256];
			sprintf( temp, "Are You Really Save ItemParam = %d ?", dwItemParam );

			if( MessageBox( temp, "ShopItem Save", MB_OKCANCEL ) == IDOK )
			{
				TMSG_DWORD3 Msg;
				Msg.Category = MP_RMTOOL_CHARACTER;
				Msg.Protocol = MP_RMTOOL_CHARACTER_USINGSHOPITEMINFO_EDIT_SYN;
				Msg.dwTemplateIdx = m_dwDocumentIdx;
				Msg.dwData1 = m_dwCharIdx;	//캐릭터 인덱스
				Msg.dwData2 = dwDBIdx;
				Msg.dwData3 = dwItemParam;

				NETWORK->Send( &Msg, sizeof(Msg) );

				OnCancel();
			}
		}
	}			
}

void CUsingShopDlg::OnUsiBtnDelete() 
{
	CString str;
	m_etItemDBIdx.GetWindowText( str );
	DWORD dwDBIdx = atoi(str.GetBuffer(0));

	if( dwDBIdx == 0 )
	{
		MessageBox( "Select Delete Item!!", "Error" );
		return;
	}

	if( dwDBIdx )
	{
		char temp[256];
		sprintf( temp, "Are You Really Delete Item With DBIdx = %d?", dwDBIdx );

		if( MessageBox( temp, "ShopItem Delete", MB_OKCANCEL ) == IDOK )
		{

			m_etItemName.GetWindowText( str );
			DWORD dwItemIdx = atoi(str.GetBuffer(0));
			ITEM_INFO* pItemInfo = GAMERESRCMNGR->GetItemInfo( (WORD)dwItemIdx );
			if( !pItemInfo )	return;

			TMSG_DWORD3 Msg;
			Msg.Category = MP_RMTOOL_CHARACTER;
			Msg.Protocol = MP_RMTOOL_CHARACTER_USINGSHOPITEMINFO_DELETE_SYN;
			Msg.dwTemplateIdx = m_dwDocumentIdx;
			Msg.dwData1 = m_dwCharIdx;	//캐릭터 인덱스
			Msg.dwData2 = dwDBIdx;

			if( pItemInfo->ItemKind & eSHOP_ITEM_CHARM )
			{
				Msg.dwData3 = 0;
			}
			else
			{
				Msg.dwData3 = 1;
			}			

			NETWORK->Send( &Msg, sizeof(Msg) );

			OnCancel();
		}
	}		
}
