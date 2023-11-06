#include "stdafx.h"
#include ".\activeskillinfo.h"
#include "CharMove.h"
#include "Battle.h"
#include "Player.h"

cActiveSkillInfo::cActiveSkillInfo(void)
{
	ACTIVE_SKILL_INFO* info = new ACTIVE_SKILL_INFO;
	ZeroMemory( info, sizeof( *info ) );

	mpSkillInfo = info;
}
cActiveSkillInfo::~cActiveSkillInfo(void)
{
	delete mpSkillInfo;
}

BOOL cActiveSkillInfo::InitSkillInfo(CMHFile* pFile)
{
	if(	!	pFile				||
		!	pFile->IsInited()	||
			pFile->IsEOF()		||
		!	mpSkillInfo )
	{
		return FALSE;
	}

	ACTIVE_SKILL_INFO* info = ( ACTIVE_SKILL_INFO* )mpSkillInfo;
	ZeroMemory( info, sizeof( *info ) );

	char line[ MAX_PATH * 5 ] = { 0 };
	pFile->GetLine( line, sizeof( line ) );

	const char*	separator = "\t";

	info->Index						= atoi( strtok( line, separator ) );
	SafeStrCpy( info->Name, strtok( 0, separator ), sizeof( info->Name ) );
	info->Level						= atoi( strtok( 0, separator ) );
	info->Image						= atoi( strtok( 0, separator ) );
	info->Tooltip					= atoi( strtok( 0, separator ) );
	info->OperatorEffect			= atoi( strtok( 0, separator ) );
	info->TargetEffect				= atoi( strtok( 0, separator ) );
	info->SkillEffect				= atoi( strtok( 0, separator ) );
	info->AnimationTime				= atoi( strtok( 0, separator ) );
	info->CastingTime				= atoi( strtok( 0, separator ) );
	info->CoolTime					= atoi( strtok( 0, separator ) );
	info->DelayTime					= atoi( strtok( 0, separator ) );
	info->DelayType					= atoi( strtok( 0, separator ) );
	info->Battle					= atoi( strtok( 0, separator ) );
	info->Peace						= atoi( strtok( 0, separator ) );
	info->Move						= atoi( strtok( 0, separator ) );
	info->Stop						= atoi( strtok( 0, separator ) );
	info->Rest						= atoi( strtok( 0, separator ) );
	info->RequiredBuff				= atoi( strtok( 0, separator ) );
	info->Target					= atoi( strtok( 0, separator ) );
	info->Kind						= atoi( strtok( 0, separator ) );
	info->Attribute					= atoi( strtok( 0, separator ) );
	info->EquipItem[ 0 ]			= atoi( strtok( 0, separator ) );
	info->EquipItem[ 1 ]			= atoi( strtok( 0, separator ) );
	info->Area						= TARGET_KIND( atoi( strtok( 0, separator ) ) );
	info->AreaTarget				= eSkillAreaTarget( atoi( strtok( 0, separator ) ) );
	info->AreaData					= atoi( strtok( 0, separator ) );
	//info->AreaData				= atoi( strtok( 0, separator ) );
	//info->AreaData[ 1 ]				= atoi( strtok( 0, separator ) );
	//info->AreaData[ 2 ]				= atoi( strtok( 0, separator ) );
	info->Unit						= atoi( strtok( 0, separator ) );
	info->UnitDataType				= atoi( strtok( 0, separator ) );
	info->UnitData					= atoi( strtok( 0, separator ) );
	info->Success					= atoi( strtok( 0, separator ) );
	info->Cancel					= atoi( strtok( 0, separator ) );
	info->Range						= atoi( strtok( 0, separator ) );
	info->mConsumePoint.mType		= ACTIVE_SKILL_INFO::ConsumePoint::Type( atoi( strtok( 0, separator ) ) );
	info->mConsumePoint.mValue		= atoi( strtok( 0, separator ) );
	// 080602 LUJ, 스킬 사용 시 소모되는 아이템 정보 로딩
	info->mConsumeItem.wIconIdx		= atoi( strtok( 0, separator ) );
	info->mConsumeItem.Durability	= atoi( strtok( 0, separator ) );
	info->Money						= atoi( strtok( 0, separator ) );

	// 080616 LUJ, 문자열 뒤에 %가 붙어있으면 퍼센트 값으로 읽는다
	{
		// 080616 LUJ, MP 소요 수치
		{
			const char* token = strtok( 0, separator );

			if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
			{
				info->mMana.mPercent	= float( atof( token ) / 100.0f );
			}
			else
			{
				info->mMana.mPlus		= float( atof( token ) );
			}
		}

		// 080616 LUJ, HP 소요 수치
		{
			const char* token = strtok( 0, separator );

			// 080710 LUJ, HP 소모 값을 읽을 때 항상 퍼센트 형식으로 읽어오는 문제 수정
			if( _T( '%' ) == token[ _tcslen( token ) - 1 ] )
			{
				info->mLife.mPercent	= float( atof( token ) / 100.0f );
			}
			else
			{
				info->mLife.mPlus		= float( atof( token ) );
			}
		}
	}
	
	info->TrainPoint	= atoi( strtok( 0, separator ) );
	info->TrainMoney	= atoi( strtok( 0, separator ) );

	for( DWORD i = 0; i < MAX_BUFF_COUNT; ++i )
	{
		info->Buff[ i ]		= atoi( strtok( 0, separator ) );
		info->BuffRate[ i ]	= atoi( strtok( 0, separator ) );
	}

	info->IsMove				= atoi( strtok( 0, separator ) );
	// 080310 LUJ, 스킬 습득 제한 레벨을 읽는다
	info->RequiredPlayerLevel	= atoi( strtok( 0, separator ) );

	return TRUE;
}

BOOL cActiveSkillInfo::IsExcutableSkillState(CObject* pObject,int SkillLevel)
{	
	if( pObject->GetAbnormalStatus()->IsStun ||
		pObject->GetAbnormalStatus()->IsFreezing ||
		pObject->GetAbnormalStatus()->IsStone ||
		pObject->GetAbnormalStatus()->IsSlip ||
		pObject->GetAbnormalStatus()->IsParalysis )
	{
		return FALSE;
	}

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		////////////////////////////////////////////////////////////////////////////
		// 06. 07. 상태강제변경 - 이영준
		// 일정이상 스킬 사용실패시 특별한 상태를 제외한 나머지 상태는 모두 초기화
		if(!((CPlayer*)pObject)->CanSkillState())
			return FALSE;
		////////////////////////////////////////////////////////////////////////////
		switch( GetSkillInfo()->EquipItem[0] )
		{
		case 1:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_OneHand )
				{
					return FALSE;
				}
			}
			break;
		case 2:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_TwoHand )
				{
					return FALSE;
				}
			}
			break;
		case 3:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_TwoBlade )
				{
					return FALSE;
				}
			}
			break;
		case 4:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_Bow )
				{
					return FALSE;
				}
			}
			break;
		case 5:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_Gun )
				{
					return FALSE;
				}
			}
			break;
		case 6:
			{
				if( ((CPlayer*)pObject)->GetWeaponAniType() != eWeaponAnimationType_Dagger )
				{
					return FALSE;
				}
			}
			break;
		case 7:
			{
			}
			break;
		case 8:
			{
				if( ((CPlayer*)pObject)->GetWeaponEquipType() != eWeaponType_Staff )
				{
					return FALSE;
				}
			}
			break;
		case 9:
			{
				if( !((CPlayer*)pObject)->GetWearedItemIdx( eEquipSlot_Shield - 1 ) )
				{
					return FALSE;
				}
			}
			break;
		case 10:
		case 11:
		case 12:
			break;
		}
	}

	// 080616 LUJ, 퍼센트 정보도 함께 처리한다
	WORD MogongMana = WORD( float( pObject->GetMana() ) * GetSkillInfo()->mMana.mPercent + GetSkillInfo()->mMana.mPlus );
	
	// RaMa - 04.11.24  ->내력소모비율
	DWORD NeedMana = (DWORD)(MogongMana*gEventRate[eEvent_ManaRate]);
	
	// 071203 KTH -- PassiveStatus에서 소모 MP 감소를 처리한 부분
	Status* pStatus = ((CPlayer*)pObject)->GetRatePassiveStatus();
	float value = (float)(NeedMana/100);
	
	NeedMana = (DWORD) (NeedMana + ( NeedMana * (pObject->GetRateBuffStatus()->DecreaseManaRate / 100 ) ) );

	if(NeedMana)
	{
		// RaMa - 05.02.07   ->정신의 부적
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			// NeedMana *= (1.0f - ((CPlayer*)pObject)->GetShopItemStats()->NeaRuykSpend*0.01f);
			// AvatarItem에 내력소모감소옵션 추가
//			WORD manaSpend = ((CPlayer*)pObject)->GetShopItemStats()->ManaSpend + ((CPlayer*)pObject)->GetAvatarOption()->ManaSpend 
//				+ ((CPlayer*)pObject)->GetShopItemStats()->PlustimeMana;
//			if( manaSpend >= 100 )
//				NeedMana = 0;
//			else
//				NeedMana = (DWORD)(NeedMana*(1.0f - manaSpend*0.01f));

			// 06. 03 국내무쌍 - 이영준
			if( ((CPlayer*)pObject)->IsMussangMode() )
			{
				NeedMana = (DWORD)(NeedMana * 0.95f);
			}
		}
		if(pObject->GetMana() < NeedMana)
		{
			return FALSE;
		}
	}

	// 080616 LUJ, 퍼센트 정보도 함께 처리한다
	WORD MogongLife = WORD( float( pObject->GetLife() ) * GetSkillInfo()->mLife.mPercent + GetSkillInfo()->mLife.mPlus );
	
	// RaMa - 04.11.24  ->내력소모비율
	DWORD NeedLife = (DWORD)(MogongLife*gEventRate[eEvent_ManaRate]);

	if(NeedLife)
	{
		// RaMa - 05.02.07   ->정신의 부적
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
//			// NeedLife *= (1.0f - ((CPlayer*)pObject)->GetShopItemStats()->NeaRuykSpend*0.01f);
//			// AvatarItem에 내력소모감소옵션 추가
//			WORD LifeSpend = ((CPlayer*)pObject)->GetShopItemStats()->LifeSpend + ((CPlayer*)pObject)->GetAvatarOption()->LifeSpend 
//				+ ((CPlayer*)pObject)->GetShopItemStats()->PlustimeLife;
//			if( LifeSpend >= 100 )
//				NeedLife = 0;
//			else
//				NeedLife = (DWORD)(NeedLife*(1.0f - LifeSpend*0.01f));


			// 06. 03 국내무쌍 - 이영준
			if( ((CPlayer*)pObject)->IsMussangMode() )
			{
				NeedLife = (DWORD)(NeedLife * 0.95f);
			}
		}
		if(pObject->GetLife() < NeedLife)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL cActiveSkillInfo::IsInSkillRange(CObject* pObject,VECTOR3* pTargetPos,float TargetRadius)
{
	VECTOR3* p1 = CCharMove::GetPosition(pObject);
	VECTOR3* p2 = pTargetPos;
	p1->y = p2->y = 0;

	// 공격자와 맞는 넘의 타겟거리를 뺀다.
	float dist = CalcDistanceXZ(p1,p2) - TargetRadius;
	float addRange = 0.f;
	
	// 공격자가 플레이어 이면 사정거리를 300늘려준다. (오차 고려)
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		dist -= 300.f;

		CPlayer* pPlayer = ( CPlayer* )pObject;

		if( pPlayer->GetWeaponEquipType() == eWeaponType_Bow )
		{
			addRange = pPlayer->GetPassiveStatus()->Range + pPlayer->GetBuffStatus()->Range;
		}
	}

	float dis = (float)GetSkillInfo()->Range+addRange;
	
	if(dis < 0)
		dis = 0;

	if( dis < dist)	// 서버는 
		return FALSE;
		
	return TRUE;
}

ACTIVE_SKILL_INFO*	cActiveSkillInfo::GetSkillInfo()
{
	return ( ACTIVE_SKILL_INFO* )mpSkillInfo;
}
