// MonsterGuageDlg.h: interface for the CMonsterGuageDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
#define AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"

// 070109 LYW --- Include CPlayer's header file.
#include "Player.h"

// 070109 LYW --- Define max count for background image of popup menu.
#define MAX_POPUP_BGCOUNT	5
// 070109 LYW --- Define max count for menu of popup part.
#define MAX_MENU_COUNT		5

// 070123 LYW --- Define max count for face of target.
#define MAX_TARGET_FACE		3

class cStatic;
class cPushupButton;
class CObjectGuagen;
class CMonster;

enum MonsterGuageMode
{
	eMonGuage_Monster,
	eMonGuage_Character,
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	eMonGuage_Npc,
	eMonGuage_Pet,

	eMonGuage_Max
};

// 070109 LYW --- Add enum for popup menu type.
enum PopupMenu_Style
{
	e_STANDARD = 0x00, 
	e_INVITE, 
	e_EXPULSION,
} ;

enum PlayerState
{
	e_NONE = 0x00, 
	e_CAPTAIN, 
	e_MEMBER,
} ;

class CMonsterGuageDlg : public cDialog  
{
public:
	CMonsterGuageDlg();
	virtual ~CMonsterGuageDlg();
// 070123 lyw --- delete this function.
	/*
	void	setmonstername( char * szmonstername );
	*/
	void	SetMonsterLife( DWORD curVal, DWORD maxVal, int type=-1 );
	void	SetMonsterLife( CMonster * pMonster );
	// 070123 LYW --- Delete this function.
	/*
	void	SetGuildUnionName(char* GuildName, char* GuildUnionName);
	void	SetActiveMonGuageMode(int mode, BOOL bActive);
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	void	SetNpcName( char * szNpcName );
	*/

	void	Linking();
	// 070123 LYW --- Delete this function.
	//void	ShowMonsterGuageMode(int mode);
	virtual void Render();

	CMonster*	GetCurMonster() { return m_pCurMonster; }
	
	// Imsi
	void	SetObjectType( DWORD Type )		{	m_Type = Type;	}
	DWORD	GetObjectType()					{	return m_Type;	}

	// 070109 LYW --- Add function that is ActionEvent.
	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;

	// 070109 LYW --- Add functions to call popup part.
	// 070315 LYW --- MonsterGuageDlg : Modified function CheckTargetKind() ;
	//void CheckTargetKind() ;
	void CheckTargetKind(CObject* pObject) ;
	void CheckHeroAtt() ;
	void CheckTargetAtt( CPlayer* pPlayer ) ;

	void CloseTargetDlg() ;

	// 070123 LYW --- Delete this function.
	//void SetPopupMenu() ;
	//void RenderPopupMenu() ;

protected:

	// 070123 LYW --- Modified this part.
	/*
	cStatic * m_pName;
	cStatic * m_pLifeText;
	CObjectGuagen * m_pLifeGuage;
	cStatic * m_pGuildName;
	cStatic * m_pGuildUnionName;
	// 06. 01. 타겟팅창 인터페이스 변경 - 이영준
	cStatic * m_pNpcName;

	// 070109 LYW --- Add new controls.
	cButton*		m_pMenuBox ;
	cStatic*		m_pHPMark ;
	cStatic*		m_pCharLife ;
	CObjectGuagen*	m_pCharGuageLife ;
	*/
	
	cPtrList m_MonGuageCtlListArray[eMonGuage_Max];
	// 070123 LYW --- Delete this line.
	//int m_CurMode;
	CMonster * m_pCurMonster;				// 현재 표시중인 몬스터..
	DWORD	m_Type;							// ObjectType

	// 070123 LYW --- Delete this line.
	/*
	// 070109 LYW --- Add variable whether show popup menu or not.
	cStatic*	m_pST_BGImage[ MAX_POPUP_BGCOUNT ] ;
	cButton*	m_pBT_PopupMenu[ MAX_MENU_COUNT ] ;


	BOOL	m_bShowPopupMenu ;

	BYTE	m_byCurPopupMode ;

	BYTE	m_byTargetState ;
	BYTE	m_byHeroState ;
	*/

	/// PUBLIC PART.
	cStatic*		m_pHPMark ;

	BYTE			m_byCurMode ;

	BYTE	m_byCurPopupMode ;

	BYTE	m_byTargetState ;
	BYTE	m_byHeroState ;
    

	/// POPUP MENU PART.
	cButton*		m_pMenuCallBtn ;
	cButton*		m_pMenuBtn[ MAX_MENU_COUNT ] ;

	cStatic*		m_pMenuBG[ MAX_POPUP_BGCOUNT ] ;

	BOOL			m_bShowMenu ;


	/// MONSTER PART.
	cStatic*		m_pMonsterFace ;
	cStatic*		m_pMonsterName ;
	CObjectGuagen*	m_pMonsterGuage ;

	/// PLAYER PART.
	cStatic*		m_pPlayerFace ;
	cStatic*		m_pGuildName ;
	cStatic*		m_pUnionName ;
	cStatic*		m_pPlayerName ;
	cStatic*		m_playerLife ;
	cStatic*		m_pMasterName ;
	cStatic*		m_pPetName ;
	CObjectGuagen*	m_pPlayerGuage ;

	/// NPC PART.
	cStatic*		m_pNPCFace ;
	cStatic*		m_pNPCName ;
	cStatic*		m_pNPCOccupation ;

	BYTE			m_byCurObjectKind ;


public :
	void SetCurMode( BYTE mode ) { m_byCurMode = mode ; }
	BYTE GetCurMode() { return m_byCurMode ; }

	BYTE GetTargetState() { return m_byTargetState ; }

	/// FACE PART.
	void SetFace() ;

	/// NAME PART.
	void SetName( CObject* pObject, char* name ) ;

	void SetGuildName( char* guildName, char* unionName ) ;
	cStatic* GetGuildStatic() { return m_pGuildName ; }
	cStatic* GetUnionStatic() { return m_pUnionName ; }
	cStatic* GetMasterStatic() { return m_pMasterName ; }

	/// MENU PART.
	cButton* GetMenuButton() { return m_pMenuCallBtn ; }

	/// GUAGE PART.
	BOOL CheckPartyIdx( CPlayer* pPlayer ) ;
	// 070315 LYW --- MonsterGuageDlg : Modified function SetLife()
	//void SetLife( CMonster* pPlayer ) ;
	void SetLife( CPlayer* pPlayer ) ;
	void SetGuage( CObject* pObject ) ;

	cStatic* GetHPStatic() { return m_pHPMark ; }
	cStatic* GetLifeStatic() { return m_playerLife ; }

	CObjectGuagen* GetMonsterGuage() { return m_pMonsterGuage ; }
	CObjectGuagen* GetPlayerGuage() { return m_pPlayerGuage ; }

	/// POPUPMENU PART.
	void SetShowPopUpMenu( BOOL val ) { m_bShowMenu = val ; }
	void InitPopupMenu() ;

	/// MONSTER PART.
	cStatic* GetMonsterNameStatic() { return m_pMonsterName ; }

	void SetCurOBJKind( BYTE byKind ) { m_byCurObjectKind = byKind ; }
	BYTE GetCurOBJKind() { return m_byCurObjectKind ; }

	// 070418 LYW --- MonsterGuageDlg : Add fade out part.
private :
	cTimeDelay		m_FOTimer ;	// Fadeout timer.
public :
	BOOL IsStartTimer() ;

    void ResetDlg() ;			// Resetting alpha vlaue and timer.
	void StartFadeOut() ;		// Start timer.
	void FadeAlpha() ;			// Setting alpha.
};

#endif // !defined(AFX_MONSTERGUAGEDLG_H__3B4654AD_257E_42F7_9996_C2C8E89E535E__INCLUDED_)
