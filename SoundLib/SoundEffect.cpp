// SoundEffect.cpp: implementation of the CSoundEffect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MilesSoundLib.h"
#include "SoundEffect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CSoundEffect::CSoundEffect()
{

}
CSoundEffect::~CSoundEffect()
{

}
*/
/*
	Create �Ǵ� �������� �ٷ� �ڵ��� �޾Ƽ� ��� �޴°� �ƴϰ� ������ ���� �����͸� ������ �ִٰ�,
	Play()�� ȣ��Ǵ� �������� CMilesSoundLib���� ��� ���۸� �ϳ� �Ҵ� ���� �� �÷��� �Ѵ�.
*/
BOOL CSoundEffect::Create( CSoundEffectFile* pFile, HPROVIDER h3DDriver, CMilesSoundLib* pSoundLib, DWORD dwEffectIndex)
{
#ifdef	_DEBUG
	if( pFile == 0 || h3DDriver == 0 || pSoundLib == 0 )
	{
		_asm int 3;
	}
	if( pFile->GetWaveBuffer() == 0)
	{
		_asm int 3;
	}
#endif

	m_pFile			=	pFile;
	pFile->AddRef();

	m_pSoundLib		=	pSoundLib;			// �޴��� Ŭ������ ������.
	m_vPosition.x	=	m_vPosition.y	=	m_vPosition.z	=	0.0f;
	m_fVolume		=	1.0f;				// �ʱ� ����. 0.0f~1.0f����.
	m_h3DDriver		=	h3DDriver;				// ����̹� �ڵ�.
	m_hSoundSample	=	0;
	m_dwSampleIndex	=	0xffffffff;
	m_dwRefCount	=	1;					// ���۷��� ī����

	m_dwSoundLibIndex	=	dwEffectIndex;	// �޴��� Ŭ���������� this�� �ε���.

	m_fMaxDistance	=	100.0f;
	m_fMinDistance	=	10.0f; 

	return	TRUE;
}

void CSoundEffect::Delete()
{
#ifdef		_DEBUG
	if( m_dwRefCount != 0)
	{
		_asm int 3;
	}
#endif

	m_pSoundLib->DeletingSoundEffect( m_dwSoundLibIndex, this);

	m_h3DDriver	=	0;
	if( m_hSoundSample)
	{
		this->Stop();
	}
	DWORD	dwRef	=	m_pFile->Release();
#ifdef			_DEBUG
	if( dwRef == 0)
		_asm nop			// ���⼭ �ɸ��ٴ� ���� �̳��� ���� �ִ� ���� ���� �ڵ��� ���� �������� �̳��� ���߿� �������� ���Ѵ�.
#endif
	m_pFile		=	0;
	m_dwRefCount	=	0;
	m_pSoundLib		=	0;
	m_dwSampleIndex	=	0xffffffff;
	m_dwSoundLibIndex	=	0xffffffff;

}

// ���� ����ִ°�? �ܺο��� �̳��� ���� ������ �˾Ƴ� �� ���δ�.
BOOL CSoundEffect::IsAlive(void)
{
	if( m_hSoundSample == 0)
	{
		return	FALSE;
	}
	
	U32	status	=	AIL_3D_sample_status( m_hSoundSample);
	switch( status)
	{
		case SMP_DONE:
			return	FALSE;
		case	SMP_PLAYING:
			return	TRUE;
		case	SMP_STOPPED:
			_asm nop;
			break;
		case	SMP_FREE:
			_asm nop;
			break;
		case	SMP_PLAYINGBUTRELEASED:
			_asm nop;
			break;
		default:
			_asm int 3;
	}




	// �ֳ���!
	return	FALSE;

}

// ���� �÷��� �� �Ǿ��ִ°�.?
BOOL _stdcall CSoundEffect::IsLoop(void)
{
	return	m_bLoop;
}

// ���.
/*
	���� �Ҵ�ޱ⸦ �õ��Ѵ�.
	�ؼ� �Ǹ� �ǰ�, �ȵǸ� �ȵȴ�. -_-;
	�̹� �÷������� ��� �ٽ� �÷��� �Ѵ�.
*/
void _stdcall CSoundEffect::Play( BOOL bLoop)
{

	BOOL		bAllocSample	=	FALSE;

	if( !m_hSoundSample)
	{
		bAllocSample	=	AllocSample();
		if( bAllocSample == FALSE)
			goto	lbFailed;
 		_asm nop;
	}
	else
	{
		// ���� �Ҵ��� �޾Ƴ��� ��� play, stop, play stop �� ����̶�� �̷� �� �ִ�.
		_asm nop;
	}

	AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	this->SetMaxMinDistance( m_fMaxDistance, m_fMinDistance);
	this->SetVolume( m_fVolume);

	m_bLoop	=	bLoop;
	if( m_bLoop)
		AIL_set_3D_sample_loop_count(  m_hSoundSample, 0);
	else
		AIL_set_3D_sample_loop_count( m_hSoundSample, 1);

	AIL_start_3D_sample(  m_hSoundSample);


	return	;

lbFailed:
	return	;
}

// ����.
void _stdcall CSoundEffect::Stop(void)
{
	if( m_hSoundSample)
	{
		AIL_end_3D_sample(  m_hSoundSample);
		ReleaseSample();
	}
/*
	else
	{
		_asm int 3;			// Stop�� ȣ��Ǿ��ٴ� ���� ���������� ������ �Ҵ�ް� �÷��̰� �Ǿ��ٴ°��� ���Ѵ�.
							// ���� ������ �ƴ� �ѿ� ȣ��� ���� ������?
	}
*/
}

void _stdcall CSoundEffect::SetMaxMinDistance( float fMaxDist, float fMinDist)
{
#ifdef			_DEBUG
	if( fMinDist < 0.0f || fMinDist > fMaxDist)
	{
		_asm int 3;
	}
#endif
	
	if( m_hSoundSample)
	{
		AIL_set_3D_sample_distances(m_hSoundSample, fMaxDist, fMinDist);
	}

	m_fMaxDistance	=	fMaxDist;
	m_fMinDistance	=	fMinDist;
}

// ��ġ �����Ѵ�.
void _stdcall CSoundEffect::SetPosition( VECTOR3* pPosition)
{
#ifdef			_DEBUG
	if( pPosition == 0)
	{
		_asm int 3;
	}
#endif

	VECTOR3		OutPos;
	m_pSoundLib->SoundEffectSetPosition( &OutPos, this, pPosition);

	m_vPosition	=	OutPos;
	

	if( m_hSoundSample )
	{

		// �Ÿ� �׽�Ʈ�� �ؼ� m_fMaxDistance ���� �ָ� Stop ��Ų��.
/*
		fDistance	=	VECTOR3Length( &m_vPosition);
		if( fDistance > m_fMaxDistance)
		{
			Stop();
		}
		else
*/
		AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}

}

// ��� ��ġ ����.
void _stdcall CSoundEffect::SetRelativePosition( VECTOR3* pPosition)
{
#ifdef	_DEBUG
	if( pPosition == 0)
		_asm int 3;
#endif

	m_vPosition		=	*pPosition;
	if( m_hSoundSample)
	{
		AIL_set_3D_position( m_hSoundSample, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	}
}


void _stdcall CSoundEffect::SetVolume( float fVolume)
{
#ifdef			_DEBUG
	if( fVolume < 0.0f)
	{
		_asm int 3;
	}

	if( fVolume > 1.0f)
	{
		_asm int 3;
	}
#endif

	m_fVolume	=	fVolume;


	if( m_hSoundSample)
	{
		AIL_set_3D_sample_volume( m_hSoundSample, m_fVolume);
	}

}

float _stdcall CSoundEffect::GetVolume(void)
{
	return	m_fVolume;
}

DWORD _stdcall CSoundEffect::AddRef()
{
	m_dwRefCount++;
	return	m_dwRefCount;
}


DWORD _stdcall CSoundEffect::Release()
{
	m_dwRefCount--;
	if( m_dwRefCount == 0)
	{
		this->Delete();
	}
	return	0;
}

BOOL CSoundEffect::AllocSample()
{

	if( m_hSoundSample)
	{
		_asm int 3;			// �̹� �Ҵ� �޾Ƴ��µ�?
		return	FALSE;
	}

	// ������̺귯������ �Ҵ��� �޴´�.
	H3DSAMPLE	hSample;
	DWORD		dwIndex;
	BOOL	bAlloc	=	m_pSoundLib->AllocSampleToSoundEffect( &hSample, &dwIndex);
	if( bAlloc == FALSE)
	{
		// �ִ� �Ҵ緮�� �Ѿ���? ��·�� ���� ����.
		return	FALSE;
	}

	// ���� �����̶� ���ε�. �̳� ���̷��� ������.
	S32			sResult	=	AIL_set_3D_sample_file( hSample, m_pFile->GetWaveBuffer());
	if( sResult == 0)
	{
		// ���� ������ �ȵǴ� ���.		�̷� ���� ����� ����ѵ�... �ֱ׶��.. 
		// ���� �̷��� ������ ����.
		char*	szLastError	=	AIL_last_error();
		_asm nop;
		// �׳� ������ �ϰ� 
		BOOL	bRelease	=	m_pSoundLib->ReleaseSampleFromSoundEffect( dwIndex);
		if( bRelease == FALSE)
		{
			_asm int 3;
		}
		return	FALSE;
 	}

	// �̱��� ���� ������ �޴µ� ���� �ߴ�.
	m_hSoundSample	=	hSample;
	m_dwSampleIndex	=	dwIndex;


	return	TRUE;
}

void CSoundEffect::ReleaseSample()
{
	if( m_hSoundSample)
	{
		// ������ ��� �ִ�, �������� �����, 
		m_pSoundLib->ReleaseSampleFromSoundEffect( m_dwSampleIndex);
		m_hSoundSample	=	0;
		m_dwSampleIndex	=	0xffffffff;
	}
	else
		_asm nop;
}

// ���ν���. soundlib���� run�� �پ� ����ȴ�.
int CSoundEffect::Procedure(void)
{

	return	0;
}

