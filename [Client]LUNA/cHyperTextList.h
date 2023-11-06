#ifndef _HYPERTEXTLIST_
#define _HYPERTEXTLIST_

//#include "PtrList.h"

class cHyperTextList													// 하이퍼 텍스르 리트스 클래스.
{
	CYHHashTable<DIALOGUE>	m_HyperText;								// 다이얼로그 정보를 담는 테이블.
//	cPtrList	m_HyperText;
public:
	cHyperTextList();													// 생성자 함수.
	virtual ~cHyperTextList();											// 소멸자 함수.

	void LoadHyperTextFormFile(char* filePath, char* mode = "rt");		// 파일로 부터 하이퍼 텍스트를 로드하는 함수.

	DIALOGUE* GetHyperText( DWORD dwIdx );								// 다이얼로그 정보를 반환하는 함수.
};

#endif