#ifndef _CHARACTERMAKE_MANAGER_
#define _CHARACTERMAKE_MANAGER_

#include "MHfile.h"

class cStatic;

enum CHAR_M
{
//	CE_JOB,
	CE_SEX, 
	CE_MHAIR, 
	CE_WMHAIR, 
	CE_MFACE, 
	CE_WMFACE, 
	CE_WEAR, 
	CE_BOOT, 
	CE_WEAPON,
	CE_AREA,
	CE_MAX
};

// 061215 LYW --- Add enum for new make list.
enum CHARMAKE_LIST
{
	CM_MFACE, 
	CM_MHAIR, 
	CM_WMFACE, 
	CM_WMHAIR,
	CM_EMFACE,
	CM_EMHAIR,
	CM_EWFACE,
	CM_EWHAIR,
	CM_MAX
} ;

enum{CM_PREV, CM_NEXT};

struct CM_OPTION
{
	char		strName[20];
	DWORD		dwData;
	DWORD		dwHelperData;
//	void*		pDescCombo;
	cStatic*	pDescStatic;
	VECTOR2		vTextPos;
};


#define CHARMAKEMGR 	USINGTON(cCharMakeManager)

#define PATH_UOPTION		"./System/Resource/CharMake_SelectOption.bin"

// 061214 LYW ---- Add new information dialog for create character.
#define PATH_NOPTION		"./System/Resource/CharMake_List.bin"
// 061214 LYW ---- End add.

#define	CHARROTATE_RATE		0.01f
enum ENUM_CM_CLASS;
class CPlayer;
class cGuageBar;
class cStatic;

// 061215 LYW --- Include interface.
class CCharMakeNewDlg ;

class cCharMakeManager
{
protected:
	CPlayer*				m_pNewPlayer;					// »õ·Î¿î Ä³¸¯ÅÍ
	CHARACTERMAKEINFO		m_CharMakeInfo;

	cPtrList				m_UserSelectOption[CE_MAX];		// »ç¿ë?Ú°¡ ¼±ÅÃ°¡´É ¿É¼Ç ¸ñ·Ï

	DWORD					m_dwCurIndex[CE_MAX];			// ����Ʈ ������ Idx// SW050812 ������ ���õ� �ɼ�

	FLOAT					m_fAngle;
	FLOAT					m_fOldX;
	
	BOOL					m_bInit;

	cGuageBar*				m_pHeight;
	cGuageBar*				m_pWidth;

	CCharMakeNewDlg*		m_pCharMakeNewDlg ;
public:

	cCharMakeManager();
	virtual ~cCharMakeManager();

	void Init();	// ÃÊ±âÈ­
	void Release();

	void CreateNewCharacter(VECTOR3* pv3Pos);
	
	// 070523 LYW --- CharMakeManager : Modified function ReplaceCharMakeInfo.
	//BOOL ReplaceCharMakeInfo( int idx, WORD wValue );
	void ReplaceCharMakeInfo();
	void ComboBoxResetAll();
	void ListBoxResetAll();

	//SW050812
	// 070523 LYW --- CharMakeManager : Delete function that is not use.
	//void RotateSelection( int idx, int Dir );

	CPlayer* GetNewPlayer() { return m_pNewPlayer; }

	CHARACTERMAKEINFO* GetCharacterMakeInfo() { return &m_CharMakeInfo; }
	DWORD GetWeaponIndex( ENUM_CM_CLASS ) const;
	DWORD GetShieldIndex( ENUM_CM_CLASS ) const;
	DWORD GetDressIndex( ENUM_CM_CLASS ) const;
	DWORD GetCDressIndex( ENUM_CM_CLASS ) const;
	DWORD GetCGloveIndex( ENUM_CM_CLASS ) const;
	DWORD GetCShoesIndex( ENUM_CM_CLASS ) const;
	DWORD GetWingIndex( ENUM_CM_CLASS ) const;

	void NewCharacterRotate( int x );
	
	void ApplyScale();

	// 061216 LYW --- Add function to return CharMakeDlg.
	CCharMakeNewDlg* GetCharMakeNewDlg() { return m_pCharMakeNewDlg ; }

	// 061214 LYW ---- Add function to load option list.
	void LoadOptionList() ;
	// 061214 LYW ---- End add.

	// 061215 LYW --- Add fucntion to setting selection value.
	void SetOptionList( int curStatic, int curIdx ) ;

	// desc_hseos_��������01
	// S �������� �߰� added by hseos 2007.06.16
	void SetCharSexKindFromDB(int nSexKind)	{ m_CharMakeInfo.SexType = nSexKind; }
	// E �������� �߰� added by hseos 2007.06.16

	cPtrList* GetOptionList(int idx) ;
private:
	void LoadUserSelectOption();		// È­?Ï?» ?Ð¾î¿Â´Ù.
//	void SetUserSelectOption();			// ¸®½ºÆ®¿¡ ¼ÂÆÃÇÑ´Ù.
	// 070523 LYW --- CharMakeManager : Delete function that is not use.
	//void SetUserSelections();
};
EXTERNGLOBALTON(cCharMakeManager)
#endif // _CHARACTERMAKE_MANAGER_
