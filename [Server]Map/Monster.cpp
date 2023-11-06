// Monster.cpp: implementation of the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Monster.h"
#include "CharMove.h"
#include "PackedData.h"
#include "GridSystem.h"
#include "Player.h"
#include "UserTable.h"
#include "RegenNpc.h"
#include "ObjectStateManager.h"
#include "AISystem.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/server/status/status.h"
#include "../[cc]skill/server/delay/delay.h"
#include "../[cc]skill/server/info/activeskillinfo.h"
#include "MHError.h"
#include "HelpRequestManager.h"

//SW_TEST
#include "cJackpotManager.h"

#include "TileManager.h"
#include "PathManager.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "RegenPrototype.h"
#include "AIManager.h"
#include "RegenManager.h"
#include "Battle.h"

#include "StatisticsManager.h"
#include "cMonsterSpeechManager.h"
#include "SiegeWarProfitMgr.h"

/*어빌리티 삭제 - #include "AbilityManager.h"*/

#include "GuildManager.h"

// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.11.30
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.11.30
#include "Pet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//---KES Aggro 070918
//---어그로 관련 static 변수 및 함수
CMemoryPoolTempl<AGGRO>* CMonster::m_mpAggro = NULL;

void CMonster::InitAggroMemoryPool()
{
	if( m_mpAggro == NULL )
	{
		m_mpAggro = new CMemoryPoolTempl<AGGRO>;
		m_mpAggro->Init( 200*10, 200, "AGGRO" );	// 200마리 몬스터 X 각 평균 10개 리스트
	}
}

void CMonster::ReleaseAggroMemoryPool()
{
	if( m_mpAggro )
	{
		m_mpAggro->Release();
		delete m_mpAggro;
		m_mpAggro = NULL;
	}
}
//--------------------

CMonster::CMonster()
{
	m_pSInfo = NULL;
	m_pLastAttackPlayer = NULL;

	mpBattleStateDelay = new cDelay;
	mpBattleStateDelay->Init( 10000 );
}

CMonster::~CMonster()
{
	delete mpBattleStateDelay;

	m_pSInfo = NULL;
}
BOOL CMonster::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CObject::Init(kind, AgentNum, pBaseObjectInfo);
	
	//m_MonsterState = new CStateNPC;
	m_pTObject = NULL;
	m_DropItemId = 0;
	m_dwDropItemRatio = 100;
	
	m_bEventMob = FALSE;

	m_pLastAttackPlayer = NULL;

//---KES Aggro 070918
//---어그로 관련 변수들 초기화
	m_nMaxAggro	= 0;
	m_htAggroTable.Initialize( 10 );	//일반 몬스터는 10개정도면 될 것 같다. 보스몬스터는 더 늘리는 것이 좋겠다.
//-------------------

//---KES 071120
	m_bNoCheckCollision = FALSE;
//-------------

	m_DieTime = 0;
	return TRUE;
}
void CMonster::Release()
{
	SetTObject(NULL);

//---KES Aggro 070918
//---어그로 관련 변수들 릴리스	//죽은 후에 여기로 들어오나?
	RemoveAllAggro();
//-------------------

	if(m_bEventMob)
	{
		g_pAISystem->ReleaseMonsterID(GetSubID());
		g_pAISystem->ReleaseMonsterID(GetID());
		m_BaseObjectInfo.dwObjectID = 0;
		SetSubID(0);
		m_bEventMob = FALSE;
	}
	m_Distributer.Release();
	
	CObject::Release();
}

void CMonster::InitMonster(MONSTER_TOTALINFO* pTotalInfo)
{
	m_KillerPlayer = 0;
	memcpy(&m_MonsterInfo,pTotalInfo,sizeof(MONSTER_TOTALINFO));
	// 몬스터 스텟 정보 입력
	GAMERESRCMNGR->GetMonsterStats(GetMonsterKind(), &m_mon_stats);


	//m_pState = new CStateMachine;
	//m_pState->InitMachine(this);
	//m_pAIBase = new CAIBase((CObject*)this);
	//m_pAIBase->Init(GAMERESRCMNGR->GetMonsterInfo(GetMonsterKind()));
	//if( GetObjectKind() == eObjectKind_SpecialMonster || GetObjectKind() == eObjectKind_BossMonster)
	{
		m_pSInfo = GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind());
	}
	
	m_stateParam.SetState(m_BaseObjectInfo.ObjectState);
	GSTATEMACHINE.SetState( this, eMA_STAND );
	//m_aiParam.Init(GAMERESRCMNGR->GetMonsterListInfo(GetMonsterKind()));
	m_aiParam.Init();
	m_MoveInfo.KyungGongIdx = eMA_STAND;

	m_pLastAttackPlayer = NULL;

	memset( &mBuffStatus, 0, sizeof( Status ) );
	memset( &mRateBuffStatus, 0, sizeof( Status ) );
	memset( &mAbnormalStatus, 0, sizeof( AbnormalStatus ) );

	if( m_pSInfo->SpecialType == 2 )
	{
		mAbnormalStatus.IsGod = TRUE;
	}
}

void CMonster::GetMonsterTotalInfo(MONSTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_MonsterInfo,sizeof(MONSTER_TOTALINFO));
}

// 080616 LUJ, 함정 등 은신 몬스터는 위치 정보를 별도로 업데이트하기 virtual로 선언함
void CMonster::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	SEND_MONSTER_TOTALINFO* pMsg = (SEND_MONSTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_MONSTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo);
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetMonsterTotalInfo(&pMsg->TotalInfo);
	pMsg->bLogin = bLogin;

	*pMsgLen = pMsg->GetMsgLength();
}
DWORD CMonster::GetCurAttackIndex()
{
	return m_pSInfo->AttackIndex[m_aiParam.GetCurAttackKind()];
}

void CMonster::MoveStop()
{
	if(CCharMove::IsMoving(this) == FALSE)
		return;
	
	VECTOR3* pos = CCharMove::GetPosition(this);
	CCharMove::EndMove(this,gCurTime,pos);
	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwMoverID = GetID();
	msg.cpos.Compress(pos);

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}

// 데미지 입힌 객체 관리 //////////////////////////////
void CMonster::AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage)
{
	if( damage )
		m_Distributer.AddDamageObject(pPlayer, damage, plusdamage);
}

/*
void CMonster::SetPeaceRestTime(DWORD dwMaxTime)
{
	m_dwRestTime = rand()%dwMaxTime;
	
}
void CMonster::SetAttackRestTime(DWORD dwRestTime)
{
	m_dwRestTime = dwRestTime;
	
}
*/

/*
BOOL CMonster::Rest(DWORD dwTickTime)
{
	if(m_dwRestTime == 0)
		return TRUE;
	if(m_dwRestTime < dwTickTime)
	{
		m_dwRestTime = 0;
		return TRUE;
	}
	else
	{
		m_dwRestTime -= dwTickTime;
		return FALSE;
	}
}*/


void CMonster::AddChat(char * str)
{
	TESTMSG pmsg;
	pmsg.Category = MP_CHAT;
	pmsg.Protocol = MP_CHAT_FROMMONSTER_ALL;
	pmsg.dwObjectID = GetID();
	//strcpy(pmsg.Msg, str);
	SafeStrCpy( pmsg.Msg, str, MAX_CHAT_LENGTH+1 );
	//g_Console.LOG(4, pmsg->Msg);

	//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	//if(!pPlayer) return;

	//PACKEDDATA_OBJ->QuickSend(this,&pmsg,sizeof(pmsg));
	PACKEDDATA_OBJ->QuickSend(this,&pmsg,pmsg.GetMsgLength());	//CHATMSG 040324
}

void CMonster::AddSpeech( DWORD SpeechType, DWORD SpeechIdx )
{
	ASSERT(SpeechIdx>=0);

	MSG_DWORD4 msg;
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_MONSTERSPEECH;
	msg.dwObjectID = GetID();

	if( SpeechType == eMSpch_Balloon || SpeechType == eMSpch_BallonChat )	//말풍선 or 말풍선+채팅창
	{
		msg.dwData1 = SpeechType;
		msg.dwData2 = SpeechIdx;
		msg.dwData3 = 0;	//사용안함
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(MSG_DWORD4));

		return;
	}
	else if( SpeechType == eMSpch_SmallShout )	//외치기 SMALLSHOUT
	{
		msg.dwData1 = SpeechType;
		msg.dwData2 = SpeechIdx;
		msg.dwData3 = GetID();
		msg.dwData4 = GetMonsterKind();

		g_pUserTable->SetPositionUserHead();
		while( CObject* pObject = g_pUserTable->GetUserData() )
		{
			if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
			
			CPlayer* pReceiver = (CPlayer*)pObject;
			if( GetGridID() == pReceiver->GetChannelID() )
			{
				pReceiver->SendMsg( &msg, sizeof(MSG_DWORD4) );
			}
		}
		return;
	}
	else
	{
		ASSERT(0);
		return;
	}
}

void CMonster::OnStartObjectState(BYTE State,DWORD dwParam)
{
	switch(State)
	{
	case eObjectState_Die:
		m_KillerPlayer = dwParam;
		break;
	case eObjectState_TiedUp_CanSkill:	// 이동 결박
	case eObjectState_TiedUp:	// 이동,공격 모두 결박
		OnStop();
		break;
	}
}

void CMonster::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Die:
		g_pServerSystem->RemoveMonster(GetID());		
		break;
	}
}

float CMonster::GetRadius() const
{	
	return (float)m_pSInfo->MonsterRadius;
}

void CMonster::StateProcess()
{
	if( m_BaseObjectInfo.ObjectBattleState )
	{
		if( !mpBattleStateDelay->Check() )
		{
			SetObjectBattleState( eObjectBattleState_Peace );
		}
	}

	switch(GetState())
	{
	case eObjectState_Die:
		{
			DWORD RemainTime;
			if(m_ObjectState.State_End_Time >= gCurTime)
				RemainTime = m_ObjectState.State_End_Time - gCurTime;
			else
				RemainTime = 0;
			if(m_KillerPlayer && RemainTime < MONSTERREMOVE_TIME - 1500 )
			{
				CObject* pKiller = g_pUserTable->FindUser(m_KillerPlayer);

				// 080616 LUJ, 플레이어만이 보상을 가질 수 있도록 타입을 체크함
				if(	pKiller &&
					( pKiller->GetObjectKind() == eObjectKind_Player || 
					  pKiller->GetObjectKind() == eObjectKind_Pet ) )
				{
					GetBattle()->OnMonsterDistribute(this,pKiller);
				}

				m_KillerPlayer = 0;
			}
		}
		break;
	}
}

int CMonster::GetObjectTileSize()
{
	float radius = GetRadius();
	if(radius < 50)
		return 0;
	else if(radius < 150)
		return 1;
	else if(radius < 250)
		return 2;
	else
		return 2;
}

// 080616 LUJ, 함정이 타겟 처리를 별도로 하기 위해 가상 함수로 처리하고, 인자도 CObject*로 함
BOOL CMonster::SetTObject( CObject * pNewTPlayer )
{	
	CObject* m_pOldTPlayer = m_pTObject;
	if(pNewTPlayer == m_pOldTPlayer) return FALSE;

	if(	pNewTPlayer &&
		pNewTPlayer->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* player = ( CPlayer* )pNewTPlayer;

		// 검색된 몬스터가 유저 추적
		if( !player->GetInited() || player->GetState() == eObjectState_Die )
		{
//			MHERROR->OutputFile("Debug.txt", 
//				MHERROR->GetStringArg("SetTObject new mID:%u,pID:%u,mstate:%d,pstate:%d,mG:%d,pG:%d", 
//										GetID(),
//										player->GetID(), 
//										GetState(), 
//										player->GetState(),
//										GetGravity(), 
//										player->GetGravity()));
			
			return FALSE;
		}

		//KES 먼저 넣고! 나중에 비중을 계산하도록 변경하자.
		if( player->AddFollowList(this) == TRUE )
		{
			if( m_pOldTPlayer )
			{
				if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Player )
				{
					( ( CPlayer* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
				else if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Pet )	//기존에 타겟이 펫일 수도 있다.
				{
					( ( CPet* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
			}

			m_pTObject = player;
		
			DWORD dwGravity = player->GetGravity();
			if(dwGravity > 100)
			{
				if( player->RemoveFollowAsFarAs(dwGravity, this) ) //떨어져 나간 몬스터가 자신(this)일 수도 있다!
					return FALSE;	//자신이 지워졌다.
			}
			
			//SW050903	몬스터대사 관련
			MonSpeechInfo* pTemp = NULL;
			if(m_pSInfo->bForeAttack)	// 선공몹일 경우에만
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_ForeAtk );
			}
			else if( !m_pSInfo->bForeAttack && m_stateParam.stateNew == eMA_ATTACK )//!비선공이면서 헬퍼몹이면.. 헬퍼 구분을 어떻게 해야할것인가..
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_AboutHelp );
			}
			if( pTemp )
				AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );
			
			return TRUE;
		}
		
		return FALSE;
	}
	else if( pNewTPlayer && pNewTPlayer->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* player = ( CPet* )pNewTPlayer;

		// 검색된 몬스터가 유저 추적
		if( !player->GetInited() || player->GetState() == eObjectState_Die )
		{
//			MHERROR->OutputFile("Debug.txt", 
//				MHERROR->GetStringArg("SetTObject new mID:%u,pID:%u,mstate:%d,pstate:%d,mG:%d,pG:%d", 
//										GetID(),
//										player->GetID(), 
//										GetState(), 
//										player->GetState(),
//										GetGravity(), 
//										player->GetGravity()));

			return FALSE;
		}

		//KES 먼저 넣고! 나중에 비중을 계산하도록 변경하자.
		if( player->AddFollowList(this) == TRUE )
		{
			if( m_pOldTPlayer )
			{
				if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Player )
				{
					( ( CPlayer* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
				else if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Pet )	//기존에 타겟이 펫일 수도 있다.
				{
					( ( CPet* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
			}

			m_pTObject = player;
		
			DWORD dwGravity = player->GetGravity();
			if(dwGravity > 100)
			{
				if( player->RemoveFollowAsFarAs(dwGravity, this) ) //떨어져 나간 몬스터가 자신(this)일 수도 있다!
					return FALSE;	//자신이 지워졌다.
			}
			
			//SW050903	몬스터대사 관련
			MonSpeechInfo* pTemp = NULL;
			if(m_pSInfo->bForeAttack)	// 선공몹일 경우에만
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_ForeAtk );
			}
			else if( !m_pSInfo->bForeAttack && m_stateParam.stateNew == eMA_ATTACK )//!비선공이면서 헬퍼몹이면.. 헬퍼 구분을 어떻게 해야할것인가..
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_AboutHelp );
			}
			if( pTemp )
				AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );
			
			return TRUE;
		}
		
		return FALSE;
	}
	else
	{
		// pNewTPlayer == NULL
		// 몬스터가 추적 포기(rest, walkaround, stand) => 추적 플레이어 줄임
		// 몬스터 죽었을때 => 추적 플레이어 줄임
		if(	m_pOldTPlayer &&
			m_pOldTPlayer->GetObjectKind() == eObjectKind_Player )
		{
			( ( CPlayer* )m_pOldTPlayer )->RemoveFollowList(GetID());
		}

		if(	m_pOldTPlayer &&
			m_pOldTPlayer->GetObjectKind() == eObjectKind_Pet )
		{
			( ( CPet* )m_pOldTPlayer )->RemoveFollowList(GetID());
		}

		m_pTObject = NULL;
		return FALSE;
	}
	return FALSE;

}
void CMonster::DoDamage(CObject* pAttacker,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	if( pAttacker->GetGridID() != GetGridID() )
	{
//		ASSERT(0);
		return;
	}
	DWORD plusdamage = 0;
	
	SetObjectBattleState( eObjectBattleState_Battle );
	if ( pAttacker )
	{
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	}

	if( GetAbnormalStatus()->IsSlip )
	{
		EndBuffSkillByStatus( eStatusKind_Slip );
	}

//---첫빵
	// 필드보스는 제외
	if(GetObjectKind() != eObjectKind_FieldBossMonster)
	if(beforeLife == GetSMonsterList()->Life && pDamageInfo->RealDamage > 0 )
	{
		// 080108 - 첫타 추가 데미지 주석 처리.
		//plusdamage = (DWORD)(GetSMonsterList()->Life*0.2f);
		plusdamage = 0 ;
	}

//	if( pDamageInfo->RealDamage > beforeLife)
//		pDamageInfo->RealDamage = beforeLife;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		AddDamageObject((CPlayer*)pAttacker, pDamageInfo->RealDamage, plusdamage);

//---KES Aggro 070918
//---어그로 수치를 올린다.
		CalcAggro( pAttacker, pDamageInfo->RealDamage );
//---
		m_pLastAttackPlayer = pAttacker;	//KES 070918

		if( m_stateParam.stateCur == eMA_REST || m_stateParam.stateCur == eMA_RUNAWAY) return ;

		// 도망?
//		WORD RunawayType = 0;
//		if(CAIManager::IsRunawayStateChange(this, (CPlayer *)pAttacker))
//		{
//			GSTATEMACHINE.SetState(this, eMA_RUNAWAY);
//			return;
//		}


//---KES Aggro 070918
/* 기존 타겟 바꾸는 방식 추석처리
		if(CAIManager::IsTargetChange((CPlayer *)pAttacker, this))
		{
			m_pLastAttackPlayer = (CPlayer*)pAttacker;	//이 변수 안쓰이네?
			SetTObject((CPlayer *)pAttacker);
		}
*/
//-------------------
	}
	if( pAttacker->GetObjectKind() == eObjectKind_Pet )
	{
		AddDamageObject(((CPet*)pAttacker)->GetMaster(), pDamageInfo->RealDamage, plusdamage);

		CalcAggro( pAttacker, pDamageInfo->RealDamage );
		m_pLastAttackPlayer = pAttacker;	//KES 070918

		if( m_stateParam.stateCur == eMA_REST || m_stateParam.stateCur == eMA_RUNAWAY) return ;
	}
	// 도움 요청 
	// YH 헬프 받으면 또 헬프하고 또 헬프 하고 하는 경우가 발생
	// 헬프를 받았을때나 미스(공격거리)때는 핼프요청 안함
	if(pDamageInfo->RealDamage != 0)
		CHelpRequestManager::MonsterRequestProcess(this);

	
	//위 CalcAggro() 함수 호출시 이미 해준다. 하지만 맞았으니 추적 포기시간 초기화 때문에 호출해주자.
	GSTATEMACHINE.SetState(this, eMA_ATTACK);	//KES 070918

	cSkillObject* pSkill = SKILLMGR->GetSkillObject( mCurrentSkillID );

	if( pSkill )
	{
		if( SKILL_STATE_CASTING == pSkill->GetSkillState() )
		{
			cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pSkill->GetSkillInfo();

			if( pSkillInfo->GetSkillInfo()->Cancel )
			{
				WORD rate = rand() % 100;

				if( rate < pSkillInfo->GetSkillInfo()->Cancel )
				{
					MSG_DWORD msg;
					msg.Category = MP_SKILL;
					msg.Protocol = MP_SKILL_CANCEL_NOTIFY;
					msg.dwObjectID = GetID();
					msg.dwData = pSkill->GetID();

					PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof( MSG_DWORD ) );

					pSkill->SetEndState();
					pSkill->EndState();
				}
			}
		}
	}

}
void CMonster::SetLife(DWORD Life,BOOL bSendMsg)
{
//	ASSERT(Life <= m_pSInfo->Life);
	DWORD maxlife = m_pSInfo->Life;
	if(Life > maxlife)
		Life = maxlife;
	m_MonsterInfo.Life = Life;
}

void CMonster::DoDie(CObject* pAttacker)
{
//	g_Console.LOG(4, "Monster Die : GROUPID: %d, ID: %d, NAME: %s", GetMonsterGroupNum(), GetID(), m_pSInfo->Name);

	EObjectKind	attackerKind	= eObjectKind_None;
	DWORD		AttackerID		= 0;

	// 080616 LUJ, 포인터가 유효한 경우에 값을 설정하도록 함
	if( pAttacker )
	{
		AttackerID		= pAttacker->GetID();
		attackerKind	= EObjectKind( pAttacker->GetObjectKind() );
	}

	REGENNPC_OBJ->ReleaseMonsterGroup(((CMonster*)this)->GetMonsterGroupNum());
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME);

	//SW jackpot
	// 080616 LUJ, 널포인터 오류가 나지 않도록 수정
	if( eObjectKind_Player == attackerKind )
	{
//COMMENT:JACKPOT	

		JACKPOTMGR->DoJackpot((CPlayer*)pAttacker, this);

		SWPROFITMGR->AddProfitFromMonster( (CPlayer*)pAttacker, this );

		//SW060719 문파포인트
		GUILDMGR->AddHuntedMonster( (CPlayer*)pAttacker, this );

		// desc_hseos_몬스터미터01
		// S 몬스터미터 추가 added by hseos 2007.05.23
		((CPlayer*)pAttacker)->ProcMonstermeterKillMon();
		// E 몬스터미터 추가 added by hseos 2007.05.23

		// desc_hseos_데이트 존_01
		// S 데이트 존 추가 added by hseos 2007.11.30
		g_csDateManager.SRV_ProcMonsterDie((CPlayer*)pAttacker);
		// E 데이트 존 추가 added by hseos 2007.11.30
	}
	
	//SW050902
	MonSpeechInfo* pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( this->GetMonsterKind(), eMon_Speech_Death );
	if( pTemp )
		this->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );

	//SW051112 무쌍모드
	//m_pLastAttackPlayer 대신 pAttacker 로..
	// 080616 LUJ, 널포인터 오류가 나지 않도록 수정
	if( eObjectKind_Player == attackerKind )
	{
		// 06. 03 국내무쌍 - 이영준
		/*어빌리티 삭제 - if(!(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_MUSSANG, ((CPlayer*)pAttacker)->GetAbilityGroup())  < 1))*/
			((CPlayer*)pAttacker)->AddMussangPointFromLevel(GetLevel());
	}

	m_pLastAttackPlayer = NULL;
	SetTObject(NULL);
	
//---KES Aggro 070918
//---어그로 관련 변수들 릴리스	//죽은 후에 여기로 들어오나?
	RemoveAllAggro();
//-------------------

//	STATISTICSMGR->MonsterDieCount( GetMonsterKind() );	
}

void CMonster::GetSendMoveInfo(SEND_MOVEINFO* pRtInfo,CAddableInfoList* pAddInfoList)
{
	CObject::GetSendMoveInfo(pRtInfo,pAddInfoList);
	pRtInfo->KyungGongIdx = (WORD)m_stateParam.stateCur;	
}

float CMonster::DoGetMoveSpeed()
{
	WORD kind = GetMonsterKind();

	float Speed = 0.0;
	switch(m_stateParam.stateCur)
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

void CMonster::OnStop()
{
	if(CCharMove::IsMoving(this) == FALSE) return;
	
	VECTOR3* pos = CCharMove::GetPosition(this);
	CCharMove::EndMove(this, gCurTime, pos);
	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwMoverID = GetID();
	msg.cpos.Compress(pos);

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
}

void CMonster::OnMove( VECTOR3 * pPos )
{

	ASSERT(GetState() != eObjectState_Die);

	// 결박 상태이기때문에 못 움직이게 함
	if( GetState() == eObjectState_TiedUp_CanSkill ||
		GetState() == eObjectState_TiedUp )
		return;

	VECTOR3* curpos = CCharMove::GetPosition(this);
	if(m_MoveInfo.bMoving)
	{
		if(m_MoveInfo.GetMaxTargetPosIdx() != 0 && m_MoveInfo.GetTargetPosition(m_MoveInfo.GetMaxTargetPosIdx()-1)->x == pPos->x && m_MoveInfo.GetTargetPosition(m_MoveInfo.GetMaxTargetPosIdx()-1)->z == pPos->z)
			return;
	}
	if(curpos->x == pPos->x && curpos->z == pPos->z)
		return;

	//////////////////////////////////////////////////////////////////////////
	// 타일 및 충돌..

	//---KES 의미 없이 부하만 걸리는 것 같은데....
	/*
	VECTOR3 tPos;
	BOOL preo = g_pServerSystem->GetMap()->GetTileManager()->IsPreoccupied(this,pPos);
	if(preo)
	{
		tPos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(this,pPos,curpos,100,1000);
	}
	else
	{
		tPos = *pPos;
	}
	*/	
	//-------------------


	m_MoveInfo.InitTargetPosition();
//	BOOL bInv = FALSE;
//	if(m_stateParam.stateCur == eMA_PERSUIT)
		//bInv = TRUE;
//gotoPATH:

//---KES 071120
	if( IsNoCheckCollision() == TRUE )			//Ignore TTB
	{
		m_MoveInfo.GetTargetPositionArray()[0] = *pPos;
		m_MoveInfo.GetMaxTargetPosIdxRef() = 1;
	}
	else
	{
		switch(PATHMGR->GetPath(&m_MoveInfo.CurPosition,pPos,m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, m_MoveInfo.GetMaxTargetPosIdxRef(), this, FALSE))
		{	
		case PATH_BLOCKFAILED:
			{
			//---KES 이유를 모르겠다.
			//GSTATEMACHINE.SetState(this, eMA_RUNAWAY);
			return;

			VECTOR3 IDir;
			IDir.x = -m_MoveInfo.Move_Direction.x;
			IDir.z = -m_MoveInfo.Move_Direction.z;
			BeelinePoint(&m_MoveInfo.CurPosition, &IDir, 1000, pPos);
			OnMove(pPos);
		///	goto gotoPATH;
			}
			break;
		case PATH_FAILED:
			return;
		}
	}
//---------------

	WORD count = m_MoveInfo.GetMaxTargetPosIdx();

	// jsd 
	if( count == 0 )
		return;
	else if( count == 1 )
	{
		MOVE_ONETARGETPOS_FROMSERVER msg;
		SetProtocol(&msg,MP_MOVE,MP_MOVE_ONETARGET);
		msg.AddedMoveSpeed = GetMoveInfo()->AddedMoveSpeed;
		msg.dwObjectID = GetID();
		msg.SetStartPos(curpos);
		msg.SetTargetPos(m_MoveInfo.GetTargetPosition(0));

		PACKEDDATA_OBJ->QuickSend(this,&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_TARGET;
		msg.dwMoverID = GetID();
		msg.SetStartPos(&m_MoveInfo.CurPosition);
		msg.AddedMoveSpeed = m_MoveInfo.AddedMoveSpeed;
		//msg.tpos.Compress(&TargetPos);
		for(int i=0;i<count;++i)
			msg.AddTargetPos(m_MoveInfo.GetTargetPosition(i));

		PACKEDDATA_OBJ->QuickSend(this,&msg,msg.GetSize());
	}	

	//StartMove(pHero,gCurTime,&TargetPos);
	m_MoveInfo.SetCurTargetPosIdx(0);
	ASSERT(m_MoveInfo.GetCurTargetPosIdx()< m_MoveInfo.GetMaxTargetPosIdx());
	CCharMove::StartMoveEx(this,gCurTime,curpos,m_MoveInfo.GetTargetPosition(0));

}
void CMonster::DoStand()
{
	OnStop();
}
void CMonster::DoRest(BOOL bStart)
{
	OnStop();

}
BOOL CMonster::DoAttack( DWORD attackNum )
{
	if( !m_pTObject ) return FALSE;

	OnStop();
/*	
	if( GetStatus( eStatusKind_Stun )->GetTotalValue() )
	{
		return TRUE;
	}
*/
	// yh
	if( GetState() == eObjectState_TiedUp_CanMove ||
		GetState() == eObjectState_TiedUp )
		return TRUE;

	// LBS 일단 막아봅니다.
	if( GetGridID() != m_pTObject->GetGridID() )
	{
//		ASSERT(0);
		return TRUE;
	}

	SKILLMGR->MonsterAttack(attackNum, this, m_pTObject);

	return TRUE;
}

BOOL CMonster::DoWalkAround()
{
	int ran=0;
	VECTOR3 Target;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);
	//VECTOR3 domainPoint = RESPAWNMGR->GetOBJ_REGEN(GetID())->vPos;
	//VECTOR3 domainPoint = *GROUPMGR->GetRegenObject(GetMonsterGroupNum(), GetBattleID(), GetID())->GetPos();
	// YH 이벤트리콜이나 수련장등에선 ProtoType 안넣어주는것으로 수정
	CRegenPrototype* pProtoType = REGENMGR->GetPrototype(GetSubID());
	VECTOR3 domainPoint = pProtoType ? pProtoType->vPos : *pThisPos;
	VECTOR3 decisionPoint;
	AdjacentPointForOriginPoint(pThisPos, &domainPoint, (float)10000, &decisionPoint);

	float dx=0,dz=0;
	ran = rand();
	if(m_pSInfo->DomainRange==0)
		m_pSInfo->DomainRange = 1;
	
	dx = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%m_pSInfo->DomainRange) * (ran%2?1:-1);
	Target.x = decisionPoint.x + dx;
	Target.z = decisionPoint.z + dz;

	if(Target.x < 10.0f)
		Target.x = 10.0f;
	else if(Target.x > 51200.0f)
		Target.x = 51200.0f;

	if(Target.z < 10.0f)
		Target.z = 10.0f;
	else if(Target.z > 51200.0f)
		Target.z = 51200.0f;
	
	//taiyo 임시처리 
	VECTOR3 CollisonPos;
	
	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x - dx;
		Target.z = decisionPoint.z - dz;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x + dx;
		Target.z = decisionPoint.z - dz;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = decisionPoint.x - dx;
		Target.z = decisionPoint.z + dz;
	}

	//YH2DO
	if(Target.x < 10)
		Target.x = 10;
	else if(Target.x > 51200)
		Target.x = 51200;
	if(Target.z < 10)
		Target.z = 10;
	else if(Target.z > 51200)
		Target.z = 51200;
	
	OnMove(&Target);

	return TRUE;
}

BOOL CMonster::DoPursuit(CObject * pTObject)
{
//---KES 071120
/* 머 이런 코드가 다있냐
	CObject * pObj = pTObject;
	if( !m_pTObject)
		return FALSE;
	else
		pObj = m_pTObject;

	if(!pObj) return FALSE;

	VECTOR3 TObjectPos = *CCharMove::GetPosition(pObj);
*/

	if( !m_pTObject )	return FALSE;

	VECTOR3 TObjectPos = *CCharMove::GetPosition( m_pTObject );
//------------

	OnMove(&TObjectPos);

	return TRUE;
}

void CMonster::DoRunaway()
{
	MONSTEREX_LIST * pEXSInfo = (MONSTEREX_LIST *)m_pSInfo;
	VECTOR3 MovePoint;
	VECTOR3 * pThisPos = CCharMove::GetPosition(this);

	switch(m_aiParam.RunawayType)
	{
	case eRunawayType_Help:
		{
			ASSERT(m_aiParam.pHelperMonster);
			VECTOR3 * pHelperPos = CCharMove::GetPosition(m_aiParam.pHelperMonster);
			MovePoint.x = pHelperPos->x;
			MovePoint.y = 0;
			MovePoint.z = pHelperPos->z;
		}
		break;
	case eRunawayType_General:
		{
			m_aiParam.pHelperMonster = NULL;

			VECTOR3 IDir;
			IDir.x = -m_MoveInfo.Move_Direction.x;
			IDir.z = -m_MoveInfo.Move_Direction.z;
			BeelinePoint(pThisPos, &IDir, pEXSInfo->RunawayDistance, &MovePoint);
		}
		break;
	}
	VECTOR3 Target;

	Target.x = MovePoint.x;
	Target.z = MovePoint.z;

	if(Target.x < 10.0f)
		Target.x = 10.0f;
	else if(Target.x > 51200.0f)
		Target.x = 51200.0f;

	if(Target.z < 10.0f)
		Target.z = 10.0f;
	else if(Target.z > 51200.0f)
		Target.z = 51200.0f;

	//taiyo 임시처리 
	VECTOR3 CollisonPos;
	
	MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x - 100;
		Target.z = MovePoint.z - 100;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x + 100;
		Target.z = MovePoint.z - 100;
	}
	if(g_pServerSystem->GetMap()->CollisionCheck(pThisPos,&Target,&CollisonPos,this) == TRUE)
	{
		Target.x = MovePoint.x - 100;
		Target.z = MovePoint.z + 100;
	}


	if(Target.x < 10)
		Target.x = 10;
	if(Target.z < 10)
		Target.z = 10;
	if(Target.x > 51200)
		Target.x = 51200;
	if(Target.z > 51200)
		Target.z = 51200;
	
	OnMove(&Target);
}

CMonster * CMonster::DoFriendSearch(DWORD Range)
{
	return (CMonster *)g_pServerSystem->GetGridSystem()->FindMonsterInRange(CCharMove::GetPosition(this), (float)Range, this, FIND_FIRST );
}

CObject * CMonster::DoSearch()
{
	return g_pServerSystem->GetGridSystem()->FindPlayerInRange(CCharMove::GetPosition(this), (float)m_pSInfo->SearchRange, this,(BYTE)m_pSInfo->TargetSelect );
}

CObject * CMonster::OnCollisionObject()
{
	return g_pServerSystem->GetGridSystem()->FindPlayerInRange(CCharMove::GetPosition(this), (float)m_pSInfo->MonsterRadius, this, FIND_FIRST );
}

void CMonster::Drop()
{
	if( m_MonsterInfo.Group == (WORD)QUESTRECALL_GROUP_ID )	return;

	VECTOR3* KilledPosition = CCharMove::GetPosition(this);
	DWORD TotalLife = GetSMonsterList()->Life;

	// 070627 LYW --- Monster : 아이템과 경험치를 분배하는 정보들을 디스트리뷰터의 멤버 변수에 세팅한다.
	// 내부에서 참조를 많이 하기 때문에, 여러 함수에서 인자가 길어지는 이유와 반복적인 정보 생성 루틴을 막기 위해.
	if(m_Distributer.SetInfoToDistribute(m_KillerPlayer, KilledPosition, m_DropItemId, m_dwDropItemRatio, this))
	{
		m_Distributer.Distribute() ;
	}

	//m_Distributer.Distribute(m_KillerPlayer,KilledPosition,m_DropItemId, m_dwDropItemRatio, this);
}
/*
void CMonster::AddStatus(CStatus* pStatus)
{
	CObject::AddStatus(pStatus);

	if(pStatus->GetFlag() & STATUS_FLAG_MOVE)
	{
		MoveStop();
		DoPursuit();
	}
}

void CMonster::RemoveStatus(CStatus* pStatus)
{
	CObject::RemoveStatus(pStatus);

	if(pStatus->GetFlag() & STATUS_FLAG_MOVE)
	{
		MoveStop();
		DoPursuit();
	}
}
*/
void CMonster::DistributePerDamage()
{
	m_Distributer.DistributePerDamage(this);
}

// 필드보스 - 05.12 이영준
// 필드보스용 아이템 분배 함수 호출함
void CMonster::DistributeItemPerDamage()
{
	m_Distributer.DistributeItemPerDamage(this);
}

void CMonster::DistributeDamageInit()
{
	m_Distributer.DamageInit();
}

void CMonster::DistributeDeleteDamagedPlayer(DWORD CharacterID)
{
	m_Distributer.DeleteDamagedPlayer(CharacterID);
}


DWORD CMonster::DoGetPhyAttackPowerMin()
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;

	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin1;
		int tmp = m_mon_stats.PhysicalAttackMax1-m_mon_stats.PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin2;
		int tmp = m_mon_stats.PhysicalAttackMax2-m_mon_stats.PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return (DWORD)physicalAttackPower;
}


DWORD CMonster::DoGetPhyAttackPowerMax()
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;

	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin1;
		int tmp = m_mon_stats.PhysicalAttackMax1-m_mon_stats.PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = m_mon_stats.PhysicalAttackMin2;
		int tmp = m_mon_stats.PhysicalAttackMax2-m_mon_stats.PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return (DWORD)physicalAttackPower;
}

//---KES Aggro 070918
void CMonster::CalcAggro( CObject* pAttacker, int nAggroAdd )
{
	if( nAggroAdd == 0 ) return;

	AGGRO* pAggro = m_htAggroTable.GetData( pAttacker->GetID() );
	if( pAggro )							//이미 리스트에 있다면
	{
		pAggro->nAggro += nAggroAdd;		//어그로를 증가(감소)시키고
		if( pAggro->nAggro < 0 )
			pAggro->nAggro = 0;
	}
	else	//새로운 적대자라면,
	{
		if( nAggroAdd > 0 )
		{
			pAggro = m_mpAggro->Alloc();						//어그로 데이터 할당
			if( pAggro )
			{
				pAggro->dwObjectID	= pAttacker->GetID();		//적대자 아이디 대입
				pAggro->nAggro		= nAggroAdd;				//어그로 수치 대입
				m_htAggroTable.Add( pAggro, pAggro->dwObjectID );

				if( pAttacker->GetObjectKind() == eObjectKind_Player )
					((CPlayer*)pAttacker)->AddToAggroed( this );				//플레이어에도 어그로를 가진 몬스터를 등록시켜준다. 쌍방연결. (여기서만 연결한다)
				if( pAttacker->GetObjectKind() == eObjectKind_Pet )
					((CPet*)pAttacker)->AddToAggroed( this );				//플레이어에도 어그로를 가진 몬스터를 등록시켜준다. 쌍방연결. (여기서만 연결한다)
			}
			else	//alloc 실패는 일어나서는 안되지만, 일어나더라도 공격은 하기로 하자.
			{
				if( SetTObject( pAttacker ) )
					GSTATEMACHINE.SetState( this, eMA_PERSUIT );
			}
		}
	}

	if( nAggroAdd > 0 ) //어그로 증가라면
	{
		if( pAggro )
		{
			if( pAggro->nAggro > m_nMaxAggro )	//최고 어그로가 변경된다면
			{
				if( SetTObject( pAttacker ) )			//타겟을 바꾸어준다.
					GSTATEMACHINE.SetState( this, eMA_PERSUIT );
				m_nMaxAggro = pAggro->nAggro;
			}
		}
	}
	else			//어그로 감소라면 다시 찾는다.
	{
		AGGRO* pMaxAggro = FindMaxAggro();
		if( pMaxAggro )
		{
			if( SetTObject( (CPlayer*)g_pUserTable->FindUser( pMaxAggro->dwObjectID ) ) )
				GSTATEMACHINE.SetState( this, eMA_PERSUIT );
			m_nMaxAggro = pMaxAggro->nAggro;
		}
		else
		{
			SetTObject( NULL );
			m_nMaxAggro = 0;
		}		
	}
}

void CMonster::RemoveFromAggro( DWORD dwObjectID )
{
	AGGRO* pAggro = m_htAggroTable.GetData( dwObjectID );
	if( pAggro == NULL ) return;

	m_htAggroTable.Remove( dwObjectID );
	m_mpAggro->Free( pAggro );

	CObject* pObject = g_pUserTable->FindUser( pAggro->dwObjectID );
	if( pObject )
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pPlayer = ( CPlayer* )pObject;
			pPlayer->RemoveFromAggroed( GetID() );	//(여기서만 해제한다.)

			if( GetTObject() == pPlayer )	//타겟이 지워지면, 다른 타겟으로 바꾼다.
			{
				AGGRO* pMaxAggro = FindMaxAggro();
				if( pMaxAggro )
				{
					SetTObject( g_pUserTable->FindUser( pMaxAggro->dwObjectID ) );
					m_nMaxAggro = pMaxAggro->nAggro;
				}
				else
				{
					SetTObject( NULL );
					m_nMaxAggro = 0;
				}
			}
		}
		else if( pObject->GetObjectKind() == eObjectKind_Pet )
		{
			CPet* pPet = ( CPet* )pObject;
			pPet->RemoveFromAggroed( GetID() );	//(여기서만 해제한다.)

			if( GetTObject() == pPet )	//타겟이 지워지면, 다른 타겟으로 바꾼다.
			{
				AGGRO* pMaxAggro = FindMaxAggro();
				if( pMaxAggro )
				{
					SetTObject( g_pUserTable->FindUser( pMaxAggro->dwObjectID ) );
					m_nMaxAggro = pMaxAggro->nAggro;
				}
				else
				{
					SetTObject( NULL );
					m_nMaxAggro = 0;
				}
			}
		}
	}
}

void CMonster::RemoveAllAggro()
{
	m_nMaxAggro	= 0;

	m_htAggroTable.SetPositionHead();
	AGGRO* pAggro = NULL;
	while( pAggro = m_htAggroTable.GetData() )
	{
		CObject* pObject = g_pUserTable->FindUser( pAggro->dwObjectID );

		if( pObject )
		{
			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				CPlayer* pPlayer = ( CPlayer* )pObject; 
				pPlayer->RemoveFromAggroed( GetID() );	//(여기서만 해제한다.)
			}
			else if( pObject->GetObjectKind() == eObjectKind_Pet )
			{
				CPet* pPet = ( CPet* )pObject; 
				pPet->RemoveFromAggroed( GetID() );	//(여기서만 해제한다.)
			}
		}
		m_mpAggro->Free( pAggro );
	}

	m_htAggroTable.RemoveAll();
}

AGGRO* CMonster::FindMaxAggro()
{
	m_htAggroTable.SetPositionHead();
	AGGRO* pAggro		= NULL;
	AGGRO* pMaxAggro	= NULL;
	int max				= 0;
	while( pAggro = m_htAggroTable.GetData() )
	{
		if( pAggro->nAggro >= max )	// '>=' 어그로가 0인 유저도 대상이된다.
		{
			max = pAggro->nAggro;
			pMaxAggro = pAggro;
		}
	}

	return pMaxAggro;
}

void CMonster::SetObjectBattleState( BYTE state ) 
{ 
	m_BaseObjectInfo.ObjectBattleState = state; 

	if( state )
	{
		mpBattleStateDelay->Start();
	}
}

void CMonster::SetDieTime(DWORD time)
{
	m_DieTime = time;
}

void CMonster::Process()
{
	if(m_DieTime)
	{
		if(m_DieTime < gCurTime)
		{			
			MSG_DWORD2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_MONSTER_DIE;
			msg.dwObjectID	= 0;
			msg.dwData1		= 0;
			msg.dwData2		= GetID();
			
			PACKEDDATA_OBJ->QuickSend((CObject*)this,&msg,sizeof(msg));

			Die(NULL);
			m_DieTime = 0;
		}
	}
}

//-------------------
