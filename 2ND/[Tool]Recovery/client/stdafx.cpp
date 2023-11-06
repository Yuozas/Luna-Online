// stdafx.cpp : 표준 포함 파일을 포함하는 소스 파일입니다.
// client.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj는 미리 컴파일된 형식 정보를 포함합니다.

#include "stdafx.h"


void StringCopySafe( char* dest, const char* src, int size )
{
	strncpy( dest, src, size - 1 );
	dest[ size -1 ] = 0;
}