
#ifndef	TriangleLib_h_
#define	TriangleLib_h_

#include	"../4DyuchiGRX_common/typedef.h"

// ��� p�� �־��� �ﰢ���� �ڸ���, p �ڿ� ��ġ�Ѱ� ������.
// ���ϰ��� ����� �ﰢ���� ����. �ϳ��� �ﰢ���� �߶� �ִ� �ΰ��� �ﰢ���� ���� �� �����Ƿ�(�߷��� ��� �ΰ��� �Ϲ���)
// pOut�� ������ ���۵� �ּ��� �ΰ��� �ﰢ���� �� ������ �Ǿ��Ѵ�.
// �ﰢ���� �߷��� ��� 1, �ƴϸ� 2,
// �ﰢ���� ���߷������ 0 �ƴϸ� 1�� ���´�. (0�� ��� �ڿ� ������ ���.)
DWORD	ClipTriangle( VECTOR3* pOut, PLANE p, VECTOR3* pV0, VECTOR3* pV1, VECTOR3* pV2 );

#endif
