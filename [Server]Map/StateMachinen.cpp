// StateMachinen.cpp: implementation of the CStateMachinen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StateMachinen.h"
#include "MsgRouter.h"
#include "AIParam.h"
#include "Monster.h"
#include "CharMove.h"
#include "AISystem.h"
#include "PackedData.h"
#include "../[cc]skill/server/info/activeskillinfo.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "BossMonsterManager.h"
#include "RecallManager.h"
#include "AIGroupManager.h"
#include "RegenPrototype.h"
#include "Player.h"
#include "AIManager.h"
#include "HelpRequestManager.h"
#include "TileManager.h"
#include "cMonsterSpeechManager.h"
#include "../[cc]skill/server/status/status.h"
// 080616 LUJ, 함정 추가
#include "Trap.h"
#include "UserTable.h"

CStateMachinen GSTATEMACHINE;

CStateMachinen::CStateMachinen()
{

}

CStateMachinen::~CStateMachinen()
{

}
void CStateMachinen::RecvMsg( CObject * pSrcObject, CObject * pDestObject, MSG_OBJECT * msg)
{
	CStateParam * srcPr = &((CMonster*)pSrcObject)->m_stateParam;
	CStateParam * destPr = &((CMonster*)pDestObject)->m_stateParam;

	BEGIN_COMMON_MSG(msg->msgID)
		MSGOF(eMK_HelpShout)
		{
			((CMonster *)pDestObject)->AddChat("Help!");
		}
		MSGOF(eMK_Recall_Script)
		{
			RECALLINFO * pRecallInfo = ((SMONSTER_LIST*)((CMonster *)pDestObject)->GetSMonsterList())->RecallInfoClass.GetRecallInfo(msg->dwData);
			RECALLMGR->RecallObject((CMonster*)pSrcObject, pRecallInfo);
		}
		MSGOF(eMK_HelpObey)
		{
			CMonster *pTargetMonster = (CMonster *)pDestObject;
			if((CPlayer *)((CMonster *)pSrcObject)->GetTObject())
			{
//				CHelpRequestManager::SetHelperMonster((CMonster *)pDestObject, (CPlayer *)((CMonster *)pSrcObject)->GetTObject());
				//SW050901
				CHelpRequestManager::SetHelperMonster((CMonster*)pSrcObject, (CMonster *)pDestObject, (CPlayer *)((CMonster *)pSrcObject)->GetTObject());
			}
		}
		MSGOF(eMK_HelpRequest)
		{	
			CMonster *pTargetMonster = (CMonster *)pDestObject;
			
			if( destPr->stateCur == eMA_STAND || destPr->stateCur == eMA_WALKAROUND )
			{
				CRegenObject * pORInfo = GROUPMGR->GetRegenObject(pTargetMonster->GetMonsterGroupNum(), pTargetMonster->GetGridID(), pTargetMonster->GetID());
				if(pORInfo)
				if(pORInfo->IsHearing())
				{
					VECTOR3 ObjectPos	= *CCharMove::GetPosition(pSrcObject);
					VECTOR3 TObjectPos	= *CCharMove::GetPosition(pDestObject);
					DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
					if(Distance < pORInfo->GetHearingDistance() && ((CMonster *)pSrcObject)->GetTObject())
					{
//						CHelpRequestManager::SetHelperMonster((CMonster *)pDestObject, (CPlayer *)((CMonster *)pSrcObject)->GetTObject());
						//SW050901
						CHelpRequestManager::SetHelperMonster((CMonster*)pSrcObject, pTargetMonster, (CPlayer *)((CMonster *)pSrcObject)->GetTObject());
					}
				}
			}
		}
	END_COMMON_MSG
}
void CStateMachinen::SetState(CObject * obj, DWORD newState)
{
	CStateParam * pr = &((CMonster*)obj)->m_stateParam;

	if( pr->stateCur != newState )
	{
		if(newState != eMA_STAND || newState != eMA_ATTACK || newState != eMA_REST)
		{	
			MSG_WORD msg;
			msg.Category = MP_MOVE;
			msg.Protocol = MP_MOVE_MONSTERMOVE_NOTIFY;
			msg.dwObjectID = obj->GetID();
			msg.wData = (WORD)newState;
			PACKEDDATA_OBJ->QuickSend(obj,&msg,sizeof(msg));
		}
	}

	pr->stateNew		= newState;
	pr->bStateUpdate	= TRUE;
}

void CStateMachinen::Process(CObject * obj, eStateEvent evt, MSG_OBJECT * msg)
{
	CStateParam * pr = &((CMonster*)obj)->m_stateParam;
	CMonster * po = (CMonster *)obj;
	if( po->GetState() == eObjectState_Die )
	{
		return;
	}


	if(pr->bStateUpdate)
	{
		pr->bStateUpdate = FALSE;
		Update( obj, eSEVENT_Leave, NULL );
		pr->stateOld = pr->stateCur;
		pr->stateCur = pr->stateNew;
		pr->stateNew = 0;
		Update( obj, eSEVENT_Enter, NULL );
	}

	Update( obj, evt, msg );
	SubProcess( obj, evt, msg );
}

void CStateMachinen::SubProcess( CObject * obj, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)obj;
	CStateParam * pr = &((CMonster*)obj)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)obj)->m_pSInfo;
	CAIParam * aprm = &((CMonster*)obj)->m_aiParam;
	if((apr == 0) || (aprm == 0) || (pr == 0) || (po == 0))
		return;
	BEGIN_COMMON_MSG(msg->msgID)
		MSGOF(eMK_Chat)
		MSGOF(eMK_Recall_Script)
				//CRespawnObject * respObj = RESPAWNMGR->GetRespawnObject( msg->flag );
				//respObj->ForceGenerate();
	END_COMMON_MSG
	
	if( po->GetAbnormalStatus()->IsStun ||
		po->GetAbnormalStatus()->IsFreezing ||
		po->GetAbnormalStatus()->IsStone ||
		po->GetAbnormalStatus()->IsSlip ||
		po->GetAbnormalStatus()->IsParalysis )
	{
//		po->DoStand();
		return;
	}

	switch( pr->stateCur )
	{
	case eMA_STAND:
	case eMA_WALKAROUND:
		{
			CPlayer* player = 0;

			CTrap* trap = ( CTrap* )po;

			if( apr->bForeAttack && aprm->SearchLastTime + apr->SearchPeriodicTime < gCurTime )
			{
				aprm->SearchLastTime		= gCurTime;
				// 080616 LUJ, 근접한 플레이어를 찾는다
				player						= ( CPlayer* )( trap->DoSearch() );
			}
			// 250 :  충돌시 Search하는 Delay
			else if(apr->AttackNum && aprm->CollSearchLastTime + 250 < gCurTime )
			{
				aprm->CollSearchLastTime	= gCurTime;
				// 080616 LUJ, 충돌한 플레이어를 찾는다
				player						= ( CPlayer* )( trap->OnCollisionObject() );
			}
			// 080616 LUJ, 조건절에 해당되지 않으면 다음 프로세스를 기다린다
			else
			{
				break;
			}

			switch( po->GetObjectKind() )
			{
				// 080616 LUJ, 함정은 몬스터도 공격이 가능하므로 별도 처리한다
			case eObjectKind_Trap:
				{
					// 080616 LUJ, 플레이어를 우선적으로 공격하므로, 플레이어가 타겟이면 더 처리하지 않는다
					// 080708 LUJ, 기획팀(이호섭)과의 협의로 함정 공격 대상을 적으로 한정함
					if(	player &&
						trap->IsEnemy( player->GetID() ) )
					{
						trap->SetTObject( player );
						SetState( trap, eMA_ATTACK );
                        break;
					}

					const BASE_MONSTER_LIST*	monsterList = trap->GetSMonsterList();
					// 080616 LUJ, 목표 찾기는 탐색 범위를 이용한다
					CObject*					monster		= trap->DoFriendSearch( monsterList ? monsterList->SearchRange : 0 );

					SetState(
						trap,
						trap->SetTObject( monster ) ? eMA_ATTACK : eMA_STAND );
					break;
				}
			default:
				{
					if(	!	player											||
						!	player->IsVisible()								||
							player->GetObjectKind()	!= eObjectKind_Player	||
							player->GetGridID()		!= po->GetGridID()		||
							player->GetState()		== eObjectState_Die		||
						!	po->SetTObject( player ) )
					{
						break;
					}

					SetState( po, eMA_PERSUIT );
					break;
				}
			}
		}
		break;
	}
}

void CStateMachinen::Update(CObject * obj, eStateEvent evt, MSG_OBJECT * msg)
{
	CMonster * po = (CMonster *)obj;
	CStateParam * pr = &((CMonster*)obj)->m_stateParam;

	
	BEGIN_STATE(pr->stateCur)


	STATEOF(eMA_STAND)
		DoStand( po, evt, msg );
	STATEOF(eMA_WALKAROUND)	
		DoWalkAround( po, evt, msg );
	STATEOF(eMA_PERSUIT)
		DoPursuit( po, evt, msg );
	STATEOF(eMA_RUNAWAY)
		DoRunAway( po, evt, msg );
	STATEOF(eMA_ATTACK)
		DoAttack( po, evt, msg );
	STATEOF(eMA_REST)	
		DoRest( po, evt, msg );

	END_STATE
	
}

void CStateMachinen::DoRest( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;

BEGIN_EVENT(evt)
	OnEnter
	OnLeave
	OnMsg	
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnProcess

END_EVENT
}
void CStateMachinen::DoStand( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;
	MonSpeechInfo* pTemp = NULL;

BEGIN_EVENT(evt)
	OnEnter
		po->SetTObject(NULL);
		pr->stateStartTime	= gCurTime;
		pr->stateEndTime	= pr->stateStartTime + apr->StandTime /*+정지유지 시간*/;
		po->DoStand();

//---KES 071120
		if( po->IsNoCheckCollision() )
		{
			VECTOR3 vPos;
			po->GetPosition( &vPos );
			if( !g_pServerSystem->GetMap()->CollisionTilePos( vPos.x, vPos.z, g_pServerSystem->GetMapNum() ) ) //TTB가 충돌영역이 아닌경우
				po->SetNoCheckCollision( FALSE );	//Check TTB again
		}
//-------------
	OnLeave
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnMsg	
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnProcess
		if( pr->stateEndTime < gCurTime )
		{
			int rate = rand()%100;
			if(0 <= rate && rate < apr->StandRate )
			{
				SetState( po, eMA_STAND );
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( po->GetMonsterKind(), eMon_Speech_KeepWalkAround );
			}
			else
			{
				SetState( po, eMA_WALKAROUND );
				//SW051101
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( po->GetMonsterKind(), eMon_Speech_WalkAround );
			}
			
			if( pTemp )
				po->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );				
		}
END_EVENT
}

void CStateMachinen::DoWalkAround( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{	
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;
	MonSpeechInfo* pTemp = NULL;
	
	if( po->GetAbnormalStatus()->IsStun ||
		po->GetAbnormalStatus()->IsFreezing ||
		po->GetAbnormalStatus()->IsStone ||
		po->GetAbnormalStatus()->IsSlip ||
		po->GetAbnormalStatus()->IsMoveStop ||
		po->GetAbnormalStatus()->IsParalysis )

	{
		SetState( po, eMA_STAND );
		return;
	}

BEGIN_EVENT(evt)
	OnEnter	
		po->SetTObject(NULL);
		po->DoWalkAround();
		pr->stateStartTime	= gCurTime;
		pr->stateEndTime	= pr->stateStartTime + CCharMove::GetMoveEstimateTime(po);

//---KES 071120 //po->DoWalkAround() 아래에 위치해야한다. 현재 포지션으로 찾아서 하는 것은 오류가 있을 수 있다. 일단 주석.
//		if( po->IsNoCheckCollision() )
//		{
//			VECTOR3 vPos;
//			po->GetPosition( &vPos );
//			if( !g_pServerSystem->GetMap()->CollisionTilePos( vPos.x, vPos.z, po) ) //TTB가 충돌영역이 아닌경우
//				po->SetNoCheckCollision( FALSE );	//Check TTB again
//		}
//-------------

	OnLeave	
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnMsg
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnProcess
		if( pr->stateEndTime < gCurTime )
		{
			int rate = rand()%100;
			if(0 <= rate && rate < apr->StandRate )
			{
				SetState( po, eMA_STAND );
				//SW051101
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( po->GetMonsterKind(), eMon_Speech_Stand );
			}
			else
			{
				SetState( po, eMA_WALKAROUND );
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( po->GetMonsterKind(), eMon_Speech_KeepStand );
			}
			
			if( pTemp )
				po->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );
		}
END_EVENT
}


void CStateMachinen::DoPursuit( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;
	CAIParam * aprm = &((CMonster*)pObject)->m_aiParam;
	
	// yh 뻑나서 고침(황릉보물이 추격모드... -_-)
	if(apr->SkillInfo[aprm->GetCurAttackKind()] == NULL)
	{
		ASSERT(0);
		SetState(pObject, eMA_STAND);
		return;
	}

	if( !po->GetTObject() || (po->GetTObject() && ( po->GetTObject()->GetState() == eObjectState_Die ||  po->GetTObject()->GetInited() == FALSE ) ) )
	{
		SetState(pObject, eMA_STAND);
		return;
	}
	
	if( po->GetAbnormalStatus()->IsStun ||
		po->GetAbnormalStatus()->IsFreezing ||
		po->GetAbnormalStatus()->IsStone ||
		po->GetAbnormalStatus()->IsSlip ||
		po->GetAbnormalStatus()->IsMoveStop ||
		po->GetAbnormalStatus()->IsParalysis )
	{
//		SetState( po, eMA_STAND );
		return;
	}
	
BEGIN_EVENT(evt)
	OnEnter	
//		apr->PursuitForgiveStartTime = gCurTime;
		aprm->PursuitForgiveStartTime = gCurTime;
		pr->stateStartTime	= gCurTime;
		pr->stateEndTime	= pr->stateStartTime + 1000;
		po->DoPursuit();
		CAIManager::RandCurAttackKind(apr, aprm);

//---KES 071120
		po->SetNoCheckCollision( TRUE );	//Ignore TTB
//-------------

	OnLeave
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnMsg
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnProcess	
		VECTOR3 ObjectPos	= *CCharMove::GetPosition(po);
		VECTOR3 TObjectPos	= *CCharMove::GetPosition(po->GetTObject());
		DWORD	Distance = 0;

		float fDist = CalcDistanceXZ( &ObjectPos, &TObjectPos ) - po->GetRadius();

		if( fDist > 0.f )
		{
			Distance = (DWORD)fDist;
		}
//		if( aprm->prePursuitForgiveTime == 0 && ( apr->PursuitForgiveStartTime + apr->PursuitForgiveTime < gCurTime || apr->PursuitForgiveDistance < Distance ) )
		if( aprm->prePursuitForgiveTime == 0 && ( aprm->PursuitForgiveStartTime + apr->PursuitForgiveTime < gCurTime || apr->PursuitForgiveDistance < Distance ) )
		{
			//---KES Aggro 070918
			//---추적 포기로 모든 어그로 없앰
			po->RemoveAllAggro();
			//-------------------

			aprm->SearchLastTime = gCurTime + 3000; //3초간 선공몹 서치를 안하게 하기 위해서.
			SetState( po, eMA_WALKAROUND );

			//SW050902
			MonSpeechInfo* pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( po->GetMonsterKind(), eMon_Speech_ForgivePursuit );
			if( pTemp )
				po->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );	
		}	
		else if( Distance < apr->SkillInfo[aprm->GetCurAttackKind()]->GetSkillInfo()->Range )
		{
			SetState( po, eMA_ATTACK );
			//aprm->PursuitForgiveStartTime = gCurTime;
		}
		else
		{
//			if( aprm->prePursuitForgiveTime != 0 && apr->PursuitForgiveStartTime + aprm->prePursuitForgiveTime < gCurTime )
			if( aprm->prePursuitForgiveTime != 0 && aprm->PursuitForgiveStartTime + aprm->prePursuitForgiveTime < gCurTime )
			{
				//---KES Aggro 070918
				//---추적 포기로 모든 어그로 없앰
				po->RemoveAllAggro();
				//-------------------

				aprm->prePursuitForgiveTime = 0;
				aprm->SearchLastTime = gCurTime + 3000; //3초간 선공몹 서치를 안하게 하기 위해서.
				SetState( po, eMA_WALKAROUND );
			}
			//else if( Distance < apr->SkillInfo[aprm->GetCurAttackKind()]->GetSkillInfo()->Range )
			//{
			//	SetState( po, eMA_ATTACK );
				//aprm->PursuitForgiveStartTime = gCurTime;
			//}
			else
			{
				if( pr->stateEndTime < gCurTime )
				{
					pr->stateEndTime = gCurTime + 1000; 	//1초마다만 추격한다.

					po->DoPursuit();
					//추격하면서 공격패턴을 바꾼다. //KES 부하가 있으려나?
					CAIManager::RandCurAttackKind(apr, aprm);
				}
			}
		}
END_EVENT
}

void CStateMachinen::DoAttack( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;
	CAIParam * aprm = &((CMonster*)pObject)->m_aiParam;

	// targetobject가 NULL인경우가 나옴!!
	if( !po->GetTObject() || (po->GetTObject() && ( po->GetTObject()->GetState() == eObjectState_Die ||  po->GetTObject()->GetInited() == FALSE ) ) )
	{
		SetState(pObject, eMA_STAND);
		return;
	}
	
	if( po->GetAbnormalStatus()->IsStun ||
		po->GetAbnormalStatus()->IsFreezing ||
		po->GetAbnormalStatus()->IsStone ||
		po->GetAbnormalStatus()->IsSlip ||
		po->GetAbnormalStatus()->IsParalysis )
	{
//		SetState( po, eMA_STAND );
		return;
	}
BEGIN_EVENT(evt)
	OnEnter
		pr->stateStartTime	= gCurTime;
		pr->stateEndTime	= 0;
	OnLeave		
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnMsg
	/////////////////////////////////////
	////////////  NULL  /////////////////
	/////////////////////////////////////
	OnProcess
		if(aprm->CurAttackKind < apr->AttackNum)
		{	
			if(apr->SkillInfo[aprm->GetCurAttackKind()]->GetSkillInfo()->CoolTime + aprm->AttackStartTime < gCurTime)
			{		
				VECTOR3 ObjectPos	= *CCharMove::GetPosition(po);
				VECTOR3 TObjectPos	= *CCharMove::GetPosition(po->GetTObject());
				DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
				if( Distance > apr->SkillInfo[aprm->GetCurAttackKind()]->GetSkillInfo()->Range )	//추적하다 멈칫 멈칫 하는 이유는 여기에 있다.
				{
					SetState(po, eMA_PERSUIT);
				}
				else
				{
					aprm->AttackStartTime = gCurTime;

					po->DoAttack( (DWORD)apr->AttackIndex[aprm->GetCurAttackKind()] );
					CAIManager::RandCurAttackKind(apr, aprm);
				}
			}
		}
		else
		{
			SetState(po, eMA_STAND);
		}	
END_EVENT
}

void CStateMachinen::DoRunAway( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg )
{
	CMonster * po = (CMonster *)pObject;
	CStateParam * pr = &((CMonster*)pObject)->m_stateParam;
	BASE_MONSTER_LIST	* apr = ((CMonster*)pObject)->m_pSInfo;
	CAIParam * aprm = &((CMonster*)pObject)->m_aiParam;
	
	if( po->GetAbnormalStatus()->IsStun ||
		po->GetAbnormalStatus()->IsFreezing ||
		po->GetAbnormalStatus()->IsStone ||
		po->GetAbnormalStatus()->IsSlip ||
		po->GetAbnormalStatus()->IsMoveStop ||
		po->GetAbnormalStatus()->IsParalysis )
	{
//		SetState( po, eMA_STAND );
		return;
	}

BEGIN_EVENT(evt)
	OnEnter
		po->SetTObject(NULL);
		po->DoRunaway();
		pr->stateStartTime	= gCurTime;
		pr->stateMidTime	= 1000;
		pr->stateEndTime	= pr->stateStartTime + CCharMove::GetMoveEstimateTime(po) + 1000;
	OnLeave		
	OnMsg
	OnProcess
		if( pr->stateEndTime < gCurTime )
		{
			switch(aprm->RunawayType)
			{
			case eRunawayType_Help:
				{
					
					ASSERT(aprm->pTarget);
					po->SetTObject(aprm->pTarget);
					aprm->pHelperMonster->m_aiParam.prePursuitForgiveTime = 20000;
					aprm->prePursuitForgiveTime = 20000;
					SetState(po, eMA_PERSUIT);

//					CHelpRequestManager::SetHelperMonster(aprm->pHelperMonster, aprm->pTarget);
					//SW050901
					CHelpRequestManager::SetHelperMonster(po, aprm->pHelperMonster, aprm->pTarget);

					//ASSERT(po != aprm->pHelperMonster);
					//CMonster * pHelper = (CMonster *)g_pUserTable->FindUser(aprm->pHelperMonster->GetID());
					//if(!pSrcObj) return;
				}
				break;
			case eRunawayType_General:
				{
					int rate = rand()%100;
					if(0 <= rate && rate < apr->StandRate )
					{
						SetState( po, eMA_STAND );
					}
					else
					{
						SetState( po, eMA_WALKAROUND );
					}
				}
				break;
			}
			//aprm->RunawayType = eRunawayType_Terminate;
			aprm->RunawayType = eRunawayType_None;
		}
		else
		{
			if(pr->stateStartTime + pr->stateMidTime < gCurTime)	
			{
				pr->stateMidTime += 1000;

				switch(aprm->RunawayType)
				{
				case eRunawayType_Help:
					{
						po->DoPursuit(aprm->pHelperMonster);
					}
					break;
				}
			}
		}
END_EVENT
}

