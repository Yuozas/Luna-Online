#pragma once


#define CHARCALCMGR CCharacterCalcManager::GetInstance()


class CPlayer;
class CPet;

struct SetScript;
struct ITEM_INFO;


class CCharacterCalcManager  
{
	cPtrList	m_HPMPDeterminateList ;

public:
	static CCharacterCalcManager* GetInstance();
	CCharacterCalcManager();
	virtual ~CCharacterCalcManager();

	void CalcMaxLife( CPlayer* );
	void CalcMaxMana( CPlayer* );

	BOOL LoadHPMPDeterminateList();

	void Init_HPMP_Point(CPlayer*);
	BOOL Get_HPMP_Point(CPlayer*, BYTE* hpPoint, BYTE* mpPoint);

	cPtrList* GetHPMPDetermiateList() { return &m_HPMPDeterminateList ; }

	void CalcCharPhyAttackMax( CPlayer* );
	void CalcCharPhyAttackMin( CPlayer* );
	void CalcCharPhyDefense( CPlayer* );

	void CalcCharMagAttack( CPlayer* );
	void CalcCharMagDefense( CPlayer* );

	void CalcCharAccuracy( CPlayer* );

	void CalcCharEvasion( CPlayer* );

	void CalcCharCriticalRate( CPlayer* );
	void CalcCharCriticalDamage( CPlayer* );
	// 080910 LUJ, 플레이어의 방패 방어력을 계산
	void CalcCharShieldDefense( CPlayer* );

	static void ProcessLife(CPlayer*);
	static void ProcessMana(CPlayer*);
	
	static void ProcessPetLife(CPet*);
	static void ProcessPetMana(CPet*);

	static void ProcessLifeBoss(CBossMonster * pBoss);

	static void General_LifeCount(CPlayer*, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	static void General_ManaCount(CPlayer*, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	
	static void General_PetLifeCount(CPet*, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	static void General_PetManaCount(CPet*, DWORD curTime, LEVELTYPE level, DWORD life, DWORD maxlife);
	
	static void General_LifeCountBoss(CBossMonster * pBoss, DWORD curTime, DWORD life, DWORD maxlife);

	static void	Ungi_LifeCount(CPlayer*, DWORD life, DWORD maxlife, DWORD curTime);
	static void	Ungi_ManaCount(CPlayer*, DWORD curTime, DWORD energy, DWORD maxEnergy);
	static void KyungGong_ManaCount(CPlayer*, DWORD curTime,WORD KyungGongIdx);

	static void UpdateLife( CObject* );
	static void StartUpdateLife( CObject*, DWORD plusLife, BYTE recoverInterval=RECOVERINTERVALUNIT, DWORD recoverTime=RECOVERTIMEUNIT);

	static void UpdateMana(CPlayer*);
	static void StartUpdateMana(CPlayer*, DWORD plusMana, BYTE recoverInterval=RECOVERINTERVALUNIT, DWORD recoverTime=RECOVERTIMEUNIT);


	// 기존 StatsCalcManager의 기능을 이전. 기존에 Stats..클래스는 싱글턴인데 이 클래스를 멤버로 갖고 있었다. 결국 이 클래스가 따로 있을 이유는 없다...
public:
	void Initialize		( CPlayer* );
	void AddItem		( CPlayer*, const ITEMBASE& );
	void RemoveItem		( CPlayer*, const ITEMBASE& );
	void CalcCharStats	( CPlayer* );

private:
	void AddStat	( const ITEM_INFO&,		PlayerStat& );

	// 080319 LUJ, 추가 인챈트 수치를 부여함
	void AddStat	( const ITEM_INFO&,		const ITEM_OPTION&,	PlayerStat& );
	void AddStat	( const PlayerStat&,	PlayerStat& );
	void RemoveStat	( const ITEM_INFO&,		PlayerStat& );

	// 080319 LUJ, 추가 인챈트 수치를 제거함
	void RemoveStat	( const ITEM_INFO&,		const ITEM_OPTION&,	PlayerStat& );
	void RemoveStat	( const PlayerStat&,	PlayerStat& );

	void AddSetItemStats	( CPlayer*, const SetScript&, int setItemSize );
	void RemoveSetItemStats	( CPlayer*, const SetScript&, int setItemSize );
};
