// BossMonster.cpp: implementation of the CBossMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossMonster.h"
#include "BossMonsterManager.h"
#include "BossMonsterInfo.h"
#include "AIParam.h"
#include "../[cc]skill/server/info/SkillInfo.h"
#include "CharMove.h"
#include "PackedData.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "CharacterCalcManager.h"
#include "MHError.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "RegenNpc.h"
#include "ObjectStateManager.h"
#include "Player.h"
#include "UserTable.h"
#include "AIManager.h"
#include "GridSystem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "../[cc]skill/server/info/ActiveSkillInfo.h"

CBossMonster::CBossMonster()
{
}

CBossMonster::~CBossMonster()
{
}

BOOL CBossMonster::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CMonster::Init(kind, AgentNum, pBaseObjectInfo);
	
	/// 06. 08. 2차 보스 - 이영준
	/*m_CurAttackIdx = 1601;*/
	m_AttackStartTime = 0 ;
	m_EventActionValue = 0;
	m_BossEventState = NULL;
	m_IsEventStating = FALSE;
	DistributeDamageInit();
	m_bOpenning = TRUE;
	
	BOSSMONMGR->AddBossMonster(this);

	return TRUE;
}

void CBossMonster::DoDie(CObject* pAttacker)
{
	// Log 요청
	//m_Distributer.m_DamageObjectTableParty.SetPositionHead();

	MHERROR->OutputFile("BossMonsterLog.txt", MHERROR->GetStringArg("MonsterKind:%u, LastKillerID:%u, ", this->GetMonsterKind, this->GetLastKillerID));

	/*
	DAMAGEOBJ* pDamageObj = NULL;
	while( m_Distributer.m_DamageObjectTableParty.GetData() )
	{
		MHERROR->OutputFile("BossMonsterLog.txt", MHERROR->GetStringArg("PartyID:%u, PartyDamage:%u",pDamageObj->dwID, pDamageObj->dwData));
	}
	pDamageObj = NULL;
	m_Distributer.m_DamageObjectTableSolo.GetPositionHead();
	while( pDamageObj = m_Distributer.m_DamageObjectTableSolo.GetData() )
	{
		MHERROR->OutputFile("BossMonsterLog.txt", MHERROR->GetStringArg("PlayerID:%u, PlayerDamage:%u",pDamageObj->dwID, pDamageObj->dwData));
	}*/

	// 보물 소환 
	BOSSMONMGR->RegenGroup(this, GetSummonFileNum(), m_pBossMonsterInfo->GetDieGroupID());
	
	// 리젠 정보 등록
	BOSSMONMGR->SetBossRandRegenChannel(GetMonsterKind(), GetGridID(), GetMonsterGroupNum());

	// 080212 LYW --- BossMonster : 경험치나 아이템 분배는 Distributer에서 짜여진 순서를 따르도록 처리한다.
	//DistributePerDamage();
	BOSSMONMGR->DeleteBossMonster(this);
//	MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Boss Next Regen = GridID: %d, CurTime: %d, RegenDelay: %d ", GetGridID(), gCurTime, delaytime));

//	g_Console.LOG(4, "BossMonster Die : GROUPID: %d, ID: %d, NAME: %s", GetMonsterGroupNum(), GetID(), m_pSInfo->Name);
	
//	CMonster::DoDie(pAttacker);
	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();
	REGENNPC_OBJ->ReleaseMonsterGroup(((CMonster*)this)->GetMonsterGroupNum());
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME);
	SetTObject(NULL);
	SetLastAttackPlayer( NULL );

//---KES Aggro 070918
//---어그로 관련 변수들 릴리스	//죽은 후에 여기로 들어오나?
	RemoveAllAggro();
//-------------------
}

void CBossMonster::Release()
{
	if(m_BossEventState)
	{
		delete [] m_BossEventState;
		m_BossEventState = NULL;
	}
	CMonster::Release();
}

void CBossMonster::SetBossInfo(CBossMonsterInfo* pInfo)
{
	m_pBossMonsterInfo = pInfo;
	m_CurAttackIdx = pInfo->GetFirstAttackIdx();
	m_AttackStartTime = 0 ;
	m_EventActionValue = 0;
	
	BOSSEVENTSTATE* pEventState = pInfo->GetEventStateInfo();
	if(pInfo->GetMaxEventStateNum() == 0)
	{
		ASSERTMSG(0, "BossEventState Num is Zero");
	}
	else
	{		
		m_BossEventState = new BOSSEVENTSTATE[pInfo->GetMaxEventStateNum()];
		for(int i=0; i<pInfo->GetMaxEventStateNum(); ++i)
		{
			m_BossEventState[i].Action = pEventState[i].Action;
			m_BossEventState[i].ActionValue = pEventState[i].ActionValue;
			m_BossEventState[i].Condition = pEventState[i].Condition;
			m_BossEventState[i].ConditionValue = pEventState[i].ConditionValue;
			m_BossEventState[i].Count = pEventState[i].Count;
		}
	}
	m_BossState.Init();
	m_IsEventStating = FALSE;

	m_bDelete = FALSE;
}

void CBossMonster::SetLife(DWORD Life, BOOL bSendMsg)
{
	CMonster::SetLife(Life, bSendMsg);	
	
	if(m_BossEventState == 0)
	{
		ASSERTMSG(0, "Boss SetLife EventState is NULL");
	}
	else
	{
		OnLifeEvent(Life);		
	}
	
	MSG_DWORD2 msg;
	msg.Category = MP_BOSSMONSTER;
	msg.Protocol = MP_BOSS_LIFE_NOTIFY;
	msg.dwData1 = GetLife();
	msg.dwData2 = GetMonsterKind();
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}

void CBossMonster::DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	//CMonster::DoDamage(pAttacker, pDamageInfo, beforeLife);
	
	if( pAttacker->GetGridID() != GetGridID() )
		return;
	
	SetObjectBattleState( eObjectBattleState_Battle );
	if ( pAttacker )
	{
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	}

	if( GetAbnormalStatus()->IsSlip )
	{
		EndBuffSkillByStatus( eStatusKind_Slip );
	}
	
	if( pDamageInfo->RealDamage > beforeLife )
		pDamageInfo->RealDamage = (WORD)beforeLife;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		AddDamageObject( (CPlayer*)pAttacker, pDamageInfo->RealDamage, 0 );
		// 080108 KTH -- 타겟이 변경된 경우, BossState가 WalkAround일 경우에만 타겟을 잡아 준다. 그렇지 않을 경우는 아래 어그로에서 타겟을 관리한다.
		if( CAIManager::IsTargetChange((CPlayer*)pAttacker, this) && m_BossState.GetCurState() == eBossState_WalkAround )
		{
			SetTargetObject((CObject*)pAttacker);
			if(m_pTObject)
			{
				VECTOR3 TObjectPos = *CCharMove::GetPosition(m_pTObject);
				OnMove(&TObjectPos);
			}
		}
//---KES Aggro 070918
//---어그로 수치를 올린다.
		CalcAggro( (CPlayer*)pAttacker, pDamageInfo->RealDamage );
//---

//---KES Aggro 070918 타겟 지정 방식 변경으로 주석처리		

		/*if( CAIManager::IsTargetChange((CPlayer*)pAttacker, this) )
		{
			SetLastAttackPlayer( (CPlayer*)pAttacker );
			SetTObject((CPlayer*)pAttacker);
		}*/

//-------------------
	}
	
	if(m_BossState.GetCurState() != eBossState_Attack)
	{		
		if(m_BossState.GetStateEndTime() > gCurTime)
			return;
		SetBossState(eBossState_Attack);
	}	
}

void CBossMonster::Process()
{
	CMonster::Process();	//  자동소멸을 확인하기 위해 먼저호출한다.

	if( GetState() == eObjectState_Die )	return;

	if(m_BossState.IsStateUpdated())
	{		
		m_BossState.UpdateState();
	}
	
	ySWITCH(m_BossState.GetCurState())
	yCASE(eBossState_Attack)
	Attack();
	yCASE(eBossState_Pursuit)
	Pursuit();
	yCASE(eBossState_WalkAround)
	WalkAround();
	yCASE(eBossState_Stand)
	Stand();
	yCASE(eBossState_Recover)
	Recover();
	yCASE(eBossState_Summon)
	Summon();

	yENDSWITCH
}

void CBossMonster::Attack()
{	
	m_bDelete = FALSE;

	////////////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 보스의 타겟팅 방식이 매회 랜덤 결정일 경우 타겟을 새로 잡는다
	if( m_pBossMonsterInfo->GetTargetingType() == 1 )
	{
		CObject* pTarget = 	g_pServerSystem->GetGridSystem()->FindPlayerRandom(this);

		if(pTarget)
			SetTargetObject(pTarget);
	}
	////////////////////////////////////////////////////////////////////////////////

	if(( !GetTObject()) || (GetTObject() && ( GetTObject()->GetState() == eObjectState_Die ||  GetTObject()->GetInited() == FALSE ) ) )
	{
		if(m_BossState.GetStateEndTime() < gCurTime)
		{
			SetBossState(eBossState_WalkAround);
		}
		return;
	}
	if(m_BossState.IsStateFirst())
	{
		m_BossState.SetStateFirst(FALSE);
		m_AttackStartTime = gCurTime;
		cActiveSkillInfo*	pSkillInfo  = (cActiveSkillInfo*)(SKILLMGR->GetSkillInfo(m_CurAttackIdx));
		DWORD delay = pSkillInfo->GetSkillInfo()->AnimationTime + 400;

		// 080108 KTH -- 스킬 거리 체크
		VECTOR3 TObjectPos	= *CCharMove::GetPosition(GetTObject());
		if( !pSkillInfo->IsInSkillRange(this, &TObjectPos, GetTObject()->GetRadius() ) )
		{
			SetBossState(eBossState_Pursuit);
			return;
		}
		/*if( !pSkillInfo->IsInRange(*this, TObjectPos, GetTObject()->GetRadius() ) )
		{
			SetBossState(eBossState_Pursuit);
			return;
		}*/
		//DWORD delay = 30;//SKILLMGR->GetSkillInfo(m_CurAttackIdx);
		m_BossState.SetStateEndTime(gCurTime+delay);
		DoAttack();
		ChangeCurAttackIdx();

		MSG_DWORD msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_BOSS_STAND_END_NOTIFY;
		msg.dwData = GetMonsterKind();
				
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
				
		return;
	}
	else if(m_pBossMonsterInfo->IsAttackEmpty() == FALSE)
	{
		cActiveSkillInfo* pSkillInfo = (cActiveSkillInfo*)SKILLMGR->GetSkillInfo(m_CurAttackIdx);
		if(m_BossState.GetStateEndTime()  < gCurTime)
		{		
			VECTOR3 ObjectPos	= *CCharMove::GetPosition(this);
			VECTOR3 TObjectPos	= *CCharMove::GetPosition(GetTObject());
			//DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
			
			// 080108 KTH -- 스킬 거리 체크
			if( !pSkillInfo->IsInSkillRange(this, &TObjectPos, GetTObject()->GetRadius() ) )
			{
				SetBossState(eBossState_Pursuit);
			}
			/*if( !pSkillInfo->IsInRange(*this, TObjectPos, GetTObject()->GetRadius()))
			{
				SetBossState(eBossState_Pursuit);
			}*/
			else
			{				
				m_AttackStartTime = gCurTime;
				DWORD delay = ((cActiveSkillInfo*)(SKILLMGR->GetSkillInfo(m_CurAttackIdx)))->GetSkillInfo()->AnimationTime+400;//SKILLMGR->GetSkillInfo((WORD)m_CurAttackIdx)->GetDelayTime();
				m_BossState.SetStateEndTime(gCurTime+delay);

				if((m_CurAttackIdx == 1614))
					ChangeTargetForSkill(&ObjectPos);

				///////////////////////////////////////////////////////////////////////
				/// 06. 08. 2차 보스 - 이영준
				/// 안전 거리
				/// 안전 거리가 있는 스킬은 스킬 범위 안에 있는 타겟으로 설정해야 한다.
				//if(pSkillInfo->GetSafeRange())
				/*if(pSkillInfo->GetSkillInfo()->
				{
					m_pBossMonsterInfo->SetChangeTargetRange(pSkillInfo->GetSafeRange(), pSkillInfo->GetSkillRange());
					ChangeTargetForSkill(&ObjectPos);
				}*/
				///////////////////////////////////////////////////////////////////////

				DoAttack();
				ChangeCurAttackIdx();				
			}
		}
		else
		{
			// attacking
		}
	}
	else
	{
		SetBossState(eBossState_Stand);		
	}
}

void CBossMonster::Pursuit()
{
	m_bDelete = FALSE;

	BASE_MONSTER_LIST	* apr = m_pSInfo;
	cActiveSkillInfo* pSkillInfo = (cActiveSkillInfo*)(SKILLMGR->GetSkillInfo(m_CurAttackIdx));
	if(pSkillInfo == 0)
	{
		ASSERT(0);
		SetBossState(eBossState_Stand);
		return;
	}

	if( !GetTObject() || (GetTObject() && ( GetTObject()->GetState() == eObjectState_Die ||  GetTObject()->GetInited() == FALSE ) ) )
	{
		SetBossState(eBossState_Stand);
		return;
	}

	if(m_BossState.IsStateFirst()) // 처음 시작한거면 
	{		
		m_aiParam.PursuitForgiveStartTime = gCurTime;
		m_BossState.SetStateStartTime(0);
		m_BossState.SetStateEndTime(1000);
		DoPursuit();
		m_BossState.SetStateFirst(FALSE);

		SYSTEMTIME time;
		char szFile[256] = {0,};
		GetLocalTime( &time );

		sprintf(szFile, "./Log/BossMonsterLog_%02d_%04d%02d%02d_%d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay, this->GetMonsterKind() );
		FILE* fp;
		fp = fopen(szFile, "a+");
		if (fp)
		{
			fprintf(fp, "RegenTime : [%2d:%2d]\n", time.wHour, time.wMinute);
			fclose(fp);
		}
	}
	else
	{
		VECTOR3 ObjectPos	= *CCharMove::GetPosition(this);
		VECTOR3 TObjectPos	= *CCharMove::GetPosition(GetTObject());
		DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
		if( m_aiParam.prePursuitForgiveTime == 0 && ( m_aiParam.PursuitForgiveStartTime + m_pSInfo->PursuitForgiveTime < gCurTime || m_pSInfo->PursuitForgiveDistance < Distance ) )
		{
			m_aiParam.SearchLastTime = gCurTime;
			SetBossState(eBossState_WalkAround);			
		}	
		else
		{
			if( m_aiParam.prePursuitForgiveTime != 0 && m_aiParam.PursuitForgiveStartTime + m_aiParam.prePursuitForgiveTime < gCurTime )
			{
				m_aiParam.prePursuitForgiveTime = 0;
				m_aiParam.SearchLastTime = gCurTime;
				SetBossState(eBossState_WalkAround);				
			}
			//else if( Distance <  pSkillInfo->GetSkillRange())
			else if (pSkillInfo->IsInSkillRange(this, &TObjectPos, GetTObject()->GetRadius() ))
			{
				SetBossState(eBossState_Attack);
			}
			/*else if (pSkillInfo->IsInRange(*this, TObjectPos, GetTObject()->GetRadius()))
			{
				SetBossState(eBossState_Attack);
			}*/
			else
			{
				m_BossState.SetStateStartTime(gCurTime);
				DoPursuit();
			}
		}
	}
}

void CBossMonster::SetCurAttackIdx(DWORD AttackIdx)
{
	m_CurAttackIdx = AttackIdx;
}
	
DWORD CBossMonster::GetCurAttackIdx()
{
	return m_CurAttackIdx;
}

void CBossMonster::ChangeCurAttackIdx()
{
	m_CurAttackIdx = m_pBossMonsterInfo->GetNextAttackKind(m_CurAttackIdx);
}

BOOL CBossMonster::SetBossState(int state)
{
	if(GetState() == eObjectState_Die)
		return FALSE;
	if(m_BossState.GetNextState())
	{
		
		return FALSE;
	}

	m_BossState.SetState(state);

	if(state == eBossState_WalkAround || state == eBossState_Pursuit || state == eBossState_RunAway)
	{	
		// 080204 KTH -- 맵이 막혀있어도 무시한다.
		if( !IsNoCheckCollision() )
			SetNoCheckCollision( TRUE );	//Ignore TTB

		MSG_WORD msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_MONSTERMOVE_NOTIFY;
		msg.dwObjectID = GetID();
		msg.wData = (WORD)state;
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
	return TRUE;
}

void CBossMonster::WalkAround()
{
	/// 06. 09. 2차 보스 - 이영준
	/// 소멸시간
	if( m_pBossMonsterInfo->GetDeleteTime() )
	{
		if( m_bDelete )
		{
			if( m_DeleteStartTime + m_pBossMonsterInfo->GetDeleteTime() <= gCurTime )
			{
				Delete();
			}
		}
		else
		{
			m_bDelete = TRUE;
			m_DeleteStartTime = gCurTime;
		}
	}

	if(m_BossState.IsStateFirst())
	{
		SetTObject(NULL);
		DoWalkAround();
		DWORD EndTime = m_BossState.GetStateStartTime() + CCharMove::GetMoveEstimateTime(this);
		m_BossState.SetStateEndTime(EndTime);
		m_BossState.SetStateFirst(FALSE);
	}
	else
	{				
		if( m_BossState.GetStateEndTime() < gCurTime )
		{
			int rate = rand()%100;
			if(0 <= rate && rate < m_pSInfo->StandRate )
			{
				SetBossState(eBossState_Stand);
			}
			else
			{
				//SetBossState(eBossState_WalkAround);
				DoWalkAround();
			}
		}
	}
}

void CBossMonster::Stand()
{
	/// 06. 09. 2차 보스 - 이영준
	/// 소멸시간
	if( m_pBossMonsterInfo->GetDeleteTime() )
	{
		if( m_bDelete )
		{
			if( m_DeleteStartTime + m_pBossMonsterInfo->GetDeleteTime() <= gCurTime )
			{
				Delete();
			}
		}
		else
		{
			m_bDelete = TRUE;
			m_DeleteStartTime = gCurTime;
		}
	}

	if(m_BossState.IsStateFirst())
	{
		m_BossState.SetStateFirst(FALSE);
		m_LifeRecoverTime.lastCheckTime = gCurTime;

		// 080205 KTH -- 맵에 충돌체크가 해지되어 있으면 다시 초기화 시켜준다.
		if( IsNoCheckCollision() )
		{
			VECTOR3 vPos;
			GetPosition( &vPos );
			if( !g_pServerSystem->GetMap()->CollisionTilePos( vPos.x, vPos.z, g_pServerSystem->GetMapNum() ) )
				SetNoCheckCollision( FALSE );
		}

		if( m_bOpenning )
		{
			switch( GetMonsterKind() )
			{
			case eBOSSKIND_TARINTUS:
				{
					// 080109 KTH -- 오프닝 8초
					m_BossState.SetStateEndTime(gCurTime+8000);
				}
				break;
			default:
				m_BossState.SetStateEndTime(gCurTime+3500);
				break;
			}
			m_bOpenning = FALSE;
		}
		//m_BossState.SetStateEndTime(gCurTime+3500);
				
		SetTObject(NULL);
		CMonster::DoStand();
		
		MSG_DWORD msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_BOSS_STAND_NOTIFY;
		msg.dwObjectID = GetObjectKind();
		msg.dwData = GetMonsterKind();
				
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
		return;
	}
	if(DoWalkAround() == TRUE)
		return;
	BOSSUNGIJOSIK* pUngi = m_pBossMonsterInfo->GetUngiInfo();
	if(m_BossState.GetStateStartTime() + pUngi->dwStartTime < gCurTime)
	{
		DWORD dwUngiTime = pUngi->dwDelayTime;
	 
	// ungi plus time for character
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
		
		DWORD maxlife = GetMaxLife();
		DWORD curlife = GetLife();
		
		if(gCurTime - m_LifeRecoverTime.lastCheckTime > dwUngiTime)
		{
			if(curlife < maxlife)
			{
				DWORD pluslife = (DWORD)(maxlife*pUngi->fLifeRate);
				SetLife(curlife + pluslife, TRUE);
				m_LifeRecoverTime.lastCheckTime = gCurTime;
			}
		}
	}	
}

void CBossMonster::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	SEND_MONSTER_TOTALINFO* pMsg = (SEND_MONSTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_BOSSMONSTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetMonsterTotalInfo(&pMsg->TotalInfo);
	pMsg->bLogin = bLogin;

	*pMsgLen = pMsg->GetMsgLength();
}

void CBossMonster::SetEventState(BYTE EventState, DWORD ActionValue)
{
	switch(EventState)
	{
	case eBOSSACTION_RECOVER:
		{
			SetBossNextState(eBossState_Recover);			
		}
		break;
	case eBOSSACTION_SUMMON:
		{
			SetBossNextState(eBossState_Summon);
		}
		break;
	}
	
//	MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Set EventState : %d", EventState));

	SetEventActionValue(ActionValue);
}

void CBossMonster::Recover()
{
	if(m_BossState.IsStateFirst())
	{		
		m_BossState.SetStateStartTime(gCurTime);
		m_BossState.SetStateEndTime(gCurTime+3500);
		m_BossState.SetStateFirst(FALSE);
		CCharacterCalcManager::StartUpdateLife( this, GetMaxLife()*m_EventActionValue/100, 5, 2500 );
		
		MSG_DWORD msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_BOSS_REST_START_NOTIFY;
		msg.dwData = GetMonsterKind();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));	
		//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Recover Start"));
	}
	if(m_BossState.GetStateEndTime() < gCurTime)
	{
		m_IsEventStating = FALSE;
		SetBossState(eBossState_Attack);
		
		//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("EndRecover State"));
	}
}

void CBossMonster::Summon()
{
 	if(m_BossState.IsStateFirst())
	{		
		m_BossState.SetStateStartTime(gCurTime);
		m_BossState.SetStateEndTime(gCurTime+3500);
		m_BossState.SetStateFirst(FALSE);

		int GroupID = GetEventActionValue();
		BOSSMONMGR->RegenGroup(this, GetSummonFileNum(), GroupID);

		MSG_DWORD msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_BOSS_RECALL_NOTIFY;
		msg.dwData = GetMonsterKind();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

		//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Summon start"));
	}
	if(m_BossState.GetStateEndTime() < gCurTime)
	{
		m_IsEventStating = FALSE;
		SetBossState(eBossState_Attack);
		//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("EndSummon State"));
	}
}

int CBossMonster::GetEventActionValue()
{
	return m_EventActionValue;
}

void CBossMonster::SetEventActionValue(DWORD value)
{
	m_EventActionValue = value;
}

WORD CBossMonster::GetSummonFileNum()
{
	return m_pBossMonsterInfo->GetSummonFileNum();
}

float CBossMonster::DoGetMoveSpeed()
{
	WORD kind = GetMonsterKind();

	float Speed = 0.0;
	switch(m_BossState.GetCurState())
	{
	case eMA_WALKAROUND:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->WalkMove;
		}
		break;
	case eMA_RUNAWAY:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunawayMove;
		}
		break;
	case eMA_PERSUIT:
		{
			Speed = (float)GAMERESRCMNGR->GetMonsterListInfo(kind)->RunMove;
		}
		break;
	default:
		{
			Speed = 0;
//			ASSERT(0);
		}
		break;
	}

	float addrateval = GetRateBuffStatus()->MoveSpeed / 100.f;
	float addval = GetBuffStatus()->MoveSpeed;

	m_MoveInfo.AddedMoveSpeed = Speed * addrateval + addval;

	Speed += m_MoveInfo.AddedMoveSpeed;

	if( Speed < 0 ) Speed = 0;

	if( GetAbnormalStatus()->IsMoveStop )
	{
		Speed = 0;
	}

	return Speed;
}
/*
void CBossMonster::AddStatus(CStatus* pStatus)
{
 // boss block
}
*/
void CBossMonster::OnLifeEvent(DWORD Life)
{
	if(m_IsEventStating == TRUE)
		return;
	if(m_BossState.GetCurState() >= eBossState_Recover)
		return;
	for(int i = 0; i< m_pBossMonsterInfo->GetMaxEventStateNum(); ++i)
	{
		if(m_BossEventState[i].Condition == eBOSSCONDITION_LIFE)
		{
			if(m_BossEventState[i].Count)
			{
				float rate = (float)(Life  * 100 / GetMaxLife());
				if(rate < (float)m_BossEventState[i].ConditionValue)
				{
					SetEventState(m_BossEventState[i].Action, m_BossEventState[i].ActionValue);
					m_BossEventState[i].Count--;					
					break;
				}
			}
		}
	}	
}

void CBossMonster::SetBossNextState(int state)
{
	m_BossState.SetNextState(state);
	m_IsEventStating = TRUE;
}

void CBossMonster::DoAttack()
{
	OnStop();

//	cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(m_CurAttackIdx);
	cActiveSkillInfo* pSkillInfo = (cActiveSkillInfo*)SKILLMGR->GetSkillInfo(m_CurAttackIdx);
	VECTOR3 TargetPos, MonPos;
	GetPosition(&MonPos);

	SKILLMGR->MonsterAttack(m_CurAttackIdx, this, m_pTObject);

	// 070104 KTH -- 일반 타겟일 경우
/*	if( pSkillInfo->GetSkillInfo()->Area == 0) // single target
	{
		SKILLMGR->MonsterAttack(m_CurAttackIdx, this, m_pTObject);
		return;
	}
	// 070105 KTH -- 광역일 경우
	else if( pSkillInfo->GetSkillInfo()->Area == 1 ) // multi target
	{
		CTargetList TList;
		TList.Clear();
		TargetPos = *CCharMove::GetPosition(GetTObject());
		g_pUserTable->GetTargetInRange( &TargetPos, pSkillInfo->GetSkillInfo()->Range, &TList );
		SKILLMGR->MonsterAttack(m_CurAttackIdx, this, m_pTObject);
	}
	else
	{
		CTargetList TList;
		TList.Clear();
		g_pUserTable->GetTargetInRange( &MonPos, pSkillInfo->GetSkillInfo()->Range, &TList );
		SKILLMGR->MonsterAttack(m_CurAttackIdx, this, m_pTObject);
	}*/
/*	if( pSkillInfo->GetSkillInfo()->Area == 1) // multi target
	{			
		TargetPos = *CCharMove::GetPosition(GetTObject());		
		SKILLMGR->MonsterAttack((DWORD)m_CurAttackIdx, this, &TargetPos,m_pTObject->GetID());
	}*/
/*	else // range self
	{			
		SKILLMGR->MonsterAttack((DWORD)m_CurAttackIdx, this, &MonPos,m_pTObject->GetID());
	}*/
}

BOOL CBossMonster::DoWalkAround()
{
	SetBossState(eBossState_WalkAround);

	if( m_pSInfo->bForeAttack && m_aiParam.SearchLastTime + m_pSInfo->SearchPeriodicTime < gCurTime )
	{
		m_aiParam.SearchLastTime = gCurTime;
		CObject * TObject = DoSearch();
		
		SetTargetObject(TObject);

		/*
		if(m_pTObject)
		{
			VECTOR3 TObjectPos = *CCharMove::GetPosition(m_pTObject);
			OnMove(&TObjectPos);
		}
		*/
		return TRUE;
	}
	// 250 :  충돌시 Search하는 Delay
	else if(m_CurAttackIdx && m_aiParam.CollSearchLastTime + 250 < gCurTime )
	{
		m_aiParam.CollSearchLastTime = gCurTime;
		
		//if( g_pServerSystem->GetMap()->IsVillage() )
		//	return FALSE;
		
		CObject * TObject = OnCollisionObject();
		SetTargetObject(TObject);
		
		/*
		if(m_pTObject)
		{
			VECTOR3 TObjectPos = *CCharMove::GetPosition(m_pTObject);
			OnMove(&TObjectPos);
		}
		*/
		return TRUE;
	}
	return FALSE;
}

void CBossMonster::SetTargetObject(CObject* TObject)
{
	if(TObject && TObject->GetState() != eObjectState_Die )
	{
		CPlayer * pPlayer = (CPlayer *)TObject;
		if(!pPlayer->IsVisible())
		{
			return;
		}
		if(GetGridID() != TObject->GetGridID())
		{
			return;
		}
		
		m_BossState.SetState(eBossState_Pursuit);
		//SetTObject((CPlayer *)TObject);
		SetTObject(pPlayer);
	}
}

void CBossMonster::ChangeTargetForSkill(VECTOR3 * pPivotPos)
{
/*	CObject* pObject = NULL;
	g_pUserTable->GetTargetInDonutRangeInChannel(GetID(), pPivotPos, m_pBossMonsterInfo->GetChangeTargetMaxRange(), m_pBossMonsterInfo->GetChangeTargetMinRange(), pObject);
	if(pObject)
	{
		SetTargetObject(pObject);
	}
	else
	{
		/// 06. 08. 2차 보스 - 이영준
		m_CurAttackIdx = m_pBossMonsterInfo->GetFirstAttackIdx();
		DWORD delay = SKILLMGR->GetSkillInfo((WORD)m_CurAttackIdx)->GetDelayTime();
		m_BossState.SetStateEndTime(gCurTime+delay);
	}*/
}

void CBossMonster::Delete()
{
	SYSTEMTIME time;
	char szFile[256] = {0,};
	GetLocalTime( &time );

	sprintf(szFile, "./Log/BossMonsterLog_%02d_%04d%02d%02d_%d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay, this->GetMonsterKind() );
	FILE* fp;
	fp = fopen(szFile, "a+");
	if (fp)
	{
		fprintf(fp, "DeleteTime : [%2d:%2d]\n", time.wHour, time.wMinute);

		fclose(fp);
	}
	// 리젠 정보 등록 080204 KTH -- 잡지 못하였을 경우 리젠 시간을 1시간으로 마춘다.
	BOSSMONMGR->SetBossRandRegenChannel( GetMonsterKind(), GetGridID(), GetMonsterGroupNum(), 60 * 6000 );

	BOSSMONMGR->DeleteBossMonster( this );

	REGENNPC_OBJ->ReleaseMonsterGroup( ( ( CMonster* )this )->GetMonsterGroupNum() );
	OBJECTSTATEMGR_OBJ->StartObjectState( this, eObjectState_Die, 0 );
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Die, 0 );
	SetTObject( NULL );
	SetLastAttackPlayer( NULL );

//---KES Aggro 070918
//---어그로 관련 변수들 릴리스	//죽은 후에 여기로 들어오나?
	RemoveAllAggro();
//-------------------

}

