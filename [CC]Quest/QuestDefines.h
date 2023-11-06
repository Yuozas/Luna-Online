#ifndef __QUESTDEFINES_H__
#define __QUESTDEFINES_H__

#pragma pack(push,1)

#ifdef _MAPSERVER_																	// �ʼ����� ������ �Ǿ�������,
#define	PLAYERTYPE	CPlayer															// PLAYERTYPE�� CPlayer Ŭ������ �����ϰ�,
class CPlayer;																		// �÷��̾� Ŭ������ �����Ѵ�.
#else																				// �ʼ����� ����Ǿ� ���� ������,
#define PLAYERTYPE	CHero															// PLAYERTYPE�� CHero Ŭ������ �����ϰ�,
class CHero;																		// ����� Ŭ������ �����Ѵ�.
#endif																				// �б� ó���� �����Ѵ�.


#define MAX_QUEST_PROBABILITY		10000											// �ִ� ����Ʈ ����.

#define	MAX_QUEST					200												// �ִ� ����Ʈ ��.
#define	MAX_QUESTEVENT_MGR			1000											// �ִ� ����Ʈ �̺�Ʈ �Ŵ���?
#define MAX_QUESTEVENT_PLYER		100												// �ִ� ����Ʈ �̺�Ʈ �÷��̾��.
#define	MAX_SUBQUEST				32												// �ִ� ���� ����Ʈ ��.

#define	MAX_ITEMDESC_LENGTH			255												// �ִ� ������ ���� ����.
#define	MAX_ITEMDESC_LINE			20												// �ִ� ������ ���� ����.
#define MAX_PROCESSQUEST			100												// �ִ� ����Ʈ ���μ��� ��.
#define MAX_COMPLETEQUEST			100												// �ִ� �Ϸ� ����Ʈ ��.

#define VIEW_QUESTITEM_PERPAGE		12												// �� ������ �� �������� �ִ� ����Ʈ ������ ��.

#define	QUEST_GIVEUP_ID				99												// ����Ʈ ���� ���̵�.
#define MAX_QUESTITEM				100												// �ִ� ����Ʈ ������ ��.


#define TITLE_COLOR					RGB(210, 0, 255)								// ���� ����.
#define	MAINQUEST_TITLE_COLOR		RGB(100, 50, 0)									// ���� ����Ʈ ���� ����.
#define	MAINQUEST_TITLE_COLOR_SEL	RGB(0, 64, 64)									// ���� ����Ʈ ���� ����.
#define SUBQUEST_TITLE_COLOR		RGB(255, 0, 0)									// ���� ����Ʈ ���� ����.
#define SUBQUEST_TITLE_COLOR_SEL	RGB(255, 100, 0)								// ���� ����Ʈ ���� ����.
#define QUEST_GIVEUP_COLOR			RGB(0, 64, 0)									// ����Ʈ ���� ����.
#define QUEST_GIVEUP_COLOR_SEL		RGB(0, 150, 0)									// ����Ʈ ���� ����.
#define QUEST_DESC_COLOR			RGB(0, 0, 64)									// ����Ʈ ���� ����.
#define QUEST_DESC_HIGHLIGHT		RGB(128, 0, 0)									// ����Ʈ ���� ���̶���Ʈ ����.
#define	SELECT_COLOR				RGB(128, 0, 128)								// ���� �� ����.
#define	SELECT_COLOR_SEL			RGB(160, 128, 160)								// ���� �� ����.

// Ű ���� �����ϴ� ��ũ�� ����.
#define COMBINEKEY(a, b, c)				\
	if(b<100)			c=a*100+b;		\
	else if(b<1000)		c=a*1000+b;		\
	else if(b<10000)	c=a*10000+b;	


enum eQuestEvent																	// ����Ʈ �̺�Ʈ�� �����ϴ� �̳� �ڵ�.
{
	eQuestEvent_NpcTalk = 1,														// npc ��ȭ �̺�Ʈ.
	eQuestEvent_Hunt,																// ��� �̺�Ʈ.
	eQuestEvent_EndSub,																// ���� ����Ʈ ���� �̺�Ʈ.
	eQuestEvent_Count,																// ī��Ʈ �̺�Ʈ.
	eQuestEvent_GameEnter,															// ���� ���� �̺�Ʈ.
	eQuestEvent_Level,																// ���� �̺�Ʈ.
	eQuestEvent_UseItem,															// ������ ��� �̺�Ʈ.
	eQuestEvent_MapChange,															// �� ���� �̺�Ʈ.
	eQuestEvent_Die,																// ���� �̺�Ʈ.
	eQuestEvent_Time,																// �ð� �̺�Ʈ.
	eQuestEvent_HuntAll,															// ��� ��� �̺�Ʈ.
};

enum eQuestExecute																	// ����Ʈ ������ �����ϱ� ���� �̳� �ڵ�.
{
	eQuestExecute_EndQuest,															// ����Ʈ ����.
	eQuestExecute_StartQuest,														// ����Ʈ ����.

	eQuestExecute_EndSub,															// ���� ����Ʈ ����.
	eQuestExecute_EndOtherSub,														// �ٸ� ���� ����Ʈ ����.
	eQuestExecute_StartSub,															// ���� ����Ʈ ����.
	
	eQuestExecute_AddCount,															// ī��Ʈ �߰�.
	eQuestExecute_MinusCount,														// ī��Ʈ ����.

	eQuestExecute_GiveQuestItem,													// ����Ʈ �������� �ִ°�. quest item â�� ���� ��
	eQuestExecute_TakeQuestItem,													// ����Ʈ �������� ȹ���ϴ� ��.
	
	eQuestExecute_GiveItem,															// ����Ʈ �������� �ִ� ��.
	eQuestExecute_GiveMoney,														// ����Ʈ �Ӵϸ� �ִ� ��.
	eQuestExecute_TakeItem,															// ����Ʈ �������� ȹ���ϴ� ��.
	eQuestExecute_TakeMoney,														// ����Ʈ �Ӵϸ� ȹ���ϴ� ��.
	eQuestExecute_TakeExp,															// ����Ʈ ����ġ�� ȹ���ϴ� ��.
	eQuestExecute_TakeSExp,															// ���� sp ����Ʈ�� ȹ���ϴ� ��.

	eQuestExecute_RandomTakeItem,													// �����ϰ� �������� ȹ���ϴ� ��.

	eQuestExecute_TakeQuestItemFQW,													// from Ư�� �����
	eQuestExecute_AddCountFQW,														// from Ư�� ����� ī��Ʈ�� �߰��ϴ� ��.
	eQuestExecute_TakeQuestItemFW,													// from ������� ����Ʈ �������� ȹ���ϴ� ��.
	eQuestExecute_AddCountFW,														// ����Ʈ ������ ī��Ʈ�� �߰��ϴ� ��.
	
	eQuestExecute_TakeMoneyPerCount,												// ī��Ʈ ��, ����Ʈ �Ӵϸ� ȹ���ϴ� ��.
	
	eQuestExecute_RegenMonster,														// ���͸� �����ϴ� ��.
	eQuestExecute_MapChange,														// ���� ��ü�ϴ� ��.

	eQuestExecute_ChangeStage,														// ���������� �����ϴ� ��.
	
	eQuestExecute_ChangeSubAttr,													// ���� Attr�� �����ϴ� ��.

	eQuestExecute_RegistTime,														// �ð��� ����ϴ� ��.

	eQuestExecute_LevelGap,															// ���� ���̸� �����ϴ� ��.
	eQuestExecute_MonLevel,															// ���� ������ �����ϴ� ��
};

enum eQuestLimitKind																// ����Ʈ ���� ������ ���� �̳� �ڵ�.
{
	eQuestLimitKind_Level,															// ���� ����.
	eQuestLimitKind_Money,															// �Ӵ� ����.
	eQuestLimitKind_Quest,															// ����Ʈ ����.
	eQuestLimitKind_SubQuest,	
	// 070415 LYW --- QuestDefine : Active quest limit kine enum stage.
	eQuestLimitKind_Stage,															// ���� ���� ����.
	eQuestLimitKind_Attr,															// ����Ʈ �Ӽ� ���Ѵ�.
	// 071011 LYW --- QuestDefine : Add quest limit to check running quest.			// ���� ���� ����Ʈ�� �����Ѵ�.
	eQuestLimitKind_RunningQuest,
};

enum eQuestValue																	// ����Ʈ ���� ���� �̳� �ڵ�.
{
	eQuestValue_Add,																// ����Ʈ �߰�.
	eQuestValue_Minus,																// ����Ʈ ����.
	eQuestValue_Reset,
};

enum																				// ����Ʈ ���⿡ ���� �̳� �ڵ�.
{
	eQView_QList,																	// ����Ʈ ����Ʈ ����.
	eQView_QDesc,																	// ����Ʈ ���� ���� ����.
};

enum																				// ����Ʈ Ʈ���� ���� �̳� �ڵ�.
{
	eQTree_NotUse,																	// Ʈ�� ��� ���ϱ�, 
	eQTree_Close,																	// �ݱ�.
	eQTree_Open,																	// ����.
};

enum																				// ����Ʈ ������ ī��Ʈ ���� �̳� �ڵ�.
{
	eQItem_AddCount,																// ����Ʈ �߰� ī��Ʈ.
	eQItem_SetCount,																// ����Ʈ ���� ī��Ʈ.
	eQItem_GetCount,																// ����Ʈ ��ȯ ī��Ʈ.
};

struct SUBQUEST																		// ���� ����Ʈ ����ü.
{
	DWORD	dwMaxCount;																// �ִ� ī��Ʈ.
	DWORD	dwData;																	// ������ ��.
	DWORD	dwTime;																	// �ð� ��.
	SUBQUEST() : dwMaxCount(0), dwData(0), dwTime(0)	{}							// �⺻ �Լ�.
};

struct QUESTITEM																	// ����Ʈ ������ ����ü.
{
	DWORD	dwQuestIdx;																// ����Ʈ �ε���.
	DWORD	dwItemIdx;																// ������ �ε���.
	DWORD	dwItemNum;																// ������ ����.
	QUESTITEM() : dwQuestIdx(0), dwItemIdx(0), dwItemNum(0)	{}						// �⺻ �Լ�.
};

struct QUEST_ITEM_INFO																// ����Ʈ ������ ���� ����ü.
{
	DWORD		ItemIdx;															// ������ �ε���.
	DWORD		QuestKey;															// ����Ʈ Ű ��.
	char		ItemName[MAX_ITEMNAME_LENGTH+1];									// ������ ��.
	WORD		Image2DNum;															// 2d �̹���.
	WORD		SellPrice;															// �Ǹ� ����.
	char		ItemDesc[MAX_ITEMDESC_LENGTH+1];									// ������ ����.
};

#pragma pack(pop)
#endif
