#include "stdafx.h"
#include ".\petmanager.h"
#include "MHFile.h"
#include "objectmanager.h"
#include "AppearanceManager.h"
#include "chatmanager.h"
#include "itemmanager.h"
#include "gamein.h"
#include "PetResurrectionDialog.h"
#include "petstatedialog.h"
#include "petweareddialog.h"
#include "inventoryexdialog.h"
#include "storagedialog.h"

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
	mPetHPMPInfoTable.Initialize( 10 );
	mPetFriendlyPenaltyTable.Initialize( 1 );
	mPetObjectInfoTable.Initialize( 1 );

	mPetSpeechInfoTable.Initialize( 1 );
	mPetEmoticonInfoTable.Initialize( 1 );
	mFriendlyStateTable.Initialize( 1 );

	mLastSummonTime = 0;

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

	cPtrList* pList = NULL;

	mPetSpeechInfoTable.SetPositionHead();
	while( pList = mPetSpeechInfoTable.GetData() )
	{
		PTRLISTPOS pos = pList->GetHeadPosition();
		while(pos)
		{
			char* node = (char *)pList->GetNext(pos);
			SAFE_DELETE_ARRAY(node);
		}
		pList->RemoveAll();

		delete pList;
	}
	mPetSpeechInfoTable.RemoveAll();

	mPetEmoticonInfoTable.SetPositionHead();
	while( pList = mPetEmoticonInfoTable.GetData() )
	{
		PTRLISTPOS pos = pList->GetHeadPosition();
		while(pos)
		{
			DWORD* node = (DWORD *)pList->GetNext(pos);
			SAFE_DELETE(node);
		}
		pList->RemoveAll();

		delete pList;
	}
	mPetEmoticonInfoTable.RemoveAll();

	PET_FRIENDLY_STATE* pFriendlyState = NULL;

	mFriendlyStateTable.SetPositionHead();
	while( pFriendlyState = mFriendlyStateTable.GetData() )
	{
		delete pFriendlyState;
	}
	mFriendlyStateTable.RemoveAll();
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

	char buf[1024]={0,};
	char buf2[1024]={0,};

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

		if( strcmp( buf, "#HATCHEFFECT" ) == 0 )
		{
			mHatchEffect = file.GetWord();
		}

		if( strcmp( buf, "#SUMMONEFFECT" ) == 0 )
		{
			mSummonEffect = file.GetWord();
		}

		if( strcmp( buf, "#SEALEFFECT" ) == 0 )
		{
			mSealEffect = file.GetWord();
		}

		if( strcmp( buf, "#APPEAREFFECT" ) == 0 )
		{
			mAppearEffect = file.GetWord();
		}

		if( strcmp( buf, "#DISAPPEAREFFECT" ) == 0 )
		{
			mDisappearEffect = file.GetWord();
		}

		if( strcmp( buf, "#LEVELUPEFFECT" ) == 0 )
		{
			mLevelUpEffect = file.GetWord();
		}

		if( strcmp( buf, "#GRADEUPEFFECT" ) == 0 )
		{
			mGradeUpEffect = file.GetWord();
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

		if( strcmp( buf, "#NAMEHEIGHT" ) == 0 )
		{
			mNameHight = file.GetWord();
		}

		if( strcmp( buf, "#FRIENDLYSTATE" ) == 0 )
		{
			BYTE friendly = file.GetByte();

			PET_FRIENDLY_STATE* pFriendlyState = mFriendlyStateTable.GetData( friendly );

			if( !pFriendlyState )
			{
				pFriendlyState = new PET_FRIENDLY_STATE;
				pFriendlyState->Friendly = friendly;
				mFriendlyStateTable.Add( pFriendlyState, friendly );
			}
			
			file.GetString( buf );
		
			if( strcmp( buf, "*SPEACH" ) == 0 )
			{
				pFriendlyState->SpeechIndex = file.GetDword();
				pFriendlyState->SpeechRate = file.GetByte();
			}
			else if( strcmp( buf, "*EMOTICON" ) == 0 )
			{
				pFriendlyState->EmoticonIndex = file.GetDword();
				pFriendlyState->EmoticonRate = file.GetByte();
			}
			else
			{
				//데이터 오류?
			}
		}

		if( strcmp( buf, "#SPEACH" ) == 0 )
		{
			BOOL start = FALSE;
			cPtrList* pList = new cPtrList;

			DWORD index = file.GetDword();

			while(1)
			{
				if( file.IsEOF() )
					break;

				file.GetLine( buf, 1024 );

				if( buf[ 0 ] == '@' )
				{
					continue;
				}

				if( buf[ 0 ] == '{' )
				{
					start = TRUE;
					continue;
				}

				if( buf[ 0 ] == '}' )
				{
					break;
				}

				if( start )
				{
					char* pData = new char[ strlen( buf ) + 1 ];

					strcpy( pData, buf );

					pData[ strlen( buf ) ] = 0;

					pList->AddTail( pData );
				}
			}

			mPetSpeechInfoTable.Add( pList, index );
		}

		if( strcmp( buf, "#EMOTICON" ) == 0 )
		{
			BOOL start = FALSE;
			cPtrList* pList = new cPtrList;

			DWORD index = file.GetDword();

			while(1)
			{
				if( file.IsEOF() )
					break;

				file.GetString( buf );

				if( buf[ 0 ] == '@' )
				{
					continue;
				}

				if( buf[ 0 ] == '{' )
				{
					start = TRUE;
					continue;
				}

				if( buf[ 0 ] == '}' )
				{
					break;
				}

				if( start )
				{
					DWORD* pData = new DWORD;

					*pData = ( DWORD )atoi( buf );

					pList->AddTail( pData );
				}
			}

			mPetEmoticonInfoTable.Add( pList, index );
		}

		if( strcmp( buf, "#FRIENDLYPENALTY" ) == 0 )
		{
			PET_FRIENDLY_PENALTY* pData = new PET_FRIENDLY_PENALTY;

			pData->Friendly = file.GetByte();
			pData->Penalty = file.GetDword() / 100.f;
		}

		if( strcmp( buf, "#ANIMATION" ) == 0 )
		{
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

				if( strcmp( buf, "{" ) == 0 )
				{
					continue;
				}

				if( strcmp( buf, "}" ) == 0 )
				{
					break;
				}

				if( strcmp( buf, "*RUN" ) == 0 )
				{
					mRunAnimation = file.GetByte();
				}

				if( strcmp( buf, "*WALK" ) == 0 )
				{
					mWalkAnimation = file.GetByte();
				}

				if( strcmp( buf, "*IDLE" ) == 0 )
				{
					mIdleAnimation = file.GetByte();
				}

				if( strcmp( buf, "*DAMAGE" ) == 0 )
				{
					mDamageAnimation = file.GetByte();
				}

			}
		}

		if( strcmp( buf, "#CLOSEDSKILL" ) == 0 )
		{
			mClosedSkillIndex = file.GetDword();
		}

		if( strcmp( buf, "#RANGESKILL" ) == 0 )
		{
			mRangeSkillIndex = file.GetDword();
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
	{
		if(file.IsEOF() != FALSE)
			break;

		PET_INFO* pInfo = new PET_INFO;

		pInfo->Index = file.GetDword();
		// 080714 LUJ, 문자열 영역을 넘어서지 않도록 복사. 넘어설 경우 게임가드에서 자동으로 종료시킴
		SafeStrCpy( pInfo->Name, file.GetString(), sizeof( pInfo->Name ) );
		SafeStrCpy( pInfo->ModelName, file.GetString(), sizeof( pInfo->ModelName ) );

		pInfo->Image = file.GetDword();
		pInfo->Sight = file.GetDword();

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

void CPetManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch( Protocol )
	{
	case MP_PET_HATCH:
		{
			MSG_PET_INFO* pmsg = ( MSG_PET_INFO* )pMsg;

			PET_OBJECT_INFO* pObjectInfo = new PET_OBJECT_INFO;

			memcpy( pObjectInfo, &( pmsg->PetObjectInfo ), sizeof( PET_OBJECT_INFO ) );

			mPetObjectInfoTable.Add( pObjectInfo, pObjectInfo->ItemDBIdx );

			PET_INFO* pInfo = mPetInfoTable.GetData( pObjectInfo->Kind );
			CItem* pItem = ITEMMGR->GetItem( pObjectInfo->ItemDBIdx );

			if( !pInfo || !pItem )
				return;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1573), pInfo->Name, pObjectInfo->SkillSlot );

			pItem->ForcedUnseal();
			ITEMMGR->RefreshItem( pItem );
		}
		break;
	case MP_PET_RECALL_ACK:
		break;
	case MP_PET_RECALL_NACK:
		break;

	case MP_PET_TARGET_ACK:
		break;
	case MP_PET_TARGET_NACK:
		break;

	case MP_PET_ATTACK_ACK:
		HEROPET->SetCanSkillUse();
		break;
	case MP_PET_ATTACK_NACK:
		HEROPET->SetCanSkillUse();
		break;

	case MP_PET_SKILL_ACK:
		HEROPET->SetCanSkillUse();
		break;
	case MP_PET_SKILL_NACK:
		HEROPET->SetCanSkillUse();
		break;

	case MP_PET_EXP_NOTIFY:
		{
			HEROPET->AddExp();
		}
		break;
	case MP_PET_LEVELUP_NOTIFY:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			CPet* pPet = ( CPet* )OBJECTMGR->GetObject( pmsg->dwData );

			if( !pPet )
				return;

			pPet->LevelUp();

			APPEARANCEMGR->InitAppearance( pPet );

			if( pPet == HEROPET )
			{
				HEROPET->CalcStats();
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1647));
			}
			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(mLevelUpEffect,pPet,&set,0,pPet->GetID());
		}
		break;
	case MP_PET_LEVEL_NOTIFY:
		{
			MSG_DWORDBYTE* pmsg = ( MSG_DWORDBYTE* )pMsg;

			CPet* pPet = ( CPet* )OBJECTMGR->GetObject( pmsg->dwData );

			if( !pPet )
				return;

			pPet->SetLevel( pmsg->bData );

			APPEARANCEMGR->InitAppearance( pPet );

			if( pPet == HEROPET )
			{
				HEROPET->CalcStats();
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1647));
			}
		}
		break;
	case MP_PET_GRADEUP_NOTIFY:
		{
			MSG_DWORDBYTE2* pmsg = ( MSG_DWORDBYTE2* )pMsg;

			CPet* pPet = ( CPet* )OBJECTMGR->GetObject( pmsg->dwData );

			if( !pPet )
				return;

			pPet->GradeUp();

			APPEARANCEMGR->InitAppearance( pPet );
			
			if( pPet == HEROPET )
			{
				HEROPET->SetType( pmsg->bData1 );
				HEROPET->SetSkillSlot( pmsg->bData2 );
				HEROPET->CalcStats();
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1576));
				
				CItem* pItem = ITEMMGR->GetItem( HEROPET->GetItemDBIdx() );

				if( pItem )
				{
					ITEMMGR->RefreshItem( pItem );
				}
			}

			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(mGradeUpEffect,pPet,&set,0,pPet->GetID());
		}
		break;
	case MP_PET_FRIENDLY_SYNC:
		{
			MSG_BYTE* pmsg = ( MSG_BYTE* )pMsg;

			HEROPET->SetFriendly( pmsg->bData ); 

			GAMEIN->GetPetStateDlg()->SetInfo();
		}
		break;

	case MP_PET_EVOLUTION_ACK:
		break;
	case MP_PET_EVOLUTION_NACK:
		break;

	case MP_PET_RESURRECTION_ACK:
		{
			MSG_PET_INFO* pmsg = ( MSG_PET_INFO* )pMsg;

			PET_OBJECT_INFO* oldinfo = mPetObjectInfoTable.GetData( pmsg->PetObjectInfo.ItemDBIdx );
			
			if( oldinfo )
			{
				memcpy( oldinfo, &( pmsg->PetObjectInfo ), sizeof( PET_OBJECT_INFO ) );
			}

			CItem* pItem = ITEMMGR->GetItem( pmsg->PetObjectInfo.ItemDBIdx );

			if( pItem )
			{
				ITEMMGR->RefreshItem( pItem );
			}

			CPetResurrectionDialog* window = GAMEIN->GetPetResurrectionDlg();

			if(	window && window->IsActive() )
			{
				window->SetUsedItem( NULL );
				window->SetActive( FALSE );
			}
		}
		break;
	case MP_PET_RESURRECTION_NACK:
		break;
	case MP_PET_LIFE_NOTIFY:
		{
			MSG_INT* pmsg = ( MSG_INT* )pMsg;

			if( HEROPET )
			HEROPET->SetLife( HEROPET->GetLife() + pmsg->nData );
		}
		break;
	case MP_PET_MAXLIFE_NOTIFY:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			if( HEROPET )
			HEROPET->SetMaxLife( pmsg->dwData );
		}
		break;

	case MP_PET_MANA_NOTIFY:			
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			if( HEROPET )
			HEROPET->SetMana( pmsg->dwData );
		}
		break;
	case MP_PET_MAXMANA_NOTIFY:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;

			if( HEROPET )
			HEROPET->SetMaxMana( pmsg->dwData );
		}
		break;

	case MP_PET_EXPPOINT_NOTIFY:		
		break;
	case MP_PET_OBJECT_INFO:
		{
			MSG_PET_INFO* pmsg = ( MSG_PET_INFO* )pMsg;

			PET_OBJECT_INFO* oldinfo = mPetObjectInfoTable.GetData( pmsg->PetObjectInfo.ItemDBIdx );
			
			if( oldinfo )
			{
				memcpy( oldinfo, &( pmsg->PetObjectInfo ), sizeof( PET_OBJECT_INFO ) );
			}
			else
			{
				PET_OBJECT_INFO* pObjectInfo = new PET_OBJECT_INFO;

				memcpy( pObjectInfo, &( pmsg->PetObjectInfo ), sizeof( PET_OBJECT_INFO ) );

				mPetObjectInfoTable.Add( pObjectInfo, pObjectInfo->ItemDBIdx );
			}

			ITEMMGR->RefreshItemToolTip( pmsg->PetObjectInfo.ItemDBIdx );
		}
		break;

	case MP_PET_DIE_NOTIFY:
		{
			MSG_DWORD* pmsg = ( MSG_DWORD* )pMsg;
			
			PET_OBJECT_INFO* oldinfo = mPetObjectInfoTable.GetData( pmsg->dwData );
			if( oldinfo )
			{
				oldinfo->HP = 0;
				oldinfo->MP = 0;
				oldinfo->Friendly = 0;
				oldinfo->State = ePetState_Die;
			}

			CItem* pItem = ITEMMGR->GetItem( pmsg->dwData );

			if( pItem )
			{
				ITEMMGR->RefreshItem( pItem );
			}

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
		}
		break;
	case MP_PET_ITEM_INFO:
		{
			MSG_PET_ITEM_INFO* pmsg = ( MSG_PET_ITEM_INFO* )pMsg;

			CPet* pPet = ( CPet* )OBJECTMGR->GetObject( pmsg->ID );

			if( pPet )
			{
				pPet->SetWearedItem( pmsg->Item );
				APPEARANCEMGR->InitAppearance( pPet );
			}
		}
		break;

	case MP_PET_ITEM_DISCARD_ACK:
		{
			MSG_BYTE* pmsg = ( MSG_BYTE* )pMsg;

			GAMEIN->GetPetWearedDlg()->DeleteAck( pmsg->bData );
		}
		break;
	case MP_PET_ITEM_DISCARD_NACK:
		{
		}
		break;
	case MP_PET_ITEM_ERROR_NACK:
		{
		}
		break;
	case MP_PET_ITEM_MOVE_PLAYER_ACK:
		{
			MSG_PET_ITEM_MOVE_SYN* pmsg = ( MSG_PET_ITEM_MOVE_SYN* )pMsg;

			CInventoryExDialog* pPlayerInven = GAMEIN->GetInventoryDialog();
			CStorageDialog* pPlayerStorage = GAMEIN->GetStorageDialog();
			CPetWearedDialog* pPetInven = GAMEIN->GetPetWearedDlg();

			CItem* pPlayerItem = NULL;

			WORD TableIdx = ITEMMGR->GetTableIdxForAbsPos( pmsg->PlayerPos );
			
			if( TableIdx == eItemTable_Storage)
			{
				pPlayerStorage->DeleteItem( pmsg->PlayerPos, &pPlayerItem );
			}
			else if( TableIdx == eItemTable_Inventory )
			{
				pPlayerInven->DeleteItem( pmsg->PlayerPos, &pPlayerItem );
			}
			else
				return;


			cIcon* pIcon = NULL;
			CItem* pPetItem = NULL;
			pPetInven->DeleteItem( pmsg->PetPos, &pIcon );
			pPetItem = ( CItem* )pIcon;

			if( pPlayerItem )
			{
				pPlayerItem->SetPosition( pmsg->PetPos );
				pPetInven->AddItem( pmsg->PetPos, pPlayerItem );
			}

			if( pPetItem )
			{
				pPetItem->SetPosition( pmsg->PlayerPos );

				if( TableIdx == eItemTable_Storage)
				{
					pPlayerStorage->AddItem( pPetItem );
				}
				else if( TableIdx == eItemTable_Inventory )
				{
					pPlayerInven->AddItem( pPetItem );
				}
				else
					return;
			}
		}
		break;
	case MP_PET_ITEM_MOVE_PLAYER_NACK:
		{
		}
		break;
	}
}

float CPetManager::GetFriendlyPenalty( BYTE friendly  )
{
	float penalty = 1.0f;

	PET_FRIENDLY_PENALTY* pPenalty = NULL;

	mPetFriendlyPenaltyTable.SetPositionHead();
	while( pPenalty = mPetFriendlyPenaltyTable.GetData() )
	{
		if( friendly >= pPenalty->Friendly )
		{
			penalty = ( 100 - pPenalty->Penalty )* 0.01f;
		}
	}

	return penalty;
}
