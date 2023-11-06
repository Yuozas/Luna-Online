#pragma once
#include "object.h"

/// ��ų�� ����
enum SKILL_STATE
{
	SKILL_STATE_CASTING,
	SKILL_STATE_PROCESS,
	SKILL_STATE_ON,
	SKILL_STATE_END,
	SKILL_STATE_DESTROY,
};

/// ������ ����
enum DELAY_KIND
{
	DELAY_KIND_CASTING,
	DELAY_KIND_PROCESS,
	DELAY_KIND_COOLING,
	DELAY_KIND_MAX,
};

class CBattle;
class CObject;
class cTarget;
class cSkillUnit;
class cSkillInfo;
class cDelay;

/// ��ų ���� ����
struct sSKILL_CREATE_INFO
{
	/// ��ų ������Ʈ ID
	DWORD		skillObjectId;
	/// ��ų ����
	WORD		level;

	/// ������/���
	DWORD		operatorId;
//	CObject*	pOperator;
	MAINTARGET	mainTarget;
	
	/// ��ų ��ġ/����
	VECTOR3		pos;
	DIRINDEX	skillDir;

	/// ��Ʋ����
	//DWORD		battle;
	//CBattle*	pBattle;

	DWORD		remainTime;

	WORD		count;

	/// �ʱ�ȭ �Լ�
	sSKILL_CREATE_INFO()
	{
		skillObjectId = 0;
		level = 0;

//		pOperator = NULL;

		pos.x = 0;
		pos.y = 0;
		pos.z = 0;

		skillDir = 0;
		//pBattle = NULL;
		remainTime = 0;
		count = 0;
	}
};

class cSkillObject :
	public CObject
{
	// 080602 LUJ, ��ų ������Ʈ Ÿ��
public:	
	enum Type
	{
		TypeNone,
		TypeActive,
		TypeBuff,
		TypeOnOff,
	};

	inline Type GetType() const { return mType; }

protected:
	Type mType;


protected:
	/// ��ų ����
	cSkillInfo*			mpSkillInfo;

	/// ��ų ���� ����
	sSKILL_CREATE_INFO	mSkillObjectInfo;

	/// ��ų ����
	SKILL_STATE			mState;

	/// Ÿ��
	cTarget*			mpTarget;
	

public:
	/// ������ / �Ҹ���
	cSkillObject( cSkillInfo* pSkillInfo, cTarget* pTarget );
	virtual ~cSkillObject( void );

	/// ������
	DWORD				GetSkillIdx();
	WORD				GetSkillKind();
	WORD				GetSkillLevel();
	CObject*			GetOperator();
	cSkillInfo*			GetSkillInfo();
	void				GetPosition( VECTOR3* pPos );
	DIRINDEX			GetDirectionIndex();
	

	/// �ʱ�ȭ/����
	virtual void		Init( sSKILL_CREATE_INFO* pInfo );
	virtual void		Release() = 0;

	/// ���μ���
	virtual SKILL_STATE	Update();
	virtual void EndState();

	/// Ÿ�� ������Ʈ
	virtual void		UpdateTargetList( MAINTARGET* pTarget );
	
	/// Grid �ý��ۿ� �߰�/����
	virtual void		SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	virtual void		SetRemoveMsg(char* pRemoveMsg,WORD* pMsgLen,DWORD dwReceiverID);

	void				SetMultiTarget( CTargetList* pTList , WORD AreaData);

	SKILL_STATE			GetSkillState() { return mState; }
	virtual	void		SetEndState();

};
