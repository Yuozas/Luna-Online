#ifndef __DIALOGUE_LIST__
#define __DIALOGUE_LIST__

#include "../MHFile.h"																	// 묵향 파일 클래스 헤더를 불러온다.

#define MAX_DIALOGUE_COUNT 12800														// 최대 다이얼로그 카운트를 12800개로 선언한다.

#define STRESS_COLOR	RGB(0,255,255)													// 스트레스 컬러를 정의한다.
#define NORMAL_COLOR	RGB(255,255,255)												// 일반컬러를 정의한다.

class cDialogueList																		// 다이얼로그 리스트 클래스.
{
protected:
	cPtrList	m_Dialogue[MAX_DIALOGUE_COUNT];											// 리스트형 다이얼로그 배열을 최대 다이얼로그 카운트 크기만큼 선언한다.	

	DWORD		m_dwDefaultColor;														// 일반 색상을 담을 변수.
	DWORD		m_dwStressColor;														// 스트레스 색상을 담을 변수.

	void LoadDialogueList(DWORD dwId, CMHFile* fp);										// 다이얼로그 리스트를 로딩하는 함수.

public:
	cDialogueList();																	// 생성자 함수.
	virtual ~cDialogueList();															// 소멸자 함수.

	void LoadDialogueListFile(char* filePath, char* mode = "rt");						// 다이얼로그 리스트 파일을 로드하는 함수.

	DIALOGUE* GetDialogue( DWORD dwMsgId, WORD wLine );									// 다이얼로그 정보를 반환하는 함수.

	void ParsingLine( DWORD dwId, char* buf );											// 라인 정보를 파싱하는 함수.

	void AddLine( DWORD dwId, char* str, DWORD color, WORD Line, WORD type);			// 라인을 추가하는 함수.

};

#endif