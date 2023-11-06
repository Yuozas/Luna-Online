// SoundEffectFile.cpp: implementation of the CSoundEffectFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MilesSoundLib.h"
#include "SoundEffectFile.h"
#include "mss/mss.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CSoundEffectFile::CSoundEffectFile()
{

}

CSoundEffectFile::~CSoundEffectFile()
{
	
}
*/

DWORD CSoundEffectFile::AddRef()
{
	m_dwRefCount++;
	return	m_dwRefCount;
}

DWORD CSoundEffectFile::Release()
{
#ifdef	_DEBUG
	if( m_dwRefCount == 0)
	{
//		OutputDebugString("���� ����� �θ��ÿ�. -_-;");
		OutputDebugString("Call me -_-;\n");
		_asm int 3;
	}
	if( m_dwRefCount > 0x00ffffff)
	{
//		OutputDebugString("���� ����� �θ��ÿ�. -_-;");
		OutputDebugString("Call me -_-;");
		_asm int 3;
	}
#endif

	m_dwRefCount--;
	if( m_dwRefCount == 0)
	{
//		Delete();
//		delete this;		// ������ �������� �ʰ�, CMilesSoundLib::ReleaseSoundEffectFile()���� ���۷����� 0�϶� ȣ��ȴ�.
		m_pSoundLib->DeleteSoundEffectFile( this);
		return	0;
	}
	return	m_dwRefCount;
}

SOUND_ERROR_CODE CSoundEffectFile::Create(char *szFileName, void* hHashHandle, DWORD dwSoundLibIndex, CMilesSoundLib* pSoundLib)
{

	if( pSoundLib == 0 || hHashHandle == 0 || pSoundLib == 0)
	{
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}

	DWORD	dwFileSize	=	AIL_file_size( szFileName);
	if( dwFileSize == 0xffffffff)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}

	void*	pData	=	AIL_file_read( szFileName, 0);
	if( pData == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}

	S32	iFileType	=	AIL_file_type( pData, dwFileSize);


	m_pWaveData		=	0;		// �����Ͱ� �� ������.

	AILSOUNDINFO info;
	switch( iFileType)
	{
		case	AILFILETYPE_PCM_WAV:
			m_pWaveData	=	pData;
			break;
		case	AILFILETYPE_ADPCM_WAV:
			AIL_WAV_info( pData, &info);
			AIL_decompress_ADPCM( &info, &m_pWaveData, 0);
			AIL_mem_free_lock( pData);
			break;
		case	AILFILETYPE_MPEG_L3_AUDIO:
			AIL_decompress_ASI( pData, dwFileSize, szFileName, &m_pWaveData, 0, 0);
			AIL_mem_free_lock( pData);
			break;
		default:
			AIL_mem_free_lock( pData);

	}

	if( m_pWaveData == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}

	m_hHashHandle	=	hHashHandle;
	m_dwSoundLibIndex	=	dwSoundLibIndex;
	m_pSoundLib		=	pSoundLib;
	m_dwRefCount	=	1;					// ���� �����Ǵ� ���۷��� ī����.
	strcpy( m_szFileName, szFileName);		// ���� �̸� ī�� �ϰ�,

	return	SOUND_ERROR_NOERROR;
}

void CSoundEffectFile::Delete()
{

	if( m_dwRefCount != 0)
	{
		_asm int 3;				// ���� ����� �����.
		m_dwRefCount	=	0;
	}

	AIL_mem_free_lock( m_pWaveData);
	m_pWaveData	=	0;

	// SoundLib�� �뺸�Ѵ�.
//	m_pSoundLib->DeletingSoundFile( m_dwSoundLibIndex, m_hHashIndex, this);

}
