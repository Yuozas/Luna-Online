// AttackManager.cpp: implementation of the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackManager.h"
#include "Network.h"
#include "PackedData.h"
#include "ServerSystem.h"
#include "CharMove.h"
#include "RegenNPC.h"
#include "UserTable.h"
#include "ObjectStateManager.h"

#include "PartyManager.h"
#include "Party.h"
#include "SiegeWarMgr.h"
#include "Player.h"
#include "GuildManager.h"
#include "pet.h"
#include "petmanager.h"

#include "..\[CC]Header\CommonCalcFunc.h"
// 080616 LUJ, �̺�Ʈ �߻� �ÿ� ��ų�� �׼��� ���ϱ� ���� ����
#include "Event.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttackManager::CAttackManager()
{
	// 080121 KTH -- ���ǿ����� ��û���� �⺻ 35%�� ���� �Ѵ�.
	m_PvpDamageRatio = 35;
}

CAttackManager::~CAttackManager()
{

}

void CAttackManager::sendDieMsg(CObject * pAttacker,CObject* pTarget)
{
	// 070417 LYW --- AttackManager : Modified function sendDieMsg.
	/*
	MSG_DWORD2 m2c;
	m2c.Category = MP_USERCONN;

	if(pTarget->GetObjectKind() & eObjectKind_Monster)
		m2c.Protocol = MP_USERCONN_MONSTER_DIE;
	else if(pTarget->GetObjectKind() == eObjectKind_Player)
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

	m2c.dwObjectID = pAttacker->GetID();
	m2c.dwData1 = pAttacker->GetID();
	m2c.dwData2 = pTarget->GetID();
	
	PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
	*/

	MSG_DWORD2 m2c;
	m2c.Category	= MP_USERCONN;

	if(pTarget->GetObjectKind() & eObjectKind_Monster)
	{
		m2c.Protocol	= MP_USERCONN_MONSTER_DIE;

		m2c.dwObjectID	= pAttacker->GetID();
		m2c.dwData1		= pAttacker->GetID();
		m2c.dwData2		= pTarget->GetID();
		
		PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
	}
	else if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

		m2c.dwObjectID	= pAttacker->GetID();
		m2c.dwData1		= pAttacker->GetID();
		m2c.dwData2		= pTarget->GetID();
		
		PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));

		// check battle mode.
		/*if( !((CPlayer*)pTarget)->IsShowdown() )
		{
			DownExpLevel(pTarget, pAttacker) ;
		}*/
	}
	else if(pTarget->GetObjectKind() & eObjectKind_Pet)
	{
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

		m2c.dwObjectID	= pAttacker->GetID();
		m2c.dwData1		= pAttacker->GetID();
		m2c.dwData2		= pTarget->GetID();
		
		PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
	}

}
// 070418 LYW --- AttackManager : ����ġ �϶� �� ���� �ٿ� ó��
// 070418 LYW --- AttackManager : Add funtion to setting exp and level when the character was killed.
//void CAttackManager::DownExpLevel( CObject* pPlayer, CObject* pMonster )				// ����ġ �� ���� �ٿ� ó���� �ϴ� �Լ�.
//{
//	LEVELTYPE CurLevel = pPlayer->GetLevel() ;											// �÷��̾��� ���� ������ �޴´�.
//
//																						// 070516 LYW --- ����ġ �϶� ���� 10 ���� ����
//	if( CurLevel < 10 ) return ;														// ���� ������ 10���� ������ ���� ó���� �Ѵ�.
//	
//	CPlayer* pHero = ((CPlayer*)pPlayer) ;												// �÷��̾����� hero ������ �޴´�.
//
//	BYTE PenaltyNum = random(3, 5) ;													// �г�Ƽ ���� ���� 3~5���̿��� �����ϰ� �̴´�.
//
//	EXPTYPE CurExp = pHero->GetPlayerExpPoint() ;										// �÷��̾��� ���� ����ġ�� �޴´�.
//
//	EXPTYPE GoalExp		= GAMERESRCMNGR->GetMaxExpPoint(CurLevel) ;						// �÷��̾��� ������ ��ǥ ����ġ�� �޴´�.
//
//	DWORD dwExpA = GoalExp * PenaltyNum ;												// �г�Ƽ ��ġ�� ���Ѵ�.
//
//	EXPTYPE PenaltyExp = (EXPTYPE)(dwExpA / 100) ;										// �г�Ƽ ����ġ�� ���Ѵ�.
//
//	DWORD dwExp = 0 ;																	// ������ ����ġ ������ �����ϰ� 0���� �����Ѵ�.
//
//	BOOL bLevelDown = FALSE ;															// ���� �ٿ�θ� ���� ������ �����ϰ� FALSE�� ������ �Ѵ�.
//
//	if( CurExp >= PenaltyExp )															// ���� ����ġ�� �г�Ƽ ����ġ�� ũ�ų� ������,
//	{
//		dwExp = CurExp - PenaltyExp ;													// ������ ����ġ�� �����Ѵ�.
//
//		ASSERT( dwExp >= 0 ) ;															// ����ġ�� 0�̻��̾�߸� �Ѵ�.
//
//		pHero->SetPlayerExpPoint( dwExp ) ;												// �÷��̾��� ����ġ�� �����Ѵ�.
//
//		MSG_DWORD3 msg ;																// ����ġ �϶� �޽��� ���� ����ü�� �����Ѵ�.
//
//		msg.Category	= MP_USERCONN ;													// ī�װ��� MP_USERCONN���� �����Ѵ�.
//		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNEXP_NOTICE ;						// ���������� ����ġ �϶� �������ݷ� �����Ѵ�.
//		msg.dwObjectID	= pHero->GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
//		msg.dwData1		= (DWORD)PenaltyNum ;											// �г�Ƽ ��ġ�� �����Ѵ�.
//		msg.dwData2		= dwExp ;														// ����ġ�� �����Ѵ�.
//		msg.dwData3		= FALSE ;														// ��Ȱ�� �߰� ����ġ �϶����θ� FALSE�� �����Ѵ�.
//
//		pHero->SendMsg(&msg, sizeof(msg)) ;												// �÷��̾�� �޽����� �����Ѵ�.
//	}
//	else																				// ���� ����ġ�� �г�Ƽ ����ġ���� ������,
//	{
//		bLevelDown = TRUE ;																// ���� �ٿ�θ� TRUE�� �����Ѵ�.
//
//		dwExp = PenaltyExp - CurExp ;													// ������ ����ġ�� �����Ѵ�.
//
//		ASSERT( dwExp >= 0 ) ;															// ����ġ�� 0�̻��̾�� �Ѵ�.
//
//		GoalExp = GAMERESRCMNGR->GetMaxExpPoint(CurLevel-1) ;							// �Ѵܰ� �Ʒ� ������ ������ ��ǥ ����ġ�� ���Ѵ�.
//		pHero->SetLevel( CurLevel -1 ) ;												// �÷��̾��� ����ġ�� �����Ѵ�.
//		pHero->SetPlayerExpPoint(GoalExp-dwExp) ;										// �÷��̾��� ����ġ�� �����Ѵ�.
//
//		//level down.
//		MSG_DWORD4 msg ;																// ���� �ٿ� �޽��� ���� ����ü�� �����Ѵ�.
//
//		msg.Category	= MP_USERCONN ;													// ī�װ��� MP_USERCONN���� �����Ѵ�.
//		msg.Protocol	= MP_USERCONN_CHARACTER_DOWNLEVEL_NOTICE ;						// ���������� �����ٿ� �������ݷ� �����Ѵ�.
//		msg.dwObjectID	= pHero->GetID() ;												// �÷��̾��� ���̵� �����Ѵ�.
//		msg.dwData1		= pHero->GetLevel() ;											// �÷��̾��� ������ �����Ѵ�.
//		msg.dwData2		= GoalExp-dwExp ;												// �÷��̾��� ����ġ�� �����Ѵ�.
//		msg.dwData3		= (DWORD)PenaltyNum ;											// �г�Ƽ ��ġ�� �����Ѵ�.
//		msg.dwData4		= FALSE ;														// ��Ȱ�� �߰� ����ġ �϶� ���θ� FALSE�� �����Ѵ�.
//
//		pHero->SendMsg(&msg, sizeof(msg)) ;												// �÷��̾�� �޽����� �����Ѵ�.
//	}
//}

WORD CAttackManager::GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical);
	
	// RaMa - ũ��Ƽ���϶� ����Ȯ�� �߰��ؾ���
	if(pDamageInfo->bCritical)
	{
		attackPhyDamage *= 1.5f;
/*
		// ����
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
		{
			// RaMa - 04.11.18
			// �����۸������� ���ΰ�ȹ���� ��������( ~11.27)�� �ޱ�� ��.
			// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
		}
	}*/
	}
//#endif

	attackPhyDamage += AmplifiedPower;

//041213 KES
	attackPhyDamage *= fDecreaseDamageRate;
//

	//PvP �϶��� ���ݷ��� 50%��
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )

		attackPhyDamage *= 0.5f;

	// 06. 03 �������� - �̿���
	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);

	// RaMa - 04.11.24   ->����������
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)	
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;


	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption�߰�
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
//		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage*0.01f+1.0f);

		//SW060719 ��������Ʈ
		//GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );
		
		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
//			if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;

//			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
//			attackPhyDamage *= (fdam+1.0f);
		}
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
//			if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;

//			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//			attackPhyDamage *= (1.0f-fdam);
		}
	}

CalcEnd:
	return (WORD)attackPhyDamage;
}



WORD CAttackManager::GetSkillPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
		RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical );
	
	// RaMa - ũ��Ƽ���϶� ����Ȯ�� �߰��ؾ���
	if(pDamageInfo->bCritical)
	{
		attackPhyDamage *= 1.5f;
/*
		// ����
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
		{
			// RaMa - 04.11.18
			// �����۸������� ���ΰ�ȹ���� ��������( ~11.27)�� �ޱ�� ��.
			// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
		}
*/
	}
//#endif

	attackPhyDamage += AmplifiedPower;

//041213 KES
	attackPhyDamage *= fDecreaseDamageRate;
//
	
	//PvP �϶��� ���ݷ��� 50%��
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
	attackPhyDamage *= 0.5f;

	// 06. 03 �������� - �̿���
	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);
	
	// RaMa - 04.11.24   ->����������
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;


	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption�߰�, AvatarItemOption�߰�(05.08.16)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage*0.01f) + 
//			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage*0.01f);
//		attackPhyDamage *= (fdam+1.0f);

		//SW060719 ��������Ʈ
		//GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );

		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
//			if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			{
//				fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
//				attackPhyDamage *= (fdam+1.0f);
			}
		}
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
//			if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			{
//				fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//				attackPhyDamage *= (1.0f-fdam);
			}
		}
	}

//CalcEnd:

	return (WORD)attackPhyDamage;
}
WORD CAttackManager::GetSkillAttrDamage(CObject* pAttacker,CObject* pTargetObject,
		WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
		float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate )
{
	double attackAttrDamage = m_ATTACKCALC.getAttributeAttackPower(pAttacker,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
//041213 KES
	attackAttrDamage *= fDecreaseDamageRate;

	if(pDamageInfo->bDecisive)
	{
		attackAttrDamage *= 2.25f;

	}
//
	//PvP �϶��� ���ݷ��� 50%��

	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
	{
		attackAttrDamage *= 0.5f;
	}


	// 06. 03 �������� - �̿���
	float RegVal = pTargetObject->GetAttDefense(Attrib);

	if( RegVal > 1 )		RegVal = 1;
	if( RegVal < 0 )		RegVal = 0;

	RegVal *= 0.7f;
	// RaMa - 05.02.04  -> �ɷ��� ����
//	if( pTargetObject->GetObjectKind() == eObjectKind_Player )
//		RegVal += (((CPlayer*)pTargetObject)->GetShopItemStats()->RegistAttr*0.01f*0.7f);
	
	double resAttrDamage = (attackAttrDamage * (1 - RegVal) );

	// RaMa - 04.11.24   ->����������
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption�߰�, AvatarItemOption�߰�(05.08.16)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
//		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage*0.01f)+
//			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage*0.01f);
//		resAttrDamage = (resAttrDamage*(fdam+1.0f));

		//SW060719 ��������Ʈ
		//GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage );


		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
//			if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;

//			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown*0.01f;
//			resAttrDamage = (resAttrDamage*(fdam+1.0f));
		}
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> �ذ񰡸� �ɼ��߰�
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
//			if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
//				g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;

//			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
//			resAttrDamage = (resAttrDamage*(1.0f-fdam));
		}
	}

CalcEnd:	

	return (WORD)resAttrDamage;
}

void CAttackManager::Attack(BOOL bSkill, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
							float PhyAttackRate,
							WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
							float fCriticalRate,
							RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
							WORD AmplifiedPowerAttrib, BOOL bContinueAttack )
{	
	pDamageInfo->Clear();

//ȸ��	//SW060330 �̵� Object->CalcRealDamage(... ����..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//�� ���� ȸ��
		BOOL rtDodge = FALSE;
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}

		//SW060911 ȫ���û ����.
		if( ((CPlayer*)pTarget)->GetUserLevel() <= eUSERLEVEL_GM && ((CPlayer*)pTarget)->IsVisible() == FALSE )
		{
			return;
		}
	}

	// 2005 ũ�������� �̺�Ʈ
	if(( pAttacker->GetObjectKind() == eObjectKind_Player ) && ( pTarget->GetObjectKind() == eObjectKind_Monster ))
	{	// �÷��̾ ���� ���ݽ�
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		CMonster* pMonster = (CMonster*)pTarget;
		
		// ���� �̺�Ʈ ���� �������̶��
		if( pPlayer->GetWeaponEquipType() == WP_EVENT )
		{
			// ������ ����Ѱ��̸� �����Ѵ�.
			if(bSkill)
				return;

			// ���� ���ݵ� ����
			if(bContinueAttack)
				return;
		}
		else if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2)
			return;
	}
	
	if(pTarget->GetInited() == FALSE)
		return;

	WORD AttrDamage =0,PhyDamage =0;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}
	
	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
//		ASSERT(0);
		return;
	}

	if(PhyAttackRate > 0.000001f)
	{
		if(bSkill)
		{
			PhyDamage = GetSkillPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
		else
		{
			PhyDamage = GetComboPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
	}

	if(AttAttackMax != 0)
	{
		AttrDamage = GetSkillAttrDamage(pAttacker,pTarget,Attrib,AttAttackMin,AttAttackMax,AttAttackRate,fCriticalRate,pDamageInfo,fDecreaseDamageRate );
	}

//��ؿ� ���� 1.5��.. ���Ⱑ ������ ��ġ��.


	pAttacker->CalcRealAttack(pTarget, PhyDamage, AttrDamage, pDamageInfo, bContinueAttack );


//== �̻��ϰ� �Ǿ��� T_T �ϴ� �ӽ÷� �׳� ���ô�!
	WORD UpAttackDamage = 0;
	if( PhyDamage + AttrDamage < (WORD)pDamageInfo->RealDamage )
		UpAttackDamage = (WORD)(( pDamageInfo->RealDamage - ( PhyDamage + AttrDamage ) ) / 2);
//===
	
	// �������ÿ��� ������ ����
	float fsiegedamage = 0.2f;

	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
	{
		if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
		{
			// ����ؼ� 0�� ���ö��� ���ؼ� +1
			if( PhyDamage )
				PhyDamage = (WORD)(PhyDamage*fsiegedamage + 1);
			if( AttrDamage )
				AttrDamage = (WORD)(AttrDamage*fsiegedamage + 1);
			if( UpAttackDamage )
				UpAttackDamage = (WORD)(UpAttackDamage*fsiegedamage + 1);
			if( pDamageInfo->RealDamage )
				pDamageInfo->RealDamage = (TARGETDATA)(pDamageInfo->RealDamage*fsiegedamage + 1);
		}
	}
	
	pTarget->CalcRealDamage(pAttacker,PhyDamage+UpAttackDamage,AttrDamage+UpAttackDamage,pDamageInfo);

	if(bCounter)
		pTarget->CalcCounterDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo);
	else
		pDamageInfo->CounterDamage = 0;

//-----�޴� �������� ���� ü�� ���� ���

	pTarget->CalcReverseVampiric(pDamageInfo);	//����Ʈ ó���� ���Ѵ� --;
//-----


//	BOOL bVampiric = TRUE;
//	if(bVampiric)
//	{
		pAttacker->CalcVampiric(pDamageInfo);
//	}

	// 06. 03 �������� - �̿���
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pAttacker)->IsMussangMode())
			pDamageInfo->RealDamage = (TARGETDATA)(pDamageInfo->RealDamage * 1.05);
	}

	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
	
	if(pDamageInfo->CounterDamage != 0)
	{
		// �ݰ��� ���Ӽ� �������̴�. (cf: ���Ӽ��� �ݰݴ����� �����Ƿ� �ݰ��� �ݰ��� ������ ����.)
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = (WORD)pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
			ATTACKMGR->sendDieMsg(pTarget,pAttacker);
			pAttacker->Die(pTarget);
		}
	}

	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

void CAttackManager::RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,float RecoverLifeVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal = 0;
	DWORD val = 0;	//KES 070918 val; --> val = 0;  (val�� �����Ⱚ�� ���·� AddLife�� �� ����� �־. RecoverMana()�Լ��� ����)

	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		float fPlus = ( ( CPlayer* )pOperator )->GetMagicAttack() / 10;
		val = ( DWORD )( RecoverLifeVal + fPlus );

		//---KES Aggro 070918
		//ȸ������ 50%�� ��׷η� �����ش�. (���� ȸ������ �ƴ϶�, ȸ���Ϸ��� ���� 50%��. ��ȹ������ ������ �ʿ��ϸ� �ڵ� ��ġ�� �Ʒ���.)
		//��ȹ �������� �ּ��ϰ� �ڵ� �Ʒ��� ����
		//if( val && pObject->GetObjectKind() == eObjectKind_Player )
		//{
		//	((CPlayer*)pObject)->AddAggroToMyMonsters( (int)RecoverLifeVal / 2, (CPlayer*)pOperator );
		//}
		//-------------------
	}
	else if( pOperator->GetObjectKind() == eObjectKind_Pet )
	{
		float fPlus = pOperator->GetMagicAttackPower() / 10;
		val = ( DWORD )( RecoverLifeVal + fPlus );
	}

	pObject->AddLife( val, &realAddVal, FALSE );
	pResultInfo->HealLife = realAddVal;

	//---KES Aggro 071005
	//���� ȸ������ 1/3 �� ��׷η� ��������.
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( realAddVal && pObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pObject)->AddAggroToMyMonsters( (int)realAddVal / 3, pOperator );
		}
		else if( realAddVal && pObject->GetObjectKind() == eObjectKind_Pet )
		{
			((CPet*)pObject)->AddAggroToMyMonsters( (int)realAddVal / 3, pOperator );
		}
	}
	if( pOperator->GetObjectKind() == eObjectKind_Pet )
	{
		if( realAddVal && pObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pObject)->AddAggroToMyMonsters( (int)realAddVal / 3, pOperator );
		}
		else if( realAddVal && pObject->GetObjectKind() == eObjectKind_Pet )
		{
			((CPet*)pObject)->AddAggroToMyMonsters( (int)realAddVal / 3, pOperator );
		}
	}
	//--------------------
}
void CAttackManager::RecoverMana(CObject* pOperator,CObject* pObject,WORD m_Attrib,float RecoverManaVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal = 0;
	DWORD val = 0;

	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		float fPlus = ( ( CPlayer* )pOperator )->GetMagicAttack() / 10;
		val = ( DWORD )( RecoverManaVal + fPlus );
	}
	pObject->AddMana( val, &realAddVal );
	pResultInfo->RechargeMana = realAddVal;
}


void CAttackManager::AttackAbs( CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo )
{

	if( nAbsKind == 0 ) return;

	pDamageInfo->Clear();

//ȸ��	//SW060330 �̵� Object->CalcRealDamage(... ����..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//�� ���� ȸ��
		BOOL rtDodge = FALSE;
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}
	}
	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}
	
	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}

	DWORD TargetLife = pTarget->GetLife();
	DWORD MinusLife = 0;

	if( nAbsKind & eAAK_LIFE )
	{
		MinusLife = (DWORD)(TargetLife * AttackRate);
	}

	pDamageInfo->RealDamage = MinusLife;

/*
	BOOL bVampiric = TRUE;
	if(bVampiric)
	{
		pAttacker->CalcVampiric(pDamageInfo);
	}
*/	
	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
/*	
	if(pDamageInfo->CounterDamage != 0)
	{
		// �ݰ��� ���Ӽ� �������̴�. (cf: ���Ӽ��� �ݰݴ����� �����Ƿ� �ݰ��� �ݰ��� ������ ����.)
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
			ATTACKMGR->sendDieMsg(pTarget,pAttacker);
			pAttacker->Die(pTarget);
		}
	}
*/
	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}


						
void CAttackManager::AttackJinbub( CObject* pAttacker, CObject* pTarget, DWORD AttackPower, 
								   DWORD AttackMin,DWORD AttackMax, RESULTINFO* pDamageInfo,
								   float fDecreaseDamageRate )
{
	pDamageInfo->Clear();

	//ȸ��	//SW060330 �̵� Object->CalcRealDamage(... ����..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//�� ���� ȸ��
		BOOL rtDodge = FALSE;
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			return;
		}
	}


	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}
	
	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}

	if( AttackPower == 0 )
		return;

//==================
	if( AttackMax < AttackMin )
		AttackMin = AttackMax;

	DWORD PlusAttack = random( AttackMin, AttackMax );

	DWORD RealAttack = GetJinbubDamage( pAttacker, pTarget, AttackPower+PlusAttack,
										pDamageInfo, fDecreaseDamageRate );
	
	
	// �������ÿ��� ������ ����
	float fsiegedamage = 0.2f;

	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
	{
		if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
		{
			// ����ؼ� 0�� ���ö��� ���ؼ� +1
			if( RealAttack )
				RealAttack = (WORD)(RealAttack*fsiegedamage + 1);
			if( pDamageInfo->RealDamage )
				pDamageInfo->RealDamage = (TARGETDATA)(pDamageInfo->RealDamage*fsiegedamage + 1);
		}
	}

	//1. ���ݷ� ��ȭ ����.
	//2.ȸ��/����ߵ�����/������ ��� (���������̴�.)
	pTarget->CalcRealDamage( pAttacker, (WORD)RealAttack, 0, pDamageInfo );
	//3.�ݰ��� ����.
	//4.���ݹ޾����� �� ���.
	pTarget->CalcReverseVampiric(pDamageInfo);	//����Ʈ ó���� ���Ѵ� --;
	//5.�ǻ��� ����.
	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
	
	//6.�ݰ��� ����.

//============
	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

DWORD CAttackManager::GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
		RESULTINFO* pDamageInfo,float fDecreaseDamageRate)
{
//041213 KES
	DWORD attackPhyDamage = (DWORD)(AttackPower * fDecreaseDamageRate);
//

	//PvP �϶��� ���ݷ��� 50%��
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
		attackPhyDamage = (DWORD)(attackPhyDamage*0.5f);
	// 06. 03 �������� - �̿���
	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage = (DWORD)(attackPhyDamage*(1-defencePhyLevel));
	
	// RaMa - 04.11.24   ->����������
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		// attackPhyDamage *= gDamageRate;
		attackPhyDamage = (DWORD)( attackPhyDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		// attackPhyDamage *= gDamageReciveRate;
		attackPhyDamage = (DWORD)(attackPhyDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	return attackPhyDamage;
}

void CAttackManager::PhysicAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Accuracy, float AddDamage, WORD AddType )
{
	pDamageInfo->Clear();

	if( !pAttacker || !pTarget )
		return;

	if( pTarget->GetAbnormalStatus()->IsGod )
	{
		return;
	}

	// �⺻ ��ġ ����
	float Avoid = 0;
	float Attack = 1;
	float Defense = 1;
	float Shield = 0;
	float Block = 0;
	float CriticalRate = 0;
	float CriticalDamage = 0;
	float Dexterity = 0;
	WORD ALevel = pAttacker->GetLevel();
	WORD DLevel = pTarget->GetLevel();
	BOOL IsSpecial = 0;
	// 080910 LUJ, ��� �� ������ ��ġ�� ����ϱ� ���� ����
	float vitality = 0;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		
		Accuracy += pPlayer->GetAccuracy();

		int min = ( int )( pPlayer->GetPhysicAttackMin() );
		int max = ( int )( pPlayer->GetPhysicAttackMax() );

		if( max > min )
		{
			Attack = random( min, max );
		}
		else
		{
			Attack = min;
		}

		CriticalRate = pPlayer->GetCriticalRate();
		CriticalDamage = pPlayer->GetCriticalDamage();
		Dexterity = pPlayer->GetDexterity();
	}
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		CMonster* pMonster = (CMonster*)pAttacker;
		BASE_MONSTER_LIST* pInfo = pMonster->GetSMonsterList();

		float accuracy = pInfo->Accuracy + pMonster->GetBuffStatus()->Accuracy;
		if( accuracy > 0 )
		{
			Accuracy = accuracy;
		}

		float min = pInfo->AttackPhysicalMin + pMonster->GetBuffStatus()->PhysicAttack;
		float max = pInfo->AttackPhysicalMax + pMonster->GetBuffStatus()->PhysicAttack;

		if( min < 0 ) min = 0;
		if( max < 0 ) max = 0;

		Attack = random( min, max );
			
		float criticalRate = pInfo->CriticalPercent + pMonster->GetBuffStatus()->CriticalRate;

		if( criticalRate )
		{
			CriticalRate = criticalRate;
		}
	}
	else if( pAttacker->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* pPet = ( CPet* )pAttacker;

		PlayerStat* pStat = pPet->GetStat();

		Accuracy += pStat->mAccuracy.mPlus;

		int min = ( int )( pStat->mPhysicAttack.mPlus );
		int max = ( int )( pStat->mPhysicAttack.mPlus );

		if( max > min )
		{
			Attack = random( min, max );
		}
		else
		{
			Attack = min;
		}

		Attack *= PETMGR->GetFriendlyPenalty( pPet );

		CriticalRate = pStat->mCriticalRate.mPlus;
		CriticalDamage = pStat->mCriticalDamage.mPlus;
		Dexterity = pStat->mDexterity.mPlus;
	}
	else
	{
		ASSERT(0);
	}

	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pTarget;
		
		Avoid = pPlayer->GetAvoid();
		Defense = pPlayer->GetPhysicDefense();
		Block = pPlayer->GetBlock();
		// 080910 LUJ, �÷��̾ ������ ������ ������ ������
		Shield		= pPlayer->GetShieldDefense();
		vitality	= pPlayer->GetVitality();

		if( pAttacker->GetObjectKind() == eObjectKind_Player )	//���� ������ ���
		{
			CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
			if( pAttackPlayer->IsPKMode() )		//PK������ ����� ��������
				pAttackPlayer->SetPKStartTimeReset();

			if( pPlayer->IsPKMode() )			//PK������ ������� �¾�����
				pPlayer->SetPKStartTimeReset();
		}
	}
	else if( pTarget->GetObjectKind() & eObjectKind_Monster )
	{
		CMonster* pMonster = (CMonster*)pTarget;
		BASE_MONSTER_LIST* pInfo = pMonster->GetSMonsterList();

		IsSpecial = pInfo->SpecialType;

		float avoid = pInfo->Avoid + pMonster->GetBuffStatus()->Avoid;
		if( avoid > 0 )
			Avoid = avoid;
		float defense = pInfo->PhysicalDefense + pMonster->GetBuffStatus()->PhysicDefense;
		if( defense > 0 )
		{
			defense = random( ( defense * 0.8f ), ( defense * 1.1f ) );
			Defense = defense;
		}
	}
	else if( pTarget->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* pPet = (CPet*)pTarget;
		
		PlayerStat* pStat = pPet->GetStat();

		Avoid = pStat->mEvade.mPlus;
		Defense = pStat->mPhysicDefense.mPlus;
		Block = 0;
		Shield;

		Defense *= PETMGR->GetFriendlyPenalty( pPet );
		if( pAttacker->GetObjectKind() == eObjectKind_Player )	//���� ������ ���
		{
			CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
			if( pAttackPlayer->IsPKMode() )		//PK������ ����� ��������
				pAttackPlayer->SetPKStartTimeReset();
		}
	}
	else
	{
		ASSERT(0);
	}


	// ���߷� ���
	float ACC = 750 - ( ( Avoid - Accuracy ) * 13 );

	if( ACC < 1000 )
	{
		if( ACC < ( ( rand() % 1000 ) + 1 ) )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;

			return;
		}
	}

	// ������ ���
	float damage = 1;
	if( Attack > ( Defense + Shield ) )
	{
		damage = Attack - ( Defense + Shield );
	}

	if( DLevel > ALevel )
	{
		damage = damage * ( 1 + ( ( ALevel - DLevel ) * 0.03 ) );
	}

	// 080910 LUJ, ��� ó��(��ȹ: ������)
	if( 0 < Shield )
	{	
		const float random = float( rand() ) / RAND_MAX;

		if( Block > random )
		{
			// 081029 LUJ, ��ȹ ������ ���� ����
			damage					= damage * ( 0.6f - ( vitality * 0.00025f ) ) - ( Shield * 2 );
			pDamageInfo->bBlock		= TRUE;
			CriticalRate			= 0;
		}
	}

	// ũ��Ƽ��
	if( CriticalRate && CriticalRate < 1500 )
	{
		if( CriticalRate >= ( ( rand() % 1500 ) + 1 ) )
		{
			damage = ( damage * ( 150 + ( Dexterity * 0.3 ) ) * ( 100 + CriticalDamage ) ) / 10000;
			pDamageInfo->bCritical = TRUE;
		}
	}
	else if( CriticalRate >= 1500 )
	{
		damage = ( damage * ( 150 + ( Dexterity * 0.3 ) ) * ( 100 + CriticalDamage ) ) / 10000;
		pDamageInfo->bCritical = TRUE;
	}

	// �� ���

	// ��ų ������ ����
	
	if( AddType == 1 )
	{
		damage += AddDamage;
	}
	else if( AddType == 2 )
	{
		damage *= ( ( 100 + AddDamage ) / 100.f );
	}

	switch( IsSpecial )
	{
	case 1: damage = 1; break;		
	}

	if( damage < 1 )
	{
		damage = 1;
	}

	// 080115 KTH -- PVP Damage Ratio Setting
	if( pTarget->GetObjectKind() == eObjectKind_Player && pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		damage *= (this->GetPvpDamageRatio() / 100);
	}

	pDamageInfo->RealDamage = (DWORD)damage;

	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
	
	if(newLife == 0)
	{
		// 080616 LUJ, ��� �� �̺�Ʈ�� ó���ϵ��� �Ѵ�
		// 080708 LUJ, ������/�����ڸ� ��� ���ڷ� �ѱ��
		pTarget->Execute( CDieEvent( pAttacker, pTarget ) );
		pAttacker->Execute( CKillEvent( pAttacker, pTarget ) );

		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

// 080912 LUJ, ���� ũ��Ƽ�� ���� ����(��ȹ: �۰���)
void CAttackManager::MagicAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Accuracy, float AddDamage, WORD AddType )
{
	pDamageInfo->Clear();

	if( !pAttacker || !pTarget )
		return;

	if( pTarget->GetAbnormalStatus()->IsGod )
	{
		return;
	}

	// �⺻ ��ġ ����
	float Attack = 1;
	float Defense = 1;
	WORD  AttackerLevel = pAttacker->GetLevel();
	WORD  TargetLevel = pTarget->GetLevel();
	float  FailValue = 1;
	float  CriticalValue = 1;
	WORD  IsSpecial = 0;
	float Int = 1;
	float CriticalRate = pAttacker->GetBuffStatus()->MagicCriticalRate;
	// 080912 LUJ, ũ��Ƽ�� ���� ��꿡 ����ϱ� ���� �߰�
	float wisdom				= 0;
	float castingProtectValue	= pAttacker->GetBuffStatus()->CastingProtect;
	float castingProtectRate	= pAttacker->GetRateBuffStatus()->CastingProtect + 1.0f;

	if( AttackerLevel > TargetLevel )
	{
		CriticalValue = (float)(AttackerLevel - TargetLevel);
	}
	else if( TargetLevel > AttackerLevel )
	{
		FailValue = (float)(TargetLevel - AttackerLevel);
	}

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		
		Attack = pPlayer->GetMagicAttack();
		Int = pPlayer->GetIntelligence();

		CriticalRate += pPlayer->GetPassiveStatus()->MagicCriticalRate;
		// 080912 LUJ, ũ��Ƽ�� ���� ��꿡 ����ϱ� ���� ����
		wisdom 				= 	pPlayer->GetWisdom();
		castingProtectValue	+= 	pPlayer->GetPassiveStatus()->CastingProtect;
		castingProtectRate	+= 	pPlayer->GetRatePassiveStatus()->CastingProtect;
	}
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		CMonster* pMonster = (CMonster*)pAttacker;
		BASE_MONSTER_LIST* pInfo = pMonster->GetSMonsterList();

		float attack = pInfo->AttackMagicalMax + pMonster->GetBuffStatus()->MagicAttack;
		if( attack > 0 )
		{
			Attack = attack;
		}
	}
	else if( pAttacker->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* pPet = (CPet*)pAttacker;
		
		PlayerStat* pStat = pPet->GetStat();

		Attack = pStat->mMagicAttack.mPlus;
		Int = pStat->mIntelligence.mPlus;

		Attack *= PETMGR->GetFriendlyPenalty( pPet );
		CriticalRate += pPet->GetPassiveStatus()->MagicCriticalRate;
	}
	else
	{
		ASSERT(0);
	}

	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pTarget;
		
		Defense = pPlayer->GetMagicDefense();

		if( pAttacker->GetObjectKind() == eObjectKind_Player )	//���� ������ ���
		{
			CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
			if( pAttackPlayer->IsPKMode() )		//PK������ ����� ��������
				pAttackPlayer->SetPKStartTimeReset();

			if( pPlayer->IsPKMode() )			//PK������ ������� �¾�����
				pPlayer->SetPKStartTimeReset();
		}
	}
	else if( pTarget->GetObjectKind() & eObjectKind_Monster )
	{
		CMonster* pMonster = (CMonster*)pTarget;
		BASE_MONSTER_LIST* pInfo = pMonster->GetSMonsterList();
		
		IsSpecial = pInfo->SpecialType;

		float defense = pInfo->MagicalDefense + pMonster->GetBuffStatus()->MagicDefense;
		if( defense > 0 )
		{
			Defense = defense;
		}
	}
	else if( pTarget->GetObjectKind() == eObjectKind_Pet )
	{
		CPet* pPet = (CPet*)pTarget;
		
		PlayerStat* pStat = pPet->GetStat();

		Defense = pStat->mMagicDefense.mPlus;

		Defense *= PETMGR->GetFriendlyPenalty( pPet );
		if( pAttacker->GetObjectKind() == eObjectKind_Player )	//���� ������ ���
		{
			CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
			if( pAttackPlayer->IsPKMode() )		//PK������ ����� ��������
				pAttackPlayer->SetPKStartTimeReset();
		}
	}
	else
	{
		ASSERT(0);
	}

	// ���߷� ���
	if( Accuracy < 100 )
	{
		if( Accuracy < ( ( rand() % 100 ) + 1 ) )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;

			return;
		}
	}
	float dem = ( Attack * 1.75f ) - ( Defense * 4 );
	//float dem = 1;
	//if( int( Attack - Defense ) > 1 )
	//{
	//	dem = ( Attack * 1.75f ) - ( Defense * 4 );
	//}		
	float x = ( float )( 100 + pAttacker->GetLevel() - pTarget->GetLevel() );
	float y = 100 + ( ( dem / Defense ) * 25 );
	float z = 1 - ( ( pTarget->GetLevel() - pAttacker->GetLevel() ) * 0.01 );
	float damage = 1;

	if( ( AddDamage * x * y ) > 10000 )
	{
		damage = ( AddDamage * x * y * z ) / 10000;
	}

	// 080912 LUJ, ũ��Ƽ�� ���� ����(��ȹ: �۰���)
	{	
		castingProtectValue 	*= castingProtectRate;
		const int successRate	= int( max( 0, ( Int * 2.5f ) + ( 20.0f * CriticalValue ) + ( CriticalRate * 100 ) ) );
		const int failureRate	= int( max( 0, 10000.0f - ( 1000.0f * ( 1.0f - ( ( wisdom * 0.0005f ) + ( 0.002f * FailValue ) + ( castingProtectValue * 0.01f ) ) ) ) ) );
		const int random		= rand() % 10000;
		
		if( random < successRate )
		{
			damage *= ( 2 + ( Int * 0.001f ) );
			pDamageInfo->bCritical = TRUE;
		}
		// 080912 LUJ, ���� ����(��ȹ: �۰���)
		else if( random > failureRate )
		{
			damage /= ( 2 - ( wisdom * 0.002f ) );
		}
	}

	switch( IsSpecial )
	{
	case 1: damage = 1; break;		
	}

	// 080115 KTH -- PVP Damage Ratio Setting
	if( pTarget->GetObjectKind() == eObjectKind_Player && pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		damage *= (this->GetPvpDamageRatio() / 100);
	}

	if( damage < 1 )
	{
		damage = 1;
	}

	
	pDamageInfo->RealDamage = (DWORD)damage;

	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
	
	if(newLife == 0)
	{
		// 080616 LUJ, ��� �� �̺�Ʈ�� ó���ϵ��� �Ѵ�
		// 080708 LUJ, ������/�����ڸ� ��� ���ڷ� �ѱ��
		pTarget->Execute( CDieEvent( pAttacker, pTarget ) );
		pAttacker->Execute( CKillEvent( pAttacker, pTarget ) );

		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

void CAttackManager::SpecialAttack( CObject* pAttacker, CObject* pTarget, RESULTINFO* pDamageInfo, float Damage, WORD Type )
{
	pDamageInfo->Clear();

	if( !pAttacker || !pTarget )
		return;

	if( pTarget->GetAbnormalStatus()->IsGod )
	{
		return;
	}

	float damage = 0.f;
	BOOL IsSpecial = FALSE;

	if( pTarget->GetObjectKind() & eObjectKind_Monster )
	{
		CMonster* pMonster = (CMonster*)pTarget;
		BASE_MONSTER_LIST* pInfo = pMonster->GetSMonsterList();

		IsSpecial = pInfo->SpecialType;
	}

	if( Damage < 0 )
	{
		switch( IsSpecial )
		{
		case 1: Damage = -1; break;		
		}

		damage = -Damage;

		pDamageInfo->RealDamage = (DWORD)damage;

		// 080115 KTH -- PVP Damage Ratio Setting
		if( pTarget->GetObjectKind() == eObjectKind_Player && pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			damage *= (this->GetPvpDamageRatio() / 100);

			CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
			if( pAttackPlayer->IsPKMode() )			//PK������ ����� ��������
				pAttackPlayer->SetPKStartTimeReset();

			CPlayer* pTargetPlayer = (CPlayer*)pTarget;
			if( pTargetPlayer->IsPKMode() )			//PK������ ������� �¾�����
				pTargetPlayer->SetPKStartTimeReset();
		}

		DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

		if(newLife == 0)
		{
			// 080616 LUJ, ��� �� �̺�Ʈ�� ó���ϵ��� �Ѵ�
			// 080708 LUJ, ������/�����ڸ� ��� ���ڷ� �ѱ��
			pTarget->Execute( CDieEvent( pAttacker, pTarget ) );
			pAttacker->Execute( CKillEvent( pAttacker, pTarget ) );

			ATTACKMGR->sendDieMsg(pAttacker,pTarget);
			pTarget->Die(pAttacker);
		}
	}
	else
	{
		switch( IsSpecial )
		{
		case 1: Damage = 0; break;		
		}

		RecoverLife( pAttacker, pTarget, 0, Damage, pDamageInfo );
	}
}
