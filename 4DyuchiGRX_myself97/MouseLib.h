

#ifndef	_MouseLib_h_
#define	_MouseLib_h_

#include "../4DyuchiGRX_common/IGeometry.h"

// ��ũ������ x,y��ǥ�� ���� ��ǥ�� x,0,z�� �ٲ۴�. y�� ������ 0
//	����������, ī�޶� from�� y��ǥ�� 0 �̻��϶� ���򼱺��� ���� ���� ������, �������� ���ʿ��� ������ ��ǥ�� ���´�.
VECTOR3		GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY);

#endif