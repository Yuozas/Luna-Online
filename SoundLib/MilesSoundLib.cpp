// MilesSoundLib.cpp: implementation of the CMilesSoundLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MilesSoundLib.h"
#include "SoundLibErrorString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// vtable ī�ǿ� ����.
CSoundEffect	g_DummyEffect;

/*
	���� ��Ʈ��.
	SoundLib.h �� �����ڵ�� ������ ��ġ�ϵ��� ��������.
*/
char*	g_pszErrorString[]	=	{
	"SOUND_ERROR_NOERROR",
	"SOUND_ERROR_ALREADY_INITIALIZED",
	"SOUND_ERROR_INVALID_ARGUMENT",
	"SOUND_ERROR_MILES_STARTUP_FAILED",
	"SOUND_ERROR_BGMDRIVER_STARTUP_FAILED",
	"SOUND_ERROR_3DDRIVER_STARTUP_FAILED",
	"SOUND_ERROR_BGMDRIVER_NOT_LOADED",
	"SOUND_ERROR_UNABLE_OPEN_FILE",
	"SOUND_ERROR_OUT_OF_MAX_FILE_COUNT",
	"SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT"
};

CMilesSoundLib*		g_pSoundLib	=	0;

  
CMilesSoundLib::CMilesSoundLib()
{
	// miles�� �ö󰬴°�? ������ �� �÷��� üũ�ؼ� ���ܻ�Ȳ�� ���.
	m_bMilesStartUp				=	FALSE;

	// miles driver. �ϴ� �̳��� �������� 3d ����̹��� �����ȴ�.
	m_BGMDriver					=	NULL;
	m_BGMHandle					=	NULL;
	m_fBGMVolume				=	0.5f;
	
	// miles 3d driver
	m_3DDriver					=	NULL;
	m_dw3DDriverType			=	NULL;

	// sound effect file
	m_dwMaxSoundFileCount		=	0;
	m_dwSoundFileCount			=	0;
	m_ppSoundFileBuffer			=	NULL;
	m_hSoundFileIndexCreator	=	0;
	m_hSoundFileHash			=	0;

	//memorypool
	m_hSoundFileMemoryPool		=	0;

	// sound effect
	m_dwSoundEffectCount		=	0;				// ���� ���� ����.
	m_dwMaxSoundEffectCount		=	0;				// �ִ� ���� ����.
	m_ppSoundEffectBuffer		=	0;				// ������ CSoundEffect ������ ����.
	m_hSoundEffectIndexCreator	=	0;				// �����͹��ۿ� �ε��� ũ��������.

	//memorypool
	m_hSoundEffectMemoryPool	=	0;

	g_pSoundLib			=	this;
}

CMilesSoundLib::~CMilesSoundLib()
{

}

// ���� �޴��� �ʱ�ȭ.
SOUND_ERROR_CODE _stdcall CMilesSoundLib::Init(DWORD dwMaxSoundFileCount, DWORD dwMaxSoundEffectCount, char* szMilesPass, BOOL bTryEAX, BOOL bForceSoftware)
{
	BOOL	bIndexCreator;

//	bTryEAX	=	TRUE;
//	bForceSoftware	=	TRUE;
#ifdef	_DEBUG
	char	szErrorString[1024];
	wsprintf( szErrorString, "CMilesSoundLib::Init( dwMaxSoundFileCount = %d, dwMaxSoundEffectCount = %d, szMilesPass = %s, bTryEAX = %d", dwMaxSoundFileCount, dwMaxSoundEffectCount, szMilesPass, bTryEAX);
	OutputDebugString( szErrorString);
#endif

	// �̹� �ε�Ǿ��ֳ�?
	if( m_bMilesStartUp == TRUE)
	{
		return	SOUND_ERROR_ALREADY_INITIALIZED;
	}

	// ���ڰ� �̻��Ѱ�?
	if( dwMaxSoundFileCount == 0 || dwMaxSoundEffectCount == 0 || szMilesPass == 0)
	{
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}

	// ������ �ʱ�ȭ.
	char*	szStartUpError	=	0;
	char*	szReturn		=	AIL_set_redist_directory(szMilesPass);
	S32		mssStartUp		=	AIL_startup();
	if( mssStartUp == 0)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_MILES_STARTUP_FAILED;
	}
	m_bMilesStartUp		=	TRUE;

	// ������� �ʱ�ȭ.
	BOOL	bStartupBGMDriver	=	StartupBGMDriver();	
	if( bStartupBGMDriver == FALSE)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_BGMDRIVER_STARTUP_FAILED;	// ���� ī�尡 ���� ��� �ϴ� �����Ѵ�.
	}

	// 3DDriver �ʱ�ȭ.
	BOOL	bStartup3DDriver	=	Startup3DDriver( bTryEAX, bForceSoftware);
	if( bStartup3DDriver == FALSE)
	{
		szStartUpError	=	AIL_last_error();
		return	SOUND_ERROR_3DDRIVER_STARTUP_FAILED;	// �̰� �����ϴ� ��찡 ������.?
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// sound file �ʱ�ȭ.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	m_dwMaxSoundFileCount		=	dwMaxSoundFileCount;
	m_ppSoundFileBuffer			=	new	CSoundEffectFile*[m_dwMaxSoundFileCount];
	memset( m_ppSoundFileBuffer, 0, sizeof(CSoundEffectFile*) * m_dwMaxSoundFileCount);
	m_dwSoundFileCount			=	0;
	// �ε��� ũ��������.
	m_hSoundFileIndexCreator		=	ICCreate();
	bIndexCreator	=	ICInitialize( m_hSoundFileIndexCreator, m_dwMaxSoundFileCount);
	if( bIndexCreator == FALSE)
	{
		_asm int 3;			// critical
	}
	// �ؽ�.
	m_hSoundFileHash	=	VBHCreate();
	BOOL	bHash	=	VBHInitialize( m_hSoundFileHash, m_dwMaxSoundFileCount, MAX_PATH, m_dwMaxSoundFileCount);
#ifdef	_DEBUG
	if( bHash == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	����Ʈ ���Ͽ� �ؽ� ���� ����.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create Effect file hash.\n");
		_asm int 3;
	}
#endif

	// ���Ͽ� �޸� Ǯ.
	//memorypool
	m_hSoundFileMemoryPool	=	CreateStaticMemoryPool();
	BOOL	bFilePool		=	InitializeStaticMemoryPool( m_hSoundFileMemoryPool, sizeof( CSoundEffectFile), 0, m_dwMaxSoundFileCount);
#ifdef	_DEBUG
	if( bFilePool == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	����Ʈ ���Ͽ� ������ƽ �޸� Ǯ ���� ����.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create static memory pool for effect file.\n");
		_asm int 3;
	}
#endif
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// sound effect �ʱ�ȭ.
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	// ������� ���� ������� ��������Ʈ�� ����Ʈ�� �����Ѵ�.
	m_dwMaxSoundEffectCount		=	dwMaxSoundEffectCount;
	m_ppSoundEffectBuffer	=	new CSoundEffect*[m_dwMaxSoundEffectCount];
	memset( m_ppSoundEffectBuffer, 0, sizeof( CSoundEffect*)* m_dwMaxSoundEffectCount);
	m_dwSoundEffectCount			=	0;
	m_hSoundEffectIndexCreator	=	ICCreate();
	bIndexCreator	=	ICInitialize( m_hSoundEffectIndexCreator, m_dwMaxSoundEffectCount);
#ifdef	_DEBUG
	if( bIndexCreator == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	����Ʈ�� �ε��� ũ�������� ���� ����.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create index creator for effect.\n");
		_asm int 3;
	}
#endif

	m_hSoundEffectMemoryPool	=	CreateStaticMemoryPool();
	BOOL	bEffectPool			=	InitializeStaticMemoryPool( m_hSoundEffectMemoryPool, sizeof(CSoundEffect), 0, m_dwMaxSoundEffectCount);
#ifdef	_DEBUG
	if( bEffectPool == FALSE)
	{
//		OutputDebugString("CMilesSoundLib::Init()	����Ʈ�� ������ƽ �޸� Ǯ ���� ����.\n");
		OutputDebugString("CMilesSoundLib::Init()	Failed to create static memory pool for effect.\n");
		_asm int 3;
	}
#endif

	
	this->AllocSamplePool();


	return	SOUND_ERROR_NOERROR;
}
/*
DWORD _stdcall CMilesSoundLib::GetAllocatedEffectCount(void)
{
//	

	return	0xffffffff;
}
*/

SOUND_3D_DRIVER _stdcall CMilesSoundLib::Get3DDriver(void)
{
	return	(SOUND_3D_DRIVER)m_dw3DDriverType;
}

void _stdcall CMilesSoundLib::SetListener( VECTOR3* pPosition, VECTOR3* pAngle)
{
#ifdef	_DEBUG
	if( pPosition == 0 || pAngle == 0)
		_asm int 3;
#endif

	m_vListenerPos	=	*pPosition;
	m_vListenerAng	=	*pAngle;


	// �������� ���࿭�� �ٷ� �����.

	// ���������� �� ����,
	VECTOR3		PosRev	=	m_vListenerPos;
	VECTOR3_MULEQU_FLOAT( &PosRev, -1.0f);
	MATRIX4		MoveRev;
	TranslateMatrix( &MoveRev, &PosRev);
//	TranslateMatrix( &m_mListener, &PosRev);
//	MatrixMultiply2( &m_mListener , MATRIX4* mat1, MATRIX4* mat2);

	// ���������� ������.


	// z, x, y�� �������� -y, -x, -z�� ������.
	MATRIX4		Rotate, RotateY, RotateX, RotateZ;
	SetRotationYMatrix( &RotateY, -1.0f * m_vListenerAng.y);
	SetRotationXMatrix( &RotateX, -1.0f * m_vListenerAng.x);
	SetRotationZMatrix( &RotateZ, -1.0f * m_vListenerAng.z);

	MatrixMultiply3( &Rotate, &RotateY, &RotateX, &RotateZ);

	MatrixMultiply2( &m_mListenerRev, &MoveRev, &Rotate);



//	SetRotationXMatrix( &m_mListenerRev, -1.0f * m_vListenerAng.y);
	

_asm nop



}

SOUND_SPEAKER_TYPE	_stdcall CMilesSoundLib::Get3DSpeakerType(void)
{
//	Check();

	S32	SpeakerType	=	AIL_3D_speaker_type( m_3DDriver );
	switch( SpeakerType)
	{
	case	AIL_3D_2_SPEAKER:
		return	SOUND_SPEAKER_TYPE_STEREO;			// �Ϲ����� 2ü��.
	case	AIL_3D_HEADPHONE:
		return	SOUND_SPEAKER_TYPE_HEADPHONE;		// �����, ����ī���� ������ ���� ����� �������� �۵��Ѵ�.
	case	AIL_3D_4_SPEAKER:
		return	SOUND_SPEAKER_TYPE_4CH;				// 4ü�� �����Ŵ�.
	case	AIL_3D_51_SPEAKER:
		return	SOUND_SPEAKER_TYPE_6CH;				// 5.1ü�� �����Ŵ�.
	case	AIL_3D_71_SPEAKER:
		return	SOUND_SPEAKER_TYPE_8CH;				// 7.1ü�� �����Ŵ�. ���ε���7.1 ��� �����ȴ�.
	}
	return	SOUND_SPEAKER_TYPE_UNKNOWN;

}

void _stdcall CMilesSoundLib::Set3DSpeakerType( SOUND_SPEAKER_TYPE Type)
{
//	Check();

	switch( Type)
	{
	case	SOUND_SPEAKER_TYPE_STEREO:			// �Ϲ����� 2ü��.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_2_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_HEADPHONE:		// �����, ����ī���� ������ ���� ����� �������� �۵��Ѵ�.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_HEADPHONE);
		break;
	case	SOUND_SPEAKER_TYPE_4CH:				// 4ü�� �����Ŵ�.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_4_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_6CH:				// 5.1ü�� �����Ŵ�.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_51_SPEAKER);
		break;
	case	SOUND_SPEAKER_TYPE_8CH:				// 7.1ü�� �����Ŵ�. ���ε���7.1 ��� �����ȴ�.
		AIL_set_3D_speaker_type(  m_3DDriver,  AIL_3D_71_SPEAKER);
		break;
	}

}
DWORD _stdcall CMilesSoundLib::ReleaseAllSoundEffect(void)
{
//	Check();

	// ���۸� ���鼭 ���� ����Ʈ���� �����.
	if( m_dwSoundEffectCount == 0)
		return	0;


//	this->Run();

	DWORD	i;
	DWORD	dwCount = 0;
	DWORD	dwRemainedCount	=	m_dwSoundEffectCount;
	for( i = 0; i < m_dwMaxSoundEffectCount; i++)
	{
		if( m_ppSoundEffectBuffer[i] )
		{
			while( m_ppSoundEffectBuffer[i] )
			{
				m_ppSoundEffectBuffer[i]->Release();
			}
			dwCount++;
			dwRemainedCount--;
		}
	}

	if( dwRemainedCount != 0)
		_asm int 3;

	return	dwCount;
}

DWORD _stdcall CMilesSoundLib::ReleaseAllSoundFile(void)
{
//	Check();

	if( m_dwSoundFileCount == 0)
		return	0;

//	Check();

	DWORD	i;
	DWORD	dwCount	=	0;
	DWORD	dwRemainedCount	=	m_dwSoundFileCount;
	for( i = m_dwMaxSoundFileCount - 1; i < m_dwMaxSoundFileCount; i--)
	{

//		Check();

		if( m_ppSoundFileBuffer[i])
		{
//		Check();

			while( m_ppSoundFileBuffer[i])
			{
//		Check();
	
				this->ReleaseSoundEffectFile( m_ppSoundFileBuffer[i]);
			}
			dwCount++;
			dwRemainedCount--;

			if( dwRemainedCount != m_dwSoundFileCount)
				_asm int 3;
		}
		else
			_asm nop;
	}

	if( dwRemainedCount != 0)
		_asm int 3;

	return	dwCount;
}

// ���� �޴��� ����ȭ.
void _stdcall CMilesSoundLib::Delete()
{
//		Check();

	if( m_ppSoundFileBuffer)
	{
		// ���� ����Ʈ�� ������ �ִ°� üũ�Ѵ�.
		if( m_dwSoundEffectCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::SoundEffect�� �� �������� �ʾҴ�.\n");
			OutputDebugString( "IMilesSoundLib::SoundEffect resource did not fully released.\n");
			_asm int 3;
		}

		// ���� ����Ʈ ������ ����.
		delete	m_ppSoundEffectBuffer;
		m_ppSoundEffectBuffer	=	0;

		// ���� ����Ʈ �ε���ũ�������Ϳ� ���� ���ҳ�?
		DWORD	dwRemainedSoundEffectCount	=	ICGetIndexNum( m_hSoundEffectIndexCreator);
		if( dwRemainedSoundEffectCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundEffectIndexCreator ���� ���� �ִ� �ε������� �� ������ �ȉ��. ����� �θ��ÿ�");
			OutputDebugString( "IMilesSoundLib::m_hSoundEffectIndexCreator Index did not fully released. internal error. call me. -_-;.\n");
			_asm int 3;
		}
		// ���� ����Ʈ ������ �ε���ũ�������� �����.
		ICRelease( m_hSoundEffectIndexCreator);
		m_hSoundEffectIndexCreator	=	0;
		m_dwMaxSoundEffectCount		=	0;
		m_dwSoundEffectCount		=	0;
/*
		// eos�ݹ�� �ؽ� �����.
		QBHRelease( m_hSoundEffectHash);
		m_hSoundEffectHash	=	0;
*/
		// ���� ����Ʈ�� ������ƽ �޸� Ǯ�� ������.
		ReleaseStaticMemoryPool( m_hSoundEffectMemoryPool);
		m_hSoundEffectMemoryPool	=	0;

		// ���� ���� ������ �ִ°� üũ�Ѵ�.
		if( m_dwSoundFileCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::Sound Effect File�� ��� �������� �ʾҴ�.");
			OutputDebugString( "IMilesSoundLib::Sound Effect File did not fully released.\n");
			_asm int 3;
		}
		// ���� ���� ������ ���۸� �����.
		delete	m_ppSoundFileBuffer;
		m_ppSoundFileBuffer				=	0;
		if( m_hSoundFileIndexCreator == 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator�� ���̴�. ����� �θ��ÿ�.\n");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator is NULL. call me -_-;\n");
			_asm int 3;// ��¼����. =_=;
		}
		// ���� ���Ͽ� �ε��� �޴������� ������ �� ���󰬳� �˻�.
		DWORD dwRemainedSoundFileCount	=	ICGetIndexNum( m_hSoundFileIndexCreator);
		if( dwRemainedSoundFileCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator ���� ���� �ִ� �ε������� �� ������ �ȉ��. ����� �θ��ÿ�");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileIndexCreator index did not fully released. call me -_-;\n");
			_asm int 3;
		}
		// ���� ���Ͽ� �ε��� �޴����� ������.
		ICRelease( m_hSoundFileIndexCreator);
		m_hSoundFileIndexCreator	=	0;

		if( m_hSoundFileHash == 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash �� ���̴�. ����� �θ��ÿ�.");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash is NULL. call me -_-;\n");
			_asm int 3;
		}
		// ���� ���Ͽ� �ؽ��� ������ �˻�.
		DWORD*	pdwItemList	=	new	DWORD[m_dwMaxSoundFileCount];
		DWORD	dwRemainedSoundFileHashCount	=	VBHGetAllItem( m_hSoundFileHash, pdwItemList, m_dwMaxSoundFileCount);
		if( dwRemainedSoundFileHashCount != 0)
		{
//			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash ���� ���� �ִ� �������� �� �ȳ��ư���. ����� �θ��ÿ�.");
			OutputDebugString( "IMilesSoundLib::m_hSoundFileHash did not fully released. call me -_-;\n");
			_asm int 3;
		}
		delete[]	pdwItemList;
		// ���� ���Ͽ� �ؽ��� ������.
		VBHRelease( m_hSoundFileHash);
		m_hSoundFileHash	=	0;

		// ���� ���Ͽ� ������ƽ �޸� Ǯ�� ������.
		//memorypool
		ReleaseStaticMemoryPool( m_hSoundFileMemoryPool);
		m_hSoundFileMemoryPool		=	0;
	}

	ReleaseSamplePool();

	if( m_3DDriver)
	{
		Shutdown3DDriver();
		m_3DDriver	=	0;
	}

	if( m_BGMDriver)
	{
		ShutdownBGMDriver();
		m_BGMDriver	=	0;
	}

	if(m_bMilesStartUp)
	{
		AIL_shutdown();
		m_bMilesStartUp	=	FALSE;
	}

//	DeleteCriticalSection( &m_SampleCriticalSection);

	delete	this;

	return	;
}

void _stdcall CMilesSoundLib::GetErrorString( char** pszErrorString, SOUND_ERROR_CODE code)
{
	*pszErrorString	=	(g_pszErrorString[ code]);
}

BOOL CMilesSoundLib::StartupBGMDriver()
{
	if( m_BGMDriver != 0)
	{
		// �ι� ȣ�� ���.
		return	FALSE;
	}

	// �������� ���� ����ī�忡�� ������ ���� ��������.?
	// �ϴ� ���� ����̹��� ���ٸ� ���� �Ѵ�.
	HDIGDRIVER	bgmDriver	=	AIL_open_digital_driver( 48000, 16, 32, 0);
	if( bgmDriver	==	0)
	{
		return	FALSE;
	}

	m_BGMDriver	=	bgmDriver;


//lbFinishFindDriver:
	return	TRUE;

}

void CMilesSoundLib::ShutdownBGMDriver()
{
	if( m_BGMDriver == 0)
	{
		_asm int 3;
	}
	AIL_close_digital_driver(	m_BGMDriver);
	
}

SOUND_ERROR_CODE _stdcall CMilesSoundLib::PlayBGM( char *szBGMFileName)
{

//	Check();
	char* szLastError	=	0;
	if( m_BGMDriver == 0)
	{
		return	SOUND_ERROR_BGMDRIVER_NOT_LOADED;
	}

	if( szBGMFileName == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}
	
	size_t	fileNameLen	=	strlen( szBGMFileName);
	if( fileNameLen == 0)
	{
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}


	if( m_BGMHandle)
	{
		StopBGM();
	}

	m_BGMHandle	=	AIL_open_stream( m_BGMDriver, szBGMFileName, 0 );
	if( m_BGMHandle == NULL)
	{
		szLastError	=	AIL_last_error();
		return	SOUND_ERROR_UNABLE_OPEN_FILE;
	}

	AIL_set_stream_loop_count( m_BGMHandle, 0);

	this->SetBGMVolume( m_fBGMVolume);

	AIL_start_stream( m_BGMHandle);


	return	SOUND_ERROR_NOERROR;
}

void _stdcall CMilesSoundLib::StopBGM(void)
{
//		Check();
	if( m_BGMDriver == 0)
	{
		return;
	}
	AIL_close_stream( m_BGMHandle);
	m_BGMHandle	=	0;
}

void _stdcall CMilesSoundLib::SetBGMVolume( float fVolume)
{
//		Check();
	if( fVolume > 1.0f || fVolume < -1.0f)
	{
//		OutputDebugString("CMilesSoundLib::SetBGMVolume() ������ ���ġ ������ �Ѿ��.");
		OutputDebugString("CMilesSoundLib::SetBGMVolume() over hit the max volume.\n");
		_asm int 3;
	}

	if( !m_BGMHandle)
	{
//		OutputDebugString("BGM�� �÷��̵ǰ� ���� ���� ��Ȳ���� CMilesSoundLib::SetBGMVolume() �� ȣ��Ǿ���.");
//		_asm int 3;
		m_fBGMVolume	=	fVolume;
	}
	else
	{
		m_fBGMVolume	=	fVolume;
		AIL_set_stream_volume_pan( m_BGMHandle, fVolume, 0.5f );	// �ڵ�, ����, �� ��ġ(���� �߾�)
	}

}

BOOL CMilesSoundLib::Startup3DDriver(BOOL bTryEAX, BOOL bForceSoftware)
{
	HPROVIDER	driver	=	0;
	BOOL		bLoad	=	FALSE;

	if( bForceSoftware)
		goto	lbSoftware;

	if( bTryEAX == FALSE)
		goto	lbTryDSHardware;


	// EAX 3
	driver	=	Find3DDriver("Creative Labs EAX 3 (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX3;
			goto	lbFinish;
		}
	}

	// EAX 2
	driver	=	Find3DDriver("Creative Labs EAX 2 (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX2;
			goto	lbFinish;
		}
	}

	// EAX 1
	driver	=	Find3DDriver("Creative Labs EAX (TM)");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_EAX1;
			goto	lbFinish;
		}
	}

lbTryDSHardware:
	// DS3D hardware
	driver	=	Find3DDriver( "DirectSound3D Hardware Support");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DS3DHW;
			goto	lbFinish;
		}
	}

	// �� ������ �����Ŵ��� �����ȴ�.
	// �� �Ʒ����ʹ� 2����Ŀ ����.
lbSoftware:
	// MilesFast2D
	driver	=	Find3DDriver( "Miles Fast 2D Positional Audio");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_MILES;
			goto	lbFinish;
		}
	}

	// DS3D software
	driver	=	Find3DDriver( "DirectSound3D Software Emulation");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DS3DSW;
			goto	lbFinish;
		}
	}

	// Dolby
	driver	=	Find3DDriver( "Dolby Surround");
	if( driver != 0)
	{
		bLoad	=	Load3DDriver( driver);
		if( bLoad == TRUE)
		{
			m_dw3DDriverType		=	SOUND_DRIVER_TYPE_DOLBY;
			goto	lbFinish;
		}
	}


	// ������� ������ ����̹��� ã�µ� �����ߴٰ� ����.
	// ���� ī�尡 ���� ����� ��.
	m_dw3DDriverType	=	SOUND_DRIVER_TYPE_NONE;

	return	FALSE;




lbFinish:
 

	// Miles���� �䱸�ϴ� 3D�����ʸ� �����.



	_asm nop;
	return	TRUE;
	
}

SOUND_ERROR_CODE _stdcall CMilesSoundLib::CreateSoundEffectFile( SOUND_FILE_HANDLE* pOutHandle, char *szFileName)
{
//		Check();


	DWORD	dwResult	=	0xffffffff;
	BOOL	bResult;
#ifdef		_DEBUG
	if( m_BGMDriver	==	0)
	{
		*pOutHandle	=	0;
		return	SOUND_ERROR_BGMDRIVER_NOT_LOADED;
	}

	// Ȥ�� ���ΰ�? -_-;
	if( szFileName == 0 || pOutHandle == 0)
	{
		_asm int 3;
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}
#endif

	// ���� �̸� ����.
	int		iStrLen		=	lstrlen( szFileName);
#ifdef		_DEBUG
	if( iStrLen > MAX_PATH)
	{
		_asm int 3;
		return	SOUND_ERROR_INVALID_ARGUMENT;
	}
#endif

	*pOutHandle	=	0;

	CSoundEffectFile*	pFile	=	0;
	DWORD				dwNewIndex	=	0xffffffff;

	// �ؽ����� ã�´�.
	DWORD		dwHashIndex;
	DWORD		dwHashReturn	=	VBHSelect( m_hSoundFileHash, &dwHashIndex, 1, szFileName, iStrLen);
#ifdef		_DEBUG
	if( dwHashReturn >= 2)
		_asm int 3;
#endif

	if ( dwHashReturn)
	{
		// �ö�Դ� �����̴�.
		// �ش� ������ ���۷��� ī���� �ϳ� �ø���,
		m_ppSoundFileBuffer[dwHashIndex]->AddRef();

		// ���.
		*pOutHandle	=	(SOUND_FILE_HANDLE)m_ppSoundFileBuffer[dwHashIndex];
	}
	else
	{
		// ���� �ö���� ������ ���.
		// �ִ� ������ �ʰ����� �ʾҳ�?
		if( m_dwMaxSoundFileCount <= m_dwSoundFileCount)
		{
			return	SOUND_ERROR_OUT_OF_MAX_FILE_COUNT;
		}

		// �ε��� �Ҵ� �ް�,
		dwNewIndex		=	ICAllocIndex( m_hSoundFileIndexCreator) - 1;		// 0base
		// �ؽ��� �ְ�,
		void*	hHashHandle	=	VBHInsert( m_hSoundFileHash, dwNewIndex, szFileName, iStrLen);
		// �޸𸮿��� �Ҵ� �ް�.
		//memorypool
		pFile		=	(CSoundEffectFile*)LALAlloc( m_hSoundFileMemoryPool);
		// ����.
		dwResult	=	pFile->Create( szFileName, hHashHandle, dwNewIndex, this);
#ifdef			_DEBUG
		if( dwResult == SOUND_ERROR_INVALID_ARGUMENT)
			_asm int 3;				// �ߴ��� ����.
#endif
		if( dwResult == SOUND_ERROR_UNABLE_OPEN_FILE)
		{
			// ������ ���µ� �����ߴ�. 
			// ���� ��ü �����,
			memset( pFile, 0, sizeof(CSoundEffectFile));
			LALFree( m_hSoundFileMemoryPool, pFile);
			pFile	=	0;
			// �ؽ����� ������.
			bResult	=	VBHDelete( m_hSoundFileHash, hHashHandle);
			if( bResult == FALSE)
			{
				_asm int 3;
			}
			// �ε������� ������.
			ICFreeIndex( m_hSoundFileIndexCreator, dwNewIndex+1);

			return	SOUND_ERROR_UNABLE_OPEN_FILE;
		}
		// ������ �迭�� �ִ´�.
		m_ppSoundFileBuffer[dwNewIndex]	=	pFile;

		m_dwSoundFileCount++;

		// ���.
		*pOutHandle		=	(SOUND_FILE_HANDLE)pFile;
	}

//	Check();

	return	SOUND_ERROR_NOERROR;
}

DWORD CMilesSoundLib::GetAllocatedSoundEffectCount()
{
//		Check();
	return	m_dwSoundEffectCount;
}

DWORD CMilesSoundLib::GetAllocatedSoundFileCount()
{
//		Check();
	return	m_dwSoundFileCount;
}



void CMilesSoundLib::ReleaseSoundEffectFile(SOUND_FILE_HANDLE hFile)
{
		
	BOOL	bResult;
#ifdef	_DEBUG
		if( hFile == 0 || (DWORD)hFile == 0xcccccccc || (DWORD)hFile == 0xdddddddd || (DWORD)hFile == 0xcdcdcdcd)
		{
			_asm int 3;
			return	;
		}
		if( m_dwSoundFileCount == 0)
		{
			_asm int 3;
			return	;
		}
		bResult		=	IsValidFileHandle( hFile);
		if( bResult == FALSE)
			_asm int 3;

#endif

	

//	DWORD	dwIndex;

	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hFile;

	

	// dwRelease���� 0�̸� �޸𸮻󿡼� �����Ƿ� �׿����� ó���� �Ѵ�.
	DWORD	dwRelease		=	pFile->Release();

	return	;

}

/*
	Miles�� �ϳ��� sample�� �ϳ��� ds ��������۸� �ǹ��ϴ� ���̶� soundeffect �ϳ� ���鶧����
	�����ڵ� �ϳ� �߱� �޴°� ���� �ȵǴ� ���� �Ǿ���. -_-;
	�̰� ȣ�� �Ǹ� soundeffect�� ������ ���۸� �ѱ��,
	CSoundEffect::Play �� �Ǵ� ������ ������ �߱޹ް� �÷��̸� �Ѵ�.
*/
SOUND_ERROR_CODE _stdcall CMilesSoundLib::CreateSoundEffect(ISoundEffect **ppSoundEffect, SOUND_FILE_HANDLE hSound)
{
		
	BOOL	bResult;

#ifdef	_DEBUG
		if( hSound == 0 || (DWORD)hSound == 0xcccccccc || (DWORD)hSound == 0xdddddddd || (DWORD)hSound == 0xcdcdcdcd)
		{
			return	SOUND_ERROR_INVALID_ARGUMENT;
		}

		bResult		=	IsValidFileHandle( hSound);
		if( bResult == FALSE)
			return	SOUND_ERROR_INVALID_ARGUMENT;
#endif
	if( m_dwMaxSoundEffectCount <= m_dwSoundEffectCount)
	{
		// �� ������ �� á��.
		*ppSoundEffect	=	0;
		return	SOUND_ERROR_OUT_OF_MAX_EFFECT_COUNT;
	}

	// ���� �ڵ�
	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hSound;

	// ������ ������ �ε��� �ϳ� �Ҵ� �Ѵ�.
	DWORD	dwIndex	=	ICAllocIndex( m_hSoundEffectIndexCreator) - 1;

	// ���� ������Ʈ ����. �޸� �޴������� �����ϴ� ������ �ؾ��� ��.
	CSoundEffect*	pSound;
	pSound	=	(CSoundEffect*)LALAlloc( m_hSoundEffectMemoryPool);
	*((DWORD*)pSound)	=	*((DWORD*)(&g_DummyEffect));				// vtable ī��.
	pSound->Create( pFile,  m_3DDriver, this, dwIndex);
	// ������ ���ۿ��� �ְ�,
	m_ppSoundEffectBuffer[dwIndex]		=	pSound;

	m_dwSoundEffectCount++;

	*ppSoundEffect	=	pSound;

	return	SOUND_ERROR_NOERROR;
}

HPROVIDER CMilesSoundLib::Find3DDriver(const char *szDriverName)
{
	int	enumDriver;
	HPROENUM	next	=	HPROENUM_FIRST ;
	HPROVIDER	dest	=	0;
	char *name;
	int	iStrCmp;

lbStartFindDriver:

	enumDriver	=	AIL_enumerate_3D_providers( &next, &dest, &name);
	if( enumDriver == 0)
	{
		_asm nop;
		// ���̻� ���� ����̹��� ����. ã�� ����.
		return	0;
	}

	iStrCmp	=	strcmp( name, szDriverName);
	if( iStrCmp == 0)
	{
		return	dest;
		
		

	}
	goto	lbStartFindDriver;


}

BOOL CMilesSoundLib::Load3DDriver( HPROVIDER driver)
{
	char* szLastError	=	0;
	// ���̺귯�� �ø���. �ϴ� ���̷�Ʈ���� ����Ʈ���� ����̹�.
	U32	uiResult	=	AIL_open_3D_provider( driver);
	if( uiResult != 0)
	{
		szLastError	=	AIL_last_error();
		return	FALSE;
	}

	m_3DDriver	=	driver;

	return	TRUE;

}

void CMilesSoundLib::Shutdown3DDriver()
{
	if( m_3DDriver)
	{
		AIL_close_3D_provider( m_3DDriver);
		m_3DDriver	=	0;
		m_dw3DDriverType	=	SOUND_DRIVER_TYPE_NONE;
	}

}

// SounEffect�� Release�� ȣ�� �Ǿ����� SoundLib�� �뺸�ϱ� ���� ���δ�.
void CMilesSoundLib::DeletingSoundEffect( DWORD dwEffectIndex, CSoundEffect* pEffect)
{
		
	// �ϴ� ��.
#ifdef	_DEBUG
	if( dwEffectIndex	>=	m_dwMaxSoundEffectCount)
	{
		_asm int 3;
	}
	if( m_dwSoundEffectCount == 0)
	{
		_asm int 3;
	}
	if( pEffect == 0)
	{
		_asm int 3;
	}
	if( m_ppSoundEffectBuffer[dwEffectIndex] != pEffect)
	{
		_asm int 3;
	}
#endif

		LALFree( m_hSoundEffectMemoryPool, pEffect);

	// ���������� ���ۿ��� �����.
	m_ppSoundEffectBuffer[dwEffectIndex]	=	0;
	m_dwSoundEffectCount--;
	// �ε������� ���ش�.
	ICFreeIndex( m_hSoundEffectIndexCreator, dwEffectIndex+1);

}

/*
	���� ������ �ڵ��� �����ϴ��� ������ ���鼭 üũ�ϴ� ������ �Լ�.
*/
BOOL CMilesSoundLib::IsValidFileHandle(SOUND_FILE_HANDLE hSound)
{
		
	if( hSound == 0)
	{
		return	FALSE;
	}
	DWORD	i;
	BOOL	bFound	=	FALSE;
	for( i = 0; i < m_dwMaxSoundFileCount; i++)
	{
		if( m_ppSoundFileBuffer[i] == hSound)
		{
			return	TRUE;
		}
	}

	return	FALSE;
	
}


void CMilesSoundLib::SoundEffectSetPosition(VECTOR3 *pOutPos, CSoundEffect *pEffect, VECTOR3 *pPos)
{
		
	// ī�޶��� ���࿭�� ���� ���� pOutPos�� ����.
	TransformVector3_VPTR2( pOutPos, pPos, &m_mListenerRev, 1);

}


void _stdcall CMilesSoundLib::Run()
{
		_asm nop;			// ���� �ƹ��͵�...
//	m_dwMaxVoiceCount	=	30;

}

/*
void CMilesSoundLib::Check(void)
{

	DWORD	dwRef;
	DWORD	dwCount	=	0;
	int	i;
	for( i = 0; i < m_dwMaxSoundEffectCount; i++)
	{
		if( m_ppSoundEffectBuffer[i])
		{
			dwCount++;
		}
	}
	if( dwCount != m_dwSoundEffectCount)
		_asm int 3;

	dwCount	=	0;
	for( i = 0; i < m_dwMaxSoundFileCount; i++)
	{
		if( m_ppSoundFileBuffer[i])
		{
			dwRef	=	m_ppSoundFileBuffer[i]->GetRef();
			if( dwRef == 0)
				_asm nop;
			dwCount++;
		}
	}
	if( dwCount != m_dwSoundFileCount)
		_asm int 3;


}
*/

void CMilesSoundLib::DeleteSoundEffectFile(SOUND_FILE_HANDLE hFile)
{
		
	BOOL	bResult;
#ifdef	_DEBUG
		if( hFile == 0 || (DWORD)hFile == 0xcccccccc || (DWORD)hFile == 0xdddddddd || (DWORD)hFile == 0xcdcdcdcd)
		{
			_asm int 3;
			return	;
		}
		bResult		=	IsValidFileHandle( hFile);
		if( bResult == FALSE)
			_asm int 3;
#endif
	
	DWORD	dwIndex;

	CSoundEffectFile*	pFile	=	(CSoundEffectFile*)hFile;

	// dwRelease���� 0�̸� �޸𸮻󿡼� �����Ƿ� �׿����� ó���� �Ѵ�.
	DWORD	dwRelease		=	pFile->GetRef();
#ifdef		_DEBUG
	if( dwRelease != 0)
	{
		OutputDebugString("���� ����� �θ��ÿ�. -_-;");
		_asm int 3;
		return	;
	}
#endif
	
	// �ε��� ���ְ�, 
	dwIndex			=	pFile->GetSoundLibIndex();
	ICFreeIndex( m_hSoundFileIndexCreator, dwIndex+1);			// 0base
	m_ppSoundFileBuffer[dwIndex]	=	0;

	// �ؽ����� ���ְ�,
	void*	hHashIndex		=	pFile->GetHashIndex();
	VBHDelete( m_hSoundFileHash, hHashIndex);

	pFile->Delete();
		//memorypool
		memset( pFile, 0, sizeof(CSoundEffectFile));
		LALFree( m_hSoundFileMemoryPool, pFile);
	pFile	=	0;

	m_dwSoundFileCount--;



		
	_asm nop;
}

// ���� Ǯ�� ��� �ϰ�, ���� �ڷᱸ�� �����.
void CMilesSoundLib::AllocSamplePool()
{
	// �ִ� ���� ����. ���� ���� ã�ҳ�. -_-;
	U32		max_3D_samples;
	AIL_3D_provider_attribute( m_3DDriver, "Maximum supported samples", &max_3D_samples );	

	if( max_3D_samples == 0)
		_asm int 3;

	// �ִ� �������� �ϳ� �����ϳ�.? Ȥ�� ������ ������ �ϳ� ����� �ϴ°� �ƴѰ� �𸣰ڳ�. -_-;
	max_3D_samples--;
	max_3D_samples--;
	max_3D_samples--;
	
	// �ִ� ����.
	m_dwMaxSamplePoolCount			=	max_3D_samples;

	// �׽�Ʈ.
//	m_dwMaxSamplePoolCount	=	10;

	// ���� Ǯ ����.
	m_pSamplePool					=	new H3DSAMPLE[m_dwMaxSamplePoolCount];
	m_dwSamplePoolCount				=	0;
	H3DSAMPLE		hSample;
	DWORD		i;
	for( i = 0; i < m_dwMaxSamplePoolCount;i++)
	{
		hSample	=	AIL_allocate_3D_sample_handle(m_3DDriver);
		if( hSample)
		{
			m_pSamplePool[i]	=	hSample;
		}
		else
		{
			_asm int 3;  // �̱� ����? �־��� ���� �������� �� ������ �ȵǳ�?
			m_dwMaxSamplePoolCount	=	i;
			break;
		}
	}

#ifdef			_DEBUG
	if( i == 0)
	{
		// �Ѱ��� ������ ������ ���.
		_asm int 3;
	}
#endif
	

	// �ε��� ũ��������.
	m_hSamplePoolIndexCreator		=	ICCreate();
	BOOL	bIndexCreator	=	ICInitialize( m_hSamplePoolIndexCreator, m_dwMaxSamplePoolCount);
#ifdef			_DEBUG
	if( bIndexCreator == FALSE)
	{
		_asm int 3;			// critical
	}
#endif

	// �ִ� ���� ����.
	m_dwMaxVoiceCount			=	m_dwMaxSamplePoolCount;	// Ǯ ��� ������ �����ϴ°� �ƴ϶� ����Ʈ�� �ڵ� �Ҵ��ϴ� ������ �����Ѵ�.
	// ���� ��?

//	m_dwMaxVoiceCount	=	1;
}

void CMilesSoundLib::ReleaseSamplePool()
{
	// �ϴ� ISoundEffect�� ������ �� ��ȯ�ߴ°� üũ �ϰ�,
	// �������� �ʰ� ���� ������ �ִ� ��� �ɸ�
	assert( ! m_dwSamplePoolCount );

	// �ε��� ũ�������� ����.
	if( m_hSamplePoolIndexCreator)
	{
		DWORD dwRemainedSample	=	ICGetIndexNum( m_hSamplePoolIndexCreator);
		if( dwRemainedSample != 0)
			_asm int 3;			// ��� �ε����� �����Ȱ� �ƴϴ�.

		ICRelease( m_hSamplePoolIndexCreator);
		m_hSamplePoolIndexCreator	=	0;
	}
	
	// ���� Ǯ ����.
	if( m_pSamplePool )
	{
		for( DWORD i = 0; i < m_dwMaxSamplePoolCount; i++)
		{
			if( m_pSamplePool[i] == 0)
				_asm int 3;					// ����׿� Ȥ�� ���̸�???

			AIL_release_3D_sample_handle( m_pSamplePool[i]);
			m_pSamplePool[i]		=	0;
		}
		delete[]	m_pSamplePool;
		m_pSamplePool = 0;
	}
	
	m_dwMaxSamplePoolCount	=	0;
	// ����.
}

/*
 *	�ش� ���� ����Ʈ���� ������ �߱��Ѵ�.
 */
BOOL CMilesSoundLib::AllocSampleToSoundEffect(H3DSAMPLE *phOutSample, DWORD *pdwOutIndex)
{
	// ���� �߱����� �ε����� �� �ٴڳ���.?
	if( m_dwSamplePoolCount == m_dwMaxSamplePoolCount)
	{
		// �ܼ��� �Ѱ� ������ �ʰ����� ���̴�.
		return	FALSE;
	}

	if( m_dwSamplePoolCount >= m_dwMaxVoiceCount)
	{
		// �ִ� ���ù߱� ������ á��. ���� ȿ���� ������?
		return	FALSE;
	}
	
	// �ε��� ũ�������Ϳ��� �� �ε����� �ϳ� �߱� �޴´�.
	DWORD		dwNewIndex	=	ICAllocIndex( m_hSamplePoolIndexCreator) - 1;	// 1���̽�.
#ifdef			_DEBUG
	if( dwNewIndex == 0xffffffff)
		_asm int 3;
#endif

	*pdwOutIndex	=	dwNewIndex;
	*phOutSample	=	m_pSamplePool[dwNewIndex];
	
	m_dwSamplePoolCount++;

	return	TRUE;
}

/*
 *	���� ����Ʈ�� �� �� ������ �ݳ��Ѵ�.
 */
BOOL CMilesSoundLib::ReleaseSampleFromSoundEffect(DWORD dwSampleIndex)
{
	// �ش� �ε����� ���̰� �ִ°�?
	// ����.
	ICFreeIndex( m_hSamplePoolIndexCreator, dwSampleIndex+1);
	
	m_dwSamplePoolCount--;

	return	TRUE;
}

void CMilesSoundLib::SetMaxVoiceCount( DWORD dwMaxVoice)
{
	if( m_dwMaxSamplePoolCount < dwMaxVoice)
	{
		m_dwMaxVoiceCount	=	m_dwMaxSamplePoolCount;
	}
	else
		m_dwMaxVoiceCount	=	dwMaxVoice;
}

DWORD CMilesSoundLib::GetMaxVoiceCount()
{
	return	m_dwMaxVoiceCount;
}
