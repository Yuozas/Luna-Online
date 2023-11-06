// Pet.cpp : ���� �����Դϴ�.
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

// CPet ��ȭ �����Դϴ�.

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

	//// �� �Ǽ��縮 (3ĭ)
	for( int i = 0; i < SLOT_PETWEAR_NUM; ++i )
		DDX_Control(pDX, IDC_ACCESSORY1+i, m_cmrb_petacc[i]);

	//// �� �κ��丮 (20ĭ)
	for( i = 0; i < TABCELL_PETINVEN_NUM; ++i )
		DDX_Control(pDX, IDC_BTN_PETWARE1+i, m_cmrb_petinven[i]);

	//// ����â ���� ����
	for( i = 0 ;i < ePetTemp_MAX ; ++i )
		DDX_Control(pDX, IDC_ET_STATEACTION+i, m_StatusEdit[i]);

	// ���κ� ������ ��ġ�� ��
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
// CPet �޽��� ó�����Դϴ�.
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
                if (nID == IDC_ET_STATEACTION) { // IDC_ET_STATEACTION ��(0,1) �� �ٲ��� 
					GetStateRest( m_StatusEdit[ePetTemp_REST].GetData() , temp );   // �ش��ϴ� txt������ Ȱ��,�޽� ���� �ٲ�
					m_etactionview.SetWindowText(temp);
				}
				if (nID == IDC_ET_STATEALIVE) { // IDC_ET_STATEALIVE ��(0,1) �� �ٲ��� 
					GetStateAlive( m_StatusEdit[ePetTemp_ALIVE].GetData() , temp ); // �ش��ϴ� txt������ �������� ���� �ٲ� 
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
	

	// �� �κ� ��ư�� Ŭ���� ��� (20ĭ)
	if( nId >= IDC_BTN_PETWARE1 && nId < IDC_BTN_PETWARE1+TABCELL_PETINVEN_NUM )
	{
		// �κ� ��ġ���� ������
		int tab = m_cbPetWare.GetCurSel()*TABCELL_PETINVEN_NUM;
		WORD wPos = tab + nId - IDC_BTN_PETWARE1;

		// �⺻ ����
		ITEMBASE* origin = &m_WareItemInfo_org.PetInven[wPos];
		ITEMBASE* base_before = &m_WareItemInfo.PetInven[wPos];
		ITEMBASE base_after;
		
		// �ɼ� ����
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
	
		// ����â��
		result = ItemEdit( eIF_PETINVEN, wPos, base_before, base_after, pOptionInfo, Option_after );
		
		if( result == eMR_Cancel || result == eMR_Abort ) return;

		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_PETINVEN_START + wPos;
		// ����
		ResultItemEdit( eIF_PETINVEN, result, TP_PETINVEN_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );

	}
	else if( nId >= IDC_ACCESSORY1 && nId < IDC_ACCESSORY1+SLOT_PETWEAR_NUM )		// �� �Ǽ����� ��ư�� Ŭ���� ��� (3ĭ)
	{
		WORD wPos = nId - IDC_ACCESSORY1;

		// �⺻ ����
		ITEMBASE* origin = &m_AccItemInfo_org.PetAcc[wPos];
		ITEMBASE* base_before = &m_AccItemInfo.PetAcc[wPos];
		ITEMBASE base_after;
		
		// �ɼ� ����
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
	
		// ����â��
		result = ItemEdit( eIF_PETACC, wPos, base_before, base_after, pOptionInfo, Option_after );
		
		if( result == eMR_Cancel || result == eMR_Abort ) return;

		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_PETWEAR_START + wPos;
		// ����
		ResultItemEdit( eIF_PETACC, result, TP_PETWEAR_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else if( nId >= IDC_BTN_PETWARE21 && nId < IDC_BTN_PETWARE25+1 )	// ���� ������ ��ư Ŭ���Ѱ�� (5ĭ)
	{
		WORD wPos = nId - IDC_BTN_PETWARE21;

		// �⺻ ����
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

	if( nId >= IDC_ET_STATEACTION &&  nId <= IDC_ET_STAMINA ) // pet �⺻ ���� ������ ���
	{
		wPos = (WORD)(nId - IDC_ET_STATEACTION);	// Tool �������� �迭�ε���

		if( wPos > ePetTemp_STAMINA )		// tool �������� �迭 �ε��� ������ Ŭ��� ����
			return;

		BYTE type = 0;


		/////////////////////////////////////////////////////
		// error check (���� �����ͷ� ��ȯ)
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

		// ����� �����Ͱ� �ִ��� üũ�Ѵ�.
		if( !m_StatusEdit[wPos].IsChanged() )
			return;
		else
			m_StatusEdit[wPos].SetChanged(FALSE);
		/////////////////////////////////////////////////////

		// str1:����Data, str2:����Data
		m_StatusEdit[wPos].GetWindowText(str2);
		if( m_StatusEdit[wPos].IsTextTypeData() )
		{
			m_StatusEdit[wPos].GetStringData(str1.GetBuffer(0));

			if( strcmp(str1, str2) == 0 ) // ����� ����Ÿ�� �������
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

			// ����Ÿ�� ������� �ʾҰ� action, alive ��ư�� Ŭ������ �������
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


	// Control�� Index�� Key������ �Ѵ�.
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
	
	m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)) ); // �α��� ����

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

// ������ ����â
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

// ������ ���� ����
BOOL CPet::ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
								 ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption )
{
	// ���� �������� �������� �޴´�. wPos
	CChangeManager* pManager = m_pDocument->GetChangeManager();

	// � DB Table�� ������������ �˾� �´�.
	TABLE_KIND kind = pManager->GetTableKind_ByField(field);
	if( kind == eTK_None )
		return FALSE;

	BOOL bAdd = FALSE;
	CChangeItem* pItem = (CChangeItem*)pManager->ExistChangeInfo(wPos, FALSE);

	if( !pItem ) // ���� ���� ������ �ִ��� �˻��Ѵ�. (�־ ����!)
	{
		pItem = new CChangeItem;
		bAdd = TRUE;
	}

	if( result == eMR_Delete )// Delete�� ���
	{ 
		/////////////////// Local Info�� ����
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, (WORD)pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		memset(pBItem, 0, sizeof(ITEMBASE));
		memset(pAItem, 0, sizeof(ITEMBASE));
		///////////////////

		/////////////////// DB Info�� ����
		pItem->Init( eOp_Delete, kind, wPos );
		pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		///////////////////
	}
	else if( result == eMR_Edit )
	{
		/////////////////// Local Info�� ����
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, (WORD)pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		if( pAItem->Durability == 9999 ) // �ɼ� ����
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

		memcpy(pBItem, pAItem, sizeof(ITEMBASE)); // �⺻ ������ ����
		///////////////////

		/////////////////// DB Info�� ����
		if( Origin->dwDBIdx == 0 ) // Insert�� ���
		{
			pItem->Init( eOp_Insert, kind, wPos );
			pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		}
		else // Update�� ���
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
		// �˻��� �Է� ���� �޼���
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	SearchbtnEnable(FALSE); // �˻����� search��ư���� ��Ȱ��ȭ �Ѵ�.

	// ������ �˻� ��û
	TMSG_PETSEARCH PetSearch;
	PetSearch.Category = MP_RMTOOL_PET;
	PetSearch.Protocol = MP_RMTOOL_PETSEARCH_SYN;
	PetSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	PetSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	PetSearch.dwPetIndex = dw;		// pet index

	NETWORK->Send(&PetSearch, sizeof(PetSearch));

}

// �׼� ���� ����
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
		// �˻��� �Է� ���� �޼���
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	if (temp == 0)	// Rest(0) �ϰ�� ��ư�� Ŭ���ϸ� action(1)���� �ٲ��.
		m_StatusEdit[ePetTemp_REST].SetWindowTextEx( (LPCSTR)"1",1);
	else if (temp == 1) // action(1) �ϰ�� ��ư�� Ŭ���ϸ� rest(0)�� �ٲ��.
		m_StatusEdit[ePetTemp_REST].SetWindowTextEx( (LPCSTR)"0",1);

	EditControlCommand(IDC_ET_STATEACTION);

}

// �������� ���� ����
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
		// �˻��� �Է� ���� �޼���
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	if (temp == 0)	// Death(0) �ϰ�� ��ư�� Ŭ���ϸ� alive(1)���� �ٲ��.
		m_StatusEdit[ePetTemp_ALIVE].SetWindowTextEx( (LPCSTR)"1",1);
	else if (temp == 1) // alive(1) �ϰ�� ��ư�� Ŭ���ϸ� Death(0)�� �ٲ��.
		m_StatusEdit[ePetTemp_ALIVE].SetWindowTextEx( (LPCSTR)"0",1);

	EditControlCommand(IDC_ET_STATEALIVE);

}


void CPet::InitPetInfo() 
{

	m_ettype.SetWindowText(""); // Pet Type

	/// �� �κ��丮 ��ư �ʱ�ȭ
	for(int i = 0;i<TABCELL_PETINVEN_NUM;++i)
	{
		m_cmrb_petinven[i].SetWindowText("");
		m_cmrb_petinven[i].SetData(0);
		m_cmrb_petinven[i].ReSetColorText();
	}

	
	m_etowner.SetWindowText(" "); // �� ������

	for (int i = 0; i < ePetTemp_MAX; ++i)	// �� �⺻���� ����edit
		m_StatusEdit[i].SetWindowText(" ");
}

//current state rest �� ���ڰ��� �����ͼ� �������·� �ٲ�
char* CPet::GetStateRest( DWORD dwStateRest, char* temp )
{
	switch( dwStateRest )
	{
	case 0:		sprintf( temp, "Action" );	break;
	case 1:		sprintf( temp, "Rest" );	break;
	}
	return temp;
}

//current state alive �� ���ڰ��� �����ͼ� �������·� �ٲ�
char* CPet::GetStateAlive( DWORD dwStateAlive, char* temp )
{
	switch( dwStateAlive )
	{
	case 0:		sprintf( temp, "Death" );	break;
	case 1:		sprintf( temp, "Alive" );	break;
	}
	return temp;
}

// �� â�� ����
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
				m_InvenInfoError.Add( &pMsg->ItemInfo[i] );  // �������� ���
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

// �� �Ǽ����� ����
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
				m_InvenInfoError.Add( &pMsg->ItemInfo[i] );  // �����������
		}
		else
		{
			MessageBox( "PetAccItem Position Error!!", "Error!" );
		}
	}

	m_AccItemInfo = m_AccItemInfo_org;

	m_cbPetWare.EnableWindow( TRUE );
	OnSelchangeComboPetware();

	SetInvenInfoError( &m_InvenInfoError );  // �������� ����
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

void CPet::OnUserbtnSearch() //������ �ش��ϴ� ĳ���ͺ� �� ����Ʈ�� ������
{	

	InitPetInfo(); // �� ���� ��Ʈ�� �ʱ�ȭ
	InitControl(); // �� ���� �ʱ�ȭ

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

	SearchbtnEnable(FALSE); // �˻����� search��ư���� ��Ȱ��ȭ �Ѵ�.

	// ������ �˻� ��û
	TMSG_QUERYMEMBERNUM Msg;
	Msg.Category = MP_RMTOOL_PET;
	Msg.Protocol = MP_RMTOOL_PET_MEMBERNUM_SYN;
	Msg.dwTemplateIdx = GetDocument()->GetDocumentIdx();
	Msg.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	Msg.wType = 0;	// ���� �����θ� �˻�
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
	
	// listctrl column ����
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
	
	if( pData->UserLoginChk == 1 )	// �α��� �α׾ƿ� ���¸� ������
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

		lvitem.iItem = m_lcPet.GetItemCount();	// ��
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pData->dwPetDBIdx, temp, 10 );							//pet index
		
		m_lcPet.InsertItem( &lvitem );
												// 06.08.04 - pkh
		m_lcPet.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetPetNamechar(pData->dwKind));// pet ����
		m_lcPet.SetItemText( lvitem.iItem, 2, itoa(pData->dwGrade, temp, 10 ) );		// pet ���
		m_lcPet.SetItemText( lvitem.iItem, 3, itoa(pData->dwStamina, temp, 10 ) );		// pet stamina
		m_lcPet.SetItemText( lvitem.iItem, 4, itoa(pData->dwFriendShip, temp, 10 ) );	// pet ģ�е�
		m_lcPet.SetItemText( lvitem.iItem, 5, itoa(pData->dwChrIdx, temp, 10 ) );		// pet�� ������ ĳ���� index
		m_lcPet.SetItemText( lvitem.iItem, 6, pData->dwChr_Name);						// pet�� ������ ĳ���� name
		
	}
}

// �� ����Ʈ Ŭ���� pet index �׸� petidx ���� �־��� (�˻�����)
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


	//�ʱ�ȭ
	InitPetInfo();
	InitControl();

	*pResult = 0;


}

// �� ����Ʈ Ŭ���� pet index �׸� petidx ���� �־��� (�˻���)
void CPet::OnDblclkPetListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{

	/* �ָ���Ʈ ����Ŭ���� ���� �Ұ�� rm���� ������ �Ǵ� ��찡 ���ܼ� �˻� ����
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

	OnBnClickedSearch(); // pet index�� �ش��ϴ� ���� �˻�
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

	if( nId >= IDC_ET_STATEACTION &&  nId <= IDC_ET_STAMINA)  // �� �⺻ ������ ������ ���
	{
		kind = eTK_PetInfo;
	}
	else if( nId >= IDC_BTN_PETWARE1 &&  nId <= IDC_ACCESSORY1+SLOT_PETWEAR_NUM ) // �� â��, �Ǽ������� �����Ѱ��
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

// �� ������ ������ư Ŭ��
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
				MessageBox( "�κ��丮�� ������� �ʽ��ϴ�.!!", "Error!!" );
			}
			else
			{
				RecoverInven( m_pItemRecoverDlg->m_nItemIdx, count, r ); // �������
			}
		}
		SAFE_DELETE(m_pItemRecoverDlg);
}

// �� �κ� ��ĭ ī��Ʈ
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

//�������
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

// �� ������ �ʱ�ȭ
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

// ���� ������ �������ִ� �����ܿ� �Ҵ� & ȭ�麸����
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