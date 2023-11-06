// PetListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "PetListDlg.h"
#include "RMNetwork.h"
#include "DS_RMToolDoc.h"
#include "PetDoc.h"

// CPetListDlg 대화 상자입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CPetListDlg, CDialog)
CPetListDlg::CPetListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPetListDlg::IDD, pParent)
{
}

CPetListDlg::~CPetListDlg()
{
}

void CPetListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestInfoDlg)
	DDX_Control(pDX, IDC_PET_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_PET_LISTCTRL, m_lcPet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPetListDlg, CDialog)
	//{{AFX_MSG_MAP(CQuestInfoDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_PET_LISTCTRL, OnDblclkPetListctrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CPetListDlg 메시지 처리기입니다.


void CPetListDlg::SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, char* sCharName )
{
	m_dwDocumentIdx = dwDocumentIdx;
	m_dwCharIdx = dwCharIdx;
	strcpy( m_sCharName, sCharName );
}

BOOL CPetListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitPetListCtrl();
	m_etCharName.SetWindowText( m_sCharName );
	
	m_bInit = TRUE;
	
	SendMainPetList();

	m_Font.CreatePointFont(100, "Arial");
	m_lcPet.SetFont( &m_Font );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPetListDlg::InitPetListCtrl()
{
	char* tcolumn[6] = { "PetIdx", "Type", "Level", "Stamina", "Exp" };
	int tcolumnsize[6] = { 60, 90, 60, 70, 94 };

	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 6; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcPet.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcPet.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcPet.SetExtendedStyle( dwExStyle );
}

void CPetListDlg::SendMainPetList()
{
	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MAINPETLIST_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData = m_dwCharIdx;	//캐릭터 인덱스

	NETWORK->Send( &Msg, sizeof(TMSG_DWORD) );
}

void CPetListDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_dwCharIdx = 0;
	memset( m_sCharName, 0, 32 );
	m_bInit = FALSE;
	
	m_Font.DeleteObject();
}

void CPetListDlg::UpdatePetListCtrl( TMSG_MAINPETLIST* pMsg )
{
//수정
	m_lcPet. DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( DWORD i = 0; i < pMsg->dwCount; ++i )
	{
		lvitem.iItem = m_lcPet.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pMsg->MainPetList[i].dwPetDBIdx, temp, 10 );		
		m_lcPet.InsertItem( &lvitem );

		// 06.08.04 - pkh
		m_lcPet.SetItemText( lvitem.iItem, 1, GAMERESRCMNGR->GetPetNamechar(pMsg->MainPetList[i].dwKind));
		m_lcPet.SetItemText( lvitem.iItem, 2, itoa(pMsg->MainPetList[i].dwGrade, temp, 10 ) );
		m_lcPet.SetItemText( lvitem.iItem, 3, itoa(pMsg->MainPetList[i].dwStamina, temp, 10 ) );
		m_lcPet.SetItemText( lvitem.iItem, 4, itoa(pMsg->MainPetList[i].dwFriendShip, temp, 10 ) );

	}

}

void CPetListDlg::OnDblclkPetListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lcPet.GetFirstSelectedItemPosition();
	if( pos )
	{
		int index = m_lcPet.GetNextSelectedItem( pos );		
		CString str = m_lcPet.GetItemText( index, 0 );

		int count = m_lcPet.GetSelectedCount();
		if( count == 0 ) return;

		POSITION pos = m_lcPet.GetFirstSelectedItemPosition();
		if( pos == NULL )	return;

		// character Temp를 열고 character Idx를 넘긴다.
		CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
		CPetDoc* pPetDoc = (CPetDoc*)pApp->OpenNewTemplate(eTemp_Pet);

		//pPetDoc->SetInitData(m_dwDocumentIdx,atoi((char*)(LPCTSTR)str));
		pPetDoc->SetInitData(atoi((char*)(LPCTSTR)str));

		EndDialog(IDCANCEL);

	}
	
	*pResult = 0;
}
