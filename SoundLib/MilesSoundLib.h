// MilesSoundLib.h: interface for the CMilesSoundLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MILESSOUNDLIB_H__FF251218_4048_4F8F_9072_839230049027__INCLUDED_)
#define AFX_MILESSOUNDLIB_H__FF251218_4048_4F8F_9072_839230049027__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"
#include "mss/mss.h"
#include "SoundEffectFile.h"
#include "SoundEffect.h"
#include "../4DyuchiGXGFunc/global.h"



class CMilesSoundLib	:	public IMilesSoundLib
{
private:
	void Shutdown3DDriver();
	BOOL Load3DDriver( HPROVIDER driver);
	void ShutdownBGMDriver(void);

	MATRIX4			m_mListenerRev;
	VECTOR3			m_vListenerPos;
	VECTOR3			m_vListenerAng;

	// MSS
	BOOL			m_bMilesStartUp;

	// BGM
	HDIGDRIVER		m_BGMDriver;
	HSTREAM			m_BGMHandle;
	BOOL		StartupBGMDriver(void);
	float			m_fBGMVolume;

	// 3D sound driver
	HPROVIDER		m_3DDriver;
	DWORD			m_dw3DDriverType;
	BOOL			Startup3DDriver( BOOL bTryEAX, BOOL bForceSoftware);
	HPROVIDER Find3DDriver( const char* szDriverName);

	// Sound Effect File
	CSoundEffectFile**		m_ppSoundFileBuffer;
//	void**					m_ppSoundFileHashHandleBuffer;
	DWORD					m_dwMaxSoundFileCount;
	DWORD					m_dwSoundFileCount;
	INDEXCR_HANDLE			m_hSoundFileIndexCreator;
	VBHASH_HANDLE			m_hSoundFileHash;

	STMPOOL_HANDLE			m_hSoundFileMemoryPool;

	//	Sound Effect
	DWORD					m_dwSoundEffectCount;
	DWORD					m_dwMaxSoundEffectCount;
	CSoundEffect**			m_ppSoundEffectBuffer;
	INDEXCR_HANDLE			m_hSoundEffectIndexCreator;
//	VBHASH_HANDLE			m_hSoundEffectHash;					// eos�ݹ��� ���� �� ���� hSample�� ������ �ش� effect�� ã�� �� ����.
	STMPOOL_HANDLE			m_hSoundEffectMemoryPool;


	// ���� ���� Ǯ.
	H3DSAMPLE*				m_pSamplePool;
	DWORD					m_dwMaxSamplePoolCount;
	DWORD					m_dwSamplePoolCount;
	INDEXCR_HANDLE			m_hSamplePoolIndexCreator;

	DWORD					m_dwMaxVoiceCount;

public:
	void DeleteSoundEffectFile(SOUND_FILE_HANDLE hFile);
	void SoundEffectSetPosition( VECTOR3* pOutPos, CSoundEffect* pEffect, VECTOR3* pPos);
	BOOL IsValidFileHandle( SOUND_FILE_HANDLE hSound);			// ������ �ڵ��� �����Ѱ�?
//	void EndOfSoundCallback( H3DSAMPLE hSample);				// ���� ������ End Of Sample �ݹ�.

	SOUND_ERROR_CODE	_stdcall	Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware);// ���� �޴��� �ʱ�ȭ.

//	DWORD				_stdcall	GetAllocatedEffectCount(void);

	SOUND_3D_DRIVER		_stdcall	Get3DDriver(void);

	void				_stdcall	SetListener( VECTOR3* pPosition, VECTOR3* pAngle);

	SOUND_SPEAKER_TYPE	_stdcall Get3DSpeakerType(void);
	void				_stdcall Set3DSpeakerType( SOUND_SPEAKER_TYPE Type);



	SOUND_ERROR_CODE	_stdcall	PlayBGM( char* szBGMFileName);
	void				_stdcall	StopBGM(void);
	void				_stdcall	SetBGMVolume( float fVolume);

	// 3D Driver

	// Sound Effect File
	SOUND_ERROR_CODE	_stdcall	CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char* szFileName);
	void				_stdcall	ReleaseSoundEffectFile( SOUND_FILE_HANDLE hFile);

	// Sound Effect
	SOUND_ERROR_CODE	_stdcall	CreateSoundEffect( ISoundEffect** ppSoundEffect, SOUND_FILE_HANDLE hSound);

	void				_stdcall	Run();

	DWORD				_stdcall	GetAllocatedSoundEffectCount();
	DWORD				_stdcall	GetAllocatedSoundFileCount();


	void				DeletingSoundEffect( DWORD dwEffectIndex, CSoundEffect* pEffect);		// SounEffect�� Release�� ȣ�� �Ǿ����� SoundLib�� �뺸�ϱ� ���� ���δ�.
	void				DeletingSoundFile( DWORD dwFileIndex, void* hHash, CSoundEffectFile* pFile);
//	void*				AllocSample( CSoundEffect* pEffect, H3DSAMPLE hSample);				// ���ϰ��� �ؽ��� ���� ������.

	DWORD			_stdcall	ReleaseAllSoundEffect(void);
	DWORD			_stdcall	ReleaseAllSoundFile(void);


	void	_stdcall	Delete();						// ���� �޴��� ����ȭ.
	void	_stdcall	GetErrorString( char** pszErrorString, SOUND_ERROR_CODE);

	void	_stdcall	SetMaxVoiceCount( DWORD dwMaxVoice);
	DWORD	_stdcall	GetMaxVoiceCount();


	CMilesSoundLib();
	virtual ~CMilesSoundLib();

//	void				Check(void);

	BOOL ReleaseSampleFromSoundEffect( DWORD dwSampleIndex);
	BOOL AllocSampleToSoundEffect( H3DSAMPLE* phOutSample, DWORD* pdwOutIndex);
protected:
	// ���̺귯�� �ʱ�ȭ�� ���� Ǯ�� ��´�.
	void AllocSamplePool( );

	// ���� ���̺귯�� ����ȭ�� ���� Ǯ�� �����Ѵ�.
	void ReleaseSamplePool();
};

#endif // !defined(AFX_MILESSOUNDLIB_H__FF251218_4048_4F8F_9072_839230049027__INCLUDED_)
