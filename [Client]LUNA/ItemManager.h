#pragma once


#define ITEMMGR USINGTON(CItemManager)


#include "item.h"


class CHero;
class CPlayer;
class cImage;
class cWindow;
class CVirtualItem;
class CItemShow;
class cQuickItem;
class CExchangeItem;
class CBuyItem;

//---for item tooltip
struct TOOLTIP_TEXT
{
	TOOLTIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	TOOLTIP_TEXT*	pNext;
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

	// ���ؽ� Ȯ���� ���� �����ϴ� ������
	typedef std::map< DWORD, Result >	DynamicResult;
	DynamicResult								mDynamicResult;

	// ���ذ����� �ּ� ����
	DWORD mLevel;
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

struct EnchantEffect
{
	DWORD	ItemIdx;

	WORD	Effect[13][3];
};

struct EVENTINFO
{
	DWORD	Index;
	DWORD	Delay;
	DWORD	Next;
	DWORD	Item;
	DWORD	Count;
	DWORD	Window;
	char*	Msg;
};

class CItemManager  
{
	//CItemCalc m_ItemCalc;
	CYHHashTable<EVENTINFO>			mEventInfoTable;

	DWORD			m_GiftEventTime;
	DWORD			m_GifeEventCheckTime;

	CYHHashTable<CItem> m_ItemHash;

	CYHHashTable<ITEM_INFO> m_ItemInfoList;

	CYHHashTable<EnchantEffect> mEffectList;
	/////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ����/����
	// ���Ž� �����ϴ� ������
	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;
	/////////////////////////////////////////////

	CYHHashTable<TOOLTIP_TEXT>		m_ItemToolTipTextList;
	//CYHHashTable<ITEM_OPTION>	m_ItemOptionList;

	// 080124 LUJ
	// Ű: ������ DB �ε���
	typedef stdext::hash_map< DWORD, ITEM_OPTION* >	OptionMap;
	OptionMap										mOptionMap;
	//CYHHashTable<SHOPITEMBASE>		m_UsedItemList;

	CIndexGenerator m_IconIndexCreator;
	BOOL			m_bAddPrice;
	ITEMBASE		m_TempDeleteItem;

	//---KES ItemDivide Fix 071020
	POSTYPE			m_DividePos;
	int				m_DivideDur;
	//----------------------------

	// 071127 LYW --- ItemManager : ���� ������ ( ���� ���� ) ��ġ ���� ���� �߰�.
	int			m_nSealItemDelPos ;

public:
	//	//MAKESINGLETON(CItemManager)

	CItemManager();
	virtual ~CItemManager();

	//---KES ItemDivide Fix 071020
	void SetDivideItemInfo( POSTYPE pos, int dur )	{ m_DividePos = pos; m_DivideDur = dur; }
	POSTYPE GetDivideItemInfoPos()					{ return m_DividePos; }
	int		GetDivideItemInfoDur()					{ return m_DivideDur; }
	//----------------------------

	void				AddOption	( const ITEM_OPTION& );
	const ITEM_OPTION&	GetOption	( const ITEMBASE& ) const;
	const ITEM_OPTION&	GetOption	( DWORD itemDbIndex ) const;
	void				RemoveOption( DWORD itemDbIndex );


	void Release();

	CItem* MakeNewItem( const ITEMBASE*, const char* strKind);

	void ItemDelete(CItem* pItem);
	void FakeDeleteItem(CItem* pItem);
	void SendDeleteItem();

	cImage* GetIconImage(DWORD ItemIdx, cImage * pImageGet);
	CItem* GetItem(DWORD dwDBIdx);

	// 071215 LYW --- ItemManager : �������� �ҼȾ����� ���� ó�� �Լ� �߰�.
	void SetSocialIconTooltip(cQuickItem* pItem, SLOT_INFO* pSlotInfo) ;

public:
	void AddToolTip( cWindow*, DWORD toolTipIndex, DWORD color = TTTC_DESCRIPTION );
	void AddToolTip( cIcon* );
	void AddToolTip( CVirtualItem* );
	void AddToolTip( CItemShow* );
	void AddToolTip( CItem* );
	void AddToolTip( cQuickItem* );
	void AddToolTip( CExchangeItem* );
	void AddToolTip( CBuyItem* );

	// 071125 LYW --- ItemManager : �ð� ���� ���� �����Լ� �߰�.
	void AddTimeToolTip( cIcon* icon, const ITEM_OPTION&, BOOL isFirst ) ;

private:
	void AddNametoolTip			( cIcon&, const ITEMBASE& );
	void AddOptionToolTip		( cIcon&, const ITEMBASE& );
	void AddFunctionToolTip		( cIcon&, const ITEMBASE& );
	void AddMakerToolTip		( cIcon&, const ITEMBASE& );
	void AddSetToolTip			( cIcon&, const ITEMBASE& );
	void AddPetToolTip			( cIcon&, const ITEMBASE& );
	void AddDescriptionToolTip	( cIcon&, const ITEMBASE&, DWORD color = TTTC_DESCRIPTION );
	
	// 071115 LYW --- ItemManager : ���� ���ð� ���� �߰�.
	void AddRemainTimeToolTip( cIcon&, const ITEMBASE&, BOOL bFirst );

	// 071126 LYW --- ItemManager : ���� ���ð� ���� �߰�.
	void AddUseableTimeToolTip( cIcon&, const ITEMBASE& );


public:
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void MoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void PetMoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void DivideItem(ITEMOBTAINARRAY * pMsg );
	/*	
	void MoveItemToMunpa( MSG_ITEM_MUNPA_MOVE_SYN* pMsg );
	void MoveItemToStorage(MSG_ITEM_MUNPA_MOVE_SYN* pMsg);
	*/
	void MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToStorage( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg);

	void CombineItem( MSG_ITEM_COMBINE_ACK * pMsg );
	//	void DivideItem( MSG_ITEM_DIVIDE_ACK * pMsg );
	//SW051012 ����
	// 080218 LUJ, ���� �ʴ� �ɼ� ���� ������ ������
	void DeleteItem( POSTYPE absPos, CItem** ppItemOut );
	BOOL AddItem(CItem* pItem);

	//	void GetYoungyakItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetSkillItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetEquipItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
	//	void GetExtraItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);

	BYTE GetTableIdxForAbsPos(POSTYPE absPos);
	CItem* GetItemofTable(WORD TableIDX, POSTYPE absPos);

	// yunho StatsCalcManager ������ ����
	const ITEMBASE* GetItemInfoAbsIn( CPlayer*, POSTYPE absPos);

	//void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION * pItemOptionInfoOut=NULL/*, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL*/);
	// 071202 LYW --- ItemManager : ���� �������� ���� �� ��, ���� ������ ���� ������ �� ������ ���� �� �Լ��� ����ϱ� ������, 
	//								�Լ� ���� ���ڷ� �ȶ����� �������� ���θ� üũ�ϵ��� ������.
	// 080218 LUJ, ���� �ʴ� �ɼ� ������ ������
	void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, BOOL bSellDel ) ;
	BOOL IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos );

	void RefreshAllItem();
	void RefreshItem( CItem* pItem );
	void RefreshItemToolTip(DWORD ItemDBIdx);
	BOOL CanEquip(DWORD wItemIdx);
	BOOL CanUse(DWORD wItemIdx);
	BOOL CanConvertToSkill(DWORD wItemIdx,SKILL_TYPE SkillType=SKILLTYPE_MAX);

	void LoadItemList();
	void LoadItemToolTipList();
	void LoadItemEffectList();
	void LoadGiftEvent();

	EnchantEffect* GetItemEffect( DWORD ItemIdx );
	ITEM_INFO * GetItemInfo(DWORD wItemIdx);
	void SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count);
	DWORD m_dwStateParam;

	//SW050920 Rare
	//BOOL IsRareOptionItem( DWORD wItemIdx, DWORD dwRareIdx );
	BOOL IsRare( const ITEM_INFO* ) const;

	//BOOL CanGradeUp(CItem * pItem);
	BOOL IsDupItem( DWORD wItemIdx );
	
	void SetPriceToItem( BOOL bAddPrice );
	void SetDisableDialog(BOOL val, BYTE TableIdx);

	///
	TOOLTIP_TEXT* GetItemToolTipInfo( DWORD wIdx );
	
	// 060911 KKR �� ������ ���ð� �Ѹ��� 
	void PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo );
	ITEM_INFO* FindItemInfoForName( char* strName );

	//	BOOL CheckQPosForItemIdx( DWORD wItemIdx );

	void ReLinkQuickPosition(CItem* pItem);	

	//
	// 071121 LYW - Modified.
	void AddUsedItemInfo(ITEMBASE* pInfo) ;
	//void AddUsedItemInfo(SHOPITEMBASE* pInfo);
	//void RemoveUsedItemInfo(DWORD wIndex, DWORD dwDBIdx=0);
	// 071121 LYW - Modified.
	//SHOPITEMBASE* GetUsedItemInfo(DWORD Index);
	//ITEMBASE* GetUsedItemInfo(DWORD Index) ;
	void Process();
	//void DeleteShopItemInfo();
	// 071121 LYW - Modified.
	void AddUsedAvatarItemToolTip( ITEMBASE* pInfo ) ;
	//void AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo );

	void ReviveOtherOK();
	void ReviveOtherSync();
	void ReviveOtherCancel();
	void ReviveOtherConfirm( BOOL bRevive );

	//void RefreshStatsBuffIcon();
	void AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	//	BOOL CheckItemStage( BYTE bItemStage );

	void ItemDropEffect( DWORD wItemIdx );
	void MoneyDropEffect();

	//BOOL LoadRareItemInfo();
	//BOOL IsRareItemAble( DWORD ItemIdx )
	//{
	//	if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
	//	return FALSE;
	//}

	void SetIconColorFilter(DWORD dwDBIdx, DWORD clrRGBA);
	void SetIconColorFilter(cIcon* pIcon, DWORD clrRGBA);
	//for GM...�����..
#ifdef _GMTOOL_
	void SetItemIfoPositionHead()	{ m_ItemInfoList.SetPositionHead(); }
	ITEM_INFO* GetItemInfoData()	{ return m_ItemInfoList.GetData(); }
#endif

	/////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ����/����
	// ���Ž� �����ϴ� ������
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////


	// 070205 LYW --- Add functions to process network msg.
private:
	void Item_TotalInfo_Local( void* pMsg ) ;
	void Item_Storage_Item_Info_Ack( void* pMsg ) ;
	//void Item_Monster_RareObtain_Notify( void* pMsg ) ;
	void Item_ReInForce_Ack( void* pMsg ) ;
	void Item_Reinforce_Option_Ack( void* );
	void Item_ReInForce_Nack( void* pMsg ) ;
	void Item_Move_Ack( void* pMsg ) ;
	void Item_Pet_Move_Ack( void* pMsg ) ;
	void Item_Combine_Ack( void* pMsg ) ;
	void Item_Divide_Ack( void* pMsg ) ;
	void Item_Monster_Obtain_Notify( void* pMsg ) ;
	void Item_Move_Nack( void* pMsg ) ;
	void Item_Divite_Nack( void* pMsg ) ;
	void Item_Combine_Nack( void* pMsg ) ;
	void Item_Discard_Nack( void* pMsg ) ;
	void Item_Discard_Ack( void* pMsg ) ;
	void Item_Use_Notify( void* pMsg ) ;
	void Item_Use_Ack( void* pMsg ) ;

private:
	void Item_Mix_Ack( void* pMsg ) ;
	void Item_Mix_Nack( void* pMsg );
	void Item_Mix_GetItem( void* );

private:
	// DB �ε����� ���� �������� �����ϴ� �����̳�. DB���� ������ ���� ����, ������ ����
	// ������ �ſ� ������. ���� �̸� ��� �Ұ����� �������� �����س���, ������ �ö�
	// ��밡���ϵ��� �� ���̴�. ����, ���ذ� ������ ���� �ش�ȴ�
	typedef std::list< CItem* > PreIconList;
	PreIconList					mPreIconList;


	void Item_Working_Success( void* pMsg ) ;
	void Item_Working_Fail( void* pMsg ) ;
	void Item_Working_Start( void* pMsg ) ;
	void Item_Working_Stop( void* pMsg ) ;
	void Item_TimeLimit_Item_OneMinute( void* pMsg ) ;
	void Item_ShopItem_Use_Ack( void* pMsg ) ;
	void Item_ShopItem_Use_Nack( void* pMsg ) ;
	void Item_Enchant_Success_Ack( void* pMsg ) ;
	void Item_Enchant_Failed_Ack( void* pMsg );
	void Item_Enchant_Nack( void* pMsg ) ;
	void Item_Use_Nack( void* pMsg ) ;
	void Item_Sell_Ack( void* pMsg ) ;
	void Item_Sell_Nack( void* pMsg ) ;
	void Item_Buy_Ack( void* pMsg ) ;
	void Item_Buy_Nack( void* pMsg ) ;
	void Item_Dealer_Ack( void* pMsg ) ;
	//void Item_Appearance_Change( void* pMsg ) ;
	void Item_Appearance_Add( void* );
	void Item_Appearance_Remove( void* );
	void Item_Money( void* pMsg ) ;
	void Item_Obtain_Money( void* pMsg ) ;
	void Item_Divide_NewItem_Notify( void* pMsg ) ;
	void Item_Guild_Move_Ack( void* pMsg ) ;
	void Item_Guild_Move_Nack( void* pMsg ) ;
	//void Item_Guild_WareHouse_Info_Ack( void* pMsg ) ;
	//void Item_Guild_WareHouse_Info_Nack( void* pMsg ) ;
	void Item_DeleteFrom_GuildWare_Notify( void* pMsg ) ;
	void Item_AddTo_GuildWare_Notify( void* pMsg ) ;
	void Item_Storage_Move_Ack( void* pMsg ) ;
	void Item_Dissolve_GetItem( void* pMsg ) ;
	void Item_Dissolve_Ack( void* pMsg ) ;
	void Item_Dissolve_Nack( void* pMsg ) ;
	void Item_Error_Nack( void* pMsg ) ;
	void Item_ChangeItem_Nack( void* pMsg ) ;
	void Item_Use_ChangeItem_Ack( void* pMsg ) ;
	void Item_EventItem_Use( void* pMsg ) ;
	void Item_ShopItem_Info_Ack( void* pMsg ) ;
	// 071204 LYW --- ItemManager :
	void Item_Reset_Stats_Ack( void* pMsg ) ;
	// 071208 LYW --- ItemManager : ���� �ʱ�ȭ ���� ó��.
	void Item_Reset_Stats_Nack( void* pMsg ) ;
	// 071210 LYW --- ItemManager : �κ��丮 Ȯ�� ����, ���� �Լ� �߰�.
	void Item_Increase_Inventory_Ack( void* pMsg ) ;
	void Item_Increase_Inventory_Nack( void* pMsg ) ;
	// 071226 LYW --- ItemManager : �̸� ���� ������ ��� ����, ���� �Լ� �߰�.
	void Item_Change_Character_Name_Ack( void* pMsg ) ;
	void Item_Change_Character_Name_Nack( void* pMsg ) ;

	// 071203 LYW --- ItemManager
	void Item_Skill_Ack( void* pMsg ) ;

	void Item_Gift_Event_Notify( void* pMsg );

	// 070329 LYW --- ItemManager : Add function to make string.
	void CompositString( char* inputStr, char* str, ... ) ;


	// 070613 ����, ��Ʈ ������ ����
public:	
	const SetScript*	GetSetScript( DWORD itemIndex ) const;
	int					GetSetItemSize( CPlayer*, const SetScript* );

private:
	void LoadSetScript();

	typedef std::map< DWORD, std::string > SetItemName;

	void ParseSetScript( std::list< std::string >&, SetScript&, const SetItemName& );

	// ��Ʈ ������ ������ ����
	std::list< SetScript >	mSetScript;

	// ������ ���� �޾� ������ �ִ� ��Ʈ ������ ��ȯ
	// Ű: ������ �ε���, ��: ��Ʈ ������ ����
	typedef stdext::hash_map< DWORD, const SetScript* >	SetScriptMap;
	SetScriptMap										mSetScriptMap;


	// 070703 ����, ���� ������ ����
public:	
	const MixScript*		GetMixScript( DWORD itemIndex ) const;
	const ItemMixResult*	GetMixResult( DWORD itemIndex, DWORD level ) const;

	// 080228 LUJ, ���� ��ȣ ������ ����
	BOOL IsMixProtectionItem( const ITEMBASE& ) const;

private:
	void LoadMixScript();

	// Ű: ������ �ε���
	typedef stdext::hash_map< DWORD, MixScript >	MixScriptMap;
	MixScriptMap									mMixScriptMap;

	// 080228 LUJ, ���� ��ȣ ������ ����
	typedef stdext::hash_set< DWORD >	MixProtectionItemSet;
	MixProtectionItemSet				mMixProtectionItemSet;


	// 070709 ������ �̸� ������ ������
public:
	DWORD	GetItemNameColor( const ITEM_INFO& ) const;


	// 070710 ����, ���� ������ ����
public:
	const DissolveScript*	GetDissolveScript( DWORD itemIndex ) const;

private:
	void LoadDissolveScript();

	// Ű: ������ �ε���
	typedef stdext::hash_map< DWORD, DissolveScript >	DissolveScriptMap;
	DissolveScriptMap									mDissolveScriptMap;


	// 070710 ����, ��ȭ ������ ����	
public:
	// Ű: ������ �ε���
	typedef std::map< DWORD, ReinforceScript >	ReinforceScriptMap;

	const ReinforceScriptMap&	GetReinforceScript() const;
	const ReinforceScript*		GetReinforceScript( DWORD itemIndex ) const;

	// 080228 LUJ, ��ȭ ��ȣ ������ ����
	BOOL IsReinforceProtectionItem( const ITEMBASE& ) const;

private:
	void LoadReinforceScript();

	ReinforceScriptMap mReinforceScriptMap;

	// 080228 LUJ, ��ȭ ��ȣ ������ ����
	typedef stdext::hash_set< DWORD >	ReinforceProtectionItemSet;
	ReinforceProtectionItemSet			mReinforceProtectionItemSet;


	// 070810 ����, ��æƮ ������ ����
public:
	const EnchantScript*	GetEnchantScript( DWORD itemIndex ) const;
	void AddEnchantValue( ITEM_OPTION&, const EnchantScript::Ability&, int value ) const;

	// 080228 LUJ, ��æƮ ������ ������ ����
	BOOL IsEnchantableItem( const ITEMBASE&, BOOL isProtect ) const;

	// 080228 LUJ, ��æƮ ��ȣ ������ ����
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
	};

	// 080228 LUJ, ��æƮ ��ȣ ������ ��ȯ
	const EnchantProtection& GetEnchantProtection() const;

private:
	void LoadEnchantScript();

	// Ű: ������ �ε���
	typedef stdext::hash_map< DWORD, EnchantScript >	EnchantScriptMap;
	EnchantScriptMap									mEnchantScriptMap;

	// 080228 LUJ, ��æƮ ��ȣ ������ ����
	EnchantProtection mEnchantProtection;

	// 080218 LUJ, �ɼ� �ο� ������ ����
public:
	BOOL IsApplyOptionItem( DWORD itemIndex ) const;
	const ApplyOptionScript* GetApplyOptionScript( DWORD itemIndex ) const;
	LONGLONG GetItemKey( eEquipSlot, eWeaponType, eArmorType, eWeaponAnimationType ) const;
	
private:
	void LoadApplyOptionScript();

	typedef stdext::hash_map< DWORD, ApplyOptionScript >	ApplyOptionScriptMap;
	ApplyOptionScriptMap									mApplyOptionScriptMap;


	// 070914 ����, �� Ÿ�� ��ũ��Ʈ
public:
	DWORD GetCoolTimeGroupIndex( DWORD itemIndex ) const;

	// �ش� �������� ��Ÿ�� ������ üũ�Ѵ�.
	BOOL IsCoolTime( DWORD playerIndex, DWORD itemIndex ) const;

	// �������� ��Ÿ�� ���·� ����ϰų� ����
	BOOL AddCoolTime( DWORD playerIndex, DWORD itemIndex );

	// �÷��̾��� ��Ÿ�� ������ �����.
	void RemoveCoolTime( DWORD playerIndex );

	// 080326 NYJ --- ��Ÿ�Ӱ���
	void UpdateDlgCoolTime(DWORD itemIndex);	//HERO���Ը� ����.


private:
	void ProcessCoolTime();
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

	// ��Ÿ���� ���� �����Ǵ� ������ ���Ľ�Ű�µ� ����ϴ� ��ƿ��Ƽ Ŭ����.
	class CoolTimeSort
	{
	public:
		struct Tick
		{
			DWORD mPlayerIndex;		// �ش� ��Ÿ�� ������ ������ �÷��̾� �ε���
			DWORD mCoolTimeGroup;	// ��Ÿ�� �׷� �ε���
			DWORD mBegin;			// ��Ÿ�� ���� �ð�
			DWORD mEnd;				// ��Ÿ�� ���� �ð�
		};

		bool operator() ( const Tick& lhs, const Tick& rhs ) const
		{
			const DWORD	 maxTick		= UINT_MAX;
			const DWORD lhsRemainTime	= ( lhs.mBegin > lhs.mEnd ? maxTick - lhs.mBegin + lhs.mEnd : lhs.mEnd - lhs.mBegin );
			const DWORD rhsRemainTime	= ( rhs.mBegin > rhs.mEnd ? maxTick - rhs.mBegin + rhs.mEnd : rhs.mEnd - rhs.mBegin );

			return lhsRemainTime > rhsRemainTime;
		}
	};

	// ���� �ð��� ���� 
	// ���ǻ���: �ڷ�� �ݵ�� �� ����� �־���Ѵ�. �����ͷ� ���� ��� �����ϸ鼭 �� �ּҸ� �Ҿ������.
	std::priority_queue< CoolTimeSort::Tick, std::vector< CoolTimeSort::Tick >, CoolTimeSort > mCoolTimeQueue;

	// ��: ��Ÿ�� �׷�
	typedef stdext::hash_set< DWORD >	CoolTimeGroup;

	// Ű: �÷��̾� �ε���, ��: ��� ���� ��Ÿ�� ������ �׷� �ε���
	typedef stdext::hash_map< DWORD, CoolTimeGroup >	UsingCoolTimeGroup;
	UsingCoolTimeGroup									mUsingCoolTimeGroup;

	// 080326 NYJ --- ��Ÿ������ ����
	std::map<DWORD, CoolTimeSort::Tick> m_mapCoolTime;


	// 070927. ��ȯ ������ ���� ���α׷����� ǥ���ϵ��� ��
public:
	BOOL IsChangeItem( DWORD itemIndex ) const;

	// 071203 LYW --- ItemManager : ���� �ʱ�ȭ ������ ó��.
	void UseItem_ResetStats(CItem* pItem) ;
	// 071203 LYW --- ItemManager : ��ų �ʱ�ȭ ������ ó��.
	void UseItem_ResetSkill(CItem* pItem) ;
	// 071203 LYW --- ItemManager : �� �̵� ��ũ�� ó��.
	void UseItem_MapMoveScroll(CItem* pItem) ;
	void Item_MapMoveScroll_Nack( void* pMsg ) ;
	// 071206 LYW --- ItemManager : â�� Ȯ�� ������ ó��.
	void UseItem_ExtendStorage(CItem* pItem) ;
	// 080611 LYW --- ��ų ����Ʈ �߰� �ֹ��� ó��.
	void GetSkillPointByItem(CItem* pItem) ;

	// 080607 LYW --- ItemManager : â�� ��ȯ �Լ� �߰�.
	void OpenStorageByItem(CItem* pItem) ;

	// 071210 LYW --- ItemManager : �κ��丮 Ȯ�� ������ ó��.
	void UseItem_ExtendInventory(CItem* pItem) ;
	// 071206 LYW --- ItemManager : �̹� ����Ͽ� ������ Ȱ��ȭ ������ üũ�ϴ� �Լ� �߰�.
	BOOL IsUsingItem(CItem* pItem) ;
	// 071206 LYW --- ItemManager : �����۸� ������ ��� �Լ� �߰�.
	void UseItem_ItemMallItem(CItem* pItem) ;
	// 071228 LYW --- ItemManager : �̸� ���� ������ ��� �Լ� �߰�.
	void UseItem_ChangeName(char* pName) ;

	// 080218 KTH -- Reset RegistResident
	void UseItem_ResetRegistResident(CItem* pItem);

	// 080312 NYJ --- ItemManager : ��ü��ġ�� ������ ó��.
	void UseItem_WorldShout(CItem* pItem) ;
	//void Item_WorldShout_ack( void* pMsg ) ;
	void Item_WorldShout_Nack( void* pMsg ) ;

	// 080326 NYJ --- ��Ÿ������ �ʱ�ȭ
	void ClearCoolTimeInfo() {m_mapCoolTime.clear();}

	// 080412 LYW --- ItemManager : Add a function to check whether megaphone item or not.
	BOOL IsMegaphoneItem(DWORD dwItemIdx)
	{
		CoolTimeScript::ItemData::const_iterator it = mCoolTimeScript.mItemData.find(dwItemIdx) ;

		if(it != mCoolTimeScript.mItemData.end())
		{
			const CoolTimeScript::Unit& unit = it->second ;

			if( unit.mIndex == 10 ) return TRUE ;
			else return FALSE ;
		}
		else return FALSE ;
	}

private:
	void LoadChangeItem();

	typedef stdext::hash_set< DWORD >	ChangeItemSet;
	ChangeItemSet						mChangeItemSet;

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
	
	const BodyChangeScriptListMap&	GetBodyChangeScriptListMap() const;
	
private:	
	BodyChangeScriptListMap	mBodyChangeScriptListMap;
	
	void LoadBodyChangeScript();
};


EXTERNGLOBALTON(CItemManager)
