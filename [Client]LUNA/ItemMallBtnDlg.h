#pragma once
#include "./Interface/cDialog.h"											

class cButton ;																


class cItemMallBtnDlg : public cDialog										
{
	cButton*		m_pButton ;												

	int				m_nAlpha ;												
	DWORD			m_dwStartTime ;											
public:
	// BASE PART.
	cItemMallBtnDlg(void);													
	virtual ~cItemMallBtnDlg(void);											

	void Linking() ;														

	virtual void OnActionEvent(LONG lId, void* p, DWORD we ) ;				
};
