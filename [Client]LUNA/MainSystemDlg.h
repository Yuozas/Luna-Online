//=================================================================================================
//	FILE		: MainSystemDlg.h
//	PURPOSE		: Header file for main system dialog.
//	PROGRAMMER	: Yongs Lee
//	DATE		: December 19, 2006
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================
#pragma once
#ifndef __MAINSYSTEMDLG_h__
#define __MAINSYSTEMDLG_h__


//=================================================================================================
//	INCLUDE PART
//=================================================================================================
#include "./interface/cTabDialog.h"


//=================================================================================================
//	DEFINE PART
//=================================================================================================
// 070115 LYW --- Define max count of menu buttons.
#define MAX_MENU_BTN	4

// 070115 LYW --- Define max count of button controls for systemset part.
#define MAX_SYSTEMSET_BTN	4

// 070115 LYW --- Define max count of button controls for gamesystem part.
#define MAX_GAMESYSTEM_BTN	10


//=================================================================================================
//	THE OTHERS PART
//=================================================================================================
class cWindow ;

// 070115 LYW --- Call interface to need.
class cButton ;
class cStatic ;

//=================================================================================================
// NAME			: CMainSystemDlg Class.
// PURPOSE		: The class for main system dialog.
// ATTENTION	:
//=================================================================================================
// 070115 LYW --- Modified this line.
//class CMainSystemDlg : public cTabDialog
class CMainSystemDlg : public cDialog
{
protected :
	// 070115 LYW --- Add menu button.
	cButton*	m_pMenuBtn[ MAX_MENU_BTN ] ;

	// 070115 LYW --- Add static controls for background image of sub dialog.
	cStatic*	m_pSystemSet ;
	cStatic*	m_pGameSystem ;

	// 070115 LYW --- Add controls for systemset part.
	cButton*	m_pMenuSystemSet[ MAX_SYSTEMSET_BTN ] ;
	cStatic*	m_pTitleSystemSet ;
	cStatic*	m_pMarkSystemSet ;

	// 070115 LYW --- Add controls for gamesystem part.
	cButton*	m_pMenuGameSystem[ MAX_GAMESYSTEM_BTN ] ;
	cStatic*	m_pTitleGameSystem ;
	cStatic*	m_pMarkGameSystem ;

	// 080403 LYW --- MainSystemDlg : Add a button to call main dialog of chatroom system.
	//cButton*	m_pChatRoom ;

	// 070115 LYW --- Add variables whether show sub dialog or not.
	BOOL		m_bShowSystemSet ;
	BOOL		m_bShowGameSystem ;

public:
	CMainSystemDlg(void);
	virtual ~CMainSystemDlg(void);

	// 070115 LYW --- Delete this functions.
	/*
	void Render() ;
	void Add( cWindow* window ) ;
	*/

	// 070115 LYW --- Add function ActionEvent.
	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;

	/// The function for processing event from controls of this dialog.
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;

	// 070115 LYW --- Add function to linking controls to window manager.
	void Linking() ;

	// 070115 LYW --- Add function to setting systemset part.
	void SetSystemSet() ;
	
	// 070115 LYW --- Add function to setting gamesystem part.
	void SetGameSystem() ;
};
#endif //__MAINSYSTEMDLG_h__



















