/**********************************************************************

이름: MixDialog
작성: 2007/10/22 10:16:28, 이웅주

목적: 재료를 조합하여 새로운 아이템을 제작함.
                                                                     
***********************************************************************/
#pragma once

#include "interface/cDialog.h"


class cStatic;
class cIconDialog;
class cIconGridDialog;
class CItem;
class CItemShow;
class cListDialog;

struct ItemMixResult;


class CMixDialog : public cDialog
{	
public:
	CMixDialog();
	virtual ~CMixDialog();
	
	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* );
	virtual void SetActive( BOOL );
	virtual void SetActiveRecursive(BOOL val );
	
	void Linking();	
	void OnActionEvent( LONG lId, void* p, DWORD we );

	void Fail( const MSG_ITEM_MIX_ACK& );
	void Succeed( const MSG_ITEM_MIX_ACK& );
	void Restore();
	void Send();
	void Refresh();
	void RefreshResult( const ITEMBASE& );


	// 080228 LUJ, 보호 아이템 관리
public:	
	void SetProtectedActive( CItem& );

private:
	ITEMBASE mProtectionItem;


private:
	// 조합을 요청한다. 인자는 제작 개수. isCheck가 참이면 값이 맞는지 체크하여 바로 전송한다
	void Submit( DURTYPE mixSize, BOOL isCheck );

	void AddResult( const DWORD itemIndex );
	void RemoveResult();

	// 071218 LUJ, 조합 기본 아이템을 인벤토리로 돌린다
	void RemoveItem();
	void PutDescription( const ItemMixResult* );
	
	void Release();
	
	
private:
	cListDialog*		mDescription;
	cIconDialog*		mBaseDialog;
	cIconGridDialog*	mIconGridDialog;
	
	typedef std::vector< CItemShow* >		ResultIcon;
	ResultIcon								mResultIcon;

	typedef std::vector< CItem* >	ItemIcon;
	ItemIcon						mItemIcon;

	static void SubmitDivide( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void CancelDivide( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	MSG_ITEM_MIX_SYN mMessage;

	enum ePosition
	{
		ePositionSource,
		ePositionResult,
	};

	// 080228 LUJ, 일반/보호 모드 표시 위한 컨트롤
	cWindow*	mTitle;
	cWindow*	mProtectTitle;
	cWindow*	mProtectSymbol;
};
