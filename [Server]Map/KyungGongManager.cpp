// KyungGongManager.cpp: implementation of the CKyungGongManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KyungGongManager.h"

#ifdef _MAPSERVER_
#include "UserTable.h"
#include "Player.h"
#include "..\[CC]Header\GameResourceStruct.h"
#else
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKyungGongManager::CKyungGongManager()
{
	m_InfoTable.Initialize(16);
}

CKyungGongManager::~CKyungGongManager()
{
	Release();
}

void CKyungGongManager::Init()
{	
	CMHFile file;
	if(file.Init("System/Resource/KyungGongInfo.bin","rb") == FALSE)
		return;
	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		CKyungGongInfo* pKGInfo = new CKyungGongInfo;
		pKGInfo->Init(&file);

		ASSERT(m_InfoTable.GetData(pKGInfo->GetKyungGongIdx()) == FALSE);
		m_InfoTable.Add(pKGInfo,pKGInfo->GetKyungGongIdx());
	}
	
	file.Release();
}
void CKyungGongManager::Release()
{
	CKyungGongInfo* pKGInfo;
	m_InfoTable.SetPositionHead();
	while(pKGInfo = m_InfoTable.GetData())
	{
		delete pKGInfo;
	}
	m_InfoTable.RemoveAll();
}

CKyungGongInfo* CKyungGongManager::GetKyungGongInfo(WORD idx)
{
	CKyungGongInfo* p = m_InfoTable.GetData(idx);
	ASSERT(p);
	return p;
}

BOOL CKyungGongManager::IsSetableKyungGong(PLAYERTYPE* pHero,ITEMTYPE* pItem)
{
#ifdef _MAPSERVER_
	const ITEM_INFO* pInfo = pItem;
#else
	const ITEM_INFO* pInfo = pItem->GetItemInfo();
#endif
	
	if(pInfo->LimitLevel > pHero->GetLevel())
		return FALSE;

	// �ٷ�.��ø,�ɸ�,ü��
/*	if(pInfo->LimitStr > pHero->GetStrength())
		return FALSE;
	if(pInfo->LimitDex > pHero->GetDexterity())
		return FALSE;
	if(pInfo->LimitWis > pHero->GetWisdom())
		return FALSE;
	if(pInfo->LimitVit > pHero->GetVitality())
		return FALSE;

	CKyungGongInfo* pKGInfo = GetKyungGongInfo(pInfo->SkillNum);
	if(pKGInfo == NULL)
		return FALSE;
	if(pKGInfo->GetKyungGongIdx() <= pHero->GetKyungGongGrade())		// ���� ������δ� ���ư��� ����.
		return FALSE;
*/	
	return TRUE;
}

void CKyungGongManager::SetKyungGong(PLAYERTYPE* pHero,WORD KyungGongIdx)
{
	ASSERT(GetKyungGongInfo(KyungGongIdx));
	pHero->SetKyungGongGrade(KyungGongIdx);

#ifdef _MAPSERVER_
	MSG_WORD msg;
	msg.Category = MP_KYUNGGONG;
	msg.Protocol = MP_KYUNGGONG_CHANGE_NOTIFY;
	msg.wData = KyungGongIdx;
	pHero->SendMsg(&msg,sizeof(msg));	
#endif
}

void CKyungGongManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
#ifdef _MAPSERVER_
#else
	switch(Protocol) {
	case MP_KYUNGGONG_CHANGE_NOTIFY:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			HERO->SetKyungGongGrade(pmsg->wData);
		}
		break;
	}
#endif
}