// QuestUpdater.h: interface for the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
#define AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]Quest\QuestDefines.h"										// 퀘스트 정의 선언 해더파일을 불러온다.


class CPlayer;																// 플레이어 클래스를 선언한다.
class CQuestString;															// 퀘스트 스트링 클래스를 선언한다.

class CQuestUpdater															// 퀘스트 업데이터 클래스.
{
public:
	CQuestUpdater();														// 생성자 함수.
	virtual ~CQuestUpdater();												// 소멸자 함수.

	void	JournalItemAdd(JOURNALINFO* pItem);								// 저널 아이템을 추가하는 함수.

	void	CompleteQuestDelete(CQuestString* pQuestString);				// 완료 퀘스트를 삭제하는 함수.
	void	ProcessQuestAdd(CQuestString* pQuestString);					// 퀘스트 추가 프로세스.
	void	ProcessQuestDelete(CQuestString* pQuestString);					// 퀘스트 삭제 프로세스.

	void	QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo , DWORD Count);	// 퀘스트 아이템 추가 함수.
	void	QuestItemDelete(DWORD ItemIdx);									// 퀘스트 아이템 삭제 함수.
	void	QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count);		// 퀘스트 아이템 업데이트 함수.
	DWORD	GetQuestItemCount(DWORD ItemIdx);								// 퀘스트 아이템 수 반환 함수.

	//
	DWORD	GetSelectedQuestID();											// 선택 된 퀘스트 아이디 반환 함수.
	void	CloseMsgBox();													// 닫기 메시지 박스.
	
	void	DeleteQuest(DWORD QuestIdx);									// 퀘스트 삭제 함수.
	
	void	UpdateSubQuestData();											// 서브 퀘스트 업데이트 함수.
};

#endif // !defined(AFX_QUESTUPDATER_H__4AA13D4F_B271_43EE_B11B_163762DD8EE3__INCLUDED_)
