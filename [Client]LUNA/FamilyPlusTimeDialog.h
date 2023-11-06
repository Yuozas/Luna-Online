//
////Add 060803 by wonju
//


#ifndef _FAMILYPLUSTIMEDLG__
#define _FAMILYPLUSTIMEDLG__

#include "./Interface/cDialog.h"

class cStatic;
class cListDialog;

class CFamilyPlusTimeDlg :
	public cDialog
{
private:
	cStatic*		m_pCurrentHavePoint;
	cListDialog*    m_pPlusItemList;
	int             m_CurrentSelectedItem; //현재 선택된 아이템의 번호

public:
	void Linking();
	void OnActionEvnet(LONG lId, void * p, DWORD we);
	void SetActive(BOOL val);
	virtual DWORD ActionEvent(CMouse* mouseInfo);

	int GetCurrentSelectedItemNumber(){ return m_CurrentSelectedItem; }

	void SetFamilyPointText(DWORD FamilyPoint);
	void LoadPlustimeList();
	CFamilyPlusTimeDlg();
	~CFamilyPlusTimeDlg();
};

#endif // end of _FAMILYPLUSTIMEDLG__