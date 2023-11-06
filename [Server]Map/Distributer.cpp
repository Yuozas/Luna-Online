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
// 필드보스 - 05.12 이영준
#include "FieldBossMonsterManager.h"
#include "ItemManager.h"

// 070413 LYW --- Distributer : Include CommonCalcFunc.
#include "..\[CC]Header\CommonCalcFunc.h"
#include "ItemDrop.h"

// 080116 LYW --- Distributer : 데이트 매니져 헤더 포함.
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
//float gDamageReciveRate	= 1.0f;		// 받는 데미지
//float gDamageRate		= 1.0f;		// 주는 데미지
//float gNaeRuykRate	= 1.0f;		// 내력소모
//float gUngiSpeed	= 1.0f;		// 운기조식 스피드
//float gPartyExpRate	= 1.0f;		// 파티경험치
//float gAbilRate		= 1.0f;		// 특기치
//float gGetMoney		= 1.0f;		// 얻는돈의 배수
//float gSkillExpRate = 1.0f;	// 무공경험치 배수
//// Etc - 아직안됨
//float gDefence		= 1.0f;		// 방어력
//
float gEventRate[eEvent_Max];
float gEventRateFile[eEvent_Max];
BOOL g_bPlusTime = FALSE;


CDistributer::CDistributer()																	// 생성자 함수.
{
	m_DamageObjectTableSolo.Initialize(MAX_POINTACCEPTOBJECT_NUM);								// 개인의 데미지를 담은 테이블 초기화.
	m_DamageObjectTableParty.Initialize(MAX_POINTACCEPTOBJECT_NUM);								// 파티의 데미지를 담은 테이블 초기화.

	m_1stPartyID = 0;																			// 첫 데미지를 준 파티 아이디 초기화.
	m_1stPlayerID = 0;																			// 첫 데미지를 준 플레이어 아이디 초기화.
	m_PlusDamage = 0;																			// 추가 데미지 초기화.						
	m_TotalDamage = 0;																			// 데미지 합계 초기화.
	m_FirstDamage = 0 ;																			// 첫 데미지 초기화.
	m_nTotalKillerCount = 0 ;																	// 킬러 수 초기화.	
	m_HighLevel = 0 ;																			// 킬러 들 중 가장 레벨이 높은 킬러의 레벨.

	m_pKiller			= NULL ;																// 킬러 정보를 담는 포인터 초기화.
	m_pKillerParty		= NULL ;																// 킬러의 파티 정보를 담는 포인터 초기화.

	m_pMob				= NULL ;																// 몬스터 정보를 담는 포인터 초기화.
	m_pKilledPosition	= NULL ;																// 몬스터를 잡은 위치 포인터 초기화.

	m_dwDropItemID		= 0 ;																	// 드랍 아이템 아이디를 담는 변수 초기화.
	m_dwDropItemRatio	= 0 ;																	// 드랍률을 담는 변수 초기화.								
}

CDistributer::~CDistributer()																	// 소멸자 함수.
{
	Release();																					// 해제 함수를 호출한다.
}

void CDistributer::Release()																	// 해제 함수.
{
	m_1stPartyID = 0;																			// 첫 데미지를 준 파티 아이디 초기화.
	m_1stPlayerID = 0;																			// 첫 데미지를 준 플레이어 아이디 초기화.
	m_PlusDamage = 0;																			// 추가 데미지 초기화.		
	m_TotalDamage = 0;																			// 데미지 합계 초기화.
	m_FirstDamage = 0 ;																			// 첫 데미지 초기화.
	m_nTotalKillerCount = 0 ;																	// 킬러 수 초기화.
	m_HighLevel = 0 ;																			// 킬러 들 중 가장 레벨이 높은 킬러의 레벨.

	DAMAGEOBJ *sobj, *pobj = NULL;																// 개인 및 파티 데미지 테이블의 포인터.

	m_DamageObjectTableSolo.SetPositionHead();													// 개인 데미지 테이블을 헤더로 세팅한다.

	while(sobj = m_DamageObjectTableSolo.GetData())												// 데미지 오브젝트가 없을 때 까지 while.
	{
		delete sobj;																			// 데미지 오브젝트를 삭제한다.
		sobj = NULL ;																			// 데미지 오브젝트 NULL처리.
	}

	m_DamageObjectTableSolo.RemoveAll();														// 데미지 오브젝트를 비운다.				

	m_DamageObjectTableParty.SetPositionHead();													// 파티 데미지 테이블을 헤더로 세팅한다.

	while(pobj = m_DamageObjectTableParty.GetData())											// 데미지 오브젝트가 없을 때 까지 while.		
	{																							
		delete pobj;																			// 데미지 오브젝트를 삭제한다.
		pobj = NULL ;																			// 데미지 오브젝트 NULL처리.
	}																							
																								
	m_DamageObjectTableParty.RemoveAll();														// 데미지 오브젝트를 비운다.					

	m_pKiller			= NULL ;																// 킬러 정보를 담는 포인터 초기화.
	m_pKillerParty		= NULL ;																// 킬러의 파티 정보를 담는 포인터 초기화.

	m_pMob				= NULL ;																// 몬스터 정보를 담는 포인터 초기화.
	m_pKilledPosition	= NULL ;																// 몬스터를 잡은 위치 포인터 초기화.

	m_dwDropItemID		= 0 ;																	// 드랍 아이템 아이디를 담는 변수 초기화.
	m_dwDropItemRatio	= 0 ;																	// 드랍률을 담는 변수 초기화.							

	PTRLISTPOS pos = m_CandidacyList.GetHeadPosition() ;										// 파티에서 사용한 후보자를 담는 리스트를 해드로 세팅한다.

    DWORD* pMemberID = NULL ;																	// 멤버 아이디를 받을 포인터.

	while(pMemberID = (DWORD*)m_CandidacyList.GetNext(pos))										// 리스트에서 포인터를 받는다.
	{
		if(pMemberID)																			// 포인터 정보가 유효한지 체크한다.
		{
			delete pMemberID ;																	// 포인터에 있는 정보를 지운다.
			pMemberID = NULL ;																	// 포인터를 NULL 처리 한다.
		}
	}

	m_CandidacyList.RemoveAll() ;																// 리스트를 비운다.
}

void CDistributer::AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage)
{
	if( pPlayer ) 																				// 플레이어 정보가 유효한지 체크한다.
	{
		CParty* pParty = NULL ;																	// 파티 여부를 체크하기 위해 파티 포인터를 선언.
		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;									// 파티 정보를 받는다.

		if( pParty )																			// 파티정보가 유효한지 체크한다.
		{
			DoAddDamageObj(&m_DamageObjectTableParty,											// 데미지를 파티 데미지 테이블에 추가한다.
				pPlayer->GetPartyIdx(), damage+plusdamage);
		}

		DoAddDamageObj(&m_DamageObjectTableSolo,												// 데미지를 개인 데미지 테이블에 추가한다.
			pPlayer->GetID(), damage+plusdamage);	

		//if(plusdamage != 0)																		// 추가 데미지가 있는지 체크한다.
		if( m_1stPlayerID == 0 )
		{
			m_PlusDamage = plusdamage;															// 추가 데미지를 받는다.

			m_1stPlayerID = pPlayer->GetID();													// 첫 데미지를 준 플레이어 아이디를 받는다.
			m_1stPartyID = pPlayer->GetPartyIdx();												// 첫 데미지를 준 플레이어의 파티 인덱스를 받는다.
		}
		
		SetPlusTotalDamage(damage+plusdamage);													// 데미지 합계를 갱신한다.								
	}
}

void CDistributer::DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage)
{
	if( !pTable ) return ;																		// 들어온 데미지 테이블의 정보가 유효한지 체크한다.

	DAMAGEOBJ* pDObj;																			// 데미지 오브젝트 포인터를 선언한다.

	pDObj = pTable->GetData(dwID);																// 데미지 테이블에서 아이디에 해당하는 데미지 오브젝트를 받는다.

	if(pDObj != NULL)																			// 데미지 오브젝트가 유효한지 체크한다.
	{
		pDObj->dwData += damage;																// 데미지 오브젝트의 데미지에 들어온 데미지를 추가한다.
	}
	else
	{
		DAMAGEOBJ * newObj = new DAMAGEOBJ;														// 데미지 오브젝트가 없으면 새로 생성한다.

		newObj->dwID = dwID;																	// 데미지 오브젝트에 아이디를 세팅한다.
		newObj->dwData = damage;																// 데미지 오브젝트에 데미지를 세팅한다.
		
		pTable->Add(newObj, newObj->dwID);														// 데미지 테이블에 추가한다.											
	}
}


BOOL CDistributer::Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	if( !pKilledPosition ) return FALSE ;														// 들어온 위치 포인터의 정보가 유효한지 체크한다.

	VECTOR3* PlayerPosition;																	// 플레이어의 위치정보를 담는 포인터 선언.

	if( pPlayer )																				// 플레이어의 정보가 유효한지 체크한다.
	{
		if( pPlayer->GetInitState() == PLAYERINITSTATE_INITED )									// 플레이어의 초기화 상태가 PLAYERINITSTATE_INITED와 같은지 비교한다.
		{
			if( !pPlayer->IsShowdown() )														// 플레이어가 대결중인지 체크한다.
			{
				PlayerPosition = CCharMove::GetPosition(pPlayer) ;								// 플레이어의 위치정보를 받는다.										

				float fDistance = CalcDistanceXZ( pKilledPosition, PlayerPosition ) ;			// 몬스터가 죽은 위치와 플레이어의 위치 거리를 구한다.

				if( fDistance <= POINT_VALID_DISTANCE )											// 주어진 영역 안인지 체크한다.
				{
					if( pPlayer->GetState() != eObjectState_Die )								// 플레이어가 죽은 상태인지 체크한다.
					{
						if( pPlayer->GetGridID() == GridID )									// 그리드를 비교한다.
						{
							return TRUE ;														// TRUE 리턴.										
						}
					}
				}
			}
		}
	}

	return FALSE ;																				// FALSE 리턴.
}


BOOL CDistributer::ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID)
{
	if( !pKilledPosition ) return FALSE ;														// 들어온 위치 포인터의 정보가 유효한지 체크한다.

	if( pPlayer )																				// 플레이어 정보가 유효한지 체크한다.
	{
		if( pPlayer->GetInitState() == PLAYERINITSTATE_INITED )									// 플레이어 초기화 상태가 PLAYERINITSTATE_INITED와 같은지 비교한다.
		{
			if( !pPlayer->IsShowdown() )														// 플레이어가 대결중인지 비교한다.
			{
				if( pPlayer->GetState() != eObjectState_Die )									// 플레이어가 죽은 상태인지 체크한다.
				{
					if( pPlayer->GetGridID() == GridID )										// 그리드를 비교 한다.
					{
						return TRUE ;															// TRUE 리턴.														
					}
				}
			}
		}
	}

	return FALSE ;																				// FALSE 리턴.
}

void CDistributer::ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID)
{
	if( !pobj || !pBigDamage || !pBigID ) return ;												// 함수 인자로 들어오는 포인터들의 정보가 유효한지 체크한다.

	if(*pBigDamage < pobj->dwData)																// 데미지 오브젝트의 데미지가 최대 데미지보다 크면.
	{
		*pBigDamage = pobj->dwData;																// 최대 데미지에 데미지 오브젝트의 데미지를 세팅한다.
		*pBigID = pobj->dwID;																	// 최대 데미지 아이디에 데미지 오브젝트의 아이디를 세팅한다.
	}
	else if(*pBigDamage == pobj->dwData)														// 데미지 오브젝트의 데미지가 최대 데미지와 같다면.
	{
		int rate = rand() % 2;																	// 확률을 구한다.

		if(rate == 1)																			// 반의 확률로
		{
			*pBigID = pobj->dwID;																// 데미지 오브젝트의 아이디를 최대 데미지 아이디로 세팅한다.
		}
	}
}

// 필드보스 - 05.12 이영준
// 필드보스용 아이템 분배 함수
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
			// 드랍 아이템 정보가 있는가? 없다면 종료
			if( pDropItemList->m_ItemList[ index ].m_wItemIndex[0] == 0 )
				break;
	        
			// 아이템 그룹 내의 어떤 아이템을 줄지 결정
			for( WORD dropitemnum = 0; dropitemnum < MAX_FIELDBOSS_DROPITEM_NUM; dropitemnum++)
			{
				if(pDropItemList->m_ItemList[ index ].m_wItemIndex[dropitemnum] == 0)
					break;
			}

			WORD select = rand() % dropitemnum;

			// 드랍될 횟수를 가져온다
			WORD count = pDropItemList->m_ItemList[ index ].m_wCount;

			for(WORD i = 0; i < count; i++)
			{
				// 드랍 할것인가?
				if( (rand() % 100) < pDropItemList->m_ItemList[ index ].m_wDropRate )
				{
					// 변수초기화
					memset( TargetPlayerTable, 0, sizeof(CPlayer*) * 100 );
					TargetPos = 0;

					// 공격한 사람들을 검색해서 기준치 이상 데미지를 준 사람들 가려낸다
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
						// 몬스터와 +6 이상 혹은 -9 이하의 Player는 분배에서 제외
						if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
							continue;
						//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
						//	continue;
						
						// 데미지 비율 계산
						WORD DamageRate = (WORD)(((double)obj->dwData / m_TotalDamage) * 100);

						// 기준치 이상 데미지를 주었다면
						if( DamageRate >= pDropItemList->m_ItemList[ index ].m_wDamageRate )
						{
							// 데미지 비율만큼 대상 테이블에 등록한다
							for(DWORD rate = 0; rate < DamageRate; rate++)
							{
								TargetPlayerTable[ TargetPos++ ] = pReceivePlayer;
							}
						}
					}

					// 대상 테이블에 등록된 유저가 있다면
					if( TargetPos > 0 )
					{
						// 랜덤으로 테이블 위치를 결정한다
						WORD TargetIndex = rand() % TargetPos;

						// 결정된 위치에 등록된 유저를 가져온다
						CPlayer* TargetPlayer = TargetPlayerTable[ TargetIndex ];

						// 유저 정보가 유효하면 아이템을 넣어준다
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
	if( pMob )															// 몬스터 정보가 유효한지 체크.
	{
		DAMAGEOBJ *obj = NULL;											
																		
		CPlayer* pReceivePlayer = NULL;;								
							
		// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
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
			//얘네도 준다.
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

			// 필드보스 - 05.12 이영준
			// 필드보스의 경험치 수련치 돈 분
			if(pMob->GetObjectKind() == eObjectKind_FieldBossMonster)
			{
				// 몬스터와 +6 이상 혹은 -9 이하의 Player는 분배에서 제외
				if(pReceivePlayer->GetLevel() > pMob->GetSMonsterList()->Level + 6 )
					continue;
				//if(pReceivePlayer->GetLevel() < pMob->GetSMonsterList()->Level - 9 )
				//	continue;

				if(Exp == 0) Exp = 1;
				if(Abil == 0) Abil = 1;
				if(Money == 0) Money = 1;

				pReceivePlayer->AddPlayerExpPoint(Exp);
				/*어빌리티 삭제 - pReceivePlayer->AddAbilityExp(Abil);*/
				DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
			}
/*			else if(pMob->GetMonsterKind() == eBOSSKIND_EMPERIORTOMB_EVENT)
			{
				if(Exp == 0) Exp = 1;
				if(Abil == 0) Abil = 1;
				if(Money == 0) Money = 1;

				pReceivePlayer->AddPlayerExpPoint(Exp);
				/*어빌리티 삭제 - pReceivePlayer->AddAbilityExp(Abil);*//*
				DISTRIBUTERAND->SendToPersonalMoney(pReceivePlayer, Money, MonsterKind);
			}*/
			else
			{
				if(Exp >= 10000)
					pReceivePlayer->AddPlayerExpPoint(Exp);
				/*어빌리티 삭제 - if(Abil >= 50000)
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
	m_TotalDamage += Damage;																	// 데미지 합계에 주어진 데미지를 추가.
}

void CDistributer::DamageInit()
{
	m_1stPartyID = 0;																			// 첫 데미지를 준 파티 아이디 초기화.
	m_1stPlayerID = 0;																			// 첫 데미지를 준 플레이어 아이디 초기화.
	m_PlusDamage = 0;																			// 추가 데미지 초기화.		
	m_TotalDamage = 0;																			// 데미지 합계 초기화.
	m_FirstDamage = 0 ;																			// 첫 데미지 초기화.
	m_nTotalKillerCount = 0 ;																	// 킬러 수 초기화.
	m_HighLevel = 0 ;																			// 킬러 들 중 가장 레벨이 높은 킬러의 레벨.

	m_DamageObjectTableSolo.RemoveAll();														// 데미지 오브젝트 테이블을 모두 비운다.	

	m_pKiller			= NULL ;																// 킬러 정보를 담는 포인터 초기화.
	m_pKillerParty		= NULL ;																// 킬러의 파티 정보를 담는 포인터 초기화.

	m_pMob				= NULL ;																// 몬스터 정보를 담는 포인터 초기화.
	m_pKilledPosition	= NULL ;																// 몬스터를 잡은 위치 포인터 초기화.

	m_dwDropItemID		= 0 ;																	// 드랍 아이템 아이디를 담는 변수 초기화.
	m_dwDropItemRatio	= 0 ;																	// 드랍률을 담는 변수 초기화.							
}

void CDistributer::DeleteDamagedPlayer(DWORD CharacterID)
{	
	DAMAGEOBJ* pData = NULL ;																	// 데미지 오브젝트 정보를 담을 포인터를 선언 및 NULL 초기화.

	pData = m_DamageObjectTableSolo.GetData( CharacterID );										// 받은 아이디로 데미지 테이블에서 데미지 오브젝트 포인터를 받는다.

	if( pData )																					// 데미지 오브젝트 포인터가 유효한지 체크한다.
	{
		if( m_TotalDamage >= pData->dwData )													// 포인터의 데미지가 데미지 합계보다 크거나 같으면,
		{
			m_TotalDamage -= pData->dwData;														// 데미지 합계에서 포인터의 데미지 만큼 뺀다.
		}
		else																					// 포인터의 데미지가 데미지 합계보자 작으면
		{
			m_TotalDamage = 0;																	// 합계 데미지를 0으로 세팅한다.
		}
	} 

	m_DamageObjectTableSolo.Remove(CharacterID);												// 데미지 오브젝트 테이블에서 주어진 아이디의 데미지 오브젝트를 비운다.
}

BOOL CDistributer::SetInfoToDistribute(DWORD dwKillerID, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
{
	if( !pKilledPosition || !pMob ) return FALSE ;												// 함수 인자로 넘어온 정보들이 유효한지 체크한다.

	CObject* pKiller = g_pUserTable->FindUser(dwKillerID);

	if( pKiller->GetObjectKind() == eObjectKind_Player )
	{
	m_pKiller = (CPlayer *)pKiller;									// 킬러의 정보를 받는다.
	}
	else if( pKiller->GetObjectKind() == eObjectKind_Pet )
	{
		m_pKiller = ((CPet *)pKiller)->GetMaster();									// 킬러의 정보를 받는다.
	}

	if( m_pKiller )																				// 킬러의 정보가 유효한지 체크한다.
	{
		m_pKillerParty = PARTYMGR->GetParty(m_pKiller->GetPartyIdx());							// 킬러의 파티 정보를 받는다.
	}

	m_pMob				= pMob ;																// 몬스터 정보를 담는 포인터를 받는다.
	m_pKilledPosition	= pKilledPosition ;														// 몬스터를 잡은 위치 포인터를 받는다.

	m_dwDropItemID		= DropItemID ;															// 드랍 아이템 아이디를 담는 변수를 받는다.
	m_dwDropItemRatio	= DropItemRatio ;														// 드랍률을 담는 변수를 받는다.									

	return TRUE ;
}		

void CDistributer::Distribute()
{
	DAMAGEOBJ *obj = NULL ;																		// 데미지 오브젝트.

	int nKillerCount = 0 ;																		// 몬스터를 처치 한 플레이어 수.

	GetHighLevelOfKillers() ;																	// 킬러들 중 최고레벨을 구한다.

	GetTotalKillerCount() ;																		// 공격자 총 수를 구한다.

	if( m_nTotalKillerCount > 1 )																// 몬스터를 처치 한 플레이어가 다수라면, 
	{
		DistributeToKillers() ;																	// 다수 처리를 한다.
	}
	else																						// 몬스터를 처치 한 플레이어가 한명이라면, 
	{
		DistributeToKiller() ;																	// 개인 처리를 한다.
	}

	Release();																					// 데미지 테이블 등 모든 참조 정보를 초기화 한다.
}

void CDistributer::DistributeToKillers()
{
	if(!GetFirstDamange())																		// 선공격자 데미지를 구한다.
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
		// 080801 LYW --- Distributer : 첫 데미지를 준 사람이 없어도, 
		// 다른 사람들이 분배를 받을 수 있도록 return 처리를 주석 처리한다.
		//return;
	}

	MONEYTYPE money = 0 ;																		// 획득할 몬스터 머니를 구한다.

	int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;									// 몬스터와의 레벨 차이에 따른 패널티 값을 받는다.

	money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(										// 패널티를 적용한 머니를 받는다.
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

	DistributerToSolo(money) ;																	// 개인 경험치와 머니 처리를 한다.

	DistributeToPartys(money) ;																	// 파티 경험치와 머니 처리를 한다.

	DistributeItemToKillers() ;																	// 아이템 처리를 한다.
}

void CDistributer::DistributerToSolo(MONEYTYPE money)
{
	DAMAGEOBJ* obj = NULL ;																		// 데미지 정보를 받을 포인터 선언.

	CPlayer* pPlayer = NULL ;																	// 플레이어 정보를 받을 포인터 선언.

	CParty* pParty = NULL ;																		// 파티 정보를 받을 포인터 선언.

	m_DamageObjectTableSolo.SetPositionHead() ;													// 데미지 테이블을 해드로 세팅한다.

	while(obj = m_DamageObjectTableSolo.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// 데미지를 준 플레이어 정보를 받는다.

				if( pPlayer )																	// 플레이어 정보가 유효한지 체크한다.
				{
					// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
					if( !IsSameChannel(pPlayer, m_pMob) ) continue ;

					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// 플레이어의 파티 정보를 받는다.

					if( !pParty )																// 파티가 없다면, 
					{
						if( pPlayer->GetID() == m_1stPlayerID )									// 첫 데미지를 준 플레이어라면, 
						{
							DistributeToFirstKiller(pPlayer, obj->dwData) ;						// 선 공격자 경험치 처리를 한다.
						}
						else																	// 첫 데미지를 준 플레이어가 아니라면,
						{
							DistributeToOtherKiller(pPlayer, obj->dwData) ;						// 후 공격자 경험치 처리를 한다.
						}

						float fDamageRate = (float)obj->dwData / (float)(m_TotalDamage+(m_FirstDamage*0.3f))  ;		// 총 데미지에서 플레이어의 데미지 백분율을 구한다.

						float fMoney = money * fDamageRate ;									// 몬스터가 드랍한 머니에서 데미지 비율로 머니를 받는다.

						MONEYTYPE dwMoney = (DWORD)fMoney ;										// 소수 버림으로 머니 결정.
						SendMoneyToPerson(pPlayer, dwMoney) ;									// 머니를 전송한다.
					}
				}
			}
		}
	}
}

void CDistributer::DistributeToFirstKiller(CPlayer* pPlayer, DWORD dwMyDamage)
{
	// 선 공격자 경험치 처리.
	// 획득 경험치[선공격자] = ((자신의 데미지 + (자신의 데미지 *0.3)) / 모든 유저의 데미지 총합) * 몬스터 경험치.

	if( !pPlayer ) return ;	

	// 080110 LYW --- Distributer : 개인 경험치 처리, 죽었을 때 처리 하지 않도록 수정.
	if(pPlayer->GetState() == eObjectState_Die) return ;

	// 080108 LYW --- Distributer : 몬스터 경험치 받아오는 부분 수정.
	//DWORD dwMonsterExp = CalcObtainExp(m_HighLevel, 1) ;									// 몬스터 경험치.
	// 080530 LYW --- Distributer : Modified data type of experience to DWORD.
	//DWORD dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// 몬스터 경험치.
	DWORD dwMonsterExp = 0 ;
	dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;

	if( m_TotalDamage < 1 )																	// 토탈 데미지가 1보다 작으면,
	{
		m_TotalDamage = 1 ;																	// 토탈 데미지를 1로 세팅한다.
	}

	float fExpVal_A = (float)(dwMyDamage * 0.3f) ;											// 자신의 데미지 * 0.3
	float fExpVal_B = (float)(dwMyDamage + fExpVal_A) ;										// 자신의 데미지 + fExpVal_A
	float fExpVal_C = (float)(fExpVal_B / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_B / 모든 유저의 데미지 총합
	float fExpVal_D = (float)(fExpVal_C * dwMonsterExp) ;									// fExpVal_C * 몬스터 경험치

	// 080102 LYW --- Distributer : 솔로 플레이어 경험치 추가 처리.
	DWORD dwCurExp = (DWORD)floor(fExpVal_D) ;
	DWORD dwExp = dwCurExp + (DWORD)(dwCurExp*0.15f) ;												// 반올림 한 경험치를 받는다.

	DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// 플레이어에게 경험치를 전송한다.	
	if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
	{
		if( pPlayer->GetCurPet() )
			pPlayer->GetCurPet()->AddExp();
	}
	// desc_hseos_패밀리01
	// S 패밀리 추가 added by hseos 2007.07.15
	// ..패밀리 명예 포인트 처리
	g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
	// E 패밀리 추가 added by hseos 2007.07.15
}

void CDistributer::DistributeToOtherKiller(CPlayer* pPlayer, DWORD dwMyDamage)
{
	// 후 공격자 경험치 처리.
	// 획득 경험치[후공격자] = (자신의 데미지 - ((선공격자 데미지 * 0.3) / (공격자 총수 - 1))) / 모든 유저의 데미지 총합) * 몬스터 경험치.

	if( pPlayer )																				// 플레이어의 정보가 유효한지 체크한다.
	{
		// 080108 LYW --- Distributer : 몬스터 경험치 받아오는 부분 수정.
		//DWORD dwMonsterExp = CalcObtainExp(m_HighLevel, 1) ;									// 몬스터 경험치.
		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//DWORD dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// 몬스터 경험치.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(pPlayer->GetLevel(), 1) ;


		int nKillerCount = 1 ;																	// 킬러 카운트를 세팅한다.

		if( m_nTotalKillerCount > 1 )															// 킬러 카운트가 한명이상이면,
		{
			nKillerCount = m_nTotalKillerCount-1 ;												// 킬러 카운트를 다시 세팅한다.
		}
		else																					// 혹시나 잘 못 넘어올, 킬러 카운트 수 체크.
		{
			nKillerCount = 1 ;																	// 킬러 카운트를 1로 세팅한다.
		}

		if( m_TotalDamage < 1 )																	// 토탈 데미지가 1보다 작다면,
		{
			m_TotalDamage = 1 ;																	// 토탈 데미지를 1로 세팅한다.
		}

		float fExpVal_A = (float)(m_FirstDamage * 0.3f) ;										// 선공격자 데미지 * 0.3
		float fExpVal_B = (float)(fExpVal_A / (nKillerCount)) ;									// fExpVal_A / (공격자 총수 - 1)
		if( fExpVal_B <= 1 ) fExpVal_B = 1 ;
		float fExpVal_C = (float)(dwMyDamage - fExpVal_B) ;										// 자신의 데미지 - fExpVal_B
		if( fExpVal_C <= 1 ) fExpVal_C = 1 ;
		float fExpVal_D = (float)(fExpVal_C / (m_TotalDamage+fExpVal_A)) ;									// fExpVal_C / 모든 유저의 데미지 총합
		float fExpVal_E = (float)(fExpVal_D * dwMonsterExp) ;									// fExpVal_D * 몬스터 경험치.

		// 080102 LYW --- Distributer : 솔로 플레이어 경험치 추가 처리.
		DWORD dwCurExp = (DWORD)floor(fExpVal_E) ;
		DWORD dwExp = dwCurExp + (DWORD)(dwCurExp*0.15f) ;											// 반올림 한 경험치를 받는다.

		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// 플레이어에게 경험치를 전송한다.
		if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
		{
			if( pPlayer->GetCurPet() )
				pPlayer->GetCurPet()->AddExp();
		}
		// desc_hseos_패밀리01
		// S 패밀리 추가 added by hseos 2007.07.15
		// ..패밀리 명예 포인트 처리
		g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
		// E 패밀리 추가 added by hseos 2007.07.15
	}
}

void CDistributer::DistributeToPartys(MONEYTYPE money)
{
	DAMAGEOBJ* obj = NULL ;																		// 데미지 정보를 받을 포인터 선언.
	CParty* pParty = NULL ;																		// 파티 정보를 받을 포인터 선언.

	m_DamageObjectTableParty.SetPositionHead() ;												// 데미지 테이블을 해드로 세팅한다.

	while(obj = m_DamageObjectTableParty.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// 파티 정보를 받는다.

				if( pParty )																	// 파티 정보가 유효한지 체크한다.
				{
					if( pParty->GetPartyIdx() == m_1stPartyID )									// 첫 데미지를 준 파티라면, 
					{
						DistributeToFirstParty(pParty, obj->dwData) ;							// 선 공격파티 처리를 한다.
					}
					else																		// 첫 데미지를 준 파티가 아니라면,
					{
						DistributeToOtherParty(pParty, obj->dwData) ;							// 후 공격파티 처리를 한다.
					}

					float fDamageRate = (float)obj->dwData / (float)(m_TotalDamage+(m_FirstDamage*0.3f))  ;			// 전체 데미지에 대한 파티 데미지의 백분율을 구한다.

					float fMoney = money * fDamageRate ;										// 몬스터가 드랍한 머니에서 데미지 비율만큼 머니를 구한다.

					SendMoneyToPartys(fMoney) ;													// 파티로 머니를 전송한다.
				}
			}
		}
	}
}

void CDistributer::DistributeToFirstParty(CParty* pParty, DWORD dwPartyDamage)
{
	// 선 공격 파티 경험처리.
	// 획득 경험치[선공격파티] = ((자신의 파티 데미지 + (자신의 파티 데미지 * 0.3)) / 모든 파티의 데미지 총합) * 몬스터 경험치.

	if( pParty )																				// 파티 정보가 유효한지 체크한다.
	{
		LEVELTYPE highLevel = 0 ;																// 파티 멤버 중 최대 레벨.
		LEVELTYPE totalLevel = 0 ;																// 멤버들의 레벨을 받을 변수.
		LEVELTYPE curlevel = 0 ;																// 멤버들의 현재 레벨을 받을 변수.

		DWORD* pMemberID = NULL ;																// 멤버 아이디를 받을 포인터 변수.

		CPlayer* pMember = NULL ;																// 멤버 정보를 받을 포인터 변수.

		PTRLISTPOS pos = NULL ;																	// 리스트의 위치 정보를 담는 포인터.

		SaveCandidacy(pParty) ;																	// 후보 멤버들을 받는다.

		GetPartyLevels(pParty, &highLevel,&totalLevel) ;										// 파티의 최고레벨과 레벨 합게를 구한다.

		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//DWORD dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;				// 파티의 최고레벨로 몬스터 경험치를 구한다.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;

		if( m_TotalDamage < 1 )																	// 토탈 데미지가 1보다 작다면,
		{
			m_TotalDamage = 1 ;																	// 토탈 데미지를 1을 준다.
		}

		float fExpVal_A = (float)(dwPartyDamage * 0.3f) ;										// 자신의 파티데미지 * 0.3
		float fExpVal_B = (float)(dwPartyDamage + fExpVal_A) ;									// 자신의 파티데미지 + fExpVal_A
		float fExpVal_C = (float)(fExpVal_B / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_B / 모든 파티의 데미지 총합
		float fExpVal_D = (float)(fExpVal_C * dwMonsterExp) ;									// fExpVal_C * 몬스터 경험치.			

		int nCandidacyCount = m_CandidacyList.GetCount() ;										// 후보자 수를 받는다.

		if( nCandidacyCount == 0 ) return ;														// 이런 경우는 없어야 하는데, 혹시라도 생길지 모르기 때문에 예외 처리.

		if( nCandidacyCount == 1 )																// 자신 외에 보상을 받을 멤버가 없다.
		{
			pos = m_CandidacyList.FindIndex(0) ;												// 후보 리스트의 첫번째 위치 정보를 받아온다.

			if( pos )																			// 위치 정보가 유효한지 체크한다.
			{
				pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;								// 해당하는 위치의 멤버 아이디 정보를 받는다.

				if( pMemberID )																	// 멤버 아이디 정보가 유효한지 체크한다.
				{
					pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;					// 받은 아이디로 멤버 정보를 받는다.

					if( pMember )																// 멤버 정보가 유효한지 체크한다.
					{
						// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
						if( !IsSameChannel(pMember, m_pMob) ) return ;

						DWORD dwExp = (DWORD)fExpVal_D ;										// 경험치 소수점 이하는 버린다.
						// desc_hseos_패밀리01
						// S 패밀리 추가 added by hseos 2007.07.15
						// ..패밀리 명예 포인트 처리
						g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
						// E 패밀리 추가 added by hseos 2007.07.15
						
						// 080102 LYW --- Distributer : 솔로 플레이어 경험치 추가 처리.
						DWORD dwCurExp = dwExp + (DWORD)(dwExp*0.15f) ;
						DISTRIBUTERAND->SendToPersonalExp(pMember,dwCurExp);					// 경험치를 전송한다.
						if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
						{
							if( pMember->GetCurPet() )
								pMember->GetCurPet()->AddExp();
						}
					}
				}
			}
		}
		else																					// 보상을 받을 다른 플레이어가 있으면, 																	
		{

			BOOL bLevelPenalty = FALSE ;

			LEVELTYPE memberLevel ;
			LEVELTYPE lowLevel = 150 ;

			float fTotalLevel = 0 ;

			for(int count = 0 ; count < nCandidacyCount ; ++count)							// 후보 수만큼 포문을 돌린다.
			{	
				pos = m_CandidacyList.FindIndex(count) ;									// 카운트에 해당하는 위치정보를 받는다.

				if( pos )																	// 위치정보가 유효한지 체크한다.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// 위치 정보에 해당하는 멤버 아이디를 받는다.

					if( pMemberID )															// 멤버 아이디가 유효한지 체크한다.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// 멤버 아이디로 멤버 정보를 받는다.

						if( pMember )														// 멤버 정보가 유효한지 체크한다.
						{
							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;

							fTotalLevel += (memberLevel*memberLevel) ;

							if( memberLevel > highLevel ) highLevel = memberLevel ;

							if( memberLevel < lowLevel ) lowLevel = memberLevel ;
						}
					}
				}

				pMember = NULL ;															// 멤버 정보를 담는 포인터 NULL 처리.
			}

			if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;

			GetPartyLevels(pParty, &highLevel,&totalLevel) ;

			float applyRate = 0.0f ;															// 파티 보너스 경험치 포인트 변수 선언.

			switch( nCandidacyCount )															// 경험치 받을 플레이어 수 체크.
			{
			//case 2 : applyRate = 0.3f ; break ;													// 두명이면 +30%
			//case 3 : applyRate = 0.4f ; break ;													// 세명이면 +40%
			//case 4 : applyRate = 0.5f ; break ;													// 네명이면 +50%
			//case 5 : applyRate = 0.55f ; break ;												// 다섯명이면 +55%
			//case 6 : applyRate = 0.6f ; break ;													// 여섯명이면 +60%
			//case 7 : applyRate = 0.7f ; break ;													// 일곱명이면 +70%					

			// 071206 LYW --- Distributer : 파티 경험치 수정.
			case 2 : applyRate = 0.39f ;	break ;											// 두명이면 +35%
			case 3 : applyRate = 0.63f ;	break ;											// 세명이면 +60%
			case 4 : applyRate = 0.86f ;	break ;											// 네명이면 +86%
			case 5 : applyRate = 1.07f ;	break ;											// 다섯명이면 +107%
			case 6 : applyRate = 1.31f ;	break ;											// 여섯명이면 +131%
			case 7 : applyRate = 1.62f ;	break ;											// 일곱명이면 +162%
			}

			if( bLevelPenalty )
			{
				applyRate = 0.0f ;
			}

			for( int count = 0 ; count < nCandidacyCount ; ++count )							// 후보 수 만큼 포문을 돈다.
			{
				pos = m_CandidacyList.FindIndex(count) ;										// 카운트에 해당하는 위치 정보를 받는다.
				
				if( pos )																		// 위치 정보가 유효한지 체크한다.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;							// 해당하는 위치의 아이디 정보를 받는다.

					if( pMemberID )																// 아이디 정보가 유효한지 체크한다.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;				// 아이디로 멤버 정보를 받는다.

						if( pMember )															// 멤버 정보가 유효한지 체크한다.
						{
							// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
							if( !IsSameChannel(pMember, m_pMob) ) continue ;

							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;
							curlevel = (memberLevel * memberLevel);								// 멤버의 레벨을 받는다.

							float fExpA = (float)curlevel / fTotalLevel ;							// 경험치 포인트A를 구한다.
							float fExpB = (fExpVal_D * ( 1.f + applyRate )) ;						// 경험치 포인트B를 구한다.
							
							float fExp = fExpA * fExpB ;										// 경험치를 구한다.

							if(fExp > 0)														// 경험치 체크를 한다.
							{
								DWORD dwExp = (DWORD)fExp ;						// 경험치 소수점 이하는 버린다.
								// desc_hseos_패밀리01
								// S 패밀리 추가 added by hseos 2007.07.15
								// ..패밀리 명예 포인트 처리
								g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
								// E 패밀리 추가 added by hseos 2007.07.15
								if( (highLevel - memberLevel) >= 20 )
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// 경험치를 전송한다.
								}
								else
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);				// 경험치를 전송한다.							
									if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
									{
										if( pMember->GetCurPet() )
											pMember->GetCurPet()->AddExp();
									}
								}							}
						}

						pMember = NULL ;														// 멤버 정보 포인터 변수를 NULL처리.
					}
				}
			}
		}

	}
}

void CDistributer::DistributeToOtherParty(CParty* pParty, DWORD dwPartyDamage)
{
	// 후 공격 파티 경험치 처리.
	// 획득 경험치[후공격파티] = ((자신의 파티 데미지 - ((선공격 파티 데미지 * 0.3) / (파티 총 수 - 1))) / 모든 파티의 데미지 총합 ) + 몬스터 경험치.

	if( pParty )
	{
		LEVELTYPE highLevel = 0 ;																// 파티 멤버 중 최대 레벨.
		LEVELTYPE totalLevel = 0 ;																// 멤버들의 레벨을 받을 변수.
		LEVELTYPE curlevel = 0 ;																// 멤버들의 현재 레벨을 받을 변수.

		DWORD* pMemberID = NULL ;																// 멤버 아이디 정보를 받을 포인터 변수.

		CPlayer* pMember = NULL ;																// 멤버 정보를 받을 포인터 변수.

		PTRLISTPOS pos = NULL ;																	// 후보 리스트의 위치 정보 포인터 변수.

		SaveCandidacy(pParty) ;																	// 후보 멤버들을 받는다.

		GetPartyLevels(pParty,&highLevel,&totalLevel) ;											// 파티의 최고 레벨, 레벨 합을 구한다.

		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD ;
		//DWORD dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;				// 최고레벨에 따른 몬스터 경험치를 구한다.
		DWORD dwMonsterExp = 0 ;
		dwMonsterExp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;


		int nCandidacyCount = m_CandidacyList.GetCount() ;										// 리스트에 담긴 후보자 수를 구한다.

		int nKillerCount = 1 ;																	// 킬러 카운트를 세팅한다.

		if( m_nTotalKillerCount > 1 )															// 킬러 카운트가 한명 이상이면,
		{
			nKillerCount = m_TotalDamage-1 ;													// 킬러 카운트를 다시 세팅한다.
		}
		else																					// 혹시 발생할 지 모르는 상황, 킬러 수가 없다면,
		{
			nKillerCount = 1 ;																	// 킬러 수를 1로 세팅한다.
		}

		if( m_TotalDamage < 1 )																	// 토탈 데미지가 1보다 작다면,
		{
			m_TotalDamage = 1 ;																	// 토탈 데미지를 1로 세팅한다.
		}

		float fExpVal_A = (float)(m_FirstDamage * 0.3f) ;										// 선공격 파티 데미지 * 0.3

		float fExpVal_B = (float)(fExpVal_A / (nKillerCount)) ;									// fExpVal_A / (파티 총 수 - 1)

		if( fExpVal_B <= 1 ) fExpVal_B = 1 ;													// 음수 체크를 한다.

		float fExpVal_C = (float)(dwPartyDamage - fExpVal_B) ;									// 자신의 파티 데미지 - fExpVal_B

		if( fExpVal_C <= 1 ) fExpVal_C = 1 ;													// 음수 체크를한다.

		float fExpVal_D = (float)(fExpVal_C / (m_TotalDamage+fExpVal_A)) ;						// fExpVal_C / 모든 파티의 데미지 총합

		float fExpVal_E = (float)(fExpVal_D * dwMonsterExp) ;									// fExpVal_D * 몬스터 경험치.				

		if( nCandidacyCount == 0 ) return ;														// 이런 경우는 없어야 하는데, 혹시라도 생길지 모르기 때문에 예외 처리.

		if( nCandidacyCount == 1 )																// 자신 외에 보상을 받을 멤버가 없다.
		{
			pos = m_CandidacyList.FindIndex(0) ;												// 리스트의 첫번째 위치 정보를 받는다.

			if( pos )																			// 위치정보가 유효한지 검사한다.
			{
				pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;								// 위치에 해당하는 멤버 아이디 정보를 받는다.

				if( pMemberID )																	// 멤버 아이디 정보가 유효한지 체크한다.
				{
					pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;					// 멤버 아이디로 멤버 정보를 받는다.

					if( pMember )																// 멤버 정보가 유효한지 체크한다.
					{
						// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
						if( !IsSameChannel(pMember, m_pMob) ) return ;

						DWORD dwExp = (DWORD)fExpVal_E ;										// 경험치 소수점 이하는 버린다.
						// desc_hseos_패밀리01
						// S 패밀리 추가 added by hseos 2007.07.15
						// ..패밀리 명예 포인트 처리
						g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
						// E 패밀리 추가 added by hseos 2007.07.15
						// 080102 LYW --- Distributer : 솔로 플레이어 경험치 추가 처리.
						DWORD dwCurExp = dwExp + (DWORD)(dwExp*0.15f) ;
						DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);						// 경험치를 전송한다.
						if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
						{
							if( pMember->GetCurPet() )
								pMember->GetCurPet()->AddExp();
						}
					}
				}
			}
		}
		else																					// 보상을 받을 다른 플레이어가 있으면, 																	
		{
			BOOL bLevelPenalty = FALSE ;

			LEVELTYPE memberLevel ;
			LEVELTYPE lowLevel = 150 ;

			float fTotalLevel = 0 ;

			for(int count = 0 ; count < nCandidacyCount ; ++count)							// 후보 수만큼 포문을 돌린다.
			{	
				pos = m_CandidacyList.FindIndex(count) ;									// 카운트에 해당하는 위치정보를 받는다.

				if( pos )																	// 위치정보가 유효한지 체크한다.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// 위치 정보에 해당하는 멤버 아이디를 받는다.

					if( pMemberID )															// 멤버 아이디가 유효한지 체크한다.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// 멤버 아이디로 멤버 정보를 받는다.

						if( pMember )														// 멤버 정보가 유효한지 체크한다.
						{
							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;

							fTotalLevel += (memberLevel*memberLevel) ;

							if( memberLevel > highLevel ) highLevel = memberLevel ;

							if( memberLevel < lowLevel ) lowLevel = memberLevel ;
						}
					}
				}

				pMember = NULL ;															// 멤버 정보를 담는 포인터 NULL 처리.
			}

			if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;

			float applyRate = 0.0f ;															// 파티 보너스 경험치 포인트.

			switch( nCandidacyCount )															// 경험치 받을 플레이어 수 체크.
			{
			//case 2 : applyRate = 0.3f ; break ;													// 두명이면 +30%
			//case 3 : applyRate = 0.4f ; break ;													// 세명이면 +40%
			//case 4 : applyRate = 0.5f ; break ;													// 네명이면 +50%
			//case 5 : applyRate = 0.55f ; break ;												// 다섯명이면 +55%
			//case 6 : applyRate = 0.6f ; break ;													// 여섯명이면 +60%
			//case 7 : applyRate = 0.7f ; break ;													// 일곱명이면 +70%						

			// 071206 LYW --- Distributer : 파티 경험치 수정.
			case 2 : applyRate = 0.39f ;	break ;											// 두명이면 +35%
			case 3 : applyRate = 0.63f ;	break ;											// 세명이면 +60%
			case 4 : applyRate = 0.86f ;	break ;											// 네명이면 +86%
			case 5 : applyRate = 1.07f ;	break ;											// 다섯명이면 +107%
			case 6 : applyRate = 1.31f ;	break ;											// 여섯명이면 +131%
			case 7 : applyRate = 1.62f ;	break ;											// 일곱명이면 +162%
			}

			if( bLevelPenalty )
			{
				applyRate = 0.0f ;
			}

			for( int count = 0 ; count < nCandidacyCount ; ++count )							// 후보자 수만큼 포문을 돈다.
			{
				pos = m_CandidacyList.FindIndex(count) ;										// 카운트에 해당하는 위치정보를 받는다.
				
				if( pos )																		// 위치 정보가 유효한지 체크한다.
				{
					pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;							// 위치에 해당하는 멤버 아이디를 받는다.

					if( pMemberID )																// 아이디 정보가 유효한지 체크한다.
					{
						pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;				// 아이디로 멤버 정보를 받는다.

						if( pMember )															// 멤버 정보가 유효한지 체크한다.
						{
							// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
							if( !IsSameChannel(pMember, m_pMob) ) continue ;

							memberLevel = 0 ;
							memberLevel = pMember->GetLevel() ;
							curlevel = (memberLevel * memberLevel);								// 멤버의 레벨을 받는다.

							float fExpA = (float)curlevel / fTotalLevel ;						// 경험치 포인트A를 구한다.
							float fExpB = (fExpVal_E * ( 1.f + applyRate )) ;						// 경험치 포인트B를 구한다.
							
							float fExp = fExpA * fExpB ;										// 경험치를 구한다.

							if(fExp > 0)														// 경험치 체크를 한다.
							{
								DWORD dwExp = (DWORD)fExp ;						// 경험치 소수점 이하는 버린다.
								// desc_hseos_패밀리01
								// S 패밀리 추가 added by hseos 2007.07.15
								// ..패밀리 명예 포인트 처리
								g_csFamilyManager.SRV_ProcessHonorPoint(pMember, dwExp);
								// E 패밀리 추가 added by hseos 2007.07.15
								if( (highLevel - memberLevel) >= 20 )
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// 경험치를 전송한다.
								}
								else
								{
									DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);				// 경험치를 전송한다.				
									if( pMember->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
									{
										if( pMember->GetCurPet() )
											pMember->GetCurPet()->AddExp();
									}
								}
							}
						}

						pMember = NULL ;														// 멤버 정보 포인터를 NULL 처리 한다.
					}
				}
			}
		}
	}
}

void CDistributer::SendMoneyToPartys(float ChangeValue)
{
	int nCandidacyCount = m_CandidacyList.GetCount() ;											// 후보자 수를 받는다.

	if( nCandidacyCount <= 0 ) return ;															// 혹시나 발생할 예외상황.

	DWORD* pMemberID = NULL ;																	// 멤버 아이디 정보를 받을 포인터 변수.

	CPlayer* pMember = NULL ;																	// 멤버 정보를 받을 포인터 변수.

	PTRLISTPOS pos = NULL ;																		// 리스트에서의 위치 정보를 받을 포인터 변수.

	m_CandidacyList.GetHeadPosition() ;															// 후보를 담은 리스트를 해드로 세팅.

	float fMoney = ChangeValue / (float)nCandidacyCount ;										// 입력받은 머니를 후보자 수로 나눈다.

	MONEYTYPE dwMoney = (DWORD)fMoney ;															// 소수 이하는 버린다.

	for( int count = 0 ; count < nCandidacyCount ; ++count )									// 후보자 수만큼 포문을 돌린다.
	{
		pos = m_CandidacyList.FindIndex(count) ;												// 카운트에 해당하는 위치정보를 받는다.

		if( pos )																				// 위치정보가 유효한지 체크한다.
		{
			pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// 위치 정보에 해당하는 멤버 아이디를 받는다.

			if( pMemberID )																		// 멤버 아이디가 유효한지 체크한다.
			{
				pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;						// 멤버 아이디로 멤버 정보를 받는다.

				if( pMember )																	// 멤버 정보가 유효한지 체크한다.
				{
					if( m_pMob )																// 몬스터 정보가 유효한지 체크한다.
					{
						// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
						if( !IsSameChannel(pMember, m_pMob) ) continue ;

						if( pMember->SetMoney(dwMoney, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory,	// 각 멤버에게 머니를 전송한다.
							eMoneyLog_GetMonster, m_pMob->GetMonsterKind()) != dwMoney )
						{
							// error msg 보낸다. 제한량 초과
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
	DAMAGEOBJ*	obj		= NULL ;																// 데미지 정보를 받을 포인터 변수.
	CPlayer*	pPlayer = NULL ;
	CParty*		pParty  = NULL ;

	BOOL bNoPlayer = FALSE ;
	BOOL bNoParty  = FALSE ;

	// 개인 플레이어 중 아이템 획득 후보를 뽑는다.

	DWORD dwBigPlayerDamage = 0 ;																// 가장 큰 개인 데미지를 담을 변수.
	DWORD dwBigPlayerID = 0 ;																	// 가장 큰 데미지를 준 플레이어 아이디를 담을 변수.

	m_DamageObjectTableSolo.SetPositionHead() ;													// 데미지 테이블을 해드로 세팅한다.

	while(obj = m_DamageObjectTableSolo.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( !obj ) continue ;																	// 데미지 정보가 유효한지 체크한다.

		pPlayer = NULL ;
		pPlayer = (CPlayer*)g_pUserTable->FindUser(obj->dwID) ;									// 데미지에 따른 아이디로 플레이어 정보를 받는다.

		if( !pPlayer ) continue ;																// 플레이어 정보가 유효한지 체크한다.

		if( pPlayer->GetState() == eObjectState_Die ) continue ;								// 플레이어가 죽었다면, 대상에서 제외 시킨다.

		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;									// 플레이어의 파티 아이디로 파티 정보를 받는다.

		if( pParty ) continue ;																	// 파티 정보가 유효한지 체크, 파티가 없으면, 

		ChooseOne(obj, &dwBigPlayerDamage, &dwBigPlayerID) ;									// 파티가 없는 솔로 플레이어 중 가장 큰 데미지와 아이디를 받는다.
	}

	if( dwBigPlayerDamage == 0 ) bNoPlayer = TRUE ;												// 아이템 습특 후보에 개인 플레이어가 없다.

	// 파티 중 아이템 획득 파티를 뽑는다.

	DWORD dwBigPartyDamage = 0 ;																// 가장 큰 파티 데미지를 담을 변수.
	DWORD dwBigPartyID = 0 ;																	// 가장 큰 파티 테미지를 준 파타 아이디를 담을 변수.

	m_DamageObjectTableParty.SetPositionHead() ;												// 데미지 테이블을 해드로 세팅한다.

	while(obj = m_DamageObjectTableParty.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( !obj ) continue ;																	// 데미지 정보를 받는다.

		pParty = NULL ;
		pParty = PARTYMGR->GetParty(obj->dwID) ;

		if( !pParty ) continue ;

		SaveCandidacy( pParty ) ;

		if( m_CandidacyList.GetCount() == 0 ) continue ;
	
		ChooseOne(obj, &dwBigPartyDamage, &dwBigPartyID) ;										// 가장 큰 파티 데미지와 파티를 받는다.
	}

	if( dwBigPartyDamage == 0 ) bNoParty = TRUE ;

	if( bNoPlayer && bNoParty ) return ;														// 파티와 개인 모두 후보가 없으면, return 처리 한다.

	// 080717 LYW --- Distributer : 데미지 테이블에 따른, 조건 체크 순서 수정.
	// if( bNoPlayer || bNoParty )
	// else
	// 이 순서를 
	// if( !bNoPlayer && !bNoParty )
	// else 로 수정 함.

	if( !bNoPlayer && !bNoParty )
	{
		// 파티와 개인의 데미지를 비교한다.
		if( dwBigPlayerDamage == dwBigPartyDamage )													// 개인의 데미지와 파티의 데미지가 같다면,
		{
			int nResult = random(0, 100) ;															// 랜덤 수를 구한다.

			if( nResult < 50 )																		// 랜덤 수가 50미만이면, 
			{
				dwBigPlayerDamage = 0 ;																// 파티에게 아이템을 돌린다.
				dwBigPartyDamage  = 1 ;
			}
			else																					// 랜덤 수가 50을 초과하면,
			{
				dwBigPlayerDamage = 1 ;																// 개인에게 아이템을 돌린다.
				dwBigPartyDamage  = 0 ;
			}
		}

		if( dwBigPlayerDamage > dwBigPartyDamage )													// 개인의 데미지가 파티의 데미지 보다 크면,
		{
			pPlayer = NULL ;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID) ;								// 가장 큰 데미지를 준 플레이어 정보를 받는다.

			if( pPlayer )																			// 플레이어 정보가 유효한지 체크한다.
			{
				SendItemToPerson(pPlayer) ;															// 가장 큰 데미지를 준 플레이어에게 아이템을 전송한다.
			}
		}
		else if( dwBigPlayerDamage < dwBigPartyDamage )												// 파티 데미지가 개인의 데미지 보다 크면,
		{
			pParty = NULL ;
			pParty = PARTYMGR->GetParty(dwBigPartyID) ;												// 가장 큰 데미지를 준 파티 정보를 받는다.

			if( pParty )																			// 파티 정보가 유효한지 체크한다.
			{
				// 081205 LYW --- Distributer : 파티 아이템 분배 문제( 마지막 파티가 아이템 획득 문제 ) 수정.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())															// 파티의 아이템 분배 방식을 받아온다.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// 순차 분배로 아이템을 분배한다.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// 데미지 우선 순위로 아이템을 분배한다.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// 렌덤하게 멤버를 정해서 아이템을 분배한다.
				}
			}
		}
	}
	else
	{
		if( bNoPlayer )
		{
			pParty = NULL ;
			pParty = PARTYMGR->GetParty(dwBigPartyID) ;												// 가장 큰 데미지를 준 파티 정보를 받는다.

			if( pParty )																			// 파티 정보가 유효한지 체크한다.
			{
				// 081205 LYW --- Distributer : 파티 아이템 분배 문제( 마지막 파티가 아이템 획득 문제 ) 수정.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())															// 파티의 아이템 분배 방식을 받아온다.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// 순차 분배로 아이템을 분배한다.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// 데미지 우선 순위로 아이템을 분배한다.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// 렌덤하게 멤버를 정해서 아이템을 분배한다.
				}
			}
		}
		else if( bNoParty )
		{
			pPlayer = NULL ;
			pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID) ;								// 가장 큰 데미지를 준 플레이어 정보를 받는다.

			if( pPlayer )																			// 플레이어 정보가 유효한지 체크한다.
			{
				SendItemToPerson(pPlayer) ;															// 가장 큰 데미지를 준 플레이어에게 아이템을 전송한다.
			}
		}
	}	
}


//void CDistributer::DistributeItemToKillers() 
//{
//	DWORD dwBigPlayerDamage = 0 ;																// 가장 큰 개인 데미지를 담을 변수.
//	DWORD dwBigPlayerID = 0 ;																	// 가장 큰 데미지를 준 플레이어 아이디를 담을 변수.
//
//	m_DamageObjectTableSolo.SetPositionHead() ;													// 데미지 테이블을 해드로 세팅한다.
//
//	DAMAGEOBJ* obj = NULL ;																		// 데미지 정보를 받을 포인터 변수.
//
//	while(obj = m_DamageObjectTableSolo.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
//	{
//		if( obj )																				// 데미지 정보가 유효한지 체크한다.
//		{
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(obj->dwID);						// 데미지에 따른 아이디로 플레이어 정보를 받는다.
//
//			if( pPlayer )																		// 플레이어 정보가 유효한지 체크한다.
//			{
//				CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;					// 플레이어의 파티 아이디로 파티 정보를 받는다.
//
//				if( !pParty )																	// 파티 정보가 유효한지 체크, 파티가 없으면, 
//				{
//					ChooseOne(obj, &dwBigPlayerDamage, &dwBigPlayerID);							// 파티가 없는 솔로 플레이어 중 가장 큰 데미지와 아이디를 받는다.
//				}				
//			}
//		}
//	}
//
//	DWORD dwBigPartyDamage = 0 ;																// 가장 큰 파티 데미지를 담을 변수.
//	DWORD dwBigPartyID = 0 ;																	// 가장 큰 파티 테미지를 준 파타 아이디를 담을 변수.
//
//	m_DamageObjectTableParty.SetPositionHead() ;												// 데미지 테이블을 해드로 세팅한다.
//
//	while(obj = m_DamageObjectTableParty.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
//	{
//		if( obj )																				// 데미지 정보를 받는다.
//		{
//			ChooseOne(obj, &dwBigPartyDamage, &dwBigPartyID);									// 가장 큰 파티 데미지와 파티를 받는다.
//		}
//	}
//
//	if( dwBigPlayerDamage == dwBigPartyDamage )													// 개인의 데미지와 파티의 데미지가 같다면,
//	{
//		int nResult = random(0, 100) ;															// 랜덤 수를 구한다.
//
//		if( nResult < 50 )																		// 랜덤 수가 50미만이면, 
//		{
//			dwBigPlayerDamage = 0 ;																// 파티에게 아이템을 돌린다.
//			dwBigPartyDamage  = 1 ;
//		}
//		else																					// 랜덤 수가 50을 초과하면,
//		{
//			dwBigPlayerDamage = 1 ;																// 개인에게 아이템을 돌린다.
//			dwBigPartyDamage  = 0 ;
//		}
//	}
//
//	if( dwBigPlayerDamage > dwBigPartyDamage )													// 개인의 데미지가 파티의 데미지 보다 크면,
//	{
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwBigPlayerID);						// 가장 큰 데미지를 준 플레이어 정보를 받는다.
//
//		if( pPlayer )																			// 플레이어 정보가 유효한지 체크한다.
//		{
//			SendItemToPerson(pPlayer) ;															// 가장 큰 데미지를 준 플레이어에게 아이템을 전송한다.
//		}
//	}
//	else if( dwBigPlayerDamage < dwBigPartyDamage )												// 파티 데미지가 개인의 데미지 보다 크면,
//	{
//		CParty* pParty = PARTYMGR->GetParty(dwBigPartyID) ;										// 가장 큰 데미지를 준 파티 정보를 받는다.
//
//		if( pParty )																			// 파티 정보가 유효한지 체크한다.
//		{
//			switch(pParty->GetOption())															// 파티의 아이템 분배 방식을 받아온다.
//			{
//			case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;						// 순차 분배로 아이템을 분배한다.
//			case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;						// 데미지 우선 순위로 아이템을 분배한다.
//			case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;						// 렌덤하게 멤버를 정해서 아이템을 분배한다.
//			}
//		}
//	}
//}

void CDistributer::DistributeToKiller()
{
	if( m_pKiller )																				// 킬러 정보가 유효한지 체크한다.
	{
		if( !m_pKillerParty )																	// 킬러가 파티가 없다면,
		{
			DistributeToPerson( m_pKiller ) ;													// 킬러에게 아이템, 경험치, 돈을 전송한다.
		}
		else																					// 킬러가 파티에 속해있다면,
		{
			DistributeToParty(m_pKiller) ;														// 킬러의 파티에게 아이템, 경험치, 돈을 전송한다.
		}
	}
}

void CDistributer::DistributeToPerson(CPlayer* pPlayer)
{
	if( pPlayer && m_pMob )																		// 플레이어와 몬스터 정보가 유효한지 체크한다.
	{
		// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		// 080110 LYW --- Distributer : 경험치 처리 플레이어 죽었을 때 처리 하지 않도록 처리.
		if(pPlayer->GetState() == eObjectState_Die) return ;

		// 080102 LYW --- Distributer : 솔로잉 플레이어 경험치 추가 처리.
		// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
		//EXPTYPE dwCur = CalcObtainExp(pPlayer->GetLevel(), 1) ;									// 몬스터의 경험치를 받아 온다.
		DWORD dwCur = 0 ;
		dwCur = CalcObtainExp(pPlayer->GetLevel(), 1) ;

		EXPTYPE dwExp = 0 ;
		dwExp = dwCur + (EXPTYPE)(dwCur * 0.15f) ;

		WORD wMonsterKind = m_pMob->GetMonsterKind() ;											// 몬스터 종류를 받는다.
		
		MONEYTYPE money = 0 ;																	// 획득할 몬스터 머니를 구한다.

		int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;								// 몬스터와의 레벨 차이에 따른 패널티 값을 받는다.

		money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(									// 패널티를 적용한 머니를 받는다.
		(MONSTEREX_LIST *)m_pMob->GetSMonsterList(), m_pMob->GetMonsterKind(), nPenaltyType);

		// desc_hseos_패밀리01
		// S 패밀리 추가 added by hseos 2007.07.15
		// ..패밀리 명예 포인트 처리
		// 080602 LYW --- Player : 경험치 수치 (__int32) 에서 (__int64) 사이즈로 변경 처리.
		//g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
		g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, (DWORD)dwExp);
		// E 패밀리 추가 added by hseos 2007.07.15
		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;										// 플레이어에게 경험치를 전송한다.
		if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
		{
			if( pPlayer->GetCurPet() )
				pPlayer->GetCurPet()->AddExp();
		}

		SendMoneyToPerson(pPlayer, money);														// 플레이어에게 머니를 전송한다.

		SendItemToPerson(pPlayer);																// 플레이어에게 아이템을 전송한다.
	}
}

void CDistributer::DistributeToParty(CPlayer* pPlayer)
{	
	if( !m_pKilledPosition || !m_pMob ) return ;												// 위치정보와 몬스터 정보가 유효한지 체크한다.

	if( pPlayer )																				// 플레이어 정보가 유효한지 체크한다.
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());							// 플레이어의 파티 정보를 받는다.

		if( pParty )																			// 파티 정보가 유효한지 체크한다.
		{
			SaveCandidacy(pParty) ;																// 후보들을 리스트에 담는다.

			int nCandidadyCount = m_CandidacyList.GetCount() ;									// 후보들의 수를 받는다.

			if( nCandidadyCount == 0 ) return ;													// 이런 경우는 없어야 하는데, 혹시라도 생길지 모르기 때문에 예외 처리.

			if( nCandidadyCount == 1 )															// 자신 외에 보상을 받을 멤버가 없다.
			{
				DistributeToPerson( pPlayer ) ;													// 아이템, 경험치, 돈을 전송한다.
				return ;
			}
			else																				// 보상을 받을 다른 플레이어가 있으면, 																	
			{
				DWORD* pMemberID = NULL ;														// 멤버 아이디 정보를 받을 포인터 변수.

				CPlayer* pMember = NULL ;														// 멤버 정보를 받을 포인터 변수.

				PTRLISTPOS pos = NULL ;															// 후보 리스트의 위치 정보 포인터 변수.

				BOOL bLevelPenalty = FALSE ;

				LEVELTYPE highLevel = 0 ;														// 파티 멤버 중 최대 레벨.
				LEVELTYPE totalLevel = 0 ;														// 멤버들의 레벨을 받을 변수.
				LEVELTYPE curlevel = 0 ;														// 멤버들의 현재 레벨을 받을 변수.

				LEVELTYPE lowLevel = 150 ;
				LEVELTYPE memberLevel ;

				float fTotalLevel = 0 ;

				for(int count = 0 ; count < nCandidadyCount ; ++count)							// 후보 수만큼 포문을 돌린다.
				{	
					pos = m_CandidacyList.FindIndex(count) ;									// 카운트에 해당하는 위치정보를 받는다.

					if( pos )																	// 위치정보가 유효한지 체크한다.
					{
						pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// 위치 정보에 해당하는 멤버 아이디를 받는다.

						if( pMemberID )															// 멤버 아이디가 유효한지 체크한다.
						{
							pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// 멤버 아이디로 멤버 정보를 받는다.

							if( pMember )														// 멤버 정보가 유효한지 체크한다.
							{
								memberLevel = 0 ;
								memberLevel = pMember->GetLevel() ;

								fTotalLevel += (memberLevel*memberLevel) ;

								if( memberLevel > highLevel ) highLevel = memberLevel ;

								if( memberLevel < lowLevel ) lowLevel = memberLevel ;
							}
						}
					}

					pMember = NULL ;															// 멤버 정보를 담는 포인터 NULL 처리.
				}

				if( (highLevel - lowLevel) >= 20 ) bLevelPenalty = TRUE ;
	
				GetPartyLevels(pParty, &highLevel,&totalLevel) ;

				// 080530 LYW --- Distributer : Modified a data type of experience to DWORD.
				//EXPTYPE partyexp = CalcObtainExp(highLevel, m_CandidacyList.GetCount());		// 파티 내 최대 레벨을 기준으로 몬스터 경험치를 받아 온다.
				DWORD partyexp = 0 ;
				partyexp = CalcObtainExp(highLevel, m_CandidacyList.GetCount()) ;

				float applyRate = 0.0f ;														// 파티 보너스 경험치 포인트.

				switch( nCandidadyCount )														// 경험치 받을 플레이어 수 체크.
				{ 
				//case 2 : applyRate = 0.3f ; break ;												// 두명이면 +30%
				//case 3 : applyRate = 0.4f ; break ;												// 세명이면 +40%
				//case 4 : applyRate = 0.5f ; break ;												// 네명이면 +50%
				//case 5 : applyRate = 0.55f ; break ;											// 다섯명이면 +55%
				//case 6 : applyRate = 0.6f ; break ;												// 여섯명이면 +60%
				//case 7 : applyRate = 0.7f ; break ;												// 일곱명이면 +70%

				// 071206 LYW --- Distributer : 파티 경험치 수정.
				case 2 : applyRate = 0.39f ;	break ;											// 두명이면 +35%
				case 3 : applyRate = 0.63f ;	break ;											// 세명이면 +60%
				case 4 : applyRate = 0.86f ;	break ;											// 네명이면 +86%
				case 5 : applyRate = 1.07f ;	break ;											// 다섯명이면 +107%
				case 6 : applyRate = 1.31f ;	break ;											// 여섯명이면 +131%
				case 7 : applyRate = 1.62f ;	break ;											// 일곱명이면 +162%
				}	

				if( bLevelPenalty )
				{
					applyRate = 0.0f ;
				}
			
				MONEYTYPE money = 0 ;															// 획득할 몬스터 머니를 구한다.

				int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;						// 몬스터와의 레벨 차이에 따른 패널티 값을 받는다.

				money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(							// 패널티를 적용한 머니를 받는다.
				(MONSTEREX_LIST *)m_pMob->GetSMonsterList(), m_pMob->GetMonsterKind(), nPenaltyType);
								
				MONEYTYPE ObtainMoney = (MONEYTYPE)((float)money/nCandidadyCount) ;				// 1/n로 한 머니를 구한다.

				for(int count = 0 ; count < nCandidadyCount ; ++count)							// 후보 수만큼 포문을 돌린다.
				{	
					pos = m_CandidacyList.FindIndex(count) ;									// 카운트에 해당하는 위치정보를 받는다.

					if( pos )																	// 위치정보가 유효한지 체크한다.
					{
						pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;						// 위치 정보에 해당하는 멤버 아이디를 받는다.

						if( pMemberID )															// 멤버 아이디가 유효한지 체크한다.
						{
							pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;			// 멤버 아이디로 멤버 정보를 받는다.

							if( pMember )														// 멤버 정보가 유효한지 체크한다.
							{
								// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
								if( !IsSameChannel(pMember, m_pMob) ) continue ;

								memberLevel = 0 ;
								memberLevel = pMember->GetLevel();
								curlevel = (memberLevel*memberLevel) ;							// 멤버의 레벨을 받는다.

								float fExpA = (float)curlevel / fTotalLevel ;					// 경험치 포인트A를 구한다.
								float fExpB = (partyexp * ( 1.f + applyRate )) ;					// 경험치 포인트B를 구한다.
								
								float fExp = fExpA * fExpB ;									// 경험치를 구한다.

								if(fExp > 0)													// 경험치 체크를 한다.
								{
									DWORD dwExp = (DWORD)fExp ;									// 경험치 소수점 이하는 버린다.

									if( (highLevel - memberLevel) >= 20 )
									{
										DISTRIBUTERAND->SendToPersonalExp(pMember,1);			// 경험치를 전송한다.
									}
									else
									{
										DISTRIBUTERAND->SendToPersonalExp(pMember,dwExp);		// 경험치를 전송한다.
										if( pPlayer->GetLevel() < m_pMob->GetLevel() + PETMGR->GetExpPenaltyLevel() )
										{
											if( pPlayer->GetCurPet() )
												pPlayer->GetCurPet()->AddExp();
										}
									}
						
									// desc_hseos_패밀리01
									// S 패밀리 추가 added by hseos 2007.07.15
									// ..패밀리 명예 포인트 처리
									g_csFamilyManager.SRV_ProcessHonorPoint(pPlayer, dwExp);
									// E 패밀리 추가 added by hseos 2007.07.15
									SendMoneyToPerson(pMember, ObtainMoney);					// 머니를 전송한다.
								}
							}
						}
					}

					pMember = NULL ;															// 멤버 정보를 담는 포인터 NULL 처리.
				}

				// 081205 LYW --- Distributer : 파티 아이템 분배 문제( 마지막 파티가 아이템 획득 문제 ) 수정.
				SaveCandidacy( pParty ) ;

				switch(pParty->GetOption())														// 파티의 아이템 분배 정보를 받는다.
				{
				case ePartyOpt_Sequence : DistribuSequence(pParty) ;	break ;					// 순차적으로 아이템을 분배한다.
				case ePartyOpt_Damage :	  DistributeDamage(pParty) ;	break ;					// 데미지가 높은 멤버에게 아이템을 분배한다.
				case ePartyOpt_Random :	  DistributeRandom(pParty) ; 	break ;					// 렌덤하게 멤버를 선택해서 아이템을 분배한다.
				}
			}
		}
	}
}

void CDistributer::SendMoneyToPerson(CPlayer* pPlayer, MONEYTYPE ChangeValue)
{
	if( pPlayer && m_pMob )																		// 플레이어와 몬스터의 정보가 유효한지 체크한다.
	{
		// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		// 071228 LYW --- Distributer : 머니 이벤트 적용.
		MONEYTYPE eventValue = 0 ;
		eventValue = (MONEYTYPE)(gEventRate[eEvent_MoneyRate] * ChangeValue) ;
		//eventValue = (MONEYTYPE)(ChangeValue) ;

		// 머니를 플레이어에게 전송한다.
		if( pPlayer->SetMoney(eventValue, MONEY_ADDITION, MF_OBTAIN, eItemTable_Inventory, eMoneyLog_GetMonster, m_pMob->GetMonsterKind()) != eventValue )
		{
			// error msg 보낸다. 제한량 초과
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
	if( !pPlayer || !m_pMob ) return ;															// 플레이어와 몬스터 정보가 유효한지 체크한다.

	// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
	if( !IsSameChannel(pPlayer, m_pMob) ) return ;

	int nMaxItemCount = eDROPITEMKIND_MAX-1 ;														// 몬스터가 드랍할 수 있는 최대 아이템 수

	WORD MonsterKind = m_pMob->GetMonsterKind() ;												// 몬스터 종류를 받는다.

	MONSTEREX_LIST* pMonInfo = NULL ;															// 몬스터 리스트 정보를 받을 포인터 선언.
	pMonInfo = (MONSTEREX_LIST *)m_pMob->GetSMonsterList() ;									// 몬스터 리스트를 받는다.							

	BOOL bObtainItem = FALSE ;																	// 아이템 분배가 되었는지 체크하는 변수.

	if( m_HighLevel <= 0 )																		// 혹시라도 최고레벨의 값이 문제가 있다면,
	{
		m_HighLevel = pPlayer->GetLevel() ;														// 최고 레벨을 플레이어의 레벨로 세팅한다.
	}

	int nPenaltyType = GetLevelPenaltyToMonster(m_HighLevel) ;									// 몬스터와 레벨차에 따른 패널티 값을 받는다.

	if( nPenaltyType == 3 ) return ;															// 패널티 값이 3, 15레벨 이상 차이나면 아이템 드랍은 없다.

	if( pMonInfo )																				// 몬스터 리스트 정보가 유효한지 체크한다.
	{
		for( int count = 0 ;  count < nMaxItemCount ; ++count )									// 최대 드랍 수 만큼 포문을 돌린다.							
		{
			// 아이템 드람을 한다.
			if(ITEMDROP_OBJ->NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count],
				pPlayer, nPenaltyType))															// 아이템 드랍이 성공이면,
			{
				bObtainItem = TRUE ;															// 아이템이 드랍되었다고 변수를 세팅한다.
			}
		}
	}

	if( bObtainItem )																			// 아이템 할당이 이루어 졌다면, 
	{
		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx() ) ;							// 파티 정보를 받는다.

		if( pParty )																			// 파티 정보가 유효한지 체크한다.
		{
			BYTE winnerIdx = pParty->GetWinnerIdx() ;											// 당첨자 번호를 받는다.
			
			if(winnerIdx == MAX_PARTY_LISTNUM-1 )												// 당첨 번호 루프의 마지막인지 체크한다.
			{
				pParty->SetWinnerIdx(0) ;														// 당첨 번호를 처음으로 세팅한다.
			}
			else																				// 루프의 마지막이 아니면, 
			{
				pParty->SetWinnerIdx(++winnerIdx) ;												// 당첨 번호를 증가한다.						
			}
		}
	}
}

//EXPTYPE CDistributer::CalcObtainExp(LEVELTYPE KillerLevel)
// 071202 LYW --- Distributer : 파티원이 2인 이상일 때 레벨 차이를 5레벨에서 8레벨로 늘려주는 작업 처리.
// 1. 필요 정보 체크
// 2. 몬스터 경험치 받기
// 3. 레벨 차이 구하기
// 4. 레벨 차에 의한 처리
// 080530 LYW --- Distributer : Modified the return type EXPTYPE to DWORD.
//EXPTYPE CDistributer::CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount)
DWORD CDistributer::CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount)
{
	// 필요 정보 체크
	ASSERT(m_pMob) ;
	if( !m_pMob ) return 0 ;																	// 몬스터 정보를 체크한다.

	ASSERT(((CObject*)m_pMob)) ;
	if( !((CObject*)m_pMob) ) return 0 ;														// 몬스터의 오브젝트 타입의 정보가 꼭 필요하다.


	// 몬스터 경험치 받기
	DWORD dwExp = 0 ;																			// 경험치를 담을 변수를 선언한다.	   

	BYTE byObjectKind = 0 ;																		
	byObjectKind = ((CObject*)m_pMob)->GetObjectKind() ;										// 몬스터의 오브젝트 종류를 받는다.

	if( byObjectKind == eObjectKind_BossMonster )												// 보스 몬스터이면, 
	{
		WORD wMonsterKind = 0 ;
		wMonsterKind = m_pMob->GetMonsterKind() ;

		dwExp = (DWORD)BOSSREWARDSMGR->GetExpOfBossMonster(wMonsterKind) ;						// 보스 몬스터 경험치를 받는다.
	}
	else																						// 일반 몬스터이면,
	{
		dwExp = m_pMob->GetSMonsterList()->ExpPoint ;											// 일반 몬스터 경험치를 받는다.
	}


	// 레벨 차이 구하기
	LEVELTYPE MonsterLevel	= m_pMob->GetSMonsterList()->Level ;								// 몬스터 레벨을 받는다.
	LEVELTYPE levelGap		= 0 ;																// 몬스터와 플레이어의 레벨 차이.
	
	if( MonsterLevel > KillerLevel )															// 몬스터 레벨이 플레이어 보다 큰 경우.
	{
		levelGap = MonsterLevel - KillerLevel ;													// 레벨 차이를 구한다.
	}
	else																						// 플레이어 레벨이 몬스터 레벨 보다 큰 경우.
	{
		levelGap = KillerLevel - MonsterLevel ;													// 레벨 차이를 구한다.
	}

	int nLevelValue = 0 ;																		// 상황에 따른 레벨 겝 변수를 둔다.

	if( nMemberCount >= 2 )																		// 파티원이 2명 이상이면,
	{
		nLevelValue = 8 ;																		// 레벨 겝을 8레벨로 설정한다.
	}
	else																						// 2인 미만이면,
	{
		nLevelValue = 5 ;																		// 레벨 겝을 5렐벨로 설정한다.
	}


	// 레벨 차에 따른 처리
	if( MonsterLevel > KillerLevel + nLevelValue )												// 몬스터 레벨이 플레이어 레벨 보다 5레벨 이상일 때.
	{
		double dwLevel = KillerLevel+nLevelValue ;												// 레벨 포인트 세팅.
		// 080116 LYW --- Distributer : HP가 절반인 몬스터를 잡았을 때, 획득 경험치 보정 처리.
		double dwPenalty = 1.95f ;																// 페널티 포인트 세팅.
		double result = pow(dwLevel, dwPenalty) ;												// 레벨 포인트 ^ 페널티 포인트를 구한다.

		float fExp = (float)(result + 15) ;														// 플로트형의 크기 내에서 경험치를 구한다.

		if( fExp > dwExp )
		{
			return dwExp ;
		}
		else
		{
			return (DWORD)floor(fExp) ;															// 경험치를 반올림 하여 DWORD형으로 반환.
		}
	}
	else if( MonsterLevel < KillerLevel )														// 플레이어 레벨이 몬스터 레벨보다 큰 경우.
	{
		float fPenalty = 1.0f ;																	// 페널티 포인트.
		float fExp     = 0.0f ;																	// 플로트형 경험치.

		// 080116 LYW --- Distributer : 데이트 매칭 존일 경우에는 패널티를 주지 않는다.
		if (!g_csDateManager.IsChallengeZoneHere())
		{
			if( levelGap >= 6 && levelGap <= 10 )												// 플레이어 레벨이 +6~+10일 때.
			{
				fPenalty = 0.5f ;																// 페널티 포인트 50%로 세팅
			}
			else if( levelGap >= 11 && levelGap <= 15 )											// 플레이어 레벨이 +11~+15일 때.
			{
				fPenalty = 0.25f ;																// 페널티 포인트 25%로 세팅.
			}
			else if( levelGap > 15 )															// 플레이어 레벨이 +15 이상일 때.
			{
				return 1 ;																		// 경험치는 무조건 1로 반환한다.
			}
		}

		fExp = (float)(dwExp * fPenalty) ;														// 경험치를 플로트형으로 구한다.

		return (DWORD)floor(fExp) ;																// DWORD형으로 페널티 경험치 반환.
	}	
	else																						// 몬스터와 플레이어 레벨이 같을 때.
	{
		return dwExp ;																			// 플레이어와 몬스터의 레벨이 같을경우 몬스터 경험치 반환.			
	}
}

BOOL CDistributer::GetFirstDamange()
{
	m_FirstDamage = 0 ;																			// 첫 데미지 변수를 0으로 세팅한다.
	
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(m_1stPlayerID) ;						// 첫 데미지를 준 플레이어 정보를 받는다.

	if( pPlayer )																				// 플레이어 정보가 유효한지 체크한다.
	{
		DAMAGEOBJ* pDObj = NULL ;																// 데미지 정보를 담을 포인터 변수를 선언한다.

		CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;							// 플레이어의 파티정보를 받는다.

		if( pParty )																			// 파티에 속해있다면,
		{
			pDObj = m_DamageObjectTableParty.GetData(pParty->GetPartyIdx()) ;					// 파티의 데미지 정보를 받는다.

			if( pDObj )																			// 데미지 정보가 유효한지 체크한다.
			{
				m_FirstDamage = pDObj->dwData ;													// 첫 데미지를 세팅한다.

				return TRUE ;																	// 리턴 TRUE.
			}
		}
		else																					// 파티에 속해있지 않다면,
		{
			pDObj = m_DamageObjectTableSolo.GetData(pPlayer->GetID()) ;							// 플레이어의 데미지 정보를 받는다.

			if( pDObj )																			// 데미지 정보가 유효한지 체크한다.
			{
				m_FirstDamage = pDObj->dwData ;													// 첫 데미지를 세팅한다.

				return TRUE ;																	// 리턴 TRUE.
			}
		}
	}

	return FALSE ;																				// 리턴 FALSE.
}

void CDistributer::GetTotalKillerCount()
{
	DAMAGEOBJ* obj = NULL ;																		// 데미지 정보를 담을 포인터 변수를 선언한다.

	CPlayer* pPlayer = NULL ;																	// 플레이어 정보를 받을 포인터 변수를 선언한다.

	CParty* pParty = NULL ;																		// 파티 정보를 받을 포인터 변수를 선언한다.

	m_nTotalKillerCount = 0 ;																	// 킬러 카운트를 0으로 세팅한다.

	m_DamageObjectTableSolo.SetPositionHead() ;													// 개인 데미지 테이블 체크.

	while(obj = m_DamageObjectTableSolo.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// 데미지에 대한 아이디로 플레이어 정보를 받는다.

				if( pPlayer )																	// 플레이어 정보가 유효한지 체크한다.
				{
					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// 플레이어의 파티 정보를 받는다.

					if( !pParty )																// 파티가 없다면, 
					{
						++m_nTotalKillerCount ;													// 솔로 플레이어 수를 늘린다.
					}
				}
			}
		}
	}

	m_DamageObjectTableParty.SetPositionHead() ;												// 파티 테이블 체크.

	while(obj = m_DamageObjectTableParty.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// 파티 정보를 받는다.

				if( pParty )																	// 파티 정보가 유효한지 체크한다.
				{
					++m_nTotalKillerCount ;														// 솔로 플레이어 수를 늘린다.
				}
			}
		}
	}
}

void CDistributer::GetHighLevelOfKillers()
{
	DAMAGEOBJ* obj = NULL ;																		// 데미지 정보를 담을 포인터 변수를 선언한다.

	CPlayer* pPlayer = NULL ;																	// 플레이어 정보를 받을 포인터 변수를 선언한다.

	CParty* pParty = NULL ;																		// 파티 정보를 받을 포인터 변수를 선언한다.

	m_nTotalKillerCount = 0 ;																	// 킬러 카운트를 0으로 세팅한다.

	m_DamageObjectTableSolo.SetPositionHead() ;													// 개인 데미지 테이블 체크.

	while(obj = m_DamageObjectTableSolo.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID) ;						// 데미지에 대한 아이디로 플레이어 정보를 받는다.

				if( pPlayer )																	// 플레이어 정보가 유효한지 체크한다.
				{
					pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx()) ;						// 플레이어 아이디로 파티 정보를 받는다.

					if( !pParty )
					{
						LEVELTYPE playerLevel = pPlayer->GetLevel() ;							// 플레이어의 레벨을 받는다.

						if( playerLevel > m_HighLevel )											// 플레이어의 레벨이 최고 레벨보다 크다면,
						{
							m_HighLevel = playerLevel ;											// 최고 레벨에 플레이어 레벨을 세팅한다.
						}
					}
				}
			}
		}
	}

	m_DamageObjectTableParty.SetPositionHead() ;												// 파티 데미지 테이블 체크.

	while(obj = m_DamageObjectTableParty.GetData())												// 데미지 테이블에서 데미지 오브젝트를 받는다.
	{
		if( obj )
		{
			if(obj->dwData > 0)																	// 데미지가 있는지 여부를 체크한다.
			{
				pParty = PARTYMGR->GetParty(obj->dwID) ;										// 데미지에 대한 아이디로 파티 정보를 받는다.

				if( pParty )																	// 플레이어 정보가 유효한지 체크한다.
				{
					LEVELTYPE highLevel = 0 ;													// 
					LEVELTYPE totalLevel = 0 ;

					GetPartyLevels(pParty, &highLevel, &totalLevel) ;							// 플레이어의 레벨을 받는다.

					if( highLevel > m_HighLevel )												// 플레이어의 레벨이 최고 레벨보다 크다면,
					{
						m_HighLevel = highLevel ;												// 최고 레벨에 플레이어 레벨을 세팅한다.
					}
				}
			}
		}
	}
}

int CDistributer::GetLevelPenaltyToMonster(LEVELTYPE level) 
{
	LEVELTYPE MonsterLevel = m_pMob->GetSMonsterList()->Level ;									// 몬스터 레벨을 받는다.
	LEVELTYPE levelGap	   = 0 ;																// 몬스터와 플레이어의 레벨 차이.

	int nPenaltyType = 0 ;																		// 패널티 타입을 담을 변수.
	
	if( MonsterLevel > level )																	// 몬스터 레벨이 플레이어 보다 큰 경우.
	{
		levelGap = 0 ;																			// 레벨 차이를 구한다.
	}
	else																						// 플레이어 레벨이 몬스터 레벨 보다 큰 경우.
	{
		levelGap = level - MonsterLevel ;														// 레벨 차이를 구한다.
	}

	if( levelGap > 5 && levelGap < 11 )															// 레벨차이가 6~10이면, 
	{
		nPenaltyType = 1 ;																		// 드랍률 50% 감소.
	}
	else if( levelGap > 10 && levelGap < 16 )													// 레벨차이가 11~15이면, 
	{
		nPenaltyType = 2 ;																		// 드랍확률 75% 감소.
	}
	else if( levelGap > 15 )																	// 레벨 차이가 15이상이면,
	{
		nPenaltyType = 3 ;																		// 아이템 드랍 없다.
	}

	return nPenaltyType ;
}

void CDistributer::GetPartyLevels(CParty* pParty, LEVELTYPE* pHighLevel, LEVELTYPE* pTotalLevel)
{
	if( !pParty || !m_pKilledPosition || !pHighLevel || !pTotalLevel )							// 파티와 플레어어 정보가 유효한지 체크한다.
	{
		return ;
	}

	CPlayer* pMember = NULL ;																	// 멤버 정보를 담을 포인터 변수.

	DWORD PlayerID = 0 ;																		// 멤버들의 아이디를 받을 변수.

	LEVELTYPE totallevel = 0 ;																	// 파티 멤버들의 레벨 총 합.
	LEVELTYPE highlevel = 0 ;																	// 파티 멤버 중 최대 레벨.
	LEVELTYPE curlevel = 0 ;																	// 멤버들의 레벨을 받을 변수.
	
	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)									// 로그인 중인 파티원 수와 그 중 최고 레벨을 구한다.
	{
		PlayerID = pParty->GetMemberID(count);													// 카운트 대로 멤버의 아이디를 받는다.

		pMember = (CPlayer*)g_pUserTable->FindUser(PlayerID);									// 멤버 플레이어 정보를 받는다.

		if( pMember )																			// 멤버 정보가 유효한지 체크.
		{
			if( pMember->GetCurMapNum() == (DWORD)(g_pServerSystem->GetMapNum()) )				// 같은 맵에 있는지 체크한다.
			{
				if(Chk(pMember, m_pKilledPosition, m_pMob->GetGridID()))						// 경험치를 받을 수 있는 범위 내에 있는지 체크.
				{
					curlevel = pMember->GetLevel();												// 현제 멤버의 레벨을 받는다.

					if(highlevel < curlevel)													// 파티 내 최대 레벨보다 큰지 체크 한다.
					{
						highlevel = curlevel;													// 최대 레벨을 갱신한다.
					}

					totallevel += curlevel;														// 파티 레벨을 더한다.																				
				}
			}
		}
	}

	*pHighLevel = highlevel ;																	// 최고 레벨을 세팅한다.
	*pTotalLevel = totallevel ;																	// 파티 레벨의 합을 세팅한다.
}

void CDistributer::SaveCandidacy(CParty* pParty)
{
	if(!m_pMob || !pParty || !m_pKilledPosition) return ;										// 플레이어와 몬스터 정보를 체크한다.

	CPlayer* pMember = NULL ;																	// 멤버 정보를 담을 포인터 변수.
	DWORD dwMemberID = 0 ;																		// 멤버 아이디를 담을 변수.

	PTRLISTPOS pos = m_CandidacyList.GetHeadPosition() ;										// 후보리스트를 해드로 세팅한다.

    DWORD* pMemberID = NULL ;																	// 멤버 아이디를 받을 포인터 변수 선언.

	while(pMemberID = (DWORD*)m_CandidacyList.GetNext(pos))										// 멤버 아이디 정보가 없을때까지 while
	{
		if(pMemberID)																			// 멤버 정보가 있다면,
		{
			delete pMemberID ;																	// 멤버 정보를 삭제한다.
			pMemberID = NULL ;																	// 멤버 정보 포인터를 NULL처리 한다.
		}
	}

	m_CandidacyList.RemoveAll() ;																// 후보 리스트를 비운다.

	m_CandidacyList.GetHeadPosition() ;															// 후보 리스트를 해드로 세팅한다.

	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)									// 최대 파티 멤버 수 만큼 포문.
	{
		dwMemberID = pParty->GetMemberID(count);												// 멤버의 아이디를 받는다.

		pMember = (CPlayer*)g_pUserTable->FindUser(dwMemberID);									// 멤버 정보를 받는다.

		if( pMember )																			// 멤버 정보가 유효한지 체크한다.
		{
			// 071215 LYW --- Distributer : 파티 멤버가 죽은 상태에서는, 보상멤버로 추가하지 않는다.
			if( pMember->GetState() == eObjectState_Die ) continue ;

			if( pMember->GetCurMapNum() == (DWORD)(g_pServerSystem->GetMapNum()) )				// 같은 맵에 있는지 체크한다.					
			{
				if(Chk(pMember, m_pKilledPosition, m_pMob->GetGridID()))						// 보상 받을 수 있는 영역인지 체크한다.
				{
					DWORD* pCandidacyID = new DWORD ;											// 후보 아이디를 생성한다.
					*pCandidacyID = dwMemberID ;												// 후보 아이디를 세팅한다.
					m_CandidacyList.AddTail(pCandidacyID) ;										// 후보 리스트에 등록한다.
				}
			}
		}
	}
}

void CDistributer::DistribuSequence(CParty* pParty)
{
	if( !pParty ) return ;																		// 파티 정보가 유효한지 체크한다.

	int nWinnerIdx = (int)pParty->GetWinnerIdx() ;												// 파티의 당첨 번호를 받아온다.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// 후보자 수를 받는다.

	if( !pParty || nCandidacyCount <= 0 ) return ;												// 파티 정보가 유효한지, 후보자 수는 이상 없는지 체크한다.

	PTRLISTPOS pos = NULL ;																		// 리스트의 위치 정보를 담을 포인터를 받아온다.

	CPlayer* pMember = NULL ;																	// 멤버 정보를 담을 포인터를 선언한다.

	DWORD* pMemberID = NULL ;																	// 멤버 아이디를 받을 포인터 변수를 선언한다.

	if( nWinnerIdx >= nCandidacyCount )															// 현재 당첨 번호보다 후보자 수가 작거나 같다면,
	{
		pParty->SetWinnerIdx(0) ;																// 당첨번호를 맨처음으로 세팅한다.
		pos = m_CandidacyList.GetHeadPosition() ;												// 후보자 리스트의 해드 포지션을 받는다.
	}
	else if( nWinnerIdx < nCandidacyCount )														// 당첨번호보다 후보자 수가 적으면,
	{
		pos = m_CandidacyList.FindIndex(nWinnerIdx) ;											// 당첨 번호에 해당하는 위치를 받는다.
	}

	if( pos )																				// 위치 정보가 유효한지 체크한다.
	{
		pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// 당첨 위치에 해당하는 멤버 아이디를 받는다.

		if( pMemberID )																		// 멤버 아이디 주소가 유효한지 체크한다.
		{
			pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID) ;						// 멤버 아이디로 멤버 정보를 받는다.

			if( pMember )																	// 멤버 정보가 유효한제 체크한다.
			{
				// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
				if( !IsSameChannel(pMember, m_pMob) ) return ;

				SendItemToPerson( pMember ) ;												// 아이템을 멤버에게 전송한다.
			}
		}
	}
}

void CDistributer::DistributeDamage(CParty* pParty)
{
	if( !pParty ) return ;																		// 파티 정보가 유효한지 체크한다.

	DAMAGEOBJ* obj = NULL ;																		// 데미지 오브젝트 포인터.
	PTRLISTPOS pos = NULL ;																		// 후보자 리스트의 위치 포인터.

	DWORD bigDamage = 0 ;																		// 가장 큰 데미지.
	DWORD bigID = 0 ;																			// 가장 큰 데미지를 준 아이디.

	DWORD* pMemberID = NULL ;																	// 멤버 아이디를 받을 포인터.
	CPlayer* pMember = NULL ;																	// 멤버 정보를 받을 포인터.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// 후보자 수를 받는다.

	m_DamageObjectTableSolo.SetPositionHead() ;													// 데미지 테이블을 해드로 세팅한다.

	for(int n=0;n<nCandidacyCount;++n)															// 후보자 수 대로 포문을 돌면서 멤버 정보를 체크.
	{
		pos = m_CandidacyList.FindIndex(n) ;													// 후보자 리스트에서 해당하는 포인터를 받아온다.

		if( pos )																				// 포인터 정보가 유효한지 체크한다.
		{
			pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;									// 참조한 포인터에서 멤버 아이디를 받는다.

			if( pMemberID )																		// 멤버 아이디 정보가 유효한지 체크한다.
			{
				obj = m_DamageObjectTableSolo.GetData(*pMemberID) ;								// 데미지 테이블에서 아이디로 데미지 정보를 받는다.

				if( obj )																		// 데미지 정보가 유효한지 체크한다.
				{
					if(bigDamage < obj->dwData)													// 최대 데미지와 멤버의 데미지 비교.
					{
						bigDamage = obj->dwData ;												// 최대 데미지에 멤버의 데미지를 대입.
						bigID = obj->dwID ;														// 최대 데미지를 준 멤버의 아이디를 저장.				
					}
				}
			}
		}
	}

	if( bigID == 0 ) return ;																	// 가장 큰 데미지의 멤버 아이디를 체크한다.

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(bigID);									// 멤버 정보를 받는다.

	if( pPlayer )																				// 멤버 정보가 유효한지 체크.
	{
		// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
		if( !IsSameChannel(pPlayer, m_pMob) ) return ;

		SendItemToPerson( pPlayer ) ;															// 아이템을 전송한다.							
	}
}

void CDistributer::DistributeRandom(CParty* pParty)
{
	if( !pParty ) return ;																		// 파티 정보가 유효한지 체크한다.

	int nCandidacyCount = m_CandidacyList.GetCount() ;											// 후보자 수를 받는다.

	if( nCandidacyCount == 0 ) return ;															// 후보자가 없으면 리턴.

	CPlayer* pMember = NULL ;																	// 당첨자 정보 포인터.

	PTRLISTPOS pos = NULL ;																		// 리스트 위치 포인터.

	DWORD* pMemberID = NULL ;																	// 멤버 아이디 포인터.
    
	if( nCandidacyCount > 1 )																	// 후보자 수가 한명 이상이면, 
	{
		int ranNum =  random(0, nCandidacyCount-1) ;											// 당첨자 번호를 뽑는다.

		pos = m_CandidacyList.FindIndex(ranNum ) ;												// 당첨 된 번호의 포인터를 참조한다.
	}

	if( pos )																					// 리스트의 포지션 포인터 정보가 유효한지 체크한다.
	{
		pMemberID = (DWORD*)m_CandidacyList.GetAt(pos) ;										// 참조한 포인터의 멤버 아이디를 받는다.
	}
		
	if( pMemberID )																				// 멤버 아이디 정보가 유효한지 체크한다.
	{
		pMember = (CPlayer*)g_pUserTable->FindUser(*pMemberID);									// 당첨자 정보를 받는다.
	}

	if( pMember )																				// 당첨자 정보가 유효한지 체크.
	{
		// 080122 LYW --- Distributer : Player 와 Monster의 채널을 확인한다.
		if( !IsSameChannel(pMember, m_pMob) ) return ;

		SendItemToPerson( pMember ) ;															// 아이템을 전송한다.
	}
}





//=========================================================================
//	NAME : IsSameChannel
//	DESC : The function to check channel of player and monster. 080122 LYW
//  현 시점에서, 몬스터와 PLAYER의 채널이 다르면, 아이템, 돈, 경험치 분배를
//  하지 않도록 처리하기로 했으므로, Player나 Monster 정보가 유효하지 않으면,
//  false 리턴 처리를 한다.
//=========================================================================
BOOL CDistributer::IsSameChannel(CPlayer* pPlayer, CMonster* pMonster)
{
	BOOL bResult = FALSE ;														// 결과를 담을 변수.

	if( !pPlayer ) return bResult ;												// Player 정보를 체크.

	if( !pMonster ) return bResult ;											// Monster 정보를 체크.
/*
	DWORD dwLastKillerID	= 0 ;												
	dwLastKillerID			= pMonster->GetLastKillerID() ;						// 마지막으로 Monster를 Kill 한 Player 정보를 받는다.

	CPlayer* pLastKiller	= NULL ;
	pLastKiller = (CPlayer*)g_pUserTable->FindUser( dwLastKillerID ) ;			// 마지막 Killer 정보를 받는다.

	if( !pLastKiller ) return bResult ;											// Killer 정보 체크.

	DWORD dwPlayerChannel		= pPlayer->GetChannelID() ;						// Player 와 마지막 Killer 채널을 받는다.
	DWORD dwLastKillerChannel	= pLastKiller->GetChannelID() ;

	if( dwPlayerChannel == dwLastKillerChannel )								// 각 채널을 비교.
	{
		bResult = TRUE ;														// 같은 채널이면 true 세팅.
	}
*/
	if( pPlayer->GetGridID() == pMonster->GetGridID() )
	{
		bResult = TRUE ;														// 같은 채널이면 true 세팅.
	}

    return bResult ;															// 결과를 리턴한다.
}

//void CDistributer::DistribuSequence(int nCandidadyCount, CParty* pParty, WORD dwCurMap)
//{
//	BYTE winnerIdx = winnerIdx = pParty->GetWinnerIdx() ;									// 당첨자 번호.
//
//	BOOL bSendItem = FALSE ;
//
//	DWORD PlayerID = 0 ;																	// 당첨자 아이디.
//
//	for(int i=0; i<MAX_PARTY_LISTNUM; i++)													// 최대 파티 멤버 수 대로 포문을 돌면서 체크.
//	{
//		PlayerID = pParty->GetMemberID(i) ;													// 멤버의 아이디를 받는다.
//
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;						// 멤버 정보를 받는다.
//
//		if( pPlayer )																		// 멤버 정보가 유효한지 체크 한다.
//		{
//			WORD dwMapNum = pPlayer->GetCurMapNum() ;										// 멤버의 현재 맵 번호를 확인한다.
//
//			if( dwCurMap == dwMapNum )														// 몬스터를 잡은 맵 번호와 멤버의 현재 맵을 비교.
//			{
//				if(ChkExp(pPlayer, m_pKilledPosition, m_pMob->GetGridID()))					// 아이템을 받을 수 있는 영역 안에 있는지 체크.
//				{
//					if( winnerIdx == i )													// 당첨자 번호와 멤버 번호가 같은지 비교.
//					{
//						bSendItem = TRUE ;
//						SendItemToPerson( pPlayer ) ;										// 아이템을 전송한다.
//						
//						return ;
//					}
//				}
//			}
//		}
//		else
//		{
//			if(winnerIdx == MAX_PARTY_LISTNUM-1 )											// 후보자 번호를 체크 한다.
//			{
//				pParty->SetWinnerIdx(0) ;													// 후보자 번호를 처음으로 세팅.
//			}
//			else
//			{
//				pParty->SetWinnerIdx(++winnerIdx) ;											// 후보자 번호를 증가한다.
//			}
//		}
//	}
//
//	if( !bSendItem )																		// 아이템을 받은 사람이 없으면, 
//	{
//		winnerIdx = pParty->GetWinnerIdx() ;												// 당첨자 번호.
//
//		for(int i=0; i<MAX_PARTY_LISTNUM; i++)												// 한번만 더 최대 파티 멤버 수 대로 포문을 돌면서 체크.
//		{
//			PlayerID = pParty->GetMemberID(i) ;												// 멤버의 아이디를 받는다.
//
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;					// 멤버 정보를 받는다.
//
//			if( pPlayer )																	// 멤버 정보가 유효한지 체크 한다.
//			{
//				WORD dwMapNum = pPlayer->GetCurMapNum() ;									// 멤버의 현재 맵 번호를 확인한다.
//
//				if( dwCurMap == dwMapNum )													// 몬스터를 잡은 맵 번호와 멤버의 현재 맵을 비교.
//				{
//					if(ChkExp(pPlayer, m_pKilledPosition, m_pMob->GetGridID()))				// 아이템을 받을 수 있는 영역 안에 있는지 체크.
//					{
//						if( winnerIdx == i )												// 당첨자 번호와 멤버 번호가 같은지 비교.
//						{
//							SendItemToPerson( pPlayer ) ;									// 아이템을 전송한다.
//							
//							return ;
//						}
//					}
//				}
//			}
//			else
//			{
//				if(winnerIdx == MAX_PARTY_LISTNUM-1 )										// 후보자 번호를 체크 한다.
//				{
//					pParty->SetWinnerIdx(0) ;												// 후보자 번호를 처음으로 세팅.
//				}
//				else
//				{
//					pParty->SetWinnerIdx(++winnerIdx) ;										// 후보자 번호를 증가한다.										
//				}
//			}
//		}
//	}
//}

//int CDistributer::GetCandidacyCount(CParty* pParty, CPlayer* pPlayer)
//{
//	if(!m_pMob || !pPlayer || !pParty || !m_pKilledPosition) return 0 ;				// 플레이어와 몬스터 정보를 체크한다.
//
//	int nCandidacyCount = 0 ;														// 보상을 받을 플레이어 수.
//
//	CPlayer* pMember = NULL ;														// 멤버 정보를 담을 포인터 변수.
//	DWORD dwMemberID = 0 ;															// 멤버 아이디를 담을 변수.
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)						// 최대 파티 멤버 수 만큼 포문.
//	{
//		dwMemberID = pParty->GetMemberID(count);									// 멤버의 아이디를 받는다.
//
//		pMember = (CPlayer*)g_pUserTable->FindUser(dwMemberID);						// 멤버 정보를 받는다.
//
//		if( pMember )																// 멤버 정보가 유효한지 체크한다.
//		{
//			if( pMember->GetCurMapNum() == pPlayer->GetCurMapNum() )				// 같은 맵에 있는지 체크한다.					
//			{
//				if(ChkExp(pMember, m_pKilledPosition, m_pMob->GetGridID()))			// 보상 받을 수 있는 영역인지 체크한다.
//				{
//					++nCandidacyCount ;												// 보상을 받을 플레이어 수 증가.
//				}
//			}
//		}
//	}
//
//	return nCandidacyCount ;														// 보상을 받을 플레이어 수 리턴.
//}


// 070413 LYW --- Distributer : Modified function Distribute.
/*
void CDistributer::Distribute(DWORD KillerID,VECTOR3* pKilledPosition, WORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
{
	DAMAGEOBJ *obj;
	DWORD Exp = 0;
	CPlayer* pReceivePlayer;
	
	DWORD BigPlayerID; //데미지를 가장 많이 줘서 수련치와 아이템 가져갈 사람/파티
	DWORD BigPartyID;
	DWORD BigPlayerDamage, BigPartyDamage;
	BigPlayerID = BigPartyID = BigPlayerDamage = BigPartyDamage = 0;
	
	//솔로 테이블 검사
	m_DamageObjectTableSolo.SetPositionHead();
	while(obj = m_DamageObjectTableSolo.GetData())
	{
		if(obj->dwData == 0)
			continue;
		////아이템과 수련치 받을 사람 선택하기 위해 가장 큰 데미지 준 사람 선택
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
		if( pMob->GetSMonsterList()->ExpPoint)	//호박 산타
			Exp = CalcObtainExp(pMob->GetLevel() ,pReceivePlayer->GetLevel(),pMob->GetSMonsterList()->Life,obj->dwData);
			//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//			Exp = CalcObtainExp(pMob->GetLevel(), pReceivePlayer->GetLevel(), pMob->GetSMonsterList()->Life, obj->dwData, pMob->GetSMonsterList()->ExpPoint );
		else
			Exp = 0;
		if(Exp == 0)
			continue;

		// 개인에게 경험치 보냄
		DISTRIBUTERAND->SendToPersonalExp(pReceivePlayer, Exp);
	}
	
	//파티
	DAMAGEOBJ* pPobj = NULL;
	CParty* pParty;
	m_DamageObjectTableParty.SetPositionHead();
	while(pPobj = m_DamageObjectTableParty.GetData())
	{
		////큰 데미지 준 파티 선택
		ChooseOne(pPobj, &BigPartyDamage, &BigPartyID);

		pParty = PARTYMGR->GetParty(pPobj->dwID);
		if(pParty==NULL) //파티가 해산됐으면 continue
			continue;
		if(pPobj->dwData == 0)
			continue;
		if(pParty->GetPartyIdx() == m_1stPartyID)
		{
			pPobj->dwData -= m_PlusDamage;
		}

		if( pMob->GetSMonsterList()->ExpPoint)	//호박 산타		
			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel());
//SW050806 기존 데이타 테이블 파싱에서 계산 공식화로 변경
//			CalcAndSendPartyExp(pParty, pPobj->dwData, pKilledPosition, pMob->GetSMonsterList()->Life, pMob->GetGridID(), KillerID, pMob->GetLevel(), pMob->GetSMonsterList()->ExpPoint);
	}

	//abil and item
	pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(BigPlayerID);
	pParty = PARTYMGR->GetParty(BigPartyID);

	if(BigPartyDamage < BigPlayerDamage) //개인에게 보냄
	{
		SendPersonalAbilandItem(pReceivePlayer, pMob, DropItemID, DropItemRatio);
	}
	else if(BigPartyDamage == BigPlayerDamage) //데미지가 같으면 랜덤으로 선택
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
		else //파티에게
		{	
			SendPartyAbilandItem(pParty, pKilledPosition, pMob->GetGridID(), pMob->GetLevel(), DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMob->GetSMonsterList(), pMob->GetMonsterKind());
		}
	}
	else //파티에게
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
//	// 경험치 분배를 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	while(obj = m_DamageObjectTableSolo.GetData())
//	{
//		if(obj->dwData > 0)
//		{
//			pReceivePlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);
//
//			if( pReceivePlayer )
//			{
//				// 큰데미지를 준 사람의 아이디를 저장한다.
//				if( dwDamageToGiveItem < obj->dwData )
//				{
//					dwItemID = pReceivePlayer->GetID() ;
//					dwDamageToGiveItem = obj->dwData ;
//				}
//                
//				// 몬스터가 죽은 자리에서 플레이어가 영역 내에 있는지 검사한다.
//				if(Chk(pReceivePlayer, pKilledPosition, pMob->GetGridID()))
//				{
//					// 분배 비교 조건인 파티 여부와 몬스터 경험치를 구한다.
//					pParty = PARTYMGR->GetParty(pReceivePlayer->GetPartyIdx());
//
//					DWORD dwExp	= CalcObtainExp(pMob, pBigPlayer->GetLevel()) ;
//
//					// 몬스터를 처음 가격한 플레이어 처리.
//					if(obj->dwID == m_1stPlayerID)
//					{
//						if(!pParty)												// 파티가 없으면 개인 경험치를 적용한다.
//						{
//							DistributeToFirstKiller(obj, dwExp) ;			
//						}
//						else													// 파티가 있으면 파티 경험치를 적용한다.
//						{
//							// 첫 킬러 인데 파티가 있으면 어떻게 처리 하나?
//							DistributeToParty(m_1stPlayerID, pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//						}
//
//						continue ;												// 한 사람의 처리가 끝났으면 다음 사람 처리로 넘어가자.
//					}
//
//					// 나중에 몬스터를 가격한 플레이어 처리
//					if(!pParty)													// 파티가 없으면 개인 경험치를 적용한다.
//					{
//						DistributeToOtherKiller(obj, nKillerCount, dwExp) ;	
//					}
//					else														// 파티가 있으면 파티 경험치를 적용한다.
//					{
//						DistributeToParty(pReceivePlayer->GetID(), pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//					}
//				}
//			}
//		}
//	}
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 아이템 분배를 한다. - 위의 몬스터를 처치 한 사람들 중 큰 데미지를 준 사람에게 아이템을 분배한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	CPlayer* pReceiveItemPlayer = (CPlayer *)g_pUserTable->FindUser(dwItemID) ;							// 아이템을 받을 플레이어를 받는다.
//
//	if( pReceiveItemPlayer )																			// 플레이어 정보가 유효한지 체크
//	{
//		CParty* pParty = PARTYMGR->GetParty(pReceiveItemPlayer->GetPartyIdx()) ;						// 파티가 있는지 체크하기 위해 파티를 받는다.
//
//		if( pParty )																					
//		{
//			WORD dwCurMap = pReceiveItemPlayer->GetCurMapNum() ;						// 몬스터를 처치한 맵 번호를 받는다.
//			DistributeItemToParty(pParty->GetPartyIdx(), pKilledPosition, DropItemID, DropItemRatio, pMob, obj, dwCurMap) ;	// 파티가 있다면 파티 아이템 분배를 한다.
//		}
//		else
//		{
//			PickUpAcquisition(pBigPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);			// 파티가 없으면 개인 아이템 분배를 한다.
//		}
//	}
//}

//void CDistributer::DistributeToFirstKiller(DAMAGEOBJ *obj, DWORD dwExp)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 처음 몬스터를 가격한 플레이어의 경험치를 계산 및 전송한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DWORD dwFirstDamage = obj->dwData ;											// 첫 데미지를 얻는다. 
//	DWORD dwMyDamage	= obj->dwData ;											// 플레이어의 데미지를 얻는다.
//
//	float fPriority		= ((float)dwMyDamage *0.3f) ;							// 첫 데미지에 대한 추가 데미지를 적용한다.
//
//	float fExpoint		= (float)(((dwMyDamage + fPriority) / m_TotalDamage)) ;	// 경험치 적용 포인트를 구한다.
//
//	if( fExpoint <= 0 )															// 혹시라도 포인트가 음수나 0 이 나오는지 체크
//	{
//		fExpoint = 1 ;
//	}
//	
//	DWORD dwSendExp = (DWORD)GetFloatRound(fExpoint * dwExp) ;					// 포인트를 계산하여 반올림 한 경험치를 구한다.
//
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);			// 전송 할 플레이어를 받는다.
//
//	if( pPlayer )																// 플레이어 정보가 유효한지 체크
//	{
//		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwSendExp) ;					// 경험치를 플레이어에게 전송한다.
//	}
//}

//void CDistributer::DistributeToOtherKiller(DAMAGEOBJ *obj, int nKillerCount, DWORD dwExp)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 추가로 몬스터를 가격한 플레이어의 경험치를 계산 및 전송한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DWORD dwMyDamage	= obj->dwData ;											// 플레이어의 데미지를 받는다.
//	float fPriority		= ((float)dwMyDamage *0.3f) ;							// 몬스터를 처음 가격한 데미지를 받는다.
//
//	float fExpA = fPriority / (nKillerCount-1) ;								// 경험치 적용 포인트 A를 구한다.
//	float fExpB = dwMyDamage - fExpA ;											// 경험치 적용 포인트 B를 구한다.
//
//	if( obj->dwData < fExpA )													// 포인트 A가 더 크면 아래 계산에 문제가 생기기 때문에
//	{																			// 예외 처리를 한다.
//		fExpB = 0 ;
//	}
//
//	float fExpC = fExpB / m_TotalDamage ;										// 경험치 적용 포인트 C를 구한다.
//	
//	float fSendExp = fExpC * dwExp ;											// 경험치를 구한다.
//
//	DWORD dwSendExp = (DWORD)GetFloatRound(fSendExp) ;							// 경험치를 반올림 한다.
//	
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(obj->dwID);			// 전송 할 플레이어를 받는다.
//
//	if( pPlayer )																// 플레이어 정보가 유효한지 체크
//	{
//		DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwSendExp) ;					// 경험치를 플레이어에게 전송한다.
//	}
//}

//void CDistributer::DistributeToKiller(DWORD KillerID, CMonster* pMob, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, DWORD BigPlayerDamage, CPlayer* pBigPlayer, DAMAGEOBJ *obj)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 몬스터를 처치 한 플레이어가 한명일 때 처리를 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(KillerID);								// 몬스터를 처치 한 플레이어를 받는다.
//
//	if( pPlayer )																				// 플레이어 정보가 유효한지 체크한다.
//	{
//		CParty* pParty = NULL ;																	// 파티 여부를 체크하기 위해 파티 포인터 생성.
//		
//		pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());									// 플레이어의 파티를 받는다.
//
//		if(!pParty)																				// 파티가 없을 때 처리.
//		{
//			DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;							// 몬스터의 경험치를 받아 온다.
//
//			DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;									// 플레이어에게 경험치를 전송한다.
//			PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);		// 플레이어에게 아이템을 전송한다.
//		}
//		else																					// 파티가 있을 때는 파티 분배처리로 넘긴다.
//		{
//			// 파티 경험치 처리를 한다.
//			DistributeToParty(KillerID, pKilledPosition, pMob, BigPlayerDamage, pBigPlayer, DropItemID, DropItemRatio, obj) ;
//			// 파티가 있다면 파티 아이템 분배를 한다.
//			WORD dwCurMap = pPlayer->GetCurMapNum() ;											// 몬스터를 처치한 맵 번호를 받는다.
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
//	// 파티의 경험치 처리를 한다.
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
//	// 첫 킬러인지 체크 해서 파티 경험치 처리만 한다.
//
//	//DAMAGEOBJ* pPobj = NULL;
//	//CParty* pParty	 = NULL ;
//
//	//// 큰 데미지를 준 파티를 선택한다.
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
//	////파티
//	//pPobj  = NULL ;
//	//pParty = NULL ;
//
//	//m_DamageObjectTableParty.SetPositionHead();
//
//	//// 파티의 경험치 처리를 한다.
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
//	//// 아이템 분배 처리를 한다.
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
//	// 파티 아이템 분배를 처리 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CParty* pBigParty = PARTYMGR->GetParty(BigPartyID) ;					// 아이템을 받을 플레이어의 파티를 받는다.
//
//	if( pBigParty )															// 파티 정보가 유효한지 체크.
//	{
//		int onlinenumconfirm = 0 ;											// 로그인 중인 멤버를 저장한는 변수 선언. 
//
//		DWORD bigDamage = 0 ;
//		DWORD bigID = 0 ;
//
//		DWORD PlayerID = 0 ;
//		CPlayer* pPlayer = NULL ;
//
//		// 가장 큰 데미지와 가장 큰 데미지를 준 플레이어,  그리고 로그인 중인 멤버 수를 구한다.
//
//		for(int n=0;n<MAX_PARTY_LISTNUM;++n)								// 최대 파티멤버 수 대로 포문을 돌면서 멤버 정보를 체크.
//		{
//			PlayerID = pBigParty->GetMemberID(n);							// 멤버 아이디를 받는다.
//			
//			pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);			// 멤버 정보를 받는다.
//
//			if( pPlayer )													// 멤버 정보가 유효한지 체크.
//			{
//				if(ChkExp(pPlayer, pKilledPosition,pMob->GetGridID()))		// 몬스터가 죽은 자리에서 유효 범위 안인지 체크.
//				{
//					m_DamageObjectTableSolo.SetPositionHead();				// 데미지 테이블을 헤더로 세팅한다.
//
//					while(obj = m_DamageObjectTableSolo.GetData())			// 데미지 테이블을 검사한다.
//					{
//						if(obj->dwData > 0)									// 데미지가 주어졌다면.
//						{
//							if( obj->dwID == pPlayer->GetID() )				// 데미지를 준 플레이어의 아이디를 받는다.
//							{
//								if(bigDamage < obj->dwData)					// 최대 데미지와 플레이어의 데미지 비교.
//								{
//									bigDamage = obj->dwData ;				// 최대 데미지에 플레이어의 데미지를 대입.
//									bigID = obj->dwID ;						// 최대 데미지를 준 플레이어의 아이디를 저장.			
//								}
//							}
//						}
//					}
//
//					++onlinenumconfirm;										// 로그인 중인 멤버 변수 증가.
//				}
//			}
//		}
//
//		if( onlinenumconfirm == 0 ) return ;								// 로그인 중인 멤버가 없으면 리턴.
//
//		if(onlinenumconfirm == 1)											// 로그인 중인 멤버가 한명이면 자기자신 뿐, 개인 분배 처리를 한다.
//		{
//			pPlayer = (CPlayer *)g_pUserTable->FindUser(bigID);				// 플레이어를 받는다.
//
//			if( pPlayer )													// 플레이어 정보가 유효한지 체크.													
//			{
//				DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;	// 몬스터 경험치를 받아 온다.
//
//				PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);	// 개인에게 아이템을 분배 한다.
//
//				return ;													// 리턴 처리.
//			}
//		}
//
//		// 아이템 분배를 위해 아이템 분배 설정을 받는다.
//		switch(pBigParty->GetOption())										
//		{
//		// 순차 분배 
//		case ePartyOpt_Sequence : DistribuSequence(onlinenumconfirm, pBigParty, pMob, DropItemID, DropItemRatio, pKilledPosition, dwCurMap) ;	break ;
//		// 데미지 분배
//		case ePartyOpt_Damage :	  DistributeDamage(bigID, pKilledPosition, DropItemID, DropItemRatio, pMob) ;	break ;
//		// 렌덤 분배
//		case ePartyOpt_Random :	  DistributeRandom(onlinenumconfirm, pBigParty, pMob, pKilledPosition, DropItemID, DropItemRatio) ; 	break ;
//		}
//	}
//}

//void CDistributer::DistribuSequence(int onlinenumconfirm, CParty* pBigParty, CMonster* pMob, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, WORD dwCurMap)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 아이템을 순차적으로 분배한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm > 1 )																	// 후보자 수 체크.
//	{
//		BYTE winnerIdx = winnerIdx = pBigParty->GetWinnerIdx() ;								// 당첨자 번호.
//
//		DWORD PlayerID = 0 ;																	// 당첨자 아이디.
//
//		for(int i=0; i<MAX_PARTY_LISTNUM; i++)													// 최대 파티 멤버 수 대로 포문을 돌면서 체크.
//		{
//			//winnerIdx = pBigParty->GetWinnerIdx() ;												// 당첨자 번호를 뽑는다.
//
//			//PlayerID = pBigParty->GetMemberID(winnerIdx);										// 당첨자 아이디를 받는다.
//
//			//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);						// 당첨자 정보를 받는다.
//
//			//if( pPlayer )																		// 당첨자 정보가 유효한지 체크.
//			//{
//			//	PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);	// 당첨자에게 아이템 전송.
//
//			//	return ;																		// 리턴 처리.
//			//}
//
//			PlayerID = pBigParty->GetMemberID(i) ;												// 멤버의 아이디를 받는다.
//
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID) ;						// 멤버 정보를 받는다.
//
//			if( pPlayer )																		// 멤버 정보가 유효한지 체크 한다.
//			{
//				WORD dwMapNum = pPlayer->GetCurMapNum() ;										// 멤버의 현재 맵 번호를 확인한다.
//
//				if( dwCurMap == dwMapNum )														// 몬스터를 잡은 맵 번호와 멤버의 현재 맵을 비교.
//				{
//					if(ChkExp(pPlayer, pKilledPosition,pMob->GetGridID()))						// 아이템을 받을 수 있는 영역 안에 있는지 체크.
//					{
//						if( winnerIdx == i )													// 당첨자 번호와 멤버 번호가 같은지 비교.
//						{
//							// 아이템을 전송한다.
//							PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition, onlinenumconfirm);
//
//							//if(winnerIdx == MAX_PARTY_LISTNUM-1 )								// 당첨자 번호가 한바퀴 돌았는지 체크.
//							//{
//							//	pBigParty->SetWinnerIdx(0) ;									// 당첨자 번호를 리셋.
//							//}
//							//else																// 당첨자 번호가 한바퀴를 돌지 않았다면.
//							//{
//							//	pBigParty->SetWinnerIdx(++winnerIdx) ;							// 당첨자 번호를 다음 번호로 세팅.
//							//}
//
//							// 리턴 처리를 한다.
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
//	// 가장 큰 데미지를 준 멤버에게 아이템을 분배 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(bigID);									// 플레이어를 받는다.
//
//	if( pPlayer )																				// 플레이어 정보가 유효한지 체크.
//	{
//		PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);			// 아이템을 전송한다.
//	}
//}
//
//void CDistributer::DistributeRandom(int onlinenumconfirm, CParty* pBigParty, CMonster* pMob, VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 렌덤하게 아이템을 분배 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm >= 1 )																	// 후보자 수 체크.
//	{
//		DWORD PlayerID = 0 ;																	// 당첨자 아이디.
//		CPlayer* pPlayer = NULL ;																// 당첨자 정보 포인터.
//
//		int ranNum =  random(0, onlinenumconfirm-1) ;											// 당첨자 번호를 뽑는다.
//
//		PlayerID = pBigParty->GetMemberID(ranNum);												// 당첨자 아이디를 받는다.
//		
//		pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);									// 당첨자 정보를 받는다.
//
//		if(pPlayer)																				// 당첨자 정보가 유효한지 체크.
//		{
//			PickUpAcquisition(pPlayer, pMob, DropItemID, DropItemRatio, pKilledPosition);		// 아이템을 전송한다.
//			return ;
//		}
//	}
//}

//void CDistributer::Distribute(DWORD KillerID,VECTOR3* pKilledPosition, DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob)
//{
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 가장 큰 데미지를 준 플레이어와 몬스터를 죽인 플레이어 카운트를 구한다.
//    ///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	DAMAGEOBJ *obj = NULL ;													// 데미지 오브젝트.
//
//	DWORD BigPlayerID		= 0 ;											// 가장 큰 데미지를 준 플레이어 아이디.
//	DWORD BigPlayerDamage	= 0 ;											// 가장 큰 데미지.
//
//	CPlayer* pBigPlayer = NULL ;											// 가장 큰 데미지를 준 플레이어 포인터.
//
//	int nKillerCount = 0 ;													// 몬스터를 처치 한 플레이어 수.
//
//	m_DamageObjectTableSolo.SetPositionHead();								// 데미지 오브젝트 테이블을 처음으로 세팅.
//
//	while(obj = m_DamageObjectTableSolo.GetData())							// 데미지 오브젝트가 유효한 동안 체크.
//	{
//		if(obj->dwData > 0)													// 데미지가 있는지 체크.
//		{
//			ChooseOne(obj, &BigPlayerDamage, &BigPlayerID) ;				// 가장 큰 데미지와 플레이어 아이디를 받는다.
//
//			pBigPlayer = (CPlayer *)g_pUserTable->FindUser(BigPlayerID);	// 가장 큰 데미지를 준 플레이어 정보를 받는다.
//
//			if( pBigPlayer ) ++nKillerCount ;								// 플레이어 정보가 유효하면 킬러 카운트 증가.
//		}
//	}
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 보상 분배를 시작한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	if( nKillerCount > 1 )														
//	{
//		// 몬스터를 처치 한 플레이어가 다수일 때 처리.
//		DistributeToKillers(KillerID, obj, pKilledPosition, pMob, pBigPlayer, nKillerCount, BigPlayerDamage, DropItemID, DropItemRatio) ;
//	}
//	else
//	{
//		// 몬스터를 처치 한 플레이어가 한명일 때 처리.
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
	//SW050806 기존 데이타 테이블 파싱에서 공식화로 변경
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

		// 06. 06. 파티 경험치 분배 수정 - 이영준
		// 분배대상이 2명이상일때만 추가 경험치 부여
		if(onlinenumconfirm > 1)
		{
			// 파티경험치 비율 추가 - RaMa 04.11.24 	
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
//	// 파티 경험치 처리를 한다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CPlayer* pPlayer[MAX_PARTY_LISTNUM] ;										// 파티 멤버들의 정보를 담을 포인터 선언.
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count )					// 최대 파티 멤버 수 대로 포문.
//	{
//		pPlayer[count] = NULL ;													// 멤버 포인터를 NULL 처리 한다.
//	}
//
//	DWORD PlayerID = 0 ;														// 멤버들의 아이디를 받을 변수.
//	DWORD onlinenumconfirm = 0 ;												// 경험치를 받을 멤버 수.
//
//	LEVELTYPE levelsum = 0 ;													// 파티 멤버들의 레벨 총 합.
//	LEVELTYPE maxlevel = 0 ;													// 파티 멤버 중 최대 레벨.
//	LEVELTYPE curlevel = 0 ;													// 멤버들의 레벨을 받을 변수.
//	
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)					// 로그인 중인 파티원 수와 그 중 최고 레벨을 구한다.
//	{
//		PlayerID = pParty->GetMemberID(count);									// 카운트 대로 멤버의 아이디를 받는다.
//
//		pPlayer[count] = (CPlayer*)g_pUserTable->FindUser(PlayerID);			// 멤버 플레이어 정보를 받는다.
//
//		if( pPlayer[count] )													// 플레이어 정보가 유효한지 체크.
//		{
//			if(ChkExp(pPlayer[count], pKilledPosition,GridID))					// 경험치를 받을 수 있는 범위 내에 있는지 체크.
//			{
//				++onlinenumconfirm;												// 경험치를 받을 멤버 수 증가.
//
//				curlevel = pPlayer[count]->GetLevel();							// 현제 멤버의 레벨을 받는다.
//
//				if(maxlevel < curlevel)											// 파티 내 최대 레벨보다 큰지 체크 한다.
//				{
//					maxlevel = curlevel;										// 최대 레벨을 갱신한다.
//				}
//
//				levelsum += curlevel;											// 파티 레벨을 더한다.
//			}
//		}
//	}
//
//	if( onlinenumconfirm == 0 ) return ;										// 이런 경우는 없어야 하는데, 혹시라도 생길지 모르기 때문에 예외 처리.
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 온라인 중인 멤버가 한명이면, 파티 생성 만하고 멤버가 없는 경우 이거나, 
//	// 다른 멤버들은 로그아웃한 상태이다. 또는 다른 맵에 있는 경우이다.
//	///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	if( onlinenumconfirm == 1 )													// 경험치를 받을 플레이어 수 체크.
//	{
//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(KillerID);			// 플레이어 정보를 받아 온다.
//
//		if( pPlayer )															// 플레이어 정보가 유효한지 체크.
//		{
//			DWORD dwExp	= CalcObtainExp(pMob, pPlayer->GetLevel()) ;			// 몬스터 경험치를 받는다.
//
//			DISTRIBUTERAND->SendToPersonalExp(pPlayer, dwExp) ;					// 개인에게 경험치를 전송한다.
//
//			return ;															// 리턴 처리.
//		}
//	}
//	
//	EXPTYPE partyexp = CalcObtainExp(pMob, maxlevel);							// 파티 내 최대 레벨을 기준으로 몬스터 경험치를 받아 온다.
//
//	if(partyexp == 0) return ;													// 이런 경우는 없어야 하지만, 혹시라도 생길지 모르기 때문에 예외 처리.
//
//	float applyRate = 0.0f ;													// 파티 보너스 경험치 포인트.
//
//	switch( onlinenumconfirm )													// 경험치 받을 플레이어 수 체크.
//	{
//	case 2 : applyRate = 0.4f ; break ;											// 두명이면 +40%
//	case 3 : applyRate = 0.6f ; break ;											// 세명이면 +60%
//	case 4 : applyRate = 0.7f ; break ;											// 네명이면 +70%
//	case 5 : applyRate = 0.8f ; break ;											// 다섯명이면 +80%
//	case 6 : applyRate = 0.9f ; break ;											// 여섯명이면 +90%
//	case 7 : applyRate = 1.0f ; break ;											// 일곱명이면 +100%
//	}
//
//	for(int count = 0 ; count < MAX_PARTY_LISTNUM ; ++count)					// 로그인 한 멤버들에게 분배 처리를 한다.
//	{		
//		if(pPlayer[count])														// 멤버 정보가 유효하다면.
//		{
//			if(ChkExp(pPlayer[count],pKilledPosition,GridID))					// 경험치를 받을 수 있는 영역에 있는지 체크.
//			{
//				curlevel = pPlayer[count]->GetLevel();							// 멤버의 레벨을 받는다.
//
//				float fExpA = (float)curlevel / (float)levelsum ;				// 경험치 포인트A를 구한다.
//				float fExpB = (partyexp * ( 1 + applyRate )) ;					// 경험치 포인트B를 구한다.
//				
//				float fExp = fExpA * fExpB ;									// 경험치를 구한다.
//
//				if(fExp > 0)													// 경험치 체크를 한다.
//				{
//					DWORD dwExp = (DWORD)GetFloatInt( fExp ) ;					// 경험치 소수점 이하는 버린다.
//					DISTRIBUTERAND->SendToPersonalExp(pPlayer[count],dwExp);	// 경험치를 전송한다.
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
	
//	가)	1		= 100% 데미지 : 경험치 100%
//	나)	0.8~1	= 80%~100%미만 데미지 : 경험치 80%
//	다)	0.6~0.8	= 60%~80% 미만 데미지 : 경험치 60%
//	라)	0.4~0.6			40%~60% 미만 데미지 : 경험치 40%
//	마)	0.2~0.4			20%~40% 미만 데미지 : 경험치 20%
//	바)	0~0.2			20%미만 : 경험치 없음.

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

//// 070413 LYW --- Distributer : 몬스터 경험치 패널티 적용
//DWORD CDistributer::CalcObtainExp( CMonster* pMob, LEVELTYPE KillerLevel)
//{
//	LEVELTYPE MonsterLevel = pMob->GetSMonsterList()->Level ;				// 몬스터 레벨을 받는다.
//	LEVELTYPE levelGap = 0 ;												// 몬스터와 플레이어의 레벨 차이.
//	
//	if( MonsterLevel > KillerLevel )										// 몬스터 레벨이 플레이어 보다 큰 경우.
//	{
//		levelGap = MonsterLevel - KillerLevel ;								// 레벨 차이를 구한다.
//	}
//	else																	// 플레이어 레벨이 몬스터 레벨 보다 큰 경우.
//	{
//		levelGap = KillerLevel - MonsterLevel ;								// 레벨 차이를 구한다.
//	}
//
//	if( MonsterLevel > KillerLevel + 5 )									// 몬스터 레벨이 플레이어 레벨 보다 5레벨 이상일 때.
//	{
//		double dwLevel = KillerLevel+5 ;									// 레벨 포인트 세팅.
//		double dwPenalty = 1.95 ;											// 페널티 포인트 세팅.
//		double result = pow(dwLevel, dwPenalty) ;							// 레벨 포인트 ^ 페널티 포인트를 구한다.
//
//		float fExp = (float)(result + 15) ;									// 플로트형의 크기 내에서 경험치를 구한다.
//
//		return (DWORD)GetFloatRound(fExp) ;									// 경험치를 반올림 하여 DWORD형으로 반환.
//	}
//	else if( MonsterLevel < KillerLevel )									// 플레이어 레벨이 몬스터 레벨보다 큰 경우.
//	{
//		float fPenalty = 1.0f ;												// 페널티 포인트.
//		float fExp     = 0.0f ;												// 플로트형 경험치.
//		DWORD dwExp	   = pMob->GetSMonsterList()->ExpPoint ;				// 몬스터 경험치.
//
//		if( levelGap >= 6 && levelGap <= 10 )								// 플레이어 레벨이 +6~+10일 때.
//		{
//			fPenalty = 0.5f ;												// 페널티 포인트 50%로 세팅
//		}
//		else if( levelGap >= 11 && levelGap <= 15 )							// 플레이어 레벨이 +11~+15일 때.
//		{
//			fPenalty = 0.25f ;												// 페널티 포인트 25%로 세팅.
//		}
//		else if( levelGap > 15 )											// 플레이어 레벨이 +15 이상일 때.
//		{
//			return 1 ;														// 경험치는 무조건 1로 반환한다.
//		}
//
//		fExp = (float)(dwExp * fPenalty) ;									// 경험치를 플로트형으로 구한다.
//		return (DWORD)GetFloatRound(fExp) ;									// DWORD형으로 페널티 경험치 반환.
//	}	
//
//	return pMob->GetSMonsterList()->ExpPoint ;								// 플레이어와 몬스터의 레벨이 같을경우 몬스터 경험치 반환.
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
//	//경험치가 0이면 특기 수련치도 안준다. (ex: 호박이벤트)
//	if(pMonster->GetSMonsterList()->ExpPoint)
///*어빌리티 삭제 - 		DISTRIBUTERAND->SendToPersonalAbil(pReceivePlayer, pMonster->GetLevel());*/
//	DISTRIBUTERAND->SendToPersonalItem(pReceivePlayer, DropItemID, DropItemRatio, (MONSTEREX_LIST *)pMonster->GetSMonsterList(), pMonster->GetMonsterKind());
//}

//// 070419 LYW --- Distributer : Add function to requite.
//void CDistributer::PickUpAcquisition(CPlayer* pReceivePlayer, CMonster* pMonster, DWORD DropItemID, DWORD DropItemRatio, VECTOR3* pKilledPosition, int onlinenumconfirm, WORD dwCurMap)
//{
//	if(!pReceivePlayer)																	// 받는 플레이어의 정보를 체크.
//	{		
//		Release();																		// 초기화 처리를 한다.
//		return;
//	}
//
//	WORD MonsterKind = pMonster->GetMonsterKind() ;										// 몬스터 종류를 받는다.
//
//	// 드랍 머니를 받는다.
//	MONEYTYPE money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator((MONSTEREX_LIST *)pMonster->GetSMonsterList(), pReceivePlayer, MonsterKind);
//    
//	CParty* pParty = PARTYMGR->GetParty( pReceivePlayer->GetPartyIdx() ) ;				// 파티 정보를 받는다. 
//
//	if( pParty )																		// 파티 정보가 유효한지 체크
//	{
//		// 받을 수 있는 사람을 체크 이부분은 인자로 받아 오자.
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
//		if( onlinenumconfirm == 1 )														// 받는 사람이 한명이면 개인에게 전송한다.
//		{
//			if( pReceivePlayer->SetMoney(money, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != money )
//			{
//				// error msg 보낸다. 제한량 초과
//				MSGBASE msg;
//				msg.Category = MP_ITEM;
//				msg.Protocol = MP_ITEM_MONEY_ERROR;
//				msg.dwObjectID = pReceivePlayer->GetID();
//				
//				pReceivePlayer->SendMsg(&msg, sizeof(msg));
//			}
//
//			// 개인에게 아이템을 전송한다.
//			//		for( BYTE count = 0 ;  count < 5 ; ++count )
//			//		{
//			//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//			//		}
//			return ;
//		}
//
//		MONEYTYPE partyMoney = money/onlinenumconfirm ;									// 각각의 파티원이 나누어 받을 금액을 구한다.
//
//		for(BYTE n=0;n<MAX_PARTY_LISTNUM;++n)											// 파티 멤버 수 대로 포문을 돈다.
//		{
//			PlayerID = pParty->GetMemberID(n);											// 멤버 아이디를 받는다.
//
//			pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);						// 멤버 정보를 받는다.
//
//			if( pPlayer )																// 멤버 정보가 유효한지 체크한다.
//			{
//				DWORD dwMapNum = pPlayer->GetCurMapNum() ;								// 멤버의 현재 맵 번호를 받는다.
//
//				if( wMapNum == dwCurMap )												// 멤버의 맵이 몬스터가 처리된 맵과 같은지 비교.
//				{
//					if(ChkExp(pPlayer, pKilledPosition,pMonster->GetGridID()))			// 멤버가 몬스터가 처리된 영역내에 있는지 비교.
//					{
//						// 멤버에게 획득 머니를 1/n 한 머니를 전송한다.
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
//		// 위너를 선정해서 아이템을 전송한다.
//		// 개인에게 아이템을 전송한다.
//		//		for( BYTE count = 0 ;  count < 5 ; ++count )
//		//		{
//		//			NewMonsterDropItem(MonsterKind, &pMonInfo->dropItem[count], pPlayer);
//		//		}
//	}
//	else 
//	{
//		if( pReceivePlayer->SetMoney(money, MONEY_ADDITION, 1, eItemTable_Inventory, eMoneyLog_GetMonster, MonsterKind) != money )
//		{
//			// error msg 보낸다. 제한량 초과
//			MSGBASE msg;
//			msg.Category = MP_ITEM;
//			msg.Protocol = MP_ITEM_MONEY_ERROR;
//			msg.dwObjectID = pReceivePlayer->GetID();
//			
//			pReceivePlayer->SendMsg(&msg, sizeof(msg));
//		}
//
//		// 개인에게 아이템을 전송한다.
//		// 개인에게 아이템을 전송한다.
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
//	//경험치가 0이면 특기 수련치도 안준다. (ex: 호박이벤트)
//	if(pMonInfo->ExpPoint)
///*어빌리티 삭제 - 		pParty->SendAbil(MonsterLevel, &MemberInfo, MaxLevel);*/
//	pParty->SendItem(&MemberInfo, DropItemId, DropItemRatio, pMonInfo, MonsterKind, MaxLevel);
//}


//DWORD CDistributer::GetTotalDamage()
//{
//	return m_TotalDamage;
//}
