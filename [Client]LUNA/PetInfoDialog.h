#pragma once
#include "interface\ctabdialog.h"

class CPetInvenDialog;
class CPetStateDialog;

class CPetInfoDialog :
	public cTabDialog
{
	CPetInvenDialog*	mpPetInvenDialog;
	CPetStateDialog*	mpPetStateDialog;

	BYTE				mCurTab;

public:
	CPetInfoDialog(void);
	virtual ~CPetInfoDialog(void);
	
	virtual void OnActionEvent(LONG id, void* p, DWORD event);

	virtual void Add(cWindow * window);
	virtual void Render();
	void Linking();

	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	virtual void SetActive( BOOL val );
};
