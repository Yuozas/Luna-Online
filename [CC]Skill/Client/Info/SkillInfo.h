#pragma once

#include "PtrList.h"
#include "MHFile.h"

class cSkillObject;
class CObject;
class CHero;
class CHeroPet;
class CActionTarget;

class cSkillInfo
{
protected:
	cPtrList		m_SkillObjectPool;
	SKILL_INFO*		mpSkillInfo;

public:
	cSkillInfo(void);
	virtual ~cSkillInfo(void);

	virtual BOOL	InitSkillInfo(CMHFile* pFile) = 0;

	cSkillObject*	GetSkillObject();
	void			ReleaseSkillObject(cSkillObject* pSObj);

	WORD		GetKind();
	DWORD		GetIndex();
	char*		GetName();
	DWORD		GetTooltip();
	int			GetImage();
	// 070613 LYW --- SkillInfo : Add function to return skill level.
	WORD		GetLevel() ;

	// 080602 LUJ, 소모 아이템 정보를 인수로 추가
	void SendMsgToServer(CHero* pOperator,CActionTarget* pTarget, const ITEMBASE* = 0 );

	void SendPetMsgToServer(CHeroPet* pOperator,CActionTarget* pTarget, const ITEMBASE* = 0 );
};
