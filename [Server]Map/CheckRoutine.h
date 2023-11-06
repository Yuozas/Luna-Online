// CheckRoutine.h: interface for the CCheckRoutine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_)
#define AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPlayer;
enum eCheckBits{ CB_EXIST = 1, CB_ICONIDX=2, CB_QABSPOS=4, CB_DURA=8, CB_ALL=15, CB_ENOUGHDURA=16, };
#define CHKRT CCheckRoutine::GetInstance()
class CCheckRoutine  
{
public:
	CCheckRoutine();
	virtual ~CCheckRoutine();
	GETINSTANCE(CCheckRoutine)

	BOOL StateOf(CPlayer * pPlayer, DWORD state);
	BOOL MoneyOf(CPlayer * pPlayer, eITEMTABLE tabIdx, MONEYTYPE moneyAmount);
	BOOL LevelOf(CPlayer * pPlayer, LEVELTYPE level);
	BOOL DistanceOf(CPlayer * pPlayer, VECTOR3 *vTPos, DWORD dist);

	// ������ ��ɼ��� �����Ƽ ���� ����
	/*�����Ƽ ���� - BOOL AbilityOf(CPlayer * pPlayer, eABILITY_USE_JOB eKind);*/
	// ������ ���ϼ� üũ(��ġ,����,����������)
	BOOL ItemOf(CPlayer * pPlayer, POSTYPE AtPos, DWORD wItemIdx, DURTYPE Dura, POSTYPE QuickPos, WORD flag = CB_ALL);
	
	BOOL DearlerItemOf(WORD wDealerIdx, DWORD wBuyItemIdx);
	// �̹� ���� ������ ���� ��Ͽ���
	// ��ϵǾ� ������ TRUE
	// �ƴϸ� FALSE
	BOOL SkillOf(CPlayer * pPlayer, DWORD wSkillIdx);

	//BOOL ItemStageOf(CPlayer* pPlayer, BYTE bItemStage );
	
};

#endif // !defined(AFX_CHECKROUTINE_H__1F0CA9EB_BC00_41F9_B11E_E8E021026AB2__INCLUDED_)
