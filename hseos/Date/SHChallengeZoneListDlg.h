#ifndef _SHCHALLENGEZONELISTDLG
#define _SHCHALLENGEZONELISTDLG

#include "./Interface/cDialog.h"
#include "./Interface/cListCtrl.h"

class CSHChallengeZoneListDlg : public cDialog
{
private:
	cListCtrl*		m_pcsChallengeZoneList;
	int				m_nSelChallengeZone;

public:
	CSHChallengeZoneListDlg() ;
	~CSHChallengeZoneListDlg() ;

	void		 	Linking();
	virtual void 	SetActive( BOOL val );
	virtual void 	Render();
	virtual DWORD	ActionEvent(CMouse* mouseInfo);
	void			OnActionEvent( LONG lId, void* p, DWORD we );

	void			SelectChallengeZone(int nRowIndex);
};



#endif