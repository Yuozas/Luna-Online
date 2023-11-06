#pragma once
//=================================================================================================
//	FILE		: Tutorial.h
//	PURPOSE		: The class that stored all info of tutorial.
//	PROGRAMMER	: Yongs Lee
//	DATE		: October 16, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "stdafx.h"															// ǥ�� ��������� �����Ѵ�.




//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
enum START_CONDITION														// Ʃ�丮�� �ߵ� ���� enum�� �����Ѵ�.
{
	e_FIRST_GAMEIN = 0,														// ó�� ���ӿ� ���� ��.
	e_COMPLETE_TUTORIAL,													// ������ Ʃ�丮���� �Ϸ� ���� ��.
	e_LEVEL,																// ���� ������ �������� ��.
	e_GET_ITEM,																// Ư�� �������� ȹ������ ��.
	e_TALK_NPC,																// Ư�� npc�� ��ȭ�� �������� ��.
	e_NOT_RUNNING,															// ��Ȳ�� ���� �ߵ��� ���� �ʴ� Ʃ�丮��.

	e_MAX_SCONDITION,														// �ִ� �ߵ� ���� ��.
} ;

enum COMPLETE_CONDITION														// Ʃ�丮�� �Ϸ� ���� enum�� �����Ѵ�.
{
	e_CLICK_OKBTN = 0,														// OK ��ư�� Ŭ�� �ؾ� �� ��.
	e_NPC_TALK,																// Ư�� npc�� ��ȭ�� �ؾ� �� ��.
	e_OPEN_DIALOG,															// Ư�� ���̾�α׸� ����� �Ҷ�.
	e_CLICK_SPECIFIED_BTN,													// ������ ��ư�� Ŭ���ؾ� �� ��,
	e_RECEIVE_QUEST,														// ������ ����Ʈ�� �����ؾ� �� ��,

	e_MAX_CCONDITION,														// �ִ� �Ϸ� ���� ��.
} ;

enum ITEM_CONDITION
{
	e_GET_FIRST_ITEM = 0,
	e_GET_STACK_ITEM,
} ;

struct SUB_TUTORIAL															// ���� Ʃ�丮�� ����ü�� ���� �Ѵ�.
{
	int nIndex ;															// ���� Ʃ�丮�� �ε���.
	int nSpeech_Idx ;														// Ʃ�丮�� ��� �ε���.
	int nSound_Idx ;														// ���� �ε���.
	int nNpc_Idx ;															// NPC �ε���.
	int nFace_Idx ;															// �� ǥ�� �ε���.

	int nComplete_Condition1 ;												// �Ϸ����� 1.
	int nComplete_Condition2 ;												// �Ϸ����� 2.

	char szCompleteCondition2[24] ;											// �Ϸ� ������ �ؽ�Ʈ�� ��� ���� ����.

	SUB_TUTORIAL()															// ���� Ʃ�丮�� ����ü�� �ʱ�ȭ �Ѵ�.
	{
		nIndex		= 0 ;													// ���� Ʃ�丮�� �ε����� 0���� �ʱ�ȭ �Ѵ�.
		nSpeech_Idx = 0 ;													// ��� �ε����� 0���� �ʱ�ȭ �Ѵ�.
		nSound_Idx  = 0 ;													// ���� �ε����� 0���� �ʱ�ȭ �Ѵ�.
		nNpc_Idx	= 0 ;													// npc �ε����� 0���� �ʱ�ȭ �Ѵ�.
		nFace_Idx	= 0 ;													// npc ��ǥ�� �ε����� 0���� �ʱ�ȭ �Ѵ�.

		nComplete_Condition1 = 0 ;											// ù��° �Ϸ� ������ 0���� �ʱ�ȭ �Ѵ�.
		nComplete_Condition2 = 0 ;											// �ι�° �Ϸ� ������ 0���� �ʱ�ȭ �Ѵ�.

		memset(szCompleteCondition2, 0, sizeof(szCompleteCondition2)) ;		// �ι�° �Ϸ� ���� ��Ʈ���� �ʱ�ȭ �Ѵ�.
	} ;
} ;





//=========================================================================
//		Ʃ�丮�� Ŭ���� ����
//=========================================================================
class cTutorial																// Ʃ�丮�� ������ ���� Ŭ������ �����Ѵ�.
{
	int m_nIndex ;															// Ʃ�丮�� �ε���.

	int m_nStartCondition1 ;												// �ߵ� ����1.
	int m_nStartCondition2 ;												// �ߵ� ����2.

	BOOL	m_bCompleted ;													// ���� Ʃ�丮���� �Ϸ� ���θ� üũ�ϴ� ����.

	cPtrList			m_SubTutorial_List ;								// ���� Ʃ�丮�� ������ ���� ����Ʈ.
public:
	cTutorial(void);														// ������ �Լ�.
	virtual ~cTutorial(void);												// �Ҹ��� �Լ�.

	void Init() ;															// �ʱ�ȭ �Լ�.

	void Release() ;														// ���� �Լ�.

	void Set_Index(int nIndex) { m_nIndex = nIndex ; }						// Ʃ�丮�� �ε����� �����ϴ� �Լ�.
	int  Get_Index() { return m_nIndex ; }									// Ʃ�丮�� �ε����� ��ȯ�ϴ� �Լ�.

	void Set_StartCondition1(int nStartCondition1)							// �� 1 �ߵ� ������ �����ϴ� �Լ�.
	{
		m_nStartCondition1 = nStartCondition1 ; 							// �� 1 �ߵ� ������ �����Ѵ�.
	}
	int  Get_StartCondition1() { return m_nStartCondition1 ; }				// �� 1 �ߵ� ������ ��ȯ�ϴ� �Լ�.

	void Set_StartCondition2(int nStartCondition2)							// �� 2 �ߵ� ������ �����ϴ� �Լ�.
	{
		m_nStartCondition2 = nStartCondition2 ; 							// �� 2 �ߵ� ������ �����Ѵ�.
	}
	int  Get_StartCondition2() { return m_nStartCondition2 ; }				// �� 2 �ߵ� ������ ��ȯ�ϴ� �Լ�.

	int	 Get_SubTutorial_Count() { return m_SubTutorial_List.GetCount() ; }	// ���� Ʃ�丮�� ����Ʈ�� ī��Ʈ�� �����ϴ� �Լ�.

	void Add_SubTutorial(SUB_TUTORIAL* pSubTutorial) ;						// ���� Ʃ�丮���� �߰��ϴ� �Լ�.
	SUB_TUTORIAL* Get_SubTutorial(int nIndex) ;								// �ε����� �ش��ϴ� ���� Ʃ�丮�� ������ �����ϴ� �Լ�.

	void Set_Complete(BOOL bVal) { m_bCompleted = bVal ; }					// Ʃ�丮���� �Ϸ� ���θ� �����ϴ� �Լ�.
	BOOL Is_Completed() { return m_bCompleted ; }							// Ʃ�丮���� �Ϸ� ���θ� �����ϴ� �Լ�.
};
