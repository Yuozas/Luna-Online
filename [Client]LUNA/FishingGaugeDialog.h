#pragma once
#include "./Interface/cDialog.h"

#define FISHING_GAUGE_HOLDTIME	200

class cGuageBar;
class CObjectGuagen;

class CFishingGaugeDialog :
	public cDialog
{
protected:
	cGuageBar*		m_pFishingGB;		// ���� ��������.
	CObjectGuagen*	m_pFishingTimeGauge;// ���� �ð�������
	DWORD			m_dwHoldTime;		// ������ Ȧ���ð�

public:
	CFishingGaugeDialog(void);
	virtual ~CFishingGaugeDialog(void);

	void Linking();
	virtual void SetActive(BOOL val);
	virtual void Render();

	cGuageBar*		GetFishingGB()			{return m_pFishingGB;}
	CObjectGuagen*	GetFishingTimeGauge()	{return m_pFishingTimeGauge;}

	void SetHoldTime();
	DWORD GetHoldTime()						{return m_dwHoldTime;}
};
