#include "stdafx.h"
#include "fieldsubmonster.h"
#include "PackedData.h"

CFieldSubMonster::CFieldSubMonster(void)
{
}

CFieldSubMonster::~CFieldSubMonster(void)
{
}

void CFieldSubMonster::DoDie(CObject* pAttacker)
{
	CMonster::DoDie(pAttacker);	

	// CFieldBossMonsterManager�� �׾��ٴ°��� �뺸���ش�
	FIELDBOSSMONMGR->SubDead(this);
}

void CFieldSubMonster::SetLife(DWORD Life,BOOL bSendMsg)
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

BOOL CFieldSubMonster::IsBattle()
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

BOOL CFieldSubMonster::IsDistruct()
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

void CFieldSubMonster::Recover()	
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
			}
		}
	}	
}