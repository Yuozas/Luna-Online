#include "stdafx.h"
#include "MouseEventThrower.h"
#include "MouseEventReceiver.h"
#include "interface\cWindowManager.h"

#include "MousePointer.h"
#include "ObjectManager.h"
#include "gamein.h"
#include "MouseCursor.h"
#include "MHMap.h"
#include "WindowIDEnum.h"

#include "MonsterGuageDlg.h"

// desc_hseos_농장시스템_01
// S 농장시스템 추가 added by hseos 2007.09.18
#include "../hseos/Farm/SHFarmManager.h"
// E 농장시스템 추가 added by hseos 2007.09.18

// 080618 KTH add
#include "../hseos/Farm/FarmAnimalDlg.h"

#include "MoveManager.h"

CMouseEventThrower::CMouseEventThrower()
{
	m_pCurrentReceiver	= NULL;
	m_pDefaultReceiver	= NULL;

	m_bCancelSelect		= FALSE;
}

CMouseEventThrower::~CMouseEventThrower()
{

}

void CMouseEventThrower::SetDefaultReceiver(CMouseEventReceiver* pDefaultReceiver)
{
	m_pDefaultReceiver = pDefaultReceiver;
}

void CMouseEventThrower::SetCurrentReceiver(CMouseEventReceiver* pCurrentReceiver)
{
	m_pCurrentReceiver = pCurrentReceiver;
}

void CMouseEventThrower::SetReceiverToDefault()
{
	ASSERT(m_pDefaultReceiver);
	m_pCurrentReceiver = m_pDefaultReceiver;
}


#define MOUSE_MOVE_TICK		400
#define MOUSE_PRESS_TICK	400
#define MOUSE_ON_TICK		200

//---KES 따라가기
#define FOLLOW_TICK			500

void CMouseEventThrower::Follow()
{
	static DWORD dwLastFollowTick = 0;

	if( HERO )
	if( HERO->GetFollowPlayer() )
	{
		if( gCurTime - dwLastFollowTick >= FOLLOW_TICK )
		{
			dwLastFollowTick = gCurTime;

			CObject* pObject = OBJECTMGR->GetObject( HERO->GetFollowPlayer() );
			if( pObject )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					VECTOR3 vMyPos, vTargetPos;

					HERO->GetPosition( &vMyPos );
					pObject->GetPosition( &vTargetPos );

					float dist = CalcDistanceXZ( &vMyPos, &vTargetPos );

					if( dist > 300.0f )
					{
						vTargetPos = ( vTargetPos - vMyPos ) * ( dist - 200.0f ) / dist + vMyPos;
						m_pCurrentReceiver->OnClickGround(MOUSEEVENT_LPRESS, &vTargetPos);
					}
				}
			}
			else
			{
				HERO->SetFollowPlayer( 0 );
				MOVEMGR->HeroMoveStop();
			}
		}
	}
}
//--------------

void CMouseEventThrower::Process(CMouse* Mouse)
{
	static CObject* pOldMouseOverObject		= NULL;
	static DWORD dwMouseMoveTick			= 0;
	static DWORD dwMouseOnTick				= 0;
	static DWORD dwMousePressTick			= 0;

	if( WINDOWMGR->IsMouseInputProcessed() || WINDOWMGR->IsDragWindow() )
	{
		if( pOldMouseOverObject )
		{
			m_pCurrentReceiver->OnMouseLeave( pOldMouseOverObject );
			pOldMouseOverObject = NULL;
		}
		
		dwMousePressTick = gCurTime;
		return;	//윈도우 드래그중일때는 마우스 이벤트 체크가 불필요.
	}

	DWORD MouseEvent = MOUSEEVENT_NONE;

	if( Mouse->LButtonDoubleClick() || Mouse->RButtonDoubleClick() )
		MouseEvent = MOUSEEVENT_DBLCLICK;
	else
	{
		if( Mouse->LButtonDown() )
			MouseEvent = MOUSEEVENT_LCLICK;
		else if( Mouse->RButtonDown() )
			MouseEvent = MOUSEEVENT_RCLICK;

		if( Mouse->LButtonPressed() )
			MouseEvent |= MOUSEEVENT_LPRESS;
	}

//---캐릭터 선택 해제관련
	if( Mouse->RButtonDown() )	m_bCancelSelect = TRUE;
	if( Mouse->RButtonDrag() )	m_bCancelSelect = FALSE;

	if( OBJECTMGR->GetSelectedObjectID() && Mouse->RButtonUp() && m_bCancelSelect )
	{
		//OBJECTMGR->SetSelectedObject( NULL );
//		if( GAMEIN->GetMonsterGuageDlg())
//			GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
//		if(GAMEIN->GetPlayerGuageDlg())
//			GAMEIN->GetPlayerGuageDlg()->SetActive(FALSE);

		m_bCancelSelect = FALSE;
	}
	
	if( MouseEvent == MOUSEEVENT_NONE )
	{
		if( gCurTime - dwMouseOnTick > MOUSE_ON_TICK )
		{
			dwMouseOnTick = gCurTime;
			//if( OPTIONMGR->GetGameOption()->bGraphicCursor ) //그래픽커서일때만
			//---마우스가 오브젝트 위로, 아래함수 빼고 FPS체크 confirm
			CObject* pOverObject = (CObject*)GetSelectedObject( Mouse->GetMouseX(), Mouse->GetMouseY() );
			//		CObject* pOverObject = NULL;
			
			if( pOldMouseOverObject != pOverObject )
			{
				m_pCurrentReceiver->OnMouseLeave( pOldMouseOverObject );
				m_pCurrentReceiver->OnMouseOver( pOverObject );
				
				pOldMouseOverObject = pOverObject;
			}
			//---마우스가 타일 위로
			if( pOverObject == NULL )
			{
				CURSOR->SetCursor( eCURSOR_DEFAULT );
				//못가는 지역 마우스커서 바꾸기
/*				VECTOR3* TargetPos = GetPickedPosition( Mouse->GetMouseX(), Mouse->GetMouseX() );
				if( TargetPos )
				{
					if( MAP->CollisionTilePos( TargetPos->x, TargetPos->z, MAP->GetMapNum(), HERO ) )
						CURSOR->SetCursor( eCURSOR_CANTMOVE );//on ground but can't go there
					else
						CURSOR->SetCursor( eCURSOR_DEFAULT );//on ground, can go there
				}
				else
					CURSOR->SetCursor( eCURSOR_CANTMOVE );//not on ground
					*/
			}
		}

		
/*		
		/////공격중엔.. 자동 따라가기
		if( HERO )
		if( HERO->IsAutoAttacking() )
		{
			HERO->GetAutoAttackTarget()->g
			if( )
			dwMouseMoveTick = gCurTime
		pHero->SetCurComboNum(0);
		CAction MoveAction;
		if(pSkillInfo->IsSkill() == FALSE)
		{
			pSkillInfo = GetSkillInfo(GetComboSkillIdx(pHero));
		}
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroActionMove(&target,&MoveAction);
		return FALSE;
*/		
	}
//---마우스 클릭
	else
	{
		// 프로그레스 창이 떠 있으면 움직이지 못하도록 한다.
		{
			cDialog* dialog = WINDOWMGR->GetWindowForID( PROGRESS_DIALOG );
			ASSERT( dialog );

			if( dialog->IsDisable() )
			{
				return;
			}
		}
		
		// desc_hseos_농장시스템_01
		// S 농장시스템 추가 added by hseos 2007.09.18
		// ..농장 모션 중이면 마우스 입력을 받지 않는다.
		if (g_csFarmManager.CLI_GetIngCharMotion())	return;
		// E 농장시스템 추가 added by hseos 2007.09.18

		//CObject* pObject = (CObject*)GetSelectedObject( Mouse->GetMouseEventX(), Mouse->GetMouseEventY() , PICK_TYPE_PER_FACE );
		CObject* pObject = (CObject*)GetSelectedObject( Mouse->GetMouseEventX(), Mouse->GetMouseEventY());

		// 080111 KTH -- 보스 몬스터 일 경우에만 PICK_TYPE_PER_FACE 호출
		if( pObject && pObject->GetObjectKind() == eObjectKind_BossMonster )
		{
			pObject = (CObject*)GetSelectedObject( Mouse->GetMouseEventX(), Mouse->GetMouseEventY(), PICK_TYPE_PER_FACE );
		}

		if( pObject )
		{
			switch( pObject->GetEngineObjectType() )
			{
			case eEngineObjectType_Character:
				{
					m_pCurrentReceiver->OnClickPlayer(MouseEvent,(CPlayer*)pObject);
					break;
				}
			case eEngineObjectType_Monster:
				{
					m_pCurrentReceiver->OnClickMonster(MouseEvent,(CMonster*)pObject);
					break;
				}
			case eEngineObjectType_Npc:
				{
					m_pCurrentReceiver->OnClickNpc(MouseEvent,(CNpc*)pObject);
					break;
				}
			case eEngineObjectType_SkillObject:
				{
					m_pCurrentReceiver->OnClickSkillObject(MouseEvent,(cSkillObject*)pObject);
					break;
				}
			case eEngineObjectType_Effect:
				{
					ASSERT(0);
					break;
				}
			case eEngineObjectType_FarmRenderObj:
				{
					m_pCurrentReceiver->OnClickFarmRenderObj(MouseEvent,(CSHFarmRenderObj*)pObject);

					// 080618 KTH -- not select target
					if ( GAMEIN->GetAnimalDialog()->IsActive() )
						return;
					break;
				}
			default:
				{
					ASSERT( 0 );
					break;
				}
			}
			
			if( MouseEvent & MOUSEEVENT_DBLCLICK )
			{
				if( pObject->GetEngineObjectType() == eEngineObjectType_Character )
				{
					m_pCurrentReceiver->OnDbClickPlayer( MouseEvent,(CPlayer*)pObject );
				}
				else if( pObject->GetEngineObjectType() == eEngineObjectType_Monster )
				{
					m_pCurrentReceiver->OnDbClickMonster( MOUSEEVENT_LCLICK, (CMonster*)pObject );
				}
				else if( pObject->GetEngineObjectType() == eEngineObjectType_Pet )
				{
					m_pCurrentReceiver->OnDbClickPet( MOUSEEVENT_LCLICK, (CPet*)pObject );
				}
//				else if( pObjectBase->GetEngineObjectType() == eEngineObjectType_Npc )
//				{
//					m_pCurrentReceiver->OnDbClickNpc( MouseEvent, (CNpc*)pObject );
//				}
			}

			if( MouseEvent & MOUSEEVENT_LCLICK && pObject != HERO )	//자신은 클릭이 안된다.
			{
				int ObjectType = pObject->GetEngineObjectType() ;

				if( ObjectType == eEngineObjectType_Monster )
				{
					pObject->ShowObjectName( TRUE, RGB_HALF( 255, 141, 39 ) );
				}
				else
				{
					pObject->ShowObjectName( TRUE, NAMECOLOR_SELECTED );
				}

				OBJECTMGR->SetSelectedObject( pObject );
			}
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
			{
				if( gCurTime - dwMouseMoveTick >= MOUSE_MOVE_TICK )
				{
					dwMouseMoveTick = gCurTime;
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseEventX(),Mouse->GetMouseEventY());
					if(!TargetPos)
						return;
					m_pCurrentReceiver->OnClickGround(MouseEvent,TargetPos);

					//---KES 따라가기
					if(HERO)
						HERO->SetFollowPlayer( 0 );
					//---------------
				}
			}
			else if( MouseEvent & MOUSEEVENT_LPRESS )
			{
				if( gCurTime - dwMousePressTick >= MOUSE_PRESS_TICK )
				{
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseX(), Mouse->GetMouseY());
					if(!TargetPos)
						return;
					m_pCurrentReceiver->OnClickGround(MouseEvent,TargetPos);

					//---KES 따라가기
					if(HERO)
						HERO->SetFollowPlayer( 0 );
					//---------------
				}				
			}
		}
	}
}
