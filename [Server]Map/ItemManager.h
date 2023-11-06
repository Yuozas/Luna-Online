#pragma once

// < 함수를 구현 할 때 반드시 지켜야 할 것 >
// 1. 함수에서 에러처리는 완벽히 처리 할 것
// 2. 에러처리 할때 (원래 작업으로 되돌릴 것 RollBack!!!)

//#define MAX_MIX_PERCENT	10000

#define EVENTITEM_ALL_RECOVER 53032
#define ITEMMGR CItemManager::GetInstance()

class CItemSlot;
class CBackupSlot;

struct DealerItem
{
	BYTE Tab;
	BYTE Pos;
	DWORD ItemIdx;
};
struct DealerData
{

	//	POS pos;
	//public:
	//	void SetPositionHead(){	pos = m_DealItemList.GetFirstPos();	}
	//	DealerItem* GetNextItem(){	return m_DealItemList.GetNextPos(pos);	}
	BOOL FindItem(DWORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx)
				return TRUE;
		}

		return FALSE;
	}

	cLinkedList<DealerItem> m_DealItemList;
};


// 조합 아이템 기본 자료구조
struct ItemMixResult
{
	DWORD	mItemIndex;
	WORD	mRequiredLevel;
	DWORD	mMoney;
	float	mSuccessRate;

	// 키: 아이템 인덱스, 값: 소요 개수
	typedef stdext::hash_map< DWORD, DWORD >	Material;
	Material									mMaterial;
};


typedef std::list< ItemMixResult >	MixScript;


// 세트 아이템 기본 자료 구조
struct SetScript
{
	std::string mName;

	// 세트로 구성되는 아이템 
	typedef stdext::hash_set< DWORD >	Item;
	Item								mItem;

	struct Element
	{
		PlayerStat	mStat;

		// 키: 스킬 인덱스, 값: 레벨 증감
		typedef std::map< DWORD, BYTE >	Skill;
		Skill							mSkill;
	};

	// 키: 능력이 발현되는 세트 아이템 구성 개수
	typedef stdext::hash_map< int, Element >	Ability;
	Ability										mAbility;
};


struct DissolveScript
{
	typedef struct
	{
		DWORD mItemIndex;
		DWORD mQuantity;
	}
	Result;

	// 분해시 무조건 출현하는 아이템
	typedef std::list< Result > StaticResult;
	StaticResult				mStaticResult;

	// 분해시 확률에 따라 출현하는 아이템. lower_bound()를 쓰므로 hash_map을 쓰면 안된다.
	typedef std::map< DWORD, Result >	DynamicResult;
	DynamicResult						mDynamicResult;

	// 분해가능한 최소 레벨
	DWORD mLevel;
};


struct ApplyOptionScript
{
	struct
	{
		int	mMin;
		int mMax;
	}
	mLevel;

	// 키: 확률, 값: 부여될 옵션 개수
	typedef std::map< float, DWORD >	OptionSizeMap;
	OptionSizeMap						mOptionSizeMap;

	// 적용될 아이템 종류
	typedef stdext::hash_set< LONGLONG >	TargetTypeSet;
	TargetTypeSet							mTargetTypeSet;

	struct Option
	{
		ITEM_OPTION::Drop::Key	mKey;

		float	mBeginValue;
		float	mEndValue;
	};
	
	// 확률마다 선택될 옵션 종류와 수치 범위를 담고 있다
	// 키: 선택 확률
	typedef std::map< float, Option >	OptionTypeMap;
	OptionTypeMap						mOptionTypeMap;
};


struct ReinforceScript
{
	enum eType
	{
		eTypeNone,
		eTypeStrength,
		eTypeDexterity,
		eTypeVitality,
		eTypeIntelligence,
		eTypeWisdom,
		eTypeLife,
		eTypeMana,
		eTypeLifeRecovery,
		eTypeManaRecovery,
		eTypePhysicAttack,
		eTypePhysicDefence,
		eTypeMagicAttack,
		eTypeMagicDefence,
		eTypeMoveSpeed,
		eTypeEvade,
		eTypeAccuracy,
		eTypeCriticalRate,
		eTypeCriticalDamage,
		eTypeMax
	};

	eType	mType;		// 강화로 상승되는 능력 종류
	DWORD	mMax;		// 강화로 올릴 수 있는 최대 수치
	float	mBias;		// 강화를 위한 값
	BOOL	mForRare;	// 레어 아이템 강화 가능 여부

	// 해당 재료로 강화할 수 있는 아이템 종류(장착 슬롯으로 구분)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};


// 070810 웅주, 인챈트 스크립트
struct EnchantScript
{
	// 인챈트를 부여하는 아이템(인챈트 스크롤 등)이 가지는 인덱스
	DWORD mItemIndex;

	enum eType
	{
		eTypeNone,
		eTypeStrength,
		eTypeDexterity,
		eTypeVitality,
		eTypeIntelligence,
		eTypeWisdom,
		eTypeLife,
		eTypeMana,
		eTypeLifeRecovery,
		eTypeManaRecovery,
		eTypePhysicAttack,
		eTypePhysicDefence,
		eTypeMagicAttack,
		eTypeMagicDefence,
		eTypeMoveSpeed,
		eTypeEvade,
		eTypeAccuracy,
		eTypeCriticalRate,
		eTypeCriticalDamage,
		eTypeMax
	};

	// 인챈 가능한 아이템 착용 제한 레벨 범위
	struct
	{
		DWORD mMin;
		DWORD mMax;
	}
	mItemLevel;

	DWORD mEnchantMaxLevel;	// 최대로 인챈트할 수 있는 단계

	typedef stdext::hash_set< eType >	Ability;
	Ability								mAbility;

	// 해당 재료로 인챈트할 수 있는 아이템 종류(장착 슬롯으로 구분)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};

// 071028 LYW --- ItemManager : Add enum code for error of discard item from quest group.
enum QG_DISCARD_ERR															// 퀘스트 그룹에서 아이템 버리기를 할 때 발생하는 에러 정의.
{
	err_No_Error = 10,														// 에러 없음.
	err_Invalid_CItemSlot,													// 아이템 슬롯 포인터 에러.
	err_Invalid_ITEMBASE,													// 아이템 기본 포인터 에러.
	err_Not_Same_ItemIdx,													// 아이템 인덱스 일치 하지 않음.
	err_Not_StackItem,														// 스택 아이템이 아닐경우 에러.
	err_Not_NormalItem,														// 일반 아이템이 아닐경우 에러.
	err_Over_ItemCount,														// 최대 스택 카운트 오버 에러.
	err_Invalid_RemainCount,												// 남은 아이템 수 에러.
	err_Failed_UpdateItem,													// 아이템 업데이트 에러.
	err_Failed_DeleteItem,													// 아이템 삭제 에러.
} ;


class	CMHFile;
class	ITEM_REINFORCE_INFO;

struct	ITEM_INFO;
struct	MSG_ITEM_DISSOLVE_ACK;


class CItemManager  
{
	// desc_hseos_체인지아이템01
	// S 체인지아이템 추가 added by hseos 2007.07.29
	enum CHANGEITEM_RESULT_KIND										// 체인지 아이템 결과 종류
	{
		TRK_ITEM = 0,												// ..아이템
		TRK_MONEY,													// ..돈
	};

	struct stCHANGEITEM_RESULT_BASE
	{
		CHANGEITEM_RESULT_KIND	eResultKind;						// ..결과 종류
		DWORD					nValue01;							// ..수치1
		DWORD					nValue02;							// ..수치2
		DWORD					nValue03;							// ..수치3
		DWORD					nProbaValue;						// ..확률
	};

	struct stCHANGEITEM_RESULT
	{
		int						  nResultBaseNum;
		stCHANGEITEM_RESULT_BASE* stResultBase;
	};

	struct stCHANGEITEM												// 체인지 아이템 구조
	{
		int						nChangeItemID;						// ..체인지 아이템 ID
		int						nResultNum;							// ..결과 개수
		stCHANGEITEM_RESULT*	stResult;							// ..결과
	};
	// E 체인지아이템 추가 added by hseos 2007.07.29

	CYHHashTable<DealerData>	m_DealerTable;
	DWORD m_Key; //조합, 강화, 등급업시 로그 남길 때 같은 조합이었다는 것을 표시하기 위해.

	DWORD MakeNewKey();

	// desc_hseos_체인지아이템01
	// S 체인지아이템 추가 added by hseos 2007.07.29
	stCHANGEITEM*			m_stChangeItemInfo;
	int						m_nChangeItemInfoNum;
	// E 체인지아이템 추가 added by hseos 2007.07.29

public:
	CItemManager();
	virtual ~CItemManager();
	//GETINSTANCE(CItemManager);

	static CItemManager* GetInstance();

	void LoadDealerItem();
	DealerData * GetDealer(WORD wDealerIdx);
	int DivideItem( CPlayer*, DWORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur);
	int CombineItem( CPlayer*, DWORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur);
	int MoveItem( CPlayer*, DWORD FromItemIdx, POSTYPE FromPos, DWORD ToItemIdx, POSTYPE& ToPos);
	int MovePetItem( CPlayer*, DWORD FromItemIdx, POSTYPE FromPos, DWORD ToItemIdx, POSTYPE& ToPos);
	int DiscardItem(CPlayer*, POSTYPE whatPos, DWORD whatItemIdx, DURTYPE whatItemNum);

	// 071027 LYW --- ItemManager : Add function to discard stack item from quest group.
	int QG_Discard_StackItem(CPlayer* pPlayer, POSTYPE whatPos, DWORD whatItemIdx, int* nGiveNum) ;

	// 071027 LYW --- ItemManager : Add function to discard normal item from quest group.
	int QG_Discard_NormalItem(CPlayer* pPlayer, POSTYPE whatPos, DWORD whatItemIdx, int* nGiveNum) ;

	void SendErrorMsg( CPlayer*, MSG_ITEM_ERROR * msg, int msgSize, int ECode);
	void SendAckMsg( CPlayer*, MSGBASE * msg, int msgSize);
	void SendGuildErrorMsg( CPlayer*, BYTE Protocol, int ECode );

	//	int ObtainItem(CPlayer*, ITEMOBTAINARRAYINFO * pArrayInf, WORD whatItemIdx, WORD whatItemNum);
	int ObtainItemEx(CPlayer*, ITEMOBTAINARRAYINFO * pArrayInfo, DWORD whatItemIdx, WORD whatItemNum, WORD* EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD wSeal=0);
	
	int SellItem( CPlayer*, POSTYPE whatPos, DWORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx );
	int BuyItem( CPlayer*, DWORD buyItemIdx, WORD buyItemNum, WORD DealerIdx );

	// 필드보스 - 05.12 이영준
	// 몬스터가 동일 아이템을 여러게 드랍할수 있도록 변경
	void MonsterObtainItem(CPlayer*, DWORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum = 1);

	int CheatObtainItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum );
	//SW051011 Cheat Rare
	//int CheatObtainRareItem(CPlayer*, DWORD obtainItemIdx, WORD ItemNum, ITEM_OPTION* pOptionInfo);

	// desc_hseos_몬스터미터01
	// S 몬스터미터 추가 added by hseos 2007.07.08
	int ObtainMonstermeterItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum );
	// E 몬스터미터 추가 added by hseos 2007.07.08

	// desc_hseos_결혼_01
	// S 결혼 추가 added by hseos 2007.12.13
	int ObtainGeneralItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum, WORD nObtainKind, BYTE nProtocal );
	// E 결혼 추가 added by hseos 2007.12.13

	static void ObtainItemDBResult(CPlayer*, WORD ArrayID);
	static void BuyItemDBResult(CPlayer*, WORD ArrayID );	
	static void DivideItemDBResult(CPlayer*, WORD ArrayID);
	//SW050920 Rare
	//static void RareItemDBResult( CPlayer*, WORD ArrayID, ITEM_RARE_OPTION_INFO* );

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* );

	void LoadItemList();
	void ReloadItemList();

	void SetItemInfo(DWORD ItemIdx,ITEM_INFO*, CMHFile* );
	ITEM_INFO * GetItemInfo( DWORD wItemIdx );


	int UseItem( CPlayer*, WORD TargetPos, DWORD wItemIdx );

	// 20071122
	BOOL UseSealingItem( CPlayer*, ITEMBASE*, const ITEM_INFO& );
	BOOL isSealItem(const ITEMBASE* pItemBase);

	// desc_hseos_농장시스템_01
	// S 농장시스템 추가 added by hseos 2007.09.11
	ITEM_INFO* GetUseItemInfo(CPlayer*, WORD TargetPos, DWORD wItemIdx);
	// E 농장시스템 추가 added by hseos 2007.09.11

	//void ReinforceItemDBResult( CPlayer*, POSTYPE TargetPos, ITEM_OPTION* );

	//SW050920 Rare
	//	void RareItemDBResult(CPlayer*, DWORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION* pRareInfo);
	//	void RareItemDBResult(CPlayer*, WORD ArrayID, ITEM_OPTION* pRareOptionInfo);

	ITEMOBTAINARRAYINFO *	Alloc(CPlayer*, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx = NULL);
	void Free(CPlayer*, ITEMOBTAINARRAYINFO * info);
	BOOL IsDupItem( DWORD wItemIdx );

	// 071204 LUJ
	BOOL IsTwoHand( DWORD wItemIdx );

	//SW050920 Rare
	BOOL IsRare( const ITEM_INFO& ) const;
	BOOL IsPetSummonItem( DWORD wItemIdx );

	BOOL ItemMoveUpdateToDBbyTable(CPlayer*, DWORD dwfromDBIdx, DWORD dwfromIconIdx,  POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
	void ItemUpdatebyTable(CPlayer*, DWORD dwDBIdx, DWORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition);
	const ITEMBASE * GetItemInfoAbsIn(CPlayer*, POSTYPE Pos);

	void ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer*, ITEMBASE* pItem);

	// 071211 KTH -- CPlayer 추가
	unsigned int GetTotalEmptySlotNum(CItemSlot*, CPlayer*); //060612 Add by wonju

	// desc_hseos_체인지아이템01
	// S 체인지아이템 추가 added by hseos 2007.07.29	2007.08.03
	BOOL LoadScriptFileDataChangeItem();
	BOOL ProcChangeItem(CPlayer* pPlayer, DWORD nItemID);
	BOOL IsChangeItem(DWORD nItemID);
	// E 체인지아이템 추가 added by hseos 2007.07.29	2007.08.03

	WORD CheckExtraSlot(CPlayer*, CItemSlot*, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum);
	WORD GetCanBuyNumInSpace(CPlayer*, CItemSlot*, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum);
private:
	
	WORD GetCanBuyNumInMoney(CPlayer*, WORD butNum, MONEYTYPE Price);
	WORD GetCanBuyNumInFishingPoint(CPlayer*, WORD buyNum, MONEYTYPE Point);
	BOOL CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer*);
	BOOL CanbeMoved(DWORD wIconIdx,POSTYPE pos, CPlayer*);
	BOOL CanEquip(ITEM_INFO*, CPlayer*);
	void ClearQuickPos(ITEMBASE*, POSTYPE ToPos);

	CYHHashTable<ITEM_INFO> m_ItemInfoList;
	CMemoryPoolTempl<ITEMOBTAINARRAYINFO> m_ItemArrayPool;
	CIndexGenerator		m_ArrayIndexCreator;
	//
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;

public:
	void	Process();

	// jsd quest
	int		ObtainItemFromQuest( CPlayer*, DWORD wItemKind, DWORD dwItemNum );
	WORD	GetWeaponKind( DWORD wWeapon );
	void	GetItemKindType( DWORD wItemIdx, WORD* wKind, WORD* wType );

	int		ObtainItemFromChangeItem( CPlayer*, DWORD wItemKind, WORD wItemNum );
	BOOL	CheckHackNpc( CPlayer*, WORD wNpcIdx, WORD wParam=0 );	
	// RaMa	
	int UseShopItem(CPlayer*, SHOPITEMUSEBASE pInfo, SHOPITEMBASE* pShopItem);
	BOOL IsUseAbleShopItem( CPlayer*, DWORD ItemIdx, POSTYPE ItemPos );
	void ShopItemUseUpgrade(ITEM_INFO* pShopItemInfo, CPlayer*, POSTYPE ItemPos, DWORD wItemIdx);

	BOOL LoadAvatarEquipList();
	BOOL ItemUnsealing(CPlayer*, POSTYPE absPos);

	int CheatObtainShopItem( CPlayer*, DWORD obtainItemIdx, DWORD ItemNum );

	BOOL	CheckHackItemMove( CPlayer*, CItemSlot* pFromSlot, CItemSlot* pToSlot );

	// desc_hseos_아이템 유효성체크01
	// S 아이템 유효성체크 추가 added by hseos 2007.05.31
	BOOL	IsValidItem(DWORD nItemID)		{ if (m_ItemInfoList.GetData(nItemID)) return TRUE ; return FALSE; }
	// E 아이템 유효성체크 추가 added by hseos 2007.05.31


	// 070613 웅주, 세트 아이템 관리
public:
	// 적용된 세트 스크립트를 저장한다
	void AddSetScript( const SetScript*, const SetScript::Element* );

	// 저장된 세트 스크립트를 지운다
	void RemoveSetScript( const SetScript* );

	const SetScript* GetSetScript( DWORD itemIndex ) const;

	// 해당 세트 스크립트에 해당하는 아이템이 플레이어가 몇개나 있는지 반환한다.
	int GetSetItemSize( CPlayer*, const SetScript* );

private:
	void LoadSetScript();

	typedef stdext::hash_map< DWORD, std::string > SetItemName;
	void ParseSetScript( std::list< std::string >&, SetScript&, const SetItemName& );

	// 0706 웅주, 세트 아이템 정보를 보관
	std::list< SetScript >	mSetScript;

	// 아이템 값을 받아 가지고 있는 세트 정보를 반환
	// 키: 아이템 인덱스, 값: 세트 아이템 정보
	typedef stdext::hash_map< DWORD, const SetScript* >	SetScriptMap;
	SetScriptMap										mSetScriptMap;

	// 070703 웅주, 조합 아이템 관리
private:
	//typedef std::map< POSTYPE, ITEMBASE >	MIrrorSlot;

	eLogitemmoney		Mix( CPlayer*, const MSG_ITEM_MIX_SYN2&, const CBackupSlot* );
	const MixScript*	GetMixScript( DWORD itemIndex ) const;
	void AddMixOption( CPlayer&, const ITEMBASE& );

private:
	void LoadMixScript();
	void LoadMixSetting();

	// 키: 아이템 인덱스
	typedef stdext::hash_map< DWORD, MixScript >	MixScriptMap;
	MixScriptMap									mMixScriptMap;

	struct MixSetting
	{
		enum eAbility
		{
			eAbilityNone,
			eAbilityStrength,
			eAbilityDexterity,
			eAbilityIntelligence,
			eAbilityVitality,
			eAbilityWisdom,
		};

		enum eEquippment
		{
			eEquippmentNone,
			eEquippmentSword,
			eEquippmentAxe,
			eEquippmentMace,
			eEquippmentDagger,
			eEquippmentBow,
			eEquippmentStaff,
			eEquippmentRobe,
			eEquippmentLightArmor,
			eEquippmentHeavyArmor,
		};

		// 각 구간 별 범위값이 들어있다. 힘 20%, 민첩이 80%라 하자. 이때 100까지의 랜덤값을 얻어 map의 lower_bound() 메소드를 이용하여
		// 적용할 능력치를 얻어낸다.
		typedef std::map< DWORD, eAbility >			Ability;
		typedef std::map< eEquippment, Ability >	Equippment;
		Equippment									mEquippment;

		typedef struct Range
		{
			DWORD mMin;
			DWORD mMax;
		};

		// 키: 레벨 값. 값: 적용할 수치 범위
		typedef std::map< DWORD, Range >	RangeMap;
		RangeMap							mRangeMap;
	}
	mMixSetting;

	// 080228 LUJ, 조합 보호 아이템 여부
	BOOL IsMixProtectionItem( const ITEMBASE& ) const;

	// 080228 LUJ, 조합 보호 아이템 관리
	typedef stdext::hash_set< DWORD >	MixProtectionItemSet;
	MixProtectionItemSet				mMixProtectionItemSet;


	// 070710 웅주, 분해 아이템 관리
public:
	eLogitemmoney			Dissolve( CPlayer*, const MSG_ITEM_DISSOLVE_SYN2&, const CBackupSlot* );
	const DissolveScript*	GetDissolveScript( DWORD itemIndex ) const;

private:
	void LoadDissolveScript();

	// 키: 아이템 인덱스
	typedef std::map< DWORD, DissolveScript >	DissolveScriptMap;
	DissolveScriptMap							mDissolveScriptMap;


	// 070710 웅주, 강화 아이템 관리
public:
	eLogitemmoney	Reinforce( CPlayer*, const MSG_ITEM_REINFORCE_SYN&, const CBackupSlot* );

	const ReinforceScript*	GetReinforceScript( DWORD itemIndex ) const;

private:
	void LoadReinforceScript();

	// 키: 아이템 인덱스
	typedef stdext::hash_map< DWORD, ReinforceScript >	ReinforceScriptMap;
	ReinforceScriptMap									mReinforceScriptMap;

	// 080228 LUJ, 강화 보호 아이템 여부
	BOOL IsReinforceProtectionItem( const ITEMBASE& ) const;

	// 080228 LUJ, 강화 보호 아이템 관리
	typedef stdext::hash_set< DWORD >	ReinforceProtectionItemSet;
	ReinforceProtectionItemSet			mReinforceProtectionItemSet;


	// 070810 웅주, 인챈트 아이템 관리
public:
	MP_PROTOCOL_ITEM		Enchant( CPlayer*, const MSG_ITEM_ENCHANT_SYN*, eLogitemmoney& );
	const EnchantScript*	GetEnchantScript( DWORD itemIndex ) const;
	const EnchantScript*	GetEnchantScript( eEquipSlot ) const;

private:
	void LoadEnchantScript();
	void AddEnchantValue( ITEM_OPTION&, const EnchantScript::Ability&, int value ) const;

	typedef stdext::hash_map< DWORD, EnchantScript >	EnchantScriptMap;
	EnchantScriptMap									mEnchantScriptMap;

	typedef stdext::hash_map< eEquipSlot, const EnchantScript* >	SlotToEnchantScript;
	SlotToEnchantScript												mSlotToEnchantScript;

	struct EnchantProtection
	{
		// 080320 LUJ, 보호 정보
		struct Data
		{
			// 080328 LUJ, 범위 구조체 추가
			struct Range
			{
				DWORD mMin;
				DWORD mMax;
			};

			// 080320 LUJ, 보호되는 인챈트 레벨 범위
			Range mEnchantLevelRange;

			// 080328 LUJ, 보호 가능한 아이템 장착 레벨
			Range mItemLimitLevelRange;

			// 080328 LUJ, 인챈트 마이너스 보너스 적용 시에도 보호되는 하한 인챈트 레벨
			DWORD mMinimumEnchantLevel;

			// 080328 LUJ, 인챈트 확률에 가감되는 값
			int mAddedPercent;

			// 080328 LUJ, 인챈트 성공/실패 시 인챈트 레벨이 얼마나 상승/하락할 지 결정하는 확률 맵. 키: 확률, 값: 추가될 인챈트 레벨
			typedef std::map< float, int > BonusMap;

			// 080328 LUJ, 인챈트 성공 시 사용하는 플러스 보너스 맵
			BonusMap mPlusBonusMap;

			// 080328 LUJ, 인챈트 실패 시 사용하는 마이너스 보너스 맵
			BonusMap mMinusBonusMap;			
		};

		// 080228 LUJ, 인챈트 보호 아이템
		// 080320 LUJ, 맵으로 바꾸고, 인챈트 보호 아이템의 종류를 값으로 설정함
		typedef stdext::hash_map< DWORD, Data >	ItemMap;
		ItemMap									mItemMap;
	}
	mEnchantProtection;

	
private:
	// 080124, 웅주. TCPL 한국판 890p. 오버플로를 이용해서 0~1 사이의 실수 랜덤값을 만든다 
	class CRandom
	{
	public:
		CRandom() :
		mValue( rand() )
		{}

		DWORD GetDword()
		{
			return mValue = mValue * 1103515245 + rand();
		}

		float GetFloat()
		{
			return float( GetDword() ) / UINT_MAX;
		}

	private:
		DWORD mValue;
	}
	mRandom;

	// 080124 웅주, 드롭 옵션 관리
public:
	void AddDropOption( const ITEMBASE&, ITEM_OPTION& );

private:
	// 080130 LUJ, 	몬스터로부터 획득되는 아이템에 랜덤으로 옵션이 붙는다.
	//				이에 대한 설정 정보를 보관하는 구조체
	//				주의: 확률 맵은 해쉬 컨테이너를 이용해서는 안된다
	struct DropOptionScript
	{
		// 키: 확률, 값: 인챈트 수치
		typedef std::map< float, DWORD >	EnchantLevelMap;
		EnchantLevelMap						mEnchantLevelMap;

		// 키: 확률, 값: 부여될 옵션 개수
		typedef std::map< float, DWORD >	OptionSizeMap;
		OptionSizeMap						mOptionSizeMap;

		typedef ApplyOptionScript::Option	Option;

		// 확률마다 선택될 옵션 종류와 수치 범위를 담고 있다
		// 키: 선택 확률
		typedef std::map< float, Option >	OptionMap;

		// 키: 아이템 장착 레벨별로 적용할 옵션에 대한 확률 맵을 갖고 있다
		typedef std::map< DWORD, OptionMap >	LevelMap;
		LevelMap								mLevelMap;
	};

	void LoadDropOptionScript();

	LONGLONG GetItemKey( eEquipSlot, eWeaponType, eArmorType, eWeaponAnimationType ) const;
	
	// 키: 아이템 키. 아이템 정보로부터 고유한 키를 만든다
	typedef stdext::hash_map< LONGLONG, const DropOptionScript* >	DropOptionScriptMap;
	DropOptionScriptMap												mDropOptionScriptMap;

	typedef std::list< DropOptionScript >	DropOptionScriptList;
	DropOptionScriptList					mDropOptionScriptList;


	// 080215 LUJ, 유료로 적용되는 옵션 관리
public:
	void ApplyOption( const ITEMBASE& sourceItem, const ITEMBASE& targetItem, ITEM_OPTION& );

private:
	void LoadApplyOptionScript();

	// 아이템 인덱스
	typedef stdext::hash_map< DWORD, ApplyOptionScript >	ApplyOptionScriptMap;
	ApplyOptionScriptMap									mApplyOptionScriptMap;


	// 070914 웅주, 쿨타임 아이템 관리
private:
	//void ProcessCoolTime();
	void LoadCoolTimeScript();

	struct CoolTimeScript
	{
		struct Unit
		{
			DWORD mIndex;	// 쿨타임 그룹 인덱스
			DWORD mTime;	// 쿨타임 적용 시간: 밀리세컨드
		};

		// 키: 아이템 인덱스
		typedef stdext::hash_map< DWORD, Unit >	ItemData;
		ItemData								mItemData;

		// 값: 아이템 인덱스
		typedef stdext::hash_set< DWORD >				ItemIndex;

		// 키: 그룹 인덱스
		typedef stdext::hash_map< DWORD, ItemIndex >	GroupData;
		GroupData										mGroupData;
	}
	mCoolTimeScript;

	// 값: 쿨타임 그룹
	typedef stdext::hash_set< DWORD >					CoolTimeGroup;

	// 키: 플레이어 인덱스, 값: 사용 중인 쿨타임 아이템 그룹 인덱스
	typedef stdext::hash_map< DWORD, CoolTimeGroup >	UsingCoolTimeGroup;
	UsingCoolTimeGroup									mUsingCoolTimeGroup;

public:
	DWORD GetCoolTimeGroupIndex( DWORD itemIndex ) const;

	// 해당 아이템이 쿨타임 중인지 체크한다.
	//BOOL IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const;

	// 아이템을 쿨타임 상태로 등록하거나 해제
	//BOOL AddCoolTime( DWORD playerIndex, DWORD itemIndex );

	// 플레이어의 쿨타임 정보를 지운다.
	void RemoveCoolTime( DWORD playerIndex );
public:
	// 맵 이동 주문서
	// 071220 LUJ, 성공 결과를 반환토록 함
	BOOL Move_ChangeMap(CPlayer* pPlayer, MAPTYPE mapType);

	// 071204 LYW --- ItemManager : 스탯 초기화 함수 추가.
	BOOL UseItem_ResetStats(CPlayer* pPlayer, BOOL bCheat = FALSE) ;

	// 071204 LYW --- ItemManager : 스킬 초기화 함수 추가.
	BOOL UseItem_ResetSkill(CPlayer* pPlayer, BOOL bCheat = FALSE) ;

	// 080611 LYW --- ItemManager : 스킬포인트 획득 주문서 추가.
	BYTE UseItem_GetSkillPointByItem(CPlayer* pPlayer, WORD wPoint) ;


	// 080124 LUJ, 옵션 관리. 풀을 이용하고 관리의 용이성을 위해 통합적으로 관리한다
public:
	const ITEM_OPTION& GetOption( const ITEMBASE& ) const;
	void AddOption( const ITEM_OPTION& );
	void RemoveOption( const ITEMBASE& );
	void RemoveOption( DWORD itemDbIndex );

private:
	typedef stdext::hash_map< DWORD, ITEM_OPTION* >	OptionMap;
	OptionMap										mOptionMap;

	CMemoryPoolTempl< ITEM_OPTION >	mOptionPool;


	// 080414 LUJ, 외양 변환 스크립트 처리
public:
	struct BodyChangeScript
	{
		DWORD		mGender;
		RaceType	mRace;
		PART_TYPE	mPart;

		// 080414 LUJ, 텍스트 번호(InterfaceMsg.bin 참조)
		DWORD		mText;
		DWORD		mValue;
	};

	typedef std::list< BodyChangeScript >					BodyChangeScriptList;
	typedef stdext::hash_map< DWORD, BodyChangeScriptList >	BodyChangeScriptListMap;

private:	
	BodyChangeScriptListMap	mBodyChangeScriptListMap;

	void LoadBodyChangeScript();


	//081027 NYJ - 몬스터소환아이템
public:
	struct stMonSummonInfo
	{
		DWORD MonsterIdx;		// 몬스터 인덱스
		DWORD minRate;
		DWORD maxRate;

		stMonSummonInfo() {	MonsterIdx = minRate = maxRate = 0;	}
	};

	struct stMonSummonGroupInfo
	{
		WORD	GroupIdx;		// 그룹인덱스
		DWORD	dwDieTime;
		bool	bNotForbidden;
		
		std::vector<stMonSummonInfo*>	vecBossMosSummonGroup;
		std::vector<stMonSummonInfo*>	vecMonSummonGroup;


		stMonSummonGroupInfo() { GroupIdx = dwDieTime =0; bNotForbidden=false;}

		~stMonSummonGroupInfo()
		{
			std::vector<stMonSummonInfo*>::iterator iter;
			for(iter=vecBossMosSummonGroup.begin(); iter!=vecBossMosSummonGroup.end(); iter++)
			{
				if((*iter))
					SAFE_DELETE((*iter));
			}

			for(iter=vecMonSummonGroup.begin(); iter!=vecMonSummonGroup.end(); iter++)
			{
				if((*iter))
					SAFE_DELETE((*iter));
			}
		}

		DWORD MonSummon(DWORD rate)
		{
			int nBossCnt = vecBossMosSummonGroup.size() - 1;
			if(0<=nBossCnt && rate <= vecBossMosSummonGroup[nBossCnt]->maxRate)
			{
				// 확률테이블내에 위치한 보스몬스터 소환함.
				std::vector<stMonSummonInfo*>::iterator iter;
				for(iter=vecBossMosSummonGroup.begin(); iter!=vecBossMosSummonGroup.end(); iter++)
				{
					if((*iter)->minRate<=rate && rate<=(*iter)->maxRate)
						return (*iter)->MonsterIdx;
				}
				return 0;
			}
			else
			{
				// 081211 LYW --- ItemManager : ggomgrak 요청으로 체크인 함.
				// 1/n 확률로 일반몬스터 소환함.
				int nMonCnt = vecMonSummonGroup.size();

				if(nMonCnt < 1)
				{
					return 0;
				}
				else
				{
					int nRand = rand()%nMonCnt;
					return vecMonSummonGroup[nRand]->MonsterIdx;
				}
			}
		}
	};

	void LoadMonSummonItemInfo();

private:
	
	std::vector<stMonSummonGroupInfo*>	m_vecMonSummonGroupInfo;
	std::vector<int> m_vecMonSummonForbiddenMap;
};
