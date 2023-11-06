// SkillReportDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "client.h"
#include "SkillReportDialog.h"
#include "SkillDialog.h"


// CSkillReportDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSkillReportDialog, CDialog)
CSkillReportDialog::CSkillReportDialog( CclientApp& application, CWnd* pParent /*=NULL*/)
	: CDialog(CSkillReportDialog::IDD, pParent)
	, mApplication( application )
{
	EnableAutomation();
}


CSkillReportDialog::~CSkillReportDialog()
{}


void CSkillReportDialog::OnFinalRelease()
{
	// �ڵ�ȭ ��ü�� ���� ������ ������ �����Ǹ� OnFinalRelease��
	// ȣ��˴ϴ�. �⺻ Ŭ�������� �ڵ����� ��ü�� �����մϴ�.
	// �⺻ Ŭ������ ȣ���ϱ� ���� ��ü�� �ʿ��� �߰� ���� �۾���
	// �߰��Ͻʽÿ�.

	CDialog::OnFinalRelease();
}


void CSkillReportDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKILL_REPORT_LIST_CTRL, mReportListCtrl);
}


BEGIN_MESSAGE_MAP(CSkillReportDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSkillReportDialog, CDialog)
END_DISPATCH_MAP()

// ����: IID_ISkillReportDialog�� ���� ������ �߰��Ͽ�
// VBA���� ���� �������� ���ε��� �����մϴ�. 
// �� IID�� .IDL ���Ͽ� �ִ� dispinterface�� GUID�� ��ġ�ؾ� �մϴ�.

// {4D618123-CCB0-461D-8637-EFCB2E430741}
static const IID IID_ISkillReportDialog =
{ 0x4D618123, 0xCCB0, 0x461D, { 0x86, 0x37, 0xEF, 0xCB, 0x2E, 0x43, 0x7, 0x41 } };

BEGIN_INTERFACE_MAP(CSkillReportDialog, CDialog)
	INTERFACE_PART(CSkillReportDialog, IID_ISkillReportDialog, Dispatch)
END_INTERFACE_MAP()



BOOL CSkillReportDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// ����Ʈ ��Ʈ�� �ʱ�ȭ
	{
		const char* separator = "................................................";

		{
			const DWORD addStyle = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
			CRect rect;

			{
				mReportListCtrl.GetWindowRect( rect );

				mReportListCtrl.InsertColumn( 0, "",	LVCFMT_LEFT, int( rect.Width() * 0.4f ) );
				mReportListCtrl.InsertColumn( 1, "",	LVCFMT_LEFT, int( rect.Width() * 0.13f ) );
				mReportListCtrl.InsertColumn( 2, "",	LVCFMT_LEFT, int( rect.Width() * 0.17f ) );
				mReportListCtrl.InsertColumn( 3, "",	LVCFMT_LEFT, int( rect.Width() * 0.3f ) );
				mReportListCtrl.SetExtendedStyle( mReportListCtrl.GetExtendedStyle() | addStyle );

				// ���
				{
					const DWORD row = mReportListCtrl.GetItemCount();

					CString textName;
					textName.LoadString( IDS_STRING20 );
					mReportListCtrl.InsertItem( row, textName, 0 );

					CString textLevel;
					textLevel.LoadString( IDS_STRING5 );
					mReportListCtrl.SetItemText( row, 1, textLevel );

					CString textSkillPoint;
					textSkillPoint.LoadString( IDS_STRING96 );
					mReportListCtrl.SetItemText( row, 2, textSkillPoint );

					CString textMoney;
					textMoney.LoadString( IDS_STRING199 );
					mReportListCtrl.SetItemText( row, 3, textMoney );
				}
				
				// ���м�
				{
					const DWORD row = mReportListCtrl.GetItemCount();

					mReportListCtrl.InsertItem( row, separator, 0 );
					mReportListCtrl.SetItemText( row, 1, separator );
					mReportListCtrl.SetItemText( row, 2, separator );
					mReportListCtrl.SetItemText( row, 3, separator );
				}
			}
		}

		const CSkillDialog* dialog = mApplication.GetSkillDialog();
		ASSERT( dialog );

		const CSkillDialog::SkillMap& skillMap = dialog->mSkillMap;

		
		DWORD usedMoney = 0;
		DWORD usedPoint = 0;

		for(
			CSkillDialog::SkillMap::const_iterator it = skillMap.begin();
			skillMap.end() != it;
			++it )
		{
			const SKILL_BASE& skill = it->second;

			for( int level = 1; level <= skill.Level; ++level )
			{
				const SkillScript* script = mApplication.GetSkillScript( skill.wSkillIdx / 100 * 100 + level );

				if( ! script )
				{
					continue;
				}

				{
					const DWORD row = mReportListCtrl.GetItemCount();

					mReportListCtrl.InsertItem( row, script->mName, 0 );

					CString text;
					text.Format( "%d", level );
					mReportListCtrl.SetItemText( row, 1, text );

					text.Format( "%d", script->mPoint );
					mReportListCtrl.SetItemText( row, 2, text );

					text.Format( "%d", script->mMoney );
					mReportListCtrl.SetItemText( row, 3, text );
				}

				{
					usedMoney += script->mMoney;
					usedPoint += script->mPoint;
				}
			}
		}

		if( skillMap.empty() )
		{
			const DWORD row = mReportListCtrl.GetItemCount();

			CString textEmpty;
			textEmpty.LoadString( IDS_STRING203 );

			mReportListCtrl.InsertItem( row, textEmpty, 0 );
		}

		// ���м�
		{
			const DWORD row = mReportListCtrl.GetItemCount();

			mReportListCtrl.InsertItem( row, separator, 0 );
			mReportListCtrl.SetItemText( row, 1, separator );
			mReportListCtrl.SetItemText( row, 2, separator );
			mReportListCtrl.SetItemText( row, 3, separator );
		}

		// ���
		{
			const DWORD row = mReportListCtrl.GetItemCount();

			CString textSum;
			textSum.LoadString( IDS_STRING204 );

			mReportListCtrl.InsertItem( row, textSum, 0 );

			CString text;
			text.Format( "%d", usedPoint );
			mReportListCtrl.SetItemText( row, 2, text );

			text.Format( "%d", usedMoney );
			mReportListCtrl.SetItemText( row, 3, text );
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}