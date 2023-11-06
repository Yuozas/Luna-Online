#pragma once


#include "Interface/cTabDialog.h"


class cIconDialog;
class cIconGridDialog;
class CItem;
class CVirtualItem;
class CItemShow;
class cPushupButton;


class CDissolveDialog : public cTabDialog
{
public:
	CDissolveDialog();
	virtual ~CDissolveDialog();
	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* );	
	virtual void SetActiveRecursive( BOOL );
	virtual void SetActive( BOOL );
	virtual void Add( cWindow* );
	virtual void Render();

	void OnActionEvent( LONG lId, void* p, DWORD we );
	void Linking();
	void EndDissolution( BOOL bSuccess );

	void Succeed( const MSG_ITEM_DISSOLVE_ACK& );
	void Fail();
	void Restore();
	void Send();
	

private:
	// isCheck가 참이면 실제로 전송하지 않고 검사 후 대기창을 표시한다.
	void Submit( BOOL isCheck );
	void Release();

	void AddItem( CItem* );
	void RemoveItem();

	void AddResult( DWORD wDissolutionItemIdx );
	void RemoveResult();


private:
	cIconDialog*	mBaseDialog;
	CItem*			mItemIcon;
	CItemShow*		mSpecialIcon;
	
	cPushupButton*	mTabButton1;
	cPushupButton*	mTabButton2;
	cPushupButton*	mTabButton3;
	
	typedef std::vector< cIconGridDialog* > TabSheet;
	TabSheet								mTabSheet;

	typedef std::vector< CItemShow* >	ResultIcon;
	ResultIcon							mResultIcon;

	MSG_ITEM_DISSOLVE_SYN				mMessage;
};
