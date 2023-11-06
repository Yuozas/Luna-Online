// ExtractSelectedDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "PackExtract.h"
#include "PackExtractDlg.h"
#include "ExtractSelectedDlg.h"
#include ".\extractselecteddlg.h"

extern CPackExtractDlg* g_pPackExtractDlg;

// ExtractSelectedDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(ExtractSelectedDlg, CDialog)
ExtractSelectedDlg::ExtractSelectedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ExtractSelectedDlg::IDD, pParent)
	, m_bClearDir(FALSE)
{
}

ExtractSelectedDlg::~ExtractSelectedDlg()
{
	g_pPackExtractDlg->EndExtractPackFileWithSelectListDlg();
}

void ExtractSelectedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, m_SeletList);
	DDX_Check(pDX, IDC_CLEAR_DIR_CHECK, m_bClearDir);
}


BEGIN_MESSAGE_MAP(ExtractSelectedDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DELTE_BUTTON, OnBnClickedDelteButton)
END_MESSAGE_MAP()


// ExtractSelectedDlg �޽��� ó�����Դϴ�.

BOOL ExtractSelectedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	g_pPackExtractDlg->AddListFromSelectedPackFile( m_SeletList );


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void ExtractSelectedDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCount = m_SeletList.GetSelCount();

	if( nCount )
	{
		UpdateData( TRUE );

		CArray<int,int> aryListBoxSel;

		aryListBoxSel.SetSize( nCount );
		m_SeletList.GetSelItems( nCount, aryListBoxSel.GetData() );

		g_pPackExtractDlg->SetPackProgressRange( nCount );
		g_pPackExtractDlg->StartExtractPackFileWithSelectListDlg( m_bClearDir );

		for( int i=0; i < nCount ; ++i )
		{
			CString strFileName;
			m_SeletList.GetText( aryListBoxSel[i], strFileName );

			g_pPackExtractDlg->ExtractPackFile( (LPSTR)(LPCTSTR)strFileName );

			g_pPackExtractDlg->SetPackProgressPos( i + 1 );
		}

		g_pPackExtractDlg->EndExtractPackFileWithSelectListDlg();

		AfxMessageBox( "�������� �Ϸ�!" );
	}	

	OnOK();
}

void ExtractSelectedDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	g_pPackExtractDlg->EndExtractPackFileWithSelectListDlg();

	OnCancel();
}

void ExtractSelectedDlg::OnBnClickedDelteButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nCount = m_SeletList.GetSelCount();

	if( nCount )
	{
		UpdateData( TRUE );

		CArray<int,int> aryListBoxSel;
		CArray<CString> arySelectedFileName;

		aryListBoxSel.SetSize( nCount );
		m_SeletList.GetSelItems( nCount, aryListBoxSel.GetData() );

		g_pPackExtractDlg->SetPackProgressRange( nCount );
		if( !g_pPackExtractDlg->StartDeletePackFileWithSelectListDlg() )
			return;

		for( int i=0; i < nCount ; ++i )
		{
			CString strFileName;
			m_SeletList.GetText( aryListBoxSel[i], strFileName );

			g_pPackExtractDlg->DeletePackFile( (LPSTR)(LPCTSTR)strFileName );

			g_pPackExtractDlg->SetPackProgressPos( i + 1 );

			arySelectedFileName.Add( strFileName );
		}

		for( int i=0 ; i < arySelectedFileName.GetCount() ; ++i )
		{
			int nIndex = 0;
			nIndex = m_SeletList.FindString( nIndex, arySelectedFileName[i] );

			m_SeletList.DeleteString( nIndex );
		}

		g_pPackExtractDlg->EndDeletePackFileWithSelectListDlg();

		AfxMessageBox( "���� ���� �Ϸ�!" );
	}
}
