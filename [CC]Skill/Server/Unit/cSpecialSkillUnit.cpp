#include "stdafx.h"
#include ".\cspecialskillunit.h"
#include "../Target/Target.h"
#include "Object.h"
#include "Monster.h"
#include "Player.h"
#include "../Object/SkillObject.h"
#include "StateMachinen.h"
// desc_hseos_����Ʈ ��_01
// S ����Ʈ �� �߰� added by hseos 2007.12.03
#include "../hseos/Date/SHDateManager.h"
// E ����Ʈ �� �߰� added by hseos 2007.12.03
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

			// desc_hseos_����Ʈ ��_01
			// S ����Ʈ �� �߰� added by hseos 2007.12.03
			// ..ç���� �������� ��Ȱ �Ұ�
			if (g_csDateManager.IsChallengeZoneHere())
			{
				return FALSE;
			}
			// E ����Ʈ �� �߰� added by hseos 2007.12.03

			if( mpTarget->GetKind() < TARGET_KIND_MULTI )
			{
				pTarget = mpTarget->GetData();

				if(!pTarget)
					return FALSE;

				if( pTarget->GetObjectKind() != eObjectKind_Player )
					return FALSE;

				pPlayer = ( ( CPlayer* )pTarget );

				// 080317 LUJ, ���/PK ���� ��뿡�Դ� ����� �� ����
				if( pPlayer->IsPKMode() ||
					pPlayer->IsShowdown() )
				{
					return FALSE;
				}
				// 080318 LUJ, ��밡 PK�� ���� ����ġ ��ս� ������ ��쿡�� ����� �� ����
				else if( pPlayer->IsNoExpPenaltyByPK() )
				{
					return FALSE;
				}

				// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
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
					// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
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
				if( mpParentSkill->GetOperator()->GetObjectKind() == eObjectKind_Player ) //���ٿ��� üũ�ϹǷ� �������� üũ ����
				{
					pTarget->GetAbnormalStatus()->IsAttract = TRUE;

					//---KES Aggro 070918
					((CMonster*)pTarget)->CalcAggro( (CPlayer*)mpParentSkill->GetOperator(), (int)mAddDamage );

					//��׷� �������� �����Ͽ� �ּ�ó��
					/*
					if( ((CMonster*)pTarget)->SetTObject( (CPlayer*)(mpParentSkill->GetOperator()) ) )
					{
						GSTATEMACHINE.SetState( pTarget, eMA_PERSUIT );
					}
					*/
					//--------------------
				}
			}
			/// Multi Target�� ���
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

					//��׷� �������� �����Ͽ� �ּ�ó��
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
			/// Multi Target�� ���
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
