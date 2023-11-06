#pragma once
//=================================================================================================
//	FILE		: TutorialDlg.h
//	PURPOSE		: Window interface to render speech and NPC image of TUTORIAL.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		GM툴 모드로 실행 할 때 에러 메시지를 출력하기 위한 에러 메시저 정의
//=========================================================================
#ifdef _GMTOOL_

#define TDERR_MSG1		"Invalid tutorial pointer"							// 인자로 넘어온 튜토리얼 포인터 에러.
#define TDERR_MSG2		"Failed to receive subtutorial info : %d"			// 인자로 넘어온 튜토리얼 포인터 에러.
#define TDERR_MSG3		"Failed to receive msg info : %d"					// 메시지를 받는데 실패.
#define TDERR_MSG4		"There is no message : %d"							// 메시지가 없다.
#define TDERR_MSG5		"Failed to receive msg line : %d msg - %d line"		// 몇 번 메시지의 몇 번 라인을 받는데 실패.
#define TDERR_MSG6		"Invalid current sub tutorial info"					// 현재 서브 퀘스트 정보 에러.
#define TDERR_MSG7		"Failed to receive npc file name - npc%d emotion%d" // npc 이미지 받기 실패.
#define TDERR_MSG8		"Failed to load sprite of npc - npc%d emotion%d"	// npc의 이미지 스프라이트 로드 실패.
#define TDERR_MSG9		"Invalid check condition of npc talk : %d npc"		// 잘못된 npc 대화 조건 체크라는 에러.
#define TDERR_MSG10		"Invalid sound index : %d - %d - %d"				// 몇번 튜토리얼 몇번 서브 튜토리얼의 사운드 인덱스 에러.

#endif 





//=========================================================================
//		필요한 헤더파일을 불러온다.
//=========================================================================
#include "./Interface/cDialog.h"											// 다이얼로그 인터페이스를 포함한다.
#include "cImageSelf.h"





//=========================================================================
//		필수 정의 파트.
//=========================================================================
#define TUTORIAL_FONTIDX	7												// 튜토리얼 출력용 폰트 인덱스를 정의한다.
#define NPC_IMAGE_PATH		"./data/interface/2dimage/npciamge/"			// NPC 이미지를 로드하기 위한 경로를 정의 한다.





//=========================================================================
//		필요한 컨트롤 인터페이스를 선언한다.
//=========================================================================
class cButton ;																// 버튼 클래스를 선언한다.
class cImageSelf ;															// 이미지 클래스를 선언한다.
class cListDialog ;															// 리스트 다이얼로그 클래스를 선언한다.
class cStatic ;																// 스태틱 클래스를 선언한다.

class cTutorial ;															// 튜토리얼 클래스를 선언한다.

struct SUB_TUTORIAL ;														// 서브 튜토리얼 구조체를 선언한다.





//=========================================================================
//		튜토리얼 다이얼로그 클래스 정의
//=========================================================================
class cTutorialDlg : public cDialog											// 튜토리얼 다이얼로그 클래스.
{
	cListDialog*		m_pSpeechList ;										// 튜토리얼 대사 리스트 다이얼로그.

	cButton*			m_pOkBtn ;											// 확인 버튼 포인터.
	cButton*			m_pPrevBtn ;										// 이전 버튼 포인터.
	cButton*			m_pNextBtn ;										// 다음 버튼 포인터.
	cButton*			m_pPassBtn ;										// 패스 버튼 포인터.

	cStatic*			m_pSpeechBack ;										// 대사 배경 이미지 스태틱 포인터.

	cTutorial*			m_pTutorial ;										// 멤버 튜토리얼 포인터.
	SUB_TUTORIAL*		m_pCurSubTutorial ;									// 현재 서브 튜토리얼의 정보를 담을 포인터.

	BOOL				m_bShowTutorial ;									// 튜토리얼을 보여줄 지 여부를 결정하는 변수.

	int					m_nCurSubIndex ;									// 현재 서브 튜토리얼 인덱스를 담는 변수.
	int					m_nTotalSubCount ;									// 총 서브 튜토리얼의 개수를 담는 변수.

	cImageSelf			m_NpcImage ;										// NPC 이미지 출력을 위한 이미지 포인터.
	int					m_nImageAlpha ;										// NPC 이미지의 알파 값을 담을 변수.
	DWORD				m_dwStartTime ;

	int					m_nDispWidth ;										// 스크린 가로 사이즈.
	int					m_nDispHeight ;										// 스크린 세로 사이즈.

	int					m_nBtnStartXpos ;									// 버튼 시작 x좌표.
	int					m_nBtnStartYpos ;									// 버튼 시작 y좌표.

	VECTOR2				m_vImagePos ;										// npc 이미지 출력 위치 벡터.

	cPtrList			m_SpeechMsgList ;									// 대사 내용 메시지 리스트.

public:
	cTutorialDlg(void);														// 생성자 함수.
	virtual ~cTutorialDlg(void);											// 소멸자 함수.

	void Linking() ;														// 다이얼로그 하위 컨트롤들을 링크하는 함수.

	virtual void Render() ;													// 렌더 함수.

	//virtual DWORD ActionEvent(CMouse* mouseInfo) ;							// 마우스 이벤트 처리를 할 함수.
	virtual void OnActionEvent(LONG lId, void* p, DWORD we) ;				// 지정 된 컨트롤 이벤트를 처리 할 함수.

	void Set_TutorialInfo(cTutorial* pTutorial) ;							// 튜토리얼 정보를 세팅하는 함수.
	void Start_Tutorial() ;													// 튜토리얼 진행함수.
	//void Start_Tutorial(cTutorial* pTutorial) ;								// 튜토리얼 진행함수.

	void LoadNpcImage() ;													// ncp 이미지를 로딩하는 함수.

	void Prepare_Tutorial() ;												// 튜토리얼 진행을 미리 준비하는 함수.

	void ResetSpeechMsgList() ;												// 대사 리스트를 다시 세팅하는 함수.

	void DeActiveAllBtn() ;													// 모든 버튼을 비활성화 하는 함수.
	void ActiveBtns() ;														// 상황에 맞는 버튼을 활성화 하는 함수.

	BOOL IsSameEndNpc(DWORD dwNpcIdx) ;										// 튜토리얼 종료 조건과 같은 npc 인지 체크하는 함수.

	BOOL IsHavePrevSubTutorial() ;											// 이전 서브 튜토리얼이 있는지 여부를 체크하는 함수.
	BOOL IsHaveNextSubTutorial() ;											// 다음 서브 튜토리얼이 있는지 여부를 체크하는 함수.
	void StartNextSubTutorial() ;											// 다음 서브 튜토리얼을 진행하는 함수.

	void UpdateTutorial() ;													// 튜토리얼을 완료하고 업데이트 하는 함수.

	SUB_TUTORIAL* GetCurSubTutorial() ;										// 현재 서브 튜토리얼을 반환하는 함수.

	cStatic* GetSpeechBack() { return m_pSpeechBack ; }						// 대사 배경을 반환하는 함수.
};
