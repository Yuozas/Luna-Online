// FameManager.cpp: implementation of the CFameManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FameManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFameManager::CFameManager()
{
	m_bIsUpdated = FALSE;
	m_StartUpdateTime = 0;
	m_UpdatedDate = 0;
}

CFameManager::~CFameManager()
{

}

void CFameManager::ChangePlayerFame(CPlayer* pPlayer, BYTE eFameCase)
{
	FAMETYPE fameval = 0;
	switch(eFameCase)
	{
	//���� ������ ���� ���
	case eFame_beMaster:
		{
			fameval = pPlayer->GetFame() + 30;
		}
		break;
	case eFame_beMember:
		{
			fameval = pPlayer->GetFame() + 10;
		}
		break;
	case eFame_beMembertoSenior:
		{
			fameval = pPlayer->GetFame() + 5;
		}
		break;
	case eFame_beMembertoViceMaster:
		{
			fameval = pPlayer->GetFame() + 15;
		}
		break;
	case eFame_beSeniortoViceMaster:
		{
			fameval = pPlayer->GetFame() + 10;
		}
		break;
		//����Ǿ��� ��
	case eFame_beSeniortoMember:
		{
			fameval = pPlayer->GetFame() - 10;
		}
		break;
	case eFame_beViceMastertoSenior:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;
	case eFame_beViceMastertoMember:
		{
			fameval = pPlayer->GetFame() - 25;
		}
		break;
		//���������
	case eFame_beVicemastertoNotmember:
		{
			fameval = pPlayer->GetFame() - 25;
		}
		break;
	case eFame_beSeniortoNotmember:
		{
			fameval = pPlayer->GetFame() - 20;
		}
		break;
	case eFame_beMembertoNotmember:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;

	//���� ��ü�� 
	case eFame_BreakupMaster:
		{
			fameval = pPlayer->GetFame() - 70;
		}
		break;
	case eFame_BreakupViceMaster:
		{
			fameval = pPlayer->GetFame() - 30;
		}
		break;
	case eFame_BreakupSenior:
		{
			fameval = pPlayer->GetFame() - 20;
		}
		break;
	case eFame_BreakupMember:
		{
			fameval = pPlayer->GetFame() - 15;
		}
		break;
	}

	SetPlayerFame(pPlayer, fameval);
	FameCharacterUpdate(pPlayer->GetID(), fameval); //db update
}

void CFameManager::ChangePlayersVSPlayersFame(CPlayer** ppWinPlayers, int WinerNum, CPlayer** ppLosePlayer, int LoserNum, BYTE FameCase)
{
}

void CFameManager::Process()
{
}

BOOL CFameManager::IsTimetoFameUpdate()
{	
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	if(((BYTE)ti.wDay != m_UpdatedDate) && (m_bIsUpdated == TRUE)) //���� ������Ʈ ������ ���� ����.
		m_bIsUpdated = FALSE;

	if((m_bIsUpdated == FALSE) && (ti.wHour >= m_StartUpdateTime))
	{
		m_UpdatedDate = (BYTE)ti.wDay;
		return TRUE;
	}

	return FALSE;
}
	

void CFameManager::MunpaFameUpdate(WORD MapNum)
{
	m_bIsUpdated = TRUE;
//	FameMunpaUpdate(0, MapNum);
}


void CFameManager::SetPlayerFame(CPlayer* pPlayer, FAMETYPE FameVal)
{
	pPlayer->SetFame(FameVal);
	SendFame(pPlayer, FameVal);	
}

void CFameManager::SendFame(CPlayer* pPlayer, FAMETYPE FameVal)
{
	MSG_FAME msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_FAME_NOTIFY;
	msg.Fame = FameVal;
	
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CFameManager::SetMunpaFame(DWORD MunpaIDX, FAMETYPE Val)
{
}

void CFameManager::ChangePlayerBadFame(CPlayer* pPlayer, int eBadFameKind)
{
	FAMETYPE BadFame = pPlayer->GetBadFame() + eBadFameKind;
	if( BadFame < 0 ) BadFame = 0;
	else if( BadFame > 1000000000 ) BadFame = 1000000000;

	pPlayer->SetFame( BadFame );
	BadFameCharacterUpdate( pPlayer->GetID(), BadFame ); //db update
	SendBadFameMsg( pPlayer, BadFame );
}

void CFameManager::SendBadFameMsg(CPlayer* pPlayer, FAMETYPE BadFameVal)
{
	MSG_FAME msg;
	msg.Category	= MP_CHAR;
	msg.Protocol	= MP_CHAR_BADFAME_NOTIFY;
	msg.dwObjectID	= pPlayer->GetID();
	msg.Fame		= BadFameVal;
	
	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
}

void CFameManager::SetPlayerBadFame(CPlayer* pPlayer, FAMETYPE BadFameVal)
{
	pPlayer->SetBadFame(BadFameVal);
}

BOOL CFameManager::BailBadFame(CPlayer* pPlayer, FAMETYPE BailBadFame)
{
	// �Ǹ�ġ�� �´���..
	if( pPlayer->GetBadFame() < MIN_BADFAME_FOR_BAIL + BailBadFame )
		return FALSE;

	// ���� �������
	MONEYTYPE CurMoney = pPlayer->GetMoney();
	if( CurMoney < (MONEYTYPE)(BailBadFame * BAIL_PRICE) )
		return FALSE;

	return TRUE;
}
