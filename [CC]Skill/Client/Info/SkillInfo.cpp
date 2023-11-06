#include "stdafx.h"
#include ".\skillinfo.h"
#include "../Object/SkillObject.h"
#include "../Factory/SkillObjectFactory.h"

#include "ActionTarget.h"
#include "Hero.h"
#include "HeroPet.h"

cSkillInfo::cSkillInfo(void)
{
}

cSkillInfo::~cSkillInfo(void)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,cSkillObject*,pSObj)
		delete pSObj;
	PTRLISTSEARCHEND;
	m_SkillObjectPool.RemoveAll();
}


cSkillObject* cSkillInfo::GetSkillObject()
{
	cSkillObject* pSObj;
	if(m_SkillObjectPool.GetCount() == 0)
		pSObj = cSkillObjectFactory::CreateSkillObject(this);
	else
		pSObj = (cSkillObject*)m_SkillObjectPool.RemoveTail();
	return pSObj;
}

void cSkillInfo::ReleaseSkillObject(cSkillObject* pSObj)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,cSkillObject*,pPreObj)
		if(pSObj == pPreObj)
		{			
			return;
		}
	PTRLISTSEARCHEND

	m_SkillObjectPool.AddHead(pSObj);
}

WORD cSkillInfo::GetKind()
{
	return mpSkillInfo->Kind;
}

DWORD cSkillInfo::GetIndex()
{
	return mpSkillInfo->Index;
}

char* cSkillInfo::GetName()		
{	
	return mpSkillInfo->Name;	
}

DWORD cSkillInfo::GetTooltip()
{
	return mpSkillInfo->Tooltip;
}

int	cSkillInfo::GetImage()
{
	return mpSkillInfo->Image;
}

// 070613 LYW --- SkillInfo : Add function to return skill level.
WORD cSkillInfo::GetLevel()
{
	return mpSkillInfo->Level ;
}

// 080602 LUJ, 소모 아이템 정보를 인수로 추가
void cSkillInfo::SendMsgToServer(CHero* pOperator,CActionTarget* pTarget, const ITEMBASE* itemBase )
{
	MSG_SKILL_START_SYN msg;
	ZeroMemory( &msg, sizeof( msg ) );
	MAINTARGET MainTarget;
	
	pTarget->GetMainTarget( &MainTarget );
	
	msg.InitMsg( GetIndex(), &MainTarget, pOperator->GetAngleDeg(), pOperator->GetID() );
	
	pTarget->GetTargetArray(&msg.TargetList);

	// 080602 LUJ, 아이템 정보 복사
	if( itemBase )
	{
		msg.mConsumeItem = *itemBase;
	}

	NETWORK->Send(&msg,msg.GetMsgLength());
}

void cSkillInfo::SendPetMsgToServer(CHeroPet* pOperator,CActionTarget* pTarget, const ITEMBASE* itemBase )
{
	MSG_SKILL_START_SYN msg;
	ZeroMemory( &msg, sizeof( msg ) );
	MAINTARGET MainTarget;
	
	pTarget->GetMainTarget( &MainTarget );
	
	msg.InitMsg( GetIndex(), &MainTarget, pOperator->GetAngleDeg(), pOperator->GetID() );
	
	pTarget->GetTargetArray(&msg.TargetList);

	// 080602 LUJ, 아이템 정보 복사
	if( itemBase )
	{
		msg.mConsumeItem = *itemBase;
	}

	NETWORK->Send(&msg,msg.GetMsgLength());
}
