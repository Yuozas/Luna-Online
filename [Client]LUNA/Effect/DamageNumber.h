// DamageNumber.h: interface for the CDamageNumber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_)
#define AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cImageSelf.h"

#define MAX_DAMAGE_JARISU	10

enum eDamageNumberKind
{
	eDNK_Yellow,
	eDNK_Green,
	eDNK_Red,

	eDNK_Max,
};

class CDamageNumber  
{

	struct NumberData
	{
		cImageSelf* pImage;
		VECTOR2 spos;
		void Draw(float fAlpha,VECTOR2* pos);
		void SetImage(cImageSelf* p,VECTOR2* pos);
	};

	static cImageSelf* m_pNumberImage[eDNK_Max];
//	static cImageSelf* m_pMissImage;
	static VECTOR2	   m_CriticalImgSize;
	static cImageSelf* m_pCriticalImage;
	static cImageSelf* m_pDodgeImage;

	DWORD m_CreatedTime;
	VECTOR3 m_Position;
	VECTOR3 m_PositionCritical;
	VECTOR3 m_Direction;
	float m_fVelocity;
	float m_fAlpha;
	
	BOOL m_bCritical;
	BOOL m_bDecisive;
	BOOL m_bDodge;
	
	BOOL m_bDraw;
	NumberData m_Numbers[MAX_DAMAGE_JARISU];		// �������� �ִ� 5�ڸ�����
	DWORD m_Damage;
	DWORD m_Jarisu;	
public:
	static BOOL LoadImage();		// ��ó�� �ʱ�ȭ
	static void DeleteImage();		// �ǳ��� ���ﶧ...
	
	CDamageNumber();
	virtual ~CDamageNumber();

	void SetDodge(VECTOR3* pPos,VECTOR3* pVelocity);
	void SetDamage(DWORD Damage,VECTOR3* pPos,VECTOR3* pVelocity,BYTE nDamageNumberKind,BOOL bCritical,BOOL bDecisive);
	BOOL Render();		// fAlpha = [0,1]
	void SetAlpha(float fAlpha)		{	m_fAlpha = fAlpha;	}
	void Clear()			{	m_bDraw = FALSE;	}

	// desc_hseos_���͹���01
	// S ���͹��� �߰� added by hseos 2007.05.29
	static cImageSelf*		GetImage(eDamageNumberKind eKind, int nNum)	{ return &m_pNumberImage[eKind][nNum]; }
	// E ���͹��� �߰� added by hseos 2007.05.29
};

#endif // !defined(AFX_DAMAGENUMBER_H__16CD2C8A_D6B3_4267_8435_F7B47E40EC64__INCLUDED_)
