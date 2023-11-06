#include "stdafx.h"
#include "GuildLevelUpDialog.h"
#include "interface/cStatic.h"
#include "WindowIDEnum.h"
#include "GuildManager.h"
#include "ObjectManager.h"
//#include "GuildDialog.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "ObjectStateManager.h"
#include "cWindowManager.h"
#include "ChatManager.h"


CGuildLevelUpDialog::CGuildLevelUpDialog()
{
	m_type = WT_GUILDLEVELUPDLG;
}


CGuildLevelUpDialog::~CGuildLevelUpDialog()
{}


void CGuildLevelUpDialog::Linking()
{
	// ��ư ���ε�
	{
		mSubmitButton = GetWindowForID( GD_LUOKBTN );
		ASSERT( mSubmitButton );
	}

	// ��� ���� �����Ǿ��� ��� �̹���
	{
		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU1COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU2COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU3COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU4COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU5COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU6COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU7COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU8COMPLETE );
			ASSERT( image );

			mCompleteStaticList.push_back( image );
		}
	}

	// ��� ������ �������� ������ ��� �̹���
	{
		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU1NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU2NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU3NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU4NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU5NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU6NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU7NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU8NOTCOMPLETE );
			ASSERT( image );

			mIncompleteStaticList.push_back( image );
		}
	}

	// ���� �ؽ�Ʈ
	{
		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU1 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU2 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU3 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU4 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU5 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU6 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU7 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}

		{
			cStatic* image = ( cStatic* )GetWindowForID( GD_LU8 );
			ASSERT( image );

			mLevelStaticList.push_back( image );
		}
	}

	ASSERT( mCompleteStaticList.size() == mIncompleteStaticList.size() && mCompleteStaticList.size() == mLevelStaticList.size() );
}


void CGuildLevelUpDialog::SetLevel( DWORD level )
{
	ASSERT( 0 < level );

	// C++�� �ε����� 0�� �����̳�, ��� ������ 1���� �����̹Ƿ� �ϳ� ����
	--level;

	// ���� ������ �̹����� �Ѱ� ����
	if( mCompleteStaticList.size() > level )
	{
		StaticList::const_iterator end_it = mIncompleteStaticList.begin();

		std::advance( end_it, level );

		for(	StaticList::const_iterator it = mIncompleteStaticList.begin();
				end_it != it;
				++it )
		{
			cStatic* image = *it;

			image->SetActive( FALSE );
		}

		for(	;
				mIncompleteStaticList.end() != end_it;
				++end_it )
		{
			cStatic* image = *it;

			image->SetActive( TRUE );
		}
	}

	// ���� ���� �̹����� �Ѱ� ����
	if( mCompleteStaticList.size() > level )
	{
		StaticList::const_iterator end_it = mCompleteStaticList.begin();

		std::advance( end_it, level );

		for(	StaticList::const_iterator it = mCompleteStaticList.begin();
				end_it != it;
				++it )
		{
			cStatic* image = *it;

			image->SetActive( TRUE );
		}

		for(	;
				mCompleteStaticList.end() != end_it;
				++end_it )
		{
			cStatic* image = *it;

			image->SetActive( FALSE );
		}
	}

	// ���� ǥ�õ� �ٲ��ش�
	if( mLevelStaticList.size() > level )
	{
		for(	StaticList::const_iterator it = mLevelStaticList.begin();
				mLevelStaticList.end() != it;
				++it )
		{
			cStatic* image = *it;

			image->SetFGColor( RGB_HALF( 255, 255, 255 ) );
		}

		StaticList::const_iterator selected = mLevelStaticList.begin();

		std::advance( selected, level );

		cStatic* image = *selected;

		image->SetFGColor( RGB_HALF( 255, 255, 0 ) );
	}

	if( mLevelStaticList.size() - 1 == level )
	{
		mSubmitButton->SetActive( FALSE );
	}

	/*
	DWORD i = 0;

	for( i=0; i<lvl; ++i)
	{
		m_pLevelupNotComplete[i]->SetActive(FALSE);
		m_pLevelupComplete[i]->SetActive(TRUE);
	}
	for( i=lvl; i< 4; ++i)
	{
		m_pLevelupNotComplete[i]->SetActive(TRUE);
		m_pLevelupComplete[i]->SetActive(FALSE);
	}
	for( i=0; i<5; ++i )
	{
		m_pLevel[i]->SetFGColor(RGB_HALF(255, 255, 255));
	}

	m_pLevel[level-1]->SetFGColor(RGB_HALF(255,255,0));
	*/
}

void CGuildLevelUpDialog::SetActive( BOOL IsActive )
{
	if( IsActive )
	{
		// �渶�� �ƴϸ� �� â�� �� �� ����... ���� �渶 ã��: ��� â�� ������ �̸��� ������ ��
		{
			CHero* hero = OBJECTMGR->GetHero();

			if( strcmp( hero->GetObjectName(), GUILDMGR->GetMasterName() ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 579 ) );
				return;
			}
		}

		// �ִ� ������ �����߾ ��������. ��� ã����? ���� �������� ���� ������ ������ �����ߴٴ� ����
		{
			if( ! GUILDMGR->GetLevelSetting( GUILDMGR->GetLevel() + 1 ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 604 ) );
				return;
			}
		}

		// ���� ����
		{
			SetLevel( GUILDMGR->GetLevel() );
		}		
	}
	else
	{
		// NPC ��ũ��Ʈ â(NpcChat.bin)
		cDialog* dialog = WINDOWMGR->GetWindowForID( NSI_SCRIPTDLG );
		ASSERT( dialog );

		CHero* hero = OBJECTMGR->GetHero();

		if(		hero									&&
				eObjectState_Deal == hero->GetState()	&&
			!	dialog->IsActive()  )
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
	}

	cDialog::SetActive( IsActive );
}


void CGuildLevelUpDialog::OnActionEvent( LONG id, void* p, DWORD event )
{
	switch( id )
	{
	case GD_LUOKBTN:
		{
			// ����: �ִ� ���� �����̳� ������ ���δ� �̹� â ���� �����ߴ�.

			const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( GUILDMGR->GetLevel() + 1 );
			ASSERT( setting );			

			CHero* hero = OBJECTMGR->GetHero();

			// �� ����ϳ�
			if( setting->mRequiredMoney > hero->GetMoney() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 605 ) );
				break;
			}
			else if( setting->mRequiredMasterLevel > hero->GetLevel() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1092 ), setting->mRequiredMasterLevel );
				break;
			}

			// ��� ����Ʈ�� ���
			if( setting->mRequiredScore > GUILDMGR->GetScore() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 605 ) );
				return;
			}

			// ������ ��û(
			{
				MSGBASE message;
				message.Category	= MP_GUILD;
				message.Protocol	= MP_GUILD_LEVELUP_SYN;
				message.dwObjectID	= hero->GetID();

				NETWORK->Send( &message, sizeof( message ) );	
			}

			SetDisable( TRUE );
			break;
		}
	}
}
