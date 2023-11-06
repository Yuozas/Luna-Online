#include "stdafx.h"
#include "ExchangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cStatic.h"

#include "ExchangeManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "Item.h"

#include "ProgressDialog.h"
#include "InventoryExDialog.h"
#include "./Input/UserInput.h"

#include "ItemManager.h"
#include "QuickManager.h"

// 071125 LYW --- ExchangeDialog : �޽��� �ڽ� ��� ����.
#include "cMsgBox.h"


CExchangeDialog::CExchangeDialog()
{
	m_bInit				= FALSE;
	m_pMyIconGridDlg	= NULL;
	m_pOpIconGridDlg	= NULL;
	m_pMyMoneyEditBox	= NULL;
	m_pOpMoneyEditBox	= NULL;
	m_pLockBtn			= NULL;
	m_pExchangeBtn		= NULL;
	m_pInputMoneyBtn	= NULL;

	m_pMyNameStc		= NULL;
	m_pOpNameStc		= NULL;

	m_bMyLock			= FALSE;
	m_bOpLock			= FALSE;

	m_dwMoney			= 0;
	m_dwOpMoney			= 0;

	m_bExchangable		= TRUE;

	m_nMyItemNum		= 0;
	m_nOpItemNum		= 0;	
}

CExchangeDialog::~CExchangeDialog()
{

}

void CExchangeDialog::Linking()
{
	m_pMyIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
	m_pOpIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_OPPOSITE );
	m_pMyMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_MYMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );				//��ũ��Ʈ��
	m_pMyMoneyEditBox->SetAlign( TXT_RIGHT );
//	m_pMyMoneyEditBox->SetValidCheck( VCM_NUMBER );
	m_pOpMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_OPMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );
	m_pOpMoneyEditBox->SetAlign( TXT_RIGHT );
	m_pLockBtn			= (cPushupButton*)GetWindowForID( XCI_BTN_LOCK );
	m_pExchangeBtn		= (cPushupButton*)GetWindowForID( XCI_BTN_EXCHANGE );
	m_pInputMoneyBtn	= (cButton*)GetWindowForID( XCI_BTN_INPUTMONEY );
	
	m_pMyNameStc		= (cStatic*)GetWindowForID( XCI_INFO2 );
	m_pOpNameStc		= (cStatic*)GetWindowForID( XCI_INFO1 );
	//m_pLockImage		= WINDOWMGR->GetImageOfFile( "./Data/Interface/2DImage/GameIn/28/120210.tif", 28 );
	SCRIPTMGR->GetImage( 77, &m_LockImage, PFT_HARDPATH );

	m_bInit				= TRUE;
}
/*
DWORD CExchangeDialog::ActionEvent( CMouse* mouseInfo, CKeyboard* keyInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

//	LONG x = mouseInfo->GetClickDownX();
//	LONG y = mouseInfo->GetClickDownY();

//	if( mouseInfo->LButtonClick() )
//	{
//		we = m_pInputMoneyBtn->ActionEvent( mouseInfo, keyInfo );
//		if( !( we & WE_LBTNCLICK ) )
//		{
//			we = m_pMyMoneyEditBox->ActionEvent( mouseInfo, keyInfo );
//			if( !( we & WE_LBTNCLICK ) )
//				InputMoney( 0, m_dwMoney );		//�߰� : ������ �ȵǾ����� ���ʿ� ����
//		}
//	}
	
	return cDialog::ActionEvent(mouseInfo, keyInfo);
}
*/
void CExchangeDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == XCI_BTN_EXIT || lId == XCI_BTN_CLOSE )
		{
			MSGBASE msg;
			msg.Category	= MP_EXCHANGE;
			msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
			msg.dwObjectID	= HERO->GetID();

			NETWORK->Send( &msg, sizeof( MSGBASE ) );
			SetDisable( TRUE );
			//�̻��ϴ�! ���⼭ cancel�޼����� ������
			//ack������ �Ʒ���. confirm
//			SetActive( FALSE );
		}
		else if( lId == XCI_BTN_INPUTMONEY )
		{
			if( m_bMyLock )
			{
				//"��ݻ��¿����� �� �Է��� �ȵ˴ϴ�."
			}
			else
			{
				//���� �� �ɾ��ٸ� ����. //��ư �ȴ��������ϱ�.
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_MONEY,
								x, y, OnInputMoney, OnInputCancel, this, NULL, CHATMGR->GetChatMsg(29) );
				SetDisable( TRUE );
				//pDivideBox->SetMaxValue( MAX_MONEY );?
			}
		}
	}
	if( we & WE_PUSHDOWN )
	{
		if( lId == XCI_BTN_LOCK )
		{
			if( EXCHANGEMGR->IsLockSyncing() )
			{
				m_pLockBtn->SetPush( FALSE );	//DOWN���� ���ƾ� �Ѵ�.
			}
			else
			{
				MyLock( TRUE );
				EXCHANGEMGR->Lock( TRUE );	//NetworkMsg
			}
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			if( m_bMyLock && !EXCHANGEMGR->IsLockSyncing() && m_bOpLock )
			{
				//�κ��丮�� ���� �ľ�..
				int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
				if( nBlankNum < m_nOpItemNum - m_nMyItemNum )
				{
					//�߰� : ��ȯ�Ұ��� �޼���
					//�κ��丮 ������ �����մϴ�.
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 474 ) );

					m_pExchangeBtn->SetPush( FALSE );
				}
				else if( m_dwOpMoney + HERO->GetMoney() > MAX_INVENTORY_MONEY ) //��üũ
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(459) );

					m_pExchangeBtn->SetPush( FALSE );
				}
				else
				{
					//�߰� : �κ��丮 ��� ����	//quickâ��.. �κ��� �������� ��� �Ҵ�?
					//GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
					//SetDisable( TRUE );
					EXCHANGEMGR->Exchange();
				}
			}
			else
			{
				//���� ��� ���� �ؾ� ��ȯ �Ͻ� �� �ֽ��ϴ�.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(450) );
				m_pExchangeBtn->SetPush( FALSE );
			} 
		}
	}
	if( we & WE_PUSHUP )
	{
		if( lId == XCI_BTN_LOCK )
		{
			m_pLockBtn->SetPush( TRUE );	//UP���� ���ƾ� �Ѵ�.
/*
			if( EXCHANGEMGR->IsLockSyncing() || EXCHANGEMGR->IsExchangeSyncing() )
			{
				m_pLockBtn->SetPush( TRUE );	//UP���� ���ƾ� �Ѵ�.
			}
			else
			{
				EXCHANGEMGR->Lock( FALSE );	//network msg
			}
*/
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			m_pExchangeBtn->SetPush( TRUE );
		}
	}
	if( we & WE_LBTNCLICK )
	{
		if( lId == XCI_ICD_MINE )
		{
//			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem( INV );
//			cIconGridDialog* pIGD = (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
//			if( pItem )
//				pIGD->AddIcon( 0, 0, pItem );
		}
	}

}

void CExchangeDialog::SetPlayerName( char* pMyName, char* pOpName )
{
	m_pMyNameStc->SetStaticText( pMyName );
	m_pOpNameStc->SetStaticText( pOpName );
}

void CExchangeDialog::SetActive( BOOL val )
{
	if( m_bDisable )		return;
	if( m_bActive == val )	return;

	if( val )
	{
		CProgressDialog* progressDialog = ( CProgressDialog* )WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
		ASSERT( progressDialog );

		if( progressDialog->IsDisable() )
		{
			return;
		}

		cDialog* dissolveDialog		= WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
		cDialog* enchantDialog		= WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
		cDialog* reinforceDialog	= WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
		cDialog* mixDialog			= WINDOWMGR->GetWindowForID( ITMD_MIXDLG );

		ASSERT( dissolveDialog && enchantDialog && reinforceDialog && mixDialog );

		dissolveDialog->SetActive( FALSE );
		enchantDialog->SetActive( FALSE );
		reinforceDialog->SetActive( FALSE );
		mixDialog->SetActive( FALSE );
	}

//	cDialog::SetActiveRecursive( val );	//���Է�â�����̾��µ�.... ���ص� �ɵ� confirm
	cDialog::SetActive( val );
	
	if( val )
	{
		m_pMyMoneyEditBox->SetEditText( "0" );
		m_pOpMoneyEditBox->SetEditText( "0" );

		m_pLockBtn->SetPush( FALSE );
		m_pExchangeBtn->SetPush( FALSE );

		m_pInputMoneyBtn->SetDisable( FALSE );
		m_pInputMoneyBtn->SetActive( TRUE );		
	}
	else
	{
		if( !m_bInit ) return;	//Link() ȣ�� �� ����

		//������ �����
//		cIcon *pIcon = NULL;
		int i = 0;
		int nMyItemNum = m_nMyItemNum;
		int nOpItemNum = m_nOpItemNum;

		for( i = 0 ; i < nMyItemNum ; ++i )
			DelItem( 0, 1 );	//�߰� : ������
		for( i = 0 ; i < nOpItemNum ; ++i )
			DelItem( 1, 1 );	//�߰� : ������

		m_bMyLock	= FALSE;
		m_bOpLock	= FALSE;

		m_dwMoney	= 0;
		m_dwOpMoney = 0;


		m_bExchangable = TRUE;

		m_nMyItemNum		= 0;
		m_nOpItemNum		= 0;


//		���⿡ �� �ִ°���? confirm
//		MSGBASE msg;
//		msg.Category	= MP_EXCHANGE;
//		msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
//		msg.dwObjectID	= HERO->GetID();

//		NETWORK->Send( &msg, sizeof( MSGBASE ) );

		EXCHANGEMGR->Init();	//�̰͵�...

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
	}

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_EXCHANGEDLGICON, m_bActive );
}

void CExchangeDialog::Render()
{
	cDialog::Render();

	if( !m_LockImage.IsNull() )
	{
		int count = 0 ;

		VECTOR2 vPos;

		if( m_bMyLock )
		{
//			VECTOR2 vPos = { m_pMyIconGridDlg->m_absPos.x + 3, m_pMyIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos,
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( count = 0 ; count < 12 ; ++count )
			{
				//if( !m_pMyIconGridDlg->IsAddable( i ) ) continue;
				vPos.x = ( m_pMyIconGridDlg->m_absPos.x ) + ( count % 6 ) * 39 ;
				vPos.y = ( m_pMyIconGridDlg->m_absPos.y ) + ( count / 6 ) * 39 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}

		if( m_bOpLock )
		{
//			VECTOR2 vPos = { m_pOpIconGridDlg->m_absPos.x + 3, m_pOpIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos, 
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( count = 0 ; count < 12 ; ++count )
			{
				//if( !m_pOpIconGridDlg->IsAddable( i ) ) continue;
				
				vPos.x = ( m_pOpIconGridDlg->m_absPos.x ) + ( count % 6 ) * 39 ;
				vPos.y = ( m_pOpIconGridDlg->m_absPos.y ) + ( count / 6 ) * 39 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}
	}
}

BOOL CExchangeDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	if( m_bMyLock ) return FALSE;

	if( m_bOpLock ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1245 ) );
		return FALSE;
	}

	if( m_bDisable ) return FALSE;	//��ȯâ �������̸� ����

	if( icon->GetType() != WT_ITEM )
		return FALSE;


/*	// �������� �ŷ����� ��޼���
	if(((CItem*)icon)->GetItemKind() == eEXTRA_ITEM_USABLE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
		return FALSE;
	}
*/
	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(435) );
		
		return FALSE;
	}

/*	if( ((CItem*)icon)->GetItemKind() & eSHOP_ITEM )
	{
		// ������ �������� ���� �������� ����Ǯ���� �ŷ� �ȵǵ���
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pInfo )			return FALSE;
		if( pInfo->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemBaseInfo().ItemParam & ITEM_PARAM_SEAL) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(521) );
				return FALSE;
			}
		}
		// �ŷ� �ȵǴ� ������
		if( ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_EQUIP && pInfo->BuyPrice )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(521) );
			return FALSE;
		}
		else if( pInfo->MeleeAttackMax )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(521) );
			return FALSE;
		}
	}
*/
//	if( ((CItem*)icon)->GetItemKind() == eQUEST_ITEM_PET || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET)

/*
	if( ITEMMGR->IsPetSummonItem( ((CItem*)icon)->GetItemIdx() ) )
	{
		//�ش� �� ���� �� ģ�е� �˻�. 30%��������..
		const ITEMBASE* itembase = ((CItem*)icon)->GetItemBaseInfo();
		if(!PETMGR->CheckDefaultFriendship(itembase->dwDBIdx))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(526) );
			return FALSE;
		}
		//��ȯ ���� �� ����������
		if( PETMGR->IsCurPetSummonItem(itembase->dwDBIdx) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(526) );
			return FALSE;
		}
	}
*/
	// changeitem �߿� �ŷ��ȵǴ°�
	if(! ( ((CItem*)icon)->GetItemInfo()->Trade ) )
	{
		return FALSE;
	}
	
	if( ((CItem*)icon)->GetItemInfo()->Category == eItemCategory_Expend )
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(437) );
			return FALSE;
		}
	}
	else
	{
		if( ((CItem*)icon)->IsQuickLink() != 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(437) );
			return FALSE;
		}
	}

	if( m_pMyIconGridDlg->PtInWindow( x, y ) && m_bMyLock == FALSE )
	{
		// 071125 LYW --- ExchangeDialog : ���� ���� �� ������ ��ȯ �Ұ� ó�� �߰�.
		const ITEMBASE& pItemBase = ((CItem*)icon)->GetItemBaseInfo();

		if( pItemBase.nSealed == eITEM_TYPE_UNSEAL )
		{
			WINDOWMGR->MsgBox(MBI_SEALITEM_EXCHANGE, MBT_OK, CHATMGR->GetChatMsg(1174));

			return FALSE ;
		}

		if( EXCHANGEMGR->LinkItem( (CItem*)icon ) )
			SetDisable( TRUE );
	}
	return FALSE;
}

void CExchangeDialog::FakeDeleteItem( POSTYPE pos )
{
//---KES 071205 ��� ������ ���� �������� ��ȯâ���� ���� �� ������ ����
/*
	if( m_bDisable ) return; //��ȯâ �������̸� ����

	MSG_WORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_DELITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.wData		= pos;

	NETWORK->Send( &msg, sizeof( MSG_WORD ) );

	SetDisable( TRUE );
	//�߰� : ��ȯâ ����
*/
}


void CExchangeDialog::AddItem( int nType, POSTYPE pos, cIcon* pItem )
{
	if( nType == 0 )
	{
		m_pMyIconGridDlg->AddIcon( pos-1, pItem );
		++m_nMyItemNum;

		//---�߰�

	}
	else
	{
		m_pOpIconGridDlg->AddIcon( pos-1, pItem );
		++m_nOpItemNum;
	}

	//�߰� : ��ȯâ ���� Ǯ��
}


void CExchangeDialog::DelItemOptionInfoAll()
{
	//��ҵǾ�����	: itemOptioninfo�� �����Ѵ�.

	CExchangeItem*	pIcon = NULL;
	ITEM_INFO*		pInfo = NULL;
	for( int i = 1 ; i <= m_nOpItemNum ; ++i )
	{
		DelItemOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItemOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	ITEMMGR->RemoveOption( pIcon->GetDBId() );
}

//void CExchangeDialog::DelItemRareOptionInfoAll()
//{
//	for( int i = 1; i <= m_nOpItemNum; ++i )
//	{
//		DelItemRareOptionInfo( (POSTYPE)i );
//	}
//}

void CExchangeDialog::DelItem( int nType, POSTYPE pos )
{
	cIcon* pIcon = NULL;

	if( nType == 0 )
	{
		if( m_pMyIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;
			
			EXCHANGEMGR->UnLinkItem( 0, pIcon );

			for( int i = pos ; i < m_nMyItemNum ; ++i )
			{
				m_pMyIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				((CExchangeItem*)pIcon)->SetPosition( i );
				m_pMyIconGridDlg->AddIcon( i - 1, pIcon );
			}

			//linkitem�� �ϳ� ���̱�.////////////
			--m_nMyItemNum;
		}
	}
	else
	{
		if( m_pOpIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;

			EXCHANGEMGR->UnLinkItem( 1, pIcon );

			for( int i = pos ; i < m_nOpItemNum ; ++i )
			{
				m_pOpIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				m_pOpIconGridDlg->AddIcon( i - 1, pIcon );
			}
			--m_nOpItemNum;
		}
	}

	//�߰� : ��ȯâ ���� Ǯ��	
}

DWORD CExchangeDialog::InputMoney( int nType, DWORD dwMoney )
{
	if( nType == 0 )
	{
		m_dwMoney = dwMoney;
		m_pMyMoneyEditBox->SetEditText( AddComma( dwMoney ) );
	}
	else
	{
		m_dwOpMoney = dwMoney;
		m_pOpMoneyEditBox->SetEditText( AddComma( dwMoney ) ); //ȫ���� �ǽð��� �ƴ϶� ���ɶ��� �����ش�.
	}

	return dwMoney;
}

void CExchangeDialog::OpLock( BOOL bLock )
{
	m_bOpLock = bLock;

	int i;
	for( i = 0 ; i < m_nOpItemNum ; ++i )
	{
		if( m_pOpIconGridDlg->GetIconForIdx( i ) )
		{
			m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
			m_pOpIconGridDlg->GetIconForIdx( i )->SetMovable( FALSE );
		}
	}

	if( bLock == FALSE )	//��밡 ���� Ǯ��.. ���� ���µ� �ٲ۴�.
	{
		//�߰� : Syncing�� ���� ���� �����ϱ�
		m_pExchangeBtn->SetPush( FALSE );

		//�߰� : �۵� ����
#ifdef _HK_LOCAL_
		m_pOpMoneyEditBox->SetEditText( "0" );
#endif

		/////////MyLock
		m_bMyLock = FALSE;

		for( i = 0 ; i < m_nMyItemNum ; ++i )
		{
			if( m_pMyIconGridDlg->GetIconForIdx( i ) )
			{
				m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( FALSE );
				m_pMyIconGridDlg->GetIconForIdx( i )->SetMovable( FALSE );
			}
		}
		
		m_pInputMoneyBtn->SetDisable( FALSE );
		m_pLockBtn->SetPush( FALSE );
	/////////////
	}
#ifdef _HK_LOCAL_
	else	//lock
	{
		m_pOpMoneyEditBox->SetEditText( AddComma( m_dwOpMoney ) );
	}
#endif
}

void CExchangeDialog::MyLock( BOOL bLock )
{
	m_bMyLock = bLock;

	int i;
	for( i = 0 ; i < m_nMyItemNum ; ++i )
	{
		if( m_pMyIconGridDlg->GetIconForIdx( i ) )
		{
			m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
			m_pMyIconGridDlg->GetIconForIdx( i )->SetMovable( FALSE );
		}
	}
	
	if( bLock )
	{
		m_pInputMoneyBtn->SetDisable( TRUE );

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
	}
	else
	{
		m_bOpLock = bLock;
		for( i = 0 ; i < m_nOpItemNum ; ++i )		//�Լ��� ��ACK�϶� �����Ѵ�?
		{
			if( m_pOpIconGridDlg->GetIconForIdx( i ) )
			{
				m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
				m_pOpIconGridDlg->GetIconForIdx( i )->SetMovable( FALSE );
			}
		}

		m_pInputMoneyBtn->SetDisable( FALSE );
	}

	m_pLockBtn->SetPush( bLock );
}


void CExchangeDialog::OnInputMoney( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetMoney() )
	{
		pDlg->SetDisable( FALSE );
		return;
	}
	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() || pDlg->IsOpLock() )
	{	//���ɸ� ���¶��
	
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1245 ) );
	
		pDlg->SetDisable( FALSE );
		return;
	}

	if( pDlg->GetMoney() > 0 )
	{
		pDlg->SetDisable( FALSE );
		return;
	}

	DWORD dwCurMoney = HERO->GetMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTMONEY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );

	GAMEIN->GetExchangeDialog()->MoneyInputButtonEnable( FALSE );
}


void CExchangeDialog::OnInputCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}

void CExchangeDialog::MoneyInputButtonEnable( BOOL bEnable )
{
	if( bEnable == FALSE )
		m_pInputMoneyBtn->SetDisable( FALSE );	//disable ���¿��� setactive������ �ȵȴ�.
	m_pInputMoneyBtn->SetActive( bEnable );
}
