// SkillDalayManager.cpp: implementation of the CSkillDalayManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillDalayManager.h"

#include "MHFile.h"
#include "Network.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSkillDelayManager);


CSkillDelayManager::CSkillDelayManager()
{
	m_htSkillUse.Initialize( 500 );

	m_mpSkillUse = new CMemoryPoolTempl<SKILLUSE>;
	m_mpSkillUse->Init( 500, 10, "SKILLUSE" );

	m_htPrimereSkill.Initialize( 10 );
}

CSkillDelayManager::~CSkillDelayManager()
{
	m_htSkillUse.SetPositionHead();

	SKILLUSE* pSkillUse = NULL;
	while( pSkillUse = m_htSkillUse.GetData() )
	{
		m_mpSkillUse->Free( pSkillUse );
	}

	m_htSkillUse.RemoveAll();

	m_htPrimereSkill.SetPositionHead();
	PRIMERESKILL* pPrimereSkill = NULL;

	while( pPrimereSkill = m_htPrimereSkill.GetData() )
	{
		delete pPrimereSkill;
	}

	m_htPrimereSkill.RemoveAll();

	m_mpSkillUse->Release();
	SAFE_DELETE( m_mpSkillUse );
}

void CSkillDelayManager::LoadSkillUseInfo()
{
	CMHFile file;	

	if( !file.Init( ".\\Resource\\PremierSkill.bin", "rb" ) )
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		DWORD dwGroupNum	= file.GetDword();
		DWORD dwSkillIndex	= file.GetDword();
		DWORD dwDelay		= file.GetDword();
		
		PRIMERESKILL* pPrimereSkill = new PRIMERESKILL;
		pPrimereSkill->dwSkillIndex = dwSkillIndex;
		pPrimereSkill->dwDelay		= dwDelay;
		m_htPrimereSkill.Add( pPrimereSkill, dwSkillIndex );
	}

	file.Release();
}

BOOL CSkillDelayManager::IsPrimereSkill( DWORD dwSkillIndex )
{
	if( m_htPrimereSkill.GetData( dwSkillIndex ) == NULL ) return FALSE;

	return TRUE;
}

//��ų�� ����� �� ������ return TRUE
BOOL CSkillDelayManager::AddSkillUse( DWORD dwCharacterID, DWORD dwSkillIndex, BOOL bForce )
{
	//��¹����� �ƴϸ�
	PRIMERESKILL* pPrimereSkill = m_htPrimereSkill.GetData( dwSkillIndex );
	if( pPrimereSkill == NULL )
		return TRUE;

	DWORD dwDelay = pPrimereSkill->dwDelay;

	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );

	//��ų�� ����� ���� �ִٸ�
	if( pOld )
	{
		if( bForce )	//�޼��� ���� ���̶� ������ ����
		{
			//���� ����
			ASSERT( dwCharacterID == pOld->dwCharacterID );
			pOld->dwDelay		= dwDelay;
			pOld->dwSkillIndex	= dwSkillIndex;
			pOld->dwStartTime	= gCurTime;

			return TRUE;
		}
		else
		{
			//dwDelay�ð��� �����ٸ�
			if( gCurTime - pOld->dwStartTime + 5000 >= dwDelay )	//���� ����Ѵ�.(5��)
			{
				//���� ����
				ASSERT( dwCharacterID == pOld->dwCharacterID );
				pOld->dwDelay		= dwDelay;
				pOld->dwSkillIndex	= dwSkillIndex;
				pOld->dwStartTime	= gCurTime;
				
				//�ٸ� ������Ʈ�� ���� ���� ������ �����־�� �Ѵ�.
				SendMsgToAgentServer( dwCharacterID, dwSkillIndex );
				
				return TRUE;
			}
			else	//���� delay�� ���Ҵٸ�
			{
				return FALSE;
			}
		}
	}
	else	//��ų�� ����� ���� ���ٸ�
	{
		SKILLUSE* pSkillUse = m_mpSkillUse->Alloc();
		
		pSkillUse->dwCharacterID	= dwCharacterID;
		pSkillUse->dwSkillIndex		= dwSkillIndex;
		pSkillUse->dwDelay			= dwDelay;
		pSkillUse->dwStartTime		= gCurTime;
		
		m_htSkillUse.Add( pSkillUse, dwCharacterID );

		//�ٸ�������Ʈ�� ������ ������ �Ѵ�.
		if( !bForce )	//�ٸ�������Ʈ�� ���� ���� �޼��� �ϰ��� �Ⱥ�����.
			SendMsgToAgentServer( dwCharacterID, dwSkillIndex );

		return TRUE;
	}
}
/*
void CSkillDelayManager::RemoveSkillUse( DWORD dwCharacterID )
{
	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );

	if( pOld )
	{
		m_htSkillUse.Remove( dwCharacterID );
		m_mpSkillUse->Free( pOld );
	}
}
*/

//send other agent!
void CSkillDelayManager::SendMsgToAgentServer( DWORD dwCharacterID, DWORD dwSkillIndex )
{
	MSG_DWORD2 msg;
	msg.Protocol	= MP_SKILL;
	msg.Category	= MP_SKILL_START_SYN;
	msg.dwObjectID	= dwCharacterID;
	msg.dwData1		= dwCharacterID;
	msg.dwData2		= dwSkillIndex;

	g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
}


void CSkillDelayManager::SendSkillDelayMsgToClient( DWORD dwCharacterID )
{

	SKILLUSE* pOld = m_htSkillUse.GetData( dwCharacterID );
	
	if( pOld )
	{
		//dwDelay�ð��� ���� �ִٸ�
		if( gCurTime - pOld->dwStartTime + 5000 < pOld->dwDelay )	//���� ����Ѵ�.(5��)
		{
			ASSERT( dwCharacterID == pOld->dwCharacterID );
			//Ŭ���̾�Ʈ�� �����̸� �����ش�.			
			MSG_DWORD2 msg;
			msg.Category	= MP_SKILL;
			msg.Protocol	= MP_SKILL_DELAY_NOTIFY;
			msg.dwObjectID	= dwCharacterID;
			msg.dwData1		= pOld->dwDelay;	//���� �����̽ð�			
			msg.dwData2		= pOld->dwDelay - ( gCurTime - pOld->dwStartTime );	//�����ð�

			g_Network.Send2User( (MSGBASE*)&msg, sizeof(msg) );
		}
	}
}