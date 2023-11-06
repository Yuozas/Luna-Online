#pragma once
#include "interface\cdialog.h"

class cStatic;
class cComboBox;

class CPetStateDialog :
	public cDialog
{
	cStatic*	mName;
	cStatic*	mLevel;
	cStatic*	mType;
	cStatic*	mFriendly;
	cStatic*	mGrade;

	cStatic*	mStr;
	cStatic*	mDex;
	cStatic*	mVit;
	cStatic*	mInt;
	cStatic*	mWis;

	cStatic*	mPhysicAttack;
	cStatic*	mMagicAttack;
	cStatic*	mPhysicDefense;
	cStatic*	mMagicDefense;
	cStatic*	mAccuracy;
	cStatic*	mAvoid;

	cComboBox*	mAI;

public:
	CPetStateDialog(void);
	virtual ~CPetStateDialog(void);

	void Linking();
	
	virtual void OnActionEvent(LONG id, void* p, DWORD event);
	virtual void SetActive( BOOL val );

	void SetInfo();
};
