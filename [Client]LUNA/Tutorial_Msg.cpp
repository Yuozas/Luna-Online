#include "stdafx.h"
#include ".\tutorial_msg.h"

cTutorial_Msg::cTutorial_Msg(void)
{
}

cTutorial_Msg::~cTutorial_Msg(void)
{
}





void cTutorial_Msg::Release()
{
	MSG_LINE* pMsgLine ;

	PTRLISTPOS pDeletePos = NULL ;
	pDeletePos = m_Msg_List.GetHeadPosition() ;

	while(pDeletePos)
	{
		pMsgLine = NULL ;
		pMsgLine = (MSG_LINE*)m_Msg_List.GetNext(pDeletePos) ;

		if( !pMsgLine ) continue ;

		delete pMsgLine ;
	}

	m_Msg_List.RemoveAll() ;
}





void cTutorial_Msg::Add_MsgLine(char* pMsg)
{
	if( !pMsg )
	{
		return ;
	}

	MSG_LINE* pMsgLine = NULL ;
	pMsgLine = new MSG_LINE ;

	if( !pMsgLine )
	{
		return ;
	}

	int nMsgCount = m_Msg_List.GetCount() ;

	pMsgLine->nLine = nMsgCount ;
	strcpy(pMsgLine->msg, pMsg) ;

	m_Msg_List.AddTail( pMsgLine ) ;
}





MSG_LINE* cTutorial_Msg::Get_MsgLine(int nIndex)
{
	int nMsg_Count = 0 ;													// �޽��� ���� ���� ���� ������ �����ϰ� 0���� �ʱ�ȭ �Ѵ�.
	nMsg_Count = m_Msg_List.GetCount() ;									// �޽��� ���� ������ �޴´�.

	if( nIndex < 0 || nIndex >= nMsg_Count ) return NULL ;					// ���ڷ� �Ѿ�� �ε�����, 0���� �۰ų�, ���� �޽��� ���� ������ ũ��, null return ó���� �Ѵ�.

	PTRLISTPOS returnPos = NULL ;											// �ε����� �ش��ϴ� �޽��� ���� ��ġ�� ���� �����͸� �����ϰ� null ó���� �Ѵ�.
	returnPos = m_Msg_List.FindIndex(nIndex) ;								// �ε����� �ش��ϴ� �޽��� ������ ��ġ������ �޴´�.

	if( !returnPos ) return NULL ;											// ��ġ ������ ��ȿ���� ������, null return ó���� �Ѵ�.

	return (MSG_LINE*)m_Msg_List.GetAt(returnPos) ;							// ��ġ�� �ش��ϴ�, �޽��� ���� ������ return ó���� �Ѵ�.
}
