//
// ���� work�����忡�� ������ �޽����� �Ѱ��� �޽��� ó�� ������� ������ ���� ����Ÿ �����̴�.
// ��� �޽����� MSG_CONTAINER�� ���� ������ �޽��� Ÿ���� �ο��ȴ�.
// �� Ŭ������ �޽��� Ư������ push�� dispatch�� �� �ִ�.��Ƽ �����忡�� ����ȭ�� �ʿ��ϱ� ������ 
// lock(),Unlock()�Լ��� �����ϸ� �� Ŭ������ �＼�� �ϱ� ���ؼ��� �ݵ�� lock()�� �� �ʿ䰡 �ִ�.
//
//#define _WIN32_WINNT 0x0500

#ifndef QUE_H
#define QUE_H

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/net_define.h"


struct HEADER
{
	DWORD				m_dwHeader;

	void				SetHeader(DWORD dwHandle) {m_dwHeader = dwHandle;}	// ��� ����.
	void				SetActionType(ACTION_TYPE acType) {m_dwHeader &= INVERSE_ACTION_TYPE_BIT_MASK; m_dwHeader |= acType;}//�׼� Ÿ�� ����
	void				SetActorType(ACTOR_TYPE actType) { m_dwHeader &= INVERSE_ACTOR_TYPE_BIT_MASK;m_dwHeader |= actType;}// �׼� Ÿ�Ը���
	ACTION_TYPE			GetActionType() { return (ACTION_TYPE)(m_dwHeader & ACTION_TYPE_BIT_MASK);}// �׼� Ÿ�Ը���
	DWORD				GetConnectionIndex() { return m_dwHeader & CONNECTION_INDEX_BIT_MASK;}// ��Ʈ��ũ Ŭ���������� �ε��� ����
	ACTOR_TYPE			GetActorType() {return (ACTOR_TYPE)(m_dwHeader & ACTOR_TYPE_BIT_MASK);}// ���������� ���������� ����
};
	

// recv�� ��Ŷ�� ���� disconnect��Ȳ�̳� ���� ������� worker�����尣�� ��� �� ���� �̺�Ʈ��
// �ϰ��ǰ� ó���ϱ� ���� ����Ÿ �����̴�.
// disconnect,recv udp,recv tcp,switch queue���� ��� �� �޽��� �����̳ʷ� ĸ��ȭ�ȴ�.
#define	SIZE_MSG_CONTAINER	16
struct MSG_CONTAINER
{
	HEADER						m_dwHeader;
	DWORD						m_dwSize;				// �޽��� �����̳� ������
	DWORD						m_dwMsgLength;		// �޽��� ����
	DWORD						m_dwReserved;			// ���������� ������ ���ܵ� 4����Ʈ.���⼱ udp���� ����Ѵ�.
	char						m_pMsg[1];			// �޽����� ����Ű�� ���� �ε����� �迭.
	
	DWORD						GetMsgLength()			{return m_dwMsgLength;}	// �� �����̳ʿ� ����ִ� �޽��� ���� ����
	char*						GetMsg()				{return m_pMsg;}	// ����ִ� �޽��� ������ ����
	DWORD						GetContainerSize()		{return m_dwSize;}	// �� �����̳��� ������ ����
	DWORD						GetRawContainerSize()	{return SIZE_MSG_CONTAINER;}
	DWORD						GetHeader()				{return m_dwHeader.m_dwHeader;}

	void						SetActionType(ACTION_TYPE acType) {m_dwHeader.m_dwHeader &= INVERSE_ACTION_TYPE_BIT_MASK; m_dwHeader.m_dwHeader |= acType;}//�׼� Ÿ�� ����
	ACTION_TYPE					GetActionType() { return (ACTION_TYPE)(m_dwHeader.m_dwHeader & ACTION_TYPE_BIT_MASK);}// �׼� Ÿ�Ը���
	DWORD						GetConnectionIndex() { return m_dwHeader.m_dwHeader & CONNECTION_INDEX_BIT_MASK;}// ��Ʈ��ũ Ŭ���������� �ε��� ����
	ACTOR_TYPE					GetActorType() {return (ACTOR_TYPE)(m_dwHeader.m_dwHeader & ACTOR_TYPE_BIT_MASK);}// ���������� ���������� ����
	
	DWORD						GetFromAddr() {return m_dwReserved;}	// �޽����� �۽��� �ּ�.(UDP�� ��츸 �ش�)
	void						SetAddress(DWORD addr) {m_dwReserved = addr;}	// �޽��� �۽��� �ּ� ����(UDP��)
	void						Clear()	
	{
		// ��� ���� �ʱ�ȭ.
		m_dwHeader.m_dwHeader = 0;
		m_dwReserved = 0;
		m_dwMsgLength = 0;
		m_dwSize = SIZE_MSG_CONTAINER;
	}
	MSG_CONTAINER()		
	{
		Clear();
	}
};
// �޽����� ó���ϱ� ���� �޽��� ť.
// ���⿡ PUSH�� �޽��� �����̳ʵ��� ���� �����尡 �ѹ� ó���Ѵ�.
class CMsgQue
{
	DWORD					m_dwMsgNum;				// �׿��ִ� �޽��� ����
	char*					m_pBuffer;
	DWORD					m_dwOffset;				// ���� �޽����� ���� �������� ��ġ
	DWORD					m_dwBufferPosition;		// ���� ���� ����.
	DWORD					m_dwMaxBufferSize;		// �ִ� ���� ������
	BOOL					m_bWaitFlag;			// �޽��� ť�� �� á������ ó��.TRUE�̸� wait. false�̸� �޽��� ���
	
	
	
public:

	void					SetWait()	{m_bWaitFlag = TRUE;}
	void					ReleaseWait() {m_bWaitFlag = FALSE;}

	DWORD	inline			GetMsgNum() {return m_dwMsgNum;}
	DWORD	inline			GetBufferPosition() {return m_dwBufferPosition;}
	DWORD	inline			GetMaxBufferSize() {return m_dwMaxBufferSize;}
	BOOL					PushMsg(DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum);


	BOOL					Add(CMsgQue* pSrc);
	BOOL					Move(CMsgQue* pDest);
	
	friend BOOL				Add(CMsgQue* pSrc);
	friend BOOL				Move(CMsgQue* pDest);
	void	inline			Clear() {m_dwMsgNum = 0; m_dwBufferPosition = 0; m_dwOffset = 0;}
		

	CMsgQue(DWORD dwMaxBufferSize);
	~CMsgQue();

};
MSG_CONTAINER* GetMsgContainer(CMsgQue* pMsgQue);
extern CRITICAL_SECTION g_csWrite;

BOOL PushMsg(CMsgQue* pMsgQue,DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum);

#endif
