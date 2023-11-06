#pragma once
//=================================================================================================
//	FILE		: cChangeNameDialog.h
//	PURPOSE		: Dialog interface to change name of hero.
//	PROGRAMMER	: Yongs Lee
//	DATE		: December 26, 2007
//	ADDITION	:
//	ATTENTION	:
//=================================================================================================





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "./Interface/cDialog.h"





//=========================================================================
//		�ʼ� ���� ��Ʈ.
//=========================================================================
#define CN_MIN_NAME_LENGTH		4
#define CN_MAX_NAME_LENGTH		17





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class cButton ;
class cEditBox ;






//=========================================================================
//		�̸� ���� ���̾�α� Ŭ���� ����
//=========================================================================
class cChangeNameDialog : public cDialog
{
	cEditBox* m_pNameBox ;

	cButton* m_pChangeBtn ;
	cButton* m_pCancelBtn ;
public:
	cChangeNameDialog(void);
	virtual ~cChangeNameDialog(void);

	void Linking() ;

	virtual void OnActionEvent( LONG id, void* p, DWORD event ) ;

	//void Change_Name_Syn( char* pName ) ;

	void Clear_NameBox() ;
};
