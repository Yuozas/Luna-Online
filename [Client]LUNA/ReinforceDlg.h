#pragma once


#include "interface/cDialog.h"


class CItem;
class CVirtualItem;
class cIconDialog;
class cIconGridDialog;
class cStatic;


class CReinforceDlg : public cDialog  
{
public:
	CReinforceDlg();
	virtual ~CReinforceDlg();

	void Linking();
	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* );

	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual void SetActive( BOOL );
	
	void Succeed( const MSG_ITEM_REINFORCE_OPTION_ACK& );
	void Fail();
	void Restore();
	void Send();


	// 080228 LUJ, ��ȣ ������ ����
public:	
	void SetProtectedActive( CItem& );

private:
	ITEMBASE mProtectionItem;


private:
	// ���� ��ȭ�� �������� ������ ���ο� ��Ḧ ��Ͻ�Ű�� �׷��� ������ ������� �ʴ´�.
	// 080228 LUJ, �������͸� ���� ���� ���������� ����
	void AddMaterial( CItem& );

	// ��Ḧ �Ұ��Ѵ�.
	void RemoveMaterial();

	void Submit( BOOL isCheck );	
	void Release();
 

private:
	CVirtualItem*		mItemIcon;

	typedef std::vector< CItem* >	MaterialIcon;
	MaterialIcon					mMaterialIcon;
	
	cIconDialog*		mBaseDialog;
	cIconGridDialog*	mIconGridDialog;
	cStatic*			mTextQuantity;

	int					mQuantity;
	const int			mMaxQuantity;

	BOOL				mIsReinforcing;

	MSG_ITEM_REINFORCE_SYN	mMessage;

	// 080228 LUJ, ��ȣ�Ǵ� �ɼ�. ��ȣ ������ �� ��ġ�� ���� ���� ��� ������ �޽����� ǥ���ϱ� ���� �����Ѵ�
	ITEM_OPTION		mProtectedOption;

	// 080228 LUJ, �Ϲ�/��ȣ ��� ǥ�ÿ� ��Ʈ��
	cWindow*	mTitle;
	cWindow*	mProtectTitle;
	cWindow*	mProtectSymbol;
};
