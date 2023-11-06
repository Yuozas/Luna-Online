/**********************************************************************

이름: ProgressDialog
작성: 2007/09/18 17:35:59, 이웅주

목적:	조합/분해/강화/인챈트 때 프로그레스바를 표시한다.
		각각의 작업은 요청 내역을 저장해두었다가, 설정된 시간 이후에
		자동으로 요청된다.
                                                                     
***********************************************************************/
#pragma once


#include "interface/cDialog.h"

// 071213 KTH -- 딜레이 2초
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
		eActionEnchant,		// 인챈트
		eActionMix,			// 조합
		eActionReinforce,	// 강화
		eActionDissolve,	// 분해
		eActionChange,		// 변환
		eActionApplyOption,	// 옵션 적용
		eActionPetSummon,
		eActionPetGradeUp,
		eActionPetResurrection,

	};

	// 작업을 대기했다가 일정시간 경과 후 요청한다.
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
