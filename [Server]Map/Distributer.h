// Distributer.h: interface for the CDistributer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_)
#define AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_POINTACCEPTOBJECT_NUM 2

struct DAMAGEOBJ
{
	DWORD dwID;
	DWORD dwData;
};

class CParty;
class CDistributeWay;
class MONSTEREX_LIST;

class CDistributer  //responsible for decision who will get compensation when monster died
{
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableSolo;									// 개인의 데미지를 담은 테이블.
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableParty;									// 파티의 데미지를 담은 테이블.

	DWORD	m_PlusDamage;																		// 추가 데미지.
	DWORD	m_1stPlayerID;																		// 처음 데미지를 준 플레이어의 아이디.
	DWORD	m_1stPartyID;																		// 처음 데미지를 준 파티의 아이디.
	DWORD	m_TotalDamage;																		// 데미지 합계.
	DWORD   m_FirstDamage ;																		// 첫 데미지.

	int		m_nTotalKillerCount ;																// 총 공격자 수(파티는 1로 함)

	LEVELTYPE m_HighLevel ;																		// 킬러 들 중 가장 레벨이 높은 킬러의 레벨.

	void SetPlusTotalDamage(DWORD Damage);														// 합계 데미지를 설정하는 함수.				

	CPlayer*	m_pKiller ;																		// 킬러 정보를 담는 포인터.
	CParty*		m_pKillerParty ;																// 킬러의 파티 정보를 담는.
																								 
	CMonster*	m_pMob ;																		// 몬스터 정보를 담는 포인터.
	VECTOR3*	m_pKilledPosition ;																// 몬스터를 잡은 위치 포인터.

	DWORD		m_dwDropItemID ;																// 드랍 아이템 아이디를 담는 변수.
	DWORD		m_dwDropItemRatio ;																// 드랍률을 담는 변수.				

	cPtrList	m_CandidacyList ;																// 파티에서 사용하는 후보를 담아두는 리스트.

public:
	CDistributer()	;																			// 생성자 함수.
	~CDistributer()	;																			// 소멸자 함수.

	void Release() ;																			// 해제 함수.

	void AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage) ;					// 데미지 테이블에 데미지 추가하는 함수.
	void DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage) ;			// 해당 테이블에 데미지를 추하하는 함수.

	BOOL Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID) ;						// 거리에 따른 경험치를 얻을수 있는 체크함수.
	BOOL ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID) ;						// 거리에 따른 경험치 얻기를 뺀 체크함수.

	void ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID) ;						// 데미지 테이블에서 큰데미지의 파티나 플레이어를 고르는 함수.

	// 필드보스 - 05.12 이영준
	// 필드보스용 아이템 분배 함수
	void DistributeItemPerDamage(CMonster* pMob) ;
	void DistributePerDamage(CMonster* pMob) ;

	void DamageInit() ;																			// 데미지 관리에 필요한 변수를을 초기화 하는 함수.
	void DeleteDamagedPlayer(DWORD CharacterID) ;												// 데미지(개인)테이블에서 주어진 아이디의 데미지를 빼는 함수.

	BOOL SetInfoToDistribute(DWORD dwKillerID, VECTOR3* pKilledPosition,						// 몬스터의 보상관련 함수들이 필요한 정보를 세팅하는 함수.
		DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob) ;			

	void Distribute() ;																			// 몬스터의 보상을 처리하는 함수.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// 킬러가 여러명일 때 처리하는 함수 파트																												 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	void DistributeToKillers() ;																// 킬러가 다수일 때 처리하는 함수.

	void DistributerToSolo(MONEYTYPE money) ;													// 킬러가가 파티에 소속되지 않았을때 처리하는 함수.

	void DistributeToFirstKiller(CPlayer* pPlayer, DWORD dwMyDamage) ;							// 파티가 없고, 첫 데미지를 준 킬러를 처리하는 함수.
	void DistributeToOtherKiller(CPlayer* pPlayer, DWORD dwMyDamage) ;							// 파티가 없고, 나중에 데미지를 준 킬러를 처리하는 함수.

	void DistributeToPartys(MONEYTYPE money) ;													// 킬러가 파티에 소속되었을 때 처리하는 함수.

	void DistributeToFirstParty(CParty* pParty, DWORD dwPartyDamage) ;							// 첫 데미지를 준 파티를 처리하는 함수.
	void DistributeToOtherParty(CParty* pParty, DWORD dwPartyDamage) ;							// 나중에 데미지를 준 파티를 처리하는 함수.

	void SendMoneyToPartys(float ChangeValue) ;													// 킬러의 파티로 머니를 전송하는 함수.
	void DistributeItemToKillers() ;															// 킬러가 다수일 때 아이템 분배를 처리하는 함수.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// 킬러가 한명일 때 처리하는 함수 파트																													 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	void DistributeToKiller() ;																	// 킬러가 한명일 때 처리하는 함수.

	void DistributeToPerson(CPlayer* pPlayer) ;													// 킬러가 한명이고, 파티에 소속되지 않았을 때 처리하는 함수.
	void DistributeToParty(CPlayer* pPlayer) ;													// 킬러가 한명이고, 파티에 소속되었을 때 처리하는 함수. 

	void SendMoneyToPerson(CPlayer* pPlayer, MONEYTYPE ChangeValue) ;							// 킬러에게 머니를 전송하는 함수.
	void SendItemToPerson(CPlayer* pPlayer) ;													// 킬러에게 아이템을 전송하는 함수.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// 보상 처리를 하는 과정 중 공통으로 사용하는 함수 파트																									 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//EXPTYPE CalcObtainExp(LEVELTYPE KillerLevel) ;												// 주어진 레벨에 따른 몬스터의 경험치를 반환하는 함수.
	// 071202 LYW --- Distributer : 파티원이 2인 이상일 때 레벨 차이를 5레벨에서 8레벨로 늘려주는 작업 처리.
	// 080530 LYW --- Distributer : Modified the return type EXPTYPE to DWORD.
	//EXPTYPE CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount) ;							// 주어진 레벨에 따른 몬스터의 경험치를 반환하는 함수.
	DWORD CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount) ;								// 주어진 레벨에 따른 몬스터의 경험치를 반환하는 함수.

	BOOL GetFirstDamange() ;																	// 파티던, 개인이던, 첫 데미지를 구하는 함수.

	void GetTotalKillerCount() ;																// 파티는 1로하고 총 킬러수를 구하는 함수.
	void GetHighLevelOfKillers() ;																// 킬러 들 중 최고 레벨을 뽑는다.
	int  GetLevelPenaltyToMonster(LEVELTYPE level) ;											// 입력 된 레벨과 몬스터의 레벨 차이를 반환하는 함수.

	void GetPartyLevels(CParty* pParty, LEVELTYPE* pHighLevel, LEVELTYPE* pTotalLevel) ;		// 파티의 최고레벨과, 총 레벨 합을 구하는 함수.

	void SaveCandidacy(CParty* pParty) ;														// 파티원 중 후보를 담는 함수.

	void DistribuSequence(CParty* pParty) ;														// 순차적으로 아이템을 분배하는 함수.
	void DistributeDamage(CParty* pParty) ;														// 데미지가 높은 파티원에게 아이템을 분배하는 함수.
	void DistributeRandom(CParty* pParty) ;														// 랜덤하게 파티원을 선택해서 아이템을 분해하는 함수.

	// 080122 LYW --- Distributer : Player 와 Monster가 같은 채널에 있는지 반환하는 함수 추가.
	BOOL IsSameChannel(CPlayer* pPlayer, CMonster* pMonster) ;
};

#endif // !defined(AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_)
