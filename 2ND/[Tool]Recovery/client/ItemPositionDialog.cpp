// ItemPositionDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "ItemPositionDialog.h"
#include ".\itempositiondialog.h"


// CItemPositionDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemPositionDialog, CDialog)
CItemPositionDialog::CItemPositionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CItemPositionDialog::IDD, pParent)
	, mAreaType( Item::AreaNone )
{
	EnableAutomation();
}

CItemPositionDialog::~CItemPositionDialog()
{
}

void CItemPositionDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CItemPositionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ITEM_POSITION_INVENTORY_RADIO, mInventoryRadioButton );
	DDX_Control(pDX, IDC_ITEM_POSITION_PRIVATE_STORAGE_RADIO, mPrivateStorageRadioButton );
	DDX_Control(pDX, IDC_ITEM_POSITION_CASH_STORAGE_RADIO, mCashStorageRadioButton );
}


BEGIN_MESSAGE_MAP(CItemPositionDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemPositionDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IItemPositionDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {2D0B3A3A-534F-4ACF-9F87-33ECE4809188}
static const IID IID_IItemPositionDialog =
{ 0x2D0B3A3A, 0x534F, 0x4ACF, { 0x9F, 0x87, 0x33, 0xEC, 0xE4, 0x80, 0x91, 0x88 } };

BEGIN_INTERFACE_MAP(CItemPositionDialog, CDialog)
	INTERFACE_PART(CItemPositionDialog, IID_IItemPositionDialog, Dispatch)
END_INTERFACE_MAP()


// CItemPositionDialog �޽��� ó�����Դϴ�.

void CItemPositionDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if( mInventoryRadioButton.GetCheck() )
	{
		mAreaType = Item::AreaInventory;
	}
	else if( mPrivateStorageRadioButton.GetCheck() )
	{
		mAreaType = Item::AreaPrivateStorage;
	}	
	else if( mCashStorageRadioButton.GetCheck() )
	{
		mAreaType = Item::AreaCashStorage;
	}
	
	OnOK();
}


void CItemPositionDialog::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	mAreaType = Item::AreaNone;

	OnCancel();
}


INT_PTR CItemPositionDialog::DoModal( POSTYPE InventoryPosition, POSTYPE privateStoragePosition, POSTYPE cashStoragePosition )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mIsInventoryPosition		= ( USHRT_MAX != InventoryPosition );
	mIsPrivateStoragePosition	= ( USHRT_MAX != privateStoragePosition );
	mIsCashStoragePosition		= ( USHRT_MAX != cashStoragePosition );

	return CDialog::DoModal();
}


Item::Area CItemPositionDialog::GetAreaType() const
{
	return mAreaType;
}


BOOL CItemPositionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	mInventoryRadioButton.EnableWindow		( mIsInventoryPosition );
	mPrivateStorageRadioButton.EnableWindow	( mIsPrivateStoragePosition );
	mCashStorageRadioButton.EnableWindow	( mIsCashStoragePosition );

	if( mIsInventoryPosition )
	{
		mInventoryRadioButton.SetCheck( 1 );
	}
	else if( mIsPrivateStoragePosition )
	{
		mPrivateStorageRadioButton.SetCheck( 1 );
	}
	else if( mIsCashStoragePosition )
	{
		mCashStorageRadioButton.SetCheck( 1 );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
