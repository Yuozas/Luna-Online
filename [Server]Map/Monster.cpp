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

/*�����Ƽ ���� - #include "AbilityManager.h"*/

#include "GuildManager.h"

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.11.30
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.11.30
#include "Pet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//---KES Aggro 070918
//---��׷� ���� static ���� �� �Լ�
CMemoryPoolTempl<AGGRO>* CMonster::m_mpAggro = NULL;

void CMonster::InitAggroMemoryPool()
{
	if( m_mpAggro == NULL )
	{
		m_mpAggro = new CMemoryPoolTempl<AGGRO>;
		m_mpAggro->Init( 200*10, 200, "AGGRO" );	// 200���� ���� X �� ��� 10�� ����Ʈ
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
//---��׷� ���� ������ �ʱ�ȭ
	m_nMaxAggro	= 0;
	m_htAggroTable.Initialize( 10 );	//�Ϲ� ���ʹ� 10�������� �� �� ����. �������ʹ� �� �ø��� ���� ���ڴ�.
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
//---��׷� ���� ������ ������	//���� �Ŀ� ����� ������?
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
	// ���� ���� ���� �Է�
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

// 080616 LUJ, ���� �� ���� ���ʹ� ��ġ ������ ������ ������Ʈ�ϱ� virtual�� ������
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

// ������ ���� ��ü ���� //////////////////////////////
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

	if( SpeechType == eMSpch_Balloon || SpeechType == eMSpch_BallonChat )	//��ǳ�� or ��ǳ��+ä��â
	{
		msg.dwData1 = SpeechType;
		msg.dwData2 = SpeechIdx;
		msg.dwData3 = 0;	//������
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(MSG_DWORD4));

		return;
	}
	else if( SpeechType == eMSpch_SmallShout )	//��ġ�� SMALLSHOUT
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
	case eObjectState_TiedUp_CanSkill:	// �̵� ���
	case eObjectState_TiedUp:	// �̵�,���� ��� ���
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

				// 080616 LUJ, �÷��̾�� ������ ���� �� �ֵ��� Ÿ���� üũ��
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

// 080616 LUJ, ������ Ÿ�� ó���� ������ �ϱ� ���� ���� �Լ��� ó���ϰ�, ���ڵ� CObject*�� ��
BOOL CMonster::SetTObject( CObject * pNewTPlayer )
{	
	CObject* m_pOldTPlayer = m_pTObject;
	if(pNewTPlayer == m_pOldTPlayer) return FALSE;

	if(	pNewTPlayer &&
		pNewTPlayer->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* player = ( CPlayer* )pNewTPlayer;

		// �˻��� ���Ͱ� ���� ����
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

		//KES ���� �ְ�! ���߿� ������ ����ϵ��� ��������.
		if( player->AddFollowList(this) == TRUE )
		{
			if( m_pOldTPlayer )
			{
				if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Player )
				{
					( ( CPlayer* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
				else if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Pet )	//������ Ÿ���� ���� ���� �ִ�.
				{
					( ( CPet* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
			}

			m_pTObject = player;
		
			DWORD dwGravity = player->GetGravity();
			if(dwGravity > 100)
			{
				if( player->RemoveFollowAsFarAs(dwGravity, this) ) //������ ���� ���Ͱ� �ڽ�(this)�� ���� �ִ�!
					return FALSE;	//�ڽ��� ��������.
			}
			
			//SW050903	���ʹ�� ����
			MonSpeechInfo* pTemp = NULL;
			if(m_pSInfo->bForeAttack)	// �������� ��쿡��
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_ForeAtk );
			}
			else if( !m_pSInfo->bForeAttack && m_stateParam.stateNew == eMA_ATTACK )//!�񼱰��̸鼭 ���۸��̸�.. ���� ������ ��� �ؾ��Ұ��ΰ�..
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

		// �˻��� ���Ͱ� ���� ����
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

		//KES ���� �ְ�! ���߿� ������ ����ϵ��� ��������.
		if( player->AddFollowList(this) == TRUE )
		{
			if( m_pOldTPlayer )
			{
				if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Player )
				{
					( ( CPlayer* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
				else if( m_pOldTPlayer->GetObjectKind() == eObjectKind_Pet )	//������ Ÿ���� ���� ���� �ִ�.
				{
					( ( CPet* )m_pOldTPlayer )->RemoveFollowList(GetID());
				}
			}

			m_pTObject = player;
		
			DWORD dwGravity = player->GetGravity();
			if(dwGravity > 100)
			{
				if( player->RemoveFollowAsFarAs(dwGravity, this) ) //������ ���� ���Ͱ� �ڽ�(this)�� ���� �ִ�!
					return FALSE;	//�ڽ��� ��������.
			}
			
			//SW050903	���ʹ�� ����
			MonSpeechInfo* pTemp = NULL;
			if(m_pSInfo->bForeAttack)	// �������� ��쿡��
			{
				pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( GetMonsterKind(), eMon_Speech_ForeAtk );
			}
			else if( !m_pSInfo->bForeAttack && m_stateParam.stateNew == eMA_ATTACK )//!�񼱰��̸鼭 ���۸��̸�.. ���� ������ ��� �ؾ��Ұ��ΰ�..
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
		// ���Ͱ� ���� ����(rest, walkaround, stand) => ���� �÷��̾� ����
		// ���� �׾����� => ���� �÷��̾� ����
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

//---ù��
	// �ʵ庸���� ����
	if(GetObjectKind() != eObjectKind_FieldBossMonster)
	if(beforeLife == GetSMonsterList()->Life && pDamageInfo->RealDamage > 0 )
	{
		// 080108 - ùŸ �߰� ������ �ּ� ó��.
		//plusdamage = (DWORD)(GetSMonsterList()->Life*0.2f);
		plusdamage = 0 ;
	}

//	if( pDamageInfo->RealDamage > beforeLife)
//		pDamageInfo->RealDamage = beforeLife;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		AddDamageObject((CPlayer*)pAttacker, pDamageInfo->RealDamage, plusdamage);

//---KES Aggro 070918
//---��׷� ��ġ�� �ø���.
		CalcAggro( pAttacker, pDamageInfo->RealDamage );
//---
		m_pLastAttackPlayer = pAttacker;	//KES 070918

		if( m_stateParam.stateCur == eMA_REST || m_stateParam.stateCur == eMA_RUNAWAY) return ;

		// ����?
//		WORD RunawayType = 0;
//		if(CAIManager::IsRunawayStateChange(this, (CPlayer *)pAttacker))
//		{
//			GSTATEMACHINE.SetState(this, eMA_RUNAWAY);
//			return;
//		}


//---KES Aggro 070918
/* ���� Ÿ�� �ٲٴ� ��� �߼�ó��
		if(CAIManager::IsTargetChange((CPlayer *)pAttacker, this))
		{
			m_pLastAttackPlayer = (CPlayer*)pAttacker;	//�� ���� �Ⱦ��̳�?
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
	// ���� ��û 
	// YH ���� ������ �� �����ϰ� �� ���� �ϰ� �ϴ� ��찡 �߻�
	// ������ �޾������� �̽�(���ݰŸ�)���� ������û ����
	if(pDamageInfo->RealDamage != 0)
		CHelpRequestManager::MonsterRequestProcess(this);

	
	//�� CalcAggro() �Լ� ȣ��� �̹� ���ش�. ������ �¾����� ���� ����ð� �ʱ�ȭ ������ ȣ��������.
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

	// 080616 LUJ, �����Ͱ� ��ȿ�� ��쿡 ���� �����ϵ��� ��
	if( pAttacker )
	{
		AttackerID		= pAttacker->GetID();
		attackerKind	= EObjectKind( pAttacker->GetObjectKind() );
	}

	REGENNPC_OBJ->ReleaseMonsterGroup(((CMonster*)this)->GetMonsterGroupNum());
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME);

	//SW jackpot
	// 080616 LUJ, �������� ������ ���� �ʵ��� ����
	if( eObjectKind_Player == attackerKind )
	{
//COMMENT:JACKPOT	

		JACKPOTMGR->DoJackpot((CPlayer*)pAttacker, this);

		SWPROFITMGR->AddProfitFromMonster( (CPlayer*)pAttacker, this );

		//SW060719 ��������Ʈ
		GUILDMGR->AddHuntedMonster( (CPlayer*)pAttacker, this );

		// desc_hseos_���͹���01
		// S ���͹��� �߰� added by hseos 2007.05.23
		((CPlayer*)pAttacker)->ProcMonstermeterKillMon();
		// E ���͹��� �߰� added by hseos 2007.05.23

		// desc_hseos_����Ʈ ��_01
		// S ����Ʈ �� �߰� added by hseos 2007.11.30
		g_csDateManager.SRV_ProcMonsterDie((CPlayer*)pAttacker);
		// E ����Ʈ �� �߰� added by hseos 2007.11.30
	}
	
	//SW050902
	MonSpeechInfo* pTemp = MON_SPEECHMGR->GetCurStateSpeechIndex( this->GetMonsterKind(), eMon_Speech_Death );
	if( pTemp )
		this->AddSpeech( pTemp->SpeechType, pTemp->SpeechIndex );

	//SW051112 ���ָ��
	//m_pLastAttackPlayer ��� pAttacker ��..
	// 080616 LUJ, �������� ������ ���� �ʵ��� ����
	if( eObjectKind_Player == attackerKind )
	{
		// 06. 03 �������� - �̿���
		/*�����Ƽ ���� - if(!(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_MUSSANG, ((CPlayer*)pAttacker)->GetAbilityGroup())  < 1))*/
			((CPlayer*)pAttacker)->AddMussangPointFromLevel(GetLevel());
	}

	m_pLastAttackPlayer = NULL;
	SetTObject(NULL);
	
//---KES Aggro 070918
//---��׷� ���� ������ ������	//���� �Ŀ� ����� ������?
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

	// ��� �����̱⶧���� �� �����̰� ��
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
	// Ÿ�� �� �浹..

	//---KES �ǹ� ���� ���ϸ� �ɸ��� �� ������....
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
			//---KES ������ �𸣰ڴ�.
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

	// LBS �ϴ� ���ƺ��ϴ�.
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
	// YH �̺�Ʈ�����̳� �������� ProtoType �ȳ־��ִ°����� ����
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
	
	//taiyo �ӽ�ó�� 
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
/* �� �̷� �ڵ尡 ���ֳ�
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

	//taiyo �ӽ�ó�� 
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

	// 070627 LYW --- Monster : �����۰� ����ġ�� �й��ϴ� �������� ��Ʈ�������� ��� ������ �����Ѵ�.
	// ���ο��� ������ ���� �ϱ� ������, ���� �Լ����� ���ڰ� ������� ������ �ݺ����� ���� ���� ��ƾ�� ���� ����.
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

// �ʵ庸�� - 05.12 �̿���
// �ʵ庸���� ������ �й� �Լ� ȣ����
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
	if( pAggro )							//�̹� ����Ʈ�� �ִٸ�
	{
		pAggro->nAggro += nAggroAdd;		//��׷θ� ����(����)��Ű��
		if( pAggro->nAggro < 0 )
			pAggro->nAggro = 0;
	}
	else	//���ο� �����ڶ��,
	{
		if( nAggroAdd > 0 )
		{
			pAggro = m_mpAggro->Alloc();						//��׷� ������ �Ҵ�
			if( pAggro )
			{
				pAggro->dwObjectID	= pAttacker->GetID();		//������ ���̵� ����
				pAggro->nAggro		= nAggroAdd;				//��׷� ��ġ ����
				m_htAggroTable.Add( pAggro, pAggro->dwObjectID );

				if( pAttacker->GetObjectKind() == eObjectKind_Player )
					((CPlayer*)pAttacker)->AddToAggroed( this );				//�÷��̾�� ��׷θ� ���� ���͸� ��Ͻ����ش�. �ֹ濬��. (���⼭�� �����Ѵ�)
				if( pAttacker->GetObjectKind() == eObjectKind_Pet )
					((CPet*)pAttacker)->AddToAggroed( this );				//�÷��̾�� ��׷θ� ���� ���͸� ��Ͻ����ش�. �ֹ濬��. (���⼭�� �����Ѵ�)
			}
			else	//alloc ���д� �Ͼ���� �ȵ�����, �Ͼ���� ������ �ϱ�� ����.
			{
				if( SetTObject( pAttacker ) )
					GSTATEMACHINE.SetState( this, eMA_PERSUIT );
			}
		}
	}

	if( nAggroAdd > 0 ) //��׷� �������
	{
		if( pAggro )
		{
			if( pAggro->nAggro > m_nMaxAggro )	//�ְ� ��׷ΰ� ����ȴٸ�
			{
				if( SetTObject( pAttacker ) )			//Ÿ���� �ٲپ��ش�.
					GSTATEMACHINE.SetState( this, eMA_PERSUIT );
				m_nMaxAggro = pAggro->nAggro;
			}
		}
	}
	else			//��׷� ���Ҷ�� �ٽ� ã�´�.
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
			pPlayer->RemoveFromAggroed( GetID() );	//(���⼭�� �����Ѵ�.)

			if( GetTObject() == pPlayer )	//Ÿ���� ��������, �ٸ� Ÿ������ �ٲ۴�.
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
			pPet->RemoveFromAggroed( GetID() );	//(���⼭�� �����Ѵ�.)

			if( GetTObject() == pPet )	//Ÿ���� ��������, �ٸ� Ÿ������ �ٲ۴�.
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
				pPlayer->RemoveFromAggroed( GetID() );	//(���⼭�� �����Ѵ�.)
			}
			else if( pObject->GetObjectKind() == eObjectKind_Pet )
			{
				CPet* pPet = ( CPet* )pObject; 
				pPet->RemoveFromAggroed( GetID() );	//(���⼭�� �����Ѵ�.)
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
		if( pAggro->nAggro >= max )	// '>=' ��׷ΰ� 0�� ������ ����̵ȴ�.
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
