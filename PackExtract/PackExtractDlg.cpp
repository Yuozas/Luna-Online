// PackExtractDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PackExtract.h"
#include "PackExtractDlg.h"
#include ".\packextractdlg.h"
#include "ExtractSelectedDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPackExtractDlg* g_pPackExtractDlg;

UINT CallExtractPackFile( LPVOID pParam )
{
	CPackExtractDlg* pDlg = (CPackExtractDlg*)pParam;
	if( pDlg )
		pDlg->ExtractPackFile();

	return 0;
}

UINT CallCompressPackFile( LPVOID pParam )
{
	CPackExtractDlg* pDlg = (CPackExtractDlg*)pParam;
	if( pDlg )
		pDlg->CompressPackFile();

	return 0;
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPackExtractDlg ��ȭ ����



CPackExtractDlg::CPackExtractDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPackExtractDlg::IDD, pParent)
	, m_bDeleteSource(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ZeroMemory( m_szBaseDirectory, MAX_PATH );

	m_pidlDesktop = NULL;
	m_pDesktopFolder = NULL;
	SHGetDesktopFolder(&m_pDesktopFolder);
	m_pMalloc = NULL;
	SHGetMalloc(&m_pMalloc);

	m_pStorage = NULL;

	CMiniDump::Begin();
}

CPackExtractDlg::~CPackExtractDlg()
{
	if (m_pidlDesktop != NULL)
		m_pMalloc->Free(m_pidlDesktop);
	if (m_pMalloc != NULL)
		m_pMalloc->Release();
	if (m_pDesktopFolder != NULL)
		m_pDesktopFolder->Release();

	if( m_pStorage )
		m_pStorage->Release();

	m_vPackFile.clear();
	m_vCompressFolder.clear();
	m_vSelectedPackFileList.clear();

	CMiniDump::End();
}

void CPackExtractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PACK_LIST, m_PackListBox);
	DDX_Control(pDX, IDC_PROGRESS_LIST, m_ListProgress);
	DDX_Control(pDX, IDC_PROGRESS_PACK, m_PackProgress);
	DDX_Control(pDX, IDC_TREE1, m_ExtractFolderPos);
	DDX_Control(pDX, IDC_MODE_EXTRACT_RADIO, m_ModeExtractBtn);
	DDX_Control(pDX, IDC_MODE_COMPRESS_RADIO, m_ModeCompressBtn);
	DDX_Check(pDX, IDC_DELETE_AFTER_PROC_CHECK, m_bDeleteSource);
	DDX_Control(pDX, IDC_EXTRACT_FILENAME_BUTTON1, m_ExtractFileNameInputBtn);
}

BEGIN_MESSAGE_MAP(CPackExtractDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_EXTRACT_PACKFILE_BUTTON, OnBnClickedExtractPackfileButton)
	ON_BN_CLICKED(ID_DELETE_PACKFILE_BUTTON, OnBnClickedDeletePackfileButton)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, OnTvnItemexpandedTree1)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, OnTvnItemexpandingTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_MODE_EXTRACT_RADIO, OnBnClickedModeExtractRadio)
	ON_BN_CLICKED(IDC_MODE_COMPRESS_RADIO, OnBnClickedModeCompressRadio)
	ON_BN_CLICKED(IDC_DELETE_AFTER_PROC_CHECK, OnBnClickedDeleteAfterProcCheck)
	ON_MESSAGE( UM_UPDATEDATA, OnUpdateDlg )
	ON_BN_CLICKED(IDC_EXTRACT_FILENAME_BUTTON1, OnBnClickedExtractFilenameButton1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnBnClickedHelpButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_LBN_DBLCLK(IDC_PACK_LIST, OnLbnDblclkPackList)
END_MESSAGE_MAP()


// CPackExtractDlg �޽��� ó����

BOOL CPackExtractDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	GetCurrentDirectory( MAX_PATH, m_szBaseDirectory );

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	HMODULE hStorage = LoadLibrary( "FileStorage.dll" );
	if( hStorage )
	{
		CREATE_INSTANCE_FUNC	pFunc;
		pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(hStorage,"DllCreateInstance");
		HRESULT hr = pFunc((void**)&m_pStorage);
		if (hr != S_OK)
		{
			AfxMessageBox( "Failed LoadLibrary() - FileStorage.dll" );
			SendMessage( WM_CLOSE );
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox( "Failed LoadLibrary() - FileStorage.dll" );
		SendMessage( WM_CLOSE );
		return FALSE;
	}

	m_pStorage->Initialize( MAX_FILE_ATOM_NUM, 4096, 128, FILE_ACCESS_METHOD_ONLY_FILE );

	/////////////////////////////////////////////////////////////
	// Tree Ctrl
	//- Desktop �� PIDL�� ��´�. (�ֻ��� PIDL)
	//- ���� �������� ���Ͽ� System Image List �� Setting �Ѵ�.
	//- Desktop ���� �����۵��� Tree View Item ���� ����Ѵ�.
	//- Tree View �� �ʱ� Item Expand (Ȯ��)�� �����Ѵ�.
	/////////////////////////////////////////////////////////////

	CTreeCtrl& Tree = m_ExtractFolderPos;
	///////////////////////////////////////////////////
	// Get Desktop PIDL
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &m_pidlDesktop);
	///////////////////////////////////////////////////
	// Set Image List
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	HIMAGELIST hSysImageList = (HIMAGELIST) SHGetFileInfo((LPCTSTR)m_pidlDesktop, 0,
		&sfi, sizeof(SHFILEINFO), SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	m_IconImageList.Attach( hSysImageList );
	Tree.SetImageList( &m_IconImageList, TVSIL_NORMAL );    
//	TreeView_SetImageList(this->m_hWnd, hSysImageList, TVSIL_NORMAL);
	///////////////////////////////////////////////////
	// Insert Desktop Folder as a Tree View Item
	// to get file info
	UINT uFlags = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON
		| SHGFI_DISPLAYNAME;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	SHGetFileInfo((LPCTSTR) m_pidlDesktop, 0, &sfi, sizeof(SHFILEINFO), uFlags);
	TVINSERTSTRUCT tvi;
	ZeroMemory(&tvi, sizeof(TVINSERTSTRUCT));
	tvi.hParent = TVI_ROOT;
	tvi.hInsertAfter = TVI_LAST;
	tvi.item.pszText = sfi.szDisplayName;
	tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
	tvi.item.iImage = tvi.item.iSelectedImage = sfi.iIcon;
	tvi.item.cChildren = 1;
	tvi.item.lParam = (LPARAM) m_pidlDesktop;

	m_hDesktopItem = Tree.InsertItem(&tvi);
	///////////////////////////////////////////////////
	// Expand desktop folder
	EnumChildItems(m_pDesktopFolder, m_pidlDesktop, m_hDesktopItem);
	Tree.Expand(m_hDesktopItem, TVE_EXPAND);


	// Mode ����
	m_eMode = ePackMode_Extract;
	m_ModeExtractBtn.SetCheck( TRUE );
	m_ModeCompressBtn.SetCheck( FALSE );
	m_bIsWorking = FALSE;

	g_pPackExtractDlg = this;
	m_pSelectedPackFileHandle = NULL;


	// ��ɾ� ���ڰ� �ִ� ���
	if( __argc > 1 )
	{
		CString strPath = __argv[1];

		// ���ϰ�ο��� ������θ� �����.
		strPath = strPath.Left( strPath.ReverseFind( '\\' ) );

		CString strFileName = __argv[1];
		strFileName.Delete( 0, strFileName.ReverseFind( '\\' ) + 1 );

		// pak ������ �ƴѰ��
		if( strstr( (LPCTSTR)strFileName, ".pak" ) == NULL )
		{
			UpdateTreePath( m_szBaseDirectory );
		}
		else
		{
			// List�� ������ �ֵ��� �����Ѵ�.(pak���� ���� ����� ��쿡��)
			UpdateTreePath( strPath );

			m_PackListBox.ResetContent();
			m_vPackFile.clear();

			m_PackListBox.AddString( strFileName );
			m_vPackFile.push_back( strFileName );
		}
	}
	else
	{
		// Tree�� �����η� �����Ѵ�.
		UpdateTreePath( m_szBaseDirectory );
	}
	
	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CPackExtractDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPackExtractDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CPackExtractDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPackExtractDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
	CDialog::OnLButtonDown(nFlags, point);
}

void CPackExtractDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	SendMessage( WM_NCLBUTTONUP, HTCAPTION, 0 );
	CDialog::OnLButtonUp(nFlags, point);
}


void CPackExtractDlg::OnBnClickedExtractPackfileButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_bIsWorking )
	{
		AfxMessageBox( "�۾��� ����ɶ����� ��ٷ� �ֽʽÿ�!" );
		return;
	}

	switch( m_eMode )
	{
	case ePackMode_Extract:
		if( m_vPackFile.empty() )
			return;

		AfxBeginThread( CallExtractPackFile, (LPVOID)this );
		break;
	case ePackMode_Compress:
		if( m_vCompressFolder.empty() )
			return;

		AfxBeginThread( CallCompressPackFile, (LPVOID)this );
		break;
	}
}

void CPackExtractDlg::Release()
{
	m_PackListBox.ResetContent();
	m_vPackFile.clear();
	m_vCompressFolder.clear();

	m_ListProgress.SetPos( 0 );
	m_PackProgress.SetPos( 0 );
}

BOOL CPackExtractDlg::GetSelectedFolderByTree( char* pDirName, DWORD dwDirNameLen )
{
	// No Selected Folder
	HTREEITEM hSelectedItem = m_ExtractFolderPos.GetSelectedItem();
	if( hSelectedItem == NULL )
		return FALSE;

	// Get Current Selected Folder
	ZeroMemory( pDirName, dwDirNameLen );
	LPITEMIDLIST pidl = (LPITEMIDLIST) m_ExtractFolderPos.GetItemData( hSelectedItem );
	SHGetPathFromIDList( pidl, pDirName );

	return TRUE;
}

void CPackExtractDlg::LoadFolderFile( LPCTSTR strLoadPath, LPCTSTR strFileFilter, vector<CString>& vecName, BOOL bLoadDirectory, BOOL bFullPath )
{
	char szBaseDir[ MAX_PATH ] = {0,};
	GetCurrentDirectory( MAX_PATH, szBaseDir );
	if( !SetCurrentDirectory( strLoadPath ) )
		return;

	HANDLE hFindFile = NULL;
	WIN32_FIND_DATA FindFileData = { 0, };	
    
	hFindFile = ::FindFirstFile( strFileFilter, &FindFileData );
	if( hFindFile == INVALID_HANDLE_VALUE )
	{
		SetCurrentDirectory( szBaseDir );
		return;
	}

	// ù��° ���� ����
	if( !(FindFileData.cFileName == _T( "." ) || _tcscmp( FindFileData.cFileName, _T( ".." ) ) == 0 ) &&
		!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
	{
		// ������ ����
		if( bFullPath )
		{
			CString strPath( strLoadPath );
			strPath.AppendFormat( "\\%s", FindFileData.cFileName );
			vecName.push_back( strPath );
		}
		else
		{
			CString str( FindFileData.cFileName );
			vecName.push_back( str );
		}
	}

	while( ::FindNextFile( hFindFile, &FindFileData ) )
	{
		if( FindFileData.cFileName == _T( "." ) ||
			_tcscmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
		{
			continue;
		}
		else if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
				bLoadDirectory )
		{			
			// ������ ����
			if( bFullPath )
			{
				CString strPath( strLoadPath );
				strPath.AppendFormat( "\\%s", FindFileData.cFileName );
				vecName.push_back( strPath );
			}
			else
			{
				CString str( FindFileData.cFileName );
				vecName.push_back( str );
			}
				
		}
		else if( !(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				!bLoadDirectory )
		{
			// ������ ����
			if( bFullPath )
			{
				CString strPath( strLoadPath );
				strPath.AppendFormat( "\\%s", FindFileData.cFileName );
				vecName.push_back( strPath );
			}
			else
			{
				CString str( FindFileData.cFileName );
				vecName.push_back( str );
			}
		}
	}

	::FindClose( hFindFile );

	SetCurrentDirectory( szBaseDir );
}

void CPackExtractDlg::RemoveFileInDir( LPCTSTR strDirPath )
{
	CFileFind finder;
	BOOL bWorking = TRUE;
	CString strDirFile( strDirPath );
	strDirFile.Append( "\\*.*" );
	bWorking = finder.FindFile(strDirFile);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if(finder.IsDots()) continue;
		if(finder.IsDirectory())    RemoveDir(finder.GetFilePath());
		else    ::DeleteFile(finder.GetFilePath());
	}
	finder.Close();
}

void CPackExtractDlg::RemoveDir( LPCTSTR strDirPath )
{
	RemoveFileInDir( strDirPath );

	::RemoveDirectory( strDirPath );
}

void CPackExtractDlg::LoadPackFile()
{
	if( !GetSelectedFolderByTree( m_szCurDirectory, MAX_PATH ) )
	{
		return;
	}

	Release();

	// ".pak" File Load
	LoadFolderFile( m_szCurDirectory, "*.pak", m_vPackFile, FALSE, FALSE );

	// Load�� File ListBox�� ���
	vector<CString>::iterator iter;
	for( iter = m_vPackFile.begin() ; iter != m_vPackFile.end() ; ++iter )
	{
		m_PackListBox.AddString( *iter );
	}

	UpdateData( FALSE );
}

void CPackExtractDlg::LoadCompressFolder()
{
	if( !GetSelectedFolderByTree( m_szCurDirectory, MAX_PATH ) )
	{
		return;
	}

	Release();

	// Directory Load For ListBox
	LoadFolderFile( m_szCurDirectory, "*.*", m_vCompressFolder, TRUE, FALSE );

	// Load�� File ListBox�� ���
	vector<CString>::iterator iter;
	for( iter = m_vCompressFolder.begin() ; iter != m_vCompressFolder.end() ; ++iter )
	{
		m_PackListBox.AddString( *iter );
	}
}

void CPackExtractDlg::ExtractPackFile()
{
	m_bIsWorking = TRUE;

	// Extract Pack File
	int nListProgressPos, nPackProgressPos;
	nListProgressPos = nPackProgressPos = 0;

	m_ListProgress.SetRange( 0, (short)m_vPackFile.size() );

	vector<CString>::iterator iter;
	for( iter = m_vPackFile.begin() ; iter != m_vPackFile.end() ; ++iter )
	{
		// Create Folder
		CString strDirectory( m_szCurDirectory );
		strDirectory.AppendFormat( "\\%s", *iter );
		strDirectory.Delete( strDirectory.Find( ".pak" ), 4 );

		if( !CreateDirectory( strDirectory, NULL ) )
		{
			switch( GetLastError() )
			{
			case ERROR_ALREADY_EXISTS:
				{
					// Folder ����� �������� ���� �����
					RemoveFileInDir( strDirectory );
					break;
				}
			default:
				{
					__asm int 3;
					AfxMessageBox( "Failed CreateDirectory() !" );
					return;
				}
			}
		}

		SetCurrentDirectory( strDirectory );

		// File Extract
		CString strPackFile( m_szCurDirectory );
		strPackFile.AppendFormat( "\\%s", *iter );
		void*	pPackFileHandle = NULL;
		pPackFileHandle = m_pStorage->MapPackFile( (LPSTR)(LPCSTR)strPackFile, TRUE );
		if (!pPackFileHandle)
			break;

		FSPACK_FILE_INFO	fileInfo;
		FSFILE_ATOM_INFO*	pFileInfoList;
		DWORD dwFileInfoNum;
		m_pStorage->GetPackFileInfo(pPackFileHandle,&fileInfo);
		if(pPackFileHandle)
		{
			dwFileInfoNum = m_pStorage->CreateFileInfoList(pPackFileHandle,&pFileInfoList,MAX_FILE_ATOM_NUM);

			m_PackProgress.SetRange( 0, (short)dwFileInfoNum );
			nPackProgressPos = 0;

			// ���� ��ȸ
			for (DWORD i=0; i<dwFileInfoNum; i++)
			{
				m_pStorage->ExtractFile( pFileInfoList[i].pszFileName );

				m_PackProgress.SetPos( ++nPackProgressPos );

				SendMessage( UM_UPDATEDATA );
			}

		}

		m_pStorage->UnmapPackFile( pPackFileHandle );

		m_ListProgress.SetPos( ++nListProgressPos );

		SendMessage( UM_UPDATEDATA );

		if( m_bDeleteSource )
		{
			::DeleteFile( strPackFile );
		}
	}

	m_bIsWorking = FALSE;

	SetCurrentDirectory( m_szCurDirectory );

	AfxMessageBox( "�������� �Ϸ�" );

	Release();
}

void CPackExtractDlg::OpenExtractPackFileWithSelectListDlg( CString strSelectedPackFile )
{
	m_bIsWorking = TRUE;

	// PackFile Open
	CString strPackFile( m_szCurDirectory );
	strPackFile.AppendFormat( "\\%s", strSelectedPackFile );	
	m_pSelectedPackFileHandle = m_pStorage->MapPackFile( (LPSTR)(LPCSTR)strPackFile, TRUE );
	if (!m_pSelectedPackFileHandle)
	{
		return;
	}

	m_strSelectedPackFileName = strSelectedPackFile;

	ExtractSelectedDlg SelectedDlg;

	FSPACK_FILE_INFO	fileInfo;
	FSFILE_ATOM_INFO*	pFileInfoList;
	DWORD dwFileInfoNum;
	m_pStorage->GetPackFileInfo( m_pSelectedPackFileHandle, &fileInfo );
	if( m_pSelectedPackFileHandle )
	{
		dwFileInfoNum = m_pStorage->CreateFileInfoList( m_pSelectedPackFileHandle, &pFileInfoList, MAX_FILE_ATOM_NUM );

		// ���� ��ȸ
		for (DWORD i=0; i<dwFileInfoNum; i++)
		{
			// ���� �̸� ����
			CString str( pFileInfoList[i].pszFileName );
			m_vSelectedPackFileList.push_back( str );
		}
	}

	if( SelectedDlg.DoModal() == IDOK )
	{
	}
}

void CPackExtractDlg::AddListFromSelectedPackFile( CListBox& listbox )
{
	vector<CString>::iterator iter;
	for( iter = m_vSelectedPackFileList.begin() ; iter != m_vSelectedPackFileList.end() ; ++iter )
	{
		listbox.AddString( *iter );
	}
}

void CPackExtractDlg::StartExtractPackFileWithSelectListDlg( BOOL bClearFoloder )
{
	// Create Folder
	CString strDirectory( m_szCurDirectory );
	strDirectory.AppendFormat( "\\%s", m_strSelectedPackFileName );
	strDirectory.Delete( strDirectory.Find( ".pak" ), 4 );

	if( !CreateDirectory( strDirectory, NULL ) )
	{
		switch( GetLastError() )
		{
		case ERROR_ALREADY_EXISTS:
			{
				// Folder ����� �������� ���� �����
				if( bClearFoloder )
					RemoveFileInDir( strDirectory );
				break;
			}
		default:
			{
#ifdef _DEBUG
				__asm int 3;
#endif
				AfxMessageBox( "Failed CreateDirectory() !" );
				return;
			}
		}
	}

	SetCurrentDirectory( strDirectory );
}

void CPackExtractDlg::EndExtractPackFileWithSelectListDlg()
{
	m_vSelectedPackFileList.clear();
	SetCurrentDirectory( m_szCurDirectory );

	if( !m_pSelectedPackFileHandle )
		return;

	m_bIsWorking = FALSE;

	m_pStorage->UnmapPackFile( m_pSelectedPackFileHandle );

	m_pSelectedPackFileHandle = NULL;
}

BOOL CPackExtractDlg::StartDeletePackFileWithSelectListDlg()
{
	return m_pStorage->LockPackFile( m_pSelectedPackFileHandle, 0 );
}

void CPackExtractDlg::EndDeletePackFileWithSelectListDlg()
{
	m_dwUnlockFileProcTotalNum = 0;
	m_pStorage->UnlockPackFile( m_pSelectedPackFileHandle, CallUnlockFileProc );
}

void CPackExtractDlg::ExtractPackFile( char* szExtractFileName )
{
	m_pStorage->ExtractFile( szExtractFileName );
}

void CPackExtractDlg::DeletePackFile( char* szExtractFileName )
{
	m_pStorage->DeleteFileFromPackFile( szExtractFileName );
}

void CPackExtractDlg::CompressPackFile()
{
	m_bIsWorking = TRUE;

	int nListProgressPos = 0;
	m_ListProgress.SetRange( 0, (short)m_vCompressFolder.size() );

	vector<CString>::iterator iter;
	for( iter = m_vCompressFolder.begin() ; iter != m_vCompressFolder.end() ; ++iter )
	{
		CompressFile( *iter );
		
		m_ListProgress.SetPos( ++nListProgressPos );
	}

	SetCurrentDirectory( m_szCurDirectory );

	AfxMessageBox( "���� �Ϸ�!" );

	m_bIsWorking = FALSE;

	Release();
}

void CPackExtractDlg::CompressFile( CString strFolderName )
{
	SetCurrentDirectory( m_szCurDirectory );

	// ��ü ���� �б�
	CString strPath( m_szCurDirectory );
	CString strPackName( m_szCurDirectory );
	strPath.AppendFormat( "\\%s", strFolderName );
	strPackName.AppendFormat( "\\%s.pak", strFolderName );

	// Folder �� ���� �б�
	vector<CString> vecFileName;
	LoadFolderFile( strPath, "*.*", vecFileName, FALSE, TRUE );
	
	void*	pPackFileHandle = NULL;
	pPackFileHandle = m_pStorage->MapPackFile( (LPSTR)(LPCSTR)strPackName, TRUE );
	if (!pPackFileHandle)
	{
		m_pStorage->UnmapPackFile( pPackFileHandle );
		return;
	}

	m_dwUnlockFileProcTotalNum = 0;

	m_pStorage->LockPackFile( pPackFileHandle, 0 );
	
	vector<CString>::iterator iter;
	for( iter = vecFileName.begin() ; iter != vecFileName.end() ; ++iter )
	{
		if( m_pStorage->IsExistInFileStorage( (LPSTR)(LPCTSTR)*iter ) )
		{
			char msg[512] = {0,};
			sprintf( msg, "[ �����ϴ� ���� %s.pak, %s ]", strFolderName, *iter );
			OutputDebugString( msg );

			m_pStorage->DeleteFileFromPackFile( (LPSTR)(LPCTSTR)*iter );
		}
		m_pStorage->InsertFileToPackFile( pPackFileHandle, (LPSTR)(LPCTSTR)*iter );
	}

	m_pStorage->UnlockPackFile( pPackFileHandle, CallUnlockFileProc );
	m_pStorage->UnmapPackFile( pPackFileHandle );

	// ����, ���� ����
	if( m_bDeleteSource )
	{
		RemoveDir( strPath );
	}
}

DWORD __stdcall CallUnlockFileProc( DWORD dwCurCount, DWORD dwTotalCount, void* pArg )
{
	if( g_pPackExtractDlg )
		g_pPackExtractDlg->UnlockFileProc( dwCurCount, dwTotalCount, pArg );

	return 0;
}

void CPackExtractDlg::UnlockFileProc( DWORD dwCurCount, DWORD dwTotalCount, void* pArg )
{
	if( m_dwUnlockFileProcTotalNum == 0 )
	{
		m_dwUnlockFileProcTotalNum = dwTotalCount;
		m_PackProgress.SetRange( 0, (short)dwTotalCount );
	}

	m_PackProgress.SetPos( dwCurCount + 1 );

	SendMessage( UM_UPDATEDATA );
}

void CPackExtractDlg::OnBnClickedDeletePackfileButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int nIndex = m_PackListBox.GetCurSel();
	int nCount = m_PackListBox.GetSelCount();

	CArray<int,int> aryListBoxSel;
	CArray<CString> arySelectedFileName;

	aryListBoxSel.SetSize( nCount );
	m_PackListBox.GetSelItems( nCount, aryListBoxSel.GetData() );
	for( int i=0; i < nCount ; ++i )
	{
		CString strFileName;
		m_PackListBox.GetText( aryListBoxSel[i], strFileName );
		arySelectedFileName.Add( strFileName );
	}

	if( (nIndex != LB_ERR) && (nCount > 0) )
	{
		switch( m_eMode )
		{
		case ePackMode_Extract:
			{
				for( int i=0; i < nCount ; ++i )
				{
					CString strFileName = arySelectedFileName[i];
					// �ش� ���� List ���� ����
					vector<CString>::iterator iter;
					for( iter = m_vPackFile.begin() ; iter != m_vPackFile.end() ; ++iter )
					{
						if( iter->Compare( arySelectedFileName[i] ) == 0 )
						{
							nIndex = m_PackListBox.FindString( -1, arySelectedFileName[i] );
							m_PackListBox.DeleteString( nIndex );
							m_vPackFile.erase( iter );
							break;
						}
					}				
				}
			}
			break;
		case ePackMode_Compress:
			{
				for( int i=0; i < nCount ; ++i )
				{
					CString strFileName = arySelectedFileName[i];

					// �ش� ���� List ���� ����
					vector<CString>::iterator iter;
					for( iter = m_vCompressFolder.begin() ; iter != m_vCompressFolder.end() ; ++iter )
					{
						if( iter->Compare( strFileName ) == 0 )
						{
							nIndex = m_PackListBox.FindString( -1, strFileName );
							m_PackListBox.DeleteString( nIndex );
							m_vCompressFolder.erase( iter );
							break;
						}
					}
				}
			}
			break;
		}	

		
	}

	UpdateData( FALSE );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EnumChildItems()
//		- ����(�θ�) IShellFolder pointer �� PIDL, HTREEITEM�� �ް� �ȿ� �ִ� Child Item ���� Tree �� ����Ѵ�.
//		* ó������ ��� ���丮, ������丮�� �а� Tree Item �� ������� �ʴ´�. [+] ��ư�� ������ �Ǵ� �������� �ʿ�� ����Ѵ�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPackExtractDlg::EnumChildItems(LPSHELLFOLDER pFolder, LPITEMIDLIST pParent, HTREEITEM hParent)
{
	LPENUMIDLIST pEnumIDList = NULL;
	LPITEMIDLIST pItem = NULL;
	LPITEMIDLIST pidlAbs = NULL;
	ULONG ulFetched = 0;
	TVINSERTSTRUCT tvi;
	if (pFolder->EnumObjects(this->m_hWnd, SHCONTF_FOLDERS, &pEnumIDList) == NOERROR)
	{
		while (pEnumIDList->Next(1, &pItem, &ulFetched) == NOERROR)
		{
			// to memorize absolute PIDL
			pidlAbs = ILAppend(pParent, pItem);
			// to set tree view item
			ZeroMemory(&tvi, sizeof(TVINSERTSTRUCT));
			tvi.hParent = hParent;
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN | TVIF_PARAM;
			tvi.item.lParam = (LPARAM) pidlAbs;
			// fill another properties of item(.pszText, .iImage, ...)
			FillItem(tvi, pFolder, pidlAbs, pItem);
			m_pMalloc->Free(pItem);
		}
		pEnumIDList->Release();
	}
}

///////////////////////////////////////////////////////////////
// FillItem ()
//		- Tree View�� ����� ��Ҹ� �����ϰ� Tree �� Insert �Ѵ�
///////////////////////////////////////////////////////////////
void CPackExtractDlg::FillItem(TVINSERTSTRUCT &tvi, LPSHELLFOLDER pFolder, LPITEMIDLIST pidl, LPITEMIDLIST pidlRelative)
{
	CTreeCtrl& Tree = m_ExtractFolderPos;
	STRRET sName;
	TCHAR szBuf[MAX_PATH] = {0};
	DWORD dwAttributes = 0;
	UINT uFlags = 0;
	SHFILEINFO sfi;
	// ** In the virtual folder cases, SHGetFileInfo.szDisplayName is impossible
	///////////////////////////////////////////////////
	// Set Display Name
	if (tvi.item.mask & TVIF_TEXT)
	{
		// get display name
		pFolder->GetDisplayNameOf(pidlRelative, 0, &sName);
		UTStrretToString(pidlRelative, &sName, szBuf);
		tvi.item.pszText = szBuf;
	}
	///////////////////////////////////////////////////
	// Set Display Image

	if (tvi.item.mask & (TVIF_IMAGE | TVIF_SELECTEDIMAGE))
	{
		// get some attributes
		dwAttributes = SFGAO_FOLDER | SFGAO_LINK;
		pFolder->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlRelative, &dwAttributes);
		// get correct icon
		uFlags = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_DISPLAYNAME;
		if (dwAttributes & SFGAO_LINK)
			uFlags |= SHGFI_LINKOVERLAY;
		if (tvi.item.mask & TVIF_IMAGE)
		{
			ZeroMemory(&sfi, sizeof(SHFILEINFO));
			SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(SHFILEINFO), uFlags);

			tvi.item.iImage = sfi.iIcon;
		}
		if (tvi.item.mask & TVIF_SELECTEDIMAGE)
		{
			if (dwAttributes & SFGAO_FOLDER)
				uFlags |= SHGFI_OPENICON;
			ZeroMemory(&sfi, sizeof(SHFILEINFO));
			SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(SHFILEINFO), uFlags);

			tvi.item.iSelectedImage = sfi.iIcon;
		}
	}
	///////////////////////////////////////////////////
	// Set (+) Button or not
	if (tvi.item.mask & (TVIF_CHILDREN))
	{
		// get some attributes
		dwAttributes = SFGAO_FOLDER;
		pFolder->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlRelative, &dwAttributes);
		// get children
		tvi.item.cChildren = 0;
		if (dwAttributes & SFGAO_FOLDER)
		{
			dwAttributes = SFGAO_HASSUBFOLDER;
			pFolder->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlRelative, &dwAttributes);
			tvi.item.cChildren = (dwAttributes & SFGAO_HASSUBFOLDER) ? 1 : 0;
		}
	}
	
	Tree.InsertItem(&tvi);
}

//////////////////////////////////////////////////////////
// UTStrretToString()
//		- GetDisplayName()���� ����� ���� String���� ��ȯ
//////////////////////////////////////////////////////////
void CPackExtractDlg::UTStrretToString(LPITEMIDLIST pidl, LPSTRRET pStr, LPSTR pszBuf)
{
	lstrcpy(pszBuf, "");
	switch(pStr->uType)
	{
	case STRRET_WSTR: // Unicode string
		WideCharToMultiByte(CP_ACP, 0, pStr->pOleStr, -1, pszBuf, MAX_PATH, NULL, NULL);
		break;
	case STRRET_OFFSET: // Offset
		lstrcpy(pszBuf, reinterpret_cast<LPSTR>(pidl) + pStr->uOffset);
		break;
	case STRRET_CSTR: // ANSI string
		lstrcpy(pszBuf, pStr->cStr);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// OnItemexpanding()
//		- ��ư�� ������ �� �ش� Tree Item���� PIDL�� �޴´�. (lParam)
//		- ���࿡ �ش� ������ �� ���� Ȯ��� ���� ���ٸ� ��, �Ϻ� �����۵��� ��ϵ��� �ʾҴٸ� Absolute PIDL�� ����
//		  Desktop Shell Folder Interface ���� �ش� Shell Folder Interface�� ��� EnumChildItems()�� ȣ�����ش�.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPackExtractDlg::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CTreeCtrl& Tree = m_ExtractFolderPos;
	if (pNMTreeView->action & TVE_EXPAND)
	{
		///////////////////////////////////////////////////
		// insert item when it needs children
		if (Tree.GetChildItem( pNMTreeView->itemNew.hItem ) == NULL)
		{
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			LPITEMIDLIST pidl;
			LPSHELLFOLDER pFolder;
			pidl = (LPITEMIDLIST) Tree.GetItemData(pNMTreeView->itemNew.hItem);
			if (pNMTreeView->itemNew.hItem != m_hDesktopItem)
			{
				m_pDesktopFolder->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID *)&pFolder);
				EnumChildItems(pFolder, pidl, pNMTreeView->itemNew.hItem);
				pFolder->Release();
			}
			else
				EnumChildItems(m_pDesktopFolder, pidl, pNMTreeView->itemNew.hItem);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}
	}

	*pResult = 0;
}

// refer to `Working with Item ID Lists` of MSDN
LPITEMIDLIST CPackExtractDlg::ILGetNextItemID(LPCITEMIDLIST pidl)
{
	// Check for valid pidl.
	if (pidl == NULL)
		return NULL;
	// Get the size of the specified item identifier.
	int cb = pidl->mkid.cb;
	// If the size is zero, it is the end of the list.
	if (cb == 0)
		return NULL;
	// Add cb to pidl (casting to increment by bytes).
	pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + cb);
	// Return NULL if it is null-terminating, or a pidl otherwise.
	return (pidl->mkid.cb == 0) ? NULL : (LPITEMIDLIST) pidl;
}
UINT CPackExtractDlg::ILGetSize(LPCITEMIDLIST pidl)
{
	UINT cbTotal = 0;

	if (pidl)
	{
		cbTotal += sizeof(pidl->mkid.cb); // Null terminator
		while (pidl)
		{
			cbTotal += pidl->mkid.cb;
			pidl = ILGetNextItemID(pidl);
		}
	}

	return cbTotal;
}
LPITEMIDLIST CPackExtractDlg::ILAppend(LPITEMIDLIST pidlParent, LPITEMIDLIST pidlChild)
{
	LPITEMIDLIST pidl = NULL;
	if(pidlParent == NULL)
		return NULL;
	if(pidlChild == NULL)
		return pidlParent;

	UINT cb1 = ILGetSize(pidlParent) - sizeof(pidlParent->mkid.cb);
	UINT cb2 = ILGetSize(pidlChild);
	pidl = (LPITEMIDLIST)m_pMalloc->Alloc(cb1 + cb2);
	if(pidl) {
		CopyMemory(pidl, pidlParent, cb1);
		CopyMemory(((LPSTR)pidl) + cb1, pidlChild, cb2);
	}
	return pidl;
}

void CPackExtractDlg::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	OnItemexpanding( pNMHDR, pResult );
}

void CPackExtractDlg::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	OnItemexpanding( pNMHDR, pResult );
}

void CPackExtractDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	if( m_bIsWorking )
	{
		AfxMessageBox( "�۾��� ���� �Ŀ� �õ��ϼ���..." );
		return;
	}

	switch( m_eMode )
	{
	case ePackMode_Extract:
		LoadPackFile();
		break;
	case ePackMode_Compress:
		LoadCompressFolder();
		break;
	}	
}

void CPackExtractDlg::OnBnClickedModeExtractRadio()
{
	if( m_bIsWorking && m_eMode == ePackMode_Extract )
	{
		m_ModeExtractBtn.SetCheck( FALSE );
		m_ModeCompressBtn.SetCheck( TRUE );
		AfxMessageBox( "�۾��� ���� �Ŀ� �õ��ϼ���!" );

		return;
	}
	m_eMode = ePackMode_Extract;

	switch( m_eMode )
	{
	case ePackMode_Extract:
		LoadPackFile();
		break;
	case ePackMode_Compress:
		LoadCompressFolder();
		break;
	}
}

void CPackExtractDlg::OnBnClickedModeCompressRadio()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_bIsWorking && m_eMode == ePackMode_Compress )
	{
		m_ModeExtractBtn.SetCheck( TRUE );
		m_ModeCompressBtn.SetCheck( FALSE );
		AfxMessageBox( "�۾��� ���� �Ŀ� �õ��ϼ���!" );

		return;
	}
	m_eMode = ePackMode_Compress;

	switch( m_eMode )
	{
	case ePackMode_Extract:
		LoadPackFile();
		break;
	case ePackMode_Compress:
		LoadCompressFolder();
		break;
	}
}

void CPackExtractDlg::OnBnClickedDeleteAfterProcCheck()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData( TRUE );
}

LRESULT CPackExtractDlg::OnUpdateDlg( WPARAM wParam, LPARAM lParam )
{
	UpdateData( BOOL(wParam) );

	return 0;
}

void CPackExtractDlg::OnBnClickedExtractFilenameButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( m_eMode != ePackMode_Extract )
		return;

	int nIndex = m_PackListBox.GetCurSel();
	int nCount = m_PackListBox.GetCount();

	if( (nIndex != LB_ERR) && (nCount > nIndex) )
	{
		CString str;
		int nTextLen = m_PackListBox.GetTextLen( nIndex );
		m_PackListBox.GetText( nIndex, str.GetBuffer( nTextLen ) );

		OpenExtractPackFileWithSelectListDlg( str );
	}
	else
	{
		AfxMessageBox( "�׸��� �����ϰ� �����ϼž� �մϴ�!" );
	}
}

void CPackExtractDlg::OnLbnDblclkPackList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedExtractFilenameButton1();
}


void CPackExtractDlg::ExpandTreeItem( HTREEITEM hItem )
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	LPITEMIDLIST pidl;
	LPSHELLFOLDER pFolder;
	pidl = (LPITEMIDLIST) m_ExtractFolderPos.GetItemData( hItem );
	if ( hItem != m_hDesktopItem)
	{
		m_pDesktopFolder->BindToObject(pidl, NULL, IID_IShellFolder, (LPVOID *)&pFolder);
		EnumChildItems(pFolder, pidl, hItem);
		pFolder->Release();
	}
	else
		EnumChildItems(m_pDesktopFolder, pidl, hItem);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

HTREEITEM CPackExtractDlg::GetTreeItem( HTREEITEM hRootItem, LPCTSTR szName, BOOL bExactly )
{
	if( hRootItem == NULL )
		hRootItem = m_ExtractFolderPos.GetRootItem();

	HTREEITEM hChildItem = m_ExtractFolderPos.GetNextItem( hRootItem, TVGN_CHILD );
	if( hChildItem == NULL )
	{
		ExpandTreeItem( hRootItem );
		hChildItem = m_ExtractFolderPos.GetNextItem( hRootItem, TVGN_CHILD );
	}
	CString strChildName = m_ExtractFolderPos.GetItemText( hChildItem );
	strChildName.MakeUpper();
	CString strTargetName( szName );
	strTargetName.MakeUpper();


	if( bExactly )
	{
		if( strcmp( (LPCTSTR)strChildName, strTargetName ) == 0 )
			return hChildItem;
	}
	else
	{
		if( strstr( (LPCTSTR)strChildName, strTargetName ) != NULL )
			return hChildItem;
	}

	while( (hChildItem = m_ExtractFolderPos.GetNextItem( hChildItem, TVGN_NEXT )) != NULL )
	{
		strChildName = m_ExtractFolderPos.GetItemText( hChildItem );
		strChildName.MakeUpper();
		if( bExactly )
		{
			if( strcmp( (LPCTSTR)strChildName, strTargetName ) == 0 )
				return hChildItem;
		}
		else
		{
			if( strstr( (LPCTSTR)strChildName, strTargetName ) != NULL )
				return hChildItem;
		}
	}

	return NULL;
}

HTREEITEM CPackExtractDlg::GetTreeItem( HTREEITEM hRootItem, int csidl )
{
	if( hRootItem == NULL )
		hRootItem = m_ExtractFolderPos.GetRootItem();

	HTREEITEM hChildItem = m_ExtractFolderPos.GetNextItem( hRootItem, TVGN_CHILD );
	if( hChildItem == NULL )
	{
		ExpandTreeItem( hRootItem );
		hChildItem = m_ExtractFolderPos.GetNextItem( hRootItem, TVGN_CHILD );
	}

	LPITEMIDLIST pidl;
	if( SHGetSpecialFolderLocation(NULL, csidl, &pidl) != S_OK )
		return NULL;

	CString strChildName = m_ExtractFolderPos.GetItemText( hChildItem );
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	UINT uFlags = SHGFI_PIDL | SHGFI_DISPLAYNAME;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	SHGetFileInfo((LPCTSTR)pidl, 0, &sfi, sizeof(SHFILEINFO), uFlags);

	if( strcmp( (LPCTSTR)strChildName, sfi.szDisplayName ) == 0 )
		return hChildItem;

	while( (hChildItem = m_ExtractFolderPos.GetNextItem( hChildItem, TVGN_NEXT )) != NULL )
	{
		strChildName = m_ExtractFolderPos.GetItemText( hChildItem );
		if( strcmp( (LPCTSTR)strChildName, sfi.szDisplayName ) == 0 )
			return hChildItem;
	}

	return NULL;
}

BOOL CPackExtractDlg::UpdateTreePath( CString strPath )
{
	HTREEITEM hItem = GetTreeItem( NULL, CSIDL_DRIVES );
	if( hItem == NULL )
		return FALSE;

	HTREEITEM hTargetTree = NULL;
	CString token;
	int nCurPos = 0;
	token = strPath.Tokenize( "\\", nCurPos );
	while( token != "" )
	{
		hTargetTree = GetTreeItem( hItem, token, TRUE );
		if( hTargetTree == NULL )
		{
			hTargetTree = GetTreeItem( hItem, token, FALSE );
			if( hTargetTree == NULL )
				return FALSE;

		}

		m_ExtractFolderPos.SelectItem( hTargetTree );
		hItem = hTargetTree;
		token = strPath.Tokenize( "\\", nCurPos );
	}

	if( hTargetTree == NULL )
		return FALSE;

	// ��� Ȯ��
	LPITEMIDLIST pidl;
	pidl = (LPITEMIDLIST)m_ExtractFolderPos.GetItemData( hTargetTree );
	char szTargetPath[MAX_PATH];
	::SHGetPathFromIDList( pidl, (LPSTR)(LPCTSTR)szTargetPath );
	CString strTargetPath = szTargetPath;

	// �빮�ڷ� ��ȯ
	strTargetPath.MakeUpper();
	strPath.MakeUpper();

	// �������� '\'�� �ƴѰ�� '\'�� �ٿ��ش�
	if( strPath.GetAt( strPath.GetLength() - 1 ) != '\\' )
		strPath.Append( "\\" );
	if( strTargetPath.GetAt( strPath.GetLength() - 1 ) != '\\' )
		strTargetPath.Append( "\\" );

	if( strPath.Compare( strTargetPath ) != 0 )
	{
		return FALSE;
	}

	// Ȱ��ȭ �� ��Ŀ���� �Ѱ��ش�
	SetForegroundWindow();
    m_ExtractFolderPos.SetFocus();
	return TRUE;
}
void CPackExtractDlg::OnDropFiles(HDROP hDropInfo)
{
	// Drag File ���� ����
	int nDragFileCnt = DragQueryFile( hDropInfo, 0xFFFFFFFF, NULL, 0 );

	// 1��° Drag File �̸� �о����
	char szPath[256];
	DragQueryFile( hDropInfo, 0, szPath, 256 );
	DWORD dwFileAttr = GetFileAttributes( szPath );
	

	// Directory �� ���
	if( dwFileAttr & FILE_ATTRIBUTE_DIRECTORY )
	{
		// Mode ����
		m_eMode = ePackMode_Compress;
		m_ModeExtractBtn.SetCheck( FALSE );
		m_ModeCompressBtn.SetCheck( TRUE );

		CString strPath = szPath;
		// ���� ������ ��ġ�� �����Ѵ�.
		strPath = strPath.Left( strPath.ReverseFind( '\\' ) );
		UpdateTreePath( strPath );

		m_PackListBox.ResetContent();
		m_vCompressFolder.clear();

		for( int index=0 ; index < nDragFileCnt ; ++index )
		{
			DragQueryFile( hDropInfo, index, szPath, 256 );
			DWORD dwFileAttr = GetFileAttributes( szPath );
			// Directory�� �ƴ� ���
			if( !(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) )
				continue;

			CString strFolder = szPath;
			strFolder.Delete( 0, strFolder.ReverseFind( '\\' ) + 1 );

			m_PackListBox.AddString( strFolder );
			m_vCompressFolder.push_back( strFolder );
		}
	}
	// File �� ���
	else
	{
		// Mode ����
		m_eMode = ePackMode_Extract;
		m_ModeExtractBtn.SetCheck( TRUE );
		m_ModeCompressBtn.SetCheck( FALSE );

		CString strPath = szPath;
		// ���ϰ�ο��� ������θ� �����.
		strPath = strPath.Left( strPath.ReverseFind( '\\' ) );

		// Drop File�� ��η� Tree�� �����Ѵ�.
		if( UpdateTreePath( strPath ) == FALSE )
		{
			// ���� ���н� �⺻(������)���� �ǵ�����.
			UpdateTreePath( m_szBaseDirectory );
			return;
		}

		// ���õ� File�� List�� ������ �ֵ��� �����Ѵ�.
		m_PackListBox.ResetContent();
		m_vPackFile.clear();

		for( int index=0 ; index < nDragFileCnt ; ++index )
		{
			DragQueryFile( hDropInfo, index, szPath, 256 );
			DWORD dwFileAttr = GetFileAttributes( szPath );
			// Directory�� ���
			if( dwFileAttr & FILE_ATTRIBUTE_DIRECTORY )
				continue;

			strPath = szPath;
			strPath.Delete( 0, strPath.ReverseFind( '\\' ) + 1 );

			// pak ������ �ƴѰ��
			if( strstr( (LPCTSTR)strPath, ".pak" ) == NULL )
				continue;

			m_PackListBox.AddString( strPath );
			m_vPackFile.push_back( strPath );
		}
	}

	

	CDialog::OnDropFiles(hDropInfo);
}

void CPackExtractDlg::OnBnClickedHelpButton()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
