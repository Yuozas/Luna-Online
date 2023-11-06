#include "stdafx.h"
#include "Player.h"
#include "GameResourceManager.h"
#include "MoveManager.h "
#include "./interface/cWindowManager.h"
#include "AppearanceManager.h"
#include "PartyManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"
//#include "KyungGongManager.h"
#include "StreetStallTitleTip.h"
#include "StreetStallManager.h"
#include "GameIn.h"
#include "PeaceWarModeManager.h"
#include "ObjectBalloon.h"
//#include "MonsterGuageDlg.h"

#include "MainGame.h"
#include "WindowIdEnum.h"
#include "MapChange.h"
#include "cMsgBox.h"

#include "ObjectStateManager.h"
#include "PKManager.h"
#include "ChatManager.h"
#include "GuildFieldWar.h"

#include "OptionManager.h"

//#include "UngijosikManager.h"

#include "CharacterDialog.h"
#include "MonsterGuageDlg.h"
//#include "WeatherManager.h"

// 070122 LYW --- Include header file.
#include "cResourceManager.h"

// desc_hseos_���͹���01
// S ���͹��� �߰� added by hseos 2007.05.23
#include "../hseos/Common/SHMath.h"
// E ���͹��� �߰� added by hseos 2007.05.23

// desc_hseos_�󱼱�ü01
// S �󱼱�ü �߰� added by hseos 2007.06.08
#include ".\facialmanager.h"
// E �󱼱�ü �߰� added by hseos 2007.06.08

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.26
#include "../hseos/Family/SHFamilyManager.h"
// E �йи� �߰� added by hseos 2007.07.26

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.18
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.09.18

// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.12.02
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.12.02

//#include "QuestGroup.h"
CPlayer::CPlayer()
{
	m_pFaceShapeObject		= NULL;
//	m_pQuestGroup = NULL;
	m_ItemUseEffect = -1;

	// 070228 LYW --- Player : Setting boolen value to false.
	m_bStartCheck = FALSE ;

	// 070312 LYW --- Player : Setting boolen value to true of eye part.
	m_bActiveEye  = FALSE ;

	m_bClosed	  = FALSE ;

	// 070315 LYW --- Player : Add for test.
#ifdef _DEBUG
	m_bFollow	  = FALSE ;
	m_pFollowTarget = NULL ;
#endif
	
	// 070503 LYW --- Player : Check emotion kind.
	m_bRunningEmotion = FALSE ;

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.17
	m_pcsFamilyEmblem = NULL;
	// E �йи� �߰� added by hseos 2007.07.17

	m_pWingObject = NULL;

	mWeaponEffect[0] = 0;
	mWeaponEffect[1] = 0;
	mWeaponEffect[2] = 0;
}

CPlayer::~CPlayer()
{
//	if( m_pQuestGroup )
//		delete m_pQuestGroup;

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.17
	if (m_pcsFamilyEmblem)
	{
		m_pcsFamilyEmblem->Release();
	}
	SAFE_DELETE(m_pcsFamilyEmblem);
	// E �йи� �߰� added by hseos 2007.07.17
}

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.08.06
void CPlayer::Release()
{
	if (m_pcsFamilyEmblem)
	{
		m_pcsFamilyEmblem->Release();
	}
	SAFE_DELETE(m_pcsFamilyEmblem);

	CObject::Release();
}
// E �йи� �߰� added by hseos 2007.08.06

void CPlayer::InitPlayer(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_CharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));

	memset(&m_MoveInfo,0,sizeof(MOVE_INFO));
	memset(&m_ShopItemOption, 0, sizeof(SHOPITEMOPTION));
	memset(&m_AvatarOption, 0, sizeof(AVATARITEMOPTION));	

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	//////////////////////////////////////////////////////////////////////////

	for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)	
		m_ShopItemOption.Avatar[i] = 1;
	
	SetBaseMotion();

	// 070313 LYW --- GameIn : Initialize eye process of hero.
	//InitializeEye() ;

	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.07
	m_pFaceShapeObject		= NULL;
	ZeroMemory(&m_stFaceAction, sizeof(m_stFaceAction));
	// ..�ε� �� �ʱ�ȭ�� �ð� �����̸� �����ؼ� 3�� ������ ����ش�.
	m_stFaceAction.nFaceShapeTimeTick		= gCurTime + 3000;
	// E �󱼱�ü �߰� added by hseos 2007.06.07

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.17
	m_pcsFamilyEmblem = NULL;
	// E �йи� �߰� added by hseos 2007.07.17

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.19
	m_nFarmMotionTimeTick = NULL;
	m_nFarmMotionTime = NULL;
	// E ����ý��� �߰� added by hseos 2007.09.19

	m_IsDrawHeart = FALSE;

	m_pWingObject = NULL;

	// 071003 LYW --- Player : Setting class index.
	SettingClassIndex() ;
	mWeaponEffect[0] = 0;
	mWeaponEffect[1] = 0;
	mWeaponEffect[2] = 0;

	// 071003 LYW --- Player : Send msg to server.
	//SendCompletedMsg() ;
}

// 071003 LYW --- Player : Add function to send message to server about completed initialize player.
//void CPlayer::SendCompletedMsg()
//{
//	MSGBASE msg;
//	msg.Category = MP_PARTY;
//	msg.Protocol = MP_PARTY_REAL_MEMBER_LOGIN;
//	msg.dwObjectID = GetID();
//	NETWORK->Send(&msg, sizeof(msg));
//}

/*void CPlayer::AddChat(char * chatMsg)
{

	m_pObjectName->GetObjectChatTip()->SetText(chatMsg);
	m_pObjectName->GetObjectChatTip()->SetDisappearTime(5000);
	if(!m_pObjectName->IsActive())
	{
		DropObjectName();
		m_pObjectName->GetObjectChatTip()->SetActive(TRUE);
		WINDOWMGR->AddChatTooltip(m_pObjectName);
	}	
}*/

void CPlayer::GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_CharacterInfo,sizeof(CHARACTER_TOTALINFO));
}


void CPlayer::Process()
{
	// 070503 LYW --- Player : Motice to conduct manager.
	if( GetEngineObject()->IsEndMotion() )
	{
		if( m_bRunningEmotion )
		{
			// desc_hseos_����ý���_01
			// S ����ý��� �߰� added by hseos 2007.09.18
			// APPEARANCEMGR->ShowWeapon( this ) ;
			if (g_csFarmManager.CLI_GetIngCharMotion() == FALSE)
			{
				APPEARANCEMGR->ShowWeapon( this ) ;
			}
			// E ����ý��� �߰� added by hseos 2007.09.18
			m_bRunningEmotion = FALSE ;
		}
	}

	if( GetState() == eObjectState_Die )
	{
		CMonsterGuageDlg* dialog = GAMEIN->GetMonsterGuageDlg();

		if( dialog		&&
			this == ( CObject* )dialog->GetCurMonster() )
		{
			dialog->SetActive( FALSE );
		}

	}

	// 070228 LYW --- Player : Add check facial time part.
	if( m_bStartCheck )
	{
		if( m_FacialTime.Check() == 0 )
		{
			ReturnFace() ;
		}
	}

	// 070312 LYW --- Player : Add eye part.
	/*
	if(m_bActiveEye)
	{
		DWORD dwRemainTime = EYE_LOOFTIME - EYE_CLOSETIME ;
		DWORD dwCheckTime  = m_EyeTime.Check() ;

		if( (dwCheckTime < dwRemainTime) && (dwCheckTime>0) )
		{
			if( !m_bClosed )
			{
				CloseEye() ;
			}
		}
		else if( dwCheckTime == 0 )
		{
			InitializeEye() ;
		}
	}
	*/
	// 070315 LYW --- Player : Add for test.
#ifdef _DEBUG
	if(m_bFollow)
	{
		if(m_pFollowTarget)
		{
			VECTOR3 vPos ;
			m_pFollowTarget->GetPosition(&vPos) ;
			MOVEMGR->SetHeroTarget(&vPos);
		}
	}
#endif //_DEBUG

	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.07
	ProcessFaceAction();
	// E �󱼱�ü �߰� added by hseos 2007.06.07

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.19
	ProcFarmMotion();
	// E ����ý��� �߰� added by hseos 2007.09.19

	CObject::Process();
}

void CPlayer::SetWearedItemIdx(DWORD WearedPosition,DWORD ItemIdx)
{
	ASSERT( sizeof( m_CharacterInfo.WearedItemIdx ) / sizeof( DWORD ) > WearedPosition );

	m_CharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;

	SetBaseMotion();			// ���Ⱑ �ٲ�� �⺻������ �ٲ��
}

WORD CPlayer::GetWeaponEquipType() {
	//YH2DO EquipType�� ���� ���� �ʿ�!!!!
	DWORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
	if(WeaponItemIdx == 0)
		return 0;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);
	ASSERT(pItemInfo);
	if(pItemInfo->WeaponType != 0)
		return pItemInfo->WeaponType;
	else
		return 0;
}

// 080703 LUJ, ����� ǥ���� �� �ִ� �ִϸ��̼� Ÿ���� ��ȯ�Ѵ�
eWeaponAnimationType CPlayer::GetWeaponAniType()
{
	const ITEM_INFO* leftInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Weapon ) );
	const ITEM_INFO* rightInfo	= ITEMMGR->GetItemInfo( GetWearedItemIdx( eWearedItem_Shield ) );

	const eWeaponAnimationType	leftType	= eWeaponAnimationType( leftInfo ? leftInfo->WeaponAnimation : eWeaponAnimationType_None );
	const eWeaponAnimationType	rightType	= eWeaponAnimationType( rightInfo ? rightInfo->WeaponAnimation : eWeaponAnimationType_None );

	// 080703 LUJ, ����� ���Ⱑ �ٸ��� �̵����� �ƴϴ�. �޼տ� ���⸦ �� ����� ��쵵 ���������̴�.
	if( leftType != rightType ||
		leftType == eWeaponAnimationType_None )
	{
		return leftType;
	}

	return eWeaponAnimationType_TwoBlade;
}

void CPlayer::SetBaseMotion()
{
	WORD WeaponEquipType = GetWeaponAniType();

	if( GetObjectBattleState() )
	{
		m_StandardMotion = CHARACTER_MOTION[ eCharacterMotion_Battle ][ WeaponEquipType ];
	}
	else
	{
		m_StandardMotion = CHARACTER_MOTION[ eCharacterMotion_Standard ][ WeaponEquipType ];
	}

	if(m_MoveInfo.MoveMode == eMoveMode_Run)
	{	// �ٱ�	
		m_Move_Start_Motion = CHARACTER_MOTION[ eCharacterMotion_Run ][ WeaponEquipType ];
		m_Move_Ing_Motion = CHARACTER_MOTION[ eCharacterMotion_Run ][ WeaponEquipType ];
		m_Move_End_Motion = -1;


		DWORD itemidx = GetWearedItemIdx(eWearedItem_Wing);
		if( itemidx )
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( itemidx );
			if(pInfo)
			{
				switch( pInfo->Part3DType )
				{
				case ePartType_Wing:
					{
						m_Move_Start_Motion = 199;
						m_Move_Ing_Motion = 199;
					}
					break;
				case ePartType_Wing2:
					{
					}
					break;
				}
			}
		}
	}
	else
	{	// �ȱ�
		m_Move_Start_Motion = CHARACTER_MOTION[ eCharacterMotion_Walk ][ WeaponEquipType ];
		m_Move_Ing_Motion = CHARACTER_MOTION[ eCharacterMotion_Walk ][ WeaponEquipType ];
		m_Move_End_Motion = -1;
		
		DWORD itemidx = GetWearedItemIdx(eWearedItem_Wing);
		if( itemidx )
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( itemidx );
			if(pInfo)
			{
				switch( pInfo->Part3DType )
				{
				case ePartType_Wing:
					{
						m_Move_Start_Motion = 199;
						m_Move_Ing_Motion = 199;
					}
					break;
				case ePartType_Wing2:
					{
					}
					break;
				}
			}
		}
	}
	/*
	if( WeaponEquipType == 0 )
		WeaponEquipType = 2;
	
	if(m_MoveInfo.KyungGongIdx)
	{	// ������̸�
		// ��� ������ ���� ������� 1,2,3�� �����Ѵ�.
		CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
		ASSERT(pKGInfo);
		int MoveType;
		if(pKGInfo != NULL)
			MoveType = pKGInfo->GetMoveType() - 1;		// -1 �� 1���� �����ϱ� ������ 0���� ������ ����
		else
			MoveType = 0;
		
		if(MoveType == 1)	// �ι�° ����� ���۵��� �� by SSAMI
			m_Move_Start_Motion = eMotion_KyungGong1_Move_NoWeapon + MoveType*3;
		else
			m_Move_Start_Motion = eMotion_KyungGong1_Start_NoWeapon + MoveType*3;
		m_Move_Ing_Motion = eMotion_KyungGong1_Move_NoWeapon + MoveType*3;

		// RaMa - 05.09.05  - ������������ ����
		if( GetAvatarOption()->bKyungGong || GetShopItemStats()->bKyungGong )
			m_Move_End_Motion = m_Move_End_Motion;
		else
		{
			#ifdef TAIWAN_LOCAL
				m_Move_End_Motion = m_Move_End_Motion;
			#else
				m_Move_End_Motion = eMotion_KyungGong1_End_NoWeapon;// + MoveType*3;
			#endif
		}
		
		if(m_CharacterInfo.bPeace == TRUE)		// ��ȭ��� �̳�
		{	// ��ȭ���
			if(WeaponEquipType == WP_DO)
				m_StandardMotion = eMotion_Peace_Standard_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_StandardMotion = eMotion_Peace_Standard_Change;
			else
				m_StandardMotion = eMotion_Peace_Standard;
		}
		else
		{
			WORD WeaponEquipType = GetWeaponEquipType();

			//////////////////////////////////////////////////////////////////////////
			// �̺�Ʈ���� Ÿ���� ��� �尩! �� ���� ��� ���
			if(WeaponEquipType == WP_EVENT)
				WeaponEquipType = WP_GWUN;
			//////////////////////////////////////////////////////////////////////////
			
			m_StandardMotion = eMotion_Battle_Gum_Standard + WeaponEquipType -1;			
		}
		return;
	}

	if(m_CharacterInfo.bPeace == TRUE)		// ��ȭ��� �̳�
	{	// ��ȭ���
		if(WeaponEquipType == WP_DO)
			m_StandardMotion = eMotion_Peace_Standard_Do;
		else if(WeaponEquipType == WP_CHANG)
			m_StandardMotion = eMotion_Peace_Standard_Change;
		else
			m_StandardMotion = eMotion_Peace_Standard;

		if(m_MoveInfo.MoveMode == eMoveMode_Walk)
		{	// �ȱ�
			if(WeaponEquipType == WP_DO)
				m_Move_Start_Motion = eMotion_Peace_Walk_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_Move_Start_Motion = eMotion_Peace_Walk_Change;
			else
				m_Move_Start_Motion = eMotion_Peace_Walk;
			m_Move_Ing_Motion = m_Move_Start_Motion;
			m_Move_End_Motion = -1;
		}
		else
		{	// �ٱ�	
			if(WeaponEquipType == WP_DO)
				m_Move_Start_Motion = eMotion_Peace_Run_Do;
			else if(WeaponEquipType == WP_CHANG)
				m_Move_Start_Motion = eMotion_Peace_Run_Change;
			else
				m_Move_Start_Motion = eMotion_Peace_Run;
			m_Move_Ing_Motion = m_Move_Start_Motion;
			m_Move_End_Motion = -1;
		}
	}
	else
	{	// �������
		m_StandardMotion = eMotion_Battle_Gum_Standard + WeaponEquipType -1;
		
		if(m_MoveInfo.MoveMode == eMoveMode_Walk)
		{	// �ȱ�
			m_Move_Start_Motion = eMotion_Battle_Gum_Walk + WeaponEquipType -1;
			m_Move_Ing_Motion = eMotion_Battle_Gum_Walk + WeaponEquipType -1;
			m_Move_End_Motion = -1;
		}
		else
		{	// �ٱ�	
			m_Move_Start_Motion = eMotion_Battle_Gum_Run + WeaponEquipType -1;
			m_Move_Ing_Motion = eMotion_Battle_Gum_Run + WeaponEquipType -1;
			m_Move_End_Motion = -1;
		}
	}
*/
}

BOOL CPlayer::StartSocietyAct( int nStartMotion, int nIngMotion, int nEndMotion, BOOL bHideWeapon )
{
	//��� ���°� �ƴϸ� �ȵȴ�.
	if( GetState() != eObjectState_None && GetState() != eObjectState_Society
		&& GetState() != eObjectState_Immortal ) return FALSE;

	OBJECTSTATEMGR->EndObjectState( this, GetState(), 0 );	//���� ���¸� ������.

	m_ObjectState.State_Start_Motion	= (short int)nStartMotion;
	m_ObjectState.State_Ing_Motion		= (short int)nIngMotion;
	m_ObjectState.State_End_Motion		= (short int)nEndMotion;
	m_ObjectState.MotionSpeedRate = 1.0f;

	if( nEndMotion != -1 )
		m_ObjectState.State_End_MotionTime	= GetEngineObject()->GetAnimationTime(nEndMotion);
	else
		m_ObjectState.State_End_MotionTime = 0;


	if(m_ObjectState.State_Start_Motion != -1)
	{
		ChangeMotion( m_ObjectState.State_Start_Motion, FALSE );

		if( m_ObjectState.State_Ing_Motion != -1 )
			ChangeBaseMotion( m_ObjectState.State_Ing_Motion );
	}

	if(m_ObjectState.State_End_MotionTime)
	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;

	if( bHideWeapon )
		APPEARANCEMGR->HideWeapon( this );

	m_ObjectState.BeforeState		= m_BaseObjectInfo.ObjectState;
	m_BaseObjectInfo.ObjectState	= eObjectState_Society;
	m_ObjectState.State_Start_Time	= gCurTime;
	m_ObjectState.bEndState			= FALSE;
	
	if( m_ObjectState.State_Ing_Motion == -1 )
		OBJECTSTATEMGR->EndObjectState( this, eObjectState_Society, GetEngineObject()->GetAnimationTime( m_ObjectState.State_Start_Motion ) );

	return TRUE;
}


void CPlayer::SetMotionInState(BYTE State)
{
	WORD WeaponEquipType = GetWeaponAniType();

	switch(State)
	{
	case eObjectState_StreetStall_Owner:
		{
			// 070504 LYW --- Player : Modified motion.
			/*
			m_ObjectState.State_Start_Motion = eMotion_StreetStall_Start;
			m_ObjectState.State_Ing_Motion = eMotion_StreetStall_Ing;
			m_ObjectState.State_End_Motion = eMotion_StreetStall_End;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
			*/
			m_ObjectState.State_Start_Motion	= CHARACTER_MOTION[ eCharacterMotion_RestStart ][ WeaponEquipType ];
			m_ObjectState.State_Ing_Motion		= CHARACTER_MOTION[ eCharacterMotion_Rest ][ WeaponEquipType ];
			m_ObjectState.State_End_Motion		= CHARACTER_MOTION[ eCharacterMotion_RestEnd ][ WeaponEquipType ];
			m_ObjectState.State_End_MotionTime	= GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
			m_ObjectState.MotionSpeedRate = 1.0f;

			SetWingObjectMotion( eWingMotion_Idle );
			SetWingObjectBaseMotion( eWingMotion_Idle );
		}
		break;
	case eObjectState_Move:
		{
			m_ObjectState.State_Start_Motion = m_Move_Start_Motion;
			m_ObjectState.State_Ing_Motion = m_Move_Ing_Motion;
			m_ObjectState.State_End_Motion = m_Move_End_Motion;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(m_Move_End_Motion);
			m_ObjectState.MotionSpeedRate = 1.0f;

			if(m_MoveInfo.MoveMode == eMoveMode_Run)
			{
				SetWingObjectMotion( eWingMotion_Run );
				SetWingObjectBaseMotion( eWingMotion_Run );
			}
			else
			{
				SetWingObjectMotion( eWingMotion_Walk );
				SetWingObjectBaseMotion( eWingMotion_Walk );
			}
		}
		break;
	case eObjectState_Immortal:
	case eObjectState_None:
	case eObjectState_Exchange:
	case eObjectState_Deal:
	//case eObjectState_Munpa:
	case eObjectState_PrivateWarehouse:
	case eObjectState_StreetStall_Guest:
		{
			m_ObjectState.State_Start_Motion = m_StandardMotion;
			
			//YH2DO
//			if( GetState() == eObjectState_SkillSyn ||
//				GetState() == eObjectState_SkillUsing )
//				m_ObjectState.State_Start_Motion = -1;

			if( GetState() == eObjectState_Rest )
			{
				m_ObjectState.State_Start_Motion	= CHARACTER_MOTION[ eCharacterMotion_RestEnd ][ WeaponEquipType ];
			}

			m_ObjectState.State_Ing_Motion = m_StandardMotion;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;

			SetWingObjectMotion( eWingMotion_Idle );
			SetWingObjectBaseMotion( eWingMotion_Idle );
		}
		break;
	case eObjectState_Ungijosik:
		{
			m_ObjectState.State_Start_Motion = eMotion_Ungijosik_Start;
			m_ObjectState.State_Ing_Motion = eMotion_Ungijosik_Ing;
			m_ObjectState.State_End_Motion = eMotion_Ungijosik_End;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
			m_ObjectState.MotionSpeedRate = 1.0f;

			SetWingObjectMotion( eWingMotion_Idle );
			SetWingObjectBaseMotion( eWingMotion_Idle );
		}
		break;
		
	case eObjectState_Engrave:
		{
			m_ObjectState.State_Start_Motion = 327;
			m_ObjectState.State_Ing_Motion = 328;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = GetEngineObject()->GetAnimationTime(eMotion_Ungijosik_End);
			m_ObjectState.MotionSpeedRate = 1.0f;

			SetWingObjectMotion( eWingMotion_Idle );
			SetWingObjectBaseMotion( eWingMotion_Idle );
		}
		break;

	case eObjectState_Rest :
		{
			m_ObjectState.State_Start_Motion	= CHARACTER_MOTION[ eCharacterMotion_RestStart ][ WeaponEquipType ];
			if( GetState() == eObjectState_Rest )
			{
				m_ObjectState.State_Start_Motion = CHARACTER_MOTION[ eCharacterMotion_Rest ][ WeaponEquipType ];;
			}
			m_ObjectState.State_Ing_Motion		= CHARACTER_MOTION[ eCharacterMotion_Rest ][ WeaponEquipType ];
			m_ObjectState.State_End_Motion		= -1 ;
			m_ObjectState.State_End_MotionTime	= 0 ;
			m_ObjectState.MotionSpeedRate = 1.0f;

			SetWingObjectMotion( eWingMotion_Idle );
			SetWingObjectBaseMotion( eWingMotion_Idle );
		}
		break ;

	default:
		{
			m_ObjectState.State_Start_Motion = -1;
			m_ObjectState.State_Ing_Motion = -1;
			m_ObjectState.State_End_Motion = -1;
			m_ObjectState.State_End_MotionTime = 0;
			m_ObjectState.MotionSpeedRate = 1.0f;
		}
		break;
	}
}

BYTE CPlayer::GetLifePercent()
{
	return (BYTE)((m_CharacterInfo.Life*100) / GetMaxLife());
}

//////////////////////////////////////////////////////////////////////////
// ObjectStateManager���� State�� ���۰� ������ ȣ�����ִ� �Լ���
void CPlayer::OnStartObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_Move:
		{

		}
		break;
	case eObjectState_Connect:
		{
			CHero* hero = OBJECTMGR->GetHero();

			if( hero && 
				CheckHeart( hero ) )
			{
				//hero->AddMatch( this );
				hero->AddMatch();

				SetHeart( TRUE );
			}
			// desc_hseos_�йи�01
			// S �йи� �߰� added by hseos 2007.11.21
			// ..���� ǥ��
			if (GetFamilyIdx())
			{
				g_csFamilyManager.CLI_ShowEmblem(this, GetFamilyIdx(), GetCharacterTotalInfo()->nFamilyEmblemChangedFreq);
			}
			// E �йи� �߰� added by hseos 2007.11.21
		}
		break;

	case eObjectState_Exit:
		{
			if( m_IsDrawHeart )
			{
				CHero* hero = OBJECTMGR->GetHero();

				//hero->RemoveMatch( this );
				hero->RemoveMatch();
			}
			// desc_hseos_�йи�01
			// S �йи� �߰� added by hseos 2007.07.26
			// ..���� ����
			if (m_pcsFamilyEmblem)
			{
				m_pcsFamilyEmblem->Release();
				SAFE_DELETE(m_pcsFamilyEmblem);
			}
			// E �йи� �߰� added by hseos 2007.07.26
		}
		break;
	case eObjectState_Ungijosik:
		{
			APPEARANCEMGR->HideWeapon(this);
			OBJECTEFFECTDESC desc(eEffect_Ungijosik);
			AddObjectEffect(STATE_EFFECT_ID,&desc,1);
		}
		break;
	case eObjectState_StreetStall_Owner:
		{
			APPEARANCEMGR->HideWeapon(this);
		}
		break;
	case eObjectState_ItemUse:
		{
			if( m_ItemUseEffect == -1 )
				return;

			EFFECTMGR->StartPlayerEffectProcess( HERO, m_ItemUseEffect );
			m_ItemUseEffect = -1;
		}
		break;
	}
}

void CPlayer::OnEndObjectState(BYTE State)
{
	switch(State)
	{
	case eObjectState_StreetStall_Owner:
	case eObjectState_Ungijosik:
	case eObjectState_Society:
		{
			/*
			if(GetWeaponEquipType() == WP_AMGI)
			{
				// 070124 LYW --- PeaceWar : Delete peacewar part.
				//if(PEACEWARMGR->IsPeaceMode(this) == FALSE)
					//APPEARANCEMGR->ShowWeapon(this);
			}
			else if(GetWeaponEquipType() != WP_GWUN)
			{
				APPEARANCEMGR->ShowWeapon(this);
			}
			*/
		}
		break;
	case eObjectState_Move:
		{
			//CHero* hero = OBJECTMGR->GetHero();

			//if(		hero			&& 
			//		m_IsMatch		&&
			//	!	m_IsDrawHeart	)
			//{
			//	OBJECTEFFECTDESC desc(20);
			//	desc.Position.x = 0;
			//	desc.Position.y = 170;
			//	desc.Position.z = 0;

			//	AddObjectEffect( HEART_EFFECT, &desc, 1 );

			//	m_IsDrawHeart = TRUE;

			//	//hero->AddHeart( this );
			//}
		}
		break;
	case eObjectState_Die:
		{
			if(GetID() != HEROID)
				GetEngineObject()->EnablePick();
		}
		break;
	case eObjectState_ItemUse:
		{
			if( m_ItemUseMotion==eMotion_Item_ChangeHair_1 || m_ItemUseMotion==eMotion_Item_ChangeHair_2 || m_ItemUseMotion==eMotion_Item_ChangeHair_3 )
			{
				APPEARANCEMGR->InitAppearance( this );
			}

			//
			if(GetID() != HEROID )
				goto MotionReset;
			if( m_ItemUseMotion==eMotion_Item_Teleport_1 || m_ItemUseMotion==eMotion_Item_Teleport_2 || m_ItemUseMotion==eMotion_Item_Teleport_3 )
			{
				DWORD map = GAMEIN->GetMoveMap();
				// 070122 LYW --- Modified this line.
				//WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
				WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, RESRCMGR->GetMsg( 290 ) );
				MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);				
				MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
			}

MotionReset:
			m_ItemUseMotion = -1;
			m_ItemUseEffect = -1;
		}
		break;
	}
	
	RemoveObjectEffect(STATE_EFFECT_ID);	//????
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// �ൿ �Լ���..
void CPlayer::Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive) 
{	
	if(pAttacker)
	{
		MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);

		if( HERO )
		if( pAttacker == HERO && HERO->IsPKMode() && !this->IsPKMode() )
//		if( GUILDFIELDWAR->GetEnemyGuildIdx() != 0 )
		{
			PKMGR->SetPKContinueTime( PKMGR->GetPKContinueTime() + 10*60*1000 );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1239 ), 10 );	//---KES PK 071124
			//msg �������� ��� ���ӽð��� �߰��� 60�ʸ�ŭ �þ���ϴ�.
		}
	}

	//---KES PK 071124
	if( this == HERO && HERO->IsPKMode() )	//������ PK�ð� ����
	{
		//---������ ���� PK��� ���ӽð� ����
		DWORD dwPKContinueTime = PKMGR->GetPKContinueTime();
		if( dwPKContinueTime == 0 ) return;

	    if( dwPKContinueTime > 10*60*1000 )
			dwPKContinueTime -= 10*60*1000;
		else
			dwPKContinueTime = 0;

		PKMGR->SetPKContinueTime( dwPKContinueTime );
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1240 ), 10 );	//---KES PK 071124
		//-----------------------------------
	}


	WORD weapon = GetWeaponAniType();

	WORD DieMotion = CHARACTER_MOTION[ eCharacterMotion_Die1 ][ weapon ];
	WORD DiedMotion = CHARACTER_MOTION[ eCharacterMotion_Died1 ][ weapon ];


	int DieEffect;
	if(bFatalDamage)
	{
		DieEffect = eEffect_Die2;	// ���󰡼� �ױ�
	
		SetWingObjectBaseMotion( eWingMotion_Died1 );
		SetWingObjectMotion( eWingMotion_Die1 );
	
	}
	else
	{
		DieEffect = eEffect_Die1;	// �׳� �ױ�

		SetWingObjectBaseMotion( eWingMotion_Died2 );
		SetWingObjectMotion( eWingMotion_Die2 );
	}

	TARGETSET set;
	set.pTarget = this;
	set.ResultInfo.bCritical = bCritical;
	set.ResultInfo.bDecisive = bDecisive;
	
	EFFECTMGR->StartEffectProcess(DieEffect,pAttacker,&set,1,GetID(),
							EFFECT_FLAG_DIE|EFFECT_FLAG_GENDERBASISTARGET);


	ChangeMotion( DieMotion, 0 );
	ChangeBaseMotion( DiedMotion );

	CObject::Die(pAttacker,bFatalDamage,bCritical,bDecisive);

	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.12
	// ..���� �� �� ������..
	SetFaceShape(GetCharacterTotalInfo()->FaceType*2+1+72);
	// E �󱼱�ü �߰� added by hseos 2007.06.12
}

void CPlayer::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive)
{
	// desc_hseos_������ȭ����01
	// S ������ȭ���� �ּ�ó�� added by hseos 2007.08.02
	SetObjectBattleState( eObjectBattleState_Battle );
	if (pAttacker)
		pAttacker->SetObjectBattleState( eObjectBattleState_Battle );
	// E ������ȭ���� �ּ�ó�� added by hseos 2007.08.02

	if(Damage != 0)
	{
		if( GetState() == eObjectState_None )	
		{
			if(pAttacker)
				MOVEMGR->SetLookatPos(this,&pAttacker->GetCurPosition(),0,gCurTime);
			
			//SW050826 ������� �ڵ���ȯ
			if( this == HERO )
			{
//				if( PEACEWARMGR->IsPeaceMode(this) )
//				{
//					PEACEWARMGR->ToggleHeroPeace_WarMode();
//				}
//				else if( !PEACEWARMGR->IsPeaceMode(this) )
//				{
//					PEACEWARMGR->SetCheckTime( gCurTime );
//				}
				// 070124 LYW --- PeaceWar : Delete peacewar part.
				/*
				if( m_CharacterInfo.bPeace )
				{
					PEACEWARMGR->ToggleHeroPeace_WarMode();
				}
				else
				{
					PEACEWARMGR->SetCheckTime( gCurTime );
				}
				*/
			}
		}
		
		// 070503 LYW --- Player : Modified rest damage part.
		/*
		if( GetState() == eObjectState_Ungijosik && m_ObjectState.bEndState == FALSE &&
			GetEngineObject()->GetCurMotion() != eMotion_Ungijosik_Start)
		{	// ��������̸� ��������� ������ ����
			//ChangeMotion(1,FALSE);
			ChangeMotion(eMotion_Ungijosik_Damage,FALSE);
			*/

		if( GetState() == eObjectState_Rest && m_ObjectState.bEndState == FALSE &&
			GetEngineObject()->GetCurMotion() != eObjectState_Rest)
		{	// ��������̸� ��������� ������ ����
			//ChangeMotion(1,FALSE);
			//ChangeMotion(eObjectState_RestDamage,FALSE);
			OBJECTSTATEMGR->StartObjectState(this, eObjectState_None);

			
			MSG_BYTE msg ;

			msg.Category	= MP_EMOTION ;
			msg.Protocol	= MP_EMOTION_SITSTAND_SYN ;
			msg.dwObjectID	= HERO->GetID() ;
			msg.bData		= eObjectState_None ;

			NETWORK->Send(&msg, sizeof(MSG_BYTE));
			
		}
		else if(GetState() == eObjectState_None)
		{	// �Ϲ� ������ ����	
			//ChangeMotion(1,FALSE);

			WORD weapon = GetWeaponAniType();

			switch(DamageKind) {
			case eDamageKind_Front:
				ChangeMotion( CHARACTER_MOTION[ eCharacterMotion_FrontDamage ][ weapon ], FALSE );
				break;
			// 080708 LYW --- Player : ���� Ÿ���� �߰��Ǹ鼭 �Ʒ� �� �ִϸ��̼� ����� ���� �Ǿ���.
			//case eDamageKind_Left:
			//	ChangeMotion( CHARACTER_MOTION[ eCharacterMotion_LeftDamage ][ weapon ], FALSE );
			//	break;
			//case eDamageKind_Right:
			//	ChangeMotion( CHARACTER_MOTION[ eCharacterMotion_RightDamage ][ weapon ], FALSE );
			//	break;
			}

			SetWingObjectMotion( eWingMotion_Hit );
		}
		
		int DamageEffect = -1;
	
		if( bCritical )
		{
			DamageEffect = eEffect_CriticalDamage;
		}
		else
		{
			DamageEffect = eEffect_NormalDamage;
		}

		if(DamageEffect != -1)
		{
			TARGETSET set;
			set.pTarget = this;
			set.ResultInfo.bCritical = bCritical;
			set.ResultInfo.bDecisive = bDecisive;
			
			EFFECTMGR->StartEffectProcess(DamageEffect,pAttacker,&set,1,GetID(),
											EFFECT_FLAG_GENDERBASISTARGET);
		}
	}
	
	if( pAttacker == HERO || pAttacker == HEROPET ||this == HERO )	//�ڽ��� ������ ��ȣ�� ǥ�� //KES 040801
	{
		EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
	}
	else if( pAttacker )
	{
		if( OPTIONMGR->IsShowMemberDamage() )
		if( PARTYMGR->IsPartyMember( pAttacker->GetID() ) )
		{
			EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}
	}	

	// Life ����
	DWORD life = GetLife();
	if(life < Damage)
		life = 0;
	else
		life = life - Damage;
	SetLife(life);

	// 080616 LUJ, Ŭ���̾�Ʈ�� HP �ս��� ���� ó���ϱ� ������ ������ ������ �߻��� �� �ִ�
	//				5�� ��ų�� ���� �������� HP/MP�� ��ȯ�ϴ� ����� �߰��Ǿ��� �����̴�.
	//				���� HP �ս��� �߻��ϸ� ������ HP ������ ��û�ؼ� ������Ʈ�ϵ��� �Ѵ�
	if( Damage )
	{
		MSGBASE message;
		ZeroMemory( &message, sizeof( message ) );
		message.Category	= MP_CHAR;
		message.Protocol	= MP_CHAR_LIFE_GET_SYN;
		message.dwObjectID	= GetID();

		NETWORK->Send( &message, sizeof( message ) );
	}
}
void CPlayer::SetLife(DWORD life, BYTE type)
{
	m_CharacterInfo.Life = life; 

	
	if(GetState() == eObjectState_Die)
		life = 0;

	if( GAMEIN->GetMonsterGuageDlg())
	{
		if( GAMEIN->GetMonsterGuageDlg()->GetCurMonster() == (CObject*)this )
		{
			GAMEIN->GetMonsterGuageDlg()->SetMonsterLife( life, GetMaxLife() );
		}
	}
}

void CPlayer::Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal)
{

	// desc_hseos_������ȭ����01
	// S ������ȭ���� �ּ�ó�� added by hseos 2007.08.02
	if( GetObjectBattleState() == eObjectBattleState_Battle )
	{
		if( pHealer )	
			pHealer->SetObjectBattleState( eObjectBattleState_Battle );
	}
	// E ������ȭ���� �ּ�ó�� added by hseos 2007.08.02

	DWORD life = GetLife() + HealVal;
	SetLife(life);
}

void CPlayer::Recharge(CObject* pHealer,BYTE RechargeKind,DWORD RechargeVal)
{
	// desc_hseos_������ȭ����01
	// S ������ȭ���� �ּ�ó�� added by hseos 2007.08.02
	if( GetObjectBattleState() == eObjectBattleState_Battle )
	{
		if( pHealer )
			pHealer->SetObjectBattleState( eObjectBattleState_Battle );
	}
	// E ������ȭ���� �ּ�ó�� added by hseos 2007.08.02

	DWORD mana = GetMana() + RechargeVal;
	SetMana(mana);
}

//---KES �����˻� 2008.3.11
void CPlayer::ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pObjectBalloon )									return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Title ) )	return;

	if( STREETSTALLMGR->IsSearchWordIn( strTitle ) )
		m_pObjectBalloon->ShowStreetStallTitle( bShow, strTitle, RGB_HALF(255,255,0), RGB_HALF(100,100,255) );
	else
		m_pObjectBalloon->ShowStreetStallTitle( bShow, strTitle, RGB_HALF(10,10,10), RGB_HALF(255,255,255) );

}

void CPlayer::SetStreetStallTitle( char* strTitle )
{
	if( m_pObjectBalloon )
	if( m_pObjectBalloon->GetSSTitleTip() )
	{
		if( STREETSTALLMGR->IsSearchWordIn( strTitle ) )
		{
			m_pObjectBalloon->GetSSTitleTip()->SetFGColor( RGB_HALF(255,255,0) );
			m_pObjectBalloon->GetSSTitleTip()->SetBGColor( RGB_HALF(100,100,255) );
		}
		else
		{
			m_pObjectBalloon->GetSSTitleTip()->SetFGColor( RGB_HALF(10,10,10) );
			m_pObjectBalloon->GetSSTitleTip()->SetBGColor( RGB_HALF(255,255,255) );		
		}

		m_pObjectBalloon->GetSSTitleTip()->SetTitle( strTitle );		
	}
	
}

void CPlayer::ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pObjectBalloon )									return;
	if( !(m_bObjectBalloonBits & ObjectBalloon_Title ) )	return;
	
	if( STREETSTALLMGR->IsSearchWordIn( strTitle ) )
		m_pObjectBalloon->ShowStreetBuyStallTitle( bShow, strTitle, RGB_HALF(255,255,0), RGB_HALF(255,100,100) );
	else
		m_pObjectBalloon->ShowStreetBuyStallTitle( bShow, strTitle, RGB_HALF(10,10,10), RGB_HALF(255,255,255) );
}

void CPlayer::SetStreetBuyStallTitle( char* strTitle )
{
	if( m_pObjectBalloon )
	if( m_pObjectBalloon->GetSBSTitleTip() )
	{
		if( STREETSTALLMGR->IsSearchWordIn( strTitle ) )
		{
			m_pObjectBalloon->GetSBSTitleTip()->SetFGColor( RGB_HALF(255,255,0) );
			m_pObjectBalloon->GetSBSTitleTip()->SetBGColor( RGB_HALF(255,100,100) );
		}
		else
		{
			m_pObjectBalloon->GetSBSTitleTip()->SetFGColor( RGB_HALF(10,10,10) );
			m_pObjectBalloon->GetSBSTitleTip()->SetBGColor( RGB_HALF(255,255,255) );		
		}

		m_pObjectBalloon->GetSBSTitleTip()->SetTitle( strTitle );
	}		
		
}

void CPlayer::RefreshStreetStallTitle()
{
	if( !m_pObjectBalloon )									return;

	CStreetStallTitleTip* pSS = m_pObjectBalloon->GetSSTitleTip();
	if( pSS )
	if( pSS->IsShow() )
	{
		if( STREETSTALLMGR->IsSearchWordIn( pSS->GetTitle() ) )
		{
			pSS->SetFGColor( RGB_HALF(255,255,0) );
			pSS->SetBGColor( RGB_HALF(100,100,255) );
		}
		else
		{
			pSS->SetFGColor( RGB_HALF(10,10,10) );
			pSS->SetBGColor( RGB_HALF(255,255,255) );
		}
	}

	CStreetStallTitleTip* pSB = m_pObjectBalloon->GetSBSTitleTip();
	if( pSB )
	if( pSB->IsShow() )
	{
		if( STREETSTALLMGR->IsSearchWordIn( pSB->GetTitle() ) )
		{
			pSB->SetFGColor( RGB_HALF(255,255,0) );
			pSB->SetBGColor( RGB_HALF(255,100,100) );
		}
		else
		{
			pSB->SetFGColor( RGB_HALF(10,10,10) );
			pSB->SetBGColor( RGB_HALF(255,255,255) );
		}		
	}
}

//-----------------------

float CPlayer::DoGetMoveSpeed()
{
	CHero*	hero	= HERO;
	float	Speed	= 0;

	if(m_MoveInfo.KyungGongIdx)
	{
		if( this == hero )
		{
			Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx);
		}
		else
		{
			Speed = MOVEMGR->GetKyungGongSpeed(m_MoveInfo.KyungGongIdx);
		}
		
		// �ƹ�Ÿ ������ ��� ���ǵ� 
		Speed += GetAvatarOption()->KyunggongSpeed;
		// ���� ��� �ӵ� ���
		Speed += GetShopItemStats()->KyungGongSpeed;
	}
	else
	{
		if(m_MoveInfo.MoveMode == eMoveMode_Run)
			Speed = RUNSPEED;
		else
			Speed = WALKSPEED;
	}

	if( this == hero )
	{		
		// 080630 LUJ, ��Ʈ ���������� ȹ��� �̵� �ӵ� ���ʽ��� ����ǵ��� ��
		float addrateval	= ( hero->GetRateBuffStatus()->MoveSpeed + hero->GetRatePassiveStatus()->MoveSpeed ) / 100.f + hero->GetItemStats().mMoveSpeed.mPercent + hero->GetSetItemStats().mMoveSpeed.mPercent;
		float addval		= hero->GetBuffStatus()->MoveSpeed + hero->GetPassiveStatus()->MoveSpeed + hero->GetItemStats().mMoveSpeed.mPlus + hero->GetSetItemStats().mMoveSpeed.mPlus;

		m_MoveInfo.AddedMoveSpeed = Speed * addrateval + addval;
	}

	Speed += m_MoveInfo.AddedMoveSpeed;

	return max( 1, Speed );
}

void CPlayer::SetPKMode( BOOL bPKMode )
{
	m_CharacterInfo.bPKMode = bPKMode ? true : false;	
}

DWORD CPlayer::DoGetMaxLife()
{
	return m_CharacterInfo.MaxLife;
}

void CPlayer::SetBadFame(FAMETYPE val)
{
	m_CharacterInfo.BadFame = val;
}

void CPlayer::SetShopItemOptionInfo(SHOPITEMOPTION* pInfo)
{
	if(!pInfo)			return;
	memcpy(&m_ShopItemOption, pInfo, sizeof(SHOPITEMOPTION));
}

void CPlayer::SetGuildIdxRank(DWORD GuildIdx, BYTE Rank)
{ 
	m_CharacterInfo.MunpaID = GuildIdx;
	m_CharacterInfo.PositionInMunpa = Rank;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_CharacterInfo.MarkName = MarkName;
}

void CPlayer::SetNickName(char* pNickName)
{
	SafeStrCpy(m_CharacterInfo.NickName, pNickName, MAX_GUILD_NICKNAME+1);
}

MARKNAMETYPE CPlayer::GetGuildMarkName()
{
	return m_CharacterInfo.MarkName;
}

void CPlayer::SetGuildIdx(DWORD GuildIdx)
{
	m_CharacterInfo.MunpaID = GuildIdx;
}

void CPlayer::ClearGuildInfo()
{
	m_CharacterInfo.MunpaID = 0;
	m_CharacterInfo.PositionInMunpa = GUILD_NOTMEMBER;
	m_CharacterInfo.MarkName = 0;
	SafeStrCpy(m_CharacterInfo.NickName, "", MAX_GUILD_NICKNAME+1);
	SafeStrCpy(m_CharacterInfo.GuildName, "", MAX_GUILD_NAME+1);
}

char* CPlayer::GetNickName()
{
	return m_CharacterInfo.NickName;
}

void CPlayer::SetGuildName(char* GuildName)
{
	SafeStrCpy(m_CharacterInfo.GuildName, GuildName, MAX_GUILD_NAME+1);
}

char* CPlayer::GetGuildName()
{
	return m_CharacterInfo.GuildName;
}

// 070415 LYW --- PLAYER : Active functions to setting and return stage.
//void CPlayer::SetStage( BYTE Stage )
void CPlayer::SetStage( BYTE grade, BYTE index )
{
	//m_CharacterInfo.Stage = Stage;
	//m_CharacterInfo.JobGrade = Stage ;
	m_CharacterInfo.JobGrade = grade ;
	m_CharacterInfo.Job[grade - 1] = index ;
}

void CPlayer::SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo )
{
	m_CharacterInfo.Gender = pInfo->Gender;
	m_CharacterInfo.HairType = pInfo->HairType;
	m_CharacterInfo.FaceType = pInfo->FaceType;
	m_CharacterInfo.Height = pInfo->Height;
	m_CharacterInfo.Width = pInfo->Width;

	APPEARANCEMGR->AddCharacterPartChange( GetID() );
}

//---KES CHEAT CHANGESIZE
void CPlayer::SetCharacterSize( float fSize )
{
	m_CharacterInfo.Width = m_CharacterInfo.Height = fSize ;

	APPEARANCEMGR->AddCharacterPartChange( GetID() );
}
//-----------------------

void CPlayer::SetSiegeName( DWORD NameType )
{
	if( m_pObjectBalloon )
		m_pObjectBalloon->SetSiegeName( NameType, strlen(GetObjectName()) );
}

void CPlayer::CalcAvatarOption()
{
	memset( &m_AvatarOption, 0, sizeof(AVATARITEMOPTION) );

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( i >= eAvatar_Weared_Hair )		break;
		if( m_ShopItemOption.Avatar[i]==0 )					continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( m_ShopItemOption.Avatar[i] );
		if( !pItemInfo )		continue;
		
		// �ٰ�
/*		if( pItemInfo->Str > 0 )
			m_AvatarOption.Str += pItemInfo->Str;
		// ��ø
		if( pItemInfo->Dex > 0 )
			m_AvatarOption.Dex += pItemInfo->Dex;
		// ü��
		if( pItemInfo->Vit > 0 )
			m_AvatarOption.Vit += pItemInfo->Vit;
		// �ɸ�
		if( pItemInfo->Wis > 0 )
			m_AvatarOption.Wis += pItemInfo->Wis;
		// �ִ�����
		if( pItemInfo->Life > 0 )
			m_AvatarOption.Life += (WORD)pItemInfo->Life;
		// �ִ볻��
		if( pItemInfo->Mana > 0 )
			m_AvatarOption.Mana += pItemInfo->Mana;
		// ������ݷ�
		if( pItemInfo->MeleeAttackMin > 0 )
			m_AvatarOption.Attack += pItemInfo->MeleeAttackMin;
		// ũ��Ƽ�ü�ġ
		if( pItemInfo->CriticalPercent > 0 )
			m_AvatarOption.Critical += pItemInfo->CriticalPercent;
		// �ݰ�Ȯ��
		if( pItemInfo->Plus_SkillIdx > 0 )
			m_AvatarOption.CounterPercent += pItemInfo->Plus_SkillIdx;
		// �ݰݽ� ��������
		if( pItemInfo->Plus_Value > 0 )
			m_AvatarOption.CounterDamage += pItemInfo->Plus_Value;
		// ���������
		if( pItemInfo->AllPlus_Kind == 1 )
			m_AvatarOption.bKyungGong = 1;
		// ���¼Ҹ𰨼�
		if( pItemInfo->LimitVit > 0 )
			m_AvatarOption.ManaSpend += pItemInfo->LimitVit;
		// ���� ������
		if( pItemInfo->LimitJob > 0 )
			m_AvatarOption.NeagongDamage += pItemInfo->LimitJob;
		// �ܰ� ������
		if( pItemInfo->LimitGender > 0 )
			m_AvatarOption.WoigongDamage += pItemInfo->LimitGender;		
		// ���� �������� ����
		if( pItemInfo->LimitLevel > 0 )
			m_AvatarOption.TargetPhyDefDown += pItemInfo->LimitLevel;
		// ���� �Ӽ����׷� ����
		if( pItemInfo->LimitStr > 0 )
			m_AvatarOption.TargetAttrDefDown += pItemInfo->LimitStr;
		// ���� ���ݷ� ����
		if( pItemInfo->LimitDex > 0 )
			m_AvatarOption.TargetAtkDown += pItemInfo->LimitDex;
		// ȸ���� �߰� ����ġ
		if( pItemInfo->LimitWis )
			m_AvatarOption.RecoverRate += pItemInfo->LimitWis;
		// ����ӵ� ���
		if( pItemInfo->ItemGrade )
			m_AvatarOption.KyunggongSpeed += pItemInfo->ItemGrade;
		// ��ȥ������ �����ӵ�
		if( pItemInfo->RangeType )
			m_AvatarOption.MussangCharge += pItemInfo->RangeType;
		// ������� ���¼Ҹ𿩺�
		if( pItemInfo->EquipKind == 1 )
			m_AvatarOption.NaeruykspendbyKG = 1;
		//////////////////////////////////////////////////////
		// 06. 07 ���� ����(�ϰ�) - �̿���
		if( pItemInfo->ManaRecover )
			m_AvatarOption.Decisive += pItemInfo->ManaRecover;
		//////////////////////////////////////////////////////
		
*/	}

	// Character Dialog
	if( GetID() == HEROID )
		GAMEIN->GetCharacterDialog()->RefreshInfo();
}

//////////////////////////////////////////////////////////////////////////
// 06. 06. 2�� ���� - �̿���
// ���� ��ȯ �߰�
void CPlayer::AddPassiveStatsOption(SKILLOPTION *pSkillOption)
{
	m_PassiveStatsOption.Life += pSkillOption->Life;
	m_PassiveStatsOption.Mana += pSkillOption->Mana;
	m_PassiveStatsOption.PhyDef += pSkillOption->PhyDef;
	m_PassiveStatsOption.AttDef += pSkillOption->AttDef;

	m_PassiveStatsOption.Range += pSkillOption->Range;
	m_PassiveStatsOption.ReduceMana += pSkillOption->ReduceMana;
	m_PassiveStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_PassiveStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_PassiveStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemovePassiveStatsOption(SKILLOPTION *pSkillOption)
{
	m_PassiveStatsOption.Life -= pSkillOption->Life;
	m_PassiveStatsOption.Mana -= pSkillOption->Mana;
	m_PassiveStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_PassiveStatsOption.AttDef -= pSkillOption->AttDef;
	  
	m_PassiveStatsOption.Range -= pSkillOption->Range;
	m_PassiveStatsOption.ReduceMana -= pSkillOption->ReduceMana;
	m_PassiveStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_PassiveStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_PassiveStatsOption.AttAtk -= pSkillOption->AttAtk;
}

void CPlayer::AddActiveStatsOption(SKILLOPTION *pSkillOption)
{
	m_ActiveStatsOption.Life += pSkillOption->Life;
	m_ActiveStatsOption.Mana += pSkillOption->Mana;
	m_ActiveStatsOption.PhyDef += pSkillOption->PhyDef;
	m_ActiveStatsOption.AttDef += pSkillOption->AttDef;
	  
	m_ActiveStatsOption.Range += pSkillOption->Range;
	m_ActiveStatsOption.ReduceMana += pSkillOption->ReduceMana;
	m_ActiveStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_ActiveStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_ActiveStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveActiveStatsOption(SKILLOPTION *pSkillOption)
{
	m_ActiveStatsOption.Life -= pSkillOption->Life;
	m_ActiveStatsOption.Mana -= pSkillOption->Mana;
	m_ActiveStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_ActiveStatsOption.AttDef -= pSkillOption->AttDef;
	  
	m_ActiveStatsOption.Range -= pSkillOption->Range;
	m_ActiveStatsOption.ReduceMana -= pSkillOption->ReduceMana;
	m_ActiveStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_ActiveStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_ActiveStatsOption.AttAtk -= pSkillOption->AttAtk;
}

void CPlayer::AddOnOffStatsOption(SKILLOPTION *pSkillOption)
{
	m_OnOffStatsOption.Life += pSkillOption->Life;
	m_OnOffStatsOption.Mana += pSkillOption->Mana;
	m_OnOffStatsOption.PhyDef += pSkillOption->PhyDef;
	m_OnOffStatsOption.AttDef += pSkillOption->AttDef;
	  
	m_OnOffStatsOption.Range += pSkillOption->Range;
	m_OnOffStatsOption.ReduceMana += pSkillOption->ReduceMana;
	m_OnOffStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_OnOffStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_OnOffStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveOnOffStatsOption(SKILLOPTION *pSkillOption)
{
	m_OnOffStatsOption.Life -= pSkillOption->Life;
	m_OnOffStatsOption.Mana -= pSkillOption->Mana;
	m_OnOffStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_OnOffStatsOption.AttDef -= pSkillOption->AttDef;
	  
	m_OnOffStatsOption.Range -= pSkillOption->Range;
	m_OnOffStatsOption.ReduceMana -= pSkillOption->ReduceMana;
	m_OnOffStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_OnOffStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_OnOffStatsOption.AttAtk -= pSkillOption->AttAtk;
}

void CPlayer::AddBuffStatsOption(SKILLOPTION *pSkillOption)
{
	m_BuffStatsOption.Life += pSkillOption->Life;
	m_BuffStatsOption.Mana += pSkillOption->Mana;
	m_BuffStatsOption.PhyDef += pSkillOption->PhyDef;
	m_BuffStatsOption.AttDef += pSkillOption->AttDef;
	  
	m_BuffStatsOption.Range += pSkillOption->Range;
	m_BuffStatsOption.ReduceMana += pSkillOption->ReduceMana;
	m_BuffStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_BuffStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_BuffStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveBuffStatsOption(SKILLOPTION *pSkillOption)
{
	m_BuffStatsOption.Life -= pSkillOption->Life;
	m_BuffStatsOption.Mana -= pSkillOption->Mana;
	m_BuffStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_BuffStatsOption.AttDef -= pSkillOption->AttDef;
	  
	m_BuffStatsOption.Range -= pSkillOption->Range;
	m_BuffStatsOption.ReduceMana -= pSkillOption->ReduceMana;
	m_BuffStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_BuffStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_BuffStatsOption.AttAtk -= pSkillOption->AttAtk;
}

// 070228 LYW --- Player : Add function for facial part.
void CPlayer::ChangeFace( BYTE faceNum )
{
	MOD_LIST* pFaceList = GetModList() ;
	BYTE face	= m_CharacterInfo.FaceType;

	m_EngineObject.SetMaterialIndex( faceNum ) ;

	m_FacialTime.Init( FACIAL_TIME ) ;
	m_FacialTime.Start() ;

	m_bStartCheck = TRUE ;

	// 070312 LYW --- Player : Stop close eye.
	m_bActiveEye = FALSE ;
}

void CPlayer::ReturnFace()
{
	MOD_LIST* pFaceList = GetModList() ;
	BYTE face	= m_CharacterInfo.FaceType;

	m_EngineObject.SetMaterialIndex( 0x01 ) ;

	m_bStartCheck = FALSE ;

	// 070312 LYW --- Player : Start close eye.
	InitializeEye() ;
}

// 070528 LYW --- Player : Add function to syn to server play emoticon.
void CPlayer::PlayEmoticon_Syn(BYTE EmoticonNum)
{
	MSG_INT data ;

	data.Category	= MP_EMOTION ;
	data.Protocol	= MP_EMOTICON_SYN ;
	data.dwObjectID	= HEROID ;
	data.nData		= EmoticonNum ;

	NETWORK->Send( &data, sizeof(MSG_INT) ) ;
}

void CPlayer::PlayEmoticon(int nEmoticonNum)
{
	EFFECTMGR->StartPlayerEffectProcess(this, nEmoticonNum) ;
}

// 070312 LYW --- Player : Add function for eye part.
void CPlayer::CloseEye() 
{
	MOD_LIST* pFaceList = GetModList() ;
	BYTE face	= m_CharacterInfo.FaceType;

	m_EngineObject.SetMaterialIndex( 0x02 ) ;

	m_bClosed = TRUE ;
}

void CPlayer::InitializeEye()
{
	MOD_LIST* pFaceList = GetModList() ;
	BYTE face	= m_CharacterInfo.FaceType;

	m_EngineObject.SetMaterialIndex( 0x01 ) ;

	m_EyeTime.End() ;
	m_EyeTime.Init(EYE_LOOFTIME) ;
	m_EyeTime.Start() ;

	m_bActiveEye = TRUE ;
	m_bClosed	 = FALSE ;
}

// 070312 LYW --- Player : Add function to return MOD_LIST.
MOD_LIST* CPlayer::GetModList() 
{
	BYTE gender = m_CharacterInfo.Gender;
	BYTE race	= m_CharacterInfo.Race;

	return &GAMERESRCMNGR->m_FaceModList[race][gender];
}
//////////////////////////////////////////////////////////////////////////


void CPlayer::SetFamilyIdxRank(DWORD FamilyIdx, BYTE Rank)
{ 
	m_CharacterInfo.FamilyID = FamilyIdx;
	m_CharacterInfo.PositionInFamily = Rank;
}

void CPlayer::SetFamilyMarkName(MARKNAMETYPE MarkName)
{
	m_CharacterInfo.FamilyMarkName = MarkName;
}

void CPlayer::SetFamilyNickName(char* pNickName)
{
	SafeStrCpy(m_CharacterInfo.FamilyNickName, pNickName, MAX_GUILD_NICKNAME+1);
}

MARKNAMETYPE CPlayer::GetFamilyMarkName()
{
	return m_CharacterInfo.MarkName;
}

void CPlayer::SetFamilyIdx(DWORD FamilyIdx)
{
	m_CharacterInfo.FamilyID = FamilyIdx;
}

void CPlayer::ClearFamilyInfo()
{
	m_CharacterInfo.FamilyID = 0;
	m_CharacterInfo.PositionInFamily = FAMILY_NOTMEMBER;
	m_CharacterInfo.MarkName = 0;
	m_CharacterInfo.FamilyNickName[ 0 ]	= 0;
	m_CharacterInfo.FamilyName[ 0 ]		= 0;
}

char* CPlayer::GetFamilyNickName()
{
	return m_CharacterInfo.FamilyNickName;
}

void CPlayer::SetFamilyName(char* FamilyName)
{
	SafeStrCpy( m_CharacterInfo.FamilyName, FamilyName, sizeof( m_CharacterInfo.FamilyName ) );
}

char* CPlayer::GetFamilyName()
{
	return m_CharacterInfo.FamilyName;
}

// desc_hseos_�󱼱�ü01
// S �󱼱�ü �߰� added by hseos 2007.06.07
void CPlayer::SetFaceAction(int nAction, int nShape)
{ 
	if (nAction == FACE_ACTION_KIND_EYE_BLINK)
	{
		m_stFaceAction.nFaceShapeState = PLAYER_FACE_SHAPE_EYE_OPEN;
	}

	m_stFaceAction.nFaceActionKind = nAction; 
	m_stFaceAction.nFaceShapeTimeTick = gCurTime;
	SetFaceShape(nShape);
}

void CPlayer::ProcessFaceAction()
{
	switch(m_stFaceAction.nFaceActionKind)
	{
	// �� ������
	case FACE_ACTION_KIND_EYE_BLINK:
		{
 			if (this->IsDied()) break;

			int nRndTime = 0;
			switch(m_stFaceAction.nFaceShapeState)
			{
				// �� �� ����
				case PLAYER_FACE_SHAPE_EYE_OPEN:
					// ���� �ڿ��������� ���� +-������ ����
					nRndTime = CSHMath::GetRandomNum(0, CFacialManager::PLAYER_FACE_EYE_BLINK_OPEN_ADD_RND_TIME);
					if (CSHMath::GetRandomNum(0,1)) nRndTime *= -1;
					// ǥ�� �ð� üũ
					if (gCurTime - m_stFaceAction.nFaceShapeTimeTick > CFacialManager::PLAYER_FACE_EYE_BLINK_OPEN_TIME+nRndTime)
					{
						m_stFaceAction.nFaceShapeState = PLAYER_FACE_SHAPE_EYE_CLOSE;
						m_stFaceAction.nFaceShapeTimeTick = gCurTime;
						SetFaceShape(GetCharacterTotalInfo()->FaceType*2+1+72);
					}
					break;
				// �� ���� ����
				case PLAYER_FACE_SHAPE_EYE_CLOSE: 
					// ���� �ڿ��������� ���� +-������ ����
					nRndTime = CSHMath::GetRandomNum(0, CFacialManager::PLAYER_FACE_EYE_BLINK_CLOSE_ADD_RND_TIME);
					if (CSHMath::GetRandomNum(0,1)) nRndTime *= -1;
					// ǥ�� �ð� üũ
 					if (gCurTime - m_stFaceAction.nFaceShapeTimeTick > CFacialManager::PLAYER_FACE_EYE_BLINK_CLOSE_TIME+nRndTime)
					{
						m_stFaceAction.nFaceShapeState = PLAYER_FACE_SHAPE_EYE_OPEN;
						m_stFaceAction.nFaceShapeTimeTick = gCurTime;
						SetFaceShape(GetCharacterTotalInfo()->FaceType*2+72);
					}
					break;
			}
		}
		break;
	// ä��â ��ɾ�
	case FACE_ACTION_KIND_EYE_CHATTING_CMD:
		if (gCurTime - m_stFaceAction.nFaceShapeTimeTick > CFacialManager::PLAYER_FACE_SHAPE_TIME)
		{
			SetFaceAction(FACE_ACTION_KIND_EYE_BLINK, GetCharacterTotalInfo()->FaceType*2+72);
		}
		break;
	}
}
// E �󱼱�ü �߰� added by hseos 2007.06.07

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.09.19
VOID CPlayer::StartFarmMotion(int nMotionNum, DWORD nMotionTime)
{
	APPEARANCEMGR->HideWeapon(this, this->GetEngineObject());
	HEFFPROC nEff = EFFECTMGR->StartEffectProcess(nMotionNum, this, NULL, 0, 0);
	if (HERO->GetID() == GetID()) g_csFarmManager.CLI_SetIngCharMotion(nEff);

	m_nFarmMotionTimeTick = gCurTime;
	m_nFarmMotionTime = nMotionTime;
	// ĳ���� �̸� �Ʒ� ���α׷��� ����
	GetObjectBalloon()->GetProgressBar()->CLI_Start(CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_TYPE, CSHFarmManager::FARM_CHAR_MOTION_PROGRESSBAR_WIDTH, nMotionTime);
}

VOID CPlayer::ProcFarmMotion()
{
	if (m_nFarmMotionTime &&
  		gCurTime - m_nFarmMotionTimeTick > m_nFarmMotionTime)
	{
		APPEARANCEMGR->ShowWeapon( this, this->GetEngineObject() ) ;
		if (HERO->GetID() == GetID()) g_csFarmManager.CLI_SetIngCharMotion(NULL);

		m_nFarmMotionTimeTick = NULL;
		m_nFarmMotionTime = NULL;
	}
}
// E ����ý��� �߰� added by hseos 2007.09.19

BOOL CPlayer::CheckHeart( CPlayer* opponent )
{
	const DATE_MATCHING_INFO&	myData			= GetDateMatchingInfo();
	const DATE_MATCHING_INFO&	opponentData	= opponent->GetDateMatchingInfo();
	const CHARACTER_TOTALINFO*	opponentInfo	= opponent->GetCharacterTotalInfo();

	if(		opponent == this						||
		!	opponentInfo->bVisible					||
		!	myData.bIsValid							||
		!	opponentData.bIsValid					||
			myData.byGender	== opponentData.byGender	)
    {
        return FALSE;
    }

    int score = 100;

    // ���� üũ
    {
        const int ageGap = abs( ( int )myData.dwAge - ( int )opponentData.dwAge );

        score -= ageGap;
    }

    // ���� ���� üũ
    {
        const int areaGap = ( myData.dwRegion == opponentData.dwRegion ) ? 0 : 10;

        score -= areaGap;
    }

    // ���� ��: �ϳ��� ����Ʈ, �ϳ��� �¿� �ְ� ����Ʈ�� �ݺ��ڷ� ������. �¿� ���� ������ �����Ѵ�.
    // ��ȣ ���� ��
    {
        const size_t size = sizeof( myData.pdwGoodFeeling ) / sizeof( DWORD );

        std::set< DWORD > opponentFavorite;
        opponentFavorite.insert( opponentData.pdwGoodFeeling, opponentData.pdwGoodFeeling + size );

        for( size_t i = 0; i < size; ++i )
        {
			bool		isFound		= false;				
			const DWORD myFavorite	= myData.pdwGoodFeeling[ i ];

			for( size_t j = 0; j < size; ++j )
			{
				const DWORD opponentFavorite = opponentData.pdwGoodFeeling[ j ];

				if( myFavorite == opponentFavorite )
				{
					isFound = true;
					break;
				}
			}

			if( ! isFound )
			{
				const int favoriteGap = 10;

				score -= favoriteGap;
			}
        }
    }

    // ���� ���� ��
    {
        const size_t size = sizeof( myData.pdwBadFeeling ) / sizeof( DWORD );

        std::set< DWORD > opponentFavorite;
        opponentFavorite.insert( opponentData.pdwBadFeeling, opponentData.pdwBadFeeling + size );

        for( size_t i = 0; i < size; ++i )
        {
			bool		isFound		= false;				
			const DWORD myFavorite	= myData.pdwBadFeeling[ i ];

			for( size_t j = 0; j < size; ++j )
			{
				const DWORD opponentFavorite = opponentData.pdwBadFeeling[ j ];

				if( myFavorite == opponentFavorite )
				{
					isFound = true;
					break;
				}
			}

			if( ! isFound )
			{
				const int favoriteGap = 10;

				score -= favoriteGap;
			}
        }
    }

	const int matchScore = CSHDateManager::NOTICE_HEART_MATCHING_POING;

    return matchScore <= score;
}


const DATE_MATCHING_INFO& CPlayer::GetDateMatchingInfo() const
{
	return m_CharacterInfo.DateMatching;
}


void CPlayer::SetHeart( BOOL isHeart )
{
	if(	isHeart )
	{
		if( ! m_IsDrawHeart )
		{
			OBJECTEFFECTDESC desc(20);
			
			AddObjectEffect( HEART_EFFECT, &desc, 1 );

			m_IsDrawHeart = TRUE;
		}
	}
	else
	{
		RemoveObjectEffect( HEART_EFFECT );

		m_IsDrawHeart = FALSE;
	}
}


const CPlayer::SetItemLevel& CPlayer::GetSetItemLevel() const
{
	return mSetItemLevel;
}


CPlayer::SetItemLevel& CPlayer::GetSetItemLevel()
{
	return mSetItemLevel;
}




// 0701003 LYW --- Player : Add function to setting class index.
void CPlayer::SettingClassIndex()
{
	int nClass		= m_CharacterInfo.Job[0] ;								// Ŭ���� �迭 ���� �޴´�.
	int nRace		= m_CharacterInfo.Race ;								// ���� ��ȣ�� �޴´�.
	int nJobGrade	= m_CharacterInfo.JobGrade ;							// Ŭ���� �ܰ踦 �޴´�.
	int nJobNum		= m_CharacterInfo.Job[nJobGrade] ;						// ���� Ŭ���� ���� �޴´�.

	if( nJobGrade == 1 )													// ���� �ܰ谡 1�϶���, �迭���� �Ѱ��� �����ۿ� ���� ������
	{
		nJobNum = 1 ;														// ���� ��ȣ�� 1�� �����Ѵ�.
	}

	m_nClassIndex = ((nClass)*1000) + ((nRace+1)*100) + (nJobGrade*10) + nJobNum ;	// �ϼ� �� Ŭ���� ��ȣ�� �޴´�.
}





