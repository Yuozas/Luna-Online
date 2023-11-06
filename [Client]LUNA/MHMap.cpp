// MHMap.cpp: implementation of the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHMap.h"

#include "GameResourceManager.h"
#include "TileManager.h"
#include "MHFile.h"
//#include "Mp3/BGMLoader.h"
#include "ObjectManager.h"
#include "MainGame.h"
#include "Engine/Engine.h"
#include "GameIn.h"
#include "./Audio/MHAudioManager.h"
#include "MHCamera.h"
//#include "./mp3/BGMLoader.h"



#include "MiniMapDlg.h"
#include "QuestManager.h"
#include "ItemManager.h"

// desc_hseos_����ý���_01
// S ����ý��� �߰� added by hseos 2007.05.02
#include "../hseos/Farm/SHFarmManager.h"
// E ����ý��� �߰� added by hseos 2007.05.02


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMHMap)
CMHMap::CMHMap()
{
	sky = NULL;
	m_pTileManager = NULL;
	mapDesc = NULL;
	m_bIsInited = FALSE;
	m_CloudTable.Initialize(64);
	
	m_bVillage = FALSE;

	m_dwQuestNpcId = 1000;
}

CMHMap::~CMHMap()
{
//	Release();
	SAFE_DELETE(mapDesc);
}

void CMHMap::InitMap(MAPTYPE MapNum)
{
	m_dwQuestNpcId = 1000;
	m_bIsInited = TRUE;
	m_MapNum = MapNum;
	//int farmZone = 0;

	// 090921 LUJ, ���� ���� ��쿡 �ε��ϵ��� �Ѵ�	
	//if( g_csFarmManager.GetFarmZoneID(
	//	MapNum,
	//	&farmZone ) )
	//{
	//	MAINGAME->GetEngine()->GetGraphicsEngine()->LoadPack(
	//		"data/3dData/farm.pak");
	//}
	//LOGFILE("LoadMapDesc(MapNum)");
	LoadMapDesc(MapNum);

	switch(MapNum)
	{
		case Alker_Farm:
		//case Nera_Farm:
		//case PVPMAP:
		case Moon_Blind_Swamp:
		case Red_Orc_Outpost: 
		case Moon_Blind_Forest: 
		case Haunted_Mine_1F:
		case Haunted_Mine_2F:
		case The_Gate_Of_Alker:
		case Alker_Harbor: 
		case Ruins_Of_Draconian: 
		case Zakandia: 
		case Tarintus: 
		case Mont_Blanc_Port: 
		case Dryed_Gazell_Fall: 
		case Zakandia_Outpost: 
		case The_Dark_Portal: 
		case Distorted_crevice:
		case The_Way_To_The_Howling_Ravine:
		case Howling_Ravine:
		case Howling_Cave_1F:
		case Howling_Cave_2F:
		case Ghost_Tree_Swamp: 
		case Lair_of_Kierra:
		//case UnderGroundPrison:
		case The_Valley_Of_Fairy:
		case The_Town_of_Nera_Castle:
		case The_Great_Garden:
		//case TheKnightsGravecase:
		case A_Harbor_of_Nera:
		//case Temple_of_Greed:
		//case Broken_Shipwrecked:
		//case The_tower_of_wizardly_Low:
		//case The_tower_of_wizardly_Middle:
		//case The_tower_of_wizardly_High:
		//case The_Death_tower:
		//case The_Spirit_Forest:
		//case The_Cristal_Empire:
		//case TreeOfKenai:
		//case TheCryingTavern:
		//case TheUndergroundPassage:
		//case SahelZone:
		//case MirageInTheParasus:
		//case RuinsOfAncientElpis:
		//case DragoniansTomb1:
		//case DragoniansTomb2:
		//case GraniteZone:
		//case GuildCamp:
		//case TwistedEarth:
		//case The_Castle_of_Rushen:
		//case The_Culvert:
		//case The_Castle_of_Zevyn:
		//case Advanced_Dungeon:
		case DateDungeon_Cemetery:
		case DateDungeon_BrokenHouse:
		case DateDungeon_DarkCave:
		//case GuildTournamentStadium:
		//case DesolatedGorge:
		//case DateDungeon_CookieGarden:
		//case AlkerResidence:
			{
				FILE* file = fopen( "system\\launcher.sav", "r" );

				if( file )
				{
					char buffer[ 100 ];																
					const char* separator = " =\n";													
					while( fgets( buffer, sizeof( buffer ), file ) )
						{
							const char* token = strtok( buffer, separator );							
							
							if( 0 == token || ';' == token[ 0 ] )										
							{
								continue;																
							}
							else if( ! strcmpi( "FOV", token ) )									
							{
								token = strtok( NULL, separator );										
								if( ! strcmpi("50", token))
								{
									mapDesc->Fov = 50;
								}
								else if( ! strcmpi("60", token))
								{
									mapDesc->Fov = 60;
								}
								else if( ! strcmpi("70", token))
								{
									mapDesc->Fov = 70;
								}
								else if( ! strcmpi("80", token))
								{
									mapDesc->Fov = 80;
								}
								else if( ! strcmpi("90", token))
								{
									mapDesc->Fov = 90;
								}
								else if( ! strcmpi("100", token))
								{
									mapDesc->Fov = 100;
								}
								else
								{
									mapDesc->Fov = 50;
								}
							}
							else if( ! strcmpi( "", token ) )
							{
								mapDesc->Fov = 50;
							}
						}
				}
			fclose(file);
			}
			break;
	}

	DIRECTORYMGR->SetLoadMode(eLM_Map);
	{
		// �ӽ�
		//LOGFILE("map.Init(mapDesc->MapFileName)");
		BOOL rt;
		rt = map.Init(mapDesc->MapFileName);
		
		//LOGFILE("SAFE_DELETE(sky)");
		SAFE_DELETE(sky);
		if(mapDesc->bSky)
		{
			//LOGFILE("sky");
			sky = new CEngineSky;
			sky->CreateSky(mapDesc->SkyMod,mapDesc->SkyAnm, &mapDesc->SkyOffset);
		}
		
		
		//LOGFILE("m_pTileManager");
		DIRECTORYMGR->SetLoadMode(eLM_Resource);
		m_pTileManager = new CTileManager;
		if(m_pTileManager->LoadTileInfo(mapDesc->TileFileName) == FALSE)
			SAFE_DELETE(m_pTileManager);		
		DIRECTORYMGR->SetLoadMode(eLM_Map);
	}
	DIRECTORYMGR->SetLoadMode(eLM_Root);

	//LOGFILE("ApplyMapDesc()");
	ApplyMapDesc();

	// ���� �̸� �ε�
//	PreLoadData();
	LoadPreData(MapNum);
}

/*
void CMHMap::PreLoadData()
{
	BASE_MONSTER_LIST* pMonsterInfo = GAMERESRCMNGR->GetMonsterListInfo(13);

}
*/

void CMHMap::LoadPreData(MAPTYPE MapNum)
{	
	sPRELOAD_INFO* pPreLoadInfo = GAMERESRCMNGR->GetPreDataInfo(MapNum);
	if(!pPreLoadInfo)		return;
	
	//
	BASE_MONSTER_LIST* pBMonInfo = NULL;

	//----------------------------------------------------------------
	// Monster
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Monster);

	for(int i=0; i<pPreLoadInfo->Count[ePreLoad_Monster]; i++)
	{
		pBMonInfo = GAMERESRCMNGR->GetMonsterListInfo(pPreLoadInfo->Kind[ePreLoad_Monster][i]);
		if(pBMonInfo)
			CEngineObject::PreLoadObject(pBMonInfo->ChxName);
	}

	//----------------------------------------------------------------
	// Item
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Character);

	MOD_LIST* pModList_Elf_Man = &GAMERESRCMNGR->m_ModFileList[RaceType_Elf][GENDER_MALE];
	MOD_LIST* pModList_Elf_Woman = &GAMERESRCMNGR->m_ModFileList[RaceType_Elf][GENDER_FEMALE];
	StaticString* pString = NULL;
	int PartModelNum = 0;
	for(i=0; i<pPreLoadInfo->Count[ePreLoad_Item]; i++)
	{		
		PartModelNum = ITEMMGR->GetItemInfo(pPreLoadInfo->Kind[ePreLoad_Item][i])->Part3DModelNum;

		// Man
		pString = &pModList_Elf_Man->ModFile[PartModelNum];
		if(pString == 0)
		{
			char buf[128];
			sprintf(buf, "No Name ModList! Plz Check again.ItemNum : %d", pPreLoadInfo->Kind[ePreLoad_Item][i]);
			ASSERTMSG(0, buf);
		}			
		if(pString->Str)
			CEngineObject::PreLoadObject(pString->Str);

		// Woman
		pString = &pModList_Elf_Woman->ModFile[PartModelNum];
		if(pString->Str)
			CEngineObject::PreLoadObject(pString->Str);
	}
	
	//----------------------------------------------------------------
	// Mod
	//----------------------------------------------------------------
	PRELOAD* pMod = NULL;
	cPtrList* pList = GAMERESRCMNGR->GetPreLoadModlist();

	PTRLISTPOS pos = pList->GetHeadPosition();
	while(pos)
	{
		pMod = (PRELOAD*)pList->GetNext(pos);
		if(pMod)
		{
			CEngineObject::PreLoadObject(pMod->FileName.Str);
		}
	}

	//----------------------------------------------------------------
	// Effect
	//----------------------------------------------------------------
	DIRECTORYMGR->SetLoadMode(eLM_Effect);

	PRELOAD* pEff = NULL;
	pList = GAMERESRCMNGR->GetPreLoadEfflist();

	pos = pList->GetHeadPosition();
	while(pos)
	{
		pEff = (PRELOAD*)pList->GetNext(pos);
		if(pEff)
		{
			g_pExecutive->PreLoadGXObject(pEff->FileName.Str);			
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 06. 04. PreLoadData �߰���� - �̿���
	// ������ PreLoad
	WORD* ItemList = GAMERESRCMNGR->GetPreLoadItemlist();

	if(ItemList)
	{
		WORD count = ItemList[0];

		for(WORD cnt = 1; cnt < count + 1; cnt++)
		{
			PartModelNum = ITEMMGR->GetItemInfo(ItemList[cnt])->Part3DModelNum;

			// Man
			pString = &pModList_Elf_Man->ModFile[PartModelNum];
			if(pString == 0)
			{
				char buf[128];
				sprintf(buf, "No Name ModList! Plz Check again.ItemNum : %d", pPreLoadInfo->Kind[ePreLoad_Item][i]);
				ASSERTMSG(0, buf);
			}			
			if(pString->Str)
				CEngineObject::PreLoadObject(pString->Str);

			// Woman
			pString = &pModList_Elf_Woman->ModFile[PartModelNum];
			if(pString->Str)
				CEngineObject::PreLoadObject(pString->Str);
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	// 06. 05. PreLoadData �߰���� - �̿���
	// ����Ʈ PreLoad
	sPRELOAD_EFFECT_INFO* pPreLoadEffectInfo = GAMERESRCMNGR->GetPreEffectInfo(MapNum);
	if(!pPreLoadEffectInfo)		return;

	PRELOAD* pEffect = NULL;
	pList = &(pPreLoadEffectInfo->Effect);

	pos = pList->GetHeadPosition();
	while(pos)
	{
		pEffect = (PRELOAD*)pList->GetNext(pos);
		if(pEffect)
		{
			CEngineObject::PreLoadObject(pEffect->FileName.Str);
		}
	}
	///////////////////////////////////////////////////////////////////////////////

	DIRECTORYMGR->SetLoadMode(eLM_Root);
}

void CMHMap::ApplyMapDesc()
{	
	if(mapDesc == NULL)
	{
		//LOGFILE("!!!!mapDesc == NULL!!!!");
	}
	// CameraFilter
	if(mapDesc->CameraFilter[0])
	{
		m_CameraFilterObject.Init(mapDesc->CameraFilter,NULL,eEngineObjectType_Effect);
		m_CameraFilterObject.AttachToCamera(mapDesc->CameraFilterDist);
		CAMERA->SetCameraFilter(&m_CameraFilterObject);
	}

	MAINGAME->GetEngine()->GetGraphicsEngine()->SetBackGroundColor(mapDesc->backColor);
	MAINGAME->GetEngine()->GetGraphicsEngine()->SetFixHeight(mapDesc->bFixHeight,mapDesc->FixHeight);
	
	//YH2DO Remove Global Variable
	bRenderSky = mapDesc->bSky;

	g_pExecutive->GetGeometry()->SetAmbientColor(0,mapDesc->Ambient);

	if(mapDesc->fogdesc.bEnable)
	{
		// �ӽ�
		g_pExecutive->GetGeometry()->EnableFog(0);
		g_pExecutive->GetGeometry()->SetFog(&mapDesc->fogdesc,0);
	}
	else
	{
		g_pExecutive->GetGeometry()->DisableFog(0);
	}

	VECTOR3 pos;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	
	g_pExecutive->GetGeometry()->ResetCamera(&pos,CAMERA_NEAR,mapDesc->DefaultSight,DEGTORAD(mapDesc->Fov),0);
	CAMERA->SetMaxSightDistance( mapDesc->DefaultSight );
	CAMERA->SetCameraFov( DEGTORAD(mapDesc->Fov) );

	//////////////////////////////////////////////////////////////////////////
	// BGM 
//	if(mapDesc->BGMSoundNum)
//	{
		//LOGFILE("AUDIOMGR->PlayBGM");
		AUDIOMGR->PlayBGM( mapDesc->BGMSoundNum );

		//LOGFILE("AUDIOMGR->PlayBGM End");
		
		//MP3->PlayFile(AUDIOMGR->GetFileName(mapDesc->BGMSoundNum),TRUE);
		//mapDesc->BGMSound[0] = 0;
//	}
	
	//////////////////////////////////////////////////////////////////////////
	// ���⼺ ����Ʈ
	g_pExecutive->GetGeometry()->EnableDirectionalLight(0);
	
	DIRECTIONAL_LIGHT_DESC LightDesc;
	LightDesc.dwAmbient = 0;
	LightDesc.dwDiffuse = 0xffffffff;
	LightDesc.dwSpecular = 0xffffffff;

	LightDesc.v3Dir = mapDesc->SunPos * -1.f;
	LightDesc.bEnable = TRUE;
	g_pExecutive->GetGeometry()->SetDirectionalLight(&LightDesc,0);
	//////////////////////////////////////////////////////////////////////////
	

	//////////////////////////////////////////////////////////////////////////
	// �׸���
	if(GAMERESRCMNGR->m_GameDesc.bShadow == MHSHADOW_DETAIL)
	{
		VECTOR3 to;
		to.x = 0;
		to.y = 0;
		to.z = 0;
		//float len = VECTOR3Length(&mapDesc->SunPos);
		float len = VECTOR3Length(&m_SunPosForShadow);
		m_Shadowlight.InitShadowLight(0,&pos,&to,gPHI/16, len + 1000);
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// �� �� ��
	if(mapDesc->bSun)
	{
		DIRECTORYMGR->SetLoadMode(eLM_Map);
		m_SunObject.Init(mapDesc->SunObject,NULL,eEngineObjectType_Effect);
		DIRECTORYMGR->SetLoadMode(eLM_Root);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ����
	if(mapDesc->CloudNum)
	{
		CEngineCloud::LoadCloudList(mapDesc->CloudListFile);
		CEngineCloud::SetCloudHeight(mapDesc->CloudHMin,mapDesc->CloudHMax);
		VECTOR3 vel;
		vel.x = 20;
		vel.y = 0;
		vel.z = 10;
		CEngineCloud::SetCloudVelocity(&vel);
		ASSERT(CEngineCloud::GetMaxCloudKindNum() != 0);
		DWORD numperside = (DWORD)sqrt((double)mapDesc->CloudNum);
		DWORD n=0;
		float sizeperside = 51200.f / numperside;
		for(DWORD x=0;x<numperside;++x)
		{
			for(DWORD z=0;z<numperside;++z)
			{
				CEngineCloud* pCloud = new CEngineCloud;
				float fx = x*sizeperside + rand()%(DWORD(sizeperside));
				float fz = z*sizeperside + rand()%(DWORD(sizeperside));
				pCloud->Init(rand()%pCloud->GetMaxCloudKindNum(),fx,fz);
				m_CloudTable.Add(pCloud,n++);
			}
		}
	}

	//LOGFILE("Apply MapDesc Ends");
}

void CMHMap::Restore()
{

}


void CMHMap::Release()
{
	m_CameraFilterObject.DetachFromCamera();
	CAMERA->SetCameraFilter(NULL);
	m_CameraFilterObject.Release();

	AUDIOMGR->StopBGM();

	m_CloudTable.SetPositionHead();
	while(CEngineCloud* pCloud = m_CloudTable.GetData())
	{
		delete pCloud;
	}
	m_CloudTable.RemoveAll();
	CEngineCloud::ReleaseCloudList();

	m_bIsInited = FALSE;
	ReleasePool();
	ProcessGarbageObject();
	m_Shadowlight.Release();
	SAFE_DELETE(mapDesc);
	SAFE_DELETE(sky);
	m_SunObject.Release();

	map.Release();
	
	SAFE_DELETE(m_pTileManager);

}

void CMHMap::Invalidate()
{
//	ReleasePool();
//	ProcessGarbageObject();
}


void CMHMap::Process(DWORD CurTime)
{	
	if( !m_bIsInited ) return;
	
	static DWORD SumTick = 0;
	static DWORD LastTime = 0;
	static DWORD Tick = 0;

	if(LastTime)
	{
		Tick = CurTime - LastTime;
		LastTime = CurTime;
	}
	else
	{
		LastTime = CurTime;
		return;
	}


	if(mapDesc->bSky && sky)
	{
		SumTick += Tick;
		if(SumTick > 50)
		{
			sky->IncreaseSkyAniFrame(1);
			SumTick -= 50;
		}
	
		sky->RenderSky();
	}
/*
	if(GAMERESRCMNGR->m_GameDesc.bShadow == MHSHADOW_DETAIL)	//
	{
		VECTOR3 pos;
		pos = m_ShadowPivotPos + mapDesc->SunPos;
		m_Shadowlight.SetPosition(&pos);
		LIGHT_DESC desc;
		m_Shadowlight.GetLightDesc(&desc);
		desc.v3Point = pos;
		desc.v3To = m_ShadowPivotPos;
		m_Shadowlight.SetLightDesc(&desc);
	}
*/	

	if(mapDesc->bSun)
	{
		VECTOR3 pos;
		Normalize(&pos,&mapDesc->SunPos);
		pos = pos * mapDesc->SunDistance;
		pos = m_ShadowPivotPos;// + pos;
		pos.x += 10000;
		pos.y -= 2000;
		m_SunObject.SetEngObjPosition(&pos);
	}
	
	
}
DWORD CMHMap::GetTileWidth()
{
	if(m_pTileManager)
		return m_pTileManager->GetTileWidth();
	return 100;
}
BOOL CMHMap::CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3* Target, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	return m_pTileManager->CollisionLine(pStart, pEnd, Target, MapNum,pObject);
}

BOOL CMHMap::NonCollisionLine(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* Target, MAPTYPE MapNum, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	return m_pTileManager->NonCollisionLine(pStart, pEnd, Target, MapNum, pObject);
}

BOOL CMHMap::IsInTile(int cellX, int cellY, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->IsInTile(cellX, cellY, MapNum, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisionTileCell(int x, int y, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTile(x, y, MapNum,pObject))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CMHMap::CollisionTilePos(int x, int y, MAPTYPE MapNum,CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTile(int(x/ TILECOLLISON_DETAIL), int(y/ TILECOLLISON_DETAIL), MapNum, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj)
{
	if(m_pTileManager == NULL)
		return FALSE;
	if(m_pTileManager->CollisonCheck(pStart,pEnd,pRtCollisonPos,pObj) == TRUE)
		return TRUE;


	return FALSE;
}

void CMHMap::SetShadowPivotPos(VECTOR3* pPivotPos)
{
	m_ShadowPivotPos = *pPivotPos;
		
	VECTOR3 pos;
//	pos = m_ShadowPivotPos + mapDesc->SunPos;
	pos = m_ShadowPivotPos + m_SunPosForShadow;
	
	m_Shadowlight.SetPosition(&pos);
	LIGHT_DESC desc;
	m_Shadowlight.GetLightDesc(&desc);
	desc.v3Point = pos;
	desc.v3To = m_ShadowPivotPos;
	m_Shadowlight.SetLightDesc(&desc);
}

BOOL CMHMap::LoadMapDesc(MAPTYPE MapNum)
{	
	char mapdescfile[256];
	CMHFile file;
	sprintf(mapdescfile,"data\\script\\mapset\\MAP%d.bmhm",MapNum);
	if(file.Init(mapdescfile,"rb") == FALSE)
		return FALSE;
	
	char value[64] = {0,};
	SAFE_DELETE(mapDesc);
	mapDesc = new MAPDESC;

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy(value,strupr(file.GetString()));

		if(strcmp(value,"*SIGHT") == 0)
		{
			mapDesc->DefaultSight = file.GetFloat();
		}
		else if(strcmp(value,"*FOV") == 0)
		{
			mapDesc->Fov = file.GetFloat();
		}
		else if(strcmp(value,"*FOG") == 0)
		{
			mapDesc->fogdesc.bEnable = file.GetBool();
		}
		else if(strcmp(value,"*BRIGHT") == 0)
		{
			DWORD dd;
			dd = file.GetDword();
			mapDesc->Ambient = RGBA_MAKE(dd,dd,dd,dd);
		}
		else if(strcmp(value,"*FOGCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file.GetDword();
			g = file.GetDword();
			b = file.GetDword();
			a = file.GetDword();
			mapDesc->fogdesc.dwColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FOGDENSITY") == 0)
		{
			mapDesc->fogdesc.fDensity = file.GetFloat();
		}
		else if(strcmp(value,"*FOGSTART") == 0)
		{
			mapDesc->fogdesc.fStart = file.GetFloat();
		}
		else if(strcmp(value,"*FOGEND") == 0)
		{
			mapDesc->fogdesc.fEnd = file.GetFloat();
		}
		else if(strcmp(value,"*MAP") == 0)
		{
			file.GetString(mapDesc->MapFileName);
		}
		else if(strcmp(value,"*TILE") == 0)
		{
			char buf[64];
			file.GetString(buf);
			sprintf( mapDesc->TileFileName, "%s%s", "../map/", buf );
		}
		else if(strcmp(value,"*SKYMOD") == 0)
		{
			file.GetString(mapDesc->SkyMod);
		}
		else if(strcmp(value,"*SKYANM") == 0)
		{
			file.GetString(mapDesc->SkyAnm);
		}
		else if(strcmp(value,"*SKYBOX") == 0)
		{
			mapDesc->bSky = file.GetBool();
		}
		else if(strcmp(value,"*BGM") == 0)
		{
			mapDesc->BGMSoundNum = file.GetWord();
		}
		else if(strcmp(value,"*COLOR") == 0)
		{
			BYTE r = file.GetByte();
			BYTE g = file.GetByte();
			BYTE b = file.GetByte();
			mapDesc->Ambient = RGBA_MAKE(r,g,b,255);
		}
		else if(strcmp(value,"*SUNPOS") == 0)
		{
			mapDesc->SunPos.x = file.GetFloat();
			mapDesc->SunPos.y = file.GetFloat();
			mapDesc->SunPos.z = file.GetFloat();

			float dist = VECTOR3Length( &mapDesc->SunPos );
			m_SunPosForShadow = mapDesc->SunPos / dist * 2000;	//�Ÿ��� õ? //�ݰ溸�� ª�ƾ�?
			m_SunPosForShadow.y += 1000;
			
		}
		else if(strcmp(value,"*SUNOBJECT") == 0)
		{
			file.GetString(mapDesc->SunObject);
		}
		else if(strcmp(value,"*SUN") == 0)
		{
			mapDesc->bSun = file.GetBool();
		}
		else if(strcmp(value,"*SUNDISTANCE") == 0)
		{
			mapDesc->SunDistance = file.GetFloat();
		}
		else if(strcmp(value,"*BACKCOLOR") == 0)
		{
			DWORD r,g,b,a;
			r = file.GetDword();
			g = file.GetDword();
			b = file.GetDword();
			a = file.GetDword();
			mapDesc->backColor = RGBA_MAKE(r,g,b,a);
		}
		else if(strcmp(value,"*FIXHEIGHT") == 0)
		{
			mapDesc->bFixHeight = TRUE;
			mapDesc->FixHeight = file.GetFloat();			
		}
		else if(strcmp(value,"*CLOUD") == 0)
		{
			mapDesc->CloudNum = file.GetDword();			
		}
		else if(strcmp(value,"*CLOUDLIST") == 0)
		{
			file.GetString(mapDesc->CloudListFile);
		}
		else if(strcmp(value,"*CLOUDHEIGHT") == 0)
		{
			mapDesc->CloudHMin = file.GetInt();
			mapDesc->CloudHMax = file.GetInt();
		}
		else if(strcmp(value,"*CAMERAFILTER") == 0)
		{
			file.GetString(mapDesc->CameraFilter);
		}		
		else if(strcmp(value,"*CAMERAFILTERDIST") == 0)
		{
			mapDesc->CameraFilterDist = file.GetFloat();
		}	
		//trustpak 2005/04/15
		else if(strcmp(value, "*SKYOFFSET") == 0)
		{
			mapDesc->SkyOffset.x = file.GetFloat();
			mapDesc->SkyOffset.y = file.GetFloat();
			mapDesc->SkyOffset.z = file.GetFloat();
		}
		///
	}
	
	file.Release();

	return TRUE;
}

void CMHMap::LoadNpcForQuestDialog()
{
	CMHFile file ;

	file.Init("System/Resource/StaticNpc.bin", "rb") ;

	if( file.IsInited() == FALSE )
	{
		ASSERTMSG(0, "����ƽ ������ �ε��ϴµ� �����ߴ�~!!") ;
		return ;
	}

	int nCount = 0 ;

	stNPCIDNAME NpcIdName ;

	while(TRUE)
	{
		if( file.IsEOF() )
		{
			break ;
		}

		memset(&NpcIdName, 0, sizeof(&NpcIdName)) ;

		file.GetInt() ;

		file.GetInt() ;

		strcpy(NpcIdName.name, file.GetString()) ;

		NpcIdName.wId = file.GetWord() ;

		QUESTMGR->SetNpcIDName(nCount, &NpcIdName) ;

		for( int count = 0 ; count < 3 ; ++count )
		{
			file.GetInt() ;
		}

		++nCount ;
	}
}


void CMHMap::LoadStaticNpc(MAPTYPE MapNum)											// ����ƽ npc�� �ε��ϴ� �Լ�.
{
	MAPTYPE npcMapnum = 0 ;															// npc �� ��ȣ�� ���� ������ �����Ѵ�.

	char value[256] = {0,};															// �ӽ� ���۸� �����ϰ� �ʱ�ȭ �Ѵ�.

	CMHFile file;																	// ���������� �����Ѵ�.

	file.Init("System/Resource/StaticNpc.bin","rb");								// ����ƽ npc ������ �д´�.

	if(file.IsInited() == FALSE)													// �б⿡ �����ϸ�,
		return;																		// ����ó���� �Ѵ�.

	NPC_TOTALINFO tinfo;															// npc ��Ż ������ ���� ����ü�� �����Ѵ�.
	BASEOBJECT_INFO binfo;															// �⺻ ������Ʈ ������ ���� ����ü�� �����Ѵ�.
	BASEMOVE_INFO minfo;															// �⺻ �̵� ������ ���� ����ü�� �����Ѵ�.

	DWORD n=0;																		// ī��Ʈ�� ������ �����ϰ� 0���� �����Ѵ�.

	char idxbuf[128] = { 0, };														// �ε��� ���۸� �����ϰ� �ʱ�ȭ �Ѵ�.
	char seps[] = ",";																// �޸� ��Ʈ���� �����Ѵ�.
	char* token = NULL;																// ��ū�� �����Ѵ�.

	while(1)																		// while���� ������.
	{
		if(file.IsEOF())															// ������ ���� �����ߴٸ�,
			break;																	// while���� ����������.

		npcMapnum = file.GetWord();													// ���Ͽ��� �� ��ȣ�� �о���δ�.

		if(npcMapnum != MapNum)														// npc �ʰ� ���� �� ��ȣ�� ���� ������,
		{
			file.GetLine(value,256);												// ������ �о���δ�.
			continue;																// ��Ƽ���� �Ѵ�.
		}

		binfo.dwObjectID = STATICNPC_ID_START + n++;								// �⺻ ������Ʈ ���̵� �����Ѵ�.
		binfo.ObjectState = 0;														// ������Ʈ ���¸� 0���� �����Ѵ�.
		tinfo.MapNum = MapNum;														// ��Ż ������ �� ��ȣ�� �����Ѵ�.
		minfo.bMoving = FALSE;														// �̵� ������ �̵� ���θ� false�� �����Ѵ�.
		minfo.KyungGongIdx = 0;														// ��� �ε����� 0���� �����Ѵ�.
		minfo.MoveMode = 0;															// �̵� ��带 0���� �����Ѵ�.

		tinfo.NpcKind = file.GetWord();												// ��Ż ������ npc ������ �����Ѵ�.
		// 080218 LYW --- MHMap : ������Ʈ �̸��� ���� Ȯ��.
		SafeStrCpy( binfo.ObjectName, file.GetString(), 32+1 );						// �⺻ ������ npc �̸��� �����Ѵ�.
		//SafeStrCpy( binfo.ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );		// �⺻ ������ npc �̸��� �����Ѵ�.
		tinfo.NpcUniqueIdx = file.GetWord();										// ��Ż ������ npc ����ũ �ε����� �����Ѵ�.

		NPC_LIST* pInfo = GAMERESRCMNGR->GetNpcInfo(tinfo.NpcKind);					// npc ������ �����Ͽ� npc ����Ʈ ������ �޴´�.

		if(pInfo)																	// npc ����Ʈ ������ ��ȿ�ϸ�,
		{
			tinfo.NpcJob = pInfo->JobKind;											// ��Ż ������ npc ������ �����Ѵ�.
		}
		else																		// npc ����Ʈ ������ ��ȿ���� ������,
		{
			tinfo.NpcJob = 0;														// ������ 0���� �����Ѵ�.
		}
		
		//jop�� 0�̸� �̸��� ������.
		if( tinfo.NpcJob == 0 )														// ������ 0�̸�,
		{
			binfo.ObjectName[0] = 0;												// �̸��� �����.
		}

		minfo.CurPosition.x = file.GetFloat();										// �̵������� ���Ϸ� ���� ���� ��ġ�� ��ġ�� �����Ѵ�.
		minfo.CurPosition.y = 0;
		minfo.CurPosition.z = file.GetFloat();
		
		CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);

		//����.....
		float fDir = file.GetFloat();												// ������ �о���δ�.
		pNpc->SetAngle( DEGTORAD( fDir ) );											// ������ �����Ѵ�.
		
		GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);							// �̴ϸʿ� npc �������� �߰��Ѵ�.
		
		// Npc Add
		QUESTMGR->SetNpcData( pNpc );												// ����Ʈ �Ŵ����� npc �����͸� �����Ѵ�.
	}
	
	file.Release();																	// ������ �����Ѵ�.
}

void CMHMap::AddQuestNpc( QUESTNPCINFO* pInfo )
{
	if( pInfo->wMapNum != GetMapNum() )
		return;
	CObject* pObject = OBJECTMGR->GetObject( pInfo->dwObjIdx );
	if( pObject )
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	binfo.dwObjectID = STATICNPC_ID_START + m_dwQuestNpcId++;
	pInfo->dwObjIdx = binfo.dwObjectID;
	binfo.ObjectState = 0;
	tinfo.MapNum = pInfo->wMapNum;
	minfo.bMoving = FALSE;
	minfo.KyungGongIdx = 0;
	minfo.MoveMode = 0;

	tinfo.NpcKind = pInfo->wNpcKind;
//	strcpy( binfo.ObjectName, pInfo->sName );
	SafeStrCpy( binfo.ObjectName, pInfo->sName, MAX_NAME_LENGTH+1 );
	tinfo.NpcUniqueIdx = pInfo->wNpcIdx;
	NPC_LIST* pList = GAMERESRCMNGR->GetNpcInfo(tinfo.NpcKind);
	if(pInfo)
	{
		tinfo.NpcJob = pList->JobKind;
	}
	else
	{
		tinfo.NpcJob = 0;
	}

	minfo.CurPosition.x = pInfo->vPos.x;
	minfo.CurPosition.y = 0;
	minfo.CurPosition.z = pInfo->vPos.z;

	CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);

	//����.....
	float fDir = pInfo->fDir;

	pNpc->SetAngle( DEGTORAD( fDir ) );
	
	GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);

	// Npc Add
	QUESTMGR->SetNpcData( pNpc );


	TARGETSET set;
	set.pTarget = pNpc;
	//EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pNpc,&set,0,pNpc->GetID());
	EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pNpc,&set,0,pNpc->GetID());
}
