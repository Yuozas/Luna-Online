// QuestItemAddDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "QuestItemAddDialog.h"
#include ".\questitemadddialog.h"


// QuestItemAddDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CQuestItemAddDialog, CDialog)
CQuestItemAddDialog::CQuestItemAddDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CQuestItemAddDialog::IDD, pParent)
	, mApplication( application )
	, mQuestItemIndex( 0 )
{
	EnableAutomation();
}


CQuestItemAddDialog::~CQuestItemAddDialog()
{
}

void CQuestItemAddDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CQuestItemAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_QUEST_ITEM_ADD_QUEST_RADIO, mQuestRadio );
	DDX_Control( pDX, IDC_QUEST_ITEM_ADD_ITEM_RADIO, mItemRadio );
	DDX_Control(pDX, IDC_QUEST_ITEM_FINDED_QUEST_LIST, mQuestListCtrl);
	DDX_Control(pDX, IDC_QUEST_ITEM_FINDED_ITEM_LIST, mitemListCtrl);
	DDX_Control(pDX, IDC_QUEST_ITEM_ADD_EDIT, mKeywordEdit);
	DDX_Control(pDX, IDC_QUEST_ITEM_ADD_QUEST_DESC_STATIC, mQuestDescriptionStatic);
	DDX_Control(pDX, IDC_QUEST_ITEM_ADD_ITEM_DESC_STATIC, mItemDescriptionStatic);
}


BEGIN_MESSAGE_MAP(CQuestItemAddDialog, CDialog)
	ON_EN_CHANGE(IDC_QUEST_ITEM_ADD_EDIT, OnEnChangeQuestItemAddEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_QUEST_ITEM_FINDED_QUEST_LIST, OnLvnItemchangedQuestItemFindedQuestList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_QUEST_ITEM_FINDED_ITEM_LIST, OnLvnItemchangedQuestItemFindedItemList)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQuestItemAddDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IQuestItemAddDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {6033C4D5-88A6-4350-8A8C-54DF74D456F8}
static const IID IID_IQuestItemAddDialog =
{ 0x6033C4D5, 0x88A6, 0x4350, { 0x8A, 0x8C, 0x54, 0xDF, 0x74, 0xD4, 0x56, 0xF8 } };

BEGIN_INTERFACE_MAP(CQuestItemAddDialog, CDialog)
	INTERFACE_PART(CQuestItemAddDialog, IID_IQuestItemAddDialog, Dispatch)
END_INTERFACE_MAP()


// QuestItemAddDialog �޽��� ó�����Դϴ�.

DWORD CQuestItemAddDialog::GetQuestItemIndex() const
{
	return mQuestItemIndex;
}


void CQuestItemAddDialog::OnEnChangeQuestItemAddEdit()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString keyword;

	mKeywordEdit.GetWindowText( keyword );

	if( keyword.IsEmpty() )
	{
		return;
	}

	mQuestListCtrl.DeleteAllItems();
	mitemListCtrl.DeleteAllItems();

	if( mQuestRadio.GetCheck() )
	{
		const CclientApp::QuestStringMap& stringMap = mApplication.GetQuestStringMap();

		CString text;

		for(
			CclientApp::QuestStringMap::const_iterator it = stringMap.begin();
			stringMap.end() != it;
			++it )
		{
			const int row = mQuestListCtrl.GetItemCount();

			const CclientApp::QuestString& string = it->second;

			const std::string name( string.mTitle );

			if( std::string::npos == name.find( keyword ) )
			{
				continue;
			}

			text.Format( "%u", string.mMainQuestIndex );
			mQuestListCtrl.InsertItem( row, text, 0 );

			text.Format( "%u", string.mSubQuestIndex );
			mQuestListCtrl.SetItemText( row, 1, text );

			mQuestListCtrl.SetItemText( row, 2, string.mTitle );
		}
	}
	else if( mItemRadio.GetCheck() )
	{
		const CclientApp::QuestItemMap& itemMap = mApplication.GetQuestItemMap();

		CString text;

		for(
			CclientApp::QuestItemMap::const_iterator it = itemMap.begin();
			itemMap.end() != it;
			++it )
		{
			const int row = mitemListCtrl.GetItemCount();

			const CclientApp::QuestItem& item = it->second;

			const std::string name( item.mName );

			if( std::string::npos == name.find( keyword ) )
			{
				continue;
			}

			text.Format( "%d", it->first );
			mitemListCtrl.InsertItem( row, text, 0 );

			mitemListCtrl.SetItemText( row, 1, item.mName );
		}
	}	
}


void CQuestItemAddDialog::OnLvnItemchangedQuestItemFindedQuestList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	POSITION position = mQuestListCtrl.GetFirstSelectedItemPosition();

	const int row = mQuestListCtrl.GetNextSelectedItem( position );

	if( 0 > row )
	{
		return;
	}

	const DWORD mainQuestIndex	= atoi( mQuestListCtrl.GetItemText( row, 0 ) );
	const DWORD subQuestIndex	= atoi( mQuestListCtrl.GetItemText( row, 1 ) );

	mQuestDescriptionStatic.SetWindowText(
		mApplication.GetQuestDescription( mainQuestIndex, subQuestIndex ) );
	
	// add quest item
	{
		mitemListCtrl.DeleteAllItems();

		const CclientApp::QuestItemMap& itemMap = mApplication.GetQuestItemMap();

		CString text;

		for(
			CclientApp::QuestItemMap::const_iterator it = itemMap.begin();
			itemMap.end() != it;
			++it )
		{
			const CclientApp::QuestItem& item = it->second;

            if( item.mMainQuestIndex != mainQuestIndex )
			{
				continue;
			}

			const int	row			= mitemListCtrl.GetItemCount();
			const DWORD	itemIndex	= it->first;

			text.Format( "%d", itemIndex );

            mitemListCtrl.InsertItem( row, text, 0 );
			mitemListCtrl.SetItemText( row, 1, item.mName );
		}
	}	
}


void CQuestItemAddDialog::OnLvnItemchangedQuestItemFindedItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	POSITION position = mitemListCtrl.GetFirstSelectedItemPosition();

	const int row = mitemListCtrl.GetNextSelectedItem( position );

	mQuestItemIndex = atoi( mitemListCtrl.GetItemText( row, 0 ) );

	mItemDescriptionStatic.SetWindowText(
		mApplication.GetQuestItemDescription( mQuestItemIndex ) );
}


void CQuestItemAddDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	CDialog::OnOK();
}


BOOL CQuestItemAddDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// list control
	{
		CRect rect;
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		{
			int step = -1;

			mitemListCtrl.GetWindowRect( &rect );
			mitemListCtrl.SetExtendedStyle( mitemListCtrl.GetExtendedStyle() | addStyle );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mitemListCtrl.InsertColumn( ++step, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mitemListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 1.0f ) );
		}

		{
			int step = -1;

			mQuestListCtrl.GetWindowRect( &rect );
			mQuestListCtrl.SetExtendedStyle( mQuestListCtrl.GetExtendedStyle() | addStyle );

			CString textMain;
			textMain.LoadString( IDS_STRING191 );

			CString textSub;
			textSub.LoadString( IDS_STRING192 );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );

			mQuestListCtrl.InsertColumn( ++step, textMain + " " + textIndex, LVCFMT_LEFT, 0 );
			mQuestListCtrl.InsertColumn( ++step, textSub + " " + textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mQuestListCtrl.InsertColumn( ++step, textName, LVCFMT_LEFT, int( rect.Width() * 1.0f ) );
		}
	}

	// radio button
	{
		mItemRadio.SetCheck( BST_CHECKED );
		mQuestRadio.SetCheck( BST_UNCHECKED );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}