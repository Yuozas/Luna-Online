#pragma once
#include "target.h"

class cMultiTarget :
	public cTarget
{
	CYHHashTable<CObject>	mTagetTable;

public:
	cMultiTarget(void);
	virtual ~cMultiTarget(void);

	virtual void Init( cSkillObject* pParentSkill, MAINTARGET* pMainTarget );
	virtual void Update();

	virtual	void		SetPositionHead();
	virtual CObject*	GetData();
	virtual CObject*	GetData( DWORD idx );
	virtual void		AddData( CObject* pObject );
	virtual void		Remove( DWORD idx );
	virtual void		RemoveAll();
};
