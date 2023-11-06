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
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.12.03
#include "petmanager.h"
// 080602 LUJ, ��ų ��� �� �Ҹ� ������� ����
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
		//ASSERTMSG(0,"SkillList�� �ε����� ���߽��ϴ�.");
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
		//ASSERTMSG(0,"SkillList�� �ε����� ���߽��ϴ�.");
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
		//ASSERTMSG(0,"SkillList�� �ε����� ���߽��ϴ�.");
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
		// 080710 LUJ, �ѱ� �⺻ ��ų ���
	case eWeaponAnimationType_Gun:			Skill = 7;	break;
		// 080703 LUJ, �̵����� �ܰ�/�Ѽչ��Ⱑ ���� �ִϸ��̼��� �ִ�.
	case eWeaponAnimationType_TwoBlade:
		{
			Skill = ( eWeaponType_Dagger == pHero->GetWeaponEquipType() ? 8 : 10 );
			break;
		}
	}
	

	SkillNum = SkillNum + ( ( Skill ) * 100 ) + CurComboNum + 1;

	return SkillNum;
}


// ��ų�� ���
BOOL cSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bSkill)
{
	DWORD SkillNum;
	cSkillInfo* pSkillInfo;

	/// ��ų�� �⺻������ ���
	if(bSkill == FALSE)
	{
		/// �⺻���� ������ �����´�
		SkillNum = GetComboSkillIdx(pHero);
		pSkillInfo = GetSkillInfo(SkillNum);
		mpSkillInfo = pSkillInfo;
	}
	else
		pSkillInfo = mpSkillInfo;
		
	/// ��ų ������ ���ٸ� ����
	if(pSkillInfo == NULL)
		return FALSE;

	/// ��ų�� �����Ѵ�
	return ExcuteSkillSYN(pHero,pTarget,pSkillInfo);
}

BOOL cSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	
	/// ��ų ������ ������ ����
	if(pInfo == NULL)
		return FALSE;

//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// ���� ���� ������� �Ǵ�
	if(pSkillInfo->IsValidTarget( pHero, pTarget ) == FALSE )
	{
		return FALSE;
	}

	/// ��ų ������ �����´�
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// ���� �������� �Ǵ�
	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,&target) == FALSE)
	{
		/// �⺻ �������� ��ȯ�ϰ� ��ų ������ �Ѵ�
		//pHero->SetCurComboNum(0);
		//printf( "%d\t%s\n", 0, "cSkillManager::ExcuteSkillSYN1", 0 );
		return FALSE;
	}

	/// Ÿ�� ���� - �����ؾ� ��

	/// ��ȭ����� ���
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	/*
	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)
		/// �������� �ٲ۴�
		PEACEWARMGR->ToggleHeroPeace_WarMode();
		*/
	
	/// Ÿ���� ��ġ���� ������ ����
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	/// �����Ÿ� �Ǵ�
	if(pSkillInfo->IsInSkillRange(pHero,&target) == TRUE)
	/// �����Ÿ� �����̸�
	{	
		/// �̵� ����
		MOVEMGR->HeroMoveStop();
		
		/// �ڽ��� Ÿ���� ��쿡�� ���� ������ �ٲ��� �ʴ´�.
		if( pSkillInfo->GetSkillInfo()->Target != 1 )
			MOVEMGR->SetLookatPos(pHero,pTargetPos,0,gCurTime);
				
		/// �̵� �ൿ�� �ʱ�ȭ
		pHero->SetMovingAction(NULL);

		/// ��ų ���
		return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
	}
	else
	/// �����Ÿ� ���̸�
	{	
		/// �⺻���� �ʱ�ȭ
		pHero->SetCurComboNum(0);
		
		/// ��ų ����� ���� �̵�����
		CAction MoveAction;
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroActionMove(&target,&MoveAction);
		return FALSE;
	}

	return TRUE;
}

BOOL cSkillManager::RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	/// ��ų ������ ������ ����
	if(pInfo == NULL)
		return FALSE;
	
//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// ��ų ������ �����´�
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// ���� �������� �Ǵ�
	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pTarget) == FALSE)
	{
		/// �⺻ �������� ��ȯ�ϰ� ��ų ������ �Ѵ�
		pHero->SetCurComboNum(0);
		return FALSE;
	}

	// 080602 LUJ, �Ҹ� ������ ������ ����
	ITEMBASE consumeItem;
	ZeroMemory( &consumeItem, sizeof( consumeItem ) );

	// 080602 LUJ, �Ҹ� ����Ʈ/�������� ������� �˻�
	{
		const ACTIVE_SKILL_INFO*	info	= pSkillInfo->GetSkillInfo();
		CHero*						hero	= OBJECTMGR->GetHero();

		if( ! info ||
			! hero )
		{
			return FALSE;
		}

		// 080602 LUJ, ����Ʈ �˻�
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

		// 080602 LUJ, ������ �˻�
		if( info->Money > hero->GetMoney() )
		{
			CHATMGR->AddMsg(
				CTC_SYSMSG,
				CHATMGR->GetChatMsg( 1333 ),
				info->Money - hero->GetMoney() );

			return FALSE;
		}
		// 080602 LUJ, ������ �˻�
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

			// 080602 LUJ, �Ҹ��� �������� ã�� ����
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

	/// ȿ���ߵ� �����̰� ���� ��ų�� Ŭ���̾�Ʈ���� �ٷ� Ÿ�ٸ���Ʈ �ۼ�
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

	/// ���� �ൿ �ʱ�ȭ
	HERO->SetNextAction(NULL);

	/// ������ ��ų ���� ��û�� �Ѵ�
	// 080602 LUJ, �Ҹ� ������ ������ �߰��Ѵ�
	pSkillInfo->SendMsgToServer( pHero, pTarget, &consumeItem );
	pSkillInfo->SetCooling();

	// 080602 LUJ, �Ҹ�� �������� ��� ó���Ѵ�
	{
		CItem* item = ITEMMGR->GetItem( consumeItem.dwDBIdx );

		if( item )
		{
			item->SetLock( TRUE );
		}
	}

	/// �ӽ� ��ų ��ü ����
	DoCreateTempSkillObject( pSkillInfo, pTarget );

	/// �⺻���� ����
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

	/// ��ų ��� ���� �ʱ�ȭ
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
	// ���� ����� �����Ҽ� ����
	//if(pTarget->GetTargetID() != 0)
	//{
	//	CObject* pTObj = OBJECTMGR->GetObject(pTarget->GetTargetID());
	//	if(pTObj == NULL)
	//		return FALSE;
	//	if(pTObj->IsDied() == TRUE)
	//		return FALSE;
	//}
	//////////////////////////////////////////////////////////////////////////
	

	// Guild Tournament�� ���������� Observer�̸� ���Ұ�
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pHero->GetBattleTeam() == 2 )
			return FALSE;	
	}

//---KES ���󰡱�
	pHero->SetFollowPlayer( 0 );
//---------------

	if( pHero->GetState() == eObjectState_SkillSyn ||
		pHero->GetState() == eObjectState_SkillUsing)
		SetNextComboSkill(pHero,pTarget,bSkill);
	else
		ExcuteSkillSYN(pHero,pTarget,bSkill);		//return FALSE ó��....����.. ��������?

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

	// ���� Ÿ�� ���θ� üũ�ؾ� �Ѵ�.

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

					// desc_hseos_����Ʈ ��_01
					// S ����Ʈ �� �߰� added by hseos 2007.12.03
					// ..ç���� �������� ��Ȱ �Ұ�
					if (g_csDateManager.IsChallengeZoneHere())
					{
						return FALSE;
					}
					// E ����Ʈ �� �߰� added by hseos 2007.12.03

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
	*/ //GetMugongSkillIdx() �Լ��� ������� �ʰ� �Ǿ� ����
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
			if(pHero->GetCurComboNum() < 3)	// �ڵ������� �޺� 2������	12/3�� ȸ�� ��� 3���� 2�� �ٲ�
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
	/// �ӽ� ��ų��ü ����
	
	/// ���� ����
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

	/// ��ü ����
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ���� ��ȯ
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
	/// �ӽ� ��ü�� ��ü���ش�
	cSkillInfo* pSInfo = GetSkillInfo( pSOInfo->SkillIdx );

	/// �нú�� ���� ��ų�� �ӽ� ��ü�� ���鸮 ����.
	if( pSInfo->GetKind() == SKILLKIND_PASSIVE ||
		pSInfo->GetKind() == SKILLKIND_BUFF )
	{
		return FALSE;
	}

	/// �ӽ� ��ü�� �����´�.
	cSkillObject* pTempObj = m_SkillObjectTable.GetData( TEMP_SKILLOBJECT_ID );

	/// �ӽ� ��ü�� ����...
	if( !pTempObj )
	{
		return FALSE;
	}

	/// ��ų �ε����� �ٸ���!
	if( pTempObj->GetSkillIdx() != pSOInfo->SkillIdx )
	{
		return FALSE;
	}

	/// ��ų ������Ʈ ���̺��� �ӽ� ��ü�� �����.
	m_SkillObjectTable.Remove( TEMP_SKILLOBJECT_ID );
	
	/// ID �缳��
	pTempObj->ResetSkillObjectId( pSOInfo->SkillObjectIdx );

	/// �ٽ� ���̺� �ִ´�.
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

// 080318 LUJ, ��Ƽ ���� â�� �����ϵ��� ��
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

	// 080318 LUJ, ��Ƽ ����â ����
	{
		CPartyDialog* dialog = GAMEIN->GetPartyDialog();

		if( dialog )
		{
			SKILLOBJECT_INFO objectInfo	= { 0 };
			objectInfo.SkillIdx						= pBuffObj->GetSkillIdx();
			objectInfo.SkillObjectIdx				= pBuffObj->GetID();
			objectInfo.RemainTime					= pmsg->dwData2;
			objectInfo.MainTarget.dwMainTargetID	= skillCreateInfo->mainTarget.dwMainTargetID;

			// 080318 LUJ, �ð��� ���� ������ ��� ������ �����̹Ƿ� ������� �ð��� �־�����
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

// 080318 LUJ, ��Ƽ���� ��ų �߰�/����/�������� ���, ��Ƽ â�� ǥ���ϵ��� ��
void cSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SKILL_SKILLOBJECT_ADD:
		{
			MSG_SKILLOBJECT_ADD2* pmsg = (MSG_SKILLOBJECT_ADD2*)pMsg;
			
			// 080318 LUJ, ��Ƽ������ ������ �߰��Ѵ�
			{
				SKILLOBJECT_INFO	objectInfo	= pmsg->SkillObjectInfo;
				cBuffSkillInfo*		skillInfo	= ( cBuffSkillInfo* )( GetSkillInfo( objectInfo.SkillIdx ) );
				CPartyDialog*		dialog		= GAMEIN->GetPartyDialog();

				if(	skillInfo												&&
					skillInfo->GetKind() == SKILLKIND_BUFF					&&
					dialog )
				{
					// 080318 LUJ, �ð��� ���� ������ ��� ������ �����̹Ƿ� ������� �ð��� �־�����
					if( ! objectInfo.RemainTime )
					{
						objectInfo.RemainTime = skillInfo->GetSkillInfo()->DelayTime;
					}

					// 080318 LUJ, ���� �ð��� ������� ���������� ǥ�õȴ�
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

			// 080318 LUJ, ��Ƽ�����Լ� ������ �����ش�
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

			// 080610 LUJ, GM������ ��ų �ߵ� �� ���� ������ �ֿܼ� ��Ÿ������ ��
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
	// 080602 LUJ, ��ų �ߵ� �� ����Ʈ/������ �Ҹ� ó���Ѵ�. ���� ó������ �ʾƵ� �������� ���۵ȴ�
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

			// 080602 LUJ, ����Ʈ �Ҹ� ó��
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
						// 080602 LUJ, �������� Ŭ���̾�Ʈ�� �����ϹǷ� ���� ó�� �ʿ� ����
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
			
			// 080602 LUJ, ������ �Ҹ� ó��
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
	// 080602 LUJ, ��ų �ߵ� ����
	case MP_SKILL_CONSUME_NACK:
		{
			const MSG_DWORD2*	message		= ( MSG_DWORD2* )pMsg;
			const DWORD			skillIndex	= message->dwData1;
			const DWORD			itemDbIndex	= message->dwData2;

			// 080602 LUJ, �Ҹ�� �������� ��������Ƿ� Ǯ����� �Ѵ�.
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
	// ����: http://www.codeproject.com/useritems/VC2003MeyersSingletonBug.asp
	//
	// ���� �ζ��� �Լ��� �����ø��� �����ȴ�(�ζ��� �Լ��� ���� ��ġ�� �� ��ũ���� �ʴ´�). 
	// �̷� ���� �̱��� �������� �����ø��� �����ȴ�!
	// �̸� �������� ������ ��ũ ������ ���������Ѵ�. C ���Ͽ� ���� �����ϸ� �ذ�ȴ�.
	static cSkillManager instance;

	return &instance;
}

void cSkillManager::Process()
{
	cSkillObject* pSkillObject = NULL;
	
	m_SkillObjectTable.SetPositionHead();

	while(pSkillObject = m_SkillObjectTable.GetData())
	{
		/// ��ų ������Ʈ ������Ʈ
		pSkillObject->Update();
	}
}

void cSkillManager::SkillCancel( cSkillObject* pSkill )
{
	// �ӽ� ��ü�� ����Ҽ� ����.
	if( pSkill->GetID() == TEMP_SKILLOBJECT_ID )
	{
		return;
	}
	
	// ������ ��� �޼����� ������
	MSG_DWORD msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_CANCEL_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = pSkill->GetID();

	NETWORK->Send( &msg, sizeof( msg ) );

	// ĳ���� ������ üũ ����
	cTimeDelay* pDelay = HERO->GetSkillCancelDelay();
	pDelay->End();

	// ��ų ����
	HERO->SetCurrentSkill( NULL );

	// ��ų ���� ����
	pSkill->Cancel();
}


// ��ų�� ���
BOOL cSkillManager::ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill)
{
	DWORD SkillNum;
	cSkillInfo* pSkillInfo;

	/// ��ų�� �⺻������ ���
	if(bSkill == FALSE)
	{
		/// �⺻���� ������ �����´�
		SkillNum = GetPetComboSkillIdx(pHeroPet);
		pSkillInfo = GetSkillInfo(SkillNum);
		mpSkillInfo = pSkillInfo;
	}
	else
		pSkillInfo = mpSkillInfo;
		
	/// ��ų ������ ���ٸ� ����
	if(pSkillInfo == NULL)
		return FALSE;

	/// ��ų�� �����Ѵ�
	return ExcutePetSkillSYN(pHeroPet,pTarget,pSkillInfo);
}

BOOL cSkillManager::ExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	
	/// ��ų ������ ������ ����
	if(pInfo == NULL)
		return FALSE;

//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// ���� ���� ������� �Ǵ�
	if(pSkillInfo->IsValidTarget( HERO, pTarget ) == FALSE )
	{
		return FALSE;
	}

	/// ��ų ������ �����´�
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// ���� �������� �Ǵ�
	if(pSkillInfo->IsExcutablePetSkillState(pHeroPet,SkillLevel,&target) == FALSE)
	{
		/// �⺻ �������� ��ȯ�ϰ� ��ų ������ �Ѵ�
		//pHero->SetCurComboNum(0);
		//printf( "%d\t%s\n", 0, "cSkillManager::ExcuteSkillSYN1", 0 );
		return FALSE;
	}

	/// Ÿ�� ���� - �����ؾ� ��

	/// ��ȭ����� ���
	// 070124 LYW --- PeaceWar : Delete peacewar part.
	/*
	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)
		/// �������� �ٲ۴�
		PEACEWARMGR->ToggleHeroPeace_WarMode();
		*/
	
	/// Ÿ���� ��ġ���� ������ ����
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	/// �����Ÿ� �Ǵ�
	if(pSkillInfo->IsInPetSkillRange(pHeroPet,&target) == TRUE)
	/// �����Ÿ� �����̸�
	{	
		/// �̵� ����
		MOVEMGR->HeroPetMoveStop();
		
		/// �ڽ��� Ÿ���� ��쿡�� ���� ������ �ٲ��� �ʴ´�.
		if( pSkillInfo->GetSkillInfo()->Target != 1 )
			MOVEMGR->SetLookatPos(pHeroPet,pTargetPos,0,gCurTime);
				
		/// �̵� �ൿ�� �ʱ�ȭ
		pHeroPet->SetMovingAction(NULL);

		/// ��ų ���
		return RealExcutePetSkillSYN(pHeroPet,&target,pSkillInfo);
	}
	else
	/// �����Ÿ� ���̸�
	{	
		/// ��ų ����� ���� �̵�����
		CAction MoveAction;
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroPetActionMove(&target,&MoveAction);
		return FALSE;
	}

	return TRUE;
}

BOOL cSkillManager::RealExcutePetSkillSYN(CHeroPet* pHeroPet,CActionTarget* pTarget,cSkillInfo* pInfo)
{
	/// ��ų ������ ������ ����
	if(pInfo == NULL)
		return FALSE;
	
//	if( pInfo->GetKind() == SKILLKIND_BUFF || pInfo->GetKind() == SKILLKIND_PASSIVE )
//		return FALSE;

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )pInfo;
	
	/// ��ų ������ �����´�
	//int SkillLevel = pHero->GetSkillLevel(pSkillInfo->GetIndex());
	int SkillLevel = pSkillInfo->GetIndex() % 100;

	/// ���� �������� �Ǵ�
	if(pSkillInfo->IsExcutablePetSkillState(pHeroPet,SkillLevel,pTarget) == FALSE)
	{
		return FALSE;
	}

	// 080602 LUJ, �Ҹ� ������ ������ ����
	ITEMBASE consumeItem;
	ZeroMemory( &consumeItem, sizeof( consumeItem ) );

	// 080602 LUJ, �Ҹ� ����Ʈ/�������� ������� �˻�
	{
		const ACTIVE_SKILL_INFO*	info	= pSkillInfo->GetSkillInfo();
		CHero*						hero	= OBJECTMGR->GetHero();

		if( ! info ||
			! hero )
		{
			return FALSE;
		}

		// 080602 LUJ, ����Ʈ �˻�
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

		// 080602 LUJ, ������ �˻�
		if( info->Money > hero->GetMoney() )
		{
			CHATMGR->AddMsg(
				CTC_SYSMSG,
				CHATMGR->GetChatMsg( 1333 ),
				info->Money - hero->GetMoney() );

			return FALSE;
		}
		// 080602 LUJ, ������ �˻�
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

			// 080602 LUJ, �Ҹ��� �������� ã�� ����
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

	/// ȿ���ߵ� �����̰� ���� ��ų�� Ŭ���̾�Ʈ���� �ٷ� Ÿ�ٸ���Ʈ �ۼ�
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

	/// ���� �ൿ �ʱ�ȭ
	pHeroPet->SetNextAction(NULL);

	/// ������ ��ų ���� ��û�� �Ѵ�
	// 080602 LUJ, �Ҹ� ������ ������ �߰��Ѵ�
	pSkillInfo->SendPetMsgToServer( pHeroPet, pTarget, &consumeItem );
	pSkillInfo->SetPetCooling();

	// 080602 LUJ, �Ҹ�� �������� ��� ó���Ѵ�
	{
		CItem* item = ITEMMGR->GetItem( consumeItem.dwDBIdx );

		if( item )
		{
			item->SetLock( TRUE );
		}
	}

	/// �ӽ� ��ų ��ü ����
	//DoCreateTempSkillObject( pSkillInfo, pTarget );

	if( ( ( pInfo->GetIndex() / 100000 ) % 10 ) == 2 )
	{
		pHeroPet->DisableAutoAttack();
	}
	//OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillStart);

	/// ��ų ��� ���� �ʱ�ȭ
	mpSkillInfo = NULL;
	
	return TRUE;
}

BOOL cSkillManager::OnPetSkillCommand(CHeroPet* pHeroPet,CActionTarget* pTarget,BOOL bSkill)
{
	// Guild Tournament�� ���������� Observer�̸� ���Ұ�
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
		ExcutePetSkillSYN(pHeroPet,pTarget,bSkill);		//return FALSE ó��....����.. ��������?

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

	// ���� Ÿ�� ���θ� üũ�ؾ� �Ѵ�.

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

					// desc_hseos_����Ʈ ��_01
					// S ����Ʈ �� �߰� added by hseos 2007.12.03
					// ..ç���� �������� ��Ȱ �Ұ�
					if (g_csDateManager.IsChallengeZoneHere())
					{
						return FALSE;
					}
					// E ����Ʈ �� �߰� added by hseos 2007.12.03

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
