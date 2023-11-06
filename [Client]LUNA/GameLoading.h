// GameLoading.h: interface for the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_)
#define AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "cImageSelf.h"
#include "cImage.h"

#define GAMELOADING USINGTON(CGameLoading)


class CLoadingDlg;

class CGameLoading : public CGameState  
{					
	HANDLE hThread;
	BOOL m_bFirst;
	BOOL m_bThreadTerminated;
	BOOL m_bMapLoadComplete;
	MAPTYPE m_MapNum;

	CLoadingDlg*	m_pDlg;
	cImageSelf		m_LoadingImage;

	// 070208 LYW --- CharMake : Add variables for position and scale of image.
	VECTOR2		m_tr ;
	VECTOR2		m_scale ;

	cImage  m_BackImage;
	VECTOR2		m_BackScale;
	VECTOR2		m_BackPos;
public:

	CGameLoading();
	virtual ~CGameLoading();
//	//MAKESINGLETON(CGameLoading)
		
	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);



	// custom func
	void EnterGame();

	// 070206 LYW --- Add functions to process network msg.
public :
	void UserConn_CharacterSelect_Ack( void* pMsg ) ;
	void UserConn_CharacterSelect_Nack() ;
	void Cheat_EventNotify_On( void* pMsg ) ;
	void Cheat_EventNotify_Off( void* pMsg ) ;

	// 070208 LYW --- CharMake : Add function to setting position and scale of image.
	void SetPositionScale() ;
};

EXTERNGLOBALTON(CGameLoading)

#endif // !defined(AFX_GAMELOADING_H__1AFABF78_3E9E_4B20_B18B_E07438BFB3E9__INCLUDED_)
