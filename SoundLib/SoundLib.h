
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOUNDLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOUNDLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
/*
	�޼� ��ǥ��.
	x�� ������, y�� ��, z�� ���̶� �����Ѵ�.
*/
#ifndef	_SoundLib_h_
#define	_SoundLib_h_


#ifdef SOUNDLIB_EXPORTS
#define SOUNDLIB_API extern "C" __declspec(dllexport)
#else
#define SOUNDLIB_API extern "C" __declspec(dllimport)
#endif

#include <windows.h>
#include "../4DyuchiGRX_common/typedef.h"


enum	SOUND_3D_DRIVER
{
	SOUND_DRIVER_TYPE_NONE,
	SOUND_DRIVER_TYPE_EAX3,
	SOUND_DRIVER_TYPE_EAX2,
	SOUND_DRIVER_TYPE_EAX1,
	SOUND_DRIVER_TYPE_DS3DHW,
	SOUND_DRIVER_TYPE_DS3DSW,
	SOUND_DRIVER_TYPE_MILES,
	SOUND_DRIVER_TYPE_DOLBY
};

enum	SOUND_SPEAKER_TYPE
{
	SOUND_SPEAKER_TYPE_STEREO,			// �Ϲ����� 2ü��.
	SOUND_SPEAKER_TYPE_HEADPHONE,		// �����, ����ī���� ������ ���� ����� �������� �۵��Ѵ�.
	SOUND_SPEAKER_TYPE_4CH,				// 4ü�� �����Ŵ�.
	SOUND_SPEAKER_TYPE_6CH,				// 5.1ü�� �����Ŵ�.
	SOUND_SPEAKER_TYPE_8CH,				// 7.1ü�� �����Ŵ�. ���ε���7.1 ��� �����ȴ�.
	SOUND_SPEAKER_TYPE_UNKNOWN			// miles���� ���������̶��� �����ϴµ� ���� ��. 
};

typedef		void*	SOUND_FILE_HANDLE;

// IMilesSoundLib::GetLastError �� ���ϵǴ� ������.
enum SOUND_ERROR_CODE
{
	SOUND_ERROR_NOERROR,
	SOUND_ERROR_ALREADY_INITIALIZED,
	SOUND_ERROR_INVALID_ARGUMENT,
	SOUND_ERROR_MILES_STARTUP_FAILED,
	SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,			// Init�� ���� ī�尡 ���� ��� �� �޽����� ���� Ƣ��´�.
	SOUND_ERROR_3DDRIVER_STARTUP_FAILED,			// Init�� 3D ����̹��� ��ã���� ������ ����. �̷� ��찡 ������?
	SOUND_ERROR_BGMDRIVER_NOT_LOADED,
	SOUND_ERROR_UNABLE_OPEN_FILE,
	SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
	SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
};

struct	ISoundEffect
{
	virtual	void	_stdcall	Play( BOOL bLoop)=0;
	/*
		���� ���.
		
		BOOL	bLoop     �̰��� TRUE�� ������ ����.

		������ ��ġ, ������ ����, ������ max,min�� �������� �Ҹ��� ����Ѵ�.
		���������δ� Play �ϴ� ���� Miles�� ���ð� ������ �����Ű�� start �Ѵ�.
	*/
	virtual void	_stdcall	Stop(void)=0;
	/*
		�Ҹ��� ������Ų��.
		���������δ� Stop �ϴ� ���� �Ҹ��� end ��Ű�� Miles�� ������ ������ �Ѵ�.
	*/

	virtual	void	_stdcall	SetMaxMinDistance( float fMaxDist, float fMinDist)=0;
	/*
		�Ҹ��� �鸮�� �ִ� �Ÿ�, �ּ� �Ÿ��� �ִ´�.

		float fMaxDist			// �ִ� �Ÿ�. 
		float fMinDist			// �ּ� �Ÿ�. �Ÿ� 0���� �� �Ÿ������� ������ �پ���� �ʴ´�.

		fMinDist�� fMaxDist�� ���θ� �� �ϰų� ���������� �޾�鼭 ���� �ؾ��� ��.
	*/

	virtual	void	_stdcall	SetPosition( VECTOR3* pPosition)=0;
	/*
		����Ʈ�� ��ġ�� �����Ѵ�.

		VECTOR3* pPosition			���忡���� ����Ʈ�� ��ǥ�� �����Ѵ�.

		SetPosition�� �� ���, IMilesSoundLib::SetListener ���� ������ ī�޶� �����ؼ� ī�޶� ������� ��ġ�� ���� �Ѵ�.
	*/

	virtual	void	_stdcall	SetRelativePosition( VECTOR3* pPosition)=0;
	/*
		����Ʈ�� ����� ��ġ�� �����Ѵ�.

		VECTOR3* pPosition			ī�޶󿡼��� ��� ��ġ.

		SetPosition()�� �ٸ��� �̰��� ī�޶� 0,0,0�� ��ġ��Ų ���·� �����ϰ� �� ���밪�� �����ϴ� ȿ���� �ش�.
	*/

	virtual	void	_stdcall	SetVolume( float fVolume)=0;
	/*
		������ �����Ѵ�.

		float fVolume			0.0f ���� 1.0f ���� ������ ������ �� �ִ�.
	*/
	virtual	float	_stdcall	GetVolume(void)=0;
	/*
		������ ��´�.
	*/

	virtual	BOOL	_stdcall	IsAlive(void)=0;
	/*
		���尡 ����ǰ� sample�� ��� �ִ°��� �����Ѵ�.
	*/

	virtual	BOOL	_stdcall	IsLoop(void)=0;
	/*
		���� �÷��� �� �Ǿ��ִ°�.?
	*/

	virtual	DWORD	_stdcall	AddRef()=0;
	virtual	DWORD	_stdcall	Release()=0;
	/*
		���۷��� ī���͸� ����.
		�Ϲ������� Release() �ϴ� ���� ���۷���ī���Ͱ� 0�� �Ǹ鼭 �޸𸮿��� �������.
	*/
};

struct	IMilesSoundLib
{
	virtual	SOUND_ERROR_CODE _stdcall Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware) = 0;// ���� �޴��� �ʱ�ȭ.
	/*
		IMilesSoundLib�� �ʱ�ȭ �Ѵ�.

		DWORD	dwMaxSoundFileCount�� �ִ��� �÷� ���� �� �ִ� wav, mp3������ ����,
		DWORD	dwMaxSoundEffectCount�� ���� ��ü�� �Ҹ��� �� �Ҹ� ��ü �ִ� ����.
		char*	szMilesPass�� ����� ����ִ� ���丮 �⺻������ "redist"������ "miles"�� �ű��..
		BOOL	bTryEAX�� 3D ����̹��� ���� �� eax�� �õ� �ϴ°� ���θ� �˸���.
		BOOL	bForceSoftware�� ����Ʈ�������̹��� ������ �õ��Ѵ�. ��κ��� ��� ������ ���̰�, �̶� bTryEAX�� ���õȴ�.
		���ϰ��� �� ���� ���� �����ϸ� SOUND_ERROR_NOERROR,
		Init�� �ι�° �Ǵ� ��� SOUND_ERROR_ALREADY_INITIALIZED,
		���� ���ڰ� �̻��� ��� SOUND_ERROR_INVALID_ARGUMENT,
		������ �ʱ�ȭ�� �����ϸ� SOUND_ERROR_MILES_STARTUP_FAILED,
		BGM����̹��� ã�� ���� ���, ����ī�尡 ���ٸ� SOUND_ERROR_BGMDRIVER_STARTUP_FAILED,
		3D ���� ����̹��� �ø��� ���ϸ� SOUND_ERROR_3DDRIVER_STARTUP_FAILED
		�� �ʾ��.
		���� ����ī�尡 ������� �� ����� SOUND_ERROR_BGMDRIVER_STARTUP_FAILED�� ó�����ְ�,
		������ ������ ���α׷��Ӹ� ã����. -_-;
	*/

	virtual	SOUND_3D_DRIVER		_stdcall	Get3DDriver(void)=0;
	/*
		���� �����ִ� 3D ����̹��� �����Ѵ�.
	*/

	virtual	void				_stdcall	SetListener( VECTOR3* pPosition, VECTOR3* pAngle)=0;
	/*
		û���� ��ġ�� ���� ���� �����Ѵ�.
		VECTOR3* pPosition		 û���� ��ġ.
		VECTOR3* pAngle			û�ڰ� ���� ����, radius �����̴�.

		�� ���� ������ ������ �ٸ� ISoundEffect�� SetPosition�ϱ� ���� ���� ���� �Ǵ� ���� �����ϴ�.
	*/

	virtual	SOUND_SPEAKER_TYPE	_stdcall Get3DSpeakerType(void)=0;
	/*
		�����ִ� 3D driver�� ����ϴ� ����Ŀ ��ġ�� ���¸� �˾Ƴ���.
		�̴� ������/���� �� ��������� �ִ� ����Ŀ ��ġ�� ������.
	*/

	virtual	void				_stdcall Set3DSpeakerType( SOUND_SPEAKER_TYPE Type)=0;
	/*
		3D driver�� ����ϴ� ����Ŀ Ÿ���� ������ �����Ѵ�.
	*/

	// BGM
	virtual	SOUND_ERROR_CODE _stdcall PlayBGM( char* szBGMFileName)=0;
	/*
		szBGMFileName���� ��õ� ������ ��׶���� �÷��� �Ѵ�.
		
		char* szBGMFileName		���� BGM���� �̸��� �ִ´�. .wav, .mp3���� �����ϴ�.

		������ ��� SOUND_ERROR_NOERROR,
		Init�� �������� ���� ��� SOUND_ERROR_BGMDRIVER_NOT_LOADED �� �����ϰ� �ƹ����� ���Ѵ�.
		���� �̸��� �߸����� ��� SOUND_ERROR_UNABLE_OPEN_FILE
		�� �����Ѵ�.

	*/

	virtual	void			 _stdcall StopBGM(void)=0;
	/*
		�÷��� �ǰ� �ִ� BGM�� �����Ѵ�.
		Init�� �������� ���� ���¿��� ȣ��ǵ� ��� ����. 
	*/

	virtual void			_stdcall SetBGMVolume( float fVolume)=0;
	/*
	 *	BGM�� ������ �����Ѵ�. ������ 1.0f ���� 0.0f����.
	 */

	// Sound Effect File
	virtual SOUND_ERROR_CODE _stdcall CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char* szFileName)=0;
	/*
		���� ���Ͽ� ���� �ڵ��� �ϳ� �����. �������� �ӵ��� ���� ���� ����Ʈ�� ���� �̸����� �ٷ� �������� �����Ƿ�
		���� �ڵ��� �ϳ� ����� �� �ڵ�� ���� ����Ʈ�� �����.

		SOUND_FILE_HANDLE* pOutHandle		���� ���� �ڵ��� ���� ������.
		char* szFileName					�Ҵ��� ���� �̸�.

		������ ��� SOUND_ERROR_NOERROR,
		BGM����̹��� �ö��� ���� ���(����ī�尡 ���� ��� ��..) SOUND_ERROR_BGMDRIVER_NOT_LOADED,
		�־��� ���ڿ� ������ ���� ��� SOUND_ERROR_INVALID_ARGUMENT,
		�ִ� ���� ��ü ������ �ʰ��� ��� SOUND_ERROR_OUT_OF_MAX_FILE_COUNT,
		�־��� ������ ���� ������ ��� SOUND_ERROR_UNABLE_OPEN_FILE
		�� �����Ѵ�.

		���������� ���ҽ� ������ �ǹǷ� ���� �����̸��� ���� Creat�� �ϸ� �̸� �߱� �Ǿ��� �ڵ��� �����ϰ�,
		���۷��� ī���Ͱ� 1 �ö󰣴�.
	*/

	virtual	void			 _stdcall ReleaseSoundEffectFile( SOUND_FILE_HANDLE hFile)=0;
	/*
		hFile�� �־��� ������ ������ �Ѵ�.
		
		�ϳ��� ���Ͽ� ���� �ڵ��� ������ �߱޵� ���̹Ƿ�, ���������� hFile�� ���۷��� ī���͸� �ϳ� ����߸���.
		���۷��� ī���Ͱ� 0�� �Ǵ� ���� ���� �޸𸮿��� ��������.
	*/

	virtual	SOUND_ERROR_CODE _stdcall CreateSoundEffect( ISoundEffect** ppSoundEffect, SOUND_FILE_HANDLE hSound)=0;
	/*
		hSound ���� �ڵ��� �÷��� �ϴ� ���� ��ü�� �����.
		����� ���� �÷��̰� �Ǵ� ���� �ƴϰ� ���� �� ���� �������̽��� Play ����� �Ѵ�.

		ISoundEffect** ppSoundEffect		�������̽� ������.
		SOUND_FILE_HANDLE hSound			������� ������ ���� �ڵ�.

		�����ϸ� SOUND_ERROR_NOERROR,
		���ڷ� �־��� hSound�� ���� ���̸� SOUND_ERROR_INVALID_ARGUMENT,
		Init�� �־��� �ִ� ����Ʈ ������ �ʰ��ϸ� SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT
		�� �����Ѵ�.

		���� ISoundEffect ��ü�� ���ﶧ ISoundEffect->Release()�� ����Ѵ�.
	*/

	virtual	void			_stdcall	Run()=0;

	virtual	DWORD			_stdcall	GetAllocatedSoundEffectCount()=0;
	/*
		���� �����ִ� ���� ����Ʈ�� ������ �����Ѵ�.

		����׿�.
	*/
	virtual	DWORD			_stdcall	GetAllocatedSoundFileCount()=0;
	/*
		���� �����ִ� ���� ������ ������ �����Ѵ�.

		����׿�.
	*/

	
	virtual	DWORD			_stdcall	ReleaseAllSoundEffect(void)=0;
	/*
		�����ִ� ��� Effect�� �����.

		�̽������� ������ ���� ���� Sound Effect�� ������ ���ϵȴ�.

		�����ϸ� �� �Լ� ���� ���� �׶��׶� �ϳ��ϳ� ���� ������ ���� ��õ�Ѵ�.
	*/

	virtual	DWORD			_stdcall	ReleaseAllSoundFile(void)=0;
	/*
		�����ִ� ��� File�� �����.

		�̽������� ������ ���� ���� Sound Effect File�� ������ ���ϵȴ�.

		�����ϸ� �� �Լ� ���� ���� �׶��׶� �ϳ��ϳ� ���� ������ ���� ��õ�Ѵ�.
	*/

	virtual	void _stdcall Delete() = 0;
	/*
		ISoundLib�� �Ҹ���.

		���ҽ��� ����� �������� �������¿��� ȣ��Ǹ� ���ù����� ���ͷ�Ʈ���� �ɸ� ���̸�.
	*/

	virtual	void _stdcall GetErrorString( char** pszErrorString, SOUND_ERROR_CODE)=0;
	/*
		SOUND_ERROR_NOERROR ���� �������� �޾����� �װ͵��� ���ڿ��� ġȯ���ش�.
		������ �Լ�.
	*/

	virtual	void	_stdcall	SetMaxVoiceCount( DWORD dwMaxVoice)=0;
	virtual	DWORD	_stdcall	GetMaxVoiceCount()=0;

};


SOUNDLIB_API BOOL CreateSoundLib( IMilesSoundLib** ppISoundLib);



#endif