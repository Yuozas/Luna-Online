// Npc.cpp: implementation of the CNpc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Npc.h"
#include "GameResourceManager.h"
#include "MOTIONDESC.h"		// 080321 NYJ --- NPC모션 enum을 위해 include
#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNpc::CNpc()
{
	m_pSInfo = NULL;
	m_dwLastMotionTime = 0;	// 080321 NYJ --- IDLE모션변경
	m_nNextMotionTime = 5000;
}

CNpc::~CNpc()
{

}


void CNpc::InitNpc(NPC_TOTALINFO* pTotalInfo)
{
	memcpy(&m_NpcInfo,pTotalInfo,sizeof(NPC_TOTALINFO));
	memset(m_QuestList, 0, 4*MAX_NPCQUEST_LIST);
	m_QuestCount = 0;
	m_pSInfo = GAMERESRCMNGR->GetNpcInfo(GetNpcKind());

	m_nNextMotionTime = 20000 + rand()%30000;
	m_dwLastMotionTime = gCurTime;	// 080321 NYJ --- IDLE모션변경
}



void CNpc::Process()
{
	if(IsDied())
	{
		DWORD time;
		time = REMOVEFADE_START_TIME;

		if( gCurTime - m_DiedTime > time )
		{
			ShowObjectName( FALSE );
			ShowChatBalloon( FALSE, NULL );
			//그림자도 없애주자.
			if(m_ShadowObj.IsInited())
			{
				m_ShadowObj.Release();
			}

			DWORD ToTime = time;
			DWORD ElTime = gCurTime - m_DiedTime - time;
			float alp = 1 - (ElTime / (float)ToTime);
			if(alp <= 0)
				alp = 0;
			GetEngineObject()->SetAlpha(alp);
		}
#ifdef _TESTCLIENT_
		if(gCurTime - m_DiedTime > MONSTERREMOVE_TIME)
		{
			OBJECTMGR->AddGarbageObject(this);
		}
#endif
	}

	CObject::Process();

	const DWORD dwLatencyTime = 60000;	// 60sec
	const DWORD dwTimeRange = 15000;	// 15sec
	
	// 080321 NYJ --- IDLE모션변경은 일정시간(dwLatencyTime+dwTimeRange)에 한번씩 바꾼다.
	if(m_nNextMotionTime<gCurTime && m_dwLastMotionTime<=gCurTime-m_nNextMotionTime && GetEngineObject()->IsEndMotion())
	{
		DWORD dwTest = m_ObjectState.State_End_Time;
		SetMotionInState(eObjectState_None);
		m_dwLastMotionTime = gCurTime;
		m_nNextMotionTime = dwLatencyTime + rand()%dwTimeRange;
	}
}

void CNpc::SetMotionInState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{
			ASSERT(0);
		}
		break;
	case eObjectState_None:
		{
			const int nStand2Motion = 2;	// 080321 NYJ --- IDLE2 모션은 하드코딩으로 넣는다.
			m_ObjectState.State_Start_Motion = 1;
			m_ObjectState.State_Ing_Motion = 1;
			m_ObjectState.State_End_Motion = 1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;

			ChangeMotion(nStand2Motion,FALSE);
		}
		break;
	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
		}
		break;
	}
}

void CNpc::OnClicked()
{
	//if( GetNpcKind() <= 5 || GetNpcKind() == 8 || GetNpcKind() == 10 || GetNpcKind() == 12 ||
	//    GetNpcKind() == 13 || GetNpcKind() == 15 || GetNpcKind() == 25 ) 
	{
		ChangeBaseMotion(1);
		ChangeMotion(5,FALSE);
	}
	
	/*
	//////////////////////////////////////////////////////////////////////////
	// 임시 하드코딩 	
	// PJH2DO
	if( GetNpcKind() == 31 ||
		GetNpcKind() == 32 ||
		GetNpcKind() == 33 ||
		GetNpcKind() == 34 )
	{
		ChangeMotion(2);
	}
	*/
}


void CNpc::AddQuestIndex(int Idx)
{
	if(Idx <= 0)		return;

	m_QuestList[m_QuestCount] = Idx;
	++m_QuestCount;
}

void CNpc::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive)
{
	if(GetNpcJob() == BOMUL_ROLE)
	{
		ChangeBaseMotion(1);
		ChangeMotion(2,FALSE);
	}
}