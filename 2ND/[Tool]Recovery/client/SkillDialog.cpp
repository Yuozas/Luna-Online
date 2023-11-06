// SkillDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "SkillDialog.h"
#include "SkillLogDialog.h"
#include "SkillReportDialog.h"


// CSkillDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSkillDialog, CDialog)
CSkillDialog::CSkillDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CSkillDialog::IDD, pParent)
	, mApplication( application )

{
	EnableAutomation();
}

CSkillDialog::~CSkillDialog()
{
}

void CSkillDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CSkillDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//DDX_Control( pDX, IDC_SKILL_MY_LIST, mMyListCtrl );
	DDX_Control(pDX, IDC_SKILL_LEVEL_COMBO, mLevelCombo);
	DDX_Control(pDX, IDC_SKILL_UPDATE_BUTTON, mUpdateButton);
	DDX_Control(pDX, IDC_SKILL_ENABLE_LIST, mEnableListCtrl);
	DDX_Control(pDX, IDC_SKILL_ENABLE_TIP_STATIC, mEnableSkillStatic);
	DDX_Control(pDX, IDC_SKILL_MY_TIP_STATIC, mMySkillStatic );
	DDX_Control(pDX, IDC_SKILL_MY_LIST2, mMyListCtrl);
	DDX_Control(pDX, IDC_SKILL_REPORT_BUTTON, mReportButton);
}


BEGIN_MESSAGE_MAP(CSkillDialog, CDialog)
	ON_BN_CLICKED(IDC_SKILL_UPDATE_BUTTON, OnBnClickedSkillUpdateButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SKILL_DIALOG_LIST, OnLvnItemchangedSkillDialogList)
	ON_BN_CLICKED(IDC_SKILL_REMOVE_BUTTON, OnBnClickedSkillRemoveButton)
	ON_BN_CLICKED(IDC_SKILL_ADD_BUTTON, OnBnClickedSkillAddButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SKILL_ENABLE_LIST, OnLvnItemchangedSkillEnableList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SKILL_MY_LIST, OnLvnItemchangedSkillMyList)
	ON_BN_CLICKED(IDC_SKILL_LOG_BUTTON, OnBnClickedSkillLogButton)
	ON_BN_CLICKED(IDC_SKILL_REPORT_BUTTON, OnBnClickedSkillReportButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSkillDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_ISkillDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {29F98D55-AAD8-4CFC-87D7-48E62C581E64}
static const IID IID_ISkillDialog =
{ 0x29F98D55, 0xAAD8, 0x4CFC, { 0x87, 0xD7, 0x48, 0xE6, 0x2C, 0x58, 0x1E, 0x64 } };

BEGIN_INTERFACE_MAP(CSkillDialog, CDialog)
	INTERFACE_PART(CSkillDialog, IID_ISkillDialog, Dispatch)
END_INTERFACE_MAP()


// CSkillDialog �޽��� ó�����Դϴ�.


void CSkillDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_SKILL_GET_ALL_ACK:
		{
			mSkillMap.clear();

			const MSG_RM_SKILL* m = ( MSG_RM_SKILL* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const SKILL_BASE&	data	= m->mData[ i ];
				
				if( mSkillMap.end() == mSkillMap.find( data.wSkillIdx ) )
				{
					mSkillMap.insert( std::make_pair( data.wSkillIdx, data ) );
				}
				else
				{
					ASSERT( 0 );
				}
			}

			PutSkill();

			break;
		}
	case MP_RM_SKILL_SET_LEVEL_ACK:
		{
			const MSG_DWORD3* m = ( MSG_DWORD3* )message;

			const DWORD skillDbIndex	= m->dwData1;
			const DWORD	skillIndex		= m->dwData2;
			const BYTE level			= BYTE( m->dwData3 );

			if( mSkillMap.end() == mSkillMap.find( skillIndex ) )
			{
				ASSERT( 0 && "Index is not exist" );
				return;
			}

			SKILL_BASE& data = mSkillMap[ skillIndex ];

			ASSERT( data.dwDBIdx == skillDbIndex );

			data.Level	= level;

			// ����Ʈ ��Ʈ�� ������ ����
			for( int i = 0; i < mMyListCtrl.GetItemCount(); ++i )
			{
				const CString dbIndex = mMyListCtrl.GetItemText( i, 0 );

				if( skillDbIndex == atoi( dbIndex ) )
				{
					CString text;
					text.Format( "%d", level );

					mMyListCtrl.SetItemText( i, 3, text );
					break;
				}
			}

			// �޽��� �ڽ� ǥ��
			{
				CString textUpdateWasSucceed;
				textUpdateWasSucceed.LoadString( IDS_STRING28 );
				
				MessageBox( textUpdateWasSucceed, "", MB_ICONINFORMATION | MB_OK );
			}
			
			break;
		}
	case MP_RM_SKILL_ADD_ACK:
		{
			const MSG_RM_SKILL* m = ( MSG_RM_SKILL* )message;

			for( DWORD i = 0; i < m->mSize; ++i )
			{
				const SKILL_BASE& data = m->mData[ i ];

				mSkillMap.insert( std::make_pair( data.wSkillIdx, data ) );
			}

			PutSkill();

			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_ICONINFORMATION | MB_OK );

			break;
		}
	case MP_RM_SKILL_REMOVE_ACK:
		{
			const MSG_DWORD2* m = ( MSG_DWORD2* )message;

			mSkillMap.erase( m->dwData2 );

			PutSkill();
			
			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_ICONINFORMATION | MB_OK );
			break;
		}
	case MP_RM_SKILL_ADD_NACK:
	case MP_RM_SKILL_REMOVE_NACK:
	case MP_RM_SKILL_SET_LEVEL_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateWasFailed, "", MB_ICONERROR | MB_OK );
			break;
		}
	case MP_RM_SKILL_NACK_BY_AUTH:
		{
			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


BOOL CSkillDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), FALSE );
	SetIcon( AfxGetApp()->LoadIcon( IDR_MAINFRAME ), TRUE );

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//CDataExchange dx( this, TRUE );
	//DoDataExchange( &dx );

	// list control initialization
	{
		const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
		CRect rect;

		{
			mMyListCtrl.GetWindowRect( rect );

			CString textDbIndex;
			textDbIndex.LoadString( IDS_STRING13 );
			mMyListCtrl.InsertColumn( 0, textDbIndex,	LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mMyListCtrl.InsertColumn( 1, textIndex,		LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mMyListCtrl.InsertColumn( 2, textName,		LVCFMT_LEFT, int( rect.Width() * 0.5f ) );

			CString textLevel;
			textLevel.LoadString( IDS_STRING5 );
			mMyListCtrl.InsertColumn( 3, textLevel,		LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textPassive;
			textPassive.LoadString( IDS_STRING197 );
			mMyListCtrl.InsertColumn( 4, textPassive,		LVCFMT_LEFT, int( rect.Width() * 0.14f ) );
			
			CString textSkillPoint;
			textSkillPoint.LoadString( IDS_STRING96 );
			mMyListCtrl.InsertColumn( 5, textSkillPoint,		LVCFMT_LEFT, int( rect.Width() * 0.14f ) );

			CString textCost;
			textCost.LoadString( IDS_STRING198 );
			mMyListCtrl.InsertColumn( 6, textCost,	LVCFMT_LEFT, int( rect.Width() * 0.2f ) );

			mMyListCtrl.SetExtendedStyle( mMyListCtrl.GetExtendedStyle() | addStyle );
		}

		{
			mEnableListCtrl.GetWindowRect( rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );
			mEnableListCtrl.InsertColumn( 0, textIndex, LVCFMT_LEFT, 0 );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mEnableListCtrl.InsertColumn( 1, textName, LVCFMT_LEFT, int( rect.Width() * 0.7f ) );

			CString textPassive;
			textPassive.LoadString( IDS_STRING197 );
			mEnableListCtrl.InsertColumn( 2, textPassive, LVCFMT_LEFT, int( rect.Width() * 0.14f ) );

			mEnableListCtrl.SetExtendedStyle( mEnableListCtrl.GetExtendedStyle() | addStyle );
		}		
	}

	// combo box initialization
	{
		CRect rect;
		mLevelCombo.GetWindowRect( rect );
		mLevelCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );
	}

	// static initialization
	{
		mEnableSkillStatic.SetWindowText( "" );
		mMySkillStatic.SetWindowText( "" );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CSkillDialog::Request( DWORD serverIndex, const CString& playerName, DWORD playerIndex )
{
	mPlayerIndex	= playerIndex;
	mPlayerName		= playerName;
	mServerIndex	= serverIndex;

	{
		CString textTitleFormat;
		textTitleFormat.LoadString( IDS_STRING200 );

		CString text;
		text.Format( textTitleFormat, mApplication.GetServerName( serverIndex ), playerName );

		SetWindowText( text );
	}

	{
		MSGBASE message;

		message.Category	= MP_RM_SKILL;
		message.Protocol	= MP_RM_SKILL_GET_ALL_SYN;
		message.dwObjectID	= playerIndex;

		mApplication.Send( serverIndex, message, sizeof( message ) );
	}
}


void CSkillDialog::OnBnClickedSkillUpdateButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION position = mMyListCtrl.GetFirstSelectedItemPosition();

	const CString dbIndex = mMyListCtrl.GetItemText( mMyListCtrl.GetNextSelectedItem( position ), 0 );

	if( dbIndex.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	CString level;
	mLevelCombo.GetLBText( mLevelCombo.GetCurSel(), level );

	MSG_DWORD2 message;

	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_SET_LEVEL_SYN;
	message.dwObjectID	= mPlayerIndex;
	message.dwData1		= atoi( dbIndex );
	message.dwData2		= atoi( level );

	// TODO: ���� ������ ������ ������Ʈ���� �ʴ� �ڵ� �߰��� ��

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CSkillDialog::OnLvnItemchangedSkillDialogList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	const DWORD row = pNMLV->iItem;
	
	// ��ų�� �ִ� ���� ������ŭ �޺� �ڽ��� �����Ѵ�
	{
		const CString index = mMyListCtrl.GetItemText( row, 1 );

		const DWORD size = mApplication.GetSkillSize( atoi( index ) );
		CString text;

		mLevelCombo.ResetContent();

		for( DWORD i = 1; i <= size; i++ )
		{
			text.Format( "%d", i );
			mLevelCombo.AddString( text );
		}
	}

	// ���� ������ �����س��´�
	{
		const CString level = mMyListCtrl.GetItemText( row, 3 );

		mLevelCombo.SetCurSel( atoi( level ) - 1 );
	}

	// ����
	{
		const CString index = mMyListCtrl.GetItemText( pNMLV->iItem, 1 );

		mMySkillStatic.SetWindowText( mApplication.GetTip( atoi( index ) ) );
	}
}


void CSkillDialog::OnBnClickedSkillRemoveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION	position	= mMyListCtrl.GetFirstSelectedItemPosition();
	CString		dbIndex		= mMyListCtrl.GetItemText( mMyListCtrl.GetNextSelectedItem( position ), 0 );

	if( dbIndex.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}

	CString textDoYouDeleteIt;
	textDoYouDeleteIt.LoadString( IDS_STRING108 );

	if( IDNO == MessageBox( textDoYouDeleteIt, "", MB_ICONWARNING | MB_YESNO ) )
	{
		return;
	}

	MSG_DWORD message;

	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_REMOVE_SYN;
	message.dwObjectID	= mPlayerIndex;
	message.dwData		= atoi( dbIndex );

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CSkillDialog::OnBnClickedSkillAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	POSITION	position	= mEnableListCtrl.GetFirstSelectedItemPosition();
	CString		index		= mEnableListCtrl.GetItemText( mEnableListCtrl.GetNextSelectedItem( position ), 0 );

	if( index.IsEmpty() )
	{
		CString textSelectTarget;
		textSelectTarget.LoadString( IDS_STRING31 );

		MessageBox( textSelectTarget, "", MB_ICONERROR | MB_OK );
		return;
	}
	
	CString textDoYouAddIt;
	textDoYouAddIt.LoadString( IDS_STRING201 );

	if( IDNO == MessageBox( textDoYouAddIt, "", MB_ICONWARNING | MB_YESNO ) )
	{
		return;
	}

	MSG_DWORD message;

	message.Category	= MP_RM_SKILL;
	message.Protocol	= MP_RM_SKILL_ADD_SYN;
	message.dwObjectID	= mPlayerIndex;
	message.dwData		= atoi( index );

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}


void CSkillDialog::PutSkill()
{
	// ���� ��ų�� ǥ���Ѵ�
	{
		mMyListCtrl.DeleteAllItems();

		DWORD usedMoney = 0;
		DWORD userPoint = 0;

		for( SkillMap::const_iterator it = mSkillMap.begin(); mSkillMap.end() != it; ++it )
		{
			const DWORD			index	= it->first;
			const SKILL_BASE&	data	= it->second;
			const SkillScript*	script	= mApplication.GetSkillScript( index );

			if( ! script )
			{
				ASSERT( 0 );
				continue;
			}

			const DWORD row		= mMyListCtrl.GetItemCount();
			int			step	= 0;

			CString text;
			text.Format( "%d", data.dwDBIdx );
			mMyListCtrl.InsertItem( row, text, 0 );

			text.Format( "%d", data.wSkillIdx );
			mMyListCtrl.SetItemText( row, ++step, text );

			mMyListCtrl.SetItemText( row, ++step, script->mName );

			text.Format( "%d", data.Level );
			mMyListCtrl.SetItemText( row, ++step, text );

			mMyListCtrl.SetItemText( row, ++step, script->mIsPassive ? "*" : "" );

			DWORD point	= 0;
			DWORD money	= 0;
			{
				for( int level = 1; level <= data.Level; ++level )
				{
					const SkillScript* skillScript = mApplication.GetSkillScript( index / 100 * 100 + level );

					if( skillScript )
					{
						point += skillScript->mPoint;
						money += skillScript->mMoney;
					}
				}

				usedMoney += money;
				userPoint += point;
			}

			text.Format( "%d", point );
            mMyListCtrl.SetItemText( row, ++step, text );

			text.Format( "%d", money );
			mMyListCtrl.SetItemText( row, ++step, text );
		}

		CString textYouConsumeToLearnSkillHowManyPointAndGold;
		textYouConsumeToLearnSkillHowManyPointAndGold.LoadString( IDS_STRING202 );

		CString text;
		text.Format(
			textYouConsumeToLearnSkillHowManyPointAndGold,
			userPoint,
			usedMoney );

		mReportButton.SetWindowText( text );
	}
	
	// �߰� ������ ��ų�� ǥ���Ѵ�
	{
		mEnableListCtrl.DeleteAllItems();

		// �� �ʿ��� ������ ��ų �ε����� ����ִ�
		const CclientApp::SkillSizeMap& skillSizeMap = mApplication.GetSkillSizeMap();

		for( CclientApp::SkillSizeMap::const_iterator it = skillSizeMap.begin(); skillSizeMap.end() != it; ++it )
		{
			const DWORD index = it->first;

			if( mSkillMap.end() != mSkillMap.find( index ) )
			{
				continue;
			}

			const DWORD row		= mEnableListCtrl.GetItemCount();
			int			step	= 0;

			CString text;
			text.Format( "%d", index );
			mEnableListCtrl.InsertItem( row, text, 0 );

			const SkillScript* script = mApplication.GetSkillScript( index );

			if( ! script )
			{
				ASSERT( 0 );
				continue;
			}

			mEnableListCtrl.SetItemText( row, ++step, script->mName );

			if( script->mIsPassive )
			{
				mEnableListCtrl.SetItemText( row, ++step, "*" );
			}
		}
	}
}


void CSkillDialog::OnLvnItemchangedSkillEnableList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	// ����
	{
		const CString index = mEnableListCtrl.GetItemText( pNMLV->iItem, 0 );

		mEnableSkillStatic.SetWindowText( mApplication.GetTip( atoi( index ) ) );
	}	
}


void CSkillDialog::OnLvnItemchangedSkillMyList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	*pResult = 0;

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	const DWORD row = pNMLV->iItem;

	// ��ų�� �ִ� ���� ������ŭ �޺� �ڽ��� �����Ѵ�
	{
		const CString index = mMyListCtrl.GetItemText( row, 1 );

		const DWORD size = mApplication.GetSkillSize( atoi( index ) );
		CString text;

		mLevelCombo.ResetContent();

		for( DWORD i = 1; i <= size; i++ )
		{
			text.Format( "%d", i );
			mLevelCombo.AddString( text );
		}
	}

	// ���� ������ �����س��´�
	{
		const CString level = mMyListCtrl.GetItemText( row, 3 );

		mLevelCombo.SetCurSel( atoi( level ) - 1 );
	}

	// ����
	{
		const CString index = mMyListCtrl.GetItemText( pNMLV->iItem, 1 );

		mMySkillStatic.SetWindowText( mApplication.GetTip( atoi( index ) ) );
	}
}


void CSkillDialog::OnBnClickedSkillLogButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CSkillLogDialog* dialog = mApplication.GetSkillLogDialog();
	ASSERT( dialog );

	dialog->DoModal( mApplication.GetServerName( mServerIndex ), mPlayerName, mPlayerIndex );
}


void CSkillDialog::OnBnClickedSkillReportButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CSkillReportDialog dialog( mApplication );

	dialog.DoModal();
}