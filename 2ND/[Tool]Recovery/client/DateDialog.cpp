// DateDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "DateDialog.h"
#include ".\datedialog.h"


// CDateDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC( CDateDialog, CDialog)
CDateDialog::CDateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDateDialog::IDD, pParent)
{
	EnableAutomation();
}

CDateDialog::~CDateDialog()
{}

void CDateDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.
	
	CDialog::OnFinalRelease();
}

void CDateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_PREVIOUS_DATA_STATIC, mPreviousDateStatic);
	DDX_Control(pDX, IDC_DATE_YEAR_COMBO, mYearCombo);
	DDX_Control(pDX, IDC_DATE_MONTH_COMBO, mMonthCombo);
	DDX_Control(pDX, IDC_DATE_DAY_COMBO, mDayCombo);
	DDX_Control(pDX, IDC_DATE_HOUR_COMBO, mHourCombo);
	DDX_Control(pDX, IDC_DATE_MINUTE_COMBO, mMinuteCombo);
	DDX_Control(pDX, IDC_DATE_SECOND_COMBO, mSecondCombo);
}


BEGIN_MESSAGE_MAP(CDateDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDateDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_IDateDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {6D446E54-0FAF-4065-B28D-B21497A84B11}
static const IID IID_IDateDialog =
{ 0x6D446E54, 0xFAF, 0x4065, { 0xB2, 0x8D, 0xB2, 0x14, 0x97, 0xA8, 0x4B, 0x11 } };

BEGIN_INTERFACE_MAP(CDateDialog, CDialog)
	INTERFACE_PART(CDateDialog, IID_IDateDialog, Dispatch)
END_INTERFACE_MAP()


// CDateDialog �޽��� ó�����Դϴ�.

BOOL CDateDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	// ��¥ �޺� ����
	{
		const DWORD minute	= 60;
		const DWORD hour	= 60 * minute;
		const DWORD day		= 24 * hour;
		const DWORD month	= 30 * day;
		const DWORD year	= 12 * month;

		CRect	rect;
		CString text;

		{
			mYearCombo.GetWindowRect( rect );
			mYearCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxYear = 5;

			for( int i = 0; i <= maxYear; ++i )
			{
				text.Format( "%d", i );

				mYearCombo.AddString( text );
			}

			mYearCombo.SetCurSel( mSecond / year );

			mSecond %= year;
		}

		{
			mMonthCombo.GetWindowRect( rect );
			mMonthCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxMonth = 12;

			for( int i = 0; i <= maxMonth; ++i )
			{
				text.Format( "%d", i );

				mMonthCombo.AddString( text );
			}

			mMonthCombo.SetCurSel( mSecond / month );

			mSecond %= month;
		}

		{
			mDayCombo.GetWindowRect( rect );
			mDayCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxDay = 30;

			for( int i = 0; i <= maxDay; ++i )
			{
				text.Format( "%d", i );

				mDayCombo.AddString( text );
			}

			mDayCombo.SetCurSel( mSecond / day );

			mSecond %= day;
		}

		{
			mHourCombo.GetWindowRect( rect );
			mHourCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxHour = 24;

			for( int i = 0; i <= maxHour; ++i )
			{
				text.Format( "%d", i );

				mHourCombo.AddString( text );
			}

			mHourCombo.SetCurSel( mSecond / hour );

			mSecond %= hour;
		}

		{
			mMinuteCombo.GetWindowRect( rect );
			mMinuteCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxMinute = 60;

			for( int i = 0; i <= maxMinute; ++i )
			{
				text.Format( "%d", i );

				mMinuteCombo.AddString( text );
			}

			mMinuteCombo.SetCurSel( mSecond / minute );

			mSecond %= minute;
		}

		{
			mSecondCombo.GetWindowRect( rect );
			mSecondCombo.SetWindowPos( 0, 0, 0, rect.Width(), 200, SWP_NOMOVE | SWP_NOZORDER );

			const int maxSecond = 60;

			for( int i = 0; i <= maxSecond; ++i )
			{
				text.Format( "%d", i );

				mSecondCombo.AddString( text );
			}

			mSecondCombo.SetCurSel( mSecond );
		}
	}

	{
		CString text;
		text.LoadString( IDS_DATE_DIALOG_TEXT1 );

		mPreviousDateStatic.SetWindowText( text + mDateString );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


INT_PTR CDateDialog::DoModal( DWORD second, const CString& dateString )
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	mSecond		= second;
	mDateString	= dateString;

	return CDialog::DoModal();
}


void CDateDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	const DWORD second	= 1;
	const DWORD minute	= 60 * second;
	const DWORD hour	= 60 * minute;
	const DWORD day		= 24 * hour;
	const DWORD month	= 30 * day;
	const DWORD year	= 12 * month;
	
	CString text;
	mSecond	= 0;

	mYearCombo.GetLBText( mYearCombo.GetCurSel(), text );
	mSecond += atoi( text ) * year;

	mMonthCombo.GetLBText( mMonthCombo.GetCurSel(), text );
	mSecond += atoi( text ) * month;

	mDayCombo.GetLBText( mDayCombo.GetCurSel(), text );
	mSecond += atoi( text ) * day;

	mHourCombo.GetLBText( mHourCombo.GetCurSel(), text );
	mSecond += atoi( text ) * hour;

	mMinuteCombo.GetLBText( mMinuteCombo.GetCurSel(), text );
	mSecond += atoi( text ) * minute;

	mSecondCombo.GetLBText( mSecondCombo.GetCurSel(), text );
	mSecond += atoi( text );

	CDialog::OnOK();
}


DWORD CDateDialog::GetSecond() const
{
	return mSecond;
}