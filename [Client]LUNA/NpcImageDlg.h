#pragma once
#include ".\interface\cdialog.h"									// 다이얼로그 인터페이스 클래스 헤더 불러오기.
#include "cImageSelf.h"												// 이미지 셀프 클래스 헤더 불러오기.

#include "cStatic.h"												// 스태틱 클래스 헤더 불러오기.

enum NPC_EMOTION													// NPC 모션 이넘 코드.
{
	EMOTION_NORMAL,													// 기본 모션.
	EMOTION_HAPPY,													// 행복 모션.
	EMOTION_SURPRISE,												// 놀람 모션.
	EMOTION_UNHAPPY,												// 불행 모션.
	EMOTION_ANGRY,													// 화남 모션.

	EMOTION_MAX,													// 모션 최대수치.
};

struct NPC_IMAGE													// NPC 이미지 구조체.
{
	WORD		idx;												// 인덱스.
	cImageSelf	image[5];											// 이미지 배열.
};

class cNpcImageDlg : public cDialog									// 기본 다이얼로그를 상속받은 NPC 이미지 다이얼로그 클래스.
{
	WORD	mNpcIdx;												// NPC 인덱스.
	BYTE	mEmotion;												// 모션번호.
	DWORD	COLOR;													// 색상 변수.
	DWORD	COLOR2;													// 색상 변수.

	VECTOR2	SCALE;													// 스케일 벡터.
	VECTOR2	POS;													// 위치 벡터.
	VECTOR2 BACK_SCALE;												// 배경 스케일 벡터.
	VECTOR2 BACK_POS;												// 배경 위치 벡터.

	//cImage	mImageTop;												// 탑 이미지.
	cImage	mImageBack;												// 배경 이미지.
	//cImage	mImageBottom;											// 버텀 이미지.

	NPC_IMAGE* mpCurImage;											// NPC 이미지 포인터.

	cStatic* m_pTopImage ;											// 탑이미지 스태틱.
	cStatic* m_pBottomImage ;										// 버텀 이미지 스태틱.

	int m_nDispWidth ;												// 디스플레이 가로 사이즈.
	int m_nDispHeight ;												// 디스플레이 세로 사이즈.
public:
	cNpcImageDlg(void);												// 생성자 함수.
	virtual ~cNpcImageDlg(void);									// 소멸자 함수.

	void Linking();													// 링크 함수.

	void LoadingImage(WORD indx) ;									// 이미지 로드 함수.

	void SetActive( BOOL val );										// 활성, 비활성화 함수.

	void SetNpc( WORD NpcUniqueIdx );								// NPC 설정 함수.

	void SetEmotion( BYTE emotion ) { mEmotion = emotion; }			// 모션 설정 함수.

	virtual void Render();											// 렌더 함수.

	void SetDisplayWH() ;											// 디스플레이 가로세로 사이즈를 세팅하는 함수.
};










//#pragma once
//#include ".\interface\cdialog.h"									// 다이얼로그 인터페이스 클래스 헤더 불러오기.
//#include "cImageSelf.h"												// 이미지 셀프 클래스 헤더 불러오기.
//
//#include "cStatic.h"												// 스태틱 클래스 헤더 불러오기.
//
//enum NPC_EMOTION													// NPC 모션 이넘 코드.
//{
//	EMOTION_NORMAL,													// 기본 모션.
//	EMOTION_HAPPY,													// 행복 모션.
//	EMOTION_SURPRISE,												// 놀람 모션.
//	EMOTION_UNHAPPY,												// 불행 모션.
//	EMOTION_ANGRY,													// 화남 모션.
//
//	EMOTION_MAX,													// 모션 최대수치.
//};
//
//struct NPC_IMAGE													// NPC 이미지 구조체.
//{
//	WORD		idx;												// 인덱스.
//	cImageSelf	image[5];											// 이미지 배열.
//};
//
//class cNpcImageDlg : public cDialog									// 기본 다이얼로그를 상속받은 NPC 이미지 다이얼로그 클래스.
//{
//	WORD	mNpcIdx;												// NPC 인덱스.
//	BYTE	mEmotion;												// 모션번호.
//	DWORD	COLOR;													// 색상 변수.
//
//	VECTOR2	SCALE;													// 스케일 벡터.
//	VECTOR2	POS;													// 위치 벡터.
//	VECTOR2 BACK_SCALE;												// 배경 스케일 벡터.
//	VECTOR2 BACK_POS;												// 배경 위치 벡터.
//
//	cImage	mImageTop;												// 탑 이미지.
//	cImage	mImageBack;												// 배경 이미지.
//	cImage	mImageBottom;											// 버텀 이미지.
//
//	NPC_IMAGE* mpCurImage;											// NPC 이미지 포인터.
//
//	// 070507 LYW --- NpcImageDlg : Delete image table.
//	//CYHHashTable< NPC_IMAGE > mImageTable;
//
//	// 070507 LYW --- NpcImageDlg : Delete notice part.
//	/*
//	// 070328 LYW --- NpcImageDlg : Add static control for notice.
//	cStatic*	m_pNotice ;
//
//	// 070328 LYW --- NpcImageDlg : Add member variable for notice.
//	BOOL		m_bActivedNotice ;
//
//	// 070329 LYW --- NpcImageDlg : Add member variables for alpha.
//	int			m_nAlphaValue ;
//	int			m_nContinueCount ;
//	*/
//
//	cStatic* m_pTopImage ;											// 탑이미지 스태틱.
//	cStatic* m_pBottomImage ;										// 버텀 이미지 스태틱.
//
//public:
//	cNpcImageDlg(void);												// 생성자 함수.
//	virtual ~cNpcImageDlg(void);									// 소멸자 함수.
//
//	// 070507 LYW --- NpcImageDlg : Delete function to linking image dlg.
//	void Linking();
//	// 070508 LYW --- NpcImageDlg : Add function to loading image.
//	void LoadingImage(WORD indx) ;
//	void SetActive( BOOL val );
//	void SetNpc( WORD NpcUniqueIdx );
//	void SetEmotion( BYTE emotion ) { mEmotion = emotion; }
//	virtual void Render();
//
//	// 070328 LYW --- NpcImageDlg : Add function to initialize notice.
//	void InitializeNotice( char* msgStr ) ;
//};
