// EffectTriggerGravityMoveUnitDesc.h: interface for the CEffectTriggerGravityMoveUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERGRAVITYMOVEUNITDESC_H__0273B169_F24F_4B38_8326_EDAFFFDD79BC__INCLUDED_)
#define AFX_EFFECTTRIGGERGRAVITYMOVEUNITDESC_H__0273B169_F24F_4B38_8326_EDAFFFDD79BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

struct EFFECTPOSITIONDESC
{
	VECTOR3 m_Pos;
	BYTE m_PositionBasis;	//0:Ÿ��, 1:�ڽ�, 2:����Ÿ��, 3:������ġ
	BYTE m_RotateBasis;		//0:Ÿ��, 1:�ڽ�, 2:����Ÿ��, 3:������ġ
	void Set(const VECTOR3& vpos,BYTE PosBasis,BYTE RotateBasis)
	{
		m_Pos = vpos;
		m_PositionBasis = PosBasis;
		m_RotateBasis = RotateBasis;
	}
	BOOL GetEffectPos(CObject* pTarget,CObject* pOperator,MAINTARGET* pMainTarget,VECTOR3* pRtPos);
};

struct GRAVITYMOVEINFO
{
	VECTOR3 m_InitPos;
	VECTOR3 m_Velocity;
	float m_RemainTime;
	EFFECTPOSITIONDESC m_Target;
	CObject* m_pTargetObj;
};

class CEffectTriggerGravityMoveUnitDesc : public CEffectTriggerUnitDesc  
{
	EFFECTPOSITIONDESC m_PosFrom;
	EFFECTPOSITIONDESC m_PosTo;
	EFFECTPOSITIONDESC m_Velocity;
	

	DWORD m_Duration;
public:
	BOOL m_bTurn;
	
	CEffectTriggerGravityMoveUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerGravityMoveUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

	void CalcGravityMoveInfo(CObject* pTarget,CObject* pOperator,MAINTARGET* pMainTarget,GRAVITYMOVEINFO* pInfo);

};

#endif // !defined(AFX_EFFECTTRIGGERGRAVITYMOVEUNITDESC_H__0273B169_F24F_4B38_8326_EDAFFFDD79BC__INCLUDED_)
