#include "stdafx.h"
#include ".\skillmanager.h"
#include "../Info/ActiveSkillInfo.h"
#include "../Info/BuffSkillInfo.h"
#include "../Object/OnOffSkillObject.h"
#include "../Delay/Delay.h"

#include "UserTable.h"

#include "Player.h"
// 080708 LUJ, 광역 스킬을 사용하는 함정 처리를 위해 참조
#include "Trap.h"

#include "CharMove.h"
#include "PackedData.h"

#include "BattleSystem_Server.h"
#include "MapDBMsgParser.h"
#include "SkillTreeManager.h"

#include "ObjectStateManager.h"

#include "ItemManager.h"
// 080602 LUJ, 전서버 대상 스킬 기능위해 참조
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
	/// Hash 테이블 초기화
	m_SkillInfoTable.Initialize(1000);
	m_SkillObjectTable.Initialize(10000);
	m_SkillTreeTable.Initialize(64);

	/// 스킬 리스트 로딩
	LoadSkillInfoList();
}

void cSkillManager::Release()
{
	/// 스킬 오브젝트 삭제
	cSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while(pObject = m_SkillObjectTable.GetData())
	{
		ReleaseSkillObject(pObject,SKILL_RELEASE_DELETE);
	}
	m_SkillObjectTable.RemoveAll();

	/// 스킬 정보 삭제
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

	file.Init("System/Resource/skill_exception.bin","rb");

	if(file.IsInited() == FALSE)
	{
		//ASSERTMSG(0,"SkillList를 로드하지 못했습니다.");
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

	/// 반환값
	DWORD result;
	
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		/// 스킬 오브젝트 업데이트
		result = pSObj->Update();

		/// 스킬 해제 대상
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

			// 080610 LUJ, 스킬 실패 시 메시지를 반환하도록 함
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
	// 080602 LUJ, 전서버 대상 스킬이 전파됨
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

			// 080602 LUJ, 스킬 대상에 따라 적절한 집단을 선택한다
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
					// 080602 LUJ, 패밀리는 맵 서버에 데이타가 없고, 에이전트에 있다. 번거롭지만
					//				DB에 회원 목록을 쿼리해서 버프를 적용하도록 하자.
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

			// 080602 LUJ, 선택된 집단을 대상으로 버프 스킬을 적용한다
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

// 080602 LUJ, 버프 스킬을 적용한다
// 080616 LUJ, CPlayer -> CObject로 변경
// 080602 LUJ, 버프 스킬을 적용한다
// 080616 LUJ, CPlayer -> CObject로 변경
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

		// 080602 LUJ, DelayTime을 최소 1이상 준 이유: CBuffSkillObject::ProcessStart()의 버프 스킬 생성 부분에서 참조함
		BuffSkillStart(
			object.GetID(),
			buff->Index,
			buff->DelayTime ? buff->DelayTime : 1,
			buff->Count );

		// 080602 LUJ, DelayTime을 최소 1이상 준 이유: CBuffSkillObject::ProcessStart()의 버프 스킬 생성 부분에서 참조함
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
	/// 시전자 정보가 없다면 실패
	// 080602 LUJ, 스킬 발동은 플레이어만 할 수 있으므로 캐스팅한다
	CPlayer* pOperator = ( CPlayer* )g_pUserTable->FindUser( pmsg->Operator );

	// 080602 LUJ, 검사 구문 추가
	if( !	pOperator											||
			pOperator->GetObjectKind()	!= eObjectKind_Player	||
			pOperator->CurCastingSkillID )
	{
		return SKILL_OPERATOR_INVALUED;
	}

	// 080410 KTH -- 일반 스킬일 경우 스킬 트리에 포함 되어있는 스킬인지 확인한다.
	if( pItemBase == NULL )
	{
		// 일반 공격이 아닐 경우에만 아래 코드를 실행한다.
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
	// 080410 KTH -- 아이템 사용으로 스킬을 사용할 경우 아이템의 정보를 확인한다.
	else
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );

		if( pSkillInfo == NULL || pItemInfo == NULL )
			return SKILL_OPERATOR_INVALUED;

		if( pmsg->SkillIdx != pItemInfo->SupplyValue )
			return SKILL_OPERATOR_INVALUED;
	}

	/// 스킬 정보가 없다면 실패
	cSkillInfo* ptemp = GetSkillInfo( pmsg->SkillIdx );

	ASSERT( ptemp );
	if( ptemp == NULL )
	{
		return SKILL_INFO_INVALUED;
	}
	/// 스킬 종류 판단
	WORD skillKind = ptemp->GetKind();
//	if( skillKind == SKILLKIND_BUFF || skillKind == SKILLKIND_PASSIVE )
//	{
//		return SKILL_INFO_INVALUED;
//	}

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )ptemp;

	if( pOperator->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, 침묵 중에는 물리 공격만 가능하다
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

	// 080509 LUJ, 플레이어가 쿨타임 전에 스킬 요청을 하는지 체크한다
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
	// 080616 LUJ, 이미 보유한 함정은 생성할 수 없도록 한다
	if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_TRAP &&
		pOperator->IsHaveTrap( pSkillInfo->GetSkillInfo()->UnitData ) )
	{
		return SKILL_COOLING;
	}
	/// 스킬 레벨 판단
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

	/// 최초 접속시 무적 상태 해제
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );

	/// 길드 토너먼트나 공성전에서 관람자는 스킬을 사용할 수 없다
	if( pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetBattleTeam() == 2 )			
			return SKILL_STATE_INVALUED;
	}

	/// 스킬을 사용할수 있는 상태인지 확인
	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel) == FALSE)
	{
		return SKILL_STATE_INVALUED;
	}

	VECTOR3 TargetPos;
	CObject * pTargetObject = NULL;
	float TargetRadius;

	/// 타겟 설정 확인
	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		/// 타겟이 없다
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

	/// 타겟 오브젝트의 크기를 구한다
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
	/// 사정거리 판단
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
	
	/// 이미 사용자가 사용한 스킬이 있는 경우
	
	/// On/Off형 스킬의 경우
	if( skillKind == SKILLKIND_ONOFF )
	{
		/// 이미 사용자에게 걸려있는 스킬이면 그 스킬을 해제한다
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

	// 080602 LUJ, 전역 스킬은 DB에서 직접 처리한 후 전송된다. 취소 등을 위해 오브젝트 ID를 직접 받아온다
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	// 080602 LUJ, 스킬 사용 시 소모 처리.
	//
	//		처리 개요:	일단 스킬을 발동시킨 후, 전서버에서 변경되는 포인트는 DB에서 비교시킨다.
	//					비교가 성공하면 소모시키고, 클라이언트에도 알린다. 비교가 실패하면 발동된 스킬을
	//					취소시킬 것이다. void RSkillCheckUsable() 참조
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

		// 081009 LUJ, 스킬의 호출 회수가 많아 DB에 부담이 크므로, 유효한 값이 있을 때만 처리한다.
		if( pmsg->mConsumeItem.wIconIdx	||
			consumeBadFame			||
			consumeExperience		||
			consumeFamilyPoint		||
			consumeGuildPoint		||
			skillInfo->Money )
		{
			const ITEMBASE& consumeItem = pmsg->mConsumeItem;

			// 080602 LUJ, DB에서 처리가 실패하면 스킬을 취소시킬 것이다
			//				적절한 타겟에 스킬을 적용하기 위해서는 반드시 이 프로시저를 호출해야한다
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
	
	/// 스킬 생성 정보
	sSKILL_CREATE_INFO info;

	info.level = SkillLevel;
//	info.pOperator = pOperator;
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	

	/// 스킬 오브젝트 생성
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 스킬 객체 ID
	// 080602 LUJ, DB 업데이트 실패 시 스킬 오브젝트를 취소할 수 있도록 위에서 인덱스를 미리 받아놓도록 했다
	info.skillObjectId = skillObjectIndex;
	
	/// 스킬 생성 위치
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos.x = TargetPos.x;
		info.pos.z = TargetPos.z;
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pOperator);
	}

	/// 스킬 객체 초기화
	pSObj->SetMultiTarget( &( pmsg->TargetList ), pSkillInfo->GetSkillInfo()->AreaData );
	pSObj->Init( &info );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// 테이블에 등록
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pOperator->SetCurrentSkill( pSObj->GetID() );

//---KES AUTONOTE	
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )	//---타겟이 몬스터일 경우만
				pOperator->SetLastActionTime( gCurTime );		
	}
//--------------

	// 080509 LUJ, 스킬 쿨타임을 저장한다
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* player = ( CPlayer* )pOperator;

		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( skill )
		{
			player->SetCoolTime( *skill );
		}
	}

	/// 스킬 생성 성공
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
		// 080616 LUJ, 침묵 상태인 경우, 물리 공격을 제외한 모든 스킬을 차단한다
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


	// 스킬 오브젝트 생성
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 스킬 객체 ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pMonster;
	info.operatorId = pMonster->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = diridx;	
	info.remainTime = 0;
	/// 스킬 생성 위치
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

	// 테이블에 등록
	pSObj->Init( &info );
	pSObj->SetBattle( pMonster->GetBattleID(), pMonster->GetBattleTeam() );
	pSObj->SetGridID( pMonster->GetGridID() );

	// 080708 LUJ, 타겟 목록을 담기 위해 선언
	CTargetList targetList;
	targetList.Clear();

	// 080708 LUJ, 함정이 광역 타겟을 설정할 경우 몬스터를 포함한 타겟 리스트를 얻은 후,
	//				적군이 아닌 플레이어는 리스트에서 제외해야한다
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
			// 080708 LUJ, 플레이어는 적만 타겟으로 잡는다
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
			// 080708 LUJ, 몬스터 대상으로 광역 타겟을 잡도록 추가. 시체 폭파의 경우 몬스터를 공격해야 하는데
			//				현재는 적군이 항상 몬스터로 되어 있어 AreaTarget을 사용하기 곤란함.
			//				플레이어를 타겟에 넣지 않는 이유는, 폭파 스킬 자체는 아군/적군 식별을 할 수 없어
			//				영역 안의 모든 플레이어를 공격하기 때문.
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
		// 080616 LUJ, 침묵 상태인 경우, 물리 공격을 제외한 모든 스킬을 차단한다
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

	// 스킬 오브젝트 생성
	sSKILL_CREATE_INFO info;
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 스킬 객체 ID
	info.skillObjectId = GetNewSkillObjectID();

	info.level = 1;
//	info.pOperator = pPet;
	info.operatorId = pPet->GetID();
	info.mainTarget.SetMainTarget( pTarget->GetID() );
	info.skillDir = diridx;	
	info.remainTime = 0;
	/// 스킬 생성 위치
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

	// 테이블에 등록
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
		// 080604 KTH -- 함수에 그리드 추가
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
	
	/// 스킬 오브젝트 생성
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 스킬 객체 ID
	pCreateInfo->skillObjectId = GetNewSkillObjectID();

	/// 스킬 객체 초기화
	pSObj->Init( pCreateInfo );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// 테이블에 등록
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
	/// 스킬 오브젝트 메모리 해제
	case SKILL_RELEASE_DELETE:
		{
			/// 삭제하려는 스킬을 찾아본다
			cSkillObject* ptemp = m_SkillObjectTable.GetData( pSkillObject->GetID() );

			/// 인자값으로 받은 스킬과 다른 스킬이면 문제있다
			if( ptemp != pSkillObject )
			{
				ASSERT(0);
				return;
			}

			/// 스킬을 삭제한다
			m_SkillObjectTable.Remove( pSkillObject->GetID() );
			g_pServerSystem->RemoveSkillObject( pSkillObject->GetID() );
			
			/// 메모리 해제
			delete pSkillObject;
		}
		break;
	/// 스킬 오브젝트 재활용
	case SKILL_RELEASE_RECYCLE:
		{
			/// 스킬 정보를 가져온다
			cSkillInfo* pSInfo = GetSkillInfo( pSkillObject->GetSkillIdx() );

			/// 스킬 정보가 없다면 메모리 해제 해버린다.
			if(!pSInfo)
			{
				ASSERT(0);
				ReleaseSkillObject(pSkillObject, SKILL_RELEASE_DELETE);
				return;
			}

			/// 스킬을 삭제한다
			m_SkillObjectTable.Remove( pSkillObject->GetID() );
			g_pServerSystem->RemoveSkillObject( pSkillObject->GetID() );

			/// 스킬 객체는 스킬 정보에 돌려준다
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

	// 100115 LUJ, 다른 플레이어에 의해 타겟이 바뀌지 않도록 검사를 철저히 한다
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

// 080616 LUJ, 액티브 스킬을 추가한다
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

	/// 스킬 생성 정보
	sSKILL_CREATE_INFO createInfo;
	ZeroMemory( &createInfo, sizeof( createInfo ) );
	createInfo.skillObjectId	= skillObject->GetID();
	createInfo.level			= skillInfo->GetLevel();
//	createInfo.pOperator		= &object;
	createInfo.operatorId		= object.GetID();
	createInfo.mainTarget.SetMainTarget( object.GetID() );

	/// 스킬 생성 위치
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
	// 080708 LUJ, 반드시 초기화가 필요하다
	targetList.Clear();
	
	// 080616 LUJ, 타겟을 설정한다
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
				// 080708 LUJ, Release()가 호출되어야 타겟 내의 버퍼 길이가 설정된다
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

	/// 스킬 객체 초기화n
	skillObject->SetMultiTarget( &targetList , skillInfo->GetSkillInfo()->AreaData);
	skillObject->Init( &createInfo );
	skillObject->SetBattle( object.GetBattleID(), object.GetBattleTeam() );
	skillObject->SetGridID( object.GetGridID() );

	/// 테이블에 등록
	m_SkillObjectTable.Add( skillObject, skillObject->GetID() );
	g_pServerSystem->AddSkillObject( skillObject, &createInfo.pos );
}

SKILL_RESULT cSkillManager::OnPetSkillStartSyn(MSG_SKILL_START_SYN* pmsg, ITEMBASE* pItemBase)
{
	/// 시전자 정보가 없다면 실패
	// 080602 LUJ, 스킬 발동은 플레이어만 할 수 있으므로 캐스팅한다
	CPet* pOperator = ( CPet* )g_pUserTable->FindUser( pmsg->Operator );

	// 080602 LUJ, 검사 구문 추가
	if( !	pOperator											||
			pOperator->GetObjectKind()	!= eObjectKind_Pet	||
			pOperator->CurCastingSkillID )
	{
		return SKILL_OPERATOR_INVALUED;
	}

	// 080410 KTH -- 일반 스킬일 경우 스킬 트리에 포함 되어있는 스킬인지 확인한다.
	if( pItemBase )
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItemBase->wIconIdx );
		cSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo( pmsg->SkillIdx );

		if( pSkillInfo == NULL || pItemInfo == NULL )
			return SKILL_INFO_INVALUED;

		if( pmsg->SkillIdx / 100 != pItemInfo->SupplyValue / 100 )
			return SKILL_INFO_INVALUED;
	}

	/// 스킬 정보가 없다면 실패
	cSkillInfo* ptemp = GetSkillInfo( pmsg->SkillIdx );

	ASSERT( ptemp );
	if( ptemp == NULL )
	{
		return SKILL_INFO_INVALUED;
	}
	/// 스킬 종류 판단
	WORD skillKind = ptemp->GetKind();
//	if( skillKind == SKILLKIND_BUFF || skillKind == SKILLKIND_PASSIVE )
//	{
//		return SKILL_INFO_INVALUED;
//	}

	cActiveSkillInfo* pSkillInfo = ( cActiveSkillInfo* )ptemp;

	if( pOperator->GetAbnormalStatus()->IsSilence )
	{
		// 080616 LUJ, 침묵 중에는 물리 공격만 가능하다
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
	// 080616 LUJ, 이미 보유한 함정은 생성할 수 없도록 한다
	if( pSkillInfo->GetSkillInfo()->Unit == UNITKIND_TRAP &&
		pOperator->IsHaveTrap( pSkillInfo->GetSkillInfo()->UnitData ) )
	{
		return SKILL_COOLING;
	}
*//*
	// 080509 LUJ, 플레이어가 쿨타임 전에 스킬 요청을 하는지 체크한다
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

	/// 최초 접속시 무적 상태 해제
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );

	/// 길드 토너먼트나 공성전에서 관람자는 스킬을 사용할 수 없다
	if( pOperator->GetMaster()->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetMaster()->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetMaster()->GetBattleTeam() == 2 )			
			return SKILL_STATE_INVALUED;
	}

	/// 스킬을 사용할수 있는 상태인지 확인
	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel) == FALSE)
	{
		return SKILL_STATE_INVALUED;
	}

	VECTOR3 TargetPos;
	CObject * pTargetObject = NULL;
	float TargetRadius;

	/// 타겟 설정 확인
	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		/// 타겟이 없다
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

	/// 타겟 오브젝트의 크기를 구한다
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
	/// 사정거리 판단
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
	
	/// 이미 사용자가 사용한 스킬이 있는 경우
	
	/// On/Off형 스킬의 경우
	if( skillKind == SKILLKIND_ONOFF )
	{
		/// 이미 사용자에게 걸려있는 스킬이면 그 스킬을 해제한다
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

	// 080602 LUJ, 전역 스킬은 DB에서 직접 처리한 후 전송된다. 취소 등을 위해 오브젝트 ID를 직접 받아온다
	const DWORD skillObjectIndex = GetNewSkillObjectID();

	/// 스킬 생성 정보
	sSKILL_CREATE_INFO info;

	info.level = SkillLevel;
//	info.pOperator = pOperator;
	info.operatorId = pOperator->GetID();
	info.mainTarget = pmsg->MainTarget;
	info.skillDir = pmsg->SkillDir;	

	/// 스킬 오브젝트 생성
	cSkillObject* pSObj = pSkillInfo->GetSkillObject();

	/// 스킬 객체 ID
	// 080602 LUJ, DB 업데이트 실패 시 스킬 오브젝트를 취소할 수 있도록 위에서 인덱스를 미리 받아놓도록 했다
	info.skillObjectId = skillObjectIndex;
	
	/// 스킬 생성 위치
	if( pSkillInfo->GetSkillInfo()->Target )
	{
		info.pos.x = TargetPos.x;
		info.pos.z = TargetPos.z;
	}
	else
	{
		info.pos = *CCharMove::GetPosition(pOperator);
	}

	/// 스킬 객체 초기화
	pSObj->SetMultiTarget( &( pmsg->TargetList ), pSkillInfo->GetSkillInfo()->AreaData );
	pSObj->Init( &info );
	pSObj->SetBattle( pOperator->GetBattleID(), pOperator->GetBattleTeam() );
	pSObj->SetGridID( pOperator->GetGridID() );

	/// 테이블에 등록
	m_SkillObjectTable.Add( pSObj, pSObj->GetID() );
	g_pServerSystem->AddSkillObject( pSObj, &info.pos );

	pOperator->SetCurrentSkill( pSObj->GetID() );
/*
//---KES AUTONOTE	
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )	//---타겟이 몬스터일 경우만
				pOperator->SetLastActionTime( gCurTime );		
	}
//--------------
*//*
	// 080509 LUJ, 스킬 쿨타임을 저장한다
	{
		const ACTIVE_SKILL_INFO* skill = pSkillInfo->GetSkillInfo();

		if( skill )
		{
			pOperator->SetCoolTime( *skill );
		}
	}
*/
	/// 스킬 생성 성공
	return SKILL_SUCCESS;
}
BOOL cSkillManager::IsLearnableSkill( const CHARACTER_TOTALINFO& totalInfo, const SKILL_BASE& skillBase )
{
	// 090106 LUJ, grade 값을 사용한 후 감산하도록 수정
	for(	WORD grade = totalInfo.JobGrade;
			0 < grade;
			--grade )
	{
		const DWORD jobType		= DWORD( totalInfo.Job[ grade - 1 ] );
		// 090106 LUJ, 직업이 1단계인 경우 jobType은 항상 1이 되어야한다
		const DWORD jobIndex	= ( totalInfo.Job[ 0 ] * 1000 ) + ( ( totalInfo.Race + 1 ) * 100 ) + ( grade * 10 ) + ( 1 == grade ? 1 : jobType );
		// 090105 LUJ, 직업 번호를 이용해서 스킬 학습 트리의 포인터를 획득한다
		const SKILL_TREE_INFO* const skillTree = m_SkillTreeTable.GetData( jobIndex );

		if( ! skillTree )
		{
			break;
		}

		// 090105 LUJ, 스킬 트리에 저장된 스킬 중 클라이언트 요청에 적합한 조건이 있는지 검사한다
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
			// 090106 LUJ, 불필요한 연산 제거
			const DWORD clientSkillLevel = skillBase.Level;

			if( serverSkillLevel >= clientSkillLevel )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
