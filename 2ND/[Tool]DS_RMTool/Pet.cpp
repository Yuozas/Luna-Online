// Pet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "Pet.h"
#include "PetDoc.h"
#include "RMNetwork.h"
#include "ChangeValue.h"
#include "ItemDlg.h"
#include "MasInfoManager.h"
#include "ItemRecoverDlg.h"
#include "ItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPet, CLDPropPage)

CPet::CPet(): CLDPropPage(CPet::IDD)
{
	//{{AFX_DATA_INIT(CGuildInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDocument = NULL;
	m_PetTable.Initialize( 500 );

	memset( &m_WareItemInfo_org, 0, sizeof(m_WareItemInfo_org) );
	memset( m_WareItemOptionInfo_org, 0, sizeof(ITEM_OPTION_INFO)*TABCELL_PETINVEN_NUM );
	memset( &m_WareItemInfo, 0, sizeof(m_WareItemInfo) );
	memset( m_WareItemOptionInfo, 0, sizeof(ITEM_OPTION_INFO)*TABCELL_PETINVEN_NUM );

	m_pItemDlg = NULL;

	m_wPower = eOperPower_Max;

	m_dwPetDBIdx = NULL;

}

CPet::~CPet()
{
	ReleasePetTable();
}

void CPet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPet)
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_COMB_PETWARE, m_cbPetWare);
	DDX_Control(pDX, IDC_CMRB_ACTION, m_cmrbPetAction);
	DDX_Control(pDX, IDC_CMRB_ALIVE, m_cmrbPetAlive);
	DDX_Control(pDX, IDC_ET_INDEX, m_etindex);
	DDX_Control(pDX, IDC_ET_TYPE, m_ettype);
	DDX_Control(pDX, IDC_ET_OWNER, m_etowner);

	DDX_Control(pDX, IDC_BTN_USERSEARCH, m_btnUserSearch);
	DDX_Control(pDX, IDC_ET_SEARCH, m_etSearch);
	DDX_Control(pDX, IDC_LT_PET, m_lcPet);
	DDX_Control(pDX, IDC_COMB_PETWARE, m_cbPetWare);
	DDX_Control(pDX, IDC_BTN_PET_SAVE, m_btnpetsave);
	DDX_Control(pDX, IDC_USER_STATUS, m_stStatus);

	DDX_Control(pDX, IDC_ET_STATEACTIONVIEW, m_etactionview);
	DDX_Control(pDX, IDC_ET_STATEALIVEVIEW, m_etaliveview);
	//}}AFX_DATA_MAP

	//// 펫 악세사리 (3칸)
	for( int i = 0; i < SLOT_PETWEAR_NUM; ++i )
		DDX_Control(pDX, IDC_ACCESSORY1+i, m_cmrb_petacc[i]);

	//// 펫 인벤토리 (20칸)
	for( i = 0; i < TABCELL_PETINVEN_NUM; ++i )
		DDX_Control(pDX, IDC_BTN_PETWARE1+i, m_cmrb_petinven[i]);

	//// 정보창 관련 변수
	for( i = 0 ;i < ePetTemp_MAX ; ++i )
		DDX_Control(pDX, IDC_ET_STATEACTION+i, m_StatusEdit[i]);

	// 펫인벤 포지션 겹치는 거
	for( i = 0 ; i < ERRORSTRUCT ; ++i )
		DDX_Control(pDX, IDC_BTN_PETWARE21+i, m_InvenItemError[i]);

	//////////////////////////////////////////////////////////////////////	
}


BEGIN_MESSAGE_MAP(CPet, CLDPropPage)
	//{{AFX_MSG_MAP(CMunpaInfo)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedSearch)
	ON_BN_CLICKED(IDC_CMRB_ACTION, OnBnClickedStateActive)
	ON_BN_CLICKED(IDC_CMRB_ALIVE, OnBnClickedStateAlive)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBnClickedRecover)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_USERSEARCH, OnUserbtnSearch)
	ON_NOTIFY(NM_CLICK, IDC_LT_PET, OnClickPetList)
	ON_NOTIFY(NM_DBLCLK, IDC_LT_PET, OnDblclkPetListctrl)
	ON_CBN_SELCHANGE(IDC_COMB_PETWARE, OnSelchangeComboPetware)
	ON_BN_CLICKED(IDC_BTN_PET_SAVE, OnBnClickedPetSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CPet 메시지 처리기입니다.
BOOL CPet::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	switch( nCode )
	{
	case BN_CLICKED:
		{
			ButtonControlCommand( nID );
		}
		break;
	case EN_KILLFOCUS:
		{
			EditControlCommand(nID);
		}
		break;
	case EN_CHANGE:
		{
			if( (nID >= IDC_ET_STATEACTION && nID <= IDC_ET_STAMINA) )
			{
				char temp[255] = {0,};
                if (nID == IDC_ET_STATEACTION) { // IDC_ET_STATEACTION 값(0,1) 이 바뀔경우 
					GetStateRest( m_StatusEdit[ePetTemp_REST].GetData() , temp );   // 해당하는 txt값으로 활동,휴식 값을 바꿈
					m_etactionview.SetWindowText(temp);
				}
				if (nID == IDC_ET_STATEALIVE) { // IDC_ET_STATEALIVE 값(0,1) 이 바뀔경우 
					GetStateAlive( m_StatusEdit[ePetTemp_ALIVE].GetData() , temp ); // 해당하는 txt값으로 생존유무 값을 바꿈 
					m_etaliveview.SetWindowText(temp);
				}

				CMREdit* pEdit = (CMREdit*)GetDlgItem(nID);
				if( pEdit) pEdit->SetChanged(TRUE);
			}
		}
		break;
	}

	return CLDPropPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPet::ButtonControlCommand( UINT nId )
{
	CMRButton* pBtn = (CMRButton*)GetDlgItem( nId );
	if( !pBtn )	return;

	int result, strreturn;

	char	ownername[17];
	
	m_etowner.GetWindowText(ownername,17);
	strreturn =  strcmp(ownername, "");

	if (strreturn == 0) return;
	

	// 펫 인벤 버튼을 클릭한 경우 (20칸)
	if( nId >= IDC_BTN_PETWARE1 && nId < IDC_BTN_PETWARE1+TABCELL_PETINVEN_NUM )
	{
		// 인벤 위치값을 가져옴
		int tab = m_cbPetWare.GetCurSel()*TABCELL_PETINVEN_NUM;
		WORD wPos = tab + nId - IDC_BTN_PETWARE1;

		// 기본 정보
		ITEMBASE* origin = &m_WareItemInfo_org.PetInven[wPos];
		ITEMBASE* base_before = &m_WareItemInfo.PetInven[wPos];
		ITEMBASE base_after;
		
		// 옵션 정보
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
	
		// 변경창뜸
		result = ItemEdit( eIF_PETINVEN, wPos, base_before, base_after, pOptionInfo, Option_after );
		
		if( result == eMR_Cancel || result == eMR_Abort ) return;

		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_PETINVEN_START + wPos;
		// 수정
		ResultItemEdit( eIF_PETINVEN, result, TP_PETINVEN_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );

	}
	else if( nId >= IDC_ACCESSORY1 && nId < IDC_ACCESSORY1+SLOT_PETWEAR_NUM )		// 펫 악세서리 버튼을 클릭한 경우 (3칸)
	{
		WORD wPos = nId - IDC_ACCESSORY1;

		// 기본 정보
		ITEMBASE* origin = &m_AccItemInfo_org.PetAcc[wPos];
		ITEMBASE* base_before = &m_AccItemInfo.PetAcc[wPos];
		ITEMBASE base_after;
		
		// 옵션 정보
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
	
		// 변경창뜸
		result = ItemEdit( eIF_PETACC, wPos, base_before, base_after, pOptionInfo, Option_after );
		
		if( result == eMR_Cancel || result == eMR_Abort ) return;

		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_PETWEAR_START + wPos;
		// 변경
		ResultItemEdit( eIF_PETACC, result, TP_PETWEAR_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else if( nId >= IDC_BTN_PETWARE21 && nId < IDC_BTN_PETWARE25+1 )	// 에러 아이템 버튼 클릭한경우 (5칸)
	{
		WORD wPos = nId - IDC_BTN_PETWARE21;

		// 기본 정보
		ITEMBASE* base_before = &m_InvenInfoError.ItemInfo[wPos];
		
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
		{
			MessageBox( "Can't Change Item Position!!" );
			return;
		}

		base_before->Position = pos + TP_PETINVEN_START;
		memcpy( &m_WareItemInfo.PetInven[pos], base_before, sizeof(ITEMBASE) );

		ResultPositionEdit( 0, base_before );

		DWORD id = IDC_BTN_PETWARE1 + pos;

		if( pos >= SLOT_PETINVEN_NUM )
			return;

		char buff[256] = {0,};
		ITEM_INFO* pInfo;

		if( base_before->wIconIdx != 0)
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(base_before->wIconIdx);
			if( pInfo )
				sprintf(buff, pInfo->ItemName);
			else
				sprintf(buff, "%d", base_before->wIconIdx);
		}
		else
			sprintf(buff, "");

		CMRButton* btn = (CMRButton*)GetDlgItem(id);
		btn->SetWindowText(buff);
		btn->SetData(base_before->wIconIdx);
		btn->SetColorText(COLOR_BUTTON_CHANGED);

		btn = (CMRButton*)GetDlgItem(nId);
		btn->Release();
		btn->ShowWindow( FALSE );
	}

}

void CPet::EditControlCommand(UINT nId)
{

	WORD wPos;
	CChangeData* pData = NULL;
	CString str1, str2;
	DWORD before_data, after_data;

	if( nId >= IDC_ET_STATEACTION &&  nId <= IDC_ET_STAMINA ) // pet 기본 정보 변경인 경우
	{
		wPos = (WORD)(nId - IDC_ET_STATEACTION);	// Tool 데이터의 배열인덱스

		if( wPos > ePetTemp_STAMINA )		// tool 데이터의 배열 인덱스 값보다 클경우 오류
			return;

		BYTE type = 0;


		/////////////////////////////////////////////////////
		// error check (이전 데이터로 전환)
		TABLE_KIND kind = GetTableKindByControlID(nId);

		if( m_StatusEdit[wPos].GetWindowTextLength() == 0 || kind == eTK_None )
		{
			if( m_StatusEdit[wPos].IsTextTypeData() )
				m_StatusEdit[wPos].GetStringData(str1.GetBuffer(0));
			else
			{
				before_data = m_StatusEdit[wPos].GetData();
				str1.Format("%d", before_data);
			}

			m_StatusEdit[wPos].SetWindowTextEx(str1);
			m_StatusEdit[wPos].SetChanged(FALSE);
			return;
		}

		// 변경된 데이터가 있는지 체크한다.
		if( !m_StatusEdit[wPos].IsChanged() )
			return;
		else
			m_StatusEdit[wPos].SetChanged(FALSE);
		/////////////////////////////////////////////////////

		// str1:변경Data, str2:이전Data
		m_StatusEdit[wPos].GetWindowText(str2);
		if( m_StatusEdit[wPos].IsTextTypeData() )
		{
			m_StatusEdit[wPos].GetStringData(str1.GetBuffer(0));

			if( strcmp(str1, str2) == 0 ) // 변경된 데이타가 같은경우
			{
				CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
				pEdit->ResetEditTextColor();
				pEdit->SetInSave(FALSE);
				m_pDocument->GetChangeManager()->Delete(nId);
				return;
			}
		}
		else
		{
			before_data = m_StatusEdit[wPos].GetData();

			m_StatusEdit[wPos].CheckMaxValue(str2.GetBuffer(0));
			m_StatusEdit[wPos].GetWindowText(str2);
			after_data = (DWORD)atoi(str2.GetBuffer(0));

			// 데이타가 변경되지 않았고 action, alive 버튼을 클릭하지 않은경우
			if( (before_data == after_data) && nId != IDC_ET_STATEACTION && nId != IDC_ET_STATEALIVE)
			{
				CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
				pEdit->ResetEditTextColor();
				pEdit->SetInSave(FALSE);
				m_pDocument->GetChangeManager()->Delete(nId);
				return;
			}

			str1.Format("%d", before_data);
			type = 1;
		}

		pData = (CChangeData*)m_pDocument->GetChangeManager()->ExistChangeInfo(nId);

		if( !pData )
		{
			pData = new CChangeData;

			pData->Init(eOp_Update, kind, wPos);
			pData->SetData(str1.GetBuffer(0), str2.GetBuffer(0), type);
		}
		else
		{
			pData->Init(eOp_Update, kind, wPos);
			pData->SetData(str1.GetBuffer(0), str2.GetBuffer(0), type);
			return;
		}

	}
	else
		return;


	// Control의 Index를 Key값으로 한다.
	m_pDocument->GetChangeManager()->Add(pData, nId);
	CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
	pEdit->SetEditTextColor(COLOR_EDIT_CHANGED);
	pEdit->SetInSave();

}


BOOL CPet::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

	char temp[256] = {0, };
	for( int i = 0; i < TAB_PET_INVENTORY_NUM; ++i )
	{
		sprintf( temp, "Tab%d", i+1 );
		m_cbPetWare.AddString( temp );
	}
	m_cbPetWare.SetCurSel( 0 );
	
	m_cbPetWare.EnableWindow( FALSE );
	
	m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)) ); // 로그인 상태

	InitPetListControl();
	UpdateData( FALSE );

	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	m_wPower = pApp->GetCurOperatorPower();
	
	if (m_dwPetDBIdx != NULL)
	{
		m_etindex.SetWindowText( itoa(m_dwPetDBIdx, temp, 10 ) );	
		OnBnClickedSearch();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 아이템 변경창
int CPet::ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after,
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

// 아이템 변경 적용
BOOL CPet::ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
								 ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption )
{
	// 실제 아이템의 포지션을 받는다. wPos
	CChangeManager* pManager = m_pDocument->GetChangeManager();

	// 어떤 DB Table에 넣을것인지를 알아 온다.
	TABLE_KIND kind = pManager->GetTableKind_ByField(field);
	if( kind == eTK_None )
		return FALSE;

	BOOL bAdd = FALSE;
	CChangeItem* pItem = (CChangeItem*)pManager->ExistChangeInfo(wPos, FALSE);

	if( !pItem ) // 기존 수정 정보가 있는지 검사한다. (있어도 무시!)
	{
		pItem = new CChangeItem;
		bAdd = TRUE;
	}

	if( result == eMR_Delete )// Delete인 경우
	{ 
		/////////////////// Local Info의 수정
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, (WORD)pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		memset(pBItem, 0, sizeof(ITEMBASE));
		memset(pAItem, 0, sizeof(ITEMBASE));
		///////////////////

		/////////////////// DB Info의 수정
		pItem->Init( eOp_Delete, kind, wPos );
		pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		///////////////////
	}
	else if( result == eMR_Edit )
	{
		/////////////////// Local Info의 수정
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, (WORD)pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		if( pAItem->Durability == 9999 ) // 옵션 셋팅
		{
			if( OriginOption && OriginOption->dwOptionIdx != 0 )
			{
				pAOption->dwOptionIdx = OriginOption->dwOptionIdx;
				m_pDocument->AddLocalOptionInfo(pAOption, FALSE);
			}
			else
			{
				WORD wOptionIdx = (WORD)m_pDocument->AddLocalOptionInfo(pAOption, TRUE);
				pAOption->dwOptionIdx = wOptionIdx;
			}
			pAItem->Durability = (WORD)pAOption->dwOptionIdx;
		}

		memcpy(pBItem, pAItem, sizeof(ITEMBASE)); // 기본 아이템 변경
		///////////////////

		/////////////////// DB Info의 수정
		if( Origin->dwDBIdx == 0 ) // Insert인 경우
		{
			pItem->Init( eOp_Insert, kind, wPos );
			pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		}
		else // Update인 경우
		{ 
			pItem->Init( eOp_Update, kind, wPos );
			pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		}
		///////////////////
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	if( bAdd )
		pManager->Add( pItem, wPos, FALSE );

	////////////////////////////////////////////////////////////
	// Button Text Setting
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	if( pAItem->wIconIdx != 0)
	{
		pInfo = GAMERESRCMNGR->GetItemInfo(pAItem->wIconIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", pAItem->wIconIdx);
	}
	else
		sprintf(buff, "");

	CMRButton* btn = (CMRButton*)GetDlgItem(nControlIdx);
	btn->SetWindowText(buff);
	btn->SetData(pAItem->wIconIdx);
	btn->SetColorText(COLOR_BUTTON_CHANGED);
	////////////////////////////////////////////////////////////

	return TRUE;
    
}

void CPet::ReleasePetTable()
{
	PETBASEINFO* pData = NULL;
	m_PetTable.SetPositionHead();
	while( pData = m_PetTable.GetData() )
		delete pData;
	m_PetTable.RemoveAll();
}

void CPet::OnBnClickedSearch() 
{
	// TODO: Add your control notification handler code here

	DWORD dw;
	int nSearchType = -1;

	if( m_etindex.GetWindowTextLength() != 0 )
	{
		nSearchType = 0;
		dw = GetDlgItemInt(IDC_ET_INDEX);
	}

	if( nSearchType == -1 )
	{
		// 검색어 입력 에러 메세지
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	SearchbtnEnable(FALSE); // 검색동안 search버튼들을 비활성화 한다.

	// 서버에 검색 요청
	TMSG_PETSEARCH PetSearch;
	PetSearch.Category = MP_RMTOOL_PET;
	PetSearch.Protocol = MP_RMTOOL_PETSEARCH_SYN;
	PetSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	PetSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	PetSearch.dwPetIndex = dw;		// pet index

	NETWORK->Send(&PetSearch, sizeof(PetSearch));

}

// 액션 상태 변경
void CPet::OnBnClickedStateActive() 
{
	// TODO: Add your control notification handler code here
//	DWORD dw;
	CString tempcs;
	int temp;
	int nSearchType = -1;

	if( m_StatusEdit[ePetTemp_REST].GetWindowTextLength() != 0 )
	{
		nSearchType = 0;
		m_StatusEdit[ePetTemp_REST].GetWindowText(tempcs);
		temp = atoi((char*)(LPCTSTR)tempcs);
	}

	if( nSearchType == -1 )
	{
		// 검색어 입력 에러 메세지
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	if (temp == 0)	// Rest(0) 일경우 버튼을 클릭하면 action(1)으로 바뀐다.
		m_StatusEdit[ePetTemp_REST].SetWindowTextEx( (LPCSTR)"1",1);
	else if (temp == 1) // action(1) 일경우 버튼을 클릭하면 rest(0)로 바뀐다.
		m_StatusEdit[ePetTemp_REST].SetWindowTextEx( (LPCSTR)"0",1);

	EditControlCommand(IDC_ET_STATEACTION);

}

// 생존유무 상태 변경
void CPet::OnBnClickedStateAlive() 
{
	// TODO: Add your control notification handler code here
	CString tempcs;
	int temp;
	int nSearchType = -1;

	if( m_StatusEdit[ePetTemp_ALIVE].GetWindowTextLength() != 0 )
	{
		nSearchType = 0;
		m_StatusEdit[ePetTemp_ALIVE].GetWindowText(tempcs);
		temp = atoi((char*)(LPCTSTR)tempcs);
	}

	if( nSearchType == -1 )
	{
		// 검색어 입력 에러 메세지
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	if (temp == 0)	// Death(0) 일경우 버튼을 클릭하면 alive(1)으로 바뀐다.
		m_StatusEdit[ePetTemp_ALIVE].SetWindowTextEx( (LPCSTR)"1",1);
	else if (temp == 1) // alive(1) 일경우 버튼을 클릭하면 Death(0)로 바뀐다.
		m_StatusEdit[ePetTemp_ALIVE].SetWindowTextEx( (LPCSTR)"0",1);

	EditControlCommand(IDC_ET_STATEALIVE);

}


void CPet::InitPetInfo() 
{

	m_ettype.SetWindowText(""); // Pet Type

	/// 펫 인벤토리 버튼 초기화
	for(int i = 0;i<TABCELL_PETINVEN_NUM;++i)
	{
		m_cmrb_petinven[i].SetWindowText("");
		m_cmrb_petinven[i].SetData(0);
		m_cmrb_petinven[i].ReSetColorText();
	}

	
	m_etowner.SetWindowText(" "); // 팻 소유자

	for (int i = 0; i < ePetTemp_MAX; ++i)	// 팻 기본정보 변경edit
		m_StatusEdit[i].SetWindowText(" ");
}

//current state rest 의 숫자값을 가져와서 문자형태로 바꿈
char* CPet::GetStateRest( DWORD dwStateRest, char* temp )
{
	switch( dwStateRest )
	{
	case 0:		sprintf( temp, "Action" );	break;
	case 1:		sprintf( temp, "Rest" );	break;
	}
	return temp;
}

//current state alive 의 숫자값을 가져와서 문자형태로 바꿈
char* CPet::GetStateAlive( DWORD dwStateAlive, char* temp )
{
	switch( dwStateAlive )
	{
	case 0:		sprintf( temp, "Death" );	break;
	case 1:		sprintf( temp, "Alive" );	break;
	}
	return temp;
}

// 펫 창고 정보
void CPet::SetPetWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg )
{

	DWORD pos = 0;

	memset(&m_WareItemInfo_org, 0, sizeof(m_WareItemInfo_org));
	memset(&m_InvenInfoError, 0, sizeof(m_InvenInfoError));

	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		pos = pMsg->ItemInfo[i].Position;
		if( pos >= TP_PETINVEN_START && pos < TP_PETINVEN_END )
		{
			pos = pMsg->ItemInfo[i].Position - TP_PETINVEN_START;
			if( m_WareItemInfo_org.PetInven[pos].dwDBIdx == 0 )
				m_WareItemInfo_org.PetInven[pos] = pMsg->ItemInfo[i];
			else
				m_InvenInfoError.Add( &pMsg->ItemInfo[i] );  // 오류값을 기록
		}
		else
		{
			MessageBox( "PetWareItem Position Error!!", "Error!" );
		}
	}

	m_WareItemInfo = m_WareItemInfo_org;

	m_cbPetWare.EnableWindow( TRUE );
//	OnSelchangeComboPetware();

}

// 펫 악세서리 정보
void CPet::SetPetAccItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg )
{

	DWORD pos = 0;

	memset(&m_AccItemInfo_org, 0, sizeof(m_AccItemInfo_org));
	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		pos = pMsg->ItemInfo[i].Position;
		if( pos >= TP_PETWEAR_START && pos < TP_PETWEAR_END )
		{
			pos = pMsg->ItemInfo[i].Position - TP_PETWEAR_START;
			if( m_AccItemInfo_org.PetAcc[pos].dwDBIdx == 0 )
				m_AccItemInfo_org.PetAcc[pos] = pMsg->ItemInfo[i];
			else
				m_InvenInfoError.Add( &pMsg->ItemInfo[i] );  // 오류값을기록
		}
		else
		{
			MessageBox( "PetAccItem Position Error!!", "Error!" );
		}
	}

	m_AccItemInfo = m_AccItemInfo_org;

	m_cbPetWare.EnableWindow( TRUE );
	OnSelchangeComboPetware();

	SetInvenInfoError( &m_InvenInfoError );  // 오류값을 셋팅
}

void CPet::OnSelchangeComboPetware() 
{
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	int nCurSel = m_cbPetWare.GetCurSel();
	int tab = nCurSel*TABCELL_PETINVEN_NUM;

	// Item Setting
	for(int i = 0; i < TABCELL_PETINVEN_NUM; ++i )
	{
		if( m_WareItemInfo.PetInven[tab+i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(m_WareItemInfo.PetInven[tab+i].wIconIdx);
			if( pInfo )	sprintf(buff, pInfo->ItemName);
			else		sprintf(buff, "%d", m_WareItemInfo.PetInven[tab+i].wIconIdx);
		}
		else	sprintf(buff, "");

		m_cmrb_petinven[i].SetWindowText(buff);
		m_cmrb_petinven[i].SetData(m_WareItemInfo.PetInven[tab+i].wIconIdx);
	}

	// Acc Item Setting
	for(int i = 0; i < SLOT_PETWEAR_NUM; ++i )
	{
		if( m_AccItemInfo.PetAcc[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(m_AccItemInfo.PetAcc[i].wIconIdx);
			if( pInfo )	sprintf(buff, pInfo->ItemName);
			else		sprintf(buff, "%d", m_AccItemInfo.PetAcc[i].wIconIdx);
		}
		else	sprintf(buff, "");

		m_cmrb_petacc[i].SetWindowText(buff);
		m_cmrb_petacc[i].SetData(m_AccItemInfo.PetAcc[i].wIconIdx);
	}

}

void CPet::OnUserbtnSearch() //계정에 해당하는 캐릭터별 팻 리스트를 가져옴
{	

	InitPetInfo(); // 팻 정보 콘트롤 초기화
	InitControl(); // 팻 변수 초기화

	m_lcPet.DeleteAllItems(); 

	if( m_etSearch.GetWindowTextLength() == 0 )
	{
		MessageBox("Input Search String!!", "Error", MB_OK);
		return;
	}

	CString str;
	m_etSearch.GetWindowText( str );

	if( str.GetLength() > 16 )
	{
		MessageBox("Long Search Data!!", "Error", MB_OK);
		return;
	}

	SearchbtnEnable(FALSE); // 검색동안 search버튼들을 비활성화 한다.

	// 서버에 검색 요청
	TMSG_QUERYMEMBERNUM Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PET_MEMBERNUM_SYN;
	Msg.dwTemplateIdx = GetDocument()->GetDocumentIdx();
	Msg.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	Msg.wType = 0;	// 계정 명으로만 검색
	strcpy( Msg.strSearch, str.GetBuffer(0) );

	NETWORK->Send( &Msg, sizeof(TMSG_QUERYMEMBERNUM) );

}

CPetDoc* CPet::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPetDoc)));
	return (CPetDoc*)m_pDocument;
}

void CPet::InitPetListControl()
{
	char* column[7] = { "PetIdx", "Type", "Level", "Stamina", "Exp" , "Chr_idx", "Chr_Name"};
	int columnsize[7] = { 50, 64, 50, 60, 70, 0, 110 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 7; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = columnsize[i];
		lvcolumn.pszText = column[i];
		m_lcPet.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcPet.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcPet.SetExtendedStyle( dwExStyle );
	
}

void CPet::SetPetInfo( TMSG_PETINFO* pMsg )
{

	InitPetInfo();
	InitControl();

	PETBASEINFO* pData = &pMsg->PetInfo[0];
	char temp[256] = {0, };
	m_ettype.SetWindowText( GAMERESRCMNGR->GetPetNamechar(pData->dwKind) );		// 06.08.04 - pkh 
	m_etindex.SetWindowText( (LPCTSTR)itoa(pData->dwPetDBIdx, temp, 10));
	m_StatusEdit[ePetTemp_REST].SetWindowTextEx( (LPCTSTR)itoa(pData->wRest, temp, 10),1);
	m_StatusEdit[ePetTemp_ALIVE].SetWindowTextEx( (LPCTSTR)itoa(pData->wAlive,temp, 10),1);
	m_StatusEdit[ePetTemp_GRADE].SetWindowTextEx( (LPCTSTR)itoa(pData->dwGrade,temp, 10), 1);
	m_StatusEdit[ePetTemp_FRIENDSHIP].SetWindowTextEx( (LPCTSTR)itoa(pData->dwFriendShip,temp, 10), 1);
	m_StatusEdit[ePetTemp_STAMINA].SetWindowTextEx( (LPCTSTR)itoa(pData->dwStamina,temp, 10), 1);
	m_etowner.SetWindowText( (LPCTSTR)itoa(pData->dwMasterIdx,temp, 10));
	m_etowner.SetWindowText( pData->Loginid);
}

void CPet::SetUserLoginChk( TMSG_USERLOGINCHK* pMsg )
{

	USERLOGINCHK* pData = &pMsg->UserLoginChk[0];
	
	if( pData->UserLoginChk == 1 )	// 로그인 로그아웃 상태를 보여줌
		m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3)) );
	else
		m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2)) );

}

void CPet::SetPetList( DWORD Count, TMSG_PETLISTRESULT* pMsg  )
{
	m_lcPet.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( DWORD i = 0; i < Count; ++i )
	{
		
		PETLIST* pData = &pMsg->sPetList[i];

		lvitem.iItem = m_lcPet.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pData->dwPetDBIdx, temp, 10 );							//pet index
		
		m_lcPet.InsertItem( &lvitem );
												// 06.08.04 - pkh
		m_lcPet.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetPetNamechar(pData->dwKind));// pet 종류
		m_lcPet.SetItemText( lvitem.iItem, 2, itoa(pData->dwGrade, temp, 10 ) );		// pet 등급
		m_lcPet.SetItemText( lvitem.iItem, 3, itoa(pData->dwStamina, temp, 10 ) );		// pet stamina
		m_lcPet.SetItemText( lvitem.iItem, 4, itoa(pData->dwFriendShip, temp, 10 ) );	// pet 친밀도
		m_lcPet.SetItemText( lvitem.iItem, 5, itoa(pData->dwChrIdx, temp, 10 ) );		// pet을 소유한 캐릭터 index
		m_lcPet.SetItemText( lvitem.iItem, 6, pData->dwChr_Name);						// pet을 소유한 캐릭터 name
		
	}
}

// 팻 리스트 클릭시 pet index 항목에 petidx 값을 넣어줌 (검색안함)
void CPet::OnClickPetList(NMHDR* pNMHDR, LRESULT* pResult) 
{

	POSITION pos = m_lcPet.GetFirstSelectedItemPosition();

	if( pos == NULL )
	{
		return;
	}

	int nCurSelectedIdx = m_lcPet.GetNextSelectedItem(pos);
	
	CString str = m_lcPet.GetItemText( nCurSelectedIdx, 1 );

	if( str != "Create" )
	{

		str = m_lcPet.GetItemText( nCurSelectedIdx, 0 );
		m_etindex.SetWindowText( (char*)(LPCTSTR)str );	

	}
	else
	{
		return;
	}


	//초기화
	InitPetInfo();
	InitControl();

	*pResult = 0;


}

// 팻 리스트 클릭시 pet index 항목에 petidx 값을 넣어줌 (검색함)
void CPet::OnDblclkPetListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{

	/* 팻리스트 더블클릭을 빨리 할경우 rm툴이 먹통이 되는 경우가 생겨서 검색 막음
//	CString cstemp;

	POSITION pos = m_lcPet.GetFirstSelectedItemPosition();
	if( pos == NULL )
	{
		return;
	}

	int nCurSelectedIdx = m_lcPet.GetNextSelectedItem(pos);
	CString str = m_lcPet.GetItemText( nCurSelectedIdx, 1 );
	if( str != "Create" )
	{
		str = m_lcPet.GetItemText( nCurSelectedIdx, 0 );
//		m_etindex.GetWindowText(cstemp);
//		if( str == cstemp)	return;

		m_etindex.SetWindowText( (char*)(LPCTSTR)str );
	}
	else
	{
		return;
	}

	*pResult = 0;

	InitPetInfo();
	InitControl();

	OnBnClickedSearch(); // pet index에 해당하는 정보 검색
	*/

}

void CPet::OnBnClickedPetSave() 
{
	if( m_wPower != eOperPower_Master && m_wPower != eOperPower_QA )
		return;

	SearchbtnEnable(FALSE);

	m_pDocument->ProcessChangeManager();
}

TABLE_KIND CPet::GetTableKindByControlID(UINT nId)
{
	TABLE_KIND kind = eTK_None;

	if( nId >= IDC_ET_STATEACTION &&  nId <= IDC_ET_STAMINA)  // 펫 기본 정보를 수정한 경우
	{
		kind = eTK_PetInfo;
	}
	else if( nId >= IDC_BTN_PETWARE1 &&  nId <= IDC_ACCESSORY1+SLOT_PETWEAR_NUM ) // 펫 창고, 악세서리를 수정한경우
	{
		kind = eTK_PetItem;
	}
	return kind;
}

void CPet::GetChangeBaseInfo(TMSG_PET_UPDATEBASEINFO* msg)
{
	char temp[255] = {0,};
	msg->wRest = (WORD)m_StatusEdit[ePetTemp_REST].GetData();
	msg->wAlive = (WORD)m_StatusEdit[ePetTemp_ALIVE].GetData();
	msg->wGrade = (WORD)m_StatusEdit[ePetTemp_GRADE].GetData();
	msg->dwFriendShip = m_StatusEdit[ePetTemp_FRIENDSHIP].GetData();
	msg->dwStamina = m_StatusEdit[ePetTemp_STAMINA].GetData();
}

// 펫 아이템 복구버튼 클릭
void CPet::OnBnClickedRecover() 
{
	// TODO: Add your control notification handler code here
		int		strreturn;
		char	ownername[17];
		
		m_etowner.GetWindowText(ownername,17);
		strreturn =  strcmp(ownername, "");

		if (strreturn == 0) return;

		m_pItemRecoverDlg = new CItemRecoverDlg;
		int result = m_pItemRecoverDlg->DoModal();
		if( result == 99 )
		{
			int total = 0;
			int count = m_pItemRecoverDlg->m_nItemNum / 5;
			int r = m_pItemRecoverDlg->m_nItemNum % 5;
			if( r )
				total = count + 1;
			else
				total = count;
			
			if( total > GetEmptyPositionCount( 0 ) )
			{
				MessageBox( "인벤토리가 충분하지 않습니다.!!", "Error!!" );
			}
			else
			{
				RecoverInven( m_pItemRecoverDlg->m_nItemIdx, count, r ); // 복구모듈
			}
		}
		SAFE_DELETE(m_pItemRecoverDlg);
}

// 펫 인벤 빈칸 카운트
int CPet::GetEmptyPositionCount( WORD wType )
{
	int count = 0;
	int i = 0;
	switch( wType )
	{
	case 0:		// inven
		{
			for( i = 0; i < SLOT_PETINVEN_NUM; ++i )
			{
				if( m_WareItemInfo_org.PetInven[i].dwDBIdx == 0 &&
					m_WareItemInfo_org.PetInven[i].wIconIdx == 0 )
					++count;
			}
		}
		break;
	}

	return count;
}

//복구모듈
void CPet::RecoverInven( DWORD dwItemIdx, int nCount, int nR )
{
	CChangeManager* pManager = m_pDocument->GetChangeManager();
	CChangeItem* pItem = NULL;
	ITEMBASE* pAItemBase = NULL;
	ITEM_INFO* pInfo = NULL;
	CMRButton* btn = NULL;
	char buff[256] = {0,};
	
	for( int i = 0; i < nCount; ++i )
	{
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
			return;
		pAItemBase = &m_WareItemInfo.PetInven[pos];
		pAItemBase->wIconIdx = (WORD)dwItemIdx;
		pAItemBase->Position = pos+TP_PETINVEN_START;
		pAItemBase->Durability = 5;
		pItem = new CChangeItem;
		pItem->Init( eOp_Insert, eTK_PetItem, pos );
		ITEM_OPTION_INFO Option;
		memset( &Option, 0, sizeof(Option) );
		pItem->SetData( pAItemBase, pAItemBase, &Option, &Option );

		pManager->Add( pItem, pos, FALSE );

		pInfo = GAMERESRCMNGR->GetItemInfo((WORD)dwItemIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", dwItemIdx);

		int nTab = m_cbPetWare.GetCurSel();
		int nControlIdx = (pos / 20);
		if( nTab == nControlIdx )
		{
			nControlIdx = IDC_BTN_PETWARE1 + (pos%20);
			btn = (CMRButton*)GetDlgItem(nControlIdx);
			btn->SetWindowText(buff);
			btn->SetData(dwItemIdx);
			btn->SetColorText(COLOR_BUTTON_CHANGED);
		}
	}

	if( nR )
	{
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
			return;
		pAItemBase = &m_WareItemInfo.PetInven[pos];
		pAItemBase->wIconIdx = (WORD)dwItemIdx;
		pAItemBase->Position = TP_PETINVEN_START + pos;
		pAItemBase->Durability = nR;
		pItem = new CChangeItem;
		pItem->Init( eOp_Insert, eTK_PetItem, pos );
		ITEM_OPTION_INFO Option;
		memset( &Option, 0, sizeof(Option) );
		pItem->SetData( pAItemBase, pAItemBase, &Option, &Option );

		pManager->Add( pItem, pos, FALSE );

		pInfo = GAMERESRCMNGR->GetItemInfo((WORD)dwItemIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", dwItemIdx);

		int nTab = m_cbPetWare.GetCurSel();
		int nControlIdx = (pos / 20);
		if( nTab == nControlIdx )
		{
			nControlIdx = IDC_BTN_PETWARE1 + (pos%20);
			btn = (CMRButton*)GetDlgItem(nControlIdx);
			btn->SetWindowText(buff);
			btn->SetData(dwItemIdx);
			btn->SetColorText(COLOR_BUTTON_CHANGED);
		}
	}
}

int CPet::GetEmptyPosition( DWORD dwType )
{
	int i = 0;
	switch( dwType )
	{
	case 0:		// inven
		{
			for( i = 0; i < SLOT_PETINVEN_NUM; ++i )
			{
				if( m_WareItemInfo.PetInven[i].dwDBIdx == 0 &&
					m_WareItemInfo.PetInven[i].wIconIdx == 0 )
					return i;
			}
			return -1;
		}
		break;
	}

	return -1;
}

// 각 변수들 초기화
void CPet::InitControl()
{
	int i = 0;
	for(i=0;i< ePetTemp_MAX;++i)
		m_StatusEdit[i].Release();
	for(i = 0;i<TABCELL_PETINVEN_NUM;++i)
		m_cmrb_petinven[i].Release();
	for(i=0;i<SLOT_PETWEAR_NUM;++i)
		m_cmrb_petacc[i].Release();
	for(i=0;i<ERRORSTRUCT;++i)
	{
		m_InvenItemError[i].Release();
		m_InvenItemError[i].ShowWindow( FALSE );
	}

}

// 에러 아이템 숨겨져있던 아이콘에 할당 & 화면보여줌
void CPet::SetInvenInfoError( TMSG_CHARACTER_ITEMBASE* inven )
{
	char buff[256] = {0,};
	DWORD i = 0;
	ITEM_INFO* pInfo = NULL;

	for( i = 0; i < inven->dwCount; ++i )
	{
		if( inven->ItemInfo[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( inven->ItemInfo[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", inven->ItemInfo[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_InvenItemError[i].SetWindowText( buff );
		m_InvenItemError[i].SetData( inven->ItemInfo[i].wIconIdx );	
		m_InvenItemError[i].ShowWindow( TRUE );	
	}
}

void CPet::ResultPositionEdit( DWORD dwType, ICONBASE* pItem )
{
	if( !pItem )
		return;

	CChangePosition* pPosition = new CChangePosition;

	switch( dwType )
	{
	case 0:		// inven
		{
			ITEMBASE* pp = (ITEMBASE*)pItem;
			pPosition->SetData( 0, m_pDocument->m_sBaseInfo.dwObjectID, pItem->dwDBIdx, pItem->Position );
		}
		break;
	}	

	m_pDocument->GetChangeManager()->AddPositionTable( pPosition, pItem->dwDBIdx );
}

HBRUSH CPet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = NULL;

	switch (nCtlColor) 
	{ 
	case CTLCOLOR_EDIT:
		{
			UINT nId = pWnd->GetDlgCtrlID();
			if( (nId >= IDC_ET_STATEACTION && nId <= IDC_ET_STAMINA) )
			{
				if( ((CMREdit*)pWnd)->HasSaveData() )
					pDC->SetTextColor( ((CMREdit*)pWnd)->GetEditTextColor() );
			}
			else
				hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
		}
		break;
	default:
			hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
		break;
	}

	return hbr;
}

void CPet::SearchbtnEnable(int BOOL)
{
	m_btnUserSearch.EnableWindow(BOOL);
	m_btnSearch.EnableWindow(BOOL);

	m_btnpetsave.EnableWindow(BOOL);
}