#include "stdafx.h"
#include ".\cspecialskillunit.h"
#include "../Target/Target.h"
#include "Object.h"
#include "Monster.h"
#include "Player.h"
#include "../Object/SkillObject.h"
#include "StateMachinen.h"
// desc_hseos_데이트 존_01
// S 데이트 존 추가 added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E 데이트 존 추가 added by hseos 2007.12.03
#include "MapNetworkMsgParser.h"
#include "../Manager/SkillManager.h"

cSpecialSkillUnit::cSpecialSkillUnit( cSkillObject* pParentSkill, cTarget* pTarget )
: cSkillUnit( pParentSkill, pTarget )
{
}

cSpecialSkillUnit::~cSpecialSkillUnit(void)
{
}

BOOL cSpecialSkillUnit::Excute()
{
	CObject*	pTarget = NULL;

	switch( mUnitType )
	{
	case UNITKIND_RESURRECTION:
		{
			CPlayer* pPlayer = NULL;

			// desc_hseos_데이트 존_01
			// S 데이트 존 추가 added by hseos 2007.12.03
			// ..챌린지 존에서는 부활 불가
			if (g_csDateManager.IsChallengeZoneHere())
			{
				return FALSE;
			}
			// E 데이트 존 추가 added by hseos 2007.12.03

			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;

				if( pTarget->GetObjectKind() != eObjectKind_Player )
					return FALSE;

				pPlayer = ( ( CPlayer* )pTarget );

				// 080317 LUJ, 듀얼/PK 중인 상대에게는 사용할 수 없다
				if( pPlayer->IsPKMode() ||
					pPlayer->IsShowdown() )
				{
					return FALSE;
				}
				// 080318 LUJ, 상대가 PK로 인한 경험치 비손실 상태일 경우에도 사용할 수 없다
				else if( pPlayer->IsNoExpPenaltyByPK() )
				{
					return FALSE;
				}

				// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
				//DWORD exp = pPlayer->ReviveBySkill();
				//exp = exp * ( mAddDamage / 100.f );

				EXPTYPE exp = pPlayer->ReviveBySkill();
				exp = (EXPTYPE)(exp * ( mAddDamage / 100.f )) ;

				if( exp )
				{
					pPlayer->AddPlayerExpPoint( exp );
				}
			}
			else
			{
				mpTarget->SetPositionHead();
				while( pTarget = mpTarget->GetData() )
				{
					if( pTarget->GetObjectKind() != eObjectKind_Player )
						continue;
					
					pPlayer = ( ( CPlayer* )pTarget );
					// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
					//DWORD exp = pPlayer->ReviveBySkill();
					//exp = exp * ( mAddDamage / 100.f );

					EXPTYPE exp = pPlayer->ReviveBySkill();
					exp = (EXPTYPE)(exp * ( mAddDamage / 100.f )) ;

					if( exp )
					{
						pPlayer->AddPlayerExpPoint( exp );
					}
				}
			}
		}
		break;
	case UNITKIND_FAKE_DAMAGE:
		{
			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;

				if( pTarget->GetObjectKind() != eObjectKind_Monster )
					return FALSE;

//				if( pTarget->GetObjectKind() == eObjectKind_Monster && mpParentSkill->GetOperator()->GetObjectKind() == eObjectKind_Player )
				if( mpParentSkill->GetOperator()->GetObjectKind() == eObjectKind_Player ) //윗줄에서 체크하므로 몬스터인지 체크 삭제
				{
					pTarget->GetAbnormalStatus()->IsAttract = TRUE;

					//---KES Aggro 070918
					((CMonster*)pTarget)->CalcAggro( (CPlayer*)mpParentSkill->GetOperator(), (int)mAddDamage );

					//어그로 개념으로 변경하여 주석처리
					/*
					if( ((CMonster*)pTarget)->SetTObject( (CPlayer*)(mpParentSkill->GetOperator()) ) )
					{
						GSTATEMACHINE.SetState( pTarget, eMA_PERSUIT );
					}
					*/
					//--------------------
				}
			}
			/// Multi Target인 경우
			else
			{
				mpTarget->SetPositionHead();
				while( pTarget = mpTarget->GetData() )
				{
					if( pTarget->GetObjectKind() != eObjectKind_Monster )
						continue;

					//---KES Aggro 070918
					if( mpParentSkill->GetOperator()->GetObjectKind() == eObjectKind_Player )
					{
						pTarget->GetAbnormalStatus()->IsAttract = TRUE;

						((CMonster*)pTarget)->CalcAggro( (CPlayer*)mpParentSkill->GetOperator(), (int)mAddDamage );
					}

					//어그로 개념으로 변경하여 주석처리
					/*
					if( ((CMonster*)pTarget)->SetTObject( (CPlayer*)(mpParentSkill->GetOperator()) ) )
					{
						GSTATEMACHINE.SetState( pTarget, eMA_PERSUIT );
					}
					*/
					//--------------------
				}
			}
		}
		break;
	case UNITKIND_DECOY:
		{
			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;
				
				if( pTarget->GetObjectKind() != eObjectKind_Monster )
					return FALSE;

				if( ((CMonster*)pTarget)->SetTObject( (CPlayer*)(mpParentSkill->GetOperator()) ) )
				{
					GSTATEMACHINE.SetState( pTarget, eMA_PERSUIT );
				}
			}
			/// Multi Target인 경우
			else
			{
				mpTarget->SetPositionHead();
				while( pTarget = mpTarget->GetData() )
				{
					if( pTarget->GetObjectKind() != eObjectKind_Monster )
						continue;

					if( ((CMonster*)pTarget)->SetTObject( (CPlayer*)(mpParentSkill->GetOperator()) ) )
					{
						GSTATEMACHINE.SetState( pTarget, eMA_PERSUIT );
					}
				}
			}
		}
		break;
	case UNITKIND_RETURN:
		{
			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;
				
				if( pTarget->GetObjectKind() != eObjectKind_Player )
					return FALSE;

				CPlayer* pPlayer = ( CPlayer* )pTarget;

				if( pPlayer->IsPKMode() )
				{
					return FALSE;
				}

				MSG_WORD message;
				message.dwObjectID	= pPlayer->GetID();
				message.Category	= MP_USERCONN;
				message.Protocol	= MP_USERCONN_RETURN_SYN;
				message.wData		= pPlayer->GetPlayerLoginPoint();

				UserConn_Return_Syn( pPlayer->GetAgentNum(), ( char* )&message );
			}
			else
			{
				mpTarget->SetPositionHead();
				while( pTarget = mpTarget->GetData() )
				{
					if( pTarget->GetObjectKind() != eObjectKind_Player )
						continue;	
				
					CPlayer* pPlayer = ( CPlayer* )pTarget;

					if( pPlayer->IsPKMode() )
					{
						continue;
					}

					MSG_WORD message;
					message.dwObjectID	= pPlayer->GetID();
					message.Category	= MP_USERCONN;
					message.Protocol	= MP_USERCONN_RETURN_SYN;
					message.wData		= pPlayer->GetPlayerLoginPoint();

					UserConn_Return_Syn( pPlayer->GetAgentNum(), ( char* )&message );
				}
			}
		}
		break;
	case UNITKIND_SPELLBREAK:
		{
			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;

				if( pTarget->CurCastingSkillID )
				{
					MSG_DWORD msg;
					msg.dwObjectID = pTarget->GetID();
					msg.dwData = pTarget->CurCastingSkillID;
					SKILLMGR->OnSkillCancel( &msg );
				}
			}
			else
			{
				mpTarget->SetPositionHead();
				while( pTarget = mpTarget->GetData() )
				{
					if( pTarget->CurCastingSkillID )
					{
						MSG_DWORD msg;
						msg.dwObjectID = pTarget->GetID();
						msg.dwData = pTarget->CurCastingSkillID;
						SKILLMGR->OnSkillCancel( &msg );
					}
				}
			}
		}
		break;

	}

	return TRUE;
}
