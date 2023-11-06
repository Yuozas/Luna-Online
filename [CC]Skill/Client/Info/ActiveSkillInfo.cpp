#include "stdafx.h"
#include ".\activeskillinfo.h"

#include "ActionTarget.h"

#include "GameIn.h" 
#include "Hero.h"

#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "ChatManager.h"
#include "BattleSystem_client.h"
#include "ObjectManager.h"

#include "MussangManager.h"
#include "ShowdownManager.h"

#include "../status/status.h"
#include "../[CC]Header/GameResourceStruct.h"
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.12.03

#include "itemmanager.h"
#include "petmanager.h"

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
	//info->AreaData[ 0 ]				= atoi( strtok( 0, separator ) );
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
	// 080602 LUJ, ��ų ��� �� �Ҹ�Ǵ� ������ ���� �ε�
	info->mConsumeItem.wIconIdx		= atoi( strtok( 0, separator ) );
	info->mConsumeItem.Durability	= atoi( strtok( 0, separator ) );
	info->Money						= atoi( strtok( 0, separator ) );

	// 080616 LUJ, ���ڿ� �ڿ� %�� �پ������� �ۼ�Ʈ ������ �д´�
	{
		// 080616 LUJ, MP �ҿ� ��ġ
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

		// 080616 LUJ, HP �ҿ� ��ġ
		{
			const char* token = strtok( 0, separator );

			// 080710 LUJ, HP �Ҹ� ���� ���� �� �׻� �ۼ�Ʈ �������� �о���� ���� ����
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
	// 080310 LUJ, ��ų ���� ���� ������ �д´�
	info->RequiredPlayerLevel	= atoi( strtok( 0, separator ) );

	return TRUE;
}

BOOL cActiveSkillInfo::IsExcutableSkillState(CHero* pHero,int SkillLevel,CActionTarget* pTarget)
{	
	if( pHero->GetAbnormalStatus()->IsStun ||
		pHero->GetAbnormalStatus()->IsFreezing ||
		pHero->GetAbnormalStatus()->IsStone ||
		pHero->GetAbnormalStatus()->IsSlip ||
		pHero->GetAbnormalStatus()->IsParalysis )
	{
		return FALSE;
	}

	if( pHero->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� ���� ���� ������ ������ ��ü ��ų�� ������� ���ϵ��� �Ѵ�
		if( GetSkillInfo()->Unit != UNITKIND_PHYSIC_ATTCK )
			return FALSE;
	}
	if( pHero->GetAbnormalStatus()->IsBlockAttack )
	{
		if( GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return FALSE;
	}
/*	
	if( GetSkillInfo()->Weapon != 0 )
	{
		if( pHero->GetWeaponEquipType() == 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
			return FALSE;
		}
	}
*/	
	if( !GetSkillInfo()->Battle && pHero->GetObjectBattleState() == eObjectBattleState_Battle )
		return FALSE;

	if( !GetSkillInfo()->Peace && pHero->GetObjectBattleState() == eObjectBattleState_Peace )
		return FALSE;

	if( !GetSkillInfo()->Move && pHero->GetBaseMoveInfo()->bMoving )
		return FALSE;

	if( !GetSkillInfo()->Stop && !pHero->GetBaseMoveInfo()->bMoving )
		return FALSE;

//	if( !GetSkillInfo()->Rest )


	BOOL chk = TRUE;

	switch( GetSkillInfo()->EquipItem[0] )
	{
	case 1:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_OneHand )
			{
				chk = FALSE;
			}
		}
		break;
	case 2:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_TwoHand )
			{
				chk = FALSE;
			}
		}
		break;
	case 3:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_TwoBlade )
			{
				chk = FALSE;
			}
		}
		break;
	case 4:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_Bow )
			{
				chk = FALSE;
			}
		}
		break;
	case 5:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_Gun )
			{
				chk = FALSE;
			}
		}
		break;
	case 6:
		{
			if( pHero->GetWeaponAniType() != eWeaponAnimationType_Dagger )
			{
				chk = FALSE;
			}
		}
		break;
	case 7:
		{
		}
		break;
	case 8:
		{
			if( pHero->GetWeaponEquipType() != eWeaponType_Staff )
			{
				chk = FALSE;
			}
		}
		break;
	case 9:
		{
			if( !pHero->GetWearedItemIdx( eEquipSlot_Shield - 1 ) )
			{
				chk = FALSE;
			}
		}
		break;
	case 10:
	case 11:
	case 12:
		break;
	}
	
	if( !chk )
	{
		pHero->SetNextAction(NULL);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(770) );
		return FALSE;
	}

	if( GetSkillInfo()->RequiredBuff )
	{
		DWORD idx = GetSkillInfo()->RequiredBuff / 100;
		cSkillObject* pSkill = pHero->GetBuffSkillObject( idx );
		
		if( pSkill )
		{
			if( pSkill->GetSkillIdx() < GetSkillInfo()->RequiredBuff )
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	if( pHero->GetSkillDelay()->Check() )
	{
		CAction NextAction;
		NextAction.InitSkillAction(this,pTarget);

		pHero->SetNextAction(&NextAction);
//		pHero->SetNextAction(NULL);

		return FALSE;
	}

	if( mCoolTime.Check() )
	{
		pHero->SetNextAction(NULL);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(771) );
		return FALSE;
	}

#ifndef _TESTCLIENT_
	//////////////////////////////////////////////////////////////////////////////////
	// 06. 06. ���¼Ҹ� ����ȿ�� üũ - �̿���
	// Ŭ���̾�Ʈ�� ���¼Ҹ� ����ȿ�� üũ�� ���ϴ� ���� �߰� 
	// 080616 LUJ, �ۼ�Ʈ ������ ó���ϵ��� �Ѵ�
	DWORD NeedMana = DWORD( float( HERO->GetMana() ) * GetSkillInfo()->mMana.mPercent + GetSkillInfo()->mMana.mPlus );

	Status* pStatus = NULL ;
	pStatus = HERO->GetRateBuffStatus() ;
	
	if(NeedMana)
	{
		WORD manaSpend = HERO->GetShopItemStats()->ManaSpend + HERO->GetAvatarOption()->ManaSpend ;
			//+ HERO->GetShopItemStats()->PlustimeMana;
		NeedMana = (DWORD)(NeedMana*(1.0f - manaSpend*0.01f));

		if( pStatus->DecreaseManaRate > 0 )
		{
			float fRate = 0.0f ;
			fRate = pStatus->DecreaseManaRate/100 ;

			float fRealMana = NeedMana * fRate ;
			NeedMana -= (DWORD)fRealMana ;
		}

		//if( pHero->GetMana() < GetNeedMana(SkillLevel) )		// ���°˻�
		if( pHero->GetMana() < NeedMana )		// ���°˻�
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(656) );
			return FALSE;
		}
	}
	
	// 080616 LUJ, �ۼ�Ʈ ������ ó���ϵ��� �Ѵ�
	DWORD NeedLife = DWORD( float( HERO->GetLife() ) * GetSkillInfo()->mLife.mPercent + GetSkillInfo()->mLife.mPlus );
	
	if(NeedLife)
	{
//		WORD manaSpend = HERO->GetShopItemStats()->ManaSpend + HERO->GetAvatarOption()->ManaSpend 
//			+ HERO->GetShopItemStats()->PlustimeMana;
//		NeedMana = (DWORD)(NeedMana*(1.0f - manaSpend*0.01f));

		//if( pHero->GetMana() < GetNeedMana(SkillLevel) )		// ���°˻�
		if( pHero->GetLife() < NeedLife )		// ���°˻�
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1117) );
			return FALSE;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
#endif //_TESTCLIENT_

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanSkill &&
		pHero->GetState() != eObjectState_Society
		) 
	{

		if( pHero->GetState() != eObjectState_SkillSyn &&
			pHero->GetState() != eObjectState_SkillUsing &&
			pHero->GetState() != eObjectState_SkillBinding &&
			pHero->GetState() != eObjectState_SkillStart &&			
			pHero->GetState() != eObjectState_SkillDelay )
		{
			//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(141) );
			pHero->DisableAutoAttack();
		}

		return FALSE;
	}

	if( pHero->GetState() == eObjectState_Society )
	{
		if( pHero->EndSocietyAct() == FALSE )
		{
			pHero->DisableAutoAttack();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL cActiveSkillInfo::IsInSkillRange(CHero* pHero,CActionTarget* pTarget)
{
	VECTOR3 p1;
	pHero->GetPosition(&p1);
	VECTOR3* p2 = pTarget->GetTargetPosition();
	VECTOR3 hpos;
	pHero->GetPosition(&hpos);
	if(p2 == NULL)
		return FALSE;

	p1.y = p2->y = 0;
	
	// �����ڿ� �´� ���� Ÿ�ٰŸ��� ����.
	float dist = CalcDistanceXZ(&p1,p2) - pTarget->GetRadius();
	float addRange = 0;

	if( pHero->GetWeaponEquipType() == eWeaponType_Bow )
	{
		addRange = pHero->GetPassiveStatus()->Range + pHero->GetBuffStatus()->Range;
	}

	float dis = GetSkillInfo()->Range+addRange;
	if(dis < 0)
		dis = 0;

	if( dis < dist)
		return FALSE;
		
	return TRUE;
}

BOOL cActiveSkillInfo::IsExcutablePetSkillState(CHeroPet* pHeroPet,int SkillLevel,CActionTarget* pTarget)
{	
	BOOL chk = FALSE;
	DWORD index = 0;
	ITEM_INFO* pInfo = NULL;
	for( WORD pos = ePetEquipSlot_Skill1; pos <= ePetEquipSlot_Skill6; pos ++ )
	{
		index = pHeroPet->GetWearedItem( pos );
		pInfo = ITEMMGR->GetItemInfo( index );
		if( pInfo )
		{
			if( pInfo->SupplyType == ITEM_KIND_PET_SKILL && pInfo->SupplyValue / 100 == GetIndex() / 100 )
				chk = TRUE;
		}
	}

	if( GetIndex() == PETMGR->GetClosedSkillIndex() ||
		GetIndex() == PETMGR->GetRangeSkillIndex() )
		chk = TRUE;
		

	if( !chk )
		return FALSE;

	if( pHeroPet->GetAbnormalStatus()->IsStun ||
		pHeroPet->GetAbnormalStatus()->IsFreezing ||
		pHeroPet->GetAbnormalStatus()->IsStone ||
		pHeroPet->GetAbnormalStatus()->IsSlip ||
		pHeroPet->GetAbnormalStatus()->IsParalysis )
	{
		return FALSE;
	}

	if( pHeroPet->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� ���� ���� ������ ������ ��ü ��ų�� ������� ���ϵ��� �Ѵ�
		if( GetSkillInfo()->Unit != UNITKIND_PHYSIC_ATTCK )
			return FALSE;
	}
	if( pHeroPet->GetAbnormalStatus()->IsBlockAttack )
	{
		if( GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return FALSE;
	}
/*	
	if( GetSkillInfo()->Weapon != 0 )
	{
		if( pHero->GetWeaponEquipType() == 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
			return FALSE;
		}
	}
*/	
	if( !GetSkillInfo()->Battle && pHeroPet->GetObjectBattleState() == eObjectBattleState_Battle )
		return FALSE;

	if( !GetSkillInfo()->Peace && pHeroPet->GetObjectBattleState() == eObjectBattleState_Peace )
		return FALSE;

	if( !GetSkillInfo()->Move && pHeroPet->GetBaseMoveInfo()->bMoving )
		return FALSE;

	if( !GetSkillInfo()->Stop && !pHeroPet->GetBaseMoveInfo()->bMoving )
		return FALSE;


	if( pHeroPet->GetSkillDelay()->Check() )
	{
		CAction NextAction;
		NextAction.InitSkillAction(this,pTarget);

		pHeroPet->SetNextAction(&NextAction);

		return FALSE;
	}

	if( mPetCoolTime.Check() )
	{
		pHeroPet->SetNextAction(NULL);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(771) );
		return FALSE;
	}

	if( pHeroPet->GetState() != eObjectState_None &&
		pHeroPet->GetState() != eObjectState_Move &&
		pHeroPet->GetState() != eObjectState_TiedUp_CanSkill &&
		pHeroPet->GetState() != eObjectState_Society
		) 
	{

		if( pHeroPet->GetState() != eObjectState_SkillSyn &&
			pHeroPet->GetState() != eObjectState_SkillUsing &&
			pHeroPet->GetState() != eObjectState_SkillBinding &&
			pHeroPet->GetState() != eObjectState_SkillStart &&			
			pHeroPet->GetState() != eObjectState_SkillDelay )
		{
			pHeroPet->DisableAutoAttack();
		}

		return FALSE;
	}

	return TRUE;
}

BOOL cActiveSkillInfo::IsInPetSkillRange(CHeroPet* pHeroPet,CActionTarget* pTarget)
{
	VECTOR3 p1;
	pHeroPet->GetPosition(&p1);
	VECTOR3* p2 = pTarget->GetTargetPosition();
	VECTOR3 hpos;
	pHeroPet->GetPosition(&hpos);
	if(p2 == NULL)
		return FALSE;

	p1.y = p2->y = 0;
	
	// �����ڿ� �´� ���� Ÿ�ٰŸ��� ����.
	float dist = CalcDistanceXZ(&p1,p2) - pTarget->GetRadius();

	float dis = GetSkillInfo()->Range;
	if(dis < 0)
		dis = 0;

	if( dis < dist)
		return FALSE;
		
	return TRUE;
}

ACTIVE_SKILL_INFO*	cActiveSkillInfo::GetSkillInfo()
{
	return ( ACTIVE_SKILL_INFO* )mpSkillInfo;
}

BOOL cActiveSkillInfo::IsValidTarget(CHero* pHero,CActionTarget* pTarget)
{
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pHero);
	CObject* pTargetObj = NULL;
	if(pTarget->GetTargetID() != 0)
		pTargetObj = OBJECTMGR->GetObject(pTarget->GetTargetID());

	if(pTargetObj == NULL)
		return TRUE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Npc )
		return FALSE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Monster )
	{
		if( ( ( CMonster* )pTargetObj )->GetSInfo()->SpecialType == 2 )
			return FALSE;
	}

	if( !GetSkillInfo()->Area )
	{
		switch( GetSkillInfo()->Unit )
		{
		case UNITKIND_PHYSIC_ATTCK:
		case UNITKIND_MAGIC_ATTCK:
		case UNITKIND_FAKE_DAMAGE:
		case UNITKIND_DECOY:
			{
				if( pBattle->IsEnemy(pHero,pTargetObj) == FALSE )
					return FALSE;
			}	
			break;
		// desc_hseos_����Ʈ ��_01
		// S ����Ʈ �� �߰� added by hseos 2008.01.16
		// ..ç���� �������� ��Ȱ �Ұ�
		/*
		case UNITKIND_LIFE_RECOVER:
		case UNITKIND_RESURRECTION:
			{
				if( pBattle->IsEnemy(pHero,pTargetObj) == TRUE )
					return FALSE;
			}
			break;
		*/
		case UNITKIND_LIFE_RECOVER:
			{
				if( pBattle->IsEnemy(pHero,pTargetObj) == TRUE )
					return FALSE;
			}
			break;
		case UNITKIND_RESURRECTION:
			{
				if( pBattle->IsEnemy(pHero,pTargetObj) == TRUE )
					return FALSE;
				if (g_csDateManager.IsChallengeZoneHere())
				{
					return FALSE;
				}
				// 080317 LUJ, �� ���� �� ����� �� ����
				else if( SHOWDOWNMGR->IsShowdown() )
				{
					return FALSE;
				}
			}
			break;
		// E ����Ʈ �� �߰� added by hseos 2008.01.16
		}
	}
	else
	{
		switch( GetSkillInfo()->Target )
		{
		case eSkillTargetKind_OnlyOthers:
		case eSkillTargetKind_AllObject:
			{
				switch( GetSkillInfo()->Unit )
				{
				case UNITKIND_PHYSIC_ATTCK:
				case UNITKIND_MAGIC_ATTCK:
				case UNITKIND_FAKE_DAMAGE:
				case UNITKIND_DECOY:
					{
						if( pBattle->IsEnemy(pHero,pTargetObj) == FALSE )
							return FALSE;
					}	
					break;
				case UNITKIND_LIFE_RECOVER:
				case UNITKIND_RESURRECTION:
					{
						if( pBattle->IsEnemy(pHero,pTargetObj) == TRUE )
							return FALSE;
						// desc_hseos_����Ʈ ��_01
						// S ����Ʈ �� �߰� added by hseos 2007.12.03
						// ..ç���� �������� ��Ȱ �Ұ�
						if (g_csDateManager.IsChallengeZoneHere())
						{
							return FALSE;
						}
						// E ����Ʈ �� �߰� added by hseos 2007.12.03
						// 080317 LUJ, �� ���� �� ����� �� ����
						else if( SHOWDOWNMGR->IsShowdown() )
						{
							return FALSE;
						}
					}
					break;
				}
			}
			break;
		}
	}

	return TRUE;
}

void cActiveSkillInfo::SetCooling()
{
	mCoolTime.Init( GetSkillInfo()->CoolTime );
	mCoolTime.Start();
}

void cActiveSkillInfo::SetPetCooling()
{
	mPetCoolTime.Init( GetSkillInfo()->CoolTime );
	mPetCoolTime.Start();
}



//=========================================================================
//	NAME : GetBuffSkillIdx
//	DESC : 071228 LYW
//		 : ���� �ε����� ��ȯ�ϴ� �Լ� �߰�.
//=========================================================================
DWORD cActiveSkillInfo::GetBuffSkillIdx(int index)
{
	DWORD dwBuffSkillIdx = 0 ;

	if( index < 0 || index >= MAX_BUFF_COUNT )
	{
		return dwBuffSkillIdx ;
	}

	return ((ACTIVE_SKILL_INFO*)mpSkillInfo)->Buff[index] ;
}
