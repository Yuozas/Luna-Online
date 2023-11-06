#pragma once


#include "GameState.h"
#include "cImageSelf.h"
#include "CharMakeCamera.h"


#define CHARMAKE USINGTON(CCharMake)


class CCharMakeNewDlg ;
class CPlayer;
class cStatic;

// 061218 LYW --- Add enum for scene.
class CCharMake : public CGameState  
{
	enum ENUM_CM_SCENE
	{
		e_SCENE_IN = 0,
		e_SCENE_CHANGING, 
		e_SCENE_OUT,
	} ;

	//trustpak 2005/03/19
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	CCharMakeCamera*	m_pCharMakeCamera;

	BOOL m_bEnterGame;

	//
	// 061217 LYW --- Delete this code.
	//cCharMakeDlg*	m_pCharMakeDlg;

	// 061214 LYW --- CharMake : Define pointer variable to access from this class.
	CCharMakeNewDlg*	m_pCharMakeNewDlg ;

	// Dialog Pointer

	friend class cWindowManager;
	/*
	friend void CM_GengoalSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_SimmakSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DexSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_StaSpinBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DuplCheckEditFunc(LONG lId, void * p, DWORD we);
	friend void CM_HeadChangeLBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_HeadChangeRBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FaceChangeLBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FaceChangeRBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_MuInBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_GwunSaBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_DoJukBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_GungSaBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_SangInBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_EuWonBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_OkBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_MaleBtnFunc(LONG lId, void * p, DWORD we);
	friend void CM_FemaleBtnFunc(LONG lId, void * p, DWORD we);
	*/
	// 061217 LYW --- Delete this code.
	//friend extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
	//friend extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
	friend extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
	friend extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
	// 061217 LYW --- Delete this code.
	//friend extern void  CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);

	// 070208 LYW --- CharMake : Add variables for position and scale of image.
	VECTOR2		m_v2TopPos ;
	VECTOR2		m_v2BottomPos ;
	VECTOR2		m_scale ;

public:
	CCharMake();
	virtual ~CCharMake();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	// 061217 LYW --- Delete this code.
	// LBS 03.10.29
	/*
	void SetCharMakeDialog(cCharMakeDlg* pDlg) { m_pCharMakeDlg = pDlg; }
	cCharMakeDlg* GetCharMakeDialog() { return m_pCharMakeDlg; }
	*/

	// 061214 LYW --- CharMake : Add function to return dialog for creation character.
	void SetCharMakeNewDlg( CCharMakeNewDlg* pDlg ) { m_pCharMakeNewDlg = pDlg ; }
	CCharMakeNewDlg* GetCharMakeNewDlg() { return m_pCharMakeNewDlg ; }


	void DisplayNotice( int MsgNum );
	void SetDisableDlg( BOOL val );

	// 061217 LYW --- Add for test.
	VECTOR3 m_v3Eye ;
	VECTOR3 m_v3Rotate ;
	VECTOR3 m_vCharPos ;
	FLOAT	m_fCharAngle ;
	// end test.

	// 061218 LYW --- Add vector for camera.
	VECTOR3 m_v3Eye_Elf ;
	VECTOR3 m_v3Rotate_Elf ;

	VECTOR3 m_v3Eye_Human ;
	VECTOR3 m_v3Rotate_Human ;

	VECTOR3 m_vElfPos ;
	VECTOR3 m_vHumanPos ;

	FLOAT   m_fElfAngle ;
	FLOAT   m_fHumanAngle ;

	cStatic* m_pAlphaScreen ;
	IDISpriteObject* m_pSprite ;

	// 061218 LYW --- Add variable for change scene.
	// 070522 LYW --- Delete change scene.
	//BOOL m_bChangeScene ;

	// 070522 LYW --- Delete change scene.
	/*
	void SetChangeScene( BOOL val ) { m_bChangeScene = val ; }
	void SetChangeMode( BYTE mode ) { m_byChangeSceneMode = mode ; }
	void ChangeScene( BYTE mode ) ;
	*/
	void InitializeAlphaScreen() ;

	// 061218 LYW --- Add variable have order.
	BYTE m_byOrder ;
	// 070522 LYW --- Delete change scene.
	//BYTE m_byChangeSceneMode ;
	DWORD m_dwAlphaCount ;
	
//	void SetComboList(CharMakeCombo* pList);
	// 070205 LYW --- Add functions to process network msg.
public :
	void UserConn_Character_NameCheck_Nack( void* pMsg ) ;
	void UserConn_Character_NameCheck_Ack() ;
	void UserConn_Character_Make_Nack() ;
	void UserConn_Character_Make_Ack( void* pMsg ) ;
	void UserConn_CharacterList_Ack( void* pMsg ) ;
	void Cheat_EventNotify_On( void* pMsg ) ;
	void Cheat_EventNotify_Off( void* pMsg ) ;

	// 070208 LYW --- CharMake : Add function to setting position and scale of image.
	void SetPositionScale() ;


	// 070918 웅주, 마우스를 드래그하여 캐릭터를 회전시킴
private:
	void ProcessDragRotation();
/*
	struct
	{
		float mRadian;		// 회전시킬 값
		float mSpeed;		// 회전 속도
	}
	mRotation;

	struct
	{
		int		mX;
		int		mY;
		DWORD	mIsDrag;
	}
	mPosition;
*/
};


EXTERNGLOBALTON(CCharMake)
