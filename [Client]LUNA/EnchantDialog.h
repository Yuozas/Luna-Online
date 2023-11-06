#pragma once


#include "interface/cDialog.h"


class CItem;
class CVirtualItem;
class cIconDialog;
class cStatic;

struct EnchantScript;


class CEnchantDialog : public  cDialog
{
public:
	CEnchantDialog();
	virtual ~CEnchantDialog();

	void Linking();
	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* );
	
	virtual void OnActionEvent(LONG lId, void* p, DWORD we);
	virtual void SetActive( BOOL );

	void Restore();
	void Succeed( const MSG_ITEM_ENCHANT_ACK& );
	void Fail();

	void Send();


	// 080228 LUJ, ��ȣ ������ ����
public:	
	void SetProtectedActive( CItem& );

private:
	ITEMBASE mProtectionItem;

	// 080228 LUJ, �Ϲ�/��ȣ ��� ǥ�ÿ� ��Ʈ��
	cWindow*	mTitle;
	cWindow*	mProtectTitle;
	cWindow*	mProtectSymbol;


private:
	BOOL IsEnchantable( const ITEMBASE*, const EnchantScript* ) const;
	void RemoveMaterial();

	cIconDialog*	mIconDialog;
	CVirtualItem*	mSourceItem;
	CItem*			mMaterialItem;
	cStatic*		mResult;

	MSG_ITEM_ENCHANT_SYN	mMessage;
};