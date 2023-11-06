#pragma once
//=================================================================================================
//	FILE		: cNumberPadDialog.h
//	PURPOSE		: Dialog interface to create some numbers.
//	PROGRAMMER	: Yongs Lee
//	DATE		: January 9, 2008
//	ADDITION	:
//	ATTENTION	: ����, ��� ��ȣ�� �Է� ������, Ű���尡 �ƴ�, ���콺�� ����Ͽ� ó���ϱ� ����, 
//				  ���� �Է� �������̽��� �ʿ��ϴ�. ���� ���� ���� �Է��� �ʿ伺�� ���� �� �����Ƿ�,
//				  �ʿ��� ������ �ӽ� �ۼ����� �ƴ�, �ϳ��� �������̽��� �߰��Ͽ�, ����� �� �ֵ��� 
//				  �Ѵ�. ������ �� �ʿ��� ��ɸ� ����������, �߱� �ʿ��ѱ���� �߰��Ͽ� ����ϵ���.
//=================================================================================================





//=========================================================================
//		�ʿ��� ��������� �ҷ��´�.
//=========================================================================
#include "./Interface/cDialog.h"





//=========================================================================
//		�ʼ� ���� ��Ʈ.
//=========================================================================
#define MAX_CIPHER		4													// �ִ� �ڸ����� �����Ѵ�.





//=========================================================================
//		�ʿ��� ��Ʈ�� �������̽��� �����Ѵ�.
//=========================================================================
class cButton ;
class cStatic ;

enum NUMBER_BUTTONS
{
	e_Btn_0 = 0,
	e_Btn_1,
	e_Btn_2,
	e_Btn_3,
	e_Btn_4,
	e_Btn_5,
	e_Btn_6,
	e_Btn_7,
	e_Btn_8,
	e_Btn_9,
	e_Btn_BackSpace,

	e_Btn_Max,
} ;





//=========================================================================
//		���� �е� ���̾�α� Ŭ���� ����
//=========================================================================
class cNumberPadDialog : public cDialog
{
	int		m_nNumber[MAX_CIPHER] ;											// �Է� ����.
	int		m_nCipherCount ;												// �ڸ��� ī��Ʈ. 0 = 1���� ����, 1 = 10���� ����, ....

	char	m_NumberStr[MAX_CIPHER+1] ;										// ���ڸ� ���ڿ��� ���� ����.

	cStatic*	m_pNumberBox ;												// ���� �ڽ� ��Ʈ��.

	cButton*	m_pNumberBtn[e_Btn_Max] ;									// ���� ��ư�� ��Ʈ��.

public:
	cNumberPadDialog(void) ;												// ������ �Լ�.
	virtual ~cNumberPadDialog(void) ;										// �Ҹ��� �Լ�.

	void Initialize() ;														// �ʱ�ȭ �Լ�.

	void Linking() ;														// ���� ��Ʈ�� ��ũ �Լ�.

	virtual void OnActionEvent( LONG id, void* p, DWORD event ) ;			// �׼� �̺�Ʈ ó�� �Լ�.

	void	IncreaseUnit(int nNumber) ;										// ���� ������ �����ϴ� �Լ�.
	void	DecreaseUnit() ;												// ���� ������ �����ϴ� �Լ�.

	void	PrintNumberToNumberBox() ;										// ���� �ڽ��� ���ڸ� ����ϴ� �Լ�.

	int		GetNumber(int nCipher) ;										// ���ڸ� �����ϴ� �Լ�.
	char*	GetNumberStr() { return m_NumberStr ; }							// ���� ���ڿ��� �����ϴ� �Լ�.

	int		GetMaxCipherCount() { return MAX_CIPHER ; }						// �ִ� �ڸ����� �����ϴ� �Լ�.
	int		GetCurCipherCount() { return m_nCipherCount ; }					// ���� �ڸ����� �����ϴ� �Լ�.

	virtual void SetActive( BOOL val ) ;									// Ȱ��ȭ ���θ� �����ϴ� �Լ�.
};
