#pragma once

#define GAMENOTIFYMGR USINGTON(CGameNotifyManager)

#define MAX_LINE_LIMIT	5																// ���� ��� ���� �ִ� ���� ��.

class CGameNotifyDlg ;																	// ���� ���� ���̾�α� Ŭ������ �����Ѵ�.
class CItem ;

enum GAMENOTIFY_STYLE																	// ���� ���� ��Ÿ�� �̳� �ڵ�.
{
	eSTYLE_DEFAULT = 0 ,																// �⺻ ��Ÿ��.
	eSTYLE_QUEST,																		// ����Ʈ ������.
} ;

enum QUEST_COMPOSIT																		// ����Ʈ �������� �޽��� �߰��� ����ϴ� ���� Ÿ�� �̳� �ڵ�.
{
	eQC_DEFAULT_STR = 0,																// ����Ʈ �⺻ ��Ʈ��.
    eQC_MAINQUEST_NAME,																	// ���� ����Ʈ �̸�.
	eQC_SUBQUEST_NAME,																	// ���� ����Ʈ �̸�.
	eQC_COUNT,																			// ī��Ʈ.
	eQC_COMPLETE,																		// �Ϸ�.
	eQC_EXPERIENCE,																		// ����ġ.
	eQC_MONEY,																			// �Ӵ�.
	eQC_ITEM,																			// ������.

	eQC_MAX,																			// ����Ʈ ���� ���� �ִ� ��.
} ;

struct sGAMENOTIFY_MSG																	// ���� �޽��� ����ü.
{
	int nNotifyType ;																	// ���� Ÿ��.

	//int nMinAlpha ;																		// �ּ� ���� ��.

	/*int red ;

	int gree ;

	int blue ;*/

	//int nAlpha ;

	RECT rect ;

	int nDelayTime ;																	// ������ �ð�.

	cPtrList itemList ;																	// ��Ʈ�� ������ ����Ʈ.

	sGAMENOTIFY_MSG()																	// ������ �Լ�.
	{
		nNotifyType = eSTYLE_DEFAULT ;													// ���� Ÿ���� �⺻ Ÿ������ �����Ѵ�.

		//nMinAlpha = 255 ;																	// �ּ� ���� ���� 0���� �����Ѵ�.

		/*red = 255 ;

		gree = 255 ;

		blue = 255 ;*/

		//nAlpha = 255 ;

		memset(&rect, 0, sizeof(RECT)) ;

		nDelayTime = 170 ;																// ������ Ÿ���� 3�ʷ� �����Ѵ�.

		itemList.RemoveAll() ;															// ������ ����Ʈ�� ��� ����.
	}
} ;

const DWORD dwQuestStringColor[eQC_MAX] =												// ����Ʈ ��Ʈ�� ������ ���� �迭.
{
	//RGBA_MAKE( 0, 255, 255, 255 ),														// ����Ʈ �⺻ ��Ʈ�� ����.
	//RGBA_MAKE(255, 255, 0, 255),														// ���� ����Ʈ �̸� ����.
	//RGBA_MAKE(0, 255, 0, 255),															// ���� ����Ʈ �̸� ����.
	//RGBA_MAKE(255, 255, 255, 255),														// ī��Ʈ ����.
	//RGBA_MAKE(255, 255, 255, 255),														// �Ϸ� ����.
	//RGBA_MAKE(255, 10, 10, 255),														// ����ġ ����.
	//RGBA_MAKE(255, 255, 10, 255),															// �Ӵ� ����.
	//RGBA_MAKE(10, 10, 255, 255),															// ������ ����.

	RGB( 0, 255, 255 ),														// ����Ʈ �⺻ ��Ʈ�� ����.
	RGB(255, 255, 0),														// ���� ����Ʈ �̸� ����.
	RGB(0, 255, 0),															// ���� ����Ʈ �̸� ����.
	RGB(255, 255, 255),														// ī��Ʈ ����.
	RGB(255, 255, 255),														// �Ϸ� ����.
	RGB(255, 10, 10),														// ����ġ ����.
	RGB(255, 255, 10),															// �Ӵ� ����.
	RGB(10, 10, 255),															// ������ ����.
} ;

class CGameNotifyManager																// ���� ���� �Ŵ��� Ŭ����.
{
	CGameNotifyDlg*		m_pNotifyDlg ;													// ���� ���� ��¿� ���̾�α� ������.

	cPtrList			m_MsgList ;

public:
	CGameNotifyManager(void);															// ������ �Լ�.
	virtual ~CGameNotifyManager(void);													// �Ҹ��� �Լ�.

	void Init() ;																		// �ʱ�ȭ �Լ�.

	void SetNotifyDlg(CGameNotifyDlg* pDlg) { m_pNotifyDlg = pDlg ; }

	void AddMsg(sGAMENOTIFY_MSG* pGameNotifyMsg) ;										// ���� ���� �޽��� �߰� �Լ�.

	void Render() ;

	// QUEST PART //
	//void StartQuest(char* pQuestTitle) ;												// ����Ʈ ���� ������ �ϴ� �Լ�.
	//void EndQuest(char* pQuestTitle) ;													// ����Ʈ ���� ������ �ϴ� �Լ�.
	void UpdateSubQuest(SEND_SUBQUEST_UPDATE* msg) ;									// ����Ʈ ������Ʈ ������ �����ϴ� �Լ�.
	void EndSubQuest(SEND_QUEST_IDX* msg) ;												// ���� ����Ʈ ����� ������ �����ϴ� �Լ�.
	//void TakeItem(CItem* pItem) ;														// ����Ʈ �������� ȹ�� �� ������ ����ϴ� �Լ�.
	//void TakeMoney(char* pMoney) ;														// ����Ʈ ������� �����ϴ� �Լ�.
	//void TakeExp(DWORD dwExp) ;															// ����Ʈ ���� ����ġ�� �����ϴ� �Լ�.
};

EXTERNGLOBALTON(CGameNotifyManager)