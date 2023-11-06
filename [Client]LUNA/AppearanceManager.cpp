// AppearanceManager.cpp: implementation of the CAppearanceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppearanceManager.h"

#include "Player.h"
#include "Monster.h"
#include "BossMonster.h"
#include "Npc.h"
#include "ItemGround.h"

#include ".\Engine\EngineObject.h"
#include ".\Engine\EngineEffect.h"
#include "ObjectStateManager.h"

#include "ObjectManager.h"
#include "GameResourceManager.h"
#include "ItemManager.h"
#include "PeaceWarModeManager.h"
#include "MotionManager.h"
#include "CharMakeManager.h"
#include "MainGame.h"
#include "MHMap.h"
#include "MapObject.h"
#include "StreetStallManager.h"

#include "MHCamera.h"

// 070121 LYW --- Include header file.
#include "ObjectBalloon.h"
#include "OBalloonName.h"


#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "battle.h"
#include "BattleSystem_Client.h"
#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"
#endif

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.10.05
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.10.05
#include "InventoryExDialog.h"
#include "GameIn.h"

#include "PetManager.h"

enum
{
	eConfirmAddPlayer_NoErr=0,
	eConfirmAddPlayer_NoData,
	eConfirmAddPlayer_DontAddPlayer,
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CAppearanceManager)

CAppearanceManager::CAppearanceManager()
{

}

CAppearanceManager::~CAppearanceManager()
{
	Release();
}

void CAppearanceManager::Init()
{
	GAMERESRCMNGR->LoadModFileList(GAMERESRCMNGR->m_ModFileList);
	GAMERESRCMNGR->LoadHairModFileList(GAMERESRCMNGR->m_HairModList);
	GAMERESRCMNGR->LoadFaceModFileList(GAMERESRCMNGR->m_FaceModList);
	GAMERESRCMNGR->LoadAvatarEquipList();
	GAMERESRCMNGR->LoadPetModList();

#ifndef _RESOURCE_WORK_NOT_APPLY_
	m_bUseReservation			= FALSE;	
	


	m_pReserItemPlayer = new RESERVATION_ITEM_PLAYER[MAX_RESERVATION_ITEM_PLAYER];

	int i = 0;
	for (i = 0; i < MAX_RESERVATION_ITEM_PLAYER; i++)
	{
		m_lstFreeReserItemPlayer.AddTail(&m_pReserItemPlayer[i]);
	}

	m_dwUsedReserItemPlayerNum	= 0;

#endif
}

void CAppearanceManager::Release()
{	
#ifndef _RESOURCE_WORK_NOT_APPLY_	
	m_lstFreeReserItemPlayer.RemoveAll();


	if (NULL != m_pReserItemPlayer)
	{
		delete[] m_pReserItemPlayer;
		m_pReserItemPlayer = NULL;
	}
#endif
}

void CAppearanceManager::InitAppearanceToEffect(CObject* pObject,CEngineEffect* pEffect)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		SetCharacterAppearanceToEffect(pPlayer,pEffect);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Monster)
	{
		ASSERT(0);	
	}
	else if(pObject->GetObjectKind() == eObjectKind_Npc)
	{
		ASSERT(0);	
	}
}

BOOL CAppearanceManager::SetCharacterAppearanceToEngineObject(CPlayer* pPlayer, CEngineObject* pEngineObject, BOOL IsForEffect)
{
	int PartType;
	int PartModelNum;
	
	BYTE gender = pPlayer->m_CharacterInfo.Gender;
	BYTE race = pPlayer->m_CharacterInfo.Race;
	ASSERT(gender < 2);
	if( gender >= 2 ) gender = 0;

	BYTE hair = pPlayer->m_CharacterInfo.HairType;
	BYTE face = pPlayer->m_CharacterInfo.FaceType;
	DWORD *WearedItemIdx = pPlayer->m_CharacterInfo.WearedItemIdx;
	
	MOD_LIST* pHairList = &GAMERESRCMNGR->m_HairModList[race][gender];
	MOD_LIST* pFaceList = &GAMERESRCMNGR->m_FaceModList[race][gender];
	MOD_LIST* pModList = &GAMERESRCMNGR->m_ModFileList[race][gender];

	/// 모델 정보 초기화
	pEngineObject->Init(pModList->BaseObjectFile,pPlayer,eEngineObjectType_Character);
	pPlayer->SetWingObject( NULL );
	BOOL rt;

	rt = pEngineObject->ChangePart(eAppearPart_Face,"NULL.MOD");
	pPlayer->SetFaceObject( NULL );
	
	EFFECTMGR->ForcedEndEffect( pPlayer->mWeaponEffect[0] );
	EFFECTMGR->ForcedEndEffect( pPlayer->mWeaponEffect[1] );
	EFFECTMGR->ForcedEndEffect( pPlayer->mWeaponEffect[2] );

	pPlayer->mWeaponEffect[0] = 0;
	pPlayer->mWeaponEffect[1] = 0;
	pPlayer->mWeaponEffect[2] = 0;

	/// 머리 셋팅
	ASSERT(pHairList->MaxModFile > hair);
	if( pHairList->MaxModFile <= hair )		hair = 0;
	pEngineObject->ChangePart(eAppearPart_Hair,pHairList->ModFile[hair]);

	BOOL bFace = TRUE;
	BOOL bHelmet = TRUE;
	BOOL bGlasses = TRUE;
	BOOL bMask = TRUE;

	/// 장착 아이템
	/// ChangePart를 사용할 경우 Attach된 오브젝트를 모두 해제한뒤
	/// ChangePart를 사용한뒤 다시 Attach를 사용해야 하므로
	/// Attach후 ChangePart를 사용할 경우 문제가 발생할 요지가 있다.
	/// 따라서 ChangePart를 사용해야 하는 장착 아이템을 먼저 처리한뒤
	/// Attach를 사용해야 하는 장착아이템을 처리한다.

	/// ChangePart를 사용하는 부분	
	for(int n=0;n<eWearedItem_Max;++n)
	{
		if(WearedItemIdx[n])
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(WearedItemIdx[n]);
			
			/// 아이템 정보 없다...
			if(!pInfo)
				continue;

			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			/// 3D 파트 정보가 없으면 아이콘만 표시되는 녀석들이다.
			if( PartType == ePartType_None )
				continue;
			if( PartModelNum == 65535 )
				continue;

			switch( PartType )
			{
			case ePartType_Hair:
				{
					pEngineObject->ChangePart( eAppearPart_Hair, pModList->ModFile[PartModelNum] );
				}
				break;
			case ePartType_Body:
			case ePartType_Costume_Body:
				{
					pEngineObject->ChangePart( eAppearPart_Body, pModList->ModFile[PartModelNum] );
				}
				break;
			case ePartType_Hand:
			case ePartType_Costume_Hand:
				{
					pEngineObject->ChangePart( eAppearPart_Hand, pModList->ModFile[PartModelNum] );
				}
				break;
			case ePartType_Foot:
			case ePartType_Costume_Foot:
				{
					pEngineObject->ChangePart( eAppearPart_Foot, pModList->ModFile[PartModelNum] );
				}
				break;
			case ePartType_Head_Panda:
				{
					pEngineObject->ChangePart( eAppearPart_Hair, pModList->ModFile[PartModelNum] );
					bHelmet = FALSE;
					bGlasses = FALSE;
					bMask = FALSE;
				}
				break;
			case ePartType_Head_Chicken:
				{
					pEngineObject->ChangePart( eAppearPart_Hair, pModList->ModFile[PartModelNum] );
					bHelmet = FALSE;
					bGlasses = TRUE;
					bMask = TRUE;
				}
				break;
			case ePartType_Head_Tights:
				{
					pEngineObject->ChangePart( eAppearPart_Hair, pModList->ModFile[PartModelNum] );
					bHelmet = TRUE;
					bGlasses = TRUE;
					bMask = TRUE;
				}
				break;
			case ePartType_Head_Bag:
				{
					pEngineObject->ChangePart( eAppearPart_Hair, pModList->ModFile[PartModelNum] );
					bHelmet = FALSE;
					bGlasses = FALSE;
					bMask = TRUE;
				}
				break;
			case ePartType_Head_Mask:
				{
					bHelmet = TRUE;
					bGlasses = FALSE;
					bMask = FALSE;
				}
			}
		}
	}

	/// Attach를 사용하는 부분
	for(int n=0;n<eWearedItem_Max;++n)
	{
		if(WearedItemIdx[n])
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(WearedItemIdx[n]);
			
			/// 아이템 정보 없다...
			if(!pInfo)
				continue;

			PartType = pInfo->Part3DType;
			PartModelNum = pInfo->Part3DModelNum;

			/// 3D 파트 정보가 없으면 아이콘만 표시되는 녀석들이다.
			if( PartType == ePartType_None )
				continue;
			if( PartModelNum == 65535 )
				continue;

			switch( PartType )
			{
			case ePartType_OneHand:
			case ePartType_TwoHand:
			case ePartType_TwoBlade:
				{
					if( pPlayer->GetState() != eObjectState_Ungijosik && pPlayer->GetState() != eObjectState_StreetStall_Owner )
					{
						ShowWeapon( pPlayer, pEngineObject );
					}
				}
				break;
			case ePartType_Shield:
				{
					CEngineObject* pShield = NULL;
					pShield = new CEngineObject;
					pShield->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					rt = pEngineObject->AttachWeapon(pShield,"Lhand");	
				}
				break;
			case ePartType_Helmet:
				{
					if( bHelmet )
					{
						CEngineObject* pHelmet = NULL;
						pHelmet = new CEngineObject;
						pHelmet->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
						rt = pEngineObject->AttachDress(pHelmet,"Bone_head");	
					}
				}
				break;
			case ePartType_Head_Panda:
				{
					bFace = FALSE;
				}
/*			case ePartType_Head_Chicken:
			case ePartType_Head_Tights:
			case ePartType_Head_Bag:
			case ePartType_Head_Mask:
				{
					CEngineObject* pHelmet = NULL;
					pHelmet = new CEngineObject;
					pHelmet->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					rt = pEngineObject->AttachDress(pHelmet,"Bone_head");	
				}
*/				break;
			case ePartType_Mask:
				{
					if( bMask )
					{
						CEngineObject* pMask = NULL;
						pMask = new CEngineObject;
						pMask->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
						rt = pEngineObject->AttachDress(pMask,"Bone_head");	
					}
				}
				break;
			case ePartType_Glasses:
				{
					if( bGlasses )
					{
						CEngineObject* pGlasses = NULL;
						pGlasses = new CEngineObject;
						pGlasses->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
						rt = pEngineObject->AttachDress(pGlasses,"Bone_head");	
					}
				}
				break;
			case ePartType_Wing:
			case ePartType_Wing2:
				{
					CEngineObject* pWing = new CEngineObject;
					pWing->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					rt = pEngineObject->AttachDress(pWing,"Bip01 Spine1");

					if( !IsForEffect )
					{
						pPlayer->SetWingObject( pWing );
					}
				}
				break;
			case ePartType_Head_Mask:
				{
					CEngineObject* pMask = NULL;
					pMask = new CEngineObject;
					pMask->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
					rt = pEngineObject->AttachDress(pMask,"Bone_head");	
				}
				break;
			}
		}
	}

	/// 07.05.23 이영준 - 얼굴 교체 임시 블럭
	/// 데이터 패치후 적용 예정
	if( bFace )
	{
		CEngineObject* pFace = new CEngineObject;

		// desc_hseos_얼굴교체01
		// S 얼굴교체 추가 added by hseos 2007.06.07
		// E 얼굴교체 추가 added by hseos 2007.06.07
		pFace->Init(pFaceList->ModFile[0],NULL,eEngineObjectType_Weapon);
		rt = pEngineObject->AttachDress(pFace,"Bip01 Head");

		if( !IsForEffect )
		{
			pPlayer->SetFaceObject(pFace);

			// desc_hseos_얼굴교체01
			// S 얼굴교체 추가 added by hseos 2007.06.07
			// pFace->SetMaterialIndex( face + 72 );
			pFace->SetMaterialIndex( face*2 + 72 );
			// E 얼굴교체 추가 added by hseos 2007.06.07
		}
	}
	else
	{
		pPlayer->SetFaceObject(NULL);
	}

	/// 날개 테스트 코드
	{
//		CEngineObject* pWing = new CEngineObject;
//		pWing->Init("wing_test.chr",NULL,eEngineObjectType_Weapon);
//		rt = pEngineObject->AttachDress(pWing,"Bip01 Spine1");
	}

	//스케일
	VECTOR3 scale;	
	
//---KES CHEAT CHANGESIZE
	if( pPlayer->m_CharacterInfo.Width > 3.0f ) pPlayer->m_CharacterInfo.Width = 3.0f;
	else if( pPlayer->m_CharacterInfo.Width < 0.3f ) pPlayer->m_CharacterInfo.Width = 0.3f;

	if( pPlayer->m_CharacterInfo.Height > 3.0f ) pPlayer->m_CharacterInfo.Height = 3.0f;
	else if( pPlayer->m_CharacterInfo.Height < 0.3f ) pPlayer->m_CharacterInfo.Height = 0.3f;
//----------------------
	SetVector3(&scale,
		pPlayer->m_CharacterInfo.Width,
		pPlayer->m_CharacterInfo.Height,
		pPlayer->m_CharacterInfo.Width);

	pEngineObject->SetScale(&scale);
		
	pEngineObject->ApplyHeightField(TRUE);
	pPlayer->SetPosition(&pPlayer->m_MoveInfo.CurPosition);
	pPlayer->SetAngle(pPlayer->m_RotateInfo.Angle.ToRad());
	
	if(pPlayer->GetID() == HEROID)
	{
		pPlayer->GetEngineObject()->DisablePick();
	}
/*	
	// 잔상 테스트 코드
	CEngineObject* pKGEffect = new CEngineObject;
	pKGEffect->Init("jansang.MOD",NULL,eEngineObjectType_Effect);
	pKGEffect->Attach(pEngineObject,"Bip01 Head");	
	pKGEffect->InitializeIllusionEffect(20,NULL);
	pKGEffect->BeginIllusionEffect();
*/

	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pPlayer->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
	
	BYTE level = pPlayer->m_CharacterInfo.WeaponEnchant;
	DWORD WeaponIdx = WearedItemIdx[eWearedItem_Weapon]; 

	if( WeaponIdx && level >= 3 )
	{
		level -= 3;

		EnchantEffect* pEffect = ITEMMGR->GetItemEffect( WeaponIdx );

		if( pEffect )
		{
			for( int i = 0; i < 3; i++ )
			{
				if( pEffect->Effect[ level ][ i ] == 0 )
					continue;

				TARGETSET set;
				set.pTarget = pPlayer;
				pPlayer->mWeaponEffect[ i ] = EFFECTMGR->StartEffectProcess( pEffect->Effect[ level ][ i ], pPlayer, &set, 0, pPlayer->GetID() );
			}
		}
	}
	return TRUE;
}


void CAppearanceManager::ShowWeapon(CPlayer* pPlayer)
{
	SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);
	ShowWeapon(pPlayer,pPlayer->GetEngineObject());
	
	if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
	}

	OBJECTMGR->ApplyShadowOption(pPlayer);
/*
	
	DWORD* AvatarItemIdx = pPlayer->GetShopItemStats()->Avatar;
	if( AvatarItemIdx[eAvatar_Dress] > 0 )
	{		
		AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( AvatarItemIdx[eAvatar_Dress] );
		if( pAvatarItem )
		{
			WORD weapon = pPlayer->GetWeaponEquipType();

			for(int i=eAvatar_Weared_Gum; i<=eAvatar_Weared_Amgi; ++i)
			{
				if( pAvatarItem->Item[i] == 0 && weapon == i-(eAvatar_Weared_Gum-1) )
				{
					HideWeapon( pPlayer, &pPlayer->m_EngineObject );
					break;
				}				
			}
		}
	}
*/
}
void CAppearanceManager::HideWeapon(CPlayer* pPlayer)
{
	SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);
	HideWeapon(pPlayer,pPlayer->GetEngineObject());
	
	if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
	}

	OBJECTMGR->ApplyShadowOption(pPlayer);
}

void CAppearanceManager::SetCharacterAppearance(CPlayer* pPlayer)
{
	SetCharacterAppearanceToEngineObject(pPlayer,&pPlayer->m_EngineObject);

	if(pPlayer->GetState() == eObjectState_None)
		OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);
	if( pPlayer->GetState() == eObjectState_Move &&
		pPlayer->m_MoveInfo.KyungGongIdx == 0)
		pPlayer->SetState(eObjectState_Move);
	if( pPlayer->GetState() == eObjectState_Rest )
	{	
		pPlayer->SetState(eObjectState_Rest);
	}

	if(pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)	
	{
		if( pPlayer->GetCharacterTotalInfo()->HideLevel == 0 )
		{
#ifdef _GMTOOL_
		if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
			pPlayer->GetEngineObject()->SetAlpha( 0.3f );
		else
#endif
			pPlayer->GetEngineObject()->HideWithScheduling();
			//pPlayer->GetEngineObject()->Hide();
		}
		else
		{
			OBJECTMGR->SetHide( pPlayer->GetID(), pPlayer->GetCharacterTotalInfo()->HideLevel );
		}
	}
	
	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pPlayer->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13

	//---KES 중복 호출중?
	//OBJECTMGR->ApplyShadowOption(pPlayer);
}

void CAppearanceManager::SetBossMonsterAppearance(CBossMonster* pMonster)
{
	pMonster->m_EngineObject.Release();
	
	WORD mkind = pMonster->GetMonsterKind();
	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(mkind);
	
	pMonster->m_EngineObject.Init(pList->ChxName, pMonster, eEngineObjectType_Monster);
	pMonster->m_EngineObject.ApplyHeightField(TRUE);
	
	//////////////////////////////////////////////////////////////////////////
	// 몬스터 크기 적용
	VECTOR3 Scale;
	// taiyo test
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pMonster->m_EngineObject.SetScale(&Scale);

	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pMonster->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
}

void CAppearanceManager::SetMonsterAppearance(CMonster* pMonster)
{
	pMonster->m_EngineObject.Release();
	
	WORD mkind = pMonster->GetMonsterKind();

	BASE_MONSTER_LIST* pList = GAMERESRCMNGR->GetMonsterListInfo(mkind);
	pMonster->m_EngineObject.Init(pList->ChxName,pMonster,eEngineObjectType_Monster);
	pMonster->m_EngineObject.ApplyHeightField(TRUE);


	/////////////////////////////////////////////////////////////////////////
	// taiyo 
	// 모션 정보 로드

	MOTIONMGR->SetMotionInfo(pMonster, 0);

	//////////////////////////////////////////////////////////////////////////
	// 몬스터 크기 적용
	VECTOR3 Scale;
	// taiyo test
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pMonster->m_EngineObject.SetScale(&Scale);

	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pMonster->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
}
void CAppearanceManager::SetNpcAppearance(CNpc* pNpc)
{
	pNpc->m_EngineObject.Release();
		
	WORD nkind = pNpc->GetNpcKind();
	NPC_LIST* pList = GAMERESRCMNGR->GetNpcInfo(nkind);
	int ChxNum = pList->ModelNum;
	char* pNpcChxName = GAMERESRCMNGR->GetNpcChxName(ChxNum);
	
	pNpc->m_EngineObject.Init(pNpcChxName,pNpc,eEngineObjectType_Npc);
	pNpc->SetPosition(&pNpc->m_MoveInfo.CurPosition);
	pNpc->m_EngineObject.ApplyHeightField(TRUE);
	pNpc->SetAngle(pNpc->m_RotateInfo.Angle.ToRad());
	
	pNpc->m_EngineObject.ChangeMotion(1);

	pNpc->m_EngineObject.RandMotionFrame();
	
	if( pNpc->GetNpcJob() == 0 )
		pNpc->m_EngineObject.DisablePick();

	// 070121 LYW --- Setting to disable pick if npc kind is 31.
	if( pNpc->GetNpcKind() == 31 )
	{
		pNpc->m_EngineObject.DisablePick() ;
	}

	//////////////////////////////////////////////////////////////////////////
	// Npc 크기 적용
	VECTOR3 Scale;
	//
	Scale.x = Scale.y = Scale.z = pList->Scale;
	pNpc->m_EngineObject.SetScale(&Scale);

	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pNpc->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
}

void CAppearanceManager::SetPetAppearance(CPet* pPet)
{
	int PartType;
	int PartModelNum;
	BOOL rt;

//	pPet->m_EngineObject.Release();
	
	DWORD kind = pPet->GetKind();

	PET_INFO* pInfo = PETMGR->GetPetInfo( kind );
	MOD_LIST* pModList = &GAMERESRCMNGR->mPetModList;
	
	pPet->m_EngineObject.Init( pInfo->ModelName, pPet, eEngineObjectType_Pet );
	pPet->m_EngineObject.ApplyHeightField(TRUE);
	
	for( int i = ePetEquipSlot_Face; i <= ePetEquipSlot_Tail; i++ ) 
	{
		DWORD itemidx = pPet->mWearedItem[ i ];
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(itemidx);

		/// 아이템 정보 없다...
		if(!pInfo)
			continue;

		PartType = pInfo->Part3DType;
		PartModelNum = pInfo->Part3DModelNum;

		/// 3D 파트 정보가 없으면 아이콘만 표시되는 녀석들이다.
		if( PartType == ePartType_None )
			continue;
		if( PartModelNum == 65535 )
			continue;

		switch( PartType )
		{
		case ePartType_Pet_Face:
			{
				CEngineObject* pCostume = NULL;
				pCostume = new CEngineObject;
				pCostume->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Pet);
				rt = pPet->m_EngineObject.AttachDress(pCostume,"Bone02");	
			}
			break;
		case ePartType_Pet_Head:
			{
				CEngineObject* pCostume = NULL;
				pCostume = new CEngineObject;
				pCostume->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Pet);
				rt = pPet->m_EngineObject.AttachDress(pCostume,"Bone24");	
			}
			break;
		case ePartType_Pet_Wing:
			{
				CEngineObject* pCostume = NULL;
				pCostume = new CEngineObject;
				pCostume->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Pet);
				rt = pPet->m_EngineObject.AttachDress(pCostume,"Bone01");	
			}
			break;
		case ePartType_Pet_Tail:
			{
				CEngineObject* pCostume = NULL;
				pCostume = new CEngineObject;
				pCostume->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Pet);
				rt = pPet->m_EngineObject.AttachDress(pCostume,"Bone07");	
			}
			break;
		}
	}
	
	MOTIONMGR->SetMotionInfo(pPet, 0);
	
	VECTOR3 Scale;
	float	addScale = ( pPet->GetLevel() / 5 ) * 0.05f;

	Scale.x = Scale.y = Scale.z = 1.0f + addScale;
	pPet->m_EngineObject.SetScale(&Scale);

	// desc_hseos_라이트효과01
	// S 라이트효과 추가 added by hseos 2007.06.13
	pPet->GetEngineObject()->EnableSelfIllumin();
	// E 라이트효과 추가 added by hseos 2007.06.13
	pPet->SetPosition(&pPet->m_MoveInfo.CurPosition);
	pPet->SetAngle(pPet->m_RotateInfo.Angle.ToRad());
}

void CAppearanceManager::SetMapObjectAppearance(CMapObject* pMapObject, char* strData, float fScale )
{
	pMapObject->m_EngineObject.Release();

	pMapObject->m_EngineObject.Init( strData, pMapObject, eEngineObjectType_MapObject) ;

	pMapObject->SetPosition( &pMapObject->m_MoveInfo.CurPosition );
	pMapObject->m_EngineObject.ApplyHeightField(TRUE);
	pMapObject->SetAngle( pMapObject->m_RotateInfo.Angle.ToRad() );

	VECTOR3 Scale;
	Scale.x = Scale.y = Scale.z = fScale;
	pMapObject->m_EngineObject.SetScale(&Scale);
}

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.05.07
void CAppearanceManager::SetFarmRenderObjAppearance(CSHFarmRenderObj* pFarmRenderObj)
{
	pFarmRenderObj->m_EngineObject.Release();
		
	WORD nKind = pFarmRenderObj->GetInfo()->nKind;
	CSHFarmManager::stFARMRENDEROBJLIST* pList = g_csFarmManager.CLI_GetFarmRenderObjList(nKind);
	
	pFarmRenderObj->m_EngineObject.Init(pList->szChxName, pFarmRenderObj, eEngineObjectType_FarmRenderObj);
	pFarmRenderObj->SetPosition(&pFarmRenderObj->m_MoveInfo.CurPosition);
	pFarmRenderObj->m_EngineObject.ApplyHeightField(TRUE);
	pFarmRenderObj->SetAngle(pFarmRenderObj->m_RotateInfo.Angle.ToRad());
	
	pFarmRenderObj->m_EngineObject.ChangeMotion(1);

	pFarmRenderObj->m_EngineObject.RandMotionFrame();
	
	//////////////////////////////////////////////////////////////////////////
	// Npc 크기 적용
	VECTOR3 Scale;
	//
	Scale.x = Scale.y = Scale.z = pList->nScale;
	pFarmRenderObj->m_EngineObject.SetScale(&Scale);
	pFarmRenderObj->GetEngineObject()->EnableSelfIllumin();
}
// E 농장시스템 추가 added by hseos 2007.05.07

void CAppearanceManager::SetCharacterAppearanceToEffect(CPlayer* pPlayer,CEngineEffect* pRtEffect)
{
	SetCharacterAppearanceToEngineObject(pPlayer,pRtEffect,TRUE);
}

void CAppearanceManager::InitAppearance(CObject* pObject)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		SetCharacterAppearance((CPlayer*)pObject);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Monster)
	{
		SetMonsterAppearance((CMonster*)pObject);
	}
	else if(pObject->GetObjectKind() == eObjectKind_Npc)
	{
		SetNpcAppearance((CNpc*)pObject);
	}
	else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
	{
		SetBossMonsterAppearance((CBossMonster*)pObject);
	}
	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.05.07
	else if (pObject->GetObjectKind() == eObjectKind_FarmObj)
	{
		SetFarmRenderObjAppearance((CSHFarmRenderObj*)pObject);
	}
	// E 농장시스템 추가 added by hseos 2007.05.07
	else if( pObject->GetObjectKind() == eObjectKind_Pet )
	{
		SetPetAppearance((CPet*)pObject);
	}
}

void CAppearanceManager::SetScalebyGuageBar(DWORD PlayerID)
{
	CObject* pPlayer;	
	pPlayer = OBJECTMGR->GetObject(PlayerID);
	if(pPlayer == NULL)
	{
		ASSERT(0);
		return;
	}
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

}

void CAppearanceManager::AddCharacterPartChange(DWORD PlayerID)
{
	m_PtrList.AddHead((void*)PlayerID);
}

void CAppearanceManager::ProcessAppearance()
{
	DWORD PlayerID;
	CObject* pPlayer;
	while(PlayerID = (DWORD)m_PtrList.RemoveTail())
	{
		pPlayer = OBJECTMGR->GetObject(PlayerID);
		if(pPlayer == NULL)
			continue;
		ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);		
		SetCharacterAppearance((CPlayer*)pPlayer);
	}
}


void CAppearanceManager::ShowWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject)
{
	DWORD WeaponIdx = pPlayer->GetWearedItemIdx(eWearedItem_Weapon);
	if(WeaponIdx == 0)
		return;
	
	BYTE gender = pPlayer->m_CharacterInfo.Gender;
	BYTE race = pPlayer->m_CharacterInfo.Race;

	ASSERT(gender < 2);
	MOD_LIST* pModList = &GAMERESRCMNGR->m_ModFileList[race][gender];

	HideWeapon(pPlayer,pEngineObject);
	
	int PartType,PartModelNum;
	{	// 무기 Attach 처리
		if(WeaponIdx)
		{
			PartType = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DType;
			PartModelNum = ITEMMGR->GetItemInfo(WeaponIdx)->Part3DModelNum;
			if( PartType != ePartType_None )
			{
				WORD WeaponType = ITEMMGR->GetItemInfo(WeaponIdx)->WeaponType;
				CEngineObject* pWeapon = new CEngineObject;

				pWeapon->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);

				if( WeaponType == eWeaponType_Bow )
					pEngineObject->AttachWeapon(pWeapon,LEFT_HAND);
				else
					pEngineObject->AttachWeapon(pWeapon,RIGHT_HAND);
			}
		}
	}


	// 방패 Attach 처리
	DWORD ShieldIdx = 0 ; 
	ShieldIdx = pPlayer->GetWearedItemIdx(eWearedItem_Shield);

	if(ShieldIdx == 0)
		return;
	
	if(ShieldIdx != 0)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(ShieldIdx) ;

		PartType = pInfo->Part3DType;
		PartModelNum = pInfo->Part3DModelNum;

		if( PartType != ePartType_None )
		{
			WORD ShieldType = pInfo->WeaponType;
			CEngineObject* pShield = new CEngineObject;

			pShield->Init(pModList->ModFile[PartModelNum],NULL,eEngineObjectType_Weapon);
			
			pEngineObject->AttachWeapon(pShield,LEFT_HAND);
		}
	}
}

void CAppearanceManager::HideWeapon(CPlayer* pPlayer,CEngineObject* pEngineObject)
{
	pEngineObject->RemoveAllAttachedWeapon();
}

void CAppearanceManager::SetAvatarItem( CPlayer* pPlayer, WORD ItemIdx, BOOL bAdd )
{
	DWORD* pAvatarItem = pPlayer->GetShopItemStats()->Avatar;
	AVATARITEM* pAvatarUseInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if( !pAvatarUseInfo )		return;
	
	if( bAdd )
	{
		for(int i=0; i<eAvatar_Max; i++)
		{
			if( i >= eAvatar_Weared_Hair )
			{
				if( !pAvatarUseInfo->Item[i] )
					pAvatarItem[i] = 0;
			}
			else if( i == pAvatarUseInfo->Position )
			{
				if( pAvatarItem[i] )				
				if( pPlayer->GetID() == HERO->GetID() )
				{
					MSG_DWORD_WORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_CHANGE;
					msg.dwObjectID = HERO->GetID();
					msg.wData = i;						// 어느위치의 데이타인지
					msg.dwData = pAvatarItem[i];					
					NETWORK->Send( &msg, sizeof(msg) );
				}

				// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatarItem[i] );
				if( !pTemp )		continue;
				for(int k=0; k<eAvatar_Max; k++)
				{
					if( k >= eAvatar_Weared_Hair )
					if( !pTemp->Item[k] )
						pAvatarItem[k] = 1;
				}

				pAvatarItem[i] = ItemIdx;
			}
			else if( !pAvatarUseInfo->Item[i] && pAvatarItem[i] )
			{				
				if( pPlayer->GetID() == HERO->GetID() )
				{
					MSG_WORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
					msg.dwObjectID = HERO->GetID();
					msg.wData = ItemIdx;					// 어느위치의 데이타인지					
					NETWORK->Send( &msg, sizeof(msg) );
				}				
				
				// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
				AVATARITEM* pAvatarUseInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatarItem[i] );
				if( !pAvatarUseInfo )		continue;
				for(int k=0; k<eAvatar_Max; k++)
				{
					if( k >= eAvatar_Weared_Hair )
					if( !pAvatarUseInfo->Item[k] )
						pAvatarItem[k] = 1;					
				}

				pAvatarItem[i] = 0;
			}
		}
	}
	else
	{		
		for(int i=0; i<eAvatar_Max; i++)
		{
			if( i < eAvatar_Weared_Hair )
			{
				if( i == pAvatarUseInfo->Position )				
					pAvatarItem[i] = 0;
			}			
			else
			{
				if( !pAvatarUseInfo->Item[i] )
					pAvatarItem[i] = 1;
			}
		}		
	}
	
	AddCharacterPartChange( pPlayer->GetID() );
}

#ifndef _RESOURCE_WORK_NOT_APPLY_


VOID CAppearanceManager::CancelReservation(CObject* pObject)
{
	if (FALSE == m_bUseReservation)
	{
		return;
	}

	if (NULL == pObject)
	{
		return;
	}

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		RESERVATION_ITEM_PLAYER* pReservItem = (RESERVATION_ITEM_PLAYER*)pObject->GetEngineObject()->GetCacheListData();

		if (NULL != pReservItem)
		{
			m_lstReserveToAddPlayerList.Remove((void*)pReservItem);
		}

		pObject->GetEngineObject()->SetCacheListData(NULL);
	}
}

VOID CAppearanceManager::CancelAlphaProcess(CObject* pObject)
{
	if (FALSE == m_bUseReservation)
	{
		return;
	}

	if (NULL == pObject)
	{
		return;
	}

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		pObject->GetEngineObject()->SetAlpha(1.0f);
		m_lstAlphaProcessing.Remove(pObject);
	}
}

BOOL CAppearanceManager::ReserveToAppearObject(CObject* pObject, void* pMsg)
{
	if (FALSE == m_bUseReservation)
	{
		InitAppearance(pObject);
		return FALSE;
	}
		
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{	

		RESERVATION_ITEM_PLAYER* pReservItem = AllocReservationItem();
		if (NULL != pReservItem)
		{
			pReservItem->pObject	= (CPlayer*)pObject;
			pReservItem->Msg		= *(SEND_CHARACTER_TOTALINFO*)pMsg;

			m_lstReserveToAddPlayerList.AddTail((void*)pReservItem);	
			
			pObject->GetEngineObject()->SetCacheListData((void*)pReservItem);			
		}
		else
		{
			pObject->GetEngineObject()->SetCacheListData(NULL);
		}
		
	}
	/*
	else if(pObject->GetObjectKind() == eObjectKind_Monster)
	{		
		m_lstReserveToAddMonsterList.AddTail((void*)pObject);		
	}
	*/
	else
	{
		InitAppearance(pObject);
	}
	
	return TRUE;
}


#define REACTION_PROCESS_RESERVATION_TIME	200
#define LOAD_OBJECT_NUMBER_PER_PROCESS		3


VOID CAppearanceManager::ProcessReservation(void)
{
	if (FALSE == m_bUseReservation)
	{		
		return;
	}	
	

	DWORD	dwCurrentTick = GetTickCount();
	int		iLoopLimit = 0;
	int		i = 0;

	if (dwCurrentTick >= m_dwLastProcessReservationTime + REACTION_PROCESS_RESERVATION_TIME)
	{	
		for(int i=0; i<LOAD_OBJECT_NUMBER_PER_PROCESS; ++i)
		{
			if (0 < m_lstReserveToAddPlayerList.GetCount())
			{
				RESERVATION_ITEM_PLAYER* pReservItem = NULL;
				pReservItem = (RESERVATION_ITEM_PLAYER*)m_lstReserveToAddPlayerList.RemoveHead();

				if (NULL != pReservItem)
				{
					CPlayer* pPlayer = pReservItem->pObject;

					if (NULL != pPlayer)
					{
						InitAppearance(pPlayer);
						
						PostProcessAddPlayer(pPlayer, &pReservItem->Msg);
						
						if( pPlayer->GetCharacterTotalInfo()->bVisible )
						{
							pPlayer->GetEngineObject()->SetAlpha(0.2f);
							m_lstAlphaProcessing.AddTail(pPlayer);
						}
					}

					FreeReservationItem(pReservItem);
					pReservItem = NULL;

					pPlayer->GetEngineObject()->SetCacheListData(NULL);

				}
			}
		}

		// player
		
		/*
		if ( LOAD_OBJECT_NUMBER_PER_PROCESS >= m_lstReserveToAddPlayerList.GetCount() )
		{
			iLoopLimit = m_lstReserveToAddPlayerList.GetCount();
		}
		else
		{
			iLoopLimit = LOAD_OBJECT_NUMBER_PER_PROCESS;
		}
		
		int i = 0;		
		RESERVATION_ITEM_PLAYER* pReservItem = NULL;
		for (i = 0; i < iLoopLimit; i++) 
		{
			pReservItem = (RESERVATION_ITEM_PLAYER*)m_lstReserveToAddPlayerList.RemoveHead();

			if (NULL != pReservItem)
			{
				CPlayer* pPlayer = pReservItem->pObject;

				if (NULL != pPlayer)
				{
					InitAppearance(pPlayer);		
					
					//pPlayer->GetEngineObject()->SetCompoundAlpha(50);			

					PostProcessAddPlayer(pPlayer, &pReservItem->Msg);				

					pPlayer->GetEngineObject()->SetAlpha(0.2f);
					m_lstAlphaProcessing.AddTail(pPlayer);
				}

				FreeReservationItem(pReservItem);
				pReservItem = NULL;


			}							
		}	
		*/

		// monster

		/*
		CMonster* pMonster = NULL;

		if ( LOAD_OBJECT_NUMBER_PER_PROCESS >= m_lstReserveToAddMonsterList.GetCount() )
		{
			iLoopLimit = m_lstReserveToAddMonsterList.GetCount();
		}
		for (i = 0; i < iLoopLimit; i++)
		{			
			pMonster = (CMonster*)m_lstReserveToAddMonsterList.RemoveHead();
			InitAppearance(pMonster);	
			
			
			//pMonster->GetEngineObject()->SetCompoundAlpha(100);
			//m_lstAlphaProcessing.AddTail(pMonster);
			
		}
		*/


		// npc

		m_dwLastProcessReservationTime = dwCurrentTick;
	}
}



RESERVATION_ITEM_PLAYER* CAppearanceManager::AllocReservationItem(VOID)
{

	if (m_dwUsedReserItemPlayerNum < MAX_RESERVATION_ITEM_PLAYER)
	{
		++m_dwUsedReserItemPlayerNum;
		return (RESERVATION_ITEM_PLAYER*)m_lstFreeReserItemPlayer.RemoveHead();
	}

	return NULL;
}

BOOL CAppearanceManager::FreeReservationItem(RESERVATION_ITEM_PLAYER* pItem)
{
	if (NULL == pItem)
	{
		return FALSE;
	}

	if (0 < m_dwUsedReserItemPlayerNum)
	{
		--m_dwUsedReserItemPlayerNum;
		m_lstFreeReserItemPlayer.AddTail((void*)pItem);
	}

	return TRUE;
}


VOID CAppearanceManager::ProcessAlphaAppearance(VOID)
{		
	float fAlpha = 0.0f;

	cPtrList removelist;

	PTRLISTPOS pos = m_lstAlphaProcessing.GetHeadPosition();
	
	while( pos )
	{
		CObject* pObject = (CObject*)m_lstAlphaProcessing.GetNext(pos);
		
		if( pObject )
		{
			fAlpha = pObject->GetEngineObject()->GetAlpha();			
			
			if (0.9f <= fAlpha)
			{
				pObject->GetEngineObject()->SetAlpha(1.0f);					
				
				//m_lstAlphaProcessing.GetNext(pos);
				//m_lstAlphaProcessing.Remove(pObject);
				removelist.AddTail(pObject);
			}
			else
			{
				fAlpha += 0.025f;
				
				pObject->GetEngineObject()->SetAlpha(fAlpha);			
			}
		}
	}

	pos = removelist.GetHeadPosition();
	while( pos )
	{
		CObject* pObject = (CObject*)removelist.GetNext(pos);
		if( pObject )
			m_lstAlphaProcessing.Remove( pObject );
	}
	removelist.RemoveAll();
}



DWORD ConfirmAddPlayer( CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pmsg )
{
	if( !pPlayer || !pmsg )		return eConfirmAddPlayer_NoData;

	WORD TournamentTeam = 0;

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)			
			yCASE(CAddableInfoList::GTournament)
				iter.GetInfoData( &TournamentTeam );
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	if( TournamentTeam == 2 )			return eConfirmAddPlayer_DontAddPlayer;

	return eConfirmAddPlayer_NoErr;
}

void PostProcessAddPlayer(CPlayer* pPlayer, SEND_CHARACTER_TOTALINFO* pmsg)
{
	if (NULL == pPlayer)
	{
		return;
	}

	if (NULL == pmsg)
	{
		return;
	}


	OBJECTSTATEMGR->InitObjectState(pPlayer);
	OBJECTMGR->ApplyOverInfoOption( pPlayer );
	OBJECTMGR->ApplyShadowOption( pPlayer );

	///


	BOOL bShowdown = FALSE;
	BOOL bPartyWar = FALSE;
	BOOL bGTournament = FALSE;
	BOOL bSiegeWar = FALSE;
	int wPartyTeam = 0;
	int wGTournamentTeam = 0;
	WORD StallKind = 0;
	SEIGEWAR_CHARADDINFO SiegeWarInfo;
	COMPRESSEDPOS ShowdownPos;
	static char StreetStallTitle[MAX_STREETSTALL_TITLELEN+1] = {0,};

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)			
			yCASE(CAddableInfoList::ShowdownInfo)
				bShowdown = TRUE;
				iter.GetInfoData(&ShowdownPos);
			yCASE(CAddableInfoList::StreetStall)
				StallKind = eSK_SELL;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::StreetBuyStall)
				StallKind = eSK_BUY;
				iter.GetInfoData(StreetStallTitle);
			yCASE(CAddableInfoList::PartyWar)
				bPartyWar = TRUE;
				iter.GetInfoData( &wPartyTeam );
			yCASE(CAddableInfoList::GTournament)
				bGTournament = TRUE;
				iter.GetInfoData( &wGTournamentTeam );
			yCASE(CAddableInfoList::SiegeWar)
				bSiegeWar = TRUE;
				iter.GetInfoData( &SiegeWarInfo );				
		yENDSWITCH
		
		iter.ShiftToNextData();
	}

	if( pmsg->TotalInfo.bPKMode )
	{
		//OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
		OBJECTEFFECTDESC desc( 39 );	//---KES PK
		pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
	}

	//SW051112 무쌍모드
	if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE))
	{
		if( pmsg->TotalInfo.bMussangMode )
		{
			// 06. 03. 국내무쌍 - 이영준
/*			switch(pPlayer->GetStage())
			{
			case eStage_Normal:	
				{
*/					OBJECTEFFECTDESC desc(1035);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
/*				}
				break;
			case eStage_Hwa:		
			case eStage_Hyun:
				{
					OBJECTEFFECTDESC desc(888);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
				}
				break;
			case eStage_Geuk:
			case eStage_Tal:
				{
					OBJECTEFFECTDESC desc(889);
					pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
				}
				break;
			}
*/		}
	}
	if(pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pPlayer;
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pPlayer,&set,0,pPlayer->GetID());
		pPlayer->ChangeBaseMotion( pPlayer->GetStandardMotion() );
	}
	else
	{
		if( pmsg->BaseObjectInfo.ObjectState == eObjectState_StreetStall_Owner )
		{
			if(StallKind == eSK_SELL)
				pPlayer->ShowStreetStallTitle( TRUE, StreetStallTitle );
			else if(StallKind == eSK_BUY)
				pPlayer->ShowStreetBuyStallTitle( TRUE, StreetStallTitle );
		}
	}

	// 해당 효과를 제거해달라는 기획팀 요구로 인해 주석 처리
	//if( bShowdown )
	//{
	//	if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam1 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_oneself_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
	//	}
	//	else if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam2 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_enemy_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//}

	//if( bPartyWar )
	//{
	//	if( wPartyTeam == 1 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_oneself_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//	else if( wPartyTeam == 2 )
	//	{
	//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_enemy_s.beff"));
	//		pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
	//	}
	//}
	
	if( bGTournament )
	{
		// 해당 효과를 제거해달라는 기획팀 요구로 인해 주석 처리
		/*if( wGTournamentTeam == 0 )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_oneself_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			pPlayer->GetCharacterTotalInfo()->bVisible = true;
		}
		else if( wGTournamentTeam == 1 )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("maintain_dual_enemy_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
			pPlayer->GetCharacterTotalInfo()->bVisible = true;
		}
		else */if( wGTournamentTeam == 2 )
		{
			// Observer
			pPlayer->GetCharacterTotalInfo()->bVisible = false;
			if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM && CAMERA->GetCameraMode() != eCM_EyeView )
				pPlayer->GetEngineObject()->SetAlpha( 0.3f );
			else
				pPlayer->GetEngineObject()->HideWithScheduling();
		}		
	}

	if( bSiegeWar && HERO )
	{
		((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName( pPlayer, SiegeWarInfo.Team, pPlayer->GetGuildIdx() );

		if( SiegeWarInfo.bEngrave )
		{
			OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_Engrave);
			pPlayer->ChangeMotion(329, TRUE);

			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
			pPlayer->AddObjectEffect( ENGRAVE_EFFECTID, &desc, 1, HERO );			
		}

		if( pPlayer->IsRestraintMode() )
		{
			pPlayer->SetRestraintMode( TRUE );
			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			//OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
			OBJECTEFFECTDESC desc( 39 );	//---KES PK
			pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);
		}
	}
}




#endif
