#ifndef _OPTIONDIALOG_H
#define _OPTIONDIALOG_H


#include "./Interface/cTabDialog.h"
#include "OptionManager.h"

//class cButton;

class COptionDialog : public cTabDialog
{
protected:
	sGAMEOPTION		m_GameOption;

public :
	void UpdateData( BOOL bSave ) ;

public :
	COptionDialog() ;
	virtual ~COptionDialog() ;

	virtual void Add( cWindow* window ) ;
	virtual void SetActive( BOOL val ) ;

	void Linking() ;
	void OnActionEvent( LONG lId, void* p, DWORD we ) ;

#ifndef _JAPAN_LOCAL_
	void DisableGraphicTab(BOOL bDisable) ;
#endif //_JAPAN_LOCAL_
};

////class cButton;
//
//class COptionDialog : public cTabDialog
//{
//protected:
//
//	sGAMEOPTION		m_GameOption;
////	cButton*		m_pBtnPreview;
//	// 061206 LYW --- Add Flag For CheckBox. ( Chatting Option Part )
//	BOOL			m_bCheckSystem ;
//
//protected:
//
//	void UpdateData( BOOL bSave );
//public:
//
//	COptionDialog();
//	virtual ~COptionDialog();
//
//	virtual void Add(cWindow * window);
//	virtual void SetActive(BOOL val);
//
//	void Linking();
//	void OnActionEvent(LONG lId, void * p, DWORD we);
//
//	void DisableGraphicTab(BOOL bDisable);
//
//	// 2005.12.28ÀÏ Ãß°¡ ´« On/Off
//	int GetEffectSnow() { return m_GameOption.nEffectSnow;}
//
//	// 061206 LYW --- Add Function to Setting checkbox.
//	void SetCheckBox( int nTabNum, LONG lId, BOOL* bOptionValue ) ;
///*
//	/// 061206 LYW --- Processing Event Functions.
//	virtual DWORD ActionEvent( CMouse* mouseInfo ) ;
//	*/
//};





#endif
