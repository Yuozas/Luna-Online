//  PetView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "PetView.h"
#include "Pet.h"
#include "PetDoc.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PetView.cpp : 구현 파일입니다.
//
IMPLEMENT_DYNCREATE(CPetView, CFormView)

BEGIN_MESSAGE_MAP(CPetView, CFormView)
	//{{AFX_MSG_MAP(CDS_RMToolView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CPetView::CPetView() : CFormView(CPetView::IDD)
{
	//{{AFX_DATA_INIT(CMunpaView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSheet = NULL;

	for(int i=0;i<ePetPage_Max;++i)
		m_pPageList[i] = NULL;
}


CPetView::~CPetView()
{
	SAFE_DELETE(m_pSheet);

	for(int i=0;i<ePetPage_Max;++i)
		SAFE_DELETE(m_pPageList[i]);
}

void CPetView::DoDataExchange(CDataExchange* pDX)
{

	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPetView)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}

/////////////////////////////////////////////////////////////////////////////
// CAdminView diagnostics

// CPetView 진단입니다.

#ifdef _DEBUG
void CPetView::AssertValid() const
{
	CView::AssertValid();
}

void CPetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

void CPetView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_PET_SHEET);
	m_pSheet = new CLDPropSheet;

	// page link
	CPet*	pPage = new CPet;
	m_pPageList[ePetPage_PetInfo] = pPage;

	for(int i=0;i<ePetPage_Max;++i)
	{
		if( m_pPageList[i] != NULL )
			m_pSheet->AddPage(m_pPageList[i]);
	}
	//
	if( !m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0) ) 
	{
		SAFE_DELETE(m_pSheet);
		for( int i = 0; i < ePetPage_Max; ++i )
			SAFE_DELETE( m_pPageList[i] );
		return;
	}

	CPetDoc* pDoc = GetDocument();
	if( pDoc == NULL )
	{
		SAFE_DELETE(m_pSheet);
		for( int i = 0; i < ePetPage_Max; ++i )
			SAFE_DELETE( m_pPageList[i] );
		return;
	}
	for( i = ePetPage_Max-1; i >= 0; --i )
	{
		m_pSheet->SetActivePage( i );
		if( m_pPageList[i] != NULL )
			m_pPageList[i]->InitPage( (void*)pDoc );
	}
	//
	for(i=ePetPage_Max-1;i>=0;--i)
		m_pSheet->SetActivePage(i);

//	DWORD style = m_pSheet->GetTabControl()->GetStyle();
//	m_pSheet->GetTabControl()->ModifyStyle(0, TCS_BOTTOM);
//

// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 800, 525, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(),
	SWP_NOZORDER | SWP_NOACTIVATE);
//

}

CPetDoc* CPetView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPetDoc)));
	return (CPetDoc*)m_pDocument;
}

void CPetView::SetPetInfo( TMSG_PETINFO* pMsg )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SetPetInfo( pMsg );	
}

void CPetView::SetPetList( DWORD Count, TMSG_PETLISTRESULT* pMsg )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SetPetList( Count, pMsg );	
}

void CPetView::SetUserLoginChk(TMSG_USERLOGINCHK* pMsg )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SetUserLoginChk( pMsg );	
}

void CPetView::SetPetWareItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SetPetWareItemInfo( pMsg );
}
void CPetView::SetPetAccItemInfo( TMSG_CHARACTER_ITEMBASE* pMsg )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SetPetAccItemInfo( pMsg );
}


void CPetView::SearchbtnEnable( int BOOL )
{
	CPet* pPage = (CPet*)m_pPageList[ePetPage_PetInfo];
	if( pPage )
		pPage->SearchbtnEnable( BOOL );
}