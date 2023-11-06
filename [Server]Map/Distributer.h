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
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableSolo;									// ������ �������� ���� ���̺�.
	CYHHashTable<DAMAGEOBJ>			m_DamageObjectTableParty;									// ��Ƽ�� �������� ���� ���̺�.

	DWORD	m_PlusDamage;																		// �߰� ������.
	DWORD	m_1stPlayerID;																		// ó�� �������� �� �÷��̾��� ���̵�.
	DWORD	m_1stPartyID;																		// ó�� �������� �� ��Ƽ�� ���̵�.
	DWORD	m_TotalDamage;																		// ������ �հ�.
	DWORD   m_FirstDamage ;																		// ù ������.

	int		m_nTotalKillerCount ;																// �� ������ ��(��Ƽ�� 1�� ��)

	LEVELTYPE m_HighLevel ;																		// ų�� �� �� ���� ������ ���� ų���� ����.

	void SetPlusTotalDamage(DWORD Damage);														// �հ� �������� �����ϴ� �Լ�.				

	CPlayer*	m_pKiller ;																		// ų�� ������ ��� ������.
	CParty*		m_pKillerParty ;																// ų���� ��Ƽ ������ ���.
																								 
	CMonster*	m_pMob ;																		// ���� ������ ��� ������.
	VECTOR3*	m_pKilledPosition ;																// ���͸� ���� ��ġ ������.

	DWORD		m_dwDropItemID ;																// ��� ������ ���̵� ��� ����.
	DWORD		m_dwDropItemRatio ;																// ������� ��� ����.				

	cPtrList	m_CandidacyList ;																// ��Ƽ���� ����ϴ� �ĺ��� ��Ƶδ� ����Ʈ.

public:
	CDistributer()	;																			// ������ �Լ�.
	~CDistributer()	;																			// �Ҹ��� �Լ�.

	void Release() ;																			// ���� �Լ�.

	void AddDamageObject(CPlayer* pPlayer, DWORD damage, DWORD plusdamage) ;					// ������ ���̺� ������ �߰��ϴ� �Լ�.
	void DoAddDamageObj(CYHHashTable<DAMAGEOBJ> * pTable, DWORD dwID, DWORD damage) ;			// �ش� ���̺� �������� �����ϴ� �Լ�.

	BOOL Chk(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID) ;						// �Ÿ��� ���� ����ġ�� ������ �ִ� üũ�Լ�.
	BOOL ChkExp(CPlayer* pPlayer, VECTOR3* pKilledPosition, DWORD GridID) ;						// �Ÿ��� ���� ����ġ ��⸦ �� üũ�Լ�.

	void ChooseOne(DAMAGEOBJ * pobj, DWORD* pBigDamage, DWORD* pBigID) ;						// ������ ���̺��� ū�������� ��Ƽ�� �÷��̾ ���� �Լ�.

	// �ʵ庸�� - 05.12 �̿���
	// �ʵ庸���� ������ �й� �Լ�
	void DistributeItemPerDamage(CMonster* pMob) ;
	void DistributePerDamage(CMonster* pMob) ;

	void DamageInit() ;																			// ������ ������ �ʿ��� �������� �ʱ�ȭ �ϴ� �Լ�.
	void DeleteDamagedPlayer(DWORD CharacterID) ;												// ������(����)���̺��� �־��� ���̵��� �������� ���� �Լ�.

	BOOL SetInfoToDistribute(DWORD dwKillerID, VECTOR3* pKilledPosition,						// ������ ������� �Լ����� �ʿ��� ������ �����ϴ� �Լ�.
		DWORD DropItemID, DWORD DropItemRatio,CMonster* pMob) ;			

	void Distribute() ;																			// ������ ������ ó���ϴ� �Լ�.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// ų���� �������� �� ó���ϴ� �Լ� ��Ʈ																												 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	void DistributeToKillers() ;																// ų���� �ټ��� �� ó���ϴ� �Լ�.

	void DistributerToSolo(MONEYTYPE money) ;													// ų������ ��Ƽ�� �Ҽӵ��� �ʾ����� ó���ϴ� �Լ�.

	void DistributeToFirstKiller(CPlayer* pPlayer, DWORD dwMyDamage) ;							// ��Ƽ�� ����, ù �������� �� ų���� ó���ϴ� �Լ�.
	void DistributeToOtherKiller(CPlayer* pPlayer, DWORD dwMyDamage) ;							// ��Ƽ�� ����, ���߿� �������� �� ų���� ó���ϴ� �Լ�.

	void DistributeToPartys(MONEYTYPE money) ;													// ų���� ��Ƽ�� �ҼӵǾ��� �� ó���ϴ� �Լ�.

	void DistributeToFirstParty(CParty* pParty, DWORD dwPartyDamage) ;							// ù �������� �� ��Ƽ�� ó���ϴ� �Լ�.
	void DistributeToOtherParty(CParty* pParty, DWORD dwPartyDamage) ;							// ���߿� �������� �� ��Ƽ�� ó���ϴ� �Լ�.

	void SendMoneyToPartys(float ChangeValue) ;													// ų���� ��Ƽ�� �Ӵϸ� �����ϴ� �Լ�.
	void DistributeItemToKillers() ;															// ų���� �ټ��� �� ������ �й踦 ó���ϴ� �Լ�.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// ų���� �Ѹ��� �� ó���ϴ� �Լ� ��Ʈ																													 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	void DistributeToKiller() ;																	// ų���� �Ѹ��� �� ó���ϴ� �Լ�.

	void DistributeToPerson(CPlayer* pPlayer) ;													// ų���� �Ѹ��̰�, ��Ƽ�� �Ҽӵ��� �ʾ��� �� ó���ϴ� �Լ�.
	void DistributeToParty(CPlayer* pPlayer) ;													// ų���� �Ѹ��̰�, ��Ƽ�� �ҼӵǾ��� �� ó���ϴ� �Լ�. 

	void SendMoneyToPerson(CPlayer* pPlayer, MONEYTYPE ChangeValue) ;							// ų������ �Ӵϸ� �����ϴ� �Լ�.
	void SendItemToPerson(CPlayer* pPlayer) ;													// ų������ �������� �����ϴ� �Լ�.

	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	// ���� ó���� �ϴ� ���� �� �������� ����ϴ� �Լ� ��Ʈ																									 //
	//-------------------------------------------------------------------------------------------------------------------------------------------------------//
	//EXPTYPE CalcObtainExp(LEVELTYPE KillerLevel) ;												// �־��� ������ ���� ������ ����ġ�� ��ȯ�ϴ� �Լ�.
	// 071202 LYW --- Distributer : ��Ƽ���� 2�� �̻��� �� ���� ���̸� 5�������� 8������ �÷��ִ� �۾� ó��.
	// 080530 LYW --- Distributer : Modified the return type EXPTYPE to DWORD.
	//EXPTYPE CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount) ;							// �־��� ������ ���� ������ ����ġ�� ��ȯ�ϴ� �Լ�.
	DWORD CalcObtainExp(LEVELTYPE KillerLevel, int nMemberCount) ;								// �־��� ������ ���� ������ ����ġ�� ��ȯ�ϴ� �Լ�.

	BOOL GetFirstDamange() ;																	// ��Ƽ��, �����̴�, ù �������� ���ϴ� �Լ�.

	void GetTotalKillerCount() ;																// ��Ƽ�� 1���ϰ� �� ų������ ���ϴ� �Լ�.
	void GetHighLevelOfKillers() ;																// ų�� �� �� �ְ� ������ �̴´�.
	int  GetLevelPenaltyToMonster(LEVELTYPE level) ;											// �Է� �� ������ ������ ���� ���̸� ��ȯ�ϴ� �Լ�.

	void GetPartyLevels(CParty* pParty, LEVELTYPE* pHighLevel, LEVELTYPE* pTotalLevel) ;		// ��Ƽ�� �ְ�����, �� ���� ���� ���ϴ� �Լ�.

	void SaveCandidacy(CParty* pParty) ;														// ��Ƽ�� �� �ĺ��� ��� �Լ�.

	void DistribuSequence(CParty* pParty) ;														// ���������� �������� �й��ϴ� �Լ�.
	void DistributeDamage(CParty* pParty) ;														// �������� ���� ��Ƽ������ �������� �й��ϴ� �Լ�.
	void DistributeRandom(CParty* pParty) ;														// �����ϰ� ��Ƽ���� �����ؼ� �������� �����ϴ� �Լ�.

	// 080122 LYW --- Distributer : Player �� Monster�� ���� ä�ο� �ִ��� ��ȯ�ϴ� �Լ� �߰�.
	BOOL IsSameChannel(CPlayer* pPlayer, CMonster* pMonster) ;
};

#endif // !defined(AFX_DISTRIBUTER_H__925C29CC_0A77_451A_8755_6CB51833B600__INCLUDED_)
