#pragma once
#include ".\interface\cdialog.h"

#include "../QuickManager.h"

class cIconGridDialog;
class cStatic;
class cQuickItem;
class CItem;
class cSkillBase;

//// 080702 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� ���� �ִ� �������� 4�� �����Ѵ�.
////#define MAX_SLOTPAGE	8
//#define MAX_SLOTPAGE	4
//#define MAX_SLOTNUM		10

// 070216 LYW --- QuickSlotDlg : Include cConductBase interface.
class cConductBase ;

class cQuickSlotDlg :
	public cDialog
{
	cIconGridDialog* mpSheet[ MAX_SLOTPAGE ];
	cStatic*	mpStatic;

	WORD	mCurSheet;
	WORD	mStartSheetNum;

	// 080703 LYW --- QuickSlotDlg : Ȯ�彽�� �߰��� ���� ó��.
	//void FakeMoveQuickItem(LONG x, LONG y, cQuickItem* icon);
	void FakeMoveQuickItem(cDialog* pFromDlg, LONG x, LONG y, cQuickItem * icon) ;

	void FakeAddItem(LONG x, LONG y, CItem* icon);
	void FakeAddSkill(LONG x, LONG y, cSkillBase* icon);
	// 070216 LYW --- QuickSlotDlg : Add function to add conduct to quick slot.
	void FakeAddConduct( LONG x, LONG y, cConductBase* icon ) ;

public:
	cQuickSlotDlg(void);
	virtual ~cQuickSlotDlg(void);

	virtual void Add(cWindow * window);
	void Linking();
	void SetStartSheetNum( WORD num ) { mStartSheetNum = num; }

	void OnActionEvnet(LONG lId, void * p, DWORD we);

	void SelectPage( WORD page );
	void SetActive( BOOL val );
	void SetDisable( BOOL val );


	// 080703 LYW --- QuickSlotDlg : Ȯ�� ���� �߰��� ���� �Լ� ����.
	//BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	BOOL FakeMoveIcon(cDialog* pFromDlg, LONG mouseX, LONG mouseY, cIcon * icon);
	void FakeDeleteQuickItem();
	//void DeleteQuickItem(int nTabNum, int nPos) ;

	WORD GetCurSheet() { return mCurSheet + mStartSheetNum; }

	// 070917 ����, ��Ÿ�� ó��
	void SetCoolTime( DWORD itemIndex,  DWORD miliSecond );
	// 080326 NYJ --- ���������� ��Ÿ�Ӱ���.
	void SetCoolTimeAbs( DWORD itemIndex, DWORD dwBegin, DWORD dwEnd, DWORD miliSecond );

    // 071124 LYW --- QuickSlotDlg : �̺�Ʈ �Լ� �߰�.
	virtual DWORD ActionEvent(CMouse* mouseInfo) ;
};