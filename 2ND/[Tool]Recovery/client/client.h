// client.h : client ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CclientApp:
// �� Ŭ������ ������ ���ؼ��� client.cpp�� �����Ͻʽÿ�.
//
#include "grid_control/GridCtrl.h"


// ��ų ��ũ��Ʈ���� ����ϴ� �ڷᱸ���� MFC�� �ڷᱸ���� �浹�Ͽ� SkillInfo.h�� ������ �� ����
// ���� �⺻���� ������ �Ľ��ؼ� ������� ����ü�� �������ߴ�.
struct SkillScript
{
	DWORD	mIndex;
	CString	mName;
	BOOL	mIsPassive;
	DWORD	mMoney;	// �н��� �ʿ��� ��
	DWORD	mPoint;	// �н��� �ʿ��� ����Ʈ
};

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


struct QuestScript
{
	DWORD	mMonsterIndex;
	DWORD	mMonsterHuntCount;

	// Ű: ������ �ε���, ��: �ʿ� ����
	typedef std::map< DWORD, DURTYPE >	ItemMap;
	ItemMap								mItemMap;
};


class CLoginDialog;
class CNewUserDialog;
class CPlayerDialog;
class CItemDialog;
class CWaitDialog;
class CSkillDialog;
class CItemFindDialog;
class CFamilyDialog;
class CQuestDialog;
class CQuestLogDialog;
class CGuildDialog;
class CExperienceLogDialog;
class CItemLogDialog;
class CStatLogDialog;
class CSkillLogDialog;
class CItemOptionLogDialog;
class CGuildLogDialog;
class CGuildWarehouseLogDialog;
class CUserDialog;
class CJobLogDialog;
class CGuildScoreLogDialog;
class CFamilyCropLogDialog;
class CFamilyPointLogDialog;
//class CUserCreateDialog;
//class CUserRestoreDialog;
class CPermissionDialog;
class COperatorDialog;
class COperatorLogDialog;
class CNameLogDialog;


class CclientApp : public CWinApp
{
public:
	CclientApp();
	~CclientApp();


// ������
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage( MSG* );
	void Parse( DWORD serverIndex, const MSGROOT* );
	void Send( const CString& serverName, const MSGROOT&, size_t );
	void Send( DWORD serverIndex, const MSGROOT&, size_t );
	void SendAll( const MSGROOT&, size_t );
	const CString	GetRankName( DWORD rank ) const;

	CString GetServerName( DWORD serverIndex ) const;
	DWORD	GetServerIndex( const char* serverName ) const;

	// ������ ���� ����
	size_t	GetConnectionSize() const;
	
protected:
	void SetWait( BOOL );


// ����
protected:
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;


public:
	CDialog*				GetWaitDialog();
	CNewUserDialog*			GetNewUserDialog();
	CItemDialog*			GetItemDialog();
	CSkillDialog*			GetSkillDialog();
	CPlayerDialog*			GetPlayerDialog();
	CQuestDialog*			GetQuestDialog();
	CQuestLogDialog*		GetQuestLogDialog();
	CFamilyDialog*			GetFamilyDialog();
	CExperienceLogDialog*	GetExperienceDialog();
	CStatLogDialog*			GetStatLogDialog();
	CSkillLogDialog*		GetSkillLogDialog();
	CItemLogDialog*			GetItemLogDialog();
	CItemOptionLogDialog*	GetItemOptionLogDialog();
	CGuildLogDialog*		GetGuildLogDialog();
	CGuildWarehouseLogDialog*	GetGuildItemLogDialog();
	CUserDialog*			GetUserDialog();
	CJobLogDialog*			GetJobLogDialog();
	CGuildScoreLogDialog*	GetGuildScoreLogDialog();
	CFamilyCropLogDialog*	GetFamilyCropLogDialog();
	CFamilyPointLogDialog*	GetFamilyPointLogDialog();
	//CUserCreateDialog*		GetUserCreateDialog();
	//CUserRestoreDialog*		GetUserRestoreDialog();
	CPermissionDialog*		GetPermissionDialog();
	CNameLogDialog*			GetNameLogDialog();

	//CPlayerDialog*	GetPlayerDialog();
	//CItemDialog*	GetItemDialog();

	// �������� �޽����� �ö����� ����Ѵ�
	
	
protected:
	// ���� �Ҵ�� ��ȭâ�� ��Ƶΰ�, �Ͻÿ� �����Ѵ�
	typedef std::list< CDialog* >	DialogList;
	DialogList						mDialogList;

	CLoginDialog* 				mLoginDialog;
	CNewUserDialog* 			mNewUserDialog;
	CPlayerDialog* 				mPlayerDialog;
	CItemDialog* 				mItemDialog;
	CWaitDialog* 				mWaitDialog;
	CSkillDialog*				mSkillDialog;
	CItemFindDialog*			mItemFindDialog;
	CFamilyDialog*				mFamilyDialog;
	CQuestDialog*				mQuestDialog;
	CQuestLogDialog*			mQuestLogDialog;
	CGuildDialog*				mGuildDialog;
	CExperienceLogDialog*		mExperienceLogDialog;
	CItemLogDialog*				mItemLogDialog;
	CStatLogDialog*				mStatLogDialog;
	CSkillLogDialog*			mSkillLogDialog;
	CItemOptionLogDialog*		mItemOptionLogDialog;
	CGuildLogDialog*			mGuildLogDialog;
	CGuildWarehouseLogDialog*	mGuildWarehouseLogDialog;
	CUserDialog*				mUserDialog;
	CJobLogDialog*				mJobLogDialog;
	CGuildScoreLogDialog*		mGuildScoreLogDialog;
	CFamilyCropLogDialog*		mFamilyCropLogDialog;
	CFamilyPointLogDialog*		mFamilyPointLogDialog;
	//CUserCreateDialog*			mUserCreateDialog;
	//CUserRestoreDialog*			mUserRestoreDialog;
	CPermissionDialog*			mPermissionDialog;
	COperatorDialog*			mOperatorDialog;
	COperatorLogDialog*			mOperatorLogDialog;
	CNameLogDialog*				mNameLogDialog;


protected:
	BOOL			mIsWaiting;


	// ������ ����
public:
	const char*			GetItemName( DWORD itemIndex ) const;
	DWORD				GetItemIndex( const CString& ) const;
	const ITEM_INFO*	GetItemScript( DWORD itemIndex ) const;
	BOOL				IsDuplicateItem( DWORD itemIndex ) const;

	// ������ ��ġ�� ���� � ������ �κ��丮���� ��ȯ�Ѵ�
	const CString		GetItemPositionType( Item::Area, POSTYPE ) const;

	const CString GetLogText( eFamilyLog )		const;
	const CString GetLogText( eGuildLog )		const;
	const CString GetLogText( eLogSkill )		const;
	const CString GetLogText( eLogitemmoney )	const;
	const CString GetLogText( eQuestLog )		const;

	// �׸��带 �����ϴ� ��ƿ��Ƽ �Լ�
	static void CreateGrid( CWnd* parent, CGridCtrl*, UINT id, const CRect&, const char**,		BOOL autoHeight );
	static void CreateGrid( CWnd* parent, CGridCtrl*, UINT id, const CRect&, const CString*,	BOOL autoHeight );

	// ���� ���� �����ϴ� ��ƿ��Ƽ �Լ�
	static void SetCellValue( DWORD value, CGridCtrl&, GV_ITEM&, int state );
	static void SetCellValue( const char*, CGridCtrl&, GV_ITEM&, int state );

	// ��¥ �޺��� ���� �����ϴ� ��ƿ��Ƽ �Լ�
	static void SetDateCombo( CComboBox& yearCombo, CComboBox& monthCombo, CComboBox& dayCombo, BOOL isToday );

	// ���ϵ�ī�� ���ڸ� DB���� ��밡���ϰ� �ٲ��ش�
	static void ConvertWildCardForDb( char* text );

	// ����Ʈ ��Ʈ���� ������ ������ �����Ѵ�
	static void SaveToExcel( const CString& fileName, const CString& tableName, const CListCtrl& );

	// ���õ� �׸��� Ŭ�� ���忡 �����Ѵ�
	static void SaveToClipBoard( CListCtrl& );

	// ����Ʈ ��Ʈ�ѿ� ���� �̸��� �����Ѵ�
	static void SetServerListCtrl( CListCtrl& );

	// ���� ����� �����Ѵ�
	static void UpdateServerListCtrl( CListCtrl& );

	// ������ ����� �����Ѵ�
	void SetItemList( CListCtrl& ) const;
	void SetItemList( CListCtrl&, eItemCategory ) const;

	// ���ڿ��� �ش��ϴ� �ܾ ���Ե� ��� �������� �޺��� �����Ѵ�
	void SetItemCombo( CComboBox&, const CString& keyword ) const;

	// ��ȿ�� ���� ����� �����Ѵ�
	void SetServerComboBox( CComboBox& ) const;

	// ���� ����� �����Ѵ�
	void SetJobCombo( CComboBox&, DWORD jobType, DWORD race, DWORD jobLevel, DWORD jobIndex );

	// ��� ���� ����� �����Ѵ�
	void SetRankCombo( CComboBox& ) const;

	// ����� ���� ����� �����Ѵ�
	void SetUserLevelCombo( CComboBox& ) const;
 	
	// ������ ��ũ��Ʈ
protected:
	void LoadItemScript();	

	// Ű: ������ �ε���
	typedef std::map< DWORD, ITEM_INFO >	ItemMap;
	ItemMap									mItemMap;

	// ��æƮ ��ũ��Ʈ
public:
	// ��æƮ�� �ҿ�Ǵ� ��� �ε����� ��ũ��Ʈ�� �����´�
	const EnchantScript* GetEnchantScript( DWORD itemIndex ) const;

	// ��æƮ�Ǵ� ��� �������� �ʿ�� �ϴ� ��æƮ ������ ��ȣ�� ���´�
	DWORD				GetEnchantItemIndex( DWORD itemIndex ) const;

protected:
	void LoadEnchantScript();

	typedef stdext::hash_map< DWORD, EnchantScript >	EnchantScriptMap;
	EnchantScriptMap									mEnchantScriptMap;


	// ��ų ��ũ��Ʈ
public:
	typedef std::map< DWORD, DWORD >	SkillSizeMap;

	const SkillScript* GetSkillScript( DWORD skillIndex ) const;
	DWORD GetSkillSize( DWORD skillIndex ) const;
	const SkillSizeMap& GetSkillSizeMap() const;	
	const char* GetTip( DWORD index ) const;

protected:
	void LoadSkillScript();
	void LoadTip();
	
	// Ű: ��ų �ε���
	typedef stdext::hash_map< DWORD, SkillScript >	SkillScriptMap;
	SkillScriptMap									mSkillScriptMap;

	// Ű: ��ų �ε���, ��: ��ų �ִ� ����	
	SkillSizeMap						mSkillSizeMap;

	// Ű: ��ų �ε���
	typedef stdext::hash_map< DWORD, CString >	SkillTipMap;
	SkillTipMap									mSkillTipMap;


	// ����Ʈ ��ũ��Ʈ
public:
	const char*	GetQuestTitle( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	const char*	GetQuestDescription( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	const char*	GetQuestItemName( DWORD questItemIndex ) const;
	const char* GetQuestItemDescription( DWORD questItemIndex ) const;
	DWORD		GetMainQuestIndex( DWORD questItemIndex ) const;
	DWORD		GetSubQuestIndex( DWORD questItemIndex ) const;

protected:
	// ����Ʈ �������� �д´�
	void LoadQuestItemScript();

public:
	struct QuestItem
	{
		DWORD	mMainQuestIndex;
		DWORD	mSubQuestIndex;
		CString mName;
		CString mDescription;
	};

	// Ű: ����Ʈ ������ �ε���
	typedef stdext::hash_map< DWORD, QuestItem >	QuestItemMap;
	const QuestItemMap& GetQuestItemMap() const;

protected:
	QuestItemMap mQuestItemMap;

// ����Ʈ ���ڿ��� �д´�
	void LoadQuestStringScript();

public:
	struct QuestString
	{
		DWORD	mMainQuestIndex;
		DWORD	mSubQuestIndex;

		CString	mTitle;
		CString	mDescription;
	};

	// Ű: ��������Ʈ �ε����� ��������Ʈ �ε����� MAKELPPARAM���� ���յǾ� �ִ�
	typedef stdext::hash_map< DWORD, QuestString >	QuestStringMap;	
	const QuestStringMap& GetQuestStringMap() const;

protected:	
	QuestStringMap mQuestStringMap;


	// ����Ʈ ��ũ��Ʈ�� �д´�
public:
	const QuestScript* GetQuestScript( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	
protected:
	void LoadQuestScript();

	// Ű: ��������Ʈ �ε����� ��������Ʈ �ε����� MAKELPPARAM���� ���յǾ� �ִ�
	typedef stdext::hash_map< DWORD, QuestScript >	QuestScriptMap;
	QuestScriptMap									mQuestScriptMap;


	// �������̽� �޽���
public:
	const char* GetJobName( DWORD code ) const;
	const char* GetInterfaceMessage( DWORD Index ) const;

protected:
	void LoadInterfaceMessage();

	typedef stdext::hash_map< DWORD, CString >	InterfaceMessage;
	InterfaceMessage							mInterfaceMessage;

	
	// �ֹε���� ������ �д´�
public:
	typedef std::map< DWORD, CString >	LicenseFavoriteMap;
	typedef std::map< DWORD, CString >	LicenseAreaMap;
	typedef std::map< DWORD, CString >	LicenseAgeMap;

	const LicenseFavoriteMap&	GetLicenseFavoriteMap() const;
	const LicenseAreaMap&		GetLicenseAreaMap() const;
	const LicenseAgeMap&		GetLicenseAgeMap() const;

protected:
	void LoadLicenseScript();

	LicenseFavoriteMap							mLicenseFavoriteMap;
	LicenseAreaMap								mLicenseAreaMap;
	LicenseAgeMap								mLicenseAgeMap;


	// ���۹� �̸��� �д´�
public:
	const char* GetCropname( DWORD cropIndex, DWORD cropLevel ) const;

protected:
	void LoadFarmScript();
	// Ű: MAKELONG( ���۹� ����, ) 
	typedef std::map< DWORD, CString >	FarmCropMap;
	FarmCropMap							mFarmCropMap;

public:
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMenuNewuserDialog();
	afx_msg void OnMenuItemSearchDialog();
	afx_msg void OnMenuGuildDialog();
	afx_msg void OnMenuItemLogDialog();
	afx_msg void OnMenuFamliyDialog();
	afx_msg void OnMenuPermissionDialog();
	afx_msg void OnMenuOperatorDialog();
	afx_msg void OnMenuPasswordDialog();
	afx_msg void OnMenuOperatorLogDialog();
	afx_msg void OnMenuItemOptionLogDialog();


public:
	const CString GetText( eGM_POWER ) const;
	const CString GetText( RecoveryLog ) const;


	// 080304 LUJ, �α׸� ǥ��
public:
	void Log( const TCHAR*, ... );
};

extern CclientApp theApp;
