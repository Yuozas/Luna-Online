/*
*/
#include "../4DyuchiGRX_common/typedef.h"

// �����̴� ���Ǿ �ﰢ���� ���� ������.?
BOOL	WhereDidMovingSphereMeetTri(  VECTOR3* OUT pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fRadius, VECTOR3* pTri);
// ������ ��ġ, �������� t, �������� �����̵� �÷���, ���� ����, ���� to, �� ������, �ﰢ��.
// ������ ��ġ�� ���߽�, t, ������ ��ġ(��鱸�ϱ�), ���Ǿ�����, ���Ǿ���, ������, �ﰢ��)

// �����̴� ���Ǿ ���Ǿ��� ���� ������.?
BOOL	WhereDidMovingSphereMeetSphere( VECTOR3* pCandidate, float* pT, VECTOR3* pWhereMeet, VECTOR3* pSFrom, VECTOR3* pSTo, float fSRadius, VECTOR3* pTSPivot, float fTSRadius);
// ������ ��ġ, ������ t, ������ ����, �����̴� ���Ǿ�from, to, ������, ������ ���Ǿ�pivot, ������.

// Ǫ��. Ǫ��.
void	SpherePushDir( VECTOR3* OUT pCandidate, VECTOR3* IN pFrom, VECTOR3* IN pTo, VECTOR3* IN pMeet, VECTOR3* IN pMeetPos, PLANE* IN pSlidingPlane, BOUNDING_VOLUME* pBV);
