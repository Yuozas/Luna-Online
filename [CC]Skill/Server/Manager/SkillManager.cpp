#include "stdafx.h"
#include ".\skillmanager.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Info/BuffSkillInfo.h"
#include "../Object/OnOffSkillObject.h"
#include "../Delay/Delay.h"

#include "UserTable.h"

#include "Player.h"
// 080708 LUJ, ���� ��ų�� ����ϴ� ���� ó���� ���� ����
#include "Trap.h"

#include "CharMove.h"
#include "PackedData.h"

#include "BattleSystem_Server.h"
#include "MapDBMsgParser.h"
#include "SkillTreeManager.h"

#include "ObjectStateManager.h"

#include "ItemManager.h"
// 080602 LUJ, ������ ��� ��ų ������� ����
#include "Guild.h"
#include "GuildManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "..\hseos\Family\SHFamilyManager.h"
#include "pet.h"

cSkillManager::cSkillManager(void)
{
}

cSkillManager::~cSkillManager(void)
{
}

void cSkillManager::Init()
{
	/// Hash ���̺� �ʱ�ȭ
	m_SkillInfoTable.Initialize(1000);
	m_SkillObjectTable.Initialize(10000);
	m_SkillTreeTable.Initialize(64);

	/// ��ų ����Ʈ �ε�
	LoadSkillInfoList();
}

void cSkillManager::Release()
{
	/// ��ų ������Ʈ ����
	cSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while(pObject = m_SkillObjectTable.GetData())
	{
		ReleaseSkillObject(pObject,SKILL_RELEASE_DELETE);
	}
	m_SkillObjectTable.RemoveAll();

	/// ��ų ���� ����
	cSkillInfo* pSInfo = NULL;
	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();

	SKILL_TREE_INFO* pInfo = NULL;

	m_SkillTreeTable.SetPositionHead();
	while(pInfo = m_SkillTreeTable.GetData())
	{
		delete pInfo;
	}
	m_SkillTreeTable.RemoveAll();

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

	file.Init("System/Resource/skill_exception.bin","rb");

	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList�� �ε����� ���߽��ϴ�.");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		EXCEPTION* pException = new EXCEPTION;

		pException->Index = file.GetDword();
		pException->BuffIndex = file.GetDword();
		pException->TargetKind = file.GetWord();
		pException->TargetIndex = file.GetDword();
		pException->Type = file.GetWord();
		pException->Grade = file.GetWord();

		cSkillInfo* pSkill = m_SkillInfoTable.GetData( pException->BuffIndex );
		
		if( !pSkill || pSkill->GetKind() != SKILLKIND_BUFF || pSkill->GetKind() != SKILLKIND_PASSIVE )
		{
			continue;
		}

		cBuffSkillInfo* pBuff = ( cBuffSkillInfo* )pSkill;

		pBuff->AddException( pException );
	}

	file.Release();
}

cSkillObject* cSkillManager::GetSkillObject(DWORD SkillObjID)
{
	return (cSkillObject*)m_SkillObjectTable.GetData(SkillObjID);
}

cSkillInfo* cSkillManager::GetSkillInfo(DWORD SkillInfoIdx)
{
	return (cSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

void cSkillManager::Process()
{
	cSkillObject* pSObj;

	/// ��ȯ��
	DWORD result;
	
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		/// ��ų ������Ʈ ������Ʈ
		result = pSObj->Update();

		/// ��ų ���� ���
		if( result == SKILL_STATE_DESTROY )
		{
			ReleaseSkillObject(pSObj,SKILL_RELEASE_RECYCLE);
		}
	}
}

void cSkillManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = ( MSG_SKILL_START_SYN* )pMsg;
				
			CObject* object = g_pUserTable->FindUser( pmsg->Operator );
		
			if( ! object )
			{
				break;
			}

			SKILL_RESULT result = SKILL_FAIL_ETC;

			if( object->GetObjectKind() == eObjectKind_Player )
				result = OnSkillStartSyn( pmsg );
			else if( object->GetObjectKind() == eObjectKind_Pet )
				result = OnPetSkillStartSyn( pmsg );

			// 080610 LUJ, ��ų ���� �� �޽����� ��ȯ�ϵ��� ��
			if( result != SKILL_SUCCESS )
			{
				CObject* object = g_pUserTable->FindUser( pmsg->Operator );

				if( ! object )
				{
					break;
				}

				MSG_DWORD2 message;
				ZeroMemory( &message, sizeof( message ) );
				message.Category	= MP_SKILL;
				message.Protocol	= MP_SKILL_START_NACK;
				message.dwData1		= pmsg->SkillIdx;
				message.dwData2		= result;

				object->SendMsg( &message, sizeof( message ) );
			}
		}
		break;
	// 080602 LUJ, ������ ��� ��ų�� ���ĵ�
	case MP_SKILL_START_TO_MAP:
		{
			const MSG_ITEM_SKILL_START_TO_MAP* receivedMessage	= ( MSG_ITEM_SKILL_START_TO_MAP* )pMsg;

			const ACTIVE_SKILL_INFO* skillInfo = 0;
			{
				cActiveSkillInfo* info = ( cActiveSkillInfo* )GetSkillInfo( receivedMessage->mSkill.wSkillIdx );

				if( ! info )
				{
					break;
				}

				skillInfo = info->GetSkillInfo();

				if( !	skillInfo		||
						skillInfo->Area != TARGET_KIND_WORLD )
				{
					break;
				}
			}

			typedef std::set< DWORD >	ObjectIndexSet;
			ObjectIndexSet				objectIndexSet;

			// 080602 LUJ, ��ų ��� ���� ������ ������ �����Ѵ�
			switch( skillInfo->AreaTarget )
			{
			case eSkillAreaTargetGuild:
				{
					GUILDMEMBERINFO memberInfo[ MAX_GUILD_MEMBER ] = { 0 };

					const DWORD	guildIndex	= receivedMessage->dwObjectID;
					CGuild*		guild		= GUILDMGR->GetGuild( receivedMessage->dwObjectID );

					if( ! guild )
					{
						break;
					}

					guild->GetTotalMember( memberInfo );

					for(
						DWORD size = sizeof( memberInfo ) / sizeof( *memberInfo );
						size--; )
					{
						objectIndexSet.insert( memberInfo[ size ].MemberIdx );
					}

					break;
				}
			case eSkillAreaTargetFamily:
				{
					const DWORD familyIndex	 = receivedMessage->dwObjectID;
					// 080602 LUJ, �йи��� �� ������ ����Ÿ�� ����, ������Ʈ�� �ִ�. ���ŷ�����
					//				DB�� ȸ�� ����� �����ؼ� ������ �����ϵ��� ����.
					g_DB.FreeMiddleQuery(
						RSkillAddToFamily,
						skillInfo->Index,
						"EXEC DBO.MP_FAMILY_MEMBER_LoadInfo %d",
						familyIndex );
					break;
				}
			case eSkillAreaTargetParty:
				{
					const DWORD	partyIndex	= receivedMessage->dwObjectID;
					CParty*		party		= PARTYMGR->GetParty( partyIndex );

					if( ! party )
					{
						break;
					}

					const PARTYMEMBER* member = party->GetPartyMember();

					if( ! member )
					{
						break;
					}

					for( DWORD i = 0; i < MAX_PARTY_LISTNUM; ++i )
					{
						objectIndexSet.insert( member[ i ].MemberID );
					}

					break;
				}
			}

			// 080602 LUJ, ���õ� ������ ������� ���� ��ų�� �����Ѵ�
			for(
				ObjectIndexSet::const_iterator it = objectIndexSet.begin();
				objectIndexSet.end() != it;
				++it )
			{
				CPlayer* player = ( CPlayer* )g_pUserTable->FindUser( *it );

				if( !	player ||
						player->GetObjectKind() != eObjectKind_Player )
				{
					continue;
				}

				AddBuffSkill( *player, *skillInfo );
			}

			break;
		}
	case MP_SKILL_UPDATE_TARGET_SYN:
		{
			MSG_SKILL_UPDATE_TARGET* pmsg = ( MSG_SKILL_UPDATE_TARGET* )pMsg;

			OnSkillTargetUpdate( pmsg );
		}
		break;
	case MP_SKILL_CANCEL_SYN:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;
			OnSkillCancel( pmsg );
		}
		break;
	}
}

// 080602 LUJ, ���� ��ų�� �����Ѵ�
// 080616 LUJ, CPlayer -> CObject�� ����
// 080602 LUJ, ���� ��ų�� �����Ѵ�
// 080616 LUJ, CPlayer -> CObject�� ����
void cSkillManager::AddBuffSkill( CObject& object, const ACTIVE_SKILL_INFO& skillInfo )
{
	for(
		DWORD size = sizeof( skillInfo.Buff ) / sizeof( *( skillInfo.Buff ) );
		size--; )
	{
		const DWORD			buffSkillIndex	= skillInfo.Buff[ size ];
		cBuffSkillInfo*		info			= ( cBuffSkillInfo* )GetSkillInfo( buffSkillIndex );

		if( !	info ||
				info->GetKind() != SKILLKIND_BUFF )
		{
			continue;
		}

		const BUFF_SKILL_INFO* buff = info->GetSkillInfo();

		if( ! buff )
		{
			continue;
		}

		// 080602 LUJ, DelayTime�� �ּ� 1�̻� �� ����: CBuffSkillObject::ProcessStart()�� ���� ��ų ���� �κп��� ������
		BuffSkillStart(
			object.GetID(),
			buff->Index,
			buff->DelayTime ? buff->DelayTime : 1,
			buff->Count );

		// 080602 LUJ, DelayTime�� �ּ� 1�̻� �� ����: CBuffSkillObject::ProcessStart()�� ���� ��ų ���� �κп��� ������
		CharacterBuffAdd(
			object.GetID(),
			buff->Index,
			buff->DelayTime ? buff->DelayTime : 1,
			buff->Count,
			buff->IsEndTime );
	}
}

SKILL_RESULT cSkillManager::OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase)
{
	/// ������ ������ ���ٸ� ����
	// 080602 LUJ, ��ų �ߵ��� �÷��̾ �� �� �����Ƿ� ĳ�����Ѵ�
	CPlayer* pOperator = ( CPlayer* )g_pUserTable->FindUser( pmsg->Operator );

	// 080602 LUJ, �˻� ���� �߰�
	if( !	pOperator											||
			pOperator->GetObjectKind()	!= eObjectKind_Player	||
			pOperator->CurCastingSkillID )
	{
		return SKILL_OPERATOR_INVALUED;
	}

	// 080410 KTH -- �Ϲ� ��ų�� ��� ��ų Ʈ���� ���� �Ǿ��ִ� ��ų���� Ȯ���Ѵ�.
	if( pItemBase == NULL )
	{
		// �Ϲ� ������ �ƴ� ��쿡�� �Ʒ� �ڵ带 �����Ѵ�.
		if( (pmsg->SkillIdx / 100000) % 10 )
		{
			CPlayer* pPlayer = (CPlayer*)pOperator;
			cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );
			if( pSkillInfo == NULL )
				return SKILL_OPERATOR_INVALUED;
			cSkillTree* pSkillTree = pPlayer->GetSkillTree();
			if( pSkillTree == NULL )
				return SKILL_OPERATOR_INVALUED;
			SKILL_BASE* pSkillBase = pSkillTree->GetSkillBase( pmsg->SkillIdx / 100 * 100 + 1);

			if( pSkillBase == NULL || pSkillBase->Level != pmsg->SkillIdx - (pmsg->SkillIdx/100*100) )
				return SKILL_OPERATOR_INVALUED;
		}
	}
	// 080410 KTH -- ������ ������� ��ų�� ����� ��� �������� ������ Ȯ���Ѵ�.
	else
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );

		if( pSkillInfo == NULL || pItemInfo == NULL )
			return SKILL_OPERATOR_INVALUED;

		if( pmsg->SkillIdx != pItemInfo->SupplyValue )
			return SKILL_OPERATOR_INVALUED;
	}

	/// ��ų ������ ���ٸ� ����
	cSkillInfo* ptemp = GetSkillInfo( pmsg->SkillIdx );

	ASSERT( ptemp );
	if( ptemp == NULL )
	{
		return SKILL_INFO_INVALUED;
	}
	/// ��ų ���� �Ǵ�
	WORD skillKind = ptemp->GetKind();
//	if( skillKind == SKILLKIND_BUFF || skillKind == SKILLKIND_PASSIVE )
//	{
//		return SKILL_INFO_INVALUED;
//	}

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )ptemp;

	if( pOperator->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� �߿��� ���� ���ݸ� �����ϴ�
		if( skillKind != SKILLKIND_PHYSIC )
		{
			return SKILL_STATE_INVALUED;
		}
	}
	if( pOperator->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}

	// 080509 LUJ, �÷��̾ ��Ÿ�� ���� ��ų ��û�� �ϴ��� üũ�Ѵ�
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* player = ( CPlayer* )pOperator;

		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( !	skill ||
				player->IsCoolTime( *skill ) )
		{
			return SKILL_COOLING;
		}
	}
	// 080616 LUJ, �̹� ������ ������ ������ �� ������ �Ѵ�
	if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_TRAP &&
		pOperator->IsHaveTrap( pSkillInfo->GetSkillInfo()->UnitData ) )
	{
		return SKILL_COOLING;
	}
	/// ��ų ���� �Ǵ�
/*	int SkillLevel = pOperator->GetSkillLevel( pmsg->SkillIdx );
	
	if( pmsg->SkillIdx > 100 )
	{
		if( SkillLevel == 0 || SkillLevel == -1 )	
		{
			return SKILL_LEVEL_INVALUED;
		}
	}
	else
	{
		SkillLevel = 1;
	}
*/

	int SkillLevel = pSkillInfo->GetSkillInfo()->Level;

	/// ���� ���ӽ� ���� ���� ����
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );

	/// ��� ��ʸ�Ʈ�� ���������� �����ڴ� ��ų�� ����� �� ����
	if( pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetBattleTeam() == 2 )			
			return SKILL_STATE_INVALUED;
	}

	/// ��ų�� ����Ҽ� �ִ� �������� Ȯ��
	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel) == FALSE)
	{
		return SKILL_STATE_INVALUED;
	}

	VECTOR3 TargetPos;
	CObject * pTargetObject = NULL;
	float TargetRadius;

	/// Ÿ�� ���� Ȯ��
	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		/// Ÿ���� ����
		return SKILL_TARGET_INVALUED;
	}

	switch( pSkillInfo->GetSkillInfo()->Target )
	{
	case eSkillTargetKind_None:
		{
			return SKILL_TARGET_INVALUED;
		}
		break;
	case eSkillTargetKind_OnlyOthers:
		{
			if( pTargetObject == pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_Ground:
		{
		}
		break;
	case eSkillTargetKind_OnlySelf:
		{
			if( pTargetObject != pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_AllObject:
		{
		}
		break;
	}

	/// Ÿ�� ������Ʈ�� ũ�⸦ ���Ѵ�
	if(pmsg->MainTarget.MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
		TargetRadius = 0;
	else
	{
		pTargetObject = g_pUserTable->FindUser( pmsg->MainTarget.dwMainTargetID );
		TargetRadius = pTargetObject->GetRadius();
	}
	/*if(pSkillInfo->IsInRange(*pOperator, TargetPos, TargetRadius) == FALSE )
	{
		return SKILL_RANGE_NOTENOUGH;
	}*/
	/// �����Ÿ� �Ǵ�
	if( pSkillInfo->IsInSkillRange( pOperator, &TargetPos, TargetRadius ) == FALSE )
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo;
			DamageInfo.Clear();
			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		return SKILL_RANGE_NOTENOUGH;
	}
	
	/// �̹� ����ڰ� ����� ��ų�� �ִ� ���
	
	/// On/Off�� ��ų�� ���
	if( skillKind == SKILLKIND_ONOFF )
	{
		/// �̹� ����ڿ��� �ɷ��ִ� ��ų�̸� �� ��ų�� �����Ѵ�
/*		cSkillObject* pSkill = pTargetObject->GetOnOffSkillList()->GetData( pmsg->SkillIdx );
		
		if( pSkill )
		{
			cOnOffSkillObject* pSkillObject = ( cOnOffSkillObject* )pSkill;
			pSkillObject->Destroy();

			return SKILL_DESTROY;
		}
*/	
		BOOL destroy = FALSE;

		for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
		{
			DWORD idx = pSkillInfo->GetSkillInfo()->Buff[ i ];
			idx = idx / 100;

			if( idx )
			{
				cSkillObject* pSkill = pTargetObject->GetBuffSkillList()->GetData( idx );

				if( pSkill )
				{
					if( pSkill->GetSkillIdx() <= pSkillInfo->GetSkillInfo()->Buff[ i ] )
					{
						pSkill->SetEndState();
						pSkill->EndState();
						destroy = TRUE;
					}
				}
			}
		}
		
		if( destroy )
		{
			return SKILL_DESTROY;
		}
	}

	// 080602 LUJ, ���� ��ų�� DB���� ���� ó���� �� ���۵ȴ�. ��� ���� ���� ������Ʈ ID�� ���� �޾ƿ´�
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	// 080602 LUJ, ��ų ��� �� �Ҹ� ó��.
	//
	//		ó�� ����:	�ϴ� ��ų�� �ߵ���Ų ��, ���������� ����Ǵ� ����Ʈ�� DB���� �񱳽�Ų��.
	//					�񱳰� �����ϸ� �Ҹ��Ű��, Ŭ���̾�Ʈ���� �˸���. �񱳰� �����ϸ� �ߵ��� ��ų��
	//					��ҽ�ų ���̴�. void RSkillCheckUsable() ����
	{
		const ACTIVE_SKILL_INFO* skillInfo = pSkillInfo->GetSkillInfo();

		if( ! skillInfo )
		{
			return SKILL_INFO_INVALUED;
		}

		DWORD consumeFamilyPoint	= 0;
		DWORD consumeGuildPoint		= 0;
		DWORD consumeBadFame		= 0;
		DWORD consumeExperience		= 0;
		
		switch( skillInfo->mConsumePoint.mType )
		{
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeFamily:
			{
				consumeFamilyPoint = skillInfo->mConsumePoint.mValue;
				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeGuild:
			{
				consumeGuildPoint	= skillInfo->mConsumePoint.mValue;
				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypePlayerKill:
			{
				consumeBadFame		= skillInfo->mConsumePoint.mValue;
				break;
			}
		case ACTIVE_SKILL_INFO::ConsumePoint::TypeExperience:
			{
				consumeExperience	= skillInfo->mConsumePoint.mValue;
				break;
			}
		}

		// 081009 LUJ, ��ų�� ȣ�� ȸ���� ���� DB�� �δ��� ũ�Ƿ�, ��ȿ�� ���� ���� ���� ó���Ѵ�.
		if( pmsg->mConsumeItem.wIconIdx	||
			consumeBadFame			||
			consumeExperience		||
			consumeFamilyPoint		||
			consumeGuildPoint		||
			skillInfo->Money )
		{
			const ITEMBASE& consumeItem = pmsg->mConsumeItem;

			// 080602 LUJ, DB���� ó���� �����ϸ� ��ų�� ��ҽ�ų ���̴�
			//				������ Ÿ�ٿ� ��ų�� �����ϱ� ���ؼ��� �ݵ�� �� ���ν����� ȣ���ؾ��Ѵ�
			g_DB.FreeMiddleQuery(
				RSkillCheckUsable,
				skillObjectIndex,
				"EXEC DBO.MP_SKILL_CHECK_USABLE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
				pOperator->GetID(),
				skillInfo->Index,
				consumeItem.dwDBIdx,
				consumeItem.Position,
				skillInfo->mConsumeItem.Durability,
				ITEMMGR->IsDupItem( consumeItem.wIconIdx ),
				consumeFamilyPoint,
				consumeGuildPoint,
				consumeBadFame,
				consumeExperience,
				skillInfo->Money );
		}
	}
	
	/// ��ų ���� ����
	sSKILL_CREATE_INFO info;

	info.level = SkillLevel;
//	info.pOperator = pOperator;
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	

	/// ��ų ������Ʈ ����
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ��ų ��ü ID
	// 080602 LUJ, DB ������Ʈ ���� �� ��ų ������Ʈ�� ����� �� �ֵ��� ������ �ε����� �̸� �޾Ƴ����� �ߴ�
	info.skillObjectId = skillObjectIndex;
	
	/// ��ų ���� ��ġ
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos.x = TargetPos.x;
		info.pos.z = TargetPos.z;
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pOperator);
	}

	/// ��ų ��ü �ʱ�ȭ
	pSObj->SetMultiTarget( &( pmsg->TargetList ), pSkillInfo->GetSkillInfo()->AreaData );
	pSObj->Init( &info );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// ���̺� ���
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pOperator->SetCurrentSkill( pSObj->GetID() );

//---KES AUTONOTE	
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )	//---Ÿ���� ������ ��츸
				pOperator->SetLastActionTime( gCurTime );		
	}
//--------------

	// 080509 LUJ, ��ų ��Ÿ���� �����Ѵ�
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* player = ( CPlayer* )pOperator;

		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( skill )
		{
			player->SetCoolTime( *skill );
		}
	}

	/// ��ų ���� ����
	return SKILL_SUCCESS;
}

void cSkillManager::MonsterAttack(DWORD SkillIdx,CObject* pMonster,CObject* pTarget)
{
	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return;

	if( pMonster->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� ������ ���, ���� ������ ������ ��� ��ų�� �����Ѵ�
		if( pSkillInfo->GetSkillInfo()->Unit != UNITKIND_PHYSIC_ATTCK )
			return;
	}
	if( pMonster->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return;
	}
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pMonPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));


	// ��ų ������Ʈ ����
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ��ų ��ü ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pMonster;
	info.operatorId = pMonster->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = diridx;	
	info.remainTime = 0;
	/// ��ų ���� ��ġ
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos = *CCharMove::GetPosition(pTarget);
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pMonster);
	}
	
	// battle
	//info.pBattle = pMonster->GetBattle();

	// ���̺� ���
	pSObj->Init( &info );
	pSObj->SetBattle( pMonster->GetBattleID(), pMonster->GetBattleTeam() );
	pSObj->SetGridID( pMonster->GetGridID() );

	// 080708 LUJ, Ÿ�� ����� ��� ���� ����
	CTargetList targetList;
	targetList.Clear();

	// 080708 LUJ, ������ ���� Ÿ���� ������ ��� ���͸� ������ Ÿ�� ����Ʈ�� ���� ��,
	//				������ �ƴ� �÷��̾�� ����Ʈ���� �����ؾ��Ѵ�
	if( pMonster->GetObjectKind() == eObjectKind_Trap &&
		pSkillInfo->GetSkillInfo()->Area == TARGET_KIND_MULTI )
	{
		CUserTable::ObjectKindSet objectKindeSet;
		objectKindeSet.insert( eObjectKind_Player );
		objectKindeSet.insert( eObjectKind_Monster );
		objectKindeSet.insert( eObjectKind_BossMonster );
		objectKindeSet.insert( eObjectKind_FieldBossMonster );
		objectKindeSet.insert( eObjectKind_FieldSubMonster );

		CTargetList originalTargetList;
		originalTargetList.Clear();

		g_pUserTable->GetTargetInRange(
			CCharMove::GetPosition(pTarget),
			pSkillInfo->GetSkillInfo()->Range,
			&originalTargetList,
			pMonster->GetGridID(),
			0,
			objectKindeSet );

		CTrap* trap = ( CTrap* )pMonster;

		CTargetListIterator iterator( &originalTargetList );
		CTargetListIterator iteratorExceptFriendly( &targetList );

		while( iterator.GetNextTarget() )
		{
			CObject* object = g_pUserTable->FindUser( iterator.GetTargetID() );

			if( ! object )
			{
				continue;
			}
			// 080708 LUJ, �÷��̾�� ���� Ÿ������ ��´�
			else if(	object->GetObjectKind() == eObjectKind_Player	&&
					!	trap->IsEnemy( object->GetID() ) )
			{
				continue;
			}

			iteratorExceptFriendly.AddTargetWithNoData( object->GetID(), SKILLRESULTKIND_NONE );
		}

		iteratorExceptFriendly.Release();
	}
	else
	{
		switch( pSkillInfo->GetSkillInfo()->Area )
		{
		case TARGET_KIND_MULTI:
			{
				g_pUserTable->GetTargetInRange(
					CCharMove::GetPosition( pTarget ),
					pSkillInfo->GetSkillInfo()->Range,
					&targetList,
					pMonster->GetGridID() );
				break;
			}
			// 080708 LUJ, ���� ������� ���� Ÿ���� �⵵�� �߰�. ��ü ������ ��� ���͸� �����ؾ� �ϴµ�
			//				����� ������ �׻� ���ͷ� �Ǿ� �־� AreaTarget�� ����ϱ� �����.
			//				�÷��̾ Ÿ�ٿ� ���� �ʴ� ������, ���� ��ų ��ü�� �Ʊ�/���� �ĺ��� �� �� ����
			//				���� ���� ��� �÷��̾ �����ϱ� ����.
		case TARGET_KIND_MULTI_MONSTER:
			{
				CUserTable::ObjectKindSet objectKindeSet;
				objectKindeSet.insert( eObjectKind_Monster );
				objectKindeSet.insert( eObjectKind_BossMonster );
				objectKindeSet.insert( eObjectKind_FieldBossMonster );
				objectKindeSet.insert( eObjectKind_FieldSubMonster );

				g_pUserTable->GetTargetInRange(
					CCharMove::GetPosition( pTarget ),
					pSkillInfo->GetSkillInfo()->Range,
					&targetList,
					pMonster->GetGridID(),
					0,
					objectKindeSet );
				break;
			}
		}
	}
	
	pSObj->SetMultiTarget( &targetList , pSkillInfo->GetSkillInfo()->AreaData);

	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pMonster->SetCurrentSkill( pSObj->GetID() );
}

BOOL cSkillManager::PetAttack(DWORD SkillIdx,CObject* pPet,CObject* pTarget)
{
	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return SKILL_INFO_INVALUED;

	if( pPet->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� ������ ���, ���� ������ ������ ��� ��ų�� �����Ѵ�
		if( pSkillInfo->GetSkillInfo()->Unit != UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}
	if( pPet->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}
	
	VECTOR3* pPetPos = CCharMove::GetPosition(pPet);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pPetPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));

	float dist = CalcDistanceXZ( pPetPos, pTargetPos ) - pTarget->GetRadius();

	if( dist > pSkillInfo->GetSkillInfo()->Range )
		return SKILL_RANGE_NOTENOUGH;

	// ��ų ������Ʈ ����
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ��ų ��ü ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pPet;
	info.operatorId = pPet->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = diridx;	
	info.remainTime = 0;
	/// ��ų ���� ��ġ
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos = *CCharMove::GetPosition(pTarget);
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pPet);
	}
	
	// battle
	//info.pBattle = pMonster->GetBattle();

	// ���̺� ���
	pSObj->Init( &info );
	pSObj->SetBattle( pPet->GetBattleID(), pPet->GetBattleTeam() );
	pSObj->SetGridID( pPet->GetGridID() );
/*	if( pSkillInfo->GetSkillInfo()->Area == 1 )
	{
		CTargetList TList;
		//CObject* pObject;
		TList.Clear();
		VECTOR3 TargetPos = *CCharMove::GetPosition(pTarget);
		//g_pUserTable->GetTargetInRange( &TargetPos, pSkillInfo->GetSkillInfo()->Range, &TList );
		// 080604 KTH -- �Լ��� �׸��� �߰�
		g_pUserTable->GetTargetInRange( &TargetPos, pSkillInfo->GetSkillInfo()->Range, &TList, pPet->GetGridID() );

		pSObj->SetMultiTarget(&TList);
	}
*/	
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pPet->SetCurrentSkill( pSObj->GetID() );

	return SKILL_SUCCESS;
}

cSkillObject* cSkillManager::OnBuffSkillStart( cSkillInfo* pSkillInfo, sSKILL_CREATE_INFO* pCreateInfo )
{
	CObject* pOperator = g_pUserTable->FindUser( pCreateInfo->operatorId );

	if( !pOperator )
		return NULL;
	
	/// ��ų ������Ʈ ����
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ��ų ��ü ID
	pCreateInfo->skillObjectId = GetNewSkillObjectID();

	/// ��ų ��ü �ʱ�ȭ
	pSObj->Init( pCreateInfo );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// ���̺� ���
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &( pCreateInfo->pos ) );

	return pSObj;
}

void cSkillManager::BuffSkillStart( DWORD characteridx, DWORD skillidx, DWORD remaintime, WORD count )
{
	CObject* pTarget = g_pUserTable->FindUser( characteridx );

	if( !pTarget )
	{
		return;
	}

	cSkillInfo* pSkillInfo = GetSkillInfo( skillidx );
	sSKILL_CREATE_INFO createinfo;

	createinfo.level = pSkillInfo->GetLevel();
	createinfo.operatorId = characteridx;
//	createinfo.pOperator = pTarget;
	createinfo.mainTarget.SetMainTarget( characteridx );
	//createinfo.pBattle = pTarget->GetBattle();
	createinfo.pos = *CCharMove::GetPosition( pTarget );
	createinfo.skillDir = 0;
	createinfo.skillObjectId = 0;
	createinfo.remainTime = remaintime;
	createinfo.count = count;

	OnBuffSkillStart( pSkillInfo, &createinfo );
}

void cSkillManager::AddPassive( CPlayer* pTarget, DWORD SkillIdx )
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

		if( !pSkillInfo )
		{
			continue;
		}

		if( pSkillInfo->GetSkillInfo()->ActiveRule )
		{
			pTarget->AddSpecialSkill( pSkillInfo );
		}
		else
		{
			pSkillInfo->AddPassiveStatus( pTarget );
		}
	}
}

void cSkillManager::RemovePassive( CPlayer* pTarget, DWORD SkillIdx )
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
			pTarget->RemoveSpecialSkill( pSkillInfo );
		}
		else
		{
			pSkillInfo->RemovePassiveStatus( pTarget );
		}
	}
}

void cSkillManager::ReleaseSkillObject( cSkillObject* pSkillObject, int ReleaseKind )
{
	switch( ReleaseKind )
	{
	/// ��ų ������Ʈ �޸� ����
	case SKILL_RELEASE_DELETE:
		{
			/// �����Ϸ��� ��ų�� ã�ƺ���
			cSkillObject* ptemp = m_SkillObjectTable.GetData( pSkillObject->GetID() );

			/// ���ڰ����� ���� ��ų�� �ٸ� ��ų�̸� �����ִ�
			if( ptemp != pSkillObject )
			{
				ASSERT(0);
				return;
			}

			/// ��ų�� �����Ѵ�
			m_SkillObjectTable.Remove( pSkillObject->GetID() );
			g_pServerSystem->RemoveSkillObject( pSkillObject->GetID() );
			
			/// �޸� ����
			delete pSkillObject;
		}
		break;
	/// ��ų ������Ʈ ��Ȱ��
	case SKILL_RELEASE_RECYCLE:
		{
			/// ��ų ������ �����´�
			cSkillInfo* pSInfo = GetSkillInfo( pSkillObject->GetSkillIdx() );

			/// ��ų ������ ���ٸ� �޸� ���� �ع�����.
			if(!pSInfo)
			{
				ASSERT(0);
				ReleaseSkillObject(pSkillObject, SKILL_RELEASE_DELETE);
				return;
			}

			/// ��ų�� �����Ѵ�
			m_SkillObjectTable.Remove( pSkillObject->GetID() );
			g_pServerSystem->RemoveSkillObject( pSkillObject->GetID() );

			/// ��ų ��ü�� ��ų ������ �����ش�
			pSInfo->ReleaseSkillObject( pSkillObject );	
		}
		break;
	}
}

void cSkillManager::ReleaseAllSkillInBattle(DWORD BattleID)
{
}

DWORD cSkillManager::GetNewSkillObjectID()
{
	static tempID = SKILLOBJECT_ID_START;
	if(++tempID >= SKILLOBJECT_ID_MAX)
		tempID = SKILLOBJECT_ID_START;
	return tempID;
}


DWORD cSkillManager::GetSkillSize( DWORD index ) const
{
	SkillSize::const_iterator it = mSkillSize.find( index );

	return mSkillSize.end() == it ? 0 : it->second;
}


cSkillManager* cSkillManager::GetInstance()
{
	static cSkillManager instance;

	return &instance;
}


void cSkillManager::OnSkillTargetUpdate( MSG_SKILL_UPDATE_TARGET* pmsg )
{
	cSkillObject* pSkillObject = m_SkillObjectTable.GetData( pmsg->SkillObjectID );
/*
	if( pSkillObject )
	{
		pSkillObject->SetMultiTarget( &( pmsg->TargetList ) );;
	}
*/
	if(0 == pSkillObject)
	{
		return;
	}
	cActiveSkillInfo* skillInfo = ( cActiveSkillInfo* )GetSkillInfo( pmsg->IDx );
	const ACTIVE_SKILL_INFO* IDx = skillInfo->GetSkillInfo();
	//cSkillInfo* Info = m_SkillInfoTable.GetData( pmsg->Range );
	if(0 == IDx)
	{
		return;
	}
	if(IDx->AreaData != pmsg->Range)
	{
		return;
	}

	CObject* const object = pSkillObject->GetOperator();

	// 100115 LUJ, �ٸ� �÷��̾ ���� Ÿ���� �ٲ��� �ʵ��� �˻縦 ö���� �Ѵ�
	if(0 == object)
	{
		return;
	}
	else if(eObjectKind_Player != object->GetObjectKind())
	{
		return;
	}
	else if(pmsg->dwObjectID != object->GetID())
	{
		return;
	}

	pSkillObject->SetMultiTarget(&(pmsg->TargetList), (pmsg->Range));
}

void cSkillManager::OnSkillCancel( MSG_DWORD* pmsg )
{
	CObject* pOperator = g_pUserTable->FindUser( pmsg->dwObjectID );

	if( !pOperator )
	{
		return;
	}

	cSkillObject* pSkillObject = m_SkillObjectTable.GetData( pmsg->dwData );

	if( !pSkillObject )
	{
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_CANCEL_NOTIFY;
	msg.dwObjectID = pmsg->dwObjectID;
	msg.dwData = pmsg->dwData;

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pOperator, &msg, sizeof( MSG_DWORD ) );

	pSkillObject->SetEndState();
	pSkillObject->EndState();
}

// 080616 LUJ, ��Ƽ�� ��ų�� �߰��Ѵ�
void cSkillManager::AddActiveSkill( CObject& object, DWORD skillIndex )
{
	cActiveSkillInfo* skillInfo = ( cActiveSkillInfo* )GetSkillInfo( skillIndex );

	if( ! skillInfo )
	{
		return;
	}

	cSkillObject* skillObject = skillInfo->GetSkillObject();

	if( ! skillObject )
	{
		return;
	}

	/// ��ų ���� ����
	sSKILL_CREATE_INFO createInfo;
	ZeroMemory( &createInfo, sizeof( createInfo ) );
	createInfo.skillObjectId	= skillObject->GetID();
	createInfo.level			= skillInfo->GetLevel();
//	createInfo.pOperator		= &object;
	createInfo.operatorId		= object.GetID();
	createInfo.mainTarget.SetMainTarget( object.GetID() );

	/// ��ų ���� ��ġ
	if( skillInfo->GetSkillInfo()->Target )
	{
		VECTOR3 position = { 0 };
		object.GetPosition( &position );

		createInfo.pos.x = position.x;
		createInfo.pos.z = position.z;
	}
	else
	{
		createInfo.pos = *CCharMove::GetPosition( &object );
	}

	CTargetList targetList;
	// 080708 LUJ, �ݵ�� �ʱ�ȭ�� �ʿ��ϴ�
	targetList.Clear();
	
	// 080616 LUJ, Ÿ���� �����Ѵ�
	{
		const ACTIVE_SKILL_INFO* info = skillInfo->GetSkillInfo();

		if( ! info )
		{
			return;
		}

		switch( info->Area )
		{
		case TARGET_KIND_SINGLE:
		case TARGET_KIND_SINGLE_CIRCLE:
			{
				CTargetListIterator iter( &targetList );

				iter.AddTargetWithNoData( object.GetID(), SKILLRESULTKIND_NONE );
				// 080708 LUJ, Release()�� ȣ��Ǿ�� Ÿ�� ���� ���� ���̰� �����ȴ�
				iter.Release();
				break;
			}
		case TARGET_KIND_MULTI:
		case TARGET_KIND_MULTI_ARC:
		case TARGET_KIND_MULTI_CIRCLE:
		case TARGET_KIND_MULTI_RECT:
			{
				VECTOR3 position = { 0 };
				object.GetPosition( &position );

				g_pUserTable->GetTargetInRange(
					&position,
					info->AreaData,
					&targetList,
					object.GetGridID() );
				break;
			}
		case TARGET_KIND_MULTI_MONSTER:
			{
				CUserTable::ObjectKindSet objectKindeSet;
				objectKindeSet.insert( eObjectKind_Monster );
				objectKindeSet.insert( eObjectKind_BossMonster );
				objectKindeSet.insert( eObjectKind_FieldBossMonster );
				objectKindeSet.insert( eObjectKind_FieldSubMonster );

				VECTOR3 position = { 0 };
				object.GetPosition( &position );

				g_pUserTable->GetTargetInRange(
					&position,
					info->AreaData,
					&targetList,
					object.GetGridID(),
					0,
					objectKindeSet );
				break;
			}
		}
	}

	/// ��ų ��ü �ʱ�ȭn
	skillObject->SetMultiTarget( &targetList , skillInfo->GetSkillInfo()->AreaData);
	skillObject->Init( &createInfo );
	skillObject->SetBattle( object.GetBattleID(), object.GetBattleTeam() );
	skillObject->SetGridID( object.GetGridID() );

	/// ���̺� ���
	m_SkillObjectTable.Add( skillObject, skillObject->GetID() );
	g_pServerSystem->AddSkillObject( skillObject, &createInfo.pos );
}

SKILL_RESULT cSkillManager::OnPetSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase)
{
	/// ������ ������ ���ٸ� ����
	// 080602 LUJ, ��ų �ߵ��� �÷��̾ �� �� �����Ƿ� ĳ�����Ѵ�
	CPet* pOperator = ( CPet* )g_pUserTable->FindUser( pmsg->Operator );

	// 080602 LUJ, �˻� ���� �߰�
	if( !	pOperator											||
			pOperator->GetObjectKind()	!= eObjectKind_Pet	||
			pOperator->CurCastingSkillID )
	{
		return SKILL_OPERATOR_INVALUED;
	}

	// 080410 KTH -- �Ϲ� ��ų�� ��� ��ų Ʈ���� ���� �Ǿ��ִ� ��ų���� Ȯ���Ѵ�.
	if( pItemBase )
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );

		if( pSkillInfo == NULL || pItemInfo == NULL )
			return SKILL_INFO_INVALUED;

		if( pmsg->SkillIdx / 100 != pItemInfo->SupplyValue / 100 )
			return SKILL_INFO_INVALUED;
	}

	/// ��ų ������ ���ٸ� ����
	cSkillInfo* ptemp = GetSkillInfo( pmsg->SkillIdx );

	ASSERT( ptemp );
	if( ptemp == NULL )
	{
		return SKILL_INFO_INVALUED;
	}
	/// ��ų ���� �Ǵ�
	WORD skillKind = ptemp->GetKind();
//	if( skillKind == SKILLKIND_BUFF || skillKind == SKILLKIND_PASSIVE )
//	{
//		return SKILL_INFO_INVALUED;
//	}

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )ptemp;

	if( pOperator->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, ħ�� �߿��� ���� ���ݸ� �����ϴ�
		if( skillKind != SKILLKIND_PHYSIC )
		{
			return SKILL_STATE_INVALUED;
		}
	}
	if( pOperator->GetAbnormalStatus()->IsBlockAttack )
	{
		if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_PHYSIC_ATTCK )
			return SKILL_STATE_INVALUED;
	}
/*
	// 080616 LUJ, �̹� ������ ������ ������ �� ������ �Ѵ�
	if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_TRAP &&
		pOperator->IsHaveTrap( pSkillInfo->GetSkillInfo()->UnitData ) )
	{
		return SKILL_COOLING;
	}
*//*
	// 080509 LUJ, �÷��̾ ��Ÿ�� ���� ��ų ��û�� �ϴ��� üũ�Ѵ�
	{
		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( !	skill ||
				pOperator->IsCoolTime( *skill ) )
		{
			return SKILL_COOLING;
		}
	}
*/
	int SkillLevel = pSkillInfo->GetSkillInfo()->Level;

	/// ���� ���ӽ� ���� ���� ����
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );

	/// ��� ��ʸ�Ʈ�� ���������� �����ڴ� ��ų�� ����� �� ����
	if( pOperator->GetMaster()->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetMaster()->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetMaster()->GetBattleTeam() == 2 )			
			return SKILL_STATE_INVALUED;
	}

	/// ��ų�� ����Ҽ� �ִ� �������� Ȯ��
	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel) == FALSE)
	{
		return SKILL_STATE_INVALUED;
	}

	VECTOR3 TargetPos;
	CObject * pTargetObject = NULL;
	float TargetRadius;

	/// Ÿ�� ���� Ȯ��
	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		/// Ÿ���� ����
		return SKILL_TARGET_INVALUED;
	}

	switch( pSkillInfo->GetSkillInfo()->Target )
	{
	case eSkillTargetKind_None:
		{
			return SKILL_TARGET_INVALUED;
		}
		break;
	case eSkillTargetKind_OnlyOthers:
		{
			if( pTargetObject == pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_Ground:
		{
		}
		break;
	case eSkillTargetKind_OnlySelf:
		{
			if( pTargetObject != pOperator )
			{
				return SKILL_TARGET_INVALUED;
			}
		}
		break;
	case eSkillTargetKind_AllObject:
		{
		}
		break;
	}

	/// Ÿ�� ������Ʈ�� ũ�⸦ ���Ѵ�
	if(pmsg->MainTarget.MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
		TargetRadius = 0;
	else
	{
		pTargetObject = g_pUserTable->FindUser( pmsg->MainTarget.dwMainTargetID );
		TargetRadius = pTargetObject->GetRadius();
	}	
	/*if( pSkillInfo->IsInRange(*pOperator, TargetPos, TargetRadius) == FALSE)
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo = {0};
			DamageInfo.mSkillIndex = pSkillInfo->GetIndex();

			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		return SKILL_RANGE_NOTENOUGH;
	}*/
	/// �����Ÿ� �Ǵ�
	if( pSkillInfo->IsInSkillRange( pOperator, &TargetPos, TargetRadius ) == FALSE )
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo;
			DamageInfo.Clear();
			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		return SKILL_RANGE_NOTENOUGH;
	}
	
	/// �̹� ����ڰ� ����� ��ų�� �ִ� ���
	
	/// On/Off�� ��ų�� ���
	if( skillKind == SKILLKIND_ONOFF )
	{
		/// �̹� ����ڿ��� �ɷ��ִ� ��ų�̸� �� ��ų�� �����Ѵ�
/*		cSkillObject* pSkill = pTargetObject->GetOnOffSkillList()->GetData( pmsg->SkillIdx );
		
		if( pSkill )
		{
			cOnOffSkillObject* pSkillObject = ( cOnOffSkillObject* )pSkill;
			pSkillObject->Destroy();

			return SKILL_DESTROY;
		}
*/	
		BOOL destroy = FALSE;

		for( WORD i = 0; i < MAX_BUFF_COUNT; i++ )
		{
			DWORD idx = pSkillInfo->GetSkillInfo()->Buff[ i ];
			idx = idx / 100;

			if( idx )
			{
				cSkillObject* pSkill = pTargetObject->GetBuffSkillList()->GetData( idx );

				if( pSkill )
				{
					if( pSkill->GetSkillIdx() <= pSkillInfo->GetSkillInfo()->Buff[ i ] )
					{
						pSkill->SetEndState();
						pSkill->EndState();
						destroy = TRUE;
					}
				}
			}
		}
		
		if( destroy )
		{
			return SKILL_DESTROY;
		}
	}

	// 080602 LUJ, ���� ��ų�� DB���� ���� ó���� �� ���۵ȴ�. ��� ���� ���� ������Ʈ ID�� ���� �޾ƿ´�
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	/// ��ų ���� ����
	sSKILL_CREATE_INFO info;

	info.level = SkillLevel;
//	info.pOperator = pOperator;
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	

	/// ��ų ������Ʈ ����
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// ��ų ��ü ID
	// 080602 LUJ, DB ������Ʈ ���� �� ��ų ������Ʈ�� ����� �� �ֵ��� ������ �ε����� �̸� �޾Ƴ����� �ߴ�
	info.skillObjectId = skillObjectIndex;
	
	/// ��ų ���� ��ġ
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos.x = TargetPos.x;
		info.pos.z = TargetPos.z;
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pOperator);
	}

	/// ��ų ��ü �ʱ�ȭ
	pSObj->SetMultiTarget( &( pmsg->TargetList ), pSkillInfo->GetSkillInfo()->AreaData );
	pSObj->Init( &info );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// ���̺� ���
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pOperator->SetCurrentSkill( pSObj->GetID() );
/*
//---KES AUTONOTE	
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )	//---Ÿ���� ������ ��츸
				pOperator->SetLastActionTime( gCurTime );		
	}
//--------------
*//*
	// 080509 LUJ, ��ų ��Ÿ���� �����Ѵ�
	{
		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( skill )
		{
			pOperator->SetCoolTime( *skill );
		}
	}
*/
	/// ��ų ���� ����
	return SKILL_SUCCESS;
}
BOOL cSkillManager::IsLearnableSkill( const CHARACTER_TOTALINFO& totalInfo, const SKILL_BASE& skillBase )
{
	// 090106 LUJ, grade ���� ����� �� �����ϵ��� ����
	for(	WORD grade = totalInfo.JobGrade;
			0 < grade;
			--grade )
	{
		const DWORD jobType		= DWORD( totalInfo.Job[ grade - 1 ] );
		// 090106 LUJ, ������ 1�ܰ��� ��� jobType�� �׻� 1�� �Ǿ���Ѵ�
		const DWORD jobIndex	= ( totalInfo.Job[ 0 ] * 1000 ) + ( ( totalInfo.Race + 1 ) * 100 ) + ( grade * 10 ) + ( 1 == grade ? 1 : jobType );
		// 090105 LUJ, ���� ��ȣ�� �̿��ؼ� ��ų �н� Ʈ���� �����͸� ȹ���Ѵ�
		const SKILL_TREE_INFO* const skillTree = m_SkillTreeTable.GetData( jobIndex );

		if( ! skillTree )
		{
			break;
		}

		// 090105 LUJ, ��ų Ʈ���� ����� ��ų �� Ŭ���̾�Ʈ ��û�� ������ ������ �ִ��� �˻��Ѵ�
		for(	DWORD i = 0;
				i < skillTree->Count;
				++i )
		{
			const DWORD serverSkillIndex = skillTree->SkillIndex[ i ] / 100;
			const DWORD clientSkillIndex = skillBase.wSkillIdx / 100;

			if( serverSkillIndex != clientSkillIndex )
			{
				continue;
			}

			const DWORD serverSkillLevel = skillTree->SkillIndex[ i ] % 100;
			// 090106 LUJ, ���ʿ��� ���� ����
			const DWORD clientSkillLevel = skillBase.Level;

			if( serverSkillLevel >= clientSkillLevel )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
