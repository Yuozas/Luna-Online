#include "stdafx.h"
#include ".\cskilltrainingdlg.h"
#include ".\interface\clistdialog.h"
#include ".\interface\cstatic.h"
#include "WindowIDEnum.h"
#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/info/activeskillinfo.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "GameIn.h"
#include "cskilltreemanager.h"
#include "cSkillBase.h"
#include "itemmanager.h"
#include "./Interface/cScriptManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "../Interface/cWindowManager.h"

// 080310 LUJ, ��ų �䱸 ���� ǥ���ϱ� ���� ����
#include "cResourceManager.h"
// 080417 LUJ, ��� ��ų ǥ������ ����
#include "GuildManager.h"


cSkillTrainingDlg::cSkillTrainingDlg(void) :
mGuildMode( FALSE )
{
	m_Index = 0;
}

cSkillTrainingDlg::~cSkillTrainingDlg(void)
{
}

void cSkillTrainingDlg::Linking()
{
	m_SkillListDlg = ( cListDialog* )GetWindowForID( SKILL_TRAINLIST );
	m_SkillListDlg->SetShowSelect( TRUE );
	m_SkillListDlg->SetAutoScroll( FALSE );

	m_SkillTipListDlg = ( cListDialog* )GetWindowForID( SKILL_TRAINTIPLIST );
	m_SkillTipListDlg->SetShowSelect( FALSE );
	m_SkillTipListDlg->SetAutoScroll( FALSE );

	m_SkillPoint = ( cStatic* )GetWindowForID( SKILL_TRAINPOINT );
	m_Gold = ( cStatic* )GetWindowForID( SKILL_TRAINGOLD );

	m_Icon = ( cStatic* )GetWindowForID( SKILL_TRAIN_ICON );
	m_level = ( cStatic* )GetWindowForID( SKILL_TRAIN_LEVEL );
	m_NeedSp = ( cStatic* )GetWindowForID( SKILL_TRAIN_SP );
	m_NeedMana = ( cStatic* )GetWindowForID( SKILL_TRAIN_MANA );
	m_NeedGold = ( cStatic* )GetWindowForID( SKILL_TRAIN_GOLD );
}

// 080417 LUJ, ��� ��ų ǥ��
void cSkillTrainingDlg::OpenGuildSkill()
{
	CHero* hero = OBJECTMGR->GetHero();

	if( ! hero )
	{
		return;
	}
	// 080417 LUJ, ��� �����͸� â�� �� �� �ִ�
	else if(	!	hero->GetGuildIdx()		||
					strcmp( hero->GetObjectName(), GUILDMGR->GetMasterName() ) )
	{
        CHATMGR->AddMsg( CTC_GUILD_NOTICE, CHATMGR->GetChatMsg( 976 ) );
		return;
	}
	
	mGuildMode = TRUE;

	// 080417 LUJ, ��Ȳ�� �°� �μ� â�� ǥ���Ѵ�
	{
		cWindow* windowSkillText	= GetWindowForID( SKILL_TRAIN_POINT_TEXT );
		cWindow* windowSkillImage	= GetWindowForID( SKILL_TRAIN_POINT_IMAGE );

		cWindow* windowGuildText	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_TEXT );
		cWindow* windowGuildImage	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_IMAGE );

		if( windowSkillText && windowSkillImage &&
			windowGuildText && windowGuildImage )
		{
			windowSkillText->SetActive( FALSE );
			windowSkillImage->SetActive( FALSE );

			windowGuildText->SetActive( TRUE );
			windowGuildImage->SetActive( TRUE );
		}

		cStatic* windowTitle = ( cStatic* )GetWindowForID( SKILL_TRAIN_TITLE );

		if( windowTitle )
		{
			static const std::string title( RESRCMGR->GetMsg( 924 ) );

			windowTitle->SetStaticText( title.c_str() );
		}
	}

	DWORD count = m_SkillListDlg->GetItemCount();
	int top = m_SkillListDlg->GetTopListItemIdx();
	m_SkillListDlg->RemoveAll();

	WORD JobGrade = HERO->GetCharacterTotalInfo()->JobGrade;
	WORD pos = 0;

	typedef CGuildManager::SkillSettingMap SkillSettingMap;
	const SkillSettingMap* settingMap			= GUILDMGR->GetSkillSetting( GUILDMGR->GetLevel() );
	const SkillSettingMap* maxLevelSettingMap	= GUILDMGR->GetMaxLevelSkillSetting();

	// 080417 LUJ, ������ ���� ������ ���� ��� �ְ� ������ ������ ����Ѵ�
	if( ! maxLevelSettingMap )
	{
		static const std::string textGuild( RESRCMGR->GetMsg( 131 ) );

		CHATMGR->AddMsg( CTC_GUILD_NOTICE, "[%s] %s", textGuild.c_str(), CHATMGR->GetChatMsg( 538 ) );
		return;
	}
	else if( ! settingMap )
	{
		settingMap = maxLevelSettingMap;
	}

	typedef std::map< DWORD, BYTE > SkillMap;
	SkillMap						skillMap;

	// 080417 LUJ, ������ ���� ���� ��ų�� ������ �� �����Ƿ�, ��ų ���� �ִ� ���� ���� ¦���� ���� ���� �����Ѵ�
	for(
		SkillSettingMap::const_iterator it = maxLevelSettingMap->begin();
		maxLevelSettingMap->end() != it;
		++it )
	{
		const DWORD							index	= it->first;
		const CGuildManager::SkillSetting&	setting = it->second;

        if( skillMap.end() == skillMap.find( index ) )
		{
			skillMap.insert( std::make_pair( index, setting.mLevel ) );
		}
		// 080417 LUJ, ���� ��ų ������ ���� ��� �׻� ū ���� ���� �ִ´�
		else
		{
			BYTE& level = skillMap[ index ];

			level = max( level, setting.mLevel );
		}
	}
	
	for(
		SkillMap::const_iterator it = skillMap.begin();
		skillMap.end() != it;
		++it )
	{
		const DWORD	index = it->first;
		const BYTE	level = it->second;
		
		cSkillInfo* skillInfo = SKILLMGR->GetSkillInfo( index );
		
		if( !	skillInfo ||
				// 080417 LUJ, ��ų�� ��� ������� ǥ���� �ʿ䰡 �����Ƿ�, �ִ� ������ �����ߴ��� �˻�
				level <= GUILDMGR->GetSkillLevel( index ) )
		{
			continue;
		}

		BOOL isCheck = FALSE;

		for( DWORD i = 0; i < pos; ++i )
		{
			if( m_SkillList[ i ].index == index )
			{
				isCheck = TRUE;
				break;
			}
		}

		if( isCheck )
		{
			continue;
		}			

		const char* kind	= 0;
		DWORD		color	= 0;

		switch( skillInfo->GetKind() )
		{
		case SKILLKIND_PHYSIC:
		case SKILLKIND_MAGIC:
		case SKILLKIND_ONOFF:
			{
				kind = CHATMGR->GetChatMsg( 175 );
				color = RGB_HALF( 184, 117, 30 );
				break;
			}
		case SKILLKIND_PASSIVE:
			{
				kind = CHATMGR->GetChatMsg( 176 );
				color = RGB_HALF( 73, 65, 150 );
				break;
			}
		}

		const DWORD skillLevel = GUILDMGR->GetSkillLevel( index );

		// 080417 LUJ, ���� ��ų���� ������ �ϳ� �� ���� ������ ���������� �Ѵ�
		cSkillInfo* pRealSkillInfo	= SKILLMGR->GetSkillInfo( skillInfo->GetIndex() + skillLevel );

		if( ! pRealSkillInfo )
		{
			continue;
		}
		// 080310 LUJ,	��ų ���� ���� ������ �ƴ� ��� �ٸ� ���� ǥ���Ѵ�
		else
		{
			const SkillSettingMap::const_iterator it = settingMap->find( index );

			if( settingMap->end() == it )
			{
				color = ICONCLR_DISABLE;
			}
			else
			{
				const CGuildManager::SkillSetting& setting = it->second;

				if( setting.mLevel < skillLevel + 1 )
				{
					color = ICONCLR_DISABLE;
				}
			}
		}
		
		char buf[ MAX_PATH ] = { 0 };
		sprintf( buf, "[%s] %s %d %s",
			kind,
			CHATMGR->GetChatMsg( 177 ),
			GUILDMGR->GetSkillLevel( index ) + 1,
			pRealSkillInfo->GetName() );

		m_SkillListDlg->AddItem( buf, color );
		m_SkillList[ pos ].index	= index;
		m_SkillList[ pos++ ].level	= GUILDMGR->GetSkillLevel( index ) + 1;
	}

	if( count != m_SkillListDlg->GetItemCount() )
	{
		m_Index = 0;
		m_SkillListDlg->SetCurSelectedRowIdx( 0 );
		top = 0;
	}

	m_SkillListDlg->SetTopListItemIdx( top );

	SetSkillInfo();
	SetGold();
	
	// 080417 LUJ, ��� ���� ����
	{
		char text[ MAX_PATH ] = { 0 };
		sprintf( text, "%d", GUILDMGR->GetScore() );

		m_SkillPoint->SetStaticText( text );
	}

	// 080417 LUJ, ��ġ ���. ���â �����̳� ���� �� ���� ������ �ִ� �ʿ� ��ųâ�� ǥ���Ѵ�
	{
		cDialog* dialog = WINDOWMGR->GetWindowForID( GD_GUILDDLG );

		if( dialog &&
			dialog->IsActive() )
		{
			const DISPLAY_INFO& screen	= GAMERESRCMNGR->m_GameDesc.dispInfo;

			// 080417 LUJ, ������ ���� ������ �ִ� ���
			if( screen.dwWidth > dialog->GetAbsX() + dialog->GetWidth() + GetWidth() )
			{
				SetAbsXY(
					LONG( dialog->GetAbsX() + dialog->GetWidth() ),
					LONG( dialog->GetAbsY() ) );
			}
			// 080417 LUJ, ������ ���� ������ �ִ� ���
			else
			{
				SetAbsXY(
					LONG( dialog->GetAbsX() - GetWidth() ),
					LONG( dialog->GetAbsY() ) );
			}
		}
	}

	SetActive( TRUE );
}

// 080618 LYW --- cSkillTrainingDlg : ��ų ���� â�� ��ų ��� �Լ� ����.
//void cSkillTrainingDlg::OpenDialog()
//{
//	mGuildMode = FALSE;
//
//	// 080417 LUJ, ��Ȳ�� �°� �μ� â�� ǥ���Ѵ�
//	{
//		cWindow* windowSkillText	= GetWindowForID( SKILL_TRAIN_POINT_TEXT );
//		cWindow* windowSkillImage	= GetWindowForID( SKILL_TRAIN_POINT_IMAGE );
//
//		cWindow* windowGuildText	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_TEXT );
//		cWindow* windowGuildImage	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_IMAGE );
//
//		if( windowSkillText && windowSkillImage &&
//			windowGuildText && windowGuildImage )
//		{
//			windowSkillText->SetActive( TRUE );
//			windowSkillImage->SetActive( TRUE );
//
//			windowGuildText->SetActive( FALSE );
//			windowGuildImage->SetActive( FALSE );
//		}
//
//		cStatic* windowTitle = ( cStatic* )GetWindowForID( SKILL_TRAIN_TITLE );
//
//		if( windowTitle )
//		{
//			static const std::string title( RESRCMGR->GetMsg( 351 ) );
//
//			windowTitle->SetStaticText( title.c_str() );
//		}
//	}
//
//	DWORD count = m_SkillListDlg->GetItemCount();
//	int top = m_SkillListDlg->GetTopListItemIdx();
//	m_SkillListDlg->RemoveAll();
//	
//	WORD Job;
//	WORD JobGrade = HERO->GetCharacterTotalInfo()->JobGrade;
//	WORD pos = 0;
//
//	for( WORD CurGrade = JobGrade; CurGrade > 0; CurGrade-- )
//	{
//		BYTE jobtype = 0 ;
//
//		if( CurGrade == 1 )
//		{
//			jobtype = 1 ;
//		}
//		else
//		{
//			jobtype = HERO->GetCharacterTotalInfo()->Job[ CurGrade - 1 ] ;
//		}
//
//		Job = ( HERO->GetCharacterTotalInfo()->Job[ 0 ] * 1000 ) +
//			  ( ( HERO->GetCharacterTotalInfo()->Race + 1 ) * 100 ) + 
//			  ( ( CurGrade ) * 10 ) + 
//			  jobtype;
//
//		SKILL_TREE_INFO* pInfo = SKILLMGR->GetSkillTreeInfo( Job );
//
//		if( !pInfo )
//		{
//			return;
//		}
//
//		for( WORD cnt = 0; cnt < pInfo->Count; cnt++ )
//		{
//			DWORD index = ( ( DWORD )pInfo->SkillIndex[ cnt ] / 100 ) * 100 + 1;
//			DWORD level = pInfo->SkillIndex[ cnt ] % 100;
//
//			cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( index );
//			cSkillBase* pSkillBase = SKILLTREEMGR->GetSkill( index );
//
//			if( !pSkillInfo )
//			{
//				continue;
//			}
//
//			BOOL chk = 0;
//			for( WORD idx = 0; idx < pos; idx++ )
//			{
//				if( m_SkillList[ idx ].index == index )
//				{
//					chk = 1;
//					break;
//				}
//			}
//			if( chk )
//			{
//				continue;
//			}
//
//			if( pSkillBase )
//			{
//				DWORD curLevel;
//
//				if( ! pSkillBase->GetDBIdx() )
//				{
//					const SKILL_BASE* skillBase	= SKILLTREEMGR->GetStoredSkill( index );
//
//					curLevel = skillBase ? skillBase->Level : 0;
//				}
//				else
//				{
//					curLevel = pSkillBase->GetSkillBase()->Level;
//				}
//
//				if( level > curLevel )
//				{
//					level = curLevel + 1;
//				}
//				else 
//				{
//					continue;
//				}
//			}
//			else
//			{
//				level = 1;
//			}
//
//			char buf[ 256 ];
//			char* kind;
//			DWORD color = 0;
//			switch( pSkillInfo->GetKind() )
//			{
//			case SKILLKIND_PHYSIC:
//			case SKILLKIND_MAGIC:
//			case SKILLKIND_ONOFF:
//				{
//					kind = CHATMGR->GetChatMsg( 175 );
//					// 080507 LUJ, � �������̽� �׸��� ����Ǿ �� ���̵��� ����
//					color = RGB_HALF( 255, 255, 255 );
//				}
//				break;
//
//			case SKILLKIND_PASSIVE:
//				{
//					kind = CHATMGR->GetChatMsg( 176 );
//					color = RGB_HALF( 73, 65, 150 );
//				}
//				break;
//			}
//			
//			cSkillInfo* pRealSkillInfo = SKILLMGR->GetSkillInfo( pSkillInfo->GetIndex() - 1 + level );
//
//			if( ! pRealSkillInfo )
//			{
//				continue;
//			}
//
//			// 080310 LUJ,	��ų ���� ���� ������ �ƴ� ��� �ٸ� ���� ǥ���Ѵ�
//			//
//			//				��ų Ʈ���δ� �ʿ��� ������ ���� �� �����Ƿ�, ǥ���� ��ų��
//			//				�ְ� �� �������� ������� ��ȸ�Ͽ� ǥ�� ���θ� �����Ѵ�.
//			{
//				// 080310 LUJ,	������ ĳ����������, ���� ��ų�� ��� cActiveSkillInfo���·� ���� ĳ���õǾ�
//				//				�������� ���ǰ� �����Ƿ�, ���⼭�� ����Ѵ�
//				const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pRealSkillInfo )->GetSkillInfo();
//
//				if( !	info ||
//						info->RequiredPlayerLevel > HERO->GetLevel() )
//				{
//					color = ICONCLR_DISABLE;
//				}
//			}
//
//			sprintf( buf, "[%s] %s %d %s", kind, CHATMGR->GetChatMsg( 177 ), level, pRealSkillInfo->GetName() );
//
//			m_SkillListDlg->AddItem( buf, color );
//			m_SkillList[ pos ].index = index;
//			m_SkillList[ pos++ ].level = (BYTE)level;
//		}
//	}
//
//	if( count != m_SkillListDlg->GetItemCount() )
//	{
//		m_Index = 0;
//		m_SkillListDlg->SetCurSelectedRowIdx( 0 );
//		top = 0;
//	}
//
//	m_SkillListDlg->SetTopListItemIdx( top );
//
//	SetSkillInfo();
//	SetGold();
//	SetSkillPoint();
//	SetActive( TRUE );
//}




//-------------------------------------------------------------------------------------------------
//	NAME : DisplayGuildSkill
//	DESC : 080417 LUJ, ��Ȳ�� �°� �μ� â�� ǥ���Ѵ�
//-------------------------------------------------------------------------------------------------
void cSkillTrainingDlg::DisplayGuildSkill()
{
	cWindow* windowSkillText	= GetWindowForID( SKILL_TRAIN_POINT_TEXT );
	cWindow* windowSkillImage	= GetWindowForID( SKILL_TRAIN_POINT_IMAGE );

	cWindow* windowGuildText	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_TEXT );
	cWindow* windowGuildImage	= GetWindowForID( SKILL_TRAIN_GUILD_POINT_IMAGE );

	if( windowSkillText && windowSkillImage &&
		windowGuildText && windowGuildImage )
	{
		windowSkillText->SetActive( TRUE );
		windowSkillImage->SetActive( TRUE );

		windowGuildText->SetActive( FALSE );
		windowGuildImage->SetActive( FALSE );
	}

	cStatic* windowTitle = ( cStatic* )GetWindowForID( SKILL_TRAIN_TITLE );

	if( windowTitle )
	{
		static const std::string title( RESRCMGR->GetMsg( 351 ) );

		windowTitle->SetStaticText( title.c_str() );
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : GetSkillLevelFromJobTree
//	DESC : 080618 LYW : HERO�� ���� ���� �� �߿� �ش��ϴ� ��ų�� �ִ��� Ȯ���ϰ�, �ִٸ�,
//		 : �� ��ų�� ���� ������ �ִ� ������ ��ȯ�ϴ� �Լ� �߰�.
//-------------------------------------------------------------------------------------------------
int cSkillTrainingDlg::GetSkillLevelFromJobTree(DWORD dwSkillIdx, WORD wCurJobLevel)
{
	// ������ ����� ���� ����.
	int nResult = -1 ;


	// ���� ���� ������ üũ�Ѵ�.
	if(wCurJobLevel == 1) return nResult ;


	// ���� �ε��� / ��ų ������ ���� ���� ����.
	WORD wJobIdx ;
	SKILL_TREE_INFO* pInfo ;

	DWORD dwIndex ;

	WORD wCount ;

	BYTE byJobType ;


	// ���� �������� üũ�ϸ�, ��ų�� ���� �����߾����� Ȯ���Ѵ�.
	for( WORD CurGrade = wCurJobLevel-1; CurGrade > 0; CurGrade-- )
	{
		byJobType = 0 ;

		// ���� �迭�� ��ȿ�� üũ.
		if( CurGrade == 1 ) byJobType = 1 ;
		else byJobType = HERO->GetCharacterTotalInfo()->Job[ CurGrade - 1 ] ;

		if(!byJobType) return nResult ;

		// ���� ������ �´� ���� �ε����� �޴´�.
		wJobIdx = 0 ;											
		wJobIdx = ( HERO->GetCharacterTotalInfo()->Job[ 0 ] * 1000 ) +
				( ( HERO->GetCharacterTotalInfo()->Race + 1 ) * 100 ) + 
				( ( CurGrade ) * 10 ) + 
				byJobType ;

		// ������ �´� ��ų Ʈ�� ������ �޴´�.
		pInfo = NULL ;
		pInfo = SKILLMGR->GetSkillTreeInfo( wJobIdx ) ;

		if(!pInfo) continue ;

		// ������ ���� ��ų ���� ��ŭ for���� ���� ��ų�� �߰��Ѵ�.
		for( wCount = 0 ; wCount < pInfo->Count ; wCount++ )
		{
			// ��ų �ε��� �ޱ�.
			dwIndex = 0 ;
			dwIndex = ( ( DWORD )pInfo->SkillIndex[ wCount ] / 100 ) * 100 + 1 ;

			// ���ڷ� �Ѿ�� ��ų�� ���� ��ų���� üũ�Ѵ�.
			if(dwSkillIdx != dwIndex) continue ;

			if(pInfo->SkillIndex[ wCount ] < dwIndex) return nResult ;

			nResult = ((int)pInfo->SkillIndex[ wCount ] - dwIndex) + 1 ;
			return nResult ;
		}
	}

	// ��� ����.
	return nResult ;
}





//-------------------------------------------------------------------------------------------------
// NAME : AddSkillToSkillListDlg
// DESC : 080618 LYW : ���� ���� ������ ���� ��ȣ�� �Է� �ϸ�, �׿� ���� ��ų�� ��ų����Ʈ 
//		: ���̾�α׿� �߰��ϴ� �Լ��� �߰��Ѵ�.
//-------------------------------------------------------------------------------------------------
void cSkillTrainingDlg::AddSkillToSkillListDlg(BYTE byJobType, WORD wJobLevel, WORD* pPos)
{
	// ���� �迭�� ��ȿ�� üũ.
	if( wJobLevel == 1 ) byJobType = 1 ;
	if(!byJobType) return ;


	// ���� ������ �´� ���� �ε����� �޴´�.
	WORD wJobIdx = 0 ;											
	wJobIdx = ( HERO->GetCharacterTotalInfo()->Job[ 0 ] * 1000 ) +
			( ( HERO->GetCharacterTotalInfo()->Race + 1 ) * 100 ) + 
			( ( wJobLevel ) * 10 ) + 
			byJobType ;


	// ������ �´� ��ų Ʈ�� ������ �޴´�.
	SKILL_TREE_INFO* pInfo = SKILLMGR->GetSkillTreeInfo( wJobIdx ) ;
	if(!pInfo) return ;


	// ������ ���� ��ų�� �߰��ϱ� ���� �������� �����Ѵ�.
	WORD  wCount ;												// for���� ���� ī��Ʈ ����.
	WORD  wCount2 ;												//

	DWORD dwIndex ;												// ��ų �ε���.
	DWORD dwLevel ;												// ��ų ����.
	DWORD dwCurLevel ;											// ���� ��ų ����.

	cSkillInfo* pSkillInfo ;									// ��ų ���� ������.
	cSkillBase* pSkillBase ;									// ��ų �⺻ ���� ������.
	cSkillInfo* pRealSkillInfo ;								// ���� ��ų ������.

	BYTE byChecked ;											// üũ �� ��ų���� ���θ� ���� ����.

	char buf[ 256 ] ;											// ��ų��� ������ ���� �� ����.
	char* kind ;												// ��ų ������ ���� ������ (��Ƽ��/�нú�)
	DWORD dwColor ;												// ��ų���� ������ ���� ����.


	// ������ ���� ��ų ���� ��ŭ for���� ���� ��ų�� �߰��Ѵ�.
	for( wCount = 0 ; wCount < pInfo->Count ; wCount++ )
	{
		// ��ų �ε��� �ޱ�.
		dwIndex = 0 ;
		dwIndex = ( ( DWORD )pInfo->SkillIndex[ wCount ] / 100 ) * 100 + 1 ;

		// ��ų ���� �ޱ�.
		dwLevel = 0 ;
		dwLevel = pInfo->SkillIndex[ wCount ] % 100 ;

		// ��ų ���� �ޱ�.
		pSkillInfo = NULL ;
		pSkillInfo = SKILLMGR->GetSkillInfo( dwIndex ) ;

		// ��ų �⺻ ���� �ޱ�.
		pSkillBase = NULL ;
		pSkillBase = SKILLTREEMGR->GetSkill( dwIndex ) ;

		// ��ų ���� üũ.
		if( !pSkillInfo ) continue ;

		// üũ �� ��ų���� Ȯ���Ѵ�.
		byChecked = FALSE ;
		for( wCount2 = 0 ; wCount2 < *pPos ; wCount2++ )
		{
			if( m_SkillList[ wCount2 ].index == dwIndex )
			{
				byChecked = TRUE ;
				break ;
			}
		}
		if( byChecked ) continue ;

		// ��ų �⺻ ������ ��ȿ���� üũ ��, ��ų ������ �����Ѵ�.
		if( pSkillBase )
		{
			// ���� ��ų���� �ʱ�ȭ.
			dwCurLevel = 0 ;

			// ��ų�� DB? �ε����� ��ȿ�ϸ�,
			if( ! pSkillBase->GetDBIdx() )
			{
				// ȹ�� �� ��ų �⺻ ������ �޴´�.
				const SKILL_BASE* skillBase	= SKILLTREEMGR->GetStoredSkill( dwIndex ) ;
				// ������ �����Ѵ�.
				dwCurLevel = skillBase ? skillBase->Level : 0 ;
			}
			// ��ų�� DB? �ε����� ��ȿ���� ������,
			else
			{
				// ��ų�� �⺻ ������ �޴´�.
				dwCurLevel = pSkillBase->GetSkillBase()->Level ;
			}

			// ��ų ������, ���� ������ ��ų�� �������� ������,
			if( dwLevel > dwCurLevel )
			{
				// �Ѵܰ� ���� ������ ������Ų��.
				dwLevel = dwCurLevel + 1 ;
			}
			// �̵� ���� �ƴѰ�쿡��, ���� �������� ���� �� ��ų���� Ȯ���Ѵ�.
			else 
			{
				// ���� ���� �� �߿���, ���� ��ų�� ���� ��ų�� ������ �������� üũ�ϰ�,
				// ������ �Ѱ� �޴´�.
				int nResult = -1 ;
				nResult = GetSkillLevelFromJobTree(dwIndex, wJobLevel) ;

				if(nResult == -1) continue ;
				else
				{
					dwLevel = ((DWORD)nResult) ;
					
					if(dwLevel > dwCurLevel)
					{
						dwLevel = dwCurLevel + 1 ;
					}
					else continue ;
				}
			}
		}
		// ��ų �⺻ ������ ��ȿ���� ������, ���� �������� ���� ��ų�̹Ƿ�, ������ 1�� �����Ѵ�.
		else
		{
			dwLevel = 1 ;
		}

		// ��ų ������ ����, ��ų��� ����, ���� ������ �����Ѵ�.
		switch( pSkillInfo->GetKind() )
		{
		case SKILLKIND_PHYSIC :										// ���� ��ų.
		case SKILLKIND_MAGIC :										// ���� ��ų.
		case SKILLKIND_ONOFF :										// ON/OFF�� ��ų.
			{
				// [��Ƽ��] ���ڿ��� �޴´�.
				kind = CHATMGR->GetChatMsg( 175 ) ;
				// 080507 LUJ, � �������̽� �׸��� ����Ǿ �� ���̵��� ����
				dwColor = RGB_HALF( 255, 255, 255 ) ;
			}
			break ;

		case SKILLKIND_PASSIVE :									// �нú� ��ų.
			{
				// [�нú�] ���ڿ��� �޴´�.
				kind = CHATMGR->GetChatMsg( 176 ) ;
				// �нú� ������ �����Ѵ�.
				dwColor = RGB_HALF( 73, 65, 150 ) ;
			}
			break ;

		//default : dwColor = 0 ; break ;
		}
		
		// ������ �´� ��¥ ��ų ������ �޴´�.
		pRealSkillInfo = NULL ;
		pRealSkillInfo = SKILLMGR->GetSkillInfo( pSkillInfo->GetIndex() - 1 + dwLevel ) ;

		// ��ų ���� üũ.
		if( ! pRealSkillInfo ) continue ;

		// 080310 LUJ,	��ų ���� ���� ������ �ƴ� ��� �ٸ� ���� ǥ���Ѵ�
		//
		//				��ų Ʈ���δ� �ʿ��� ������ ���� �� �����Ƿ�, ǥ���� ��ų��
		//				�ְ� �� �������� ������� ��ȸ�Ͽ� ǥ�� ���θ� �����Ѵ�.
		{
			// 080310 LUJ,	������ ĳ����������, ���� ��ų�� ��� cActiveSkillInfo���·� ���� ĳ���õǾ�
			//				�������� ���ǰ� �����Ƿ�, ���⼭�� ����Ѵ�
			const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pRealSkillInfo )->GetSkillInfo() ;

			if( !info || info->RequiredPlayerLevel > HERO->GetLevel() )
			{
				dwColor = ICONCLR_DISABLE ;
			}
		}

		// ��ų ���� / ��ų�� / ���� ���� ������ ���ۿ� �����Ѵ�.
		sprintf( buf, "[%s] %s %d %s", kind, CHATMGR->GetChatMsg( 177 ), dwLevel, pRealSkillInfo->GetName() ) ;

		// ��ų����Ʈ ���̾�α׿� �߰��Ѵ�.
		m_SkillListDlg->AddItem( buf, dwColor ) ;

		// ��ų ����Ʈ�� �ε����� ������ �߰��Ѵ�.
		m_SkillList[ *pPos ].index = dwIndex ;
		m_SkillList[ *pPos ].level = (BYTE)dwLevel ;

		*pPos = (*pPos + 1) ;
	}
}





//-------------------------------------------------------------------------------------------------
//	NAME : OpenDialog
//	DESC : 080618 LYW : ��ų ���� â�� ��� ����� �����Ѵ�.
//		 : 20080613�� ���׸���Ʈ 38��
//		 : ���� ������ ��ų ������ ���� �������� ���� ���, �� �̻� ������ �� ���� ����.
//		 : ���� �����ϵ��� ���� ��.
//-------------------------------------------------------------------------------------------------
void cSkillTrainingDlg::OpenDialog()
{
	// ��ų ����Ʈ ���̾�α� üũ.
	if(!m_SkillListDlg) return ;


	// ��� ��带 FALSE�� ����.
	mGuildMode = FALSE;


	// ��Ȳ�� �°� ��� �μ�â�� ǥ���Ѵ�.
	DisplayGuildSkill() ;


	// ��ų ī��Ʈ / ž �ε����� �޴´�.
	DWORD count = 0 ;
	count = m_SkillListDlg->GetItemCount() ;

	int top = 0 ;
	top = m_SkillListDlg->GetTopListItemIdx() ;


	// ��ų ����Ʈ ���̾�α׸� ����.
	m_SkillListDlg->RemoveAll() ;
	

	// ��ų ������ ���� �ʿ��� �������� �����Ѵ�.
	WORD JobGrade = HERO->GetCharacterTotalInfo()->JobGrade ;	// ���� ���� ���� (1~6��)


	// ���� �� HERO�� ���Ŀ� ������ FOR���� ���� ��ų�� ��ų ����Ʈ�� �߰��Ѵ�.
	BYTE byJobType ;
	WORD wPos = 0 ;												// ��ġ ���� ���� ����.
	for( WORD CurGrade = JobGrade; CurGrade > 0; CurGrade-- )
	{
		byJobType = 0 ;
		byJobType = HERO->GetCharacterTotalInfo()->Job[ CurGrade - 1 ] ;

		AddSkillToSkillListDlg(byJobType, CurGrade, &wPos) ;
	}


	// ��ų ī��Ʈ�� ���ߴٸ�, ��ų����Ʈ�� ž���� ���ΰ�ħ�Ѵ�.
	if( count != m_SkillListDlg->GetItemCount() )
	{
		m_Index = 0 ;
		m_SkillListDlg->SetCurSelectedRowIdx( 0 ) ;
		top = 0 ;
	}


	// ��ų ����Ʈ ���̾�α׸� ž�ε��� ��ġ�� �����Ѵ�.
	m_SkillListDlg->SetTopListItemIdx( top ) ;


	// ��ų ������ �����Ѵ�.
	SetSkillInfo() ;


	// ��带 �����Ѵ�.
	SetGold() ;


	// ��ų ����Ʈ�� �����Ѵ�.
	SetSkillPoint() ;


	// ��ų ����â�� Ȱ��ȭ �Ѵ�.
	SetActive( TRUE ) ;
}

void cSkillTrainingDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case SKILL_TRAINOK:
			{
				if( m_Index != -1 )
				{
					DWORD skillIndex		= m_SkillList[ m_Index ].index;
					BYTE skillNextLevel	= m_SkillList[ m_Index ].level;
					cSkillBase* pSkill			= SKILLTREEMGR->GetSkill( skillIndex );

					if( pSkill )
					{
						BYTE		skillLevel;

						if( pSkill->GetDBIdx() )
						{
							skillLevel	= pSkill->GetLevel();
						}
						else
						{
							const SKILL_BASE* storedSkill = SKILLTREEMGR->GetStoredSkill( skillIndex );

							skillLevel = ( storedSkill ? storedSkill->Level : 0 );
						}

						if( skillLevel >= skillNextLevel )
						{
							return;
						}
					}

					DWORD idx = skillIndex - 1 + skillNextLevel;

					cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( idx );

					if( !pSkillInfo )
					{
						return;
					}

					// 080417 LUJ, ��� ��ų�� ��� ȹ�� ������ �����ϴ��� �˻�
					if( mGuildMode )
					{
						const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pSkillInfo )->GetSkillInfo();

						if( ! info )
						{
							static const std::string textGuild( RESRCMGR->GetMsg( 131 ) );

							CHATMGR->AddMsg(
								CTC_GUILD_NOTICE,
								"[%s] %s",
								textGuild.c_str(),
								CHATMGR->GetChatMsg( 1444 ) );
							break;
						}
						else if(	info->TrainMoney > HERO->GetMoney()	||
									info->TrainPoint > GUILDMGR->GetScore() )
						{
							CHATMGR->AddMsg( CTC_GUILD_NOTICE, CHATMGR->GetChatMsg( 605 ) );
							break;
						}

						// 080417 LUJ, ��� ��ų ������ ��ȿ���� �˻�(��ų �������� �ϴ� ���� �������)
						if( ! GUILDMGR->GetSkillLevel( skillIndex ) )
						{
							const CGuildManager::LevelSetting* setting = GUILDMGR->GetLevelSetting( GUILDMGR->GetLevel() );

							if( !	setting	||
									setting->mSkillSize <= GUILDMGR->GetSkillSize() )
							{
								CHATMGR->AddMsg( CTC_GUILD_NOTICE, CHATMGR->GetChatMsg( 977 ), GUILDMGR->GetLevel() + 1 );
								break;
							}
						}
					}

					cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SKILLTRAIN, MBT_YESNO, CHATMGR->GetChatMsg( 1172 ), pSkillInfo->GetName(), pSkillInfo->GetLevel() );		

					if( !pMsgBox ) 
					{
						return;				
					}

					SetDisable( TRUE );
				}
			}
			break;
		case SKILL_TRAINCANCEL:
			{
				SetActive( FALSE );
			}
			break;
		}
	}
	else if( we & WE_ROWCLICK )
	{
		if( IsDisable() )
		{
			return;
		}

		m_Index = m_SkillListDlg->GetCurSelectedRowIdx();
		SetSkillInfo();
	}
}

void cSkillTrainingDlg::SetSkillInfo()
{
	WORD INFO_LINE_LEN = 35;

	if( m_Index == -1 )
		return;

	m_SkillTipListDlg->RemoveAll();

	DWORD index = ( ( m_SkillList[ m_Index ].index / 100 ) * 100 ) + m_SkillList[ m_Index ].level;
	
	cSkillInfo* pInfo = SKILLMGR->GetSkillInfo( index );

	if( !pInfo )
		return;

	DWORD tooltip = pInfo->GetTooltip();

	TOOLTIP_TEXT* pTooltipText = ITEMMGR->GetItemToolTipInfo( tooltip );

	char buf[ 512 ] = "";

	while( pTooltipText )
	{
		strcat( buf, pTooltipText->strToolTip );
		pTooltipText = pTooltipText->pNext;
	}

	// 080417 LUJ, ��� ��ų ǥ�� ������ ��, �䱸 ������ �˷��ش�
	if( mGuildMode )
	{
		typedef CGuildManager::SkillSettingMap SkillSettingMap;
		const SkillSettingMap* currentSettingMap	= GUILDMGR->GetSkillSetting( GUILDMGR->GetLevel() );
		const SkillSettingMap* maxLevelSettingMap	= GUILDMGR->GetMaxLevelSkillSetting();

		if( ! maxLevelSettingMap )
		{
			return;
		}
		else if( ! currentSettingMap )
		{
			currentSettingMap = maxLevelSettingMap;
		}

		cWindow* window	= GetWindowForID( SKILL_TRAINOK );

		if( window )
		{
			const DWORD skillIndex			= m_SkillList[ m_Index ].index;
			const DWORD skillLevel			= m_SkillList[ m_Index ].level;
			DWORD		requiredGuildLevel	= 0;

			// 080417 LUJ, �ش� ��ų�� �䱸�ϴ� �ּ� ��� ������ ã�´�
			for(
				const SkillSettingMap* settingMap = 0;
				( settingMap = GUILDMGR->GetSkillSetting( ++requiredGuildLevel ) ) != maxLevelSettingMap; )
			{
				if( ! settingMap )
				{
					currentSettingMap = maxLevelSettingMap;
					break;
				}
				
				const SkillSettingMap::const_iterator it = settingMap->find( skillIndex );

				if( settingMap->end() == it )
				{
					continue;
				}

				const CGuildManager::SkillSetting& setting = it->second;

				if( skillLevel <= setting.mLevel )
				{
					break;
				}	
			}

			char text[ MAX_PATH ] = { 0 };
			{
				// 080417 LUJ, ���ҽ� �ؽ�Ʈ�� �׼��� ������ ������ ���� �������Ƿ� ��� �����ͷ� ������ �� ����. �� ���縦 �صξ�� �Ѵ�.
				static const std::string textGuild			( RESRCMGR->GetMsg( 131 ) );
				static const std::string textRequiredLevel	( RESRCMGR->GetMsg( 627 ) );

				sprintf( text, "[%s %s%d]",
					textGuild.c_str(),
					textRequiredLevel.c_str(),
					requiredGuildLevel );
			}

			// 080417 LUJ, ���� ������ ��� �� ���� ��ų�� ��� ���� ǥ��
			{
				DWORD color = ICONCLR_DISABLE;

				const SkillSettingMap::const_iterator it = currentSettingMap->find( skillIndex );

				if( currentSettingMap->end() != it )
				{
					const CGuildManager::SkillSetting& setting = it->second;

					if( setting.mLevel >= skillLevel )
					{
						color = TTTC_RAREITEM;
					}
				}

				window->SetActive( TTTC_RAREITEM == color );
				m_SkillTipListDlg->AddItem( text, color );
			}
		}
	}
	// 080310 LUJ, ��ų�� �н� ���� �������� �÷��̾� ������ ���� ��� ǥ���Ѵ�
	else
	{		
		cWindow* window	= GetWindowForID( SKILL_TRAINOK );

		// 080310 LUJ,	������ ĳ����������, ���� ��ų�� ��� cActiveSkillInfo���·� ���� ĳ���õǾ�
		//				�������� ���ǰ� �����Ƿ�, ���⼭�� ����Ѵ�
		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pInfo )->GetSkillInfo();

		if( window	&&
			info )
		{
			if( info->RequiredPlayerLevel )
			{
				char text[ MAX_PATH ] = { 0 };
				sprintf( text, "[%s%d]", RESRCMGR->GetMsg( 627 ), info->RequiredPlayerLevel );

				if( info->RequiredPlayerLevel > HERO->GetLevel() )
				{
					m_SkillTipListDlg->AddItem( text, ICONCLR_DISABLE );

					window->SetActive( FALSE );
				}
				else
				{
					m_SkillTipListDlg->AddItem( text, TTTC_RAREITEM );

					window->SetActive( TRUE );
				}
			}
			// 080319 LUJ, ��� �� ���� ��ų�� �ִٰ� ���� ��� ��ư�� ǥ�õ��� �ʴ� ���� ����
			else
			{
				window->SetActive( TRUE );
			}
		}
	}

	WORD pos = 0;
	char Temp[128] = "";
	char* str = buf;
	int nLen = strlen( buf );
	int nCpyNum = 0;

	while( nLen > INFO_LINE_LEN )
	{
		if( ( str + INFO_LINE_LEN ) != CharNext( CharPrev( str, str + INFO_LINE_LEN ) ) )
			nCpyNum = INFO_LINE_LEN - 1;
		else
			nCpyNum = INFO_LINE_LEN;

		strncpy( Temp, str, nCpyNum );
		Temp[nCpyNum] = 0;
		m_SkillTipListDlg->AddItem( Temp, RGB_HALF( 255, 255, 255 ) );

		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;

		nLen = strlen( str );
	}
	if( nLen > 0 )
	{
		strncpy( Temp, str, nLen );
		Temp[nLen] = 0;
		m_SkillTipListDlg->AddItem( Temp, RGB_HALF( 255, 255, 255 ) );
	}

	m_SkillTipListDlg->SetTopListItemIdx( 0 );

	cImage BasicImage;

	SCRIPTMGR->GetImage( pInfo->GetImage(), &BasicImage, PFT_SKILLPATH );
	
	LONG x = (LONG)(m_Icon->GetAbsX());
	LONG y = (LONG)(m_Icon->GetAbsY());

	m_Icon->Init((LONG)(m_Icon->GetRelX()), (LONG)(m_Icon->GetRelY()), m_Icon->GetWidth(), m_Icon->GetHeight(), &BasicImage, SKILL_TRAIN_ICON);

	m_Icon->SetAbsXY( x, y );

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;

	char text[256] = "";

	if( mGuildMode )
	{
		sprintf( text, "%d", GUILDMGR->GetSkillLevel( index / 100 * 100 + 1 )  + 1 );
	}
	else
	{
		sprintf( text, "%d", pSkillInfo->GetSkillInfo()->Level );
	}
	
	m_level->SetStaticText( text );

	sprintf( text, "%d", pSkillInfo->GetSkillInfo()->TrainPoint );
	m_NeedSp->SetStaticText( text );
	m_NeedGold->SetStaticText( AddComma( pSkillInfo->GetSkillInfo()->TrainMoney ) );

	// 080616 LUJ, �ۼ�Ʈ ������ ó���ϵ��� �Ѵ�
	{
		const ACTIVE_SKILL_INFO* info = pSkillInfo->GetSkillInfo();

		if( ! info )
		{
			return;
		}
		else if( info->mMana.mPercent )
		{
			sprintf( text, "%0.f%%", info->mMana.mPercent * 100.0f );
			m_NeedMana->SetStaticText( text );
		}
		else if( info->mMana.mPlus )
		{
			sprintf( text, "%0.f", info->mMana.mPlus );
			m_NeedMana->SetStaticText( text );
		}
	}
}

void cSkillTrainingDlg::SetGold()
{
	m_Gold->SetStaticText( AddComma( HERO->GetHeroTotalInfo()->Money ) );
}

void cSkillTrainingDlg::SetSkillPoint()
{
	char buf[256];

	sprintf( buf, "%d", HERO->GetHeroTotalInfo()->SkillPoint );
	m_SkillPoint->SetStaticText( buf );
}

void cSkillTrainingDlg::Training()
{
	if( m_Index != -1 )
	{
		const DWORD skillIndex		= m_SkillList[ m_Index ].index;
		const BYTE skillNextLevel	= m_SkillList[ m_Index ].level;
		cSkillBase* pSkill			= SKILLTREEMGR->GetSkill( skillIndex );

		if( pSkill )
		{
			BYTE		skillLevel;

			if( pSkill->GetDBIdx() )
			{
				skillLevel	= pSkill->GetLevel();
			}
			else
			{
				const SKILL_BASE* storedSkill = SKILLTREEMGR->GetStoredSkill( skillIndex );

				skillLevel = ( storedSkill ? storedSkill->Level : 0 );
			}

			if( skillLevel >= skillNextLevel )
			{
				return;
			}
		}

		MSG_SKILL_UPDATE msg;
		ZeroMemory( &msg, sizeof( msg ) );

		// 080417 LUJ, ��� ��ų ȹ�� ��� �߰�
		if( mGuildMode )
		{
			msg.Category 			= MP_GUILD;
			msg.Protocol 			= MP_GUILD_ADD_SKILL_SYN;
		}
		else
		{
			msg.Category 			= MP_SKILLTREE;
			msg.Protocol 			= MP_SKILLTREE_UPDATE_SYN;
		}
		
		msg.dwObjectID			= HEROID;
		msg.SkillBase.wSkillIdx = skillIndex;
		msg.SkillBase.Level		= skillNextLevel;

		NETWORK->Send( &msg, sizeof( msg ) );
	}	
}
