// HeroMove.cpp: implementation of the CHeroMove class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#endif	//_CHEATENABLE_
#include "HeroMove.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "MousePointer.h"
//#include "WindowIdEnum.h"
//#include "interface\cWindowManager.h"
//#include "./Interface/cListDialog.h"
#include "Interface/cEditBox.h"

#include "GameResourceManager.h"
#include "ExchangeManager.h"
//#include "cMsgBox.h"
#include "chatmanager.h"
#include "NpcScriptManager.h"

#include "BattleSystem_Client.h"

#include "Input/UserInput.h"
#include "MouseCursor.h"
#include "Interface/cWindowManager.h"
#include "PartyManager.h"


#include "../[cc]skill/client/manager/skillmanager.h"
#include "../[cc]skill/client/object/skillobject.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07	2007.09.13
#include "../hseos/Farm/SHFarmManager.h"
#include "NpcScriptDialog.h"
// E 농장시스템 추가 added by hseos 2007.05.07	2007.09.13

// 080515 KTH --
#include "../hseos/Farm/FarmAnimalDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHeroMove::CHeroMove()
{

}

CHeroMove::~CHeroMove()
{

}

void CHeroMove::OnMouseOver( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseOver( pObject );
	
	if( HERO )
	{
		CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
		if( pBattle->IsEnemy( HERO, pObject ) )
			CURSOR->SetCursor( eCURSOR_ATTACK );
		else
		{
			if( pObject->GetObjectKind() == eObjectKind_Npc )
			{
				if( ((CNpc*)pObject)->GetNpcKind() != 30 && ((CNpc*)pObject)->GetNpcKind() != 31 )
				CURSOR->SetCursor( eCURSOR_DEALER );
			}
			else
				CURSOR->SetCursor( eCURSOR_DEFAULT );
		}			
	}
}

void CHeroMove::OnMouseLeave( CObject* pObject )
{
	if( !pObject )	return;
	OBJECTMGR->OnMouseLeave( pObject );
}

void CHeroMove::OnClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if(HEROID == pPlayer->GetID()) return;
	
	// RaMa - 05.05.27  -> Guild Tournament에 Observer로 참가한 경우 클릭 안되게..
/*	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleTeamID( HERO ) == 2 )			// Observer
		return;*/

//귓말아이디 등록
	//if( g_UserInput.GetKeyboard()->GetKeyPressed( KEY_MENU ) )
		// 061204 LYW --- Delete Preexistence Chatting Dialog.
		/*
	if( GAMEIN->GetChatDialog() )
	{
		static char str[MAX_NAME_LENGTH+3];
		wsprintf( str, "/%s ", pPlayer->GetObjectName() );
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetEditText( str );
		GAMEIN->GetChatDialog()->GetChatEditBox()->SetFocusEdit( TRUE );
		return;
	}
	*/

//---RClick Menu
	if( MouseEvent == MOUSEEVENT_RCLICK )
	{
		//if( GAMEIN->GetRClickMenuDlg() )
		//{
		//	GAMEIN->GetRClickMenuDlg()->ShowPlayerMenu( TRUE, pPlayer );
		//}
		//090218 modified by xb
	}
	else
	{
		OnAttack( pPlayer, MouseEvent );	//마우스 오른클릭일 경우 공격은 없겠지.
	}
//--------------



/*
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	
	if( pBattle->GetBattleKind() == eBATTLE_KIND_NONE )
	{
		if( g_keyboard.GetKeyPressed( KEY_CONTROL ) )
		{
//			OnAttack( pPlayer, MouseEvent );
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				OBJECTMGR->SetSelectedObject( pPlayer );
		}
	}
	//KES SHOWDOWN 031020 문파전, 비무중일 때
	else if( pBattle->GetBattleState() == eBATTLE_STATE_FIGHT )
	{
		if( pBattle->IsEnemy( HERO, pPlayer ) )
		{
			OnAttack( pPlayer, MouseEvent );
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				OBJECTMGR->SetSelectedObject( pPlayer );
		}
	}
*/
}

void CHeroMove::OnClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	// RaMa - 05.05.27  -> Guild Tournament에 Observer로 참가한 경우 클릭 안되게..
/*	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( BATTLESYSTEM->GetBattle( HERO )->GetBattleTeamID( HERO ) == 2 )			// Observer
		return;*/

#ifdef _TESTCLIENT_
	if(KEYBOARD->GetKeyPressed(KEY_CONTROL))
	{
		if(MouseEvent & MOUSEEVENT_LCLICK)
		{
			TARGETSET Target;
			Target.pTarget = pMonster;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,pMonster->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		else if(MouseEvent == MOUSEEVENT_RCLICK)
		{
			TARGETSET Target;
			Target.pTarget = HERO;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;
			//Target.TargetData = 30;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.MonsterEffect,pMonster,&Target,1,HERO->GetID(),EFFECT_FLAG_HEROATTACK);
		}
		return;
	}
	else
	{
		CActionTarget Target;
		Target.InitActionTarget(pMonster,NULL);
		BOOL bSkill = FALSE;
		if(MouseEvent & MOUSEEVENT_RCLICK)
		{
			bSkill = TRUE;
			SKILLMGR->OnSkillCommand(HERO, &Target, bSkill);
		}
	}
#endif

	OnAttack( pMonster, MouseEvent );
}

void CHeroMove::OnClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	
	BYTE BattleKind = BATTLESYSTEM->GetBattle(HERO)->GetBattleKind();
	if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )	//비무중
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
		CActionTarget Target;
		Target.InitActionTarget( pNpc, NULL );

		CAction MoveAction;
		float fDistance = NPC_TALKING_DISTANCE;
		if( pNpc->GetNpcJob() == SYMBOL_ROLE )
			fDistance = NPC_TALKING_DISTANCE/2;
		MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
		MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pNpc->GetID() )
		{

//			float dist = MOVEMGR->CalcDistanceXZ(pNpc,HERO);
//			if(dist > NPC_TALKING_DISTANCE)
//			{
				CActionTarget Target;
 				Target.InitActionTarget( pNpc, NULL );

				CAction MoveAction;
				float fDistance = NPC_TALKING_DISTANCE;
				if( pNpc->GetNpcJob() == SYMBOL_ROLE )
					fDistance = NPC_TALKING_DISTANCE/2;
				MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
//			}
/*
			else
			{
				switch( pNpc->GetNpcKind() )
				{
				case 31:
					{
						char buf[32] = {0,};
						int nLen = strlen( pNpc->GetObjectName() ) - 6; //"가는길"제외
						strncpy( buf, pNpc->GetObjectName(), nLen );
						buf[nLen] = 0;

						cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_MAPCHANGE, MBT_YESNO, CHATMGR->GetChatMsg(360), buf );

						if( pBox )
						{
							pBox->SetParam( pNpc->GetID() );							
							MOVEMGR->HeroMoveStop();
							HERO->SetState(eObjectState_Deal);
						}
					}
					break;
				case 30:
					{
						cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(361), pNpc->GetObjectName() );
						if( pBox )
						{
							pBox->SetParam( pNpc->GetID() );
							MOVEMGR->HeroMoveStop();
							HERO->SetState(eObjectState_Deal);
						}
					}
					break;
				default:
					{
						NPCSCRIPTMGR->StartNpcScript( pNpc );
						MOVEMGR->HeroMoveStop();
						HERO->SetState(eObjectState_Deal);
					}
					break;
				}

				pNpc->OnClicked();
			}
*/
		}
//		else			
//		{
//			OBJECTMGR->SetSelectedObject( pNpc );
//		}
	}
}

void CHeroMove::OnClickSkillObject(DWORD MouseEvent,cSkillObject* pObj)
{
	OnAttack( pObj, MouseEvent );
}

void CHeroMove::OnClickGround(DWORD MouseEvent,VECTOR3* pPos)
{
#ifdef _TESTCLIENT_

	if(KEYBOARD->GetKeyPressed(KEY_MENU) && KEYBOARD->GetKeyPressed(KEY_SPACE))
	{
		VECTOR3 pos;
		pos.x = pPos->x;
		pos.y = 0;
		pos.z = pPos->z;
		MOVEMGR->SetPosition(HERO,&pos);
	}
	else
	{
		if( MouseEvent & MOUSEEVENT_LCLICK )
			MOVEMGR->SetHeroTarget(pPos);
		else
			MOVEMGR->SetHeroTarget(pPos,FALSE);

		HERO->DisableAutoAttack();
	}
#else

#ifdef _CHEATENABLE_
	if(KEYBOARD->GetKeyPressed(KEY_MENU) && KEYBOARD->GetKeyPressed(KEY_SPACE))
	{
#ifndef _GMTOOL_
		if( CHEATMGR->IsCheatEnable() )	
#endif
		{
			MOVE_POS msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_MOVE_SYN;
			msg.dwObjectID = HEROID;
			msg.dwMoverID = HEROID;
			msg.cpos.Compress(pPos);
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else
#endif	//_CHEATENABLE_
	{
		if(HERO)
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
				MOVEMGR->SetHeroTarget(pPos);
			else
				MOVEMGR->SetHeroTarget(pPos,FALSE);

			HERO->DisableAutoAttack();
			HERO->SetCurComboNum(0);
			HERO->SetMovingAction(NULL);
		}
	}

#endif	//_TESTCLIENT_
}

void CHeroMove::OnClickMapObject(DWORD MouseEvent, CObject* pObj)
{

}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07
void CHeroMove::OnClickFarmRenderObj(DWORD MouseEvent, CSHFarmRenderObj* pObj)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )	//비무중
		return;

	// 집사 대화창이 떠 있으면 리턴~
   	if (g_csFarmManager.CLI_GetTargetObj() && 
 		GAMEIN->GetNpcScriptDialog()->IsActive())
	{
		return;
	}

	// 080515 KTH -- 가축 창이 열려 있으면 리턴.
	if ( GAMEIN->GetAnimalDialog()->IsActive() )
		return;

	if( MouseEvent & MOUSEEVENT_DBLCLICK )
	{
  		CActionTarget Target;
		Target.InitActionTarget( pObj, NULL );

		CAction MoveAction;	

		// 080509 KTH -- 가축 마우스 이벤트 처리
		if( g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind >= CSHFarmManager::RENDER_ANIMAL_KIND &&
			g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind < CSHFarmManager::RENDER_ANIMAL_KIND + CSHFarmManager::RENDER_ANIMAL_KIND_NUM )
		{
			// 거리체크..
			VECTOR3 stHeroPos;
			VECTOR3 stTargetPos;
			HERO->GetPosition(&stHeroPos);
			pObj->GetPosition(&stTargetPos);

			CAnimal* pAnimal = g_csFarmManager.CLI_GetAnimalInfo((CSHFarmZone::FARM_ZONE)g_csFarmManager.CLI_GetTargetObjID()->nFarmZone, g_csFarmManager.CLI_GetTargetObjID()->nFarmID, g_csFarmManager.CLI_GetTargetObjID()->nAnimalCageID, g_csFarmManager.CLI_GetTargetObjID()->nAnimalID);

			float nDistance = CalcDistanceXZ(&stTargetPos, &stHeroPos); 

			if( pAnimal->GetOwner() != HERO->GetID() )
				return;

			// 080515 KTH -- Distance Check Add
			if (nDistance <= CSHFarmManager::PEN_OBJECT_CONTROL_DISTANCE)
			{
				MOVEMGR->SetLookatPos(g_csFarmManager.CLI_GetTargetObj(), &stHeroPos, 0, gCurTime);
				MOVEMGR->SetLookatPos(HERO, &stTargetPos, 0, gCurTime);

				GAMEIN->GetAnimalDialog()->SetActive(TRUE);
				GAMEIN->GetAnimalDialog()->SetAnimal(pAnimal);
			}
			else
			{
				MoveAction.InitAction( eActionKind_Farm, NULL, (float)CSHFarmManager::PEN_OBJECT_CONTROL_DISTANCE, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
			}
		}
		// 080411 KTH -- 더블클릭 했을 경우에서 축사을 하면 팻 입장 다이얼로그를 띠어주는 작업을 해야함... 이쪽이 적합할듯...
		/*if( g_csFarmManager.CLI_GetTargetObj()->GetInfo()->nKind == CSHFarmManager::RENDER_BASIC_ANIMAL_CAGE_SLOT_KIND )
		{
			MoveAction.InitAction( eActionKind_TalkToPet, NULL, (float)CSHFarmManager::PEN_OBJECT_CONTROL_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
		}
		else
		{
			MoveAction.InitAction( eActionKind_TalkToPet, NULL, (float)CSHFarmManager::OBJECT_CONTROL_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );	
		}*/
	}	
	else if( MouseEvent & MOUSEEVENT_LCLICK )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pObj->GetID() )
		{
			CActionTarget Target;
			Target.InitActionTarget( pObj, NULL );

			CAction MoveAction;
			MoveAction.InitAction( eActionKind_Farm, NULL, (float)CSHFarmManager::OBJECT_CONTROL_DISTANCE, &Target );
			MOVEMGR->SetHeroActionMove( &Target, &MoveAction );		
		}
	}
}
// E 농장시스템 추가 added by hseos 2007.05.07


#define ATTACK_TICK	400

void CHeroMove::OnAttack(CObject* pTargetObj,DWORD MouseEvent)
{	
	static CObject* pLastObj		= NULL;
	static DWORD	dwAttackTime	= 0;
	
	if( HERO && pTargetObj )
	{
		if( HERO->GetCharacterTotalInfo()->bVisible )	//안보일때는 공격이 안되도록 수정 //KES
		{
			if( pLastObj != pTargetObj || gCurTime - dwAttackTime > ATTACK_TICK )
			{
				// RaMa - 2005.01.31   -> 죽은애는 공격할 수 없다.
				if( pTargetObj->GetState() == eObjectState_Die )
					return;

				if( MouseEvent & MOUSEEVENT_LCLICK )
				{
					if( OBJECTMGR->GetSelectedObjectID() == pTargetObj->GetID() )
					{
						if( HERO->IsPKMode() )
						if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
						{ //범위공격으로 파티원을 공격할때는 메세지 없게 하기 위해
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
							return;
						}
						
						CActionTarget Target;
						Target.InitActionTarget(pTargetObj,NULL);
						
						HERO->EnableAutoAttack(&Target);		
						SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
						pLastObj		= pTargetObj;
						dwAttackTime	= gCurTime;
					}
				}
				else if( MouseEvent & MOUSEEVENT_DBLCLICK )
				{
					if( HERO->IsPKMode() )
					if( PARTYMGR->IsPartyMember( pTargetObj->GetID() ) )						
					{ //범위공격으로 파티원을 공격할때는 메세지 없게 하기 위해
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(630) );
						return;
					}
					CActionTarget Target;
					Target.InitActionTarget(pTargetObj,NULL);
					
					HERO->EnableAutoAttack(&Target);		
					SKILLMGR->OnSkillCommand(HERO, &Target, FALSE);	
					pLastObj		= pTargetObj;
					dwAttackTime	= gCurTime;
				}
			}
		}
	}
}

// LBS 노점상 관련 03.09.30
void CHeroMove::OnDbClickPlayer(DWORD MouseEvent,CPlayer* pPlayer)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	if( OBJECTMGR->GetSelectedObjectID() )
	{
		if( OBJECTMGR->GetSelectedObjectID() == pPlayer->GetID() )
		{
			//노점상일 경우에만 메세지를 보내자.
			if( pPlayer->GetState() == eObjectState_StreetStall_Owner )
			{
				CActionTarget Target;
				Target.InitActionTarget( pPlayer, NULL );

				CAction MoveAction;
				MoveAction.InitStallAction( STREETSTALL_ENTER_DISTANCE, &Target );
				MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
			}
			else
			{
				OnAttack( pPlayer, MOUSEEVENT_DBLCLICK );
			}
		}
	}
}

void CHeroMove::OnDbClickMonster(DWORD MouseEvent,CMonster* pMonster)
{
	OnAttack( pMonster, MouseEvent );
}

void CHeroMove::OnDbClickPet(DWORD MouseEvent,CPet* pPet)
{
	OnAttack( pPet, MouseEvent );
}

void CHeroMove::OnDbClickNpc(DWORD MouseEvent, CNpc* pNpc)
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;

	CActionTarget Target;
	Target.InitActionTarget( pNpc, NULL );
				
	CAction MoveAction;
	float fDistance = NPC_TALKING_DISTANCE;
	if( pNpc->GetNpcJob() == SYMBOL_ROLE )
		fDistance = NPC_TALKING_DISTANCE/2;
	MoveAction.InitAction( eActionKind_TalkToNpc, NULL, fDistance, &Target );
	MOVEMGR->SetHeroActionMove( &Target, &MoveAction );
}

void CHeroMove::AttackSelectedObject()
{
	CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
	if( !pTargetObj ) return;

	OnAttack(pTargetObj,MOUSEEVENT_LCLICK);
}

// LBS 03.10.09
void CHeroMove::SelectLoginPoint( WORD LoginPointIndex )
{
	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_SAVEPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = LoginPointIndex;
				
	NETWORK->Send(&msg, sizeof(msg));

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(207) );

	HERO->GetCharacterTotalInfo()->LoginPoint_Index = LoginPointIndex;
}


void CHeroMove::MapChange( WORD MapIndex, DWORD ItemIdx, DWORD ItemPos )
{

	MSG_DWORD3 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = MapIndex;
	msg.dwData2 = ItemIdx;
	msg.dwData3 = ItemPos;
	
	NETWORK->Send(&msg, sizeof(msg));	
	
	g_UserInput.SetInputFocus(FALSE);
}

// 070917 LYW --- HeroMove : Add function to process change map through the npc.
void CHeroMove::MapChangeToNpc(DWORD dwMapNum, float xPos, float zPos)
{
	MSG_DWORD3 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_NPC_CHANGEMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = dwMapNum;
	msg.dwData2 = (DWORD)xPos;
	msg.dwData3 = (DWORD)zPos;
	
	NETWORK->Send(&msg, sizeof(msg));	
	
	g_UserInput.SetInputFocus(FALSE);
}

// 070121 LYW --- Add fucntion to change map.
void CHeroMove::ChangeMap( WORD MapIndex )
{
	//---KES 071007 메세지 연속으로 보내지는 것 방지
	static DWORD dwLastChangeTime = 0;

	if( gCurTime - dwLastChangeTime < 3000 )	return;	//3초 이내는 보내지 않는다.
	else dwLastChangeTime = gCurTime;
	//-----------------------------------------------

	MSG_DWORD msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CHANGEMAP_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData		= MapIndex ;
	
	NETWORK->Send(&msg, sizeof(msg));	
	
	g_UserInput.SetInputFocus(FALSE);
}
