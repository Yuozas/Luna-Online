// stdafx.cpp : ǥ�� ���� ������ �����ϴ� �ҽ� �����Դϴ�.
// client.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj�� �̸� �����ϵ� ���� ������ �����մϴ�.

#include "stdafx.h"


void StringCopySafe( char* dest, const char* src, int size )
{
	strncpy( dest, src, size - 1 );
	dest[ size -1 ] = 0;
}