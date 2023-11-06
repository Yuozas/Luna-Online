#include "stdafx.h"
#include "CharacterCalcManager.h"
#include "Player.h"
#include "KyungGongManager.h"
#include "CharMove.h"
#include "BossMonster.h"
#include "WeatherManager.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "ItemManager.h"
#include "MHFile.h"
#include "FishingManager.h"
#include "pet.h"


// 070410 LYW --- CCharacterCalcManager : Add struct for hp mp determination.
struct HPMP_DETERMINATE
{
	WORD	wClass_Idx ;

	BYTE	byHP_Value ;
	BYTE	byMP_Value ;
} ;


CCharacterCalcManager::CCharacterCalcManager()
{
	{
		const BOOL result = LoadHPMPDeterminateList();
		ASSERT( result );
	}
}


CCharacterCalcManager::~CCharacterCalcManager()
{
	PTRLISTPOS pos = m_HPMPDeterminateList.GetHeadPosition();

	for(	HPMP_DETERMINATE* pDeterminate;
			pDeterminate = (HPMP_DETERMINATE*)m_HPMPDeterminateList.GetNext(pos);
			)
	{
		SAFE_DELETE( pDeterminate );
	}
}


CCharacterCalcManager* CCharacterCalcManager::GetInstance()
{
	static CCharacterCalcManager instance;

	return &instance;
}


void CCharacterCalcManager::StartUpdateLife( CObject* pObject, DWORD plusLife, BYTE recoverInterval, DWORD recoverTime )
{
	DWORD MaxVal = pObject->GetMaxLife();
	DWORD CurVal = pObject->GetLife();
	DWORD DamagedVal = MaxVal-CurVal;
	if(DamagedVal < plusLife)
		plusLife = DamagedVal;

	pObject->m_YYLifeRecoverTime.bStart = TRUE;
	pObject->m_YYLifeRecoverTime.count = recoverInterval;												// recoverInterval���� ���� ȸ���Ѵ�
	pObject->m_YYLifeRecoverTime.recoverUnitAmout = plusLife/pObject->m_YYLifeRecoverTime.count;		// �ѹ��� ȸ���� ��
	pObject->m_YYLifeRecoverTime.recoverDelayTime = recoverTime/pObject->m_YYLifeRecoverTime.count;		// recoverTime�ʰ� ȸ���� �Ϸ�ȴ�
	pObject->m_YYLifeRecoverTime.lastCheckTime = 0;														// ó���� �ٷ� ȸ���Ѵ�.

	UpdateLife( pObject );
}

void CCharacterCalcManager::StartUpdateMana( CPlayer* pPlayer, DWORD plusMana, BYTE recoverInterval, DWORD recoverTime  )
{
	DWORD MaxVal = pPlayer->GetMaxMana();
	DWORD CurVal = pPlayer->GetMana();
	DWORD DamagedVal = MaxVal-CurVal;
	if(DamagedVal < plusMana)
		plusMana = DamagedVal;

	pPlayer->m_YYManaRecoverTime.bStart = TRUE;
	pPlayer->m_YYManaRecoverTime.count = recoverInterval;												// recoverInterval���� ���� ȸ���Ѵ�
	pPlayer->m_YYManaRecoverTime.recoverUnitAmout = plusMana/pPlayer->m_YYManaRecoverTime.count;		// �ѹ��� ȸ���� ��
	pPlayer->m_YYManaRecoverTime.recoverDelayTime = recoverTime/pPlayer->m_YYManaRecoverTime.count;		// recoverTime�ʰ� ȸ���� �Ϸ�ȴ�
	pPlayer->m_YYManaRecoverTime.lastCheckTime = 0;														// ó���� �ٷ� ȸ���Ѵ�.

	UpdateMana( pPlayer );
}
void CCharacterCalcManager::UpdateMana(CPlayer* pPlayer)
{

	if( pPlayer->GetState() == eObjectState_Die )
		return;

	if( pPlayer->m_YYManaRecoverTime.bStart && pPlayer->m_YYManaRecoverTime.count )
	{
		if(pPlayer->m_YYManaRecoverTime.recoverDelayTime + pPlayer->m_YYManaRecoverTime.lastCheckTime < gCurTime )
		{
			pPlayer->m_YYManaRecoverTime.lastCheckTime = gCurTime;
			// ���� ����
			pPlayer->AddMana( pPlayer->m_YYManaRecoverTime.recoverUnitAmout, NULL );
			--pPlayer->m_YYManaRecoverTime.count;
			if( pPlayer->m_YYManaRecoverTime.count ==0 )
			{
				pPlayer->m_YYManaRecoverTime.bStart = FALSE;
				//CObjectEvent::ObjectEvent(pPlayer, OE_LEVELUP, &m_HeroCharacterInfo.Level);
			}
		}
	}
}

void CCharacterCalcManager::UpdateLife(CObject* pObject)
{
	if( pObject->GetState() == eObjectState_Die )
		return;

	if( pObject->m_YYLifeRecoverTime.bStart && pObject->m_YYLifeRecoverTime.count )
	{
		if(pObject->m_YYLifeRecoverTime.recoverDelayTime + pObject->m_YYLifeRecoverTime.lastCheckTime < gCurTime )
		{
			pObject->m_YYLifeRecoverTime.lastCheckTime = gCurTime;
			// ����� ����
			pObject->AddLife( pObject->m_YYLifeRecoverTime.recoverUnitAmout, NULL );
			--pObject->m_YYLifeRecoverTime.count;
			if( pObject->m_YYLifeRecoverTime.count ==0 )
			{
				pObject->m_YYLifeRecoverTime.bStart = FALSE;
				//CObjectEvent::ObjectEvent(pObject, OE_LIFERECOVERCOMPLETED, NULL);
			}
		}
	}
}

// 070410 LYW --- CStatsCalcManager : Add function to load HP & MP determination file.
BOOL CCharacterCalcManager::LoadHPMPDeterminateList()
{
	CMHFile fp ;

	if( ! fp.Init( "./System/Resource/HPMPdetermination.bin", "rb" ) )
	{
		MessageBox(NULL,"Loading failed!! ./System/Resource/HPMPdetermination.bin",0,0);
		return FALSE;
	}

	while( ! fp.IsEOF() )
	{
		HPMP_DETERMINATE* pDeterminate = new HPMP_DETERMINATE ;

		pDeterminate->wClass_Idx = fp.GetWord() ;
		pDeterminate->byHP_Value = fp.GetByte() ;
		pDeterminate->byMP_Value = fp.GetByte() ;

		m_HPMPDeterminateList.AddTail( pDeterminate ) ;
	}

	return TRUE ;
}

// 070410 LYW --- CStatsCalcManager : Add function to setting hp & mp determinate.
void CCharacterCalcManager::Init_HPMP_Point(CPlayer * pPlayer)
{
	// Setting class index.
	BYTE byFirst	= pPlayer->GetJop(0) ;
	BYTE bySecond	= pPlayer->GetRacial() + 1 ;	// Have to plus 1 cause of start index of racial is 1. ( from determination.bin )
	BYTE byThird	= pPlayer->GetJopGrade() ;	// Have to minus 1 cause of start index of jop array is 0.( from array of code. 
	BYTE byFourth	= 0;

	if( byThird == 1 )
	{
		byFourth = 1;
	}
	else
	{
		byFourth = pPlayer->GetJop(byThird - 1);
	}

	WORD wClassIdx	= ( byFirst * 1000 ) + ( bySecond * 100 ) + ( byThird * 10 ) + byFourth ;


	// Setting hp & mp determination value.
	PTRLISTPOS pos = m_HPMPDeterminateList.GetHeadPosition() ;

	while( pos )
	{
		HPMP_DETERMINATE* pDeterminate = (HPMP_DETERMINATE*)m_HPMPDeterminateList.GetNext(pos) ;

		if( pDeterminate )
		{
			if( pDeterminate->wClass_Idx == wClassIdx )
			{
				pPlayer->SetHP_Point(pDeterminate->byHP_Value) ;
				pPlayer->SetMP_Point(pDeterminate->byMP_Value) ;

				break ;
			}
		}
	}
}

// 070429 LYW --- CCharacterCalcManager : Add function to return hp&mp determinate.
BOOL CCharacterCalcManager::Get_HPMP_Point(CPlayer* pPlayer, BYTE* hpPoint, BYTE* mpPoint)
{
	// Setting class index.
	BYTE byFirst	= pPlayer->GetJop(0) ;
	BYTE bySecond	= pPlayer->GetRacial() + 1 ;	// Have to plus 1 cause of start index of racial is 1. ( from determination.bin )
	BYTE byThird	= pPlayer->GetJopGrade() ;	// Have to minus 1 cause of start index of jop array is 0.( from array of code. )
	BYTE byFourth	= 0;

	if( byThird == 1 )
	{
		byFourth = 1;
	}
	else
	{
		byFourth = pPlayer->GetJop(byThird - 1);
	}

	WORD wClassIdx	= ( byFirst * 1000 ) + ( bySecond * 100 ) + ( byThird * 10 ) + byFourth ;


	// Setting hp & mp determination value.
	PTRLISTPOS pos = m_HPMPDeterminateList.GetHeadPosition() ;

	while( pos )
	{
		HPMP_DETERMINATE* pDeterminate = (HPMP_DETERMINATE*)m_HPMPDeterminateList.GetNext(pos) ;

		if( pDeterminate )
		{
			if( pDeterminate->wClass_Idx == wClassIdx )
			{
				*hpPoint = pDeterminate->byHP_Value ;
				*mpPoint = pDeterminate->byMP_Value ;

				return TRUE ;
			}
		}
	}

	return FALSE ;
}

void CCharacterCalcManager::CalcMaxLife(CPlayer * pPlayer)
{
	// 070410 LYW --- CharacterCalc : Modified function to clalculate max life.
	/*
	DWORD MaxLife = 0;
	WORD Sta = pPlayer->GetVitality();
	LEVELTYPE level = pPlayer->GetLevel();

	//[����] * 5 + [ü��] * 10
	MaxLife = ( level * 5 ) + ( Sta * 10 );	
	
	// + ������ ����
	MaxLife += pPlayer->GetItemStats()->MaxLife;
	
	// + Ư�� ����
	//�����Ƽ ���� - MaxLife += pPlayer->GetAbilityStats()->LifeUp;

	// RaMa - 04.11.17   -> ShopItemOptionÃß°¡
	MaxLife += pPlayer->GetShopItemStats()->Life;

	// RaMa - 05.02.16   AvatarOption
	MaxLife += pPlayer->GetAvatarOption()->Life;

	MaxLife += pPlayer->GetSkillStatsOption()->Life;

	pPlayer->SetMaxLife(MaxLife);
	
	pPlayer->CheckLifeValid();
	*/
	
	float dwBaseVital = pPlayer->GetRealVitality() * 0.2f;
	float dwCurVital  = pPlayer->GetVitality() * 0.2f;

	LEVELTYPE level = pPlayer->GetLevel() ;

	double dBaseHP			= ( dwCurVital * (10 + (0.05 * (level * pPlayer->GetHP_Point()))) ) ;
	double dIncreaseHP		= ( level - 1 ) *  ( ( dwBaseVital + pPlayer->GetHP_Point() ) * 0.5 );

	double dwTotalHP = (dBaseHP + dIncreaseHP) ;
	
	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�+
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mLife;

		// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����	
		dwTotalHP = ( dwTotalHP + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mLife;

		// 080725 LUJ, �ּ� 0���� üũ�ϴ� �κ��� ����
		dwTotalHP = ( dwTotalHP + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	float addRateVal = ( 100 + pPlayer->GetRatePassiveStatus()->MaxLife + pPlayer->GetRateBuffStatus()->MaxLife ) / 100.f;
	float addVal = pPlayer->GetPassiveStatus()->MaxLife + pPlayer->GetBuffStatus()->MaxLife;

	// 080725 LUJ, �ִ밪�� �ּ��� 1�� �ǵ��� �Ѵ�
	dwTotalHP = max( 1.0f, dwTotalHP * addRateVal + addVal );
	
	pPlayer->mMaxLife = (float)dwTotalHP;

	pPlayer->SetMaxLife( ( DWORD )dwTotalHP );
}


void CCharacterCalcManager::CalcMaxMana(CPlayer * pPlayer)
{
	// 070410 LYW --- CharacterCalc : Modified function to clalculate max mana.
	/*
	DWORD MaxEnergy = 0;
	WORD wis = pPlayer->GetWisdom();
	LEVELTYPE level = pPlayer->GetLevel();

	// [����] * 5 + [�ɸ�] * 10
	MaxEnergy = ( level * 5 ) + ( wis * 10 );

	MaxEnergy += pPlayer->GetItemStats()->MaxMana;
	
	// + Ư�� ����
	//�����Ƽ ���� - MaxEnergy += pPlayer->GetAbilityStats()->manaUp;

	// RaMa - 04.11.17   -> ShopItemOptionÃß°¡
	MaxEnergy += pPlayer->GetShopItemStats()->Mana;

	// RaMa - 05.02.16   AvatarOption
	MaxEnergy += pPlayer->GetAvatarOption()->Mana;

	MaxEnergy += pPlayer->GetSkillStatsOption()->Mana;

	pPlayer->SetMaxMana(MaxEnergy);

	pPlayer->CheckManaValid();
	*/

	float dwBaseWisdom = (DWORD)(pPlayer->GetRealWisdom() * 0.2f);
	float dwCurWisdom  = (DWORD)(pPlayer->GetWisdom() * 0.2f);

	LEVELTYPE level = pPlayer->GetLevel() ;

	double dBaseMP		= ( dwCurWisdom * (6 + (0.05 * (level * pPlayer->GetMP_Point()))) ) ;
	double dIncreaseMP	= ( level - 1 ) * ( ( dwBaseWisdom + pPlayer->GetMP_Point() ) * 0.5 );	

	double dwTotalMP = (DWORD)(dBaseMP + dIncreaseMP) ;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mMana;

		// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
		dwTotalMP = ( dwTotalMP + value.mPlus ) * ( 1.0f + value.mPercent );
	}

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mMana;

		// 080725 LUJ, �ּ� 0���� üũ�ϴ� �κ��� ����
		dwTotalMP = ( dwTotalMP + value.mPlus ) * ( 1.0f + value.mPercent );
	}
	
	float addRateVal = ( 100 + pPlayer->GetRatePassiveStatus()->MaxMana + pPlayer->GetRateBuffStatus()->MaxMana ) / 100.f;
	float addVal = pPlayer->GetPassiveStatus()->MaxMana + pPlayer->GetBuffStatus()->MaxMana;

	// 080725 LUJ, �ִ밪�� �ּ��� 1�� �ǵ��� �Ѵ�
	dwTotalMP = max( 1.0f, dwTotalMP * addRateVal + addVal );
	
	pPlayer->mMaxMana = (float)dwTotalMP;

	pPlayer->SetMaxMana( ( DWORD )dwTotalMP );
}


// 070412 LYW --- ���ݷ� ��ġ ó�� ( ���� )
// 070410 LYW --- StatsCalcManager : Add functions for calculate stats of player.
void CCharacterCalcManager::CalcCharPhyAttackMax( CPlayer* pPlayer )
{
	const Status*	passiveStatus	= pPlayer->GetPassiveStatus();
	DWORD			wWeaponAttack	= 0 ;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mPhysicAttack;

		wWeaponAttack += DWORD( value.mPlus * ( 1.0f + value.mPercent ) );
	}

	DWORD 			wEnchantAttack	= 1 ;
	DWORD 			wLevel			= pPlayer->GetLevel() ;
	float 			wPassiveSkill	= passiveStatus->PhysicAttack;
	float 			wAttackA		= wWeaponAttack + wEnchantAttack + wLevel + wPassiveSkill + 3 ;
	float 			wStrengthAll	= float( pPlayer->GetStrength() );

	wStrengthAll *= 0.2f;

	float wMasteryLevel	= 0 ;

	switch( pPlayer->GetWeaponAniType() )
	{ 
	case eWeaponAnimationType_OneHand:	wMasteryLevel = passiveStatus->OneHanded;	break;
	case eWeaponAnimationType_TwoHand:	wMasteryLevel = passiveStatus->TwoHanded;	break;
	case eWeaponAnimationType_TwoBlade:	wMasteryLevel = passiveStatus->TwoBlade;	break;
	}

	switch( pPlayer->GetWeaponEquipType() )
	{
	case eWeaponType_Sword:		wMasteryLevel += passiveStatus->Sword;	break;
	case eWeaponType_Mace:		wMasteryLevel += passiveStatus->Mace;	break;
	case eWeaponType_Axe:		wMasteryLevel += passiveStatus->Axe;	break;
	case eWeaponType_Dagger:	wMasteryLevel = passiveStatus->Dagger;	break;
	case eWeaponType_Staff:		wMasteryLevel = passiveStatus->Staff;	break;
	case eWeaponType_Bow:		wMasteryLevel = passiveStatus->Bow;		break;
	case eWeaponType_Gun:		wMasteryLevel = passiveStatus->Gun;		break;
	}

	float wAttackBuffVal = pPlayer->GetBuffStatus()->PhysicAttack ;

	float wAttackB		= 1000 + (((wStrengthAll -10) * 20) + (wMasteryLevel * 25) + (wAttackBuffVal * 10)) ;

	float dwTotalAttack = ((wAttackA * wAttackB) / 1000)  ;

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mPhysicAttack;

		dwTotalAttack = max( 0, ( dwTotalAttack + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	pPlayer->mPhysicAttackMax = dwTotalAttack;
}

void CCharacterCalcManager::CalcCharPhyAttackMin( CPlayer* pPlayer )
{
	//	player_calc_stats* pcharStats = pPlayer->GetCharStats() ;

	float wWeaponAttack  = 0;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mPhysicAttack;

		wWeaponAttack = value.mPlus * ( 1.0f + value.mPercent );
	}

	switch( pPlayer->GetWeaponEquipType() )
	{
	case eWeaponType_Dagger:
	case eWeaponType_Bow:
		{
			wWeaponAttack = wWeaponAttack * 0.9f;
		}
		break;
	case eWeaponType_Sword:
	case eWeaponType_Gun:
		{
			wWeaponAttack = wWeaponAttack * 0.7f;
		}
		break;
	case eWeaponType_Mace:
	case eWeaponType_Axe:
	case eWeaponType_Staff:
		{
			wWeaponAttack = wWeaponAttack * 0.8f;
		}
		break;
	}

	const Status*	passiveStatus	= pPlayer->GetPassiveStatus();
	DWORD			wEnchantAttack	= 1 ;
	DWORD			wLevel			= pPlayer->GetLevel() ;
	float			wPassiveSkill	= passiveStatus->PhysicAttack;
	float			wAttackA		= wWeaponAttack + wEnchantAttack + wLevel + wPassiveSkill + 3 ;
	float			wStrengthAll	= float( pPlayer->GetStrength() );

	wStrengthAll *= 0.2f;

	float wMasteryLevel	= 0 ;

	switch( pPlayer->GetWeaponAniType() )
	{ 
	case eWeaponAnimationType_OneHand:	wMasteryLevel = passiveStatus->OneHanded;	break;
	case eWeaponAnimationType_TwoHand:	wMasteryLevel = passiveStatus->TwoHanded;	break;
	case eWeaponAnimationType_TwoBlade:	wMasteryLevel = passiveStatus->TwoBlade;	break;
	}

	switch( pPlayer->GetWeaponEquipType() )
	{
	case eWeaponType_Sword:		wMasteryLevel += passiveStatus->Sword;	break;
	case eWeaponType_Mace:		wMasteryLevel += passiveStatus->Mace;	break;
	case eWeaponType_Axe:		wMasteryLevel += passiveStatus->Axe;	break;
	case eWeaponType_Dagger:	wMasteryLevel = passiveStatus->Dagger;	break;
	case eWeaponType_Staff:		wMasteryLevel = passiveStatus->Staff;	break;
	case eWeaponType_Bow:		wMasteryLevel = passiveStatus->Bow;		break;
	case eWeaponType_Gun:		wMasteryLevel = passiveStatus->Gun;		break;
	}

	float wAttackBuffVal = pPlayer->GetBuffStatus()->PhysicAttack ;

	float wAttackB		= 1000 + (((wStrengthAll -10) * 20) + (wMasteryLevel * 25) + (wAttackBuffVal * 10)) ;

	float dwTotalAttack = ((wAttackA * wAttackB) / 1000)  ;

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value value = pPlayer->GetSetItemStats().mPhysicAttack;

		dwTotalAttack = max( 0, ( dwTotalAttack + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	pPlayer->mPhysicAttackMin = dwTotalAttack;
}

void CCharacterCalcManager::CalcCharPhyDefense( CPlayer* pPlayer ) 
{
	float wDefenseAll		= 0;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mPhysicDefense;

		wDefenseAll = value.mPlus * ( 1.0f + value.mPercent );
	}

	float wEnchantAll		= 1 ;
	float wLevel				= pPlayer->GetLevel() ;
	float wPassiveSkill		= pPlayer->GetPassiveStatus()->PhysicDefense;

	DWORD Armor = pPlayer->GetWearedItemIdx( eWearedItem_Dress );
	DWORD Boots = pPlayer->GetWearedItemIdx( eWearedItem_Shoes );
	DWORD Glove = pPlayer->GetWearedItemIdx( eWearedItem_Glove );

	ITEM_INFO* pArmor = ITEMMGR->GetItemInfo( Armor );
	ITEM_INFO* pBoots = ITEMMGR->GetItemInfo( Boots );
	ITEM_INFO* pGlove = ITEMMGR->GetItemInfo( Glove );

	/// �α� �迭
	if( pPlayer->m_HeroCharacterInfo.Job[0] == 2 )
	{
		if( ( pArmor && pArmor->ArmorType == eArmorType_Metal ) ||
			( pBoots && pBoots->ArmorType == eArmorType_Metal ) ||
			( pGlove && pGlove->ArmorType == eArmorType_Metal ) )
		{
			if( pPlayer->GetPassiveStatus()->HeavyArmor < 1 )
				wDefenseAll = 0.f;
		}
	}
	/// ������ �迭
	else if( pPlayer->m_HeroCharacterInfo.Job[0] == 3 )
	{
		if( ( pArmor && pArmor->ArmorType == eArmorType_Metal ) ||
			( pBoots && pBoots->ArmorType == eArmorType_Metal ) ||
			( pGlove && pGlove->ArmorType == eArmorType_Metal ) )
		{
			if( pPlayer->GetPassiveStatus()->HeavyArmor < 1 )
				wDefenseAll = 0.f;
		}
		else if( ( pArmor && pArmor->ArmorType == eArmorType_Leather ) ||
				 ( pBoots && pBoots->ArmorType == eArmorType_Leather ) ||
				 ( pGlove && pGlove->ArmorType == eArmorType_Leather ) )
		{
			if( pPlayer->GetPassiveStatus()->LightArmor < 1 )
				wDefenseAll = 0.f;
		}
	}	


	float wDefenseA			= ( wDefenseAll / 2 ) + wEnchantAll + wLevel + wPassiveSkill ;
	float wVitalityAll		= float( pPlayer->GetVitality() );

	float wMasteryLevel		= 0 ;
	wVitalityAll *= 0.2f;

	if( pArmor && pBoots && pGlove )
	if( ( pArmor->ArmorType == pBoots->ArmorType ) && ( pArmor->ArmorType == pGlove->ArmorType ) )
	{
		switch( pArmor->ArmorType )
		{
		case eArmorType_Robe:		wMasteryLevel = pPlayer->GetPassiveStatus()->Robe;			break;
		case eArmorType_Leather:	wMasteryLevel = pPlayer->GetPassiveStatus()->LightArmor;	break;
		case eArmorType_Metal:		wMasteryLevel = pPlayer->GetPassiveStatus()->HeavyArmor;	break;
		}
	}

	float wDefenseBuffVal	= pPlayer->GetBuffStatus()->PhysicDefense;

	float wDefenseB			= 500 + (((wVitalityAll - 20) * 5) + (wMasteryLevel * 30) + (wDefenseBuffVal * 10)) ;

	float dwTotalDefense	= ((wDefenseA * wDefenseB) / 1000)  ;

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mPhysicDefense;

		dwTotalDefense = max( 0, ( dwTotalDefense + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	pPlayer->mPhysicDefense = dwTotalDefense;
}

void CCharacterCalcManager::CalcCharMagAttack( CPlayer* pPlayer ) 
{
	float wMagicAttack		= 0;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mMagicAttack;

		wMagicAttack = value.mPlus * ( 1.0f + value.mPercent );
	}

	float wEnchantAttack		= 1 ;
	float wLevel				= pPlayer->GetLevel() ;
	float wPassiveSkill		= pPlayer->GetPassiveStatus()->MagicAttack;

	float wAttackA			= wMagicAttack + wEnchantAttack + wLevel + wPassiveSkill ;
	float wIntelligenceAll	= float( pPlayer->GetIntelligence() );

	float wAttackBuffVal		= pPlayer->GetBuffStatus()->MagicAttack;
	wIntelligenceAll *= 0.2f;

	float wAttackB			= 1000 + (((wIntelligenceAll -10) * 45) + (wAttackBuffVal * 10)) ;

	float dwTotalAttack		= ((wAttackA * wAttackB) / 350)  ;

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mMagicAttack;

		dwTotalAttack = max( 0, ( dwTotalAttack + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	pPlayer->mMagicAttack = dwTotalAttack;
}

void CCharacterCalcManager::CalcCharMagDefense( CPlayer* pPlayer ) 
{
	float wMagDefenseAll	= 0 ;

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mMagicDefense;

		wMagDefenseAll = value.mPlus * ( 1.0f + value.mPercent );
	}

	float wEnchantAll		= 1 ;
	float wLevel			= pPlayer->GetLevel() ;
	float wPassiveSkill		= pPlayer->GetPassiveStatus()->MagicDefense;

	float wDefenseA			= ( wMagDefenseAll / 5 ) + wEnchantAll + wLevel + wPassiveSkill ;
	float wWisdomAll		= float( pPlayer->GetWisdom() );

	float wDefenseBuffVal	= pPlayer->GetBuffStatus()->MagicDefense;
	wWisdomAll *= 0.2f;

	float wDefenseB			= 500 + (((wWisdomAll - 20) * 5) + (wDefenseBuffVal * 10)) ;

	float dwTotalDefense	= ((wDefenseA * wDefenseB) / 200)  ;

	// 070705 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mMagicDefense;

		dwTotalDefense = max( 0, ( dwTotalDefense + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	pPlayer->mMagicDefense = dwTotalDefense;
}

void CCharacterCalcManager::CalcCharAccuracy( CPlayer* pPlayer ) 
{
	CHARACTER_TOTALINFO totalInfo ;
	pPlayer->GetCharacterTotalInfo(&totalInfo) ;

	BYTE byJobOrder		= totalInfo.Job[0] ;

	DWORD wLevel		= pPlayer->GetLevel() ;
	float wDexterityAll	= float( pPlayer->GetDexterity() );
	float wStrengthAll	= float( pPlayer->GetStrength() );

	DWORD wClassOrderVal = 0 ;
	float wPassiveSkill	= pPlayer->GetPassiveStatus()->Accuracy;
	float wBuffSkillVal	= pPlayer->GetBuffStatus()->Accuracy;
	wDexterityAll *= 0.2f;
	wStrengthAll *= 0.2f;

	switch( byJobOrder )
	{
	case 1 : wClassOrderVal = 20 ; break ;
	case 2 : wClassOrderVal = 25 ; break ;
	case 3 : wClassOrderVal = 15 ; break ;
	}

	float wAccuracy		= ( wDexterityAll / 1.4f ) + wLevel + wClassOrderVal + ( wStrengthAll / 4 );

	float dwTotalAccuracy = wAccuracy + wPassiveSkill + wBuffSkillVal ;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mAccuracy;

		dwTotalAccuracy = max( 0, ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mAccuracy;

		// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
		pPlayer->mAccuracy = ( dwTotalAccuracy + value.mPlus ) * ( 1.0f + value.mPercent );
	}
}

void CCharacterCalcManager::CalcCharEvasion( CPlayer* pPlayer ) 
{
	CHARACTER_TOTALINFO totalInfo ;
	pPlayer->GetCharacterTotalInfo(&totalInfo) ;

	BYTE byJobOrder		= totalInfo.Job[0] ;

	DWORD wLevel		= pPlayer->GetLevel() ;
	float wDexterityAll	= float( pPlayer->GetDexterity() );

	DWORD wClassOrderVal = 0 ;
	float wPassiveSkill	= pPlayer->GetPassiveStatus()->Avoid ;
	float wBuffSkillVal	= pPlayer->GetBuffStatus()->Avoid ;
	wDexterityAll *= 0.2f;

	switch( byJobOrder )
	{
	case 1 : wClassOrderVal = 20 ; break ;
	case 2 : wClassOrderVal = 25 ; break ;
	case 3 : wClassOrderVal = 15 ; break ;
	}

	float wEvasion			= ( wDexterityAll / 2 ) + wLevel + wClassOrderVal;
	float dwTotalEvasion	= wEvasion + wPassiveSkill + wBuffSkillVal;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mEvade;

		dwTotalEvasion = max( 0, ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mEvade;

		// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
		pPlayer->mAvoid = ( dwTotalEvasion + value.mPlus ) * ( 1.0f + value.mPercent );
	}
}

void CCharacterCalcManager::CalcCharCriticalRate( CPlayer* pPlayer ) 
{
	float wDexterityAll	= float( pPlayer->GetDexterity() );

	float wPassiveSkill	= pPlayer->GetPassiveStatus()->CriticalRate ;
	float wBuffSkillVal	= pPlayer->GetBuffStatus()->CriticalRate ;
	wDexterityAll *= 0.2f;

	DWORD wLevel		= pPlayer->GetLevel();
	float dwCritical	= 125 + wLevel + ((wDexterityAll - 15) * 7) + wPassiveSkill + wBuffSkillVal ;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mCriticalRate;

		dwCritical = max( 0, ( dwCritical + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mCriticalRate;

		pPlayer->mCriticalRate = dwCritical + value.mPlus * ( 1.0f + value.mPercent );
	}
}


void CCharacterCalcManager::CalcCharCriticalDamage( CPlayer* pPlayer ) 
{
	float wPassiveSkill	= pPlayer->GetPassiveStatus()->CriticalDamage ;
	float wBuffSkillVal	= pPlayer->GetBuffStatus()->CriticalDamage ;

	float dwCriticalDamage	= wPassiveSkill + wBuffSkillVal;

	// 071002 ����, ��Ʈ ���������� �ο��� �ɷ�ġ�� �߰��Ѵ�
	{
		const PlayerStat::Value& value = pPlayer->GetSetItemStats().mCriticalDamage;

		dwCriticalDamage = max( 0, ( dwCriticalDamage + value.mPlus ) * ( 1.0f + value.mPercent ) );
	}

	// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
	{
		const PlayerStat::Value& value = pPlayer->GetItemStats().mCriticalDamage;

		// 080408 LUJ, �ۼ�Ʈ �ɷ��� �ùٷ� ������ �ʴ� ���� ����
		pPlayer->mCriticalDamage = ( dwCriticalDamage + value.mPlus ) * ( 1.0f + value.mPercent );
	}
}

// 080910 LUJ, �÷��̾��� ���� ������ ���
void CCharacterCalcManager::CalcCharShieldDefense(CPlayer* pPlayer)
{
	CItemSlot* slot = pPlayer->GetSlot( eItemTable_Weared );
	
	if( ! slot )
	{
		return;
	}
	
	const ITEMBASE*	itemBase = slot->GetItemInfoAbs( TP_WEAR_START + eWearedItem_Shield );
	
	if( ! itemBase )
	{
		return;
	}
	
	const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( itemBase->wIconIdx );
	
	// 080910 LUJ, ���а� �����Ǿ� ���� ����
	if( !	itemInfo ||
			itemInfo->Part3DType != ePartType_Shield )
	{
		pPlayer->SetShieldDefence( 0 );
		return;
	}
	
	const ITEM_OPTION& itemOption = ITEMMGR->GetOption( *itemBase );
	
	float defenseValue	= float( itemInfo->PhysicDefense ) + itemOption.mReinforce.mPhysicDefence;
	float defenseRate	= 1.0f;	
	
	// 080910 LUJ, ��� �ɼ��� �����Ѵ�
	{
		const ITEM_OPTION::Drop& drop = itemOption.mDrop;
	
		for( DWORD i = 0; i < sizeof( drop.mValue ) / sizeof( *drop.mValue ); ++i )
		{
			const ITEM_OPTION::Drop::Value& dropValue = drop.mValue[ i ];
	
			switch( dropValue.mKey )
			{
			case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
				{
					defenseValue += dropValue.mValue;
					break;
				}
			case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
				{
					defenseRate += dropValue.mValue;
					break;
				}
			}
		}
	}	
	
	// 080910 LUJ, ��æƮ �ɼ��� �����Ѵ�
	{
		const EnchantScript* script = ITEMMGR->GetEnchantScript( itemOption.mEnchant.mIndex );
	
		if( script && itemOption.mEnchant.mLevel )
		{
			for(	EnchantScript::Ability::const_iterator it = script->mAbility.begin();
					script->mAbility.end() != it;
					++it )
			{
				switch( EnchantScript::eType( *it ) )
				{
				case ReinforceScript::eTypePhysicDefence:
					{
						defenseValue += GetBonusEnchantValue( *itemInfo, itemOption, itemInfo->PhysicDefense );
						break;
					}
				}
			}
		}
	}
	
	pPlayer->SetShieldDefence( DWORD( defenseValue * defenseRate ) );
}

void CCharacterCalcManager::ProcessLife(CPlayer * pPlayer)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return;

	DWORD life = pPlayer->GetLife();
	DWORD maxlife = pPlayer->GetMaxLife();
	LEVELTYPE level = pPlayer->GetLevel();

	if( life == maxlife )
	{
		return;
	}
	
	DWORD curTime = 0;
	if(!pPlayer->m_LifeRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPlayer->m_LifeRecoverTime.lastCheckTime = gCurTime;
		pPlayer->m_LifeRecoverTime.bStart = FALSE;
		return;
	}


	if(pPlayer->GetState() == eObjectState_Ungijosik)
		Ungi_LifeCount(pPlayer,life,maxlife,curTime);
	else
		General_LifeCount(pPlayer, curTime, level, life, maxlife);

}


void CCharacterCalcManager::General_LifeCount(CPlayer * pPlayer, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife)
{
	/*
	DWORD TimeLength = 5000;

	if(curTime - pPlayer->m_LifeRecoverTime.lastCheckTime > TimeLength)
	{// 	���� ȸ�� (5�ʴ�) ü�� : ��ü�� 1%
		DWORD UpLife = pPlayer->GetRecoverLife();
	
		// 06. 03 �������� - �̿���
		float fRate = 0.01f;

		if(pPlayer->IsMussangMode())
		{
//			switch(pPlayer->GetStage())
//			{
//			case eStage_Normal:	
				fRate = 0.015f;
//				break;
//			case eStage_Hwa:		
//			case eStage_Geuk:
//				fRate = 0.015f;
//				break;
//			case eStage_Hyun:
//			case eStage_Tal:
//				fRate = 0.02f;
//				break;
//			}
		}
		DWORD CalcLife = life+(DWORD)(maxlife*fRate)+UpLife;
			
		pPlayer->SetLife(CalcLife);
		pPlayer->m_LifeRecoverTime.lastCheckTime = curTime; //KES confirm
	}
	*/

	// 070412 LYW --- ü�� ���� ����
	// 070412 LYW --- CharacterCalcManager : Modified function General_Life.
	DWORD TimeLength = 3000;

	if(curTime - pPlayer->m_LifeRecoverTime.lastCheckTime > TimeLength)
	{
		/*WORD wVitalityAll		= pPlayer->GetLifeRecover();
		WORD wOtherOptionVal	= pPlayer->GetPassiveStatus()->LifeRecoverRate + pPlayer->GetBuffStatus()->LifeRecoverRate ;*/
		float dwVitalityAll		= pPlayer->GetVitality() * 0.2f;
		float dwOtherOptionVal	= pPlayer->GetPassiveStatus()->LifeRecoverRate + pPlayer->GetBuffStatus()->LifeRecoverRate ;	// dwOtherOptionVal�� ������ �Ǵ� ��찡 ������?

		DWORD dwUpLife			= 0;

		// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
		{
			const PlayerStat::Value& value = pPlayer->GetItemStats().mRecoveryLife;

			dwUpLife = DWORD( value.mPlus * ( 1.0f + value.mPercent ) );
		}

		// Ȥ�� �𸣴ϱ� �׷��� üũ!!
		if( dwOtherOptionVal <= 0 ) dwOtherOptionVal = 0 ;
		
		if(pPlayer->GetState() == eObjectState_Rest)
		{
			dwUpLife += (DWORD)( dwVitalityAll + dwOtherOptionVal ) ;
		}
		else
		{
			dwUpLife += (DWORD)( dwVitalityAll/2 + dwOtherOptionVal ) ;
		}

		// 071129 ����, ��Ʈ ������ ��ġ�� ���⼭ ����Ѵ�. �������� �⸳���� ����� �ٸ���, CalcCharLifeRecoverRate()����
		//				�ߺ��Ͽ� ���� ��ġ�� ����ϱ� �����̴�.
		{
			const PlayerStat::Value& value = pPlayer->GetSetItemStats().mRecoveryLife;

			dwUpLife = DWORD( max( 0, ( dwUpLife + value.mPlus ) * ( 1.0f + value.mPercent ) ) );
		}

		if( dwUpLife <= 0 )
		{
			dwUpLife = 1 ;
		}

		DWORD dwCalcLife = life + dwUpLife ;
				
		pPlayer->SetLife(dwCalcLife);
		pPlayer->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::ProcessPetLife(CPet * pPet)
{
	if(pPet->GetState() == eObjectState_Die)
		return;

	DWORD life = pPet->GetLife();
	DWORD maxlife = pPet->GetMaxLife();
	LEVELTYPE level = pPet->GetLevel();

	if( life == maxlife )
	{
		return;
	}
	
	DWORD curTime = 0;
	if(!pPet->m_LifeRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPet->m_LifeRecoverTime.lastCheckTime = gCurTime;
		pPet->m_LifeRecoverTime.bStart = FALSE;
		return;
	}


	General_PetLifeCount(pPet, curTime, level, life, maxlife);
}


void CCharacterCalcManager::General_PetLifeCount(CPet * pPet, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife)
{
	DWORD TimeLength = 3000;

	if(curTime - pPet->m_LifeRecoverTime.lastCheckTime > TimeLength)
	{
		float dwVitalityAll		= pPet->mPetStat.mVitality.mPlus * 0.2f;
		float dwOtherOptionVal	= pPet->GetBuffStatus()->LifeRecoverRate ;	// dwOtherOptionVal�� ������ �Ǵ� ��찡 ������?

		DWORD dwUpLife			= 0;

		// Ȥ�� �𸣴ϱ� �׷��� üũ!!
		if( dwOtherOptionVal <= 0 ) dwOtherOptionVal = 0 ;
		
		dwUpLife += (DWORD)( dwVitalityAll/2 + dwOtherOptionVal ) ;

		if( dwUpLife <= 0 )
		{
			dwUpLife = 1 ;
		}

		DWORD dwCalcLife = life + dwUpLife ;
				
		pPet->SetLife(dwCalcLife);
		pPet->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::General_LifeCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD life, DWORD maxlife)
{
	// 	���� ȸ�� (5�ʴ�) ü�� : ��ü�� 1%
	if(curTime - pBoss->m_LifeRecoverTime.lastCheckTime > 5000)
	{
		pBoss->SetLife(life+(DWORD)(maxlife*0.001f), TRUE);
		pBoss->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::Ungi_LifeCount(CPlayer* pPlayer, DWORD life, DWORD maxlife, DWORD curTime)
{
	// RaMa - 04.11.24  ->������Ľð�
	DWORD dwUngiTime = 5000;
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
	dwUngiTime = (DWORD)(dwUngiTime*(1/gEventRate[eEvent_UngiSpeed]));

	// 2005 ũ�������� �̺�Ʈ �ڵ�
/*	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		dwUngiTime /= 2;
*/	
	//������ ������� 5�ʸ��� ��ü���� 10(����ġ) + 3% ������
	if(curTime - pPlayer->m_LifeRecoverTime.lastCheckTime > dwUngiTime)
	{
		/*�����Ƽ ���� - DWORD baseplus = (DWORD)(10 + maxlife*(0.03f)) + pPlayer->m_AbilityStats.UngiUpVal;*/
		DWORD pluslife = (DWORD)(maxlife*pPlayer->GetUngiPlusRate());
		pPlayer->SetLife(life + /*�����Ƽ ���� - baseplus +*/ pluslife);
		pPlayer->m_LifeRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::ProcessLifeBoss(CBossMonster * pBoss)
{
	if(pBoss->GetState() == eObjectState_Die)
		return;

	DWORD life = pBoss->GetLife();
	DWORD maxlife = pBoss->GetMaxLife();

	if(life == maxlife) return;
	
	DWORD curTime = 0;
	if(!pBoss->m_LifeRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pBoss->m_LifeRecoverTime.lastCheckTime = gCurTime;
		pBoss->m_LifeRecoverTime.bStart = FALSE;
		return;
	}

	General_LifeCountBoss(pBoss, curTime, life, maxlife);
}

void CCharacterCalcManager::ProcessMana(CPlayer * pPlayer)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return;
	
	DWORD energy = pPlayer->GetMana();
	DWORD maxEnergy = pPlayer->GetMaxMana();
	LEVELTYPE level = pPlayer->GetLevel();
	WORD KGIdx = pPlayer->m_MoveInfo.KyungGongIdx;

	if(KGIdx != 0)
	{
		KyungGong_ManaCount(pPlayer,gCurTime,KGIdx);
		return;
	}

	if(energy == maxEnergy) return;

	DWORD curTime = 0;
	if(!pPlayer->m_ManaRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPlayer->m_ManaRecoverTime.lastCheckTime = gCurTime;
		pPlayer->m_ManaRecoverTime.bStart = FALSE;
		return;
	}

	if(pPlayer->GetState() == eObjectState_Ungijosik)
		Ungi_ManaCount(pPlayer,curTime,energy,maxEnergy);
	else
		General_ManaCount(pPlayer, curTime, level, energy, maxEnergy);
	

}

void CCharacterCalcManager::General_ManaCount(CPlayer* pPlayer, DWORD curTime, LEVELTYPE level, DWORD energy, DWORD maxEnergy)
{
	/*
	DWORD TimeLength = 5000;

	//���� ȸ�� (5�ʴ�) ���� : ��ü�� 1%
	if(curTime - pPlayer->m_ManaRecoverTime.lastCheckTime > TimeLength)
	{
		// RaMa - 04.11.17  ->ShopItemOption?Ç ³»·ÂÈ¸º¹·ê 1.5¹è Áõ°¡.
		float fRate = 0.01f;
		if( pPlayer->GetShopItemStats()->RecoverRate )
		{
			fRate += pPlayer->GetShopItemStats()->RecoverRate*0.01f;
		}

		// 06. 03 �������� - �̿���
		if(pPlayer->IsMussangMode())
		{
//			switch(pPlayer->GetStage())
//			{
//			case eStage_Normal:	
				fRate = fRate * 1.5f;
//				break;
//			case eStage_Hwa:		
//			case eStage_Geuk:
//				fRate = fRate * 1.5f;
//				break;
//			case eStage_Hyun:
//			case eStage_Tal:
//				fRate = fRate * 2.0f;
//				break;
//			}
		}

		pPlayer->SetMana(energy+(DWORD)(maxEnergy*fRate));
		pPlayer->m_ManaRecoverTime.lastCheckTime = curTime;
	}
	*/
	// 070412 LYW --- ���� ���� ����
	// 070412 LYW --- CharacterCalcManager : Modified function General_Mana.
	DWORD TimeLength = 5000;

	if(curTime - pPlayer->m_ManaRecoverTime.lastCheckTime > TimeLength)
	{
		/*WORD wWisdomAll		= pPlayer->GetManaRecover();
		WORD wOtherOptionVal	= pPlayer->GetPassiveStatus()->ManaRecoverRate + pPlayer->GetBuffStatus()->ManaRecoverRate ;*/
		float dwWisdomAll		= pPlayer->GetWisdom() * 0.2f;
		float dwOtherOptionVal	= pPlayer->GetPassiveStatus()->ManaRecoverRate + pPlayer->GetBuffStatus()->ManaRecoverRate ;	// dwOtherOptionVal�� ������ �� Ȯ�� ����.
		DWORD dwUpMana			= 0;

		// 080313 LUJ, ������ ������ �Ϲݰ� ��Ʈ�� �и�
		{
			const PlayerStat::Value& value = pPlayer->GetItemStats().mRecoveryMana;

			dwUpMana = DWORD( value.mPlus * ( 1.0f + value.mPercent ) );
		}

		// �׷��� üũ ����!!
		if( dwOtherOptionVal <= 0 ) dwOtherOptionVal = 0 ;
		
		if( pPlayer->GetState() == eObjectState_Rest)
		{
			dwUpMana += (DWORD)( dwWisdomAll + dwOtherOptionVal ) ;
		}
		else
		{
			dwUpMana += (DWORD)( dwWisdomAll/2+ dwOtherOptionVal ) ;
		}

		// 071129 ����, ��Ʈ ������ ��ġ�� ���⼭ ����Ѵ�. �������� �⸳���� ����� �ٸ���, CalcCharManaRecoverRate()����
		//				�ߺ��Ͽ� ���� ��ġ�� ����ϱ� �����̴�.
		{
			const PlayerStat::Value& value = pPlayer->GetSetItemStats().mRecoveryMana;

			dwUpMana = DWORD( max( 0, ( dwUpMana + value.mPlus ) * ( 1.0f + value.mPercent ) ) );
		}

		if( dwUpMana <= 0 )  
		{
			dwUpMana = 1 ;
		}

		DWORD dwCalcMana = energy + dwUpMana ;
				
		pPlayer->SetMana(dwCalcMana);
		pPlayer->m_ManaRecoverTime.lastCheckTime = curTime;
	}
}


void CCharacterCalcManager::ProcessPetMana(CPet * pPet)
{
	if(pPet->GetState() == eObjectState_Die)
		return;
	
	DWORD energy = pPet->GetMana();
	DWORD maxEnergy = pPet->GetMaxMana();
	LEVELTYPE level = pPet->GetLevel();

	if(energy == maxEnergy) return;

	DWORD curTime = 0;
	if(!pPet->m_ManaRecoverTime.bStart)
	{
		curTime = gCurTime;
	}
	else
	{
		curTime = pPet->m_ManaRecoverTime.lastCheckTime = gCurTime;
		pPet->m_ManaRecoverTime.bStart = FALSE;
		return;
	}

	General_PetManaCount(pPet, curTime, level, energy, maxEnergy);
	
}

void CCharacterCalcManager::General_PetManaCount(CPet* pPet, DWORD curTime, LEVELTYPE level, DWORD energy, DWORD maxEnergy)
{
	// 070412 LYW --- ���� ���� ����
	// 070412 LYW --- CharacterCalcManager : Modified function General_Mana.
	DWORD TimeLength = 5000;

	if(curTime - pPet->m_ManaRecoverTime.lastCheckTime > TimeLength)
	{
		float dwWisdomAll		= pPet->mPetStat.mWisdom.mPlus * 0.2f;
		float dwOtherOptionVal	= pPet->GetBuffStatus()->ManaRecoverRate ;	// dwOtherOptionVal�� ������ �� Ȯ�� ����.
		DWORD dwUpMana			= 0;

		// �׷��� üũ ����!!
		if( dwOtherOptionVal <= 0 ) dwOtherOptionVal = 0 ;
		
		dwUpMana += (DWORD)( dwWisdomAll/2+ dwOtherOptionVal ) ;

		if( dwUpMana <= 0 )  
		{
			dwUpMana = 1 ;
		}

		DWORD dwCalcMana = energy + dwUpMana ;
				
		pPet->SetMana(dwCalcMana);
		pPet->m_ManaRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::Ungi_ManaCount(CPlayer* pPlayer, DWORD curTime, DWORD energy, DWORD maxEnergy)
{	
	// RaMa - 04.11.24  ->������Ľð�
	DWORD dwUngiTime = 5000;
	// dwUngiTime = dwUngiTime*(1/gUngiSpeed);
	dwUngiTime = (DWORD)(dwUngiTime*(1/gEventRate[eEvent_UngiSpeed]));

	// 2005 ũ�������� �̺�Ʈ �ڵ�
/*	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		dwUngiTime /= 2;
*/
	//������ ������� 5�ʸ��� ��ü���� 10(����ġ) + 3% ������
	if(curTime - pPlayer->m_ManaRecoverTime.lastCheckTime > dwUngiTime)
	{
		// RaMa - 04.11.17  ->ShopItemOption?Ç ³»·ÂÈ¸º¹·ê 1.5¹è Áõ°¡.
		DWORD recover = 0;
		recover = (DWORD)(10 + maxEnergy*(0.03f)) /*�����Ƽ ���� - + pPlayer->m_AbilityStats.UngiUpVal*/;
//		recover += (DWORD)(maxEnergy*pPlayer->GetUngiPlusRate());
		recover = (DWORD)(recover + (maxEnergy*pPlayer->GetUngiPlusRate()));

//		if( pPlayer->GetShopItemStats()->RecoverRate )
//		{
//			recover *= (DWORD)((pPlayer->GetShopItemStats()->RecoverRate*0.01f));
//			recover = (DWORD)(recover * ((pPlayer->GetShopItemStats()->RecoverRate*0.01f)));
//		}

		pPlayer->SetMana(energy + recover);
		pPlayer->m_ManaRecoverTime.lastCheckTime = curTime;
	}
}

void CCharacterCalcManager::KyungGong_ManaCount(CPlayer* pPlayer, DWORD curTime,WORD KyungGongIdx)
{
	ASSERT(KyungGongIdx);
	CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(KyungGongIdx);
	ASSERT(pKGInfo);
		
	if(curTime - pPlayer->m_KyungGongMana_LastCheckTime > 3000)
	{
		// RaMa - 06.06.08
//		if( pPlayer->GetAvatarOption()->NaeruykspendbyKG == 0 )
//			pPlayer->ReduceMana(pKGInfo->GetNeedMana());

		if(pPlayer->GetMana() == 0)
		{
			CCharMove::ForceStopKyungGong(pPlayer);
		}
		pPlayer->m_KyungGongMana_LastCheckTime = curTime;
	}
}


void CCharacterCalcManager::Initialize( CPlayer* player )
{
	// ��Ʈ �ɷ� �ʱ�ȭ
	player->ResetSetItemStatus();

	PlayerStat& stat = player->GetItemStats();
	ZeroMemory( &stat, sizeof( stat ) );

	typedef std::map< const SetScript*, DWORD > SetItemSize;
	SetItemSize									setItemSize;

	float* pFishItemRate = player->GetFishItemRate();
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE* item = ITEMMGR->GetItemInfoAbsIn(player, part);

		if(	0 == item || 
			0 == item->dwDBIdx )
		{
			continue;
		}

		const ITEM_INFO * info = ITEMMGR->GetItemInfo(item->wIconIdx);

		if( ! info )
		{
			continue;
		}

		// ��Ʈ ������ �ɷ� �߰��� ����, ���� ������ ��Ʈ ��ũ��Ʈ�� ���� ������ ������ �����ص���
		{
			const SetScript* script = ITEMMGR->GetSetScript( item->wIconIdx );

			if( script )
			{
				++setItemSize[ script ];
			}
		}		

		AddStat( *info, stat );
		AddStat( *info, ITEMMGR->GetOption( *item ), stat );

		// ������ ������ ���� ���ù����Ȯ�� ���
		stFishingRate* pFishingRate = FISHINGMGR->GetFishingUtilityRate(info->ItemIdx);
		if(pFishingRate)
		{
			int i;
			for(i=0; i<MAX_FISHITEM; i++)
			{
				if(pFishingRate->FishList[i].nGrade < 0)
					continue;

				pFishItemRate[pFishingRate->FishList[i].nGrade] += pFishingRate->FishList[i].fRate;

				if(99.0f < pFishItemRate[pFishingRate->FishList[i].nGrade])
					pFishItemRate[pFishingRate->FishList[i].nGrade] = 99.0f;
			}
		}
	}

	// ��Ʈ �ɷ� ����. ���� �ܰ���� ������Ű�� �����ؾ� �Ѵ�
	for(
		SetItemSize::const_iterator it = setItemSize.begin();
		setItemSize.end() != it;
		++it )
	{
		const SetScript*	script	= it->first;
		const DWORD			size	= it->second;

		for( DWORD i = 1; i <= size; ++i )
		{
			AddSetItemStats( player, *script, i );
		}
	}

	CalcCharStats( player );
}


void CCharacterCalcManager::CalcCharStats(CPlayer* player)
{
	PlayerStat& char_stats = player->GetCharStats();
	ZeroMemory( &char_stats, sizeof( char_stats ) );

	HERO_TOTALINFO pHeroInfo;
	player->GetHeroTotalInfo( &pHeroInfo );
	
	char_stats.mDexterity.mPlus		= float( pHeroInfo.Dex );
	char_stats.mStrength.mPlus		= float( pHeroInfo.Str );
	char_stats.mVitality.mPlus		= float( pHeroInfo.Vit  );
	char_stats.mIntelligence.mPlus	= float( pHeroInfo.Int  );
	char_stats.mWisdom.mPlus		= float( pHeroInfo.Wis  );

	Init_HPMP_Point( player ) ;
	CalcMaxLife( player ) ;
	CalcMaxMana( player ) ;
	CalcCharPhyAttackMax( player ) ;
	CalcCharPhyAttackMin( player ) ;
	CalcCharPhyDefense( player ) ;
	CalcCharMagAttack( player ) ;
	CalcCharMagDefense( player ) ;
	CalcCharAccuracy( player ) ;
	CalcCharEvasion( player ) ;
	CalcCharCriticalRate( player ) ;
	CalcCharCriticalDamage( player ) ;
	// 080910 LUJ, ���� ���� ���
	CalcCharShieldDefense( player );
}


void CCharacterCalcManager::AddSetItemStats( CPlayer* player, const SetScript& script, int setItemSize )
{
	// ��Ʈ �������� ������ ���� ����ȴ�. ��Ʈ�� �����Ǵ� ������ �׷���� �ְ�, ����� ��Ʈ ������ ������ ���� �ɷ��� ���ʷ� �ο��ȴ�.

	// ��Ʈ ��ũ��Ʈ�� �ο��� ������ �����ͼ� �� �������� ����
	// ��Ʈ ��ũ��Ʈ�� �ɷ��� �䱸�ϴ� ��Ʈ ������ ���� ������ �����ϸ� �ɷ� �ο�

	typedef CPlayer::SetItemLevel SetItemLevel;

	CPlayer::SetItemLevel& setLevel = player->GetSetItemLevel();

	int& level = setLevel[ &script ];

	ASSERT( size_t( level ) < script.mAbility.size() );
	SetScript::Ability::const_iterator it = script.mAbility.begin();
	std::advance( it, level );

	for(	;
		script.mAbility.end() != it;
		++it, ++level )
	{
		const int size = it->first;

		if( size > setItemSize )
		{
			break;
		}

		// �ɷ��� �ο�����
		{
			const SetScript::Element&	element = it->second;

			// 080313 LUJ, ��Ʈ ������ ���ȿ� ó��
			AddStat( element.mStat, player->GetSetItemStats() );

			for(	SetScript::Element::Skill::const_iterator inner = element.mSkill.begin();
				element.mSkill.end() != inner;
				++inner )
			{
				const DWORD index	= inner->first;
				const BYTE	level	= BYTE( inner->second );

				player->AddSetSkill( index, level );
			}

		}
	}

	if( 0 == level )
	{
		setLevel.erase( &script );
	}
}


void CCharacterCalcManager::RemoveSetItemStats( CPlayer* player, const SetScript& script, int setItemSize )
{
	// �ش� �÷��̾ ���� ��Ʈ ��ũ��Ʈ�� �ο��� ������ �����´�.
	// ������ �ű���� �����ؼ� �Ʒ��� ����. ������ �׳� ��
	// ĳ���� ��Ʈ �ɷ��� ������ ����
	// ��ġ�� ��ȭ������ ��ġ ���� ��û

	typedef CPlayer::SetItemLevel SetItemLevel;

	CPlayer::SetItemLevel& setLevel = player->GetSetItemLevel();

	// �ش� ��ũ��Ʈ�� ����� ������ ���ٴ� �� ����� ��Ʈ �ɷ��� ���ٴ� ��
	if( setLevel.end() == setLevel.find( &script ) )
	{
		return;
	}

	CPlayer::SetItemLevel::iterator level_it = setLevel.find( &script );

	if( setLevel.end() == level_it )
	{
		return;
	}

	// 080602 LUJ, ���� ���� ������ �޾ƾ߸� ���� �ٲ� �� �����
	int& level = level_it->second;

	if( level > int( script.mAbility.size() ) )
	{
		return;
	}

	SetScript::Ability::const_iterator it = script.mAbility.begin();
	std::advance( it, level - 1 );

	for( ;; )	
	{
		const int size = it->first;

		if( size <= setItemSize )
		{
			break;
		}

		// �ɷ��� ��������
		{
			const SetScript::Element&	element = it->second;
			
			// 080313 LUJ, ��Ʈ ������ ���ȿ� ó��
			RemoveStat( element.mStat, player->GetSetItemStats() );

			for(
				SetScript::Element::Skill::const_iterator inner = element.mSkill.begin();
				element.mSkill.end() != inner;
				++inner )
			{
				const DWORD index	= inner->first;
				//const BYTE	level	= BYTE( inner->second );

				player->RemoveSetSkill( index );
			}
		}

		--level;

		if( script.mAbility.begin() == it-- )
		{
			break;
		}
	}


	if( ! level )
	{
		setLevel.erase( &script );
	}
}


void CCharacterCalcManager::AddItem( CPlayer* player, const ITEMBASE& item )
{
	PlayerStat& stat = player->GetItemStats();
	float* pFishItemRate = player->GetFishItemRate();

	const ITEM_INFO * info = ITEMMGR->GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	AddStat( *info, stat );
	AddStat( *info, ITEMMGR->GetOption( item ), stat );	
	
	// ��Ʈ ������ ���, ���� 070613
	{
		const SetScript* setScript = ITEMMGR->GetSetScript( item.wIconIdx );

		if( setScript )
		{
			AddSetItemStats( player, *setScript, ITEMMGR->GetSetItemSize( player, setScript ) );
		}
	}

	// TODO : ������ ����
	player->PassiveSkillCheckForWeareItem();
	CalcCharStats( player );	

	// ������ ������ ���� ���ù����Ȯ�� ���
	stFishingRate* pFishingRate = FISHINGMGR->GetFishingUtilityRate(info->ItemIdx);
	if(pFishingRate)
	{
		int i;
		for(i=0; i<MAX_FISHITEM; i++)
		{
			if(pFishingRate->FishList[i].nGrade < 0)
				continue;

			pFishItemRate[pFishingRate->FishList[i].nGrade] += pFishingRate->FishList[i].fRate;

			if(99.0f < pFishItemRate[pFishingRate->FishList[i].nGrade])
				pFishItemRate[pFishingRate->FishList[i].nGrade] = 99.0f;
		}
	}
}


void CCharacterCalcManager::RemoveItem( CPlayer* player, const ITEMBASE& item )
{
	const ITEM_INFO * info = ITEMMGR->GetItemInfo( item.wIconIdx );

	if( ! info )
	{
		return;
	}

	PlayerStat& stat = player->GetItemStats();
	float* pFishItemRate = player->GetFishItemRate();

	RemoveStat( *info, stat );
	RemoveStat( *info, ITEMMGR->GetOption( item ), stat );

	// ��Ʈ ������ ���, ���� 070613
	{
		const SetScript* setScript = ITEMMGR->GetSetScript( item.wIconIdx );

		if( setScript )
		{
			RemoveSetItemStats( player, *setScript, ITEMMGR->GetSetItemSize( player, setScript ) );
		}
	}

	// TODO : ������ ����
	player->PassiveSkillCheckForWeareItem();
	CalcCharStats( player );
	
	// ������ ������ ���� ���ù����Ȯ�� ���
	stFishingRate* pFishingRate = FISHINGMGR->GetFishingUtilityRate(info->ItemIdx);
	if(pFishingRate)
	{
		int i;
		for(i=0; i<MAX_FISHITEM; i++)
		{
			if(pFishingRate->FishList[i].nGrade < 0)
				continue;

			pFishItemRate[pFishingRate->FishList[i].nGrade] -= pFishingRate->FishList[i].fRate;

			if(pFishItemRate[pFishingRate->FishList[i].nGrade] < 0.0f)
				pFishItemRate[pFishingRate->FishList[i].nGrade] = 0.0f;
		}
	}
}


void CCharacterCalcManager::AddStat( const ITEM_INFO& info, PlayerStat& stat )
{
	stat.mPhysicAttack.mPlus	+= float( info.PhysicAttack );
	stat.mMagicAttack.mPlus		+= float( info.MagicAttack );
	stat.mPhysicDefense.mPlus	+= float( info.PhysicDefense );
	stat.mMagicDefense.mPlus	+= float( info.MagicDefense );
	stat.mStrength.mPlus		+= float( info.ImprovementStr );
	stat.mDexterity.mPlus		+= float( info.ImprovementDex );
	stat.mVitality.mPlus 		+= float( info.ImprovementVit );
	stat.mWisdom.mPlus 			+= float( info.ImprovementWis );
	stat.mIntelligence.mPlus	+= float( info.ImprovementInt );
	stat.mLife.mPlus 			+= float( info.ImprovementLife );
	stat.mMana.mPlus 			+= float( info.ImprovementMana );
}


// 080319 LUJ, �߰� ��æƮ ��ġ�� �ο��Ѵ�
// 080320 LUJ, �Լ��� ���� ���� ���������� ��
void CCharacterCalcManager::AddStat( const ITEM_INFO& info, const ITEM_OPTION& option, PlayerStat& stat )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;

	stat.mPhysicAttack.mPlus	+= float( reinforce.mPhysicAttack );
	stat.mPhysicDefense.mPlus	+= float( reinforce.mPhysicDefence );
	stat.mMagicAttack.mPlus		+= float( reinforce.mMagicAttack );
	stat.mMagicDefense.mPlus	+= float( reinforce.mMagicDefence );
	stat.mStrength.mPlus		+= float( reinforce.mStrength		+ mix.mStrength );
	stat.mDexterity.mPlus		+= float( reinforce.mDexterity		+ mix.mDexterity );
	stat.mVitality.mPlus		+= float( reinforce.mVitality		+ mix.mVitality );
	stat.mWisdom.mPlus			+= float( reinforce.mWisdom			+ mix.mWisdom );
	stat.mIntelligence.mPlus	+= float( reinforce.mIntelligence	+ mix.mIntelligence );
	stat.mLife.mPlus			+= float( reinforce.mLife );
	stat.mMana.mPlus			+= float( reinforce.mMana );
	stat.mRecoveryLife.mPlus	+= float( reinforce.mLifeRecovery );
	stat.mRecoveryMana.mPlus	+= float( reinforce.mManaRecovery );
	stat.mCriticalRate.mPlus	+= float( reinforce.mCriticalRate );
	stat.mCriticalDamage.mPlus	+= float( reinforce.mCriticalDamage );
	stat.mAccuracy.mPlus		+= float( reinforce.mAccuracy );
	stat.mMoveSpeed.mPlus		+= float( reinforce.mMoveSpeed );
	stat.mEvade.mPlus			+= float( reinforce.mEvade );

	const ITEM_OPTION::Drop& drop = option.mDrop;

	for( DWORD i = 0; i < sizeof( drop.mValue ) / sizeof( *drop.mValue ); ++i )
	{
		const ITEM_OPTION::Drop::Value& value = drop.mValue[ i ];

		// 080410 LUJ, �Ǽ� ������ ���ֱ� ���� �Ҽ��� ���ڸ� �Ʒ��� �����Ѵ�
		switch( value.mKey )
		{
		case ITEM_OPTION::Drop::KeyPlusStrength:
			{
				stat.mStrength.mPlus = Round( stat.mStrength.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusIntelligence:
			{
				stat.mIntelligence.mPlus = Round( stat.mIntelligence.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusDexterity:
			{
				stat.mDexterity.mPlus = Round( stat.mDexterity.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusWisdom:
			{
				stat.mWisdom.mPlus = Round( stat.mWisdom.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusVitality:
			{
				stat.mVitality.mPlus = Round( stat.mVitality.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalAttack:
			{
				stat.mPhysicAttack.mPlus = Round( stat.mPhysicAttack.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
			{
				stat.mPhysicDefense.mPlus = Round( stat.mPhysicDefense.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalAttack:
			{
				stat.mMagicAttack.mPlus = Round( stat.mMagicAttack.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalDefence:
			{
				stat.mMagicDefense.mPlus = Round( stat.mMagicDefense.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalRate:
			{
				stat.mCriticalRate.mPlus = Round( stat.mCriticalRate.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalDamage:
			{
				stat.mCriticalDamage.mPlus = Round( stat.mCriticalDamage.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusAccuracy:
			{
				stat.mAccuracy.mPlus = Round( stat.mAccuracy.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusEvade:
			{
				stat.mEvade.mPlus = Round( stat.mEvade.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMoveSpeed:
			{
				stat.mMoveSpeed.mPlus = Round( stat.mMoveSpeed.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLife:
			{
				stat.mLife.mPlus = Round( stat.mLife.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMana:
			{
				stat.mMana.mPlus = Round( stat.mMana.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLifeRecovery:
			{
				stat.mRecoveryLife.mPlus = Round( stat.mRecoveryLife.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusManaRecovery:
			{
				stat.mRecoveryMana.mPlus = Round( stat.mRecoveryMana.mPlus + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentStrength:
			{
				stat.mStrength.mPercent = Round( stat.mStrength.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentIntelligence:
			{
				stat.mIntelligence.mPercent = Round( stat.mIntelligence.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentDexterity:
			{
				stat.mDexterity.mPercent = Round( stat.mDexterity.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentWisdom:
			{
				stat.mWisdom.mPercent = Round( stat.mWisdom.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentVitality:
			{
				stat.mVitality.mPercent = Round( stat.mVitality.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalAttack:
			{
				stat.mPhysicAttack.mPercent = Round( stat.mPhysicAttack.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
			{
				stat.mPhysicDefense.mPercent = Round( stat.mPhysicDefense.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalAttack:
			{
				stat.mMagicAttack.mPercent = Round( stat.mMagicAttack.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalDefence:
			{
				stat.mMagicDefense.mPercent = Round( stat.mMagicDefense.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalRate:
			{
				stat.mCriticalRate.mPercent = Round( stat.mCriticalRate.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalDamage:
			{
				stat.mCriticalDamage.mPercent = Round( stat.mCriticalDamage.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentAccuracy:
			{
				stat.mAccuracy.mPercent = Round( stat.mAccuracy.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentEvade:
			{
				stat.mEvade.mPercent = Round( stat.mEvade.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMoveSpeed:
			{
				stat.mMoveSpeed.mPercent = Round( stat.mMoveSpeed.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLife:
			{
				stat.mLife.mPercent = Round( stat.mLife.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMana:
			{
				stat.mMana.mPercent = Round( stat.mMana.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLifeRecovery:
			{
				stat.mRecoveryLife.mPercent = Round( stat.mRecoveryLife.mPercent + value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentManaRecovery:
			{
				stat.mRecoveryMana.mPercent = Round( stat.mRecoveryMana.mPercent + value.mValue, 3 );
				break;
			}
		}
	}

	// 080320 LUJ,	�߰� ��æƮ �ɷ�ġ�� �ο��Ѵ�
	// 080421 LUJ,	�߰� ��æƮ ��ġ ������ �ܼ�ȭ 
	{
		const EnchantScript* script = ITEMMGR->GetEnchantScript( option.mEnchant.mIndex );

		if( script && option.mEnchant.mLevel )
		{
			for(
				EnchantScript::Ability::const_iterator it = script->mAbility.begin();
				script->mAbility.end() != it;
				++it )
			{
				switch( EnchantScript::eType( *it ) )
				{
				case ReinforceScript::eTypeStrength:
					{
						stat.mStrength.mPlus += GetBonusEnchantValue( info, option, info.ImprovementStr );
						break;
					}
				case ReinforceScript::eTypeDexterity:
					{
						stat.mDexterity.mPlus += GetBonusEnchantValue( info, option, info.ImprovementDex );
						break;
					}
				case ReinforceScript::eTypeVitality:
					{
						stat.mVitality.mPlus += GetBonusEnchantValue( info, option, info.ImprovementVit );
						break;
					}
				case ReinforceScript::eTypeWisdom:
					{
						stat.mWisdom.mPlus += GetBonusEnchantValue( info, option, info.ImprovementWis );
						break;
					}
				case ReinforceScript::eTypeIntelligence:
					{
						stat.mIntelligence.mPlus += GetBonusEnchantValue( info, option, info.ImprovementInt );
						break;
					}
				case ReinforceScript::eTypeLife:
					{
						stat.mLife.mPlus += GetBonusEnchantValue( info, option, info.ImprovementLife );
						break;
					}
				case ReinforceScript::eTypeMana:
					{
						stat.mMana.mPlus += GetBonusEnchantValue( info, option, info.ImprovementMana );
						break;
					}
				case ReinforceScript::eTypePhysicAttack:
					{
						stat.mPhysicAttack.mPlus += GetBonusEnchantValue( info, option, info.PhysicAttack );
						break;
					}
				case ReinforceScript::eTypePhysicDefence:
					{
						stat.mPhysicDefense.mPlus += GetBonusEnchantValue( info, option, info.PhysicDefense );
						break;
					}
				case ReinforceScript::eTypeMagicAttack:
					{
						stat.mMagicAttack.mPlus += GetBonusEnchantValue( info, option, info.MagicAttack );
						break;
					}
				case ReinforceScript::eTypeMagicDefence:
					{
						stat.mMagicDefense.mPlus += GetBonusEnchantValue( info, option, info.MagicDefense );
						break;
					}
					// 080319 LUJ, �߰� ��æƮ ��ġ�� �ο��� �� ���� �Ӽ���
				case ReinforceScript::eTypeMoveSpeed:
				case ReinforceScript::eTypeEvade:
				case ReinforceScript::eTypeAccuracy:
				case ReinforceScript::eTypeLifeRecovery:
				case ReinforceScript::eTypeManaRecovery:
				case ReinforceScript::eTypeCriticalRate:
				case ReinforceScript::eTypeCriticalDamage:
					{
						break;
					}
				}
			}
		}
	}
}


void CCharacterCalcManager::AddStat( const PlayerStat& srcStat, PlayerStat& destStat )
{
	destStat.mPhysicAttack.mPercent		+= srcStat.mPhysicAttack.mPercent;
	destStat.mPhysicAttack.mPlus		+= srcStat.mPhysicAttack.mPlus;

	destStat.mMagicAttack.mPercent		+= srcStat.mMagicAttack.mPercent;
	destStat.mMagicAttack.mPlus			+= srcStat.mMagicAttack.mPlus;

	destStat.mPhysicDefense.mPercent	+= srcStat.mPhysicDefense.mPercent;
	destStat.mPhysicDefense.mPlus		+= srcStat.mPhysicDefense.mPlus;

	destStat.mMagicDefense.mPercent		+= srcStat.mMagicDefense.mPercent;
	destStat.mMagicDefense.mPlus		+= srcStat.mMagicDefense.mPlus;

	destStat.mStrength.mPercent			+= srcStat.mStrength.mPercent;
	destStat.mStrength.mPlus			+= srcStat.mStrength.mPlus;

	destStat.mDexterity.mPercent 		+= srcStat.mDexterity.mPercent;
	destStat.mDexterity.mPlus 			+= srcStat.mDexterity.mPlus;

	destStat.mVitality.mPercent			+= srcStat.mVitality.mPercent;
	destStat.mVitality.mPlus			+= srcStat.mVitality.mPlus;

	destStat.mIntelligence.mPercent 	+= srcStat.mIntelligence.mPercent;
	destStat.mIntelligence.mPlus 		+= srcStat.mIntelligence.mPlus;

	destStat.mWisdom.mPercent 			+= srcStat.mWisdom.mPercent;
	destStat.mWisdom.mPlus 				+= srcStat.mWisdom.mPlus;

	destStat.mLife.mPercent				+= srcStat.mLife.mPercent;
	destStat.mLife.mPlus				+= srcStat.mLife.mPlus;

	destStat.mMana.mPercent				+= srcStat.mMana.mPercent;
	destStat.mMana.mPlus				+= srcStat.mMana.mPlus;

	destStat.mRecoveryLife.mPercent	+= srcStat.mRecoveryLife.mPercent;
	destStat.mRecoveryLife.mPlus		+= srcStat.mRecoveryLife.mPlus;

	destStat.mRecoveryMana.mPercent		+= srcStat.mRecoveryMana.mPercent;
	destStat.mRecoveryMana.mPlus		+= srcStat.mRecoveryMana.mPlus;

	destStat.mAccuracy.mPercent			+= srcStat.mAccuracy.mPercent;
	destStat.mAccuracy.mPlus			+= srcStat.mAccuracy.mPlus;

	destStat.mEvade.mPercent			+= srcStat.mEvade.mPercent;
	destStat.mEvade.mPlus				+= srcStat.mEvade.mPlus;

	destStat.mCriticalRate.mPercent		+= srcStat.mCriticalRate.mPercent;
	destStat.mCriticalRate.mPlus		+= srcStat.mCriticalRate.mPlus;

	destStat.mCriticalDamage.mPercent	+= srcStat.mCriticalDamage.mPercent;
	destStat.mCriticalDamage.mPlus		+= srcStat.mCriticalDamage.mPlus;

	destStat.mMoveSpeed.mPercent		+= srcStat.mMoveSpeed.mPercent;
	destStat.mMoveSpeed.mPlus			+= srcStat.mMoveSpeed.mPlus;
}


void CCharacterCalcManager::RemoveStat( const ITEM_INFO& info, PlayerStat& stat )
{
	stat.mPhysicAttack.mPlus	-= float( info.PhysicAttack );
	stat.mMagicAttack.mPlus		-= float( info.MagicAttack );
	stat.mPhysicDefense.mPlus	-= float( info.PhysicDefense );
	stat.mMagicDefense.mPlus	-= float( info.MagicDefense );
	stat.mStrength.mPlus		-= float( info.ImprovementStr );
	stat.mDexterity.mPlus		-= float( info.ImprovementDex );
	stat.mVitality.mPlus 		-= float( info.ImprovementVit );
	stat.mWisdom.mPlus 			-= float( info.ImprovementWis );
	stat.mIntelligence.mPlus	-= float( info.ImprovementInt );
	stat.mLife.mPlus 			-= float( info.ImprovementLife );
	stat.mMana.mPlus 			-= float( info.ImprovementMana );
}


// 080319 LUJ, �߰� ��æƮ ��ġ�� ������
// 080320 LUJ, ������ �߰� ��æƮ ��ġ�� �Լ��� ������
void CCharacterCalcManager::RemoveStat( const ITEM_INFO& info, const ITEM_OPTION& option, PlayerStat& stat )
{
	const ITEM_OPTION::Reinforce&	reinforce	= option.mReinforce;
	const ITEM_OPTION::Mix&			mix			= option.mMix;

	stat.mPhysicAttack.mPlus	-= float( reinforce.mPhysicAttack );
	stat.mPhysicDefense.mPlus	-= float( reinforce.mPhysicDefence );
	stat.mMagicAttack.mPlus		-= float( reinforce.mMagicAttack );
	stat.mMagicDefense.mPlus	-= float( reinforce.mMagicDefence );
	stat.mStrength.mPlus		-= float( reinforce.mStrength		+ mix.mStrength );
	stat.mDexterity.mPlus		-= float( reinforce.mDexterity		+ mix.mDexterity );
	stat.mVitality.mPlus		-= float( reinforce.mVitality		+ mix.mVitality );
	stat.mWisdom.mPlus			-= float( reinforce.mWisdom			+ mix.mWisdom );
	stat.mIntelligence.mPlus	-= float( reinforce.mIntelligence	+ mix.mIntelligence );
	stat.mLife.mPlus			-= float( reinforce.mLife );
	stat.mMana.mPlus			-= float( reinforce.mMana );
	stat.mRecoveryLife.mPlus	-= float( reinforce.mLifeRecovery );
	stat.mRecoveryMana.mPlus	-= float( reinforce.mManaRecovery );
	stat.mCriticalRate.mPlus	-= float( reinforce.mCriticalRate );
	stat.mCriticalDamage.mPlus	-= float( reinforce.mCriticalDamage );
	stat.mAccuracy.mPlus		-= float( reinforce.mAccuracy );
	stat.mMoveSpeed.mPlus		-= float( reinforce.mMoveSpeed );
	stat.mEvade.mPlus			-= float( reinforce.mEvade );

	const ITEM_OPTION::Drop& drop = option.mDrop;

	for( DWORD i = 0; i < sizeof( drop.mValue ) / sizeof( *drop.mValue ); ++i )
	{
		const ITEM_OPTION::Drop::Value& value = drop.mValue[ i ];

		// 080410 LUJ, �Ǽ� ������ ���ֱ� ���� �Ҽ��� ���ڸ� �Ʒ��� �����Ѵ�
		switch( value.mKey )
		{
		case ITEM_OPTION::Drop::KeyPlusStrength:
			{
				stat.mStrength.mPlus = Round( stat.mStrength.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusIntelligence:
			{
				stat.mIntelligence.mPlus = Round( stat.mIntelligence.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusDexterity:
			{
				stat.mDexterity.mPlus = Round( stat.mDexterity.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusWisdom:
			{
				stat.mWisdom.mPlus = Round( stat.mWisdom.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusVitality:
			{
				stat.mVitality.mPlus = Round( stat.mVitality.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalAttack:
			{
				stat.mPhysicAttack.mPlus = Round( stat.mPhysicAttack.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusPhysicalDefence:
			{
				stat.mPhysicDefense.mPlus = Round( stat.mPhysicDefense.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalAttack:
			{
				stat.mMagicAttack.mPlus = Round( stat.mMagicAttack.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMagicalDefence:
			{
				stat.mMagicDefense.mPlus = Round( stat.mMagicDefense.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalRate:
			{
				stat.mCriticalRate.mPlus = Round( stat.mCriticalRate.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusCriticalDamage:
			{
				stat.mCriticalDamage.mPlus = Round( stat.mCriticalDamage.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusAccuracy:
			{
				stat.mAccuracy.mPlus = Round( stat.mAccuracy.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusEvade:
			{
				stat.mEvade.mPlus = Round( stat.mEvade.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMoveSpeed:
			{
				stat.mMoveSpeed.mPlus = Round( stat.mMoveSpeed.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLife:
			{
				stat.mLife.mPlus = Round( stat.mLife.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusMana:
			{
				stat.mMana.mPlus = Round( stat.mMana.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusLifeRecovery:
			{
				stat.mRecoveryLife.mPlus = Round( stat.mRecoveryLife.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPlusManaRecovery:
			{
				stat.mRecoveryMana.mPlus = Round( stat.mRecoveryMana.mPlus - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentStrength:
			{
				stat.mStrength.mPercent = Round( stat.mStrength.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentIntelligence:
			{
				stat.mIntelligence.mPercent = Round( stat.mIntelligence.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentDexterity:
			{
				stat.mDexterity.mPercent = Round( stat.mDexterity.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentWisdom:
			{
				stat.mWisdom.mPercent = Round( stat.mWisdom.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentVitality:
			{
				stat.mVitality.mPercent = Round( stat.mVitality.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalAttack:
			{
				stat.mPhysicAttack.mPercent = Round( stat.mPhysicAttack.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentPhysicalDefence:
			{
				stat.mPhysicDefense.mPercent = Round( stat.mPhysicDefense.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalAttack:
			{
				stat.mMagicAttack.mPercent = Round( stat.mMagicAttack.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMagicalDefence:
			{
				stat.mMagicDefense.mPercent = Round( stat.mMagicDefense.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalRate:
			{
				stat.mCriticalRate.mPercent = Round( stat.mCriticalRate.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentCriticalDamage:
			{
				stat.mCriticalDamage.mPercent = Round( stat.mCriticalDamage.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentAccuracy:
			{
				stat.mAccuracy.mPercent = Round( stat.mAccuracy.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentEvade:
			{
				stat.mEvade.mPercent = Round( stat.mEvade.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMoveSpeed:
			{
				stat.mMoveSpeed.mPercent = Round( stat.mMoveSpeed.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLife:
			{
				stat.mLife.mPercent = Round( stat.mLife.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentMana:
			{
				stat.mMana.mPercent = Round( stat.mMana.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentLifeRecovery:
			{
				stat.mRecoveryLife.mPercent = Round( stat.mRecoveryLife.mPercent - value.mValue, 3 );
				break;
			}
		case ITEM_OPTION::Drop::KeyPercentManaRecovery:
			{
				stat.mRecoveryMana.mPercent = Round( stat.mRecoveryMana.mPercent - value.mValue, 3 );
				break;
			}
		}
	}

	// 080320 LUJ,	�߰� ��æƮ �ɷ�ġ�� �����Ѵ�
	// 080421 LUJ,	�߰� ��æƮ ��ġ ������ �ܼ�ȭ 
	{
		const EnchantScript* script = ITEMMGR->GetEnchantScript( option.mEnchant.mIndex );

		if( script && option.mEnchant.mLevel )
		{
			for(
				EnchantScript::Ability::const_iterator it = script->mAbility.begin();
				script->mAbility.end() != it;
				++it )
			{
				switch( EnchantScript::eType( *it ) )
				{
				case ReinforceScript::eTypeStrength:
					{
						stat.mStrength.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementStr );
						break;
					}
				case ReinforceScript::eTypeDexterity:
					{
						stat.mDexterity.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementDex );
						break;
					}
				case ReinforceScript::eTypeVitality:
					{
						stat.mVitality.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementVit );
						break;
					}
				case ReinforceScript::eTypeWisdom:
					{
						stat.mWisdom.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementWis );
						break;
					}
				case ReinforceScript::eTypeIntelligence:
					{
						stat.mIntelligence.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementInt );
						break;
					}
				case ReinforceScript::eTypeLife:
					{
						stat.mLife.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementLife );
						break;
					}
				case ReinforceScript::eTypeMana:
					{
						stat.mMana.mPlus -= GetBonusEnchantValue( info, option, info.ImprovementMana );
						break;
					}
				case ReinforceScript::eTypePhysicAttack:
					{
						stat.mPhysicAttack.mPlus -= GetBonusEnchantValue( info, option, info.PhysicAttack );
						break;
					}
				case ReinforceScript::eTypePhysicDefence:
					{
						stat.mPhysicDefense.mPlus -= GetBonusEnchantValue( info, option, info.PhysicDefense );
						break;
					}
				case ReinforceScript::eTypeMagicAttack:
					{
						stat.mMagicAttack.mPlus -= GetBonusEnchantValue( info, option, info.MagicAttack );
						break;
					}
				case ReinforceScript::eTypeMagicDefence:
					{
						stat.mMagicDefense.mPlus -= GetBonusEnchantValue( info, option, info.MagicDefense );
						break;
					}
					// 080319 LUJ, �߰� ��æƮ ��ġ�� �ο��� �� ���� �Ӽ���
				case ReinforceScript::eTypeMoveSpeed:
				case ReinforceScript::eTypeEvade:
				case ReinforceScript::eTypeAccuracy:
				case ReinforceScript::eTypeLifeRecovery:
				case ReinforceScript::eTypeManaRecovery:
				case ReinforceScript::eTypeCriticalRate:
				case ReinforceScript::eTypeCriticalDamage:
					{
						break;
					}
				}
			}
		}
	}
}


void CCharacterCalcManager::RemoveStat( const PlayerStat& srcStat, PlayerStat& destStat )
{
	destStat.mPhysicAttack.mPercent		-= srcStat.mPhysicAttack.mPercent;
	destStat.mPhysicAttack.mPlus		-= srcStat.mPhysicAttack.mPlus;

	destStat.mMagicAttack.mPercent		-= srcStat.mMagicAttack.mPercent;
	destStat.mMagicAttack.mPlus			-= srcStat.mMagicAttack.mPlus;

	destStat.mPhysicDefense.mPercent	-= srcStat.mPhysicDefense.mPercent;
	destStat.mPhysicDefense.mPlus		-= srcStat.mPhysicDefense.mPlus;

	destStat.mMagicDefense.mPercent		-= srcStat.mMagicDefense.mPercent;
	destStat.mMagicDefense.mPlus		-= srcStat.mMagicDefense.mPlus;

	destStat.mStrength.mPercent			-= srcStat.mStrength.mPercent;
	destStat.mStrength.mPlus			-= srcStat.mStrength.mPlus;

	destStat.mDexterity.mPercent 		-= srcStat.mDexterity.mPercent;
	destStat.mDexterity.mPlus 			-= srcStat.mDexterity.mPlus;

	destStat.mVitality.mPercent			-= srcStat.mVitality.mPercent;
	destStat.mVitality.mPlus			-= srcStat.mVitality.mPlus;

	destStat.mIntelligence.mPercent 	-= srcStat.mIntelligence.mPercent;
	destStat.mIntelligence.mPlus 		-= srcStat.mIntelligence.mPlus;

	destStat.mWisdom.mPercent 			-= srcStat.mWisdom.mPercent;
	destStat.mWisdom.mPlus 				-= srcStat.mWisdom.mPlus;

	destStat.mLife.mPercent				-= srcStat.mLife.mPercent;
	destStat.mLife.mPlus				-= srcStat.mLife.mPlus;

	destStat.mMana.mPercent				-= srcStat.mMana.mPercent;
	destStat.mMana.mPlus				-= srcStat.mMana.mPlus;

	destStat.mRecoveryLife.mPercent	-= srcStat.mRecoveryLife.mPercent;
	destStat.mRecoveryLife.mPlus		-= srcStat.mRecoveryLife.mPlus;

	destStat.mRecoveryMana.mPercent		-= srcStat.mRecoveryMana.mPercent;
	destStat.mRecoveryMana.mPlus		-= srcStat.mRecoveryMana.mPlus;

	destStat.mAccuracy.mPercent			-= srcStat.mAccuracy.mPercent;
	destStat.mAccuracy.mPlus			-= srcStat.mAccuracy.mPlus;

	destStat.mEvade.mPercent			-= srcStat.mEvade.mPercent;
	destStat.mEvade.mPlus				-= srcStat.mEvade.mPlus;

	destStat.mCriticalRate.mPercent		-= srcStat.mCriticalRate.mPercent;
	destStat.mCriticalRate.mPlus		-= srcStat.mCriticalRate.mPlus;

	destStat.mCriticalDamage.mPercent	-= srcStat.mCriticalDamage.mPercent;
	destStat.mCriticalDamage.mPlus		-= srcStat.mCriticalDamage.mPlus;

	destStat.mMoveSpeed.mPercent		-= srcStat.mMoveSpeed.mPercent;
	destStat.mMoveSpeed.mPlus			-= srcStat.mMoveSpeed.mPlus;
}
