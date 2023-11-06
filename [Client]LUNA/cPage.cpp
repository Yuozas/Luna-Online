

#include "stdafx.h"													// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "cPage.h"													// 페이지 클래스 헤더를 불러온다.

/////////////////////////////////
// cPageBase Class
/////////////////////////////////
cPageBase::cPageBase()												// 생성자 함수.
{
	m_nDialogueCount = 0;											// 다이얼로그 카운트를 0으로 세팅한다.
	
	m_nNextPageId = -1;												// 다음 페이지 아이디를 -1로 세팅한다.
	m_nPrevPageId = -1;												// 이전 페이지 아이디를 -1로 세팅한다.

	mEmotion = 0;													// 이모션을 0으로 세팅한다.
}


cPageBase::~cPageBase()												// 소멸자 함수.
{
	RemoveAll();													// 모두 비우기 함수를 호출한다.
}

void cPageBase::RemoveAll()											// 모두 비우기 함수.
{
	POS pos = m_pDialogue.GetFirstPos();							// 다이얼로그 리스트의 첫 위치 정보를 받는다.

	if(!pos) return ;												// 위치 정보가 유효한지 체크한다.

	DWORD* pData = NULL ;											// 정보를 받을 포인터 변수를 선언하고, NULL 처리를 한다.

	while( pData = m_pDialogue.GetNextPos( pos ) )					// 위치 정보에 해당하는 정보를 받는다.
	{
		delete pData;												// 정보를 삭제한다.
		pData = NULL ;												// 벙보를 나타내는 포인터를 NULL 처리를 한다.
	}

	m_pDialogue.DeleteAll();										// 다이얼로그 리스트를 모두 비운다.
}

void cPageBase::Init( DWORD dwId, BYTE emotion )					// 초기화 함수.
{
	m_dwPageId = dwId;												// 페이지 아이디를 세팅한다.

	mEmotion = emotion;												// 이모션을 세팅한다.
}

void cPageBase::AddDialogue(DWORD dwId)								// 다이얼로그 추가함수.
{
	DWORD* dwVal = new DWORD;										// DWORD형 메모리를 할당한다.

	*dwVal = dwId;													// 아이디를 세팅한다.

	m_pDialogue.AddTail(dwVal);										// 다이얼로그 리스트에 추가한다.

	++m_nDialogueCount;												// 다이얼로그 카운트를 증가한다.
}

DWORD cPageBase::GetDialogue()
{
	if( m_nDialogueCount < 1 ) return 0;							// 다이얼로그 카운트가 0과 같으면 0을 리턴한다.
	else
	{
		return *m_pDialogue.GetAt(0) ;
	}
}

DWORD cPageBase::GetDialogue(int index)
{
	if( m_nDialogueCount < 1 ) return 0 ;
	else if( index < 0 || index  > m_nDialogueCount - 1 )
	{
		return 0 ;
	}
	else
	{
		return *m_pDialogue.GetAt(index) ;
	}
}

DWORD cPageBase::GetRandomDialogue()								// 랜덤 다이얼로그를 반환하는 함수.
{
	if( m_nDialogueCount == 0 ) return 0;							// 다이얼로그 카운트가 0과 같으면 0을 리턴한다.


	if( m_nDialogueCount > 1 )										// 다이얼로그 카운트가 1개 보다 많다면,
	{
		return *(m_pDialogue.GetAt(rand()%m_nDialogueCount));		// 랜덤하게 다이얼로그 정보를 리턴한다.
	}
	else															// 다이얼로그 카운트가 1개 이하면,
	{
		return *(m_pDialogue.GetAt(0));								// 첫 번째 다이얼로그 정보를 리턴한다.
	}
}

/////////////////////////////////
// cPage Class
/////////////////////////////////
cPage::cPage()														// 생성자 함수.
{
	m_nHyperLinkCount = 0;											// 하이퍼 링크 카운트를 0으로 세팅한다.
}

cPage::~cPage()														// 소멸자 함수.
{
	RemoveAll();													// 모두 비우기 함수를 호출한다.
}

void cPage::RemoveAll()												// 모두 비우기 함수.
{
	POS pos = m_pHyperLink.GetFirstPos();							// 하이퍼 링크 리스트의 첫 위치 정보를 받는다.

	if( !pos ) return ;												// 위치 정보가 유효하지 않으면 리턴 처리를 한다.

	HYPERLINK* pHL = NULL ;											// 하이퍼 링크 정보를 받을 포인터를 선언하고 NULL 처리를 한다.

	while( pHL = m_pHyperLink.GetNextPos( pos ) )					// 포인터 정보가 유효할 동안 while문을 돌린다.
	{
		delete pHL;													// 정보를 삭제한다.
		pHL = NULL ;												// 포인터를 NULL 처리를 한다.
	}

	m_pHyperLink.DeleteAll();										// 하이퍼 링크 리스트를 모두 비운다.
}

void cPage::AddHyperLink(HYPERLINK* pLink)							// 하이퍼 링크 정보를 추가하는 함수.
{
	HYPERLINK* pVal = new HYPERLINK;								// 하이퍼 링크를 담을 메모리를 할당한다.

	memcpy( pVal, pLink, sizeof(HYPERLINK) );						// 하이퍼 링크 정보를 복사한다.

	m_pHyperLink.AddTail( pVal );									// 하이퍼 링크 리스트에 정보를 추가한다.

	++m_nHyperLinkCount;											// 하이퍼 링크 카운트를 증가한다.
}

HYPERLINK* cPage::GetHyperText( DWORD dwIdx )						// 하이퍼 링크 정보를 반환하는 함수.
{
	return m_pHyperLink.GetAt(dwIdx);								// 인덱스에 해당하는 하이퍼 링크 정보를 받환한다.
}