// QuantityDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "QuantityDialog.h"
#include ".\quantitydialog.h"


// CQuantityDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CQuantityDialog, CDialog)
CQuantityDialog::CQuantityDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CQuantityDialog::IDD, pParent)
{
	EnableAutomation();

}

CQuantityDialog::~CQuantityDialog()
{
}

void CQuantityDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}

void CQuantityDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUANTITY_COMBO, mQuantityCombo);
}


BEGIN_MESSAGE_MAP(CQuantityDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQuantityDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IQuantityDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {274BE6CD-CD99-4766-A494-7D8A2C249616}
static const IID IID_IQuantityDialog =
{ 0x274BE6CD, 0xCD99, 0x4766, { 0xA4, 0x94, 0x7D, 0x8A, 0x2C, 0x24, 0x96, 0x16 } };

BEGIN_INTERFACE_MAP(CQuantityDialog, CDialog)
	INTERFACE_PART(CQuantityDialog, IID_IQuantityDialog, Dispatch)
END_INTERFACE_MAP()


// CQuantityDialog �޽��� ó�����Դϴ�.

void CQuantityDialog::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString quantity;

	//mQuantityCombo.GetLBText( mQuantityCombo.GetCurSel(), quantity );
	mQuantityCombo.GetWindowText( quantity );
	
	mQuantity = atoi( quantity );

	OnOK();
}


BOOL CQuantityDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// combo box
	{
		CRect rect;

		mQuantityCombo.GetWindowRect( rect );
		mQuantityCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

		CString text;

		for( int i = 1; MAX_YOUNGYAKITEM_DUPNUM >= i; ++i )
		{			
			text.Format( "%d", i );

			mQuantityCombo.AddString( text );
		}

		mQuantityCombo.SetCurSel( 0 );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


int CQuantityDialog::GetQuantity() const
{
	return mQuantity;
}