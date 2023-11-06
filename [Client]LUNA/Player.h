#pragma once

// 070228 LYW --- Player : Include header file of CTimeDelay class.
#include "../[cc]skill/client/delay/timedelay.h"

// 070312 LYW --- Player : Include game resource.
#include "Object.h"

// 070228 LYW --- Player : Define change time of face.
#define FACIAL_TIME		5000

// 070312 LYW --- Player : Define times of eye part.
#define EYE_LOOFTIME	5000
#define EYE_CLOSETIME	4700

struct MOD_LIST;

class cStreetStallTitleTip; // LBS 03.09.23

// desc_hseos_�йи�01
// S �йи� �߰� added by hseos 2007.07.17
class cImageSelf;
// E �йи� �߰� added by hseos 2007.07.17

struct SetScript;


class CPlayer : public CObject  
{
//////////////////////////////////////////////////////////////////////////////////////////
/// ���� �����

protected: 
	/// �ɸ��� �⺻����
	CHARACTER_TOTALINFO		m_CharacterInfo;	
	
	/// ĳ�� ������ ����
	SHOPITEMOPTION			m_ShopItemOption;	
	AVATARITEMOPTION		m_AvatarOption;

	/// ��ų �ɼ� ����
	SKILLSTATSOPTION		m_SkillStatsOption;
	SKILLSTATSOPTION		m_PassiveStatsOption;
	SKILLSTATSOPTION		m_ActiveStatsOption;
	SKILLSTATSOPTION		m_OnOffStatsOption;
	SKILLSTATSOPTION		m_BuffStatsOption;

	/// ������ Ÿ��Ʋ
	cStreetStallTitleTip*	m_pSSTitle;
	
	/// ��

	// 070228 LYW --- Player : Add facial part.
	cTimeDelay				m_FacialTime ;
	BOOL					m_bStartCheck ;

	// 070312 LYW --- Player : Add eye part.
	cTimeDelay				m_EyeTime ;
	BOOL					m_bActiveEye ;
	BOOL					m_bClosed ;

	// 070503 LYW --- Player : Check emotion kind.
	BOOL					m_bRunningEmotion ;

private:
	/// ���� ����	
	int						m_StandardMotion;
	int						m_Move_Start_Motion;
	int						m_Move_Ing_Motion;
	int						m_Move_End_Motion;

	int						m_ItemUseMotion;
	int						m_ItemUseEffect;

	CEngineObject*			m_pWingObject;

	/// ����� ����
	WANTEDTYPE				m_WantedIDX;

	// 070315 LYW --- Player : Add for test.
#ifdef _DEBUG
	BOOL					m_bFollow ;
	CObject*				m_pFollowTarget ;
#endif //_DEBUG

	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.07
	CEngineObject*			m_pFaceShapeObject;
	stFACEACTION			m_stFaceAction;
	// E �󱼱�ü �߰� added by hseos 2007.06.07

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.17
	cImageSelf*				m_pcsFamilyEmblem;
	// E �йи� �߰� added by hseos 2007.07.17

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.19
	DWORD					m_nFarmMotionTimeTick;
	DWORD					m_nFarmMotionTime;
	// E ����ý��� �߰� added by hseos 2007.09.19

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	UINT					m_nChallengeZoneEnterFreq;
	UINT					m_nChallengeZoneEnterBonusFreq;
	// E ����Ʈ �� �߰� added by hseos 2007.11.27

	// 071003 LYW --- Player : Add class index to member variable.
	int						m_nClassIndex ;

/// ���� �����
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// �Լ� �����

	/// ������/�Ҹ���
public:
	CPlayer();
	virtual					~CPlayer();

	/// �ʱ�ȭ
private:
	void					InitPlayer( CHARACTER_TOTALINFO* pTotalInfo );

public:
	/// ���� Ŭ���� �� �Լ�
	friend class			CObjectManager;
	friend class			CAppearanceManager;
	friend void				CS_BtnFuncEnter( LONG lId, void* p, DWORD we );

	///--------------------------------------------------------------------/
	/// �ɸ��� �⺻ ���� ���� �Լ�-----------------------------------------/

	/// �ɸ��� �⺻ ����
	void					GetCharacterTotalInfo( CHARACTER_TOTALINFO* pRtInfo );
	CHARACTER_TOTALINFO*	GetCharacterTotalInfo()	{ return &m_CharacterInfo; }

	/// ����
	virtual void			SetLevel(LEVELTYPE level) { m_CharacterInfo.Level = level; }
	LEVELTYPE				GetLevel() const { return m_CharacterInfo.Level; }
	
	/// ����
	BYTE					GetGender() { return m_CharacterInfo.Gender; }
	BYTE					GetRace() { return m_CharacterInfo.Race; }
	/// ����
	// 070415 LYW --- PLAYER : Active and modified functions to setting and return stage. 
	//virtual void			SetStage( BYTE Stage );
	// 070416 LYW --- Player : Modified function SetStage.
	virtual void			SetStage( BYTE grade, BYTE index ) ;
	BYTE					GetStage() { return m_CharacterInfo.JobGrade; }
	
	/// �ɸ��� ���� ����
	void					SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );

//---KES CHEAT CHANGESIZE
	void SetCharacterSize( float fSize );
//-----------------------
	///--------------------------------------------------------------------/
	/// �������ͽ�&���� ��ġ ���� ���� �Լ�--------------------------------/
	
	/// �����
	virtual DWORD			GetLife() { return m_CharacterInfo.Life; }
	virtual void			SetMaxLife(DWORD maxlife) { m_CharacterInfo.MaxLife = maxlife; }
	virtual void			SetLife( DWORD life, BYTE type = 1 );
	virtual DWORD			DoGetMaxLife();

	virtual float			DoGetMoveSpeed();

	BYTE					GetLifePercent();

	///--------------------------------------------------------------------/
	/// ����/�ൿ ���� �Լ�------------------------------------------------/

	/// ���
	virtual void			ClearGuildInfo();
	void					SetGuildIdx( DWORD GuildIdx );
	DWORD					GetGuildIdx() { return m_CharacterInfo.MunpaID; }
	void					SetGuildIdxRank(DWORD GuildIdx, BYTE Rank); 
	void					SetGuildMemberRank(BYTE rank) { m_CharacterInfo.PositionInMunpa = rank; } //���Ŀ����� �ڽ��� ���� ����
	BYTE					GetGuildMemberRank() { return m_CharacterInfo.PositionInMunpa; } //���Ŀ����� �ڽ��� ����
	void					SetGuildMarkName( MARKNAMETYPE MarkName );
	MARKNAMETYPE			GetGuildMarkName();
	void					SetGuildName( char* GuildName );
	char*					GetGuildName();
	void					SetNickName( char* pNickName );
	char*					GetNickName();

	// ���� ���
	void					SetGuildUnionIdx( DWORD dwUnionIdx ) { m_CharacterInfo.dwGuildUnionIdx = dwUnionIdx; }
	void					SetGuildUnionName( char* pName ) { strncpy( m_CharacterInfo.sGuildUnionName, pName, MAX_GUILD_NAME+1 ); }
	void					SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_CharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }
	DWORD					GetGuildUnionIdx() { return m_CharacterInfo.dwGuildUnionIdx; }
	char*					GetGuildUnionName() { return m_CharacterInfo.sGuildUnionName; }
	DWORD					GetGuildUnionMarkIdx() { return m_CharacterInfo.dwGuildUnionMarkIdx; }

	// �йи�
	virtual void			ClearFamilyInfo();
	void					SetFamilyIdx( DWORD FamilyIdx );
	DWORD					GetFamilyIdx() { return m_CharacterInfo.FamilyID; }
	void					SetFamilyIdxRank(DWORD FamilyIdx, BYTE Rank); 
	void					SetFamilyMemberRank(BYTE rank) { m_CharacterInfo.PositionInFamily = rank; } //���Ŀ����� �ڽ��� ���� ����
	BYTE					GetFamilyMemberRank() { return m_CharacterInfo.PositionInFamily; } //���Ŀ����� �ڽ��� ����
	void					SetFamilyMarkName( MARKNAMETYPE MarkName );
	MARKNAMETYPE			GetFamilyMarkName();
	void					SetFamilyName( char* FamilyName );
	char*					GetFamilyName();
	void					SetFamilyNickName( char* pNickName );
	char*					GetFamilyNickName();

	// 070801 ����, ���� �йи� �����ϸ� �ּ�ó��
	//void					SetFamilyUnionIdx( DWORD dwUnionIdx ) { m_CharacterInfo.dwFamilyUnionIdx = dwUnionIdx; }
	//void					SetFamilyUnionName( char* pName ) { strncpy( m_CharacterInfo.sFamilyUnionName, pName, MAX_FAMILY_NAME+1 ); }
	//void					SetFamilyUnionMarkIdx( DWORD dwMarkIdx )	{ m_CharacterInfo.dwFamilyUnionMarkIdx = dwMarkIdx; }
	//DWORD					GetFamilyUnionIdx() { return m_CharacterInfo.dwFamilyUnionIdx; }
	//char*					GetFamilyUnionName() { return m_CharacterInfo.sFamilyUnionName; }
	//DWORD					GetFamilyUnionMarkIdx() { return m_CharacterInfo.dwFamilyUnionMarkIdx; }

	/// ������
	void					SetSiegeName( DWORD NameType );
	void					SetRestraintMode( bool val ) { m_CharacterInfo.bRestraint = val; }
	BOOL					IsRestraintMode() { return m_CharacterInfo.bRestraint; }

	/// PK
	void					SetPKMode( BOOL bPKMode );
	BOOL					IsPKMode() { return m_CharacterInfo.bPKMode; }

	/// �Ǹ�ġ
	virtual void			SetBadFame(FAMETYPE val);
	FAMETYPE				GetBadFame() { return m_CharacterInfo.BadFame; }	
	
	/// �����
	WANTEDTYPE				GetWantedIdx() { return m_WantedIDX; }
	void					SetWantedIdx(WANTEDTYPE val) { m_WantedIDX = val; }

	/// ���� ����	
	virtual void			SetMotionInState( BYTE State );	

	// 070503 LYW --- Player : Add function to setting check emotion.
	void SetRunningEmotion(BOOL val) { m_bRunningEmotion  = val ; }

	// 070509 LYW --- Player : Add function to return to check emotion.
	BOOL IsRunningEmotion() { return m_bRunningEmotion ; }

	void					SetBaseMotion();
	int						GetStandardMotion() { return m_StandardMotion; }

	/// ����/��ȭ���
	void					SetPeaceMode( bool bPeace )	{ m_CharacterInfo.bPeace = bPeace; }

	/// �ൿ
	virtual BOOL			StartSocietyAct( int nStartMotion, int nIngMotion = -1, int nEndMotion = -1, BOOL bHideWeapon = FALSE );

	/// ����Ʈ
	BOOL					OnEndEffectProc( HEFFPROC hEff );

	/// ������ ��� ����
	void					SetItemUseMotionIdx( int Idx ) { m_ItemUseMotion = Idx;	}
	void					SetItemUseEffectIdx( int Idx ) { m_ItemUseEffect = Idx;	}

	///--------------------------------------------------------------------/
	/// ���μ��� ���� �Լ�-------------------------------------------------/

	/// ���μ���
	void					Process();
	
	/// �⺻ ���� ���μ���
	virtual void			OnStartObjectState( BYTE State );
	virtual void			OnEndObjectState( BYTE State );

	virtual void			Die( CObject* pAttacker, BOOL bFatalDamage, BOOL bCritical, BOOL bDecisive );
	virtual void			Damage( CObject* pAttacker, BYTE DamageKind, DWORD Damage, BOOL bCritical, BOOL bDecisive );
	virtual void			Heal( CObject* pHealer, BYTE HealKind, DWORD HealVal );
	virtual void			Recharge( CObject* pHealer, BYTE RechargeKind, DWORD RechargeVal );


	///--------------------------------------------------------------------/
	/// ������ ���� ���� �Լ�----------------------------------------------/

	/// ������ ����
	void					SetWearedItemIdx( DWORD WearedPosition, DWORD ItemIdx );
	DWORD					GetWearedItemIdx( DWORD WearedPosition ) { return m_CharacterInfo.WearedItemIdx[WearedPosition]; }
	WORD					GetWeaponEquipType();
	// 080703 LUJ, ����� ǥ���� �� �ִ� �ִϸ��̼� Ÿ���� ��ȯ�Ѵ�
	eWeaponAnimationType	GetWeaponAniType();
	
	/// ������
	void					ShowStreetStallTitle( BOOL bShow, char* strTitle, DWORD dwFontColor = RGB_HALF(10,10,10) );
	void					SetStreetStallTitle( char* strTitle );
	void					ShowStreetBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwFontColor = RGB_HALF(10,10,10) );
	void					SetStreetBuyStallTitle( char* strTitle );

//---KES �����˻� 2008.3.11
	void					RefreshStreetStallTitle();
//---------------

	/// ĳ�þ�����
	void					SetAvatarInfo( WORD* Avatar ) { memcpy( m_ShopItemOption.Avatar, Avatar, sizeof(WORD)*eAvatar_Max); }
	void					SetShopItemOptionInfo(SHOPITEMOPTION* pInfo);
	SHOPITEMOPTION*			GetShopItemStats() { return &m_ShopItemOption; }
	AVATARITEMOPTION*		GetAvatarOption() { return &m_AvatarOption; }
	void					CalcAvatarOption();


	///--------------------------------------------------------------------/
	/// ��ų ���� ���� �Լ�------------------------------------------------/

	/// ��ų ȿ��
	SKILLSTATSOPTION*		GetSkillStatsOption() { return &m_SkillStatsOption; }
	SKILLSTATSOPTION*		GetPassiveStatsOption() { return &m_PassiveStatsOption; }
	SKILLSTATSOPTION*		GetActiveStatsOption() { return &m_ActiveStatsOption; }
	SKILLSTATSOPTION*		GetOnOffStatsOption() { return &m_OnOffStatsOption; }
	SKILLSTATSOPTION*		GetBuffStatsOption() { return &m_BuffStatsOption; }

	void					AddPassiveStatsOption( SKILLOPTION *pSkillOption );
	void					AddActiveStatsOption( SKILLOPTION *pSkillOption );
	void					AddOnOffStatsOption( SKILLOPTION *pSkillOption );
	void					AddBuffStatsOption( SKILLOPTION *pSkillOption );
	void					RemovePassiveStatsOption( SKILLOPTION *pSkillOption );
	void					RemoveActiveStatsOption( SKILLOPTION *pSkillOption );
	void					RemoveOnOffStatsOption( SKILLOPTION *pSkillOption );
	void					RemoveBuffStatsOption( SKILLOPTION *pSkillOption );

	// 070228 LYW --- Player : Add function for facial part.
	cTimeDelay*	GetFacialTime() { return &m_FacialTime ;}

	void ChangeFace( BYTE faceNum ) ;
	void ReturnFace() ;

	// 070307 LYW --- Player : Add function for emoticon part.
	void PlayEmoticon_Syn(BYTE EmoticonNum) ;
	void PlayEmoticon(int nEmoticonNum) ;

	// 070312 LYW --- Player : Add function for eye part.
	void CloseEye() ;
	void InitializeEye() ;

	// 070312 LYW --- Player : Add function to return MOD_LIST.
	MOD_LIST* GetModList() ;

	// 070313 LYW --- Player : Add function to return time of eye.
	cTimeDelay* GetEyeTime() { return &m_EyeTime ; }

	// 070315 LYW --- Player : Add follow part for test.
#ifdef _DEBUG
	void SetFollow( BOOL val ) { m_bFollow = val ; }
	BOOL IsFollow() { return m_bFollow ; }

	void SetFollowTarget( CObject* pTarget ) { m_pFollowTarget = pTarget ; }
#endif //_DEBUG

	// desc_hseos_�ֹε��01
	// S �ֹε�� �߰� added by hseos 2007.06.06
	void				SetResidentRegistInfo(DATE_MATCHING_INFO* pInfo)		{ m_CharacterInfo.DateMatching = *pInfo; }
	// E �ֹε�� �߰� added by hseos 2007.06.06

	// desc_hseos_�󱼱�ü01
	// S �󱼱�ü �߰� added by hseos 2007.06.07
	void				SetFaceObject(CEngineObject* pObject)					{ m_pFaceShapeObject = pObject; }	
	CEngineObject*		GetFaceObject()											{ return m_pFaceShapeObject ; }	
	void				SetFaceAction(int nAction, int nShape);	
	void				SetFaceShape(int nShape)								{ if(m_pFaceShapeObject)m_pFaceShapeObject->SetMaterialIndex(nShape); }
	void				ProcessFaceAction();
	// E �󱼱�ü �߰� added by hseos 2007.06.07

	// desc_hseos_�йи�01
	// S �йи� �߰� added by hseos 2007.07.17
	cImageSelf*			GetFamilyEmblem()										{ return m_pcsFamilyEmblem; }
	void				SetFamilyEmblem(cImageSelf*	pcsEmblem)					{ m_pcsFamilyEmblem = pcsEmblem; }
	
	virtual void		Release();
	// E �йи� �߰� added by hseos 2007.07.17

	// desc_hseos_����ý���_01
	// S ����ý��� �߰� added by hseos 2007.09.19
	void				StartFarmMotion(int nMotionNum, DWORD nMotionTime);
	void				ProcFarmMotion();
	// E ����ý��� �߰� added by hseos 2007.09.19

	// desc_hseos_����Ʈ ��_01
	// S ����Ʈ �� �߰� added by hseos 2007.11.27
	UINT				GetChallengeZoneEnterFreq()								{ return m_nChallengeZoneEnterFreq; }
	UINT				GetChallengeZoneEnterBonusFreq()						{ return m_nChallengeZoneEnterBonusFreq; }
	void				SetChallengeZoneEnterFreq(UINT nFreq)					{ m_nChallengeZoneEnterFreq = nFreq; }
	void				SetChallengeZoneEnterBonusFreq(UINT nFreq)				{ m_nChallengeZoneEnterBonusFreq = nFreq; }
	// E ����Ʈ �� �߰� added by hseos 2007.11.27

	// desc_hseos_��ȥ_01
	// S ��ȥ �߰� added by hseos 2007.12.13
	char*				GetMarriagePartnerName()								{ return m_CharacterInfo.szMarriagePartnerName; }
	char*				GetMarriagePartnerNickName()							{ return m_CharacterInfo.szMarriagePartnerNickName; }
	void				SetMarriagePartnerName(char* pszName)					{ SafeStrCpy(m_CharacterInfo.szMarriagePartnerName,		pszName,	 MAX_NAME_LENGTH+1); }
	void				SetMarriagePartnerNickName(char* pszNickname)			{ SafeStrCpy(m_CharacterInfo.szMarriagePartnerNickName, pszNickname, MAX_NAME_LENGTH+1); }
	// E ��ȥ �߰� added by hseos 2007.12.13

	// ����, ��Ī ����
public:
	// �Էµ� �÷��̾�� ��Ī�Ǵ��� �˻��Ѵ�. 
	BOOL CheckHeart( CPlayer* );
	const DATE_MATCHING_INFO& GetDateMatchingInfo() const;
	void SetHeart( BOOL );

private:
	// ����ο� ��Ī�Ǿ����� ����
	//BOOL m_IsMatch;

protected:
	// ��Ʈ ����Ʈ�� �׸��� �ִ��� ����
	BOOL m_IsDrawHeart;
	

	// ����, 070705 ��Ʈ ������ �ɷ� ����
public:
	typedef std::map< const SetScript*, int >	SetItemLevel;

	const SetItemLevel&	GetSetItemLevel() const;
	SetItemLevel&		GetSetItemLevel();

private:
	SetItemLevel			mSetItemLevel;

public:
	void SetWingObject( CEngineObject* pObj ) { m_pWingObject = pObj; }
	void SetWingObjectMotion( BYTE idx )
	{
		if( m_pWingObject )
		{
			m_pWingObject->ChangeMotion( idx, 0 );
		}
	}

	void SetWingObjectBaseMotion( BYTE idx )
	{
		if( m_pWingObject )
		{
			m_pWingObject->ChangeBaseMotion( idx );
		}
	}

	// 0701003 LYW --- Player : Add function to setting and return class index.
	void SettingClassIndex() ;
	int GetClassIndex() { return m_nClassIndex ; }

	DWORD mWeaponEffect[3];
};
