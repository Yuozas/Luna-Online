
#ifndef	_MyMouseLib_h_
#define	_MyMouseLib_h_

#include "../4dyuchigrx_common/typedef.h"


//	x, y, z������ ���� ȸ������������ ī�޶� ����.
void RotateCamera( CAMERA_DESC& cd, float x, float y, float z);
//	x, y, z�� �̵��������� ī�޶� ����.
CAMERA_DESC MoveCamera( const CAMERA_DESC& cd, float x, float y, float z);




#endif