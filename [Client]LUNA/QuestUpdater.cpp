// QuestUpdater.cpp: implementation of the CQuestUpdater class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestUpdater.h"
#include "GameIn.h"
#include "QuestTotalDialog.h"
#include "Player.h"
#include "Quest.h"
#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\SubQuestInfo.h"
#include "..\[CC]Quest\QuestString.h"
#include "QuestManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestUpdater::CQuestUpdater()															// 생성자 함수.
{

}

CQuestUpdater::~CQuestUpdater()															// 소멸자 함수.
{

}


void CQuestUpdater::JournalItemAdd(JOURNALINFO* pItem)									// 저널 아이템 추가 함수.
{
	GAMEIN->GetQuestTotalDialog()->JournalItemAdd(pItem);								// 퀘스트 다이얼로그에 저널 아이템을 추가한다.
}

void CQuestUpdater::CompleteQuestDelete(CQuestString* pQuestString)						// 완료 퀘스트 삭제 함수.
{
	GAMEIN->GetQuestTotalDialog()->CompleteQuestDelete(pQuestString);					// 퀘스트 다이얼로그에서 완료 퀘스트를 삭제한다.
}

void CQuestUpdater::ProcessQuestAdd(CQuestString* pQuestString)							// 퀘스트 추가 프로세스.
{	
	GAMEIN->GetQuestTotalDialog()->ProcessQuestAdd(pQuestString);						// 퀘스트 다이얼로그에 퀘스트를 추가한다.
}

void CQuestUpdater::ProcessQuestDelete(CQuestString* pQuestString)						// 퀘스트 삭제 프로세스.
{
	GAMEIN->GetQuestTotalDialog()->ProcessQuestDelete(pQuestString);					// 퀘스트 다이얼로그에 퀘스트 삭제를 한다.
}

void  CQuestUpdater::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)			// 퀘스트 아이템 추가 함수.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemAdd(pQuestItemInfo, Count);					// 퀘스트 다이얼로그에 퀘스트 아이템을 추가한다.
}

void CQuestUpdater::QuestItemDelete(DWORD ItemIdx)										// 퀘스트 아이템 삭제 함수.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemDelete(ItemIdx);							// 퀘스트 다이얼로그에서 퀘스트 아이템을 삭제한다.
}

void CQuestUpdater::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count)				// 퀘스트 아이템 업그레이드 함수.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(type, ItemIdx, Count);				// 퀘스트 다이얼로그에서 퀘스트 아이템을 업데이트 한다.
}

DWORD CQuestUpdater::GetQuestItemCount(DWORD ItemIdx)									// 퀘스트 아이템 카운트를 반환하는 함수.
{
	return GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(eQItem_GetCount, ItemIdx, 0);	// 퀘스트 다이얼로그에서 퀘스트 아이템을 업데이트 하여 리턴한다.
}

void CQuestUpdater::DeleteQuest(DWORD QuestIdx)											// 퀘스트 삭제 함수.
{
	GAMEIN->GetQuestTotalDialog()->GiveupQuestDelete(QuestIdx);							// 퀘스트 다이얼로그에서 포기 퀘스트를 삭제한다.
}

DWORD CQuestUpdater::GetSelectedQuestID()												// 선택한 퀘스트 아이디를 반환하는 함수.
{
	return GAMEIN->GetQuestTotalDialog()->GetSelectedQuestID();							// 퀘스트 다이얼로그에서 선택한 퀘스트의 아이디를 반환한다.
}

void CQuestUpdater::CloseMsgBox()														// 클로즈 메시지 박스 함수.
{
	GAMEIN->GetQuestTotalDialog()->CloseMsgBox();										// 퀘스트 다이얼로그의 메시지 박스를 닫는다.
}

void CQuestUpdater::UpdateSubQuestData()												// 서브 퀘스트 데이터를 업데이트 하는 함수.
{
	GAMEIN->GetQuestTotalDialog()->UpdateSubQuestData();								// 퀘스트 다이얼로그에서 서브 퀘스트 데이터를 업데이트 한다.
}