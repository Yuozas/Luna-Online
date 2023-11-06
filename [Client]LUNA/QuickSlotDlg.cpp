#include "stdafx.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include ".\quickslotdlg.h"


#include "ObjectManager.h"
#include "cSkillBase.h"
// 070216 LYW --- QuickSlotDlg : Include header file of cConductBase.
#include "ConductBase.h"
#include "ConductInfo.h"

#include "cQuickItem.h"
#include "QuickManager.h"
#include "GameIn.h"

#include "ItemManager.h"

#include "InventoryExDialog.h"
#include "./Input/Mouse.h"

#include "ChatManager.h"
#include "MacroManager.h"
#include "../[cc]skill/client/manager/skillmanager.h"

// 080707 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
#include "../Interface/cWindowManager.h"

cQuickSlotDlg::cQuickSlotDlg(void)
{
	mCurSheet = 0;
	mStartSheetNum = 0;
}

cQuickSlotDlg::~cQuickSlotDlg(void)
{
}

void cQuickSlotDlg::Add(cWindow * window)
{
	if(window->GetType() == WT_STATIC )
	{
		// 080702 LYW --- QuickSlotDlg : Ȯ�� ���Կ� ���� �۾�.
		//if( window->GetID() == QI_TABNUMBER )
		if( window->GetID() == QI_TABNUMBER || window->GetID() == QI2_TABNUMBER )
		{
			mpStatic = ( cStatic* )window;
		}
	}
	else if(window->GetType() == WT_ICONGRIDDIALOG )
	{
		// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� �ִ� ������ ���� ����.
		//if( mCurSheet > 7 )
		if( mCurSheet > MAX_SLOTPAGE-1 )
		{
			return;
		}

		mpSheet[ mCurSheet++ ] = ( cIconGridDialog* )window;

	}
	
	cDialog::Add(window);
}

void cQuickSlotDlg::Linking()
{
	cQuickItem* pItem = NULL;

	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� ���� �ִ� �������� �����Ѵ�.
	//for( WORD sheet = 0; sheet < 8; sheet++ )
	//{
	//	for( WORD pos = 0; pos < 10; pos++ )
	//	{
	//		pItem = QUICKMGR->NewQuickItem( ( sheet + mStartSheetNum ), pos );
	//		mpSheet[ sheet ]->AddIcon( pos, pItem );
	//	}
	//}

	for( WORD sheet = 0; sheet < MAX_SLOTPAGE; sheet++ )
	{
		for( WORD pos = 0; pos < MAX_SLOTNUM; pos++ )
		{
			if(GetID() == QI1_QUICKSLOTDLG)
			{
				pItem = QUICKMGR->NewQuickItem( ( sheet + mStartSheetNum ), pos );
			}
			else
			{
				pItem = QUICKMGR->NewQuickItem( ( (sheet+MAX_SLOTPAGE) + mStartSheetNum ), pos );
			}

			mpSheet[ sheet ]->AddIcon( pos, pItem );
		}
	}
}

void cQuickSlotDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	//if( we & WE_BTNCLICK )
	//{
	//	if( lId == QI_PREVPAGEBTN )
	//	{
	//		SelectPage( mCurSheet - 1 );
	//	}
	//	else if( lId == QI_NEXTPAGEBTN )
	//	{
	//		SelectPage( mCurSheet + 1 );
	//	}
	//}
	//else if( we & WE_LBTNDBLCLICK )
	//{
	//	QUICKMGR->UseQuickItem( mCurSheet + mStartSheetNum, ( WORD )( mpSheet[ mCurSheet ]->GetCurSelCellPos() ) );
	//}

	if( we & WE_BTNCLICK )
	{
		if( lId == QI_PREVPAGEBTN || lId == QI2_PREVPAGEBTN)
		{
			SelectPage( mCurSheet - 1 );
		}
		else if( lId == QI_NEXTPAGEBTN || lId == QI2_NEXTPAGEBTN )
		{
			SelectPage( mCurSheet + 1 );
		}
	}
	else if( we & WE_LBTNDBLCLICK )
	{
		// 080704 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
		//QUICKMGR->UseQuickItem( mCurSheet + mStartSheetNum, ( WORD )( mpSheet[ mCurSheet ]->GetCurSelCellPos() ) );
		if(this->GetID() == QI1_QUICKSLOTDLG)
		{
			QUICKMGR->UseQuickItem( mCurSheet + mStartSheetNum, ( WORD )( mpSheet[ mCurSheet ]->GetCurSelCellPos() ) );
		}
		else
		{
			QUICKMGR->UseQuickItem( (mCurSheet+MAX_SLOTPAGE) + mStartSheetNum, ( WORD )( mpSheet[ mCurSheet ]->GetCurSelCellPos() ) );
		}
	}
}

void cQuickSlotDlg::SelectPage( WORD page )
{
	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� �ִ� ���� �������� �����Ѵ�.
	//if( page > 8 )
	//{
	//	page = 7;
	//}
	//if( page > 7 )
	//{
	//	page = 0;
	//}

	if( page > MAX_SLOTPAGE )
	{
		page = MAX_SLOTPAGE-1;
	}
	if( page > MAX_SLOTPAGE-1 )
	{
		page = 0;
	}

	mCurSheet = page; 

	char buf[ 16 ];

	sprintf( buf, "%d", mCurSheet );

	mpStatic->SetStaticText( buf );

	SetActive( IsActive() );
}

void cQuickSlotDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val )
	{
		// 080702 LYW --- QuickSkotDlg : Ȯ�� ���� �߰��� ���� �ִ� ������ ���� �����Ѵ�.
		//for( WORD i = 0; i < 8; i++ )
		for( WORD i = 0; i < MAX_SLOTPAGE; i++ )
		{
			if( i == mCurSheet )
			{
				mpSheet[ i ]->SetActive( TRUE );
				continue;
			}

			mpSheet[ i ]->SetActive( FALSE );
		}
	}
}

void cQuickSlotDlg::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );
}

// 080703 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� �Լ� ����.
//BOOL cQuickSlotDlg::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
BOOL cQuickSlotDlg::FakeMoveIcon(cDialog* pFromDlg, LONG mouseX, LONG mouseY, cIcon * icon)
{
	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� �ִ� �������� ����.
	//if( mCurSheet > 8 )
	if( mCurSheet > MAX_SLOTPAGE )
	{
		return FALSE;
	}

	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
	{
		return FALSE;
	}

	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM )
	{
		return FALSE;
	}

	if(!QUICKMGR->CanEquip(icon))
	{
		return FALSE;
	}

	switch( icon->GetType() )
	{
	case WT_QUICKITEM:
		{
			// 080703 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� ó��.
			//FakeMoveQuickItem(mouseX, mouseY, (cQuickItem *)icon);
			FakeMoveQuickItem(pFromDlg, mouseX, mouseY, (cQuickItem *)icon);
		}
		break;
	case WT_ITEM:
		{
			FakeAddItem(mouseX, mouseY, (CItem *)icon);
		}
		break;
	case WT_SKILL:
		{
			FakeAddSkill(mouseX, mouseY, (cSkillBase *)icon);
		}
		break;
	// 070216 LYW --- QuickSlotDlg : Add case part for conduct.
	case WT_CONDUCT :
		{
			FakeAddConduct(mouseX, mouseY, (cConductBase*)icon) ;
		}
		break ;
	}
	return FALSE;
}

// 080703 LYW --- QuickSlotDlg : Ȯ�彽�� �߰��� ���� ó��.
//void cQuickSlotDlg::FakeMoveQuickItem(LONG x, LONG y, cQuickItem * icon)
//{
//	WORD tab = 0;
//	WORD pos1 = 0;
//	WORD pos2 = 0;
//	
//	if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( x, y, pos2 ) ) )
//	{
//		return;
//	}
//
//	icon->GetPosition( tab, pos1 );
//
//	MSG_QUICK_CHANGE_SYN msg;
//	msg.Category = MP_QUICK;
//	msg.Protocol = MP_QUICK_CHANGE_SYN;
//	msg.dwObjectID = HEROID;
//	msg.tabNum1 = ( BYTE )tab;
//	msg.pos1 = ( BYTE )pos1;
//	msg.tabNum2 = ( BYTE )( mCurSheet + mStartSheetNum );
//	msg.pos2 = ( BYTE )pos2;
//
//	NETWORK->Send( &msg, sizeof( msg ) );
//}

void cQuickSlotDlg::FakeMoveQuickItem(cDialog* pFromDlg, LONG x, LONG y, cQuickItem * icon)
{
	// ���� ������ üũ.
	if(!pFromDlg) return ;

	// ������ ������ üũ.
	if(!icon) return ;


	// �̵� ó���� �ʿ��� �ӽ� �������� �����Ѵ�.
	WORD wThisTab	= 0 ;
	WORD wBeforeTab = 0 ;
	WORD wThisPos	= 0 ;
	WORD wBeforePos = 0 ;


	// ���� �� / ��ġ�� �޴´�.
	icon->GetPosition( wBeforeTab, wBeforePos );

	// ���� ���� �޴´�.
	wThisTab = mCurSheet + mStartSheetNum ;
	
	// ���� ��ġ�� �޴´�.
	if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( x, y, wThisPos ) ) )
	{
		return;
	}


	// ���� ���԰�, ���� ������ ���踦 üũ�Ѵ�.
	LONG lThisSlotID = this->GetID() ;
	LONG lFromSlotID = pFromDlg->GetID() ;

	//// ���� â�� ���ν����� ���.
	//if(lThisSlotID == QI1_QUICKSLOTDLG)
	//{
	//	// Ȯ�� �������� ���� ���� ���������� �̵��̶��,
	//	if(lThisSlotID != lFromSlotID)
	//	{
	//		if(lFromSlotID == QI2_QUICKSLOTDLG)
	//		{
	//			//wBeforeTab -= MAX_SLOTPAGE ;
	//		}
	//	}
	//}
	// ���� â�� Ȯ�� ������ ���.
	//else if(lThisSlotID == QI2_QUICKSLOTDLG)
	if(lThisSlotID == QI2_QUICKSLOTDLG)
	{
		// Ȯ�� ���� �������� �̵��̶��,
		if(lThisSlotID == lFromSlotID)
		{
			wThisTab	+= MAX_SLOTPAGE ;
			//wBeforeTab	+= MAX_SLOTPAGE ;
		}
		// ���� �������� ���� Ȯ�� ���������� �̵��̶��,
		else
		{
			wThisTab	+= MAX_SLOTPAGE ;
		}
	}


	// �̵� ������ ������ ������.
	MSG_QUICK_CHANGE_SYN msg;
	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_CHANGE_SYN;
	msg.dwObjectID = HEROID;
	msg.tabNum1 = ( BYTE )wBeforeTab;
	msg.pos1 = ( BYTE )wBeforePos;
	msg.tabNum2 = ( BYTE )wThisTab;
	msg.pos2 = ( BYTE )wThisPos;

	NETWORK->Send( &msg, sizeof( msg ) );
}

void cQuickSlotDlg::FakeAddItem(LONG x, LONG y, CItem * icon)
{
	WORD pos = 0;
	if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( x, y, pos ) ) )
	{
		return;
	}

	MSG_QUICK_ADD_SYN msg;

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_ADD_SYN;
	msg.dwObjectID = HEROID;

	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	//msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	if(this->GetID() == QI1_QUICKSLOTDLG)
	{
		msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	}
	else
	{
		msg.tabNum = ( BYTE )( (mCurSheet+MAX_SLOTPAGE) + mStartSheetNum );
	}

	msg.pos = ( BYTE )pos;
	msg.slot.dbIdx = icon->GetDBIdx();
	msg.slot.idx = icon->GetItemIdx();

	if( icon->GetItemInfo()->EquipType == eEquipType_Pet )
	{
		if( icon->GetItemInfo()->SupplyType == ITEM_KIND_PET_SKILL )
		{
			msg.slot.data = 0;
			msg.slot.kind = QuickKind_PetSkill;
		}
		else
		{
			msg.slot.data = 0;
			msg.slot.kind = QuickKind_PetItem;
		}
	}
	else if( ITEMMGR->IsDupItem( icon->GetItemIdx() ) )
	{
		msg.slot.data = 0;
		msg.slot.kind = QuickKind_MultiItem;
	}
	else
	{
		msg.slot.data = 0;
		msg.slot.kind = QuickKind_SingleItem;
	}

	NETWORK->Send( &msg, sizeof( msg ) );
}

void cQuickSlotDlg::FakeAddSkill(LONG x, LONG y, cSkillBase * icon)
{
	WORD pos = 0;
	if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( x, y, pos ) ) )
	{
		return;
	}

	MSG_QUICK_ADD_SYN msg;

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_ADD_SYN;
	msg.dwObjectID = HEROID;
	
	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	//msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	if(this->GetID() == QI1_QUICKSLOTDLG)
	{
		msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	}
	else
	{
		msg.tabNum = ( BYTE )( (mCurSheet+MAX_SLOTPAGE) + mStartSheetNum );
	}

	msg.pos = ( BYTE )pos;
	msg.slot.kind = QuickKind_Skill;
	msg.slot.dbIdx = icon->GetDBIdx();
	msg.slot.idx = icon->GetSkillIdx();
	msg.slot.data = icon->GetLevel();

	NETWORK->Send( &msg, sizeof( msg ) );
}

// 070216 LYW --- QuickSlotDlg : Add function to add conduct to quick slot.
void cQuickSlotDlg::FakeAddConduct( LONG x, LONG y, cConductBase* icon )
{
	WORD pos = 0 ;

	if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( x, y, pos ) ) )
	{
		return ;
	}

	MSG_QUICK_ADD_SYN msg ;

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_ADD_SYN;
	msg.dwObjectID = HEROID;
	
	// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	//msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	if(this->GetID() == QI1_QUICKSLOTDLG)
	{
		msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	}
	else
	{
		msg.tabNum = ( BYTE )( (mCurSheet+MAX_SLOTPAGE) + mStartSheetNum );
	}

	msg.pos = ( BYTE )pos;
	msg.slot.kind = QuickKind_Conduct;
	msg.slot.dbIdx = icon->m_pConductInfo->GetConductIdx() ;
	msg.slot.idx = icon->m_pConductInfo->GetConductIdx() ;
	msg.slot.data = 0;

	NETWORK->Send( &msg, sizeof( msg ) );

}

void cQuickSlotDlg::FakeDeleteQuickItem()
{
	MSG_QUICK_REMOVE_SYN msg;

	msg.Category = MP_QUICK;
	msg.Protocol = MP_QUICK_REMOVE_SYN;
	msg.dwObjectID = HEROID;
	// 080706 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	//msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	if(this->GetID() == QI1_QUICKSLOTDLG)
	{
		msg.tabNum = ( BYTE )( mCurSheet + mStartSheetNum );
	}
	else
	{
		msg.tabNum = ( BYTE )( (mCurSheet+MAX_SLOTPAGE) + mStartSheetNum );
	}
	msg.pos = ( BYTE )( mpSheet[ mCurSheet ]->GetCurSelCellPos() );

	NETWORK->Send( &msg, sizeof( msg ) );
}

//void cQuickSlotDlg::DeleteQuickItem(int nTabNum, int nPos)
//{
//	MSG_QUICK_REMOVE_SYN msg;
//
//	msg.Category = MP_QUICK;
//	msg.Protocol = MP_QUICK_REMOVE_SYN;
//	msg.dwObjectID = HEROID;
//	msg.tabNum = (BYTE)nTabNum ;
//	msg.pos = (BYTE)nPos ;
//
//	NETWORK->Send( &msg, sizeof( msg ) );
//}


void cQuickSlotDlg::SetCoolTime( DWORD itemIndex, DWORD miliSecond )
{
	const DWORD groupIndex = ITEMMGR->GetCoolTimeGroupIndex( itemIndex );

	for( WORD page = 0; page < MAX_SLOTPAGE; ++page )
	{
		cIconGridDialog* tab = mpSheet[ page ];
		
		for( WORD iconIndex = 0; tab->GetCellNum() > iconIndex; ++iconIndex )
		{
			cQuickItem* icon = ( cQuickItem* )tab->GetIconForIdx( iconIndex );

			if( ! icon )
			{
				continue;
			}

			const SLOT_INFO* slot = icon->GetSlotInfo();

			if( !	slot									||
				(	QuickKind_MultiItem		!= slot->kind	&&
					QuickKind_SingleItem	!= slot->kind ) )
			{
				continue;
			}

			if( slot->idx == itemIndex	||
				(	groupIndex && 
					groupIndex == ITEMMGR->GetCoolTimeGroupIndex( slot->idx ) ) )
			{
				icon->SetCoolTime( miliSecond );
			}
		}
	}
}


void cQuickSlotDlg::SetCoolTimeAbs( DWORD itemIndex, DWORD dwBegin, DWORD dwEnd, DWORD miliSecond )
{
	const DWORD groupIndex = ITEMMGR->GetCoolTimeGroupIndex( itemIndex );

	for( WORD page = 0; page < MAX_SLOTPAGE; ++page )
	{
		cIconGridDialog* tab = mpSheet[ page ];
		
		for( WORD iconIndex = 0; tab->GetCellNum() > iconIndex; ++iconIndex )
		{
			cQuickItem* icon = ( cQuickItem* )tab->GetIconForIdx( iconIndex );

			if( ! icon )
			{
				continue;
			}

			const SLOT_INFO* slot = icon->GetSlotInfo();

			if( !	slot									||
				(	QuickKind_MultiItem		!= slot->kind	&&
					QuickKind_SingleItem	!= slot->kind ) )
			{
				continue;
			}

			if( slot->idx == itemIndex	||
				(	groupIndex && 
					groupIndex == ITEMMGR->GetCoolTimeGroupIndex( slot->idx ) ) )
			{
				icon->SetCoolTimeAbs( dwBegin, dwEnd, miliSecond );
			}
		}
	}
}



// 071124 LYW --- QuickSlotDlg : �̺�Ʈ �Լ� �߰�.
DWORD cQuickSlotDlg::ActionEvent(CMouse* mouseInfo)
{
	// 080707 LYW --- QuickSlotDlg : Ȯ�� ���� �߰� ó��.
	// �� ������ �� ���� ( �⺻/Ȯ�� )�� ��ŷ ��������, 
	// �� ���� ���̾�α��� ������Ʈ���� ���� �Ǿ���.
	// ������ ���� �߰��Ǵ� Ȯ�� ������, �� ���̾�α�(cQuickDlg)�� 
	// ���� ��Ʈ���� �ƴϹǷ�, �巹�� ��, ���� ��ġ ������ �ʿ��ϴ�.
	cDialog* pExSlot = NULL ;
	pExSlot = WINDOWMGR->GetWindowForID(QI2_QUICKSLOTDLG) ;

	if(pExSlot)
	{
		LONG lDlgIdx = 0 ;
		lDlgIdx = this->GetID() ;

		if(lDlgIdx == QI1_QUICKSLOTDLG)
		{
			float fXPos = this->GetAbsX() + 20 ;
			float fYPos = this->GetAbsY() - 50 ;

			pExSlot->SetAbsXY(fXPos, fYPos) ;
		}
	}

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);

	if( we == WE_MOUSEOVER )
	{
		WORD pos = 0;
		if( !( mpSheet[ mCurSheet ]->GetPositionForXYRef( mouseInfo->GetMouseX(), mouseInfo->GetMouseY(), pos ) ) )
		{
			return we ;
		}

		cQuickItem* pQuickItem = NULL ;
		pQuickItem = QUICKMGR->GetQuickItem(mCurSheet, pos) ;

		if( !pQuickItem ) return we ;

		QUICKMGR->AddToolTip(pQuickItem) ;
	}

	return we ;
}
