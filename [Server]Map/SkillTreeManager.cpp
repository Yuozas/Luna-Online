#include "stdafx.h"
#include "SkillTreeManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "../[cc]skill/server/info/activeskillinfo.h"
//#include "ItemManager.h"
//#include "..\[CC]Header\GameResourceManager.h"


CSkillTreeManager::CSkillTreeManager()
{

}

CSkillTreeManager::~CSkillTreeManager()
{

}
void CSkillTreeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
/*	case MP_SKILLTREE_MOVE_SYN:
		{
			MSG_SKILL_MOVE_SYN * msg = (MSG_SKILL_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);			
			if(!pPlayer) return;
			if(MoveSkill(pPlayer, msg->FromPos, msg->ToPos))
			{
				//횇짭쨋처?횑쩐챨횈짰쩔징째횚쨉쨉 쨘쨍쨀쨩횁횥
				MSG_SKILL_MOVE_ACK msg2;
				memcpy(&msg2, msg, sizeof(MSG_SKILL_MOVE_SYN));
				msg2.Protocol = MP_SKILLTREE_MOVE_ACK;
				pPlayer->SendMsg(&msg2, sizeof(MSG_SKILL_MOVE_ACK));
			}
			else
			{
				MSGBASE Msg;
				Msg.Category = MP_SKILLTREE;
				Msg.Protocol = MP_SKILLTREE_MOVE_NACK;
				pPlayer->SendMsg(&Msg, sizeof(Msg));
			}
		}
		break;
/*
			case MP_SKILLTREE_ADD_SYN:
				{
					MSG_SKILL_ADD * msg = (MSG_SKILL_ADD *)pMsg;
					CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
					if(pPlayer == NULL)
						return;
					SKILLMGR_OBJ->AddSkill(pPlayer, msg);
				}
				break;*/
/*	case MP_SKILLTREE_REM_SYN:
		{
			MSG_SKILL_REM_SYN * msg = (MSG_SKILL_REM_SYN *)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(pPlayer == NULL)
				return;
			if(RemSkill(pPlayer, msg->wSkillIdx, msg->TargetPos,eLog_SkillDiscard))
			{
				MSG_SKILL_REM_ACK ToMsg;
				ToMsg.Category = MP_SKILLTREE;
				ToMsg.Protocol = MP_SKILLTREE_REM_ACK;
				ToMsg.dwObjectID = msg->dwObjectID;
				ToMsg.wSkillIdx = msg->wSkillIdx;
				ToMsg.TargetPos = msg->TargetPos;

				pPlayer->SendMsg(&ToMsg, sizeof(ToMsg));
			}
			else
			{
				MSGBASE Msg;
				Msg.Category = MP_SKILLTREE;
				Msg.Protocol = MP_SKILLTREE_REM_NACK;
				pPlayer->SendMsg(&Msg, sizeof(Msg));
			}
		}
		break;
	case MP_SKILLTREE_OPTION_SYN:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSG_WORD4 msg;
			msg.Category = MP_SKILLTREE;
			msg.wData1 = pmsg->wData1;
			msg.wData2 = pmsg->wData2;
			msg.wData3 = pmsg->wData3;
			msg.wData4 = pmsg->wData4;

			if( SetOption(pPlayer, pmsg->wData1, pmsg->wData2, pmsg->wData3, pmsg->wData4) )
				msg.Protocol = MP_SKILLTREE_OPTION_ACK;
			else
				msg.Protocol = MP_SKILLTREE_OPTION_NACK;
			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	case MP_SKILLTREE_OPTION_CLEAR_SYN:
		{
			MSG_WORD4* pmsg = (MSG_WORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSG_WORD4 msg;
			msg.Category = MP_SKILLTREE;
			msg.wData1 = pmsg->wData1;
			msg.wData2 = pmsg->wData2;
			msg.wData3 = pmsg->wData3;
			msg.wData4 = pmsg->wData4;

			if( ClearOption(pPlayer, pmsg->wData1, pmsg->wData2, pmsg->wData3, pmsg->wData4) )
				msg.Protocol = MP_SKILLTREE_OPTION_CLEAR_ACK;
			else
				msg.Protocol = MP_SKILLTREE_OPTION_CLEAR_NACK;

			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;*/
	case MP_SKILLTREE_UPDATE_SYN:
		{
			MSG_SKILL_UPDATE*	pmsg	= ( MSG_SKILL_UPDATE* )pMsg;
			CPlayer*			pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

			// 071206 KTH -- 스킬을 리셋중에 업데이트 메시지가 들어오면 취소 시킨다.
			if(! pPlayer || pPlayer->IsResetSkill() )
			{
				return;
			}
			else if(FALSE == SKILLMGR->IsLearnableSkill(pPlayer->GetCharacterTotalInfo(), pmsg->SkillBase))
			{
				break;
			}

			const BYTE	skillLevel		= pmsg->SkillBase.Level;
			const DWORD baseSkillIndex	= pmsg->SkillBase.wSkillIdx;
			
			const BOOL	isSetSkill	= pPlayer->IsSetSkill( baseSkillIndex );
			cSkillTree* pSkillTree	= pPlayer->GetSkillTree();
			SKILL_BASE* pSkillBase	= pSkillTree->GetSkillBase( baseSkillIndex );
			SKILL_BASE* storedSkill =  pPlayer->GetStoredSkill( baseSkillIndex );

			DWORD skillindex = 0;

			const ACTIVE_SKILL_INFO* info = 0;

			BOOL	isNewSkill = FALSE;

			// 새로운 스킬일 경우 두가지
			// 스킬 정보가 없는 경우
			if( !pSkillBase	)
			{
				isNewSkill= TRUE;
			}
			// 셋트 스킬이면서 원본 스킬이 없는 경우
			else if( isSetSkill && ! storedSkill )
			{
				isNewSkill= TRUE;
			}

			// 배우려는 실제 스킬 Index
			// 신규 스킬은 기본 Index
			if( isNewSkill )
			{
				skillindex = baseSkillIndex;
			}
			// 셋트 스킬은 원본 스킬의 스킬 레벨+1 스킬 Index
			// 기본적으로 스킬 Index가 레벨 1 스킬의 Index이므로 
			// 거기에 레벨을 그냥 더해주면 + 1과 같다.
			else if( isSetSkill )
			{
				skillindex = storedSkill->wSkillIdx + storedSkill->Level;
			}
			// 그 외는 기존 스킬 레벨+1 스킬 Index
			else
			{
				skillindex = pSkillBase->wSkillIdx + pSkillBase->Level;
			}
			
			// 스킬 Index로 정보를 가져온다.
			cActiveSkillInfo* activeSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillindex );
			if( ! activeSkillInfo )
			{
				return;
			}
			info = activeSkillInfo->GetSkillInfo();

			// 현재 Player의 정보와 비교
			DWORD point = pPlayer->GetSkillPoint();
			DWORD money = pPlayer->GetMoney();

			// 모자르면 실패
			if( point < info->TrainPoint ||
				money < info->TrainMoney )
			{
				UpdateNack( pPlayer, 0 );
				return;
			}
			// 080310 LUJ, 습득 가능한 레벨인지 검사
			else if( pPlayer->GetLevel() < info->RequiredPlayerLevel )
			{
				UpdateNack( pPlayer, 0 );
				return;
			}

			// 돈과 SP소모
			point -= info->TrainPoint;

			pPlayer->GetHeroTotalInfo()->SkillPoint = point;
			pPlayer->SetMoney( info->TrainMoney, MONEY_SUBTRACTION );

			SkillPointUpdate( pPlayer->GetID(), point );

			// 새로운 스킬이면 Insert
			if( isNewSkill )
			{
				skillindex = baseSkillIndex;
				SkillInsertToDB( pPlayer->GetID(), baseSkillIndex, 1 );

				{
					// 071231 LUJ, 로그
					LogItemMoney(
						pPlayer->GetID(),
						pPlayer->GetObjectName(),
						baseSkillIndex,
						"lvl:1",
						eLog_ItemSkillLearn,
						pPlayer->GetMoney(),
						0,
						info->TrainMoney,
						0,
						0,
						0,
						0,
						0,
						0 );
				}
			}
			// 기존 스킬은 업데이트
			else
			{
				// 기존 스킬 정보를 복사해두고 삭제한다
				SKILL_BASE* skill = pSkillTree->NewSkillBase();
				*skill = *pSkillBase;
				pSkillTree->RemoveSkillBase( pSkillBase );

				// 셋트 스킬인 경우
				if( isSetSkill )
				{
					// 원본 스킬 정보의 레벨을 올린뒤 업데이트 시킨다.
					++( storedSkill->Level );

					// 071231 KTH -- GetID Add
					SkillUpdateToDB( storedSkill, pPlayer->GetID() );

					InsertLogSkill( pPlayer, storedSkill, eLog_SkillLevelup );

					++skill->Level;

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, storedSkill );
				}
				// 셋트 스킬이 아닌경우
				else
				{
					// 기존 스킬 정보의 레벨을 올린뒤 업데이트 시킨다.
					++skill->Level;

					// 071231 KTH -- GetID Add
					SkillUpdateToDB( skill, pPlayer->GetID() );

					InsertLogSkill( pPlayer, skill, eLog_SkillLevelup );

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, skill );
				}
			}
/*
			const BYTE	skillLevel		= pmsg->SkillBase.Level;
			const DWORD baseSkillIndex	= pmsg->SkillBase.wSkillIdx;
			const DWORD skillIndex		= baseSkillIndex - 1 + skillLevel;
			
			const ACTIVE_SKILL_INFO* info = 0;

			{
				cActiveSkillInfo* activeSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillIndex );

				if( ! activeSkillInfo )
				{
					return;
				}

				info = activeSkillInfo->GetSkillInfo();
			}		

			{
				DWORD point = pPlayer->GetSkillPoint();
				DWORD money = pPlayer->GetMoney();

				if( point < info->TrainPoint ||
					money < info->TrainMoney )
				{
					UpdateNack( pPlayer, 0 );
					return;
				}

				point -= info->TrainPoint;

				pPlayer->GetHeroTotalInfo()->SkillPoint = point;
				pPlayer->SetMoney( info->TrainMoney, MONEY_SUBTRACTION );

				SkillPointUpdate( pPlayer->GetID(), point );
			}			

			const BOOL	isSetSkill	= pPlayer->IsSetSkill( baseSkillIndex );
			cSkillTree* pSkillTree	= pPlayer->GetSkillTree();
			SKILL_BASE* pSkillBase	= pSkillTree->GetSkillBase( baseSkillIndex );

			// 새로운 스킬인 경우 추가하자. 세트 스킬이고 저장된 정보가 없는 경우에도 추가해야한다.
			if( !	pSkillBase	)
			{
				SkillInsertToDB( pPlayer->GetID(), baseSkillIndex, 1 );
				return;
			}
			
			SKILL_BASE* storedSkill =  pPlayer->GetStoredSkill( baseSkillIndex );

			if( isSetSkill && ! storedSkill )
			{
				SkillInsertToDB( pPlayer->GetID(), baseSkillIndex, 1 );
				return;
			}

			// 업데이트: 현재 스킬을 제거하고 새로운 스킬로 다시 적용하자
			{
				SKILL_BASE* skill = pSkillTree->NewSkillBase();

				*skill = *pSkillBase;

						
				pSkillTree->RemoveSkillBase( pSkillBase );
				
				// 지금 적용된 스킬이 세트 스킬일 경우 이전 정보를 복구를 대비해 업데이트해주고, 세트 스킬또한
				// 증가시켜 적용시켜야 한다. 저장된 것도 현재보다 하나 더, 적용된 세트 스킬도 지금보다 하나 더
				// 한다. RemoveSkillBase()를 하면 스킬 정보가 풀에서 해제되므로 메모리값이 정상이 아닐 수도 있음을 유의하자.
				// 전송시에는 저장된 값을 보내야한다. 클라이언트가 세트 스킬인지 판단하여 적절히 레벨을 다시 조정할 것이다.
				if( isSetSkill )
				{
					++( storedSkill->Level );

					SkillUpdateToDB( storedSkill );

					// 071109 웅주, 스킬 
					InsertLogSkill( pPlayer, storedSkill, eLog_SkillLevelup );

					++skill->Level;

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, storedSkill );
				}
				else
				{
					// UPDATE를 배우는 용도로만 쓰기 때문에 증가 연산을 할 수 있다
					// 그렇지 않은 경우가 있다면 아래 코드는 수정되어야 한다
					//skill->Level = skillLevel;
					++skill->Level;

					// 071109 웅주, 스킬 
					SkillUpdateToDB( skill );

					InsertLogSkill( pPlayer, skill, eLog_SkillLevelup );

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, skill );
				}
			}
*/
		}
		break;
	} 
}


void CSkillTreeManager::UpdateSkill( SKILL_BASE* pSkillBase, CPlayer* pPlayer )
{
	// 071231 KTH -- GetID Add
	SkillUpdateToDB( pSkillBase, pPlayer->GetID() );
}


void CSkillTreeManager::UpdateAck( CPlayer * pPlayer, SKILL_BASE* pSkillBase )
{
	MSG_SKILL_UPDATE msg;
	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_UPDATE_ACK;
	msg.SkillBase = *pSkillBase;

	pPlayer->SendMsg( &msg, sizeof( msg ) );
}

void CSkillTreeManager::UpdateNack( CPlayer * pPlayer, BYTE result )
{
	MSG_BYTE msg;

	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_UPDATE_NACK;
	msg.bData = result;

	pPlayer->SendMsg( &msg, sizeof( msg ) );
}

void CSkillTreeManager::AddSkill( CPlayer * pPlayer, SKILL_BASE* pSkillBase )
{
	SkillInsertToDB( pPlayer->GetID(), pSkillBase->wSkillIdx, pSkillBase->Level );
}

void CSkillTreeManager::AddSkillDBResult( CPlayer* pPlayer, SKILL_BASE* pSkillBase )
{
	cSkillTree* pSkillTree	= pPlayer->GetSkillTree();
	const DWORD skillIndex	= pSkillBase->wSkillIdx;
	SKILL_BASE* newSkill	= pSkillTree->NewSkillBase();
	
	UpdateAck( pPlayer, pSkillBase );

	//InsertLogSkill( *pPlayer, *pSkillBase, eLog_SkillLearn );

	*newSkill = *pSkillBase;

	SKILL_BASE* currentSkill = pSkillTree->GetSkillBase( skillIndex );

	// 적용된 스킬이 없거나.. 
	// 세트 스킬이 적용된 상태가 아니면 삽입하고 끝낸다
	if( !	currentSkill	||
			currentSkill->dwDBIdx )
	{
		pSkillTree->RemoveSkillBase( currentSkill );
		pSkillTree->AddSkillBase( newSkill );

		return;
	}
	
	// 저장된 스킬이 있는가? 이는 곧 세트 스킬이 적용되었다는 의미
	// 저장된 스킬을 갱신하고, 세트 스킬을 하나 증가시켜 다시 적용시킨다.
	{
		SKILL_BASE* storedSkill = pPlayer->GetStoredSkill( skillIndex );

		if( storedSkill )
		{
			storedSkill->Level = pSkillBase->Level;
		}
		else
		{
			pPlayer->SetStoredSkill( *pSkillBase );
		}
		
		{
			newSkill->dwDBIdx	= 0;
			newSkill->wSkillIdx	= skillIndex;
			newSkill->Level		= currentSkill->Level + 1;
	
			pSkillTree->RemoveSkillBase( currentSkill );
			pSkillTree->AddSkillBase( newSkill );
		}
	}
}

/*
BOOL CSkillTreeManager::RemSkill(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, BYTE bType)
{
	SKILLBASE * pSkill = pPlayer->GetSkillBase(TargetPos);
	if(!pSkill || pSkill->wIconIdx != wSkillIdx)
		return FALSE;

	// db
	SkillDeleteToDB(pSkill->dwDBIdx);

	// Log 쨔짬째첩 쨩챔횁짝
	InsertLogSkill( bType, pPlayer->GetID(), pSkill->wIconIdx, pSkill->dwDBIdx, pSkill->Level, pSkill->ExpPoint );

	// 쨔짬째첩횁짚쨘쨍 쨩챔횁짝
	memset(pSkill, 0, sizeof(SKILLBASE));

	return TRUE;
}

BOOL CSkillTreeManager::MoveSkill(CPlayer * pPlayer, POSTYPE FromPos, POSTYPE ToPos)
{	
	if(FromPos == ToPos)
		return FALSE;

	if( FromPos >= TP_JINBUB_START && FromPos < TP_JINBUB_END )
	{
		if( ToPos >= TP_SKILL1_START && ToPos < TP_SKILL2_END )
			return FALSE;
	}
	else if( FromPos >= TP_SKILL1_START && FromPos < TP_SKILL2_END )
	{
		if( ToPos >= TP_JINBUB_START && ToPos < TP_JINBUB_END )
			return FALSE;
	}

	SKILLBASE FromSkill = *pPlayer->GetSkillBase(FromPos);
	SKILLBASE ToSkill = *pPlayer->GetSkillBase(ToPos);
	


	if( ToSkill.Position == 0 ) // 쩔횇짹챈 ?횣쨍짰째징 쨘챰쩐챤 ?횜쨈횂째챈쩔챙
	{
		FromSkill.Position = ToPos;
		pPlayer->SetSkillBase(FromPos, &ToSkill); // 짹창횁쨍 ?횣쨍짰 쨘챰쩔챙짹창
		pPlayer->SetSkillBase(FromSkill.Position, &FromSkill); // 쨩천쨌횓쩔챤 ?횣쨍짰쩔징 쨀횜짹창

		SkillUpdateToDB(&FromSkill,"MOVE_NULL");
	}
	else
	{
		if( FromSkill.dwDBIdx == 0 )
			return FALSE;
		
		SWAPVALUE(FromSkill.Position, ToSkill.Position)
		ASSERT(FromSkill.Position);
		pPlayer->SetSkillBase(FromSkill.Position, &FromSkill);
		pPlayer->SetSkillBase(ToSkill.Position, &ToSkill);

		SkillMoveUpdateToDB(FromSkill.dwDBIdx, FromSkill.Position, ToSkill.dwDBIdx, ToSkill.Position, "MOVE_FROM");
	}

	//CharacterSkillUpdate(FromSkill);
	//CharacterSkillUpdate(ToSkill);

	return TRUE;
}

void CSkillTreeManager::AddSkillDBResult(CPlayer * pPlayer, SKILLBASE * pSkillBase)
{
	MSG_SKILL_ADD_ACK msg;
	msg.Category = MP_SKILLTREE;
	msg.Protocol = MP_SKILLTREE_ADD_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.SkillBase = *pSkillBase;

	pPlayer->SetSkillBase(pSkillBase->Position, pSkillBase);
	pPlayer->SendMsg(&msg, sizeof(MSG_SKILL_ADD_ACK));

	if(pSkillBase->QuickPosition)
	{
		MSG_QUICK_ADD_ACK msg;
		msg.Category = MP_QUICK;
		msg.Protocol = MP_QUICK_ADD_ACK;
		msg.OldSrcItemIdx = 0;
		msg.OldSrcPos = 0;
		msg.SrcPos = pSkillBase->Position;
		msg.SrcItemIdx = pSkillBase->wIconIdx;
		msg.QuickPos = pSkillBase->QuickPosition;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
	// Log 쨔짬째첩 횊쨔쨉챈..
	InsertLogSkill( eLog_SkillLearn, pPlayer->GetID(), pSkillBase->wIconIdx, pSkillBase->dwDBIdx, pSkillBase->Level, pSkillBase->ExpPoint );
}
BOOL CSkillTreeManager::AddSkill( CPlayer * pPlayer, WORD wSkillIdx, WORD ItemKind, POSTYPE QuickPos, BYTE bLevel, WORD Option)
{
	// 횁횩쨘쨔 횄쩌횇짤
	if(pPlayer->GetSkillBaseBySkillIdx(wSkillIdx))
		return FALSE;
	
	// 횈짰쨍짰횄쩌횇짤
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(wSkillIdx);
	if( pInfo == NULL )
		return FALSE;


	// 절초 검색 hs
	SKILL_CHANGE_INFO * pChangeInfo = SKILLMGR->GetSkillChangeInfo(wSkillIdx);
	if(pChangeInfo)
	{
		if(pPlayer->GetSkillBaseBySkillIdx(pChangeInfo->wTargetSkillIdx))
			return FALSE;
	}

	// 쨘처째첩째짙
	POSTYPE StartPos,EndPos;
	if(ItemKind == eSKILL_ITEM_JINBUB)
	{
		StartPos = TP_JINBUB_START;
		EndPos = TP_JINBUB_END;
	}
	else
	{		
		StartPos = TP_SKILL1_START;
		EndPos = TP_SKILL1_END;
		if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_SkillExtend ) )
			EndPos = TP_SKILL2_END;
	}

	POSTYPE ToPos = EndPos;
	SKILLBASE* pSkill;
	for( POSTYPE n = StartPos ; n < EndPos ; ++n )
	{
		pSkill = pPlayer->GetSkillBase(n);
		if(pSkill->dwDBIdx == 0)
		{
			ToPos = n;
			break;
		}
	}
	if(ToPos == EndPos)
		return FALSE;

	BYTE weared = 0;
	switch(ConvAbsPos2SkillPos(ToPos))
	{
	case TP_SKILL1_START:
	case TP_JINBUB_START:
		weared = 1;
		break;
	}

	SKILLBASE SkillBase;
	SkillBase.Position			= ToPos;
	SkillBase.dwDBIdx			= 9;
	SkillBase.ExpPoint			= 9;
	SkillBase.QuickPosition	= 9;
	SkillBase.Level				= 9;
    SkillBase.wIconIdx			= wSkillIdx;
	SkillBase.OptionIndex		= Option;

	pPlayer->SetSkillBase(SkillBase.Position, &SkillBase);

	ASSERTMSG(ToPos, "0쨔첩 횈첨횁철쩌횉쩔징 쨔짬째첩?쨩 쨀횜?쨍쨌횁 횉횛쨈횕쨈횢.");
	//db
	
	if( pInfo->GetNeedExp( 0 ) == (EXPTYPE)(-1) )
	{
		bLevel = 12;
	}


	//SkillInsertToDB( pPlayer->GetID(), wSkillIdx, ToPos, weared, bLevel, Option );

	return TRUE;
}

void CSkillTreeManager::ChangeSkill(CPlayer* pPlayer, WORD wRemSkillIdx, POSTYPE TargetPos, WORD wAddSkillIdx, WORD LogType)
{
	SKILLBASE* pSkill = pPlayer->GetSkillBase(TargetPos);
	POSTYPE QPos = pSkill->QuickPosition;
	WORD Option = pSkill->OptionIndex;
	if(RemSkill(pPlayer, wRemSkillIdx, TargetPos, (BYTE)LogType))
	{
		MSG_SKILL_REM_ACK ToMsg;
		ToMsg.Category = MP_SKILLTREE;
		ToMsg.Protocol = MP_SKILLTREE_REM_ACK;
		ToMsg.dwObjectID = pPlayer->GetID();
		ToMsg.wSkillIdx = wRemSkillIdx;
		ToMsg.TargetPos = TargetPos;
		
		pPlayer->SendMsg(&ToMsg, sizeof(ToMsg));
	}
	AddSkill(pPlayer, wAddSkillIdx, eSKILL_ITEM, 0, 12, Option);
}

BOOL CSkillTreeManager::SetOption(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos)
{
	SKILLBASE * pSkill = pPlayer->GetSkillBase(TargetPos);
	if(!pSkill || pSkill->wIconIdx != wSkillIdx)
		return FALSE;

	CSkillInfo* pInfo = NULL;
	SKILLINFO* pSInfo = NULL;
	SKILLOPTION* pOption = NULL;
	SKILLOPTION* pOldOption = NULL;

	// 스킬 정보를 가져온다
	pInfo = SKILLMGR->GetSkillInfo(wSkillIdx);
	if(!pInfo)	return FALSE;
 	pSInfo = pInfo->GetSkillInfo();
	if(!pSInfo)	return FALSE;

	// 변환 불가 스킬이면 실패
	if(pSInfo->ChangeKind == eSkillChange_None) return FALSE;

	// 아이템 정보를 가져온다
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	pOption = SKILLMGR->GetSkillOptionByItemIndex(ItemIdx);
	if(!pOption) return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(ItemIdx);
	if(!pItemInfo) return FALSE;

	// 레벨 제한
	if(pPlayer->GetLevel() < pItemInfo->LimitLevel)
		return FALSE;
/*
	// 경지 제한
	switch(pOption->OptionGrade)
	{
	case 1:
		// 1단계는 일반
		break;
	case 2:
		// 2단계는 화경 극마 이상
		if(!(pPlayer->GetStage() & eStage_Hwa || pPlayer->GetStage() & eStage_Geuk))
			return FALSE;
		break;
	case 3:
		// 3단계는 현경 탈마 이상
		if(!(pPlayer->GetStage() == eStage_Hyun || pPlayer->GetStage() == eStage_Tal))
			return FALSE;
		break;
	}
*/
/*	// 모든 변환 가능 스킬이 아니면 변환 가능 종류와 아이템의 종류가 다르면 실패한다.
	if(pSInfo->ChangeKind != eSkillChange_All && pSInfo->ChangeKind != pOption->SkillKind)
		return FALSE;

	// 이미 변환된 무공인지 확인한다
	pOldOption = SKILLMGR->GetSkillOption(pSkill->OptionIndex);
	if(pOldOption)
	{
		// 이미 적용된 변환의 종류와 다르면 실패한다.
		if(pOldOption->OptionKind != pOption->OptionKind)
			return FALSE;

		// 이미 적용된 변환보다 낮거나 같은 등급이면 실패한다.
		if(pOldOption->OptionGrade >= pOption->OptionGrade)
			return FALSE;
	}

	ITEMMGR->DiscardItem(pPlayer, ItemPos, ItemIdx, 1);
	pSkill->OptionIndex = pOption->Index;

	InsertLogSkill( eLog_SkillOption, pPlayer->GetID(), pSkill->wIconIdx, pSkill->dwDBIdx, pSkill->OptionIndex, pSkill->ExpPoint );
	SkillUpdateToDB(pSkill, "OPTION");

	return TRUE;
}

BOOL CSkillTreeManager::ClearOption(CPlayer * pPlayer, WORD wSkillIdx, POSTYPE TargetPos, WORD ItemIdx, WORD ItemPos)
{
	SKILLBASE * pSkill = pPlayer->GetSkillBase(TargetPos);
	if(!pSkill || pSkill->wIconIdx != wSkillIdx)
		return FALSE;

	// 아이템 정보를 가져온다
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	// 이미 변환된 무공인지 확인한다
	if(pSkill->OptionIndex == eSkillOption_None)
		return FALSE;

	if(ItemIdx == 10750)
	{
		pPlayer->SkillLevelDown(pSkill->wIconIdx, eLog_SkillDestroyByOptionClear);
	}
	
	ITEMMGR->DiscardItem(pPlayer, ItemPos, ItemIdx, 1);
	
	pSkill->OptionIndex = 0;
	
	InsertLogSkill( eLog_SkillOptionClear, pPlayer->GetID(), pSkill->wIconIdx, pSkill->dwDBIdx, pSkill->OptionIndex, pSkill->ExpPoint );
	SkillUpdateToDB(pSkill, "OPTION");

	return TRUE;
}
*/


CSkillTreeManager* CSkillTreeManager::GetInstance()
{
	static CSkillTreeManager instance;

	return &instance;
}
