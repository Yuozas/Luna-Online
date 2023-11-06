// WearedExDialog.h: interface for the CWearedExDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_)
#define AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIconDialog.h"


struct SetScript;


class CWearedExDialog : public cIconDialog  
{
public:
	CWearedExDialog();
	virtual ~CWearedExDialog();

	BOOL AddItem(WORD relPos, cIcon * InIcon);
	BOOL DeleteItem(WORD relPos, cIcon ** outIcon);

	// 070411 LYW --- WearedExDialog : Add function to refresh character info dialog.
	void RefreshCharacterInfo() ;
	void RefreshArmorItem();

private:
	// �ش� �� �ε����� ���� �������� ������ ��ģ��
	void RefreshSetItem( const ITEMBASE& );
};

#endif // !defined(AFX_WEAREDEXDIALOG_H__921DB71C_9868_404D_9125_3AEE663B3341__INCLUDED_)
