// FilteringTable.h: interface for the CFilteringTable class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CFILTERINGTABLE_H_
#define _CFILTERINGTABLE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MHCLIENT_
#define FILTERTABLE	USINGTON(CFilteringTable)
#else
#define FILTERTABLE	CFilteringTable::GetInstance()
#endif


#ifdef _FILE_BIN_
#define FILEPATH_FILTERWORD "./System/Resource/FilterWord.bin"
#else
#define FILEPATH_FILTERWORD "./System/Resource/FilterWord.txt"
#endif


enum FILTERKIND
{
	eFilter_GM,
	eFilter_System,
	eFilter_Slang,
	eFilter_Byte,
	
	eFilter_Count,
};


enum eFILTER_METHOD
{
						// ��) ������ ���ʹܾ��� ��
	eFM_WHOLE_MATCH,	//-> ����		: ����
	eFM_INCLUDE,		//-> ������		: ����	
	eFM_ALLOWSPACE,		//-> ��������	: ����	
};

// desc_hseos_�������͸�01
// S �������͸� �߰� added by hseos 2007.06.25
// ..���͸����� ���ܷ� ó���� ��� �� Ÿ��
enum FILTER_EXCEPTION_TYPE
{
	FET_NONE = 0,				// ����
	FET_WHISPER_CHARNAME,		// �Ӹ� ĳ���͸�
};
// E �������͸� �߰� added by hseos 2007.06.25

//#define _FILTER_TEST_

struct FILTER_NODE
{
	char cChar;
	char cExChar;
//	char bSpread;
	char bEndFlag;

	FILTER_NODE* pChild;
	FILTER_NODE* pSibling;
//	FILTER_NODE* pParent;

	FILTER_NODE():pChild(0),pSibling(0),/*pParent(0),bSpread(0),*/bEndFlag(0),cExChar(0),cChar(0) {}
};


class CFilteringTable
{

#ifdef _JAPAN_LOCAL_

protected:
	struct RANGE_ARRAY
	{
		WORD Start;
		WORD End;

		BOOL IsInRange( WORD DBChar )
		{
			if( DBChar >= Start && DBChar <= End ) return TRUE;
			return FALSE;
		}
	};

	RANGE_ARRAY*	m_RangeArray;
	DWORD			m_dwRangeArrayCount;

public:

	BOOL IsInDBCRange( WORD DBChar )
	{
		for( DWORD i = 0 ; i < m_dwRangeArrayCount ; ++i )
		{
			if( m_RangeArray[i].IsInRange( DBChar ) )
				return TRUE;
		}
		return FALSE;
	}
	


#endif

protected:

	FILTER_NODE		m_RootNode[eFilter_Count];

	int				m_nStrPos;
//	char*			m_strFile;		//���͸���ũ��Ʈ���� ù�ּ�
	
//�߰��� ã���� �����ϱ� ���� ������
	BOOL			m_bSearched;
	BOOL			m_bSpread;
	
	char			m_strBuf[256];
//

	FILTER_NODE*	m_pCurNode;

	char*			m_pCurStrPos;

#ifdef _FILTER_TEST_
	int				m_nMemory;
#endif

	// desc_hseos_�������͸�01
	// S �������͸� �߰� added by hseos 2007.06.25
	FILTER_EXCEPTION_TYPE	m_eFilterExceptionType;
	// E �������͸� �߰� added by hseos 2007.06.25
	
protected:

	void AddWord( char* pWord, int nKind );
	void AddNode( FILTER_NODE* pNode, char* pWord );
	void DeleteNode( FILTER_NODE* pNode );
	void CleanNode( FILTER_NODE* pNode );

	void SearchNode( FILTER_NODE* pNode, char* pStr, int nDepth, BOOL bDBC );	
//	void SearchNode_AllowSpace( FILTER_NODE* pNode, char* pStr, BOOL bDBC );

	BOOL LoadFilterWord( char* strFile );
	BOOL LoadFilterWordBinary( char* strFile );
	void SettingFilterWord( char* pStr, DWORD bufLen );

	BOOL FM_WholeMatch( char* pStr, int nKind );
	BOOL FM_Include( char* pStr, int nKind );
//	BOOL FM_AllowSpace( char* pStr, int nKind, int nAllowSpace );

	int RemoveSpace( char* pStrIn, char* pStrOut );

public:

#ifndef _MHCLIENT_
	MAKESINGLETON( CFilteringTable )
#endif

	CFilteringTable();
	virtual ~CFilteringTable();
	
	void Init();
	void Release();

	BOOL FilterWordInString( char* pStr, int nKind, int nMethod );

	BOOL IsInvalidCharInclude( unsigned char* pStr, BOOL AllowAllDBC = FALSE );
	BOOL IsUsableName( char* pName, BOOL bUseByte = TRUE );
	BOOL FilterChat( char* pChat );

	BOOL IsCharInString( char* pStr, char* pNoChar );

	// desc_hseos_�������͸�01
	// S �������͸� �߰� added by hseos 2007.06.25
	void SetExceptionInvalidCharIncludeType(FILTER_EXCEPTION_TYPE eFilterExceptionType) { m_eFilterExceptionType = eFilterExceptionType; }
	BOOL IsExceptionInvalidCharInclude(unsigned char* pszText);
	// E �������͸� �߰� added by hseos 2007.06.25

#ifdef _FILTER_TEST_
	int	GetMemory() { return m_nMemory; }
#endif
};

#ifdef _MHCLIENT_
EXTERNGLOBALTON(CFilteringTable)
#endif





#endif // _CFILTERINGTABLE_H_
