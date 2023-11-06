#include "stdafx.h"
#include ".\timedelay.h"

cTimeDelay::cTimeDelay(void)
{
	mStart = 0;
	mDelay = 0;
	mRemain = 0;
	mRate = 0.0f;
}

cTimeDelay::~cTimeDelay(void)
{
}

void cTimeDelay::Init( DWORD time )
{
	mDelay = time;
}

void cTimeDelay::Start()
{
	mStart = gCurTime;
	mbStart = TRUE;
	mRemain = mDelay;
	mRate = 1.0f;
}

DWORD cTimeDelay::Check()
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
			mRate = (float)((double)mRemain / (double)mDelay);

			/// �����ð��� �����ش�
			return mRemain;
		}
	}
	else
	{
		return 0;	
	}
}

void cTimeDelay::End()
{
	mbStart = FALSE;
}

