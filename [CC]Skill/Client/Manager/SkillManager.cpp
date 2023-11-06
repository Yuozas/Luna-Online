#include "stdafx.h"
#include ".\skillmanager.h"

#include "../Info/SkillInfo.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Info/BuffSkillInfo.h"
#include "../Object/SkillObject.h"
#include "../Object/BuffSkillObject.h"

#include "ActionTarget.h"
#include "Hero.h"
#include "MoveManager.h"
#include "QuickManager.h"
#include "Gamein.h"
#include "cQuickItem.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "ExchangeManager.h"

#include "GameResourceManager.h"

#include "PKManager.h"
#include "ObjectActionManager.h"
#include "BattleSystem_Client.h"
#include "../../[CC]BattleSystem/GTournament/Battle_GTournament.h"

#include "PeaceWarModeManager.h"

#include "MAINGAME.h"

#include "PeaceWarModeManager.h"
#include "InventoryExDialog.h"
#include "Item.h"
#include "PartyWar.h"

#include "../delay/timedelay.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.12.03
#include "petmanager.h"
// 080602 LUJ, 스킬 사용 시 소모 기능위해 참조
#include "GuildManager.h"
#include "ItemManager.h"
#include "CharacterDialog.h"
#include "FamilyDialog.h"
#include "petmanager.h"

cSkillManager::cSkillManager(void)
{
	mpSkillInfo = NULL;

	Init();
}

cSkillManager::~cSkillManager(void)
{
	Release();
}

void cSkillManager::Init()
{
	m_SkillInfoTable.Initialize(64);
	m_SkillObjectTable.Initialize(64);
	m_SkillTreeTable.Initialize(64);
	LoadSkillInfoList();
}
void cSkillManager::Release()
{
	cSkillInfo* pSInfo = NULL;

	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();
	cSkillObject* pSObj = NULL;

	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		delete pSObj;
	}
	m_SkillObjectTable.RemoveAll();

	SKILL_TREE_INFO* pInfo = NULL;

	m_SkillTreeTable.SetPositionHead();
	while(pInfo = m_SkillTreeTable.GetData())
	{
		delete pInfo;
	}
	m_SkillTreeTable.RemoveAll();
}

void cSkillManager::ReleaseAllSkillObject()
{
	cSkillObject* pSObj = NULL;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		delete pSObj;
	}
	m_SkillObjectTable.RemoveAll();
}

void cSkillManager::LoadSkillInfoList()
{
	CMHFile file;
	file.Init("System/Resource/SkillList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		cSkillInfo* pInfo = ( cSkillInfo* )( new cActiveSkillInfo );
		pInfo->InitSkillInfo(&file);

		ASSERT(m_SkillInfoTable.GetData(pInfo->GetIndex()) == NULL);
		m_SkillInfoTable.Add(pInfo,pInfo->GetIndex());

		{
			const DWORD index = pInfo->GetIndex() / 100 * 100 + 1;

			++mSkillSize[ index ];
		}
	}

	file.Release();

	file.Init("System/Resource/Skill_Buff_List.bin","rb");
	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		cSkillInfo* pInfo = ( cSkillInfo* )( new cBuffSkillInfo );
		pInfo->InitSkillInfo(&file);

		ASSERT(m_SkillInfoTable.GetData(pInfo->GetIndex()) == NULL);
		m_SkillInfoTable.Add(pInfo,pInfo->GetIndex());

		{
			const DWORD index = pInfo->GetIndex() / 100 * 100 + 1;

			++mSkillSize[ index ];
		}
	}

	file.Release();

	file.Init("System/Resource/skill_get_list.bin","rb");

	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SKILL_TREE_INFO* pInfo = new SKILL_TREE_INFO;

		pInfo->ClassIndex = file.GetWord();
		pInfo->Count = file.GetWord();

		for( WORD cnt = 0; cnt < pInfo->Count; cnt++ )
		{
			pInfo->SkillIndex[ cnt ] = file.GetDword();
		}

		m_SkillTreeTable.Add( pInfo, pInfo->ClassIndex );
	}

	file.Release();

}

cSkillInfo*	cSkillManager::GetSkillInfo(DWORD SkillInfoIdx)
{
	return (cSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

SKILL_TREE_INFO* cSkillManager::GetSkillTreeInfo( WORD ClassIndex )
{
	return (SKILL_TREE_INFO*)m_SkillTreeTable.GetData( ClassIndex );
}

DWORD cSkillManager::GetSkillTooltipInfo(DWORD SkillInfoIdx)
{
	cSkillInfo* pInfo = m_SkillInfoTable.GetData( SkillInfoIdx );

	if(!pInfo)
		return 0;

	if( pInfo->GetKind() <= SKILLKIND_ONOFF )
	{
		return pInfo->GetTooltip();
	}
	else
	{
		return 0;
	}
}

cSkillObject* cSkillManager::GetSkillObject(DWORD SkillObjectID)
{
	return (cSkillObject*)m_SkillObjectTable.GetData(SkillObjectID);
}

DWORD cSkillManager::GetComboSkillIdx(CHero* pHero)
{
	DWORD SkillNum = 1000000;

	CHARACTER_TOTALINFO* pInfo = pHero->GetCharacterTotalInfo();

	int WeaponAniType = pHero->GetWeaponAniType();
	int CurComboNum = pHero->GetCurComboNum();
	int Skill = 0;

	if( CurComboNum == MAX_COMBO_NUM )
	{
		CurComboNum = 0;
		pHero->SetCurComboNum(0);
	}

	switch( WeaponAniType )
	{
	case eWeaponAnimationType_None:			Skill = 1;	break;
	case eWeaponAnimationType_OneHand:		Skill = 3;	break;
	case eWeaponAnimationType_TwoHand:		Skill = 2;	break;
	case eWeaponAnimationType_Dagger:		Skill = 4;	break;
	case eWeaponAnimationType_Staff:		Skill = 6;	break;
	case eWeaponAnimationType_Bow:			Skill = 5;	break;
		// 080710 LUJ, 총기 기본 스킬 등록
	case eWeaponAnimationType_Gun:			Skill = 7;	break;
		// 080703 LUJ, 이도류는 단검/한손무기가 따로 애니메이션이 있다.
	case eWeaponAnimationType_TwoBlade:
		{
			Skill = ( eWeaponType_Dagger == pHero->GetWeaponEquipType() ? 8 : 10 );
			break;
		}
	}
	

	SkillNum = SkillNum + ( ( Skill ) * 100 ) + CurComboNum + 1;

	return SkillNum;
}


// 스킬을 사용
BOOL cSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bSkill)
{
	DWORD SkillNum;
	cSkillInfo* pSkillInfo;

	/// 스킬이 기본공격일 경우
	if(bSkill == FALSE)
	{
		/// 기본공격 정보를 가져온다
		SkillNum = GetComboSkillIdx(pHero);
		pSkillInfo = GetSkillInfo(SkillNum);
		mpSkillInfo = pSkillInfo;
	}
	else
		pSkillInfo = mpSkillInfo;
		
	/// 스킬 정보가 없다면 실패
	if(pSkillInfo == NULL)
		return FALSE;

	/// 스킬을 실행한다
	return ExcuteSkillSYN(pHero,pTarget,pSkillInfo);
}

BOOL cSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	
	/// 스킬 정보가 없으면 실패
	if(pInfo == NULL)
		return FALSE;

//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// 공격 가능 대상인지 판단
	if(pSkillInfo->IsValidTarget( pHero, pTarget ) == FALSE )
	{
		return FALSE;
	}

	/// 스킬 레벨을 가져온다
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// 실행 가능한지 판단
	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,&target) == FALSE)
	{
		/// 기본 공격으로 전환하고 스킬 사용실패 한다
		//pHero->SetCurComboNum(0);
		//printf( "%d\t%s\n", 0, "cSkillManager::ExcuteSkillSYN1", 0 );
		return FALSE;
	}

	/// 타겟 설정 - 수정해야 함

	/// 평화모드일 경우
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	/*
	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)
		/// 전투모드로 바꾼다
		PEACEWARMGR->ToggleHeroPeace_WarMode();
		*/
	
	/// 타겟의 위치값이 없으면 실패
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	/// 사정거리 판단
	if(pSkillInfo->IsInSkillRange(pHero,&target) == TRUE)
	/// 사정거리 안쪽이면
	{	
		/// 이동 중지
		MOVEMGR->HeroMoveStop();
		
		/// 자신이 타겟일 경우에는 보는 방향을 바꾸지 않는다.
		if( pSkillInfo->GetSkillInfo()->Target != 1 )
			MOVEMGR->SetLookatPos(pHero,pTargetPos,0,gCurTime);
				
		/// 이동 행동을 초기화
		pHero->SetMovingAction(NULL);

		/// 스킬 사용
		return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
	}
	else
	/// 사정거리 밖이면
	{	
		/// 기본공격 초기화
		pHero->SetCurComboNum(0);
		
		/// 스킬 사용을 위한 이동설정
		CAction MoveAction;
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroActionMove(&target,&MoveAction);
		return FALSE;
	}

	return TRUE;
}

BOOL cSkillManager::RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	/// 스킬 정보가 없으면 실패
	if(pInfo == NULL)
		return FALSE;
	
//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// 스킬 레벨을 가져온다
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// 실행 가능한지 판단
	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pTarget) == FALSE)
	{
		/// 기본 공격으로 전환하고 스킬 사용실패 한다
		pHero->SetCurComboNum(0);
		return FALSE;
	}

	// 080602 LUJ, 소모 아이템 정보가 담긴다
	ITEMBASE consumeItem;
	ZeroMemory( &consumeItem, sizeof( consumeItem ) );

	// 080602 LUJ, 소모 포인트/아이템이 충분한지 검사
	{
		const ACTIVE_SKILL_INFO*	info	= pSkillInfo->GetSkillInfo();
		CHero*						hero	= OBJECTMGR->GetHero();

		if( ! info ||
			! hero )
		{
			return FALSE;
		}

		// 080602 LUJ, 포인트 검사
		{
			const ACTIVE_SKILL_INFO::ConsumePoint& point = info->mConsumePoint;

			switch( point.mType )
			{
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
				{
					CSHFamily* family = hero->GetFamily();

					if( ! family )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 828 ) );
						return FALSE;
					}

					const CSHFamily::stINFO_EX* info = family->GetEx();

					if( !	info	||
							info->nHonorPoint < point.mValue )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1568 ),
							point.mValue - info->nHonorPoint );
						return FALSE;
					}
					
					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
				{
					if( ! hero->GetGuildIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 939 ) );
						return FALSE;
					}
					else if( GUILDMGR->GetScore() < point.mValue )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1566 ),
							point.mValue - GUILDMGR->GetScore() );
						return FALSE;
					}

					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
				{
					if( hero->GetExpPoint() < EXPTYPE( point.mValue ) )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1567 ),
							EXPTYPE( point.mValue ) - hero->GetExpPoint() );
						return FALSE;
					}

					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
				{
					if( hero->GetBadFame() < FAMETYPE( point.mValue ) )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1565 ),
							FAMETYPE( point.mValue ) - hero->GetBadFame() );
						return FALSE;
					}

					break;
				}
			}
		}

		// 080602 LUJ, 소지금 검사
		if( info->Money > hero->GetMoney() )
		{
			CHATMGR->AddMsg(
				CTC_SYSMSG,
				CHATMGR->GetChatMsg( 1333 ),
				info->Money - hero->GetMoney() );

			return FALSE;
		}
		// 080602 LUJ, 아이템 검사
		else if( info->mConsumeItem.wIconIdx )
		{
			CInventoryExDialog* inventoryDialog	= GAMEIN->GetInventoryDialog();

			if( ! inventoryDialog )
			{
				return FALSE;
			}

			const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
			const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount();

			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				CItem* item = inventoryDialog->GetItemForPos( position );

				if( !	item													||
						item->GetItemIdx()		!= info->mConsumeItem.wIconIdx	||
					(	ITEMMGR->IsDupItem( item->GetItemIdx() )	&&
						item->GetDurability()	< info->mConsumeItem.Durability ) )
				{
					continue;
				}

				consumeItem = item->GetItemBaseInfo();
				break;
			}

			// 080602 LUJ, 소모할 아이템을 찾지 못함
			if( ! consumeItem.wIconIdx )
			{
				const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( info->mConsumeItem.wIconIdx );

				if( ! itemInfo )
				{
					return FALSE;
				}

				CHATMGR->AddMsg(
					CTC_SYSMSG,
					CHATMGR->GetChatMsg( 1543 ),
					itemInfo->ItemName, info->mConsumeItem.Durability );
				
				return FALSE;
			}
		}
	}

	/// 효과발동 딜레이가 없는 스킬은 클라이언트에서 바로 타겟리스트 작성
	//if( pSkillInfo->GetSkillInfo()->DelayType == 0 )
	{
		VECTOR3* pTargetPos = pTarget->GetTargetPosition();
		if(pTargetPos == NULL)
			return FALSE;

		switch( pSkillInfo->GetSkillInfo()->Area )
		{
		case 1:
			{
				pTarget->SetTargetObjectsInRange( pTargetPos, pSkillInfo->GetSkillInfo()->AreaData, pSkillInfo->GetSkillInfo()->AreaTarget );
			}
			break;
		}
	}

	float AttackSpeedRate = 1.0f;

	DWORD idx = pInfo->GetIndex();

	if( (idx / 100000) % 10 )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
		{
			AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->PhysicSkillSpeedRate + HERO->GetRateBuffStatus()->PhysicSkillSpeedRate ) * 0.01f );
		}
		else if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_MAGIC_ATTCK )
		{
			AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->MagicSkillSpeedRate + HERO->GetRateBuffStatus()->MagicSkillSpeedRate ) * 0.01f );
		}
	}
	else
	{
		AttackSpeedRate += ( ( HERO->GetRatePassiveStatus()->NormalSpeedRate + HERO->GetRateBuffStatus()->NormalSpeedRate ) * 0.01f );
	}
	
	if( AttackSpeedRate < 0 )
	{
		AttackSpeedRate = 1.0f;
	}

	DWORD time = pSkillInfo->GetSkillInfo()->AnimationTime / AttackSpeedRate;
	DWORD CastingTime = pSkillInfo->GetSkillInfo()->CastingTime / AttackSpeedRate;
	HERO->GetSkillDelay()->Init( time );
	HERO->GetSkillDelay()->Start();

	HERO->StartSkillProgressBar( CastingTime );

	/// 다음 행동 초기화
	HERO->SetNextAction(NULL);

	/// 서버에 스킬 생성 요청을 한다
	// 080602 LUJ, 소모 아이템 정보를 추가한다
	pSkillInfo->SendMsgToServer( pHero, pTarget, &consumeItem );
	pSkillInfo->SetCooling();

	// 080602 LUJ, 소모될 아이템을 잠금 처리한다
	{
		CItem* item = ITEMMGR->GetItem( consumeItem.dwDBIdx );

		if( item )
		{
			item->SetLock( TRUE );
		}
	}

	/// 임시 스킬 객체 생성
	DoCreateTempSkillObject( pSkillInfo, pTarget );

	/// 기본공격 설정
	//pHero->SetCurComboNum(0);
	if( !( ( pInfo->GetIndex() / 100000 ) % 10 ) )
	{
		pHero->SetCurComboNum( pHero->GetCurComboNum() + 1 );
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(777), pSkillInfo->GetName() );
		pHero->SetCurComboNum(0);
	}

	if( ( ( pInfo->GetIndex() / 100000 ) % 10 ) == 2 )
	{
		//HERO->DisableAutoAttack();
	}
	//OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillStart);

	/// 스킬 사용 정보 초기화
	mpSkillInfo = NULL;
	
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	//PEACEWARMGR->SetCheckTime(gCurTime);

#ifdef _TESTCLIENT_
	static IDDDD = 0;
	
	MSG_SKILLOBJECT_ADD2 msg;
	
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_SKILLOBJECT_ADD;
	msg.bCreate = TRUE;

	msg.SkillObjectInfo.SkillObjectIdx = SKILLOBJECT_ID_START+IDDDD++;
	msg.SkillObjectInfo.SkillIdx = pSkillInfo->GetIndex();

	if( pSkillInfo->GetSkillInfo()->Target )
	{
		msg.SkillObjectInfo.Pos = *pTarget->GetTargetPosition();
	}
	else
	{
		HERO->GetPosition( &msg.SkillObjectInfo.Pos );
	}

	msg.SkillObjectInfo.StartTime = 0;
	msg.SkillObjectInfo.Direction = DEGTODIR( HERO->GetAngleDeg() );
	msg.SkillObjectInfo.SkillLevel = ( BYTE )SkillLevel;
	msg.SkillObjectInfo.Operator = HERO->GetID();

	pTarget->GetMainTarget( &msg.SkillObjectInfo.MainTarget );

	CBattle* pBattle = BATTLESYSTEM->GetBattle(HERO);

	msg.SkillObjectInfo.BattleID = pBattle->GetBattleID();
	msg.SkillObjectInfo.BattleKind = pBattle->GetBattleKind();

	NetworkMsgParse( MP_SKILL_SKILLOBJECT_ADD, &msg );

	MSG_SKILL_RESULT msg2;

	msg2.InitMsg( msg.SkillObjectInfo.SkillObjectIdx );
	msg2.SkillDamageKind = false;
	CTargetListIterator iter(&msg2.TargetList);
	
	CObject* pObject = OBJECTMGR->GetObject( msg.SkillObjectInfo.MainTarget.dwMainTargetID );
	
	if( pObject )
	{
		RESULTINFO dinfo;
		dinfo.Clear();
		dinfo.bCritical = 1;//rand()%5 ? FALSE : TRUE;
		dinfo.RealDamage = rand() % 50;
		dinfo.CounterDamage = 0;
		dinfo.StunTime = 1000;//(rand() % 30 == 0) ? 2000 : 0;
		iter.AddTargetWithResultInfo(pObject->GetID(),1,&dinfo);
	}
	iter.Release();

	NetworkMsgParse( MP_SKILL_SKILL_RESULT, &msg2 );
#endif

	return TRUE;
}

BOOL cSkillManager::OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bSkill)
{
	//////////////////////////////////////////////////////////////////////////
	// 죽은 사람은 공격할수 없음
	//if(pTarget->GetTargetID() != 0)
	//{
	//	CObject* pTObj = OBJECTMGR->GetObject(pTarget->GetTargetID());
	//	if(pTObj == NULL)
	//		return FALSE;
	//	if(pTObj->IsDied() == TRUE)
	//		return FALSE;
	//}
	//////////////////////////////////////////////////////////////////////////
	

	// Guild Tournament나 공성전에서 Observer이면 사용불가
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pHero->GetBattleTeam() == 2 )
			return FALSE;	
	}

//---KES 따라가기
	pHero->SetFollowPlayer( 0 );
//---------------

	if( pHero->GetState() == eObjectState_SkillSyn ||
		pHero->GetState() == eObjectState_SkillUsing)
		SetNextComboSkill(pHero,pTarget,bSkill);
	else
		ExcuteSkillSYN(pHero,pTarget,bSkill);		//return FALSE 처리....없다.. 괜찮을까?

	return TRUE;
}

BOOL cSkillManager::OnSkillCommand( DWORD idx )
{
	CActionTarget Target;

	cSkillInfo* pSkillInfo = m_SkillInfoTable.GetData( idx );
	if( !pSkillInfo )
	{
		return FALSE;
	}


	WORD skillkind = pSkillInfo->GetKind();
	if( skillkind == SKILLKIND_PASSIVE || skillkind == SKILLKIND_BUFF )
	{
		return FALSE;
	}

	cActiveSkillInfo* pInfo = ( cActiveSkillInfo* )pSkillInfo;

	// 지면 타겟 여부를 체크해야 한다.

	switch( pInfo->GetSkillInfo()->Target )
	{
	case eSkillTargetKind_None:
		{
			return FALSE;
		}
		break;
	case eSkillTargetKind_OnlyOthers:
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();

			if( targetObj == HERO )
			{
				return FALSE;
			}

			if( targetObj )
			{
				//---KES PK
				if( targetObj->GetObjectKind() == eObjectKind_Player )
				{
					PKMGR->SetPKStartTimeReset();
				}
				//---------

				if( pInfo->GetSkillInfo()->Unit == UNITKIND_RESURRECTION )
				{
					if( targetObj->GetObjectKind() != eObjectKind_Player)
					{
						return FALSE;
					}

					if( !targetObj->IsDied() || targetObj->GetState() != eObjectState_Die )
					{
						return FALSE;
					}

					// desc_hseos_데이트 존_01
					// S 데이트 존 추가 added by hseos 2007.12.03
					// ..챌린지 존에서는 부활 불가
					if (g_csDateManager.IsChallengeZoneHere())
					{
						return FALSE;
					}
					// E 데이트 존 추가 added by hseos 2007.12.03

					Target.InitActionTarget(targetObj,NULL);
				}
				else
				{
					if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
					{
						return FALSE;
					}

					Target.InitActionTarget(targetObj,NULL);
					HERO->EnableAutoAttack(&Target);
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655) );
				return FALSE;
			}
		}
		break;
	case eSkillTargetKind_Ground:
		{
		}
		break;
	case eSkillTargetKind_OnlySelf:
		{
			Target.InitActionTarget(HERO,NULL);	
		}
		break;
	case eSkillTargetKind_AllObject:
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();

			if( targetObj )
			{
				if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
				{
					return FALSE;
				}

				Target.InitActionTarget(targetObj,NULL);
				HERO->EnableAutoAttack(&Target);
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655) );
				return FALSE;
			}	
		}
		break;
	}

	mpSkillInfo = pSkillInfo;	

	if( idx )
	{
		return OnSkillCommand( HERO, &Target, TRUE );
	}
	else
	{
		return OnSkillCommand( HERO, &Target, FALSE );
	}
}

void cSkillManager::SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bSkill)
{
	cSkillInfo* pNextSkill;
	DWORD NextSkillIdx;
	/*
	if(bMugong)
	NextSkillIdx = GetMugongSkillIdx(pHero);
	else
	NextSkillIdx = GetComboSkillIdx(pHero);
	pNextSkill = GetSkillInfo(NextSkillIdx);
	*/ //GetMugongSkillIdx() 함수를 사용하지 않게 되어 수정
	if(bSkill)
		pNextSkill = mpSkillInfo;
	else
	{
		NextSkillIdx = GetComboSkillIdx(pHero);
		pNextSkill = GetSkillInfo(NextSkillIdx);
	}

	if(pNextSkill == NULL)
		return;
	CAction act;
	act.InitSkillAction( ( cActiveSkillInfo* )pNextSkill, pTarget );
	pHero->SetNextAction(&act);
}

void cSkillManager::OnPetComboTurningPoint(CHeroPet* pHeroPet)
{
	if(pHeroPet->GetNextAction()->HasAction())
	{
		pHeroPet->GetNextAction()->ExcutePetAction(pHeroPet);
		pHeroPet->GetNextAction()->Clear();
	}
	else
	{		
		if(pHeroPet->IsAutoAttacking())
		{
			if(OnPetSkillCommand(pHeroPet,pHeroPet->GetAutoAttackTarget(),FALSE) == FALSE)
				pHeroPet->DisableAutoAttack();
		}
	}
}

void cSkillManager::OnComboTurningPoint(CHero* pHero)
{
	if(pHero->GetNextAction()->HasAction())
	{
		if(pHero->GetNextAction()->GetActionKind() != eActionKind_Skill)
		{
			pHero->SetCurComboNum(0);
		}

		pHero->GetNextAction()->ExcuteAction(pHero);
		pHero->GetNextAction()->Clear();
	}
	else
	{		
		if(pHero->IsAutoAttacking())
		{
			if(pHero->GetCurComboNum() < 3)	// 자동공격은 콤보 2까지만	12/3일 회의 결과 3에서 2로 바뀜
			{
				if(OnSkillCommand(pHero,pHero->GetAutoAttackTarget(),FALSE) == FALSE)
					pHero->DisableAutoAttack();
			}
			else
			{
				OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillDelay);
				OBJECTSTATEMGR->EndObjectState(pHero,eObjectState_SkillDelay,300);
				pHero->SetCurComboNum(0);
			}
		}
	}
}

void cSkillManager::DoCreateSkillObject(cSkillObject* pSObj,SKILLOBJECT_INFO* pSOInfo)
{
	pSObj->Init( pSOInfo );
	
	cSkillObject* pPreObj = m_SkillObjectTable.GetData(pSObj->GetID());
	//ASSERT(pPreObj == NULL);	
	if(pPreObj != NULL)
	{
		ReleaseSkillObject(pPreObj);
	}

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
//	OBJECTMGR->AddSkillObject(pSObj);
}

void cSkillManager::DoCreateTempSkillObject(cActiveSkillInfo* pSkillInfo, CActionTarget* pTarget)
{
	/// 임시 스킬객체 생성
	
	/// 정보 셋팅
	SKILLOBJECT_INFO SkillObjectInfo;

	SkillObjectInfo.SkillObjectIdx = TEMP_SKILLOBJECT_ID;
	SkillObjectInfo.SkillIdx = pSkillInfo->GetIndex();

	if( pSkillInfo->GetSkillInfo()->Target )
	{
		SkillObjectInfo.Pos = *pTarget->GetTargetPosition();
	}
	else
	{
		HERO->GetPosition( &SkillObjectInfo.Pos );
	}

	SkillObjectInfo.StartTime = 0;
	SkillObjectInfo.Direction = DEGTODIR( HERO->GetAngleDeg() );
	SkillObjectInfo.SkillLevel = ( BYTE )( pSkillInfo->GetLevel() );
	SkillObjectInfo.Operator = HERO->GetID();

	pTarget->GetMainTarget( &SkillObjectInfo.MainTarget );

	CBattle* pBattle = BATTLESYSTEM->GetBattle(HERO);

	SkillObjectInfo.BattleID = pBattle->GetBattleID();

	float AttackSpeedRate = 1.0f;

	if( pSkillInfo->GetKind() == SKILLKIND_PHYSIC ||
		pSkillInfo->GetKind() == SKILLKIND_MAGIC )
	{
		DWORD idx = pSkillInfo->GetIndex();

		if( (idx / 100000) % 10 )
		{
			if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			{
				AttackSpeedRate += ( ( HERO->GetRateBuffStatus()->PhysicSkillSpeedRate + HERO->GetRatePassiveStatus()->PhysicSkillSpeedRate ) * 0.01f );
			}
			else if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_MAGIC_ATTCK )
			{
				AttackSpeedRate += ( ( HERO->GetRateBuffStatus()->MagicSkillSpeedRate + HERO->GetRatePassiveStatus()->MagicSkillSpeedRate ) * 0.01f );
			}
		}
		else
		{
			AttackSpeedRate += ( ( HERO->GetRateBuffStatus()->NormalSpeedRate + HERO->GetRatePassiveStatus()->NormalSpeedRate ) * 0.01f );
		}
	}

	SkillObjectInfo.Rate = AttackSpeedRate;

	/// 객체 생성
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 방향 전환
	VECTOR3 MainTargetPos;
	GetMainTargetPos(&SkillObjectInfo.MainTarget,&MainTargetPos,NULL);
	MOVEMGR->SetLookatPos(HERO,&MainTargetPos,0,gCurTime);

	DoCreateSkillObject( pSObj, &SkillObjectInfo );

	cTimeDelay* pDelay = HERO->GetSkillCancelDelay();

	pDelay->Init( pSkillInfo->GetSkillInfo()->CastingTime );
	pDelay->Start();

	HERO->SetCurrentSkill( pSObj );

	if( HEROPET )
	{
		HEROPET->SetTarget( SkillObjectInfo.MainTarget.dwMainTargetID );
	}
}

BOOL cSkillManager::DoChangeTempSkillObject(SKILLOBJECT_INFO* pSOInfo)
{
	/// 임시 객체를 교체해준다
	cSkillInfo* pSInfo = GetSkillInfo( pSOInfo->SkillIdx );

	/// 패시브와 버프 스킬은 임시 객체를 만들리 없다.
	if( pSInfo->GetKind() == SKILLKIND_PASSIVE ||
		pSInfo->GetKind() == SKILLKIND_BUFF )
	{
		return FALSE;
	}

	/// 임시 객체를 가져온다.
	cSkillObject* pTempObj = m_SkillObjectTable.GetData( TEMP_SKILLOBJECT_ID );

	/// 임시 객체가 없다...
	if( !pTempObj )
	{
		return FALSE;
	}

	/// 스킬 인덱스가 다르다!
	if( pTempObj->GetSkillIdx() != pSOInfo->SkillIdx )
	{
		return FALSE;
	}

	/// 스킬 오브젝트 테이블에서 임시 객체를 지운다.
	m_SkillObjectTable.Remove( TEMP_SKILLOBJECT_ID );
	
	/// ID 재설정
	pTempObj->ResetSkillObjectId( pSOInfo->SkillObjectIdx );

	/// 다시 테이블에 넣는다.
	m_SkillObjectTable.Add( pTempObj, pTempObj->GetID() );

	return TRUE;
}

void cSkillManager::ReleaseSkillObject(cSkillObject* pSkillObject)
{
	cSkillInfo* pSInfo = GetSkillInfo(pSkillObject->GetSkillIdx());
	if(pSInfo == NULL)
	{
		ASSERT(0);
		return;
	}
	//ASSERT(pSInfo);

	m_SkillObjectTable.Remove(pSkillObject->GetID());
	OBJECTMGR->AddGarbageObject(pSkillObject);
	
	pSkillObject->ReleaseSkillObject();
	pSInfo->ReleaseSkillObject(pSkillObject);
}
void cSkillManager::OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD2* pmsg)
{
	CObject* pOperator = OBJECTMGR->GetObject(pmsg->SkillObjectInfo.Operator);

	if(pOperator == NULL)
	{
		return;
	}

	DWORD SkillIdx = pmsg->SkillObjectInfo.SkillIdx;
	cSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);

	BOOL rt = FALSE;

	if( pOperator == HERO && pmsg->bCreate )
	{
		rt = DoChangeTempSkillObject( &pmsg->SkillObjectInfo );
	}

	if( !rt )
	{
		if( pSkillInfo )
		{
			cSkillObject* pSObj = pSkillInfo->GetSkillObject();

			VECTOR3 MainTargetPos;
			GetMainTargetPos(&pmsg->SkillObjectInfo.MainTarget,&MainTargetPos,NULL);
			MOVEMGR->SetLookatPos(pOperator,&MainTargetPos,0,gCurTime);

			DoCreateSkillObject(pSObj,&pmsg->SkillObjectInfo);
		}
		else
		{
			ASSERT( 0 );
		}		
	}
}
void cSkillManager::OnReceiveSkillObjectRemove(MSG_DWORD* pmsg)
{
	cSkillObject* pSObj = GetSkillObject(pmsg->dwData);
	if(pSObj == NULL)
	{
		//ASSERTMSG(0,"SkillObject Remove Failed");
		return;
	}
	ReleaseSkillObject(pSObj);
}

void cSkillManager::OnReceiveSkillResult(MSG_SKILL_RESULT* pmsg)
{
	cSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(NULL,&pmsg->TargetList,eDamageKind_Front);
		return;
	}

	pSObj->OnReceiveSkillResult(pmsg);
}

void cSkillManager::OnReceiveSkillCancel(MSG_DWORD* pmsg)
{
	cSkillObject* pSObj = GetSkillObject(pmsg->dwData);

	if( pSObj )
	{
		pSObj->Cancel();
	}
}

// 080318 LUJ, 파티 버프 창을 갱신하도록 함
void cSkillManager::OnReceiveSkillDelayReset(MSG_DWORD2* pmsg)
{
	cSkillObject* pSObj = GetSkillObject(pmsg->dwData1);

	if( !	pSObj ||
		pSObj->GetSkillInfo()->GetKind() != SKILLKIND_BUFF )
	{
		return;
	}

	cBuffSkillObject* pBuffObj = ( cBuffSkillObject* )pSObj;

	sSKILL_CREATE_INFO* skillCreateInfo = pBuffObj->GetSkillObjectInfo();

	if( ! skillCreateInfo )
	{
		return;
	}
	else if( skillCreateInfo->operatorId == HEROID )
	{
		pBuffObj->ResetDelay();

		if( pmsg->dwData2 )
		{				
			pBuffObj->SetCount( ( WORD )pmsg->dwData2 );
		}
	}

	// 080318 LUJ, 파티 버프창 갱신
	{
		CPartyDialog* dialog = GAMEIN->GetPartyDialog();

		if( dialog )
		{
			SKILLOBJECT_INFO objectInfo	= { 0 };
			objectInfo.SkillIdx						= pBuffObj->GetSkillIdx();
			objectInfo.SkillObjectIdx				= pBuffObj->GetID();
			objectInfo.RemainTime					= pmsg->dwData2;
			objectInfo.MainTarget.dwMainTargetID	= skillCreateInfo->mainTarget.dwMainTargetID;

			// 080318 LUJ, 시간이 없는 버프는 방금 생성된 버프이므로 정보대로 시간을 넣어주자
			if( ! objectInfo.RemainTime )
			{
				objectInfo.RemainTime = ( ( cBuffSkillInfo* )pBuffObj->GetSkillInfo() )->GetSkillInfo()->DelayTime;
			}

			objectInfo.RemainTime	+= gCurTime;

			dialog->AddSkill( objectInfo );
		}
	}
}

void cSkillManager::OnReceiveSkillCount(MSG_DWORD2* pmsg)
{
	cSkillObject* pSObj = GetSkillObject( pmsg->dwData1 );

	if( pSObj && pSObj->GetSkillInfo()->GetKind() == SKILLKIND_BUFF )
	{
		cBuffSkillObject* pBuffObj = ( cBuffSkillObject* )pSObj;

		if( pBuffObj->GetSkillObjectInfo()->operatorId == HEROID )
		{
			pBuffObj->SetCount( ( WORD )pmsg->dwData2 );
		}
	}
}

// 080318 LUJ, 파티원이 스킬 추가/삭제/갱신했을 경우, 파티 창에 표시하도록 함
void cSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SKILL_SKILLOBJECT_ADD:
		{
			MSG_SKILLOBJECT_ADD2* pmsg = (MSG_SKILLOBJECT_ADD2*)pMsg;
			
			// 080318 LUJ, 파티원에게 버프를 추가한다
			{
				SKILLOBJECT_INFO	objectInfo	= pmsg->SkillObjectInfo;
				cBuffSkillInfo*		skillInfo	= ( cBuffSkillInfo* )( GetSkillInfo( objectInfo.SkillIdx ) );
				CPartyDialog*		dialog		= GAMEIN->GetPartyDialog();

				if(	skillInfo												&&
					skillInfo->GetKind() == SKILLKIND_BUFF					&&
					dialog )
				{
					// 080318 LUJ, 시간이 없는 버프는 방금 생성된 버프이므로 정보대로 시간을 넣어주자
					if( ! objectInfo.RemainTime )
					{
						objectInfo.RemainTime = skillInfo->GetSkillInfo()->DelayTime;
					}

					// 080318 LUJ, 현재 시간을 더해줘야 정상적으로 표시된다
					objectInfo.RemainTime	+= gCurTime;
					objectInfo.SkillIdx		= skillInfo->GetSkillInfo()->Index;					

					dialog->AddSkill( objectInfo );
				}
			}

			OnReceiveSkillObjectAdd(pmsg);
		}
		break;
	case MP_SKILL_SKILLOBJECT_REMOVE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			// 080318 LUJ, 파티원에게서 버프를 지워준다
			{
				SKILLOBJECT_INFO objectInfo = { 0 };
				objectInfo.SkillObjectIdx	= pmsg->dwData;

				cSkillObject*	skillObject = GetSkillObject( pmsg->dwData );
				cSkillInfo*		skillInfo	= 0;
				
				if( skillObject )
				{
					CObject* operatorObject	= skillObject->GetOperator();
					
					skillInfo								= skillObject->GetSkillInfo();
					objectInfo.Operator						= operatorObject->GetID();
					objectInfo.SkillIdx						= skillObject->GetSkillIdx();

					const sSKILL_CREATE_INFO* skillCreateInfo	= skillObject->GetSkillObjectInfo();

					if( skillCreateInfo )
					{
						objectInfo.MainTarget.dwMainTargetID	= skillCreateInfo->mainTarget.dwMainTargetID;
					}					
				}

				CPartyDialog* dialog = GAMEIN->GetPartyDialog();

				if( skillInfo								&&
					skillInfo->GetKind() == SKILLKIND_BUFF	&&
					dialog )
				{
					dialog->RemoveSkill( objectInfo );
				}
			}

			OnReceiveSkillObjectRemove(pmsg);
		}
		break;
	case MP_SKILL_SKILL_RESULT:
		{
			MSG_SKILL_RESULT* pmsg = (MSG_SKILL_RESULT*)pMsg;
			OnReceiveSkillResult(pmsg);
		}
		break;
	case MP_SKILL_START_NACK:
		{
			const MSG_DWORD2* message = (MSG_DWORD2*)pMsg;
			OBJECTSTATEMGR->StartObjectState(HERO,eObjectState_None);

			// 080610 LUJ, GM툴에서 스킬 발동 시 오류 원인을 콘솔에 나타내도록 함
#ifdef _GMTOOL_
			const DWORD			skillIndex	=				message->dwData1;
			const SKILL_RESULT	result		= SKILL_RESULT( message->dwData2 );

			cSkillInfo* skillInfo = GetSkillInfo( skillIndex );

			if( ! skillInfo )
			{
				break;
			}

            const char* resultText = "unknown";

			switch( result )
			{
			case SKILL_OPERATOR_INVALUED:
				{
					resultText	= "opeator is invalued";
					break;
				}
			case SKILL_INFO_INVALUED:
				{
					resultText	= "information is invalued";
					break;
				}
			case SKILL_LEVEL_INVALUED:
				{
					resultText	= "level is invalued";
					break;
				}
			case SKILL_STATE_INVALUED:
				{
					resultText	= "state is invalued";
					break;
				}
			case SKILL_TARGET_INVALUED:
				{
					resultText	= "target is invalued";
					break;
				}
			case SKILL_RANGE_NOTENOUGH:
				{
					resultText	= "range is not enough";
					break;
				}
			case SKILL_COOLING:
				{
					resultText	= "state is cooling";
					break;
				}
			case SKILL_FAIL_ETC:
				{
					resultText	= "failed by unknown reason";
					break;
				}
			case SKILL_DESTROY:
				{
					resultText	= "skill is destoryed";
					break;
				}
			}

			cprintf( "%s(%d): %s\n",
				skillInfo->GetName(),
				skillInfo->GetIndex(),
				resultText );
#endif
		}
		break;
	case MP_SKILL_CANCEL_NOTIFY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			OnReceiveSkillCancel(pmsg);
		}
		break;
	case MP_SKILL_DELAY_RESET_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillDelayReset(pmsg);
		}
		break;
	// 080602 LUJ, 스킬 발동 시 포인트/아이템 소모 처리한다. 돈은 처리하지 않아도 서버에서 전송된다
	case MP_SKILL_START_ACK:
		{
			const MSG_DWORD2* message = ( MSG_DWORD2* )pMsg;

			const DWORD skillIndex	= message->dwData1;
			const DWORD itemDbIndex	= message->dwData2;

            const ACTIVE_SKILL_INFO* skillInfo = 0;
			{
				cActiveSkillInfo* info = ( cActiveSkillInfo* )GetSkillInfo( skillIndex );

				if( ! info )
				{
					break;
				}
				
				skillInfo = info->GetSkillInfo();

				if( ! skillInfo )
				{
					break;
				}
			}

			// 080602 LUJ, 포인트 소모 처리
			{
				CHero* hero = OBJECTMGR->GetHero();

				if( ! hero )
				{
					break;
				}

				const ACTIVE_SKILL_INFO::ConsumePoint& point = skillInfo->mConsumePoint;

				switch( point.mType )
				{
				case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
				case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
				case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
					{
						// 080602 LUJ, 서버에서 클라이언트도 변경하므로 별도 처리 필요 없음
						break;
					}
				case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
					{
						CCharacterDialog*	dialog	= GAMEIN->GetCharacterDialog();
						CHero*				hero	= OBJECTMGR->GetHero();

						if( ! hero ||
							! dialog )
						{
							break;
						}

						const FAMETYPE fame = hero->GetBadFame() - point.mValue;

						hero->SetBadFame( fame );
						dialog->SetBadFame( fame );
						break;
					}
				}
			}
			
			// 080602 LUJ, 아이템 소모 처리
			{
				CItem* item = ITEMMGR->GetItem( itemDbIndex );

				if( ! item )
				{
					break;
				}

				if( item->GetDurability() > skillInfo->mConsumeItem.Durability &&
					ITEMMGR->IsDupItem( item->GetItemIdx() ) )
				{
					item->SetDurability( item->GetDurability() - skillInfo->mConsumeItem.Durability );
					item->SetLock( FALSE );
				}
				else
				{
					CItem* deletedItem = 0;

					ITEMMGR->DeleteItem( item->GetPosition(), &deletedItem );
				}
			}
		}
		break;
	// 080602 LUJ, 스킬 발동 실패
	case MP_SKILL_CONSUME_NACK:
		{
			const MSG_DWORD2*	message		= ( MSG_DWORD2* )pMsg;
			const DWORD			skillIndex	= message->dwData1;
			const DWORD			itemDbIndex	= message->dwData2;

			// 080602 LUJ, 소모될 아이템이 잠겨있으므로 풀어줘야 한다.
			{
				CItem* item = ITEMMGR->GetItem( itemDbIndex );

				if( item )
				{
					item->SetLock( FALSE );
				}
			}

			{
				cActiveSkillInfo* skillInfo = ( cActiveSkillInfo* )GetSkillInfo( skillIndex );

				if( ! skillInfo )
				{
					break;
				}

				const ACTIVE_SKILL_INFO* activeSkillInfo = skillInfo->GetSkillInfo();

				if( ! activeSkillInfo )
				{
					break;
				}

				CHATMGR->AddMsg(
					CTC_SYSMSG,
					"%s%s",
					activeSkillInfo->Name,
					CHATMGR->GetChatMsg( 1543 ) );
			}
		}
		break;
	case MP_SKILL_COUNT_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillCount(pmsg);
		}
		break;
	}
}


void cSkillManager::AddPassive( DWORD SkillIdx )
{
	cSkillInfo* pInfo = GetSkillInfo( SkillIdx );
	cActiveSkillInfo* pActive = ( cActiveSkillInfo* )pInfo;

	if( !pActive )
	{
		return;
	}
	
	for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; cnt++ )			//KES 070530 : WORD --> DWORD
	{
		DWORD index = pActive->GetSkillInfo()->Buff[ cnt ];		//KES 070530 : WORD --> DWORD

		if( index == 0 )
		{
			continue;
		}

		cBuffSkillInfo* pSkillInfo = ( cBuffSkillInfo* )GetSkillInfo( index );

		if( pSkillInfo->GetSkillInfo()->ActiveRule )
		{
			HERO->AddSpecialSkill( pSkillInfo );
		}
		else
		{
			pSkillInfo->AddPassiveStatus();
		}
	}
}

void cSkillManager::RemovePassive( DWORD SkillIdx )
{
	cSkillInfo* pInfo = GetSkillInfo( SkillIdx );
	cActiveSkillInfo* pActive = ( cActiveSkillInfo* )pInfo;
	
	for( DWORD cnt = 0; cnt < MAX_BUFF_COUNT; cnt++ )			//KES 070530 : WORD --> DWORD
	{
		DWORD index = pActive->GetSkillInfo()->Buff[ cnt ];		//KES 070530 : WORD --> DWORD

		if( index == 0 )
		{
			continue;
		}

		cBuffSkillInfo* pSkillInfo = ( cBuffSkillInfo* )GetSkillInfo( index );

		if( pSkillInfo->GetSkillInfo()->ActiveRule )
		{
			HERO->RemoveSpecialSkill( pSkillInfo );
		}
		else
		{
			pSkillInfo->RemovePassiveStatus();
		}
	}
}

DWORD cSkillManager::GetSkillSize( DWORD index ) const
{
	SkillSize::const_iterator it = mSkillSize.find( index );

	return mSkillSize.end() == it ? 0 : it->second;
}


cSkillManager* cSkillManager::GetInstance()
{
	// 참조: http://www.codeproject.com/useritems/VC2003MeyersSingletonBug.asp
	//
	// 정적 인라인 함수는 참조시마다 생성된다(인라인 함수는 직접 대치될 뿐 링크되지 않는다). 
	// 이로 인해 싱글턴 변수까지 참조시마다 생성된다!
	// 이를 막으려면 단일한 링크 지점을 만들어줘야한다. C 파일에 직접 구현하면 해결된다.
	static cSkillManager instance;

	return &instance;
}

void cSkillManager::Process()
{
	cSkillObject* pSkillObject = NULL;
	
	m_SkillObjectTable.SetPositionHead();

	while(pSkillObject = m_SkillObjectTable.GetData())
	{
		/// 스킬 오브젝트 업데이트
		pSkillObject->Update();
	}
}

void cSkillManager::SkillCancel( cSkillObject* pSkill )
{
	// 임시 객체는 취소할수 없다.
	if( pSkill->GetID() == TEMP_SKILLOBJECT_ID )
	{
		return;
	}
	
	// 서버에 취소 메세지를 보낸다
	MSG_DWORD msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_CANCEL_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = pSkill->GetID();

	NETWORK->Send( &msg, sizeof( msg ) );

	// 캐스팅 딜레이 체크 해제
	cTimeDelay* pDelay = HERO->GetSkillCancelDelay();
	pDelay->End();

	// 스킬 해제
	HERO->SetCurrentSkill( NULL );

	// 스킬 연출 중지
	pSkill->Cancel();
}


// 스킬을 사용
BOOL cSkillManager::ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill)
{
	DWORD SkillNum;
	cSkillInfo* pSkillInfo;

	/// 스킬이 기본공격일 경우
	if(bSkill == FALSE)
	{
		/// 기본공격 정보를 가져온다
		SkillNum = GetPetComboSkillIdx(pHeroPet);
		pSkillInfo = GetSkillInfo(SkillNum);
		mpSkillInfo = pSkillInfo;
	}
	else
		pSkillInfo = mpSkillInfo;
		
	/// 스킬 정보가 없다면 실패
	if(pSkillInfo == NULL)
		return FALSE;

	/// 스킬을 실행한다
	return ExcutePetSkillSYN(pHeroPet,pTarget,pSkillInfo);
}

BOOL cSkillManager::ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	
	/// 스킬 정보가 없으면 실패
	if(pInfo == NULL)
		return FALSE;

//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// 공격 가능 대상인지 판단
	if(pSkillInfo->IsValidTarget( HERO, pTarget ) == FALSE )
	{
		return FALSE;
	}

	/// 스킬 레벨을 가져온다
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// 실행 가능한지 판단
	if(pSkillInfo->IsExcutablePetSkillState(pHeroPet,SkillLevel,&target) == FALSE)
	{
		/// 기본 공격으로 전환하고 스킬 사용실패 한다
		//pHero->SetCurComboNum(0);
		//printf( "%d\t%s\n", 0, "cSkillManager::ExcuteSkillSYN1", 0 );
		return FALSE;
	}

	/// 타겟 설정 - 수정해야 함

	/// 평화모드일 경우
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	/*
	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)
		/// 전투모드로 바꾼다
		PEACEWARMGR->ToggleHeroPeace_WarMode();
		*/
	
	/// 타겟의 위치값이 없으면 실패
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	/// 사정거리 판단
	if(pSkillInfo->IsInPetSkillRange(pHeroPet,&target) == TRUE)
	/// 사정거리 안쪽이면
	{	
		/// 이동 중지
		MOVEMGR->HeroPetMoveStop();
		
		/// 자신이 타겟일 경우에는 보는 방향을 바꾸지 않는다.
		if( pSkillInfo->GetSkillInfo()->Target != 1 )
			MOVEMGR->SetLookatPos(pHeroPet,pTargetPos,0,gCurTime);
				
		/// 이동 행동을 초기화
		pHeroPet->SetMovingAction(NULL);

		/// 스킬 사용
		return RealExcutePetSkillSYN(pHeroPet,&target,pSkillInfo);
	}
	else
	/// 사정거리 밖이면
	{	
		/// 스킬 사용을 위한 이동설정
		CAction MoveAction;
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroPetActionMove(&target,&MoveAction);
		return FALSE;
	}

	return TRUE;
}

BOOL cSkillManager::RealExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	/// 스킬 정보가 없으면 실패
	if(pInfo == NULL)
		return FALSE;
	
//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// 스킬 레벨을 가져온다
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// 실행 가능한지 판단
	if(pSkillInfo->IsExcutablePetSkillState(pHeroPet,SkillLevel,pTarget) == FALSE)
	{
		return FALSE;
	}

	// 080602 LUJ, 소모 아이템 정보가 담긴다
	ITEMBASE consumeItem;
	ZeroMemory( &consumeItem, sizeof( consumeItem ) );

	// 080602 LUJ, 소모 포인트/아이템이 충분한지 검사
	{
		const ACTIVE_SKILL_INFO*	info	= pSkillInfo->GetSkillInfo();
		CHero*						hero	= OBJECTMGR->GetHero();

		if( ! info ||
			! hero )
		{
			return FALSE;
		}

		// 080602 LUJ, 포인트 검사
		{
			const ACTIVE_SKILL_INFO::ConsumePoint& point = info->mConsumePoint;

			switch( point.mType )
			{
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
				{
					CSHFamily* family = hero->GetFamily();

					if( ! family )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 828 ) );
						return FALSE;
					}

					const CSHFamily::stINFO_EX* info = family->GetEx();

					if( !	info	||
							info->nHonorPoint < point.mValue )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1568 ),
							point.mValue - info->nHonorPoint );
						return FALSE;
					}
					
					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
				{
					if( ! hero->GetGuildIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 939 ) );
						return FALSE;
					}
					else if( GUILDMGR->GetScore() < point.mValue )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1566 ),
							point.mValue - GUILDMGR->GetScore() );
						return FALSE;
					}

					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
				{
					if( hero->GetExpPoint() < EXPTYPE( point.mValue ) )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1567 ),
							EXPTYPE( point.mValue ) - hero->GetExpPoint() );
						return FALSE;
					}

					break;
				}
			case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
				{
					if( hero->GetBadFame() < FAMETYPE( point.mValue ) )
					{
						CHATMGR->AddMsg(
							CTC_SYSMSG,
							CHATMGR->GetChatMsg( 1565 ),
							FAMETYPE( point.mValue ) - hero->GetBadFame() );
						return FALSE;
					}

					break;
				}
			}
		}

		// 080602 LUJ, 소지금 검사
		if( info->Money > hero->GetMoney() )
		{
			CHATMGR->AddMsg(
				CTC_SYSMSG,
				CHATMGR->GetChatMsg( 1333 ),
				info->Money - hero->GetMoney() );

			return FALSE;
		}
		// 080602 LUJ, 아이템 검사
		else if( info->mConsumeItem.wIconIdx )
		{
			CInventoryExDialog* inventoryDialog	= GAMEIN->GetInventoryDialog();

			if( ! inventoryDialog )
			{
				return FALSE;
			}

			const POSTYPE	inventoryStartPosition	= TP_INVENTORY_START;
			const POSTYPE	inventoryEndPosition	= TP_INVENTORY_END + TABCELL_INVENTORY_NUM * HERO->Get_HeroExtendedInvenCount();

			for(
				POSTYPE position = inventoryStartPosition;
				inventoryEndPosition > position;
				++position )
			{
				CItem* item = inventoryDialog->GetItemForPos( position );

				if( !	item													||
						item->GetItemIdx()		!= info->mConsumeItem.wIconIdx	||
					(	ITEMMGR->IsDupItem( item->GetItemIdx() )	&&
						item->GetDurability()	< info->mConsumeItem.Durability ) )
				{
					continue;
				}

				consumeItem = item->GetItemBaseInfo();
				break;
			}

			// 080602 LUJ, 소모할 아이템을 찾지 못함
			if( ! consumeItem.wIconIdx )
			{
				const ITEM_INFO* itemInfo = ITEMMGR->GetItemInfo( info->mConsumeItem.wIconIdx );

				if( ! itemInfo )
				{
					return FALSE;
				}

				CHATMGR->AddMsg(
					CTC_SYSMSG,
					CHATMGR->GetChatMsg( 1543 ),
					itemInfo->ItemName, info->mConsumeItem.Durability );
				
				return FALSE;
			}
		}
	}

	/// 효과발동 딜레이가 없는 스킬은 클라이언트에서 바로 타겟리스트 작성
	//if( pSkillInfo->GetSkillInfo()->DelayType == 0 )
	{
		VECTOR3* pTargetPos = pTarget->GetTargetPosition();
		if(pTargetPos == NULL)
			return FALSE;

		switch( pSkillInfo->GetSkillInfo()->Area )
		{
		case 1:
			{
				pTarget->SetTargetObjectsInRange( pTargetPos, pSkillInfo->GetSkillInfo()->AreaData, pSkillInfo->GetSkillInfo()->AreaTarget );
			}
			break;
		}
	}

	float AttackSpeedRate = 1.0f;

	DWORD idx = pInfo->GetIndex();

	if( (idx / 100000) % 10 )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
		{
			AttackSpeedRate += ( ( pHeroPet->GetRatePassiveStatus()->PhysicSkillSpeedRate + pHeroPet->GetRateBuffStatus()->PhysicSkillSpeedRate ) * 0.01f );
		}
		else if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_MAGIC_ATTCK )
		{
			AttackSpeedRate += ( ( pHeroPet->GetRatePassiveStatus()->MagicSkillSpeedRate + pHeroPet->GetRateBuffStatus()->MagicSkillSpeedRate ) * 0.01f );
		}
	}
	else
	{
		AttackSpeedRate += ( ( pHeroPet->GetRatePassiveStatus()->NormalSpeedRate + pHeroPet->GetRateBuffStatus()->NormalSpeedRate ) * 0.01f );
	}
	
	if( AttackSpeedRate < 0 )
	{
		AttackSpeedRate = 1.0f;
	}

	DWORD time = pSkillInfo->GetSkillInfo()->AnimationTime / AttackSpeedRate;
	DWORD CastingTime = pSkillInfo->GetSkillInfo()->CastingTime / AttackSpeedRate;
	pHeroPet->GetSkillDelay()->Init( time );
	pHeroPet->GetSkillDelay()->Start();

	//HERO->StartSkillProgressBar( CastingTime );

	/// 다음 행동 초기화
	pHeroPet->SetNextAction(NULL);

	/// 서버에 스킬 생성 요청을 한다
	// 080602 LUJ, 소모 아이템 정보를 추가한다
	pSkillInfo->SendPetMsgToServer( pHeroPet, pTarget, &consumeItem );
	pSkillInfo->SetPetCooling();

	// 080602 LUJ, 소모될 아이템을 잠금 처리한다
	{
		CItem* item = ITEMMGR->GetItem( consumeItem.dwDBIdx );

		if( item )
		{
			item->SetLock( TRUE );
		}
	}

	/// 임시 스킬 객체 생성
	//DoCreateTempSkillObject( pSkillInfo, pTarget );

	if( ( ( pInfo->GetIndex() / 100000 ) % 10 ) == 2 )
	{
		pHeroPet->DisableAutoAttack();
	}
	//OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillStart);

	/// 스킬 사용 정보 초기화
	mpSkillInfo = NULL;
	
	return TRUE;
}

BOOL cSkillManager::OnPetSkillCommand(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill)
{
	// Guild Tournament나 공성전에서 Observer이면 사용불가
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( HERO->GetBattleTeam() == 2 )
			return FALSE;	
	}


	if( pHeroPet->GetState() == eObjectState_SkillSyn ||
		pHeroPet->GetState() == eObjectState_SkillUsing)
		SetNextPetComboSkill(pHeroPet,pTarget,bSkill);
	else
		ExcutePetSkillSYN(pHeroPet,pTarget,bSkill);		//return FALSE 처리....없다.. 괜찮을까?

	return TRUE;
}

BOOL cSkillManager::OnPetSkillCommand( DWORD idx )
{
	CActionTarget Target;

	cSkillInfo* pSkillInfo = m_SkillInfoTable.GetData( idx );

	if( !pSkillInfo )
	{
		return FALSE;
	}


	WORD skillkind = pSkillInfo->GetKind();
	if( skillkind == SKILLKIND_PASSIVE || skillkind == SKILLKIND_BUFF )
	{
		return FALSE;
	}

	cActiveSkillInfo* pInfo = ( cActiveSkillInfo* )pSkillInfo;

	// 지면 타겟 여부를 체크해야 한다.

	switch( pInfo->GetSkillInfo()->Target )
	{
	case eSkillTargetKind_None:
		{
			return FALSE;
		}
		break;
	case eSkillTargetKind_OnlyOthers:
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();

			if( targetObj == HERO )
			{
				return FALSE;
			}

			if( targetObj )
			{
				//---KES PK
				if( targetObj->GetObjectKind() == eObjectKind_Player )
				{
					PKMGR->SetPKStartTimeReset();
				}
				//---------

				if( pInfo->GetSkillInfo()->Unit == UNITKIND_RESURRECTION )
				{
					if( targetObj->GetObjectKind() != eObjectKind_Player)
					{
						return FALSE;
					}

					if( !targetObj->IsDied() || targetObj->GetState() != eObjectState_Die )
					{
						return FALSE;
					}

					// desc_hseos_데이트 존_01
					// S 데이트 존 추가 added by hseos 2007.12.03
					// ..챌린지 존에서는 부활 불가
					if (g_csDateManager.IsChallengeZoneHere())
					{
						return FALSE;
					}
					// E 데이트 존 추가 added by hseos 2007.12.03

					Target.InitActionTarget(targetObj,NULL);
				}
				else
				{
					if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
					{
						return FALSE;
					}

					Target.InitActionTarget(targetObj,NULL);
					HEROPET->EnableAutoAttack(&Target);
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655) );
				return FALSE;
			}
		}
		break;
	case eSkillTargetKind_Ground:
		{
		}
		break;
	case eSkillTargetKind_OnlySelf:
		{
			Target.InitActionTarget(HEROPET,NULL);	
		}
		break;
	case eSkillTargetKind_AllObject:
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();

			if( targetObj )
			{
				if( targetObj->IsDied() || targetObj->GetState() == eObjectState_Die )
				{
					return FALSE;
				}

				Target.InitActionTarget(targetObj,NULL);
				HEROPET->EnableAutoAttack(&Target);
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655) );
				return FALSE;
			}	
		}
		break;
	}

	mpSkillInfo = pSkillInfo;	

	if( idx )
	{
		return OnPetSkillCommand( HEROPET, &Target, TRUE );
	}
	else
	{
		return OnPetSkillCommand( HEROPET, &Target, FALSE );
	}
}


DWORD cSkillManager::GetPetComboSkillIdx(CHeroPet* pHeroPet)
{
	DWORD skillidx;

	switch( pHeroPet->GetType() )
	{
	case ePetType_Basic:
	case ePetType_Physic:	skillidx = PETMGR->GetClosedSkillIndex();	break;
	case ePetType_Magic:
	case ePetType_Support:	skillidx = PETMGR->GetRangeSkillIndex();	break;
	}

	return skillidx;
}

void cSkillManager::SetNextPetComboSkill(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill)
{
	cSkillInfo* pNextSkill;
	DWORD NextSkillIdx;

	if(bSkill)
		pNextSkill = mpSkillInfo;
	else
	{
		NextSkillIdx = GetPetComboSkillIdx(pHeroPet);
		pNextSkill = GetSkillInfo(NextSkillIdx);
	}

	if(pNextSkill == NULL)
		return;
	CAction act;
	act.InitSkillAction( ( cActiveSkillInfo* )pNextSkill, pTarget );
	pHeroPet->SetNextAction(&act);
}
