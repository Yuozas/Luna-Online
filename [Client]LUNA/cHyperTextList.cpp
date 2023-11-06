

#include "stdafx.h"															// 표준 시스템과, 프로젝트가 지정한, 자주쓰는 해더들을 모은 해더파일을 불러온다.

#include "../MHFile.h"														// 묵향 파일 클래스 헤더를 불러온다.

#include "cHyperTextList.h"													// 하이퍼 텍스트 리스트 클래스 헤더를 불러온다.


cHyperTextList::cHyperTextList()											// 생성자 함수.
{
	m_HyperText.Initialize(1000);											// 다이얼로그 정보를 담는 테이블을 1000개로 초기화 한다.
}

cHyperTextList::~cHyperTextList()											// 소멸자 함수.
{
	m_HyperText.SetPositionHead();											// 테이블을 헤드 포지션으로 세팅한다.

	DIALOGUE* value;														// 다이얼로그 정보를 받을 포인터를 선언한다.

	while(value = m_HyperText.GetData())									// 다이얼로그 정보가 없을 때까지 while문을 돌린다.
	{
		delete value;														// 다이얼로그 정보를 삭제한다.
	}
			
	m_HyperText.RemoveAll();												// 테이블을 모두 비운다.
}

void cHyperTextList::LoadHyperTextFormFile(char* filePath, char* mode)		// 파일로 부터 하이퍼 텍스트 정보를 로딩하는 함수.
{
	int nLen;																// 길이 정보를 담을 변수.

	DIALOGUE* pTemp;														// 다이얼로그 정보를 담을 포인터.

	CMHFile fp;																// 묵향 파일을 선언한다.

	if(!fp.Init(filePath, mode))											// 받은 경로의 파일을 받은 모드로 열기에 실패하면,
	{
		return;																// 리턴 처리를 한다.
	}

	char buff[256]={0,};													// 임시 버퍼를 선언한다.

	DWORD idx;																// 인덱스 정보를 담을 변수를 선언한다.

	while(1)																// while문을 돌린다.
	{
		idx = fp.GetInt();													// 인덱스를 받는다.

		if(fp.IsEOF())														// 포인터가 파일의 끝을 가리키면,
		{
			break;															// while문을 탈출한다.
		}

		fp.GetLine( buff, 256 );											// 임시 버퍼에 라인을 읽어들인다.

		if( nLen = strlen(buff) )											// 길이 정보와 버퍼의 크기가 같으면,
		{
			pTemp = new DIALOGUE;											// 다이얼로그 정보 메모리를 할당한다.

			pTemp->Init();													// 다이얼로그 정보를 초기화 한다.

			strcpy(pTemp->str, buff);										// 다이얼로그 정보의 스트링에 버퍼의 내용을 복사한다.

			m_HyperText.Add(pTemp, idx);									// 하이퍼 텍스트를 추가한다.
		}
	}
}

DIALOGUE* cHyperTextList::GetHyperText( DWORD dwIdx )						// 다이얼로그 정보를 반환하는 함수.
{
	return (DIALOGUE*)m_HyperText.GetData(dwIdx);							// 받아들인 인덱스에 해당하는 다이얼로그 정보를 반환한다.
}