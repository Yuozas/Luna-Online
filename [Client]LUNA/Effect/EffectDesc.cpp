// EffectDesc.cpp: implementation of the CEffectDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectDesc.h"
#include "GameResourceManager.h"

#include "EffectUnitDesc.h"
#include "Effect.h"

#include "..\MHFile.h"
#include "LightEffectUnitDesc.h"
#include "ObjectEffectUnitDesc.h"
#include "AnimationEffectUnitDesc.h"
#include "DamageEffectUnitDesc.h"
#include "CameraEffectUnitDesc.h"
#include "SoundEffectUnitDesc.h"
#include "MoveEffectUnitDesc.h"


#include "EffectTrigger.h"
#include "EffectTriggerOnUnitDesc.h"
#include "EffectTriggerOffUnitDesc.h"
#include "EffectTriggerMoveUnitDesc.h"
#include "EffectTriggerAttachUnitDesc.h"
#include "EffectTriggerDetachUnitDesc.h"
#include "EffectTriggerCameraRotateUnitDesc.h"
#include "EffectTriggerCameraAngleUnitDesc.h"
#include "EffectTriggerCameraZoomUnitDesc.h"
#include "EffectTriggerChangeCameraUnitDesc.h"
#include "EffectTriggerSetBaseMotionUnitDesc.h"
#include "EffectTriggerCameraShakeUnitDesc.h"
#include "EffectTriggerLinkUnitDesc.h"
#include "EffectTriggerFadeOutUnitDesc.h"
#include "EffectTriggerAnimateUnitDesc.h"
#include "EffectTriggerIllusionUnitDesc.h"
#include "EffectTriggerGravityMoveUnitDesc.h"


#include "..\Engine\EngineObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectDesc::CEffectDesc()
{
	m_MaxEffectUnitDesc = 0;
	m_MaxEffectTriggerDesc = 0;
	m_ppEffectUnitDescArray = NULL;
	m_ppEffectTriggerUnitDesc = NULL;
	m_EffectEndTime = 10000;
	m_bRepeat = FALSE;
	m_NextEffect = 0;
	m_OperatorAnimationTime = 0;
}

CEffectDesc::~CEffectDesc()
{
	if(m_ppEffectUnitDescArray)
	{
		for(DWORD n=0;n<m_MaxEffectUnitDesc;++n)
		{
			CEffectUnitDesc* unitDesc = m_ppEffectUnitDescArray[n];

			SAFE_DELETE( unitDesc );
		}

		delete [] m_ppEffectUnitDescArray;
	}
	if(m_ppEffectTriggerUnitDesc)
	{
		for(DWORD n=0;n<m_MaxEffectTriggerDesc;++n)
		{
			CEffectTriggerUnitDesc* unitDesc = m_ppEffectTriggerUnitDesc[ n ];
			SAFE_DELETE( unitDesc );
		}

		delete [] m_ppEffectTriggerUnitDesc;
	}
}

BOOL CEffectDesc::LoadEffectDesc(int EffectKind,CMHFile* pFile)
{
	m_EffectKind = EffectKind;

	if( ! pFile->IsInited() )
	{
		m_MaxEffectUnitDesc = 0;
		m_MaxEffectTriggerDesc = 0;

		return FALSE;
	}

	m_FileName = pFile->GetFileName();

	char token[ 128 ];
	DWORD TriggerNum = 0;

	//////////////////////////////////////////////////////////////////////////
	// Damage Percent ������
	BOOL bDamageUnit = FALSE;
	float Percent = 0;

	while(! pFile->IsEOF() )
	{
		pFile->GetString( token );

		if( std::string( "#REPEAT" ) == token )
		{
			m_bRepeat = pFile->GetBool();
		}
		else if( std::string( "#NEXTEFFECT" ) == token )
		{
			m_NextEffect = FindEffectNum( pFile->GetString() );
		}
		else if( std::string( "#EFFECTENDTIME" ) == token )
		{
			pFile->GetString( token );
			DWORD time;

			if( token[0] == 'f' || token[0] == 'F' )
			{
				if( 2 > strlen( token ) )
				{
					MessageBox(NULL,"f������ ���ڸ� �ٿ����ּ���!!!!",NULL,NULL);//pjslocal
					return FALSE;
				}

				const float frame = (float)atof( &token[1] );
				const float tickPerFrame = GAMERESRCMNGR->m_GameDesc.TickPerFrame;
				time = ( DWORD )( tickPerFrame * frame );		// �������ϰ�� �ð����� ��ȯ
			}
			else
			{
				time = atoi( token );
			}

			m_EffectEndTime = time;
		}
		else if( std::string( "#MAXEFFECTUNIT" ) == token )
		{
			m_MaxEffectUnitDesc = pFile->GetDword();
			m_ppEffectUnitDescArray = new CEffectUnitDesc*[m_MaxEffectUnitDesc];
			ZeroMemory( m_ppEffectUnitDescArray, sizeof(CEffectUnitDesc*)*m_MaxEffectUnitDesc );
		}
		else if( std::string( "#NEWEFFECTUNIT" ) == token )
		{
			const DWORD unitnum = pFile->GetDword();

			if(m_MaxEffectUnitDesc <= unitnum)
			{
				MessageBox(NULL,"Check EffectUnit Num and MaxUnitNum!!!",0,0);
				return FALSE;
			}

			CEffectUnitDesc*& desc = m_ppEffectUnitDescArray[ unitnum ];

			if( desc )
			{
				std::string message = std::string( m_FileName ) + ": " + std::string( token ) + ": unitnum�� �ߺ��Ǿ����ϴ�. �ߺ��� ȿ���� ���õ˴ϴ�";
				MessageBox( 0, message.c_str(), 0, 0 );
				continue;
			}

			pFile->GetString( token );

			if( std::string( "LIGHT" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CLightEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "OBJECT" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CObjectEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "ANIMATION" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CAnimationEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "DAMAGE" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CDamageEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "CAMERA" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CCameraEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "SOUND" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CSoundEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else if( std::string( "MOVE" ) == token )
			{
				BOOL bDO = pFile->GetBool();
				desc = new CMoveEffectUnitDesc(bDO);
				desc->ParseScript(pFile);
			}
			else
			{
				// ����, ���� ��ũ��Ʈ�� ������ �ʴ� Ű���尡 ����� ���� ����... MOVE, SOUNDNUM ��...
				//assert( 0 && "not defined keyword" );
			}
		}
		else if( std::string( "#MAXTRIGGER" ) == token )
		{
			m_MaxEffectTriggerDesc		= pFile->GetDword();
			m_ppEffectTriggerUnitDesc	= new CEffectTriggerUnitDesc*[m_MaxEffectTriggerDesc];
			ZeroMemory( m_ppEffectTriggerUnitDesc, sizeof(CEffectTriggerUnitDesc*)*m_MaxEffectTriggerDesc );
		}
		else if( std::string( "#TRIGGER" ) == token )
		{
			pFile->GetString( token );

			DWORD time;

			if(token[0] == 'f' || token[0] == 'F')
			{
				const float frame			= (float)atof(&token[1]);
				const float tickPerFrame	= GAMERESRCMNGR->m_GameDesc.TickPerFrame;
				time = (DWORD)( tickPerFrame * frame );		// �������ϰ�� �ð����� ��ȯ
			}
			else
			{
				time = atoi( token );
			}

			if(m_MaxEffectTriggerDesc <= TriggerNum)
			{
				MessageBox(NULL,"Check TriggerNum and MaxTriggerNum!!!",0,0);
				return FALSE;
			}

			CEffectTriggerUnitDesc*& desc = m_ppEffectTriggerUnitDesc[ TriggerNum ];

			if( desc )
			{
				std::string message = std::string( m_FileName ) + ": " + std::string( token ) + ": TriggerNum�� �ߺ��Ǿ����ϴ�. �ߺ��� ȿ���� ���õ˴ϴ�";
				MessageBox( 0, message.c_str(), 0, 0 );
				return FALSE;
			}

			const DWORD unitnum = pFile->GetDword();
			pFile->GetString( token );

			if( std::string( "ON") == token )
			{
				desc = new CEffectTriggerOnUnitDesc(time,unitnum);

#ifdef _TESTCLIENT_
				if(	m_MaxEffectUnitDesc > unitnum &&
					m_ppEffectUnitDescArray[unitnum]->m_EffectUnitType == eEffectUnit_Damage )
				{
					CDamageEffectUnitDesc* unitDesc = ( CDamageEffectUnitDesc* )( m_ppEffectUnitDescArray[unitnum] );
					assert( unitDesc );

					Percent += unitDesc->GetDamageRate();
					bDamageUnit = TRUE;
				}
#endif
			}
			else if( std::string( "OFF") == token )
			{
				desc = new CEffectTriggerOffUnitDesc(time,unitnum);
			}
			else if( std::string( "MOVE") == token )
			{
				desc = new CEffectTriggerMoveUnitDesc(time,unitnum);
			}
			else if( std::string( "GRAVITYMOVE" ) == token )
			{
				desc = new CEffectTriggerGravityMoveUnitDesc(time,unitnum);
			}
			else if( std::string( "ATTACH" ) == token )
			{
				desc = new CEffectTriggerAttachUnitDesc(time,unitnum);
			}
			else if( std::string( "DETACH" ) == token )
			{
				desc = new CEffectTriggerDetachUnitDesc(time,unitnum);
			}
			else if( std::string( "CAMERAROTATE" ) == token )
			{
				desc = new CEffectTriggerCameraRotateUnitDesc(time,unitnum);
			}
			else if( std::string( "CAMERAZOOM" ) == token )
			{
				desc = new CEffectTriggerCameraZoomUnitDesc(time,unitnum);
			}
			else if( std::string( "CAMERAANGLE" ) == token )
			{
				desc = new CEffectTriggerCameraAngleUnitDesc(time,unitnum);
			}
			else if( std::string( "CAMERACHANGE" ) == token )
			{
				desc = new CEffectTriggerChangeCameraUnitDesc(time,unitnum);
			}
			else if( std::string( "SETBASEMOTION" ) == token )
			{
				desc = new CEffectTriggerSetBaseMotionUnitDesc(time,unitnum);
			}
			else if( std::string( "CAMERASHAKE" ) == token )
			{
				desc = new CEffectTriggerCameraShakeUnitDesc(time,unitnum);
			}
			else if( std::string( "LINK" ) == token )
			{
				desc = new CEffectTriggerLinkUnitDesc(time,unitnum);
			}
			else if( std::string( "FADEOUT" ) == token )
			{
				desc = new CEffectTriggerFadeOutUnitDesc(time,unitnum);
			}
			else if( std::string( "FADEIN" ) == token )
			{
				desc = new CEffectTriggerFadeOutUnitDesc(time,unitnum);
			}
			else if( std::string( "ANIMATE" ) == token )
			{
				desc = new CEffectTriggerAnimateUnitDesc(time,unitnum);
//				desc->ParseScript( pFile );
			}
			else if( std::string( "ILLUSION" ) == token )
			{
				desc = new CEffectTriggerIllusionUnitDesc(time,unitnum);

			}
			else
			{
				//assert( 0 && "not defined keyword" );
			}

			if( desc )
			{
				desc->ParseScript( pFile );
				++TriggerNum;
			}
		}
	}

	if(m_MaxEffectTriggerDesc != TriggerNum)
	{
		MessageBox(NULL,"MaxTriggerNum is too BIG!!!",0,0);
		return FALSE;
	}

	if(bDamageUnit && (0.99 > Percent || Percent > 1.01))
	{
#ifdef _TESTCLIENT_
		MessageBox(NULL,"������ Percent�� ���� 100�� �ƴմϴ�!!!",0,0);
		return FALSE;
#endif
	}

	return TRUE;
}

BOOL CEffectDesc::GetEffect(EFFECTPARAM* pParam,CEffect* pOutEffect)
{
	pOutEffect->Init(	m_MaxEffectUnitDesc,pParam,
						this,
						m_MaxEffectTriggerDesc,
						m_ppEffectTriggerUnitDesc );

	for(DWORD n = 0; n < m_MaxEffectUnitDesc; ++n )
	{
		CEffectUnitDesc* effectUnitDesc = m_ppEffectUnitDescArray[ n ];

		if( ! effectUnitDesc )
		{			
			// �� ������ ����Ʈ ��ũ��Ʈ�� #MAXEFFECTUNIT�� ���ǵ� ����Ʈ ���ֺ��� Ŭ ��� �߻��Ѵ�
			// �ɰ��� ������ �ƴ����� ���� �ʴ� �޸𸮸� �Ҵ��ϱ� ������ �����ϴ� ���� �ٶ����ϴ�.
			// � ��ũ��Ʈ���� �׷� ������ �ִ��� �˷��� ������ ���� ����.
			//
			// �� �Լ��� ��ȣ���ڸ� ���� ����Ʈ ��ȣ(�ڵ忡���� EffectDescNum)�� �����Ǿ� �ִ�.
			// �� ��ȣ�� LIST_*.befl �� �ϳ��� ����(����, ������ �ٸ� ����Ʈ�� ���ǵǾ� �ִ�)
			// ��ȣ�� � ����Ʈ ������ �����ϴ��� Ȯ������. �� ������ ���� #NEWEFFECTUNIT���� 
			// ������ ����Ʈ ������ #MAXEFFECTUNIT�� �������� Ȯ���ϰ� �ƴϸ� �°� ��������.
			assert( 0 && "effectUnitDesc is null. MaxEffectUnitDesc is wrong. See this effect script." );
			return FALSE;
		}

		CEffectUnit* pEffectUnit = effectUnitDesc->GetEffectUnit(pParam);

		//////////////////////////////////////////////////////////////////////////
		// virtual ���ε� ���� ���� ������ CEffectUnit�� �����ڿ��� ����� �ű� [4/29/2003]
		if( effectUnitDesc->IsDangledToOperator() )
		{
			TARGETSET Targetset;
			Targetset.pTarget = pParam->m_pOperator;
			pEffectUnit->InitEffect(1,&Targetset);
		}
		else if( pParam->m_nTargetNum )
		{
			pEffectUnit->InitEffect(pParam->m_nTargetNum,pParam->m_pTargetSet);
		}
		
		pOutEffect->SetEffectUnit(n, pEffectUnit);
	}

	return TRUE;
}

DWORD CEffectDesc::GetOperatorAnimatioEndTime(CEngineObject* pEngineObject)
{	
	if( m_OperatorAnimationTime )
	{
		return m_OperatorAnimationTime;
	}

	for(DWORD n = 0; n < m_MaxEffectUnitDesc; ++n )
	{
		CEffectUnitDesc* effectUnitDesc = m_ppEffectUnitDescArray[n];

		if(		effectUnitDesc->m_EffectUnitType != eEffectUnit_Animation ||
			!	effectUnitDesc->IsDangledToOperator() )
		{
			continue;
		}

		CAnimationEffectUnitDesc* pDesc = (CAnimationEffectUnitDesc*)effectUnitDesc;

		const DWORD AniTime = pEngineObject->GetAnimationTime(pDesc->GetMotionNum());

		for(DWORD trig=0;trig<m_MaxEffectTriggerDesc;++trig)
		{
			CEffectTriggerUnitDesc* desc = m_ppEffectTriggerUnitDesc[trig];

			if( desc &&
				desc->GetUnitNum() == n )
			{
				const DWORD AniStartTime = desc->GetStartTime();

				if(AniStartTime + AniTime >= m_OperatorAnimationTime)
				{
					m_OperatorAnimationTime = AniStartTime + AniTime;
					break;
				}
			}
		}
	}

	return m_OperatorAnimationTime;
}
