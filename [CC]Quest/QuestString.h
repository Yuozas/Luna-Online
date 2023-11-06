#pragma once


#include "..\[CC]Quest\QuestDefines.h"


//struct QString
//{
//	QString() :
//	Str( 0 ),
//	Line( 0 )
//	{}
//
//	~QString()
//	{
//		if(Str) delete [] Str;
//	}
//
//	char* Str;
//	int		Line;
//	DWORD	Color;
//
//	void operator = (char* pchar)
//	{
//		Str = new char[strlen(pchar)+1];
//		strcpy(Str,pchar);
//	}
//	operator LPCTSTR() const
//	{
//		return Str;
//	}
//	operator PCHAR() const
//	{
//		return Str;
//	}
//};

struct QString
{
	char Str[128] ;
	int		Line;
	DWORD	Color;
};



class CQuestString												// ����Ʈ ��Ʈ�� Ŭ����.
{	
	cPtrList		m_TitleList;								// Ÿ��Ʋ ����Ʈ.
	cPtrList		m_DescList;									// ���� ����Ʈ.

	DWORD			m_mIndex;									// ���� �ε���.
	DWORD			m_sIndex;									// ���� �ε���.
	DWORD			m_Key;										// Ű���� ��� ����.

	BOOL			m_bTreeTitle ;								// Ʈ�� �������� ���θ� Ȯ���ϴ� ����.

	char			m_TitleBuf[128] ;

	BOOL			m_bComplete ;

	int				m_nCount ;
	int				m_nTotalCount ;
	
public:
	
	CQuestString();												// ������ �Լ�.
	virtual ~CQuestString();									// �Ҹ��� �Լ�.

	void	StringLoader(char* str);							// ��Ʈ���� �ε��ϴ� �Լ�.

	void	SetIndex(DWORD mIdx, DWORD sIdx)		{	m_mIndex = mIdx;	m_sIndex=sIdx;	COMBINEKEY(m_mIndex, m_sIndex, m_Key);	}	// �ε����� �����ϴ� �Լ�.

	void	SetQuestID(DWORD dwIdx)	{ m_mIndex = dwIdx ; }		// ���� ����Ʈ �ε����� �����ϴ� �Լ�.
	DWORD	GetQuestID()	{	return m_mIndex;	}			// ���� ����Ʈ �ε����� ��ȯ�ϴ� �Լ�.

	void	SetSubID(DWORD dwIdx) { m_sIndex = dwIdx ; }		// ���� ����Ʈ �ε����� �����ϴ� �Լ�.
	DWORD	GetSubID()		{	return m_sIndex;	}			// ���� ����Ʈ �ε����� ��ȯ�ϴ� �Լ�.

	DWORD	GetKey()		{	return m_Key;		}			// Ű ���� ��ȯ�ϴ� �Լ�.

	BOOL IsHighlight(char* pStr, int& line, BOOL bTitle = FALSE) ;
	void	AddLine(char* pStr, int& line, BOOL bTitle=FALSE);	// ������ �߰��ϴ� �Լ�.

	cPtrList*	GetTitle()	{	return &m_TitleList;	}		// Ÿ��Ʋ ����Ʈ�� ��ȯ�ϴ� �Լ�.
	cPtrList*	GetDesc()	{	return &m_DescList;	}			// ���� ����Ʈ�� ��ȯ�ϴ� �Լ�.
	//void ReleaseComponents() ;

	void SetTitleStr(char* pStr) { memset(m_TitleBuf, 0, 128) ; memcpy(m_TitleBuf, pStr, strlen(pStr)) ; }
	char* GetTitleStr() { return m_TitleBuf ; }

	void SetTreeTitle(BOOL val) { m_bTreeTitle = val ; }
	BOOL IsTreeTitle() { return m_bTreeTitle ; }

	void SetComplete(BOOL val) 
	{
		m_bComplete = val ; 
	}
	BOOL IsComplete() { return m_bComplete ; }

	void SetCount(int nCount) { m_nCount = nCount ; }
	int  GetCount() { return m_nCount ; }

	void SetTotalCount(int nTotalCount) { m_nTotalCount = nTotalCount ; }
	int  GetTotalCount() { return m_nTotalCount ; }
};
