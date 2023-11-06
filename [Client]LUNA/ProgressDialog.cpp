#include "StdAfx.h"
#include "ProgressDialog.h"
#include "cStatic.h"
#include "ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "ObjectManager.h"

#include "ReinforceDlg.h"
#include "DissolveDialog.h"
#include "EnchantDialog.h"
#include "MixDialog.h"
#include "InventoryExDialog.h"
#include "ApplyOptionDialog.h"
#include "PetResurrectionDialog.h"

const DWORD unableColor = RGB( 190, 190, 190 );
const DWORD enableColor = RGB( 255, 255, 255 );


CProgressDialog::CProgressDialog()
{
	ZeroMemory( &mAction, sizeof( mAction ) );
}


CProgressDialog::~CProgressDialog()
{}


void CProgressDialog::Linking()
{
	mText	= ( cStatic* )		GetWindowForID( PROGRESS_DIALOG_TEXT );
	mGauge	= ( CObjectGuagen* )GetWindowForID( PROGRESS_DIALOG_GAUGE );
	//mButton	=					GetWindowForID( PROGRESS_DIALOG_CLOSE );

	ASSERT( mText && mGauge /*&& mButton*/ );

	mAction.mIsWaiting	= FALSE;

	SetAlwaysTop( TRUE );
}


void CProgressDialog::SetActive( BOOL isActive )
{
	{
		cDialog* dialog = WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
		ASSERT( dialog );

		dialog->SetDisable( isActive );
	}
	
	if( ! isActive )
	{
		Restore();
	}
		
	//if( isActive )
	//{
	//	const DISPLAY_INFO& screen	= GAMERESRCMNGR->m_GameDesc.dispInfo;
	//	const DWORD			x		= ( screen.dwWidth - m_width ) / 2;
	//	const DWORD			y		= ( screen.dwHeight - m_height ) / 2;

	//	SetAbsXY( x, y );
	//}

	cDialog::SetActive( isActive );
}


void CProgressDialog::Wait( eAction action )
{
	if( mAction.mIsWaiting )
	{
		CHATMGR->AddMsg( CTC_OPERATEITEM, CHATMGR->GetChatMsg( 1082 ) );

		return;
	}

	// ������ ����
	{
		// 071211 LUJ ��ȹ���� ��û���� 5��->2�ʷ� ����
		//const DWORD waitingMiliSecond = 2000;

		mAction.mBeginTick	= GetTickCount();
		//mAction.mEndTick	= mAction.mBeginTick + waitingMiliSecond;
		mAction.mEndTick	= mAction.mBeginTick + WAITMILISECOND;
		mAction.mType		= action;
		mAction.mIsWaiting	= TRUE;

		mGauge->SetValue( 0, 0 );
		//mGauge->SetValue( GUAGEVAL( 1.0f ), waitingMiliSecond );
		mGauge->SetValue( GUAGEVAL( 1.0f ), WAITMILISECOND );
	}

	// ��ư ��Ȱ��ȭ
	//{
	//	mButton->SetImageRGB( unableColor );
	//}

	// �ؽ�Ʈ ����
	{
		switch( action )
		{
		case eActionMix:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1073 ) );
				break;
			}
		case eActionReinforce:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1074 ) );
				break;
			}
		case eActionDissolve:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1075 ) );
				break;
			}
		case eActionEnchant:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1076 ) );
				break;
			}
		case eActionChange:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1425 ) );
				break;
			}
		case eActionApplyOption:
			{
				mText->SetStaticText( CHATMGR->GetChatMsg( 1424 ) );
				break;
			}
		default:
			{
				ASSERT( 0 );
				break;
			}
		}
	}

	SetActive( TRUE );
	SetDisable( TRUE );

	// ������ �����Ͽ� �ش� �÷��̾ ������ �۾� ������ �ϰ� ������ �˸���.
	// ����: �������� ���¸� �����ؾ��� �� �ִ�. �׷��� ������ ���� �׸���� ���� �÷��̾�Դ� ���������� ������ ���� ���̴�.
	//		 ĳ���� �ɱ� ����(rest)�� ���������� ���̵��� ������ߴ� ���� ����ϵ���
	{
		CHero* hero = OBJECTMGR->GetHero();
		ASSERT( hero );

		MSG_DWORD message;
		message.Category	= MP_ITEM;
		message.Protocol	= MP_ITEM_WORKING_START;
		message.dwObjectID	= hero->GetID();
		message.dwData		= hero->GetID();

		NETWORK->Send( &message, sizeof( message ) );
	}
}


void CProgressDialog::Render()
{
	if( mAction.mIsWaiting )
	{
		const DWORD currentTick = GetTickCount();

		// ƽ ���� �����÷ε� ���¿� �ƴ� ���� ��� üũ�ؾ� �Ѵ�.
		if( ( mAction.mBeginTick < mAction.mEndTick && mAction.mEndTick < currentTick ) ||
			( mAction.mBeginTick > mAction.mEndTick && mAction.mEndTick < currentTick && mAction.mBeginTick > currentTick ) )
		{
			SetActive( FALSE );

			switch( mAction.mType )
			{
			case eActionDissolve:
				{
					CDissolveDialog* dialog = ( CDissolveDialog* )WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionReinforce:
				{
					CReinforceDlg* dialog = ( CReinforceDlg* )WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionMix:
				{
					CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionEnchant:
				{
					CEnchantDialog* dialog = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionChange:
			case eActionPetSummon:
			case eActionPetGradeUp:
				{
					CInventoryExDialog* dialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionApplyOption:
				{
					CApplyOptionDialog* dialog = ( CApplyOptionDialog* )WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );
					ASSERT( dialog );

					dialog->Send();
					break;
				}
			case eActionPetResurrection:
				{
					CPetResurrectionDialog* dialog = ( CPetResurrectionDialog* )WINDOWMGR->GetWindowForID( PET_RES_DLG );
					ASSERT( dialog );

					dialog->Send();
				}
				break;

			default:
				{
					ASSERT( 0 );
					break;
				}
			}

			mAction.mIsWaiting	= FALSE;
		}
	}

	cDialog::Render();
}


void CProgressDialog::SetText( const char* text )
{
	mText->SetStaticText( text );
}


void CProgressDialog::Cancel()
{
	Restore();
	SetActive( FALSE );

	switch( mAction.mType )
	{
	case eActionDissolve:
		{
			CDissolveDialog* dialog = ( CDissolveDialog* )WINDOWMGR->GetWindowForID( DIS_DISSOLUTIONDLG );
			ASSERT( dialog );

			dialog->Restore();
			dialog->SetActive( FALSE );	

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1084 ) );
			break;
		}
	case eActionReinforce:
		{
			CReinforceDlg* dialog = ( CReinforceDlg* )WINDOWMGR->GetWindowForID( ITR_REINFORCEDLG );
			ASSERT( dialog );

			dialog->Restore();
			dialog->SetActive( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1084 ) );
			break;
		}
	case eActionMix:
		{
			CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
			ASSERT( dialog );

			dialog->Restore();
			dialog->SetActive( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG,CHATMGR->GetChatMsg( 785 ) );
			break;
		}
	case eActionEnchant:
		{
			CEnchantDialog* dialog = ( CEnchantDialog* )WINDOWMGR->GetWindowForID( ITD_UPGRADEDLG );
			ASSERT( dialog );

			dialog->Restore();
			dialog->SetActive( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1084 ) );
			break;
		}
	case eActionChange:
		{
			CInventoryExDialog* dialog = ( CInventoryExDialog* )WINDOWMGR->GetWindowForID( IN_INVENTORYDLG );
			ASSERT( dialog );

			dialog->Restore();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1084 ) );
			break;
		}
	case eActionApplyOption:
		{
			CApplyOptionDialog* dialog = ( CApplyOptionDialog* )WINDOWMGR->GetWindowForID( APPLY_OPTION_DIALOG );
			ASSERT( dialog );

			dialog->Restore();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1084 ) );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}	
}


void CProgressDialog::Restore()
{
	SetDisable( FALSE );

	mAction.mIsWaiting	= FALSE;
}


void CProgressDialog::OnActionEvent( LONG id, void* p, DWORD event )
{
}
