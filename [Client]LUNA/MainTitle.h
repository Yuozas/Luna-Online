// MainTitle.h: interface for the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_)
#define AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _UI_SERVERSET_

#include "GameState.h"
#include "cImageSelf.h"
#include "Monster.h"
#include "cImage.h"

#define TITLE USINGTON(CMainTitle)

class CEngineMap;
class CEngineCamera;
class cDialog;
class CServerListDialog;

#define _MAX_MAINTITLE_MONSTER_NUM 3


class CMainTitle : public CGameState  
{
	CEngineCamera*	m_pCamera;

	BOOL	m_bDisconntinToDist;
	DWORD	m_DistAuthKey;
	DWORD	m_UserIdx;
		
//KES 031107
	cDialog*	m_pLogoWindow;
	DWORD		m_dwStartTime;
	BOOL		m_bInit;
#ifdef TAIWAN_LOCAL
	cDialog*	m_pAdvice;
#endif
// JSD
	CServerListDialog*	m_pServerListDlg;
	char				m_DistributeAddr[16];
	WORD				m_DistributePort;
	BOOL				m_bServerList;

//KES
	DWORD		m_dwDiconWaitTime; // for disconnect from Distribute;
	DWORD		m_dwWaitTime;	//for agent connection
	BOOL		m_bWaitConnectToAgent;
	BOOL		m_bNoDiconMsg;

	DWORD		m_ConnectionServerNo;

	// 070208 LYW --- MainTitle : Add variables for position and scale of image.
	VECTOR2		m_v2TopPos ;
	VECTOR2		m_v2BottomPos ;
	VECTOR2		m_scale ;
	std::string mLoginKey;

public:
	CMainTitle();
	virtual ~CMainTitle();
	BOOL	Init(void* pInitParam);
	void	Release(CGameState* pNextGameState);

	void	Process();
	void	BeforeRender();
	void	AfterRender();	
	void	NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	DWORD	GetDistAuthKey()		{ return m_DistAuthKey;	}
	DWORD	GetUserIdx()			{ return m_UserIdx; }
	void	OnLoginError(DWORD errorcode,DWORD dwParam);
	void	OnDisconnect();
	
	// JSD
	void	ConnectToServer( int index );
	void	ShowServerList();
	void	SetServerList()		{ m_bServerList = TRUE; }

	CServerListDialog*	GetServerListDialog()	{ return m_pServerListDlg; }
	void SetLoginKey(LPCTSTR loginKey) { mLoginKey = loginKey; }
	const std::string& GetLoginKey() const { return mLoginKey; }
	void	StartWaitConnectToAgent( BOOL bStart );
	void	NoDisconMsg()			{ m_bNoDiconMsg = TRUE; }
	
	BOOL	m_bDynamicDlg;

	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;

#ifdef _UI_SERVERSET_
	cImage		m_ServerSetDesc;
#endif

	// 070208 LYW --- MainTitle : Add function to setting position and scale of image.
	void SetPositionScale() ;

	//trustpak 2005/04/01
private:	
	
#ifndef TAIWAN_LOCAL
	void	InitMainTitleMonsters(void);	
	void	RemoveMainTitleMonsters(void);
	void	ProcessMainTitleMonsters(void);

	CMonster*	m_apMonster[_MAX_MAINTITLE_MONSTER_NUM];
	VECTOR3		m_av3Target[_MAX_MAINTITLE_MONSTER_NUM];	
	VECTOR3		m_av3LastPosition[_MAX_MAINTITLE_MONSTER_NUM];
	DWORD		m_adwLastActionTime[_MAX_MAINTITLE_MONSTER_NUM];
	INT			m_aiLastAction[_MAX_MAINTITLE_MONSTER_NUM];

	RECT	m_rcMainTitleMonsterArea;

#endif


///


	
// 070205 LYW --- Add functions to process network msg.
public:
	/// NETWORK PARSER PART.
	void Cheat_CheckIp_Nack() ;
	void UserConn_Use_Dynamic_Ack( void* pMsg ) ;
	void UserConn_Dist_ConnectSuccess( void* pMsg ) ;
	void UserConn_Login_Ack( void* pMsg ) ;
	void UserConn_Login_Nack( void* pMsg ) ;
	void UserConn_Server_NotReady() ;
	void UserConn_CharacterList_Ack( void* pMsg ) ;
	void UserConn_CharacterList_Nack() ;
	void UserConn_RemainTime_Notify( void* pMsg ) ;
	void UserConn_Agent_ConnectSuccess( void* pMsg ) ;
	void Cheat_EventNotify_On( void* pMsg ) ;
	void Cheat_EventNotify_Off( void* pMsg ) ;
	// desc_hseos_성별선택01
	// S 성별선택 추가 added by hseos 2007.06.16
	void UserConn_Character_SexKind(void *pMsg);
	// E 성별선택 추가 added by hseos 2007.06.16

	/// ON LOGIN ERROR PART.
	void Login_Error_Invalid_Version() ;
	void Login_Error_OverLappedLogin() ;
	void Login_Error_OverLappedLoginiNotherServer( DWORD dwParam ) ;
	void Login_Error_NoAgentServer() ;
	void Login_Error_NoDistributeServer() ;
	void Login_Error_InvalidUserLevel() ;
	void Login_Error_DistServerIsBusy() ;
	void Login_Error_WrongIdPw() ;
	void Login_Error_BlockUserLevel( DWORD dwRemainTime ) ;
	void Login_Error_Invalid_Ip() ;
	void Login_Error_DistConnet_Error() ;
	void Login_Error_MaxUser() ;
	void Login_Error_Minor_InadultServer() ;
	void Login_Error_SecededAccount() ;
	void Login_Error_Not_CloseBetaWinner() ;
	void Login_Error_NoreMainTime() ;
	void Login_Error_NoIpRegen() ;

	// 080111 LYW --- MainTitle : 보안 비밀번호 관련 에러 메시지 출력.
	void Login_Error_Invalid_Sec_Pass() ;
	void Login_Error_Change_Sec_Pass() ;
	void Login_Error_File_CRC();
	void Login_Error_LoginSession_Invalid();
};

EXTERNGLOBALTON(CMainTitle)

#endif // !defined(AFX_MAINTITLE_H__82C6A42B_0EEA_4F49_BA0C_33F88B3FFEF3__INCLUDED_)
