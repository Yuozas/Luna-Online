#include "stdafx.h"
#include "fieldbossmonster.h"
#include "PackedData.h"
#include "ObjectStateManager.h"

CFieldBossMonster::CFieldBossMonster(void)
{
}

CFieldBossMonster::~CFieldBossMonster(void)
{
}
/*
void CFieldBossMonster::AddStatus(CStatus* pStatus)
{
}
*/
BOOL CFieldBossMonster::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	CMonster::Init(kind, AgentNum, pBaseObjectInfo);
	
	// �й� �ý��� �ʱ�ȭ
	DistributeDamageInit();
	
	return TRUE;
}

void CFieldBossMonster::DoDie(CObject* pAttacker)
{
	// ����ġ ����ġ ���� �й��Ѵ�
	DistributePerDamage();
	// �������� �й��Ѵ�
	DistributeItemPerDamage();

	// �ϴ� Ŭ���̾�Ʈ�� ������ �������ش�
	DWORD AttackerID;
	if(pAttacker == NULL)
		AttackerID = 0;
	else 
		AttackerID = pAttacker->GetID();

	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,AttackerID);
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die,MONSTERREMOVE_TIME+3000);

	SetTObject(NULL);
	SetLastAttackPlayer( NULL );

//---KES Aggro 070918
//---��׷� ���� ������ ������	//���� �Ŀ� ����� ������?
	RemoveAllAggro();
//-------------------

	// CFieldBossMonsterManager�� �׾��ٴ°��� �뺸���ش�
	FIELDBOSSMONMGR->BossDead(this);

	//CMonster::DoDie(pAttacker);	
}

void CFieldBossMonster::SetLife(DWORD Life,BOOL bSendMsg)
{
	CMonster::SetLife(Life, bSendMsg);	
	
	if(bSendMsg)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_BOSSMONSTER;
		msg.Protocol = MP_FIELD_LIFE_NOTIFY;
		msg.dwData1 = GetLife();
		msg.dwData2 = GetMonsterKind();
		msg.dwData3 = GetID();
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}
}

BOOL CFieldBossMonster::IsBattle()
{
	// ��ȭ ��� �϶� FALSE ����
	if( m_stateParam.stateCur < eMA_PERSUIT )
		return FALSE;

	// ���� ��� �϶� �Ҹ�ð� ī��Ʈ ����
	m_Info.m_dwCountTime = m_Info.m_dwDistructTime;
	// ȸ���ð��� ����
	m_Info.m_dwPieceTime = gCurTime;

	// TRUE ����
	return TRUE;
}

BOOL CFieldBossMonster::IsDistruct()
{
	BOOL rt = FALSE;
	
	// ������ üũ �ð��� 0�� ��� ���� �ð����� �����ϰ� FALSE ����
	if( m_Info.m_dwLastCheckTime != 0 )
	{
		if( ( gCurTime - m_Info.m_dwLastCheckTime )  <  m_Info.m_dwCountTime )
		{
			m_Info.m_dwCountTime -= ( gCurTime - m_Info.m_dwLastCheckTime );
		}
		else
		{
			m_Info.m_dwCountTime = 0;

			rt = TRUE;
		}
	}

	m_Info.m_dwLastCheckTime = gCurTime;

	return rt;
}

void CFieldBossMonster::Recover()	
{
	if(m_Info.m_dwPieceTime + m_Info.m_dwRecoverStartTime < gCurTime)
	{
		DWORD maxlife = GetMaxLife();
		DWORD curlife = GetLife();
		
		if(gCurTime - m_LifeRecoverTime.lastCheckTime > m_Info.m_dwRecoverDelayTime)
		{
			if(curlife < maxlife)
			{
				DWORD pluslife = (DWORD)(maxlife * m_Info.m_fLifeRate);
				SetLife(curlife + pluslife, TRUE);
				m_LifeRecoverTime.lastCheckTime = gCurTime;

				// Life�� ��� ȸ��������
				if(curlife + pluslife >= maxlife)
					// �й� �ý��� �ʱ�ȭ
					DistributeDamageInit();
			}
		}
	}	
}