#include "stdafx.h"
#include ".\delay.h"

cDelay::cDelay(void)
{
	mStart = 0;
	mDelay = 0;
	mRemain = 0;
	mRate = 0.0f;
}

cDelay::~cDelay(void)
{
}

void cDelay::Init( DWORD time )
{
	mDelay = time;
	mStart = 0;
	mRemain = 0;
	mRate = 0.0f;
}

void cDelay::Start()
{
	mStart = gCurTime;

	mRemain = mDelay;
	mRate = 100.0f;

	mbStart = TRUE;
}

DWORD cDelay::Check()
{
	/// ������ ī��Ʈ�� ���۵Ǿ��ٸ�
	if( mbStart )
	{
		/// ���ۺ��� ������� ����ð�
		DWORD spend = gCurTime - mStart;

		/// ������ �ð����� ũ�ų� ���ٸ�
		if( mDelay <= spend )
		{
			/// ī��Ʈ ����
			End();
			return 0;
		}
		else
		{
			/// ���� �ð� ���
			mRemain = mDelay - spend;

			/// ���� �ð� ���� ���
			mRate = ( float )( mDelay / mRemain );

			/// �����ð��� �����ش�
			return mRemain;
		}
	}
	else
	{
		return 0;	
	}
}

void cDelay::End()
{
	mbStart = FALSE;

	mStart = 0;
	mRemain = 0;
	mRate = 0.0f;
}

BOOL cDelay::IsStart()
{
	return mbStart;
}

DWORD cDelay::GetStartTime()
{
	return mStart;
}

DWORD cDelay::GetDelayTime()
{
	return mDelay;
}

DWORD cDelay::GetRemainTime()
{
	return mRemain;
}

float cDelay::GetRemainRate()
{
	return mRate;
}