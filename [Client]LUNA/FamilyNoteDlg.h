#pragma once
#include "Interface\cDialog.h"

class cStatic;
class cTextArea;
class cButton;
class cEditBox;
class CItem;

class CFamilyNoteDlg :
	public cDialog
{
	cEditBox *	m_pTitleEdit;
	cTextArea * m_pNoteText;
	CItem*		m_pItem;
	BOOL		m_bUse;

public:
	CFamilyNoteDlg(void);
	~CFamilyNoteDlg(void);

	void Linking();

	void Show(CItem* pItem);
	void Use();
	void OnActionEvnet(LONG lId, void * p, DWORD we);
};
