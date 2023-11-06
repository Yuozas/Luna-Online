// SoundEffectFile.h: interface for the CSoundEffectFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDEFFECTFILE_H__6A3B42AB_3D76_4A0F_9806_A3EC3C709FDE__INCLUDED_)
#define AFX_SOUNDEFFECTFILE_H__6A3B42AB_3D76_4A0F_9806_A3EC3C709FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMilesSoundLib;

class CSoundEffectFile  
{
private:
	char	m_szFileName[MAX_PATH];
	DWORD	m_dwRefCount;

	void*	m_pWaveData;

	void*	m_hHashHandle;

	DWORD	m_dwSoundLibIndex;		// CMilesSoundLib ������ �� Ŭ������ �ε���.

	CMilesSoundLib*		m_pSoundLib;
public:

	SOUND_ERROR_CODE Create( char* szFileName, void* hHashHandle, DWORD dwIndex, CMilesSoundLib* pSoundLib);			// ������.
	void Delete();// �Ҹ���. Release�ϴ� ���� ���۷��� ī���Ͱ� 0�̸� �ܺο��� ȣ��ȴ�.

	void*	GetHashIndex(void)	{if( (DWORD)m_hHashHandle == 0xffffffff) _asm {int 3}	return m_hHashHandle;	}
	DWORD	GetSoundLibIndex(void) { return	m_dwSoundLibIndex; }

	void*	GetWaveBuffer(void){ return m_pWaveData;}
//	DWORD	GetWaveBufferLength(void){ return m_dwFileSize;}

	DWORD	GetIndex(void) { return m_dwSoundLibIndex; }
	DWORD	GetRef(void){return	m_dwRefCount;}

	DWORD AddRef();
	DWORD Release();


/*
	CSoundEffectFile();
	virtual ~CSoundEffectFile();
*/
};

#endif // !defined(AFX_SOUNDEFFECTFILE_H__6A3B42AB_3D76_4A0F_9806_A3EC3C709FDE__INCLUDED_)
