#include "stdafx.h"
#include "CharacterCalcManager.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameResourceManager.h"

// 080310 LUJ, 쓰지 않는 헤더 파일 주석 처리
//#include "Audio/MHAudioManager.h"

#include "CharacterDialog.h"
#include "GameEventManager.h"
#include "ObjectStateManager.h"
#include "ObjectActionManager.h"

#include "ExitManager.h"

#include "GameEventManager.h"
#include "QuestManager.h"
#include "JournalManager.h"
#include "QuickManager.h"
#include "cSkillTreeManager.h"

// 080310 LUJ, 쓰지 않는 헤더 파일 주석 처리
//#include "SkillPointRedist.h"
//#include "AppearanceManager.h"

#include "ItemManager.h"
#include "MussangManager.h"

// 070416 LYW --- CharacterCalcManager : Include header files to need.
#include "WindowIDEnum.h"

#include "Interface/cResourceManager.h"
#include "Interface/cWindowManager.h"

#include "MixDialog.h"
#include "FamilyDialog.h"
//#include "npcimagedlg.h"
#include "CommonCalcFunc.h"

// 080310 LUJ, 플레이어 레벨 변경에 따라 스킬 학습 창을 업데이트하기 위해 불러온다
#include "cSkillTrainingDlg.h"

// 080502 LYW --- CharacterCalcManager : 튜토리얼 헤더 포함.
#include "./TutorialManager.h"


GLOBALTON(CCharacterCalcManager)
CCharacterCalcManager::CCharacterCalcManager()
{}


CCharacterCalcManager::~CCharacterCalcManager()
{}


void CCharacterCalcManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)  
	{
	case MP_CHAR_LIFE_ACK:							Char_Life_Ack( pMsg ) ;					break;
	case MP_CHAR_LIFE_NACK:							ASSERT(0);								break;
	case MP_CHAR_MANA_ACK:							Char_Mana_Ack( pMsg ) ;					break;
	case MP_CHAR_MANA_NACK:							ASSERT(0);								break;
	case MP_CHAR_EXPPOINT_ACK:						Char_Exppoint_Ack( pMsg ) ;				break;
	case MP_CHAR_EXPPOINT_NACK:						ASSERT(0);								break;		
	case MP_CHAR_MAXLIFE_NOTIFY:					Char_MaxLife_Notify( pMsg ) ;			break;
	case MP_CHAR_MAXMANA_NOTIFY:					Char_MaxMana_Notify( pMsg ) ;			break;
	case MP_CHAR_PLAYERLEVELUP_NOTIFY:				Char_PlayerLevelup_Notify( pMsg ) ;		break;
	case MP_CHAR_LEVEL_NOTIFY:						Char_Level_Notify( pMsg ) ;				break;
	case MP_CHAR_LEVELUPPOINT_NOTIFY:				Char_LevelupPoint_Notify( pMsg ) ;		break;
	case MP_CHAR_STR_NOTIFY:						Char_Str_Notify( pMsg ) ;				break;
	case MP_CHAR_DEX_NOTIFY:						Char_Dex_Notify( pMsg ) ;				break;
	case MP_CHAR_VIT_NOTIFY:						Char_Vit_Notify( pMsg ) ;				break;
	case MP_CHAR_WIS_NOTIFY:						Char_Wis_Notify( pMsg ) ;				break;
	// 070411 LYW --- CharacterCalcManager : Add network message of Intelligence part.
	case MP_CHAR_INT_NOTIFY :						Char_Int_Notify( pMsg ) ;				break ;
	case MP_CHAR_FAME_NOTIFY:						Char_Fame_Notify( pMsg ) ;				break;
	case MP_CHAR_BADFAME_NOTIFY:					Char_BadFame_Notify( pMsg ) ;			break;
	case MP_CHAR_BADFAME_ACK:						Char_BadFame_Ack( pMsg ) ;				break;
	case MP_CHAR_BADFAME_NACK:						Char_BadFame_Nack( pMsg ) ;				break;
	case MP_CHAR_BADFAME_CHANGED:					Char_BadFame_Changed( pMsg ) ;			break;
	case MP_CHAR_PLAYTIME_ACK:						Char_PlayTime_Ack( pMsg ) ;				break;
	case MP_CHAR_PLAYTIME_NACK:						Char_PlayTime_Nack( pMsg ) ;			break;
	case MP_CHAR_EXITSTART_ACK:						Char_ExitStart_Ack( pMsg ) ;			break;
	case MP_CHAR_EXITSTART_NACK:					Char_ExitStart_Nack( pMsg ) ;			break;
	case MP_CHAR_EXIT_ACK:							EXITMGR->Exit();						break;
	case MP_CHAR_EXIT_NACK:							Char_Exit_Nack( pMsg ) ;				break;
	case MP_CHAR_YOUAREDIED:						Char_YouAreDied( pMsg ) ;				break;
	case MP_CHAR_POINTMINUS_ACK:					Char_PointNinus_Ack( pMsg ) ;			break;
	case MP_CHAR_POINTMINUS_NACK:					Char_PointMinus_Nack( pMsg ) ;			break;
		//SW051112 무쌍모드
	case MP_CHAR_MUSSANG_INFO:						Char_Mussang_Info( pMsg ) ;				break;
	case MP_CHAR_MUSSANG_ACK:						Char_Mussang_Ack( pMsg ) ;				break;
	case MP_CHAR_MUSSANG_NACK:						MUSSANGMGR->SetValidMsg(TRUE);			break;
	case MP_CHAR_MUSSANG_END:						Char_Mussang_End( pMsg ) ;				break;
	// 070412 LYW --- CharacterCalcManager : Add network message for change stage of character.
	case MP_CHAR_STAGE_NOTIFY :						Char_Stage_Notify( pMsg ) ;				break ;

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.05.23
	case MP_CHAR_MONSTERMETER_KILLMONSTER:			Char_Monstermeter_KillMonster( pMsg );	break;
	case MP_CHAR_MONSTERMETER_PLAYTIME:				Char_Monstermeter_PlayTime( pMsg );		break;
	// E 몬스터미터 추가 added by hseos 2007.05.23

	case MP_CHAR_HIDE_NOTIFY:						Char_Hide_Notify( pMsg );				break;
	case MP_CHAR_DETECT_NOTIFY:						Char_Detect_Notify( pMsg );				break;
	// 080616 LUJ, 플레이어의 현재 HP정보를 설정한다
	case MP_CHAR_LIFE_GET_ACK:
		{
			const MSG_INT*	message = ( MSG_INT* )pMsg;
			CHero*			hero	= OBJECTMGR->GetHero();

			if( ! hero )
			{
				break;
			}

			hero->SetLife( message->nData );
			break;
		}
	}
}


void CCharacterCalcManager::Char_Life_Ack( void* pMsg )
{
	MSG_INT* msg = (MSG_INT*)pMsg;
	if(HERO)
	{
		HERO->ChangeLife(msg->nData);
	}
}


void CCharacterCalcManager::Char_Mana_Ack( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetMana(msg->dwData);
	}		
}


void CCharacterCalcManager::Char_Exppoint_Ack( void* pMsg )
{
	// 070419 LYW --- CharacterCalcManager : Modified function to setting exp point.
	/*
	MSG_EXPPOINT * msg = (MSG_EXPPOINT *)pMsg;	

	if(!HERO) return;
	if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE)
	{
		int diff = msg->ExpPoint - HERO->GetExpPoint();
		if(diff<0)
		{
			diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint() + msg->ExpPoint;
			HERO->SetExpPoint(0);
		}
		HERO->SetExpPoint(msg->ExpPoint);

		// 070418 LYW --- CharacterCalcManager : Delete this line.
		if( diff > 0 )CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 120 ), diff );
	}
	else if(msg->ExpKind == MSG_EXPPOINT::EXPPOINTKIND_DIE)
	{
		int diff = HERO->GetExpPoint() - msg->ExpPoint;
		if(diff == 0)
			return;
		
		if(diff<0)	// 렙다시에...
		{
			diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - msg->ExpPoint + HERO->GetExpPoint();
		}

		HERO->SetExpPoint(msg->ExpPoint);

		// 070129 LYW --- Revival : Modified message number.
		//CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 134 ), diff );
		CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 205 ), diff );
	}
	*/

	MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg ;

	HERO->SetExpPoint(pmsg->dwData) ;
}


void CCharacterCalcManager::Char_MaxLife_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetMaxLife(msg->dwData);
	}
}


void CCharacterCalcManager::Char_MaxMana_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetMaxMana(msg->dwData);
	}
}


void CCharacterCalcManager::Char_PlayerLevelup_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	CPlayer * pPlayer = (CPlayer *)OBJECTMGR->GetObject(msg->dwData);
	if(!pPlayer) return;
	EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,pPlayer,NULL,0,0);
}


void CCharacterCalcManager::Char_Level_Notify( void* pMsg )
{
	// desc_hseos_캐릭터레벨_01
	// S 캐릭터레벨 추가 added by hseos 2007.10.29
	LEVELTYPE nOldLevel = 0;
	// E 캐릭터레벨 추가 added by hseos 2007.10.29

	MSG_LEVEL * msg = (MSG_LEVEL*)pMsg;
	// 레벨 셋팅 msg->dwData1
	if(HERO)
	{
		// desc_hseos_캐릭터레벨_01
		// S 캐릭터레벨 추가 added by hseos 2007.10.29
		nOldLevel = HERO->GetLevel();
		// E 캐릭터레벨 추가 added by hseos 2007.10.29

		// 080530 LYW --- CharacterCalcManager : Modified a data type int to EXPTYPE.
		//int diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint();
		EXPTYPE diff = 0 ;
		diff = GAMERESRCMNGR->GetMaxExpPoint(HERO->GetLevel()) - HERO->GetExpPoint();

		diff += msg->CurExpPoint;				
		
		// 070419 LYW --- CharacterCalcManager : Delete notice exp.
		/*
		if(msg->Level > HERO->GetLevel())
			CHATMGR->AddMsg( CTC_GETEXP, CHATMGR->GetChatMsg( 120 ), diff );
			*/

		HERO->SetLevel(msg->Level);
		HERO->SetMaxExpPoint(msg->MaxExpPoint);
		HERO->SetExpPoint(msg->CurExpPoint);
		if( msg->Level > HERO->GetMaxLevel() )
			HERO->SetMaxLevel( msg->Level );
		GAMEEVENTMGR->AddEvent( eGameEvent_LevelUp, HERO->GetLevel() );
		QUESTMGR->ProcessNpcMark();
	}

	char noticeBuf[128] = {0, } ;
	sprintf(noticeBuf, CHATMGR->GetChatMsg(822), HERO->GetObjectName(), msg->Level ) ;
	// desc_hseos_캐릭터레벨_01
	// S 캐릭터레벨 추가 added by hseos 2007.10.29
	// ..레벨업일 때만 여기서 메세지를 추가하고 다운일 때는 각 상황에 따라서 따로..
	// CHATMGR->AddMsg(CTC_SYSMSG, noticeBuf) ;
	if (nOldLevel < HERO->GetLevel())
		CHATMGR->AddMsg(CTC_SYSMSG, noticeBuf) ;
	// E 캐릭터레벨 추가 added by hseos 2007.10.29

	CFamilyDialog* pDlg = GAMEIN->GetFamilyDlg() ;

	if( pDlg )
	{
		pDlg->UpdateAllInfo() ;
	}

	GAMEIN->GetCharacterDialog()->RefreshInfo();

	// 조합은 히어로의 소지금/레벨에 영향받는데, 현재 상태에 기반해서 표시해주는 항목이 있으므로 정보가 변경될때마다 갱신이 필요하다
	{
		CMixDialog* dialog = ( CMixDialog* )WINDOWMGR->GetWindowForID( ITMD_MIXDLG );
		ASSERT( dialog );

		dialog->Refresh();
	}

	// 080310 LUJ, 스킬 창이 열려있는 경우 학습 가능 스킬 정보를 갱신한다
	{
		cSkillTrainingDlg* dialog = GAMEIN->GetSkillTrainingDlg();

		if( dialog &&
			dialog->IsActive() )
		{
			dialog->OpenDialog();
			dialog->SetSkillInfo();
		}
	}

	//// 080502 LYW --- GameIn : 튜토리얼 중, 레벨 업 시 실행 되는 튜토리얼 체크.
	//if(!TUTORIALMGR->IsCompleteTutorial(e_TT_LevelUp))
	//{
	//	TUTORIALMGR->Start_Specification_Tutorial(e_TT_LevelUp) ;
	//}
}


void CCharacterCalcManager::Char_LevelupPoint_Notify( void* pMsg )
{
	MSG_WORD * msg = (MSG_WORD *)pMsg;
	// 레벨 셋팅 msg->dwData1
	if(HERO)
	{
		if(msg->wData != 0)
		{				
			GAMEIN->GetCharacterDialog()->SetPointLeveling(TRUE, msg->wData);		// 포인트 배분
		}
		else
 		{
 			GAMEIN->GetCharacterDialog()->SetPointLevelingHide();
 		}
		// 080506 LYW --- CharacterCalcManager : 레벨업과 관련 된 tutorial을 체크하는 함수 호출.
		TUTORIALMGR->Check_LevelUp() ;
	}
}


void CCharacterCalcManager::Char_Str_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetStrength((WORD)msg->dwData);
	}
}


void CCharacterCalcManager::Char_Dex_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetDexterity((WORD)msg->dwData);
	}
}


void CCharacterCalcManager::Char_Vit_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetVitality((WORD)msg->dwData);
	}
}


void CCharacterCalcManager::Char_Wis_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetWisdom((WORD)msg->dwData);
	}
}

// 070411 LYW --- CharacterCalcManager : Add function to setting intelligence from notify.
void CCharacterCalcManager::Char_Int_Notify( void* pMsg )
{
	MSG_DWORD * msg = (MSG_DWORD *)pMsg;
	if(HERO)
	{
		HERO->SetIntelligence((WORD)msg->dwData);
	}
}


void CCharacterCalcManager::Char_Fame_Notify( void* pMsg )
{
	MSG_FAME * pmsg = (MSG_FAME*)pMsg;
	if(HERO)
	{
		HERO->SetFame(pmsg->Fame);
	}
}


void CCharacterCalcManager::Char_BadFame_Notify( void* pMsg )
{
	MSG_FAME * pmsg = (MSG_FAME*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
	if(pPlayer)
	{
		pPlayer->SetBadFame(pmsg->Fame);
	}
}


void CCharacterCalcManager::Char_BadFame_Ack( void* pMsg )
{
	MSG_FAME* pmsg = (MSG_FAME*)pMsg;

	// 채팅창에 메세지 추가
	char badfame[16] = { 0, };
	char money[16] = { 0, };
	strcpy(badfame, AddComma(pmsg->Fame));
	strcpy(money, AddComma(pmsg->Fame*BAIL_PRICE));
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(646), money, badfame);
}


void CCharacterCalcManager::Char_BadFame_Nack( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	// 악명치를 낮추는데 실패하였습니다
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(647));
}


void CCharacterCalcManager::Char_BadFame_Changed( void* pMsg )
{
	MSG_FAME* pmsg = (MSG_FAME*)pMsg;

	if(HERO)
	{
		HERO->SetFame(pmsg->Fame);
	}
}


void CCharacterCalcManager::Char_PlayTime_Ack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if(HERO)
	{

	}
}


void CCharacterCalcManager::Char_PlayTime_Nack( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if(HERO)
	{
		HERO->SetPlayTime(pmsg->dwData);
	}
}


void CCharacterCalcManager::Char_ExitStart_Ack( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	//종료카운트다운시작
	EXITMGR->StartExit();		
}


void CCharacterCalcManager::Char_ExitStart_Nack( void* pMsg )
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
	EXITMGR->RejectExit( pmsg->bData );
}


void CCharacterCalcManager::Char_Exit_Nack( void* pMsg )
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;			
	EXITMGR->CancelExit( pmsg->bData );
}


void CCharacterCalcManager::Char_YouAreDied( void* pMsg )
{
	if( HERO->GetState() != eObjectState_Die )
	{
		ASSERT(HERO->IsDied());

		if(gCurTime - HERO->GetDiedTime() < 10000)
			return;

		OBJECTACTIONMGR->Die(HERO,HERO,FALSE,FALSE,FALSE);
		// 070619 LYW --- ChacterCalcManager : Deactive SkillPointRedist.
		// 현재 사용하지 않아서 비활성화.
		//GAMEIN->GetSkPointDlg()->SetActive( FALSE );
	}
}


void CCharacterCalcManager::Char_PointNinus_Ack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	if( pmsg->wData )
	{
		--HERO->GetShopItemStats()->StatePoint;
	}
	else
	{
		HERO->GetShopItemStats()->StatePoint=0;
	}
}


void CCharacterCalcManager::Char_PointMinus_Nack( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(756) );
}


void CCharacterCalcManager::Char_Mussang_Info( void* pMsg )
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;

	if( !HERO ) return;

	if( pmsg->bData & eMSS_LevelUp )
	{
		MUSSANGMGR->SetMussangMaxPoint(pmsg->dwData);
		return ;
	}
		
	MUSSANGMGR->SetMussangPoint(pmsg->dwData);
	
	if( pmsg->bData & eMSS_Ready )
	{
		if( !MUSSANGMGR->IsMussangReady() )
		{
			MUSSANGMGR->SetMussangReady(TRUE);	//게이지 깜박임과 버튼 활성화 
			MUSSANGMGR->SetValidMsg(TRUE);
		}
	}
}


void CCharacterCalcManager::Char_Mussang_Ack( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if(!pPlayer) return;
		
	if( HERO == pPlayer )
	{
		MUSSANGMGR->SetMussangReady(FALSE);
		MUSSANGMGR->SetMussangMode(TRUE);	// 내부 상태 아이콘 표시/해제
	}

	if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE))
	{
		OBJECTEFFECTDESC desc(1035);
		pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
	}
}


void CCharacterCalcManager::Char_Mussang_End( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if(!pPlayer) return;
	pPlayer->RemoveObjectEffect( MUSSANG_EFFECT_ID1 );

	if( HERO == pPlayer )
	{
		MUSSANGMGR->SetMussangMode(FALSE);	// 내부 상태 아이콘 표시/해제
	}
}

// 070412 LYW --- CharacterCalcManager : Add function to change stage of character.
void CCharacterCalcManager::Char_Stage_Notify( void* pMsg )
{
	/*
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if(!pPlayer) return;

	pPlayer->SetStage( pmsg->bData ) ;
	*/

	MSG_BYTE2* pmsg = (MSG_BYTE2*)pMsg ;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID) ;
	if(!pPlayer) return ;

	pPlayer->SetStage( pmsg->bData1, pmsg->bData2 ) ;

	BYTE jobType = pPlayer->GetCharacterTotalInfo()->Job[0] ;

	int nMsgNum = 0 ;

	switch( pmsg->bData1 )
	{
	case 2 : nMsgNum = Notify_SecondJob( jobType, pmsg->bData2 ) ;	break ;
	case 3 : nMsgNum = Notify_ThirdJob( jobType, pmsg->bData2 ) ;	break ;
	case 4 : nMsgNum = Notify_FourthJob( jobType, pmsg->bData2 ) ;	break ;
	case 5 : nMsgNum = Notify_FifthJob( jobType, pmsg->bData2 ) ;	break ;
	case 6 : nMsgNum = Notify_SixthJob( jobType, pmsg->bData2 ) ;	break ;
	}

	char tempStr[64] = { 0, } ;
	sprintf( tempStr, CHATMGR->GetChatMsg( 1445 ), RESRCMGR->GetMsg(nMsgNum) ) ;

	CHATMGR->AddMsg( CTC_SYSMSG, tempStr ) ;

	CCharacterDialog* pWindow = (CCharacterDialog*)WINDOWMGR->GetWindowForID(CI_CHARDLG) ;
	pWindow->SetClassName( RESRCMGR->GetMsg(nMsgNum) ) ;

	//GAMEIN->GetFamilyDlg()->InitFamilyDialog() ;

}

// 070416 LYW --- CharacterCalcManager : Add functions to notify class update.
int CCharacterCalcManager::Notify_SecondJob( BYTE jobType, BYTE byIndex )
{
	int nMsgNum = 0 ;

	switch( jobType )
	{
	case 1 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 365 ; break ;
			case 2 : nMsgNum = 366 ; break ;
			}
		}
		break ;

	case 2 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 384 ; break ;
			case 2 : nMsgNum = 385 ; break ;
			}
		}
		break ;

	case 3 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 401 ; break ;
			case 2 : nMsgNum = 402 ; break ;
			}
		}
		break ;
	}

	return nMsgNum ;
}

int CCharacterCalcManager::Notify_ThirdJob( BYTE jobType, BYTE byIndex )
{
	int nMsgNum = 0 ;

	switch( jobType )
	{
	case 1 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 367 ; break ;
			case 2 : nMsgNum = 368 ; break ;
			case 3 : nMsgNum = 369 ; break ;
			}
		}
		break ;

	case 2 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 386 ; break ;
			case 2 : nMsgNum = 387 ; break ;
			case 3 : nMsgNum = 388 ; break ;
			}
		}
		break ;

	case 3 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 403 ; break ;
			case 2 : nMsgNum = 404 ; break ;
			case 3 : nMsgNum = 405 ; break ;
			}
		}
		break ;
	}

	return nMsgNum ;
}

int CCharacterCalcManager::Notify_FourthJob( BYTE jobType, BYTE byIndex )
{
	int nMsgNum = 0 ;

	switch( jobType )
	{
	case 1 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 370 ; break ;
			case 2 : nMsgNum = 371 ; break ;
			case 3 : nMsgNum = 372 ; break ;
			case 4 : nMsgNum = 373 ; break ;
			}
		}
		break ;

	case 2 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 389 ; break ;
			case 2 : nMsgNum = 390 ; break ;
			case 3 : nMsgNum = 391 ; break ;
			case 4 : nMsgNum = 392 ; break ;
			}
		}
		break ;

	case 3 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 406 ; break ;
			case 2 : nMsgNum = 407 ; break ;
			case 3 : nMsgNum = 408 ; break ;
			case 4 : nMsgNum = 409 ; break ;
			}
		}
		break ;
	}

	return nMsgNum ;
}

int CCharacterCalcManager::Notify_FifthJob( BYTE jobType, BYTE byIndex )
{
	int nMsgNum = 0 ;

	switch( jobType )
	{
	case 1 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 374 ; break ;
			case 2 : nMsgNum = 375 ; break ;
			case 3 : nMsgNum = 376 ; break ;
			case 4 : nMsgNum = 377 ; break ;
			case 5 : nMsgNum = 378 ; break ;
			case 6 : nMsgNum = 379 ; break ;
			}
		}
		break ;

	case 2 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 393 ; break ;
			case 2 : nMsgNum = 394 ; break ;
			case 3 : nMsgNum = 395 ; break ;
			case 4 : nMsgNum = 396 ; break ;
			// 080709 LYW --- CharacterCalcManager : 아크레인저 직업 추가.
			case 5 : nMsgNum = 1004 ; break ;
			}
		}
		break ;

	case 3 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 410 ; break ;
			case 2 : nMsgNum = 411 ; break ;
			case 3 : nMsgNum = 412 ; break ;
			case 4 : nMsgNum = 413 ; break ;
			case 5 : nMsgNum = 414 ; break ;
			}
		}
		break ;
	}

	return nMsgNum ;
}

int CCharacterCalcManager::Notify_SixthJob( BYTE jobType, BYTE byIndex )
{
	int nMsgNum = 0 ;

	switch( jobType )
	{
	case 1 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 380 ; break ;
			case 2 : nMsgNum = 381 ; break ;
			case 3 : nMsgNum = 382 ; break ;
			}
		}
		break ;

	case 2 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 397 ; break ;
			case 2 : nMsgNum = 398 ; break ;
			case 3 : nMsgNum = 399 ; break ;
			}
		}
		break ;

	case 3 :
		{
			switch( byIndex )
			{
			case 1 : nMsgNum = 415 ; break ;
			case 2 : nMsgNum = 416 ; break ;
			case 3 : nMsgNum = 417 ; break ;
			}
		}
		break ;
	}

	return nMsgNum ;
}

// desc_hseos_몬스터미터01
// S 몬스터미터 추가 added by hseos 2007.05.23	2008.01.11:CPlayer->CHero
void CCharacterCalcManager::Char_Monstermeter_KillMonster(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	if( HERO && HERO->GetID() == pmsg->dwObjectID )
	{
		HERO->SetMonstermeterKillMonNum(pmsg->dwData1, pmsg->dwData2);
	}
}

void CCharacterCalcManager::Char_Monstermeter_PlayTime(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	if( HERO && HERO->GetID() == pmsg->dwObjectID )
	{
		HERO->SetMonstermeterPlaytime(pmsg->dwData1, pmsg->dwData2);
	}
}
// E 몬스터미터 추가 added by hseos 2007.05.23	2008.01.11:CPlayer->CHero


void CCharacterCalcManager::Initialize( CHero* hero )
{
	PlayerStat& stat = hero->GetItemStats();

	// 초기화
	{
		ZeroMemory( &stat, sizeof( stat ) );

		SKILLTREEMGR->ResetSetSkill();

		{
			CHero::SetItemLevel& setLevel = hero->GetSetItemLevel();

			setLevel.clear();
		}

		// 080313 LUJ, 세트 아이템 능력 초기화
		PlayerStat& setItemStat = hero->GetSetItemStats();

		ZeroMemory( &setItemStat, sizeof( setItemStat ) );
	}	

	typedef std::map< const SetScript*, DWORD > SetItemSize;
	SetItemSize									setItemSize;

	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE* item = ITEMMGR->GetItemInfoAbsIn( hero, part);

		if(	0 == item || 
			0 == item->dwDBIdx )
		{
			continue;
		}

		const ITEM_INFO * info = ITEMMGR->GetItemInfo(item->wIconIdx);

		if( ! info )
		{
			continue;
		}

		// 세트 아이템 능력 추가를 위해, 같은 종류의 세트 스크립트를 가진 아이템 개수도 조사해두자
		{
			const SetScript* script = ITEMMGR->GetSetScript( item->wIconIdx );

			if( script )
			{
				++setItemSize[ script ];
			}
		}		

		AddStat( *info, stat );
		AddStat( *info, ITEMMGR->GetOption( *item ), stat );
	}

	// 세트 능력 적용. 낮은 단계부터 누적시키며 적용해야 한다
	for(
		SetItemSize::const_iterator it = setItemSize.begin();
		setItemSize.end() != it;
		++it )
	{
		const SetScript*	script	= it->first;
		const DWORD			size	= it->second;

		for( DWORD i = 1; i <= size; ++i )
		{
			AddSetItemStats( hero, *script, i );
		}
	}

	CalcCharStats( hero );
}


//void CCharacterCalcManager::CalcItemStats( CHero* hero )
//{
//	// 기존 스킬 트리를 복구해야함...
//	{
//		CHero::SetItemLevel& setLevel = hero->GetSetItemLevel();
//
//		for(
//			CHero::SetItemLevel::const_iterator script_it = setLevel.begin();
//			setLevel.end() != script_it;
//			++script_it )
//		{
//			const int			level	= script_it->second;
//			const SetScript&	script	= ( *script_it->first );
//
//			if( level >= int( script.mAbility.size() ) )
//			{
//				ASSERT( 0 );
//				continue;
//			}
//
//			SetScript::Ability::const_iterator ability_it = script.mAbility.begin();
//			std::advance( ability_it, level );
//
//			const SetScript::Element& element = ability_it->second;;
//
//			for(
//				SetScript::Element::Skill::const_iterator it = element.mSkill.begin();
//				element.mSkill.end() != it;
//				++it )
//			{
//				const DWORD index	= it->first;
//				
//				SKILLTREEMGR->RemoveSetSkill( index );
//				QUICKMGR->RefreshSkillItem( index );
//			}
//		}
//	}
//
//	Initialize( hero );
//
//	// 아이템 수치가 변경되면 캐릭터 수치도 변경
//	CalcCharStats( hero );
//}


// 캐릭게임In
// 레벨업시 ( 능력치 바뀔때)
// 무공발동시(+능력치) : 구조체를 하나 더 둬야 하나?
void CCharacterCalcManager::CalcCharStats( CHero* hero )
{
	PlayerStat& char_stats = hero->GetCharStats();
	ZeroMemory( &char_stats, sizeof( char_stats ) );

	HERO_TOTALINFO pHeroInfo;
	hero->GetHeroTotalInfo(&pHeroInfo);

	// 아이템 장착한거에 따른 캐릭터 수치 셋팅
	char_stats.mDexterity.mPlus		= float( pHeroInfo.Dex );
	char_stats.mStrength.mPlus		= float( pHeroInfo.Str );

	// 070411 LYW --- StatsCalcManager : Setting other stats.
	char_stats.mVitality.mPlus		= float( pHeroInfo.Vit );
	char_stats.mIntelligence.mPlus	= float( pHeroInfo.Int );
	char_stats.mWisdom.mPlus		= float( pHeroInfo.Wis );

	// 070412 LYW --- StatsCalcManager : Delete this part.
	/*
	char_stats.PhysicalDefense = (WORD)(((hero.GetVitality())/1.5)*0.6);
	char_stats.Critical = (hero.GetStrength()) / 4;
	char_stats.Decisive = (hero.GetWisdom()) / 4;
	*/
}


void CCharacterCalcManager::AddSetItemStats( CHero* hero, const SetScript& script, int setItemSize )
{
	// 세트 아이템은 다음과 같이 적용된다. 세트로 구성되는 아이템 그룹들이 있고, 장비한 세트 아이템 개수에 따라 능력이 차례로 부여된다.

	// 세트 스크립트로 부여된 레벨을 가져와서 그 다음부터 조사
	// 세트 스크립트의 능력이 요구하는 세트 아이템 개수 조건을 충족하면 능력 부여

	typedef CPlayer::SetItemLevel SetItemLevel;

	CPlayer::SetItemLevel& setLevel = hero->GetSetItemLevel();

	int& level = setLevel[ &script ];

	ASSERT( size_t( level ) < script.mAbility.size() );
	SetScript::Ability::const_iterator it = script.mAbility.begin();
	std::advance( it, level );

	bool isUpdate = false;

	for(	;
		script.mAbility.end() != it;
		++it, ++level )
	{
		const int size = it->first;

		if( size > setItemSize )
		{
			break;
		}

		isUpdate = true;

		// 능력을 부여하자
		{
			const SetScript::Element& element = it->second;

			AddStat( element.mStat, hero->GetSetItemStats() );
			
			// 주의! 맵 서버에서는 액티브 스킬을 추가할 필요 없음
			for(
				SetScript::Element::Skill::const_iterator inner = element.mSkill.begin();
				element.mSkill.end() != inner;
				++inner )
			{
				const DWORD index	= inner->first;
				const BYTE	level	= inner->second;

				SKILLTREEMGR->AddSetSkill( index, level );
				QUICKMGR->RefreshSkillItem( index );
			}
		}
	}

	if( 0 == level )
	{
		setLevel.erase( &script );
	}

	// 수치가 변화했으면 캐릭터 수치 재계산
	if( isUpdate )
	{
		CalcCharStats( hero );
	}
}


void CCharacterCalcManager::RemoveSetItemStats( CHero* hero, const SetScript& script, int setItemSize )
{
	// 해당 플레이어가 가진 세트 스크립트로 부여된 레벨을 가져온다.
	// 있으면 거기부터 시작해서 아래로 조사. 없으면 그냥 끝
	// 캐릭터 세트 능력을 가져와 변경
	// 수치가 변화했으면 수치 재계산 요청

	typedef CPlayer::SetItemLevel SetItemLevel;

	CPlayer::SetItemLevel& setLevel = hero->GetSetItemLevel();

	// 해당 스크립트로 저장된 레벨이 없다는 건 적용된 세트 능력이 없다는 뜻
	if( setLevel.end() == setLevel.find( &script ) )
	{
		return;
	}

	int& level = setLevel[ &script ];

	ASSERT( 0 < level );
	ASSERT( size_t( level ) - 1 < script.mAbility.size() );
	SetScript::Ability::const_iterator it = script.mAbility.begin();
	std::advance( it, level - 1 );

	bool isUpdate = false;

	for( ;; )	
	{
		const int size = it->first;

		if( size <= setItemSize )
		{
			break;
		}

		isUpdate = true;

		// 능력을 제거하자
		{
			const SetScript::Element& element = it->second;

			RemoveStat( element.mStat, hero->GetSetItemStats() );

			for(	SetScript::Element::Skill::const_iterator inner = element.mSkill.begin();
				element.mSkill.end() != inner;
				++inner )
			{
				const DWORD index	= inner->first;
				//const BYTE	level	= inner->second;

				SKILLTREEMGR->RemoveSetSkill( index );
				QUICKMGR->RefreshSkillItem( index );
			}
		}

		--level;

		if( script.mAbility.begin() == it-- )
		{
			break;
		}
	}

	if( 0 == level )
	{
		setLevel.erase( &script );
	}

	// 수치가 변화했으면 캐릭터 수치 재계산
	if( isUpdate )
	{
		CalcCharStats( hero );
	}
}

void CCharacterCalcManager::Char_Hide_Notify( void* pMsg )
{
	MSG_WORD* pmsg = ( MSG_WORD* )pMsg;

	OBJECTMGR->SetHide( pmsg->dwObjectID, pmsg->wData );
}

void CCharacterCalcManager::Char_Detect_Notify( void* pMsg )
{
	MSG_WORD* pmsg = ( MSG_WORD* )pMsg;

	OBJECTMGR->SetDetect( pmsg->dwObjectID, pmsg->wData );
}


void CCharacterCalcManager::AddItem( CHero* hero, const ITEMBASE& item )
{
	PlayerStat& stat = hero->GetItemStats();

	const ITEM_INFO * info = ITEMMGR->GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	AddStat( *info, stat );
	AddStat( *info, ITEMMGR->GetOption( item ), stat );

	// 세트 아이템 계산, 웅주 070613
	{
		const SetScript* setScript = ITEMMGR->GetSetScript( item.wIconIdx );

		if( setScript )
		{
			AddSetItemStats( hero, *setScript, ITEMMGR->GetSetItemSize( hero, setScript ) );
		}
	}

	// TODO : 아이템 장착
	CalcCharStats( hero );
}


void CCharacterCalcManager::RemoveItem( CHero* hero, const ITEMBASE& item )
{
	const ITEM_INFO * info = ITEMMGR->GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	PlayerStat& stat = hero->GetItemStats();

	RemoveStat( *info, stat );
	RemoveStat( *info, ITEMMGR->GetOption( item ), stat );

	// 세트 아이템 계산, 웅주 070613
	{
		const SetScript* setScript = ITEMMGR->GetSetScript( item.wIconIdx );

		if( setScript )
		{
			RemoveSetItemStats( hero, *setScript, ITEMMGR->GetSetItemSize( hero, setScript ) );
		}
	}

	// TODO : 아이템 해제
	CalcCharStats( hero );
}


void CCharacterCalcManager::AddStat(const ITEM_INFO& info, PlayerStat& stat )
{
	stat.mPhysicAttack.mPlus	+= float( info.PhysicAttack );
	stat.mMagicAttack.mPlus		+= float( info.MagicAttack );
	stat.mPhysicDefense.mPlus	+= float( info.PhysicDefense );
	stat.mMagicDefense.mPlus	+= float( info.MagicDefense );
	stat.mStrength.mPlus		+= float( info.ImprovementStr );
	stat.mDexterity.mPlus		+= float( info.ImprovementDex );
	stat.mVitality.mPlus 		+= float( info.ImprovementVit );
	stat.mWisdom.mPlus 			+= float( info.ImprovementWis );
	stat.mIntelligence.mPlus	+= float( info.ImprovementInt );
	stat.mLife.mPlus 			+= float( info.ImprovementLife );
	stat.mMana.mPlus 			+= float( info.ImprovementMana );
}


// 080319 LUJ, 추가 인챈트 수치를 부여한다
// 080320 LUJ, 추가 인챈트 수치를 함수로 가져옴
void CCharacterCalcManager::AddStat( const ITEM_INFO& info,  const ITEM_OPTION& option, PlayerStat& stat )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;

	stat.mPhysicAttack.mPlus	+= float( reinforce.mPhysicAttack );
	stat.mPhysicDefense.mPlus	+= float( reinforce.mPhysicDefence );
	stat.mMagicAttack.mPlus		+= float( reinforce.mMagicAttack );
	stat.mMagicDefense.mPlus	+= float( reinforce.mMagicDefence );
	stat.mStrength.mPlus		+= float( reinforce.mStrength		+ mix.mStrength );
	stat.mDexterity.mPlus		+= float( reinforce.mDexterity		+ mix.mDexterity );
	stat.mVitality.mPlus		+= float( reinforce.mVitality		+ mix.mVitality );
	stat.mWisdom.mPlus			+= float( reinforce.mWisdom			+ mix.mWisdom );
	stat.mIntelligence.mPlus	+= float( reinforce.mIntelligence	+ mix.mIntelligence );
	stat.mLife.mPlus			+= float( reinforce.mLife );
	stat.mMana.mPlus			+= float( reinforce.mMana );
	stat.mRecoveryLife.mPlus	+= float( reinforce.mLifeRecovery );
	stat.mRecoveryMana.mPlus	+= float( reinforce.mManaRecovery );
	stat.mCriticalRate.mPlus	+= float( reinforce.mCriticalRate );
	stat.mCriticalDamage.mPlus	+= float( reinforce.mCriticalDamage );
	stat.mAccuracy.mPlus		+= float( reinforce.mAccuracy );
	stat.mMoveSpeed.mPlus		+= float( reinforce.mMoveSpeed );
	stat.mEvade.mPlus			+= float( reinforce.mEvade );

	const ITEM_OPTION::Drop& drop = option.mDrop;

	for( DWORD i = 0; i < sizeof( drop.mValue ) / sizeof( *drop.mValue ); ++i )
	{
		const ITEM_OPTION::Drop::Value& value = drop.mValue[ i ];

		switch( value.mKey )
		{
		case ITEM_OPTION::Drop::KeyPlusStrength:
			{
				stat.mStrength.mPlus = Round( stat.mStrength.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusIntelligence:
			{
				stat.mIntelligence.mPlus = Round( stat.mIntelligence.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusDexterity:
			{
				stat.mDexterity.mPlus = Round( stat.mDexterity.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusWisdom:
			{
				stat.mWisdom.mPlus = Round( stat.mWisdom.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusVitality:
			{
				stat.mVitality.mPlus = Round( stat.mVitality.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalAttack:
			{
				stat.mPhysicAttack.mPlus = Round( stat.mPhysicAttack.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
			{
				stat.mPhysicDefense.mPlus = Round( stat.mPhysicDefense.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalAttack:
			{
				stat.mMagicAttack.mPlus = Round( stat.mMagicAttack.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalDefence:
			{
				stat.mMagicDefense.mPlus = Round( stat.mMagicDefense.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalRate:
			{
				stat.mCriticalRate.mPlus = Round( stat.mCriticalRate.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalDamage:
			{
				stat.mCriticalDamage.mPlus = Round( stat.mCriticalDamage.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusAccuracy:
			{
				stat.mAccuracy.mPlus = Round( stat.mAccuracy.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusEvade:
			{
				stat.mEvade.mPlus = Round( stat.mEvade.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMoveSpeed:
			{
				stat.mMoveSpeed.mPlus = Round( stat.mMoveSpeed.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLife:
			{
				stat.mLife.mPlus = Round( stat.mLife.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMana:
			{
				stat.mMana.mPlus = Round( stat.mMana.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLifeRecovery:
			{
				stat.mRecoveryLife.mPlus = Round( stat.mRecoveryLife.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusManaRecovery:
			{
				stat.mRecoveryMana.mPlus = Round( stat.mRecoveryMana.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentStrength:
			{
				stat.mStrength.mPercent = Round( stat.mStrength.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentIntelligence:
			{
				stat.mIntelligence.mPercent = Round( stat.mIntelligence.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentDexterity:
			{
				stat.mDexterity.mPercent = Round( stat.mDexterity.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentWisdom:
			{
				stat.mWisdom.mPercent = Round( stat.mWisdom.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentVitality:
			{
				stat.mVitality.mPercent = Round( stat.mVitality.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalAttack:
			{
				stat.mPhysicAttack.mPercent = Round( stat.mPhysicAttack.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
			{
				stat.mPhysicDefense.mPercent = Round( stat.mPhysicDefense.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalAttack:
			{
				stat.mMagicAttack.mPercent = Round( stat.mMagicAttack.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalDefence:
			{
				stat.mMagicDefense.mPercent = Round( stat.mMagicDefense.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalRate:
			{
				stat.mCriticalRate.mPercent = Round( stat.mCriticalRate.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalDamage:
			{
				stat.mCriticalDamage.mPercent = Round( stat.mCriticalDamage.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentAccuracy:
			{
				stat.mAccuracy.mPercent = Round( stat.mAccuracy.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentEvade:
			{
				stat.mEvade.mPercent = Round( stat.mEvade.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMoveSpeed:
			{
				stat.mMoveSpeed.mPercent = Round( stat.mMoveSpeed.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLife:
			{
				stat.mLife.mPercent = Round( stat.mLife.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMana:
			{
				stat.mMana.mPercent = Round( stat.mMana.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLifeRecovery:
			{
				stat.mRecoveryLife.mPercent = Round( stat.mRecoveryLife.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentManaRecovery:
			{
				stat.mRecoveryMana.mPercent = Round( stat.mRecoveryMana.mPercent + value.mValue, 3 );
				break;
			}
		}
	}

	// 080320 LUJ,	추가 인챈트 능력치를 부여한다
	// 080421 LUJ,	추가 인챈트 수치 적용을 단순화 
	{
		const EnchantScript* script = ITEMMGR->GetEnchantScript( option.mEnchant.mIndex );

		if( script && option.mEnchant.mLevel )
		{
			for(
				EnchantScript::Ability::const_iterator it = script->mAbility.begin();
				script->mAbility.end() != it;
				++it )
			{
				switch( EnchantScript::eType( *it ) )
				{
				case ReinforceScript::eTypeStrength:
					{
						stat.mStrength.mPlus += GetBonusEnchantValue( info, option, info.ImprovementStr );
						break;
					}
				case ReinforceScript::eTypeDexterity:
					{
						stat.mDexterity.mPlus += GetBonusEnchantValue( info, option, info.ImprovementDex );
						break;
					}
				case ReinforceScript::eTypeVitality:
					{
						stat.mVitality.mPlus += GetBonusEnchantValue( info, option, info.ImprovementVit );
						break;
					}
				case ReinforceScript::eTypeWisdom:
					{
						stat.mWisdom.mPlus += GetBonusEnchantValue( info, option, info.ImprovementWis );
						break;
					}
				case ReinforceScript::eTypeIntelligence:
					{
						stat.mIntelligence.mPlus += GetBonusEnchantValue( info, option, info.ImprovementInt );
						break;
					}
				case ReinforceScript::eTypeLife:
					{
						stat.mLife.mPlus += GetBonusEnchantValue( info, option, info.ImprovementLife );
						break;
					}
				case ReinforceScript::eTypeMana:
					{
						stat.mMana.mPlus += GetBonusEnchantValue( info, option, info.ImprovementMana );
						break;
					}
				case ReinforceScript::eTypePhysicAttack:
					{
						stat.mPhysicAttack.mPlus += GetBonusEnchantValue( info, option, info.PhysicAttack );
						break;
					}
				case ReinforceScript::eTypePhysicDefence:
					{
						stat.mPhysicDefense.mPlus += GetBonusEnchantValue( info, option, info.PhysicDefense );
						break;
					}
				case ReinforceScript::eTypeMagicAttack:
					{
						stat.mMagicAttack.mPlus += GetBonusEnchantValue( info, option, info.MagicAttack );
						break;
					}
				case ReinforceScript::eTypeMagicDefence:
					{
						stat.mMagicDefense.mPlus += GetBonusEnchantValue( info, option, info.MagicDefense );
						break;
					}
					// 080319 LUJ, 추가 인챈트 수치를 부여할 수 없는 속성들
				case ReinforceScript::eTypeMoveSpeed:
				case ReinforceScript::eTypeEvade:
				case ReinforceScript::eTypeAccuracy:
				case ReinforceScript::eTypeLifeRecovery:
				case ReinforceScript::eTypeManaRecovery:
				case ReinforceScript::eTypeCriticalRate:
				case ReinforceScript::eTypeCriticalDamage:
					{
						break;
					}
				}
			}
		}
	}
}


void CCharacterCalcManager::AddStat( const PlayerStat& srcStat, PlayerStat& destStat )
{
	destStat.mPhysicAttack.mPercent		+= srcStat.mPhysicAttack.mPercent;
	destStat.mPhysicAttack.mPlus		+= srcStat.mPhysicAttack.mPlus;

	destStat.mMagicAttack.mPercent		+= srcStat.mMagicAttack.mPercent;
	destStat.mMagicAttack.mPlus			+= srcStat.mMagicAttack.mPlus;

	destStat.mPhysicDefense.mPercent	+= srcStat.mPhysicDefense.mPercent;
	destStat.mPhysicDefense.mPlus		+= srcStat.mPhysicDefense.mPlus;

	destStat.mMagicDefense.mPercent		+= srcStat.mMagicDefense.mPercent;
	destStat.mMagicDefense.mPlus		+= srcStat.mMagicDefense.mPlus;

	destStat.mStrength.mPercent			+= srcStat.mStrength.mPercent;
	destStat.mStrength.mPlus			+= srcStat.mStrength.mPlus;

	destStat.mDexterity.mPercent 		+= srcStat.mDexterity.mPercent;
	destStat.mDexterity.mPlus 			+= srcStat.mDexterity.mPlus;

	destStat.mVitality.mPercent			+= srcStat.mVitality.mPercent;
	destStat.mVitality.mPlus			+= srcStat.mVitality.mPlus;

	destStat.mIntelligence.mPercent 	+= srcStat.mIntelligence.mPercent;
	destStat.mIntelligence.mPlus 		+= srcStat.mIntelligence.mPlus;

	destStat.mWisdom.mPercent 			+= srcStat.mWisdom.mPercent;
	destStat.mWisdom.mPlus 				+= srcStat.mWisdom.mPlus;

	destStat.mLife.mPercent				+= srcStat.mLife.mPercent;
	destStat.mLife.mPlus				+= srcStat.mLife.mPlus;

	destStat.mMana.mPercent				+= srcStat.mMana.mPercent;
	destStat.mMana.mPlus				+= srcStat.mMana.mPlus;

	destStat.mRecoveryLife.mPercent	+= srcStat.mRecoveryLife.mPercent;
	destStat.mRecoveryLife.mPlus		+= srcStat.mRecoveryLife.mPlus;

	destStat.mRecoveryMana.mPercent		+= srcStat.mRecoveryMana.mPercent;
	destStat.mRecoveryMana.mPlus		+= srcStat.mRecoveryMana.mPlus;

	destStat.mAccuracy.mPercent			+= srcStat.mAccuracy.mPercent;
	destStat.mAccuracy.mPlus			+= srcStat.mAccuracy.mPlus;

	destStat.mEvade.mPercent			+= srcStat.mEvade.mPercent;
	destStat.mEvade.mPlus				+= srcStat.mEvade.mPlus;

	destStat.mCriticalRate.mPercent		+= srcStat.mCriticalRate.mPercent;
	destStat.mCriticalRate.mPlus		+= srcStat.mCriticalRate.mPlus;

	destStat.mCriticalDamage.mPercent	+= srcStat.mCriticalDamage.mPercent;
	destStat.mCriticalDamage.mPlus		+= srcStat.mCriticalDamage.mPlus;

	destStat.mMoveSpeed.mPercent		+= srcStat.mMoveSpeed.mPercent;
	destStat.mMoveSpeed.mPlus			+= srcStat.mMoveSpeed.mPlus;
}


void CCharacterCalcManager::RemoveStat( const ITEM_INFO& info, PlayerStat& stat )
{
	stat.mPhysicAttack.mPlus	-= float( info.PhysicAttack );
	stat.mMagicAttack.mPlus		-= float( info.MagicAttack );
	stat.mPhysicDefense.mPlus	-= float( info.PhysicDefense );
	stat.mMagicDefense.mPlus	-= float( info.MagicDefense );
	stat.mStrength.mPlus		-= float( info.ImprovementStr );
	stat.mDexterity.mPlus		-= float( info.ImprovementDex );
	stat.mVitality.mPlus 		-= float( info.ImprovementVit );
	stat.mWisdom.mPlus 			-= float( info.ImprovementWis );
	stat.mIntelligence.mPlus	-= float( info.ImprovementInt );
	stat.mLife.mPlus 			-= float( info.ImprovementLife );
	stat.mMana.mPlus 			-= float( info.ImprovementMana );
}


// 080319 LUJ, 추가 인챈트 수치를 제거함
// 080320 LUJ, 추가 인챈트 수치를 함수를 통해 가져옴
void CCharacterCalcManager::RemoveStat( const ITEM_INFO& info, const ITEM_OPTION& option, PlayerStat& stat )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;

	stat.mPhysicAttack.mPlus	-= float( reinforce.mPhysicAttack );
	stat.mPhysicDefense.mPlus	-= float( reinforce.mPhysicDefence );
	stat.mMagicAttack.mPlus		-= float( reinforce.mMagicAttack );
	stat.mMagicDefense.mPlus	-= float( reinforce.mMagicDefence );
	stat.mStrength.mPlus		-= float( reinforce.mStrength		+ mix.mStrength );
	stat.mDexterity.mPlus		-= float( reinforce.mDexterity		+ mix.mDexterity );
	stat.mVitality.mPlus		-= float( reinforce.mVitality		+ mix.mVitality );
	stat.mWisdom.mPlus			-= float( reinforce.mWisdom			+ mix.mWisdom );
	stat.mIntelligence.mPlus	-= float( reinforce.mIntelligence	+ mix.mIntelligence );
	stat.mLife.mPlus			-= float( reinforce.mLife );
	stat.mMana.mPlus			-= float( reinforce.mMana );
	stat.mRecoveryLife.mPlus	-= float( reinforce.mLifeRecovery );
	stat.mRecoveryMana.mPlus	-= float( reinforce.mManaRecovery );
	stat.mCriticalRate.mPlus	-= float( reinforce.mCriticalRate );
	stat.mCriticalDamage.mPlus	-= float( reinforce.mCriticalDamage );
	stat.mAccuracy.mPlus		-= float( reinforce.mAccuracy );
	stat.mMoveSpeed.mPlus		-= float( reinforce.mMoveSpeed );
	stat.mEvade.mPlus			-= float( reinforce.mEvade );

	const ITEM_OPTION::Drop& drop = option.mDrop;

	for( DWORD i = 0; i < sizeof( drop.mValue ) / sizeof( *drop.mValue ); ++i )
	{
		const ITEM_OPTION::Drop::Value& value = drop.mValue[ i ];

		switch( value.mKey )
		{
		case ITEM_OPTION::Drop::KeyPlusStrength:
			{
				stat.mStrength.mPlus = Round( stat.mStrength.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusIntelligence:
			{
				stat.mIntelligence.mPlus = Round( stat.mIntelligence.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusDexterity:
			{
				stat.mDexterity.mPlus = Round( stat.mDexterity.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusWisdom:
			{
				stat.mWisdom.mPlus = Round( stat.mWisdom.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusVitality:
			{
				stat.mVitality.mPlus = Round( stat.mVitality.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalAttack:
			{
				stat.mPhysicAttack.mPlus = Round( stat.mPhysicAttack.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
			{
				stat.mPhysicDefense.mPlus = Round( stat.mPhysicDefense.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalAttack:
			{
				stat.mMagicAttack.mPlus = Round( stat.mMagicAttack.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalDefence:
			{
				stat.mMagicDefense.mPlus = Round( stat.mMagicDefense.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalRate:
			{
				stat.mCriticalRate.mPlus = Round( stat.mCriticalRate.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalDamage:
			{
				stat.mCriticalDamage.mPlus = Round( stat.mCriticalDamage.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusAccuracy:
			{
				stat.mAccuracy.mPlus = Round( stat.mAccuracy.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusEvade:
			{
				stat.mEvade.mPlus = Round( stat.mEvade.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMoveSpeed:
			{
				stat.mMoveSpeed.mPlus = Round( stat.mMoveSpeed.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLife:
			{
				stat.mLife.mPlus = Round( stat.mLife.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMana:
			{
				stat.mMana.mPlus = Round( stat.mMana.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLifeRecovery:
			{
				stat.mRecoveryLife.mPlus = Round( stat.mRecoveryLife.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusManaRecovery:
			{
				stat.mRecoveryMana.mPlus = Round( stat.mRecoveryMana.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentStrength:
			{
				stat.mStrength.mPercent = Round( stat.mStrength.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentIntelligence:
			{
				stat.mIntelligence.mPercent = Round( stat.mIntelligence.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentDexterity:
			{
				stat.mDexterity.mPercent = Round( stat.mDexterity.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentWisdom:
			{
				stat.mWisdom.mPercent = Round( stat.mWisdom.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentVitality:
			{
				stat.mVitality.mPercent = Round( stat.mVitality.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalAttack:
			{
				stat.mPhysicAttack.mPercent = Round( stat.mPhysicAttack.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
			{
				stat.mPhysicDefense.mPercent = Round( stat.mPhysicDefense.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalAttack:
			{
				stat.mMagicAttack.mPercent = Round( stat.mMagicAttack.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalDefence:
			{
				stat.mMagicDefense.mPercent = Round( stat.mMagicDefense.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalRate:
			{
				stat.mCriticalRate.mPercent = Round( stat.mCriticalRate.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalDamage:
			{
				stat.mCriticalDamage.mPercent = Round( stat.mCriticalDamage.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentAccuracy:
			{
				stat.mAccuracy.mPercent = Round( stat.mAccuracy.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentEvade:
			{
				stat.mEvade.mPercent = Round( stat.mEvade.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMoveSpeed:
			{
				stat.mMoveSpeed.mPercent = Round( stat.mMoveSpeed.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLife:
			{
				stat.mLife.mPercent = Round( stat.mLife.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMana:
			{
				stat.mMana.mPercent = Round( stat.mMana.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLifeRecovery:
			{
				stat.mRecoveryLife.mPercent = Round( stat.mRecoveryLife.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentManaRecovery:
			{
				stat.mRecoveryMana.mPercent = Round( stat.mRecoveryMana.mPercent - value.mValue, 3 );
				break;
			}
		}
	}

	// 080320 LUJ,	추가 인챈트 능력치를 제거한다
	// 080421 LUJ,	추가 인챈트 수치 적용을 단순화 
	{
		const EnchantScript* script = ITEMMGR->GetEnchantScript( option.mEnchant.mIndex );

		if( script && option.mEnchant.mLevel )
		{
			for(
				EnchantScript::Ability::const_iterator it = script->mAbility.begin();
				script->mAbility.end() != it;
			++it )
			{
				switch( EnchantScript::eType( *it ) )
				{
				case ReinforceScript::eTypeStrength:
					{
						stat.mStrength.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementStr );
						break;
					}
				case ReinforceScript::eTypeDexterity:
					{
						stat.mDexterity.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementDex );
						break;
					}
				case ReinforceScript::eTypeVitality:
					{
						stat.mVitality.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementVit );
						break;
					}
				case ReinforceScript::eTypeWisdom:
					{
						stat.mWisdom.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementWis );
						break;
					}
				case ReinforceScript::eTypeIntelligence:
					{
						stat.mIntelligence.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementInt );
						break;
					}
				case ReinforceScript::eTypeLife:
					{
						stat.mLife.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementLife );
						break;
					}
				case ReinforceScript::eTypeMana:
					{
						stat.mMana.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementMana );
						break;
					}
				case ReinforceScript::eTypePhysicAttack:
					{
						stat.mPhysicAttack.mPlus -= GetBonusEnchantValue( info, option, info.PhysicAttack );
						break;
					}
				case ReinforceScript::eTypePhysicDefence:
					{
						stat.mPhysicDefense.mPlus -= GetBonusEnchantValue( info, option, info.PhysicDefense );
						break;
					}
				case ReinforceScript::eTypeMagicAttack:
					{
						stat.mMagicAttack.mPlus -= GetBonusEnchantValue( info, option, info.MagicAttack );
						break;
					}
				case ReinforceScript::eTypeMagicDefence:
					{
						stat.mMagicDefense.mPlus -= GetBonusEnchantValue( info, option, info.MagicDefense );
						break;
					}
					// 080319 LUJ, 추가 인챈트 수치를 부여할 수 없는 속성들
				case ReinforceScript::eTypeMoveSpeed:
				case ReinforceScript::eTypeEvade:
				case ReinforceScript::eTypeAccuracy:
				case ReinforceScript::eTypeLifeRecovery:
				case ReinforceScript::eTypeManaRecovery:
				case ReinforceScript::eTypeCriticalRate:
				case ReinforceScript::eTypeCriticalDamage:
					{
						break;
					}
				}
			}
		}
	}
}


void CCharacterCalcManager::RemoveStat( const PlayerStat& srcStat, PlayerStat& destStat )
{
	destStat.mPhysicAttack.mPercent		-= srcStat.mPhysicAttack.mPercent;
	destStat.mPhysicAttack.mPlus		-= srcStat.mPhysicAttack.mPlus;

	destStat.mMagicAttack.mPercent		-= srcStat.mMagicAttack.mPercent;
	destStat.mMagicAttack.mPlus			-= srcStat.mMagicAttack.mPlus;

	destStat.mPhysicDefense.mPercent	-= srcStat.mPhysicDefense.mPercent;
	destStat.mPhysicDefense.mPlus		-= srcStat.mPhysicDefense.mPlus;

	destStat.mMagicDefense.mPercent		-= srcStat.mMagicDefense.mPercent;
	destStat.mMagicDefense.mPlus		-= srcStat.mMagicDefense.mPlus;

	destStat.mStrength.mPercent			-= srcStat.mStrength.mPercent;
	destStat.mStrength.mPlus			-= srcStat.mStrength.mPlus;

	destStat.mDexterity.mPercent 		-= srcStat.mDexterity.mPercent;
	destStat.mDexterity.mPlus 			-= srcStat.mDexterity.mPlus;

	destStat.mVitality.mPercent			-= srcStat.mVitality.mPercent;
	destStat.mVitality.mPlus			-= srcStat.mVitality.mPlus;

	destStat.mIntelligence.mPercent 	-= srcStat.mIntelligence.mPercent;
	destStat.mIntelligence.mPlus 		-= srcStat.mIntelligence.mPlus;

	destStat.mWisdom.mPercent 			-= srcStat.mWisdom.mPercent;
	destStat.mWisdom.mPlus 				-= srcStat.mWisdom.mPlus;

	destStat.mLife.mPercent				-= srcStat.mLife.mPercent;
	destStat.mLife.mPlus				-= srcStat.mLife.mPlus;

	destStat.mMana.mPercent				-= srcStat.mMana.mPercent;
	destStat.mMana.mPlus				-= srcStat.mMana.mPlus;

	destStat.mRecoveryLife.mPercent	-= srcStat.mRecoveryLife.mPercent;
	destStat.mRecoveryLife.mPlus		-= srcStat.mRecoveryLife.mPlus;

	destStat.mRecoveryMana.mPercent		-= srcStat.mRecoveryMana.mPercent;
	destStat.mRecoveryMana.mPlus		-= srcStat.mRecoveryMana.mPlus;

	destStat.mAccuracy.mPercent			-= srcStat.mAccuracy.mPercent;
	destStat.mAccuracy.mPlus			-= srcStat.mAccuracy.mPlus;

	destStat.mEvade.mPercent			-= srcStat.mEvade.mPercent;
	destStat.mEvade.mPlus				-= srcStat.mEvade.mPlus;

	destStat.mCriticalRate.mPercent		-= srcStat.mCriticalRate.mPercent;
	destStat.mCriticalRate.mPlus		-= srcStat.mCriticalRate.mPlus;

	destStat.mCriticalDamage.mPercent	-= srcStat.mCriticalDamage.mPercent;
	destStat.mCriticalDamage.mPlus		-= srcStat.mCriticalDamage.mPlus;

	destStat.mMoveSpeed.mPercent		-= srcStat.mMoveSpeed.mPercent;
	destStat.mMoveSpeed.mPlus			-= srcStat.mMoveSpeed.mPlus;
}
