// ItemInsertDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "client.h"
#include "ItemAddDialog.h"
#include "ItemDialog.h"
#include ".\itemadddialog.h"


// CItemAddDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemAddDialog, CDialog)
CItemAddDialog::CItemAddDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CItemAddDialog::IDD, pParent),
	mApplication( application )
{
	EnableAutomation();

}

CItemAddDialog::~CItemAddDialog()
{
}

void CItemAddDialog::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면 OnFinalRelease가
	// 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CDialog::OnFinalRelease();
}

void CItemAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ITEM_ADD_TAB, mTabCtrl);
	DDX_Control(pDX, IDC_ITEM_ADD_TAB, mTabCtrl);
	//DDX_Control(pDX, IDC_ITEM_ADD_LIST, mItemListCtrl1);
	DDX_Control(pDX, IDC_ITEM_ADD_LIST_WEAPON, mWeaponListCtrl);
	DDX_Control(pDX, IDC_ITEM_ADD_LIST_POTION, mPotionListCtrl);
	DDX_Control(pDX, IDC_ITEM_ADD_LIST_MATERIAL, mMaterialListCtrl);
	//DDX_Control(pDX, IDC_ITEM_ADD_LIST_QUEST, mQuestListCtrl);
	DDX_Control(pDX, IDC_ITEM_ADD_INDEX_EDIT, mItemIndexEdit);
	DDX_Control(pDX, IDC_ITEM_ADD_NAME_STATIC, mItemNameStatic);
	DDX_Control(pDX, IDC_ITEM_ADD_COMBO, mItemNameCombo);
	DDX_Control(pDX, IDC_ITEM_ADD_LIST_ETC, mEtcListCtrl);
}


BEGIN_MESSAGE_MAP(CItemAddDialog, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_ITEM_ADD_TAB, OnTcnSelchangeItemAddTab)
	//ON_BN_CLICKED(IDC_ITEM_ADD_SUBMIT_BUTTON, OnBnClickedItemAddSubmitButton)
	ON_EN_CHANGE(IDC_ITEM_ADD_INDEX_EDIT, OnEnChangeItemAddIndexEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_ADD_LIST_WEAPON, OnLvnItemchangedItemAddListWeapon)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_ADD_LIST_POTION, OnLvnItemchangedItemAddListPotion)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_ADD_LIST_MATERIAL, OnLvnItemchangedItemAddListMaterial)
	ON_CBN_SELCHANGE(IDC_ITEM_ADD_COMBO, OnCbnSelchangeItemAddCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_ADD_LIST_ETC, OnLvnItemchangedItemAddListEtc)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CItemAddDialog, CDialog)
END_DISPATCH_MAP()

// 참고: IID_IItemInsertDialog에 대한 지원을 추가하여
// VBA에서 형식 안정적인 바인딩을 지원합니다. 
// 이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {0CDDD890-B3DF-4A40-A0FC-562B97CBBBEB}
static const IID IID_IItemInsertDialog =
{ 0xCDDD890, 0xB3DF, 0x4A40, { 0xA0, 0xFC, 0x56, 0x2B, 0x97, 0xCB, 0xBB, 0xEB } };

BEGIN_INTERFACE_MAP(CItemAddDialog, CDialog)
	INTERFACE_PART(CItemAddDialog, IID_IItemInsertDialog, Dispatch)
END_INTERFACE_MAP()


// CItemAddDialog 메시지 처리기입니다.

BOOL CItemAddDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// 탭 초기화
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		//// 탭의 크기를 받아와서 약간 줄인다. 리스트 컨트롤이 탭 안에 정확히 배치되기 위함이다
		//CRect tabRect;
		//{
		//	const float space = 0.1f;

		//	CRect windowRect;
		//	GetWindowRect( windowRect );

		//	mTabCtrl.GetWindowRect( tabRect );

		//	rect.left	= abs( windowRect.left - tabRect.left );
		//	rect.right	= abs( windowRect.left + tabRect.Width() );
		//	rect.top	= abs( windowRect.top - tabRect.top );
		//	rect.bottom	= abs( tabRect.top + tabRect.Height() );

		//	tabRect.left	+= LONG( float( rect.left * space ) );
		//	tabRect.right	-= LONG( float( rect.right * space ) );
		//	tabRect.top		-= LONG( float( rect.top * space ) );
		//	tabRect.bottom	-= LONG( float( rect.bottom * space ) );
		//}

		// 장비 탭
		{
			CString textEquippment;
			textEquippment.LoadString( IDS_STRING53 );
			mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textEquippment );

			mWeaponListCtrl.GetClientRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mWeaponListCtrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mWeaponListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mWeaponListCtrl.InsertColumn( 2, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );

			mWeaponListCtrl.SetExtendedStyle( mWeaponListCtrl.GetExtendedStyle() | addStyle );
			mWeaponListCtrl.ShowWindow( SW_HIDE );
			
			mApplication.SetItemList( mWeaponListCtrl, eItemCategory_Equip );
		}

		// 포션 탭
		{
			CString textPotion;
			textPotion.LoadString( IDS_STRING59 );
			mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textPotion );

			mPotionListCtrl.GetClientRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mPotionListCtrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mPotionListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );

			mPotionListCtrl.InsertColumn( 2, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );
			mPotionListCtrl.SetExtendedStyle( mPotionListCtrl.GetExtendedStyle() | addStyle );
			mPotionListCtrl.ShowWindow( SW_HIDE );
			
			mApplication.SetItemList( mPotionListCtrl, eItemCategory_Expend );
		}

		// 재료 탭
		{
			CString textMaterial;
			textMaterial.LoadString( IDS_STRING60 );
			mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textMaterial );

			mMaterialListCtrl.GetClientRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mMaterialListCtrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mMaterialListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mMaterialListCtrl.InsertColumn( 2, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );

			mMaterialListCtrl.SetExtendedStyle( mMaterialListCtrl.GetExtendedStyle() | addStyle );
			mMaterialListCtrl.ShowWindow( SW_HIDE );
			
			mApplication.SetItemList( mMaterialListCtrl, eItemCategory_Material );
		}

		// 기타 탭
		{
			CString textEtc;
			textEtc.LoadString( IDS_STRING61 );
			mTabCtrl.InsertItem( mTabCtrl.GetItemCount(), textEtc );

			mEtcListCtrl.GetClientRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mEtcListCtrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mEtcListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );

			mEtcListCtrl.InsertColumn( 2, textLevel, LVCFMT_LEFT, int( rect.Width() * 0.18f ) );
			mEtcListCtrl.SetExtendedStyle( mEtcListCtrl.GetExtendedStyle() | addStyle );
			mEtcListCtrl.ShowWindow( SW_HIDE );
			
			mApplication.SetItemList( mEtcListCtrl, eItemCategory_Etc );
		}

		mTabCtrl.SetCurSel( 0 );
		mWeaponListCtrl.ShowWindow( SW_SHOW );
	}

	// 콤보
	{
		CRect rect;
		mItemNameCombo.GetWindowRect( rect );
		mItemNameCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
	}

	{

		mItemIndexEdit.SetFocus();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CItemAddDialog::OnTcnSelchangeItemAddTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	mWeaponListCtrl.ShowWindow( SW_HIDE );
	mPotionListCtrl.ShowWindow( SW_HIDE );
	mMaterialListCtrl.ShowWindow( SW_HIDE );
	mEtcListCtrl.ShowWindow( SW_HIDE );
	
	switch( mTabCtrl.GetCurSel() )
	{
	case 0:
		{
			mWeaponListCtrl.ShowWindow( SW_SHOW );
			break;
		}
	case 1:
		{
			mPotionListCtrl.ShowWindow( SW_SHOW );
			break;
		}
	case 2:
		{
			mMaterialListCtrl.ShowWindow( SW_SHOW );
			break;
		}
	case 3:
		{
			mEtcListCtrl.ShowWindow( SW_SHOW );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}

	RedrawWindow();
}


DWORD CItemAddDialog::GetSelectedItemIndex() const
{
	return mSelectedItemIndex;
}


void CItemAddDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CListCtrl* listCtrl = 0;

	switch( mTabCtrl.GetCurSel() )
	{
	case 0:
		{
			listCtrl = &mWeaponListCtrl;
			break;
		}
	case 1:
		{
			listCtrl = &mPotionListCtrl;
			break;
		}
	case 2:
		{
			listCtrl = &mMaterialListCtrl;
			break;
		}
		/*
	case 3:
		{
			listCtrl = &mQuestListCtrl;
			break;
		}
	case 4:
		{
			listCtrl = &mEtcListCtrl;
			break;
		}
		*/
	}

	if( ! listCtrl )
	{
		ASSERT( 0 );
		return;
	}

	//POSITION	position	= listCtrl->GetFirstSelectedItemPosition();
	//const DWORD	row			= listCtrl->GetNextSelectedItem( position );
	//const DWORD	itemIndex	= atoi( listCtrl->GetItemText( row, 0 ) );

	CString item;

	mItemIndexEdit.GetWindowText( item );

	if( item.IsEmpty() )
	{
		CString textYouShouldInputIndexOrNameOfItem;
		textYouShouldInputIndexOrNameOfItem.LoadString( IDS_STRING63 );

		MessageBox( textYouShouldInputIndexOrNameOfItem, "", MB_ICONWARNING | MB_OK );
		return;
	}

	mSelectedItemIndex	= atoi( item );

	if( ! mSelectedItemIndex )
	{
		CString name;
		mItemNameCombo.GetLBText( mItemNameCombo.GetCurSel(), name );
		mSelectedItemIndex = mApplication.GetItemIndex( name );
	}

	if( !	mSelectedItemIndex ||
		!	mApplication.GetItemScript( mSelectedItemIndex ) )
	{
		CString textItIsNoExistedItem;
		textItIsNoExistedItem.LoadString( IDS_STRING63 );

		MessageBox( textItIsNoExistedItem, "", MB_ICONWARNING | MB_OK );
		return;
	}	

	CDialog::OnOK();
}


void CItemAddDialog::OnEnChangeItemAddIndexEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString keyword;
	mItemIndexEdit.GetWindowText( keyword );

	const ITEM_INFO* info = mApplication.GetItemScript( atoi( keyword ) );

	if( ! info )
	{
		if( keyword.IsEmpty() )
		{
			return;
		}
		else if( ! atoi( keyword ) )
		{
			mApplication.SetItemCombo( mItemNameCombo, keyword );

			if( ! mItemNameCombo.GetCount() )
			{
				return;
			}

			mItemNameCombo.GetLBText( mItemNameCombo.GetCurSel(), keyword );

			CString itemIndex;
			itemIndex.Format( "%d", mApplication.GetItemIndex( keyword ) );

			if( itemIndex == "0" )
			{
				return;
			}

			info = mApplication.GetItemScript( atoi( itemIndex ) );

			if( ! info )
			{
				return;
			}

			keyword = itemIndex;
		}
	}

	if( ! info )
	{
		return;
	}

	mItemNameStatic.SetWindowText( info->ItemName );

	RedrawWindow();
}

void CItemAddDialog::OnLvnItemchangedItemAddListWeapon(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	POSITION		position	= mWeaponListCtrl.GetFirstSelectedItemPosition();
	const int		row			= mWeaponListCtrl.GetNextSelectedItem( position );
	const CString	itemIndex	= mWeaponListCtrl.GetItemText( row, 0 );

	mItemIndexEdit.SetWindowText( itemIndex );
}

void CItemAddDialog::OnLvnItemchangedItemAddListPotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	POSITION		position	= mPotionListCtrl.GetFirstSelectedItemPosition();
	const int		row			= mPotionListCtrl.GetNextSelectedItem( position );
	const CString	itemIndex	= mPotionListCtrl.GetItemText( row, 0 );

	mItemIndexEdit.SetWindowText( itemIndex );
}

void CItemAddDialog::OnLvnItemchangedItemAddListMaterial(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	POSITION		position	= mMaterialListCtrl.GetFirstSelectedItemPosition();
	const int		row			= mMaterialListCtrl.GetNextSelectedItem( position );
	const CString	itemIndex	= mMaterialListCtrl.GetItemText( row, 0 );

	mItemIndexEdit.SetWindowText( itemIndex );
}


void CItemAddDialog::OnLvnItemchangedItemAddListEtc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	POSITION		position	= mEtcListCtrl.GetFirstSelectedItemPosition();
	const int		row			= mEtcListCtrl.GetNextSelectedItem( position );
	const CString	itemIndex	= mEtcListCtrl.GetItemText( row, 0 );

	mItemIndexEdit.SetWindowText( itemIndex );
}


void CItemAddDialog::OnCbnSelchangeItemAddCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString itemName;
	mItemNameCombo.GetLBText( mItemNameCombo.GetCurSel(), itemName );

	CString text;
	text.Format( "%d", mApplication.GetItemIndex( itemName ) );

	mItemIndexEdit.SetWindowText( text );
}