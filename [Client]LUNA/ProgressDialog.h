/**********************************************************************

�̸�: ProgressDialog
�ۼ�: 2007/09/18 17:35:59, �̿���

����:	����/����/��ȭ/��æƮ �� ���α׷����ٸ� ǥ���Ѵ�.
		������ �۾��� ��û ������ �����صξ��ٰ�, ������ �ð� ���Ŀ�
		�ڵ����� ��û�ȴ�.
                                                                     
***********************************************************************/
#pragma once


#include "interface/cDialog.h"

// 071213 KTH -- ������ 2��
#define WAITMILISECOND	2000

class cStatic;
class CObjectGuagen;


class CProgressDialog : public cDialog
{
public:
	CProgressDialog();
	virtual ~CProgressDialog();

	virtual void Render();
	virtual void SetActive( BOOL );
	virtual void OnActionEvent( LONG id, void* p, DWORD event );

	void Linking();
	void SetText( const char* );

	enum eAction
	{
		eActionNone,
		eActionEnchant,		// ��æƮ
		eActionMix,			// ����
		eActionReinforce,	// ��ȭ
		eActionDissolve,	// ����
		eActionChange,		// ��ȯ
		eActionApplyOption,	// �ɼ� ����
		eActionPetSummon,
		eActionPetGradeUp,
		eActionPetResurrection,

	};

	// �۾��� ����ߴٰ� �����ð� ��� �� ��û�Ѵ�.
	void Wait( eAction );

	void Cancel();
	//void Succeed();
	//void Fail();

	eAction	GetActionType()
	{
		return	mAction.mType;
	}

private:
	void Restore();

	cStatic*		mText;
	CObjectGuagen*	mGauge;
	//cWindow*		mButton;

	struct
	{
		DWORD	mBeginTick;
		DWORD	mEndTick;
		eAction	mType;
		BOOL	mIsWaiting;

		//char*	mMessageBuffer;
		//SIZE_T	mMessageSize;
	}
	mAction;
};
