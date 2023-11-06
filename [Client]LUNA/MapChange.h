// MapChange.h: interface for the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCHANGE_H__3C1A28F1_57AF_47AE_89FD_E161A529189F__INCLUDED_)
#define AFX_MAPCHANGE_H__3C1A28F1_57AF_47AE_89FD_E161A529189F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "cImageSelf.h"
#include "cImage.h"

#define MAPCHANGE USINGTON(CMapChange)

class CLoadingDlg;

class CMapChange  : public CGameState  
{
	MAPTYPE m_MapNum;
					
	HANDLE hThread;
	BOOL m_bThreadTerminated;
	BOOL m_bMapLoadComplete;
	
	BOOL m_bFirst;
	
	CLoadingDlg* m_pDlg;
	
	int m_GameInInitKind;
	cImageSelf		m_LoadingImage;

	// 070208 LYW --- MapChange : Add variables for position and scale of image.
	VECTOR2		m_tr ;
	VECTOR2		m_scale ;

	cImage  m_BackImage;
	VECTOR2		m_BackScale;
	VECTOR2		m_BackPos;

public:
//	//MAKESINGLETON(CMapChange);
	
	CMapChange();
	virtual ~CMapChange();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();
	void BeforeRender();
	void AfterRender();
	
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);

	// custom func
	void EnterGame();

	void SetGameInInitKind(int kind)	{	m_GameInInitKind = kind;	}

	// 070208 LYW --- MapChange : Add function to setting position and scale of image.
	void SetPositionScale() ;
};
EXTERNGLOBALTON(CMapChange)
#endif // !defined(AFX_MAPCHANGE_H__3C1A28F1_57AF_47AE_89FD_E161A529189F__INCLUDED_)
