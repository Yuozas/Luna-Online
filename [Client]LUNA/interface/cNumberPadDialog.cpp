//=================================================================================================
//	FILE		: cNumberPadDialog.cpp
//	PURPOSE		: Implemention part of number pad interface.
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
#include "stdafx.h"
#include "../WindowIDEnum.h"
#include "../cMsgBox.h" 

#include "./Interface/cWindowManager.h"

#include "./Interface/cNumberPadDialog.h"
#include "./Interface/cStatic.h"








//=========================================================================
//	NAME : cNumberPadDialog
//	DESC : Construct function.
//=========================================================================
cNumberPadDialog::cNumberPadDialog(void)
{
	int count = 0 ;															// ���� for���� ���� ī��Ʈ ������ �����Ѵ�.

	for(count = 0 ; count < MAX_CIPHER ; ++count )
	{
		m_nNumber[count] = 0 ;												// �Է� ���ڸ� 0���� �ʱ�ȭ �Ѵ�.
	}

	memset(m_NumberStr, 0, sizeof(m_NumberStr)) ;							// ���� ���ڿ��� �ʱ�ȭ �Ѵ�.

	m_nCipherCount	= -1 ;													// �ڸ��� ī��Ʈ�� 0���� �ʱ�ȭ �Ѵ�.

	m_pNumberBox	= NULL ;												// ���� �ڽ� ��Ʈ���� �ʱ�ȭ �Ѵ�.

	for(count = 0 ; count < e_Btn_Max ; ++count )
	{
		m_pNumberBtn[count] = NULL ;										// ���� ��ư�� ��Ʈ���� �ʱ�ȭ �Ѵ�.
	}
}





//=========================================================================
//	NAME : ~cNumberPadDialog
//	DESC : Descruct function.
//=========================================================================
cNumberPadDialog::~cNumberPadDialog(void)
{
}





//=========================================================================
//	NAME : Initialize
//	DESC : The function to initialize all to default.
//=========================================================================
void cNumberPadDialog::Initialize() 	
{
	for(int count = 0 ; count < MAX_CIPHER ; ++count )
	{
		m_nNumber[count] = 0 ;												// �Է� ���ڸ� 0���� �ʱ�ȭ �Ѵ�.
	}

	m_nCipherCount	= -1 ;													// �ڸ��� ī��Ʈ�� 0���� �ʱ�ȭ �Ѵ�.

	m_pNumberBox->SetStaticText("") ;										// ���� �Է¶��� �ʱ�ȭ �Ѵ�.

	cStatic* pSecurityBox = NULL ;
	pSecurityBox = (cStatic*)WINDOWMGR->GetWindowForIDEx(MT_SECURITYPW) ;	// ���� ����ƽ�� �ҷ��´�.

	if( !pSecurityBox ) return ;											// ���� ����ƽ�� ��ȿ���� üũ�Ѵ�.

	pSecurityBox->SetStaticText("") ;										// ���� ����ƽ�� �ʱ�ȭ �Ѵ�.
	
	memset(m_NumberStr, 0, sizeof(m_NumberStr)) ;							// ���� ���ڿ��� �ʱ�ȭ �Ѵ�.
}





//=========================================================================
//	NAME : Linking
//	DESC : The function to link all child controls.
//=========================================================================
void cNumberPadDialog::Linking()
{
	m_pNumberBox = (cStatic*)GetWindowForID(NUMBERPAD_NUMBERBOX) ;				// ���� �ڽ� ��Ʈ���� ��ũ�Ѵ�.

	for(int count = 0 ; count < e_Btn_Max ; ++count )							// ���� ��ư ����ѵ��� ��ũ�Ѵ�.
	{
		m_pNumberBtn[count] = (cButton*)GetWindowForID(NUMBERPAD_0+count) ;
	}

	m_pNumberBox->SetActive(FALSE) ;											// ���� â�� ��Ȱ��ȭ ��Ų��.

	memset(m_NumberStr, 0, sizeof(m_NumberStr)) ;							// ���� ���ڿ��� �ʱ�ȭ �Ѵ�.
}





//=========================================================================
//	NAME : OnActionEvent
//	DESC : The function to management action event.
//=========================================================================
void cNumberPadDialog::OnActionEvent( LONG id, void* p, DWORD event ) 			
{
	if( id == NUMBERPAD_BACKSPACE )													// �� �����̽� ��ư�� ���ȴٸ�,
	{
		DecreaseUnit() ;															// ���ڸ� ���ҽ�Ű�� �Լ��� ȣ���Ѵ�.
		return ;																	// return ó���� �Ѵ�.
	}	

	int nNumber = 0 ;																// ���� ������ ��ȣ�� ���� ������ �����ϰ� �ʱ�ȭ �Ѵ�.

	switch( id )																	// ���ڷ� �Ѿ�� ���̵� Ȯ���Ѵ�.
	{
	case NUMBERPAD_0 :	nNumber = 0 ;	break ;										// ������ ��ư ��� ���ڸ� �����Ѵ�.
	case NUMBERPAD_1 :	nNumber = 1 ;	break ;
	case NUMBERPAD_2 :	nNumber = 2 ;	break ;
	case NUMBERPAD_3 :	nNumber = 3 ;	break ;
	case NUMBERPAD_4 :	nNumber = 4 ;	break ;
	case NUMBERPAD_5 :	nNumber = 5 ;	break ;
	case NUMBERPAD_6 :	nNumber = 6 ;	break ;
	case NUMBERPAD_7 :	nNumber = 7 ;	break ;
	case NUMBERPAD_8 :	nNumber = 8 ;	break ;
	case NUMBERPAD_9 :	nNumber = 9 ;	break ;
	}

	IncreaseUnit(nNumber) ;															// ���ڸ� ������Ű�� �Լ��� ȣ���Ѵ�.
}





//=========================================================================
//	NAME : IncreaseUnit
//	DESC : The function to increase unitcount and calculate number.
//=========================================================================
void cNumberPadDialog::IncreaseUnit(int nNumber) 
{
	++m_nCipherCount ;																// �ڸ����� �����Ѵ�.
	
	if( m_nCipherCount >= MAX_CIPHER ) 
	{
		m_nCipherCount = (MAX_CIPHER-1) ;			// ���� �� �ڸ����� �����Ѵ�.
		return ;
	}

	m_nNumber[m_nCipherCount] = nNumber ;											// �ش��ϴ� �ڸ����� ���ڸ� �����Ѵ�.

	PrintNumberToNumberBox() ;														// ���ڹڽ��� ���ڸ� ����Ѵ�.
}





//=========================================================================
//	NAME : DecreaseUnit
//	DESC : The function to decrease unitcount and calulate number.
//=========================================================================
void cNumberPadDialog::DecreaseUnit() 
{
	if( m_nCipherCount <= -1 ) return ;												// �ڸ����� Ȯ���Ѵ�.

	m_nNumber[m_nCipherCount] = 0 ;													// �ش��ϴ� �ڸ����� ���ڸ� �ʱ�ȭ �Ѵ�.
	--m_nCipherCount ;																// �ڸ����� �����Ѵ�.

	PrintNumberToNumberBox() ;														// ���ڹڽ��� ���ڸ� ����Ѵ�.

	if( m_nCipherCount <= -1 ) m_nCipherCount = -1 ;								// �ڸ����� �����Ѵ�.
}




//=========================================================================
//	NAME : PrintNumberToNumberBox
//	DESC : The function to print numbers to number box.
//=========================================================================
void cNumberPadDialog::PrintNumberToNumberBox() 
{
	//if( !m_pNumberBox )																// Ȥ�� �� ���� �ڽ� ��Ʈ���� ��ȿ���� üũ.
	//{
	//	WINDOWMGR->MsgBox(NULL, MBT_OK, "Invalid numberbox!!") ;
	//	return ;
	//}

	cStatic* pSecurityBox = NULL ;
	pSecurityBox = (cStatic*)WINDOWMGR->GetWindowForIDEx(MT_SECURITYPW) ;

	if( !pSecurityBox ) return ;

	memset(m_NumberStr, 0, sizeof(m_NumberStr)) ;									// ���� ���ڿ��� �ʱ�ȭ �Ѵ�.

	//m_pNumberBox->SetStaticText("") ;												// ���� �ڽ��� �ʱ�ȭ �Ѵ�.

	pSecurityBox->SetStaticText("") ;

	char securityStr[5] = {0,} ;

	switch(m_nCipherCount)															// �ڸ����� Ȯ���ϰ�, ���ڿ��� ���ڸ� ��Ÿ����.
	{
	case 0 :
		{
			sprintf(m_NumberStr, "%d", m_nNumber[0]) ;									
			sprintf(securityStr, "*", m_nNumber[0]) ;	
		}
		break ;

	case 1 :
		{
			sprintf(m_NumberStr, "%d%d", m_nNumber[0], m_nNumber[1]) ;
			sprintf(securityStr, "**", m_nNumber[0], m_nNumber[1]) ;
		}
		break ;

	case 2 :
		{
			sprintf(m_NumberStr, "%d%d%d", m_nNumber[0], m_nNumber[1], m_nNumber[2]) ;
			sprintf(securityStr, "***", m_nNumber[0], m_nNumber[1], m_nNumber[2]) ;
		}
		break ;

	case 3 :
		{
			sprintf(m_NumberStr, "%d%d%d%d", m_nNumber[0], m_nNumber[1], m_nNumber[2], m_nNumber[3]) ;
			sprintf(securityStr, "****", m_nNumber[0], m_nNumber[1], m_nNumber[2], m_nNumber[3]) ;
		}
		break ;

	default : return ;
	}

	//m_pNumberBox->SetStaticText(m_NumberStr) ;										// ���� ���ڷ� ���� �ڽ��� �����Ѵ�.

	pSecurityBox->SetStaticText(securityStr) ;
}




//=========================================================================
//	NAME : GetNumber
//	DESC : The function to return number of selected cipher.
//=========================================================================
int cNumberPadDialog::GetNumber(int nCipher)
{
	if( nCipher <= 0 || nCipher >= MAX_CIPHER ) return 0 ;							// �ڸ����� ��ȿ���� üũ�Ѵ�.

	if( m_nCipherCount <= 0 ) return 0 ;											// ��ư �Է��� ������ ���, 0 ���� ó���� �Ѵ�.

	return m_nNumber[m_nCipherCount] ;												// �ش��ϴ� ���ڸ� �����Ѵ�.
}





//=========================================================================
//	NAME : SetActive
//	DESC : The function to setting active or not this dialog.
//=========================================================================
void cNumberPadDialog::SetActive( BOOL val ) 
{
	if( val == TRUE )																// Ȱ��ȭ �� ����, 
	{
		Initialize() ;																// ��� ������ �ʱ�ȭ ���ش�.
	}

	cDialog::SetActive( val ) ;
}