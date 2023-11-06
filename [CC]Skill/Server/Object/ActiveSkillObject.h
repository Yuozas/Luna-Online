#pragma once
#include "skillobject.h"

class cBuffSkillObject;
class cDelay;
class cBuffSkillInfo;

class cActiveSkillObject :
	public cSkillObject
{
protected:
	/// 버프 스킬 테이블
	CYHHashTable<cBuffSkillObject>	m_BuffSkillTable;

	/// 딜레이
	cDelay*				mpDelay[ DELAY_KIND_MAX ];

	/// 스킬 효과 객체
	cSkillUnit*			mpSkillUnit;

	sSKILL_CREATE_INFO	mBuffSkillObjectInfo[MAX_BUFF_COUNT];
	cBuffSkillInfo*		mpBuffSkillInfo[MAX_BUFF_COUNT];
	WORD				mBuffRate[MAX_BUFF_COUNT];

	float				mAttackSpeedRate;

public:
	/// 생성자 / 소멸자
	cActiveSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cActiveSkillObject( void );

	/// 초기화/해제
	virtual void	Init( sSKILL_CREATE_INFO* pInfo );
	virtual void	Release();

	/// 스킬 효과 객체 추가
	void			AddSkillUnit( cSkillUnit* pSkillUnit );

	/// 프로세스
	virtual SKILL_STATE	Update();
	virtual void	Excute();

	void CastingStart();
	void CastingState();
	void ProcessStart();
	void ProcessState();

	/// 버프스킬 관리
	virtual void AddBuffSkillObject( cBuffSkillObject* pBuff );
	virtual void RemoveBuffSkillObject ( cBuffSkillObject* pBuff );

	// 080602 LUJ, 해당 오브젝트로 발동된 버프 스킬을 모두 제거한다
	virtual void RemoveAllBuffSkillObject();

	/// Grid 시스템에 추가/삭제
	virtual void		SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void		SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

};
