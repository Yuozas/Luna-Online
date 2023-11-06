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

CQuestUpdater::CQuestUpdater()															// ������ �Լ�.
{

}

CQuestUpdater::~CQuestUpdater()															// �Ҹ��� �Լ�.
{

}


void CQuestUpdater::JournalItemAdd(JOURNALINFO* pItem)									// ���� ������ �߰� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->JournalItemAdd(pItem);								// ����Ʈ ���̾�α׿� ���� �������� �߰��Ѵ�.
}

void CQuestUpdater::CompleteQuestDelete(CQuestString* pQuestString)						// �Ϸ� ����Ʈ ���� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->CompleteQuestDelete(pQuestString);					// ����Ʈ ���̾�α׿��� �Ϸ� ����Ʈ�� �����Ѵ�.
}

void CQuestUpdater::ProcessQuestAdd(CQuestString* pQuestString)							// ����Ʈ �߰� ���μ���.
{	
	GAMEIN->GetQuestTotalDialog()->ProcessQuestAdd(pQuestString);						// ����Ʈ ���̾�α׿� ����Ʈ�� �߰��Ѵ�.
}

void CQuestUpdater::ProcessQuestDelete(CQuestString* pQuestString)						// ����Ʈ ���� ���μ���.
{
	GAMEIN->GetQuestTotalDialog()->ProcessQuestDelete(pQuestString);					// ����Ʈ ���̾�α׿� ����Ʈ ������ �Ѵ�.
}

void  CQuestUpdater::QuestItemAdd(QUEST_ITEM_INFO* pQuestItemInfo, DWORD Count)			// ����Ʈ ������ �߰� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemAdd(pQuestItemInfo, Count);					// ����Ʈ ���̾�α׿� ����Ʈ �������� �߰��Ѵ�.
}

void CQuestUpdater::QuestItemDelete(DWORD ItemIdx)										// ����Ʈ ������ ���� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemDelete(ItemIdx);							// ����Ʈ ���̾�α׿��� ����Ʈ �������� �����Ѵ�.
}

void CQuestUpdater::QuestItemUpdate(DWORD type, DWORD ItemIdx, DWORD Count)				// ����Ʈ ������ ���׷��̵� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(type, ItemIdx, Count);				// ����Ʈ ���̾�α׿��� ����Ʈ �������� ������Ʈ �Ѵ�.
}

DWORD CQuestUpdater::GetQuestItemCount(DWORD ItemIdx)									// ����Ʈ ������ ī��Ʈ�� ��ȯ�ϴ� �Լ�.
{
	return GAMEIN->GetQuestTotalDialog()->QuestItemUpdate(eQItem_GetCount, ItemIdx, 0);	// ����Ʈ ���̾�α׿��� ����Ʈ �������� ������Ʈ �Ͽ� �����Ѵ�.
}

void CQuestUpdater::DeleteQuest(DWORD QuestIdx)											// ����Ʈ ���� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->GiveupQuestDelete(QuestIdx);							// ����Ʈ ���̾�α׿��� ���� ����Ʈ�� �����Ѵ�.
}

DWORD CQuestUpdater::GetSelectedQuestID()												// ������ ����Ʈ ���̵� ��ȯ�ϴ� �Լ�.
{
	return GAMEIN->GetQuestTotalDialog()->GetSelectedQuestID();							// ����Ʈ ���̾�α׿��� ������ ����Ʈ�� ���̵� ��ȯ�Ѵ�.
}

void CQuestUpdater::CloseMsgBox()														// Ŭ���� �޽��� �ڽ� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->CloseMsgBox();										// ����Ʈ ���̾�α��� �޽��� �ڽ��� �ݴ´�.
}

void CQuestUpdater::UpdateSubQuestData()												// ���� ����Ʈ �����͸� ������Ʈ �ϴ� �Լ�.
{
	GAMEIN->GetQuestTotalDialog()->UpdateSubQuestData();								// ����Ʈ ���̾�α׿��� ���� ����Ʈ �����͸� ������Ʈ �Ѵ�.
}