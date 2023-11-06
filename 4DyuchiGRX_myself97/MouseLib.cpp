

#include "MouseLib.h"
#include <d3dx8.h>

// ��ũ������ x,y��ǥ�� ���� ��ǥ�� x,0,z�� �ٲ۴�. y�� ������ 0
VECTOR3		GetXYZFromScreenXY( I4DyuchiGXGeometry* pGeometry, DWORD dwX, DWORD dwY, DWORD dwScreenX, DWORD dwScreenY)
{

	VECTOR3	Result;

    D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;

    // Get the pick ray from the mouse position
        MATRIX4 matProj;
//        m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
		pGeometry->GetProjectionMatrix( &matProj);

//        POINT ptCursor;		�̰� ���°� ���ҷ���...
//        GetCursorPos( &ptCursor );
//	      ScreenToClient( m_hWnd, &ptCursor );

        // Compute the vector of the pick ray in screen space
        D3DXVECTOR3 v;
        v.x =  ( ( ( 2.0f * (float)dwX ) / (float)dwScreenX ) - 1 ) / matProj._11;
        v.y = -( ( ( 2.0f * (float)dwY ) / (float)dwScreenY ) - 1 ) / matProj._22;
        v.z =  1.0f;

        // Get the inverse view matrix
        MATRIX4 matView, m;
		pGeometry->GetViewMatrix( &matView);
        D3DXMatrixInverse( (D3DXMATRIX*)&m, NULL, (D3DXMATRIX*)&matView );

        // Transform the screen space pick ray into 3D space
        vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
        vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
        vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;

		VECTOR3	From, To;
		From.x	=	vPickRayOrig.x;
		From.y	=	vPickRayOrig.y;
		From.z	=	vPickRayOrig.z;
		To.x	=	From.x + vPickRayDir.x;
		To.y	=	From.y + vPickRayDir.y;
		To.z	=	From.z + vPickRayDir.z;


		// x,z ���� ������ ���� ����ΰ�.?
		{
			// �ϴ� ������ ����� �����ϸ� ����.
//			VECTOR3	PlaneUp;
//			PlaneUp.x = 0;
//			PlaneUp.y = 1;
//			PlaneUp.z = 0;
//			if( vPickRayDir.y * PlaneUp.y == 0)
			if( vPickRayDir.y == 0)
			{
				// ���� ���� ������ �����ϴ�.
				_asm int 3;
				Result.x = 1000000.0f;
				Result.y = 1000000.0f;
				Result.z = 1000000.0f;
			}
			
			float	t = (-1*From.y) / (To.y - From.y);		// ���� �����Ŀ��� y�� 0�϶� t��.
			float	x = From.x+t * (To.x-From.x);
			float	z = From.z+t * (To.z-From.z);
			Result.x	=	x;
			Result.y	=	0;
			Result.z	=	z;
		}

		return	Result;

}

