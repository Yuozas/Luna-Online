// client.h : client 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH에서 이 파일을 포함하기 전에 'stdafx.h'를 포함하십시오.
#endif

#include "resource.h"       // 주 기호


// CclientApp:
// 이 클래스의 구현에 대해서는 client.cpp을 참조하십시오.
//
#include "grid_control/GridCtrl.h"


// 스킬 스크립트에서 사용하는 자료구조가 MFC의 자료구조와 충돌하여 SkillInfo.h를 참조할 수 없다
// 따라서 기본적인 정보만 파싱해서 담기위한 구조체를 재정의했다.
struct SkillScript
{
	DWORD	mIndex;
	CString	mName;
	BOOL	mIsPassive;
	DWORD	mMoney;	// 학습에 필요한 돈
	DWORD	mPoint;	// 학습에 필요한 포인트
};

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


struct QuestScript
{
	DWORD	mMonsterIndex;
	DWORD	mMonsterHuntCount;

	// 키: 아이템 인덱스, 값: 필요 개수
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


// 재정의
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

	// 가능한 접속 개수
	size_t	GetConnectionSize() const;
	
protected:
	void SetWait( BOOL );


// 구현
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

	// 서버에서 메시지가 올때까지 대기한다
	
	
protected:
	// 동적 할당된 대화창을 담아두고, 일시에 제거한다
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


	// 아이템 정보
public:
	const char*			GetItemName( DWORD itemIndex ) const;
	DWORD				GetItemIndex( const CString& ) const;
	const ITEM_INFO*	GetItemScript( DWORD itemIndex ) const;
	BOOL				IsDuplicateItem( DWORD itemIndex ) const;

	// 아이템 위치에 따라 어떤 종류의 인벤토리인지 반환한다
	const CString		GetItemPositionType( Item::Area, POSTYPE ) const;

	const CString GetLogText( eFamilyLog )		const;
	const CString GetLogText( eGuildLog )		const;
	const CString GetLogText( eLogSkill )		const;
	const CString GetLogText( eLogitemmoney )	const;
	const CString GetLogText( eQuestLog )		const;

	// 그리드를 생성하는 유틸리티 함수
	static void CreateGrid( CWnd* parent, CGridCtrl*, UINT id, const CRect&, const char**,		BOOL autoHeight );
	static void CreateGrid( CWnd* parent, CGridCtrl*, UINT id, const CRect&, const CString*,	BOOL autoHeight );

	// 셀에 값을 세팅하는 유틸리티 함수
	static void SetCellValue( DWORD value, CGridCtrl&, GV_ITEM&, int state );
	static void SetCellValue( const char*, CGridCtrl&, GV_ITEM&, int state );

	// 날짜 콤보에 값을 세팅하는 유틸리티 함수
	static void SetDateCombo( CComboBox& yearCombo, CComboBox& monthCombo, CComboBox& dayCombo, BOOL isToday );

	// 와일드카드 문자를 DB에서 사용가능하게 바꿔준다
	static void ConvertWildCardForDb( char* text );

	// 리스트 컨트롤의 내용을 엑셀로 저장한다
	static void SaveToExcel( const CString& fileName, const CString& tableName, const CListCtrl& );

	// 선택된 항목을 클립 보드에 저장한다
	static void SaveToClipBoard( CListCtrl& );

	// 리스트 컨트롤에 서버 이름을 세팅한다
	static void SetServerListCtrl( CListCtrl& );

	// 서버 목록을 갱신한다
	static void UpdateServerListCtrl( CListCtrl& );

	// 아이템 목록을 세팅한다
	void SetItemList( CListCtrl& ) const;
	void SetItemList( CListCtrl&, eItemCategory ) const;

	// 문자열에 해당하는 단어가 포함된 모든 아이템을 콤보에 세팅한다
	void SetItemCombo( CComboBox&, const CString& keyword ) const;

	// 유효한 서버 목록을 세팅한다
	void SetServerComboBox( CComboBox& ) const;

	// 직업 목록을 세팅한다
	void SetJobCombo( CComboBox&, DWORD jobType, DWORD race, DWORD jobLevel, DWORD jobIndex );

	// 길드 직위 목록을 세팅한다
	void SetRankCombo( CComboBox& ) const;

	// 사용자 레벨 목록을 세팅한다
	void SetUserLevelCombo( CComboBox& ) const;
 	
	// 아이템 스크립트
protected:
	void LoadItemScript();	

	// 키: 아이템 인덱스
	typedef std::map< DWORD, ITEM_INFO >	ItemMap;
	ItemMap									mItemMap;

	// 인챈트 스크립트
public:
	// 인챈트에 소요되는 재료 인덱스로 스크립트를 가져온다
	const EnchantScript* GetEnchantScript( DWORD itemIndex ) const;

	// 인챈트되는 대상 아이템이 필요로 하는 인챈트 아이템 번호를 얻어온다
	DWORD				GetEnchantItemIndex( DWORD itemIndex ) const;

protected:
	void LoadEnchantScript();

	typedef stdext::hash_map< DWORD, EnchantScript >	EnchantScriptMap;
	EnchantScriptMap									mEnchantScriptMap;


	// 스킬 스크립트
public:
	typedef std::map< DWORD, DWORD >	SkillSizeMap;

	const SkillScript* GetSkillScript( DWORD skillIndex ) const;
	DWORD GetSkillSize( DWORD skillIndex ) const;
	const SkillSizeMap& GetSkillSizeMap() const;	
	const char* GetTip( DWORD index ) const;

protected:
	void LoadSkillScript();
	void LoadTip();
	
	// 키: 스킬 인덱스
	typedef stdext::hash_map< DWORD, SkillScript >	SkillScriptMap;
	SkillScriptMap									mSkillScriptMap;

	// 키: 스킬 인덱스, 값: 스킬 최대 레벨	
	SkillSizeMap						mSkillSizeMap;

	// 키: 스킬 인덱스
	typedef stdext::hash_map< DWORD, CString >	SkillTipMap;
	SkillTipMap									mSkillTipMap;


	// 퀘스트 스크립트
public:
	const char*	GetQuestTitle( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	const char*	GetQuestDescription( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	const char*	GetQuestItemName( DWORD questItemIndex ) const;
	const char* GetQuestItemDescription( DWORD questItemIndex ) const;
	DWORD		GetMainQuestIndex( DWORD questItemIndex ) const;
	DWORD		GetSubQuestIndex( DWORD questItemIndex ) const;

protected:
	// 퀘스트 아이템을 읽는다
	void LoadQuestItemScript();

public:
	struct QuestItem
	{
		DWORD	mMainQuestIndex;
		DWORD	mSubQuestIndex;
		CString mName;
		CString mDescription;
	};

	// 키: 퀘스트 아이템 인덱스
	typedef stdext::hash_map< DWORD, QuestItem >	QuestItemMap;
	const QuestItemMap& GetQuestItemMap() const;

protected:
	QuestItemMap mQuestItemMap;

// 퀘스트 문자열을 읽는다
	void LoadQuestStringScript();

public:
	struct QuestString
	{
		DWORD	mMainQuestIndex;
		DWORD	mSubQuestIndex;

		CString	mTitle;
		CString	mDescription;
	};

	// 키: 메인퀘스트 인덱스와 서브퀘스트 인덱스가 MAKELPPARAM으로 조합되어 있다
	typedef stdext::hash_map< DWORD, QuestString >	QuestStringMap;	
	const QuestStringMap& GetQuestStringMap() const;

protected:	
	QuestStringMap mQuestStringMap;


	// 퀘스트 스크립트를 읽는다
public:
	const QuestScript* GetQuestScript( DWORD mainQuestIndex, DWORD subQuestIndex ) const;
	
protected:
	void LoadQuestScript();

	// 키: 메인퀘스트 인덱스와 서브퀘스트 인덱스가 MAKELPPARAM으로 조합되어 있다
	typedef stdext::hash_map< DWORD, QuestScript >	QuestScriptMap;
	QuestScriptMap									mQuestScriptMap;


	// 인터페이스 메시지
public:
	const char* GetJobName( DWORD code ) const;
	const char* GetInterfaceMessage( DWORD Index ) const;

protected:
	void LoadInterfaceMessage();

	typedef stdext::hash_map< DWORD, CString >	InterfaceMessage;
	InterfaceMessage							mInterfaceMessage;

	
	// 주민등록증 정보를 읽는다
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


	// 농작물 이름을 읽는다
public:
	const char* GetCropname( DWORD cropIndex, DWORD cropLevel ) const;

protected:
	void LoadFarmScript();
	// 키: MAKELONG( 농작물 종류, ) 
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


	// 080304 LUJ, 로그를 표시
public:
	void Log( const TCHAR*, ... );
};

extern CclientApp theApp;
