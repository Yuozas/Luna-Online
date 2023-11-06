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
				//Å¬¶ó?Ì¾ðÆ®¿¡°Ôµµ º¸³»ÁÜ
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

			// 071206 KTH -- ��ų�� �����߿� ������Ʈ �޽����� ������ ��� ��Ų��.
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

			// ���ο� ��ų�� ��� �ΰ���
			// ��ų ������ ���� ���
			if( !pSkillBase	)
			{
				isNewSkill= TRUE;
			}
			// ��Ʈ ��ų�̸鼭 ���� ��ų�� ���� ���
			else if( isSetSkill && ! storedSkill )
			{
				isNewSkill= TRUE;
			}

			// ������ ���� ��ų Index
			// �ű� ��ų�� �⺻ Index
			if( isNewSkill )
			{
				skillindex = baseSkillIndex;
			}
			// ��Ʈ ��ų�� ���� ��ų�� ��ų ����+1 ��ų Index
			// �⺻������ ��ų Index�� ���� 1 ��ų�� Index�̹Ƿ� 
			// �ű⿡ ������ �׳� �����ָ� + 1�� ����.
			else if( isSetSkill )
			{
				skillindex = storedSkill->wSkillIdx + storedSkill->Level;
			}
			// �� �ܴ� ���� ��ų ����+1 ��ų Index
			else
			{
				skillindex = pSkillBase->wSkillIdx + pSkillBase->Level;
			}
			
			// ��ų Index�� ������ �����´�.
			cActiveSkillInfo* activeSkillInfo = ( cActiveSkillInfo* )SKILLMGR->GetSkillInfo( skillindex );
			if( ! activeSkillInfo )
			{
				return;
			}
			info = activeSkillInfo->GetSkillInfo();

			// ���� Player�� ������ ��
			DWORD point = pPlayer->GetSkillPoint();
			DWORD money = pPlayer->GetMoney();

			// ���ڸ��� ����
			if( point < info->TrainPoint ||
				money < info->TrainMoney )
			{
				UpdateNack( pPlayer, 0 );
				return;
			}
			// 080310 LUJ, ���� ������ �������� �˻�
			else if( pPlayer->GetLevel() < info->RequiredPlayerLevel )
			{
				UpdateNack( pPlayer, 0 );
				return;
			}

			// ���� SP�Ҹ�
			point -= info->TrainPoint;

			pPlayer->GetHeroTotalInfo()->SkillPoint = point;
			pPlayer->SetMoney( info->TrainMoney, MONEY_SUBTRACTION );

			SkillPointUpdate( pPlayer->GetID(), point );

			// ���ο� ��ų�̸� Insert
			if( isNewSkill )
			{
				skillindex = baseSkillIndex;
				SkillInsertToDB( pPlayer->GetID(), baseSkillIndex, 1 );

				{
					// 071231 LUJ, �α�
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
			// ���� ��ų�� ������Ʈ
			else
			{
				// ���� ��ų ������ �����صΰ� �����Ѵ�
				SKILL_BASE* skill = pSkillTree->NewSkillBase();
				*skill = *pSkillBase;
				pSkillTree->RemoveSkillBase( pSkillBase );

				// ��Ʈ ��ų�� ���
				if( isSetSkill )
				{
					// ���� ��ų ������ ������ �ø��� ������Ʈ ��Ų��.
					++( storedSkill->Level );

					// 071231 KTH -- GetID Add
					SkillUpdateToDB( storedSkill, pPlayer->GetID() );

					InsertLogSkill( pPlayer, storedSkill, eLog_SkillLevelup );

					++skill->Level;

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, storedSkill );
				}
				// ��Ʈ ��ų�� �ƴѰ��
				else
				{
					// ���� ��ų ������ ������ �ø��� ������Ʈ ��Ų��.
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

			// ���ο� ��ų�� ��� �߰�����. ��Ʈ ��ų�̰� ����� ������ ���� ��쿡�� �߰��ؾ��Ѵ�.
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

			// ������Ʈ: ���� ��ų�� �����ϰ� ���ο� ��ų�� �ٽ� ��������
			{
				SKILL_BASE* skill = pSkillTree->NewSkillBase();

				*skill = *pSkillBase;

						
				pSkillTree->RemoveSkillBase( pSkillBase );
				
				// ���� ����� ��ų�� ��Ʈ ��ų�� ��� ���� ������ ������ ����� ������Ʈ���ְ�, ��Ʈ ��ų����
				// �������� ������Ѿ� �Ѵ�. ����� �͵� ���纸�� �ϳ� ��, ����� ��Ʈ ��ų�� ���ݺ��� �ϳ� ��
				// �Ѵ�. RemoveSkillBase()�� �ϸ� ��ų ������ Ǯ���� �����ǹǷ� �޸𸮰��� ������ �ƴ� ���� ������ ��������.
				// ���۽ÿ��� ����� ���� �������Ѵ�. Ŭ���̾�Ʈ�� ��Ʈ ��ų���� �Ǵ��Ͽ� ������ ������ �ٽ� ������ ���̴�.
				if( isSetSkill )
				{
					++( storedSkill->Level );

					SkillUpdateToDB( storedSkill );

					// 071109 ����, ��ų 
					InsertLogSkill( pPlayer, storedSkill, eLog_SkillLevelup );

					++skill->Level;

					pSkillTree->AddSkillBase( skill );
					UpdateAck( pPlayer, storedSkill );
				}
				else
				{
					// UPDATE�� ���� �뵵�θ� ���� ������ ���� ������ �� �� �ִ�
					// �׷��� ���� ��찡 �ִٸ� �Ʒ� �ڵ�� �����Ǿ�� �Ѵ�
					//skill->Level = skillLevel;
					++skill->Level;

					// 071109 ����, ��ų 
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

	// ����� ��ų�� ���ų�.. 
	// ��Ʈ ��ų�� ����� ���°� �ƴϸ� �����ϰ� ������
	if( !	currentSkill	||
			currentSkill->dwDBIdx )
	{
		pSkillTree->RemoveSkillBase( currentSkill );
		pSkillTree->AddSkillBase( newSkill );

		return;
	}
	
	// ����� ��ų�� �ִ°�? �̴� �� ��Ʈ ��ų�� ����Ǿ��ٴ� �ǹ�
	// ����� ��ų�� �����ϰ�, ��Ʈ ��ų�� �ϳ� �������� �ٽ� �����Ų��.
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

	// Log ¹«°ø »èÁ¦
	InsertLogSkill( bType, pPlayer->GetID(), pSkill->wIconIdx, pSkill->dwDBIdx, pSkill->Level, pSkill->ExpPoint );

	// ¹«°øÁ¤º¸ »èÁ¦
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
	


	if( ToSkill.Position == 0 ) // ¿Å±æ ?Ú¸®°¡ ºñ¾î ?Ö´Â°æ¿ì
	{
		FromSkill.Position = ToPos;
		pPlayer->SetSkillBase(FromPos, &ToSkill); // ±âÁ¸ ?Ú¸® ºñ¿ì±â
		pPlayer->SetSkillBase(FromSkill.Position, &FromSkill); // »õ·Î¿î ?Ú¸®¿¡ ³Ö±â

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
	// Log ¹«°ø È¹µæ..
	InsertLogSkill( eLog_SkillLearn, pPlayer->GetID(), pSkillBase->wIconIdx, pSkillBase->dwDBIdx, pSkillBase->Level, pSkillBase->ExpPoint );
}
BOOL CSkillTreeManager::AddSkill( CPlayer * pPlayer, WORD wSkillIdx, WORD ItemKind, POSTYPE QuickPos, BYTE bLevel, WORD Option)
{
	// Áßº¹ Ã¼Å©
	if(pPlayer->GetSkillBaseBySkillIdx(wSkillIdx))
		return FALSE;
	
	// Æ®¸®Ã¼Å©
	CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(wSkillIdx);
	if( pInfo == NULL )
		return FALSE;


	// ���� �˻� hs
	SKILL_CHANGE_INFO * pChangeInfo = SKILLMGR->GetSkillChangeInfo(wSkillIdx);
	if(pChangeInfo)
	{
		if(pPlayer->GetSkillBaseBySkillIdx(pChangeInfo->wTargetSkillIdx))
			return FALSE;
	}

	// ºó°ø°£
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

	ASSERTMSG(ToPos, "0¹ø Æ÷Áö¼Ç¿¡ ¹«°ø?» ³Ö?¸·Á ÇÕ´Ï´Ù.");
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

	// ��ų ������ �����´�
	pInfo = SKILLMGR->GetSkillInfo(wSkillIdx);
	if(!pInfo)	return FALSE;
 	pSInfo = pInfo->GetSkillInfo();
	if(!pSInfo)	return FALSE;

	// ��ȯ �Ұ� ��ų�̸� ����
	if(pSInfo->ChangeKind == eSkillChange_None) return FALSE;

	// ������ ������ �����´�
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	pOption = SKILLMGR->GetSkillOptionByItemIndex(ItemIdx);
	if(!pOption) return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(ItemIdx);
	if(!pItemInfo) return FALSE;

	// ���� ����
	if(pPlayer->GetLevel() < pItemInfo->LimitLevel)
		return FALSE;
/*
	// ���� ����
	switch(pOption->OptionGrade)
	{
	case 1:
		// 1�ܰ�� �Ϲ�
		break;
	case 2:
		// 2�ܰ�� ȭ�� �ظ� �̻�
		if(!(pPlayer->GetStage() & eStage_Hwa || pPlayer->GetStage() & eStage_Geuk))
			return FALSE;
		break;
	case 3:
		// 3�ܰ�� ���� Ż�� �̻�
		if(!(pPlayer->GetStage() == eStage_Hyun || pPlayer->GetStage() == eStage_Tal))
			return FALSE;
		break;
	}
*/
/*	// ��� ��ȯ ���� ��ų�� �ƴϸ� ��ȯ ���� ������ �������� ������ �ٸ��� �����Ѵ�.
	if(pSInfo->ChangeKind != eSkillChange_All && pSInfo->ChangeKind != pOption->SkillKind)
		return FALSE;

	// �̹� ��ȯ�� �������� Ȯ���Ѵ�
	pOldOption = SKILLMGR->GetSkillOption(pSkill->OptionIndex);
	if(pOldOption)
	{
		// �̹� ����� ��ȯ�� ������ �ٸ��� �����Ѵ�.
		if(pOldOption->OptionKind != pOption->OptionKind)
			return FALSE;

		// �̹� ����� ��ȯ���� ���ų� ���� ����̸� �����Ѵ�.
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

	// ������ ������ �����´�
	CItemSlot* pInven = (CItemSlot*)pPlayer->GetSlot(ItemPos);
	if(!pInven) return FALSE;

	const ITEMBASE* pItem = pInven->GetItemInfoAbs(ItemPos);
	if(!pItem) return FALSE;
	if(pItem->wIconIdx != ItemIdx) return FALSE;

	// �̹� ��ȯ�� �������� Ȯ���Ѵ�
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
