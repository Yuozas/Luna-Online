#pragma once

// < �Լ��� ���� �� �� �ݵ�� ���Ѿ� �� �� >
// 1. �Լ����� ����ó���� �Ϻ��� ó�� �� ��
// 2. ����ó�� �Ҷ� (���� �۾����� �ǵ��� �� RollBack!!!)

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


// ���� ������ �⺻ �ڷᱸ��
struct ItemMixResult
{
	DWORD	mItemIndex;
	WORD	mRequiredLevel;
	DWORD	mMoney;
	float	mSuccessRate;

	// Ű: ������ �ε���, ��: �ҿ� ����
	typedef stdext::hash_map< DWORD, DWORD >	Material;
	Material									mMaterial;
};


typedef std::list< ItemMixResult >	MixScript;


// ��Ʈ ������ �⺻ �ڷ� ����
struct SetScript
{
	std::string mName;

	// ��Ʈ�� �����Ǵ� ������ 
	typedef stdext::hash_set< DWORD >	Item;
	Item								mItem;

	struct Element
	{
		PlayerStat	mStat;

		// Ű: ��ų �ε���, ��: ���� ����
		typedef std::map< DWORD, BYTE >	Skill;
		Skill							mSkill;
	};

	// Ű: �ɷ��� �����Ǵ� ��Ʈ ������ ���� ����
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

	// ���ؽ� ������ �����ϴ� ������
	typedef std::list< Result > StaticResult;
	StaticResult				mStaticResult;

	// ���ؽ� Ȯ���� ���� �����ϴ� ������. lower_bound()�� ���Ƿ� hash_map�� ���� �ȵȴ�.
	typedef std::map< DWORD, Result >	DynamicResult;
	DynamicResult						mDynamicResult;

	// ���ذ����� �ּ� ����
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

	// Ű: Ȯ��, ��: �ο��� �ɼ� ����
	typedef std::map< float, DWORD >	OptionSizeMap;
	OptionSizeMap						mOptionSizeMap;

	// ����� ������ ����
	typedef stdext::hash_set< LONGLONG >	TargetTypeSet;
	TargetTypeSet							mTargetTypeSet;

	struct Option
	{
		ITEM_OPTION::Drop::Key	mKey;

		float	mBeginValue;
		float	mEndValue;
	};
	
	// Ȯ������ ���õ� �ɼ� ������ ��ġ ������ ��� �ִ�
	// Ű: ���� Ȯ��
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

	eType	mType;		// ��ȭ�� ��µǴ� �ɷ� ����
	DWORD	mMax;		// ��ȭ�� �ø� �� �ִ� �ִ� ��ġ
	float	mBias;		// ��ȭ�� ���� ��
	BOOL	mForRare;	// ���� ������ ��ȭ ���� ����

	// �ش� ���� ��ȭ�� �� �ִ� ������ ����(���� �������� ����)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};


// 070810 ����, ��æƮ ��ũ��Ʈ
struct EnchantScript
{
	// ��æƮ�� �ο��ϴ� ������(��æƮ ��ũ�� ��)�� ������ �ε���
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

	// ��æ ������ ������ ���� ���� ���� ����
	struct
	{
		DWORD mMin;
		DWORD mMax;
	}
	mItemLevel;

	DWORD mEnchantMaxLevel;	// �ִ�� ��æƮ�� �� �ִ� �ܰ�

	typedef stdext::hash_set< eType >	Ability;
	Ability								mAbility;

	// �ش� ���� ��æƮ�� �� �ִ� ������ ����(���� �������� ����)
	typedef stdext::hash_set< eEquipSlot >	Slot;
	Slot									mSlot;
};

// 071028 LYW --- ItemManager : Add enum code for error of discard item from quest group.
enum QG_DISCARD_ERR															// ����Ʈ �׷쿡�� ������ �����⸦ �� �� �߻��ϴ� ���� ����.
{
	err_No_Error = 10,														// ���� ����.
	err_Invalid_CItemSlot,													// ������ ���� ������ ����.
	err_Invalid_ITEMBASE,													// ������ �⺻ ������ ����.
	err_Not_Same_ItemIdx,													// ������ �ε��� ��ġ ���� ����.
	err_Not_StackItem,														// ���� �������� �ƴҰ�� ����.
	err_Not_NormalItem,														// �Ϲ� �������� �ƴҰ�� ����.
	err_Over_ItemCount,														// �ִ� ���� ī��Ʈ ���� ����.
	err_Invalid_RemainCount,												// ���� ������ �� ����.
	err_Failed_UpdateItem,													// ������ ������Ʈ ����.
	err_Failed_DeleteItem,													// ������ ���� ����.
} ;


class	CMHFile;
class	ITEM_REINFORCE_INFO;

struct	ITEM_INFO;
struct	MSG_ITEM_DISSOLVE_ACK;


class CItemManager  
{
	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.07.29
	enum CHANGEITEM_RESULT_KIND										// ü���� ������ ��� ����
	{
		TRK_ITEM = 0,												// ..������
		TRK_MONEY,													// ..��
	};

	struct stCHANGEITEM_RESULT_BASE
	{
		CHANGEITEM_RESULT_KIND	eResultKind;						// ..��� ����
		DWORD					nValue01;							// ..��ġ1
		DWORD					nValue02;							// ..��ġ2
		DWORD					nValue03;							// ..��ġ3
		DWORD					nProbaValue;						// ..Ȯ��
	};

	struct stCHANGEITEM_RESULT
	{
		int						  nResultBaseNum;
		stCHANGEITEM_RESULT_BASE* stResultBase;
	};

	struct stCHANGEITEM												// ü���� ������ ����
	{
		int						nChangeItemID;						// ..ü���� ������ ID
		int						nResultNum;							// ..��� ����
		stCHANGEITEM_RESULT*	stResult;							// ..���
	};
	// E ü���������� �߰� added by hseos 2007.07.29

	CYHHashTable<DealerData>	m_DealerTable;
	DWORD m_Key; //����, ��ȭ, ��޾��� �α� ���� �� ���� �����̾��ٴ� ���� ǥ���ϱ� ����.

	DWORD MakeNewKey();

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.07.29
	stCHANGEITEM*			m_stChangeItemInfo;
	int						m_nChangeItemInfoNum;
	// E ü���������� �߰� added by hseos 2007.07.29

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

	// �ʵ庸�� - 05.12 �̿���
	// ���Ͱ� ���� �������� ������ ����Ҽ� �ֵ��� ����
	void MonsterObtainItem(CPlayer*, DWORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum = 1);

	int CheatObtainItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum );
	//SW051011 Cheat Rare
	//int CheatObtainRareItem(CPlayer*, DWORD obtainItemIdx, WORD ItemNum, ITEM_OPTION* pOptionInfo);

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.07.08
	int ObtainMonstermeterItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum );
	// E ���͹��� �߰� added by hseos 2007.07.08

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.13
	int ObtainGeneralItem( CPlayer*, DWORD obtainItemIdx, WORD ItemNum, WORD nObtainKind, BYTE nProtocal );
	// E ��ȥ �߰� added by hseos 2007.12.13

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

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.11
	ITEM_INFO* GetUseItemInfo(CPlayer*, WORD TargetPos, DWORD wItemIdx);
	// E ����ý��� �߰� added by hseos 2007.09.11

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

	// 071211 KTH -- CPlayer �߰�
	unsigned int GetTotalEmptySlotNum(CItemSlot*, CPlayer*); //060612 Add by wonju

	// desc_hseos_ü����������01
	// S ü���������� �߰� added by hseos 2007.07.29	2007.08.03
	BOOL LoadScriptFileDataChangeItem();
	BOOL ProcChangeItem(CPlayer* pPlayer, DWORD nItemID);
	BOOL IsChangeItem(DWORD nItemID);
	// E ü���������� �߰� added by hseos 2007.07.29	2007.08.03

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

	// desc_hseos_������ ��ȿ��üũ01
	// S ������ ��ȿ��üũ �߰� added by hseos 2007.05.31
	BOOL	IsValidItem(DWORD nItemID)		{ if (m_ItemInfoList.GetData(nItemID)) return TRUE ; return FALSE; }
	// E ������ ��ȿ��üũ �߰� added by hseos 2007.05.31


	// 070613 ����, ��Ʈ ������ ����
public:
	// ����� ��Ʈ ��ũ��Ʈ�� �����Ѵ�
	void AddSetScript( const SetScript*, const SetScript::Element* );

	// ����� ��Ʈ ��ũ��Ʈ�� �����
	void RemoveSetScript( const SetScript* );

	const SetScript* GetSetScript( DWORD itemIndex ) const;

	// �ش� ��Ʈ ��ũ��Ʈ�� �ش��ϴ� �������� �÷��̾ ��� �ִ��� ��ȯ�Ѵ�.
	int GetSetItemSize( CPlayer*, const SetScript* );

private:
	void LoadSetScript();

	typedef stdext::hash_map< DWORD, std::string > SetItemName;
	void ParseSetScript( std::list< std::string >&, SetScript&, const SetItemName& );

	// 0706 ����, ��Ʈ ������ ������ ����
	std::list< SetScript >	mSetScript;

	// ������ ���� �޾� ������ �ִ� ��Ʈ ������ ��ȯ
	// Ű: ������ �ε���, ��: ��Ʈ ������ ����
	typedef stdext::hash_map< DWORD, const SetScript* >	SetScriptMap;
	SetScriptMap										mSetScriptMap;

	// 070703 ����, ���� ������ ����
private:
	//typedef std::map< POSTYPE, ITEMBASE >	MIrrorSlot;

	eLogitemmoney		Mix( CPlayer*, const MSG_ITEM_MIX_SYN2&, const CBackupSlot* );
	const MixScript*	GetMixScript( DWORD itemIndex ) const;
	void AddMixOption( CPlayer&, const ITEMBASE& );

private:
	void LoadMixScript();
	void LoadMixSetting();

	// Ű: ������ �ε���
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

		// �� ���� �� �������� ����ִ�. �� 20%, ��ø�� 80%�� ����. �̶� 100������ �������� ��� map�� lower_bound() �޼ҵ带 �̿��Ͽ�
		// ������ �ɷ�ġ�� ����.
		typedef std::map< DWORD, eAbility >			Ability;
		typedef std::map< eEquippment, Ability >	Equippment;
		Equippment									mEquippment;

		typedef struct Range
		{
			DWORD mMin;
			DWORD mMax;
		};

		// Ű: ���� ��. ��: ������ ��ġ ����
		typedef std::map< DWORD, Range >	RangeMap;
		RangeMap							mRangeMap;
	}
	mMixSetting;

	// 080228 LUJ, ���� ��ȣ ������ ����
	BOOL IsMixProtectionItem( const ITEMBASE& ) const;

	// 080228 LUJ, ���� ��ȣ ������ ����
	typedef stdext::hash_set< DWORD >	MixProtectionItemSet;
	MixProtectionItemSet				mMixProtectionItemSet;


	// 070710 ����, ���� ������ ����
public:
	eLogitemmoney			Dissolve( CPlayer*, const MSG_ITEM_DISSOLVE_SYN2&, const CBackupSlot* );
	const DissolveScript*	GetDissolveScript( DWORD itemIndex ) const;

private:
	void LoadDissolveScript();

	// Ű: ������ �ε���
	typedef std::map< DWORD, DissolveScript >	DissolveScriptMap;
	DissolveScriptMap							mDissolveScriptMap;


	// 070710 ����, ��ȭ ������ ����
public:
	eLogitemmoney	Reinforce( CPlayer*, const MSG_ITEM_REINFORCE_SYN&, const CBackupSlot* );

	const ReinforceScript*	GetReinforceScript( DWORD itemIndex ) const;

private:
	void LoadReinforceScript();

	// Ű: ������ �ε���
	typedef stdext::hash_map< DWORD, ReinforceScript >	ReinforceScriptMap;
	ReinforceScriptMap									mReinforceScriptMap;

	// 080228 LUJ, ��ȭ ��ȣ ������ ����
	BOOL IsReinforceProtectionItem( const ITEMBASE& ) const;

	// 080228 LUJ, ��ȭ ��ȣ ������ ����
	typedef stdext::hash_set< DWORD >	ReinforceProtectionItemSet;
	ReinforceProtectionItemSet			mReinforceProtectionItemSet;


	// 070810 ����, ��æƮ ������ ����
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
		// 080320 LUJ, ��ȣ ����
		struct Data
		{
			// 080328 LUJ, ���� ����ü �߰�
			struct Range
			{
				DWORD mMin;
				DWORD mMax;
			};

			// 080320 LUJ, ��ȣ�Ǵ� ��æƮ ���� ����
			Range mEnchantLevelRange;

			// 080328 LUJ, ��ȣ ������ ������ ���� ����
			Range mItemLimitLevelRange;

			// 080328 LUJ, ��æƮ ���̳ʽ� ���ʽ� ���� �ÿ��� ��ȣ�Ǵ� ���� ��æƮ ����
			DWORD mMinimumEnchantLevel;

			// 080328 LUJ, ��æƮ Ȯ���� �����Ǵ� ��
			int mAddedPercent;

			// 080328 LUJ, ��æƮ ����/���� �� ��æƮ ������ �󸶳� ���/�϶��� �� �����ϴ� Ȯ�� ��. Ű: Ȯ��, ��: �߰��� ��æƮ ����
			typedef std::map< float, int > BonusMap;

			// 080328 LUJ, ��æƮ ���� �� ����ϴ� �÷��� ���ʽ� ��
			BonusMap mPlusBonusMap;

			// 080328 LUJ, ��æƮ ���� �� ����ϴ� ���̳ʽ� ���ʽ� ��
			BonusMap mMinusBonusMap;			
		};

		// 080228 LUJ, ��æƮ ��ȣ ������
		// 080320 LUJ, ������ �ٲٰ�, ��æƮ ��ȣ �������� ������ ������ ������
		typedef stdext::hash_map< DWORD, Data >	ItemMap;
		ItemMap									mItemMap;
	}
	mEnchantProtection;

	
private:
	// 080124, ����. TCPL �ѱ��� 890p. �����÷θ� �̿��ؼ� 0~1 ������ �Ǽ� �������� ����� 
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

	// 080124 ����, ��� �ɼ� ����
public:
	void AddDropOption( const ITEMBASE&, ITEM_OPTION& );

private:
	// 080130 LUJ, 	���ͷκ��� ȹ��Ǵ� �����ۿ� �������� �ɼ��� �ٴ´�.
	//				�̿� ���� ���� ������ �����ϴ� ����ü
	//				����: Ȯ�� ���� �ؽ� �����̳ʸ� �̿��ؼ��� �ȵȴ�
	struct DropOptionScript
	{
		// Ű: Ȯ��, ��: ��æƮ ��ġ
		typedef std::map< float, DWORD >	EnchantLevelMap;
		EnchantLevelMap						mEnchantLevelMap;

		// Ű: Ȯ��, ��: �ο��� �ɼ� ����
		typedef std::map< float, DWORD >	OptionSizeMap;
		OptionSizeMap						mOptionSizeMap;

		typedef ApplyOptionScript::Option	Option;

		// Ȯ������ ���õ� �ɼ� ������ ��ġ ������ ��� �ִ�
		// Ű: ���� Ȯ��
		typedef std::map< float, Option >	OptionMap;

		// Ű: ������ ���� �������� ������ �ɼǿ� ���� Ȯ�� ���� ���� �ִ�
		typedef std::map< DWORD, OptionMap >	LevelMap;
		LevelMap								mLevelMap;
	};

	void LoadDropOptionScript();

	LONGLONG GetItemKey( eEquipSlot, eWeaponType, eArmorType, eWeaponAnimationType ) const;
	
	// Ű: ������ Ű. ������ �����κ��� ������ Ű�� �����
	typedef stdext::hash_map< LONGLONG, const DropOptionScript* >	DropOptionScriptMap;
	DropOptionScriptMap												mDropOptionScriptMap;

	typedef std::list< DropOptionScript >	DropOptionScriptList;
	DropOptionScriptList					mDropOptionScriptList;


	// 080215 LUJ, ����� ����Ǵ� �ɼ� ����
public:
	void ApplyOption( const ITEMBASE& sourceItem, const ITEMBASE& targetItem, ITEM_OPTION& );

private:
	void LoadApplyOptionScript();

	// ������ �ε���
	typedef stdext::hash_map< DWORD, ApplyOptionScript >	ApplyOptionScriptMap;
	ApplyOptionScriptMap									mApplyOptionScriptMap;


	// 070914 ����, ��Ÿ�� ������ ����
private:
	//void ProcessCoolTime();
	void LoadCoolTimeScript();

	struct CoolTimeScript
	{
		struct Unit
		{
			DWORD mIndex;	// ��Ÿ�� �׷� �ε���
			DWORD mTime;	// ��Ÿ�� ���� �ð�: �и�������
		};

		// Ű: ������ �ε���
		typedef stdext::hash_map< DWORD, Unit >	ItemData;
		ItemData								mItemData;

		// ��: ������ �ε���
		typedef stdext::hash_set< DWORD >				ItemIndex;

		// Ű: �׷� �ε���
		typedef stdext::hash_map< DWORD, ItemIndex >	GroupData;
		GroupData										mGroupData;
	}
	mCoolTimeScript;

	// ��: ��Ÿ�� �׷�
	typedef stdext::hash_set< DWORD >					CoolTimeGroup;

	// Ű: �÷��̾� �ε���, ��: ��� ���� ��Ÿ�� ������ �׷� �ε���
	typedef stdext::hash_map< DWORD, CoolTimeGroup >	UsingCoolTimeGroup;
	UsingCoolTimeGroup									mUsingCoolTimeGroup;

public:
	DWORD GetCoolTimeGroupIndex( DWORD itemIndex ) const;

	// �ش� �������� ��Ÿ�� ������ üũ�Ѵ�.
	//BOOL IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const;

	// �������� ��Ÿ�� ���·� ����ϰų� ����
	//BOOL AddCoolTime( DWORD playerIndex, DWORD itemIndex );

	// �÷��̾��� ��Ÿ�� ������ �����.
	void RemoveCoolTime( DWORD playerIndex );
public:
	// �� �̵� �ֹ���
	// 071220 LUJ, ���� ����� ��ȯ��� ��
	BOOL Move_ChangeMap(CPlayer* pPlayer, MAPTYPE mapType);

	// 071204 LYW --- ItemManager : ���� �ʱ�ȭ �Լ� �߰�.
	BOOL UseItem_ResetStats(CPlayer* pPlayer, BOOL bCheat = FALSE) ;

	// 071204 LYW --- ItemManager : ��ų �ʱ�ȭ �Լ� �߰�.
	BOOL UseItem_ResetSkill(CPlayer* pPlayer, BOOL bCheat = FALSE) ;

	// 080611 LYW --- ItemManager : ��ų����Ʈ ȹ�� �ֹ��� �߰�.
	BYTE UseItem_GetSkillPointByItem(CPlayer* pPlayer, WORD wPoint) ;


	// 080124 LUJ, �ɼ� ����. Ǯ�� �̿��ϰ� ������ ���̼��� ���� ���������� �����Ѵ�
public:
	const ITEM_OPTION& GetOption( const ITEMBASE& ) const;
	void AddOption( const ITEM_OPTION& );
	void RemoveOption( const ITEMBASE& );
	void RemoveOption( DWORD itemDbIndex );

private:
	typedef stdext::hash_map< DWORD, ITEM_OPTION* >	OptionMap;
	OptionMap										mOptionMap;

	CMemoryPoolTempl< ITEM_OPTION >	mOptionPool;


	// 080414 LUJ, �ܾ� ��ȯ ��ũ��Ʈ ó��
public:
	struct BodyChangeScript
	{
		DWORD		mGender;
		RaceType	mRace;
		PART_TYPE	mPart;

		// 080414 LUJ, �ؽ�Ʈ ��ȣ(InterfaceMsg.bin ����)
		DWORD		mText;
		DWORD		mValue;
	};

	typedef std::list< BodyChangeScript >					BodyChangeScriptList;
	typedef stdext::hash_map< DWORD, BodyChangeScriptList >	BodyChangeScriptListMap;

private:	
	BodyChangeScriptListMap	mBodyChangeScriptListMap;

	void LoadBodyChangeScript();


	//081027 NYJ - ���ͼ�ȯ������
public:
	struct stMonSummonInfo
	{
		DWORD MonsterIdx;		// ���� �ε���
		DWORD minRate;
		DWORD maxRate;

		stMonSummonInfo() {	MonsterIdx = minRate = maxRate = 0;	}
	};

	struct stMonSummonGroupInfo
	{
		WORD	GroupIdx;		// �׷��ε���
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
				// Ȯ�����̺��� ��ġ�� �������� ��ȯ��.
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
				// 081211 LYW --- ItemManager : ggomgrak ��û���� üũ�� ��.
				// 1/n Ȯ���� �Ϲݸ��� ��ȯ��.
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
