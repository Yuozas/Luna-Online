


#ifndef	_SampleHeightField_h_
#define	_SampleHeightField_h_

#include "../4DyuchiGRX_common/typedef.h"



//	IVERTEX �׷쿡�� VECTOR3�� �� ����Ʈ�ʵ带 ���ø� �ϴ� �Լ�.
//	pV�� x, z���� ����(?), pIV�� ���� ivertex, dwTriCount�� �ﰢ���� ����. dwXComponentCount, dwYComponentCount ���� ���ø��� x,y����.
//	���ϰ��� x*y���� ���ؽ���.
VECTOR3*	SampleHeightFieldFromIVertex( VECTOR3* pV, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwZ);




#endif