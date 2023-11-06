// OperatorDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "OperatorDialog.h"
#include ".\operatordialog.h"


// COperatorDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COperatorDialog, CDialog)
COperatorDialog::COperatorDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(COperatorDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();
}


COperatorDialog::~COperatorDialog()
{}


void COperatorDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}


void COperatorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPERATOR_LIST_CTRL, mOperatorListCtrl);
	DDX_Control(pDX, IDC_OPERATOR_ID_EDIT, mIdEdit);
	DDX_Control(pDX, IDC_OPERATOR_NAME_EDIT, mNameEdit);
	DDX_Control(pDX, IDC_OPERATOR_AUTH_COMBO, mRankCombo);
	DDX_Control(pDX, IDC_OPERATOR_ADD_BUTTON, mAddButton);
	DDX_Control(pDX, IDC_OPERATOR_IPADDRESS, mIpAddressCtrl);
	DDX_Control(pDX, IDC_IP_LIST_CTRL, mIpListCtrl);
	DDX_Control(pDX, IDC_OPERATOR_ADD_ID_EDIT, mAddIdEdit);
	DDX_Control(pDX, IDC_OPERATOR_ADD_NAME_EDIT, mAddNameEdit);
	DDX_Control(pDX, IDC_OPERATOR_RANK_STATIC, mRankStatic);
}


BEGIN_MESSAGE_MAP(COperatorDialog, CDialog)
	//ON_BN_CLICKED(IDC_OPERATOR_REMOVE_BUTTON, OnBnClickedOperatorRemoveButton)
	ON_BN_CLICKED(IDC_OPERATOR_ADD_BUTTON, OnBnClickedOperatorAddButton)
	ON_BN_CLICKED(IDC_OPERATOR_IP_ADD_BUTTON, OnBnClickedOperatorIpAddButton)
	ON_BN_CLICKED(IDC_OPERATOR_IP_REMOVE_BUTTON, OnBnClickedOperatorIpRemoveButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OPERATOR_LIST_CTRL, OnLvnItemchangedOperatorListCtrl)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_IP_LIST_CTRL, OnLvnItemchangedIpListCtrl)
	ON_BN_CLICKED(IDC_OPERATOR_UPDATE_BUTTON, OnBnClickedOperatorUpdateButton)
	ON_CBN_SELCHANGE(IDC_OPERATOR_AUTH_COMBO, OnCbnSelchangeOperatorAuthCombo)
	ON_BN_CLICKED(IDC_OPERATOR_REMOVE_BUTTON, OnBnClickedOperatorRemoveButton)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(COperatorDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IOperatorDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {3A19ACEC-EFED-4BF4-AA74-7C78F2B176CF}
static const IID IID_IOperatorDialog =
{ 0x3A19ACEC, 0xEFED, 0x4BF4, { 0xAA, 0x74, 0x7C, 0x78, 0xF2, 0xB1, 0x76, 0xCF } };

BEGIN_INTERFACE_MAP(COperatorDialog, CDialog)
	INTERFACE_PART(COperatorDialog, IID_IOperatorDialog, Dispatch)
END_INTERFACE_MAP()


// COperatorDialog �޽��� ó�����Դϴ�.


BOOL COperatorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// list control
	{
		const DWORD style = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;

		{
			CRect	rect;
			int		step = -1;

			mOperatorListCtrl.GetWindowRect( &rect );

			CString textIndex;
			textIndex.LoadString( IDS_STRING22 );

			mOperatorListCtrl.InsertColumn( ++step, textIndex,			LVCFMT_LEFT, int( rect.Width() * 0.0f ) );

			CString textId;
			textId.LoadString( IDS_STRING139 );
			mOperatorListCtrl.InsertColumn( ++step, textId,				LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textName;
			textName.LoadString( IDS_STRING20 );
			mOperatorListCtrl.InsertColumn( ++step, textName,			LVCFMT_LEFT, int( rect.Width() * 0.25f ) );

			CString textRankCode;
			textRankCode.LoadString( IDS_STRING4 );
			mOperatorListCtrl.InsertColumn( ++step, textRankCode,		LVCFMT_LEFT, int( rect.Width() * 0.0f ) );

			CString textRank;
			textRank.LoadString( IDS_STRING43 );
			mOperatorListCtrl.InsertColumn( ++step, textRank,			LVCFMT_LEFT, int( rect.Width() * 0.15f ) );

			CString textConnectedIp;
			textConnectedIp.LoadString( IDS_STRING140 );
			mOperatorListCtrl.InsertColumn( ++step, textConnectedIp,	LVCFMT_LEFT, int( rect.Width() * 0.4f ) );

			CString textDate;
			textDate.LoadString( IDS_STRING44 );
			mOperatorListCtrl.InsertColumn( ++step, textDate,			LVCFMT_LEFT, int( rect.Width() * 0.4f ) );

			mOperatorListCtrl.SetExtendedStyle( mOperatorListCtrl.GetExtendedStyle() | style );
		}

		{
			CRect	rect;
			int		step = -1;

			mIpListCtrl.GetWindowRect( &rect );

			mIpListCtrl.InsertColumn( ++step, "IP",	LVCFMT_LEFT, rect.Width() );

			mIpListCtrl.SetExtendedStyle( mIpListCtrl.GetExtendedStyle() | style );
		}	
	}

	// initialize combo box
	{
		CRect rect;

		mRankCombo.SetCurSel( 0 );
		mRankCombo.GetWindowRect( rect );
		mRankCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		//mRankCombo.AddString( mApplication.GetText( MSG_RM_OPERATOR::Operator::None ) );
		//mRankCombo.AddString( mApplication.GetText( MSG_RM_OPERATOR::Operator::Monitor ) );
		//mRankCombo.AddString( mApplication.GetText( MSG_RM_OPERATOR::Operator::Staff ) );
		//mRankCombo.AddString( mApplication.GetText( MSG_RM_OPERATOR::Operator::Manager ) );

		// add string by enum value order

		mRankCombo.InsertString( eGM_POWER_MASTER,		mApplication.GetText( eGM_POWER_MASTER		) );
		mRankCombo.InsertString( eGM_POWER_MONITOR,		mApplication.GetText( eGM_POWER_MONITOR		) );
		mRankCombo.InsertString( eGM_POWER_PATROLLER,	mApplication.GetText( eGM_POWER_PATROLLER	) );
		mRankCombo.InsertString( eGM_POWER_AUDITOR,		mApplication.GetText( eGM_POWER_AUDITOR		) );
		mRankCombo.InsertString( eGM_POWER_EVENTER,		mApplication.GetText( eGM_POWER_AUDITOR		) );
	}

	// edit control
	{
		mIdEdit.SetLimitText( MAX_NAME_LENGTH );
		mNameEdit.SetLimitText( MAX_NAME_LENGTH );

		mAddIdEdit.SetLimitText( MAX_NAME_LENGTH );
		mAddNameEdit.SetLimitText( MAX_NAME_LENGTH );
	}

	// static contorl
	{
		mRankStatic.SetWindowText( "" );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void COperatorDialog::Parse( DWORD serverIndex, const MSGROOT* message )
{
	mServerIndex = serverIndex;

	switch( message->Protocol )
	{
	case MP_RM_OPERATOR_GET_ACK:
		{
			const MSG_RM_OPERATOR* m = ( MSG_RM_OPERATOR* )message;

			for( size_t i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_OPERATOR::Operator& data = m->mOperator[ i ];

				const int row = mOperatorListCtrl.GetItemCount();

				{
					CString text;
					text.Format( "%d", data.mIndex );
					mOperatorListCtrl.InsertItem( row, text, 0 );
				}				

				mOperatorListCtrl.SetItemText( row, 1, data.mId );
				mOperatorListCtrl.SetItemText( row, 2, data.mName );

				{
					CString text;
					text.Format( "%d", data.mPower );
					mOperatorListCtrl.SetItemText( row, 3, text );
				}				
				
				mOperatorListCtrl.SetItemText( row, 4, mApplication.GetText( data.mPower ) );
				mOperatorListCtrl.SetItemText( row, 5, data.mIp );
				mOperatorListCtrl.SetItemText( row, 6, data.mRegistedDate );
			}
			
			break;
		}
	case MP_RM_OPERATOR_IP_GET_ACK:
		{
			const MSG_RM_OPERATOR_IP* m = ( MSG_RM_OPERATOR_IP* )message;

			for( size_t i = 0; i < m->mSize; ++i )
			{
				const MSG_RM_OPERATOR_IP::Address& address = m->mAddress[ i ];

				const int row = mOperatorListCtrl.GetItemCount();

				mIpListCtrl.InsertItem( row, address.mIp, 0 );
			}
			
			break;
		}
	case MP_RM_OPERATOR_NACK_BY_AUTH:
		{
			ShowWindow( SW_HIDE );

			CString textYouHaveNoAuthority;
			textYouHaveNoAuthority.LoadString( IDS_STRING18 );

			MessageBox( textYouHaveNoAuthority, "", MB_OK | MB_ICONERROR );
			break;
		}
	case MP_RM_OPERATOR_ADD_ACK:
		{
			const MSG_RM_OPERATOR* m = ( MSG_RM_OPERATOR* )message;			
			const MSG_RM_OPERATOR::Operator& data = m->mOperator[ 0 ];

			// add to list control
			{
				const int row = mOperatorListCtrl.GetItemCount();

				CString text;
				text.Format( "%d", data.mIndex );

				mOperatorListCtrl.InsertItem( row, text, 0 );

				mOperatorListCtrl.SetItemText( row, 1, data.mId );
				mOperatorListCtrl.SetItemText( row, 2, data.mName );

				{
					CString text;
					text.Format( "%d", data.mPower );
					mOperatorListCtrl.SetItemText( row, 3, text );
				}				

				mOperatorListCtrl.SetItemText( row, 4, mApplication.GetText( data.mPower ) );
				mOperatorListCtrl.SetItemText( row, 5, data.mIp );
				mOperatorListCtrl.SetItemText( row, 6, data.mRegistedDate );
			}

			CString textAddingForOperatorIsSucceed;
			textAddingForOperatorIsSucceed.LoadString( IDS_STRING141 );

			MessageBox( textAddingForOperatorIsSucceed, "", MB_OK | MB_ICONINFORMATION );
			break;
		}
	case MP_RM_OPERATOR_REMOVE_ACK:
		{
			const MSGBASE* m = ( MSGBASE* )message;
			
			for( int row = 0; row < mOperatorListCtrl.GetItemCount(); ++row )
			{
				if( m->dwObjectID == atoi( mOperatorListCtrl.GetItemText( row, 0 ) ) )
				{
					mOperatorListCtrl.DeleteItem( row );
					break;
				}
			}

			mNameEdit.SetWindowText( "" );
			mIdEdit.SetWindowText( "" );

			mRankCombo.SetCurSel( -1 );

			break;
		}
	case MP_RM_OPERATOR_ADD_NACK_BY_ID:
		{
			CString textSameIsExisted;
			textSameIsExisted.LoadString( IDS_STRING142 );;

			MessageBox( textSameIsExisted, "", MB_OK | MB_ICONERROR );
			break;
		}
	case MP_RM_OPERATOR_SET_ACK:
		{
			const MSG_DWORD* m = ( MSG_DWORD* )message;
			
			for( int row = 0; row < mOperatorListCtrl.GetItemCount(); ++row )
			{
				if( atoi( mOperatorListCtrl.GetItemText( row, 0 ) ) == m->dwObjectID )
				{
					CString text;
					text.Format( "%d", m->dwData );
					mOperatorListCtrl.SetItemText(
						row,
						3,
						text );

					mOperatorListCtrl.SetItemText(
						row,
						4,
						mApplication.GetText( eGM_POWER( m->dwData ) ) );
					break;
				}
			}


			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_OK | MB_ICONINFORMATION );
			break;
		}
	case MP_RM_OPERATOR_IP_ADD_ACK:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			mIpListCtrl.InsertItem( mIpListCtrl.GetItemCount(), m->Name, 0 );
		
			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_OK | MB_ICONINFORMATION );
			break;
		}
	case MP_RM_OPERATOR_IP_ADD_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateWasFailed, "", MB_OK | MB_ICONERROR );
			break;
		}
	case MP_RM_OPERATOR_IP_REMOVE_ACK:
		{
			const MSG_NAME* m = ( MSG_NAME* )message;

			for( int row = 0; row < mIpListCtrl.GetItemCount(); ++row )
			{
				if( mIpListCtrl.GetItemText( row, 0 ) == m->Name )
				{
					mIpListCtrl.DeleteItem( row );
					break;
				}
			}

			CString textUpdateWasSucceed;
			textUpdateWasSucceed.LoadString( IDS_STRING28 );

			MessageBox( textUpdateWasSucceed, "", MB_OK | MB_ICONINFORMATION );
			break;
		}
	case MP_RM_OPERATOR_IP_REMOVE_NACK:
		{
			CString textUpdateWasFailed;
			textUpdateWasFailed.LoadString( IDS_STRING29 );

			MessageBox( textUpdateWasFailed, "", MB_OK | MB_ICONERROR );
			break;
		}
	default:
		{
			ASSERT( 0 );
			break;
		}
	}
}


void COperatorDialog::Request( DWORD serverIndex )
{
	{
		mServerIndex = serverIndex;
	}
	
	{
		mIpListCtrl.DeleteAllItems();
		mOperatorListCtrl.DeleteAllItems();

		mAddIdEdit.SetWindowText( "" );
		mAddNameEdit.SetWindowText( "" );
		mRankCombo.SetCurSel( 0 );

		mRankStatic.SetWindowText( "" );
	}

	{
		MSGROOT message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_GET_SYN;

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}





void COperatorDialog::OnBnClickedOperatorUpdateButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int row;
	{
		POSITION position = mOperatorListCtrl.GetFirstSelectedItemPosition();

		row = mOperatorListCtrl.GetNextSelectedItem( position );
	}

	{
		MSG_DWORD message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_SET_SYN;
		message.dwObjectID	= atoi( mOperatorListCtrl.GetItemText( row, 0 ) );
		message.dwData		= mRankCombo.GetCurSel();

		if( atoi( mOperatorListCtrl.GetItemText( row, 3 ) ) == message.dwData )
		{
			return;
		}

		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void COperatorDialog::OnBnClickedOperatorAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	MSG_RM_OPERATOR message;
	message.Category	= MP_RM_OPERATOR;
	message.Protocol	= MP_RM_OPERATOR_ADD_SYN;
	message.mSize		= 1;

	CString id;
	{
		mAddIdEdit.GetWindowText( id );

		if( id.IsEmpty() )
		{
			CString textNameOrIdIsEmpty;
			textNameOrIdIsEmpty.LoadString( IDS_STRING143 );

			MessageBox( textNameOrIdIsEmpty, "", MB_ICONERROR | MB_OK );
			return;
		}
		else if( MAX_ROW_NUM == mOperatorListCtrl.GetItemCount() )
		{
			CString textYouCanNotAddAnyMore;
			textYouCanNotAddAnyMore.LoadString( IDS_STRING220 );

			MessageBox( textYouCanNotAddAnyMore, "", MB_ICONERROR | MB_OK );
			return;
		}

		for(
			const char* ch = id.GetString();
			*ch;
			++ch )
		{
			if(		IsDBCSLeadByte( *ch ) ||
				!	isalnum( *ch ) )
			{
				CString textItIsAllowEnglishOrNumericOnly;
				textItIsAllowEnglishOrNumericOnly.LoadString( IDS_STRING44 );

				MessageBox( textItIsAllowEnglishOrNumericOnly, "", MB_ICONERROR | MB_OK );
				return;
			}
		}
	}	

	CString name;
	{
		mAddNameEdit.GetWindowText( name );

		if( name.IsEmpty() )
		{
			CString textNameOrIdIsEmpty;
			textNameOrIdIsEmpty.LoadString( IDS_STRING143 );

			MessageBox( textNameOrIdIsEmpty, "", MB_ICONERROR | MB_OK );
			return;
		}
	}	

	for( int row = 0; row < mOperatorListCtrl.GetItemCount(); ++row )
	{
		if( id == mOperatorListCtrl.GetItemText( row, 1 ) )
		{
			CString textSameIdExist;
			textSameIdExist.LoadString( IDS_STRING142 );

			MessageBox( textSameIdExist, "", MB_ICONERROR | MB_OK );
			return;
		}
	}

	MSG_RM_OPERATOR::Operator& data = message.mOperator[ 0 ];
	{
		ZeroMemory( &data, sizeof( data ) );

		CString text;

		mAddIdEdit.GetWindowText( text );
		_tcscpy( data.mId, text );

		mAddNameEdit.GetWindowText( text );
		_tcscpy( data.mName, text );
	}

	mApplication.Send( mServerIndex, message, message.GetSize() );
}


void COperatorDialog::OnBnClickedOperatorIpAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	BYTE field0;
	BYTE field1;
	BYTE field2;
	BYTE field3;

	mIpAddressCtrl.GetAddress( field0, field1, field2, field3 );

	if( ! field0 &&
		! field1 &&
		! field2 &&
		! field3 )
	{
		CString textItIsWrongAddress;
		textItIsWrongAddress.LoadString( IDS_STRING145 );

		MessageBox( textItIsWrongAddress, "", MB_ICONERROR | MB_OK );
		return;
	}

	CString ip;
	ip.Format( "%d.%d.%d.%d", field0, field1, field2, field3 );

	for( int row = 0; row < mIpListCtrl.GetItemCount(); ++row )
	{
		if( ip == mIpListCtrl.GetItemText( row, 0 ) )
		{
			CString textItIsRegistedIpAddressAlready;
			textItIsRegistedIpAddressAlready.LoadString( IDS_STRING146 );

			MessageBox( textItIsRegistedIpAddressAlready, "", MB_ICONERROR | MB_OK );
			return;
		}
	}

	{
		MSG_NAME message;
		ZeroMemory( &message, sizeof( message ) );

		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_ADD_SYN;

		_tcscpy( message.Name, ip );
		
		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void COperatorDialog::OnBnClickedOperatorIpRemoveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	BYTE field0;
	BYTE field1;
	BYTE field2;
	BYTE field3;

	mIpAddressCtrl.GetAddress( field0, field1, field2, field3 );

	if( ! field0 &&
		! field1 &&
		! field2 &&
		! field3 )
	{
		CString textItIsWrongAddress;
		textItIsWrongAddress.LoadString( IDS_STRING145 );

		MessageBox( textItIsWrongAddress, "", MB_ICONERROR | MB_OK );
		return;
	}

	{
		MSG_NAME message;
		message.Category	= MP_RM_OPERATOR;
		message.Protocol	= MP_RM_OPERATOR_IP_REMOVE_SYN;

		sprintf( message.Name, "%d.%d.%d.%d", field0, field1, field2, field3 );
		
		mApplication.Send( mServerIndex, message, sizeof( message ) );
	}
}


void COperatorDialog::OnLvnItemchangedOperatorListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	int row;
	{
		POSITION position = mOperatorListCtrl.GetFirstSelectedItemPosition();

		row = mOperatorListCtrl.GetNextSelectedItem( position );
	}

	{
		mIdEdit.SetWindowText( mOperatorListCtrl.GetItemText( row, 1 ) );
		mNameEdit.SetWindowText( mOperatorListCtrl.GetItemText( row, 2 ) );

		mRankCombo.SetCurSel( atoi( mOperatorListCtrl.GetItemText( row, 3 ) ) );
	}

	OnCbnSelchangeOperatorAuthCombo();
}


void COperatorDialog::OnLvnItemchangedIpListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	int row;
	{
		POSITION position = mIpListCtrl.GetFirstSelectedItemPosition();

		row = mIpListCtrl.GetNextSelectedItem( position );
	}

	mIpAddressCtrl.SetWindowText( mIpListCtrl.GetItemText( row, 0 ) );
}


void COperatorDialog::OnCbnSelchangeOperatorAuthCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	switch( mRankCombo.GetCurSel() )
	{
	case eGM_POWER_MONITOR:
		{
			CString textMonitorRankDescription;
			textMonitorRankDescription.LoadString( IDS_STRING148 );
			mRankStatic.SetWindowText( textMonitorRankDescription );
			break;
		}
	case eGM_POWER_PATROLLER:
		{
			CString textStaffRankDescription;
			textStaffRankDescription.LoadString( IDS_STRING149 );

			mRankStatic.SetWindowText( textStaffRankDescription );
			break;
		}
	case eGM_POWER_MASTER:
		{
			CString textManagerRankDescription;
			textManagerRankDescription.LoadString( IDS_STRING150 );

			mRankStatic.SetWindowText( textManagerRankDescription );
			break;
		}
	case eGM_POWER_AUDITOR:
		{
			mRankStatic.SetWindowText(
				"Do not use it now" );
			break;
		}
	case eGM_POWER_EVENTER:
		{
			mRankStatic.SetWindowText(
				"Do not use it now" );
			break;
		}
	default:
		{
			CString textNoneRankDescription;
			textNoneRankDescription.LoadString( IDS_STRING147 );

			mRankStatic.SetWindowText( textNoneRankDescription );
			break;
		}
	}
}


void COperatorDialog::OnBnClickedOperatorRemoveButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	CString name;
	mNameEdit.GetWindowText( name );

	if( name.IsEmpty() )
	{
		return;
	}

	CString textDoYouRemoveit;
	textDoYouRemoveit.LoadString( IDS_STRING108 );

	if( IDNO == MessageBox( textDoYouRemoveit, "", MB_YESNO | MB_ICONWARNING ) )
	{
		return;
	}

	MSGBASE message;
	message.Category	= MP_RM_OPERATOR;
	message.Protocol	= MP_RM_OPERATOR_REMOVE_SYN;

	POSITION	position	= mOperatorListCtrl.GetFirstSelectedItemPosition();
	const int	row			= mOperatorListCtrl.GetNextSelectedItem( position );

	message.dwObjectID	= atoi( mOperatorListCtrl.GetItemText( row, 0 ) );

	mApplication.Send( mServerIndex, message, sizeof( message ) );
}
