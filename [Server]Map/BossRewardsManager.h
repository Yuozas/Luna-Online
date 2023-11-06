// BossRewardsManager.h: interface for the CBossRewardsManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOSSREWARDSMANAGER_H__101AE7AB_7992_493A_B636_1B3E323F55B0__INCLUDED_)
#define AFX_BOSSREWARDSMANAGER_H__101AE7AB_7992_493A_B636_1B3E323F55B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOSSREWARDSMGR CBossRewardsManager::GetInstance()
struct BOSSREWARDS
{
	EXPTYPE MaxExpPoint;
	DWORD MaxAbilPoint;
	MONEYTYPE MaxMoney;
};

class CBossRewardsManager  
{
	CBossRewardsManager();
	CYHHashTable<BOSSREWARDS> m_BossRewardsTable;
public:	
	GETINSTANCE(CBossRewardsManager);
	
	virtual ~CBossRewardsManager();

	void Release();
	void LoadBossRewardsInfo();
	void GetBossRewardsInfo(WORD MonsterKind, EXPTYPE &rExp, DWORD &rAbil, MONEYTYPE &rMoney);

	// 080212 LYW --- BossRewardsManager : 경험치와 돈을 리턴하는 함수 추가.
	EXPTYPE GetExpOfBossMonster(WORD MonsterKind) ;
	MONEYTYPE GetMoneyOfBossMonster(WORD MonsterKind) ;
};

#endif // !defined(AFX_BOSSREWARDSMANAGER_H__101AE7AB_7992_493A_B636_1B3E323F55B0__INCLUDED_)
