#ifndef _BATTLE_SHOWDOWN_H
#define _BATTLE_SHOWDOWN_H

#include "battle.h"
#include "BattleTeam_Showdown.h"

#ifndef _MAPSERVER_
#include "../ImageNumber.h"
#endif

#define BATTLE_SHOWDOWN_READYTIME		6000	//���� Ŭ���̾�Ʈ�� �����⿣ 5��..
#define BATTLE_SHOWDOWN_FIGHTTIME		60000
#define BATTLE_SHOWDOWN_RESULTTIME	10000

enum eSHOWDOWN_TEAM
{
	SHOWDOWNTEAM_BLUE,
	SHOWDOWNTEAM_RED,
};


struct BATTLE_INFO_SHOWDOWN : public BATTLE_INFO_BASE
{
	DWORD		Character[eBattleTeam_Max];
	VECTOR3		vStgPos;
};

class CBattle_Showdown : public CBattle
{	
	BATTLE_INFO_SHOWDOWN m_ShowdownInfo;

	CBattleTeam_Showdown m_Team[2];
	
//	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);
#ifndef _MAPSERVER_
	CImageNumber	m_ImageNumber;

//	cImage			m_ImageReady;
	cImage			m_ImageStart;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;

	VECTOR2			m_vTitlePos;
	VECTOR2			m_vTitleScale;

////ȿ�� �ӽ�
	DWORD	m_dwFadeOutStartTime;
	BOOL	m_bFadeOut;


#else
	/////////////////////////////////////////////////////////////////////////////////
	// 06. 06. ���ÿ� ������ 2�� Player�� �� ���� ó�� �ȵǴ� ���� �ذ� - �̿���
	// ������ �ϳ��ۿ� ��� �ι�° �ɸ� ó�� �Ұ��Ͽ�
	// ������ Player���� ���� ó�� �ϵ��� ����
	BOOL			m_bDieByOp[2];	//���濡�� �׾���?(�������� ������ FALSE)

#endif
	
public:
	CBattle_Showdown();
	virtual ~CBattle_Showdown();

	void Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	// Battle ���� ����
#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
//	virtual void GetBattleInfo(BATTLE_INFO_BASE*& pInfo,int& size);
#endif

	// ��,�Ʊ� ����
	virtual BOOL IsEnemy(CObject* pOperator,CObject* pTarget);
	virtual BOOL IsFriend(CObject* pOperator,CObject* pTarget);
	
	// event func
	virtual void OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnFightStart();
	virtual void OnDestroy();
	virtual void OnTeamMemberAdd( int Team, DWORD MemberID, char* Name );
	virtual BOOL OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID);
	virtual BOOL OnTeamMemberDelete(int Team,DWORD MemberID,char* Name);

	virtual void OnTeamObjectCreate(int Team,CBattleObject* pBattleObject);
	virtual void OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject);


	virtual void OnTick();

	// ���� ����
	virtual BOOL Judge();
	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);
	virtual void Victory(int WinnerTeamNum,int LoserTeamNum);
	virtual void Draw();

	

	

#ifdef _CLIENT_
	// Render		(Ŭ���̾�Ʈ�� �����)
	virtual void Render();
#endif	
};


#endif
