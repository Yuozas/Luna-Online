#include "stdafx.h"
#include "buffskillinfo.h"
#include "player.h"
#include "monster.h"
#include "CharacterCalcManager.h"
#include "CharMove.h"
#include "pet.h"


cBuffSkillInfo::cBuffSkillInfo(void)
{
	mExceptionTable.Initialize(1);
}

cBuffSkillInfo::~cBuffSkillInfo(void)
{
	EXCEPTION* pException = NULL;

	mExceptionTable.SetPositionHead();

	while( pException = mExceptionTable.GetData() )
	{
		delete pException;
	}
	mExceptionTable.RemoveAll();

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
	pSkillInfo->Tooltip = pFile->GetWord();

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
	// 071224 KTH -- 파티가 걸려 있으면 다른 처리를 하기 위하여 추가.
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

void cBuffSkillInfo::AddPassiveStatus( CPlayer* pPlayer )
{
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	Status* pStatus = NULL;

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPlayer->GetPassiveStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent ) 
	{
		pStatus = pPlayer->GetRatePassiveStatus();
	}
	else
	{
		pStatus = pPlayer->GetPassiveStatus();
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate += pSkillInfo->StatusData;	break;
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
			pPlayer->GetHeroTotalInfo()->bUsingTwoBlade = ( 0 < pSkillInfo->StatusData );
		}
		break;
	}
}

void cBuffSkillInfo::RemovePassiveStatus( CPlayer* pPlayer )
{
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	Status* pStatus = NULL;

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPlayer->GetPassiveStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent ) 
	{
		pStatus = pPlayer->GetRatePassiveStatus();
	}
	else
	{
		pStatus = pPlayer->GetPassiveStatus();
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate -= pSkillInfo->StatusData;	break;
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
			pPlayer->GetHeroTotalInfo()->bUsingTwoBlade = ( 0 < pSkillInfo->StatusData );
		}
		break;
	}
}

void cBuffSkillInfo::AddBuffStatus( CObject* pTarget )
{
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		AddPlayerBuffStatus( (CPlayer*)pTarget );
	}
	else if(  pTarget->GetObjectKind() & eObjectKind_Monster )
	{
		AddMonsterBuffStatus( (CMonster*)pTarget );
	}
	else if(  pTarget->GetObjectKind() & eObjectKind_Pet )
	{
		AddPetBuffStatus( (CPet*)pTarget );
	}
	else
	{
		ASSERT(0);
	}
}

void cBuffSkillInfo::RemoveBuffStatus( CObject* pTarget )
{
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		RemovePlayerBuffStatus( (CPlayer*)pTarget );
	}
	else if(  pTarget->GetObjectKind() & eObjectKind_Monster )
	{
		RemoveMonsterBuffStatus( (CMonster*)pTarget );
	}
	else if(  pTarget->GetObjectKind() & eObjectKind_Pet )
	{
		RemovePetBuffStatus( (CPet*)pTarget );
	}
	else
	{
		ASSERT(0);
	}
}

void cBuffSkillInfo::AddPlayerBuffStatus( CPlayer* pPlayer )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pPlayer->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPlayer->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pPlayer->GetRateBuffStatus();
	}
	else
	{
		pStatus = pPlayer->GetBuffStatus();
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
	case eStatusKind_Hide:
		{
			pPlayer->SetHide( ( WORD )( pSkillInfo->StatusData ) );
		}
		break;
	case eStatusKind_Detect:
		{
			pPlayer->SetDetect( ( WORD )( pSkillInfo->StatusData ) );
		}
		break;
	case eStatusKind_Paralysis:		
		{
			pAbnormalStatus->IsParalysis = true;	
			CCharMove::CorrectPlayerPosToServer( pPlayer );
		}
		break;
	case eStatusKind_Stun:			
		{
			pAbnormalStatus->IsStun = true;
			CCharMove::CorrectPlayerPosToServer( pPlayer );
		}
		break;
	case eStatusKind_Slip:			
		{
			pAbnormalStatus->IsSlip = true;
			CCharMove::CorrectPlayerPosToServer( pPlayer );
		}
		break;
	case eStatusKind_Freezing:		
		{
			pAbnormalStatus->IsFreezing = true;
			CCharMove::CorrectPlayerPosToServer( pPlayer );
		}
		break;
	case eStatusKind_Stone:			
		{
			pAbnormalStatus->IsStone = true;
			CCharMove::CorrectPlayerPosToServer( pPlayer );
		}
		break;
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = true;		break;
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate += pSkillInfo->StatusData;	break;
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
	// 071203 KTH -- DecreaseManaRate "소모되는 마나량의 % 감소율을 가르킨다.'
	case eStatusKind_DecreaseManaRate:	pStatus->DecreaseManaRate += pSkillInfo->StatusData;	break;
	// 071204 KTH -- IncreaseDropRate "드랍율을 상승 시켜준다 % "
	case eStatusKind_IncreaseDropRate:	pStatus->IncreaseDropRate += pSkillInfo->StatusData;	break;
	// 071217 KTH -- IsProtectExp "경험치 보호 가동(?)"
	case eStatusKind_ProtectExp:		pStatus->IsProtectExp = TRUE;	break;
	}

	CHARCALCMGR->CalcCharStats( pPlayer );
}

void cBuffSkillInfo::RemovePlayerBuffStatus( CPlayer* pPlayer )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pPlayer->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPlayer->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pPlayer->GetRateBuffStatus();
	}
	else
	{
		pStatus = pPlayer->GetBuffStatus();
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
	case eStatusKind_Hide:
		{
			pPlayer->SetHide( 0 );
		}
		break;
	case eStatusKind_Detect:
		{
			pPlayer->SetDetect( 0 );
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
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = false;		break;
	
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate -= pSkillInfo->StatusData;	break;
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
	// 071203 KTH -- DecreaseManaRate "소모되는 마나량의 % 감소율을 가르킨다."
	case eStatusKind_DecreaseManaRate:	pStatus->DecreaseManaRate -= pSkillInfo->StatusData;	break;
	// 071204 KTH -- IncreaseDropRate "드랍율을 상승 시켜준다 % "
	case eStatusKind_IncreaseDropRate:	pStatus->IncreaseDropRate -= pSkillInfo->StatusData;	break;
	// 071217 KTH -- IsProtectExp "경험치 보호 해제"
	case eStatusKind_ProtectExp:		pStatus->IsProtectExp = FALSE;	break;
	}

	CHARCALCMGR->CalcCharStats( pPlayer );

}

void cBuffSkillInfo::AddPetBuffStatus( CPet* pPet )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pPet->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPet->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pPet->GetRateBuffStatus();
	}
	else
	{
		pStatus = pPet->GetBuffStatus();
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
	case eStatusKind_Hide:
		{
		}
		break;
	case eStatusKind_Detect:
		{
		}
		break;
	case eStatusKind_Paralysis:		
		{
			pAbnormalStatus->IsParalysis = true;	
			CCharMove::CorrectPlayerPosToServer( pPet );
		}
		break;
	case eStatusKind_Stun:			
		{
			pAbnormalStatus->IsStun = true;
			CCharMove::CorrectPlayerPosToServer( pPet );
		}
		break;
	case eStatusKind_Slip:			
		{
			pAbnormalStatus->IsSlip = true;
			CCharMove::CorrectPlayerPosToServer( pPet );
		}
		break;
	case eStatusKind_Freezing:		
		{
			pAbnormalStatus->IsFreezing = true;
			CCharMove::CorrectPlayerPosToServer( pPet );
		}
		break;
	case eStatusKind_Stone:			
		{
			pAbnormalStatus->IsStone = true;
			CCharMove::CorrectPlayerPosToServer( pPet );
		}
		break;
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = true;		break;
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate += pSkillInfo->StatusData;	break;
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
	// 071203 KTH -- DecreaseManaRate "소모되는 마나량의 % 감소율을 가르킨다.'
	case eStatusKind_DecreaseManaRate:	pStatus->DecreaseManaRate += pSkillInfo->StatusData;	break;
	// 071204 KTH -- IncreaseDropRate "드랍율을 상승 시켜준다 % "
	case eStatusKind_IncreaseDropRate:	pStatus->IncreaseDropRate += pSkillInfo->StatusData;	break;
	// 071217 KTH -- IsProtectExp "경험치 보호 가동(?)"
	case eStatusKind_ProtectExp:		pStatus->IsProtectExp = TRUE;	break;
	}

	pPet->CalcStats();
}

void cBuffSkillInfo::RemovePetBuffStatus( CPet* pPet )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pPet->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pPet->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pPet->GetRateBuffStatus();
	}
	else
	{
		pStatus = pPet->GetBuffStatus();
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
	case eStatusKind_Hide:
		{
		}
		break;
	case eStatusKind_Detect:
		{
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
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = false;		break;
	
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
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate -= pSkillInfo->StatusData;	break;
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
	// 071203 KTH -- DecreaseManaRate "소모되는 마나량의 % 감소율을 가르킨다."
	case eStatusKind_DecreaseManaRate:	pStatus->DecreaseManaRate -= pSkillInfo->StatusData;	break;
	// 071204 KTH -- IncreaseDropRate "드랍율을 상승 시켜준다 % "
	case eStatusKind_IncreaseDropRate:	pStatus->IncreaseDropRate -= pSkillInfo->StatusData;	break;
	// 071217 KTH -- IsProtectExp "경험치 보호 해제"
	case eStatusKind_ProtectExp:		pStatus->IsProtectExp = FALSE;	break;
	}

	pPet->CalcStats();
}

void cBuffSkillInfo::AddMonsterBuffStatus( CMonster* pMonster )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pMonster->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pMonster->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pMonster->GetRateBuffStatus();
	}
	else
	{
		pStatus = pMonster->GetBuffStatus();
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
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = true;		break;
	case eStatusKind_Paralysis:		pAbnormalStatus->IsParalysis = true;	break;
	case eStatusKind_Stun:			pAbnormalStatus->IsStun = true;			break;
	case eStatusKind_Slip:			pAbnormalStatus->IsSlip = true;			break;
	case eStatusKind_Freezing:		pAbnormalStatus->IsFreezing = true;		break;
	case eStatusKind_Stone:			pAbnormalStatus->IsStone = true;		break;
	case eStatusKind_Silence:		pAbnormalStatus->IsSilence = true;		break;
	case eStatusKind_BlockAttack:	pAbnormalStatus->IsBlockAttack = true;	break;
	case eStatusKind_God:			pAbnormalStatus->IsGod = true;			break;
	case eStatusKind_MoveStop:		pAbnormalStatus->IsMoveStop = true;		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack += pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack += pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense += pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy += pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid += pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range += pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:	pStatus->SkillRange += pSkillInfo->StatusData;		break;
	case eStatusKind_MoveSpeed:		pStatus->MoveSpeed += pSkillInfo->StatusData;		break;

	case eStatusKind_MaxLife:		pStatus->MaxLife += pSkillInfo->StatusData;			break;
	}
}

void cBuffSkillInfo::RemoveMonsterBuffStatus( CMonster* pMonster )
{
	Status* pStatus = NULL;
	AbnormalStatus* pAbnormalStatus = pMonster->GetAbnormalStatus();
	BUFF_SKILL_INFO* pSkillInfo = GetSkillInfo();

	if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypeAdd )
	{
		pStatus = pMonster->GetBuffStatus();
	}
	else if( pSkillInfo->StatusDataType == BUFF_SKILL_INFO::StatusTypePercent )
	{
		pStatus = pMonster->GetRateBuffStatus();
	}
	else
	{
		pStatus = pMonster->GetBuffStatus();
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
	case eStatusKind_Attract:		pAbnormalStatus->IsAttract = false;		break;

	case eStatusKind_PhysicAttack:	pStatus->PhysicAttack -= pSkillInfo->StatusData;	break;
	case eStatusKind_PhysicDefense:	pStatus->PhysicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_MagicAttack:	pStatus->MagicAttack -= pSkillInfo->StatusData;		break;
	case eStatusKind_MagicDefense:	pStatus->MagicDefense -= pSkillInfo->StatusData;	break;
	case eStatusKind_Accuracy:		pStatus->Accuracy -= pSkillInfo->StatusData;		break;
	case eStatusKind_Avoid:			pStatus->Avoid -= pSkillInfo->StatusData;			break;
	case eStatusKind_CriticalRate:	pStatus->CriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_MagicCriticalRate:	pStatus->MagicCriticalRate += pSkillInfo->StatusData;	break;
	case eStatusKind_Range:			pStatus->Range -= pSkillInfo->StatusData;			break;

	case eStatusKind_SkillRange:	pStatus->SkillRange -= pSkillInfo->StatusData;		break;
	case eStatusKind_MoveSpeed:		pStatus->MoveSpeed -= pSkillInfo->StatusData;		break;

	case eStatusKind_MaxLife:		pStatus->MaxLife -= pSkillInfo->StatusData;			break;
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
		pPlayer->AddSkillException( pException );
	}
}

void cBuffSkillInfo::RemoveExceptionToTarget( CPlayer* pPlayer )
{
	EXCEPTION* pException = NULL;

	mExceptionTable.SetPositionHead();

	while( pException = mExceptionTable.GetData() )
	{
		pPlayer->AddSkillException( pException );
	}
}
