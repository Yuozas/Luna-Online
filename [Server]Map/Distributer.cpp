// Distributer.cpp: implementation of the CDistributer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distributer.h"
#include "UserTable.h"
#include "CharMove.h"
#include "AttackManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "Distribute_Random.h"
#include "Monster.h"
#include "BossRewardsManager.h"
// �ʵ庸�� - 05.12 �̿���
#include "FieldBossMonsterManager.h"
#include "ItemManager.h"

// 070413 LYW --- Distributer : Include CommonCalcFunc.
#include "..\[CC]Header\CommonCalcFunc.h"
#include "ItemDrop.h"

// 080116 LYW --- Distributer : ����Ʈ �Ŵ��� ��� ����.
#include "../hseos/Date/SHDateManager.h"

#include "../hseos/Family/SHFamilyManager.h"
#include "petmanager.h"
#include "pet.h"

#include "MHError.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//float gExpRate		= 20.0f; //090113 modified by XB delete note
//float gItemRate		= 8.0f; //090113 modified by XB delete note
//float gMoneyRate	= 16.0f; //090113 modified by XB delete note
//// RaMa -04.11.24
//float gDamageReciveRate	= 1.0f;		// �޴� ������
//float gDamageRate		= 1.0f;		// �ִ� ������
//float gNaeRuykRate	= 1.0f;		// ���¼Ҹ�
//float gUngiSpeed	= 1.0f;		// ������� ���ǵ�
//float gPartyExpRate	= 1.0f;		// ��Ƽ����ġ
//float gAbilRate		= 1.0f;		// Ư��ġ
//float gGetMoney		= 1.0f;		// ��µ��� ���
//float gSkillExpRate = 1.0f;	// ��������ġ ���
//// Etc - �����ȵ�
//float gDefence		= 1.0f;		// ����
//
float gEventRate[eEvent_Max];
float gEventRateFile[eEvent_Max];
BOOL g_bPlusTime = FALSE;


CDistributer::CDistributer()																	// ������ �Լ�.
{
	m_DamageObjectTableSolo.Initialize(MAX_POINTACCEPTOBJECT_NUM);								// ������ �������� ���� ���̺� �ʱ�ȭ.
	m_DamageObjectTableParty.Initialize(MAX_POINTACCEPTOBJECT_NUM);								// ��Ƽ�� �������� ���� ���̺� �ʱ�ȭ.

	m_1stPartyID = 0;																			// ù �������� �� ��Ƽ ���̵� �ʱ�ȭ.
	m_1stPlayerID = 0;																			// ù �������� �� �÷��̾� ���̵� �ʱ�ȭ.
	m_PlusDamage = 0;																			// �߰� ������ �ʱ�ȭ.						
	m_TotalDamage = 0;																			// ������ �հ� �ʱ�ȭ.
	m_FirstDamage = 0 ;																			// ù ������ �ʱ�ȭ.
	m_nTotalKillerCount = 0 ;																	// ų�� �� �ʱ�ȭ.	
	m_HighLevel = 0 ;																			// ų�� �� �� ���� ������ ���� ų���� ����.

	m_pKiller			= NULL ;																// ų�� ������ ��� ������ �ʱ�ȭ.
	m_pKillerParty		= NULL ;																// ų���� ��Ƽ ������ ��� ������ �ʱ�ȭ.

	m_pMob				= NULL ;																// ���� ������ ��� ������ �ʱ�ȭ.
	m_pKilledPosition	= NULL ;																// ���͸� ���� ��ġ ������ �ʱ�ȭ.

	m_dwDropItemID		= 0 ;																	// ��� ������ ���̵� ��� ���� �ʱ�ȭ.
	m_dwDropItemRatio	= 0 ;																	// ������� ��� ���� �ʱ�ȭ.								
}

CDistributer::~CDistributer()																	// �Ҹ��� �Լ�.
{
	Release();																					// ���� �Լ��� ȣ���Ѵ�.
}

void CDistributer::Release()																	// ���� �Լ�.
{
	m_1stPartyID = 0;																			// ù �������� �� ��Ƽ ���̵� �ʱ�ȭ.
	m_1stPlayerID = 0;																			// ù �������� �� �÷��̾� ���̵� �ʱ�ȭ.
	m_PlusDamage = 0;																			// �߰� ������ �ʱ�ȭ.		
	m_TotalDamage = 0;																			// ������ �հ� �ʱ�ȭ.
	m_FirstDamage = 0 ;																			// ù ������ �ʱ�ȭ.
	m_nTotalKillerCount = 0 ;																	// ų�� �� �ʱ�ȭ.
	m_HighLevel = 0 ;																			// ų�� �� �� ���� ������ ���� ų���� ����.

	DAMAGEOBJ *sobj, *pobj = NULL;																// ���� �� ��Ƽ ������ ���̺��� ������.

	m_DamageObjectTableSolo.SetPositionHead();													// ���� ������ ���̺��� ����� �����Ѵ�.

	while(sobj = m_DamageObjectTableSolo.GetData())												// ������ ������Ʈ�� ���� �� ���� while.
	{
		delete sobj;																			// ������ ������Ʈ�� �����Ѵ�.
		sobj = NULL ;																			// ������ ������Ʈ NULLó��.
	}

	m_DamageObjectTableSolo.RemoveAll();														// ������ ������Ʈ�� ����.				

	m_DamageObjectTableParty.SetPositionHead();													// ��Ƽ ������ ���̺��� ����� �����Ѵ�.

	while(pobj = m_DamageObjectTableParty.GetData())											// ������ ������Ʈ�� ���� �� ���� while.		
	{																							
		delete pobj;																			// ������ ������Ʈ�� �����Ѵ�.
		pobj = NULL ;																			// ������ ������Ʈ NULLó��.
	}																							
																								
	m_DamageObjectTableParty.RemoveAll();														// ������ ������Ʈ�� ����.					

	m_pKiller			= NULL ;																// ų�� ������ ��� ������ �ʱ�ȭ.
	m_pKillerParty		= NULL ;																// ų���� ��Ƽ ������ ��� ������ �ʱ�ȭ.

	m_pMob				= NULL ;																// ���� ������ ��� ������ �ʱ�ȭ.
	m_pKilledPosition	= NULL ;																// ���͸� ���� ��ġ ������ �ʱ�ȭ.

	m_dwDropItemID		= 0 ;																	// ��� ������ ���̵� ��� ���� �ʱ�ȭ.
	m_dwDropItemRatio	= 0 ;																	// ������� ��� ���� �ʱ�ȭ.							

	PTRLISTPOS pos = m_CandidacyList.GetHeadPosition() ;										// ��Ƽ���� ����� �ĺ��ڸ� ��� ����Ʈ�� �ص�� �����Ѵ�.

    DWORD* pMemberID = NULL ;																	// ��� ���̵� ���� ������.

	while(pMemberID = (DWORD*)m_CandidacyList.GetNext(pos))										// ����Ʈ���� �����͸� �޴´�.
	{
		if(pMemberID)																			// ������ ������ ��ȿ���� üũ�Ѵ�.
		{
			delete pMemberID ;																	// �����Ϳ� �ִ� ������ �����.
			pMemberID = NULL ;																	// �����͸� NULL ó�� �Ѵ�.
		}
	}

	m_CandidacyList.RemoveAll() ;																// ����Ʈ�� ����.
}

void CDistributer::AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage)
{
	if( pPlayer ) 																				// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
	{
		CParty* pParty = NULL ;																	// ��Ƽ ���θ� üũ�ϱ� ���� ��Ƽ �����͸� ����.
		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;									// ��Ƽ ������ �޴´�.

		if( pParty )																			// ��Ƽ������ ��ȿ���� üũ�Ѵ�.
		{
			DoAddDamageObj(&m_DamageObjectTableParty,											// �������� ��Ƽ ������ ���̺� �߰��Ѵ�.
				pPlayer->GetPartyIdx(), damage+plusdamage);
		}

		DoAddDamageObj(&m_DamageObjectTableSolo,												// �������� ���� ������ ���̺� �߰��Ѵ�.
			pPlayer->GetID(), damage+plusdamage);	

		//if(plusdamage != 0)																		// �߰� �������� �ִ��� üũ�Ѵ�.
		if( m_1stPlayerID == 0 )
		{
			m_PlusDamage = plusdamage;															// �߰� �������� �޴´�.

			m_1stPlayerID = pPlayer->GetID();													// ù �������� �� �÷��̾� ���̵� �޴´�.
			m_1stPartyID = pPlayer->GetPartyIdx();												// ù �������� �� �÷��̾��� ��Ƽ �ε����� �޴´�.
		}
		
		SetPlusTotalDamage(damage+plusdamage);													// ������ �հ踦 �����Ѵ�.								
	}
}

void CDistributer::DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage)
{
	if( !pTable ) return ;																		// ���� ������ ���̺��� ������ ��ȿ���� üũ�Ѵ�.

	DAMAGEOBJ* pDObj;																			// ������ ������Ʈ �����͸� �����Ѵ�.

	pDObj = pTable->GetData(dwID);																// ������ ���̺��� ���̵� �ش��ϴ� ������ ������Ʈ�� �޴´�.

	if(pDObj != NULL)																			// ������ ������Ʈ�� ��ȿ���� üũ�Ѵ�.
	{
		pDObj->dwData += damage;																// ������ ������Ʈ�� �������� ���� �������� �߰��Ѵ�.
	}
	else
	{
		DAMAGEOBJ * newObj = new DAMAGEOBJ;														// ������ ������Ʈ�� ������ ���� �����Ѵ�.

		newObj->dwID = dwID;																	// ������ ������Ʈ�� ���̵� �����Ѵ�.
		newObj->dwData = damage;																// ������ ������Ʈ�� �������� �����Ѵ�.
		
		pTable->Add(newObj, newObj->dwID);														// ������ ���̺� �߰��Ѵ�.											
	}
}


BOOL CDistributer::Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	if( !pKilledPosition ) return FALSE ;														// ���� ��ġ �������� ������ ��ȿ���� üũ�Ѵ�.

	VECTOR3* PlayerPosition;																	// �÷��̾��� ��ġ������ ��� ������ ����.

	if( pPlayer )																				// �÷��̾��� ������ ��ȿ���� üũ�Ѵ�.
	{
		if( pPlayer->GetInitState() == PLAYERINITSTATE_INITED )									// �÷��̾��� �ʱ�ȭ ���°� PLAYERINITSTATE_INITED�� ������ ���Ѵ�.
		{
			if( !pPlayer->IsShowdown() )														// �÷��̾ ��������� üũ�Ѵ�.
			{
				PlayerPosition = CCharMove::GetPosition(pPlayer) ;								// �÷��̾��� ��ġ������ �޴´�.										

				float fDistance = CalcDistanceXZ( pKilledPosition, PlayerPosition ) ;			// ���Ͱ� ���� ��ġ�� �÷��̾��� ��ġ �Ÿ��� ���Ѵ�.

				if( fDistance <= POINT_VALID_DISTANCE )											// �־��� ���� ������ üũ�Ѵ�.
				{
					if( pPlayer->GetState() != eObjectState_Die )								// �÷��̾ ���� �������� üũ�Ѵ�.
					{
						if( pPlayer->GetGridID() == GridID )									// �׸��带 ���Ѵ�.
						{
							return TRUE ;														// TRUE ����.										
						}
					}
				}
			}
		}
	}

	return FALSE ;																				// FALSE ����.
}


BOOL CDistributer::ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	if( !pKilledPosition ) return FALSE ;														// ���� ��ġ �������� ������ ��ȿ���� üũ�Ѵ�.

	if( pPlayer )																				// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
	{
		if( pPlayer->GetInitState() == PLAYERINITSTATE_INITED )									// �÷��̾� �ʱ�ȭ ���°� PLAYERINITSTATE_INITED�� ������ ���Ѵ�.
		{
			if( !pPlayer->IsShowdown() )														// �÷��̾ ��������� ���Ѵ�.
			{
				if( pPlayer->GetState() != eObjectState_Die )									// �÷��̾ ���� �������� üũ�Ѵ�.
				{
					if( pPlayer->GetGridID() == GridID )										// �׸��带 �� �Ѵ�.
					{
						return TRUE ;															// TRUE ����.														
					}
				}
			}
		}
	}

	return FALSE ;																				// FALSE ����.
}

void CDistributer::ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID)
{
	if( !pobj || !pBigDamage || !pBigID ) return ;												// �Լ� ���ڷ� ������ �����͵��� ������ ��ȿ���� üũ�Ѵ�.

	if(*pBigDamage < pobj->dwData)																// ������ ������Ʈ�� �������� �ִ� ���������� ũ��.
	{
		*pBigDamage = pobj->dwData;																// �ִ� �������� ������ ������Ʈ�� �������� �����Ѵ�.
		*pBigID = pobj->dwID;																	// �ִ� ������ ���̵� ������ ������Ʈ�� ���̵� �����Ѵ�.
	}
	else if(*pBigDamage == pobj->dwData)														// ������ ������Ʈ�� �������� �ִ� �������� ���ٸ�.
	{
		int rate = rand() % 2;																	// Ȯ���� ���Ѵ�.

		if(rate == 1)																			// ���� Ȯ����
		{
			*pBigID = pobj->dwID;																// ������ ������Ʈ�� ���̵� �ִ� ������ ���̵�� �����Ѵ�.
		}
	}
}

// �ʵ庸�� - 05.12 �̿���
// �ʵ庸���� ������ �й� �Լ�
void CDistributer::DistributeItemPerDamage(CMonster* pMob)
{
	DAMAGEOBJ *obj;														

	CPlayer* pReceivePlayer;											

	WORD MonsterKind;													

	CPlayer* TargetPlayerTable[ 100 ];									

	WORD TargetPos = 0;
	
	MonsterKind = pMob->GetMonsterKind();								

	FIELDBOSSDROPITEMLIST* pDropItemList = NULL ;						
	pDropItemList = FIELDBOSSMONMGR->GetFieldBossDropItemList( MonsterKind );
    
	if( pDropItemList )													
	{
		for(WORD index = 0; index < MAX_FIELDBOSS_DROPITEM_GROUP_NUM; index++)
		{
			// ��� ������ ������ �ִ°�? ���ٸ� ����
			if( pDropItemList->m_ItemList[ index ].m_wItemIndex[0] == 0 )
				break;
	        
			// ������ �׷� ���� � �������� ���� ����
			for( WORD dropitemnum = 0; dropitemnum < MAX_FIELDBOSS_DROPITEM_NUM; dropitemnum++)
			{
				if(pDropItemList->m_ItemList[ index ].m_wItemIndex[dropitemnum] == 0)
					break;
			}

			WORD select = rand() % dropitemnum;

			// ����� Ƚ���� �����´�
			WORD count = pDropItemList->m_ItemList[ index ].m_wCount;

			for(WORD i = 0; i < count; i++)
			{
				// ��� �Ұ��ΰ�?
				if( (rand() % 100) < pDropItemList->m_ItemList[ index ].m_wDropRate )
				{
					// �����ʱ�ȭ
					memset( TargetPlayerTable, 0, sizeof(CPlayer*) * 100 );
					TargetPos = 0;

					// ������ ������� �˻��ؼ� ����ġ �̻� �������� �� ����� ��������
					m_DamageObjectTableSolo.SetPositionHead();
					while(obj = m_DamageObjectTableSolo.GetData())
					{
						if(obj->dwData == 0)
							continue;		
						pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
						if(!pReceivePlayer)	
							continue;
						if(pReceivePlayer->GetInitState() != PLAYERINITSTATE_INITED)
							continue;
						if(pReceivePlayer->GetGridID() != pMob->GetGridID())
							continue;
						// ���Ϳ� +6 �̻� Ȥ�� -9 ������ Player�� �й迡�� ����
						if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
							continue;
						//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
						//	continue;
						
						// ������ ���� ���
						WORD DamageRate = (WORD)(((double)obj->dwData / m_TotalDamage) * 100);

						// ����ġ �̻� �������� �־��ٸ�
						if( DamageRate >= pDropItemList->m_ItemList[ index ].m_wDamageRate )
						{
							// ������ ������ŭ ��� ���̺� ����Ѵ�
							for(DWORD rate = 0; rate < DamageRate; rate++)
							{
								TargetPlayerTable[ TargetPos++ ] = pReceivePlayer;
							}
						}
					}

					// ��� ���̺� ��ϵ� ������ �ִٸ�
					if( TargetPos > 0 )
					{
						// �������� ���̺� ��ġ�� �����Ѵ�
						WORD TargetIndex = rand() % TargetPos;

						// ������ ��ġ�� ��ϵ� ������ �����´�
						CPlayer* TargetPlayer = TargetPlayerTable[ TargetIndex ];

						// ���� ������ ��ȿ�ϸ� �������� �־��ش�
						if( TargetPlayer )
							ITEMMGR->MonsterObtainItem( TargetPlayer, 
														pDropItemList->m_ItemList[ index ].m_wItemIndex[ select ],
														MonsterKind,
														pDropItemList->m_ItemList[ index ].m_wItemCount[ select ]);
					}
				}
			}
		}
	}

	
}

void CDistributer::DistributePerDamage(CMonster* pMob)
{	
	if( pMob )															// ���� ������ ��ȿ���� üũ.
	{
		DAMAGEOBJ *obj = NULL;											
																		
		CPlayer* pReceivePlayer = NULL;;								
							
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//DWORD damagerate, Exp, Money, Abil;
		DWORD damagerate, Money, Abil;
		DWORDEX Exp = 0 ;

		// 080530 LYW --- Distributer : Modified data type of MaxExp to DWRODEX.
		//DWORD MaxExp, MaxAbil, MaxMoney;
		DWORD MaxAbil, MaxMoney ;
		DWORDEX MaxExp = 0 ;

																		
		WORD MonsterKind;												
																		
		damagerate = Abil = Money = 0;							
		MaxAbil = MaxMoney = 0;								

		MonsterKind = pMob->GetMonsterKind();							
		BOSSREWARDSMGR->GetBossRewardsInfo(MonsterKind, MaxExp, MaxAbil, MaxMoney);	

		m_DamageObjectTableSolo.SetPositionHead();
		while(obj = m_DamageObjectTableSolo.GetData())
		{
			if(obj->dwData == 0)
				continue;		
			pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
			if(!pReceivePlayer)	
				continue;
			if(pReceivePlayer->GetInitState() != PLAYERINITSTATE_INITED)
				continue;
			if(pReceivePlayer->GetGridID() != pMob->GetGridID())
				continue;
			//��׵� �ش�.
		/*	if(pReceivePlayer->IsShowdowning() == TRUE)
				continue;		
			if(pPlayer->GetState() == eObjectState_Die)
				return FALSE; */
					
			Exp = (DWORD)(((double)obj->dwData * MaxExp) / m_TotalDamage);
			Abil = (DWORD)(((double)obj->dwData * MaxAbil) / m_TotalDamage);
			Money = (DWORD)(((double)obj->dwData * MaxMoney) / m_TotalDamage);

			// 080530 LYW --- Distributer : Modified data type of exp to DWORDEX ;
			//// 071221 KTH --- Add
			//Exp = (DWORD)(Exp*gEventRate[eEvent_ExpRate]);
			//Exp += EXPTYPE(Exp * (pReceivePlayer->GetRateBuffStatus()->GetExp/100));

			float fBuffExp = 0.0f ;
			fBuffExp = pReceivePlayer->GetRateBuffStatus()->GetExp ;

			Exp = (DWORDEX)(Exp*gEventRate[eEvent_ExpRate]) ;
			Exp += EXPTYPE(Exp*(fBuffExp/100)) ;

			// �ʵ庸�� - 05.12 �̿���
			// �ʵ庸���� ����ġ ����ġ �� ��
			if(pMob->GetObjectKind() == eObjectKind_FieldBossMonster)
			{
				// ���Ϳ� +6 �̻� Ȥ�� -9 ������ Player�� �й迡�� ����
				if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
					continue;
				//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
				//	continue;

				if(Exp == 0) Exp = 1;
				if(Abil == 0) Abil = 1;
				if(Money == 0) Money = 1;

				pReceivePlayer->AddPlayerExpPoint(Exp);
				/*�����Ƽ ���� - pReceivePlayer->AddAbilityExp(Abil);*/
				DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
			}
/*			else if(pMob->GetMonsterKind() == eBOSSKIND_EMPERIORTOMB_EVENT)
			{
				if(Exp == 0) Exp = 1;
				if(Abil == 0) Abil = 1;
				if(Money == 0) Money = 1;

				pReceivePlayer->AddPlayerExpPoint(Exp);
				/*�����Ƽ ���� - pReceivePlayer->AddAbilityExp(Abil);*//*
				DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
			}*/
			else
			{
				if(Exp >= 10000)
					pReceivePlayer->AddPlayerExpPoint(Exp);
				/*�����Ƽ ���� - if(Abil >= 50000)
					pReceivePlayer->AddAbilityExp(Abil);*/
				if(Money >= 100000)
					DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
			}
		//	DISTRIBUTERAND->SendExpAbilMoney(pReceivePlayer, Exp, Abil, Money);
		}
	}
}

void CDistributer::SetPlusTotalDamage(DWORD Damage)
{
	m_TotalDamage += Damage;																	// ������ �հ迡 �־��� �������� �߰�.
}

void CDistributer::DamageInit()
{
	m_1stPartyID = 0;																			// ù �������� �� ��Ƽ ���̵� �ʱ�ȭ.
	m_1stPlayerID = 0;																			// ù �������� �� �÷��̾� ���̵� �ʱ�ȭ.
	m_PlusDamage = 0;																			// �߰� ������ �ʱ�ȭ.		
	m_TotalDamage = 0;																			// ������ �հ� �ʱ�ȭ.
	m_FirstDamage = 0 ;																			// ù ������ �ʱ�ȭ.
	m_nTotalKillerCount = 0 ;																	// ų�� �� �ʱ�ȭ.
	m_HighLevel = 0 ;																			// ų�� �� �� ���� ������ ���� ų���� ����.

	m_DamageObjectTableSolo.RemoveAll();														// ������ ������Ʈ ���̺��� ��� ����.	

	m_pKiller			= NULL ;																// ų�� ������ ��� ������ �ʱ�ȭ.
	m_pKillerParty		= NULL ;																// ų���� ��Ƽ ������ ��� ������ �ʱ�ȭ.

	m_pMob				= NULL ;																// ���� ������ ��� ������ �ʱ�ȭ.
	m_pKilledPosition	= NULL ;																// ���͸� ���� ��ġ ������ �ʱ�ȭ.

	m_dwDropItemID		= 0 ;																	// ��� ������ ���̵� ��� ���� �ʱ�ȭ.
	m_dwDropItemRatio	= 0 ;																	// ������� ��� ���� �ʱ�ȭ.							
}

void CDistributer::DeleteDamagedPlayer(DWORD CharacterID)
{	
	DAMAGEOBJ* pData = NULL ;																	// ������ ������Ʈ ������ ���� �����͸� ���� �� NULL �ʱ�ȭ.

	pData = m_DamageObjectTableSolo.GetData( CharacterID );										// ���� ���̵�� ������ ���̺��� ������ ������Ʈ �����͸� �޴´�.

	if( pData )																					// ������ ������Ʈ �����Ͱ� ��ȿ���� üũ�Ѵ�.
	{
		if( m_TotalDamage >= pData->dwData )													// �������� �������� ������ �հ躸�� ũ�ų� ������,
		{
			m_TotalDamage -= pData->dwData;														// ������ �հ迡�� �������� ������ ��ŭ ����.
		}
		else																					// �������� �������� ������ �հ躸�� ������
		{
			m_TotalDamage = 0;																	// �հ� �������� 0���� �����Ѵ�.
		}
	} 

	m_DamageObjectTableSolo.Remove(CharacterID);												// ������ ������Ʈ ���̺��� �־��� ���̵��� ������ ������Ʈ�� ����.
}

BOOL CDistributer::SetInfoToDistribute(DWORD dwKillerID, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
{
	if( !pKilledPosition || !pMob ) return FALSE ;												// �Լ� ���ڷ� �Ѿ�� �������� ��ȿ���� üũ�Ѵ�.

	CObject* pKiller = g_pUserTable->FindUser(dwKillerID);

	if( pKiller->GetObjectKind() == eObjectKind_Player )
	{
	m_pKiller = (CPlayer *)pKiller;									// ų���� ������ �޴´�.
	}
	else if( pKiller->GetObjectKind() == eObjectKind_Pet )
	{
		m_pKiller = ((CPet *)pKiller)->GetMaster();									// ų���� ������ �޴´�.
	}

	if( m_pKiller )																				// ų���� ������ ��ȿ���� üũ�Ѵ�.
	{
		m_pKillerParty = PARTYMGR->GetParty(m_pKiller->GetPartyIdx());							// ų���� ��Ƽ ������ �޴´�.
	}

	m_pMob				= pMob ;																// ���� ������ ��� �����͸� �޴´�.
	m_pKilledPosition	= pKilledPosition ;														// ���͸� ���� ��ġ �����͸� �޴´�.

	m_dwDropItemID		= DropItemID ;															// ��� ������ ���̵� ��� ������ �޴´�.
	m_dwDropItemRatio	= DropItemRatio ;														// ������� ��� ������ �޴´�.									

	return TRUE ;
}		

void CDistributer::Distribute()
{
	DAMAGEOBJ *obj = NULL ;																		// ������ ������Ʈ.

	int nKillerCount = 0 ;																		// ���͸� óġ �� �÷��̾� ��.

	GetHighLevelOfKillers() ;																	// ų���� �� �ְ����� ���Ѵ�.

	GetTotalKillerCount() ;																		// ������ �� ���� ���Ѵ�.

	if( m_nTotalKillerCount > 1 )																// ���͸� óġ �� �÷��̾ �ټ����, 
	{
		DistributeToKillers() ;																	// �ټ� ó���� �Ѵ�.
	}
	else																						// ���͸� óġ �� �÷��̾ �Ѹ��̶��, 
	{
		DistributeToKiller() ;																	// ���� ó���� �Ѵ�.
	}

	Release();																					// ������ ���̺� �� ��� ���� ������ �ʱ�ȭ �Ѵ�.
}

void CDistributer::DistributeToKillers()
{
	if(!GetFirstDamange())																		// �������� �������� ���Ѵ�.
	{
		if( m_pMob->GetObjectKind() == eObjectKind_BossMonster )
		{
			SYSTEMTIME time;
			char szFile[256] = {0,};
			GetLocalTime( &time );

			sprintf(szFile, "./Log/BossMonsterAssertLog_%02d_%04d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
			FILE* fp;
			fp = fopen(szFile, "a+");
			if (fp)
			{
				fprintf(fp, "BossMonster Not FirstDamage Return\n");
				fclose(fp);
			}
		}
		// 080801 LYW --- Distributer : ù �������� �� ����� ���, 
		// �ٸ� ������� �й踦 ���� �� �ֵ��� return ó���� �ּ� ó���Ѵ�.
		//return;
	}

	MONEYTYPE money = 0 ;																		// ȹ���� ���� �Ӵϸ� ���Ѵ�.

	int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;									// ���Ϳ��� ���� ���̿� ���� �г�Ƽ ���� �޴´�.

	money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(										// �г�Ƽ�� ������ �Ӵϸ� �޴´�.
	(MONSTEREX_LIST *)m_pMob->GetSMonsterList(), m_pMob->GetMonsterKind(), nPenaltyType);

	if( m_pMob->GetObjectKind() == eObjectKind_BossMonster )
	{
		SYSTEMTIME time;
		char szFile[256] = {0,};
		GetLocalTime( &time );

		sprintf(szFile, "./Log/BossMonsterLog_%02d_%04d%02d%02d_%d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay, m_pMob->GetMonsterKind() );
		FILE* fp;
		fp = fopen(szFile, "a+");
		if (fp)
		{
			fprintf(fp, "Time : [%2d:%2d], HighLevel : %d, Drop Money : %u, nPenaltyType : %d\n", time.wHour, time.wMinute, m_HighLevel, money, nPenaltyType);

			m_DamageObjectTableSolo.SetPositionHead() ;

			DAMAGEOBJ* obj = NULL;
			while(obj = m_DamageObjectTableSolo.GetData())
			{
				CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;
				if( !pPlayer || !IsSameChannel(pPlayer, m_pMob) ) continue;
				fprintf(fp, "PlayerID : %u,		Damage : %u,	Level : %u,	CharacterName : %s\n", obj->dwID, obj->dwData, pPlayer->GetLevel(), ((CObject*)pPlayer)->GetObjectName());
			}
			m_DamageObjectTableParty.SetPositionHead() ;
			while(obj = m_DamageObjectTableParty.GetData())
			{
				CParty* pParty = PARTYMGR->GetParty(obj->dwID) ;
				if( !pParty ) continue;

				fprintf(fp, "\n[PartyIdx : %u][PartyDamage : %u]\n", obj->dwID, obj->dwData);

				for(int i=0; i<MAX_PARTY_LISTNUM; i++)
				{
					DWORD partyMemberIdx = 0;
					partyMemberIdx = pParty->GetMemberID(i);
					CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(partyMemberIdx) ;
					if( !pPlayer || !IsSameChannel(pPlayer, m_pMob) ) continue;

					if( partyMemberIdx )
					{
						fprintf(fp, "[PartyMemberIdx : %d]\n", partyMemberIdx);
					}
				}
			}

			fclose(fp);
		}
	}

	DistributerToSolo(money) ;																	// ���� ����ġ�� �Ӵ� ó���� �Ѵ�.

	DistributeToPartys(money) ;																	// ��Ƽ ����ġ�� �Ӵ� ó���� �Ѵ�.

	DistributeItemToKillers() ;																	// ������ ó���� �Ѵ�.
}

void CDistributer::DistributerToSolo(MONEYTYPE money)
{
	DAMAGEOBJ* obj = NULL ;																		// ������ ������ ���� ������ ����.

	CPlayer* pPlayer = NULL ;																	// �÷��̾� ������ ���� ������ ����.

	CParty* pParty = NULL ;																		// ��Ƽ ������ ���� ������ ����.

	m_DamageObjectTableSolo.SetPositionHead() ;													// ������ ���̺��� �ص�� �����Ѵ�.

	while(obj = m_DamageObjectTableSolo.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// �������� �� �÷��̾� ������ �޴´�.

				if( pPlayer )																	// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
				{
					// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
					if( !IsSameChannel(pPlayer, m_pMob) ) continue ;

					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// �÷��̾��� ��Ƽ ������ �޴´�.

					if( !pParty )																// ��Ƽ�� ���ٸ�, 
					{
						if( pPlayer->GetID() == m_1stPlayerID )									// ù �������� �� �÷��̾���, 
						{
							DistributeToFirstKiller(pPlayer, obj->dwData) ;						// �� ������ ����ġ ó���� �Ѵ�.
						}
						else																	// ù �������� �� �÷��̾ �ƴ϶��,
						{
							DistributeToOtherKiller(pPlayer, obj->dwData) ;						// �� ������ ����ġ ó���� �Ѵ�.
						}

						float fDamageRate = (float)obj->dwData / (float)(m_TotalDamage+(m_FirstDamage*0.3f))  ;		// �� ���������� �÷��̾��� ������ ������� ���Ѵ�.

						float fMoney = money * fDamageRate ;									// ���Ͱ� ����� �ӴϿ��� ������ ������ �Ӵϸ� �޴´�.

						MONEYTYPE dwMoney = (DWORD)fMoney ;										// �Ҽ� �������� �Ӵ� ����.
						SendMoneyToPerson(pPlayer, dwMoney) ;									// �Ӵϸ� �����Ѵ�.
					}
				}
			}
		}
	}
}

void CDistributer::DistributeToFirstKiller(CPlayer* pPlayer, DWORD dwMyDamage)
{
	// �� ������ ����ġ ó��.
	// ȹ�� ����ġ[��������] = ((�ڽ��� ������ + (�ڽ��� ������ *0.3)) / ��� ������ ������ ����) * ���� ����ġ.

	if( !pPlayer ) return ;	

	// 080110 LYW --- Distributer : ���� ����ġ ó��, �׾��� �� ó�� ���� �ʵ��� ����.
	if(pPlayer->GetState() == eObjectState_Die) return ;

	// 080108 LYW --- Distributer : ���� ����ġ �޾ƿ��� �κ� ����.
	//DWORD dwMonsterExp = CalcObtainExp(m_HighLevel, 1) ;									// ���� ����ġ.
	// 080530 LYW --- Distributer : Modified data type of experience to DWORD.
	//DWORD dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// ���� ����ġ.
	DWORD dwMonsterExp = 0 ;
	dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;

	if( m_TotalDamage < 1 )																	// ��Ż �������� 1���� ������,
	{
		m_TotalDamage = 1 ;																	// ��Ż �������� 1�� �����Ѵ�.
	}

	float fExpVal_A = (float)(dwMyDamage * 0.3f) ;											// �ڽ��� ������ * 0.3
	float fExpVal_B = (float)(dwMyDamage + fExpVal_A) ;										// �ڽ��� ������ + fExpVal_A
	float fExpVal_C = (float)(fExpVal_B / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_B / ��� ������ ������ ����
	float fExpVal_D = (float)(fExpVal_C * dwMonsterExp) ;									// fExpVal_C * ���� ����ġ

	// 080102 LYW --- Distributer : �ַ� �÷��̾� ����ġ �߰� ó��.
	DWORD dwCurExp = (DWORD)floor(fExpVal_D) ;
	DWORD dwExp = dwCurExp + (DWORD)(dwCurExp*0.15f) ;												// �ݿø� �� ����ġ�� �޴´�.

	DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// �÷��̾�� ����ġ�� �����Ѵ�.	
	if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
	{
		if( pPlayer->GetCurPet() )
			pPlayer->GetCurPet()->AddExp();
	}
	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.15
	// ..�йи� �� ����Ʈ ó��
	g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
	// E �йи� �߰� added by hseos 2007.07.15
}

void CDistributer::DistributeToOtherKiller(CPlayer* pPlayer, DWORD dwMyDamage)
{
	// �� ������ ����ġ ó��.
	// ȹ�� ����ġ[�İ�����] = (�ڽ��� ������ - ((�������� ������ * 0.3) / (������ �Ѽ� - 1))) / ��� ������ ������ ����) * ���� ����ġ.

	if( pPlayer )																				// �÷��̾��� ������ ��ȿ���� üũ�Ѵ�.
	{
		// 080108 LYW --- Distributer : ���� ����ġ �޾ƿ��� �κ� ����.
		//DWORD dwMonsterExp = CalcObtainExp(m_HighLevel, 1) ;									// ���� ����ġ.
		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//DWORD dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// ���� ����ġ.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;


		int nKillerCount = 1 ;																	// ų�� ī��Ʈ�� �����Ѵ�.

		if( m_nTotalKillerCount > 1 )															// ų�� ī��Ʈ�� �Ѹ��̻��̸�,
		{
			nKillerCount = m_nTotalKillerCount-1 ;												// ų�� ī��Ʈ�� �ٽ� �����Ѵ�.
		}
		else																					// Ȥ�ó� �� �� �Ѿ��, ų�� ī��Ʈ �� üũ.
		{
			nKillerCount = 1 ;																	// ų�� ī��Ʈ�� 1�� �����Ѵ�.
		}

		if( m_TotalDamage < 1 )																	// ��Ż �������� 1���� �۴ٸ�,
		{
			m_TotalDamage = 1 ;																	// ��Ż �������� 1�� �����Ѵ�.
		}

		float fExpVal_A = (float)(m_FirstDamage * 0.3f) ;										// �������� ������ * 0.3
		float fExpVal_B = (float)(fExpVal_A / (nKillerCount)) ;									// fExpVal_A / (������ �Ѽ� - 1)
		if( fExpVal_B <= 1 ) fExpVal_B = 1 ;
		float fExpVal_C = (float)(dwMyDamage - fExpVal_B) ;										// �ڽ��� ������ - fExpVal_B
		if( fExpVal_C <= 1 ) fExpVal_C = 1 ;
		float fExpVal_D = (float)(fExpVal_C / (m_TotalDamage+fExpVal_A)) ;									// fExpVal_C / ��� ������ ������ ����
		float fExpVal_E = (float)(fExpVal_D * dwMonsterExp) ;									// fExpVal_D * ���� ����ġ.

		// 080102 LYW --- Distributer : �ַ� �÷��̾� ����ġ �߰� ó��.
		DWORD dwCurExp = (DWORD)floor(fExpVal_E) ;
		DWORD dwExp = dwCurExp + (DWORD)(dwCurExp*0.15f) ;											// �ݿø� �� ����ġ�� �޴´�.

		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// �÷��̾�� ����ġ�� �����Ѵ�.
		if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
		{
			if( pPlayer->GetCurPet() )
				pPlayer->GetCurPet()->AddExp();
		}
		// desc_hseos_�йи�01
		// S �йи� �߰� added by hseos 2007.07.15
		// ..�йи� �� ����Ʈ ó��
		g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
		// E �йи� �߰� added by hseos 2007.07.15
	}
}

void CDistributer::DistributeToPartys(MONEYTYPE money)
{
	DAMAGEOBJ* obj = NULL ;																		// ������ ������ ���� ������ ����.
	CParty* pParty = NULL ;																		// ��Ƽ ������ ���� ������ ����.

	m_DamageObjectTableParty.SetPositionHead() ;												// ������ ���̺��� �ص�� �����Ѵ�.

	while(obj = m_DamageObjectTableParty.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// ��Ƽ ������ �޴´�.

				if( pParty )																	// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
				{
					if( pParty->GetPartyIdx() == m_1stPartyID )									// ù �������� �� ��Ƽ���, 
					{
						DistributeToFirstParty(pParty, obj->dwData) ;							// �� ������Ƽ ó���� �Ѵ�.
					}
					else																		// ù �������� �� ��Ƽ�� �ƴ϶��,
					{
						DistributeToOtherParty(pParty, obj->dwData) ;							// �� ������Ƽ ó���� �Ѵ�.
					}

					float fDamageRate = (float)obj->dwData / (float)(m_TotalDamage+(m_FirstDamage*0.3f))  ;			// ��ü �������� ���� ��Ƽ �������� ������� ���Ѵ�.

					float fMoney = money * fDamageRate ;										// ���Ͱ� ����� �ӴϿ��� ������ ������ŭ �Ӵϸ� ���Ѵ�.

					SendMoneyToPartys(fMoney) ;													// ��Ƽ�� �Ӵϸ� �����Ѵ�.
				}
			}
		}
	}
}

void CDistributer::DistributeToFirstParty(CParty* pParty, DWORD dwPartyDamage)
{
	// �� ���� ��Ƽ ����ó��.
	// ȹ�� ����ġ[��������Ƽ] = ((�ڽ��� ��Ƽ ������ + (�ڽ��� ��Ƽ ������ * 0.3)) / ��� ��Ƽ�� ������ ����) * ���� ����ġ.

	if( pParty )																				// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
	{
		LEVELTYPE highLevel = 0 ;																// ��Ƽ ��� �� �ִ� ����.
		LEVELTYPE totalLevel = 0 ;																// ������� ������ ���� ����.
		LEVELTYPE curlevel = 0 ;																// ������� ���� ������ ���� ����.

		DWORD* pMemberID = NULL ;																// ��� ���̵� ���� ������ ����.

		CPlayer* pMember = NULL ;																// ��� ������ ���� ������ ����.

		PTRLISTPOS pos = NULL ;																	// ����Ʈ�� ��ġ ������ ��� ������.

		SaveCandidacy(pParty) ;																	// �ĺ� ������� �޴´�.

		GetPartyLevels(pParty, &highLevel,&totalLevel) ;										// ��Ƽ�� �ְ����� ���� �հԸ� ���Ѵ�.

		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//DWORD dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;				// ��Ƽ�� �ְ����� ���� ����ġ�� ���Ѵ�.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;

		if( m_TotalDamage < 1 )																	// ��Ż �������� 1���� �۴ٸ�,
		{
			m_TotalDamage = 1 ;																	// ��Ż �������� 1�� �ش�.
		}

		float fExpVal_A = (float)(dwPartyDamage * 0.3f) ;										// �ڽ��� ��Ƽ������ * 0.3
		float fExpVal_B = (float)(dwPartyDamage + fExpVal_A) ;									// �ڽ��� ��Ƽ������ + fExpVal_A
		float fExpVal_C = (float)(fExpVal_B / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_B / ��� ��Ƽ�� ������ ����
		float fExpVal_D = (float)(fExpVal_C * dwMonsterExp) ;									// fExpVal_C * ���� ����ġ.			

		int nCandidacyCount = m_CandidacyList.GetCount() ;										// �ĺ��� ���� �޴´�.

		if( nCandidacyCount == 0 ) return ;														// �̷� ���� ����� �ϴµ�, Ȥ�ö� ������ �𸣱� ������ ���� ó��.

		if( nCandidacyCount == 1 )																// �ڽ� �ܿ� ������ ���� ����� ����.
		{
			pos = m_CandidacyList.FindIndex(0) ;												// �ĺ� ����Ʈ�� ù��° ��ġ ������ �޾ƿ´�.

			if( pos )																			// ��ġ ������ ��ȿ���� üũ�Ѵ�.
			{
				pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;								// �ش��ϴ� ��ġ�� ��� ���̵� ������ �޴´�.

				if( pMemberID )																	// ��� ���̵� ������ ��ȿ���� üũ�Ѵ�.
				{
					pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;					// ���� ���̵�� ��� ������ �޴´�.

					if( pMember )																// ��� ������ ��ȿ���� üũ�Ѵ�.
					{
						// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
						if( !IsSameChannel(pMember, m_pMob) ) return ;

						DWORD dwExp = (DWORD)fExpVal_D ;										// ����ġ �Ҽ��� ���ϴ� ������.
						// desc_hseos_�йи�01
						// S �йи� �߰� added by hseos 2007.07.15
						// ..�йи� �� ����Ʈ ó��
						g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
						// E �йи� �߰� added by hseos 2007.07.15
						
						// 080102 LYW --- Distributer : �ַ� �÷��̾� ����ġ �߰� ó��.
						DWORD dwCurExp = dwExp + (DWORD)(dwExp*0.15f) ;
						DISTRIBUTERAND->SendToPersonalExp(pMember,dwCurExp);					// ����ġ�� �����Ѵ�.
						if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
						{
							if( pMember->GetCurPet() )
								pMember->GetCurPet()->AddExp();
						}
					}
				}
			}
		}
		else																					// ������ ���� �ٸ� �÷��̾ ������, 																	
		{

			BOOL bLevelPenalty = FALSE ;

			LEVELTYPE memberLevel ;
			LEVELTYPE lowLevel = 150 ;

			float fTotalLevel = 0 ;

			for(int count = 0 ; count < nCandidacyCount ; ++count)							// �ĺ� ����ŭ ������ ������.
			{	
				pos = m_CandidacyList.FindIndex(count) ;									// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.

				if( pos )																	// ��ġ������ ��ȿ���� üũ�Ѵ�.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// ��ġ ������ �ش��ϴ� ��� ���̵� �޴´�.

					if( pMemberID )															// ��� ���̵� ��ȿ���� üũ�Ѵ�.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// ��� ���̵�� ��� ������ �޴´�.

						if( pMember )														// ��� ������ ��ȿ���� üũ�Ѵ�.
						{
							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;

							fTotalLevel += (memberLevel*memberLevel) ;

							if( memberLevel > highLevel ) highLevel = memberLevel ;

							if( memberLevel < lowLevel ) lowLevel = memberLevel ;
						}
					}
				}

				pMember = NULL ;															// ��� ������ ��� ������ NULL ó��.
			}

			if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;

			GetPartyLevels(pParty, &highLevel,&totalLevel) ;

			float applyRate = 0.0f ;															// ��Ƽ ���ʽ� ����ġ ����Ʈ ���� ����.

			switch( nCandidacyCount )															// ����ġ ���� �÷��̾� �� üũ.
			{
			//case 2 : applyRate = 0.3f ; break ;													// �θ��̸� +30%
			//case 3 : applyRate = 0.4f ; break ;													// �����̸� +40%
			//case 4 : applyRate = 0.5f ; break ;													// �׸��̸� +50%
			//case 5 : applyRate = 0.55f ; break ;												// �ټ����̸� +55%
			//case 6 : applyRate = 0.6f ; break ;													// �������̸� +60%
			//case 7 : applyRate = 0.7f ; break ;													// �ϰ����̸� +70%					

			// 071206 LYW --- Distributer : ��Ƽ ����ġ ����.
			case 2 : applyRate = 0.39f ;	break ;											// �θ��̸� +35%
			case 3 : applyRate = 0.63f ;	break ;											// �����̸� +60%
			case 4 : applyRate = 0.86f ;	break ;											// �׸��̸� +86%
			case 5 : applyRate = 1.07f ;	break ;											// �ټ����̸� +107%
			case 6 : applyRate = 1.31f ;	break ;											// �������̸� +131%
			case 7 : applyRate = 1.62f ;	break ;											// �ϰ����̸� +162%
			}

			if( bLevelPenalty )
			{
				applyRate = 0.0f ;
			}

			for( int count = 0 ; count < nCandidacyCount ; ++count )							// �ĺ� �� ��ŭ ������ ����.
			{
				pos = m_CandidacyList.FindIndex(count) ;										// ī��Ʈ�� �ش��ϴ� ��ġ ������ �޴´�.
				
				if( pos )																		// ��ġ ������ ��ȿ���� üũ�Ѵ�.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;							// �ش��ϴ� ��ġ�� ���̵� ������ �޴´�.

					if( pMemberID )																// ���̵� ������ ��ȿ���� üũ�Ѵ�.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;				// ���̵�� ��� ������ �޴´�.

						if( pMember )															// ��� ������ ��ȿ���� üũ�Ѵ�.
						{
							// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
							if( !IsSameChannel(pMember, m_pMob) ) continue ;

							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;
							curlevel = (memberLevel * memberLevel);								// ����� ������ �޴´�.

							float fExpA = (float)curlevel / fTotalLevel ;							// ����ġ ����ƮA�� ���Ѵ�.
							float fExpB = (fExpVal_D * ( 1.f + applyRate )) ;						// ����ġ ����ƮB�� ���Ѵ�.
							
							float fExp = fExpA * fExpB ;										// ����ġ�� ���Ѵ�.

							if(fExp > 0)														// ����ġ üũ�� �Ѵ�.
							{
								DWORD dwExp = (DWORD)fExp ;						// ����ġ �Ҽ��� ���ϴ� ������.
								// desc_hseos_�йи�01
								// S �йи� �߰� added by hseos 2007.07.15
								// ..�йи� �� ����Ʈ ó��
								g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
								// E �йи� �߰� added by hseos 2007.07.15
								if( (highLevel - memberLevel) >= 20 )
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// ����ġ�� �����Ѵ�.
								}
								else
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);				// ����ġ�� �����Ѵ�.							
									if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
									{
										if( pMember->GetCurPet() )
											pMember->GetCurPet()->AddExp();
									}
								}							}
						}

						pMember = NULL ;														// ��� ���� ������ ������ NULLó��.
					}
				}
			}
		}

	}
}

void CDistributer::DistributeToOtherParty(CParty* pParty, DWORD dwPartyDamage)
{
	// �� ���� ��Ƽ ����ġ ó��.
	// ȹ�� ����ġ[�İ�����Ƽ] = ((�ڽ��� ��Ƽ ������ - ((������ ��Ƽ ������ * 0.3) / (��Ƽ �� �� - 1))) / ��� ��Ƽ�� ������ ���� ) + ���� ����ġ.

	if( pParty )
	{
		LEVELTYPE highLevel = 0 ;																// ��Ƽ ��� �� �ִ� ����.
		LEVELTYPE totalLevel = 0 ;																// ������� ������ ���� ����.
		LEVELTYPE curlevel = 0 ;																// ������� ���� ������ ���� ����.

		DWORD* pMemberID = NULL ;																// ��� ���̵� ������ ���� ������ ����.

		CPlayer* pMember = NULL ;																// ��� ������ ���� ������ ����.

		PTRLISTPOS pos = NULL ;																	// �ĺ� ����Ʈ�� ��ġ ���� ������ ����.

		SaveCandidacy(pParty) ;																	// �ĺ� ������� �޴´�.

		GetPartyLevels(pParty,&highLevel,&totalLevel) ;											// ��Ƽ�� �ְ� ����, ���� ���� ���Ѵ�.

		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD ;
		//DWORD dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;				// �ְ����� ���� ���� ����ġ�� ���Ѵ�.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;


		int nCandidacyCount = m_CandidacyList.GetCount() ;										// ����Ʈ�� ��� �ĺ��� ���� ���Ѵ�.

		int nKillerCount = 1 ;																	// ų�� ī��Ʈ�� �����Ѵ�.

		if( m_nTotalKillerCount > 1 )															// ų�� ī��Ʈ�� �Ѹ� �̻��̸�,
		{
			nKillerCount = m_TotalDamage-1 ;													// ų�� ī��Ʈ�� �ٽ� �����Ѵ�.
		}
		else																					// Ȥ�� �߻��� �� �𸣴� ��Ȳ, ų�� ���� ���ٸ�,
		{
			nKillerCount = 1 ;																	// ų�� ���� 1�� �����Ѵ�.
		}

		if( m_TotalDamage < 1 )																	// ��Ż �������� 1���� �۴ٸ�,
		{
			m_TotalDamage = 1 ;																	// ��Ż �������� 1�� �����Ѵ�.
		}

		float fExpVal_A = (float)(m_FirstDamage * 0.3f) ;										// ������ ��Ƽ ������ * 0.3

		float fExpVal_B = (float)(fExpVal_A / (nKillerCount)) ;									// fExpVal_A / (��Ƽ �� �� - 1)

		if( fExpVal_B <= 1 ) fExpVal_B = 1 ;													// ���� üũ�� �Ѵ�.

		float fExpVal_C = (float)(dwPartyDamage - fExpVal_B) ;									// �ڽ��� ��Ƽ ������ - fExpVal_B

		if( fExpVal_C <= 1 ) fExpVal_C = 1 ;													// ���� üũ���Ѵ�.

		float fExpVal_D = (float)(fExpVal_C / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_C / ��� ��Ƽ�� ������ ����

		float fExpVal_E = (float)(fExpVal_D * dwMonsterExp) ;									// fExpVal_D * ���� ����ġ.				

		if( nCandidacyCount == 0 ) return ;														// �̷� ���� ����� �ϴµ�, Ȥ�ö� ������ �𸣱� ������ ���� ó��.

		if( nCandidacyCount == 1 )																// �ڽ� �ܿ� ������ ���� ����� ����.
		{
			pos = m_CandidacyList.FindIndex(0) ;												// ����Ʈ�� ù��° ��ġ ������ �޴´�.

			if( pos )																			// ��ġ������ ��ȿ���� �˻��Ѵ�.
			{
				pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;								// ��ġ�� �ش��ϴ� ��� ���̵� ������ �޴´�.

				if( pMemberID )																	// ��� ���̵� ������ ��ȿ���� üũ�Ѵ�.
				{
					pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;					// ��� ���̵�� ��� ������ �޴´�.

					if( pMember )																// ��� ������ ��ȿ���� üũ�Ѵ�.
					{
						// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
						if( !IsSameChannel(pMember, m_pMob) ) return ;

						DWORD dwExp = (DWORD)fExpVal_E ;										// ����ġ �Ҽ��� ���ϴ� ������.
						// desc_hseos_�йи�01
						// S �йи� �߰� added by hseos 2007.07.15
						// ..�йи� �� ����Ʈ ó��
						g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
						// E �йи� �߰� added by hseos 2007.07.15
						// 080102 LYW --- Distributer : �ַ� �÷��̾� ����ġ �߰� ó��.
						DWORD dwCurExp = dwExp + (DWORD)(dwExp*0.15f) ;
						DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);						// ����ġ�� �����Ѵ�.
						if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
						{
							if( pMember->GetCurPet() )
								pMember->GetCurPet()->AddExp();
						}
					}
				}
			}
		}
		else																					// ������ ���� �ٸ� �÷��̾ ������, 																	
		{
			BOOL bLevelPenalty = FALSE ;

			LEVELTYPE memberLevel ;
			LEVELTYPE lowLevel = 150 ;

			float fTotalLevel = 0 ;

			for(int count = 0 ; count < nCandidacyCount ; ++count)							// �ĺ� ����ŭ ������ ������.
			{	
				pos = m_CandidacyList.FindIndex(count) ;									// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.

				if( pos )																	// ��ġ������ ��ȿ���� üũ�Ѵ�.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// ��ġ ������ �ش��ϴ� ��� ���̵� �޴´�.

					if( pMemberID )															// ��� ���̵� ��ȿ���� üũ�Ѵ�.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// ��� ���̵�� ��� ������ �޴´�.

						if( pMember )														// ��� ������ ��ȿ���� üũ�Ѵ�.
						{
							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;

							fTotalLevel += (memberLevel*memberLevel) ;

							if( memberLevel > highLevel ) highLevel = memberLevel ;

							if( memberLevel < lowLevel ) lowLevel = memberLevel ;
						}
					}
				}

				pMember = NULL ;															// ��� ������ ��� ������ NULL ó��.
			}

			if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;

			float applyRate = 0.0f ;															// ��Ƽ ���ʽ� ����ġ ����Ʈ.

			switch( nCandidacyCount )															// ����ġ ���� �÷��̾� �� üũ.
			{
			//case 2 : applyRate = 0.3f ; break ;													// �θ��̸� +30%
			//case 3 : applyRate = 0.4f ; break ;													// �����̸� +40%
			//case 4 : applyRate = 0.5f ; break ;													// �׸��̸� +50%
			//case 5 : applyRate = 0.55f ; break ;												// �ټ����̸� +55%
			//case 6 : applyRate = 0.6f ; break ;													// �������̸� +60%
			//case 7 : applyRate = 0.7f ; break ;													// �ϰ����̸� +70%						

			// 071206 LYW --- Distributer : ��Ƽ ����ġ ����.
			case 2 : applyRate = 0.39f ;	break ;											// �θ��̸� +35%
			case 3 : applyRate = 0.63f ;	break ;											// �����̸� +60%
			case 4 : applyRate = 0.86f ;	break ;											// �׸��̸� +86%
			case 5 : applyRate = 1.07f ;	break ;											// �ټ����̸� +107%
			case 6 : applyRate = 1.31f ;	break ;											// �������̸� +131%
			case 7 : applyRate = 1.62f ;	break ;											// �ϰ����̸� +162%
			}

			if( bLevelPenalty )
			{
				applyRate = 0.0f ;
			}

			for( int count = 0 ; count < nCandidacyCount ; ++count )							// �ĺ��� ����ŭ ������ ����.
			{
				pos = m_CandidacyList.FindIndex(count) ;										// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.
				
				if( pos )																		// ��ġ ������ ��ȿ���� üũ�Ѵ�.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;							// ��ġ�� �ش��ϴ� ��� ���̵� �޴´�.

					if( pMemberID )																// ���̵� ������ ��ȿ���� üũ�Ѵ�.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;				// ���̵�� ��� ������ �޴´�.

						if( pMember )															// ��� ������ ��ȿ���� üũ�Ѵ�.
						{
							// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
							if( !IsSameChannel(pMember, m_pMob) ) continue ;

							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;
							curlevel = (memberLevel * memberLevel);								// ����� ������ �޴´�.

							float fExpA = (float)curlevel / fTotalLevel ;						// ����ġ ����ƮA�� ���Ѵ�.
							float fExpB = (fExpVal_E * ( 1.f + applyRate )) ;						// ����ġ ����ƮB�� ���Ѵ�.
							
							float fExp = fExpA * fExpB ;										// ����ġ�� ���Ѵ�.

							if(fExp > 0)														// ����ġ üũ�� �Ѵ�.
							{
								DWORD dwExp = (DWORD)fExp ;						// ����ġ �Ҽ��� ���ϴ� ������.
								// desc_hseos_�йи�01
								// S �йи� �߰� added by hseos 2007.07.15
								// ..�йи� �� ����Ʈ ó��
								g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
								// E �йи� �߰� added by hseos 2007.07.15
								if( (highLevel - memberLevel) >= 20 )
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// ����ġ�� �����Ѵ�.
								}
								else
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);				// ����ġ�� �����Ѵ�.				
									if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
									{
										if( pMember->GetCurPet() )
											pMember->GetCurPet()->AddExp();
									}
								}
							}
						}

						pMember = NULL ;														// ��� ���� �����͸� NULL ó�� �Ѵ�.
					}
				}
			}
		}
	}
}

void CDistributer::SendMoneyToPartys(float ChangeValue)
{
	int nCandidacyCount = m_CandidacyList.GetCount() ;											// �ĺ��� ���� �޴´�.

	if( nCandidacyCount <= 0 ) return ;															// Ȥ�ó� �߻��� ���ܻ�Ȳ.

	DWORD* pMemberID = NULL ;																	// ��� ���̵� ������ ���� ������ ����.

	CPlayer* pMember = NULL ;																	// ��� ������ ���� ������ ����.

	PTRLISTPOS pos = NULL ;																		// ����Ʈ������ ��ġ ������ ���� ������ ����.

	m_CandidacyList.GetHeadPosition() ;															// �ĺ��� ���� ����Ʈ�� �ص�� ����.

	float fMoney = ChangeValue / (float)nCandidacyCount ;										// �Է¹��� �Ӵϸ� �ĺ��� ���� ������.

	MONEYTYPE dwMoney = (DWORD)fMoney ;															// �Ҽ� ���ϴ� ������.

	for( int count = 0 ; count < nCandidacyCount ; ++count )									// �ĺ��� ����ŭ ������ ������.
	{
		pos = m_CandidacyList.FindIndex(count) ;												// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.

		if( pos )																				// ��ġ������ ��ȿ���� üũ�Ѵ�.
		{
			pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// ��ġ ������ �ش��ϴ� ��� ���̵� �޴´�.

			if( pMemberID )																		// ��� ���̵� ��ȿ���� üũ�Ѵ�.
			{
				pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;						// ��� ���̵�� ��� ������ �޴´�.

				if( pMember )																	// ��� ������ ��ȿ���� üũ�Ѵ�.
				{
					if( m_pMob )																// ���� ������ ��ȿ���� üũ�Ѵ�.
					{
						// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
						if( !IsSameChannel(pMember, m_pMob) ) continue ;

						if( pMember->SetMoney(dwMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory,	// �� ������� �Ӵϸ� �����Ѵ�.
							eMoneyLog_GetMonster, m_pMob->GetMonsterKind()) != dwMoney )
						{
							// error msg ������. ���ѷ� �ʰ�
							MSGBASE msg;
							msg.Category = MP_ITEM;
							msg.Protocol = MP_ITEM_MONEY_ERROR;
							msg.dwObjectID = pMember->GetID();
							
							pMember->SendMsg(&msg, sizeof(msg));
						}
					}
				}
			}
		}
	}
}





//=========================================================================
//	NAME : DistributeItemToKillers
//	DESC : The function to distribute item to killers. 080114 LYW
//=========================================================================
void CDistributer::DistributeItemToKillers() 
{
	DAMAGEOBJ*	obj		= NULL ;																// ������ ������ ���� ������ ����.
	CPlayer*	pPlayer = NULL ;
	CParty*		pParty  = NULL ;

	BOOL bNoPlayer = FALSE ;
	BOOL bNoParty  = FALSE ;

	// ���� �÷��̾� �� ������ ȹ�� �ĺ��� �̴´�.

	DWORD dwBigPlayerDamage = 0 ;																// ���� ū ���� �������� ���� ����.
	DWORD dwBigPlayerID = 0 ;																	// ���� ū �������� �� �÷��̾� ���̵� ���� ����.

	m_DamageObjectTableSolo.SetPositionHead() ;													// ������ ���̺��� �ص�� �����Ѵ�.

	while(obj = m_DamageObjectTableSolo.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( !obj ) continue ;																	// ������ ������ ��ȿ���� üũ�Ѵ�.

		pPlayer = NULL ;
		pPlayer = (CPlayer*)g_pUserTable->FindUser(obj->dwID) ;									// �������� ���� ���̵�� �÷��̾� ������ �޴´�.

		if( !pPlayer ) continue ;																// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.

		if( pPlayer->GetState() == eObjectState_Die ) continue ;								// �÷��̾ �׾��ٸ�, ��󿡼� ���� ��Ų��.

		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;									// �÷��̾��� ��Ƽ ���̵�� ��Ƽ ������ �޴´�.

		if( pParty ) continue ;																	// ��Ƽ ������ ��ȿ���� üũ, ��Ƽ�� ������, 

		ChooseOne(obj, &dwBigPlayerDamage, &dwBigPlayerID) ;									// ��Ƽ�� ���� �ַ� �÷��̾� �� ���� ū �������� ���̵� �޴´�.
	}

	if( dwBigPlayerDamage == 0 ) bNoPlayer = TRUE ;												// ������ ��Ư �ĺ��� ���� �÷��̾ ����.

	// ��Ƽ �� ������ ȹ�� ��Ƽ�� �̴´�.

	DWORD dwBigPartyDamage = 0 ;																// ���� ū ��Ƽ �������� ���� ����.
	DWORD dwBigPartyID = 0 ;																	// ���� ū ��Ƽ �׹����� �� ��Ÿ ���̵� ���� ����.

	m_DamageObjectTableParty.SetPositionHead() ;												// ������ ���̺��� �ص�� �����Ѵ�.

	while(obj = m_DamageObjectTableParty.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( !obj ) continue ;																	// ������ ������ �޴´�.

		pParty = NULL ;
		pParty = PARTYMGR->GetParty(obj->dwID) ;

		if( !pParty ) continue ;

		SaveCandidacy( pParty ) ;

		if( m_CandidacyList.GetCount() == 0 ) continue ;
	
		ChooseOne(obj, &dwBigPartyDamage, &dwBigPartyID) ;										// ���� ū ��Ƽ �������� ��Ƽ�� �޴´�.
	}

	if( dwBigPartyDamage == 0 ) bNoParty = TRUE ;

	if( bNoPlayer && bNoParty ) return ;														// ��Ƽ�� ���� ��� �ĺ��� ������, return ó�� �Ѵ�.

	// 080717 LYW --- Distributer : ������ ���̺� ����, ���� üũ ���� ����.
	// if( bNoPlayer || bNoParty )
	// else
	// �� ������ 
	// if( !bNoPlayer && !bNoParty )
	// else �� ���� ��.

	if( !bNoPlayer && !bNoParty )
	{
		// ��Ƽ�� ������ �������� ���Ѵ�.
		if( dwBigPlayerDamage == dwBigPartyDamage )													// ������ �������� ��Ƽ�� �������� ���ٸ�,
		{
			int nResult = random(0, 100) ;															// ���� ���� ���Ѵ�.

			if( nResult < 50 )																		// ���� ���� 50�̸��̸�, 
			{
				dwBigPlayerDamage = 0 ;																// ��Ƽ���� �������� ������.
				dwBigPartyDamage  = 1 ;
			}
			else																					// ���� ���� 50�� �ʰ��ϸ�,
			{
				dwBigPlayerDamage = 1 ;																// ���ο��� �������� ������.
				dwBigPartyDamage  = 0 ;
			}
		}

		if( dwBigPlayerDamage > dwBigPartyDamage )													// ������ �������� ��Ƽ�� ������ ���� ũ��,
		{
			pPlayer = NULL ;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID) ;								// ���� ū �������� �� �÷��̾� ������ �޴´�.

			if( pPlayer )																			// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
			{
				SendItemToPerson(pPlayer) ;															// ���� ū �������� �� �÷��̾�� �������� �����Ѵ�.
			}
		}
		else if( dwBigPlayerDamage < dwBigPartyDamage )												// ��Ƽ �������� ������ ������ ���� ũ��,
		{
			pParty = NULL ;
			pParty = PARTYMGR->GetParty(dwBigPartyID) ;												// ���� ū �������� �� ��Ƽ ������ �޴´�.

			if( pParty )																			// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
			{
				// 081205 LYW --- Distributer : ��Ƽ ������ �й� ����( ������ ��Ƽ�� ������ ȹ�� ���� ) ����.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())															// ��Ƽ�� ������ �й� ����� �޾ƿ´�.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// ���� �й�� �������� �й��Ѵ�.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// ������ �켱 ������ �������� �й��Ѵ�.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// �����ϰ� ����� ���ؼ� �������� �й��Ѵ�.
				}
			}
		}
	}
	else
	{
		if( bNoPlayer )
		{
			pParty = NULL ;
			pParty = PARTYMGR->GetParty(dwBigPartyID) ;												// ���� ū �������� �� ��Ƽ ������ �޴´�.

			if( pParty )																			// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
			{
				// 081205 LYW --- Distributer : ��Ƽ ������ �й� ����( ������ ��Ƽ�� ������ ȹ�� ���� ) ����.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())															// ��Ƽ�� ������ �й� ����� �޾ƿ´�.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// ���� �й�� �������� �й��Ѵ�.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// ������ �켱 ������ �������� �й��Ѵ�.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// �����ϰ� ����� ���ؼ� �������� �й��Ѵ�.
				}
			}
		}
		else if( bNoParty )
		{
			pPlayer = NULL ;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID) ;								// ���� ū �������� �� �÷��̾� ������ �޴´�.

			if( pPlayer )																			// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
			{
				SendItemToPerson(pPlayer) ;															// ���� ū �������� �� �÷��̾�� �������� �����Ѵ�.
			}
		}
	}	
}


//void CDistributer::DistributeItemToKillers() 
//{
//	DWORD dwBigPlayerDamage = 0 ;																// ���� ū ���� �������� ���� ����.
//	DWORD dwBigPlayerID = 0 ;																	// ���� ū �������� �� �÷��̾� ���̵� ���� ����.
//
//	m_DamageObjectTableSolo.SetPositionHead() ;													// ������ ���̺��� �ص�� �����Ѵ�.
//
//	DAMAGEOBJ* obj = NULL ;																		// ������ ������ ���� ������ ����.
//
//	while(obj = m_DamageObjectTableSolo.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
//	{
//		if( obj )																				// ������ ������ ��ȿ���� üũ�Ѵ�.
//		{
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(obj->dwID);						// �������� ���� ���̵�� �÷��̾� ������ �޴´�.
//
//			if( pPlayer )																		// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
//			{
//				CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;					// �÷��̾��� ��Ƽ ���̵�� ��Ƽ ������ �޴´�.
//
//				if( !pParty )																	// ��Ƽ ������ ��ȿ���� üũ, ��Ƽ�� ������, 
//				{
//					ChooseOne(obj, &dwBigPlayerDamage, &dwBigPlayerID);							// ��Ƽ�� ���� �ַ� �÷��̾� �� ���� ū �������� ���̵� �޴´�.
//				}				
//			}
//		}
//	}
//
//	DWORD dwBigPartyDamage = 0 ;																// ���� ū ��Ƽ �������� ���� ����.
//	DWORD dwBigPartyID = 0 ;																	// ���� ū ��Ƽ �׹����� �� ��Ÿ ���̵� ���� ����.
//
//	m_DamageObjectTableParty.SetPositionHead() ;												// ������ ���̺��� �ص�� �����Ѵ�.
//
//	while(obj = m_DamageObjectTableParty.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
//	{
//		if( obj )																				// ������ ������ �޴´�.
//		{
//			ChooseOne(obj, &dwBigPartyDamage, &dwBigPartyID);									// ���� ū ��Ƽ �������� ��Ƽ�� �޴´�.
//		}
//	}
//
//	if( dwBigPlayerDamage == dwBigPartyDamage )													// ������ �������� ��Ƽ�� �������� ���ٸ�,
//	{
//		int nResult = random(0, 100) ;															// ���� ���� ���Ѵ�.
//
//		if( nResult < 50 )																		// ���� ���� 50�̸��̸�, 
//		{
//			dwBigPlayerDamage = 0 ;																// ��Ƽ���� �������� ������.
//			dwBigPartyDamage  = 1 ;
//		}
//		else																					// ���� ���� 50�� �ʰ��ϸ�,
//		{
//			dwBigPlayerDamage = 1 ;																// ���ο��� �������� ������.
//			dwBigPartyDamage  = 0 ;
//		}
//	}
//
//	if( dwBigPlayerDamage > dwBigPartyDamage )													// ������ �������� ��Ƽ�� ������ ���� ũ��,
//	{
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID);						// ���� ū �������� �� �÷��̾� ������ �޴´�.
//
//		if( pPlayer )																			// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			SendItemToPerson(pPlayer) ;															// ���� ū �������� �� �÷��̾�� �������� �����Ѵ�.
//		}
//	}
//	else if( dwBigPlayerDamage < dwBigPartyDamage )												// ��Ƽ �������� ������ ������ ���� ũ��,
//	{
//		CParty* pParty = PARTYMGR->GetParty(dwBigPartyID) ;										// ���� ū �������� �� ��Ƽ ������ �޴´�.
//
//		if( pParty )																			// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
//		{
//			switch(pParty->GetOption())															// ��Ƽ�� ������ �й� ����� �޾ƿ´�.
//			{
//			case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// ���� �й�� �������� �й��Ѵ�.
//			case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// ������ �켱 ������ �������� �й��Ѵ�.
//			case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// �����ϰ� ����� ���ؼ� �������� �й��Ѵ�.
//			}
//		}
//	}
//}

void CDistributer::DistributeToKiller()
{
	if( m_pKiller )																				// ų�� ������ ��ȿ���� üũ�Ѵ�.
	{
		if( !m_pKillerParty )																	// ų���� ��Ƽ�� ���ٸ�,
		{
			DistributeToPerson( m_pKiller ) ;													// ų������ ������, ����ġ, ���� �����Ѵ�.
		}
		else																					// ų���� ��Ƽ�� �����ִٸ�,
		{
			DistributeToParty(m_pKiller) ;														// ų���� ��Ƽ���� ������, ����ġ, ���� �����Ѵ�.
		}
	}
}

void CDistributer::DistributeToPerson(CPlayer* pPlayer)
{
	if( pPlayer && m_pMob )																		// �÷��̾�� ���� ������ ��ȿ���� üũ�Ѵ�.
	{
		// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		// 080110 LYW --- Distributer : ����ġ ó�� �÷��̾� �׾��� �� ó�� ���� �ʵ��� ó��.
		if(pPlayer->GetState() == eObjectState_Die) return ;

		// 080102 LYW --- Distributer : �ַ��� �÷��̾� ����ġ �߰� ó��.
		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//EXPTYPE dwCur = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// ������ ����ġ�� �޾� �´�.
		DWORD dwCur = 0 ;
		dwCur = CalcObtainExp(pPlayer->GetLevel(), 1) ;

		EXPTYPE dwExp = 0 ;
		dwExp = dwCur + (EXPTYPE)(dwCur * 0.15f) ;

		WORD wMonsterKind = m_pMob->GetMonsterKind() ;											// ���� ������ �޴´�.
		
		MONEYTYPE money = 0 ;																	// ȹ���� ���� �Ӵϸ� ���Ѵ�.

		int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;								// ���Ϳ��� ���� ���̿� ���� �г�Ƽ ���� �޴´�.

		money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(									// �г�Ƽ�� ������ �Ӵϸ� �޴´�.
		(MONSTEREX_LIST *)m_pMob->GetSMonsterList(), m_pMob->GetMonsterKind(), nPenaltyType);

		// desc_hseos_�йи�01
		// S �йи� �߰� added by hseos 2007.07.15
		// ..�йи� �� ����Ʈ ó��
		// 080602 LYW --- Player : ����ġ ��ġ (__int32) ���� (__int64) ������� ���� ó��.
		//g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
		g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, (DWORD)dwExp);
		// E �йи� �߰� added by hseos 2007.07.15
		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// �÷��̾�� ����ġ�� �����Ѵ�.
		if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
		{
			if( pPlayer->GetCurPet() )
				pPlayer->GetCurPet()->AddExp();
		}

		SendMoneyToPerson(pPlayer, money);														// �÷��̾�� �Ӵϸ� �����Ѵ�.

		SendItemToPerson(pPlayer);																// �÷��̾�� �������� �����Ѵ�.
	}
}

void CDistributer::DistributeToParty(CPlayer* pPlayer)
{	
	if( !m_pKilledPosition || !m_pMob ) return ;												// ��ġ������ ���� ������ ��ȿ���� üũ�Ѵ�.

	if( pPlayer )																				// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());							// �÷��̾��� ��Ƽ ������ �޴´�.

		if( pParty )																			// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
		{
			SaveCandidacy(pParty) ;																// �ĺ����� ����Ʈ�� ��´�.

			int nCandidadyCount = m_CandidacyList.GetCount() ;									// �ĺ����� ���� �޴´�.

			if( nCandidadyCount == 0 ) return ;													// �̷� ���� ����� �ϴµ�, Ȥ�ö� ������ �𸣱� ������ ���� ó��.

			if( nCandidadyCount == 1 )															// �ڽ� �ܿ� ������ ���� ����� ����.
			{
				DistributeToPerson( pPlayer ) ;													// ������, ����ġ, ���� �����Ѵ�.
				return ;
			}
			else																				// ������ ���� �ٸ� �÷��̾ ������, 																	
			{
				DWORD* pMemberID = NULL ;														// ��� ���̵� ������ ���� ������ ����.

				CPlayer* pMember = NULL ;														// ��� ������ ���� ������ ����.

				PTRLISTPOS pos = NULL ;															// �ĺ� ����Ʈ�� ��ġ ���� ������ ����.

				BOOL bLevelPenalty = FALSE ;

				LEVELTYPE highLevel = 0 ;														// ��Ƽ ��� �� �ִ� ����.
				LEVELTYPE totalLevel = 0 ;														// ������� ������ ���� ����.
				LEVELTYPE curlevel = 0 ;														// ������� ���� ������ ���� ����.

				LEVELTYPE lowLevel = 150 ;
				LEVELTYPE memberLevel ;

				float fTotalLevel = 0 ;

				for(int count = 0 ; count < nCandidadyCount ; ++count)							// �ĺ� ����ŭ ������ ������.
				{	
					pos = m_CandidacyList.FindIndex(count) ;									// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.

					if( pos )																	// ��ġ������ ��ȿ���� üũ�Ѵ�.
					{
						pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// ��ġ ������ �ش��ϴ� ��� ���̵� �޴´�.

						if( pMemberID )															// ��� ���̵� ��ȿ���� üũ�Ѵ�.
						{
							pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// ��� ���̵�� ��� ������ �޴´�.

							if( pMember )														// ��� ������ ��ȿ���� üũ�Ѵ�.
							{
								memberLevel = 0 ;
								memberLevel = pMember->GetLevel() ;

								fTotalLevel += (memberLevel*memberLevel) ;

								if( memberLevel > highLevel ) highLevel = memberLevel ;

								if( memberLevel < lowLevel ) lowLevel = memberLevel ;
							}
						}
					}

					pMember = NULL ;															// ��� ������ ��� ������ NULL ó��.
				}

				if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;
	
				GetPartyLevels(pParty, &highLevel,&totalLevel) ;

				// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
				//EXPTYPE partyexp = CalcObtainExp(highLevel, m_CandidacyList.GetCount());		// ��Ƽ �� �ִ� ������ �������� ���� ����ġ�� �޾� �´�.
				DWORD partyexp = 0 ;
				partyexp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;

				float applyRate = 0.0f ;														// ��Ƽ ���ʽ� ����ġ ����Ʈ.

				switch( nCandidadyCount )														// ����ġ ���� �÷��̾� �� üũ.
				{ 
				//case 2 : applyRate = 0.3f ; break ;												// �θ��̸� +30%
				//case 3 : applyRate = 0.4f ; break ;												// �����̸� +40%
				//case 4 : applyRate = 0.5f ; break ;												// �׸��̸� +50%
				//case 5 : applyRate = 0.55f ; break ;											// �ټ����̸� +55%
				//case 6 : applyRate = 0.6f ; break ;												// �������̸� +60%
				//case 7 : applyRate = 0.7f ; break ;												// �ϰ����̸� +70%

				// 071206 LYW --- Distributer : ��Ƽ ����ġ ����.
				case 2 : applyRate = 0.39f ;	break ;											// �θ��̸� +35%
				case 3 : applyRate = 0.63f ;	break ;											// �����̸� +60%
				case 4 : applyRate = 0.86f ;	break ;											// �׸��̸� +86%
				case 5 : applyRate = 1.07f ;	break ;											// �ټ����̸� +107%
				case 6 : applyRate = 1.31f ;	break ;											// �������̸� +131%
				case 7 : applyRate = 1.62f ;	break ;											// �ϰ����̸� +162%
				}	

				if( bLevelPenalty )
				{
					applyRate = 0.0f ;
				}
			
				MONEYTYPE money = 0 ;															// ȹ���� ���� �Ӵϸ� ���Ѵ�.

				int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;						// ���Ϳ��� ���� ���̿� ���� �г�Ƽ ���� �޴´�.

				money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(							// �г�Ƽ�� ������ �Ӵϸ� �޴´�.
				(MONSTEREX_LIST *)m_pMob->GetSMonsterList(), m_pMob->GetMonsterKind(), nPenaltyType);
								
				MONEYTYPE ObtainMoney = (MONEYTYPE)((float)money/nCandidadyCount) ;				// 1/n�� �� �Ӵϸ� ���Ѵ�.

				for(int count = 0 ; count < nCandidadyCount ; ++count)							// �ĺ� ����ŭ ������ ������.
				{	
					pos = m_CandidacyList.FindIndex(count) ;									// ī��Ʈ�� �ش��ϴ� ��ġ������ �޴´�.

					if( pos )																	// ��ġ������ ��ȿ���� üũ�Ѵ�.
					{
						pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// ��ġ ������ �ش��ϴ� ��� ���̵� �޴´�.

						if( pMemberID )															// ��� ���̵� ��ȿ���� üũ�Ѵ�.
						{
							pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// ��� ���̵�� ��� ������ �޴´�.

							if( pMember )														// ��� ������ ��ȿ���� üũ�Ѵ�.
							{
								// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
								if( !IsSameChannel(pMember, m_pMob) ) continue ;

								memberLevel = 0 ;
								memberLevel = pMember->GetLevel();
								curlevel = (memberLevel*memberLevel) ;							// ����� ������ �޴´�.

								float fExpA = (float)curlevel / fTotalLevel ;					// ����ġ ����ƮA�� ���Ѵ�.
								float fExpB = (partyexp * ( 1.f + applyRate )) ;					// ����ġ ����ƮB�� ���Ѵ�.
								
								float fExp = fExpA * fExpB ;									// ����ġ�� ���Ѵ�.

								if(fExp > 0)													// ����ġ üũ�� �Ѵ�.
								{
									DWORD dwExp = (DWORD)fExp ;									// ����ġ �Ҽ��� ���ϴ� ������.

									if( (highLevel - memberLevel) >= 20 )
									{
										DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// ����ġ�� �����Ѵ�.
									}
									else
									{
										DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);		// ����ġ�� �����Ѵ�.
										if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
										{
											if( pPlayer->GetCurPet() )
												pPlayer->GetCurPet()->AddExp();
										}
									}
						
									// desc_hseos_�йи�01
									// S �йи� �߰� added by hseos 2007.07.15
									// ..�йи� �� ����Ʈ ó��
									g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
									// E �йи� �߰� added by hseos 2007.07.15
									SendMoneyToPerson(pMember, ObtainMoney);					// �Ӵϸ� �����Ѵ�.
								}
							}
						}
					}

					pMember = NULL ;															// ��� ������ ��� ������ NULL ó��.
				}

				// 081205 LYW --- Distributer : ��Ƽ ������ �й� ����( ������ ��Ƽ�� ������ ȹ�� ���� ) ����.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())														// ��Ƽ�� ������ �й� ������ �޴´�.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;					// ���������� �������� �й��Ѵ�.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;					// �������� ���� ������� �������� �й��Ѵ�.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;					// �����ϰ� ����� �����ؼ� �������� �й��Ѵ�.
				}
			}
		}
	}
}

void CDistributer::SendMoneyToPerson(CPlayer* pPlayer, MONEYTYPE ChangeValue)
{
	if( pPlayer && m_pMob )																		// �÷��̾�� ������ ������ ��ȿ���� üũ�Ѵ�.
	{
		// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		// 071228 LYW --- Distributer : �Ӵ� �̺�Ʈ ����.
		MONEYTYPE eventValue = 0 ;
		eventValue = (MONEYTYPE)(gEventRate[eEvent_MoneyRate] * ChangeValue) ;
		//eventValue = (MONEYTYPE)(ChangeValue) ;

		// �Ӵϸ� �÷��̾�� �����Ѵ�.
		if( pPlayer->SetMoney(eventValue, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_GetMonster, m_pMob->GetMonsterKind()) != eventValue )
		{
			// error msg ������. ���ѷ� �ʰ�
			MSGBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_MONEY_ERROR;
			msg.dwObjectID = pPlayer->GetID();
			
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
}

void CDistributer::SendItemToPerson(CPlayer* pPlayer)
{
	if( !pPlayer || !m_pMob ) return ;															// �÷��̾�� ���� ������ ��ȿ���� üũ�Ѵ�.

	// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
	if( !IsSameChannel(pPlayer, m_pMob) ) return ;

	int nMaxItemCount = eDROPITEMKIND_MAX-1 ;														// ���Ͱ� ����� �� �ִ� �ִ� ������ ��

	WORD MonsterKind = m_pMob->GetMonsterKind() ;												// ���� ������ �޴´�.

	MONSTEREX_LIST* pMonInfo = NULL ;															// ���� ����Ʈ ������ ���� ������ ����.
	pMonInfo = (MONSTEREX_LIST *)m_pMob->GetSMonsterList() ;									// ���� ����Ʈ�� �޴´�.							

	BOOL bObtainItem = FALSE ;																	// ������ �й谡 �Ǿ����� üũ�ϴ� ����.

	if( m_HighLevel <= 0 )																		// Ȥ�ö� �ְ����� ���� ������ �ִٸ�,
	{
		m_HighLevel = pPlayer->GetLevel() ;														// �ְ� ������ �÷��̾��� ������ �����Ѵ�.
	}

	int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;									// ���Ϳ� �������� ���� �г�Ƽ ���� �޴´�.

	if( nPenaltyType == 3 ) return ;															// �г�Ƽ ���� 3, 15���� �̻� ���̳��� ������ ����� ����.

	if( pMonInfo )																				// ���� ����Ʈ ������ ��ȿ���� üũ�Ѵ�.
	{
		for( int count = 0 ;  count < nMaxItemCount ; ++count )									// �ִ� ��� �� ��ŭ ������ ������.							
		{
			// ������ ����� �Ѵ�.
			if(ITEMDROP_OBJ->NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count],
				pPlayer, nPenaltyType))															// ������ ����� �����̸�,
			{
				bObtainItem = TRUE ;															// �������� ����Ǿ��ٰ� ������ �����Ѵ�.
			}
		}
	}

	if( bObtainItem )																			// ������ �Ҵ��� �̷�� ���ٸ�, 
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;							// ��Ƽ ������ �޴´�.

		if( pParty )																			// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
		{
			BYTE winnerIdx = pParty->GetWinnerIdx() ;											// ��÷�� ��ȣ�� �޴´�.
			
			if(winnerIdx == MAX_PARTY_LISTNUM-1 )												// ��÷ ��ȣ ������ ���������� üũ�Ѵ�.
			{
				pParty->SetWinnerIdx(0) ;														// ��÷ ��ȣ�� ó������ �����Ѵ�.
			}
			else																				// ������ �������� �ƴϸ�, 
			{
				pParty->SetWinnerIdx(++winnerIdx) ;												// ��÷ ��ȣ�� �����Ѵ�.						
			}
		}
	}
}

//EXPTYPE CDistributer::CalcObtainExp(LEVELTYPE KillerLevel)
// 071202 LYW --- Distributer : ��Ƽ���� 2�� �̻��� �� ���� ���̸� 5�������� 8������ �÷��ִ� �۾� ó��.
// 1. �ʿ� ���� üũ
// 2. ���� ����ġ �ޱ�
// 3. ���� ���� ���ϱ�
// 4. ���� ���� ���� ó��
// 080530 LYW --- Distributer : Modified the return type EXPTYPE to DWORD.
//EXPTYPE CDistributer::CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount)
DWORD CDistributer::CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount)
{
	// �ʿ� ���� üũ
	ASSERT(m_pMob) ;
	if( !m_pMob ) return 0 ;																	// ���� ������ üũ�Ѵ�.

	ASSERT(((CObject*)m_pMob)) ;
	if( !((CObject*)m_pMob) ) return 0 ;														// ������ ������Ʈ Ÿ���� ������ �� �ʿ��ϴ�.


	// ���� ����ġ �ޱ�
	DWORD dwExp = 0 ;																			// ����ġ�� ���� ������ �����Ѵ�.	   

	BYTE byObjectKind = 0 ;																		
	byObjectKind = ((CObject*)m_pMob)->GetObjectKind() ;										// ������ ������Ʈ ������ �޴´�.

	if( byObjectKind == eObjectKind_BossMonster )												// ���� �����̸�, 
	{
		WORD wMonsterKind = 0 ;
		wMonsterKind = m_pMob->GetMonsterKind() ;

		dwExp = (DWORD)BOSSREWARDSMGR->GetExpOfBossMonster(wMonsterKind) ;						// ���� ���� ����ġ�� �޴´�.
	}
	else																						// �Ϲ� �����̸�,
	{
		dwExp = m_pMob->GetSMonsterList()->ExpPoint ;											// �Ϲ� ���� ����ġ�� �޴´�.
	}


	// ���� ���� ���ϱ�
	LEVELTYPE MonsterLevel	= m_pMob->GetSMonsterList()->Level ;								// ���� ������ �޴´�.
	LEVELTYPE levelGap		= 0 ;																// ���Ϳ� �÷��̾��� ���� ����.
	
	if( MonsterLevel > KillerLevel )															// ���� ������ �÷��̾� ���� ū ���.
	{
		levelGap = MonsterLevel - KillerLevel ;													// ���� ���̸� ���Ѵ�.
	}
	else																						// �÷��̾� ������ ���� ���� ���� ū ���.
	{
		levelGap = KillerLevel - MonsterLevel ;													// ���� ���̸� ���Ѵ�.
	}

	int nLevelValue = 0 ;																		// ��Ȳ�� ���� ���� �� ������ �д�.

	if( nMemberCount >= 2 )																		// ��Ƽ���� 2�� �̻��̸�,
	{
		nLevelValue = 8 ;																		// ���� ���� 8������ �����Ѵ�.
	}
	else																						// 2�� �̸��̸�,
	{
		nLevelValue = 5 ;																		// ���� ���� 5������ �����Ѵ�.
	}


	// ���� ���� ���� ó��
	if( MonsterLevel > KillerLevel + nLevelValue )												// ���� ������ �÷��̾� ���� ���� 5���� �̻��� ��.
	{
		double dwLevel = KillerLevel+nLevelValue ;												// ���� ����Ʈ ����.
		// 080116 LYW --- Distributer : HP�� ������ ���͸� ����� ��, ȹ�� ����ġ ���� ó��.
		double dwPenalty = 1.95f ;																// ���Ƽ ����Ʈ ����.
		double result = pow(dwLevel, dwPenalty) ;												// ���� ����Ʈ ^ ���Ƽ ����Ʈ�� ���Ѵ�.

		float fExp = (float)(result + 15) ;														// �÷�Ʈ���� ũ�� ������ ����ġ�� ���Ѵ�.

		if( fExp > dwExp )
		{
			return dwExp ;
		}
		else
		{
			return (DWORD)floor(fExp) ;															// ����ġ�� �ݿø� �Ͽ� DWORD������ ��ȯ.
		}
	}
	else if( MonsterLevel < KillerLevel )														// �÷��̾� ������ ���� �������� ū ���.
	{
		float fPenalty = 1.0f ;																	// ���Ƽ ����Ʈ.
		float fExp     = 0.0f ;																	// �÷�Ʈ�� ����ġ.

		// 080116 LYW --- Distributer : ����Ʈ ��Ī ���� ��쿡�� �г�Ƽ�� ���� �ʴ´�.
		if (!g_csDateManager.IsChallengeZoneHere())
		{
			if( levelGap >= 6 && levelGap <= 10 )												// �÷��̾� ������ +6~+10�� ��.
			{
				fPenalty = 0.5f ;																// ���Ƽ ����Ʈ 50%�� ����
			}
			else if( levelGap >= 11 && levelGap <= 15 )											// �÷��̾� ������ +11~+15�� ��.
			{
				fPenalty = 0.25f ;																// ���Ƽ ����Ʈ 25%�� ����.
			}
			else if( levelGap > 15 )															// �÷��̾� ������ +15 �̻��� ��.
			{
				return 1 ;																		// ����ġ�� ������ 1�� ��ȯ�Ѵ�.
			}
		}

		fExp = (float)(dwExp * fPenalty) ;														// ����ġ�� �÷�Ʈ������ ���Ѵ�.

		return (DWORD)floor(fExp) ;																// DWORD������ ���Ƽ ����ġ ��ȯ.
	}	
	else																						// ���Ϳ� �÷��̾� ������ ���� ��.
	{
		return dwExp ;																			// �÷��̾�� ������ ������ ������� ���� ����ġ ��ȯ.			
	}
}

BOOL CDistributer::GetFirstDamange()
{
	m_FirstDamage = 0 ;																			// ù ������ ������ 0���� �����Ѵ�.
	
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(m_1stPlayerID) ;						// ù �������� �� �÷��̾� ������ �޴´�.

	if( pPlayer )																				// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
	{
		DAMAGEOBJ* pDObj = NULL ;																// ������ ������ ���� ������ ������ �����Ѵ�.

		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;							// �÷��̾��� ��Ƽ������ �޴´�.

		if( pParty )																			// ��Ƽ�� �����ִٸ�,
		{
			pDObj = m_DamageObjectTableParty.GetData(pParty->GetPartyIdx()) ;					// ��Ƽ�� ������ ������ �޴´�.

			if( pDObj )																			// ������ ������ ��ȿ���� üũ�Ѵ�.
			{
				m_FirstDamage = pDObj->dwData ;													// ù �������� �����Ѵ�.

				return TRUE ;																	// ���� TRUE.
			}
		}
		else																					// ��Ƽ�� �������� �ʴٸ�,
		{
			pDObj = m_DamageObjectTableSolo.GetData(pPlayer->GetID()) ;							// �÷��̾��� ������ ������ �޴´�.

			if( pDObj )																			// ������ ������ ��ȿ���� üũ�Ѵ�.
			{
				m_FirstDamage = pDObj->dwData ;													// ù �������� �����Ѵ�.

				return TRUE ;																	// ���� TRUE.
			}
		}
	}

	return FALSE ;																				// ���� FALSE.
}

void CDistributer::GetTotalKillerCount()
{
	DAMAGEOBJ* obj = NULL ;																		// ������ ������ ���� ������ ������ �����Ѵ�.

	CPlayer* pPlayer = NULL ;																	// �÷��̾� ������ ���� ������ ������ �����Ѵ�.

	CParty* pParty = NULL ;																		// ��Ƽ ������ ���� ������ ������ �����Ѵ�.

	m_nTotalKillerCount = 0 ;																	// ų�� ī��Ʈ�� 0���� �����Ѵ�.

	m_DamageObjectTableSolo.SetPositionHead() ;													// ���� ������ ���̺� üũ.

	while(obj = m_DamageObjectTableSolo.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// �������� ���� ���̵�� �÷��̾� ������ �޴´�.

				if( pPlayer )																	// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
				{
					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// �÷��̾��� ��Ƽ ������ �޴´�.

					if( !pParty )																// ��Ƽ�� ���ٸ�, 
					{
						++m_nTotalKillerCount ;													// �ַ� �÷��̾� ���� �ø���.
					}
				}
			}
		}
	}

	m_DamageObjectTableParty.SetPositionHead() ;												// ��Ƽ ���̺� üũ.

	while(obj = m_DamageObjectTableParty.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// ��Ƽ ������ �޴´�.

				if( pParty )																	// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.
				{
					++m_nTotalKillerCount ;														// �ַ� �÷��̾� ���� �ø���.
				}
			}
		}
	}
}

void CDistributer::GetHighLevelOfKillers()
{
	DAMAGEOBJ* obj = NULL ;																		// ������ ������ ���� ������ ������ �����Ѵ�.

	CPlayer* pPlayer = NULL ;																	// �÷��̾� ������ ���� ������ ������ �����Ѵ�.

	CParty* pParty = NULL ;																		// ��Ƽ ������ ���� ������ ������ �����Ѵ�.

	m_nTotalKillerCount = 0 ;																	// ų�� ī��Ʈ�� 0���� �����Ѵ�.

	m_DamageObjectTableSolo.SetPositionHead() ;													// ���� ������ ���̺� üũ.

	while(obj = m_DamageObjectTableSolo.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// �������� ���� ���̵�� �÷��̾� ������ �޴´�.

				if( pPlayer )																	// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
				{
					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// �÷��̾� ���̵�� ��Ƽ ������ �޴´�.

					if( !pParty )
					{
						LEVELTYPE playerLevel = pPlayer->GetLevel() ;							// �÷��̾��� ������ �޴´�.

						if( playerLevel > m_HighLevel )											// �÷��̾��� ������ �ְ� �������� ũ�ٸ�,
						{
							m_HighLevel = playerLevel ;											// �ְ� ������ �÷��̾� ������ �����Ѵ�.
						}
					}
				}
			}
		}
	}

	m_DamageObjectTableParty.SetPositionHead() ;												// ��Ƽ ������ ���̺� üũ.

	while(obj = m_DamageObjectTableParty.GetData())												// ������ ���̺��� ������ ������Ʈ�� �޴´�.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// �������� �ִ��� ���θ� üũ�Ѵ�.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// �������� ���� ���̵�� ��Ƽ ������ �޴´�.

				if( pParty )																	// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
				{
					LEVELTYPE highLevel = 0 ;													// 
					LEVELTYPE totalLevel = 0 ;

					GetPartyLevels(pParty, &highLevel, &totalLevel) ;							// �÷��̾��� ������ �޴´�.

					if( highLevel > m_HighLevel )												// �÷��̾��� ������ �ְ� �������� ũ�ٸ�,
					{
						m_HighLevel = highLevel ;												// �ְ� ������ �÷��̾� ������ �����Ѵ�.
					}
				}
			}
		}
	}
}

int CDistributer::GetLevelPenaltyToMonster(LEVELTYPE level) 
{
	LEVELTYPE MonsterLevel = m_pMob->GetSMonsterList()->Level ;									// ���� ������ �޴´�.
	LEVELTYPE levelGap	   = 0 ;																// ���Ϳ� �÷��̾��� ���� ����.

	int nPenaltyType = 0 ;																		// �г�Ƽ Ÿ���� ���� ����.
	
	if( MonsterLevel > level )																	// ���� ������ �÷��̾� ���� ū ���.
	{
		levelGap = 0 ;																			// ���� ���̸� ���Ѵ�.
	}
	else																						// �÷��̾� ������ ���� ���� ���� ū ���.
	{
		levelGap = level - MonsterLevel ;														// ���� ���̸� ���Ѵ�.
	}

	if( levelGap > 5 && levelGap < 11 )															// �������̰� 6~10�̸�, 
	{
		nPenaltyType = 1 ;																		// ����� 50% ����.
	}
	else if( levelGap > 10 && levelGap < 16 )													// �������̰� 11~15�̸�, 
	{
		nPenaltyType = 2 ;																		// ���Ȯ�� 75% ����.
	}
	else if( levelGap > 15 )																	// ���� ���̰� 15�̻��̸�,
	{
		nPenaltyType = 3 ;																		// ������ ��� ����.
	}

	return nPenaltyType ;
}

void CDistributer::GetPartyLevels(CParty* pParty, LEVELTYPE* pHighLevel, LEVELTYPE* pTotalLevel)
{
	if( !pParty || !m_pKilledPosition || !pHighLevel || !pTotalLevel )							// ��Ƽ�� �÷���� ������ ��ȿ���� üũ�Ѵ�.
	{
		return ;
	}

	CPlayer* pMember = NULL ;																	// ��� ������ ���� ������ ����.

	DWORD PlayerID = 0 ;																		// ������� ���̵� ���� ����.

	LEVELTYPE totallevel = 0 ;																	// ��Ƽ ������� ���� �� ��.
	LEVELTYPE highlevel = 0 ;																	// ��Ƽ ��� �� �ִ� ����.
	LEVELTYPE curlevel = 0 ;																	// ������� ������ ���� ����.
	
	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)									// �α��� ���� ��Ƽ�� ���� �� �� �ְ� ������ ���Ѵ�.
	{
		PlayerID = pParty->GetMemberID(count);													// ī��Ʈ ��� ����� ���̵� �޴´�.

		pMember = (CPlayer*)g_pUserTable->FindUser(PlayerID);									// ��� �÷��̾� ������ �޴´�.

		if( pMember )																			// ��� ������ ��ȿ���� üũ.
		{
			if( pMember->GetCurMapNum() == (DWORD)(g_pServerSystem->GetMapNum()) )				// ���� �ʿ� �ִ��� üũ�Ѵ�.
			{
				if(Chk(pMember, m_pKilledPosition, m_pMob->GetGridID()))						// ����ġ�� ���� �� �ִ� ���� ���� �ִ��� üũ.
				{
					curlevel = pMember->GetLevel();												// ���� ����� ������ �޴´�.

					if(highlevel < curlevel)													// ��Ƽ �� �ִ� �������� ū�� üũ �Ѵ�.
					{
						highlevel = curlevel;													// �ִ� ������ �����Ѵ�.
					}

					totallevel += curlevel;														// ��Ƽ ������ ���Ѵ�.																				
				}
			}
		}
	}

	*pHighLevel = highlevel ;																	// �ְ� ������ �����Ѵ�.
	*pTotalLevel = totallevel ;																	// ��Ƽ ������ ���� �����Ѵ�.
}

void CDistributer::SaveCandidacy(CParty* pParty)
{
	if(!m_pMob || !pParty || !m_pKilledPosition) return ;										// �÷��̾�� ���� ������ üũ�Ѵ�.

	CPlayer* pMember = NULL ;																	// ��� ������ ���� ������ ����.
	DWORD dwMemberID = 0 ;																		// ��� ���̵� ���� ����.

	PTRLISTPOS pos = m_CandidacyList.GetHeadPosition() ;										// �ĺ�����Ʈ�� �ص�� �����Ѵ�.

    DWORD* pMemberID = NULL ;																	// ��� ���̵� ���� ������ ���� ����.

	while(pMemberID = (DWORD*)m_CandidacyList.GetNext(pos))										// ��� ���̵� ������ ���������� while
	{
		if(pMemberID)																			// ��� ������ �ִٸ�,
		{
			delete pMemberID ;																	// ��� ������ �����Ѵ�.
			pMemberID = NULL ;																	// ��� ���� �����͸� NULLó�� �Ѵ�.
		}
	}

	m_CandidacyList.RemoveAll() ;																// �ĺ� ����Ʈ�� ����.

	m_CandidacyList.GetHeadPosition() ;															// �ĺ� ����Ʈ�� �ص�� �����Ѵ�.

	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)									// �ִ� ��Ƽ ��� �� ��ŭ ����.
	{
		dwMemberID = pParty->GetMemberID(count);												// ����� ���̵� �޴´�.

		pMember = (CPlayer*)g_pUserTable->FindUser(dwMemberID);									// ��� ������ �޴´�.

		if( pMember )																			// ��� ������ ��ȿ���� üũ�Ѵ�.
		{
			// 071215 LYW --- Distributer : ��Ƽ ����� ���� ���¿�����, �������� �߰����� �ʴ´�.
			if( pMember->GetState() == eObjectState_Die ) continue ;

			if( pMember->GetCurMapNum() == (DWORD)(g_pServerSystem->GetMapNum()) )				// ���� �ʿ� �ִ��� üũ�Ѵ�.					
			{
				if(Chk(pMember, m_pKilledPosition, m_pMob->GetGridID()))						// ���� ���� �� �ִ� �������� üũ�Ѵ�.
				{
					DWORD* pCandidacyID = new DWORD ;											// �ĺ� ���̵� �����Ѵ�.
					*pCandidacyID = dwMemberID ;												// �ĺ� ���̵� �����Ѵ�.
					m_CandidacyList.AddTail(pCandidacyID) ;										// �ĺ� ����Ʈ�� ����Ѵ�.
				}
			}
		}
	}
}

void CDistributer::DistribuSequence(CParty* pParty)
{
	if( !pParty ) return ;																		// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.

	int nWinnerIdx = (int)pParty->GetWinnerIdx() ;												// ��Ƽ�� ��÷ ��ȣ�� �޾ƿ´�.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// �ĺ��� ���� �޴´�.

	if( !pParty || nCandidacyCount <= 0 ) return ;												// ��Ƽ ������ ��ȿ����, �ĺ��� ���� �̻� ������ üũ�Ѵ�.

	PTRLISTPOS pos = NULL ;																		// ����Ʈ�� ��ġ ������ ���� �����͸� �޾ƿ´�.

	CPlayer* pMember = NULL ;																	// ��� ������ ���� �����͸� �����Ѵ�.

	DWORD* pMemberID = NULL ;																	// ��� ���̵� ���� ������ ������ �����Ѵ�.

	if( nWinnerIdx >= nCandidacyCount )															// ���� ��÷ ��ȣ���� �ĺ��� ���� �۰ų� ���ٸ�,
	{
		pParty->SetWinnerIdx(0) ;																// ��÷��ȣ�� ��ó������ �����Ѵ�.
		pos = m_CandidacyList.GetHeadPosition() ;												// �ĺ��� ����Ʈ�� �ص� �������� �޴´�.
	}
	else if( nWinnerIdx < nCandidacyCount )														// ��÷��ȣ���� �ĺ��� ���� ������,
	{
		pos = m_CandidacyList.FindIndex(nWinnerIdx) ;											// ��÷ ��ȣ�� �ش��ϴ� ��ġ�� �޴´�.
	}

	if( pos )																				// ��ġ ������ ��ȿ���� üũ�Ѵ�.
	{
		pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// ��÷ ��ġ�� �ش��ϴ� ��� ���̵� �޴´�.

		if( pMemberID )																		// ��� ���̵� �ּҰ� ��ȿ���� üũ�Ѵ�.
		{
			pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;						// ��� ���̵�� ��� ������ �޴´�.

			if( pMember )																	// ��� ������ ��ȿ���� üũ�Ѵ�.
			{
				// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
				if( !IsSameChannel(pMember, m_pMob) ) return ;

				SendItemToPerson( pMember ) ;												// �������� ������� �����Ѵ�.
			}
		}
	}
}

void CDistributer::DistributeDamage(CParty* pParty)
{
	if( !pParty ) return ;																		// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.

	DAMAGEOBJ* obj = NULL ;																		// ������ ������Ʈ ������.
	PTRLISTPOS pos = NULL ;																		// �ĺ��� ����Ʈ�� ��ġ ������.

	DWORD bigDamage = 0 ;																		// ���� ū ������.
	DWORD bigID = 0 ;																			// ���� ū �������� �� ���̵�.

	DWORD* pMemberID = NULL ;																	// ��� ���̵� ���� ������.
	CPlayer* pMember = NULL ;																	// ��� ������ ���� ������.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// �ĺ��� ���� �޴´�.

	m_DamageObjectTableSolo.SetPositionHead() ;													// ������ ���̺��� �ص�� �����Ѵ�.

	for(int n=0;n<nCandidacyCount;++n)															// �ĺ��� �� ��� ������ ���鼭 ��� ������ üũ.
	{
		pos = m_CandidacyList.FindIndex(n) ;													// �ĺ��� ����Ʈ���� �ش��ϴ� �����͸� �޾ƿ´�.

		if( pos )																				// ������ ������ ��ȿ���� üũ�Ѵ�.
		{
			pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// ������ �����Ϳ��� ��� ���̵� �޴´�.

			if( pMemberID )																		// ��� ���̵� ������ ��ȿ���� üũ�Ѵ�.
			{
				obj = m_DamageObjectTableSolo.GetData(*pMemberID) ;								// ������ ���̺��� ���̵�� ������ ������ �޴´�.

				if( obj )																		// ������ ������ ��ȿ���� üũ�Ѵ�.
				{
					if(bigDamage < obj->dwData)													// �ִ� �������� ����� ������ ��.
					{
						bigDamage = obj->dwData ;												// �ִ� �������� ����� �������� ����.
						bigID = obj->dwID ;														// �ִ� �������� �� ����� ���̵� ����.				
					}
				}
			}
		}
	}

	if( bigID == 0 ) return ;																	// ���� ū �������� ��� ���̵� üũ�Ѵ�.

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(bigID);									// ��� ������ �޴´�.

	if( pPlayer )																				// ��� ������ ��ȿ���� üũ.
	{
		// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		SendItemToPerson( pPlayer ) ;															// �������� �����Ѵ�.							
	}
}

void CDistributer::DistributeRandom(CParty* pParty)
{
	if( !pParty ) return ;																		// ��Ƽ ������ ��ȿ���� üũ�Ѵ�.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// �ĺ��� ���� �޴´�.

	if( nCandidacyCount == 0 ) return ;															// �ĺ��ڰ� ������ ����.

	CPlayer* pMember = NULL ;																	// ��÷�� ���� ������.

	PTRLISTPOS pos = NULL ;																		// ����Ʈ ��ġ ������.

	DWORD* pMemberID = NULL ;																	// ��� ���̵� ������.
    
	if( nCandidacyCount > 1 )																	// �ĺ��� ���� �Ѹ� �̻��̸�, 
	{
		int ranNum =  random(0, nCandidacyCount-1) ;											// ��÷�� ��ȣ�� �̴´�.

		pos = m_CandidacyList.FindIndex(ranNum ) ;												// ��÷ �� ��ȣ�� �����͸� �����Ѵ�.
	}

	if( pos )																					// ����Ʈ�� ������ ������ ������ ��ȿ���� üũ�Ѵ�.
	{
		pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;										// ������ �������� ��� ���̵� �޴´�.
	}
		
	if( pMemberID )																				// ��� ���̵� ������ ��ȿ���� üũ�Ѵ�.
	{
		pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID);									// ��÷�� ������ �޴´�.
	}

	if( pMember )																				// ��÷�� ������ ��ȿ���� üũ.
	{
		// 080122 LYW --- Distributer : Player �� Monster�� ä���� Ȯ���Ѵ�.
		if( !IsSameChannel(pMember, m_pMob) ) return ;

		SendItemToPerson( pMember ) ;															// �������� �����Ѵ�.
	}
}





//=========================================================================
//	NAME : IsSameChannel
//	DESC : The function to check channel of player and monster. 080122 LYW
//  �� ��������, ���Ϳ� PLAYER�� ä���� �ٸ���, ������, ��, ����ġ �й踦
//  ���� �ʵ��� ó���ϱ�� �����Ƿ�, Player�� Monster ������ ��ȿ���� ������,
//  false ���� ó���� �Ѵ�.
//=========================================================================
BOOL CDistributer::IsSameChannel(CPlayer* pPlayer, CMonster* pMonster)
{
	BOOL bResult = FALSE ;														// ����� ���� ����.

	if( !pPlayer ) return bResult ;												// Player ������ üũ.

	if( !pMonster ) return bResult ;											// Monster ������ üũ.
/*
	DWORD dwLastKillerID	= 0 ;												
	dwLastKillerID			= pMonster->GetLastKillerID() ;						// ���������� Monster�� Kill �� Player ������ �޴´�.

	CPlayer* pLastKiller	= NULL ;
	pLastKiller = (CPlayer*)g_pUserTable->FindUser( dwLastKillerID ) ;			// ������ Killer ������ �޴´�.

	if( !pLastKiller ) return bResult ;											// Killer ���� üũ.

	DWORD dwPlayerChannel		= pPlayer->GetChannelID() ;						// Player �� ������ Killer ä���� �޴´�.
	DWORD dwLastKillerChannel	= pLastKiller->GetChannelID() ;

	if( dwPlayerChannel == dwLastKillerChannel )								// �� ä���� ��.
	{
		bResult = TRUE ;														// ���� ä���̸� true ����.
	}
*/
	if( pPlayer->GetGridID() == pMonster->GetGridID() )
	{
		bResult = TRUE ;														// ���� ä���̸� true ����.
	}

    return bResult ;															// ����� �����Ѵ�.
}

//void CDistributer::DistribuSequence(int nCandidadyCount, CParty* pParty, WORD dwCurMap)
//{
//	BYTE winnerIdx = winnerIdx = pParty->GetWinnerIdx() ;									// ��÷�� ��ȣ.
//
//	BOOL bSendItem = FALSE ;
//
//	DWORD PlayerID = 0 ;																	// ��÷�� ���̵�.
//
//	for(int i=0; i<MAX_PARTY_LISTNUM; i++)													// �ִ� ��Ƽ ��� �� ��� ������ ���鼭 üũ.
//	{
//		PlayerID = pParty->GetMemberID(i) ;													// ����� ���̵� �޴´�.
//
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;						// ��� ������ �޴´�.
//
//		if( pPlayer )																		// ��� ������ ��ȿ���� üũ �Ѵ�.
//		{
//			WORD dwMapNum = pPlayer->GetCurMapNum() ;										// ����� ���� �� ��ȣ�� Ȯ���Ѵ�.
//
//			if( dwCurMap == dwMapNum )														// ���͸� ���� �� ��ȣ�� ����� ���� ���� ��.
//			{
//				if(ChkExp(pPlayer, m_pKilledPosition, m_pMob->GetGridID()))					// �������� ���� �� �ִ� ���� �ȿ� �ִ��� üũ.
//				{
//					if( winnerIdx == i )													// ��÷�� ��ȣ�� ��� ��ȣ�� ������ ��.
//					{
//						bSendItem = TRUE ;
//						SendItemToPerson( pPlayer ) ;										// �������� �����Ѵ�.
//						
//						return ;
//					}
//				}
//			}
//		}
//		else
//		{
//			if(winnerIdx == MAX_PARTY_LISTNUM-1 )											// �ĺ��� ��ȣ�� üũ �Ѵ�.
//			{
//				pParty->SetWinnerIdx(0) ;													// �ĺ��� ��ȣ�� ó������ ����.
//			}
//			else
//			{
//				pParty->SetWinnerIdx(++winnerIdx) ;											// �ĺ��� ��ȣ�� �����Ѵ�.
//			}
//		}
//	}
//
//	if( !bSendItem )																		// �������� ���� ����� ������, 
//	{
//		winnerIdx = pParty->GetWinnerIdx() ;												// ��÷�� ��ȣ.
//
//		for(int i=0; i<MAX_PARTY_LISTNUM; i++)												// �ѹ��� �� �ִ� ��Ƽ ��� �� ��� ������ ���鼭 üũ.
//		{
//			PlayerID = pParty->GetMemberID(i) ;												// ����� ���̵� �޴´�.
//
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;					// ��� ������ �޴´�.
//
//			if( pPlayer )																	// ��� ������ ��ȿ���� üũ �Ѵ�.
//			{
//				WORD dwMapNum = pPlayer->GetCurMapNum() ;									// ����� ���� �� ��ȣ�� Ȯ���Ѵ�.
//
//				if( dwCurMap == dwMapNum )													// ���͸� ���� �� ��ȣ�� ����� ���� ���� ��.
//				{
//					if(ChkExp(pPlayer, m_pKilledPosition, m_pMob->GetGridID()))				// �������� ���� �� �ִ� ���� �ȿ� �ִ��� üũ.
//					{
//						if( winnerIdx == i )												// ��÷�� ��ȣ�� ��� ��ȣ�� ������ ��.
//						{
//							SendItemToPerson( pPlayer ) ;									// �������� �����Ѵ�.
//							
//							return ;
//						}
//					}
//				}
//			}
//			else
//			{
//				if(winnerIdx == MAX_PARTY_LISTNUM-1 )										// �ĺ��� ��ȣ�� üũ �Ѵ�.
//				{
//					pParty->SetWinnerIdx(0) ;												// �ĺ��� ��ȣ�� ó������ ����.
//				}
//				else
//				{
//					pParty->SetWinnerIdx(++winnerIdx) ;										// �ĺ��� ��ȣ�� �����Ѵ�.										
//				}
//			}
//		}
//	}
//}

//int CDistributer::GetCandidacyCount(CParty* pParty, CPlayer* pPlayer)
//{
//	if(!m_pMob || !pPlayer || !pParty || !m_pKilledPosition) return 0 ;				// �÷��̾�� ���� ������ üũ�Ѵ�.
//
//	int nCandidacyCount = 0 ;														// ������ ���� �÷��̾� ��.
//
//	CPlayer* pMember = NULL ;														// ��� ������ ���� ������ ����.
//	DWORD dwMemberID = 0 ;															// ��� ���̵� ���� ����.
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)						// �ִ� ��Ƽ ��� �� ��ŭ ����.
//	{
//		dwMemberID = pParty->GetMemberID(count);									// ����� ���̵� �޴´�.
//
//		pMember = (CPlayer*)g_pUserTable->FindUser(dwMemberID);						// ��� ������ �޴´�.
//
//		if( pMember )																// ��� ������ ��ȿ���� üũ�Ѵ�.
//		{
//			if( pMember->GetCurMapNum() == pPlayer->GetCurMapNum() )				// ���� �ʿ� �ִ��� üũ�Ѵ�.					
//			{
//				if(ChkExp(pMember, m_pKilledPosition, m_pMob->GetGridID()))			// ���� ���� �� �ִ� �������� üũ�Ѵ�.
//				{
//					++nCandidacyCount ;												// ������ ���� �÷��̾� �� ����.
//				}
//			}
//		}
//	}
//
//	return nCandidacyCount ;														// ������ ���� �÷��̾� �� ����.
//}


// 070413 LYW --- Distributer : Modified function Distribute.
/*
void CDistributer::Distribute(DWORD KillerID,VECTOR3* pKilledPosition, WORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
{
	DAMAGEOBJ *obj;
	DWORD Exp = 0;
	CPlayer* pReceivePlayer;
	
	DWORD BigPlayerID; //�������� ���� ���� �༭ ����ġ�� ������ ������ ���/��Ƽ
	DWORD BigPartyID;
	DWORD BigPlayerDamage, BigPartyDamage;
	BigPlayerID = BigPartyID = BigPlayerDamage = BigPartyDamage = 0;
	
	//�ַ� ���̺� �˻�
	m_DamageObjectTableSolo.SetPositionHead();
	while(obj = m_DamageObjectTableSolo.GetData())
	{
		if(obj->dwData == 0)
			continue;
		////�����۰� ����ġ ���� ��� �����ϱ� ���� ���� ū ������ �� ��� ����
		ChooseOne(obj, &BigPlayerDamage, &BigPlayerID);

		pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
		if(Chk(pReceivePlayer, pKilledPosition, pMob->GetGridID()) == FALSE)
			continue;
		if(obj->dwID == m_1stPlayerID)
		{
			obj->dwData -= m_PlusDamage;
		}
		if(pReceivePlayer->GetPartyIdx())
			continue;
		if( pMob->GetSMonsterList()->ExpPoint)	//ȣ�� ��Ÿ
			Exp = CalcObtainExp(pMob->GetLevel() ,pReceivePlayer->GetLevel(),pMob->GetSMonsterList()->Life,obj->dwData);
			//SW050806 ���� ����Ÿ ���̺� �Ľ̿��� ��� ����ȭ�� ����
//			Exp = CalcObtainExp(pMob->GetLevel(), pReceivePlayer->GetLevel(), pMob->GetSMonsterList()->Life, obj->dwData, pMob->GetSMonsterList()->ExpPoint );
		else
			Exp = 0;
		if(Exp == 0)
			continue;

		// ���ο��� ����ġ ����
		DISTRIBUTERAND->SendToPersonalExp(pReceivePlayer, Exp);
	}
	
	//��Ƽ
	DAMAGEOBJ* pPobj = NULL;
	CParty* pParty;
	m_DamageObjectTableParty.SetPositionHead();
	while(pPobj = m_DamageObjectTableParty.GetData())
	{
		////ū ������ �� ��Ƽ ����
		ChooseOne(pPobj, &BigPartyDamage, &BigPartyID);

		pParty = PARTYMGR->GetParty(pPobj->dwID);
		if(pParty==NULL) //��Ƽ�� �ػ������ continue
			continue;
		if(pPobj->dwData == 0)
			continue;
		if(pParty->GetPartyIdx() == m_1stPartyID)
		{
			pPobj->dwData -= m_PlusDamage;
		}

		if( pMob->GetSMonsterList()->ExpPoint)	//ȣ�� ��Ÿ		
			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel());
//SW050806 ���� ����Ÿ ���̺� �Ľ̿��� ��� ����ȭ�� ����
//			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel(), pMob->GetSMonsterList()->ExpPoint);
	}

	//abil and item
	pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(BigPlayerID);
	pParty = PARTYMGR->GetParty(BigPartyID);

	if(BigPartyDamage < BigPlayerDamage) //���ο��� ����
	{
		SendPersonalAbilandItem(pReceivePlayer, pMob, DropItemID, DropItemRatio);
	}
	else if(BigPartyDamage == BigPlayerDamage) //�������� ������ �������� ����
	{ 
		int rate = rand()%2;
		if(pParty)
		if(pParty->IsPartyMember(BigPlayerID) == TRUE)
		{
			SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
		}		
		else if(rate == 0)
		{
			SendPersonalAbilandItem(pReceivePlayer, pMob, DropItemID, DropItemRatio);
		}
		else //��Ƽ����
		{	
			SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
		}
	}
	else //��Ƽ����
	{
		SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
	}
	Release();
}
*/

//void CDistributer::DistributeToKillers(DWORD KillerID, DAMAGEOBJ *obj, VECTOR3* pKilledPosition, CMonster* pMob, CPlayer* pBigPlayer, int nKillerCount, DWORD BigPlayerDamage, DWORD DropItemID, DWORD DropItemRatio)
//{
//	CPlayer* pReceivePlayer = NULL ;
//	CParty* pParty = NULL ;
//
//	DWORD dwItemID = 0 ;
//	DWORD dwDamageToGiveItem = 0 ;
//
//	m_DamageObjectTableSolo.SetPositionHead();
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ����ġ �й踦 �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	while(obj = m_DamageObjectTableSolo.GetData())
//	{
//		if(obj->dwData > 0)
//		{
//			pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
//
//			if( pReceivePlayer )
//			{
//				// ū�������� �� ����� ���̵� �����Ѵ�.
//				if( dwDamageToGiveItem < obj->dwData )
//				{
//					dwItemID = pReceivePlayer->GetID() ;
//					dwDamageToGiveItem = obj->dwData ;
//				}
//                
//				// ���Ͱ� ���� �ڸ����� �÷��̾ ���� ���� �ִ��� �˻��Ѵ�.
//				if(Chk(pReceivePlayer, pKilledPosition, pMob->GetGridID()))
//				{
//					// �й� �� ������ ��Ƽ ���ο� ���� ����ġ�� ���Ѵ�.
//					pParty = PARTYMGR->GetParty(pReceivePlayer->GetPartyIdx());
//
//					DWORD dwExp	= CalcObtainExp(pMob, pBigPlayer->GetLevel()) ;
//
//					// ���͸� ó�� ������ �÷��̾� ó��.
//					if(obj->dwID == m_1stPlayerID)
//					{
//						if(!pParty)												// ��Ƽ�� ������ ���� ����ġ�� �����Ѵ�.
//						{
//							DistributeToFirstKiller(obj, dwExp) ;			
//						}
//						else													// ��Ƽ�� ������ ��Ƽ ����ġ�� �����Ѵ�.
//						{
//							// ù ų�� �ε� ��Ƽ�� ������ ��� ó�� �ϳ�?
//							DistributeToParty(m_1stPlayerID, pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//						}
//
//						continue ;												// �� ����� ó���� �������� ���� ��� ó���� �Ѿ��.
//					}
//
//					// ���߿� ���͸� ������ �÷��̾� ó��
//					if(!pParty)													// ��Ƽ�� ������ ���� ����ġ�� �����Ѵ�.
//					{
//						DistributeToOtherKiller(obj, nKillerCount, dwExp) ;	
//					}
//					else														// ��Ƽ�� ������ ��Ƽ ����ġ�� �����Ѵ�.
//					{
//						DistributeToParty(pReceivePlayer->GetID(), pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//					}
//				}
//			}
//		}
//	}
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ������ �й踦 �Ѵ�. - ���� ���͸� óġ �� ����� �� ū �������� �� ������� �������� �й��Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	CPlayer* pReceiveItemPlayer = (CPlayer *)g_pUserTable->FindUser(dwItemID) ;							// �������� ���� �÷��̾ �޴´�.
//
//	if( pReceiveItemPlayer )																			// �÷��̾� ������ ��ȿ���� üũ
//	{
//		CParty* pParty = PARTYMGR->GetParty(pReceiveItemPlayer->GetPartyIdx()) ;						// ��Ƽ�� �ִ��� üũ�ϱ� ���� ��Ƽ�� �޴´�.
//
//		if( pParty )																					
//		{
//			WORD dwCurMap = pReceiveItemPlayer->GetCurMapNum() ;						// ���͸� óġ�� �� ��ȣ�� �޴´�.
//			DistributeItemToParty(pParty->GetPartyIdx(), pKilledPosition, DropItemID, DropItemRatio, pMob, obj, dwCurMap) ;	// ��Ƽ�� �ִٸ� ��Ƽ ������ �й踦 �Ѵ�.
//		}
//		else
//		{
//			PickUpAcquisition(pBigPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);			// ��Ƽ�� ������ ���� ������ �й踦 �Ѵ�.
//		}
//	}
//}

//void CDistributer::DistributeToFirstKiller(DAMAGEOBJ *obj, DWORD dwExp)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ó�� ���͸� ������ �÷��̾��� ����ġ�� ��� �� �����Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DWORD dwFirstDamage = obj->dwData ;											// ù �������� ��´�. 
//	DWORD dwMyDamage	= obj->dwData ;											// �÷��̾��� �������� ��´�.
//
//	float fPriority		= ((float)dwMyDamage *0.3f) ;							// ù �������� ���� �߰� �������� �����Ѵ�.
//
//	float fExpoint		= (float)(((dwMyDamage + fPriority) / m_TotalDamage)) ;	// ����ġ ���� ����Ʈ�� ���Ѵ�.
//
//	if( fExpoint <= 0 )															// Ȥ�ö� ����Ʈ�� ������ 0 �� �������� üũ
//	{
//		fExpoint = 1 ;
//	}
//	
//	DWORD dwSendExp = (DWORD)GetFloatRound(fExpoint * dwExp) ;					// ����Ʈ�� ����Ͽ� �ݿø� �� ����ġ�� ���Ѵ�.
//
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);			// ���� �� �÷��̾ �޴´�.
//
//	if( pPlayer )																// �÷��̾� ������ ��ȿ���� üũ
//	{
//		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwSendExp) ;					// ����ġ�� �÷��̾�� �����Ѵ�.
//	}
//}

//void CDistributer::DistributeToOtherKiller(DAMAGEOBJ *obj, int nKillerCount, DWORD dwExp)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// �߰��� ���͸� ������ �÷��̾��� ����ġ�� ��� �� �����Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DWORD dwMyDamage	= obj->dwData ;											// �÷��̾��� �������� �޴´�.
//	float fPriority		= ((float)dwMyDamage *0.3f) ;							// ���͸� ó�� ������ �������� �޴´�.
//
//	float fExpA = fPriority / (nKillerCount-1) ;								// ����ġ ���� ����Ʈ A�� ���Ѵ�.
//	float fExpB = dwMyDamage - fExpA ;											// ����ġ ���� ����Ʈ B�� ���Ѵ�.
//
//	if( obj->dwData < fExpA )													// ����Ʈ A�� �� ũ�� �Ʒ� ��꿡 ������ ����� ������
//	{																			// ���� ó���� �Ѵ�.
//		fExpB = 0 ;
//	}
//
//	float fExpC = fExpB / m_TotalDamage ;										// ����ġ ���� ����Ʈ C�� ���Ѵ�.
//	
//	float fSendExp = fExpC * dwExp ;											// ����ġ�� ���Ѵ�.
//
//	DWORD dwSendExp = (DWORD)GetFloatRound(fSendExp) ;							// ����ġ�� �ݿø� �Ѵ�.
//	
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);			// ���� �� �÷��̾ �޴´�.
//
//	if( pPlayer )																// �÷��̾� ������ ��ȿ���� üũ
//	{
//		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwSendExp) ;					// ����ġ�� �÷��̾�� �����Ѵ�.
//	}
//}

//void CDistributer::DistributeToKiller(DWORD KillerID, CMonster* pMob, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, DWORD BigPlayerDamage, CPlayer* pBigPlayer, DAMAGEOBJ *obj)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ���͸� óġ �� �÷��̾ �Ѹ��� �� ó���� �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(KillerID);								// ���͸� óġ �� �÷��̾ �޴´�.
//
//	if( pPlayer )																				// �÷��̾� ������ ��ȿ���� üũ�Ѵ�.
//	{
//		CParty* pParty = NULL ;																	// ��Ƽ ���θ� üũ�ϱ� ���� ��Ƽ ������ ����.
//		
//		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());									// �÷��̾��� ��Ƽ�� �޴´�.
//
//		if(!pParty)																				// ��Ƽ�� ���� �� ó��.
//		{
//			DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;							// ������ ����ġ�� �޾� �´�.
//
//			DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;									// �÷��̾�� ����ġ�� �����Ѵ�.
//			PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);		// �÷��̾�� �������� �����Ѵ�.
//		}
//		else																					// ��Ƽ�� ���� ���� ��Ƽ �й�ó���� �ѱ��.
//		{
//			// ��Ƽ ����ġ ó���� �Ѵ�.
//			DistributeToParty(KillerID, pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//			// ��Ƽ�� �ִٸ� ��Ƽ ������ �й踦 �Ѵ�.
//			WORD dwCurMap = pPlayer->GetCurMapNum() ;											// ���͸� óġ�� �� ��ȣ�� �޴´�.
//			DistributeItemToParty(pParty->GetPartyIdx(), pKilledPosition, DropItemID, DropItemRatio, pMob, obj, dwCurMap) ;	
//		}
//	}
//}

//void CDistributer::DistributeToParty(DWORD KillerID,VECTOR3* pKilledPosition, CMonster* pMob, DWORD BigPlayerDamage, CPlayer* pBigPlayer, DWORD DropItemID, DWORD DropItemRatio, DAMAGEOBJ *obj)
//{
//	DAMAGEOBJ* pPobj = NULL;
//	CParty* pParty	 = NULL ;
//
//	m_DamageObjectTableParty.SetPositionHead();
//
//	// ��Ƽ�� ����ġ ó���� �Ѵ�.
//	while(pPobj = m_DamageObjectTableParty.GetData())
//	{
//		pParty = PARTYMGR->GetParty(pPobj->dwID);
//
//		if( pParty )
//		{
//			if(pPobj->dwData > 0)
//			{
//				CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob);
//			}
//		}
//	}
//
//	// ù ų������ üũ �ؼ� ��Ƽ ����ġ ó���� �Ѵ�.
//
//	//DAMAGEOBJ* pPobj = NULL;
//	//CParty* pParty	 = NULL ;
//
//	//// ū �������� �� ��Ƽ�� �����Ѵ�.
//	//DWORD BigPartyID = 0 ;
//	//DWORD BigPartyDamage = 0 ;
//
//	//m_DamageObjectTableParty.SetPositionHead();
//
//	//while(pPobj = m_DamageObjectTableParty.GetData())
//	//{
//	//	ChooseOne(pPobj, &BigPartyDamage, &BigPartyID);
//	//}
//
//	////��Ƽ
//	//pPobj  = NULL ;
//	//pParty = NULL ;
//
//	//m_DamageObjectTableParty.SetPositionHead();
//
//	//// ��Ƽ�� ����ġ ó���� �Ѵ�.
//	//while(pPobj = m_DamageObjectTableParty.GetData())
//	//{
//	//	pParty = PARTYMGR->GetParty(pPobj->dwID);
//
//	//	if( pParty )
//	//	{
//	//		if(pPobj->dwData > 0)
//	//		{
//	//			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob);
//	//		}
//	//	}
//	//}
//
//	//// ������ �й� ó���� �Ѵ�.
//	//if(BigPartyDamage <= BigPlayerDamage )
//	//{
//	//	PickUpAcquisition(pBigPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);
//	//}
//	//else
//	//{
//	//	DistributeItemToParty(BigPartyID, pKilledPosition, DropItemID, DropItemRatio, pMob, obj) ;
//	//}
//
//	//Release();
//}
//void CDistributer::DistributeItemToParty(DWORD BigPartyID, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob, DAMAGEOBJ *obj, WORD dwCurMap)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ��Ƽ ������ �й踦 ó�� �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CParty* pBigParty = PARTYMGR->GetParty(BigPartyID) ;					// �������� ���� �÷��̾��� ��Ƽ�� �޴´�.
//
//	if( pBigParty )															// ��Ƽ ������ ��ȿ���� üũ.
//	{
//		int onlinenumconfirm = 0 ;											// �α��� ���� ����� �����Ѵ� ���� ����. 
//
//		DWORD bigDamage = 0 ;
//		DWORD bigID = 0 ;
//
//		DWORD PlayerID = 0 ;
//		CPlayer* pPlayer = NULL ;
//
//		// ���� ū �������� ���� ū �������� �� �÷��̾�,  �׸��� �α��� ���� ��� ���� ���Ѵ�.
//
//		for(int n=0;n<MAX_PARTY_LISTNUM;++n)								// �ִ� ��Ƽ��� �� ��� ������ ���鼭 ��� ������ üũ.
//		{
//			PlayerID = pBigParty->GetMemberID(n);							// ��� ���̵� �޴´�.
//			
//			pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);			// ��� ������ �޴´�.
//
//			if( pPlayer )													// ��� ������ ��ȿ���� üũ.
//			{
//				if(ChkExp(pPlayer, pKilledPosition,pMob->GetGridID()))		// ���Ͱ� ���� �ڸ����� ��ȿ ���� ������ üũ.
//				{
//					m_DamageObjectTableSolo.SetPositionHead();				// ������ ���̺��� ����� �����Ѵ�.
//
//					while(obj = m_DamageObjectTableSolo.GetData())			// ������ ���̺��� �˻��Ѵ�.
//					{
//						if(obj->dwData > 0)									// �������� �־����ٸ�.
//						{
//							if( obj->dwID == pPlayer->GetID() )				// �������� �� �÷��̾��� ���̵� �޴´�.
//							{
//								if(bigDamage < obj->dwData)					// �ִ� �������� �÷��̾��� ������ ��.
//								{
//									bigDamage = obj->dwData ;				// �ִ� �������� �÷��̾��� �������� ����.
//									bigID = obj->dwID ;						// �ִ� �������� �� �÷��̾��� ���̵� ����.			
//								}
//							}
//						}
//					}
//
//					++onlinenumconfirm;										// �α��� ���� ��� ���� ����.
//				}
//			}
//		}
//
//		if( onlinenumconfirm == 0 ) return ;								// �α��� ���� ����� ������ ����.
//
//		if(onlinenumconfirm == 1)											// �α��� ���� ����� �Ѹ��̸� �ڱ��ڽ� ��, ���� �й� ó���� �Ѵ�.
//		{
//			pPlayer = (CPlayer *)g_pUserTable->FindUser(bigID);				// �÷��̾ �޴´�.
//
//			if( pPlayer )													// �÷��̾� ������ ��ȿ���� üũ.													
//			{
//				DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;	// ���� ����ġ�� �޾� �´�.
//
//				PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);	// ���ο��� �������� �й� �Ѵ�.
//
//				return ;													// ���� ó��.
//			}
//		}
//
//		// ������ �й踦 ���� ������ �й� ������ �޴´�.
//		switch(pBigParty->GetOption())										
//		{
//		// ���� �й� 
//		case ePartyOpt_Sequence : DistribuSequence(onlinenumconfirm, pBigParty, pMob, DropItemID, DropItemRatio, pKilledPosition, dwCurMap) ;	break ;
//		// ������ �й�
//		case ePartyOpt_Damage :	  DistributeDamage(bigID, pKilledPosition, DropItemID, DropItemRatio, pMob) ;	break ;
//		// ���� �й�
//		case ePartyOpt_Random :	  DistributeRandom(onlinenumconfirm, pBigParty, pMob, pKilledPosition, DropItemID, DropItemRatio) ; 	break ;
//		}
//	}
//}

//void CDistributer::DistribuSequence(int onlinenumconfirm, CParty* pBigParty, CMonster* pMob, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, WORD dwCurMap)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// �������� ���������� �й��Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm > 1 )																	// �ĺ��� �� üũ.
//	{
//		BYTE winnerIdx = winnerIdx = pBigParty->GetWinnerIdx() ;								// ��÷�� ��ȣ.
//
//		DWORD PlayerID = 0 ;																	// ��÷�� ���̵�.
//
//		for(int i=0; i<MAX_PARTY_LISTNUM; i++)													// �ִ� ��Ƽ ��� �� ��� ������ ���鼭 üũ.
//		{
//			//winnerIdx = pBigParty->GetWinnerIdx() ;												// ��÷�� ��ȣ�� �̴´�.
//
//			//PlayerID = pBigParty->GetMemberID(winnerIdx);										// ��÷�� ���̵� �޴´�.
//
//			//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);						// ��÷�� ������ �޴´�.
//
//			//if( pPlayer )																		// ��÷�� ������ ��ȿ���� üũ.
//			//{
//			//	PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);	// ��÷�ڿ��� ������ ����.
//
//			//	return ;																		// ���� ó��.
//			//}
//
//			PlayerID = pBigParty->GetMemberID(i) ;												// ����� ���̵� �޴´�.
//
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;						// ��� ������ �޴´�.
//
//			if( pPlayer )																		// ��� ������ ��ȿ���� üũ �Ѵ�.
//			{
//				WORD dwMapNum = pPlayer->GetCurMapNum() ;										// ����� ���� �� ��ȣ�� Ȯ���Ѵ�.
//
//				if( dwCurMap == dwMapNum )														// ���͸� ���� �� ��ȣ�� ����� ���� ���� ��.
//				{
//					if(ChkExp(pPlayer, pKilledPosition,pMob->GetGridID()))						// �������� ���� �� �ִ� ���� �ȿ� �ִ��� üũ.
//					{
//						if( winnerIdx == i )													// ��÷�� ��ȣ�� ��� ��ȣ�� ������ ��.
//						{
//							// �������� �����Ѵ�.
//							PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition, onlinenumconfirm);
//
//							//if(winnerIdx == MAX_PARTY_LISTNUM-1 )								// ��÷�� ��ȣ�� �ѹ��� ���Ҵ��� üũ.
//							//{
//							//	pBigParty->SetWinnerIdx(0) ;									// ��÷�� ��ȣ�� ����.
//							//}
//							//else																// ��÷�� ��ȣ�� �ѹ����� ���� �ʾҴٸ�.
//							//{
//							//	pBigParty->SetWinnerIdx(++winnerIdx) ;							// ��÷�� ��ȣ�� ���� ��ȣ�� ����.
//							//}
//
//							// ���� ó���� �Ѵ�.
//							return ;
//						}
//					}
//				}
//			}
//		}
//	}
//}

//void CDistributer::DistributeDamage(DWORD bigID, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ���� ū �������� �� ������� �������� �й� �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(bigID);									// �÷��̾ �޴´�.
//
//	if( pPlayer )																				// �÷��̾� ������ ��ȿ���� üũ.
//	{
//		PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);			// �������� �����Ѵ�.
//	}
//}
//
//void CDistributer::DistributeRandom(int onlinenumconfirm, CParty* pBigParty, CMonster* pMob, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// �����ϰ� �������� �й� �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm >= 1 )																	// �ĺ��� �� üũ.
//	{
//		DWORD PlayerID = 0 ;																	// ��÷�� ���̵�.
//		CPlayer* pPlayer = NULL ;																// ��÷�� ���� ������.
//
//		int ranNum =  random(0, onlinenumconfirm-1) ;											// ��÷�� ��ȣ�� �̴´�.
//
//		PlayerID = pBigParty->GetMemberID(ranNum);												// ��÷�� ���̵� �޴´�.
//		
//		pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);									// ��÷�� ������ �޴´�.
//
//		if(pPlayer)																				// ��÷�� ������ ��ȿ���� üũ.
//		{
//			PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);		// �������� �����Ѵ�.
//			return ;
//		}
//	}
//}

//void CDistributer::Distribute(DWORD KillerID,VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ���� ū �������� �� �÷��̾�� ���͸� ���� �÷��̾� ī��Ʈ�� ���Ѵ�.
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DAMAGEOBJ *obj = NULL ;													// ������ ������Ʈ.
//
//	DWORD BigPlayerID		= 0 ;											// ���� ū �������� �� �÷��̾� ���̵�.
//	DWORD BigPlayerDamage	= 0 ;											// ���� ū ������.
//
//	CPlayer* pBigPlayer = NULL ;											// ���� ū �������� �� �÷��̾� ������.
//
//	int nKillerCount = 0 ;													// ���͸� óġ �� �÷��̾� ��.
//
//	m_DamageObjectTableSolo.SetPositionHead();								// ������ ������Ʈ ���̺��� ó������ ����.
//
//	while(obj = m_DamageObjectTableSolo.GetData())							// ������ ������Ʈ�� ��ȿ�� ���� üũ.
//	{
//		if(obj->dwData > 0)													// �������� �ִ��� üũ.
//		{
//			ChooseOne(obj, &BigPlayerDamage, &BigPlayerID) ;				// ���� ū �������� �÷��̾� ���̵� �޴´�.
//
//			pBigPlayer = (CPlayer *)g_pUserTable->FindUser(BigPlayerID);	// ���� ū �������� �� �÷��̾� ������ �޴´�.
//
//			if( pBigPlayer ) ++nKillerCount ;								// �÷��̾� ������ ��ȿ�ϸ� ų�� ī��Ʈ ����.
//		}
//	}
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ���� �й踦 �����Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	if( nKillerCount > 1 )														
//	{
//		// ���͸� óġ �� �÷��̾ �ټ��� �� ó��.
//		DistributeToKillers(KillerID, obj, pKilledPosition, pMob, pBigPlayer, nKillerCount, BigPlayerDamage, DropItemID, DropItemRatio) ;
//	}
//	else
//	{
//		// ���͸� óġ �� �÷��̾ �Ѹ��� �� ó��.
//		DistributeToKiller(KillerID, pMob, DropItemID, DropItemRatio, pKilledPosition, BigPlayerDamage, pBigPlayer, obj) ;		
//	}
//
//	m_TotalDamage = 0 ;
//}

//void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel, DWORD MonsterExp )
// 070413 LYW --- Distributer : Modified function CalcAndSendPartyExp.
/*
//void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, LEVELTYPE MonsterLevel )
void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, CMonster* pMob )
{
	DAMAGEOBJ *obj = NULL;
	CPlayer* pPlayer[MAX_PARTY_LISTNUM] = {0,};
	DWORD PlayerID;
	float levelavg = 0;
	LEVELTYPE maxlevel = 0;  
	LEVELTYPE curlevel = 0;
	DWORD onlinenumconfirm = 0;	
	
	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
	{
		PlayerID = pParty->GetMemberID(n);
		if(PlayerID == 0)
			continue; 
		pPlayer[n] = (CPlayer*)g_pUserTable->FindUser(PlayerID);


		if(ChkExp(pPlayer[n], pKilledPosition,GridID) == FALSE) //060628 - Fixed by wonju
			continue;
		
		++onlinenumconfirm;
		curlevel = pPlayer[n]->GetLevel();
		if(maxlevel < curlevel)
			maxlevel = curlevel;
		levelavg += curlevel;		
	}
	
	levelavg /= (float)onlinenumconfirm;
	EXPTYPE partyexp = CalcObtainExp(MonsterLevel,maxlevel,MonsterTotalLife ,TotalDamage);
	//SW050806 ���� ����Ÿ ���̺� �Ľ̿��� ����ȭ�� ����
//	EXPTYPE partyexp = CalcObtainExp( MonsterLevel,maxlevel,MonsterTotalLife, TotalDamage, MonsterExp );
	EXPTYPE exp=0;
	if(partyexp == 0)
		return;
	
	for(n=0;n<MAX_PARTY_LISTNUM;++n)
	{		
		if(ChkExp(pPlayer[n],pKilledPosition,GridID) == FALSE) //060629 - Fixed by wonju
			continue;
				
		curlevel = pPlayer[n]->GetLevel();
		if(onlinenumconfirm != 1)
			exp = (EXPTYPE)(partyexp * ( curlevel * (10 + onlinenumconfirm) / 9.f / (float)levelavg ) 
									/ (float)onlinenumconfirm);
		else
			exp = partyexp;
		if(exp == 0)
			continue;

		// 06. 06. ��Ƽ ����ġ �й� ���� - �̿���
		// �й����� 2���̻��϶��� �߰� ����ġ �ο�
		if(onlinenumconfirm > 1)
		{
			// ��Ƽ����ġ ���� �߰� - RaMa 04.11.24 	
			exp = (EXPTYPE)(exp*gEventRate[eEvent_PartyExpRate]);
		}

		DISTRIBUTERAND->SendToPersonalExp(pPlayer[n], exp);
	}
}
*/

// 070507 LYW --- Distributer : Modified function to clac and send party exp.
//void CDistributer::CalcAndSendPartyExp( CParty* pParty, DWORD TotalDamage, VECTOR3* pKilledPosition, DWORD MonsterTotalLife, DWORD GridID, DWORD KillerID, CMonster* pMob )
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// ��Ƽ ����ġ ó���� �Ѵ�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer[MAX_PARTY_LISTNUM] ;										// ��Ƽ ������� ������ ���� ������ ����.
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )					// �ִ� ��Ƽ ��� �� ��� ����.
//	{
//		pPlayer[count] = NULL ;													// ��� �����͸� NULL ó�� �Ѵ�.
//	}
//
//	DWORD PlayerID = 0 ;														// ������� ���̵� ���� ����.
//	DWORD onlinenumconfirm = 0 ;												// ����ġ�� ���� ��� ��.
//
//	LEVELTYPE levelsum = 0 ;													// ��Ƽ ������� ���� �� ��.
//	LEVELTYPE maxlevel = 0 ;													// ��Ƽ ��� �� �ִ� ����.
//	LEVELTYPE curlevel = 0 ;													// ������� ������ ���� ����.
//	
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)					// �α��� ���� ��Ƽ�� ���� �� �� �ְ� ������ ���Ѵ�.
//	{
//		PlayerID = pParty->GetMemberID(count);									// ī��Ʈ ��� ����� ���̵� �޴´�.
//
//		pPlayer[count] = (CPlayer*)g_pUserTable->FindUser(PlayerID);			// ��� �÷��̾� ������ �޴´�.
//
//		if( pPlayer[count] )													// �÷��̾� ������ ��ȿ���� üũ.
//		{
//			if(ChkExp(pPlayer[count], pKilledPosition,GridID))					// ����ġ�� ���� �� �ִ� ���� ���� �ִ��� üũ.
//			{
//				++onlinenumconfirm;												// ����ġ�� ���� ��� �� ����.
//
//				curlevel = pPlayer[count]->GetLevel();							// ���� ����� ������ �޴´�.
//
//				if(maxlevel < curlevel)											// ��Ƽ �� �ִ� �������� ū�� üũ �Ѵ�.
//				{
//					maxlevel = curlevel;										// �ִ� ������ �����Ѵ�.
//				}
//
//				levelsum += curlevel;											// ��Ƽ ������ ���Ѵ�.
//			}
//		}
//	}
//
//	if( onlinenumconfirm == 0 ) return ;										// �̷� ���� ����� �ϴµ�, Ȥ�ö� ������ �𸣱� ������ ���� ó��.
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// �¶��� ���� ����� �Ѹ��̸�, ��Ƽ ���� ���ϰ� ����� ���� ��� �̰ų�, 
//	// �ٸ� ������� �α׾ƿ��� �����̴�. �Ǵ� �ٸ� �ʿ� �ִ� ����̴�.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm == 1 )													// ����ġ�� ���� �÷��̾� �� üũ.
//	{
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(KillerID);			// �÷��̾� ������ �޾� �´�.
//
//		if( pPlayer )															// �÷��̾� ������ ��ȿ���� üũ.
//		{
//			DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;			// ���� ����ġ�� �޴´�.
//
//			DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;					// ���ο��� ����ġ�� �����Ѵ�.
//
//			return ;															// ���� ó��.
//		}
//	}
//	
//	EXPTYPE partyexp = CalcObtainExp(pMob, maxlevel);							// ��Ƽ �� �ִ� ������ �������� ���� ����ġ�� �޾� �´�.
//
//	if(partyexp == 0) return ;													// �̷� ���� ����� ������, Ȥ�ö� ������ �𸣱� ������ ���� ó��.
//
//	float applyRate = 0.0f ;													// ��Ƽ ���ʽ� ����ġ ����Ʈ.
//
//	switch( onlinenumconfirm )													// ����ġ ���� �÷��̾� �� üũ.
//	{
//	case 2 : applyRate = 0.4f ; break ;											// �θ��̸� +40%
//	case 3 : applyRate = 0.6f ; break ;											// �����̸� +60%
//	case 4 : applyRate = 0.7f ; break ;											// �׸��̸� +70%
//	case 5 : applyRate = 0.8f ; break ;											// �ټ����̸� +80%
//	case 6 : applyRate = 0.9f ; break ;											// �������̸� +90%
//	case 7 : applyRate = 1.0f ; break ;											// �ϰ����̸� +100%
//	}
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)					// �α��� �� ����鿡�� �й� ó���� �Ѵ�.
//	{		
//		if(pPlayer[count])														// ��� ������ ��ȿ�ϴٸ�.
//		{
//			if(ChkExp(pPlayer[count],pKilledPosition,GridID))					// ����ġ�� ���� �� �ִ� ������ �ִ��� üũ.
//			{
//				curlevel = pPlayer[count]->GetLevel();							// ����� ������ �޴´�.
//
//				float fExpA = (float)curlevel / (float)levelsum ;				// ����ġ ����ƮA�� ���Ѵ�.
//				float fExpB = (partyexp * ( 1 + applyRate )) ;					// ����ġ ����ƮB�� ���Ѵ�.
//				
//				float fExp = fExpA * fExpB ;									// ����ġ�� ���Ѵ�.
//
//				if(fExp > 0)													// ����ġ üũ�� �Ѵ�.
//				{
//					DWORD dwExp = (DWORD)GetFloatInt( fExp ) ;					// ����ġ �Ҽ��� ���ϴ� ������.
//					DISTRIBUTERAND->SendToPersonalExp(pPlayer[count],dwExp);	// ����ġ�� �����Ѵ�.
//				}
//			}
//		}
//	}
//}

// 070413 LYW --- Distributer : Modified function CalcObtainExp.
/*
//DWORD CDistributer::CalcObtainExp( LEVELTYPE MonsterLevel, LEVELTYPE KillerLevel, DWORD TotalLife, DWORD Damage, DWORD MonsterExp )
DWORD CDistributer::CalcObtainExp( LEVELTYPE MonsterLevel, LEVELTYPE KillerLevel, DWORD TotalLife, DWORD Damage )
{
	if(KillerLevel >= MonsterLevel + 6)
		return 0;
	else if(KillerLevel <= MonsterLevel - 9)
		MonsterLevel = KillerLevel + 9;

	DWORD ObtainPoint = ATTACKMGR->GetAttackCalc().GetPlayerPoint(MonsterLevel,MonsterLevel - KillerLevel);
//	DWORD ObtainPoint = ATTACKMGR->GetAttackCalc().GetPlayerExpPoint( KillerLevel - MonsterLevel, MonsterExp );
//	DWORD ObtainPoint = ATTACKMGR->
	
//	��)	1		= 100% ������ : ����ġ 100%
//	��)	0.8~1	= 80%~100%�̸� ������ : ����ġ 80%
//	��)	0.6~0.8	= 60%~80% �̸� ������ : ����ġ 60%
//	��)	0.4~0.6			40%~60% �̸� ������ : ����ġ 40%
//	��)	0.2~0.4			20%~40% �̸� ������ : ����ġ 20%
//	��)	0~0.2			20%�̸� : ����ġ ����.

	DWORD perc = Damage*100 / TotalLife;
	if( (100 <= perc) )
	{
		ObtainPoint = ObtainPoint;
	}
	else if( (80 <= perc) && (perc < 100) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.8);
	}
	else if( (60 <= perc) && (perc < 80) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.6);
	}
	else if( (40 <= perc) && (perc < 60) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.4);
	}
	else if( (20 <= perc) && (perc < 40) )
	{
		ObtainPoint = (DWORD)(ObtainPoint*0.2);
	}
	else
	{
		ObtainPoint = 0;
	}

	return ObtainPoint;
}
*/

//// 070413 LYW --- Distributer : ���� ����ġ �г�Ƽ ����
//DWORD CDistributer::CalcObtainExp( CMonster* pMob, LEVELTYPE KillerLevel)
//{
//	LEVELTYPE MonsterLevel = pMob->GetSMonsterList()->Level ;				// ���� ������ �޴´�.
//	LEVELTYPE levelGap = 0 ;												// ���Ϳ� �÷��̾��� ���� ����.
//	
//	if( MonsterLevel > KillerLevel )										// ���� ������ �÷��̾� ���� ū ���.
//	{
//		levelGap = MonsterLevel - KillerLevel ;								// ���� ���̸� ���Ѵ�.
//	}
//	else																	// �÷��̾� ������ ���� ���� ���� ū ���.
//	{
//		levelGap = KillerLevel - MonsterLevel ;								// ���� ���̸� ���Ѵ�.
//	}
//
//	if( MonsterLevel > KillerLevel + 5 )									// ���� ������ �÷��̾� ���� ���� 5���� �̻��� ��.
//	{
//		double dwLevel = KillerLevel+5 ;									// ���� ����Ʈ ����.
//		double dwPenalty = 1.95 ;											// ���Ƽ ����Ʈ ����.
//		double result = pow(dwLevel, dwPenalty) ;							// ���� ����Ʈ ^ ���Ƽ ����Ʈ�� ���Ѵ�.
//
//		float fExp = (float)(result + 15) ;									// �÷�Ʈ���� ũ�� ������ ����ġ�� ���Ѵ�.
//
//		return (DWORD)GetFloatRound(fExp) ;									// ����ġ�� �ݿø� �Ͽ� DWORD������ ��ȯ.
//	}
//	else if( MonsterLevel < KillerLevel )									// �÷��̾� ������ ���� �������� ū ���.
//	{
//		float fPenalty = 1.0f ;												// ���Ƽ ����Ʈ.
//		float fExp     = 0.0f ;												// �÷�Ʈ�� ����ġ.
//		DWORD dwExp	   = pMob->GetSMonsterList()->ExpPoint ;				// ���� ����ġ.
//
//		if( levelGap >= 6 && levelGap <= 10 )								// �÷��̾� ������ +6~+10�� ��.
//		{
//			fPenalty = 0.5f ;												// ���Ƽ ����Ʈ 50%�� ����
//		}
//		else if( levelGap >= 11 && levelGap <= 15 )							// �÷��̾� ������ +11~+15�� ��.
//		{
//			fPenalty = 0.25f ;												// ���Ƽ ����Ʈ 25%�� ����.
//		}
//		else if( levelGap > 15 )											// �÷��̾� ������ +15 �̻��� ��.
//		{
//			return 1 ;														// ����ġ�� ������ 1�� ��ȯ�Ѵ�.
//		}
//
//		fExp = (float)(dwExp * fPenalty) ;									// ����ġ�� �÷�Ʈ������ ���Ѵ�.
//		return (DWORD)GetFloatRound(fExp) ;									// DWORD������ ���Ƽ ����ġ ��ȯ.
//	}	
//
//	return pMob->GetSMonsterList()->ExpPoint ;								// �÷��̾�� ������ ������ ������� ���� ����ġ ��ȯ.
//}

//void CDistributer::GetReceivePartyMember( PARTY_RECEIVE_MEMBER* rtInfo, CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE* pMaxLevel)
//{
//	int count=0;
//	DWORD PlayerID;
//	DAMAGEOBJ* pobj;
//	CPlayer* pPlayer;
//	LEVELTYPE maxlevel = 0;  
//	LEVELTYPE curlevel = 0;
//	
//	for(int n=0;n<MAX_PARTY_LISTNUM;++n)
//	{
//		PlayerID = pParty->GetMemberID(n);
//		if(PlayerID == 0)
//			continue; 
//		pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
//		if(Chk(pPlayer,pKilledPosition,GridID) == FALSE)
//			continue;
//		
////		pobj = m_DamageObjectTableSolo.GetData(PlayerID);
////		if(pobj == 0)
////			continue;
//		
//		rtInfo->pPlayer[count] = pPlayer;
//		pobj = m_DamageObjectTableSolo.GetData(PlayerID);
//		if( pobj )
//			rtInfo->Damage[count] = pobj->dwData;
//		else
//			rtInfo->Damage[count] = 0;
//		count++;
//
//		curlevel = pPlayer->GetMaxLevel();
//		if(maxlevel < curlevel)
//			maxlevel = curlevel;
//	}
//	rtInfo->count = count;
//	*pMaxLevel = maxlevel;
//}

//void CDistributer::SendPersonalAbilandItem(CPlayer* pReceivePlayer, CMonster* pMonster, DWORD DropItemID, DWORD DropItemRatio)
//{
//	if(pReceivePlayer == 0)
//	{		
//		Release();
//		return;
//	}
//	//����ġ�� 0�̸� Ư�� ����ġ�� ���ش�. (ex: ȣ���̺�Ʈ)
//	if(pMonster->GetSMonsterList()->ExpPoint)
///*�����Ƽ ���� - 		DISTRIBUTERAND->SendToPersonalAbil(pReceivePlayer, pMonster->GetLevel());*/
//	DISTRIBUTERAND->SendToPersonalItem(pReceivePlayer, DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMonster->GetSMonsterList(), pMonster->GetMonsterKind());
//}

//// 070419 LYW --- Distributer : Add function to requite.
//void CDistributer::PickUpAcquisition(CPlayer* pReceivePlayer, CMonster* pMonster, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, int onlinenumconfirm, WORD dwCurMap)
//{
//	if(!pReceivePlayer)																	// �޴� �÷��̾��� ������ üũ.
//	{		
//		Release();																		// �ʱ�ȭ ó���� �Ѵ�.
//		return;
//	}
//
//	WORD MonsterKind = pMonster->GetMonsterKind() ;										// ���� ������ �޴´�.
//
//	// ��� �Ӵϸ� �޴´�.
//	MONEYTYPE money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator((MONSTEREX_LIST *)pMonster->GetSMonsterList(), pReceivePlayer, MonsterKind);
//    
//	CParty* pParty = PARTYMGR->GetParty( pReceivePlayer->GetPartyIdx() ) ;				// ��Ƽ ������ �޴´�. 
//
//	if( pParty )																		// ��Ƽ ������ ��ȿ���� üũ
//	{
//		// ���� �� �ִ� ����� üũ �̺κ��� ���ڷ� �޾� ����.
//		//BYTE onlinenumconfirm = 0 ;														
//		//DWORD PlayerID = 0 ;
//
//		//CPlayer* pPlayer = NULL ;
//
//		//for(BYTE n=0;n<MAX_PARTY_LISTNUM;++n)
//		//{
//		//	PlayerID = pParty->GetMemberID(n);
//		//	if(PlayerID == 0)
//		//		continue; 
//		//	pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
//
//		//	if(ChkExp(pPlayer, pKilledPosition,pMonster->GetGridID()) == FALSE) 
//		//		continue;
//
//		//	++onlinenumconfirm;
//		//}
//
//		if( onlinenumconfirm == 1 )														// �޴� ����� �Ѹ��̸� ���ο��� �����Ѵ�.
//		{
//			if( pReceivePlayer->SetMoney(money, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != money )
//			{
//				// error msg ������. ���ѷ� �ʰ�
//				MSGBASE msg;
//				msg.Category = MP_ITEM;
//				msg.Protocol = MP_ITEM_MONEY_ERROR;
//				msg.dwObjectID = pReceivePlayer->GetID();
//				
//				pReceivePlayer->SendMsg(&msg, sizeof(msg));
//			}
//
//			// ���ο��� �������� �����Ѵ�.
//			//		for( BYTE count = 0 ;  count < 5 ; ++count )
//			//		{
//			//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//			//		}
//			return ;
//		}
//
//		MONEYTYPE partyMoney = money/onlinenumconfirm ;									// ������ ��Ƽ���� ������ ���� �ݾ��� ���Ѵ�.
//
//		for(BYTE n=0;n<MAX_PARTY_LISTNUM;++n)											// ��Ƽ ��� �� ��� ������ ����.
//		{
//			PlayerID = pParty->GetMemberID(n);											// ��� ���̵� �޴´�.
//
//			pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);						// ��� ������ �޴´�.
//
//			if( pPlayer )																// ��� ������ ��ȿ���� üũ�Ѵ�.
//			{
//				DWORD dwMapNum = pPlayer->GetCurMapNum() ;								// ����� ���� �� ��ȣ�� �޴´�.
//
//				if( wMapNum == dwCurMap )												// ����� ���� ���Ͱ� ó���� �ʰ� ������ ��.
//				{
//					if(ChkExp(pPlayer, pKilledPosition,pMonster->GetGridID()))			// ����� ���Ͱ� ó���� �������� �ִ��� ��.
//					{
//						// ������� ȹ�� �Ӵϸ� 1/n �� �Ӵϸ� �����Ѵ�.
//						if( pPlayer->SetMoney(partyMoney, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != partyMoney )
//						{
//							MSGBASE msg;
//							msg.Category = MP_ITEM;
//							msg.Protocol = MP_ITEM_MONEY_ERROR;
//							msg.dwObjectID = pPlayer->GetID();
//							
//							pPlayer->SendMsg(&msg, sizeof(msg));
//						}
//					}
//				}
//			}
//		}
//
//		// ���ʸ� �����ؼ� �������� �����Ѵ�.
//		// ���ο��� �������� �����Ѵ�.
//		//		for( BYTE count = 0 ;  count < 5 ; ++count )
//		//		{
//		//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//		//		}
//	}
//	else 
//	{
//		if( pReceivePlayer->SetMoney(money, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != money )
//		{
//			// error msg ������. ���ѷ� �ʰ�
//			MSGBASE msg;
//			msg.Category = MP_ITEM;
//			msg.Protocol = MP_ITEM_MONEY_ERROR;
//			msg.dwObjectID = pReceivePlayer->GetID();
//			
//			pReceivePlayer->SendMsg(&msg, sizeof(msg));
//		}
//
//		// ���ο��� �������� �����Ѵ�.
//		// ���ο��� �������� �����Ѵ�.
//		//		for( BYTE count = 0 ;  count < 5 ; ++count )
//		//		{
//		//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//		//		}
//	}
//}

//void CDistributer::SendPartyAbilandItem(CParty* pParty, VECTOR3* pKilledPosition, DWORD GridID, LEVELTYPE MonsterLevel,
//										DWORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind)
//{
//	if(!pParty)
//	{
//		Release();
//		return;
//	}
//	PARTY_RECEIVE_MEMBER MemberInfo;
//	LEVELTYPE MaxLevel = 1;
//	GetReceivePartyMember(&MemberInfo, pParty, pKilledPosition, GridID, &MaxLevel);
//
//	//����ġ�� 0�̸� Ư�� ����ġ�� ���ش�. (ex: ȣ���̺�Ʈ)
//	if(pMonInfo->ExpPoint)
///*�����Ƽ ���� - 		pParty->SendAbil(MonsterLevel, &MemberInfo, MaxLevel);*/
//	pParty->SendItem(&MemberInfo, DropItemId, DropItemRatio, pMonInfo, MonsterKind, MaxLevel);
//}


//DWORD CDistributer::GetTotalDamage()
//{
//	return m_TotalDamage;
//}
