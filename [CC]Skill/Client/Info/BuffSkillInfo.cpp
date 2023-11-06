#include "stdafx.h"
#include ".\buffskillinfo.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "CharacterDialog.h"

cBuffSkillInfo::cBuffSkillInfo(void)
{
}

cBuffSkillInfo::~cBuffSkillInfo(void)
{
	delete mpSkillInfo;
}

BOOL cBuffSkillInfo::InitSkillInfo(CMHFile* pFile)
{
	ASSERT(pFile->IsInited());
	ASSERT(pFile->IsEOF() == FALSE);

	BUFF_SKILL_INFO* pSkillInfo = new BUFF_SKILL_INFO;
	// 080616 LUJ, 새로 추가된 값들이 있는데, 버프 스킬 정보에서 모두 설정해주지 않기 때문에 초기화해줘야 한다
	ZeroMemory( pSkillInfo, sizeof( *pSkillInfo ) );

	pSkillInfo->Index = pFile->GetDword();
	SafeStrCpy( pSkillInfo->Name, pFile->GetString(), MAX_SKILL_NAME_LENGTH+1 );

	pSkillInfo->Level = pFile->GetWord();

	pSkillInfo->Image = pFile->GetInt();
	pSkillInfo->Tooltip = pFile->GetDword();

	pSkillInfo->SkillEffect = pFile->GetWord();
	pSkillInfo->Kind = pFile->GetWord();
	pSkillInfo->DelayTime = pFile->GetDword();

	pSkillInfo->Attribute = pFile->GetWord();

	pSkillInfo->EquipItem[0] = pFile->GetWord();
	pSkillInfo->EquipItem[1] = pFile->GetWord();

	// 080616 LUJ, 타입을 enum으로 변환
	pSkillInfo->Status			= eStatusKind( pFile->GetWord() );

	switch( pSkillInfo->Status )
	{
	case eStatusKind_EventSkillToVictim:
	case eStatusKind_EventSkillToKiller:
		{
			pFile->GetWord();
			pSkillInfo->mEventSkillIndex	= pFile->GetDword();
			break;
		}
	default:
		{
			pSkillInfo->StatusDataType		= BUFF_SKILL_INFO::DataType( pFile->GetWord() );
			pSkillInfo->StatusData			= pFile->GetFloat();			
			break;
		}
	}

	pSkillInfo->ActiveRule = pFile->GetWord();
	pSkillInfo->ActiveRuleType = pFile->GetWord();
	pSkillInfo->ActiveRuleData = pFile->GetWord();

	pSkillInfo->Battle = pFile->GetBool();
	pSkillInfo->Peace = pFile->GetBool();
	pSkillInfo->Move = pFile->GetBool();
	pSkillInfo->Stop = pFile->GetBool();
	pSkillInfo->Rest = pFile->GetBool();

	pSkillInfo->Die = pFile->GetBool();

	pSkillInfo->ItemCount[0] = pFile->GetWord();
	pSkillInfo->ItemCount[1] = pFile->GetWord();
	pSkillInfo->Item[0] = pFile->GetWord();
	pSkillInfo->Item[1] = pFile->GetWord();
	pSkillInfo->Money = pFile->GetDword();
	// 080616 LUJ, 소요 HP/MP 값을 실수 값으로 넣는다
	pSkillInfo->mMana.mPlus	= pFile->GetFloat();
	pSkillInfo->mLife.mPlus	= pFile->GetFloat();

	pSkillInfo->Count = pFile->GetWord();
	pSkillInfo->CountType = pFile->GetByte();

	// 080219 LUJ, 기간제 스킬 여부
	pSkillInfo->IsEndTime	= pFile->GetBool();

	mpSkillInfo = ( SKILL_INFO* )pSkillInfo;

	return TRUE;
}

BUFF_SKILL_INFO* cBuffSkillInfo::GetSkillInfo()
{
	return ( BUFF_SKILL_INFO* )mpSkillInfo;
}

void cBuffSkillInfo::AddPassiveStatus()
{
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	Status* pStatus = NULL;

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HERO->GetPassiveStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent ) 
	{
		pStatus = HERO->GetRatePassiveStatus();
	}
	else
	{
		return;
	}
	
	switch( pSkillInfo->Status )
	{
	case eStatusKind_Str:	pStatus->Str += pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex += pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit += pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int += pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis += pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str += pSkillInfo->StatusData;
			pStatus->Dex += pSkillInfo->StatusData;
			pStatus->Vit += pSkillInfo->StatusData;
			pStatus->Int += pSkillInfo->StatusData;
			pStatus->Wis += pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack += pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack += pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy += pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid += pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range += pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange += pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage += pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed += pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block += pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime += pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect += pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife += pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana += pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover += pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover += pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect += pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb += pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife += pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana += pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife += pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana += pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp += pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold += pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark += pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword += pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace += pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe += pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff += pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow += pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun += pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger += pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear += pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded += pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade += pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded += pSkillInfo->StatusData;	break;

	case eStatusKind_RobeArmor:		pStatus->Robe += pSkillInfo->StatusData;		break;
	case eStatusKind_LightArmor:	pStatus->LightArmor += pSkillInfo->StatusData;	break;
	case eStatusKind_HeavyArmor:	pStatus->HeavyArmor += pSkillInfo->StatusData;	break;
	case eStatusKind_ShieldArmor:	pStatus->Shield += pSkillInfo->StatusData;		break;

	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate += pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate += pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate += pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;
		}		
		break;
		// 080703 LUJ, 이도류 사용 가능 여부 설정
	case eStatusKind_EnableTwoBlade:
		{
			HERO->GetHeroTotalInfo()->bUsingTwoBlade = ( 0 < pSkillInfo->StatusData );
		}
	}
	
	GAMEIN->GetCharacterDialog()->RefreshInfo();
}

void cBuffSkillInfo::RemovePassiveStatus()
{
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	Status* pStatus = NULL;

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HERO->GetPassiveStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent ) 
	{
		pStatus = HERO->GetRatePassiveStatus();
	}
	else
	{
		return;
	}

	switch( pSkillInfo->Status )
	{
	case eStatusKind_Str:	pStatus->Str -= pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex -= pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit -= pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int -= pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis -= pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str -= pSkillInfo->StatusData;
			pStatus->Dex -= pSkillInfo->StatusData;
			pStatus->Vit -= pSkillInfo->StatusData;
			pStatus->Int -= pSkillInfo->StatusData;
			pStatus->Wis -= pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack -= pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack -= pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy -= pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid -= pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range -= pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange -= pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage -= pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed -= pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block -= pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime -= pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect -= pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover -= pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover -= pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect -= pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb -= pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife -= pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana -= pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold -= pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark -= pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword -= pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace -= pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe -= pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff -= pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow -= pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun -= pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger -= pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear -= pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded -= pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade -= pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded -= pSkillInfo->StatusData;	break;

	case eStatusKind_RobeArmor:		pStatus->Robe -= pSkillInfo->StatusData;		break;
	case eStatusKind_LightArmor:	pStatus->LightArmor -= pSkillInfo->StatusData;	break;
	case eStatusKind_HeavyArmor:	pStatus->HeavyArmor -= pSkillInfo->StatusData;	break;
	case eStatusKind_ShieldArmor:	pStatus->Shield -= pSkillInfo->StatusData;		break;
	
	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate -= pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate -= pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate -= pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;
		}		
		break;
		// 080703 LUJ, 이도류 사용 가능 여부 설정
	case eStatusKind_EnableTwoBlade:
		{
			HERO->GetHeroTotalInfo()->bUsingTwoBlade = ( 0 < pSkillInfo->StatusData );
		}
	}
	
	GAMEIN->GetCharacterDialog()->RefreshInfo();
}

void cBuffSkillInfo::AddBuffStatus()
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = HERO->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HERO->GetBuffStatus();
	}
	// 080707 LUJ, 상수를 enum형으로 변경하면서 Percent값을 적용하지 않는 오류가 있어 수정함
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = HERO->GetRateBuffStatus();
	}
	else
	{
		return;
	}
	
	switch( pSkillInfo->Status )
	{
	case eStatusKind_Poison:
		{
			pAbnormalStatus->IsPoison = true;
			pAbnormalStatus->Poison += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Bleeding:
		{
			pAbnormalStatus->IsBleeding = true;
			pAbnormalStatus->Bleeding += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Burning:
		{
			pAbnormalStatus->IsBurning = true;
			pAbnormalStatus->Burning += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_HolyDamage:
		{
			pAbnormalStatus->IsHolyDamage = true;
			pAbnormalStatus->HolyDamage += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Shield:
		{
			pAbnormalStatus->IsShield = true;
			pAbnormalStatus->Shield += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Paralysis:		pAbnormalStatus->IsParalysis = true;	break;
	case eStatusKind_Stun:			pAbnormalStatus->IsStun = true;			break;
	case eStatusKind_Slip:			pAbnormalStatus->IsSlip = true;			break;
	case eStatusKind_Freezing:		pAbnormalStatus->IsFreezing = true;		break;
	case eStatusKind_Stone:			pAbnormalStatus->IsStone = true;		break;
	case eStatusKind_Silence:		pAbnormalStatus->IsSilence = true;		break;
	case eStatusKind_BlockAttack:	pAbnormalStatus->IsBlockAttack = true;	break;
	case eStatusKind_God:			pAbnormalStatus->IsGod = true;			break;
	case eStatusKind_MoveStop:		pAbnormalStatus->IsMoveStop = true;		break;
	
	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate += pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate += pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate += pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;
		}		
		break;

	case eStatusKind_Str:	pStatus->Str += pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex += pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit += pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int += pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis += pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str += pSkillInfo->StatusData;
			pStatus->Dex += pSkillInfo->StatusData;
			pStatus->Vit += pSkillInfo->StatusData;
			pStatus->Int += pSkillInfo->StatusData;
			pStatus->Wis += pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack += pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack += pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy += pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid += pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range += pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange += pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage += pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed += pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block += pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime += pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect += pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife += pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana += pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover += pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover += pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect += pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb += pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife += pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana += pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife += pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana += pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp += pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold += pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark += pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword += pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace += pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe += pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff += pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow += pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun += pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger += pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear += pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded += pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade += pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded += pSkillInfo->StatusData;	break;

	case eStatusKind_DecreaseManaRate : pStatus->DecreaseManaRate += pSkillInfo->StatusData ; break ;

	}

	GAMEIN->GetCharacterDialog()->RefreshInfo();
}

void cBuffSkillInfo::RemoveBuffStatus()
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = HERO->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HERO->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = HERO->GetRateBuffStatus();
	}
	else
	{
		return;
	}

	switch( pSkillInfo->Status )
	{
	case eStatusKind_Poison:
		{
			pAbnormalStatus->IsPoison = false;
			pAbnormalStatus->Poison -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Bleeding:
		{
			pAbnormalStatus->IsBleeding = false;
			pAbnormalStatus->Bleeding -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Burning:
		{
			pAbnormalStatus->IsBurning = false;
			pAbnormalStatus->Burning -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_HolyDamage:
		{
			pAbnormalStatus->IsHolyDamage = false;
			pAbnormalStatus->HolyDamage -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Shield:
		{
			pAbnormalStatus->IsShield = false;
			pAbnormalStatus->Shield -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Paralysis:		pAbnormalStatus->IsParalysis = false;	break;
	case eStatusKind_Stun:			pAbnormalStatus->IsStun = false;		break;
	case eStatusKind_Slip:			pAbnormalStatus->IsSlip = false;		break;
	case eStatusKind_Freezing:		pAbnormalStatus->IsFreezing = false;	break;
	case eStatusKind_Stone:			pAbnormalStatus->IsStone = false;		break;
	case eStatusKind_Silence:		pAbnormalStatus->IsSilence = false;		break;
	case eStatusKind_BlockAttack:	pAbnormalStatus->IsBlockAttack = false;	break;
	case eStatusKind_God:			pAbnormalStatus->IsGod = false;			break;
	case eStatusKind_MoveStop:		pAbnormalStatus->IsMoveStop = false;	break;

	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate -= pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate -= pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate -= pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;
		}		
		break;

	case eStatusKind_Str:	pStatus->Str -= pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex -= pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit -= pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int -= pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis -= pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str -= pSkillInfo->StatusData;
			pStatus->Dex -= pSkillInfo->StatusData;
			pStatus->Vit -= pSkillInfo->StatusData;
			pStatus->Int -= pSkillInfo->StatusData;
			pStatus->Wis -= pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack -= pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack -= pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy -= pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid -= pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range -= pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange -= pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage -= pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed -= pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block -= pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime -= pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect -= pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover -= pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover -= pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect -= pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb -= pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife -= pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana -= pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold -= pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark -= pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword -= pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace -= pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe -= pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff -= pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow -= pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun -= pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger -= pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear -= pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded -= pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade -= pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded -= pSkillInfo->StatusData;	break;

	case eStatusKind_DecreaseManaRate : pStatus->DecreaseManaRate -= pSkillInfo->StatusData ; break ;
	}

	GAMEIN->GetCharacterDialog()->RefreshInfo();
}


void cBuffSkillInfo::AddPetBuffStatus()
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = HERO->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HEROPET->GetBuffStatus();
	}
	// 080707 LUJ, 상수를 enum형으로 변경하면서 Percent값을 적용하지 않는 오류가 있어 수정함
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = HEROPET->GetRateBuffStatus();
	}
	else
	{
		return;
	}
	
	switch( pSkillInfo->Status )
	{
	case eStatusKind_Poison:
		{
			pAbnormalStatus->IsPoison = true;
			pAbnormalStatus->Poison += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Bleeding:
		{
			pAbnormalStatus->IsBleeding = true;
			pAbnormalStatus->Bleeding += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Burning:
		{
			pAbnormalStatus->IsBurning = true;
			pAbnormalStatus->Burning += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_HolyDamage:
		{
			pAbnormalStatus->IsHolyDamage = true;
			pAbnormalStatus->HolyDamage += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Shield:
		{
			pAbnormalStatus->IsShield = true;
			pAbnormalStatus->Shield += pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Paralysis:		pAbnormalStatus->IsParalysis = true;	break;
	case eStatusKind_Stun:			pAbnormalStatus->IsStun = true;			break;
	case eStatusKind_Slip:			pAbnormalStatus->IsSlip = true;			break;
	case eStatusKind_Freezing:		pAbnormalStatus->IsFreezing = true;		break;
	case eStatusKind_Stone:			pAbnormalStatus->IsStone = true;		break;
	case eStatusKind_Silence:		pAbnormalStatus->IsSilence = true;		break;
	case eStatusKind_BlockAttack:	pAbnormalStatus->IsBlockAttack = true;	break;
	case eStatusKind_God:			pAbnormalStatus->IsGod = true;			break;
	case eStatusKind_MoveStop:		pAbnormalStatus->IsMoveStop = true;		break;
	
	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate += pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate += pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate += pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate += pSkillInfo->StatusData;
		}		
		break;

	case eStatusKind_Str:	pStatus->Str += pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex += pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit += pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int += pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis += pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str += pSkillInfo->StatusData;
			pStatus->Dex += pSkillInfo->StatusData;
			pStatus->Vit += pSkillInfo->StatusData;
			pStatus->Int += pSkillInfo->StatusData;
			pStatus->Wis += pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack += pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack += pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy += pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid += pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range += pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange += pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage += pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed += pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block += pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime += pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect += pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife += pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana += pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate += pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover += pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover += pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect += pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb += pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife += pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana += pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife += pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana += pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp += pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold += pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine += pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire += pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark += pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword += pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace += pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe += pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff += pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow += pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun += pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger += pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear += pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded += pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade += pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded += pSkillInfo->StatusData;	break;

	case eStatusKind_DecreaseManaRate : pStatus->DecreaseManaRate += pSkillInfo->StatusData ; break ;

	}

	HEROPET->CalcStats();
}

void cBuffSkillInfo::RemovePetBuffStatus()
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = HERO->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = HEROPET->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = HEROPET->GetRateBuffStatus();
	}
	else
	{
		return;
	}

	switch( pSkillInfo->Status )
	{
	case eStatusKind_Poison:
		{
			pAbnormalStatus->IsPoison = false;
			pAbnormalStatus->Poison -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Bleeding:
		{
			pAbnormalStatus->IsBleeding = false;
			pAbnormalStatus->Bleeding -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Burning:
		{
			pAbnormalStatus->IsBurning = false;
			pAbnormalStatus->Burning -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_HolyDamage:
		{
			pAbnormalStatus->IsHolyDamage = false;
			pAbnormalStatus->HolyDamage -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Shield:
		{
			pAbnormalStatus->IsShield = false;
			pAbnormalStatus->Shield -= pSkillInfo->StatusData;
		}
		break;
	case eStatusKind_Paralysis:		pAbnormalStatus->IsParalysis = false;	break;
	case eStatusKind_Stun:			pAbnormalStatus->IsStun = false;		break;
	case eStatusKind_Slip:			pAbnormalStatus->IsSlip = false;		break;
	case eStatusKind_Freezing:		pAbnormalStatus->IsFreezing = false;	break;
	case eStatusKind_Stone:			pAbnormalStatus->IsStone = false;		break;
	case eStatusKind_Silence:		pAbnormalStatus->IsSilence = false;		break;
	case eStatusKind_BlockAttack:	pAbnormalStatus->IsBlockAttack = false;	break;
	case eStatusKind_God:			pAbnormalStatus->IsGod = false;			break;
	case eStatusKind_MoveStop:		pAbnormalStatus->IsMoveStop = false;	break;

	case eStatusKind_NormalSpeedRate:				pStatus->NormalSpeedRate -= pSkillInfo->StatusData;			break;
	case eStatusKind_PhysicSkillSpeedRate:			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicSkillSpeedRate:			pStatus->MagicSkillSpeedRate -= pSkillInfo->StatusData;		break;
	case eStatusKind_NormalPhysicSkillSpeedRate:	
		{
			pStatus->NormalSpeedRate -= pSkillInfo->StatusData;
			pStatus->PhysicSkillSpeedRate -= pSkillInfo->StatusData;
		}		
		break;

	case eStatusKind_Str:	pStatus->Str -= pSkillInfo->StatusData;	break;
	case eStatusKind_Dex:	pStatus->Dex -= pSkillInfo->StatusData;	break;
	case eStatusKind_Vit:	pStatus->Vit -= pSkillInfo->StatusData;	break;
	case eStatusKind_Int:	pStatus->Int -= pSkillInfo->StatusData;	break;
	case eStatusKind_Wis:	pStatus->Wis -= pSkillInfo->StatusData;	break;
	case eStatusKind_All:
		{
			pStatus->Str -= pSkillInfo->StatusData;
			pStatus->Dex -= pSkillInfo->StatusData;
			pStatus->Vit -= pSkillInfo->StatusData;
			pStatus->Int -= pSkillInfo->StatusData;
			pStatus->Wis -= pSkillInfo->StatusData;
		}
		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack -= pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack -= pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy -= pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid -= pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range -= pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:		pStatus->SkillRange -= pSkillInfo->StatusData;		break;
	case eStatusKind_CriticalDamage:	pStatus->CriticalDamage -= pSkillInfo->StatusData;	break;
	case eStatusKind_MoveSpeed:			pStatus->MoveSpeed -= pSkillInfo->StatusData;		break;
	case eStatusKind_Block:				pStatus->Block -= pSkillInfo->StatusData;			break;
	case eStatusKind_CoolTime:			pStatus->CoolTime -= pSkillInfo->StatusData;		break;
	case eStatusKind_CastingProtect:	pStatus->CastingProtect -= pSkillInfo->StatusData;	break;

	case eStatusKind_MaxLife:			pStatus->MaxLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_MaxMana:			pStatus->MaxMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_LifeRecoverRate:	pStatus->LifeRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_ManaRecoverRate:	pStatus->ManaRecoverRate -= pSkillInfo->StatusData;	break;
	case eStatusKind_LifeRecover:		pStatus->LifeRecover -= pSkillInfo->StatusData;		break;
	case eStatusKind_ManaRecover:		pStatus->ManaRecover -= pSkillInfo->StatusData;		break;

	case eStatusKind_Reflect:		pStatus->Reflect -= pSkillInfo->StatusData;			break;
	case eStatusKind_Absorb:		pStatus->Absorb -= pSkillInfo->StatusData;			break;
	case eStatusKind_DamageToLife:	pStatus->DamageToLife -= pSkillInfo->StatusData;	break;
	case eStatusKind_DamageToMana:	pStatus->DamageToMana -= pSkillInfo->StatusData;	break;
	case eStatusKind_GetLife:		pStatus->GetLife -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetMana:		pStatus->GetMana -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetExp:		pStatus->GetExp -= pSkillInfo->StatusData;			break;
	case eStatusKind_GetGold:		pStatus->GetGold -= pSkillInfo->StatusData;			break;

	case eStatusKind_Attrib_None:	pStatus->Attrib_None -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Earth:	pStatus->Attrib_Earth -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Water:	pStatus->Attrib_Water -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Divine:	pStatus->Attrib_Divine -= pSkillInfo->StatusData;	break;
	case eStatusKind_Attrib_Wind:	pStatus->Attrib_Wind -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Fire:	pStatus->Attrib_Fire -= pSkillInfo->StatusData;		break;
	case eStatusKind_Attrib_Dark:	pStatus->Attrib_Dark -= pSkillInfo->StatusData;		break;

	case eStatusKind_Sword:		pStatus->Sword -= pSkillInfo->StatusData;		break;
	case eStatusKind_Mace:		pStatus->Mace -= pSkillInfo->StatusData;		break;
	case eStatusKind_Axe:		pStatus->Axe -= pSkillInfo->StatusData;			break;
	case eStatusKind_Staff:		pStatus->Staff -= pSkillInfo->StatusData;		break;
	case eStatusKind_Bow:		pStatus->Bow -= pSkillInfo->StatusData;			break;
	case eStatusKind_Gun:		pStatus->Gun -= pSkillInfo->StatusData;			break;
	case eStatusKind_Dagger:	pStatus->Dagger -= pSkillInfo->StatusData;		break;
	case eStatusKind_Spear:		pStatus->Spear -= pSkillInfo->StatusData;		break;
	case eStatusKind_TwoHanded:	pStatus->TwoHanded -= pSkillInfo->StatusData;	break;
	case eStatusKind_TwoBlade:	pStatus->TwoBlade -= pSkillInfo->StatusData;	break;
	case eStatusKind_OneHanded:	pStatus->OneHanded -= pSkillInfo->StatusData;	break;

	case eStatusKind_DecreaseManaRate : pStatus->DecreaseManaRate -= pSkillInfo->StatusData ; break ;
	}

	HEROPET->CalcStats();
}

void cBuffSkillInfo::SetObjectBattleState( CObject* pOperator, CObject* pTarget )
{	
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	switch( pSkillInfo->Status )
	{
		case eStatusKind_None:
		case eStatusKind_Str:
		case eStatusKind_Dex:
		case eStatusKind_Vit:
		case eStatusKind_Int:
		case eStatusKind_Wis:
		case eStatusKind_All:
		case eStatusKind_PhysicAttack:
		case eStatusKind_PhysicDefense:
		case eStatusKind_MagicAttack:
		case eStatusKind_MagicDefense:
		case eStatusKind_Accuracy:
		case eStatusKind_Avoid:
		case eStatusKind_CriticalRate:
		case eStatusKind_Range:
		case eStatusKind_SkillRange:
		case eStatusKind_CriticalDamage:
		case eStatusKind_MoveSpeed:
		case eStatusKind_Block:
		case eStatusKind_CoolTime:
		case eStatusKind_CastingProtect:
		case eStatusKind_MaxLife:
		case eStatusKind_MaxMana:
		case eStatusKind_LifeRecoverRate:
		case eStatusKind_ManaRecoverRate:
			{
				if( pSkillInfo->StatusData < 0 )
				{
					if( pOperator ) pOperator->SetObjectBattleState( eObjectBattleState_Battle );
					if( pTarget ) pTarget->SetObjectBattleState( eObjectBattleState_Battle );
				}
				else
				{
					if( pTarget && pTarget->GetObjectBattleState() == eObjectBattleState_Battle )
					{
						if( pOperator ) pOperator->SetObjectBattleState( eObjectBattleState_Battle );
					}
				}
			}
			break;
		case eStatusKind_Poison:
		case eStatusKind_Paralysis:
		case eStatusKind_Stun:
		case eStatusKind_Slip:
		case eStatusKind_Freezing:
		case eStatusKind_Stone:
		case eStatusKind_Silence:
		case eStatusKind_BlockAttack:
		case eStatusKind_Bleeding:
		case eStatusKind_Burning:
		case eStatusKind_HolyDamage:
			{
				if( pOperator ) pOperator->SetObjectBattleState( eObjectBattleState_Battle );
				if( pTarget ) pTarget->SetObjectBattleState( eObjectBattleState_Battle );
			}
			break;
		case eStatusKind_God:
		case eStatusKind_MoveStop:
		case eStatusKind_Shield:
			{
				if( pTarget && pTarget->GetObjectBattleState() == eObjectBattleState_Battle )
				{
					if( pOperator ) pOperator->SetObjectBattleState( eObjectBattleState_Battle );
				}
			}
			break;
	}
}

void cBuffSkillInfo::AddException( EXCEPTION* pExeption )
{
	mExceptionTable.Add( pExeption, pExeption->Index );
}

void cBuffSkillInfo::AddExceptionToTarget( CPlayer* pPlayer )
{
	EXCEPTION* pException = NULL;

	mExceptionTable.SetPositionHead();

	while( pException = mExceptionTable.GetData() )
	{
	}
}

void cBuffSkillInfo::RemoveExceptionToTarget( CPlayer* pPlayer )
{
	EXCEPTION* pException = NULL;

	mExceptionTable.SetPositionHead();

	while( pException = mExceptionTable.GetData() )
	{
	}
}
