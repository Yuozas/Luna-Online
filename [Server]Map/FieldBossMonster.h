#ifndef __FIELDBOSSMONSTER_H__
#define __FIELDBOSSMONSTER_H__

#pragma once
#include "monster.h"
#include "FieldBossMonsterManager.h"

/**
 * \ingroup MapServer
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-12-14
 *
 * \author Administrator
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */

class CFieldBossMonster :
	public CMonster
{
	// �Ҹ�/ȸ�� ���� �߰� ���°�
	FIELDMONSTERINFO m_Info;

public:
	CFieldBossMonster(void);
	virtual ~CFieldBossMonster(void);

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);

	// CMonster������ �޼��� ������ �����ش�. ȸ�����μ����� ���� ������ �ʿ�
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);

	virtual void DoDie(CObject* pAttacker);

	void SetInfo(FIELDMONSTERINFO* Info)
	{
		memcpy(&m_Info, Info, sizeof(FIELDMONSTERINFO));
	}

	BOOL IsBattle();	// ������Ȳ üũ
	BOOL IsDistruct();	// �Ҹ�ð� üũ - �Ҹ�ð��� ī��Ʈ�ؼ� �Ҹ�Ǿ� �ϸ� TRUE�� �����Ѵ�

//	virtual void AddStatus(CStatus* pStatus);
	void Recover();		// ȸ�� ���μ���
};

#endif