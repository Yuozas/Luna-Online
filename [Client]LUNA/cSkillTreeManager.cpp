#include "stdafx.h"
#include "cSkillTreeManager.h"
//#include "cSkillTreeDlg.h"
#include "cJobSkillDlg.h"

//#include "MHFile.h"
//#include "GameIn.h"
#include "../[cc]skill/client/info/ActiveSkillInfo.h"
#include "../[cc]skill/client/manager/Skillmanager.h"
#include "cSkillbase.h"
#include "cSkillTrainingDlg.h"

#include "WindowIDEnum.h"
#include "Interface/cWindowManager.h"
#include "Interface/cScriptManager.h"
#include "interface/cIconGridDialog.h"
#include "interface/cTabDialog.h"

#include "ItemManager.h"
#include "chatmanager.h"
//#include "GameDef.h"
#include "QuickManager.h"
#include "ObjectManager.h"
//#include "CheatMsgParser.h"
//#include "GameEventManager.h"

#include "GameIn.h"
#include "InventoryExDialog.h"

#include "CharacterCalcManager.h"


cSkillTreeManager::cSkillTreeManager(void)
{
	mSkillBaseTable.Initialize( 10 );
	mIconIndexCreator.Init(MAX_ITEMICON_NUM, IG_SKILL_STARTINDEX);		
}

cSkillTreeManager::~cSkillTreeManager(void)
{
	mIconIndexCreator.Release();
}

void cSkillTreeManager::InitSkillInfo( SKILL_BASE* pSkill, WORD num )
{
	for( ; num--; ++pSkill )
	{
		const DWORD	skillIndex		= pSkill->wSkillIdx;

		// 스킬 정보가 아이템 정보보다 늦게 올 경우 세트 아이템으로 적용된 스킬 정보가 사라지게 된다.
		// 이를 막기 위해 적용된 세트 스킬이 있는지 체크하고, 그럴 경우 저장해야 한다.
		{
			cSkillBase*	currentSkill	= GetSkill( skillIndex );

			// 세트 스킬이 적용되어 잇는 경우 제거하고 적용할 스킬을 세트 스킬에
			// 맞춰 더 높은 레벨로 적용시켜줘야한다.
			if(	currentSkill )
			{
				if( ! currentSkill->GetDBIdx() )
				{
					SKILL_BASE* storedSkill = GetStoredSkill( skillIndex );

					if( storedSkill )
					{
						const BYTE setLevel = currentSkill->GetLevel() - storedSkill->Level;

						storedSkill->Level	= pSkill->Level;

						pSkill->Level		+= setLevel;
						pSkill->dwDBIdx		= 0;
					}
					else
					{
						SetStoredSkill( pSkill );

						pSkill->Level		= currentSkill->GetLevel() + pSkill->Level;
						pSkill->dwDBIdx		= 0;
					}
				}

				RemoveSkill( currentSkill );
			}
		}

		cSkillBase* pSkillBase = NewSkillIcon( pSkill );

		if( ! pSkillBase )
		{
			ASSERT( 0 );
			return;
		}

		const DWORD skillSize = SKILLMGR->GetSkillSize( pSkill->wSkillIdx );

		if( skillSize < pSkill->Level )
		{
			ASSERT( 0 );
			return;
		}

		pSkillBase->SetSkillBase( pSkill );
		mSkillBaseTable.Add( pSkillBase, pSkill->wSkillIdx );
		AddSkill( pSkillBase );		
	}
}

void cSkillTreeManager::Init()
{
	/*	CMHFile file;
	file.Init("System/Resource/SkillTreeList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	WORD grade;
	WORD job;
	WORD pos;
	SKILL_BASE skill;

	skill.dwDBIdx = 0;
	skill.Level = 0;

#ifdef _TESTCLIENT_
	skill.Level = 1;
#endif

	cSkillBase* pSkillBase = NULL;
	cSkillTreeDlg* pSkillTreeDlg = GAMEIN->GetSkillTreeDlg();
	cJobSkillDlg* pJobSkillDlg = NULL;
	cIconGridDialog* pSkillDlg = NULL;

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		grade = file.GetWord() - 1;
		job = file.GetWord() - 1;
		pos = file.GetWord();
		skill.wSkillIdx = file.GetWord();


		pJobSkillDlg = ( cJobSkillDlg* )pSkillTreeDlg->GetTabSheet( ( BYTE )grade );
		pSkillDlg = ( cIconGridDialog* )pJobSkillDlg->GetTabSheet( ( BYTE )job );

		pSkillDlg->AddIcon( pos, ( cIcon * )pSkillBase );
		mSkillBaseTable.Add( pSkillBase, pSkillBase->GetSkillIdx() );
	}

	file.Release();
	*/
}

void cSkillTreeManager::Release()
{
	cSkillBase * pSkill = NULL;
	mSkillBaseTable.SetPositionHead();
	while(pSkill = mSkillBaseTable.GetData())
	{
		mSkillBaseTable.Remove( pSkill->GetSkillIdx() );
		mIconIndexCreator.ReleaseIndex( pSkill->GetID() );
		WINDOWMGR->AddListDestroyWindow( pSkill );
	}

	mSkillBaseTable.RemoveAll();
	mStoredSkill.clear();
	mStoredSetSkill.clear();
}

void cSkillTreeManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol) 
	{
	case MP_SKILLTREE_UPDATE_ACK:
		{
			MSG_SKILL_UPDATE* pmsg = ( MSG_SKILL_UPDATE* )pMsg;

			//SKILL_BASE skill;
			/*skill.wSkillIdx = pmsg->SkillBase.wSkillIdx / 100 * 100 + 1;
			skill.Level = pmsg->SkillBase.Level;
			skill.dwDBIdx = pmsg->SkillBase.dwDBIdx;*/

				const DWORD index		= pmsg->SkillBase.wSkillIdx;
			const BYTE	level		= pmsg->SkillBase.Level;
			//const DWORD	DBIndex		= pmsg->SkillBase.dwDBIdx;
			//cSkillBase* pSkillBase	= GetSkill( index );

			InitSkillInfo( &pmsg->SkillBase, 1 );

			//if( pSkillBase )
			//{
			//	// 세트 능력이 적용되어 있을 경우 세트 능력만 레벨을 상승시켜야하는 것이 아니다. 저장된 스킬 정보에도
			//	// 더해줘야한다. 게임 중에 언제라도 세트 능력이 제거될 수 있고, 이 때 이전 정보를 복구해야하기 때문이다.
			//	// 저장된 정보가 없을 때는 새로 생성해야한다.
			//	if( IsSetSkill( index ) )
			//	{
			//		SKILL_BASE* storedSkill = GetStoredSkill( index );

			//		if( storedSkill )
			//		{
			//			storedSkill->Level = level;
			//		}
			//		else
			//		{
			//			SKILL_BASE& skillBase = mStoredSkill[ index ];

			//			skillBase.dwDBIdx	= DBIndex;
			//			skillBase.Level		= level;
			//			skillBase.wSkillIdx	= index;
			//		}

			//		SKILL_BASE skillBase;

			//		skillBase.dwDBIdx	= 0;
			//		skillBase.Level		= BYTE( min( SKILLMGR->GetSkillSize( index ), DWORD( level + 1 ) ) );
			//		skillBase.wSkillIdx	= index;

			//		pSkillBase->SetSkillBase( &skillBase );
			//		SetToolTipIcon( pSkillBase );
			//	}
			//	else
			//	{
			//		SKILL_BASE skillBase;

			//		skillBase.dwDBIdx	= DBIndex;
			//		skillBase.Level		= level;
			//		skillBase.wSkillIdx	= index;

			//		pSkillBase->SetSkillBase( &skillBase );
			//		SetToolTipIcon( pSkillBase );
			//	}

			//	// 070613 LYW --- cSkillTreeManager : Add msg to chatting dlg.
			//	char szSkillInfo[128] = {0, } ;
			//	sprintf(szSkillInfo, "[%s] %d레벨을 습득하였습니다.", pSkillBase->m_pSkillInfo->GetName(), pSkillBase->m_pSkillInfo->GetLevel()) ;
			//	CHATMGR->AddMsg(CTC_TIPS, szSkillInfo) ;
			//}
			//else
			//{
			//	SKILL_BASE skillBase;
			//	skillBase.dwDBIdx	= DBIndex;
			//	skillBase.Level		= level;
			//	skillBase.wSkillIdx	= index;

			//	pSkillBase = NewSkillIcon( &skillBase );

			//	if( pSkillBase )
			//	{
			//		pSkillBase->SetSkillBase( &skillBase );
			//		mSkillBaseTable.Add( pSkillBase, pSkillBase->GetSkillIdx() );
			//		AddSkill( pSkillBase );

			//		// 070613 LYW --- cSkillTreeManager : Add msg to chatting dlg.
			//		char szSkillInfo[128] = {0, } ;
			//		sprintf(szSkillInfo, "[%s] %d레벨을 습득하였습니다.", pSkillBase->m_pSkillInfo->GetName(), pSkillBase->m_pSkillInfo->GetLevel()) ;
			//		CHATMGR->AddMsg(CTC_TIPS, szSkillInfo) ;
			//	}
			//}

			//if( ( SKILLMGR->GetSkillInfo( index ) )->GetKind() == SKILLKIND_PASSIVE )
			//{
			//	if( level > 1 )
			//	{
			//		SKILLMGR->RemovePassive( index + pSkillBase->GetLevel() - 2 );
			//	}

			//	SKILLMGR->AddPassive( index + pSkillBase->GetLevel() - 1 );
			//}

			cActiveSkillInfo* pInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( index + level - 1 );

			if( pInfo )
			{
				DWORD point = HERO->GetHeroTotalInfo()->SkillPoint -= pInfo->GetSkillInfo()->TrainPoint;
				HERO->GetHeroTotalInfo()->SkillPoint = 0;
				HERO->SetSkillPoint( point );
			}

			// 070510 LYW --- cSkillTreeManager : Update skill info.
			{
				cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );

				dialog->SetDisable( FALSE );
				dialog->OpenDialog();
			}

			QUICKMGR->RefreshSkillItem( index );

			//---KES 장비 방어력 적용 여부 071128
			if( GAMEIN->GetInventoryDialog() )
				GAMEIN->GetInventoryDialog()->RefreshArmorItem();
			//-----------------------------------

		}
		break;
	case MP_SKILLTREE_UPDATE_NACK:
		{
			// 070613 LYW --- cSkillTreeManager : Add msg to chatting dlg.
			char szSkillInfo[128] = {0, } ;
			sprintf(szSkillInfo, CHATMGR->GetChatMsg( 1444 )) ;
			CHATMGR->AddMsg(CTC_TIPS, szSkillInfo) ;

			{
				cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );

				dialog->SetDisable( FALSE );
			}
		}
		break;
	case MP_SKILLTREE_POINT_NOTIFY:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			HERO->SetSkillPoint( pmsg->dwData );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1164 ), pmsg->dwData );
		}
		break;

	// 071130 LYW --- cSkillTreeManager : 스킬 초기화 처리를 한다.
	case MP_SKILLTREE_RESET_SKILL_ACK :
		{
			ResetSkillTree( pMsg ) ;
		}
		break ;
	}

}

// 071130 LYW --- cSkillTreeManager : 스킬 초기화 함수 추가.
void cSkillTreeManager::ResetSkillTree( void* pMsg )
{
	if( !pMsg ) return ;

	SEND_SKILL_RESETINFO* pmsg = NULL ;
	pmsg = (SEND_SKILL_RESETINFO*)pMsg ;

	if( !pmsg ) return ;

	if( !HERO ) return ;

	RemoveAllSkill() ;

	CAddableInfoIterator iter(&pmsg->AddableInfo) ;

	SKILL_BASE SkillTreeInfo[ 1024 ] ;
	iter.GetInfoData( SkillTreeInfo ) ;

	ASSERT( 1024 > pmsg->SkillNum ) ;

	InitSkillInfo( SkillTreeInfo, pmsg->SkillNum ) ;

	HERO->GetHeroTotalInfo()->SkillPoint = 0;
	HERO->SetSkillPoint(pmsg->nSkillPoint) ;

	//CHATMGR->AddMsg(CTC_SYSMSG, "스킬 정보가 초기화되었습니다.") ;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1274 ) );

	cSkillTrainingDlg* dialog = ( cSkillTrainingDlg* )WINDOWMGR->GetWindowForID( SKILL_TRAINING );
	dialog->OpenDialog() ;

	QUICKMGR->Quick_Remove_AllSkill() ;

	CHARCALCMGR->Initialize( HERO );
}



void cSkillTreeManager::SetToolTipIcon(cSkillBase *pSkill)
{
	cSkillInfo* pCommSkill = pSkill->m_pSkillInfo;

	if( ! pCommSkill )
	{
		return;
	}

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 0, &imgToolTip, PFT_HARDPATH );
	pSkill->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	char line[64];

	wsprintf(line, "[%s]", pCommSkill->GetName() );

	pSkill->AddToolTipLine( line, TTTC_SKILLNAME );
	pSkill->AddToolTipLine( "" );
	wsprintf(line, "%s%d", CHATMGR->GetChatMsg( 769 ), pSkill->GetSkillBase()->Level );

	pSkill->AddToolTipLine( line, TTTC_SKILLLEVEL );

	if( pCommSkill->GetKind() != SKILLKIND_PASSIVE )
	{
		const ACTIVE_SKILL_INFO* info = ( ( cActiveSkillInfo* )pCommSkill )->GetSkillInfo();

		if( ! info )
		{
			return;
		}
		// 080616 LUJ, 상대적으로 소요되는 마나 양 표시
		else if( info->mMana.mPercent )
		{
			sprintf(
				line,
				"%s%0.0f%%",
				CHATMGR->GetChatMsg( 768 ),
				info->mMana.mPercent * 100.0f );
		}
		// 080616 LUJ, 절대적으로 표시되는 마나 양 표시
		else if( info->mMana.mPlus )
		{
			sprintf(
				line,
				"%s%0.0f",
				CHATMGR->GetChatMsg( 768 ),
				info->mMana.mPlus );
		}

		pSkill->AddToolTipLine( line, TTTC_SKILLLEVEL );

		const float cool = info->CoolTime / 1000.f;		//KES 070530 : 1000 --> 1000.f ??	//ncf

		sprintf(line, CHATMGR->GetChatMsg( 776 ), cool);

		pSkill->AddToolTipLine( line, TTTC_SKILLLEVEL );
	}
	ITEMMGR->AddToolTip( pSkill, pCommSkill->GetTooltip() );
}


cSkillBase* cSkillTreeManager::NewSkillIcon( SKILL_BASE* skillBase)
{
	cImage skillHighImage;

	{
		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( skillBase->wSkillIdx );

		if( pSkillInfo &&
			pSkillInfo->GetImage() != -1 )
		{
			SCRIPTMGR->GetImage( pSkillInfo->GetImage(), &skillHighImage, PFT_SKILLPATH );
		}
	}

	cImage skillLowImage;
	cSkillBase* icon = new cSkillBase;

	icon->Init(0,0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, &skillLowImage, &skillHighImage, mIconIndexCreator.GenerateIndex());
	icon->SetData(skillBase->wSkillIdx);
	icon->SetSkillBase(skillBase);

	SetToolTipIcon(icon);

	WINDOWMGR->AddWindow(icon);
	return icon;
}


cSkillBase* cSkillTreeManager::GetSkill(DWORD SkillIdx)
{
	return mSkillBaseTable.GetData( SkillIdx );;
}


void cSkillTreeManager::RefreshSkill()
{}


void cSkillTreeManager::AddSkill(cSkillBase* pSkill)
{	
	const DWORD skillIndex = pSkill->GetSkillBase()->wSkillIdx;
	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( skillIndex );

	if( ! pSkillInfo )
	{
		return;
	}

	cTabDialog*	skillIconDialog;

	{
		cTabDialog* skillTreeDialog	= ( cTabDialog* )WINDOWMGR->GetWindowForID( STD_SKILLTREEDLG );

		if( pSkillInfo->GetKind() == SKILLKIND_PASSIVE )
		{
			skillIconDialog = ( cTabDialog* )skillTreeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Passive );
		}
		else
		{
			skillIconDialog = ( cTabDialog* )skillTreeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Active );
		}
	}	

	for( BYTE sheet = 0; sheet < skillIconDialog->GetTabNum(); sheet++ )
	{
		cIconGridDialog* iconDialog = ( cIconGridDialog* )skillIconDialog->GetTabSheet( sheet );

		BOOL isAdded = FALSE;

		for(WORD pos = 0; pos < iconDialog->GetCellNum(); ++pos )
		{
			cIcon* icon = iconDialog->GetIconForIdx( pos );

			if( ! icon )
			{
				iconDialog->AddIcon( pos, pSkill );

				isAdded = TRUE;
				break;
			}

			cSkillBase* skillIcon = ( cSkillBase* )icon;

			if( skillIcon->GetType()		== WT_SKILL && 
				skillIcon->GetSkillIdx()	== skillIndex )
			{
				// 아이콘이 있으면 일단 제거하고 삽입해야 한다.
				iconDialog->DeleteIcon( skillIcon );
				WINDOWMGR->AddListDestroyWindow( skillIcon );
				iconDialog->AddIcon( pos, pSkill );

				mSkillBaseTable.Remove( skillIcon->GetSkillIdx() );

				NewSkillIcon( pSkill->GetSkillBase() );

				isAdded = TRUE;
				break;
			}
		}

		if( isAdded )
		{
			if( ( SKILLMGR->GetSkillInfo( skillIndex ) )->GetKind() == SKILLKIND_PASSIVE )
			{
				SKILLMGR->AddPassive( skillIndex - 1 + pSkill->GetLevel() );
			}

			return;
		}
	}	
}


void cSkillTreeManager::RemoveSkill( cSkillBase* skill )
{
	ASSERT( skill );

	const DWORD	index = skill->GetSkillIdx();
	cTabDialog*	dialog;

	{
		mSkillBaseTable.Remove( index );
	}

	{
		cTabDialog*	treeDialog	= ( cTabDialog* )WINDOWMGR->GetWindowForID( STD_SKILLTREEDLG );
		cSkillInfo*	skillInfo	= SKILLMGR->GetSkillInfo( index );

		if( SKILLKIND_PASSIVE == skillInfo->GetKind() )
		{
			dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Passive );
		}
		else
		{
			dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Active );
		}
	}

	for( BYTE i = 0; i < dialog->GetTabNum(); ++i )
	{
		cIconGridDialog* iconDialog = ( cIconGridDialog* )dialog->GetTabSheet( i );
		ASSERT( iconDialog );

		for( WORD position = 0; position < iconDialog->GetCellNum(); ++position )
		{
			if( iconDialog->IsAddable( position ) )
			{
				continue;
			}

			cIcon* icon = iconDialog->GetIconForIdx( position );

			if( !	icon				||
				WT_SKILL != icon->GetType() )
			{
				continue;
			}

			cSkillBase* skillIcon = ( cSkillBase* )icon;

			if( index != skillIcon->GetSkillIdx() )
			{
				continue;
			}

			if( ( SKILLMGR->GetSkillInfo( index ) )->GetKind() == SKILLKIND_PASSIVE )
			{
				SKILLMGR->RemovePassive( index - 1 + skillIcon->GetLevel() );
			}

			iconDialog->DeleteIcon( skillIcon );
			WINDOWMGR->AddListDestroyWindow( skillIcon );
		}
	}
}


void cSkillTreeManager::AddSetSkill( DWORD index, BYTE level )
{
	/*
	주의:	패시브 스킬은 아이템 착용/제거때 서버에서도 자동으로 적용/제거할 것이다. 
		세트 아이템으로 인한 패시브 스킬 적용에 관해 서버는 어떠한 메시지도 보내지 않음을 유의
	*/

	if( ! SKILLMGR->GetSkillInfo( index ) )
	{
		ASSERT( 0 && "It's invalid skill index" );
		return;
	}

	// 세트로 추가될 스킬을 이미 정적(DB index 보유 여부로 구분)으로 보유하고 있으면 저장한다.

	cSkillBase* skill = GetSkill( index );

	if( skill )
	{
		const SKILL_BASE* skillBase = skill->GetSkillBase();

		// 세트 스킬이 아니면 초기 정보로서 보관하고, 세트 스킬이면 별도의 자료구조에 보관하여 복구하는데 사용한다.
		if( skillBase->dwDBIdx )
		{
			// 두번 저장되면 어떤 정보를 복구해야할지 구분할 수 없게 된다. 이런 문제가 발생하면 즉시 해결해야 한다.
			ASSERT( mStoredSkill.end() == mStoredSkill.find( index ) );

			mStoredSkill[ index ] = *skillBase;
		}

		mStoredSetSkill[ index ].push( level );

		level = level + skill->GetLevel();

		RemoveSkill( skill );
	}

	// 세트 스킬 추가: 주의: 레벨은 음수일수도 양수일수도 있다
	if( 0 != level )
	{
		const short maxLevel = short( SKILLMGR->GetSkillSize( index ) );

		SKILL_BASE base;
		base.dwDBIdx	= 0;
		base.wSkillIdx	= index;
		base.Level		= BYTE( min( maxLevel, max( 0, level ) ) );

		InitSkillInfo( &base, 1 );
	}
}


void cSkillTreeManager::RemoveSetSkill( DWORD index )
{
	BYTE previousSkillLevel = 0;

	// 스킬 창에서도 제거하자
	{
		cTabDialog*	dialog;

		// 두가지 스킬 창 중 선택하자
		{
			cTabDialog*	treeDialog	= ( cTabDialog* )WINDOWMGR->GetWindowForID( STD_SKILLTREEDLG );
			cSkillInfo*	skillInfo	= SKILLMGR->GetSkillInfo( index );

			if( SKILLKIND_PASSIVE == skillInfo->GetKind() )
			{
				dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Passive );
			}
			else
			{
				dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Active );
			}
		}

		for( BYTE i = 0; i < dialog->GetTabNum(); ++i )
		{
			cIconGridDialog* iconDialog = ( cIconGridDialog* )dialog->GetTabSheet( i );
			ASSERT( iconDialog );

			for( WORD position = 0; position < iconDialog->GetCellNum(); ++position )
			{
				cIcon* icon = iconDialog->GetIconForIdx( position );

				if( !	icon ||
					WT_SKILL != icon->GetType() )
				{
					continue;
				}

				cSkillBase* skillIcon = ( cSkillBase* )icon;

				// 080417 LUJ, 세트 스킬(DB 인덱스가 0인 스킬)만 제거하도록 체크 추가
				if( skillIcon->GetSkillIdx() != index	||
					skillIcon->GetDBIdx() )
				{
					continue;
				}

				previousSkillLevel = skillIcon->GetLevel();

				const DWORD skillIndex	= index - 1 + skillIcon->GetLevel();
				cSkillInfo* info		= SKILLMGR->GetSkillInfo( skillIndex );

				if( SKILLKIND_PASSIVE == info->GetKind() )
				{
					SKILLMGR->RemovePassive( skillIndex );
				}

				iconDialog->DeleteIcon( skillIcon );
				mSkillBaseTable.Remove( index );

				WINDOWMGR->AddListDestroyWindow( skillIcon );
			}
		}
	}

	// 저장된 스킬 복구 처리
	{
		SKILL_BASE* storedSkill = GetStoredSkill( index );

		// 더 이상 저장된 세트 스킬이 없고 원래 스킬이 있으면 복구하자
		if( mStoredSetSkill.end() == mStoredSetSkill.find( index ) )
		{
			if( storedSkill )
			{
				InitSkillInfo( storedSkill, 1 );

				mStoredSkill.erase( index );
			}
		}
		// 세트 스킬이 있을 경우, 저장된 스킬이 있는지 고려하여 복구해야한다.
		else
		{
			SetSkill& setSkill = mStoredSetSkill[ index ];
			ASSERT( setSkill.size() );

			const BYTE level = setSkill.top();

			SKILL_BASE skill;
			skill.dwDBIdx	= 0;
			skill.Level		= max( 1, previousSkillLevel - level );
			skill.wSkillIdx	= index;

			InitSkillInfo( &skill, 1 );

			setSkill.pop();

			if( setSkill.empty() )
			{
				mStoredSetSkill.erase( index );
			}
		}
	}	
}


void cSkillTreeManager::SetStoredSkill( const SKILL_BASE* skill )
{
	ASSERT( mStoredSkill.end() == mStoredSkill.find( skill->wSkillIdx ) );

	mStoredSkill[ skill->wSkillIdx ]	= *skill;
}


SKILL_BASE* cSkillTreeManager::GetStoredSkill( DWORD index )
{
	StoredSkill::iterator it = mStoredSkill.find( index );

	return mStoredSkill.end() == it ? 0 : &( it->second );
}


BOOL cSkillTreeManager::IsSetSkill( DWORD index )
{
	// DB 인덱스가 없으면 세트 아이템으로 인해 부여된 스킬
	cSkillBase* skill = GetSkill( index );

	return skill && ! skill->GetDBIdx();
}


void cSkillTreeManager::ResetSetSkill()
{
	mStoredSkill.clear();
	mStoredSetSkill.clear();
}


cSkillTreeManager* cSkillTreeManager::GetInstance()
{
	static cSkillTreeManager instance;

	return &instance;
}


const SKILL_BASE* cSkillTreeManager::GetSkillBase( DWORD skillIndex )
{
	cSkillBase* skill = GetSkill( skillIndex );

	return skill ? skill->GetSkillBase() : 0;
}

// 071129 LYW --- cSkillTreeManager : 모든 스킬을 지우는 함수 추가.
void cSkillTreeManager::RemoveAllSkill()
{
	BYTE previousSkillLevel = 0;

	cTabDialog*	dialog;
	cTabDialog*	treeDialog	= ( cTabDialog* )WINDOWMGR->GetWindowForID( STD_SKILLTREEDLG );

	dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Passive );

	for( BYTE i = 0; i < dialog->GetTabNum(); ++i )
	{
		cIconGridDialog* iconDialog = ( cIconGridDialog* )dialog->GetTabSheet( i );
		ASSERT( iconDialog );

		for( WORD position = 0; position < iconDialog->GetCellNum(); ++position )
		{
			cIcon* icon = iconDialog->GetIconForIdx( position );

			if( !	icon ||
				WT_SKILL != icon->GetType() )
			{
				continue;
			}

			cSkillBase* skillIcon = ( cSkillBase* )icon;

			previousSkillLevel = skillIcon->GetLevel();

			const DWORD skillIndex	= icon->GetData() - 1 + skillIcon->GetLevel();
			cSkillInfo* info		= SKILLMGR->GetSkillInfo( skillIndex );

			if( SKILLKIND_PASSIVE == info->GetKind() )
			{
				SKILLMGR->RemovePassive( skillIndex );
			}

			iconDialog->DeleteIcon( skillIcon );
			mSkillBaseTable.Remove( icon->GetData() );

			WINDOWMGR->AddListDestroyWindow( skillIcon );
		}
	}

	dialog = ( cTabDialog* )treeDialog->GetTabSheet( ( BYTE )eSkillDlgTab_Active );

	for( BYTE i = 0; i < dialog->GetTabNum(); ++i )
	{
		cIconGridDialog* iconDialog = ( cIconGridDialog* )dialog->GetTabSheet( i );
		ASSERT( iconDialog );

		for( WORD position = 0; position < iconDialog->GetCellNum(); ++position )
		{
			cIcon* icon = iconDialog->GetIconForIdx( position );

			if( !	icon ||
				WT_SKILL != icon->GetType() )
			{
				continue;
			}

			cSkillBase* skillIcon = ( cSkillBase* )icon;

			previousSkillLevel = skillIcon->GetLevel();

			const DWORD skillIndex	= icon->GetData() - 1 + skillIcon->GetLevel();
			cSkillInfo* info		= SKILLMGR->GetSkillInfo( skillIndex );

			if( SKILLKIND_PASSIVE == info->GetKind() )
			{
				SKILLMGR->RemovePassive( skillIndex );
			}

			iconDialog->DeleteIcon( skillIcon );
			mSkillBaseTable.Remove( icon->GetData());

			WINDOWMGR->AddListDestroyWindow( skillIcon );
		}
	}
}

void cSkillTreeManager::SetPositionHead()
{
	mSkillBaseTable.SetPositionHead();
}

SKILL_BASE* cSkillTreeManager::GetSkillBase()
{
	cSkillBase* pSkillbase = mSkillBaseTable.GetData();

	if( pSkillbase )
		return pSkillbase->GetSkillBase();
	else
		return NULL;
}