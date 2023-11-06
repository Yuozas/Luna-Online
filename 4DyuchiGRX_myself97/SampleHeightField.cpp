#include "SampleHeightField.h"
#include "myself97common.h"



VECTOR3*	SampleHeightFieldFromIVertex_old3( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwY)
{
	DWORD	x = dwX;
	DWORD	z = dwY;	// ��ǥ�谣�� ����...
	// �浹ó����.
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2;



	DWORD	i = 0, j = 0, p = 0;		// �ӽú���.

	// �ٿ�� �ڽ��� �����.
	MAABB	box;
	memset( &box, 0, sizeof(box));
	for( i = 0; i < dwTriCount*3; i++)
	{
		// max
		if( pIV[i].x > box.Max.x)	box.Max.x = pIV[i].x;
		if( pIV[i].y > box.Max.y)	box.Max.y = pIV[i].y;
		if( pIV[i].z > box.Max.z)	box.Max.z = pIV[i].z;
		// min
		if( pIV[i].x < box.Min.x)	box.Min.x = pIV[i].x;
		if( pIV[i].y < box.Min.y)	box.Min.y = pIV[i].y;
		if( pIV[i].z < box.Min.z)	box.Min.z = pIV[i].z;
	}
	box.Max.y = 900000.0f;
	box.Min.y = -900000.0f;

	// x, z�� ����.
//	double	fXStep	=	(box.Max.x - box.Min.x) / (x-1);
//	double	fZStep	=	(box.Max.z - box.Min.z) / (z-1);


	VECTOR3*	pV = new VECTOR3[x*z];		// ����� ����.
//	memset( pV, 0, sizeof(VECTOR3)*x*z);
	memcpy( pV, pVInput, sizeof(VECTOR3)*x*z);

/*
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
					pV[i+j*x].x = box.Min.x + (float)fXStep*i;
					pV[i+j*x].y = box.Min.y;
					pV[i+j*x].z = box.Min.z + (float)fZStep*j;
		}
	}
	

	// �׳� ����ΰ�.?
	if( box.Max.y == box.Min.y)
	{
		for( j = 0; j < z; j++)
		{
			for( i = 0; i < x; i++)
			{
				pV[i+j*x].x = box.Min.x + (float)fXStep*i;
				pV[i+j*x].z = box.Min.z + (float)fZStep*j;
				pV[i+j*x].y = box.Min.y;
			}
		}
		goto Finish;
	}

	for( p = 0; p < dwTriCount; p++)
	{
		// �ﰢ�� �ϳ��� �����ͼ�,
		tv1.x = pIV[p*3].x;
		tv1.y = pIV[p*3].y;
		tv1.z = pIV[p*3].z;
		tv2.x = pIV[p*3+1].x;
		tv2.y = pIV[p*3+1].y;
		tv2.z = pIV[p*3+1].z;
		tv3.x = pIV[p*3+2].x;
		tv3.y = pIV[p*3+2].y;
		tv3.z = pIV[p*3+2].z;
		// �ﰢ���� �ٿ���ڽ�.
		MAABB tbox;
		tbox.Min.x = tbox.Max.x = tv1.x;
		tbox.Min.y = tbox.Max.y = tv1.y;
		tbox.Min.z = tbox.Max.z = tv1.z;
		for( i = 0; i < 3; i++)
		{
			if( tbox.Min.x > pIV[p*3+i].x) tbox.Min.x = pIV[p*3+i].x;
			if( tbox.Min.y > pIV[p*3+i].y) tbox.Min.y = pIV[p*3+i].y;
			if( tbox.Min.z > pIV[p*3+i].z) tbox.Min.z = pIV[p*3+i].z;

			if( tbox.Max.x < pIV[p*3+i].x) tbox.Max.x = pIV[p*3+i].x;
			if( tbox.Max.y < pIV[p*3+i].y) tbox.Max.y = pIV[p*3+i].y;
			if( tbox.Max.z < pIV[p*3+i].z) tbox.Max.z = pIV[p*3+i].z;
		}

		// �̰� ��ġ�� ����Ʈ�ʵ��� ���� x, y, x����, y���� ���Ѵ�.
		// x
		float	fXOffset = tbox.Min.x - box.Min.x;		
		DWORD	dwXStartIndex = int( fXOffset/fXStep);				// ������ �ε���. // int Ÿ��ĳ��Ʈ�� ����.
		if( dwXStartIndex != 0) dwXStartIndex--;
		fXOffset	=	tbox.Max.x - box.Min.x;
		DWORD	dwXEndIndex = int( fXOffset/fXStep);			// ���� �ε���.
		if( dwXEndIndex != x-1)	dwXEndIndex++;

		// z
		float	fZOffset = tbox.Min.z - box.Min.z;
		DWORD	dwZStartIndex = int( fZOffset/fZStep);				// ������ �ε���, z
		fZOffset	=	tbox.Max.z - box.Min.z;
		DWORD	dwZEndIndex	=	int( fZOffset/fZStep);				// ���� �ε���. z
		if( dwZEndIndex != z-1) dwZEndIndex++;
		if( dwZStartIndex != 0) dwZStartIndex--;

		
		for( j = dwZStartIndex; j <= dwZEndIndex; j++)		// z��Ʈ.
		{
		// for( x����.){
			for( i = dwXStartIndex; i <= dwXEndIndex; i++)
			{
		//	�̹� ������ �ﰢ���̶� �����°�.?
				lv1.x = lv2.x = pV[i+j*x].x;	// �浹ó�� �� ������.
				lv1.z = lv2.z = pV[i+j*x].z;
				lv1.y = box.Min.y;
				lv2.y = box.Max.y;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)		// ������ ���.
				{
					// �̰� ����.
					pV[i+j*x].x = box.Min.x + (float)fXStep*i;
					pV[i+j*x].y = rv1.y;
					pV[i+j*x].z = box.Min.z + (float)fZStep*j;
					break;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else
				{
					// ���� �߸��� ���̽���.
					_asm int 3;
				}


			}	// i

		}	// j
	}

*/



	for( j = 0; j < z; j++)	// z���.
	{
		for( i = 0; i < x; i++)	// x���.
		{
//			pV[i+j*x].x = box.Min.x + (float)fXStep*i;
//			pV[i+j*x].z = box.Min.z + (float)fZStep*j;
			// �浹ó���� ����.
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = box.Min.y;
			lv2.y = box.Max.y;
			//y���� ���̰�. �ﰢ���� �ѹ��� ���鼭 �浹ó�� �˻�.
			bool	bSet = false;
			for( p = 0; p < dwTriCount; p++)
			{
				tv1.x = pIV[p*3].x;
				tv1.y = pIV[p*3].y;
				tv1.z = pIV[p*3].z;
				tv2.x = pIV[p*3+1].x;
				tv2.y = pIV[p*3+1].y;
				tv2.z = pIV[p*3+1].z;
				tv3.x = pIV[p*3+2].x;
				tv3.y = pIV[p*3+2].y;
				tv3.z = pIV[p*3+2].z;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)
				{
					// �̰� ����.
					pV[i+j*x].y = rv1.y;
					bSet = true;
					break;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else
				{
					// ���� �߸��� ���̽���.
					_asm int 3;
				}
			}
			if( bSet == false)
			{
//				_asm int 3;
				// �̸��� ������...
				// �浹 ó�� �Լ��� float�� �ϴϱ� �������� Ŀ���ų�, �ﰢ���� �е��� �����ϱ� �� ����� ������.
				// �ƴϸ� �޽��� ������ �ִ� ��쵵 �̸� ���� �� �ִ�.
				pV[i+j*x].y = box.Min.y;
			}

		}
	}

Finish:
	// ���۸� ����Ʈ�ʵ� ��ǥ�迡 �°� ��迭.
/*
	VECTOR3*	pResult = new VECTOR3[x*z];
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
//			pResult[i+j*x].x = pV[i+(z-j-1)*x].x;
//			pResult[i+j*x].y = pV[i+(z-j-1)*x].z * -1;
//			pResult[i+j*x].z = pV[i+(z-j-1)*x].y;
		}
	}

	delete[] pV;

	return	pResult;
*/

/*
	//���Ʒ� �ٲ���.
	VECTOR3*	pResult = new VECTOR3[x*z];
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
			pResult[i+j*x].x = pV[i+(z-j-1)*x].x;
			pResult[i+j*x].y = pV[i+(z-j-1)*x].y;
			pResult[i+j*x].z = pV[i+(z-j-1)*x].z;
		}
	}
	delete[] pV;
	return pResult;
*/
//	LogInit();
	for( j = 0; j < z; j++)
	{
		for( i = 0; i < x; i++)
		{
//			LogPrintf("%d, %d = %f, %f, %f", i, j, pV[i+j*x].x, pV[i+j*x].y, pV[i+j*x].z);	
		}
	}
	return pV;
}
/*
VECTOR3*	SampleHeightFieldFromIVertex_old2( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD dwX, DWORD dwZ)
{
//	LogInit();
	DWORD	i, j, p;
	DWORD	x = dwX, z = dwZ;
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2, u1, v1;

	VECTOR3*	pV	=	new VECTOR3[x*z];
	memcpy( pV, pVInput, sizeof(VECTOR3) * x* z);
	// ��ü �ٿ�� �ڽ�.
	MAABB	WorldBox;
	WorldBox.Max.x = WorldBox.Min.x = pV[0].x;
	WorldBox.Max.y = WorldBox.Min.y = pV[0].y;
	WorldBox.Max.z = WorldBox.Min.z = pV[0].z;

//	for( i = 0; i < dwTriCount*3; i++)
	for( i = 0; i < x*z; i++)
	{
		// max
		if( pV[i].x > WorldBox.Max.x)	WorldBox.Max.x = pV[i].x;
		if( pV[i].y > WorldBox.Max.y)	WorldBox.Max.y = pV[i].y;
		if( pV[i].z > WorldBox.Max.z)	WorldBox.Max.z = pV[i].z;
		// min
		if( pV[i].x < WorldBox.Min.x)	WorldBox.Min.x = pV[i].x;
		if( pV[i].y < WorldBox.Min.y)	WorldBox.Min.y = pV[i].y;
		if( pV[i].z < WorldBox.Min.z)	WorldBox.Min.z = pV[i].z;
	}
	WorldBox.Max.y	=	900000.0f;
	WorldBox.Min.z	=	-900000.0f;

	for( j = 0; j < z; j++)	// z���.
	{
		for( i = 0; i < x; i++)	// x���.
		{
//			pV[i+j*x].x = WorldBox.Min.x + (float)fXStep*i;
//			pV[i+j*x].z = WorldBox.Min.z + (float)fZStep*j;
			// �浹ó���� ����.
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = WorldBox.Min.y;
			lv2.y = WorldBox.Max.y;
			//y���� ���̰�. �ﰢ���� �ѹ��� ���鼭 �浹ó�� �˻�.
			bool	bSet = false;
			for( p = 0; p < dwTriCount; p++)
			{
				tv1.x = pIV[p*3].x;
				tv1.y = pIV[p*3].y;
				tv1.z = pIV[p*3].z;
				tv2.x = pIV[p*3+1].x;
				tv2.y = pIV[p*3+1].y;
				tv2.z = pIV[p*3+1].z;
				tv3.x = pIV[p*3+2].x;
				tv3.y = pIV[p*3+2].y;
				tv3.z = pIV[p*3+2].z;


				VECTOR3 pTri[3];
				pTri[0] = tv1;
				pTri[1] = tv2;
				pTri[2] = tv3;

				if( IsCollisionTriAndLine(pTri, &lv1, &lv2))
				{
						_asm nop;
						_asm int 3;
				}



				VECTOR3 dir;
				dir.x = lv2.x - lv1.x;
				dir.y = lv2.y - lv1.y;
				dir.z = lv2.z - lv1.z;


				if( IntersectTriangle( &lv1, &dir, &tv1, &tv2, &tv3, &t1, &u1, &v1))		// �浹�������.
				{
					if( t1 <= 1.1 && t1 >= -0.1)
					{
						rv1.x = lv1.x + t1*dir.x;
						rv1.y = lv1.y + t1*dir.y;
						rv1.z = lv1.z + t1*dir.z;
						bSet = true;
						pV[i+j*x].y = rv1.y;
						break;
					}
				}


			}
			if( bSet == false)
			{
//				_asm int 3;
				// �̸��� ������...
				// �浹 ó�� �Լ��� float�� �ϴϱ� �������� Ŀ���ų�, �ﰢ���� �е��� �����ϱ� �� ����� ������.
				// �ƴϸ� �޽��� ������ �ִ� ��쵵 �̸� ���� �� �ִ�.
				pV[i+j*x].y = WorldBox.Min.y;
			}

		}
	}


	return	pV;
}
*/

VECTOR3*	SampleHeightFieldFromIVertex( VECTOR3* pVInput, IVERTEX* pIV, DWORD dwTriCount, DWORD x, DWORD z)
{
	/*
	DWORD	i, j, p;		// �ӽ� ����.
	VECTOR3 tv1, tv2, tv3, lv1, lv2, rv1, rv2;
	float	t1, t2, u1, v1;

	MOctree2		tree;
	DWORD*		pHandle	=	new DWORD[dwTriCount];

	VECTOR3*	pV	=	new VECTOR3[x*z];
	memcpy( pV, pVInput, sizeof(VECTOR3)*x*z);

	// ���� �ٿ�� �ڽ�.
	MAABB wbox;
	wbox.Max.x = wbox.Min.x = pVInput[0].x;
//	wbox.Max.y = wbox.Min.y = pVInput[0].y;
	wbox.Max.z = wbox.Min.z = pVInput[0].z;
	for( i = 0; i < x*z; i++)
	{
		if( wbox.Max.x < pV[i].x) wbox.Max.x = pV[i].x;
		if( wbox.Max.z < pV[i].z) wbox.Max.z = pV[i].z;

		if( wbox.Min.x > pV[i].x)	wbox.Min.x = pV[i].x;
		if( wbox.Min.z > pV[i].z)	wbox.Min.z = pV[i].z;
	}
	wbox.Max.y = 900000.0f;
	wbox.Min.y = -900000.0f;

	// ���̹��ؽ��� Ʈ�� ����.
	tree.Init();
	for( p = 0; p < dwTriCount; p++)
	{
		// �ٿ�� �ڽ� �����,
		VECTOR3	objMax, objMin;
		objMax.x = objMin.x = pIV[p*3].x;
		objMax.y = objMin.y = pIV[p*3].y;
		objMax.z = objMin.z = pIV[p*3].z;
		for( i = 1; i < 3; i++)
		{
			if( objMax.x < pIV[p*3+i].x)	objMax.x = pIV[p*3+i].x;
			if( objMax.y < pIV[p*3+i].y)	objMax.y = pIV[p*3+i].y;
			if( objMax.z < pIV[p*3+i].z)	objMax.z = pIV[p*3+i].z;

			if( objMin.x > pIV[p*3+i].x)	objMin.x = pIV[p*3+i].x;
			if( objMin.y > pIV[p*3+i].y)	objMin.y = pIV[p*3+i].y;
			if( objMin.z > pIV[p*3+i].z)	objMin.z = pIV[p*3+i].z;
		}
		// Ʈ���� Ǫ��, �ڵ� ��´�.
		pHandle[p] = tree.AddStaticObject( objMax, objMin);
	}
	// Ʈ�� ����.
	tree.Build( wbox.Max, wbox.Min,  5, 5);

	for( j = 0; j < z; j++)
	{
		if( j == 200)
		{
			_asm nop;
		}
		for( i = 0; i < x; i++)
		{
			lv1.x = lv2.x = pV[i+j*x].x;
			lv1.z = lv2.z = pV[i+j*x].z;
			lv1.y = 900000.0f;
			lv2.y = -900000.0f;
			// ������ ������Ʈ ��,
			DWORD	dwClock =  GetLowClock();
			DWORD	dwCount = tree.FindMeetObjectWithLineSegment( lv1, lv2);
			dwClock = GetLowClock() - dwClock;
			bool	bSet = false;
			// �浹ó��
			for( p = 0; p < dwCount; p++)
			{
				DWORD* pBuff = tree.GetCandidateBuffer();
				DWORD d = pBuff[p];

				tv1.x = pIV[d*3].x;
				tv1.y = pIV[d*3].y;
				tv1.z = pIV[d*3].z;
				tv2.x = pIV[d*3+1].x;
				tv2.y = pIV[d*3+1].y;
				tv2.z = pIV[d*3+1].z;
				tv3.x = pIV[d*3+2].x;
				tv3.y = pIV[d*3+2].y;
				tv3.z = pIV[d*3+2].z;

				COLLISIONTYPE ct = TriangleMeetLineSegment( tv1, tv2, tv3, lv1, lv2, rv1, rv2, t1, t2);
				if( ct == COLLISIONTYPE_Vertex)
				{
					// �̰� ����.
					pV[i+j*x].y = rv1.y;
					bSet = true;
					break;
				}
				else if( ct == COLLISIONTYPE_Error)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_LineSegment)
				{
					_asm int 3;
				}
				else if( ct == COLLISIONTYPE_None)
				{
					;
				}
				else
				{
					// ���� �߸��� ���̽���.
					_asm int 3;
				}

			}
			if( bSet == false)
			{
//				_asm int 3; // �ϳ��� ��������. -_-;
			}
		}
	}
	return pV;
*/
	return 0;
}