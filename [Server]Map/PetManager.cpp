#include "stdafx.h"
#include ".\petmanager.h"
#include "MHFile.h"
#include "Player.h"
#include "charmove.h"
#include "tilemanager.h"
#include "Pet.h"
#include "UserTable.h"
#include "itemmanager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MapDBMsgParser.h"
#include "../[cc]skill/server/manager/skillmanager.h"
#include "PackedData.h"

GLOBALTON(CPetManager);

CPetManager::CPetManager(void)
{
	mMaxGrade = 0;
	mMaxDistance = 3000;
	mDistanceCheckTime = 1000;
	mAreaDistance = 300;
	
	mGradeUpLevel = NULL;
	mSummonPlayerLevel = NULL;
	mExp = NULL;
	mExtendedSkillSlot = NULL;

	mPetInfoTable.Initialize( 10 );
	mPetObjectInfoTable.Initialize( 10 );
	mPetObjectTable.Initialize( 10 );
	mPetObjectInfoPool.Init( 10, 10, "PetObjectInfoPool" );
	mPetHPMPInfoTable.Initialize( 10 );
	mPetFriendlyPenaltyTable.Initialize( 1 );

	mFriendlyReduceTime = 60*60*1000;
	mFriendlyReducePoint = 1;
	
	for( int i = 0; i < ePetType_Max; i++ )
	{
		mPetStatusInfoTable[ i ].Initialize( 20 );
	}
}

CPetManager::~CPetManager(void)
{
	delete mGradeUpLevel;
	delete mSummonPlayerLevel;
	delete mExp;
	delete mExtendedSkillSlot;

	PET_INFO* pInfo = NULL;

	mPetInfoTable.SetPositionHead();
	while( pInfo = mPetInfoTable.GetData() )
	{
		delete pInfo;
	}
	mPetInfoTable.RemoveAll();

	PET_STATUS_INFO* pStatusInfo;

	for( int i = 0; i < ePetType_Max; i++ )
	{
		mPetStatusInfoTable[ i ].SetPositionHead();
		while( pStatusInfo = mPetStatusInfoTable[ i ].GetData() )
		{
			delete pStatusInfo;
		}
		mPetStatusInfoTable[ i ].RemoveAll();
	}
}

void CPetManager::SealPet( CPet* pPet )
{
	char log[256];
	sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
	FILE* fp = fopen( log, "a+" );

	if( fp )
	{
		fprintf( fp, "SEAL:\t%d\n", pPet );
		fclose( fp );
	}

	pPet->DBInfoUpdate();
	pPet->GetMaster()->SetCurPet( NULL );
	g_pServerSystem->RemovePet( pPet->GetID() );
}

void CPetManager::SummonPet( CPlayer* pPlayer, DWORD ItemDBIdx )
{
	PET_OBJECT_INFO* pObjectInfo = pPlayer->GetPetInfo( ItemDBIdx );
	
	if( !pObjectInfo )
	{
		return;
	}
	
	PET_INFO* pPetInfo = mPetInfoTable.GetData( pObjectInfo->Kind );

	if( pPetInfo )
	{
		pPlayer->SetSummonWaitPet( ItemDBIdx );
		PetItemInfoLoad( pPlayer->GetID(), pObjectInfo->ID );
	}
	else
	{
	}
}

void CPetManager::LoadPetInfo()
{
	CMHFile file;
	BOOL	didLoadMaxGrade = FALSE;
	BOOL	didLoadMaxLevel = FALSE;

	file.Init("System/Resource/PetSystemInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	char buf[256]={0,};
	while(1)
	{
		if( file.IsEOF() )
			break;

		file.GetString( buf );

		if( buf[ 0 ] == '@' )
		{
			file.GetLineX( buf, 256 );
			continue;
		}

		if( strcmp( buf,"#MAXGRADE" ) == 0 )
		{
			mMaxGrade = file.GetByte();
			didLoadMaxGrade = TRUE;
		}

		if( strcmp( buf, "#MAXLEVEL" ) == 0 )
		{
			mMaxLevel = file.GetByte();
			didLoadMaxLevel = TRUE;
		}

		if( strcmp( buf, "#GRADEUPLEVEL" ) == 0 )
		{
			if( didLoadMaxGrade )
			{
				mGradeUpLevel = new BYTE[ mMaxGrade ];

				for( int i = 0; i < mMaxGrade; i++ )
				{
					mGradeUpLevel[ i ] = file.GetByte();
				}
			}
			else
			{
				// 최대 진화 단계를 설정하지 않고 진화 레벨을 설정하였음
			}
		}

		if( strcmp( buf, "#SUMMONPLAYERLEVEL" ) == 0 )
		{
			if( didLoadMaxGrade )
			{
				mSummonPlayerLevel = new BYTE[ mMaxGrade + 1 ];

				for( int i = 0; i < mMaxGrade + 1; i++ )
				{
					mSummonPlayerLevel[ i ] = file.GetByte();
				}
			}
			else
			{
				// 최대 진화 단계를 설정하지 않고 소환 레벨 제한을 설정하였음
			}
		}

		if( strcmp( buf, "#EXP" ) == 0 )
		{
			if( didLoadMaxGrade )
			{
				mExp = new WORD[ mMaxGrade + 1 ];

				for( int i = 0; i < mMaxGrade + 1; i++ )
				{
					mExp[ i ] = file.GetWord();
				}
			}
			else
			{
				// 최대 진화 단계를 설정하지 않고 경험치를 설정하였음
			}
		}

		if( strcmp( buf, "#EXPPENALTYLEVEL" ) == 0 )
		{
			mExpPenaltyLevel = file.GetByte();
		}

		if( strcmp( buf, "#SHOPPETBASESKILLSLOT" ) == 0 )
		{
			mShopBaseSkillSlot = file.GetByte();
		}

		if( strcmp( buf, "#BASESKILLSLOT" ) == 0 )
		{
			mMinBaseSkillSlot = file.GetByte();
			mMaxBaseSkillSlot = file.GetByte();
		}

		if( strcmp( buf, "#EXTENDEDSKILLSLOT" ) == 0)
		{
			if( didLoadMaxGrade )
			{
				mExtendedSkillSlot = new BYTE[ mMaxGrade ];

				for( int i = 0; i < mMaxGrade; i++ )
				{
					mExtendedSkillSlot[ i ] = file.GetByte();
				}
			}
			else
			{
				// 최대 진화 단계를 설정하지 않고 진화 레벨을 설정하였음
			}
		}

		if( strcmp( buf, "#MAXDISTANCE" ) == 0 )
		{
			mMaxDistance = file.GetWord();
		}
		
		if( strcmp( buf, "#AREADISTANCE" ) == 0 )
		{
			mAreaDistance = file.GetWord();
		}

		if( strcmp( buf, "#DISTANCECHECKTIME" ) == 0 )
		{
			mDistanceCheckTime = file.GetDword();
		}

		if( strcmp( buf, "#HATCHDELAY" ) == 0 )
		{
			mHatchDelay = file.GetDword();
		}
		
		if( strcmp( buf, "#SUMMONDELAY" ) == 0 )
		{
			mSummonDelay = file.GetDword();
		}

		if( strcmp( buf, "#SEALDELAY" ) == 0 )
		{
			mSealDelay = file.GetDword();
		}

		if( strcmp( buf, "#SUMMONPENALTYDELAY" ) == 0 )
		{
			mSummonPenaltyDelay = file.GetDword();
		}

		if( strcmp( buf, "#RESUMMONPENALTYDELAY" ) == 0 )
		{
			mResummonPenaltyDelay = file.GetDword();
		}
		
		if( strcmp( buf, "#CLOSEDSKILL" ) == 0 )
		{
			mClosedSkillIndex = file.GetDword();
		}

		if( strcmp( buf, "#RANGESKILL" ) == 0 )
		{
			mRangeSkillIndex = file.GetDword();
		}

		if( strcmp( buf, "#FRIENDLYREDUCE" ) == 0 )
		{
			mFriendlyReduceTime = file.GetDword() * 1000 * 60;
			mFriendlyReducePoint = file.GetByte();
		}

		if( strcmp( buf, "#FRIENDLYPENALTY" ) == 0 )
		{
			PET_FRIENDLY_PENALTY* pPenalty = new PET_FRIENDLY_PENALTY;

			pPenalty->Friendly = file.GetByte();
			pPenalty->Penalty = file.GetByte();

			mPetFriendlyPenaltyTable.Add( pPenalty, pPenalty->Friendly );
		}
	}

	file.Release();

	file.Init("System/Resource/PetStatusInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		BYTE type = file.GetByte();

		PET_STATUS_INFO* pStatusInfo = new PET_STATUS_INFO;

		pStatusInfo->Level	= file.GetByte();
		pStatusInfo->STR	= file.GetDword();
		pStatusInfo->DEX	= file.GetDword();
		pStatusInfo->VIT	= file.GetDword();
		pStatusInfo->INT	= file.GetDword();
		pStatusInfo->WIS	= file.GetDword();

		mPetStatusInfoTable[ type ].Add( pStatusInfo, pStatusInfo->Level );
	}

	file.Release();

	file.Init("System/Resource/PetList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{	//fix map exit crash
		if(file.IsEOF() != FALSE)
			break;
		TCHAR text[MAX_PATH] = {0};
		file.GetLine(
			text,
			_countof(text));

		LPCTSTR textSeperator = " \t";
		LPCTSTR textIndex = _tcstok(
			text,
			textSeperator);
		LPCTSTR textName = _tcstok(
			0,
			textSeperator);
		LPCTSTR textModelName = _tcstok(
			0,
			textSeperator);
		LPCTSTR textImage = _tcstok(
			0,
			textSeperator);
		LPCTSTR textSight = _tcstok(
			0,
			textSeperator);

		PET_INFO* pInfo = new PET_INFO;
/*
		pInfo->Index = file.GetDword();

		SafeStrCpy( pInfo->Name, file.GetString(), MAX_MONSTER_NAME_LENGTH+1 );

		SafeStrCpy( pInfo->ModelName, file.GetString(), 32 );

		pInfo->Image = file.GetDword();
		pInfo->Sight = file.GetDword();
*/
		ZeroMemory(
			pInfo,
			sizeof(*pInfo));
		pInfo->Index = _ttoi(
			textIndex ? textIndex : "");
		SafeStrCpy(
			pInfo->Name,
			textName ? textName : "",
			_countof(pInfo->Name));
		SafeStrCpy(
			pInfo->ModelName,
			textModelName,
			_countof(pInfo->ModelName));
		pInfo->Image = _ttoi(
			textImage ? textImage : "");
		pInfo->Sight = _ttoi(
			textSight ? textSight : "");
		mPetInfoTable.Add( pInfo, pInfo->Index );
	}

	file.Release();

	file.Init("System/Resource/PetHPMPInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		PET_HP_MP_INFO* pInfo = new PET_HP_MP_INFO;

		pInfo->Index	= file.GetByte();
		pInfo->HP		= file.GetByte();
		pInfo->MP		= file.GetByte();

		mPetHPMPInfoTable.Add( pInfo, pInfo->Index );
	}

	file.Release();
}

void CPetManager::UsePetItem( CPlayer* pPlayer, DWORD ItemDBIdx )
{
	CPet* pPet = mPetObjectTable.GetData( ItemDBIdx );

	if( pPet )
	{
		char log[256];
		sprintf( log, "./log/pet%02d.txt", g_pServerSystem->GetMapNum() );
		FILE* fp = fopen( log, "a+" );

		if( fp )
		{
			fprintf( fp, "USERSEAL:\t%d\n", pPet );
			fclose( fp );
		}

		SealPet( pPet );
	}
	else
	{
		SummonPet( pPlayer, ItemDBIdx );
	}
}

PET_OBJECT_INFO* CPetManager::NewPetObjectInfo()
{
	return mPetObjectInfoPool.Alloc();
}

void CPetManager::AddPetObject( CPet* pPet )		
{
	mPetObjectTable.Add( pPet, pPet->GetItemDBIdx() ); 
}

void CPetManager::RemovePetObject( CPet* pPet )	
{
	mPetObjectTable.Remove( pPet->GetItemDBIdx() ); 
}

void CPetManager::NetworkMsgParse( BYTE Protocol, void* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = ( MSGBASE* )pMsg;
	

	switch(Protocol)
	{
	case MP_PET_RECALL_SYN:
		{
			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			if( PetRecall( pPet ) )
			{
			}
			else
			{
			}
		}
		break;
	case MP_PET_TARGET_SYN:
		{
			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;
			if( SetTarget( pPet, pmsg->dwData ) )
			{
			}
			else
			{
			}
		}
		break;
	case MP_PET_ATTACK_SYN:
		{
			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			if( PetAttack( pPet ) )
			{
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_PET;
				msg.Protocol = MP_PET_ATTACK_NACK;
				pPlayer->SendMsg( &msg, sizeof( msg ) ); 
			}
		}
		break;
	case MP_PET_SKILL_SYN:
		{
			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			if( PetSkillUse( pPet, pmsg->dwData ) )
			{
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_PET;
				msg.Protocol = MP_PET_SKILL_NACK;
				pPlayer->SendMsg( &msg, sizeof( msg ) ); 
			}
		}
		break;
	case MP_PET_RESURRECTION_SYN:
		{
			MSG_DWORD3* pmsg = ( MSG_DWORD3* )pMsg;

			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			PET_OBJECT_INFO* pObjectInfo = pPlayer->GetPetInfo( pmsg->dwData1 );

			if( !pObjectInfo )
			{
				return;
			}

			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pmsg->dwData3 );

			if( ITEMMGR->UseItem( pPlayer, pmsg->dwData2, pmsg->dwData3 ) == eItemUseSuccess )
			{
				MSG_ITEM_USE_ACK msg1;
				msg1.Category = MP_ITEM;
				msg1.Protocol = MP_ITEM_USE_ACK;
				msg1.dwObjectID = pPlayer->GetID();
				msg1.TargetPos = pmsg->dwData2;
				msg1.wItemIdx = pmsg->dwData3;
				
				pPlayer->SendMsg( &msg1, sizeof( msg1 ) ); 

				switch( pItemInfo->SupplyType )
				{
				case ITEM_KIND_PET_NORMAL_RESURRECTION:
					{
						pObjectInfo->Friendly = 30;
						pObjectInfo->HP = (DWORD)(pObjectInfo->MAXHP * 0.3f);
						pObjectInfo->MP = (DWORD)(pObjectInfo->MAXMP * 0.3f);
					}
					break;
				case ITEM_KIND_PET_SPECIAL_RESURRECTION:
					{
						pObjectInfo->Friendly = 100;
						pObjectInfo->HP = pObjectInfo->MAXHP;
						pObjectInfo->MP = pObjectInfo->MAXMP;
					}
					break;
				}
				pObjectInfo->State = 0;

				PetInfoUpdate( pPlayer->GetID(), 
				   pObjectInfo->ItemDBIdx, 
				   pObjectInfo->Level, 
				   pObjectInfo->Grade, 
				   pObjectInfo->Type, 
				   pObjectInfo->SkillSlot, 
				   pObjectInfo->Exp, 
				   pObjectInfo->Friendly, 
				   pObjectInfo->MAXHP, 
				   pObjectInfo->HP, 
				   pObjectInfo->MAXMP, 
				   pObjectInfo->MP, 
				   pObjectInfo->AI, 
				   pObjectInfo->State, 
				   pObjectInfo->FriendlyCheckTime );


				MSG_PET_INFO msg2;
				msg2.Category = MP_PET;
				msg2.Protocol = MP_PET_RESURRECTION_ACK;
				memcpy( &( msg2.PetObjectInfo), pObjectInfo, sizeof( PET_OBJECT_INFO ) );

				pPlayer->SendMsg( &msg2, sizeof( msg2 ) ); 

				// 080716 LUJ, 로그
				LogPet( *pObjectInfo, ePetLogRevive );
			}
		}
		break;
	case MP_PET_ITEM_DISCARD_SYN:
		{
			MSG_DWORDBYTE* pmsg = ( MSG_DWORDBYTE* )pMsg;

			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			ITEMBASE* pItem = pPet->GetWearedItem( pmsg->bData );

			if( !pItem )
			{
				return;
			}

			if( pItem->dwDBIdx != pmsg->dwData )
			{
				return;
			}
			
			ItemDeleteToDB( pmsg->dwData );
			
			LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
				eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pItem->wIconIdx, pItem->dwDBIdx,  pItem->Position, 0,
				pItem->Durability, pPlayer->GetPlayerExpPoint());
			
			pPet->SetWearedItem( pmsg->bData, NULL );

			MSG_BYTE msg;
			msg.Category = MP_PET;
			msg.Protocol = MP_PET_ITEM_DISCARD_ACK;
			msg.bData = pmsg->bData;

			pPlayer->SendMsg( &msg, sizeof( msg ) );
		}
		break;
	case MP_PET_ITEM_MOVE_PLAYER_SYN:
		{
			MSG_PET_ITEM_MOVE_SYN* pmsg = ( MSG_PET_ITEM_MOVE_SYN* )pMsg;

			CPlayer* pPlayer = ( CPlayer* )g_pUserTable->FindUser( pmsg->dwObjectID );

			if( !pPlayer )
			{
				return;
			}

			CPet* pPet = pPlayer->GetCurPet();

			if( !pPet )
			{
				return;
			}

			DWORD PlayerItemDBIdx = 0;
			DWORD PetItemDBIdx  = 0;
			WORD TableIdx = GetTableIdxPosition( pmsg->PlayerPos );
			BYTE param = 0;

			if( TableIdx == eItemTable_Storage)
			{
				param = 1;
			}
			else if( TableIdx == eItemTable_Inventory )
			{
				param = 0;
			}
			else
				return;

			CItemSlot* pPlayerSlot = pPlayer->GetSlot( pmsg->PlayerPos );
			const ITEMBASE* pPlayerItemBase = pPlayerSlot->GetItemInfoAbs( pmsg->PlayerPos );
			ITEMBASE PlayerItem;
			ZeroMemory( &PlayerItem, sizeof( ITEMBASE ) );
			
			ITEMBASE* pPetItemBase = pPet->GetWearedItem( pmsg->PetPos );
			ITEMBASE PetItem;
			ZeroMemory( &PetItem, sizeof( ITEMBASE ) );

			if( pPetItemBase )
			{
				PetItem = *pPetItemBase;
				PetItemDBIdx = PetItem.dwDBIdx;
			}

			if( pPlayerItemBase->dwDBIdx )
			{
				PlayerItem.Position = pmsg->PlayerPos;
				PlayerItemDBIdx = pPlayerItemBase->dwDBIdx;

				if( pPlayerSlot->DeleteItemAbs( pPlayer, pmsg->PlayerPos, &PlayerItem ) != EI_TRUE)
				{
					return;
				}

				pPet->SetWearedItem( pmsg->PetPos, &PlayerItem );
				LogItemMoney(
					pPlayer->GetID(),
					pPlayer->GetObjectName(),
					pPet->GetID(),
					pPet->GetObjectName(),
					eLog_ItemMoveFromPlayerToPet,
					pPlayer->GetMoney(),
					0,
					0,
					pmsg->PlayerItemIdx,
					PlayerItemDBIdx,
					pmsg->PlayerPos,
					pmsg->PetPos,
					0,
					0 );
			}

			if( PetItem.dwDBIdx )
			{
				PetItem.Position = pmsg->PlayerPos;

				if( pPlayerSlot->InsertItemAbs( pPlayer, pmsg->PlayerPos, &PetItem) != EI_TRUE)
				{
					return;
				}
				LogItemMoney(
					pPet->GetID(),
					pPet->GetObjectName(),
					pPlayer->GetID(),
					pPlayer->GetObjectName(),
					eLog_ItemMoveFromPetToPlayer,
					0,
					0,
					0,
					pmsg->PetItemIdx,
					PetItemDBIdx,
					pmsg->PetPos,
					pmsg->PlayerPos,
					0,
					0 );
			}

			/*
			if( ItemMoveUpdateToDBbyTable(
				pPlayer,
				DelFromItem.dwDBIdx,
				DelFromItem.wIconIdx,
				DelFromItem.Position,
				DelToItem.dwDBIdx,
				DelToItem.Position ) )
			{
				// Log
				ItemMoveLog(FromPos, ToPos, pPlayer, &DelFromItem);
				if(DelToItem.dwDBIdx != 0)
					ItemMoveLog(ToPos, FromPos, pPlayer, &DelToItem);
			}
			*/

			ItemMoveUpdatePetInven( pPlayer->GetID(), pPlayer->GetUserID(), pPet->GetID(), PlayerItemDBIdx, pmsg->PlayerPos, PetItemDBIdx, pmsg->PetPos, param );

			MSG_PET_ITEM_MOVE_SYN msg;
			msg = *pmsg;
			msg.Protocol = MP_PET_ITEM_MOVE_PLAYER_ACK;
			
			pPlayer->SendMsg( &msg, sizeof( msg ) );
		}
		break;
	}
}

BOOL CPetManager::SetTarget( CPet* pPet, DWORD id )
{
	CObject* pTarget = g_pUserTable->FindUser( id );

	if( !pTarget )
	{
		return FALSE;
	}

	if( pTarget->GetState() == eObjectState_Die )
	{
		return FALSE;
	}

	pPet->SetTarget( id );

	return TRUE;
}

BOOL CPetManager::PetAttack( CPet* pPet )
{
	DWORD id = pPet->GetTarget();

	CObject* pTarget = g_pUserTable->FindUser( id );

	if( !pTarget )
	{
		return FALSE;
	}

	if( pTarget->GetState() == eObjectState_Die )
	{
		return FALSE;
	}

	DWORD skillidx;

	switch( pPet->GetObjectInfo()->Type )
	{
	case ePetType_Basic:
	case ePetType_Physic:	skillidx = mClosedSkillIndex;	break;
	case ePetType_Magic:
	case ePetType_Support:	skillidx = mRangeSkillIndex;	break;
	}

	return SKILLMGR->PetAttack( skillidx, pPet, pTarget );
}

BOOL CPetManager::PetSkillUse( CPet* pPet, DWORD idx )
{
	DWORD id = pPet->GetTarget();

	CObject* pTarget = g_pUserTable->FindUser( id );

	if( !pTarget )
	{
		return FALSE;
	}

	if( pTarget->GetState() == eObjectState_Die )
	{
		return FALSE;
	}

	return SKILLMGR->PetAttack( idx, pPet, pTarget );
}

BOOL CPetManager::PetRecall( CPet* pPet )
{
	CPlayer* pPlayer = pPet->GetMaster();
	VECTOR3* pPlayerPos = CCharMove::GetPosition( pPlayer );
	VECTOR3 pos;

	int ran=0;
	float dx=0,dz=0;
	ran = rand();
	dx = float(ran%mAreaDistance) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%mAreaDistance) * (ran%2?1:-1);
	pos.x = pPlayerPos->x + dx;
	pos.z = pPlayerPos->z + dz;

	if( pos.x < 0 )				pos.x = 0;
	else if( pos.x > 51199 )	pos.x = 51199;

	if( pos.z < 0 )				pos.z = 0;
	else if( pos.z > 51199 )	pos.z = 51199;

	pos.x = ((DWORD)(pos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	pos.z = ((DWORD)(pos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);

	CCharMove::SetPosition( pPet, &pos );

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_WARP;
	msg.dwMoverID = pPet->GetID();
	msg.cpos.Compress(&pos);
	
	PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(msg));
/*
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,sizeof(msg));

	msg.Category = MP_PET;
	msg.Protocol = MP_PET_RECALL_ACK;

	pPlayer->SendMsg( &msg, sizeof( msg ) );
*/
	return TRUE;
}

BOOL CPetManager::HatchPet( CPlayer* pPlayer, const ITEMBASE* pItem )
{
	const ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo( pItem->wIconIdx );

	BYTE skillslot = 0;

	if( pItemInfo->Shop )
	{
		skillslot = mShopBaseSkillSlot;
	}
	else
	{
		skillslot = random( mMinBaseSkillSlot, mMaxBaseSkillSlot );
	}

	PetInfoAdd( pPlayer->GetID(), pPlayer->GetUserID(), pItem->dwDBIdx, pItemInfo->SupplyValue, skillslot );
	
	return TRUE;
}

WORD CPetManager::GetMaxEXP( BYTE grade )
{
	if( grade > mMaxGrade )
	{
		return 0;
	}

	return mExp[ grade ];
}

BYTE CPetManager::GetGradeUpLevel( BYTE grade )
{
	if( grade >= mMaxGrade )
	{
		return 0;
	}

	return mGradeUpLevel[ grade ];
}

float CPetManager::GetFriendlyPenalty( CPet* pPet )
{
	float penalty = 1.0f;
	BYTE friendly = pPet->GetObjectInfo()->Friendly;

	PET_FRIENDLY_PENALTY* pPenalty = NULL;

	mPetFriendlyPenaltyTable.SetPositionHead();
	while( pPenalty = mPetFriendlyPenaltyTable.GetData() )
	{
		if( friendly >= pPenalty->Friendly )
		{
			penalty = ( 100 - pPenalty->Penalty ) * 0.01f;
		}
	}

	return penalty;
}

