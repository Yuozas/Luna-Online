// DelChar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "DelChar.h"
#include "DelCharDoc.h"
#include "RMNetwork.h"
#include "MasInfoManager.h"
#include "CharacterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDelChar ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDelChar, CLDPropPage)

CDelChar::CDelChar(): CLDPropPage(CDelChar::IDD)
{
	//{{AFX_DATA_INIT(CGuildInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDocument = NULL;
//	m_DelCharTable.Initialize( 500 );

	//m_dwdelcharidx = NULL;

}

CDelChar::~CDelChar()
{
	ReleaseDelCharTable();
}

void CDelChar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDelChar)
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);		
	DDX_Control(pDX, IDC_ET_SEARCH, m_etSearch);
	DDX_Control(pDX, IDC_LT_DELCHAR, m_lCDelChar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelChar, CLDPropPage)
	//{{AFX_MSG_MAP(CMunpaInfo)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LT_DELCHAR, OnDblclkDelCharListctrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDelChar �޽��� ó�����Դϴ�.

BOOL CDelChar::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

	InitDelCharListControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelChar::ReleaseDelCharTable()
{
	DELCHARLIST* pData = NULL;
	m_DelCharTable.SetPositionHead();
	while( pData = m_DelCharTable.GetData() )
		delete pData;
	m_DelCharTable.RemoveAll();
}

void CDelChar::OnBnClickedSearch() 
{
	// TODO: Add your control notification handler code here

	m_lCDelChar.DeleteAllItems();

	CString string;
	int		nSearchType = -1;

	if( m_etSearch.GetWindowTextLength() != 0 )
	{
		nSearchType = 0;
		m_etSearch.GetWindowText(string);
	}

	if( nSearchType == -1 )
	{
		// �˻��� �Է� ���� �޼���
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	// ������ �˻� ��û
	TMSG_DELCHARLISTSEARCH DelCharSearch;
	DelCharSearch.Category = MP_RMTOOL_DELCHAR;
	DelCharSearch.Protocol = MP_RMTOOL_DELCHARLIST_SYN;
	DelCharSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	DelCharSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	strcpy(DelCharSearch.delcharname, string.GetBuffer(0));			// character name

	NETWORK->Send(&DelCharSearch, sizeof(DelCharSearch));

}

CDelCharDoc* CDelChar::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDelCharDoc)));
	return (CDelCharDoc*)m_pDocument;
}

void CDelChar::InitDelCharListControl()
{
	char* column[13] = { "Character_idx", "User_idx", "Character_name", "Flag", "Create_Date", "Create_IP",
		"Delete_Date", "Delete_IP"};	//, "Recover_Date", "Recover_IP"}; //������ �ʿ� �����Ƿ� ȭ�鿡 �ȳ�Ÿ��
	int columnsize[13] = { 100,80,120,70,130,110,130,110};	//,130,110 }; //������ �ʿ� �����Ƿ� ȭ�鿡 �ȳ�Ÿ��
	
	// listctrl column ����
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 11; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = columnsize[i];
		lvcolumn.pszText = column[i];
		m_lCDelChar.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lCDelChar.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lCDelChar.SetExtendedStyle( dwExStyle );
	
}

void CDelChar::SetDelCharList( DWORD Count, TMSG_DELCHARLISRESULT* pMsg  )
{
	m_lCDelChar.DeleteAllItems();

	char temp[256];
	char flag[256];
	LV_ITEM lvitem;

	for( DWORD i = 0; i < Count; ++i )
	{
		
		DELCHARLIST* pData = &pMsg->sDelcharList[i];

		lvitem.iItem = m_lCDelChar.GetItemCount();	// ��
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pData->dwcharacter_idx, temp, 10 );							// ĳ���� index
		
		// ĳ���� ���� (DB�����󿡼� 2�� ��츸 �ҷ���)
		switch (pData->wflag)
		{
		case 1:
			strcpy(flag, "CREATE");		break;
		case 2:
			strcpy(flag, "DELETE");		break;
		case 3:
			strcpy(flag, "RECOVER");	break;
		default:
			strcpy(flag, "NOT_DEF");	break;
		}

		m_lCDelChar.InsertItem( &lvitem );
		m_lCDelChar.SetItemText( lvitem.iItem, 1, itoa(pData->dwuser_idx, temp, 10) );		// ���� index
		m_lCDelChar.SetItemText( lvitem.iItem, 2, pData->character_name);					// ĳ���� ��
		m_lCDelChar.SetItemText( lvitem.iItem, 3, flag );									// ĳ���� ����(������츸 ����)
		m_lCDelChar.SetItemText( lvitem.iItem, 4, strcat(pData->create_date,pData->create_datetime));	// ĳ���� ��������
		m_lCDelChar.SetItemText( lvitem.iItem, 5, pData->create_ip );						// ������ ������
		m_lCDelChar.SetItemText( lvitem.iItem, 6, strcat(pData->delete_date, pData->delete_datetime));	// ĳ���� ��������
		m_lCDelChar.SetItemText( lvitem.iItem, 7, pData->delete_ip );						// ������ ������
		m_lCDelChar.SetItemText( lvitem.iItem, 8, strcat(pData->recover_date, pData->recover_datetime));	// ĳ���� ��������
		m_lCDelChar.SetItemText( lvitem.iItem, 9, pData->recover_ip );						// ������ ������

	}
}

void CDelChar::OnDblclkDelCharListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{

	// TODO: Add your control notification handler code here
	CDelCharDoc* pDoc = GetDocument();
	if( !pDoc ) return;

	int count = m_lCDelChar.GetSelectedCount();
	if( count == 0 ) return;

	POSITION pos = m_lCDelChar.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	int nCurSelectedIdx = m_lCDelChar.GetNextSelectedItem(pos);
	CString str1 = m_lCDelChar.GetItemText( nCurSelectedIdx, 0 );
	CString str2 = m_lCDelChar.GetItemText( nCurSelectedIdx, 1 );
	DWORD dwCharIdx = (DWORD)atoi( (char*)(LPCTSTR)str1 );
	DWORD dwUserIdx = (DWORD)atoi( (char*)(LPCTSTR)str2 );
	DWORD dwServer = MASINFOMGR->GetCurMasInfo()->nServerSet;

	// character Temp�� ���� character Idx�� �ѱ��.
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	CCharacterDoc* pCharacterDoc = (CCharacterDoc*)pApp->OpenNewTemplate(eTemp_Character);

	pCharacterDoc->OpenCharacterInfo(dwServer, dwUserIdx, dwCharIdx, FALSE);

	AfxGetMainWnd()->BeginModalState();
	int r = AfxGetMainWnd()->RunModalLoop();

	if( r == 5000 )
	{
		AfxGetMainWnd()->EndModalState();
		AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	}
	
	*pResult = 0;
}
