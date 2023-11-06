//  DelCharView.h.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "DelCharView.h"
#include "DelChar.h"
#include "DelCharDoc.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DelCharView.h.cpp : 구현 파일입니다.
//
IMPLEMENT_DYNCREATE(CDelCharView, CFormView)

BEGIN_MESSAGE_MAP(CDelCharView, CFormView)
	//{{AFX_MSG_MAP(CDS_RMToolView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CDelCharView::CDelCharView() : CFormView(CDelCharView::IDD)
{
	//{{AFX_DATA_INIT(CMunpaView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSheet = NULL;
	m_pPageList = NULL;
}


CDelCharView::~CDelCharView()
{
	SAFE_DELETE(m_pSheet);
	SAFE_DELETE(m_pPageList);
}

void CDelCharView::DoDataExchange(CDataExchange* pDX)
{

	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelCharView)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}

/////////////////////////////////////////////////////////////////////////////
// CAdminView diagnostics

// CDelCharView 진단입니다.

#ifdef _DEBUG
void CDelCharView::AssertValid() const
{
	CView::AssertValid();
}

void CDelCharView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CDelCharView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_DEL_CHAR_SHEET);
	m_pSheet = new CLDPropSheet;

	// page link
	CDelChar*	pPage = new CDelChar;
	m_pPageList = pPage;

	if( m_pPageList != NULL )
		m_pSheet->AddPage(m_pPageList);

	//
	if( !m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0) ) 
	{
		SAFE_DELETE(m_pSheet);
		SAFE_DELETE( m_pPageList);
		return;
	}

	CDelCharDoc* pDoc = GetDocument();
	if( pDoc == NULL )
	{
		SAFE_DELETE(m_pSheet);
		SAFE_DELETE( m_pPageList );
		return;
	}
	m_pSheet->SetActivePage( 0 );
	if( m_pPageList != NULL )
		m_pPageList->InitPage( (void*)pDoc );

	//
	m_pSheet->SetActivePage(0);

	DWORD style = m_pSheet->GetTabControl()->GetStyle();

// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 1024, 700, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(),
	SWP_NOZORDER | SWP_NOACTIVATE);
//

}

CDelCharDoc* CDelCharView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDelCharDoc)));
	return (CDelCharDoc*)m_pDocument;
}

void CDelCharView::SetDelCharList( DWORD Count, TMSG_DELCHARLISRESULT* pMsg )
{
	CDelChar* pPage = (CDelChar*)m_pPageList;
	if( pPage )
		pPage->SetDelCharList( Count, pMsg );	
}