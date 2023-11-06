// AttackCalc.cpp: implementation of the CAttackCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackCalc.h"
#include "PartyManager.h"
#include "Party.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttackCalc::CAttackCalc()
{
	srand((unsigned)time(NULL));
}

CAttackCalc::~CAttackCalc()
{
	
}
// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
//DWORD CAttackCalc::GetPlayerPoint(LEVELTYPE level1, int leve_gab)
EXPTYPE CAttackCalc::GetPlayerPoint(LEVELTYPE level1, int leve_gab)
{
	//////////////////////////////////////////////////////////////////////////
	// ?Ó½Ã ¶«»§ 
	if(level1 == 99)
		return 0;
	//////////////////////////////////////////////////////////////////////////
	
	ASSERT(level1 >= 1);
	if(leve_gab < -MONSTERLEVELRESTRICT_LOWSTARTNUM)
		leve_gab = -MONSTERLEVELRESTRICT_LOWSTARTNUM;
	else if(leve_gab >= MAX_MONSTERLEVELPOINTRESTRICT_NUM)
		leve_gab  = MAX_MONSTERLEVELPOINTRESTRICT_NUM;

	return GAMERESRCMNGR->GetPLAYERxMONSTER_POINT(level1,leve_gab);
}

DWORD CAttackCalc::GetPlayerExpPoint( int level_gap, DWORD MonsterExp )
{
	//level_gap .. ĳ���� ���� - ���� ����

	float Exp = 0.f;
	if( level_gap < -8 )		// -9��
	{
		Exp = (float)(MonsterExp * 1.5);
	}
	else if( -9 < level_gap && level_gap < 1 )	// 0 ~ -8
	{
		Exp = MonsterExp + MonsterExp * -level_gap * 0.05f;
	}
	else if( 0 < level_gap && level_gap < 5 )	// 1, 2, 3, 4
	{
		Exp = MonsterExp * ( 5 - level_gap ) * 0.2f;	//������...?
	}
	else if( level_gap == 5 )	// 5
	{
		Exp = MonsterExp * 0.1f;
	}
	else if( level_gap > 5 )		// 5��
		return 0;
	else
		return 0;

	if( (DWORD)(Exp * 10) > (DWORD)Exp * 10 )	//�Ҽ��� ù°�ڸ����� �� //�ø�
	{
		return (DWORD)Exp + 1;
	}
	else
		return (DWORD)Exp;
}

BOOL	CAttackCalc::getCritical(CObject* pAttacker,CObject* pTarget,float fCriticalRate)
{
	DWORD attackercritical = pAttacker->GetCritical();

	LEVELTYPE targetlevel = pTarget->GetLevel();

	float fCriticalrate = (attackercritical + 20.f) / (targetlevel * 20.f + 300.f);
	if(fCriticalrate > 0.2f)
		fCriticalrate = 0.2f;
	
	WORD wCriticalPercent = (WORD)(fCriticalrate * 100);

	if(fCriticalRate)
		wCriticalPercent += GetPercent(fCriticalRate,pAttacker->GetLevel(),pTarget->GetLevel());

	return (rand()%100 < wCriticalPercent);
}

double CAttackCalc::getPlayerPhysicalAttackPower(CPlayer * pPlayer,float PhyAttackRate, BOOL bCritical )
{
	double physicalAttackPower = 0;
	DWORD minVal,maxVal;
	minVal = pPlayer->GetPhyAttackPowerMin();
	maxVal = pPlayer->GetPhyAttackPowerMax();	

	if(maxVal<=minVal)
	{
		physicalAttackPower = minVal;
	}
	else
	{		
		DWORD gap = maxVal - minVal + 1;
		physicalAttackPower = minVal+rand()%gap;
	}
	

	physicalAttackPower = physicalAttackPower*PhyAttackRate;

	if(bCritical)
	{
		physicalAttackPower = physicalAttackPower*1.5f;			// critical attack
	}
	
	return physicalAttackPower;
}

double CAttackCalc::getPlayerAttributeAttackPower(CPlayer * pPlayer, 
							WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate)
{
	DWORD MinV=0,MaxV=0;

//	ASSERT(AttAttackMax >= AttAttackMin);
	if( AttAttackMax < AttAttackMin )
	{
		char buf[128] = { 0, };
		sprintf( buf, "Min:%d, Max:%d, Attrib:%d, AttAttackRate:%f",
			AttAttackMin, AttAttackMax, Attrib, AttAttackRate );
		ASSERTMSG( 0, buf );
	}

	if(AttAttackRate > 0)
	{
		// ¼Ó¼º Percent °ø°Ý·Â
		//[¼Ó¼ºRate°ø°Ý·Â] * ( ([½É¸Æ]+200) /100 ) + [½É¸Æ]/5
		DWORD wis = pPlayer->GetWisdom();
		double midtermVal = (double)(wis + 200)/(double)100;
		DWORD MinLVV = (pPlayer->GetLevel()+5) - 5;		// RateMin = (Level + 5) - 5
		DWORD MaxLVV = (pPlayer->GetLevel()+5) + 5;		// RateMax = (Level + 5) + 5
		MinV = DWORD((MinLVV * AttAttackRate * midtermVal) + wis/5 + min(wis-12,25));
		MaxV = DWORD((MaxLVV * AttAttackRate * midtermVal) + wis/5 + min(wis-12,25));
	}


	// ¼Ó¼º ?ý´ë °ø°Ý·Â
	MinV += AttAttackMin;
	MaxV += AttAttackMax;

	// ¼Ó¼º °ø°Ý·Â »ó½Â ´õÇØÁÜ(¾Æ?ÌÅÛ,Æ¯±â)
	float AttUp = 1 + pPlayer->GetAttribPlusPercent(Attrib);
	MinV = (DWORD)(AttUp * MinV);
	MaxV = (DWORD)(AttUp * MaxV);
	
	DWORD AttackPower = random(MinV,MaxV);
	return AttackPower;
}

// monster

double	CAttackCalc::getMonsterPhysicalAttackPower(CMonster * pMonster, float PhyAttackRate, BOOL bCritical)
{
	BYTE bAttackType = MONSTER_ATTACKTYPE1;
	
	double physicalAttackPower= 0;
	DWORD minVal = pMonster->GetPhyAttackPowerMin();
	DWORD maxVal = pMonster->GetPhyAttackPowerMax();	

	if(maxVal<=minVal)
	{
		physicalAttackPower = minVal;
	}
	else
	{		
		DWORD gap = maxVal - minVal + 1;
		physicalAttackPower = minVal+rand()%gap;
	}
	
	return physicalAttackPower;

/*	const monster_stats * mon_stats = pMonster->GetMonsterStats();
	double physicalAttackPower= 0;

	if(bAttackType == MONSTER_ATTACKTYPE1)
	{
		physicalAttackPower = mon_stats->PhysicalAttackMin1;
		int tmp = mon_stats->PhysicalAttackMax1-mon_stats->PhysicalAttackMin1;
		if(tmp) physicalAttackPower += rand()%tmp;
	}
	else if(bAttackType == MONSTER_ATTACKTYPE2)
	{
		physicalAttackPower = mon_stats->PhysicalAttackMin2;
		int tmp = mon_stats->PhysicalAttackMax2-mon_stats->PhysicalAttackMin2;
		if(tmp) physicalAttackPower += rand()%tmp;
	}

	return physicalAttackPower;*/
}

double	CAttackCalc::getMonsterAttributeAttackPower(CMonster * pMonster, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax)
{
	monster_stats * mon_stats = pMonster->GetMonsterStats();
	
	ASSERT(AttAttackMax >= AttAttackMin);

	DWORD gap = AttAttackMax - AttAttackMin + 1;
	return AttAttackMin+rand()%gap;
}

double	CAttackCalc::getPhysicalAttackPower(CObject * pObject,float PhyAttackRate, BOOL bCritical )
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
		return getPlayerPhysicalAttackPower((CPlayer*)pObject,PhyAttackRate,bCritical );
	else if(pObject->GetObjectKind() & eObjectKind_Monster)
		return getMonsterPhysicalAttackPower((CMonster*)pObject,PhyAttackRate,bCritical);
	else
	{
		ASSERT(0);
		return 0;
	}
}
double	CAttackCalc::getAttributeAttackPower(CObject * pObject, WORD Attrib, DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
		return getPlayerAttributeAttackPower((CPlayer*)pObject,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
	else if(pObject->GetObjectKind() & eObjectKind_Monster)
		return getMonsterAttributeAttackPower((CMonster*)pObject,Attrib,AttAttackMin,AttAttackMax);
	else
	{
		ASSERT(0);
		return 0;
	}
}

double	CAttackCalc::getPhyDefenceLevel(CObject* pObject, CObject* pAttacker)
{
	double phyDefence = pObject->GetPhyDefense();

	// RaMa - 05.02.04  -> ������ ����
	if( pObject->GetObjectKind() == eObjectKind_Player )
	{
//		phyDefence += (phyDefence*((CPlayer*)pObject)->GetShopItemStats()->RegistPhys)/100;
	}
	
	LEVELTYPE AttackerLevel = pAttacker->GetLevel();

	double phyDefenceLevel = (phyDefence*2.0 + 50) / ( AttackerLevel*20 + 150 );

	if(phyDefenceLevel < 0.0 )
	{
		ASSERT(0);
		phyDefenceLevel = 0;
	}

	if(phyDefenceLevel > 0.9)
		phyDefenceLevel = 0.9;

	return phyDefenceLevel;
}
