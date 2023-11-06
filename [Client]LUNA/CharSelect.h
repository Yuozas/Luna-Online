// CharSelect.h: interface for the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)
#define AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "ChannelDialog.h"
#include "cImageSelf.h"
#define CHARSELECT USINGTON(CCharSelect)

// 061217 LYW --- Add for test.
#include "../4DyuchiGRX_common/math.inl"
// test end.

class CPlayer;
class cDialog;
class CProfileDlg ;

// 061218 LYW --- Add Certificate interface
class CCertificateDlg ;



class CCharSelect : public CGameState  
{
	int m_CurSelectedPlayer;
	CPlayer* m_pPlayer[MAX_CHARACTER_NUM];

//	BOOL m_bDoublcClicked;
	CPlayer* m_pDoublcClickedPlayer;

	void InitCameraInCharSelect();

	void SetCurSelctedPlayer(int num);

	void PlaySelectedMotion(int num);
	void PlayDeselectedMotion(int num);

	BOOL m_bDiablePick;
	// 061215 LYW --- Delete this code for a moment.
	//cDialog* m_pSelectDlg;
	CChannelDialog* m_pChannelDlg;
	
//KES
	BOOL m_bBackToMainTitle;
	
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

	DWORD	m_ExtraCharacterCount;
	
	BOOL m_bEnterGame;

	// 061215 LYW --- Add profile dialog.
	CProfileDlg*		m_pProfile ;

	// 061218 LYW --- Add certificate dialog.
	CCertificateDlg*	m_pCertificateDlg ;

	// 070208 LYW --- CharSelect : Add variables for position and scale of image.
	VECTOR2		m_v2TopPos ;
	VECTOR2		m_v2BottomPos ;
	VECTOR2		m_scale ;

	// 070313 LYW --- CharSelect : Add array for random of character for close eye.
	int			m_nRandom[5] ;

	int m_MaxChannel[100];

public:	

	void SetMaxChannel( int set, int num ) { m_MaxChannel[set] = num; }
	int GetMaxChannel( int set ) { return m_MaxChannel[set]; }

	CCharSelect();
	virtual ~CCharSelect();
	
	////MAKESINGLETON(CCharSelect)

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	void SelectPlayer(int num);	// 0 <= num  <= 3  서있는 자리 위치번호
	CPlayer* GetCurSelectedPlayer();
	int GetCurSelectedPlayerNum() { return m_CurSelectedPlayer; }

	BOOL IsFull();
	
	void SendMsgGetChannelInfo();
	BOOL EnterGame();
	void DeleteCharacter();

	void DisplayNotice(int MsgNum);
	
	void SetDisablePick( BOOL val );
	void BackToMainTitle();

	void OnDisconnect();
	
	CChannelDialog* GetChannelDialog() { return m_pChannelDlg; }

	void SetExtraCharacterSlot( DWORD Count )			{	m_ExtraCharacterCount = Count;		}
	DWORD GetExtraCharacterSlot()						{	return m_ExtraCharacterCount;		}

	// 061215 LYW --- Add function to return and setting profile dialog.
	void SetProfileDlg( CProfileDlg* pDlg ) { m_pProfile = pDlg ; }
	CProfileDlg* GetProfileDlg() { return m_pProfile ; }

	// 061218 LYW --- Add function to return and setting certificate dialog.
	void SetCertificateDlg( CCertificateDlg* pDlg ) { m_pCertificateDlg = pDlg ; }
	CCertificateDlg* GetCertificateDlg() { return m_pCertificateDlg ; }

	// 061217 LYW --- Add for test.
	VECTOR3 m_v3Eye ;
	VECTOR3 m_v3Rotate ;
	// end test.

	// 070205 LYW --- Add functions to process network msg.
public :
	void UserConn_RemainTime_Notify( void* pMsg ) ;
	void UserConn_CharacterList_Ack( void* pMsg ) ;
	void UserConn_Character_Remove_Ack() ;
	void UserConn_Character_Remove_Nack( void* pMsg ) ;
	void UserConn_DisConnect_Ack() ;
	void UserConn_ChannelInfo_Ack( void* pMsg ) ;
	void UserConn_ChannelInfo_Nack( void* pMsg ) ;
	void Cheat_EventNotify_On( void* pMsg ) ;
	void Cheat_EventNotify_Off( void* pMsg ) ;
	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.06.17
	void UserConn_Character_SexKind(void *pMsg);
	// E 성별선택 추가 added by hseos 2007.06.17

	// 070208 LYW --- CharSelect : Add function to setting position and scale of image.
	void SetPositionScale() ;
};
EXTERNGLOBALTON(CCharSelect)
#endif // !defined(AFX_CHARSELECT_H__DE3122B4_F68B_4CDE_8F3E_C718A7BBCDA8__INCLUDED_)
