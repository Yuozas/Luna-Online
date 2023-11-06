#ifndef _SHCHALLENGEZONECLEARNO1DLG
#define _SHCHALLENGEZONECLEARNO1DLG

#include "./Interface/cDialog.h"
#include "./Interface/cListCtrl.h"
#include "ImageNumber.h"

class cStatic;

class CSHChallengeZoneClearNo1Dlg : public cDialog
{
private:
	CImageNumber	m_csChallengeZoneLimitTime1;
	CImageNumber	m_csChallengeZoneLimitTime2;

	DWORD			m_nChallengeZoneStartTime;
	DWORD			m_nChallengeZoneStartTimeTick;

	cStatic*		m_csSection;
	cStatic*		m_csName1;
	cStatic*		m_csName2;
	cStatic*		m_csTime;

public:
	CSHChallengeZoneClearNo1Dlg() ;
	~CSHChallengeZoneClearNo1Dlg() ;

	void		 	Linking();
	virtual void 	SetActive( BOOL val );
	virtual void 	Render();

	void			SetChallengeZoneStartTime(DWORD nTime) { m_nChallengeZoneStartTime = nTime; }
	void			SetChallengeZoneStartTimeTick(DWORD nTick) { m_nChallengeZoneStartTimeTick = nTick; }

	void			SetChallengeZoneLeastClearSection(char* pszSection)	{ m_csSection->SetStaticText(pszSection); }
	void			SetChallengeZoneLeastClearName1(char* pszName)		{ m_csName1->SetStaticText(pszName); }
	void			SetChallengeZoneLeastClearName2(char* pszName)		{ m_csName2->SetStaticText(pszName); }
	void			SetChallengeZoneLeastClearTime(char* pszTime)		{ m_csTime->SetStaticText(pszTime); }
};



#endif