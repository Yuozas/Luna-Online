#ifndef _CMSGBOX_H
#define _CMSGBOX_H
//=================================================================================================
//	FILE		: cMsgBox.h
//	PURPOSE		: Interface message box.
//	PROGRAMMER	: Yongs Lee
//	DATE		: January 19, 2008
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		필요한 헤더파일을 불러온다.
//=========================================================================
#include "./interface/cDialog.h"





//=========================================================================
//		필수 정의 파트.
//=========================================================================
#define MB_MAX_BTN_COUNT	2												// 메시지 박스에서 사용가능한 최대 버튼 수.

#define MAX_MB_MSG			128												// 메시지 박스의 최대 길이.





//=========================================================================
//		필요한 컨트롤 인터페이스를 선언한다.
//=========================================================================
class cStatic ;
class cTextArea ;


enum eMBType
{
	MBT_NOBTN = 0,
	MBT_OK,
	MBT_YESNO,
	MBT_CANCEL,
	MBT_COUNT,
	// desc_hseos_메세지박스01
	// S 메세지박스 추가 added by hseos 2007.10.16
	MBT_AUTOSIZE_YESNO,
	// E 메세지박스 추가 added by hseos 2007.10.16
};


enum eMB_BTN_ID
{
	MB_BTN_OK = 0,
	MB_BTN_YES,
	MB_BTN_NO,
	MB_BTN_CANCEL,
	MB_BTN_COUNT,
};





//=========================================================================
//		메시지 박스 클래스 정의
//=========================================================================
class cMsgBox : public cDialog
{
protected :
	static cImage	m_DlgImg ;
	static cImage	m_BtnImgBasic ;
	static cImage	m_BtnImgOver ;
	static cImage	m_BtnImgPress ;
	static char		m_BtnText[MB_BTN_COUNT][32] ;

	static DWORD	m_dwColor[3] ;									//basic, over, press
	static BOOL		m_bInit ;

protected:
	cTextArea*		m_pMsg ;

	DWORD			m_dwMsgColor ;

	cButton*		m_pButton[2] ;

	void (*m_cbMBFunc)(LONG lId, void * p, DWORD we) ;
	
	int				m_nDefaultBtn ;
	int				m_nMBType ;
	DWORD			m_dwParam ;

protected :	
	int				m_nWidth ;
	int				m_nHeight ;

	int				m_nBtn1_Y ;
	int				m_nBtn2_Y ;

	int				m_nBtnW ;
	int				m_nBtnH ;

	int				m_nText_X ;
	int				m_nText_Y ;

public:
	cMsgBox() ;
	~cMsgBox() ;

	virtual DWORD ActionEvent( CMouse * mouseInfo ) ;
	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo ) ;

	void SetMBFunc( cbFUNC cbMBFunc ) { m_cbMBFunc = cbMBFunc; }

	static void InitMsgBox() ;
	static BOOL IsInit() { return m_bInit; }
	void MsgBox( LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc = NULL ) ;
	
	void SetDefaultBtn( int nDefaultBtn ) { m_nDefaultBtn = nDefaultBtn; }

	BOOL PressDefaultBtn() ;
	BOOL ForcePressButton( LONG lBtnID ) ;
	void ForceClose() ;

	DWORD GetParam() { return m_dwParam; }
	void SetParam( DWORD dwParam )  { m_dwParam = dwParam; }
};



#endif





