// QuestItemManager.cpp: implementation of the CQuestItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestItemManager.h"
#include "mhFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CQuestItemManager::CQuestItemManager()																// ������ �Լ�.
{
	m_QuestItemList.Initialize(200);																// ����Ʈ ������ ����Ʈ�� 200���� �ʱ�ȭ �Ѵ�.
}

CQuestItemManager::~CQuestItemManager()																// �Ҹ��� �Լ�.
{	
	Release();																						// ���� �Լ��� ȣ���Ѵ�.
}

void CQuestItemManager::Release()																	// ���� �Լ�.
{
	QUEST_ITEM_INFO* pData = NULL;																	// ����Ʈ ������ ������ ���� ������ ������ �����Ѵ�.

	m_QuestItemList.SetPositionHead();																// ����Ʈ ������ ����Ʈ�� ���� �����Ѵ�.

	while( pData = m_QuestItemList.GetData() )														// ����Ʈ ������ ������ �ִ� ���� while���� ������.
	{
		delete pData;																				// ������ ���� �Ѵ�.
		pData = NULL;																				// ������ ��Ÿ���� �����͸� NULL ó���� �Ѵ�.
	}

	m_QuestItemList.RemoveAll();																	// ����Ʈ ������ ����Ʈ�� ��� ����.
}

void CQuestItemManager::QuestItemInfoLoad()															// ����Ʈ ������ ������ �ε��ϴ� �Լ�.
{
	CMHFile file;																					// ���� ������ �����Ѵ�.

	if(!file.Init("./system/Resource/QuestItemList.bin", "rb"))										// ����Ʈ ������ ����Ʈ ������ �б� ���� ����.
	{
		return;																						// �б⿡ �����ϸ�,
	}

	int Key_1, Key_2;																				// Ű�� 1,2�� �����Ѵ�.
	int Key = 0;																					// Ű ���� ���� ������ �����ϰ� 0���� �����Ѵ�.
	QUEST_ITEM_INFO * pInfo = NULL;	

	int max = 0;																					// �ִ� ���� ���� ������ �����ϰ� 0���� �����Ѵ�.

	while(1)																						// while���� ������.
	{
		if(file.IsEOF())																			// ������ ���̶��,
			break;																					// break�� ����������.

		ASSERT(!pInfo);																				// ����Ʈ ������ ������ assert�� �Ǵ�.

		pInfo = new QUEST_ITEM_INFO;																// ����Ʈ ������ ������ �ش��ϴ� �޸𸮸� �Ҵ��Ѵ�.

		pInfo->ItemIdx = file.GetDword();															// ����Ʈ ������ �ε����� �о���δ�.

		SafeStrCpyEmptyChange(pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);			// ����Ʈ ��Ʈ���� �о�鿩 �����Ѵ�.

		Key_1 = file.GetDword();																	// Ű �� 1�� �о���δ�.
		Key_2 = file.GetDword();																	// Ű �� 2�� �о���δ�.

		COMBINEKEY(Key_1, Key_2, Key);																// Ű �� 1,2�� �����Ͽ� Ű ���� �޴´�.

		pInfo->QuestKey = Key;																		// ����Ʈ Ű���� �����Ѵ�.

		pInfo->Image2DNum = file.GetWord();															// 2D �̹��� ��ȣ�� �޴´�.
		pInfo->SellPrice = file.GetWord();															// �Ǹ� ������ �޴´�.

		SafeStrCpyEmptyChange(pInfo->ItemDesc, file.GetString(), MAX_ITEMDESC_LENGTH+1);			// ����Ʈ ��Ʈ���� �о�鿩 ������ desc�� �����Ѵ�.

		ASSERT(m_QuestItemList.GetData(pInfo->ItemIdx) == FALSE);									// ����Ʈ ������ ����Ʈ�� assert�� �Ǵ�.

		m_QuestItemList.Add(pInfo, pInfo->ItemIdx);													// ����Ʈ ������ ����Ʈ��, ������ ������ �߰��Ѵ�.

		max = pInfo->ItemIdx;																		// �ִ� ������ �ε����� �޴´�.

		pInfo = NULL;																				// ����Ʈ ������ NULL ó�� �Ѵ�.
	}

	file.Release();																					// ������ ���� �Ѵ�.
}


QUEST_ITEM_INFO* CQuestItemManager::GetQuestItem(DWORD ItemIdx)										// ����Ʈ �������� ���� �ϴ� �Լ�.
{
	return m_QuestItemList.GetData(ItemIdx);														// ����Ʈ ������ �����͸� �޴´�.
}
