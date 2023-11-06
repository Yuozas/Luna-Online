/**********************************************************************

�̸�: MixDialog
�ۼ�: 2007/10/22 10:16:28, �̿���

����: ��Ḧ �����Ͽ� ���ο� �������� ������.
                                                                     
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


	// 080228 LUJ, ��ȣ ������ ����
public:	
	void SetProtectedActive( CItem& );

private:
	ITEMBASE mProtectionItem;


private:
	// ������ ��û�Ѵ�. ���ڴ� ���� ����. isCheck�� ���̸� ���� �´��� üũ�Ͽ� �ٷ� �����Ѵ�
	void Submit( DURTYPE mixSize, BOOL isCheck );

	void AddResult( const DWORD itemIndex );
	void RemoveResult();

	// 071218 LUJ, ���� �⺻ �������� �κ��丮�� ������
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

	// 080228 LUJ, �Ϲ�/��ȣ ��� ǥ�� ���� ��Ʈ��
	cWindow*	mTitle;
	cWindow*	mProtectTitle;
	cWindow*	mProtectSymbol;
};
