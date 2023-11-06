// MallNoticeDialog.cpp: implementation of the CMallNoticeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MallNoticeDialog.h"
#include "WindowIDEnum.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMallNoticeDialog::CMallNoticeDialog()
{

}

CMallNoticeDialog::~CMallNoticeDialog()
{

}

void CMallNoticeDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}

void CMallNoticeDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
/*	if( we & WE_BTNCLICK )
	{
		if( lId == ITEM_MALLBTN )
		{	
#ifdef TAIWAN_LOCAL
//			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.mengluna.com/shop.asp", NULL, SW_SHOWNORMAL );
#else
//			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.mengluna.com/shop.asp", NULL, SW_SHOWNORMAL );
#endif
		}
	}*/
}
