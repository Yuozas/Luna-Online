// SoundEffect.h: interface for the CSoundEffect class.
//
//////////////////////////////////////////////////////////////////////
/*
	���� ����Ʈ.
	��ǻ��� �������̽� ���Ҹ� �ϰ� ��κ��� ���� MilesSoundLib���� �� �Ϸ��� ����Ѵ�.
*/
#if !defined(AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_)
#define AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"
#include "SoundEffectFile.h"
#include "mss/mss.h"

class	CMilesSoundLib;

class CSoundEffect  :	public ISoundEffect
{
private:
	VECTOR3			m_vPosition;				// �Ҹ��� ���� ��ġ.
//	VECTOR3			m_vVelocity;
	float			m_fVolume;					// ���� 0.0f ~ 1.0f
	HPROVIDER		m_h3DDriver;				// 3D ����̹�.
	H3DSAMPLE		m_hSoundSample;				// CMilesSoundLib���� ������ �Ҵ� �޾������� �ڵ�.
	DWORD			m_dwSampleIndex;			// �Ҵ� ���� ������ �ε���.

	CSoundEffectFile*	m_pFile;				// �ҽ� ����.

	float			m_fMaxDistance;				// �ִ� �Ÿ�.
	float			m_fMinDistance;				// �ּ� �Ÿ�.

	BOOL			m_bLoop;

	DWORD			m_dwRefCount;				// ���۷��� ī��Ʈ

	CMilesSoundLib*		m_pSoundLib;
	DWORD				m_dwSoundLibIndex;				// CMilesSoundLib ������ �ε���.

	BOOL AllocSample();
	void ReleaseSample();

public:
	BOOL	Create( CSoundEffectFile* pFile, HPROVIDER h3DDriver, CMilesSoundLib* pSoundLib, DWORD dwEffectIndex);	// ������.
	void	Delete();																// �Ҹ���.

	H3DSAMPLE		GetSample(void){ return	m_hSoundSample;}
	DWORD			GetIndex(void){ return m_dwSoundLibIndex;}

	int				Procedure(void);			// ���ν���. soundlib���� run�� �پ� ����ȴ�.

// �ܺο��� ���̴� �������̽���.
	void	_stdcall	Play( BOOL bLoop);		// ���.
	void	_stdcall	Stop(void);		// ����.

	void	_stdcall	SetMaxMinDistance( float fMaxDist, float fMinDist);

	void	_stdcall	SetPosition( VECTOR3* pPosition);	// ��ġ �����Ѵ�.
	void	_stdcall	SetRelativePosition( VECTOR3* pPosition);		// ��� ��ġ ����.
	void	_stdcall	SetVolume( float fVolume);
	float	_stdcall	GetVolume(void);

	BOOL	_stdcall	IsAlive(void);		// ���� ����ִ°�? �ܺο��� �̳��� ���� ������ �˾Ƴ� �� ���δ�.
	BOOL	_stdcall	IsLoop(void);		// ���� �÷��� �� �Ǿ��ִ°�.?

	DWORD	_stdcall	AddRef();
	DWORD	_stdcall	Release();


//	CSoundEffect();
//	virtual ~CSoundEffect();

};

#endif // !defined(AFX_SOUNDEFFECT_H__E0425AA5_73A9_4935_B47F_AB7B9B83C401__INCLUDED_)
